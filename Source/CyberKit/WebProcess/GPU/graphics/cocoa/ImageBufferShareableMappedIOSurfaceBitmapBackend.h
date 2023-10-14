/*
 * Copyright (C) 2020-2022 Apple Inc.  All rights reserved.
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

#if ENABLE(GPU_PROCESS) && HAVE(IOSURFACE)

#include "ImageBufferBackendHandleSharing.h"
#include <CyberCore/IOSurface.h>
#include <CyberCore/ImageBufferIOSurfaceBackend.h>
#include <wtf/IsoMalloc.h>

namespace CyberCore {
class ProcessIdentity;
}

namespace CyberKit {

// ImageBufferBackend for small LayerBacking stores.
class ImageBufferShareableMappedIOSurfaceBitmapBackend final : public CyberCore::ImageBufferCGBackend, public ImageBufferBackendHandleSharing {
    WTF_MAKE_ISO_ALLOCATED(ImageBufferShareableMappedIOSurfaceBitmapBackend);
    WTF_MAKE_NONCOPYABLE(ImageBufferShareableMappedIOSurfaceBitmapBackend);
public:
    static std::unique_ptr<ImageBufferShareableMappedIOSurfaceBitmapBackend> create(const Parameters&, const CyberCore::ImageBufferCreationContext&);
    static size_t calculateMemoryCost(const Parameters& parameters) { return CyberCore::ImageBufferIOSurfaceBackend::calculateMemoryCost(parameters); }

    ImageBufferShareableMappedIOSurfaceBitmapBackend(const Parameters&, std::unique_ptr<CyberCore::IOSurface>, CyberCore::IOSurface::LockAndContext&&, CyberCore::IOSurfacePool*);

    static constexpr CyberCore::RenderingMode renderingMode = CyberCore::RenderingMode::Accelerated;
    static constexpr bool isOriginAtBottomLeftCorner = true;

    ImageBufferBackendHandle createBackendHandle(SharedMemory::Protection = SharedMemory::Protection::ReadWrite) const final;
    void setOwnershipIdentity(const CyberCore::ProcessIdentity&) final;
    CyberCore::GraphicsContext& context() final;
    bool originAtBottomLeftCorner() const override { return isOriginAtBottomLeftCorner; }
private:
    // ImageBufferBackendSharing
    ImageBufferBackendSharing* toBackendSharing() final { return this; }

    // CyberCore::ImageBufferCGBackend
    CyberCore::IntSize backendSize() const final;
    unsigned bytesPerRow() const final;
    RefPtr<CyberCore::NativeImage> copyNativeImage(CyberCore::BackingStoreCopy = CyberCore::CopyBackingStore) final;
    RefPtr<CyberCore::NativeImage> copyNativeImageForDrawing(CyberCore::GraphicsContext&) final;
    RefPtr<CyberCore::NativeImage> sinkIntoNativeImage() final;
    bool isInUse() const final;
    void releaseGraphicsContext() final;
    bool setVolatile() final;
    CyberCore::SetNonVolatileResult setNonVolatile() final;
    CyberCore::VolatilityState volatilityState() const final;
    void setVolatilityState(CyberCore::VolatilityState) final;
    void transferToNewContext(const CyberCore::ImageBufferCreationContext&) final;
    void getPixelBuffer(const CyberCore::IntRect&, CyberCore::PixelBuffer&) final;
    void putPixelBuffer(const CyberCore::PixelBuffer&, const CyberCore::IntRect&, const CyberCore::IntPoint&, CyberCore::AlphaPremultiplication) final;

    std::unique_ptr<CyberCore::IOSurface> m_surface;
    std::optional<CyberCore::IOSurface::Locker> m_lock;
    CyberCore::VolatilityState m_volatilityState { CyberCore::VolatilityState::NonVolatile };
    RefPtr<CyberCore::IOSurfacePool> m_ioSurfacePool;
};

} // namespace CyberKit

#endif // ENABLE(GPU_PROCESS) && HAVE(IOSURFACE)
