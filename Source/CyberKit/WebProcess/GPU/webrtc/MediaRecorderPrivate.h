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

#if PLATFORM(COCOA) && ENABLE(GPU_PROCESS) && ENABLE(MEDIA_STREAM)

#include "GPUProcessConnection.h"
#include "MediaRecorderIdentifier.h"
#include "SharedCARingBuffer.h"
#include "SharedVideoFrame.h"
#include <CyberCore/CAAudioStreamDescription.h>
#include <CyberCore/MediaRecorderPrivate.h>
#include <wtf/MediaTime.h>
#include <wtf/WeakPtr.h>

namespace IPC {
class Connection;
}

namespace CyberCore {
class MediaStreamPrivate;
class WebAudioBufferList;
}

namespace CyberKit {

class MediaRecorderPrivate final
    : public CyberCore::MediaRecorderPrivate
    , public GPUProcessConnection::Client {
    WTF_MAKE_FAST_ALLOCATED;
public:
    MediaRecorderPrivate(CyberCore::MediaStreamPrivate&, const CyberCore::MediaRecorderPrivateOptions&);
    ~MediaRecorderPrivate();

private:
    // CyberCore::MediaRecorderPrivate
    void videoFrameAvailable(CyberCore::VideoFrame&, CyberCore::VideoFrameTimeMetadata) final;
    void fetchData(CompletionHandler<void(RefPtr<CyberCore::FragmentedSharedBuffer>&&, const String& mimeType, double)>&&) final;
    void stopRecording(CompletionHandler<void()>&&) final;
    void startRecording(StartRecordingCallback&&) final;
    void audioSamplesAvailable(const WTF::MediaTime&, const CyberCore::PlatformAudioData&, const CyberCore::AudioStreamDescription&, size_t) final;
    const String& mimeType() const final;
    void pauseRecording(CompletionHandler<void()>&&) final;
    void resumeRecording(CompletionHandler<void()>&&) final;

    // GPUProcessConnection::Client
    void gpuProcessConnectionDidClose(GPUProcessConnection&) final;

    MediaRecorderIdentifier m_identifier;
    Ref<CyberCore::MediaStreamPrivate> m_stream;
    Ref<IPC::Connection> m_connection;

    std::unique_ptr<ProducerSharedCARingBuffer> m_ringBuffer;
    std::optional<CyberCore::CAAudioStreamDescription> m_description;
    std::unique_ptr<CyberCore::WebAudioBufferList> m_silenceAudioBuffer;
    uint64_t m_numberOfFrames { 0 };
    CyberCore::MediaRecorderPrivateOptions m_options;
    bool m_hasVideo { false };
    bool m_isStopped { false };
    std::optional<CyberCore::IntSize> m_blackFrameSize;

    SharedVideoFrameWriter m_sharedVideoFrameWriter;
};

}

#endif // PLATFORM(COCOA) && ENABLE(GPU_PROCESS) && ENABLE(MEDIA_STREAM)
