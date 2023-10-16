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

#include "WebGPUColor.h"
#include "WebGPUComputePassTimestampWrites.h"
#include "WebGPUError.h"
#include "WebGPUExtent3D.h"
#include "WebGPUIdentifier.h"
#include "WebGPUOrigin2D.h"
#include "WebGPUOrigin3D.h"
#include "WebGPURenderPassTimestampWrites.h"
#include <CyberCore/WebGPUColor.h>
#include <CyberCore/WebGPUComputePassTimestampWrites.h>
#include <CyberCore/WebGPUError.h>
#include <CyberCore/WebGPUExtent3D.h>
#include <CyberCore/WebGPUOrigin2D.h>
#include <CyberCore/WebGPUOrigin3D.h>
#include <CyberCore/WebGPURenderPassTimestampWrites.h>
#include <wtf/RefCounted.h>

namespace CyberCore::WebGPU {

class Adapter;
class BindGroup;
struct BindGroupDescriptor;
struct BindGroupEntry;
class BindGroupLayout;
struct BindGroupLayoutDescriptor;
struct BindGroupLayoutEntry;
struct BlendComponent;
struct BlendState;
class Buffer;
struct BufferBinding;
struct BufferBindingLayout;
struct BufferDescriptor;
struct CanvasConfiguration;
struct ColorTargetState;
class CommandBuffer;
struct CommandBufferDescriptor;
class CommandEncoder;
struct CommandEncoderDescriptor;
class CompilationMessage;
class CompositorIntegration;
struct ComputePassDescriptor;
class ComputePassEncoder;
class ComputePipeline;
struct ComputePipelineDescriptor;
struct DepthStencilState;
class Device;
struct DeviceDescriptor;
class ExternalTexture;
struct ExternalTextureBindingLayout;
struct ExternalTextureDescriptor;
struct FragmentState;
class GPU;
struct Identifier;
struct ImageCopyBuffer;
struct ImageCopyExternalImage;
struct ImageCopyTexture;
struct ImageCopyTextureTagged;
struct ImageDataLayout;
class InternalError;
struct MultisampleState;
struct ObjectDescriptorBase;
class OutOfMemoryError;
struct PipelineDescriptorBase;
class PipelineLayout;
struct PipelineLayoutDescriptor;
struct CanvasConfiguration;
class PresentationContext;
struct PresentationContextDescriptor;
struct PrimitiveState;
struct ProgrammableStage;
class QuerySet;
struct QuerySetDescriptor;
class Queue;
class RenderBundle;
struct RenderBundleDescriptor;
class RenderBundleEncoder;
struct RenderBundleEncoderDescriptor;
struct RenderPassColorAttachment;
struct RenderPassDepthStencilAttachment;
struct RenderPassDescriptor;
class RenderPassEncoder;
struct RenderPassLayout;
class RenderPipeline;
struct RenderPipelineDescriptor;
struct RequestAdapterOptions;
class Sampler;
struct SamplerBindingLayout;
struct SamplerDescriptor;
class ShaderModule;
struct ShaderModuleCompilationHint;
struct ShaderModuleDescriptor;
struct StencilFaceState;
struct StorageTextureBindingLayout;
class SupportedFeatures;
class SupportedLimits;
class Texture;
struct TextureBindingLayout;
struct TextureDescriptor;
class TextureView;
struct TextureViewDescriptor;
class ValidationError;
struct VertexAttribute;
struct VertexBufferLayout;
struct VertexState;

} // namespace CyberCore::WebGPU

namespace CyberKit::WebGPU {

struct BindGroupDescriptor;
struct BindGroupEntry;
struct BindGroupLayoutDescriptor;
struct BindGroupLayoutEntry;
struct BlendComponent;
struct BlendState;
struct BufferBinding;
struct BufferBindingLayout;
struct BufferDescriptor;
struct CanvasConfiguration;
struct ColorTargetState;
struct CommandBufferDescriptor;
struct CommandEncoderDescriptor;
struct CompilationMessage;
struct ComputePassDescriptor;
struct ComputePipelineDescriptor;
struct DepthStencilState;
struct DeviceDescriptor;
struct ExternalTextureBindingLayout;
struct ExternalTextureDescriptor;
struct FragmentState;
struct Identifier;
struct ImageCopyBuffer;
struct ImageCopyExternalImage;
struct ImageCopyTexture;
struct ImageCopyTextureTagged;
struct ImageDataLayout;
struct InternalError;
struct MultisampleState;
struct ObjectDescriptorBase;
struct OutOfMemoryError;
struct PipelineDescriptorBase;
struct PipelineLayoutDescriptor;
struct PresentationContextDescriptor;
struct CanvasConfiguration;
struct PrimitiveState;
struct ProgrammableStage;
struct QuerySetDescriptor;
class RemoteCompositorIntegrationProxy;
struct RenderBundleDescriptor;
struct RenderBundleEncoderDescriptor;
struct RenderPassColorAttachment;
struct RenderPassDepthStencilAttachment;
struct RenderPassDescriptor;
struct RenderPassLayout;
struct RenderPipelineDescriptor;
struct RequestAdapterOptions;
struct SamplerBindingLayout;
struct SamplerDescriptor;
struct ShaderModuleCompilationHint;
struct ShaderModuleDescriptor;
struct StencilFaceState;
struct StorageTextureBindingLayout;
struct SupportedFeatures;
struct SupportedLimits;
struct TextureBindingLayout;
struct TextureDescriptor;
struct TextureViewDescriptor;
struct ValidationError;
struct VertexAttribute;
struct VertexBufferLayout;
struct VertexState;

class ConvertToBackingContext : public RefCounted<ConvertToBackingContext> {
public:
    virtual ~ConvertToBackingContext() = default;

    std::optional<BindGroupDescriptor> convertToBacking(const CyberCore::WebGPU::BindGroupDescriptor&);
    std::optional<BindGroupEntry> convertToBacking(const CyberCore::WebGPU::BindGroupEntry&);
    std::optional<BindGroupLayoutDescriptor> convertToBacking(const CyberCore::WebGPU::BindGroupLayoutDescriptor&);
    std::optional<BindGroupLayoutEntry> convertToBacking(const CyberCore::WebGPU::BindGroupLayoutEntry&);
    std::optional<BlendComponent> convertToBacking(const CyberCore::WebGPU::BlendComponent&);
    std::optional<BlendState> convertToBacking(const CyberCore::WebGPU::BlendState&);
    std::optional<BufferBinding> convertToBacking(const CyberCore::WebGPU::BufferBinding&);
    std::optional<BufferBindingLayout> convertToBacking(const CyberCore::WebGPU::BufferBindingLayout&);
    std::optional<BufferDescriptor> convertToBacking(const CyberCore::WebGPU::BufferDescriptor&);
    std::optional<CanvasConfiguration> convertToBacking(const CyberCore::WebGPU::CanvasConfiguration&);
    std::optional<Color> convertToBacking(const CyberCore::WebGPU::Color&);
    std::optional<ColorDict> convertToBacking(const CyberCore::WebGPU::ColorDict&);
    std::optional<ColorTargetState> convertToBacking(const CyberCore::WebGPU::ColorTargetState&);
    std::optional<CommandBufferDescriptor> convertToBacking(const CyberCore::WebGPU::CommandBufferDescriptor&);
    std::optional<CommandEncoderDescriptor> convertToBacking(const CyberCore::WebGPU::CommandEncoderDescriptor&);
    std::optional<CompilationMessage> convertToBacking(const CyberCore::WebGPU::CompilationMessage&);
    std::optional<ComputePassDescriptor> convertToBacking(const CyberCore::WebGPU::ComputePassDescriptor&);
    std::optional<ComputePassTimestampWrite> convertToBacking(const CyberCore::WebGPU::ComputePassTimestampWrite&);
    std::optional<ComputePassTimestampWrites> convertToBacking(const CyberCore::WebGPU::ComputePassTimestampWrites&);
    std::optional<ComputePipelineDescriptor> convertToBacking(const CyberCore::WebGPU::ComputePipelineDescriptor&);
    std::optional<DepthStencilState> convertToBacking(const CyberCore::WebGPU::DepthStencilState&);
    std::optional<DeviceDescriptor> convertToBacking(const CyberCore::WebGPU::DeviceDescriptor&);
    std::optional<Error> convertToBacking(const CyberCore::WebGPU::Error&);
    std::optional<Extent3D> convertToBacking(const CyberCore::WebGPU::Extent3D&);
    std::optional<Extent3DDict> convertToBacking(const CyberCore::WebGPU::Extent3DDict&);
    std::optional<ExternalTextureBindingLayout> convertToBacking(const CyberCore::WebGPU::ExternalTextureBindingLayout&);
    std::optional<ExternalTextureDescriptor> convertToBacking(const CyberCore::WebGPU::ExternalTextureDescriptor&);
    std::optional<FragmentState> convertToBacking(const CyberCore::WebGPU::FragmentState&);
    std::optional<Identifier> convertToBacking(const CyberCore::WebGPU::Identifier&);
    std::optional<ImageCopyBuffer> convertToBacking(const CyberCore::WebGPU::ImageCopyBuffer&);
    std::optional<ImageCopyExternalImage> convertToBacking(const CyberCore::WebGPU::ImageCopyExternalImage&);
    std::optional<ImageCopyTexture> convertToBacking(const CyberCore::WebGPU::ImageCopyTexture&);
    std::optional<ImageCopyTextureTagged> convertToBacking(const CyberCore::WebGPU::ImageCopyTextureTagged&);
    std::optional<ImageDataLayout> convertToBacking(const CyberCore::WebGPU::ImageDataLayout&);
    std::optional<InternalError> convertToBacking(const CyberCore::WebGPU::InternalError&);
    std::optional<MultisampleState> convertToBacking(const CyberCore::WebGPU::MultisampleState&);
    std::optional<ObjectDescriptorBase> convertToBacking(const CyberCore::WebGPU::ObjectDescriptorBase&);
    std::optional<Origin2D> convertToBacking(const CyberCore::WebGPU::Origin2D&);
    std::optional<Origin2DDict> convertToBacking(const CyberCore::WebGPU::Origin2DDict&);
    std::optional<Origin3D> convertToBacking(const CyberCore::WebGPU::Origin3D&);
    std::optional<Origin3DDict> convertToBacking(const CyberCore::WebGPU::Origin3DDict&);
    std::optional<OutOfMemoryError> convertToBacking(const CyberCore::WebGPU::OutOfMemoryError&);
    std::optional<PipelineDescriptorBase> convertToBacking(const CyberCore::WebGPU::PipelineDescriptorBase&);
    std::optional<PipelineLayoutDescriptor> convertToBacking(const CyberCore::WebGPU::PipelineLayoutDescriptor&);
    std::optional<PresentationContextDescriptor> convertToBacking(const CyberCore::WebGPU::PresentationContextDescriptor&);
    std::optional<PrimitiveState> convertToBacking(const CyberCore::WebGPU::PrimitiveState&);
    std::optional<ProgrammableStage> convertToBacking(const CyberCore::WebGPU::ProgrammableStage&);
    std::optional<QuerySetDescriptor> convertToBacking(const CyberCore::WebGPU::QuerySetDescriptor&);
    std::optional<RenderBundleDescriptor> convertToBacking(const CyberCore::WebGPU::RenderBundleDescriptor&);
    std::optional<RenderBundleEncoderDescriptor> convertToBacking(const CyberCore::WebGPU::RenderBundleEncoderDescriptor&);
    std::optional<RenderPassColorAttachment> convertToBacking(const CyberCore::WebGPU::RenderPassColorAttachment&);
    std::optional<RenderPassDepthStencilAttachment> convertToBacking(const CyberCore::WebGPU::RenderPassDepthStencilAttachment&);
    std::optional<RenderPassDescriptor> convertToBacking(const CyberCore::WebGPU::RenderPassDescriptor&);
    std::optional<RenderPassLayout> convertToBacking(const CyberCore::WebGPU::RenderPassLayout&);
    std::optional<RenderPassTimestampWrite> convertToBacking(const CyberCore::WebGPU::RenderPassTimestampWrite&);
    std::optional<RenderPassTimestampWrites> convertToBacking(const CyberCore::WebGPU::RenderPassTimestampWrites&);
    std::optional<RenderPipelineDescriptor> convertToBacking(const CyberCore::WebGPU::RenderPipelineDescriptor&);
    std::optional<RequestAdapterOptions> convertToBacking(const CyberCore::WebGPU::RequestAdapterOptions&);
    std::optional<SamplerBindingLayout> convertToBacking(const CyberCore::WebGPU::SamplerBindingLayout&);
    std::optional<SamplerDescriptor> convertToBacking(const CyberCore::WebGPU::SamplerDescriptor&);
    std::optional<ShaderModuleCompilationHint> convertToBacking(const CyberCore::WebGPU::ShaderModuleCompilationHint&);
    std::optional<ShaderModuleDescriptor> convertToBacking(const CyberCore::WebGPU::ShaderModuleDescriptor&);
    std::optional<StencilFaceState> convertToBacking(const CyberCore::WebGPU::StencilFaceState&);
    std::optional<StorageTextureBindingLayout> convertToBacking(const CyberCore::WebGPU::StorageTextureBindingLayout&);
    std::optional<SupportedFeatures> convertToBacking(const CyberCore::WebGPU::SupportedFeatures&);
    std::optional<SupportedLimits> convertToBacking(const CyberCore::WebGPU::SupportedLimits&);
    std::optional<TextureBindingLayout> convertToBacking(const CyberCore::WebGPU::TextureBindingLayout&);
    std::optional<TextureDescriptor> convertToBacking(const CyberCore::WebGPU::TextureDescriptor&);
    std::optional<TextureViewDescriptor> convertToBacking(const CyberCore::WebGPU::TextureViewDescriptor&);
    std::optional<ValidationError> convertToBacking(const CyberCore::WebGPU::ValidationError&);
    std::optional<VertexAttribute> convertToBacking(const CyberCore::WebGPU::VertexAttribute&);
    std::optional<VertexBufferLayout> convertToBacking(const CyberCore::WebGPU::VertexBufferLayout&);
    std::optional<VertexState> convertToBacking(const CyberCore::WebGPU::VertexState&);

    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::Adapter&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::BindGroup&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::BindGroupLayout&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::Buffer&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::CommandBuffer&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::CommandEncoder&) = 0;
    virtual const RemoteCompositorIntegrationProxy& convertToRawBacking(const CyberCore::WebGPU::CompositorIntegration&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::CompositorIntegration&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::ComputePassEncoder&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::ComputePipeline&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::Device&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::ExternalTexture&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::GPU&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::PipelineLayout&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::PresentationContext&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::QuerySet&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::Queue&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::RenderBundleEncoder&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::RenderBundle&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::RenderPassEncoder&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::RenderPipeline&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::Sampler&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::ShaderModule&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::Texture&) = 0;
    virtual WebGPUIdentifier convertToBacking(const CyberCore::WebGPU::TextureView&) = 0;
};

} // namespace CyberKit::WebGPU

#endif // ENABLE(GPU_PROCESS)
