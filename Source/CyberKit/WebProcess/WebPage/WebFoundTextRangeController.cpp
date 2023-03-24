/*
 * Copyright (C) 2022 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "WebFoundTextRangeController.h"

#include "WebPage.h"
#include <CyberCore/CharacterRange.h>
#include <CyberCore/Document.h>
#include <CyberCore/DocumentMarkerController.h>
#include <CyberCore/Editor.h>
#include <CyberCore/FocusController.h>
#include <CyberCore/FrameSelection.h>
#include <CyberCore/GeometryUtilities.h>
#include <CyberCore/GraphicsContext.h>
#include <CyberCore/ImageOverlay.h>
#include <CyberCore/LocalFrame.h>
#include <CyberCore/LocalFrameView.h>
#include <CyberCore/Page.h>
#include <CyberCore/PageOverlayController.h>
#include <CyberCore/PathUtilities.h>
#include <CyberCore/PlatformMouseEvent.h>
#include <CyberCore/SimpleRange.h>
#include <CyberCore/TextIterator.h>
#include <wtf/Scope.h>

namespace CyberKit {
using namespace CyberCore;

WebFoundTextRangeController::WebFoundTextRangeController(WebPage& webPage)
    : m_webPage(webPage)
{
}

void WebFoundTextRangeController::findTextRangesForStringMatches(const String& string, OptionSet<FindOptions> options, uint32_t maxMatchCount, CompletionHandler<void(Vector<WebFoundTextRange>&&)>&& completionHandler)
{
    auto result = m_webPage->corePage()->findTextMatches(string, core(options), maxMatchCount, false);
    Vector<CyberCore::SimpleRange> findMatches = WTFMove(result.ranges);

    if (!findMatches.isEmpty())
        m_cachedFoundRanges.clear();

    AtomString frameName;
    uint64_t order = 0;
    Vector<WebFoundTextRange> foundTextRanges;
    for (auto& simpleRange : findMatches) {
        auto& document = simpleRange.startContainer().document();

        auto* element = document.documentElement();
        if (!element)
            continue;

        auto currentFrameName = document.frame()->tree().uniqueName();
        if (frameName != currentFrameName) {
            frameName = currentFrameName;
            order++;
        }

        // FIXME: We should get the character ranges at the same time as the SimpleRanges to avoid additional traversals.
        auto range = characterRange(makeBoundaryPointBeforeNodeContents(*element), simpleRange, CyberCore::findIteratorOptions());
        auto foundTextRange = WebFoundTextRange { range.location, range.length, frameName.length() ? frameName : emptyAtom(), order };

        m_cachedFoundRanges.add(foundTextRange, simpleRange);
        foundTextRanges.append(foundTextRange);
    }

    completionHandler(WTFMove(foundTextRanges));
}

void WebFoundTextRangeController::replaceFoundTextRangeWithString(const WebFoundTextRange& range, const String& string)
{
    auto simpleRange = simpleRangeFromFoundTextRange(range);
    if (!simpleRange)
        return;

    RefPtr document = documentForFoundTextRange(range);
    if (!document)
        return;

    RefPtr frame = document->frame();
    if (!frame)
        return;

    CyberCore::VisibleSelection visibleSelection(*simpleRange);
    OptionSet temporarySelectionOptions { CyberCore::TemporarySelectionOption::DoNotSetFocus, CyberCore::TemporarySelectionOption::IgnoreSelectionChanges };
    CyberCore::TemporarySelectionChange selectionChange(*document, visibleSelection, temporarySelectionOptions);

    frame->editor().replaceSelectionWithText(string, CyberCore::Editor::SelectReplacement::Yes, CyberCore::Editor::SmartReplace::No, CyberCore::EditAction::InsertReplacement);
}

void WebFoundTextRangeController::decorateTextRangeWithStyle(const WebFoundTextRange& range, FindDecorationStyle style)
{
    auto simpleRange = simpleRangeFromFoundTextRange(range);
    if (!simpleRange)
        return;

    auto currentStyleForRange = m_decoratedRanges.get(range);
    if (style == currentStyleForRange)
        return;

    m_decoratedRanges.set(range, style);

    if (currentStyleForRange == FindDecorationStyle::Highlighted && range == m_highlightedRange) {
        m_textIndicator = nullptr;
        m_highlightedRange = { };
    }

    if (style == FindDecorationStyle::Normal)
        simpleRange->start.document().markers().removeMarkers(*simpleRange, CyberCore::DocumentMarker::TextMatch);
    else if (style == FindDecorationStyle::Found)
        simpleRange->start.document().markers().addMarker(*simpleRange, CyberCore::DocumentMarker::TextMatch);
    else if (style == FindDecorationStyle::Highlighted) {
        m_highlightedRange = range;

        if (m_findPageOverlay)
            setTextIndicatorWithRange(*simpleRange);
        else
            flashTextIndicatorAndUpdateSelectionWithRange(*simpleRange);
    }

    if (m_findPageOverlay)
        m_findPageOverlay->setNeedsDisplay();
}

void WebFoundTextRangeController::scrollTextRangeToVisible(const WebFoundTextRange& range)
{
    auto simpleRange = simpleRangeFromFoundTextRange(range);
    if (!simpleRange)
        return;

    auto* document = documentForFoundTextRange(range);
    if (!document)
        return;

    CyberCore::VisibleSelection visibleSelection(*simpleRange);
    OptionSet temporarySelectionOptions { CyberCore::TemporarySelectionOption::DelegateMainFrameScroll, CyberCore::TemporarySelectionOption::RevealSelectionBounds, CyberCore::TemporarySelectionOption::DoNotSetFocus, CyberCore::TemporarySelectionOption::UserTriggered };

    if (document->isTopDocument())
        temporarySelectionOptions.add(CyberCore::TemporarySelectionOption::SmoothScroll);

    CyberCore::TemporarySelectionChange selectionChange(*document, visibleSelection, temporarySelectionOptions);
}

void WebFoundTextRangeController::clearAllDecoratedFoundText()
{
    m_cachedFoundRanges.clear();
    m_decoratedRanges.clear();
    m_webPage->corePage()->unmarkAllTextMatches();

    m_highlightedRange = { };
    m_textIndicator = nullptr;

    if (m_findPageOverlay)
        m_findPageOverlay->setNeedsDisplay();
}

void WebFoundTextRangeController::didBeginTextSearchOperation()
{
    if (m_findPageOverlay)
        m_findPageOverlay->stopFadeOutAnimation();
    else {
        m_findPageOverlay = CyberCore::PageOverlay::create(*this, CyberCore::PageOverlay::OverlayType::Document);
        m_webPage->corePage()->pageOverlayController().installPageOverlay(*m_findPageOverlay, CyberCore::PageOverlay::FadeMode::Fade);
    }

    m_findPageOverlay->setNeedsDisplay();
}

void WebFoundTextRangeController::didEndTextSearchOperation()
{
    if (m_findPageOverlay)
        m_webPage->corePage()->pageOverlayController().uninstallPageOverlay(*m_findPageOverlay, CyberCore::PageOverlay::FadeMode::Fade);
}

void WebFoundTextRangeController::addLayerForFindOverlay(CompletionHandler<void(CyberCore::GraphicsLayer::PlatformLayerID)>&& completionHandler)
{
    if (!m_findPageOverlay) {
        m_findPageOverlay = CyberCore::PageOverlay::create(*this, CyberCore::PageOverlay::OverlayType::Document, CyberCore::PageOverlay::AlwaysTileOverlayLayer::Yes);
        m_webPage->corePage()->pageOverlayController().installPageOverlay(*m_findPageOverlay, CyberCore::PageOverlay::FadeMode::DoNotFade);
        m_findPageOverlay->layer().setOpacity(0);
    }

    completionHandler(m_findPageOverlay->layer().primaryLayerID());

    m_findPageOverlay->setNeedsDisplay();
}

void WebFoundTextRangeController::removeLayerForFindOverlay()
{
    if (m_findPageOverlay)
        m_webPage->corePage()->pageOverlayController().uninstallPageOverlay(*m_findPageOverlay, CyberCore::PageOverlay::FadeMode::DoNotFade);
}

void WebFoundTextRangeController::requestRectForFoundTextRange(const WebFoundTextRange& range, CompletionHandler<void(CyberCore::FloatRect)>&& completionHandler)
{
    auto simpleRange = simpleRangeFromFoundTextRange(range);
    if (!simpleRange) {
        completionHandler({ });
        return;
    }

    auto* frameView = simpleRange->startContainer().document().frame()->view();
    completionHandler(frameView->contentsToRootView(unionRect(CyberCore::RenderObject::absoluteTextRects(*simpleRange))));
}

void WebFoundTextRangeController::redraw()
{
    if (!m_findPageOverlay)
        return;

    auto setNeedsDisplay = makeScopeExit([findPageOverlay = RefPtr { m_findPageOverlay }] {
        findPageOverlay->setNeedsDisplay();
    });

    if (!m_highlightedRange.length)
        return;

    auto simpleRange = simpleRangeFromFoundTextRange(m_highlightedRange);
    if (!simpleRange)
        return;

    setTextIndicatorWithRange(*simpleRange);
}

void WebFoundTextRangeController::willMoveToPage(CyberCore::PageOverlay&, CyberCore::Page* page)
{
    if (page)
        return;

    ASSERT(m_findPageOverlay);
    m_findPageOverlay = nullptr;
}

void WebFoundTextRangeController::didMoveToPage(CyberCore::PageOverlay&, CyberCore::Page*)
{
}

bool WebFoundTextRangeController::mouseEvent(CyberCore::PageOverlay&, const CyberCore::PlatformMouseEvent&)
{
    return false;
}

void WebFoundTextRangeController::drawRect(CyberCore::PageOverlay&, CyberCore::GraphicsContext& graphicsContext, const CyberCore::IntRect& dirtyRect)
{
    constexpr int indicatorRadius = 3;
    constexpr int indicatorBorderWidth = 1;

    constexpr auto highlightColor = CyberCore::SRGBA<uint8_t> { 255, 228, 56 };
    constexpr auto foundColor = CyberCore::Color::white;
    constexpr auto overlayBackgroundColor = CyberCore::SRGBA<uint8_t> { 26, 26, 26, 64 };
    constexpr auto shadowColor = CyberCore::Color::black.colorWithAlphaByte(128);

    constexpr float shadowOffsetX = 0;
    constexpr float shadowOffsetY = 0;
    constexpr float shadowBlurRadius = 1;

    CyberCore::IntRect borderInflatedDirtyRect = dirtyRect;
    borderInflatedDirtyRect.inflate(indicatorBorderWidth);
    Vector<CyberCore::FloatRect> rects = rectsForTextMatchesInRect(borderInflatedDirtyRect);

    graphicsContext.fillRect(dirtyRect, overlayBackgroundColor);

    auto foundFramePaths = CyberCore::PathUtilities::pathsWithShrinkWrappedRects(rects, indicatorRadius);

    CyberCore::GraphicsContextStateSaver stateSaver(graphicsContext);

    graphicsContext.setShadow(CyberCore::FloatSize(shadowOffsetX, shadowOffsetY), shadowBlurRadius, shadowColor);
    graphicsContext.setStrokeColor(foundColor);
    graphicsContext.setStrokeThickness(indicatorBorderWidth * 2);
    for (auto& path : foundFramePaths)
        graphicsContext.strokePath(path);

    graphicsContext.clearShadow();

    graphicsContext.setCompositeOperation(CyberCore::CompositeOperator::Clear);
    for (auto& path : foundFramePaths)
        graphicsContext.fillPath(path);

    if (m_textIndicator && !m_textIndicator->selectionRectInRootViewCoordinates().isEmpty()) {
        auto* indicatorImage = m_textIndicator->contentImage();
        if (!indicatorImage)
            return;

        auto textBoundingRectInRootViewCoordinates = m_textIndicator->textBoundingRectInRootViewCoordinates();
        auto textRectsInBoundingRectCoordinates = m_textIndicator->textRectsInBoundingRectCoordinates();

        auto textRectsInRootViewCoordinates = textRectsInBoundingRectCoordinates.map([&](auto rect) {
            rect.moveBy(textBoundingRectInRootViewCoordinates.location());
            return rect;
        });

        auto paths = CyberCore::PathUtilities::pathsWithShrinkWrappedRects(textRectsInRootViewCoordinates, indicatorRadius);

        graphicsContext.setCompositeOperation(CyberCore::CompositeOperator::SourceOver);
        graphicsContext.setFillColor(highlightColor);
        for (const auto& path : paths)
            graphicsContext.fillPath(path);

        graphicsContext.drawImage(*indicatorImage, textBoundingRectInRootViewCoordinates);
    }
}

RefPtr<CyberCore::TextIndicator> WebFoundTextRangeController::createTextIndicatorForRange(const CyberCore::SimpleRange& range, CyberCore::TextIndicatorPresentationTransition transition)
{
    constexpr int indicatorMargin = 1;

    OptionSet options { CyberCore::TextIndicatorOption::IncludeMarginIfRangeMatchesSelection, CyberCore::TextIndicatorOption::DoNotClipToVisibleRect };
    if (CyberCore::ImageOverlay::isInsideOverlay(range))
        options.add({ CyberCore::TextIndicatorOption::PaintAllContent, CyberCore::TextIndicatorOption::PaintBackgrounds });

#if PLATFORM(IOS_FAMILY)
    Ref frame = CheckedRef(m_webPage->corePage()->focusController())->focusedOrMainFrame();
    frame->selection().setUpdateAppearanceEnabled(true);
    frame->selection().updateAppearance();
    frame->selection().setUpdateAppearanceEnabled(false);
#endif

    return CyberCore::TextIndicator::createWithRange(range, options, transition, CyberCore::FloatSize(indicatorMargin, indicatorMargin));
}

void WebFoundTextRangeController::setTextIndicatorWithRange(const CyberCore::SimpleRange& range)
{
    m_textIndicator = createTextIndicatorForRange(range, CyberCore::TextIndicatorPresentationTransition::None);
}

void WebFoundTextRangeController::flashTextIndicatorAndUpdateSelectionWithRange(const CyberCore::SimpleRange& range)
{
    auto& document = range.startContainer().document();
    document.selection().setSelection(CyberCore::VisibleSelection(range), CyberCore::FrameSelection::defaultSetSelectionOptions(CyberCore::UserTriggered));

    if (auto textIndicator = createTextIndicatorForRange(range, CyberCore::TextIndicatorPresentationTransition::Bounce))
        m_webPage->setTextIndicator(textIndicator->data());
}

Vector<CyberCore::FloatRect> WebFoundTextRangeController::rectsForTextMatchesInRect(CyberCore::IntRect clipRect)
{
    Vector<CyberCore::FloatRect> rects;
    auto* localMainFrame = dynamicDowncast<CyberCore::LocalFrame>(m_webPage->corePage()->mainFrame());
    if (!localMainFrame)
        return rects;

    RefPtr mainFrameView = localMainFrame->view();

    for (CyberCore::Frame* frame = localMainFrame; frame; frame = frame->tree().traverseNext()) {
        auto* localFrame = dynamicDowncast<CyberCore::LocalFrame>(frame);
        if (!localFrame)
            continue;
        RefPtr document = localFrame->document();
        if (!document)
            continue;

        for (auto rect : document->markers().renderedRectsForMarkers(CyberCore::DocumentMarker::TextMatch)) {
            if (!localFrame->isMainFrame())
                rect = mainFrameView->windowToContents(localFrame->view()->contentsToWindow(enclosingIntRect(rect)));

            if (rect.isEmpty() || !rect.intersects(clipRect))
                continue;

            rects.append(rect);
        }
    }

    return rects;
}

CyberCore::Document* WebFoundTextRangeController::documentForFoundTextRange(const WebFoundTextRange& range) const
{
    auto* localMainFrame = dynamicDowncast<LocalFrame>(m_webPage->corePage()->mainFrame());
    if (!localMainFrame)
        return nullptr;

    auto& mainFrame = *localMainFrame;
    if (range.frameIdentifier.isEmpty())
        return mainFrame.document();

    if (auto* frame = dynamicDowncast<CyberCore::LocalFrame>(mainFrame.tree().find(AtomString { range.frameIdentifier }, mainFrame)))
        return frame->document();

    return nullptr;
}

std::optional<CyberCore::SimpleRange> WebFoundTextRangeController::simpleRangeFromFoundTextRange(WebFoundTextRange range)
{
    return m_cachedFoundRanges.ensure(range, [&] () -> std::optional<CyberCore::SimpleRange> {
        auto* document = documentForFoundTextRange(range);
        if (!document)
            return std::nullopt;

        return resolveCharacterRange(makeRangeSelectingNodeContents(*document->documentElement()), { range.location, range.length }, CyberCore::findIteratorOptions());
    }).iterator->value;
}

} // namespace CyberKit
