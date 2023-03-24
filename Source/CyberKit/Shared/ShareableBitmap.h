/*
 * Copyright (C) 2010-2022 Apple Inc. All rights reserved.
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

#include "SharedMemory.h"
#include <CyberCore/CopyImageOptions.h>
#include <CyberCore/DestinationColorSpace.h>
#include <CyberCore/IntRect.h>
#include <CyberCore/PlatformImage.h>
#include <wtf/ArgumentCoder.h>
#include <wtf/RefPtr.h>
#include <wtf/ThreadSafeRefCounted.h>

namespace CyberCore {
class Image;
class GraphicsContext;
}

namespace CyberKit {

struct ShareableBitmapConfiguration {
    std::optional<CyberCore::DestinationColorSpace> colorSpace;
    bool isOpaque { false };
};

class ShareableBitmapHandle  {
    WTF_MAKE_NONCOPYABLE(ShareableBitmapHandle);
public:
    ShareableBitmapHandle();
    ShareableBitmapHandle(ShareableBitmapHandle&&) = default;
    ShareableBitmapHandle(SharedMemory::Handle&&, const CyberCore::IntSize&, const ShareableBitmapConfiguration&);

    ShareableBitmapHandle& operator=(ShareableBitmapHandle&&) = default;

    bool isNull() const { return m_handle.isNull(); }

    SharedMemory::Handle& handle() { return m_handle; }

    // Take ownership of the memory for process memory accounting purposes.
    void takeOwnershipOfMemory(MemoryLedger) const;

    void clear();
private:
    friend struct IPC::ArgumentCoder<ShareableBitmapHandle, void>;
    friend class ShareableBitmap;

    mutable SharedMemory::Handle m_handle;
    CyberCore::IntSize m_size;
    ShareableBitmapConfiguration m_configuration;
};

class ShareableBitmap : public ThreadSafeRefCounted<ShareableBitmap> {
public:

    static void validateConfiguration(ShareableBitmapConfiguration&);
    static CheckedUint32 numBytesForSize(CyberCore::IntSize, const ShareableBitmapConfiguration&);
    static CheckedUint32 calculateBytesPerRow(CyberCore::IntSize, const ShareableBitmapConfiguration&);
    static CheckedUint32 calculateBytesPerPixel(const ShareableBitmapConfiguration&);

    // Create a shareable bitmap whose backing memory can be shared with another process.
    static RefPtr<ShareableBitmap> create(const CyberCore::IntSize&, ShareableBitmapConfiguration);

    // Create a shareable bitmap from an already existing shared memory block.
    static RefPtr<ShareableBitmap> create(const CyberCore::IntSize&, ShareableBitmapConfiguration, Ref<SharedMemory>&&);

    // Create a shareable bitmap from a handle.
    static RefPtr<ShareableBitmap> create(const ShareableBitmapHandle&, SharedMemory::Protection = SharedMemory::Protection::ReadWrite);
    
    // Create a shareable bitmap from a ReadOnly handle.
    static std::optional<Ref<ShareableBitmap>> createReadOnly(const std::optional<ShareableBitmapHandle>&);

    std::optional<ShareableBitmapHandle> createHandle(SharedMemory::Protection = SharedMemory::Protection::ReadWrite) const;
    
    // Create a ReadOnly handle.
    std::optional<ShareableBitmapHandle> createReadOnlyHandle() const;

    const CyberCore::IntSize& size() const { return m_size; }
    CyberCore::IntRect bounds() const { return CyberCore::IntRect(CyberCore::IntPoint(), size()); }

    void* data() const;
    size_t bytesPerRow() const { return calculateBytesPerRow(m_size, m_configuration); }
    size_t sizeInBytes() const { return numBytesForSize(m_size, m_configuration); }

    // Create a graphics context that can be used to paint into the backing store.
    std::unique_ptr<CyberCore::GraphicsContext> createGraphicsContext();

    // Paint the backing store into the given context.
    void paint(CyberCore::GraphicsContext&, const CyberCore::IntPoint& destination, const CyberCore::IntRect& source);
    void paint(CyberCore::GraphicsContext&, float scaleFactor, const CyberCore::IntPoint& destination, const CyberCore::IntRect& source);

    // This creates a bitmap image that directly references the shared bitmap data.
    // This is only safe to use when we know that the contents of the shareable bitmap won't change.
    RefPtr<CyberCore::Image> createImage();

#if USE(CG)
    // This creates a copied CGImageRef (most likely a copy-on-write) of the shareable bitmap.
    RetainPtr<CGImageRef> makeCGImageCopy();

    // This creates a CGImageRef that directly references the shared bitmap data.
    // This is only safe to use when we know that the contents of the shareable bitmap won't change.
    RetainPtr<CGImageRef> makeCGImage(CyberCore::ShouldInterpolate = CyberCore::ShouldInterpolate::No);

    CyberCore::PlatformImagePtr createPlatformImage(CyberCore::BackingStoreCopy = CyberCore::CopyBackingStore, CyberCore::ShouldInterpolate = CyberCore::ShouldInterpolate::No);
#elif USE(CAIRO)
    // This creates a BitmapImage that directly references the shared bitmap data.
    // This is only safe to use when we know that the contents of the shareable bitmap won't change.
    RefPtr<cairo_surface_t> createPersistentCairoSurface();
    RefPtr<cairo_surface_t> createCairoSurface();

    CyberCore::PlatformImagePtr createPlatformImage(CyberCore::BackingStoreCopy = CyberCore::CopyBackingStore, CyberCore::ShouldInterpolate = CyberCore::ShouldInterpolate::No) { return createCairoSurface(); }
#endif

private:
    ShareableBitmap(const CyberCore::IntSize&, ShareableBitmapConfiguration, Ref<SharedMemory>&&);

#if USE(CG)
    RetainPtr<CGImageRef> createCGImage(CGDataProviderRef, CyberCore::ShouldInterpolate) const;
    static void releaseBitmapContextData(void* typelessBitmap, void* typelessData);
#endif

#if USE(CAIRO)
    static void releaseSurfaceData(void* typelessBitmap);
#endif

    CyberCore::IntSize m_size;
    ShareableBitmapConfiguration m_configuration;

#if USE(CG)
    bool m_releaseBitmapContextDataCalled { false };
#endif

    Ref<SharedMemory> m_sharedMemory;
};

} // namespace CyberKit

