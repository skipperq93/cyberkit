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

#pragma once

#if ENABLE(WIRELESS_PLAYBACK_TARGET) && !PLATFORM(IOS_FAMILY)

#include <CyberCore/MediaPlaybackTarget.h>
#include <CyberCore/MediaPlaybackTargetContext.h>
#include <CyberCore/WebMediaSessionManagerClient.h>
#include <wtf/Ref.h>

OBJC_CLASS WebView;

namespace CyberCore {
class FloatRect;
class MediaPlaybackTarget;
class Page;
}

class WebMediaPlaybackTargetPicker : public CyberCore::WebMediaSessionManagerClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    static std::unique_ptr<WebMediaPlaybackTargetPicker> create(WebView *, CyberCore::Page&);

    explicit WebMediaPlaybackTargetPicker(WebView *, CyberCore::Page&);
    virtual ~WebMediaPlaybackTargetPicker() = default;

    void addPlaybackTargetPickerClient(CyberCore::PlaybackTargetClientContextIdentifier);
    void removePlaybackTargetPickerClient(CyberCore::PlaybackTargetClientContextIdentifier);
    void showPlaybackTargetPicker(CyberCore::PlaybackTargetClientContextIdentifier, const CyberCore::FloatRect&, bool hasVideo);
    void playbackTargetPickerClientStateDidChange(CyberCore::PlaybackTargetClientContextIdentifier, CyberCore::MediaProducer::MediaStateFlags);
    void setMockMediaPlaybackTargetPickerEnabled(bool);
    void setMockMediaPlaybackTargetPickerState(const String&, CyberCore::MediaPlaybackTargetContext::State);
    void mockMediaPlaybackTargetPickerDismissPopup();

    void invalidate();

private:
    // WebMediaSessionManagerClient
    void setPlaybackTarget(CyberCore::PlaybackTargetClientContextIdentifier, Ref<CyberCore::MediaPlaybackTarget>&&) final;
    void externalOutputDeviceAvailableDidChange(CyberCore::PlaybackTargetClientContextIdentifier, bool) final;
    void setShouldPlayToPlaybackTarget(CyberCore::PlaybackTargetClientContextIdentifier, bool) final;
    void playbackTargetPickerWasDismissed(CyberCore::PlaybackTargetClientContextIdentifier) final;
    PlatformView* platformView() const final;

    CyberCore::Page* m_page;
    WebView *m_webView;
};

#endif
