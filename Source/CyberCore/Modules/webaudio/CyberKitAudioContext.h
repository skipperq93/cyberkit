/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 * Copyright (C) 2016-2020 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "BaseAudioContext.h"
#include "CyberKitAudioListener.h"

namespace CyberCore {

class AudioBuffer;
class Document;
class HTMLMediaElement;
class MediaElementAudioSourceNode;
class MediaStream;
class MediaStreamAudioDestinationNode;
class MediaStreamAudioSourceNode;
class PeriodicWave;
class CyberKitAudioBufferSourceNode;
class CyberKitAudioPannerNode;
class CyberKitDynamicsCompressorNode;
class CyberKitOscillatorNode;

// AudioContext is the cornerstone of the web audio API and all AudioNodes are created from it.
// For thread safety between the audio thread and the main thread, it has a rendering graph locking mechanism.

class CyberKitAudioContext
    : public BaseAudioContext
{
    WTF_MAKE_ISO_ALLOCATED(CyberKitAudioContext);
public:
    // Create an CyberKitAudioContext for rendering to the audio hardware.
    static ExceptionOr<Ref<CyberKitAudioContext>> create(Document&);

    void close(DOMPromiseDeferred<void>&&);

    CyberKitAudioListener& listener() { return downcast<CyberKitAudioListener>(BaseAudioContext::listener()); }

    // The AudioNode create methods are called on the main thread (from JavaScript).
#if ENABLE(VIDEO)
    ExceptionOr<Ref<MediaElementAudioSourceNode>> createMediaElementSource(HTMLMediaElement&);
#endif
#if ENABLE(MEDIA_STREAM)
    ExceptionOr<Ref<MediaStreamAudioSourceNode>> createMediaStreamSource(MediaStream&);
    ExceptionOr<Ref<MediaStreamAudioDestinationNode>> createMediaStreamDestination();
#endif
    ExceptionOr<Ref<CyberKitAudioPannerNode>> createCyberKitPanner();
    ExceptionOr<Ref<CyberKitOscillatorNode>> createCyberKitOscillator();
    ExceptionOr<Ref<PeriodicWave>> createPeriodicWave(Float32Array& real, Float32Array& imaginary);
    ExceptionOr<Ref<CyberKitAudioBufferSourceNode>> createCyberKitBufferSource();
    ExceptionOr<Ref<CyberKitDynamicsCompressorNode>> createCyberKitDynamicsCompressor();
    ExceptionOr<Ref<AudioBuffer>> createLegacyBuffer(ArrayBuffer&, bool mixToMono);

protected:
    explicit CyberKitAudioContext(Document&);
    CyberKitAudioContext(Document&, AudioBuffer* renderTarget);

private:
    // ActiveDOMObject API.
    const char* activeDOMObjectName() const override;

    bool isCyberKitAudioContext() const final { return true; }
};

} // CyberCore

SPECIALIZE_TYPE_TRAITS_BEGIN(CyberCore::CyberKitAudioContext)
    static bool isType(const CyberCore::BaseAudioContext& context) { return context.isCyberKitAudioContext(); }
SPECIALIZE_TYPE_TRAITS_END()
