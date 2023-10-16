/*
 * Copyright (C) 2021-2023 Apple Inc. All rights reserved.
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

#include "RemoteDeviceProxy.h"
#include "WebGPUIdentifier.h"
#include <CyberCore/WebGPUCommandEncoder.h>

namespace CyberKit::WebGPU {

class ConvertToBackingContext;

class RemoteCommandEncoderProxy final : public CyberCore::WebGPU::CommandEncoder {
    WTF_MAKE_FAST_ALLOCATED;
public:
    static Ref<RemoteCommandEncoderProxy> create(RemoteDeviceProxy& parent, ConvertToBackingContext& convertToBackingContext, WebGPUIdentifier identifier)
    {
        return adoptRef(*new RemoteCommandEncoderProxy(parent, convertToBackingContext, identifier));
    }

    virtual ~RemoteCommandEncoderProxy();

    RemoteDeviceProxy& parent() { return m_parent; }
    RemoteGPUProxy& root() { return m_parent->root(); }

private:
    friend class DowncastConvertToBackingContext;

    RemoteCommandEncoderProxy(RemoteDeviceProxy&, ConvertToBackingContext&, WebGPUIdentifier);

    RemoteCommandEncoderProxy(const RemoteCommandEncoderProxy&) = delete;
    RemoteCommandEncoderProxy(RemoteCommandEncoderProxy&&) = delete;
    RemoteCommandEncoderProxy& operator=(const RemoteCommandEncoderProxy&) = delete;
    RemoteCommandEncoderProxy& operator=(RemoteCommandEncoderProxy&&) = delete;

    WebGPUIdentifier backing() const { return m_backing; }
    
    static inline constexpr Seconds defaultSendTimeout = 30_s;
    template<typename T>
    WARN_UNUSED_RETURN IPC::Error send(T&& message)
    {
        return root().streamClientConnection().send(WTFMove(message), backing(), defaultSendTimeout);
    }
    template<typename T>
    WARN_UNUSED_RETURN IPC::Connection::SendSyncResult<T> sendSync(T&& message)
    {
        return root().streamClientConnection().sendSync(WTFMove(message), backing(), defaultSendTimeout);
    }

    Ref<CyberCore::WebGPU::RenderPassEncoder> beginRenderPass(const CyberCore::WebGPU::RenderPassDescriptor&) final;
    Ref<CyberCore::WebGPU::ComputePassEncoder> beginComputePass(const std::optional<CyberCore::WebGPU::ComputePassDescriptor>&) final;

    void copyBufferToBuffer(
        const CyberCore::WebGPU::Buffer& source,
        CyberCore::WebGPU::Size64 sourceOffset,
        const CyberCore::WebGPU::Buffer& destination,
        CyberCore::WebGPU::Size64 destinationOffset,
        CyberCore::WebGPU::Size64) final;

    void copyBufferToTexture(
        const CyberCore::WebGPU::ImageCopyBuffer& source,
        const CyberCore::WebGPU::ImageCopyTexture& destination,
        const CyberCore::WebGPU::Extent3D& copySize) final;

    void copyTextureToBuffer(
        const CyberCore::WebGPU::ImageCopyTexture& source,
        const CyberCore::WebGPU::ImageCopyBuffer& destination,
        const CyberCore::WebGPU::Extent3D& copySize) final;

    void copyTextureToTexture(
        const CyberCore::WebGPU::ImageCopyTexture& source,
        const CyberCore::WebGPU::ImageCopyTexture& destination,
        const CyberCore::WebGPU::Extent3D& copySize) final;

    void clearBuffer(
        const CyberCore::WebGPU::Buffer&,
        CyberCore::WebGPU::Size64 offset = 0,
        std::optional<CyberCore::WebGPU::Size64> = std::nullopt) final;

    void pushDebugGroup(String&& groupLabel) final;
    void popDebugGroup() final;
    void insertDebugMarker(String&& markerLabel) final;

    void writeTimestamp(const CyberCore::WebGPU::QuerySet&, CyberCore::WebGPU::Size32 queryIndex) final;

    void resolveQuerySet(
        const CyberCore::WebGPU::QuerySet&,
        CyberCore::WebGPU::Size32 firstQuery,
        CyberCore::WebGPU::Size32 queryCount,
        const CyberCore::WebGPU::Buffer& destination,
        CyberCore::WebGPU::Size64 destinationOffset) final;

    Ref<CyberCore::WebGPU::CommandBuffer> finish(const CyberCore::WebGPU::CommandBufferDescriptor&) final;

    void setLabelInternal(const String&) final;

    WebGPUIdentifier m_backing;
    Ref<ConvertToBackingContext> m_convertToBackingContext;
    Ref<RemoteDeviceProxy> m_parent;
};

} // namespace CyberKit::WebGPU

#endif // ENABLE(GPU_PROCESS)
