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

#include "config.h"
#include "SpeechRecognitionRemoteRealtimeMediaSource.h"

#if ENABLE(MEDIA_STREAM)

#include "SpeechRecognitionRealtimeMediaSourceManagerMessages.h"
#include "SpeechRecognitionRemoteRealtimeMediaSourceManager.h"

#if PLATFORM(COCOA)
#include "SharedCARingBuffer.h"
#include <CyberCore/CARingBuffer.h>
#include <CyberCore/WebAudioBufferList.h>
#endif

namespace WebKit {

Ref<CyberCore::RealtimeMediaSource> SpeechRecognitionRemoteRealtimeMediaSource::create(SpeechRecognitionRemoteRealtimeMediaSourceManager& manager, const CyberCore::CaptureDevice& captureDevice, CyberCore::PageIdentifier pageIdentifier)
{
    return adoptRef(*new SpeechRecognitionRemoteRealtimeMediaSource(CyberCore::RealtimeMediaSourceIdentifier::generate(), manager, captureDevice, pageIdentifier));
}

SpeechRecognitionRemoteRealtimeMediaSource::SpeechRecognitionRemoteRealtimeMediaSource(CyberCore::RealtimeMediaSourceIdentifier identifier, SpeechRecognitionRemoteRealtimeMediaSourceManager& manager, const CyberCore::CaptureDevice& captureDevice, CyberCore::PageIdentifier pageIdentifier)
    : CyberCore::RealtimeMediaSource(captureDevice, { }, pageIdentifier)
    , m_identifier(identifier)
    , m_manager(manager)
{
    m_manager->addSource(*this, captureDevice);
}

SpeechRecognitionRemoteRealtimeMediaSource::~SpeechRecognitionRemoteRealtimeMediaSource()
{
    if (m_manager)
        m_manager->removeSource(*this);
}

void SpeechRecognitionRemoteRealtimeMediaSource::startProducingData()
{
    if (m_manager)
        m_manager->send(Messages::SpeechRecognitionRealtimeMediaSourceManager::Start { m_identifier });
}

void SpeechRecognitionRemoteRealtimeMediaSource::stopProducingData()
{
    if (m_manager)
        m_manager->send(Messages::SpeechRecognitionRealtimeMediaSourceManager::Stop { m_identifier });
}

#if PLATFORM(COCOA)

void SpeechRecognitionRemoteRealtimeMediaSource::setStorage(ConsumerSharedCARingBuffer::Handle&& handle, const CyberCore::CAAudioStreamDescription& description)
{
    m_buffer = nullptr;
    m_ringBuffer = ConsumerSharedCARingBuffer::map(description, WTFMove(handle));
    if (!m_ringBuffer)
        return;
    m_description = description;
    m_buffer = makeUnique<CyberCore::WebAudioBufferList>(description);
}

#endif

void SpeechRecognitionRemoteRealtimeMediaSource::remoteAudioSamplesAvailable(MediaTime time, uint64_t numberOfFrames)
{
#if PLATFORM(COCOA)
    if (!m_buffer) {
        LOG_ERROR("Buffer for remote source is null");
        captureFailed();
        return;
    }

    m_buffer->setSampleCount(numberOfFrames);
    m_ringBuffer->fetch(m_buffer->list(), numberOfFrames, time.timeValue());
    audioSamplesAvailable(time, *m_buffer, *m_description, numberOfFrames);
#else
    UNUSED_PARAM(time);
    UNUSED_PARAM(numberOfFrames);
#endif
}

void SpeechRecognitionRemoteRealtimeMediaSource::remoteCaptureFailed()
{
    captureFailed();
}

void SpeechRecognitionRemoteRealtimeMediaSource::remoteSourceStopped()
{
    stop();
}

} // namespace WebKit

#endif
