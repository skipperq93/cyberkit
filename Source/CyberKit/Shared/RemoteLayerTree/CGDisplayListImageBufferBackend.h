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

#if ENABLE(CG_DISPLAY_LIST_BACKED_IMAGE_BUFFER)

#include "ImageBufferBackendHandleSharing.h"
#include <CyberCore/ImageBuffer.h>
#include <CyberCore/ImageBufferCGBackend.h>
#include <wtf/IsoMalloc.h>

namespace CyberKit {

using UseCGDisplayListImageCache = CyberCore::ImageBufferCreationContext::UseCGDisplayListImageCache;

class CGDisplayListImageBufferBackend : public CyberCore::ImageBufferCGBackend, public ImageBufferBackendHandleSharing {
    WTF_MAKE_ISO_ALLOCATED(CGDisplayListImageBufferBackend);
    WTF_MAKE_NONCOPYABLE(CGDisplayListImageBufferBackend);
public:
    static size_t calculateMemoryCost(const Parameters&);

    static std::unique_ptr<CGDisplayListImageBufferBackend> create(const Parameters&, const CyberCore::ImageBufferCreationContext&);

    CyberCore::GraphicsContext& context() final;
    CyberCore::IntSize backendSize() const final;
    ImageBufferBackendHandle createBackendHandle(SharedMemory::Protection = SharedMemory::Protection::ReadWrite) const final;

    void releaseGraphicsContext() final;

    // NOTE: These all ASSERT_NOT_REACHED().
    RefPtr<CyberCore::NativeImage> copyNativeImage(CyberCore::BackingStoreCopy = CyberCore::CopyBackingStore) final;
    RefPtr<CyberCore::PixelBuffer> getPixelBuffer(const CyberCore::PixelBufferFormat& outputFormat, const CyberCore::IntRect&, const CyberCore::ImageBufferAllocator& = CyberCore::ImageBufferAllocator()) final;
    void putPixelBuffer(const CyberCore::PixelBuffer&, const CyberCore::IntRect& srcRect, const CyberCore::IntPoint& destPoint, CyberCore::AlphaPremultiplication destFormat) final;

protected:
    CGDisplayListImageBufferBackend(const Parameters&, const CyberCore::ImageBufferCreationContext&, CyberCore::RenderingMode);

    unsigned bytesPerRow() const final;

    // ImageBufferBackendSharing
    ImageBufferBackendSharing* toBackendSharing() final { return this; }

    mutable std::unique_ptr<CyberCore::GraphicsContext> m_context;
    RetainPtr<id> m_resourceCache;
    CyberCore::RenderingMode m_renderingMode;
};

class CGDisplayListAcceleratedImageBufferBackend final : public CGDisplayListImageBufferBackend {
    WTF_MAKE_ISO_ALLOCATED(CGDisplayListAcceleratedImageBufferBackend);
    WTF_MAKE_NONCOPYABLE(CGDisplayListAcceleratedImageBufferBackend);
public:
    static std::unique_ptr<CGDisplayListAcceleratedImageBufferBackend> create(const CGDisplayListImageBufferBackend::Parameters&, const CyberCore::ImageBufferCreationContext&);

protected:
    CGDisplayListAcceleratedImageBufferBackend(const Parameters&, const CyberCore::ImageBufferCreationContext&, CyberCore::RenderingMode);
};

}

#endif // ENABLE(CG_DISPLAY_LIST_BACKED_IMAGE_BUFFER)
