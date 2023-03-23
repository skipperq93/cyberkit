/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
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

#if ENABLE(MEDIA_STREAM)

#include <CyberCore/RealtimeMediaSource.h>
#include <CyberCore/RealtimeMediaSourceIdentifier.h>

#if PLATFORM(COCOA)
#include "SharedCARingBuffer.h"
#include <CyberCore/CAAudioStreamDescription.h>
#endif

namespace CyberCore {
class CaptureDevice;
#if PLATFORM(COCOA)
class WebAudioBufferList;
#endif
}

namespace WebKit {
class SpeechRecognitionRemoteRealtimeMediaSourceManager;
    
class SpeechRecognitionRemoteRealtimeMediaSource : public CyberCore::RealtimeMediaSource {
public:
    static Ref<CyberCore::RealtimeMediaSource> create(SpeechRecognitionRemoteRealtimeMediaSourceManager&, const CyberCore::CaptureDevice&, CyberCore::PageIdentifier);
    ~SpeechRecognitionRemoteRealtimeMediaSource();

    CyberCore::RealtimeMediaSourceIdentifier identifier() const { return m_identifier; }

#if PLATFORM(COCOA)
    void setStorage(ConsumerSharedCARingBuffer::Handle&&, const CyberCore::CAAudioStreamDescription&);
#endif

    void remoteAudioSamplesAvailable(MediaTime, uint64_t numberOfFrames);
    void remoteCaptureFailed();
    void remoteSourceStopped();

private:
    SpeechRecognitionRemoteRealtimeMediaSource(CyberCore::RealtimeMediaSourceIdentifier, SpeechRecognitionRemoteRealtimeMediaSourceManager&, const CyberCore::CaptureDevice&, CyberCore::PageIdentifier);

    // CyberCore::RealtimeMediaSource
    void startProducingData() final;
    void stopProducingData() final;
    const CyberCore::RealtimeMediaSourceCapabilities& capabilities() final { return m_capabilities; }
    const CyberCore::RealtimeMediaSourceSettings& settings() final { return m_settings; }

    CyberCore::RealtimeMediaSourceIdentifier m_identifier;
    WeakPtr<SpeechRecognitionRemoteRealtimeMediaSourceManager> m_manager;
    CyberCore::RealtimeMediaSourceCapabilities m_capabilities;
    CyberCore::RealtimeMediaSourceSettings m_settings;

#if PLATFORM(COCOA)
    std::optional<CyberCore::CAAudioStreamDescription> m_description;
    std::unique_ptr<ConsumerSharedCARingBuffer> m_ringBuffer;
    std::unique_ptr<CyberCore::WebAudioBufferList> m_buffer;
#endif
};

} // namespace WebKit

#endif
