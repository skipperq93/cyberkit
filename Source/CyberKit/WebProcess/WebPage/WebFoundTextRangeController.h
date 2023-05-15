/*
 * Copyright (C) 2022-2023 Apple Inc. All rights reserved.
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

#pragma once

#include "WebFindOptions.h"
#include "WebFoundTextRange.h"
#include <CyberCore/FindOptions.h>
#include <CyberCore/IntRect.h>
#include <CyberCore/PageOverlay.h>
#include <CyberCore/PlatformLayerIdentifier.h>
#include <CyberCore/SimpleRange.h>
#include <CyberCore/TextIndicator.h>
#include <wtf/Forward.h>
#include <wtf/HashMap.h>
#include <wtf/Noncopyable.h>
#include <wtf/Vector.h>

namespace CyberCore {
class Document;
}

namespace CyberKit {

class WebPage;

class WebFoundTextRangeController : private CyberCore::PageOverlay::Client {
    WTF_MAKE_FAST_ALLOCATED;
    WTF_MAKE_NONCOPYABLE(WebFoundTextRangeController);

public:
    explicit WebFoundTextRangeController(WebPage&);

    void findTextRangesForStringMatches(const String&, OptionSet<FindOptions>, uint32_t maxMatchCount, CompletionHandler<void(Vector<CyberKit::WebFoundTextRange>&&)>&&);

    void replaceFoundTextRangeWithString(const WebFoundTextRange&, const String&);

    void decorateTextRangeWithStyle(const WebFoundTextRange&, FindDecorationStyle);
    void scrollTextRangeToVisible(const WebFoundTextRange&);

    void clearAllDecoratedFoundText();

    void didBeginTextSearchOperation();
    void didEndTextSearchOperation();

    void addLayerForFindOverlay(CompletionHandler<void(CyberCore::PlatformLayerIdentifier)>&&);
    void removeLayerForFindOverlay();

    void requestRectForFoundTextRange(const WebFoundTextRange&, CompletionHandler<void(CyberCore::FloatRect)>&&);

    void redraw();

private:
    // PageOverlay::Client.
    void willMoveToPage(CyberCore::PageOverlay&, CyberCore::Page*) override;
    void didMoveToPage(CyberCore::PageOverlay&, CyberCore::Page*) override;
    bool mouseEvent(CyberCore::PageOverlay&, const CyberCore::PlatformMouseEvent&) override;
    void drawRect(CyberCore::PageOverlay&, CyberCore::GraphicsContext&, const CyberCore::IntRect& dirtyRect) override;

    RefPtr<CyberCore::TextIndicator> createTextIndicatorForRange(const CyberCore::SimpleRange&, CyberCore::TextIndicatorPresentationTransition);
    void setTextIndicatorWithRange(const CyberCore::SimpleRange&);
    void flashTextIndicatorAndUpdateSelectionWithRange(const CyberCore::SimpleRange&);

    Vector<CyberCore::FloatRect> rectsForTextMatchesInRect(CyberCore::IntRect clipRect);

    CyberCore::Document* documentForFoundTextRange(const WebFoundTextRange&) const;
    std::optional<CyberCore::SimpleRange> simpleRangeFromFoundTextRange(WebFoundTextRange);

    WeakPtr<WebPage> m_webPage;
    RefPtr<CyberCore::PageOverlay> m_findPageOverlay;

    WebFoundTextRange m_highlightedRange;

    HashMap<WebFoundTextRange, std::optional<CyberCore::SimpleRange>> m_cachedFoundRanges;
    HashMap<WebFoundTextRange, FindDecorationStyle> m_decoratedRanges;

    RefPtr<CyberCore::TextIndicator> m_textIndicator;
};

} // namespace CyberKit
