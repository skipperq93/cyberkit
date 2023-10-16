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
#include <optional>
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
struct ExternalTextureBindingLayout;
struct ExternalTextureDescriptor;
class ExternalTexture;
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
struct CanvasConfiguration;
struct PresentationContextDescriptor;
struct PrimitiveState;
struct ProgrammableStage;
struct QuerySetDescriptor;
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

class ConvertFromBackingContext {
public:
    virtual ~ConvertFromBackingContext() = default;

    std::optional<CyberCore::WebGPU::BindGroupDescriptor> convertFromBacking(const BindGroupDescriptor&);
    std::optional<CyberCore::WebGPU::BindGroupEntry> convertFromBacking(const BindGroupEntry&);
    std::optional<CyberCore::WebGPU::BindGroupLayoutDescriptor> convertFromBacking(const BindGroupLayoutDescriptor&);
    std::optional<CyberCore::WebGPU::BindGroupLayoutEntry> convertFromBacking(const BindGroupLayoutEntry&);
    std::optional<CyberCore::WebGPU::BlendComponent> convertFromBacking(const BlendComponent&);
    std::optional<CyberCore::WebGPU::BlendState> convertFromBacking(const BlendState&);
    std::optional<CyberCore::WebGPU::BufferBinding> convertFromBacking(const BufferBinding&);
    std::optional<CyberCore::WebGPU::BufferBindingLayout> convertFromBacking(const BufferBindingLayout&);
    std::optional<CyberCore::WebGPU::BufferDescriptor> convertFromBacking(const BufferDescriptor&);
    std::optional<CyberCore::WebGPU::CanvasConfiguration> convertFromBacking(const CanvasConfiguration&);
    std::optional<CyberCore::WebGPU::Color> convertFromBacking(const Color&);
    std::optional<CyberCore::WebGPU::ColorDict> convertFromBacking(const ColorDict&);
    std::optional<CyberCore::WebGPU::ColorTargetState> convertFromBacking(const ColorTargetState&);
    std::optional<CyberCore::WebGPU::CommandBufferDescriptor> convertFromBacking(const CommandBufferDescriptor&);
    std::optional<CyberCore::WebGPU::CommandEncoderDescriptor> convertFromBacking(const CommandEncoderDescriptor&);
    RefPtr<CyberCore::WebGPU::CompilationMessage> convertFromBacking(const CompilationMessage&);
    std::optional<CyberCore::WebGPU::ComputePassDescriptor> convertFromBacking(const ComputePassDescriptor&);
    std::optional<CyberCore::WebGPU::ComputePassTimestampWrite> convertFromBacking(const ComputePassTimestampWrite&);
    std::optional<CyberCore::WebGPU::ComputePassTimestampWrites> convertFromBacking(const ComputePassTimestampWrites&);
    std::optional<CyberCore::WebGPU::ComputePipelineDescriptor> convertFromBacking(const ComputePipelineDescriptor&);
    std::optional<CyberCore::WebGPU::DepthStencilState> convertFromBacking(const DepthStencilState&);
    std::optional<CyberCore::WebGPU::DeviceDescriptor> convertFromBacking(const DeviceDescriptor&);
    std::optional<CyberCore::WebGPU::Error> convertFromBacking(const Error&);
    std::optional<CyberCore::WebGPU::Extent3D> convertFromBacking(const Extent3D&);
    std::optional<CyberCore::WebGPU::Extent3DDict> convertFromBacking(const Extent3DDict&);
    std::optional<CyberCore::WebGPU::ExternalTextureBindingLayout> convertFromBacking(const ExternalTextureBindingLayout&);
    std::optional<CyberCore::WebGPU::ExternalTextureDescriptor> convertFromBacking(const ExternalTextureDescriptor&);
    std::optional<CyberCore::WebGPU::FragmentState> convertFromBacking(const FragmentState&);
    std::optional<CyberCore::WebGPU::Identifier> convertFromBacking(const Identifier&);
    std::optional<CyberCore::WebGPU::ImageCopyBuffer> convertFromBacking(const ImageCopyBuffer&);
    std::optional<CyberCore::WebGPU::ImageCopyExternalImage> convertFromBacking(const ImageCopyExternalImage&);
    std::optional<CyberCore::WebGPU::ImageCopyTexture> convertFromBacking(const ImageCopyTexture&);
    std::optional<CyberCore::WebGPU::ImageCopyTextureTagged> convertFromBacking(const ImageCopyTextureTagged&);
    std::optional<CyberCore::WebGPU::ImageDataLayout> convertFromBacking(const ImageDataLayout&);
    RefPtr<CyberCore::WebGPU::InternalError> convertFromBacking(const InternalError&);
    std::optional<CyberCore::WebGPU::MultisampleState> convertFromBacking(const MultisampleState&);
    std::optional<CyberCore::WebGPU::ObjectDescriptorBase> convertFromBacking(const ObjectDescriptorBase&);
    std::optional<CyberCore::WebGPU::Origin2D> convertFromBacking(const Origin2D&);
    std::optional<CyberCore::WebGPU::Origin2DDict> convertFromBacking(const Origin2DDict&);
    std::optional<CyberCore::WebGPU::Origin3D> convertFromBacking(const Origin3D&);
    std::optional<CyberCore::WebGPU::Origin3DDict> convertFromBacking(const Origin3DDict&);
    RefPtr<CyberCore::WebGPU::OutOfMemoryError> convertFromBacking(const OutOfMemoryError&);
    std::optional<CyberCore::WebGPU::PipelineDescriptorBase> convertFromBacking(const PipelineDescriptorBase&);
    std::optional<CyberCore::WebGPU::PipelineLayoutDescriptor> convertFromBacking(const PipelineLayoutDescriptor&);
    std::optional<CyberCore::WebGPU::PresentationContextDescriptor> convertFromBacking(const PresentationContextDescriptor&);
    std::optional<CyberCore::WebGPU::PrimitiveState> convertFromBacking(const PrimitiveState&);
    std::optional<CyberCore::WebGPU::ProgrammableStage> convertFromBacking(const ProgrammableStage&);
    std::optional<CyberCore::WebGPU::QuerySetDescriptor> convertFromBacking(const QuerySetDescriptor&);
    std::optional<CyberCore::WebGPU::RenderBundleDescriptor> convertFromBacking(const RenderBundleDescriptor&);
    std::optional<CyberCore::WebGPU::RenderBundleEncoderDescriptor> convertFromBacking(const RenderBundleEncoderDescriptor&);
    std::optional<CyberCore::WebGPU::RenderPassColorAttachment> convertFromBacking(const RenderPassColorAttachment&);
    std::optional<CyberCore::WebGPU::RenderPassDepthStencilAttachment> convertFromBacking(const RenderPassDepthStencilAttachment&);
    std::optional<CyberCore::WebGPU::RenderPassDescriptor> convertFromBacking(const RenderPassDescriptor&);
    std::optional<CyberCore::WebGPU::RenderPassLayout> convertFromBacking(const RenderPassLayout&);
    std::optional<CyberCore::WebGPU::RenderPassTimestampWrite> convertFromBacking(const RenderPassTimestampWrite&);
    std::optional<CyberCore::WebGPU::RenderPassTimestampWrites> convertFromBacking(const RenderPassTimestampWrites&);
    std::optional<CyberCore::WebGPU::RenderPipelineDescriptor> convertFromBacking(const RenderPipelineDescriptor&);
    std::optional<CyberCore::WebGPU::RequestAdapterOptions> convertFromBacking(const RequestAdapterOptions&);
    std::optional<CyberCore::WebGPU::SamplerBindingLayout> convertFromBacking(const SamplerBindingLayout&);
    std::optional<CyberCore::WebGPU::SamplerDescriptor> convertFromBacking(const SamplerDescriptor&);
    std::optional<CyberCore::WebGPU::ShaderModuleCompilationHint> convertFromBacking(const ShaderModuleCompilationHint&);
    std::optional<CyberCore::WebGPU::ShaderModuleDescriptor> convertFromBacking(const ShaderModuleDescriptor&);
    std::optional<CyberCore::WebGPU::StencilFaceState> convertFromBacking(const StencilFaceState&);
    std::optional<CyberCore::WebGPU::StorageTextureBindingLayout> convertFromBacking(const StorageTextureBindingLayout&);
    RefPtr<CyberCore::WebGPU::SupportedFeatures> convertFromBacking(const SupportedFeatures&);
    RefPtr<CyberCore::WebGPU::SupportedLimits> convertFromBacking(const SupportedLimits&);
    std::optional<CyberCore::WebGPU::TextureBindingLayout> convertFromBacking(const TextureBindingLayout&);
    std::optional<CyberCore::WebGPU::TextureDescriptor> convertFromBacking(const TextureDescriptor&);
    std::optional<CyberCore::WebGPU::TextureViewDescriptor> convertFromBacking(const TextureViewDescriptor&);
    RefPtr<CyberCore::WebGPU::ValidationError> convertFromBacking(const ValidationError&);
    std::optional<CyberCore::WebGPU::VertexAttribute> convertFromBacking(const VertexAttribute&);
    std::optional<CyberCore::WebGPU::VertexBufferLayout> convertFromBacking(const VertexBufferLayout&);
    std::optional<CyberCore::WebGPU::VertexState> convertFromBacking(const VertexState&);

    virtual CyberCore::WebGPU::Adapter* convertAdapterFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::BindGroup* convertBindGroupFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::BindGroupLayout* convertBindGroupLayoutFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::Buffer* convertBufferFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::CommandBuffer* convertCommandBufferFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::CommandEncoder* convertCommandEncoderFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::CompositorIntegration* convertCompositorIntegrationFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::ComputePassEncoder* convertComputePassEncoderFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::ComputePipeline* convertComputePipelineFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::Device* convertDeviceFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::ExternalTexture* convertExternalTextureFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::PipelineLayout* convertPipelineLayoutFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::QuerySet* convertQuerySetFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::Queue* convertQueueFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::RenderBundleEncoder* convertRenderBundleEncoderFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::RenderBundle* convertRenderBundleFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::RenderPassEncoder* convertRenderPassEncoderFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::RenderPipeline* convertRenderPipelineFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::Sampler* convertSamplerFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::ShaderModule* convertShaderModuleFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::PresentationContext* convertPresentationContextFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::Texture* convertTextureFromBacking(WebGPUIdentifier) = 0;
    virtual CyberCore::WebGPU::TextureView* convertTextureViewFromBacking(WebGPUIdentifier) = 0;
};

} // namespace CyberKit::WebGPU

#endif // ENABLE(GPU_PROCESS)
