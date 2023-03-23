/*
 * Copyright (C) 2020-2022 Apple Inc. All rights reserved.
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
#include "DisplayListRecorderFlushIdentifier.h"
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
#include "ScopedRenderingResourcesRequest.h"
#include "StreamConnectionWorkQueue.h"
#include "StreamMessageReceiver.h"
#include "StreamServerConnection.h"
#include <CyberCore/MediaPlayerIdentifier.h>
#include <CyberCore/ProcessIdentity.h>

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
}

namespace CyberKit {

class GPUConnectionToWebProcess;
class RemoteDisplayListRecorder;
struct BufferIdentifierSet;
struct PrepareBackingStoreBuffersInputData;
struct PrepareBackingStoreBuffersOutputData;
struct RemoteRenderingBackendCreationParameters;
enum class SwapBuffersDisplayRequirement : uint8_t;

class RemoteRenderingBackend : private IPC::MessageSender, public IPC::StreamMessageReceiver {
public:
    static Ref<RemoteRenderingBackend> create(GPUConnectionToWebProcess&, RemoteRenderingBackendCreationParameters&&, IPC::StreamServerConnection::Handle&&);
    virtual ~RemoteRenderingBackend();
    void stopListeningForIPC();

    RemoteResourceCache& remoteResourceCache() { return m_remoteResourceCache; }

    void didReceiveStreamMessage(IPC::StreamServerConnection&, IPC::Decoder&) final;

    // Messages to be sent.
    void didCreateImageBufferBackend(ImageBufferBackendHandle, QualifiedRenderingResourceIdentifier, RemoteDisplayListRecorder&);
    void didFlush(DisplayListRecorderFlushIdentifier);

    // Runs Function in RemoteRenderingBackend task queue.
    void dispatch(Function<void()>&&);

    IPC::StreamServerConnection& streamConnection() const { return m_streamConnection.get(); }
#if ENABLE(VIDEO)
    void performWithMediaPlayerOnMainThread(CyberCore::MediaPlayerIdentifier, Function<void(CyberCore::MediaPlayer&)>&&);
#endif

    void lowMemoryHandler(WTF::Critical, WTF::Synchronous);

#if HAVE(IOSURFACE)
    CyberCore::IOSurfacePool& ioSurfacePool() const { return m_ioSurfacePool; }
#endif

    const CyberCore::ProcessIdentity& resourceOwner() const { return m_resourceOwner; }

    void releaseResource(CyberCore::RenderingResourceIdentifier);

    GPUConnectionToWebProcess& gpuConnectionToWebProcess() { return m_gpuConnectionToWebProcess.get(); }

private:
    RemoteRenderingBackend(GPUConnectionToWebProcess&, RemoteRenderingBackendCreationParameters&&, IPC::StreamServerConnection::Handle&&);
    void startListeningForIPC();

    // IPC::MessageSender.
    IPC::Connection* messageSenderConnection() const override;
    uint64_t messageSenderDestinationID() const override;

    // Messages to be received.
    void createImageBuffer(const CyberCore::FloatSize& logicalSize, CyberCore::RenderingMode, CyberCore::RenderingPurpose, float resolutionScale, const CyberCore::DestinationColorSpace&, CyberCore::PixelFormat, CyberCore::RenderingResourceIdentifier);
    void moveToSerializedBuffer(CyberCore::RenderingResourceIdentifier, RemoteSerializedImageBufferWriteReference&&);
    void moveToImageBuffer(RemoteSerializedImageBufferWriteReference&&, CyberCore::RenderingResourceIdentifier);
    void getPixelBufferForImageBuffer(CyberCore::RenderingResourceIdentifier, CyberCore::PixelBufferFormat&&, CyberCore::IntRect&& srcRect, CompletionHandler<void()>&&);
    void getPixelBufferForImageBufferWithNewMemory(CyberCore::RenderingResourceIdentifier, SharedMemory::Handle&&, CyberCore::PixelBufferFormat&& destinationFormat, CyberCore::IntRect&& srcRect, CompletionHandler<void()>&&);
    void destroyGetPixelBufferSharedMemory();
    void putPixelBufferForImageBuffer(CyberCore::RenderingResourceIdentifier, Ref<CyberCore::PixelBuffer>&&, CyberCore::IntRect&& srcRect, CyberCore::IntPoint&& destPoint, CyberCore::AlphaPremultiplication destFormat);
    void getShareableBitmapForImageBuffer(CyberCore::RenderingResourceIdentifier, CyberCore::PreserveResolution, CompletionHandler<void(ShareableBitmapHandle&&)>&&);
    void getFilteredImageForImageBuffer(CyberCore::RenderingResourceIdentifier, Ref<CyberCore::Filter>, CompletionHandler<void(ShareableBitmapHandle&&)>&&);
    void cacheNativeImage(const ShareableBitmapHandle&, CyberCore::RenderingResourceIdentifier);
    void cacheDecomposedGlyphs(Ref<CyberCore::DecomposedGlyphs>&&);
    void cacheFont(Ref<CyberCore::Font>&&);
    void releaseAllResources();
    void finalizeRenderingUpdate(RenderingUpdateID);
    void markSurfacesVolatile(MarkSurfacesAsVolatileRequestIdentifier, const Vector<CyberCore::RenderingResourceIdentifier>&);
    void prepareBuffersForDisplay(Vector<PrepareBackingStoreBuffersInputData> swapBuffersInput, CompletionHandler<void(const Vector<PrepareBackingStoreBuffersOutputData>&)>&&);

    // Received messages translated to use QualifiedRenderingResourceIdentifier.
    void createImageBufferWithQualifiedIdentifier(const CyberCore::FloatSize& logicalSize, CyberCore::RenderingMode, CyberCore::RenderingPurpose, float resolutionScale, const CyberCore::DestinationColorSpace&, CyberCore::PixelFormat, QualifiedRenderingResourceIdentifier);
    void getShareableBitmapForImageBufferWithQualifiedIdentifier(QualifiedRenderingResourceIdentifier, CyberCore::PreserveResolution, CompletionHandler<void(ShareableBitmapHandle&&)>&&);
    void cacheNativeImageWithQualifiedIdentifier(const ShareableBitmapHandle&, QualifiedRenderingResourceIdentifier);
    void cacheDecomposedGlyphsWithQualifiedIdentifier(Ref<CyberCore::DecomposedGlyphs>&&, QualifiedRenderingResourceIdentifier);
    void releaseResourceWithQualifiedIdentifier(QualifiedRenderingResourceIdentifier);
    void cacheFontWithQualifiedIdentifier(Ref<CyberCore::Font>&&, QualifiedRenderingResourceIdentifier);

    void prepareLayerBuffersForDisplay(const PrepareBackingStoreBuffersInputData&, PrepareBackingStoreBuffersOutputData&);

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

    Lock m_remoteDisplayListsLock;
    bool m_canRegisterRemoteDisplayLists WTF_GUARDED_BY_LOCK(m_remoteDisplayListsLock) { false };
    HashMap<QualifiedRenderingResourceIdentifier, Ref<RemoteDisplayListRecorder>> m_remoteDisplayLists WTF_GUARDED_BY_CAPABILITY(m_remoteDisplayListsLock);
};

} // namespace CyberKit

#endif // ENABLE(GPU_PROCESS)
