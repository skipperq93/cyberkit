/*
 * Copyright (C) 2020-2023 Apple Inc. All rights reserved.
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

#if ENABLE(GPU_PROCESS)

#include "Connection.h"
#include "ImageBufferBackendHandle.h"
#include "MarkSurfacesAsVolatileRequestIdentifier.h"
#include "MessageReceiver.h"
#include "MessageSender.h"
#include "QualifiedRenderingResourceIdentifier.h"
#include "RemoteRenderingBackendCreationParameters.h"
#include "RemoteResourceCache.h"
#include "RemoteSerializedImageBufferIdentifier.h"
#include "RenderingBackendIdentifier.h"
#include "RenderingUpdateID.h"
#include "ScopedActiveMessageReceiveQueue.h"
#include "ScopedRenderingResourcesRequest.h"
#include "ShapeDetectionIdentifier.h"
#include "StreamConnectionWorkQueue.h"
#include "StreamMessageReceiver.h"
#include "StreamServerConnection.h"
#include <CyberCore/MediaPlayerIdentifier.h>
#include <CyberCore/ProcessIdentity.h>
#include <wtf/HashMap.h>

namespace WTF {
enum class Critical : bool;
enum class Synchronous : bool;
}

namespace CyberCore {
class DestinationColorSpace;
class FloatSize;
class MediaPlayer;
class NativeImage;
enum class RenderingMode : bool;

namespace ShapeDetection {
struct BarcodeDetectorOptions;
enum class BarcodeFormat : uint8_t;
struct FaceDetectorOptions;
}

}

namespace CyberKit {

class GPUConnectionToWebProcess;
class RemoteDisplayListRecorder;
class RemoteImageBuffer;
struct BufferIdentifierSet;
struct PrepareBackingStoreBuffersInputData;
struct PrepareBackingStoreBuffersOutputData;
struct RemoteRenderingBackendCreationParameters;
enum class SwapBuffersDisplayRequirement : uint8_t;

namespace ShapeDetection {
class ObjectHeap;
}

class RemoteRenderingBackend : private IPC::MessageSender, public IPC::StreamMessageReceiver {
public:
    static Ref<RemoteRenderingBackend> create(GPUConnectionToWebProcess&, RemoteRenderingBackendCreationParameters&&, Ref<IPC::StreamServerConnection>&&);
    virtual ~RemoteRenderingBackend();
    void stopListeningForIPC();

    RemoteResourceCache& remoteResourceCache() { return m_remoteResourceCache; }

    void didReceiveStreamMessage(IPC::StreamServerConnection&, IPC::Decoder&) final;

    void didCreateImageBuffer(Ref<RemoteImageBuffer>, QualifiedRenderingResourceIdentifier);

    // Runs Function in RemoteRenderingBackend task queue.
    void dispatch(Function<void()>&&);

    IPC::StreamServerConnection& streamConnection() const { return m_streamConnection.get(); }

    void lowMemoryHandler(WTF::Critical, WTF::Synchronous);

#if HAVE(IOSURFACE)
    CyberCore::IOSurfacePool& ioSurfacePool() const { return m_ioSurfacePool; }
#endif

    const CyberCore::ProcessIdentity& resourceOwner() const { return m_resourceOwner; }

    GPUConnectionToWebProcess& gpuConnectionToWebProcess() { return m_gpuConnectionToWebProcess.get(); }

private:
    RemoteRenderingBackend(GPUConnectionToWebProcess&, RemoteRenderingBackendCreationParameters&&, Ref<IPC::StreamServerConnection>&&);
    void startListeningForIPC();
    void workQueueInitialize();
    void workQueueUninitialize();

    // IPC::MessageSender.
    IPC::Connection* messageSenderConnection() const override;
    uint64_t messageSenderDestinationID() const override;

    // Messages to be received.
    void createImageBuffer(const CyberCore::FloatSize& logicalSize, CyberCore::RenderingMode, CyberCore::RenderingPurpose, float resolutionScale, const CyberCore::DestinationColorSpace&, CyberCore::PixelFormat, CyberCore::RenderingResourceIdentifier);
    void moveToSerializedBuffer(CyberCore::RenderingResourceIdentifier);
    void moveToImageBuffer(CyberCore::RenderingResourceIdentifier);
    void getPixelBufferForImageBuffer(CyberCore::RenderingResourceIdentifier, CyberCore::PixelBufferFormat&&, CyberCore::IntRect&& srcRect, CompletionHandler<void()>&&);
    void getPixelBufferForImageBufferWithNewMemory(CyberCore::RenderingResourceIdentifier, SharedMemory::Handle&&, CyberCore::PixelBufferFormat&& destinationFormat, CyberCore::IntRect&& srcRect, CompletionHandler<void()>&&);
    void destroyGetPixelBufferSharedMemory();
    void putPixelBufferForImageBuffer(CyberCore::RenderingResourceIdentifier, Ref<CyberCore::PixelBuffer>&&, CyberCore::IntRect&& srcRect, CyberCore::IntPoint&& destPoint, CyberCore::AlphaPremultiplication destFormat);
    void getShareableBitmapForImageBuffer(CyberCore::RenderingResourceIdentifier, CyberCore::PreserveResolution, CompletionHandler<void(ShareableBitmap::Handle&&)>&&);
    void getFilteredImageForImageBuffer(CyberCore::RenderingResourceIdentifier, Ref<CyberCore::Filter>, CompletionHandler<void(ShareableBitmap::Handle&&)>&&);
    void cacheNativeImage(ShareableBitmap::Handle&&, CyberCore::RenderingResourceIdentifier);
    void cacheDecomposedGlyphs(Ref<CyberCore::DecomposedGlyphs>&&);
    void cacheGradient(Ref<CyberCore::Gradient>&&, CyberCore::RenderingResourceIdentifier);
    void cacheFilter(Ref<CyberCore::Filter>&&, CyberCore::RenderingResourceIdentifier);
    void cacheFont(const CyberCore::Font::Attributes&, CyberCore::FontPlatformData::Attributes, std::optional<CyberCore::RenderingResourceIdentifier>);
    void cacheFontCustomPlatformData(Ref<CyberCore::FontCustomPlatformData>&&);
    void releaseAllResources();
    void releaseAllImageResources();
    void releaseRenderingResource(CyberCore::RenderingResourceIdentifier);
    void finalizeRenderingUpdate(RenderingUpdateID);
    void markSurfacesVolatile(MarkSurfacesAsVolatileRequestIdentifier, const Vector<CyberCore::RenderingResourceIdentifier>&);
#if PLATFORM(COCOA)
    void prepareBuffersForDisplay(Vector<PrepareBackingStoreBuffersInputData> swapBuffersInput, CompletionHandler<void(Vector<PrepareBackingStoreBuffersOutputData>&&)>&&);
#endif
    
    // Received messages translated to use QualifiedRenderingResourceIdentifier.
    void getShareableBitmapForImageBufferWithQualifiedIdentifier(QualifiedRenderingResourceIdentifier, CyberCore::PreserveResolution, CompletionHandler<void(ShareableBitmap::Handle&&)>&&);
    void cacheNativeImageWithQualifiedIdentifier(ShareableBitmap::Handle&&, QualifiedRenderingResourceIdentifier);
    void cacheDecomposedGlyphsWithQualifiedIdentifier(Ref<CyberCore::DecomposedGlyphs>&&, QualifiedRenderingResourceIdentifier);
    void cacheGradientWithQualifiedIdentifier(Ref<CyberCore::Gradient>&&, QualifiedRenderingResourceIdentifier);
    void cacheFilterWithQualifiedIdentifier(Ref<CyberCore::Filter>&&, QualifiedRenderingResourceIdentifier);
    void releaseRenderingResourceWithQualifiedIdentifier(QualifiedRenderingResourceIdentifier);
    void cacheFontWithQualifiedIdentifier(Ref<CyberCore::Font>&&, QualifiedRenderingResourceIdentifier);
    void cacheFontCustomPlatformDataWithQualifiedIdentifier(Ref<CyberCore::FontCustomPlatformData>&&, QualifiedRenderingResourceIdentifier);

#if PLATFORM(COCOA)
    void prepareLayerBuffersForDisplay(const PrepareBackingStoreBuffersInputData&, PrepareBackingStoreBuffersOutputData&);
#endif

    void createRemoteBarcodeDetector(ShapeDetectionIdentifier, const CyberCore::ShapeDetection::BarcodeDetectorOptions&);
    void releaseRemoteBarcodeDetector(ShapeDetectionIdentifier);
    void getRemoteBarcodeDetectorSupportedFormats(CompletionHandler<void(Vector<CyberCore::ShapeDetection::BarcodeFormat>&&)>&&);
    void createRemoteFaceDetector(ShapeDetectionIdentifier, const CyberCore::ShapeDetection::FaceDetectorOptions&);
    void releaseRemoteFaceDetector(ShapeDetectionIdentifier);
    void createRemoteTextDetector(ShapeDetectionIdentifier);
    void releaseRemoteTextDetector(ShapeDetectionIdentifier);

    IPC::StreamConnectionWorkQueue& workQueue() const { return m_workQueue; }

    Ref<IPC::StreamConnectionWorkQueue> m_workQueue;
    Ref<IPC::StreamServerConnection> m_streamConnection;
    RemoteResourceCache m_remoteResourceCache;
    Ref<GPUConnectionToWebProcess> m_gpuConnectionToWebProcess;
    CyberCore::ProcessIdentity m_resourceOwner;
    RenderingBackendIdentifier m_renderingBackendIdentifier;
    RefPtr<SharedMemory> m_getPixelBufferSharedMemory;
#if HAVE(IOSURFACE)
    Ref<CyberCore::IOSurfacePool> m_ioSurfacePool;
#endif

    HashMap<QualifiedRenderingResourceIdentifier, IPC::ScopedActiveMessageReceiveQueue<RemoteDisplayListRecorder>> m_remoteDisplayLists WTF_GUARDED_BY_CAPABILITY(workQueue());
    Ref<ShapeDetection::ObjectHeap> m_shapeDetectionObjectHeap;
};

} // namespace CyberKit

#endif // ENABLE(GPU_PROCESS)
