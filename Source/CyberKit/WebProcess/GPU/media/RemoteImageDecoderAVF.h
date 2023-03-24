/*
 * Copyright (C) 2021 Apple Inc. All rights reserved.
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

#if ENABLE(GPU_PROCESS) && HAVE(AVASSETREADER)

#include "MessageReceiver.h"
#include "RemoteImageDecoderAVFManager.h"
#include "CyberCoreArgumentCoders.h"
#include <CyberCore/ImageDecoder.h>
#include <CyberCore/ImageDecoderIdentifier.h>
#include <wtf/Function.h>
#include <wtf/HashMap.h>
#include <wtf/WeakPtr.h>

namespace CyberKit {

class GPUProcessConnection;
class WebProcess;

class RemoteImageDecoderAVF final
    : public CyberCore::ImageDecoder
    , public CanMakeWeakPtr<RemoteImageDecoderAVF> {
public:
    static Ref<RemoteImageDecoderAVF> create(RemoteImageDecoderAVFManager& manager, const CyberCore::ImageDecoderIdentifier& identifier, CyberCore::FragmentedSharedBuffer&, const String& mimeType)
    {
        return adoptRef(*new RemoteImageDecoderAVF(manager, identifier, mimeType));
    }
    RemoteImageDecoderAVF(RemoteImageDecoderAVFManager&, const CyberCore::ImageDecoderIdentifier&, const String& mimeType);

    virtual ~RemoteImageDecoderAVF();

    static bool canDecodeType(const String& mimeType);
    static bool supportsMediaType(MediaType);

    size_t bytesDecodedToDetermineProperties() const override { return 0; }

    CyberCore::EncodedDataStatus encodedDataStatus() const final;
    void setEncodedDataStatusChangeCallback(WTF::Function<void(CyberCore::EncodedDataStatus)>&&) final;
    CyberCore::IntSize size() const final;
    size_t frameCount() const final;
    CyberCore::RepetitionCount repetitionCount() const final;
    String uti() const final;
    String filenameExtension() const final;
    std::optional<CyberCore::IntPoint> hotSpot() const final { return std::nullopt; }
    String accessibilityDescription() const final { return String(); }

    CyberCore::IntSize frameSizeAtIndex(size_t, CyberCore::SubsamplingLevel = CyberCore::SubsamplingLevel::Default) const final;
    bool frameIsCompleteAtIndex(size_t) const final;
    FrameMetadata frameMetadataAtIndex(size_t) const final;

    Seconds frameDurationAtIndex(size_t) const final;
    bool frameHasAlphaAtIndex(size_t) const final;
    bool frameAllowSubsamplingAtIndex(size_t) const final;
    unsigned frameBytesAtIndex(size_t, CyberCore::SubsamplingLevel = CyberCore::SubsamplingLevel::Default) const final;

    CyberCore::PlatformImagePtr createFrameImageAtIndex(size_t, CyberCore::SubsamplingLevel = CyberCore::SubsamplingLevel::Default, const CyberCore::DecodingOptions& = CyberCore::DecodingOptions(CyberCore::DecodingMode::Synchronous)) final;

    void setExpectedContentSize(long long) final;
    void setData(const CyberCore::FragmentedSharedBuffer&, bool allDataReceived) final;
    bool isAllDataReceived() const final { return m_isAllDataReceived; }
    void clearFrameBufferCache(size_t) final;

    void encodedDataStatusChanged(size_t frameCount, const CyberCore::IntSize&, bool hasTrack);

private:
    WeakPtr<GPUProcessConnection> m_gpuProcessConnection;
    RemoteImageDecoderAVFManager& m_manager;
    CyberCore::ImageDecoderIdentifier m_identifier;

    String m_mimeType;
    String m_uti;
    bool m_isAllDataReceived { false };
    WTF::Function<void(CyberCore::EncodedDataStatus)> m_encodedDataStatusChangedCallback;
    HashMap<int, CyberCore::PlatformImagePtr, WTF::IntHash<int>, WTF::UnsignedWithZeroKeyHashTraits<int>> m_frameImages;
    Vector<ImageDecoder::FrameInfo> m_frameInfos;
    size_t m_frameCount { 0 };
    std::optional<CyberCore::IntSize> m_size;
    bool m_hasTrack { false };
};

}
#endif
