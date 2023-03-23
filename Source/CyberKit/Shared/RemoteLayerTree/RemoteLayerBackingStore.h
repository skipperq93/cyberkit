/*
 * Copyright (C) 2013-2018 Apple Inc. All rights reserved.
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

#include "ImageBufferBackendHandle.h"
#include <CyberCore/FloatRect.h>
#include <CyberCore/ImageBuffer.h>
#include <CyberCore/Region.h>
#include <wtf/MachSendRight.h>
#include <wtf/MonotonicTime.h>

OBJC_CLASS CALayer;

// FIXME: Make PlatformCALayerRemote.cpp Objective-C so we can include WebLayer.h here and share the typedef.
namespace CyberCore {
class NativeImage;
class ThreadSafeImageBufferFlusher;
typedef Vector<CyberCore::FloatRect, 5> RepaintRectList;
}

namespace CyberKit {

class PlatformCALayerRemote;
class RemoteLayerBackingStoreCollection;
enum class SwapBuffersDisplayRequirement : uint8_t;

#if ENABLE(CG_DISPLAY_LIST_BACKED_IMAGE_BUFFER)
using UseCGDisplayListImageCache = CyberCore::ImageBufferCreationContext::UseCGDisplayListImageCache;
#endif

class RemoteLayerBackingStore {
    WTF_MAKE_NONCOPYABLE(RemoteLayerBackingStore);
    WTF_MAKE_FAST_ALLOCATED;
public:
    RemoteLayerBackingStore(PlatformCALayerRemote*);
    ~RemoteLayerBackingStore();

    enum class Type : bool {
        IOSurface,
        Bitmap
    };

#if ENABLE(CG_DISPLAY_LIST_BACKED_IMAGE_BUFFER)
    enum class IncludeDisplayList : bool { No, Yes };
#endif

    struct Parameters {
        Type type { Type::Bitmap };
        CyberCore::FloatSize size;
        CyberCore::DestinationColorSpace colorSpace { CyberCore::DestinationColorSpace::SRGB() };
        float scale { 1.0f };
        bool deepColor { false };
        bool isOpaque { false };

#if ENABLE(CG_DISPLAY_LIST_BACKED_IMAGE_BUFFER)
        IncludeDisplayList includeDisplayList { IncludeDisplayList::No };
        UseCGDisplayListImageCache useCGDisplayListImageCache { UseCGDisplayListImageCache::No };
#endif
        
        void encode(IPC::Encoder&) const;
        static WARN_UNUSED_RETURN bool decode(IPC::Decoder&, Parameters&);

        bool operator==(const Parameters& other) const
        {
            return (type == other.type
                && size == other.size
                && colorSpace == other.colorSpace
                && scale == other.scale
                && deepColor == other.deepColor
                && isOpaque == other.isOpaque
#if ENABLE(CG_DISPLAY_LIST_BACKED_IMAGE_BUFFER)
                && includeDisplayList == other.includeDisplayList
                && useCGDisplayListImageCache == other.useCGDisplayListImageCache
#endif
                );
        }
    };

    void ensureBackingStore(const Parameters&);

    void setNeedsDisplay(const CyberCore::IntRect);
    void setNeedsDisplay();

    void setContents(WTF::MachSendRight&& surfaceHandle);

    // Returns true if we need to encode the buffer.
    bool layerWillBeDisplayed();
    bool needsDisplay() const;

    bool performDelegatedLayerDisplay();
    void prepareToDisplay();
    void paintContents();

    CyberCore::FloatSize size() const { return m_parameters.size; }
    float scale() const { return m_parameters.scale; }
    bool usesDeepColorBackingStore() const;
    CyberCore::DestinationColorSpace colorSpace() const { return m_parameters.colorSpace; }
    CyberCore::PixelFormat pixelFormat() const;
    Type type() const { return m_parameters.type; }
    bool isOpaque() const { return m_parameters.isOpaque; }
    unsigned bytesPerPixel() const;

    PlatformCALayerRemote* layer() const { return m_layer; }

    enum class LayerContentsType { IOSurface, CAMachPort };
    void applyBackingStoreToLayer(CALayer *, LayerContentsType, bool replayCGDisplayListsIntoBackingStore);

    void encode(IPC::Encoder&) const;
    static WARN_UNUSED_RETURN bool decode(IPC::Decoder&, RemoteLayerBackingStore&);

    void enumerateRectsBeingDrawn(CyberCore::GraphicsContext&, void (^)(CyberCore::FloatRect));

    bool hasFrontBuffer() const
    {
        return m_contentsBufferHandle || !!m_frontBuffer.imageBuffer;
    }

    // Just for RemoteBackingStoreCollection.
    void applySwappedBuffers(RefPtr<CyberCore::ImageBuffer>&& front, RefPtr<CyberCore::ImageBuffer>&& back, RefPtr<CyberCore::ImageBuffer>&& secondaryBack, SwapBuffersDisplayRequirement);
    CyberCore::SetNonVolatileResult swapToValidFrontBuffer();

    Vector<std::unique_ptr<CyberCore::ThreadSafeImageBufferFlusher>> takePendingFlushers();

    enum class BufferType {
        Front,
        Back,
        SecondaryBack
    };

    RefPtr<CyberCore::ImageBuffer> bufferForType(BufferType) const;

    // Returns true if it was able to fulfill the request. This can fail when trying to mark an in-use surface as volatile.
    bool setBufferVolatile(BufferType);
    CyberCore::SetNonVolatileResult setFrontBufferNonVolatile();

    bool hasEmptyDirtyRegion() const { return m_dirtyRegion.isEmpty() || m_parameters.size.isEmpty(); }
    bool supportsPartialRepaint() const;

    MonotonicTime lastDisplayTime() const { return m_lastDisplayTime; }

    void clearBackingStore();

    static RetainPtr<id> layerContentsBufferFromBackendHandle(ImageBufferBackendHandle&&, LayerContentsType);

private:
    RemoteLayerBackingStoreCollection* backingStoreCollection() const;

    void drawInContext(CyberCore::GraphicsContext&, WTF::Function<void()>&& additionalContextSetupCallback = nullptr);

    struct Buffer {
        RefPtr<CyberCore::ImageBuffer> imageBuffer;

        explicit operator bool() const
        {
            return !!imageBuffer;
        }

        void discard();
    };

    bool setBufferVolatile(Buffer&);
    CyberCore::SetNonVolatileResult setBufferNonVolatile(Buffer&);
    
    SwapBuffersDisplayRequirement prepareBuffers();
    void ensureFrontBuffer();
    void dirtyRepaintCounterIfNecessary();

    PlatformCALayerRemote* m_layer;

    Parameters m_parameters;

    CyberCore::Region m_dirtyRegion;

    // Used in the WebContent Process.
    Buffer m_frontBuffer;
    Buffer m_backBuffer;
    Buffer m_secondaryBackBuffer;

    // Used in the UI Process.
    std::optional<ImageBufferBackendHandle> m_bufferHandle;
    // FIXME: This should be removed and m_bufferHandle should be used to ref the buffer once ShareableBitmapHandle
    // can be encoded multiple times. http://webkit.org/b/234169
    std::optional<MachSendRight> m_contentsBufferHandle;

#if ENABLE(CG_DISPLAY_LIST_BACKED_IMAGE_BUFFER)
    RefPtr<CyberCore::ImageBuffer> m_displayListBuffer;
    std::optional<ImageBufferBackendHandle> m_displayListBufferHandle;
#endif

    Vector<std::unique_ptr<CyberCore::ThreadSafeImageBufferFlusher>> m_frontBufferFlushers;

    CyberCore::RepaintRectList m_paintingRects;

    MonotonicTime m_lastDisplayTime;
};

} // namespace CyberKit
