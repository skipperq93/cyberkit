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

#if ENABLE(GPU_PROCESS)

#include "ImageBufferBackendHandle.h"
#include "RemoteDisplayListRecorderProxy.h"
#include "RemoteSerializedImageBufferIdentifier.h"
#include "RenderingBackendIdentifier.h"
#include <CyberCore/ImageBuffer.h>
#include <CyberCore/ImageBufferBackend.h>
#include <wtf/Condition.h>
#include <wtf/Lock.h>

namespace CyberKit {

class RemoteRenderingBackendProxy;
class RemoteImageBufferProxyFlushState;

class RemoteImageBufferProxy : public CyberCore::ImageBuffer {
    friend class RemoteSerializedImageBufferProxy;
public:
    template<typename BackendType>
    static RefPtr<RemoteImageBufferProxy> create(const CyberCore::FloatSize& size, float resolutionScale, const CyberCore::DestinationColorSpace& colorSpace, CyberCore::PixelFormat pixelFormat, CyberCore::RenderingPurpose purpose, RemoteRenderingBackendProxy& remoteRenderingBackendProxy, bool avoidBackendSizeCheck = false)
    {
        auto parameters = CyberCore::ImageBufferBackend::Parameters { size, resolutionScale, colorSpace, pixelFormat, purpose };
        if (!avoidBackendSizeCheck && BackendType::calculateSafeBackendSize(parameters).isEmpty())
            return nullptr;
        auto info = populateBackendInfo<BackendType>(parameters);
        return adoptRef(new RemoteImageBufferProxy(parameters, info, remoteRenderingBackendProxy));
    }

    static RefPtr<RemoteImageBufferProxy> create(const CyberCore::ImageBufferBackend::Parameters& parameters, const CyberCore::ImageBufferBackend::Info& info, RemoteRenderingBackendProxy& remoteRenderingBackendProxy, std::unique_ptr<CyberCore::ImageBufferBackend>&& backend, CyberCore::RenderingResourceIdentifier identifier)
    {
        return adoptRef(new RemoteImageBufferProxy(parameters, info, remoteRenderingBackendProxy, WTFMove(backend), identifier));
    }

    ~RemoteImageBufferProxy();

    DisplayListRecorderFlushIdentifier lastSentFlushIdentifier() const { return m_sentFlushIdentifier; }

    CyberCore::ImageBufferBackend* ensureBackendCreated() const final;

    void clearBackend();
    void backingStoreWillChange();
    void didCreateImageBufferBackend(ImageBufferBackendHandle&&);

    std::unique_ptr<CyberCore::SerializedImageBuffer> sinkIntoSerializedImageBuffer() final;

private:
    RemoteImageBufferProxy(const CyberCore::ImageBufferBackend::Parameters&, const CyberCore::ImageBufferBackend::Info&, RemoteRenderingBackendProxy&, std::unique_ptr<CyberCore::ImageBufferBackend>&& = nullptr, CyberCore::RenderingResourceIdentifier = CyberCore::RenderingResourceIdentifier::generate());

    bool hasPendingFlush() const;

    void waitForDidFlushWithTimeout();

    RefPtr<CyberCore::NativeImage> copyNativeImage(CyberCore::BackingStoreCopy = CyberCore::CopyBackingStore) const final;
    RefPtr<CyberCore::NativeImage> copyNativeImageForDrawing(CyberCore::GraphicsContext&) const final;
    RefPtr<CyberCore::NativeImage> sinkIntoNativeImage() final;

    RefPtr<ImageBuffer> sinkIntoBufferForDifferentThread() final;
    RefPtr<ImageBuffer> cloneForDifferentThread() final;

    RefPtr<CyberCore::Image> filteredImage(CyberCore::Filter&) final;

    void drawConsuming(CyberCore::GraphicsContext& destContext, const CyberCore::FloatRect& destRect, const CyberCore::FloatRect& srcRect, const CyberCore::ImagePaintingOptions&) final;

    CyberCore::GraphicsContext& context() const final;

    RefPtr<CyberCore::PixelBuffer> getPixelBuffer(const CyberCore::PixelBufferFormat& destinationFormat, const CyberCore::IntRect& srcRect, const CyberCore::ImageBufferAllocator&) const final;
    void putPixelBuffer(const CyberCore::PixelBuffer&, const CyberCore::IntRect& srcRect, const CyberCore::IntPoint& destPoint = { }, CyberCore::AlphaPremultiplication = CyberCore::AlphaPremultiplication::Premultiplied) final;

    void convertToLuminanceMask() final;
    void transformToColorSpace(const CyberCore::DestinationColorSpace&) final;

    bool prefersPreparationForDisplay() final { return true; }
    
    void flushDrawingContext() final;
    bool flushDrawingContextAsync() final;

    std::unique_ptr<CyberCore::ThreadSafeImageBufferFlusher> createFlusher() final;
    void prepareForBackingStoreChange();

    void assertDispatcherIsCurrent() const;

    DisplayListRecorderFlushIdentifier m_sentFlushIdentifier;
    Ref<RemoteImageBufferProxyFlushState> m_flushState;
    WeakPtr<RemoteRenderingBackendProxy> m_remoteRenderingBackendProxy;
    RemoteDisplayListRecorderProxy m_remoteDisplayList;
    bool m_needsFlush { false };
};

class RemoteImageBufferProxyFlushState : public ThreadSafeRefCounted<RemoteImageBufferProxyFlushState> {
    WTF_MAKE_NONCOPYABLE(RemoteImageBufferProxyFlushState);
    WTF_MAKE_FAST_ALLOCATED;
public:
    RemoteImageBufferProxyFlushState() = default;
    void waitForDidFlushOnSecondaryThread(DisplayListRecorderFlushIdentifier);
    void markCompletedFlush(DisplayListRecorderFlushIdentifier);
    void cancel();
    DisplayListRecorderFlushIdentifier identifierForCompletedFlush() const;

private:
    mutable Lock m_lock;
    Condition m_condition;
    DisplayListRecorderFlushIdentifier m_identifier WTF_GUARDED_BY_LOCK(m_lock);
};

class RemoteSerializedImageBufferProxy : public CyberCore::SerializedImageBuffer {
    friend class RemoteRenderingBackendProxy;
public:
    ~RemoteSerializedImageBufferProxy();

    static RefPtr<CyberCore::ImageBuffer> sinkIntoImageBuffer(std::unique_ptr<RemoteSerializedImageBufferProxy>, RemoteRenderingBackendProxy&);

    CyberCore::RenderingResourceIdentifier renderingResourceIdentifier() { return m_renderingResourceIdentifier; }

    RemoteSerializedImageBufferProxy(const CyberCore::ImageBufferBackend::Parameters&, const CyberCore::ImageBufferBackend::Info&, const CyberCore::RenderingResourceIdentifier&, RemoteRenderingBackendProxy&);

    size_t memoryCost() final
    {
        return m_info.memoryCost;
    }

private:
    RefPtr<CyberCore::ImageBuffer> sinkIntoImageBuffer() final
    {
        ASSERT_NOT_REACHED();
        return nullptr;
    }

    bool isRemoteSerializedImageBufferProxy() const final { return true; }

    RemoteSerializedImageBufferReferenceTracker m_referenceTracker;

    CyberCore::ImageBufferBackend::Parameters m_parameters;
    CyberCore::ImageBufferBackend::Info m_info;

    CyberCore::RenderingResourceIdentifier m_renderingResourceIdentifier;
    RefPtr<IPC::Connection> m_connection;
};

} // namespace CyberKit

SPECIALIZE_TYPE_TRAITS_BEGIN(CyberKit::RemoteSerializedImageBufferProxy)
    static bool isType(const CyberCore::SerializedImageBuffer& buffer) { return buffer.isRemoteSerializedImageBufferProxy(); }
SPECIALIZE_TYPE_TRAITS_END()


#endif // ENABLE(GPU_PROCESS)
