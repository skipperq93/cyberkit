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

#include "RemoteAdapterProxy.h"
#include "WebGPUIdentifier.h"
#include <CyberCore/WebGPUCommandEncoderDescriptor.h>
#include <CyberCore/WebGPUDevice.h>
#include <wtf/Deque.h>

namespace CyberKit::WebGPU {

class ConvertToBackingContext;
class RemoteQueueProxy;

class RemoteDeviceProxy final : public CyberCore::WebGPU::Device {
    WTF_MAKE_FAST_ALLOCATED;
public:
    static Ref<RemoteDeviceProxy> create(Ref<CyberCore::WebGPU::SupportedFeatures>&& features, Ref<CyberCore::WebGPU::SupportedLimits>&& limits, RemoteAdapterProxy& parent, ConvertToBackingContext& convertToBackingContext, WebGPUIdentifier identifier, WebGPUIdentifier queueIdentifier)
    {
        return adoptRef(*new RemoteDeviceProxy(WTFMove(features), WTFMove(limits), parent, convertToBackingContext, identifier, queueIdentifier));
    }

    virtual ~RemoteDeviceProxy();

    RemoteAdapterProxy& parent() { return m_parent; }
    RemoteGPUProxy& root() { return m_parent->root(); }

private:
    friend class DowncastConvertToBackingContext;

    RemoteDeviceProxy(Ref<CyberCore::WebGPU::SupportedFeatures>&&, Ref<CyberCore::WebGPU::SupportedLimits>&&, RemoteAdapterProxy&, ConvertToBackingContext&, WebGPUIdentifier, WebGPUIdentifier queueIdentifier);

    RemoteDeviceProxy(const RemoteDeviceProxy&) = delete;
    RemoteDeviceProxy(RemoteDeviceProxy&&) = delete;
    RemoteDeviceProxy& operator=(const RemoteDeviceProxy&) = delete;
    RemoteDeviceProxy& operator=(RemoteDeviceProxy&&) = delete;

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
    template<typename T, typename C>
    WARN_UNUSED_RETURN IPC::StreamClientConnection::AsyncReplyID sendWithAsyncReply(T&& message, C&& completionHandler)
    {
        return root().streamClientConnection().sendWithAsyncReply(WTFMove(message), completionHandler, backing(), defaultSendTimeout);
    }

    Ref<CyberCore::WebGPU::Queue> queue() final;

    void destroy() final;

    Ref<CyberCore::WebGPU::Buffer> createBuffer(const CyberCore::WebGPU::BufferDescriptor&) final;
    Ref<CyberCore::WebGPU::Texture> createTexture(const CyberCore::WebGPU::TextureDescriptor&) final;
    Ref<CyberCore::WebGPU::Sampler> createSampler(const CyberCore::WebGPU::SamplerDescriptor&) final;
    Ref<CyberCore::WebGPU::ExternalTexture> importExternalTexture(const CyberCore::WebGPU::ExternalTextureDescriptor&) final;

    Ref<CyberCore::WebGPU::BindGroupLayout> createBindGroupLayout(const CyberCore::WebGPU::BindGroupLayoutDescriptor&) final;
    Ref<CyberCore::WebGPU::PipelineLayout> createPipelineLayout(const CyberCore::WebGPU::PipelineLayoutDescriptor&) final;
    Ref<CyberCore::WebGPU::BindGroup> createBindGroup(const CyberCore::WebGPU::BindGroupDescriptor&) final;

    Ref<CyberCore::WebGPU::ShaderModule> createShaderModule(const CyberCore::WebGPU::ShaderModuleDescriptor&) final;
    Ref<CyberCore::WebGPU::ComputePipeline> createComputePipeline(const CyberCore::WebGPU::ComputePipelineDescriptor&) final;
    Ref<CyberCore::WebGPU::RenderPipeline> createRenderPipeline(const CyberCore::WebGPU::RenderPipelineDescriptor&) final;
    void createComputePipelineAsync(const CyberCore::WebGPU::ComputePipelineDescriptor&, CompletionHandler<void(RefPtr<CyberCore::WebGPU::ComputePipeline>&&)>&&) final;
    void createRenderPipelineAsync(const CyberCore::WebGPU::RenderPipelineDescriptor&, CompletionHandler<void(RefPtr<CyberCore::WebGPU::RenderPipeline>&&)>&&) final;

    Ref<CyberCore::WebGPU::CommandEncoder> createCommandEncoder(const std::optional<CyberCore::WebGPU::CommandEncoderDescriptor>&) final;
    Ref<CyberCore::WebGPU::RenderBundleEncoder> createRenderBundleEncoder(const CyberCore::WebGPU::RenderBundleEncoderDescriptor&) final;

    Ref<CyberCore::WebGPU::QuerySet> createQuerySet(const CyberCore::WebGPU::QuerySetDescriptor&) final;

    void pushErrorScope(CyberCore::WebGPU::ErrorFilter) final;
    void popErrorScope(CompletionHandler<void(std::optional<CyberCore::WebGPU::Error>&&)>&&) final;

    void setLabelInternal(const String&) final;

    Deque<CompletionHandler<void(Ref<CyberCore::WebGPU::ComputePipeline>&&)>> m_createComputePipelineAsyncCallbacks;
    Deque<CompletionHandler<void(Ref<CyberCore::WebGPU::RenderPipeline>&&)>> m_createRenderPipelineAsyncCallbacks;
    Deque<CompletionHandler<void(std::optional<CyberCore::WebGPU::Error>&&)>> m_popErrorScopeCallbacks;

    WebGPUIdentifier m_backing;
    WebGPUIdentifier m_queueBacking;
    Ref<ConvertToBackingContext> m_convertToBackingContext;
    Ref<RemoteAdapterProxy> m_parent;
    Ref<RemoteQueueProxy> m_queue;
};

} // namespace CyberKit::WebGPU

#endif // ENABLE(GPU_PROCESS)
