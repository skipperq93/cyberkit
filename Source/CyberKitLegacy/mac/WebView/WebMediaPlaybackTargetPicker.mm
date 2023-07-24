/*
 * Copyright (C) 2015-2020 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "WebMediaPlaybackTargetPicker.h"

#if ENABLE(WIRELESS_PLAYBACK_TARGET) && !PLATFORM(IOS_FAMILY)

#import "WebView.h"
#import <CyberCore/MediaPlaybackTarget.h>
#import <CyberCore/Page.h>
#import <CyberCore/WebMediaSessionManager.h>

std::unique_ptr<WebMediaPlaybackTargetPicker> WebMediaPlaybackTargetPicker::create(WebView *webView, CyberCore::Page& page)
{
    return makeUnique<WebMediaPlaybackTargetPicker>(webView, page);
}

WebMediaPlaybackTargetPicker::WebMediaPlaybackTargetPicker(WebView *webView, CyberCore::Page& page)
    : m_page(&page)
    , m_webView(webView)
{
}

void WebMediaPlaybackTargetPicker::addPlaybackTargetPickerClient(CyberCore::PlaybackTargetClientContextIdentifier contextId)
{
    CyberCore::WebMediaSessionManager::shared().addPlaybackTargetPickerClient(*this, contextId);
}

void WebMediaPlaybackTargetPicker::removePlaybackTargetPickerClient(CyberCore::PlaybackTargetClientContextIdentifier contextId)
{
    CyberCore::WebMediaSessionManager::shared().removePlaybackTargetPickerClient(*this, contextId);
}

void WebMediaPlaybackTargetPicker::showPlaybackTargetPicker(CyberCore::PlaybackTargetClientContextIdentifier contextId, const CyberCore::FloatRect& rect, bool hasVideo)
{
    CyberCore::WebMediaSessionManager::shared().showPlaybackTargetPicker(*this, contextId, CyberCore::IntRect(rect), hasVideo, m_page ? m_page->useDarkAppearance() : false);
}

void WebMediaPlaybackTargetPicker::playbackTargetPickerClientStateDidChange(CyberCore::PlaybackTargetClientContextIdentifier contextId, CyberCore::MediaProducer::MediaStateFlags state)
{
    CyberCore::WebMediaSessionManager::shared().clientStateDidChange(*this, contextId, state);
}

void WebMediaPlaybackTargetPicker::setMockMediaPlaybackTargetPickerEnabled(bool enabled)
{
    CyberCore::WebMediaSessionManager::shared().setMockMediaPlaybackTargetPickerEnabled(enabled);
}

void WebMediaPlaybackTargetPicker::setMockMediaPlaybackTargetPickerState(const String& name, CyberCore::MediaPlaybackTargetContext::State state)
{
    CyberCore::WebMediaSessionManager::shared().setMockMediaPlaybackTargetPickerState(name, state);
}

void WebMediaPlaybackTargetPicker::mockMediaPlaybackTargetPickerDismissPopup()
{
    CyberCore::WebMediaSessionManager::shared().mockMediaPlaybackTargetPickerDismissPopup();
}

void WebMediaPlaybackTargetPicker::setPlaybackTarget(CyberCore::PlaybackTargetClientContextIdentifier contextId, Ref<CyberCore::MediaPlaybackTarget>&& target)
{
    if (!m_page)
        return;

    m_page->setPlaybackTarget(contextId, WTFMove(target));
}

void WebMediaPlaybackTargetPicker::externalOutputDeviceAvailableDidChange(CyberCore::PlaybackTargetClientContextIdentifier contextId, bool available)
{
    if (!m_page)
        return;

    m_page->playbackTargetAvailabilityDidChange(contextId, available);
}

void WebMediaPlaybackTargetPicker::setShouldPlayToPlaybackTarget(CyberCore::PlaybackTargetClientContextIdentifier contextId, bool shouldPlay)
{
    if (!m_page)
        return;

    m_page->setShouldPlayToPlaybackTarget(contextId, shouldPlay);
}

void WebMediaPlaybackTargetPicker::playbackTargetPickerWasDismissed(CyberCore::PlaybackTargetClientContextIdentifier contextId)
{
    if (m_page)
        m_page->playbackTargetPickerWasDismissed(contextId);
}

void WebMediaPlaybackTargetPicker::invalidate()
{
    m_page = nullptr;
    m_webView = nil;
    CyberCore::WebMediaSessionManager::shared().removeAllPlaybackTargetPickerClients(*this);
}

PlatformView* WebMediaPlaybackTargetPicker::platformView() const
{
    ASSERT(m_webView);
    return m_webView;
}

#endif
