/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
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

#include "RemoteImageBufferMessageHandlerProxy.h"
#include <CyberCore/ConcreteImageBuffer.h>
#include <CyberCore/DisplayListReplayer.h>

namespace CyberKit {

template<typename BackendType>
class RemoteImageBufferProxy : public CyberCore::ConcreteImageBuffer<BackendType>, public RemoteImageBufferMessageHandlerProxy, public CyberCore::DisplayList::Replayer::Delegate {
    using BaseConcreteImageBuffer = CyberCore::ConcreteImageBuffer<BackendType>;
    using BaseConcreteImageBuffer::m_backend;

public:
    static auto create(const CyberCore::FloatSize& size, float resolutionScale, CyberCore::ColorSpace colorSpace, RemoteRenderingBackendProxy& remoteRenderingBackendProxy, ImageBufferIdentifier imageBufferIdentifier)
    {
        return BaseConcreteImageBuffer::template create<RemoteImageBufferProxy>(size, resolutionScale, colorSpace, nullptr, remoteRenderingBackendProxy, imageBufferIdentifier);
    }

    RemoteImageBufferProxy(std::unique_ptr<BackendType>&& backend, RemoteRenderingBackendProxy& remoteRenderingBackendProxy, ImageBufferIdentifier imageBufferIdentifier)
        : BaseConcreteImageBuffer(WTFMove(backend))
        , RemoteImageBufferMessageHandlerProxy(remoteRenderingBackendProxy, imageBufferIdentifier)
    {
        createBackend(m_backend->logicalSize(), m_backend->backendSize(), m_backend->resolutionScale(), m_backend->colorSpace(), m_backend->createImageBufferBackendHandle());
    }

private:
    using BaseConcreteImageBuffer::flushDrawingContext;
    using BaseConcreteImageBuffer::putImageData;

    void flushDrawingContext(const CyberCore::DisplayList::DisplayList& displayList) override
    {
        if (displayList.itemCount()) {
            CyberCore::DisplayList::Replayer replayer(BaseConcreteImageBuffer::context(), displayList, this);
            replayer.replay();
        }
    }

    void flushDrawingContextAndCommit(const CyberCore::DisplayList::DisplayList& displayList, ImageBufferFlushIdentifier flushIdentifier) override
    {
        flushDrawingContext(displayList);
        m_backend->flushContext();
        commitFlushContext(flushIdentifier);
    }

    RefPtr<CyberCore::ImageData> getImageData(CyberCore::AlphaPremultiplication outputFormat, const CyberCore::IntRect& srcRect) const override
    {
        return BaseConcreteImageBuffer::getImageData(outputFormat, srcRect);
    }

    bool apply(CyberCore::DisplayList::Item& item, CyberCore::GraphicsContext&) override
    {
        if (item.type() != CyberCore::DisplayList::ItemType::PutImageData)
            return false;

        auto& putImageDataItem = static_cast<CyberCore::DisplayList::PutImageData&>(item);
        putImageData(putImageDataItem.inputFormat(), putImageDataItem.imageData(), putImageDataItem.srcRect(), putImageDataItem.destPoint(), putImageDataItem.destFormat());
        return true;
    }
};

} // namespace CyberKit

#endif // ENABLE(GPU_PROCESS)
