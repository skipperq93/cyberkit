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

#include "ImageBufferBackendHandleSharing.h"
#include <CyberCore/ImageBuffer.h>
#include <CyberCore/PlatformImageBufferBackend.h>
#include <wtf/IsoMalloc.h>

namespace CyberCore {
class ProcessIdentity;
}

namespace CyberKit {

class ShareableBitmap;

class ImageBufferShareableBitmapBackend final : public CyberCore::PlatformImageBufferBackend, public ImageBufferBackendHandleSharing {
    WTF_MAKE_ISO_ALLOCATED(ImageBufferShareableBitmapBackend);
    WTF_MAKE_NONCOPYABLE(ImageBufferShareableBitmapBackend);

public:
    static CyberCore::IntSize calculateSafeBackendSize(const Parameters&);
    static unsigned calculateBytesPerRow(const Parameters&, const CyberCore::IntSize& backendSize);
    static size_t calculateMemoryCost(const Parameters&);

    static std::unique_ptr<ImageBufferShareableBitmapBackend> create(const Parameters&, const CyberCore::ImageBufferCreationContext&);
    static std::unique_ptr<ImageBufferShareableBitmapBackend> create(const Parameters&, ImageBufferBackendHandle);

    ImageBufferShareableBitmapBackend(const Parameters&, Ref<ShareableBitmap>&&, std::unique_ptr<CyberCore::GraphicsContext>&&);

    CyberCore::GraphicsContext& context() final { return *m_context; }
    CyberCore::IntSize backendSize() const final;

    ImageBufferBackendHandle createBackendHandle(SharedMemory::Protection = SharedMemory::Protection::ReadWrite) const final;
    RefPtr<ShareableBitmap> bitmap() const final { return m_bitmap.ptr(); }
#if USE(CAIRO)
    RefPtr<cairo_surface_t> createCairoSurface() final;
#endif

    RefPtr<CyberCore::NativeImage> copyNativeImage(CyberCore::BackingStoreCopy = CyberCore::CopyBackingStore) final;

    RefPtr<CyberCore::PixelBuffer> getPixelBuffer(const CyberCore::PixelBufferFormat& outputFormat, const CyberCore::IntRect&, const CyberCore::ImageBufferAllocator&) final;
    void putPixelBuffer(const CyberCore::PixelBuffer&, const CyberCore::IntRect& srcRect, const CyberCore::IntPoint& destPoint, CyberCore::AlphaPremultiplication destFormat) final;

private:
    unsigned bytesPerRow() const final;

    ImageBufferBackendSharing* toBackendSharing() final { return this; }
    void releaseGraphicsContext() final { /* Do nothing. This is only relevant for IOSurface backends */ }

    Ref<ShareableBitmap> m_bitmap;
    std::unique_ptr<CyberCore::GraphicsContext> m_context;
};

} // namespace CyberKit
