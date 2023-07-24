/*
 * Copyright (C) 2020 Apple Inc.  All rights reserved.
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

#if ENABLE(GPU_PROCESS)

#include "ImageBufferBackendHandle.h"
#include <CyberCore/PlatformImageBufferBackend.h>
#include <wtf/IsoMalloc.h>

namespace CyberCore {
class ImageData;
}

namespace CyberKit {

class ShareableBitmap;

class ImageBufferShareableBitmapBackend : public CyberCore::PlatformImageBufferBackend {
    WTF_MAKE_ISO_ALLOCATED(ImageBufferShareableBitmapBackend);
    WTF_MAKE_NONCOPYABLE(ImageBufferShareableBitmapBackend);
public:
    static std::unique_ptr<ImageBufferShareableBitmapBackend> create(const CyberCore::FloatSize& logicalSize, const float resolutionScale, CyberCore::ColorSpace, const CyberCore::HostWindow*);
    static std::unique_ptr<ImageBufferShareableBitmapBackend> create(const CyberCore::FloatSize& logicalSize, const CyberCore::IntSize& internalSize, float resolutionScale, CyberCore::ColorSpace, ImageBufferBackendHandle);

    ImageBufferShareableBitmapBackend(const CyberCore::FloatSize& logicalSize, const CyberCore::IntSize& physicalSize, float resolutionScale, CyberCore::ColorSpace, RefPtr<ShareableBitmap>&&, std::unique_ptr<CyberCore::GraphicsContext>&&);

    ImageBufferBackendHandle createImageBufferBackendHandle() const;

    CyberCore::GraphicsContext& context() const override { return *m_context; }

    CyberCore::NativeImagePtr copyNativeImage(CyberCore::BackingStoreCopy = CyberCore::CopyBackingStore) const override;
    RefPtr<CyberCore::Image> copyImage(CyberCore::BackingStoreCopy = CyberCore::CopyBackingStore, CyberCore::PreserveResolution = CyberCore::PreserveResolution::No) const override;

    Vector<uint8_t> toBGRAData() const override;
    RefPtr<CyberCore::ImageData> getImageData(CyberCore::AlphaPremultiplication outputFormat, const CyberCore::IntRect&) const override;
    void putImageData(CyberCore::AlphaPremultiplication inputFormat, const CyberCore::ImageData&, const CyberCore::IntRect& srcRect, const CyberCore::IntPoint& destPoint, CyberCore::AlphaPremultiplication destFormat) override;

private:
#if PLATFORM(IOS_FAMILY)
    ColorFormat backendColorFormat() const override { return ColorFormat::BGRA; }
#endif

    RefPtr<ShareableBitmap> m_bitmap;
    std::unique_ptr<CyberCore::GraphicsContext> m_context;
};

} // namespace CyberKit

#endif // ENABLE(GPU_PROCESS)
