/*
 * Copyright (C) 2011 Apple Inc. All rights reserved.
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
#include "WebFullScreenManager.h"

#if ENABLE(FULLSCREEN_API)

#include "Connection.h"
#include "Logging.h"
#include "CyberCoreArgumentCoders.h"
#include "WebFrame.h"
#include "WebFullScreenManagerProxyMessages.h"
#include "WebPage.h"
#include <CyberCore/AddEventListenerOptions.h>
#include <CyberCore/Color.h>
#include <CyberCore/EventNames.h>
#include <CyberCore/FullscreenManager.h>
#include <CyberCore/HTMLVideoElement.h>
#include <CyberCore/JSDOMPromiseDeferred.h>
#include <CyberCore/LocalFrame.h>
#include <CyberCore/LocalFrameView.h>
#include <CyberCore/Quirks.h>
#include <CyberCore/RenderLayerBacking.h>
#include <CyberCore/RenderView.h>
#include <CyberCore/Settings.h>
#include <CyberCore/TypedElementDescendantIteratorInlines.h>
#include <CyberCore/UserGestureIndicator.h>

#if PLATFORM(IOS_FAMILY) || (PLATFORM(MAC) && ENABLE(VIDEO_PRESENTATION_MODE))
#include "PlaybackSessionManager.h"
#include "VideoFullscreenManager.h"
#endif

namespace CyberKit {
using namespace CyberCore;

using CyberCore::FloatSize;

static CyberCore::IntRect screenRectOfContents(CyberCore::Element* element)
{
    ASSERT(element);
    if (!element)
        return { };

    if (element->renderer() && element->renderer()->hasLayer() && element->renderer()->enclosingLayer()->isComposited()) {
        CyberCore::FloatQuad contentsBox = static_cast<CyberCore::FloatRect>(element->renderer()->enclosingLayer()->backing()->compositedBounds());
        contentsBox = element->renderer()->localToAbsoluteQuad(contentsBox);
        return element->renderer()->view().frameView().contentsToScreen(contentsBox.enclosingBoundingBox());
    }

    return element->screenRect();
}

Ref<WebFullScreenManager> WebFullScreenManager::create(WebPage* page)
{
    return adoptRef(*new WebFullScreenManager(page));
}

WebFullScreenManager::WebFullScreenManager(WebPage* page)
    : CyberCore::EventListener(CyberCore::EventListener::CPPEventListenerType)
    , m_page(page)
#if ENABLE(VIDEO)
    , m_mainVideoElementTextRecognitionTimer(RunLoop::main(), this, &WebFullScreenManager::mainVideoElementTextRecognitionTimerFired)
#endif
{
}
    
WebFullScreenManager::~WebFullScreenManager()
{
    invalidate();
}

void WebFullScreenManager::invalidate()
{
    clearElement();
#if ENABLE(VIDEO)
    setMainVideoElement(nullptr);
    m_mainVideoElementTextRecognitionTimer.stop();
#endif
}

CyberCore::Element* WebFullScreenManager::element()
{ 
    return m_element.get(); 
}

void WebFullScreenManager::videoControlsManagerDidChange()
{
#if PLATFORM(IOS_FAMILY) || (PLATFORM(MAC) && ENABLE(VIDEO_PRESENTATION_MODE))
    LOG(Fullscreen, "WebFullScreenManager %p videoControlsManagerDidChange()", this);

    auto* currentPlaybackControlsElement = m_page->playbackSessionManager().currentPlaybackControlsElement();
    if (!m_element || !is<CyberCore::HTMLVideoElement>(currentPlaybackControlsElement)) {
        setPIPStandbyElement(nullptr);
        return;
    }

    setPIPStandbyElement(downcast<CyberCore::HTMLVideoElement>(currentPlaybackControlsElement));
#endif
}

void WebFullScreenManager::setPIPStandbyElement(CyberCore::HTMLVideoElement* pipStandbyElement)
{
#if ENABLE(VIDEO)
    if (pipStandbyElement == m_pipStandbyElement)
        return;

    LOG(Fullscreen, "WebFullScreenManager %p setPIPStandbyElement() - old element %p, new element %p", this, m_pipStandbyElement.get(), pipStandbyElement);

    if (m_pipStandbyElement)
        m_pipStandbyElement->setVideoFullscreenStandby(false);

    m_pipStandbyElement = pipStandbyElement;

    if (m_pipStandbyElement)
        m_pipStandbyElement->setVideoFullscreenStandby(true);
#endif
}

void WebFullScreenManager::didReceiveMessage(IPC::Connection& connection, IPC::Decoder& decoder)
{
    didReceiveWebFullScreenManagerMessage(connection, decoder);
}

bool WebFullScreenManager::supportsFullScreen(bool withKeyboard)
{
    if (!m_page->corePage()->settings().fullScreenEnabled())
        return false;

    return m_page->injectedBundleFullScreenClient().supportsFullScreen(m_page.get(), withKeyboard);
}

static auto& eventsToObserve()
{
    static NeverDestroyed eventsToObserve = std::array {
        CyberCore::eventNames().playEvent,
        CyberCore::eventNames().pauseEvent,
        CyberCore::eventNames().loadedmetadataEvent,
    };
    return eventsToObserve.get();
}

void WebFullScreenManager::setElement(CyberCore::Element& element)
{
    if (m_element == &element)
        return;

    clearElement();

    m_element = &element;
    m_elementToRestore = element;

    for (auto& eventName : eventsToObserve())
        m_element->addEventListener(eventName, *this, { true });
}

void WebFullScreenManager::clearElement()
{
    if (!m_element)
        return;
    for (auto& eventName : eventsToObserve())
        m_element->removeEventListener(eventName, *this, { true });
    m_element = nullptr;
}

void WebFullScreenManager::enterFullScreenForElement(CyberCore::Element* element)
{
    LOG(Fullscreen, "WebFullScreenManager %p enterFullScreenForElement(%p)", this, element);

    ASSERT(element);
    if (!element)
        return;

    setElement(*element);

    bool isVideoElement = false;
#if PLATFORM(IOS_FAMILY) || (PLATFORM(MAC) && ENABLE(VIDEO_PRESENTATION_MODE))
    isVideoElement = is<HTMLVideoElement>(element);

    if (m_page->videoFullscreenManager().videoElementInPictureInPicture() && m_element->document().quirks().blocksEnteringStandardFullscreenFromPictureInPictureQuirk())
        return;

    if (auto* currentPlaybackControlsElement = m_page->playbackSessionManager().currentPlaybackControlsElement())
        currentPlaybackControlsElement->prepareForVideoFullscreenStandby();
#endif

    m_initialFrame = screenRectOfContents(m_element.get());

    FloatSize videoDimensions;
#if ENABLE(VIDEO)
    updateMainVideoElement();
    if (m_mainVideoElement)
        videoDimensions = FloatSize(m_mainVideoElement->videoWidth(), m_mainVideoElement->videoHeight());
#endif
    m_page->injectedBundleFullScreenClient().enterFullScreenForElement(m_page.get(), element, m_element->document().quirks().blocksReturnToFullscreenFromPictureInPictureQuirk(), isVideoElement, videoDimensions);
}

void WebFullScreenManager::exitFullScreenForElement(CyberCore::Element* element)
{
    LOG(Fullscreen, "WebFullScreenManager %p exitFullScreenForElement(%p) - fullscreen element %p", this, element, m_element.get());
    m_page->injectedBundleFullScreenClient().exitFullScreenForElement(m_page.get(), element);
#if ENABLE(VIDEO)
    setMainVideoElement(nullptr);
#endif
}

void WebFullScreenManager::willEnterFullScreen()
{
    LOG(Fullscreen, "WebFullScreenManager %p willEnterFullScreen() - element %p", this, m_element.get());
    ASSERT(m_element);
    if (!m_element)
        return;

    if (!m_element->document().fullscreenManager().willEnterFullscreen(*m_element)) {
        close();
        return;
    }

#if !PLATFORM(IOS_FAMILY)
    m_page->hidePageBanners();
#endif
    m_element->document().updateLayout();
    m_finalFrame = screenRectOfContents(m_element.get());
    m_page->injectedBundleFullScreenClient().beganEnterFullScreen(m_page.get(), m_initialFrame, m_finalFrame);
}

void WebFullScreenManager::didEnterFullScreen()
{
    LOG(Fullscreen, "WebFullScreenManager %p didEnterFullScreen() - element %p", this, m_element.get());
    ASSERT(m_element);
    if (!m_element)
        return;

    if (!m_element->document().fullscreenManager().didEnterFullscreen()) {
        close();
        return;
    }

#if PLATFORM(IOS_FAMILY) || (PLATFORM(MAC) && ENABLE(VIDEO_PRESENTATION_MODE))
    auto* currentPlaybackControlsElement = m_page->playbackSessionManager().currentPlaybackControlsElement();
    setPIPStandbyElement(dynamicDowncast<CyberCore::HTMLVideoElement>(currentPlaybackControlsElement));
#endif

#if ENABLE(VIDEO)
    updateMainVideoElement();
#endif
}

#if ENABLE(VIDEO)

void WebFullScreenManager::updateMainVideoElement()
{
    setMainVideoElement([&]() -> RefPtr<CyberCore::HTMLVideoElement> {
        if (!m_element)
            return nullptr;

        if (auto video = dynamicDowncast<CyberCore::HTMLVideoElement>(*m_element))
            return video;

        RefPtr<CyberCore::HTMLVideoElement> mainVideo;
        CyberCore::FloatRect mainVideoBounds;
        for (auto& video : CyberCore::descendantsOfType<CyberCore::HTMLVideoElement>(*m_element)) {
            auto rendererAndBounds = video.boundingAbsoluteRectWithoutLayout();
            if (!rendererAndBounds)
                continue;

            auto [renderer, bounds] = *rendererAndBounds;
            if (!renderer || bounds.isEmpty())
                continue;

            if (bounds.area() <= mainVideoBounds.area())
                continue;

            mainVideoBounds = bounds;
            mainVideo = &video;
        }
        return mainVideo;
    }());
}

#endif // ENABLE(VIDEO)

void WebFullScreenManager::willExitFullScreen()
{
    LOG(Fullscreen, "WebFullScreenManager %p willExitFullScreen() - element %p", this, m_element.get());
    if (!m_element)
        return;

#if ENABLE(VIDEO)
    setPIPStandbyElement(nullptr);
#endif

    m_finalFrame = screenRectOfContents(m_element.get());
    if (!m_element->document().fullscreenManager().willExitFullscreen()) {
        close();
        return;
    }
#if !PLATFORM(IOS_FAMILY)
    m_page->showPageBanners();
#endif
    m_page->injectedBundleFullScreenClient().beganExitFullScreen(m_page.get(), m_finalFrame, m_initialFrame);
}

void WebFullScreenManager::didExitFullScreen()
{
    LOG(Fullscreen, "WebFullScreenManager %p didExitFullScreen() - element %p", this, m_element.get());
    if (!m_element)
        return;

    setFullscreenInsets(CyberCore::FloatBoxExtent());
    setFullscreenAutoHideDuration(0_s);
    m_element->document().fullscreenManager().didExitFullscreen();
    clearElement();
}

void WebFullScreenManager::setAnimatingFullScreen(bool animating)
{
    ASSERT(m_element);
    if (!m_element)
        return;
    m_element->document().fullscreenManager().setAnimatingFullscreen(animating);
}

void WebFullScreenManager::requestRestoreFullScreen()
{
    ASSERT(!m_element);
    if (m_element)
        return;

    auto element = RefPtr { m_elementToRestore.get() };
    if (!element)
        return;

    CyberCore::UserGestureIndicator gestureIndicator(CyberCore::ProcessingUserGesture, &element->document());
    element->document().fullscreenManager().requestFullscreenForElement(*element, nullptr, CyberCore::FullscreenManager::ExemptIFrameAllowFullscreenRequirement);
}

void WebFullScreenManager::requestExitFullScreen()
{
    if (!m_element) {
        close();
        return;
    }

    auto& topDocument = m_element->document().topDocument();
    if (!topDocument.fullscreenManager().fullscreenElement()) {
        close();
        return;
    }
    m_element->document().fullscreenManager().cancelFullscreen();
}

void WebFullScreenManager::close()
{
    if (m_closing)
        return;
    m_closing = true;
    LOG(Fullscreen, "WebFullScreenManager %p close()", this);
    m_page->injectedBundleFullScreenClient().closeFullScreen(m_page.get());
    invalidate();
    m_closing = false;
}

void WebFullScreenManager::saveScrollPosition()
{
    if (auto* localMainFrame = dynamicDowncast<LocalFrame>(m_page->corePage()->mainFrame()))
        m_scrollPosition = localMainFrame->view()->scrollPosition();
}

void WebFullScreenManager::restoreScrollPosition()
{
    if (auto* localMainFrame = dynamicDowncast<LocalFrame>(m_page->corePage()->mainFrame())) {
        // Make sure overflow: hidden is unapplied from the root element before restoring.
        localMainFrame->view()->forceLayout();
        localMainFrame->view()->setScrollPosition(m_scrollPosition);
    }
}

void WebFullScreenManager::setFullscreenInsets(const CyberCore::FloatBoxExtent& insets)
{
    m_page->corePage()->setFullscreenInsets(insets);
}

void WebFullScreenManager::setFullscreenAutoHideDuration(Seconds duration)
{
    m_page->corePage()->setFullscreenAutoHideDuration(duration);
}

void WebFullScreenManager::setFullscreenControlsHidden(bool hidden)
{
    m_page->corePage()->setFullscreenControlsHidden(hidden);
}

void WebFullScreenManager::handleEvent(CyberCore::ScriptExecutionContext& context, CyberCore::Event& event)
{
#if ENABLE(VIDEO)
    RefPtr targetElement = dynamicDowncast<CyberCore::Element>(event.currentTarget());
    if (!m_element || !targetElement)
        return;

    Ref document = m_element->document();
    if (&context != document.ptr() || !document->fullscreenManager().isFullscreen())
        return;

    if (targetElement == m_element) {
        updateMainVideoElement();
        return;
    }

    if (targetElement == m_mainVideoElement.get()) {
        auto& targetVideoElement = downcast<CyberCore::HTMLVideoElement>(*targetElement);
        if (targetVideoElement.paused() && !targetVideoElement.seeking())
            scheduleTextRecognitionForMainVideo();
        else
            endTextRecognitionForMainVideoIfNeeded();
    }
#else
    UNUSED_PARAM(event);
    UNUSED_PARAM(context);
#endif
}

#if ENABLE(VIDEO)

void WebFullScreenManager::mainVideoElementTextRecognitionTimerFired()
{
    if (!m_element || !m_element->document().fullscreenManager().isFullscreen())
        return;

    updateMainVideoElement();

    if (!m_mainVideoElement)
        return;

    if (m_isPerformingTextRecognitionInMainVideo)
        m_page->cancelTextRecognitionForVideoInElementFullScreen();

    m_isPerformingTextRecognitionInMainVideo = true;
    m_page->beginTextRecognitionForVideoInElementFullScreen(*m_mainVideoElement);
}

void WebFullScreenManager::scheduleTextRecognitionForMainVideo()
{
    m_mainVideoElementTextRecognitionTimer.startOneShot(250_ms);
}

void WebFullScreenManager::endTextRecognitionForMainVideoIfNeeded()
{
    m_mainVideoElementTextRecognitionTimer.stop();

    if (m_isPerformingTextRecognitionInMainVideo) {
        m_page->cancelTextRecognitionForVideoInElementFullScreen();
        m_isPerformingTextRecognitionInMainVideo = false;
    }
}

void WebFullScreenManager::setMainVideoElement(RefPtr<CyberCore::HTMLVideoElement>&& element)
{
    if (element == m_mainVideoElement.get())
        return;

    static NeverDestroyed eventsToObserve = std::array {
        CyberCore::eventNames().seekingEvent,
        CyberCore::eventNames().seekedEvent,
        CyberCore::eventNames().playingEvent,
        CyberCore::eventNames().pauseEvent,
    };

    if (m_mainVideoElement) {
        for (auto& eventName : eventsToObserve.get())
            m_mainVideoElement->removeEventListener(eventName, *this, { });

        endTextRecognitionForMainVideoIfNeeded();
    }

    m_mainVideoElement = WTFMove(element);

    if (m_mainVideoElement) {
        for (auto& eventName : eventsToObserve.get())
            m_mainVideoElement->addEventListener(eventName, *this, { });

        if (m_mainVideoElement->paused())
            scheduleTextRecognitionForMainVideo();
    }
}

#endif // ENABLE(VIDEO)

} // namespace CyberKit

#endif // ENABLE(FULLSCREEN_API)
