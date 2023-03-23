# Copyright (C) 2006-2022 Apple Inc. All rights reserved.
# Copyright (C) 2006 Samuel Weinig <sam.weinig@gmail.com>
# Copyright (C) 2009 Cameron McCormack <cam@mcc.id.au>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1.  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
# 3.  Neither the name of Apple Inc. ("Apple") nor the names of
#     its contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Workaround for rdar://84212106.
find_tool = $(realpath $(shell xcrun --sdk $(SDK_NAME) -f $(1)))

PYTHON := $(call find_tool,python3)
PERL := perl
RUBY := ruby
DELETE := rm -f

ifneq ($(SDKROOT),)
    SDK_FLAGS = -isysroot $(SDKROOT)
endif

WK_CURRENT_ARCH = $(word 1, $(ARCHS))
TARGET_TRIPLE_FLAGS = -target $(WK_CURRENT_ARCH)-$(LLVM_TARGET_TRIPLE_VENDOR)-$(LLVM_TARGET_TRIPLE_OS_VERSION)$(LLVM_TARGET_TRIPLE_SUFFIX)

FRAMEWORK_FLAGS := $(addprefix -F, $(BUILT_PRODUCTS_DIR) $(FRAMEWORK_SEARCH_PATHS) $(SYSTEM_FRAMEWORK_SEARCH_PATHS))
HEADER_FLAGS := $(addprefix -I, $(BUILT_PRODUCTS_DIR) $(HEADER_SEARCH_PATHS) $(SYSTEM_HEADER_SEARCH_PATHS))
EXTERNAL_FLAGS := -DRELEASE_WITHOUT_OPTIMIZATIONS $(addprefix -D, $(GCC_PREPROCESSOR_DEFINITIONS))

platform_h_compiler_command = $(CC) -std=c++2a -x c++ $(1) $(SDK_FLAGS) $(TARGET_TRIPLE_FLAGS) $(FRAMEWORK_FLAGS) $(HEADER_FLAGS) $(EXTERNAL_FLAGS) -include "wtf/Platform.h" /dev/null

FEATURE_AND_PLATFORM_DEFINES := $(shell $(call platform_h_compiler_command,-E -P -dM) | $(PERL) -ne "print if s/\#define ((HAVE_|USE_|ENABLE_|WTF_PLATFORM_)\w+) 1/\1/")

PLATFORM_HEADER_DIR := $(realpath $(BUILT_PRODUCTS_DIR)$(WK_LIBRARY_HEADERS_FOLDER_PATH))
PLATFORM_HEADER_DEPENDENCIES := $(filter $(PLATFORM_HEADER_DIR)/%,$(realpath $(shell $(call platform_h_compiler_command,-M) | $(PERL) -e "local \$$/; my (\$$target, \$$deps) = split(/:/, <>); print split(/\\\\/, \$$deps);")))
FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES = $(CyberCore)/DerivedSources.make $(PLATFORM_HEADER_DEPENDENCIES)

# --------

JS_BINDING_IDLS := \
    $(CyberCore)/Modules/WebGPU/GPU.idl \
    $(CyberCore)/Modules/WebGPU/GPUAdapter.idl \
    $(CyberCore)/Modules/WebGPU/GPUAddressMode.idl \
    $(CyberCore)/Modules/WebGPU/GPUAutoLayoutMode.idl \
    $(CyberCore)/Modules/WebGPU/GPUBindGroup.idl \
    $(CyberCore)/Modules/WebGPU/GPUBindGroupDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPUBindGroupEntry.idl \
    $(CyberCore)/Modules/WebGPU/GPUBindGroupLayout.idl \
    $(CyberCore)/Modules/WebGPU/GPUBindGroupLayoutDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPUBindGroupLayoutEntry.idl \
    $(CyberCore)/Modules/WebGPU/GPUBlendComponent.idl \
    $(CyberCore)/Modules/WebGPU/GPUBlendFactor.idl \
    $(CyberCore)/Modules/WebGPU/GPUBlendOperation.idl \
    $(CyberCore)/Modules/WebGPU/GPUBlendState.idl \
    $(CyberCore)/Modules/WebGPU/GPUBuffer.idl \
    $(CyberCore)/Modules/WebGPU/GPUBufferBinding.idl \
    $(CyberCore)/Modules/WebGPU/GPUBufferBindingLayout.idl \
    $(CyberCore)/Modules/WebGPU/GPUBufferBindingType.idl \
    $(CyberCore)/Modules/WebGPU/GPUBufferDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPUBufferUsage.idl \
    $(CyberCore)/Modules/WebGPU/GPUCanvasCompositingAlphaMode.idl \
    $(CyberCore)/Modules/WebGPU/GPUCanvasConfiguration.idl \
    $(CyberCore)/Modules/WebGPU/GPUColorDict.idl \
    $(CyberCore)/Modules/WebGPU/GPUColorTargetState.idl \
    $(CyberCore)/Modules/WebGPU/GPUColorWrite.idl \
    $(CyberCore)/Modules/WebGPU/GPUCommandBuffer.idl \
    $(CyberCore)/Modules/WebGPU/GPUCommandBufferDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPUCommandEncoder.idl \
    $(CyberCore)/Modules/WebGPU/GPUCommandEncoderDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPUCommandsMixin.idl \
    $(CyberCore)/Modules/WebGPU/GPUCompareFunction.idl \
    $(CyberCore)/Modules/WebGPU/GPUCompilationInfo.idl \
    $(CyberCore)/Modules/WebGPU/GPUCompilationMessage.idl \
    $(CyberCore)/Modules/WebGPU/GPUCompilationMessageType.idl \
    $(CyberCore)/Modules/WebGPU/GPUComputePassDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPUComputePassEncoder.idl \
    $(CyberCore)/Modules/WebGPU/GPUComputePassTimestampLocation.idl \
    $(CyberCore)/Modules/WebGPU/GPUComputePassTimestampWrite.idl \
    $(CyberCore)/Modules/WebGPU/GPUComputePipeline.idl \
    $(CyberCore)/Modules/WebGPU/GPUComputePipelineDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPUCullMode.idl \
    $(CyberCore)/Modules/WebGPU/GPUDebugCommandsMixin.idl \
    $(CyberCore)/Modules/WebGPU/GPUDepthStencilState.idl \
    $(CyberCore)/Modules/WebGPU/GPUDevice.idl \
    $(CyberCore)/Modules/WebGPU/GPUDeviceDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPUDeviceError.idl \
    $(CyberCore)/Modules/WebGPU/GPUDeviceLost.idl \
    $(CyberCore)/Modules/WebGPU/GPUDeviceLostInfo.idl \
    $(CyberCore)/Modules/WebGPU/GPUDeviceLostReason.idl \
    $(CyberCore)/Modules/WebGPU/GPUDeviceUncapturedError.idl \
    $(CyberCore)/Modules/WebGPU/GPUErrorFilter.idl \
    $(CyberCore)/Modules/WebGPU/GPUExtent3DDict.idl \
    $(CyberCore)/Modules/WebGPU/GPUExternalTexture.idl \
    $(CyberCore)/Modules/WebGPU/GPUExternalTextureBindingLayout.idl \
    $(CyberCore)/Modules/WebGPU/GPUExternalTextureDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPUFeatureName.idl \
    $(CyberCore)/Modules/WebGPU/GPUFilterMode.idl \
    $(CyberCore)/Modules/WebGPU/GPUFragmentState.idl \
    $(CyberCore)/Modules/WebGPU/GPUFrontFace.idl \
    $(CyberCore)/Modules/WebGPU/GPUImageCopyBuffer.idl \
    $(CyberCore)/Modules/WebGPU/GPUImageCopyExternalImage.idl \
    $(CyberCore)/Modules/WebGPU/GPUImageCopyTexture.idl \
    $(CyberCore)/Modules/WebGPU/GPUImageCopyTextureTagged.idl \
    $(CyberCore)/Modules/WebGPU/GPUImageDataLayout.idl \
    $(CyberCore)/Modules/WebGPU/GPUIndexFormat.idl \
    $(CyberCore)/Modules/WebGPU/GPULoadOp.idl \
    $(CyberCore)/Modules/WebGPU/GPUMapMode.idl \
    $(CyberCore)/Modules/WebGPU/GPUMipmapFilterMode.idl \
    $(CyberCore)/Modules/WebGPU/GPUMultisampleState.idl \
    $(CyberCore)/Modules/WebGPU/GPUObjectBase.idl \
    $(CyberCore)/Modules/WebGPU/GPUObjectDescriptorBase.idl \
    $(CyberCore)/Modules/WebGPU/GPUOrigin2DDict.idl \
    $(CyberCore)/Modules/WebGPU/GPUOrigin3DDict.idl \
    $(CyberCore)/Modules/WebGPU/GPUOutOfMemoryError.idl \
    $(CyberCore)/Modules/WebGPU/GPUPipelineBase.idl \
    $(CyberCore)/Modules/WebGPU/GPUPipelineDescriptorBase.idl \
    $(CyberCore)/Modules/WebGPU/GPUPipelineLayout.idl \
    $(CyberCore)/Modules/WebGPU/GPUPipelineLayoutDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPUPowerPreference.idl \
    $(CyberCore)/Modules/WebGPU/GPUPredefinedColorSpace.idl \
    $(CyberCore)/Modules/WebGPU/GPUPrimitiveState.idl \
    $(CyberCore)/Modules/WebGPU/GPUPrimitiveTopology.idl \
    $(CyberCore)/Modules/WebGPU/GPUProgrammablePassEncoder.idl \
    $(CyberCore)/Modules/WebGPU/GPUProgrammableStage.idl \
    $(CyberCore)/Modules/WebGPU/GPUQuerySet.idl \
    $(CyberCore)/Modules/WebGPU/GPUQuerySetDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPUQueryType.idl \
    $(CyberCore)/Modules/WebGPU/GPUQueue.idl \
    $(CyberCore)/Modules/WebGPU/GPURenderBundle.idl \
    $(CyberCore)/Modules/WebGPU/GPURenderBundleDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPURenderBundleEncoder.idl \
    $(CyberCore)/Modules/WebGPU/GPURenderBundleEncoderDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPURenderEncoderBase.idl \
    $(CyberCore)/Modules/WebGPU/GPURenderPassColorAttachment.idl \
    $(CyberCore)/Modules/WebGPU/GPURenderPassDepthStencilAttachment.idl \
    $(CyberCore)/Modules/WebGPU/GPURenderPassDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPURenderPassEncoder.idl \
    $(CyberCore)/Modules/WebGPU/GPURenderPassLayout.idl \
    $(CyberCore)/Modules/WebGPU/GPURenderPassTimestampLocation.idl \
    $(CyberCore)/Modules/WebGPU/GPURenderPassTimestampWrite.idl \
    $(CyberCore)/Modules/WebGPU/GPURenderPipeline.idl \
    $(CyberCore)/Modules/WebGPU/GPURenderPipelineDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPURequestAdapterOptions.idl \
    $(CyberCore)/Modules/WebGPU/GPUSampler.idl \
    $(CyberCore)/Modules/WebGPU/GPUSamplerBindingLayout.idl \
    $(CyberCore)/Modules/WebGPU/GPUSamplerBindingType.idl \
    $(CyberCore)/Modules/WebGPU/GPUSamplerDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPUShaderModule.idl \
    $(CyberCore)/Modules/WebGPU/GPUShaderModuleCompilationHint.idl \
    $(CyberCore)/Modules/WebGPU/GPUShaderModuleDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPUShaderStage.idl \
    $(CyberCore)/Modules/WebGPU/GPUStencilFaceState.idl \
    $(CyberCore)/Modules/WebGPU/GPUStencilOperation.idl \
    $(CyberCore)/Modules/WebGPU/GPUStorageTextureAccess.idl \
    $(CyberCore)/Modules/WebGPU/GPUStorageTextureBindingLayout.idl \
    $(CyberCore)/Modules/WebGPU/GPUStoreOp.idl \
    $(CyberCore)/Modules/WebGPU/GPUSupportedFeatures.idl \
    $(CyberCore)/Modules/WebGPU/GPUSupportedLimits.idl \
    $(CyberCore)/Modules/WebGPU/GPUTexture.idl \
    $(CyberCore)/Modules/WebGPU/GPUTextureAspect.idl \
    $(CyberCore)/Modules/WebGPU/GPUTextureBindingLayout.idl \
    $(CyberCore)/Modules/WebGPU/GPUTextureDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPUTextureDimension.idl \
    $(CyberCore)/Modules/WebGPU/GPUTextureFormat.idl \
    $(CyberCore)/Modules/WebGPU/GPUTextureSampleType.idl \
    $(CyberCore)/Modules/WebGPU/GPUTextureUsage.idl \
    $(CyberCore)/Modules/WebGPU/GPUTextureView.idl \
    $(CyberCore)/Modules/WebGPU/GPUTextureViewDescriptor.idl \
    $(CyberCore)/Modules/WebGPU/GPUTextureViewDimension.idl \
    $(CyberCore)/Modules/WebGPU/GPUUncapturedErrorEvent.idl \
    $(CyberCore)/Modules/WebGPU/GPUUncapturedErrorEventInit.idl \
    $(CyberCore)/Modules/WebGPU/GPUValidationError.idl \
    $(CyberCore)/Modules/WebGPU/GPUVertexAttribute.idl \
    $(CyberCore)/Modules/WebGPU/GPUVertexBufferLayout.idl \
    $(CyberCore)/Modules/WebGPU/GPUVertexFormat.idl \
    $(CyberCore)/Modules/WebGPU/GPUVertexState.idl \
    $(CyberCore)/Modules/WebGPU/GPUVertexStepMode.idl \
    $(CyberCore)/Modules/WebGPU/NavigatorGPU.idl \
    $(CyberCore)/Modules/airplay/CyberKitPlaybackTargetAvailabilityEvent.idl \
    $(CyberCore)/Modules/applepay/ApplePayAutomaticReloadPaymentRequest.idl \
    $(CyberCore)/Modules/applepay/ApplePayCancelEvent.idl \
    $(CyberCore)/Modules/applepay/ApplePayContactField.idl \
    $(CyberCore)/Modules/applepay/ApplePayCouponCodeChangedEvent.idl \
    $(CyberCore)/Modules/applepay/ApplePayCouponCodeDetails.idl \
    $(CyberCore)/Modules/applepay/ApplePayCouponCodeUpdate.idl \
    $(CyberCore)/Modules/applepay/ApplePayDateComponents.idl \
    $(CyberCore)/Modules/applepay/ApplePayDateComponentsRange.idl \
    $(CyberCore)/Modules/applepay/ApplePayDetailsUpdateBase.idl \
    $(CyberCore)/Modules/applepay/ApplePayError.idl \
    $(CyberCore)/Modules/applepay/ApplePayErrorCode.idl \
    $(CyberCore)/Modules/applepay/ApplePayErrorContactField.idl \
    $(CyberCore)/Modules/applepay/ApplePayFeature.idl \
    $(CyberCore)/Modules/applepay/ApplePayInstallmentConfiguration.idl \
    $(CyberCore)/Modules/applepay/ApplePayInstallmentItem.idl \
    $(CyberCore)/Modules/applepay/ApplePayInstallmentItemType.idl \
    $(CyberCore)/Modules/applepay/ApplePayInstallmentRetailChannel.idl \
    $(CyberCore)/Modules/applepay/ApplePayLineItem.idl \
    $(CyberCore)/Modules/applepay/ApplePayMerchantCapability.idl \
    $(CyberCore)/Modules/applepay/ApplePayPayment.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentAuthorizationResult.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentAuthorizedEvent.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentContact.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentMethod.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentMethodSelectedEvent.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentMethodType.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentMethodUpdate.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentOrderDetails.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentPass.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentRequest.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentTiming.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentTokenContext.idl \
    $(CyberCore)/Modules/applepay/ApplePayRecurringPaymentDateUnit.idl \
    $(CyberCore)/Modules/applepay/ApplePayRecurringPaymentRequest.idl \
    $(CyberCore)/Modules/applepay/ApplePayRequestBase.idl \
    $(CyberCore)/Modules/applepay/ApplePaySession.idl \
    $(CyberCore)/Modules/applepay/ApplePaySessionError.idl \
    $(CyberCore)/Modules/applepay/ApplePaySetup.idl \
    $(CyberCore)/Modules/applepay/ApplePaySetupConfiguration.idl \
    $(CyberCore)/Modules/applepay/ApplePaySetupFeature.idl \
    $(CyberCore)/Modules/applepay/ApplePaySetupFeatureState.idl \
    $(CyberCore)/Modules/applepay/ApplePaySetupFeatureType.idl \
    $(CyberCore)/Modules/applepay/ApplePayShippingContactEditingMode.idl \
    $(CyberCore)/Modules/applepay/ApplePayShippingContactSelectedEvent.idl \
    $(CyberCore)/Modules/applepay/ApplePayShippingContactUpdate.idl \
    $(CyberCore)/Modules/applepay/ApplePayShippingMethod.idl \
    $(CyberCore)/Modules/applepay/ApplePayShippingMethodSelectedEvent.idl \
    $(CyberCore)/Modules/applepay/ApplePayShippingMethodUpdate.idl \
    $(CyberCore)/Modules/applepay/ApplePayValidateMerchantEvent.idl \
    $(CyberCore)/Modules/applepay/paymentrequest/ApplePayModifier.idl \
    $(CyberCore)/Modules/applepay/paymentrequest/ApplePayPaymentCompleteDetails.idl \
    $(CyberCore)/Modules/applepay/paymentrequest/ApplePayRequest.idl \
    $(CyberCore)/Modules/applepay-ams-ui/ApplePayAMSUIRequest.idl \
    $(CyberCore)/Modules/async-clipboard/Clipboard.idl \
    $(CyberCore)/Modules/async-clipboard/ClipboardItem.idl \
    $(CyberCore)/Modules/async-clipboard/Navigator+Clipboard.idl \
    $(CyberCore)/Modules/audiosession/DOMAudioSession.idl \
    $(CyberCore)/Modules/audiosession/Navigator+AudioSession.idl \
    $(CyberCore)/Modules/badge/Navigator+Badge.idl \
    $(CyberCore)/Modules/badge/NavigatorBadge.idl \
    $(CyberCore)/Modules/beacon/Navigator+Beacon.idl \
    $(CyberCore)/Modules/cache/CacheQueryOptions.idl \
    $(CyberCore)/Modules/cache/DOMCache.idl \
    $(CyberCore)/Modules/cache/DOMCacheStorage.idl \
    $(CyberCore)/Modules/cache/MultiCacheQueryOptions.idl \
    $(CyberCore)/Modules/cache/WindowOrWorkerGlobalScope+Caches.idl \
    $(CyberCore)/Modules/compression/CompressionStream.idl \
    $(CyberCore)/Modules/compression/CompressionStreamEncoder.idl \
    $(CyberCore)/Modules/compression/DecompressionStream.idl \
    $(CyberCore)/Modules/compression/DecompressionStreamDecoder.idl \
    $(CyberCore)/Modules/contact-picker/ContactInfo.idl \
    $(CyberCore)/Modules/contact-picker/ContactProperty.idl \
    $(CyberCore)/Modules/contact-picker/ContactsManager.idl \
    $(CyberCore)/Modules/contact-picker/ContactsSelectOptions.idl \
    $(CyberCore)/Modules/contact-picker/Navigator+Contacts.idl \
    $(CyberCore)/Modules/cookie-consent/Navigator+CookieConsent.idl \
    $(CyberCore)/Modules/cookie-consent/RequestCookieConsentOptions.idl \
    $(CyberCore)/Modules/credentialmanagement/BasicCredential.idl \
    $(CyberCore)/Modules/credentialmanagement/CredentialCreationOptions.idl \
    $(CyberCore)/Modules/credentialmanagement/CredentialRequestOptions.idl \
    $(CyberCore)/Modules/credentialmanagement/CredentialsContainer.idl \
    $(CyberCore)/Modules/credentialmanagement/Navigator+Credentials.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeyEncryptionScheme.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeyMessageEventInit.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeyMessageEvent.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeyMessageType.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeySession.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeySessionType.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeyStatusMap.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeySystemAccess.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeySystemConfiguration.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeySystemMediaCapability.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeys.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeysRequirement.idl \
    $(CyberCore)/Modules/encryptedmedia/Navigator+EME.idl \
    $(CyberCore)/Modules/encryptedmedia/legacy/CyberKitMediaKeyMessageEvent.idl \
    $(CyberCore)/Modules/encryptedmedia/legacy/CyberKitMediaKeyNeededEvent.idl \
    $(CyberCore)/Modules/encryptedmedia/legacy/CyberKitMediaKeySession.idl \
    $(CyberCore)/Modules/encryptedmedia/legacy/CyberKitMediaKeys.idl \
    $(CyberCore)/Modules/entriesapi/DOMFileSystem.idl \
    $(CyberCore)/Modules/entriesapi/ErrorCallback.idl \
    $(CyberCore)/Modules/entriesapi/FileCallback.idl \
    $(CyberCore)/Modules/entriesapi/FileSystemDirectoryEntry.idl \
    $(CyberCore)/Modules/entriesapi/FileSystemDirectoryReader.idl \
    $(CyberCore)/Modules/entriesapi/FileSystemEntriesCallback.idl \
    $(CyberCore)/Modules/entriesapi/FileSystemEntry.idl \
    $(CyberCore)/Modules/entriesapi/FileSystemEntryCallback.idl \
    $(CyberCore)/Modules/entriesapi/FileSystemFileEntry.idl \
    $(CyberCore)/Modules/entriesapi/HTMLInputElement+EntriesAPI.idl \
    $(CyberCore)/Modules/fetch/FetchBody.idl \
    $(CyberCore)/Modules/fetch/FetchHeaders.idl \
    $(CyberCore)/Modules/fetch/FetchReferrerPolicy.idl \
    $(CyberCore)/Modules/fetch/FetchRequest.idl \
    $(CyberCore)/Modules/fetch/FetchRequestCache.idl \
    $(CyberCore)/Modules/fetch/FetchRequestCredentials.idl \
    $(CyberCore)/Modules/fetch/FetchRequestDestination.idl \
    $(CyberCore)/Modules/fetch/FetchRequestInit.idl \
    $(CyberCore)/Modules/fetch/FetchRequestMode.idl \
    $(CyberCore)/Modules/fetch/FetchRequestRedirect.idl \
    $(CyberCore)/Modules/fetch/FetchResponse.idl \
    $(CyberCore)/Modules/fetch/RequestPriority.idl \
    $(CyberCore)/Modules/fetch/WindowOrWorkerGlobalScope+Fetch.idl \
    $(CyberCore)/Modules/filesystemaccess/FileSystemDirectoryHandle.idl \
    $(CyberCore)/Modules/filesystemaccess/FileSystemFileHandle.idl \
    $(CyberCore)/Modules/filesystemaccess/FileSystemHandle.idl \
    $(CyberCore)/Modules/filesystemaccess/FileSystemSyncAccessHandle.idl \
    $(CyberCore)/Modules/filesystemaccess/StorageManager+FileSystemAccess.idl \
    $(CyberCore)/Modules/gamepad/Gamepad.idl \
    $(CyberCore)/Modules/gamepad/GamepadButton.idl \
    $(CyberCore)/Modules/gamepad/GamepadEffectParameters.idl \
    $(CyberCore)/Modules/gamepad/GamepadEvent.idl \
    $(CyberCore)/Modules/gamepad/GamepadHapticActuator.idl \
    $(CyberCore)/Modules/gamepad/GamepadHapticEffectType.idl \
    $(CyberCore)/Modules/gamepad/Navigator+Gamepad.idl \
    $(CyberCore)/Modules/gamepad/WindowEventHandlers+Gamepad.idl \
    $(CyberCore)/Modules/geolocation/Geolocation.idl \
    $(CyberCore)/Modules/geolocation/GeolocationCoordinates.idl \
    $(CyberCore)/Modules/geolocation/GeolocationPosition.idl \
    $(CyberCore)/Modules/geolocation/GeolocationPositionError.idl \
    $(CyberCore)/Modules/geolocation/Navigator+Geolocation.idl \
    $(CyberCore)/Modules/geolocation/PositionCallback.idl \
    $(CyberCore)/Modules/geolocation/PositionErrorCallback.idl \
    $(CyberCore)/Modules/geolocation/PositionOptions.idl \
    $(CyberCore)/Modules/highlight/HighlightRegister.idl \
    $(CyberCore)/Modules/highlight/Highlight.idl \
    $(CyberCore)/Modules/indexeddb/IDBCursor.idl \
    $(CyberCore)/Modules/indexeddb/IDBCursorDirection.idl \
    $(CyberCore)/Modules/indexeddb/IDBCursorWithValue.idl \
    $(CyberCore)/Modules/indexeddb/IDBDatabase.idl \
    $(CyberCore)/Modules/indexeddb/IDBFactory.idl \
    $(CyberCore)/Modules/indexeddb/IDBIndex.idl \
    $(CyberCore)/Modules/indexeddb/IDBKeyRange.idl \
    $(CyberCore)/Modules/indexeddb/IDBObjectStore.idl \
    $(CyberCore)/Modules/indexeddb/IDBOpenDBRequest.idl \
    $(CyberCore)/Modules/indexeddb/IDBRequest.idl \
    $(CyberCore)/Modules/indexeddb/IDBTransaction.idl \
	$(CyberCore)/Modules/indexeddb/IDBTransactionDurability.idl \
    $(CyberCore)/Modules/indexeddb/IDBTransactionMode.idl \
    $(CyberCore)/Modules/indexeddb/IDBVersionChangeEvent.idl \
    $(CyberCore)/Modules/indexeddb/WindowOrWorkerGlobalScope+IndexedDatabase.idl \
    $(CyberCore)/Modules/mediacapabilities/AudioConfiguration.idl \
    $(CyberCore)/Modules/mediacapabilities/ColorGamut.idl \
    $(CyberCore)/Modules/mediacapabilities/HdrMetadataType.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaCapabilities.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaCapabilitiesDecodingInfo.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaCapabilitiesEncodingInfo.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaCapabilitiesInfo.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaConfiguration.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaDecodingConfiguration.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaDecodingType.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaEncodingConfiguration.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaEncodingType.idl \
    $(CyberCore)/Modules/mediacapabilities/Navigator+MediaCapabilities.idl \
    $(CyberCore)/Modules/mediacapabilities/TransferFunction.idl \
    $(CyberCore)/Modules/mediacapabilities/VideoConfiguration.idl \
    $(CyberCore)/Modules/mediacapabilities/WorkerNavigator+MediaCapabilities.idl \
    $(CyberCore)/Modules/mediacontrols/MediaControlsHost.idl \
    $(CyberCore)/Modules/mediarecorder/BlobEvent.idl \
    $(CyberCore)/Modules/mediarecorder/MediaRecorder.idl \
    $(CyberCore)/Modules/mediarecorder/MediaRecorderErrorEvent.idl \
    $(CyberCore)/Modules/mediasource/AudioTrack+MediaSource.idl \
    $(CyberCore)/Modules/mediasource/DOMURL+MediaSource.idl \
    $(CyberCore)/Modules/mediasource/ManagedMediaSource.idl \
    $(CyberCore)/Modules/mediasource/ManagedSourceBuffer.idl \
    $(CyberCore)/Modules/mediasource/MediaSource.idl \
    $(CyberCore)/Modules/mediasource/SourceBuffer.idl \
    $(CyberCore)/Modules/mediasource/SourceBufferList.idl \
    $(CyberCore)/Modules/mediasource/TextTrack+MediaSource.idl \
    $(CyberCore)/Modules/mediasource/VideoPlaybackQuality.idl \
    $(CyberCore)/Modules/mediasource/VideoTrack+MediaSource.idl \
    $(CyberCore)/Modules/mediasession/MediaImage.idl \
    $(CyberCore)/Modules/mediasession/MediaMetadata.idl \
    $(CyberCore)/Modules/mediasession/MediaMetadataInit.idl \
    $(CyberCore)/Modules/mediasession/MediaMetadataPlaylistMixin.idl \
    $(CyberCore)/Modules/mediasession/MediaPositionState.idl \
    $(CyberCore)/Modules/mediasession/MediaSession.idl \
    $(CyberCore)/Modules/mediasession/MediaSessionAction.idl \
    $(CyberCore)/Modules/mediasession/MediaSessionActionDetails.idl \
    $(CyberCore)/Modules/mediasession/MediaSessionActionHandler.idl \
    $(CyberCore)/Modules/mediasession/MediaSessionCoordinator.idl \
    $(CyberCore)/Modules/mediasession/MediaSessionCoordinatorMixin.idl \
    $(CyberCore)/Modules/mediasession/MediaSessionCoordinatorState.idl \
    $(CyberCore)/Modules/mediasession/MediaSessionPlaybackState.idl \
    $(CyberCore)/Modules/mediasession/MediaSessionPlaylistMixin.idl \
    $(CyberCore)/Modules/mediasession/MediaSessionReadyState.idl \
    $(CyberCore)/Modules/mediasession/Navigator+MediaSession.idl \
    $(CyberCore)/Modules/mediastream/CanvasCaptureMediaStreamTrack.idl \
    $(CyberCore)/Modules/mediastream/DoubleRange.idl \
    $(CyberCore)/Modules/mediastream/LongRange.idl \
    $(CyberCore)/Modules/mediastream/MediaDeviceInfo.idl \
    $(CyberCore)/Modules/mediastream/MediaDevices.idl \
    $(CyberCore)/Modules/mediastream/MediaStream.idl \
    $(CyberCore)/Modules/mediastream/MediaStreamTrack.idl \
    $(CyberCore)/Modules/mediastream/MediaStreamTrackEvent.idl \
    $(CyberCore)/Modules/mediastream/MediaTrackConstraints.idl \
    $(CyberCore)/Modules/mediastream/MediaTrackSupportedConstraints.idl \
    $(CyberCore)/Modules/mediastream/Navigator+MediaDevices.idl \
    $(CyberCore)/Modules/mediastream/OverconstrainedError.idl \
    $(CyberCore)/Modules/mediastream/OverconstrainedErrorEvent.idl \
    $(CyberCore)/Modules/mediastream/RTCAnswerOptions.idl \
    $(CyberCore)/Modules/mediastream/RTCCertificate.idl \
    $(CyberCore)/Modules/mediastream/RTCConfiguration.idl \
    $(CyberCore)/Modules/mediastream/RTCDTMFSender.idl \
    $(CyberCore)/Modules/mediastream/RTCDTMFToneChangeEvent.idl \
    $(CyberCore)/Modules/mediastream/RTCDataChannel.idl \
    $(CyberCore)/Modules/mediastream/RTCDataChannelEvent.idl \
    $(CyberCore)/Modules/mediastream/RTCDegradationPreference.idl \
    $(CyberCore)/Modules/mediastream/RTCDtlsTransport.idl \
    $(CyberCore)/Modules/mediastream/RTCDtlsTransportState.idl \
    $(CyberCore)/Modules/mediastream/RTCDtxStatus.idl \
    $(CyberCore)/Modules/mediastream/RTCEncodedAudioFrame.idl \
    $(CyberCore)/Modules/mediastream/RTCEncodedVideoFrame.idl \
    $(CyberCore)/Modules/mediastream/RTCError.idl \
    $(CyberCore)/Modules/mediastream/RTCErrorDetailType.idl \
    $(CyberCore)/Modules/mediastream/RTCErrorEvent.idl \
    $(CyberCore)/Modules/mediastream/RTCIceCandidate.idl \
    $(CyberCore)/Modules/mediastream/RTCIceCandidateInit.idl \
    $(CyberCore)/Modules/mediastream/RTCIceCandidateType.idl \
    $(CyberCore)/Modules/mediastream/RTCIceComponent.idl \
    $(CyberCore)/Modules/mediastream/RTCIceConnectionState.idl \
    $(CyberCore)/Modules/mediastream/RTCIceGatheringState.idl \
    $(CyberCore)/Modules/mediastream/RTCIceProtocol.idl \
    $(CyberCore)/Modules/mediastream/RTCIceServer.idl \
    $(CyberCore)/Modules/mediastream/RTCIceTcpCandidateType.idl \
    $(CyberCore)/Modules/mediastream/RTCIceTransport.idl \
    $(CyberCore)/Modules/mediastream/RTCIceTransportState.idl \
    $(CyberCore)/Modules/mediastream/RTCLocalSessionDescriptionInit.idl \
    $(CyberCore)/Modules/mediastream/RTCOfferAnswerOptions.idl \
    $(CyberCore)/Modules/mediastream/RTCOfferOptions.idl \
    $(CyberCore)/Modules/mediastream/RTCPeerConnection.idl \
    $(CyberCore)/Modules/mediastream/RTCPeerConnectionIceEvent.idl \
    $(CyberCore)/Modules/mediastream/RTCPeerConnectionIceErrorEvent.idl \
    $(CyberCore)/Modules/mediastream/RTCPeerConnectionState.idl \
    $(CyberCore)/Modules/mediastream/RTCPriorityType.idl \
    $(CyberCore)/Modules/mediastream/RTCRtcpParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpCapabilities.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpCodecCapability.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpCodecParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpCodingParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpContributingSource.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpEncodingParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpFecParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpHeaderExtensionParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpReceiver.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpReceiver+Transform.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpRtxParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpSendParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpSender.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpSender+Transform.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpSFrameTransform.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpSFrameTransformErrorEvent.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpScriptTransform.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpScriptTransformProvider.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpScriptTransformer.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpSynchronizationSource.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpTransceiver.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpTransceiverDirection.idl \
    $(CyberCore)/Modules/mediastream/RTCSctpTransport.idl \
    $(CyberCore)/Modules/mediastream/RTCSctpTransportState.idl \
    $(CyberCore)/Modules/mediastream/RTCSdpType.idl \
    $(CyberCore)/Modules/mediastream/RTCSessionDescription.idl \
    $(CyberCore)/Modules/mediastream/RTCSessionDescriptionInit.idl \
    $(CyberCore)/Modules/mediastream/RTCSignalingState.idl \
    $(CyberCore)/Modules/mediastream/RTCStatsReport.idl \
    $(CyberCore)/Modules/mediastream/RTCTrackEvent.idl \
    $(CyberCore)/Modules/mediastream/RTCTransformEvent.idl \
    $(CyberCore)/Modules/model-element/HTMLModelElement.idl \
    $(CyberCore)/Modules/model-element/HTMLModelElementCamera.idl \
    $(CyberCore)/Modules/notifications/Notification.idl \
    $(CyberCore)/Modules/notifications/NotificationDirection.idl \
    $(CyberCore)/Modules/notifications/NotificationEvent.idl \
    $(CyberCore)/Modules/notifications/NotificationOptions.idl \
    $(CyberCore)/Modules/notifications/NotificationPermission.idl \
    $(CyberCore)/Modules/notifications/NotificationPermissionCallback.idl \
    $(CyberCore)/Modules/paymentrequest/AddressErrors.idl \
    $(CyberCore)/Modules/paymentrequest/MerchantValidationEvent.idl \
    $(CyberCore)/Modules/paymentrequest/PayerErrorFields.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentAddress.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentComplete.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentCompleteDetails.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentCurrencyAmount.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentDetailsBase.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentDetailsInit.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentDetailsModifier.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentDetailsUpdate.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentItem.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentMethodChangeEvent.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentMethodData.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentOptions.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentRequest.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentRequestUpdateEvent.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentRequestUpdateEventInit.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentResponse.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentShippingOption.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentShippingType.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentValidationErrors.idl \
    $(CyberCore)/Modules/permissions/Navigator+Permissions.idl \
    $(CyberCore)/Modules/permissions/PermissionDescriptor.idl \
    $(CyberCore)/Modules/permissions/PermissionName.idl \
    $(CyberCore)/Modules/permissions/PermissionState.idl \
    $(CyberCore)/Modules/permissions/PermissionStatus.idl \
    $(CyberCore)/Modules/permissions/Permissions.idl \
    $(CyberCore)/Modules/permissions/WorkerNavigator+Permissions.idl \
    $(CyberCore)/Modules/pictureinpicture/DocumentOrShadowRoot+PictureInPicture.idl \
    $(CyberCore)/Modules/pictureinpicture/Document+PictureInPicture.idl \
    $(CyberCore)/Modules/pictureinpicture/HTMLVideoElement+PictureInPicture.idl \
    $(CyberCore)/Modules/pictureinpicture/PictureInPictureEvent.idl \
    $(CyberCore)/Modules/pictureinpicture/PictureInPictureWindow.idl \
    $(CyberCore)/Modules/push-api/PushEncryptionKeyName.idl \
    $(CyberCore)/Modules/push-api/PushEvent.idl \
    $(CyberCore)/Modules/push-api/PushEventInit.idl \
    $(CyberCore)/Modules/push-api/PushManager.idl \
    $(CyberCore)/Modules/push-api/PushMessageData.idl \
    $(CyberCore)/Modules/push-api/PushPermissionState.idl \
    $(CyberCore)/Modules/push-api/PushSubscription.idl \
    $(CyberCore)/Modules/push-api/PushSubscriptionChangeEvent.idl \
    $(CyberCore)/Modules/push-api/PushSubscriptionChangeEventInit.idl \
    $(CyberCore)/Modules/push-api/PushSubscriptionJSON.idl \
    $(CyberCore)/Modules/push-api/PushSubscriptionOptions.idl \
    $(CyberCore)/Modules/push-api/PushSubscriptionOptionsInit.idl \
    $(CyberCore)/Modules/push-api/ServiceWorkerGlobalScope+PushAPI.idl \
    $(CyberCore)/Modules/push-api/ServiceWorkerRegistration+PushAPI.idl \
    $(CyberCore)/Modules/remoteplayback/HTMLMediaElement+RemotePlayback.idl \
    $(CyberCore)/Modules/remoteplayback/RemotePlayback.idl \
    $(CyberCore)/Modules/remoteplayback/RemotePlaybackAvailabilityCallback.idl \
    $(CyberCore)/Modules/reporting/DeprecationReportBody.idl \
    $(CyberCore)/Modules/reporting/Report.idl \
    $(CyberCore)/Modules/reporting/ReportBody.idl \
    $(CyberCore)/Modules/reporting/ReportingObserver.idl \
    $(CyberCore)/Modules/reporting/ReportingObserverCallback.idl \
    $(CyberCore)/Modules/reporting/TestReportBody.idl \
    $(CyberCore)/Modules/screen-wake-lock/Navigator+ScreenWakeLock.idl \
    $(CyberCore)/Modules/screen-wake-lock/WakeLock.idl \
    $(CyberCore)/Modules/screen-wake-lock/WakeLockSentinel.idl \
    $(CyberCore)/Modules/screen-wake-lock/WakeLockType.idl \
    $(CyberCore)/Modules/speech/DOMWindow+SpeechSynthesis.idl \
    $(CyberCore)/Modules/speech/SpeechSynthesis.idl \
    $(CyberCore)/Modules/speech/SpeechSynthesisErrorCode.idl \
    $(CyberCore)/Modules/speech/SpeechSynthesisErrorEvent.idl \
    $(CyberCore)/Modules/speech/SpeechSynthesisErrorEventInit.idl \
    $(CyberCore)/Modules/speech/SpeechSynthesisEvent.idl \
    $(CyberCore)/Modules/speech/SpeechSynthesisEventInit.idl \
    $(CyberCore)/Modules/speech/SpeechSynthesisUtterance.idl \
    $(CyberCore)/Modules/speech/SpeechSynthesisVoice.idl \
    $(CyberCore)/Modules/speech/SpeechRecognition.idl \
    $(CyberCore)/Modules/speech/SpeechRecognitionAlternative.idl \
    $(CyberCore)/Modules/speech/SpeechRecognitionErrorCode.idl \
    $(CyberCore)/Modules/speech/SpeechRecognitionErrorEvent.idl \
    $(CyberCore)/Modules/speech/SpeechRecognitionEvent.idl \
    $(CyberCore)/Modules/speech/SpeechRecognitionResult.idl \
    $(CyberCore)/Modules/speech/SpeechRecognitionResultList.idl \
    $(CyberCore)/Modules/streams/ByteLengthQueuingStrategy.idl \
    $(CyberCore)/Modules/streams/CountQueuingStrategy.idl \
    $(CyberCore)/Modules/streams/GenericTransformStream.idl \
    $(CyberCore)/Modules/streams/ReadableByteStreamController.idl \
    $(CyberCore)/Modules/streams/ReadableStream.idl \
    $(CyberCore)/Modules/streams/ReadableStreamBYOBReader.idl \
    $(CyberCore)/Modules/streams/ReadableStreamBYOBRequest.idl \
    $(CyberCore)/Modules/streams/ReadableStreamDefaultController.idl \
    $(CyberCore)/Modules/streams/ReadableStreamDefaultReader.idl \
    $(CyberCore)/Modules/streams/ReadableStreamSink.idl \
    $(CyberCore)/Modules/streams/ReadableStreamSource.idl \
    $(CyberCore)/Modules/streams/TransformStream.idl \
    $(CyberCore)/Modules/streams/TransformStreamDefaultController.idl \
    $(CyberCore)/Modules/streams/WritableStream.idl \
    $(CyberCore)/Modules/streams/WritableStreamDefaultController.idl \
    $(CyberCore)/Modules/streams/WritableStreamDefaultWriter.idl \
    $(CyberCore)/Modules/streams/WritableStreamSink.idl \
    $(CyberCore)/Modules/storage/StorageManager.idl \
    $(CyberCore)/Modules/web-locks/NavigatorLocks.idl \
    $(CyberCore)/Modules/web-locks/WebLock.idl \
    $(CyberCore)/Modules/web-locks/WebLockGrantedCallback.idl \
    $(CyberCore)/Modules/web-locks/WebLockManager.idl \
    $(CyberCore)/Modules/web-locks/WebLockManagerSnapshot.idl \
    $(CyberCore)/Modules/web-locks/WebLockMode.idl \
    $(CyberCore)/Modules/webaudio/AnalyserNode.idl \
    $(CyberCore)/Modules/webaudio/AnalyserOptions.idl \
    $(CyberCore)/Modules/webaudio/AudioBuffer.idl \
    $(CyberCore)/Modules/webaudio/AudioBufferCallback.idl \
    $(CyberCore)/Modules/webaudio/AudioBufferOptions.idl \
    $(CyberCore)/Modules/webaudio/AudioBufferSourceNode.idl \
    $(CyberCore)/Modules/webaudio/AudioBufferSourceOptions.idl \
    $(CyberCore)/Modules/webaudio/AudioContext.idl \
    $(CyberCore)/Modules/webaudio/AudioContextLatencyCategory.idl \
    $(CyberCore)/Modules/webaudio/AudioContextOptions.idl \
    $(CyberCore)/Modules/webaudio/AudioContextState.idl \
    $(CyberCore)/Modules/webaudio/AudioDestinationNode.idl \
    $(CyberCore)/Modules/webaudio/AudioListener.idl \
    $(CyberCore)/Modules/webaudio/AudioNode.idl \
    $(CyberCore)/Modules/webaudio/AudioNodeOptions.idl \
    $(CyberCore)/Modules/webaudio/AudioParam.idl \
    $(CyberCore)/Modules/webaudio/AudioParamDescriptor.idl \
    $(CyberCore)/Modules/webaudio/AudioParamMap.idl \
    $(CyberCore)/Modules/webaudio/AudioProcessingEvent.idl \
    $(CyberCore)/Modules/webaudio/AudioProcessingEventInit.idl \
    $(CyberCore)/Modules/webaudio/AudioScheduledSourceNode.idl \
    $(CyberCore)/Modules/webaudio/AudioTimestamp.idl \
    $(CyberCore)/Modules/webaudio/AudioWorklet.idl \
    $(CyberCore)/Modules/webaudio/AudioWorkletGlobalScope.idl \
    $(CyberCore)/Modules/webaudio/AudioWorkletNode.idl \
    $(CyberCore)/Modules/webaudio/AudioWorkletNodeOptions.idl \
    $(CyberCore)/Modules/webaudio/AudioWorkletProcessor.idl \
    $(CyberCore)/Modules/webaudio/AudioWorkletProcessorConstructor.idl \
    $(CyberCore)/Modules/webaudio/AutomationRate.idl \
    $(CyberCore)/Modules/webaudio/BaseAudioContext.idl \
    $(CyberCore)/Modules/webaudio/BiquadFilterNode.idl \
    $(CyberCore)/Modules/webaudio/BiquadFilterOptions.idl \
    $(CyberCore)/Modules/webaudio/BiquadFilterType.idl \
    $(CyberCore)/Modules/webaudio/ChannelCountMode.idl \
    $(CyberCore)/Modules/webaudio/ChannelInterpretation.idl \
    $(CyberCore)/Modules/webaudio/ChannelMergerNode.idl \
    $(CyberCore)/Modules/webaudio/ChannelMergerOptions.idl \
    $(CyberCore)/Modules/webaudio/ChannelSplitterNode.idl \
    $(CyberCore)/Modules/webaudio/ChannelSplitterOptions.idl \
    $(CyberCore)/Modules/webaudio/ConstantSourceNode.idl \
    $(CyberCore)/Modules/webaudio/ConstantSourceOptions.idl \
    $(CyberCore)/Modules/webaudio/ConvolverNode.idl \
    $(CyberCore)/Modules/webaudio/ConvolverOptions.idl \
    $(CyberCore)/Modules/webaudio/DelayNode.idl \
    $(CyberCore)/Modules/webaudio/DelayOptions.idl \
    $(CyberCore)/Modules/webaudio/DistanceModelType.idl \
    $(CyberCore)/Modules/webaudio/DynamicsCompressorNode.idl \
    $(CyberCore)/Modules/webaudio/DynamicsCompressorOptions.idl \
    $(CyberCore)/Modules/webaudio/GainNode.idl \
    $(CyberCore)/Modules/webaudio/GainOptions.idl \
    $(CyberCore)/Modules/webaudio/IIRFilterNode.idl \
    $(CyberCore)/Modules/webaudio/IIRFilterOptions.idl \
    $(CyberCore)/Modules/webaudio/MediaElementAudioSourceNode.idl \
    $(CyberCore)/Modules/webaudio/MediaElementAudioSourceOptions.idl \
    $(CyberCore)/Modules/webaudio/MediaStreamAudioDestinationNode.idl \
    $(CyberCore)/Modules/webaudio/MediaStreamAudioSourceNode.idl \
    $(CyberCore)/Modules/webaudio/MediaStreamAudioSourceOptions.idl \
    $(CyberCore)/Modules/webaudio/OfflineAudioCompletionEvent.idl \
    $(CyberCore)/Modules/webaudio/OfflineAudioCompletionEventInit.idl \
    $(CyberCore)/Modules/webaudio/OfflineAudioContext.idl \
    $(CyberCore)/Modules/webaudio/OfflineAudioContextOptions.idl \
    $(CyberCore)/Modules/webaudio/OscillatorNode.idl \
    $(CyberCore)/Modules/webaudio/OscillatorOptions.idl \
    $(CyberCore)/Modules/webaudio/OscillatorType.idl \
    $(CyberCore)/Modules/webaudio/OverSampleType.idl \
    $(CyberCore)/Modules/webaudio/PannerNode.idl \
    $(CyberCore)/Modules/webaudio/PannerOptions.idl \
    $(CyberCore)/Modules/webaudio/PanningModelType.idl \
    $(CyberCore)/Modules/webaudio/PeriodicWave.idl \
    $(CyberCore)/Modules/webaudio/PeriodicWaveConstraints.idl \
    $(CyberCore)/Modules/webaudio/PeriodicWaveOptions.idl \
    $(CyberCore)/Modules/webaudio/ScriptProcessorNode.idl \
    $(CyberCore)/Modules/webaudio/StereoPannerNode.idl \
    $(CyberCore)/Modules/webaudio/StereoPannerOptions.idl \
    $(CyberCore)/Modules/webaudio/WaveShaperNode.idl \
    $(CyberCore)/Modules/webaudio/WaveShaperOptions.idl \
    $(CyberCore)/Modules/webauthn/AttestationConveyancePreference.idl \
    $(CyberCore)/Modules/webauthn/AuthenticationExtensionsClientInputs.idl \
    $(CyberCore)/Modules/webauthn/AuthenticationExtensionsClientOutputs.idl \
    $(CyberCore)/Modules/webauthn/AuthenticatorAssertionResponse.idl \
    $(CyberCore)/Modules/webauthn/AuthenticatorAttachment.idl \
    $(CyberCore)/Modules/webauthn/AuthenticatorAttestationResponse.idl \
    $(CyberCore)/Modules/webauthn/AuthenticatorResponse.idl \
    $(CyberCore)/Modules/webauthn/AuthenticatorTransport.idl \
    $(CyberCore)/Modules/webauthn/PublicKeyCredential.idl \
    $(CyberCore)/Modules/webauthn/PublicKeyCredentialCreationOptions.idl \
    $(CyberCore)/Modules/webauthn/PublicKeyCredentialDescriptor.idl \
    $(CyberCore)/Modules/webauthn/PublicKeyCredentialRequestOptions.idl \
    $(CyberCore)/Modules/webauthn/PublicKeyCredentialType.idl \
    $(CyberCore)/Modules/webauthn/ResidentKeyRequirement.idl \
    $(CyberCore)/Modules/webauthn/UserVerificationRequirement.idl \
    $(CyberCore)/Modules/webcodecs/AvcEncoderConfig.idl \
    $(CyberCore)/Modules/webcodecs/BitrateMode.idl \
    $(CyberCore)/Modules/webcodecs/LatencyMode.idl \
    $(CyberCore)/Modules/webcodecs/HardwareAcceleration.idl \
    $(CyberCore)/Modules/webcodecs/PlaneLayout.idl \
    $(CyberCore)/Modules/webcodecs/VideoColorPrimaries.idl \
    $(CyberCore)/Modules/webcodecs/VideoColorSpace.idl \
    $(CyberCore)/Modules/webcodecs/VideoColorSpaceInit.idl \
    $(CyberCore)/Modules/webcodecs/VideoMatrixCoefficients.idl \
    $(CyberCore)/Modules/webcodecs/VideoPixelFormat.idl \
    $(CyberCore)/Modules/webcodecs/VideoTransferCharacteristics.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsAlphaOption.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsEncodedVideoChunk.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsEncodedVideoChunkMetadata.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsEncodedVideoChunkOutputCallback.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsEncodedVideoChunkType.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsErrorCallback.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsCodecState.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsVideoDecoder.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsVideoDecoderConfig.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsVideoDecoderSupport.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsVideoEncoder.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsVideoEncoderConfig.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsVideoEncoderEncodeOptions.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsVideoEncoderSupport.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsVideoFrame.idl \
    $(CyberCore)/Modules/webcodecs/WebCodecsVideoFrameOutputCallback.idl \
    $(CyberCore)/Modules/webdatabase/DOMWindow+WebDatabase.idl \
    $(CyberCore)/Modules/webdatabase/Database.idl \
    $(CyberCore)/Modules/webdatabase/DatabaseCallback.idl \
    $(CyberCore)/Modules/webdatabase/SQLError.idl \
    $(CyberCore)/Modules/webdatabase/SQLResultSet.idl \
    $(CyberCore)/Modules/webdatabase/SQLResultSetRowList.idl \
    $(CyberCore)/Modules/webdatabase/SQLStatementCallback.idl \
    $(CyberCore)/Modules/webdatabase/SQLStatementErrorCallback.idl \
    $(CyberCore)/Modules/webdatabase/SQLTransaction.idl \
    $(CyberCore)/Modules/webdatabase/SQLTransactionCallback.idl \
    $(CyberCore)/Modules/webdatabase/SQLTransactionErrorCallback.idl \
    $(CyberCore)/Modules/webdriver/Navigator+WebDriver.idl \
    $(CyberCore)/Modules/websockets/CloseEvent.idl \
    $(CyberCore)/Modules/websockets/WebSocket.idl \
    $(CyberCore)/Modules/webxr/Navigator+WebXR.idl \
    $(CyberCore)/Modules/webxr/WebXRBoundedReferenceSpace.idl \
    $(CyberCore)/Modules/webxr/WebXRFrame+HandInput.idl \
    $(CyberCore)/Modules/webxr/WebXRFrame.idl \
    $(CyberCore)/Modules/webxr/WebXRHand.idl \
    $(CyberCore)/Modules/webxr/WebXRInputSource+Gamepad.idl \
    $(CyberCore)/Modules/webxr/WebXRInputSource.idl \
    $(CyberCore)/Modules/webxr/WebXRInputSourceArray.idl \
    $(CyberCore)/Modules/webxr/WebXRInputSource+HandInput.idl \
    $(CyberCore)/Modules/webxr/WebXRJointPose.idl \
    $(CyberCore)/Modules/webxr/WebXRJointSpace.idl \
    $(CyberCore)/Modules/webxr/WebXRLayer.idl \
    $(CyberCore)/Modules/webxr/WebXRPose.idl \
    $(CyberCore)/Modules/webxr/WebXRReferenceSpace.idl \
    $(CyberCore)/Modules/webxr/WebXRRenderState.idl \
    $(CyberCore)/Modules/webxr/WebXRRigidTransform.idl \
    $(CyberCore)/Modules/webxr/WebXRSession+AR.idl \
    $(CyberCore)/Modules/webxr/WebXRSession.idl \
    $(CyberCore)/Modules/webxr/WebXRSpace.idl \
    $(CyberCore)/Modules/webxr/WebXRSystem.idl \
    $(CyberCore)/Modules/webxr/WebXRViewerPose.idl \
    $(CyberCore)/Modules/webxr/WebXRView.idl \
    $(CyberCore)/Modules/webxr/WebXRViewport.idl \
    $(CyberCore)/Modules/webxr/WebXRWebGLLayer.idl \
    $(CyberCore)/Modules/webxr/XREnvironmentBlendMode.idl \
    $(CyberCore)/Modules/webxr/XREye.idl \
    $(CyberCore)/Modules/webxr/XRFrameRequestCallback.idl \
    $(CyberCore)/Modules/webxr/XRHandJoint.idl \
    $(CyberCore)/Modules/webxr/XRHandedness.idl \
    $(CyberCore)/Modules/webxr/XRInputSourceEvent.idl \
    $(CyberCore)/Modules/webxr/XRInputSourcesChangeEvent.idl \
    $(CyberCore)/Modules/webxr/XRInteractionMode.idl \
    $(CyberCore)/Modules/webxr/XRReferenceSpaceEvent.idl \
    $(CyberCore)/Modules/webxr/XRReferenceSpaceType.idl \
    $(CyberCore)/Modules/webxr/XRRenderStateInit.idl \
    $(CyberCore)/Modules/webxr/XRSessionEvent.idl \
    $(CyberCore)/Modules/webxr/XRSessionInit.idl \
    $(CyberCore)/Modules/webxr/XRSessionMode.idl \
    $(CyberCore)/Modules/webxr/XRTargetRayMode.idl \
    $(CyberCore)/Modules/webxr/XRVisibilityState.idl \
    $(CyberCore)/Modules/webxr/XRWebGLLayerInit.idl \
    $(CyberCore)/accessibility/AccessibilityRole.idl \
    $(CyberCore)/accessibility/AriaAttributes.idl \
    $(CyberCore)/animation/Animatable.idl \
    $(CyberCore)/animation/AnimationEffect.idl \
    $(CyberCore)/animation/AnimationFrameProvider.idl \
    $(CyberCore)/animation/AnimationFrameRatePreset.idl \
    $(CyberCore)/animation/AnimationPlaybackEvent.idl \
    $(CyberCore)/animation/AnimationPlaybackEventInit.idl \
    $(CyberCore)/animation/AnimationTimeline.idl \
    $(CyberCore)/animation/CSSAnimation.idl \
    $(CyberCore)/animation/CSSAnimationEvent.idl \
    $(CyberCore)/animation/CSSTransition.idl \
    $(CyberCore)/animation/CSSTransitionEvent.idl \
    $(CyberCore)/animation/CompositeOperation.idl \
    $(CyberCore)/animation/CompositeOperationOrAuto.idl \
    $(CyberCore)/animation/ComputedEffectTiming.idl \
    $(CyberCore)/animation/CustomAnimationOptions.idl \
    $(CyberCore)/animation/CustomEffect.idl \
    $(CyberCore)/animation/CustomEffectCallback.idl \
    $(CyberCore)/animation/Document+WebAnimations.idl \
    $(CyberCore)/animation/DocumentOrShadowRoot+WebAnimations.idl \
    $(CyberCore)/animation/DocumentTimeline.idl \
    $(CyberCore)/animation/DocumentTimelineOptions.idl \
    $(CyberCore)/animation/EffectTiming.idl \
    $(CyberCore)/animation/FillMode.idl \
    $(CyberCore)/animation/GetAnimationsOptions.idl \
    $(CyberCore)/animation/GlobalEventHandlers+CSSAnimations.idl \
    $(CyberCore)/animation/GlobalEventHandlers+CSSTransitions.idl \
    $(CyberCore)/animation/IterationCompositeOperation.idl \
    $(CyberCore)/animation/KeyframeAnimationOptions.idl \
    $(CyberCore)/animation/KeyframeEffect.idl \
    $(CyberCore)/animation/KeyframeEffectOptions.idl \
    $(CyberCore)/animation/OptionalEffectTiming.idl \
    $(CyberCore)/animation/PlaybackDirection.idl \
    $(CyberCore)/animation/WebAnimation.idl \
    $(CyberCore)/crypto/CryptoAlgorithmParameters.idl \
    $(CyberCore)/crypto/CryptoKey.idl \
    $(CyberCore)/crypto/CryptoKeyPair.idl \
    $(CyberCore)/crypto/CryptoKeyUsage.idl \
    $(CyberCore)/crypto/JsonWebKey.idl \
    $(CyberCore)/crypto/RsaOtherPrimesInfo.idl \
    $(CyberCore)/crypto/SubtleCrypto.idl \
    $(CyberCore)/crypto/keys/CryptoAesKeyAlgorithm.idl \
    $(CyberCore)/crypto/keys/CryptoEcKeyAlgorithm.idl \
    $(CyberCore)/crypto/keys/CryptoHmacKeyAlgorithm.idl \
    $(CyberCore)/crypto/keys/CryptoKeyAlgorithm.idl \
    $(CyberCore)/crypto/keys/CryptoRsaHashedKeyAlgorithm.idl \
    $(CyberCore)/crypto/keys/CryptoRsaKeyAlgorithm.idl \
    $(CyberCore)/crypto/parameters/AesCbcCfbParams.idl \
    $(CyberCore)/crypto/parameters/AesCtrParams.idl \
    $(CyberCore)/crypto/parameters/AesGcmParams.idl \
    $(CyberCore)/crypto/parameters/AesKeyParams.idl \
    $(CyberCore)/crypto/parameters/EcKeyParams.idl \
    $(CyberCore)/crypto/parameters/EcdhKeyDeriveParams.idl \
    $(CyberCore)/crypto/parameters/EcdsaParams.idl \
    $(CyberCore)/crypto/parameters/HkdfParams.idl \
    $(CyberCore)/crypto/parameters/HmacKeyParams.idl \
    $(CyberCore)/crypto/parameters/Pbkdf2Params.idl \
    $(CyberCore)/crypto/parameters/RsaHashedImportParams.idl \
    $(CyberCore)/crypto/parameters/RsaHashedKeyGenParams.idl \
    $(CyberCore)/crypto/parameters/RsaKeyGenParams.idl \
    $(CyberCore)/crypto/parameters/RsaOaepParams.idl \
    $(CyberCore)/crypto/parameters/RsaPssParams.idl \
    $(CyberCore)/css/CSSConditionRule.idl \
    $(CyberCore)/css/CSSContainerRule.idl \
    $(CyberCore)/css/CSSCounterStyleRule.idl \
    $(CyberCore)/css/CSSFontFaceRule.idl \
    $(CyberCore)/css/CSSFontFeatureValuesRule.idl \
    $(CyberCore)/css/CSSFontPaletteValuesRule.idl \
    $(CyberCore)/css/CSSGroupingRule.idl \
    $(CyberCore)/css/CSSImportRule.idl \
    $(CyberCore)/css/CSSImportRule+Layer.idl \
    $(CyberCore)/css/CSSLayerBlockRule.idl \
    $(CyberCore)/css/CSSLayerStatementRule.idl \
    $(CyberCore)/css/CSSKeyframeRule.idl \
    $(CyberCore)/css/CSSKeyframesRule.idl \
    $(CyberCore)/css/CSSMediaRule.idl \
    $(CyberCore)/css/CSSNamespaceRule.idl \
    $(CyberCore)/css/CSSPageRule.idl \
    $(CyberCore)/css/CSSPaintCallback.idl \
    $(CyberCore)/css/CSSPaintSize.idl \
    $(CyberCore)/css/CSSPropertyRule.idl \
    $(CyberCore)/css/CSSRule.idl \
    $(CyberCore)/css/CSSRuleList.idl \
    $(CyberCore)/css/CSSStyleDeclaration.idl \
    $(CyberCore)/css/CSSStyleRule.idl \
    $(CyberCore)/css/CSSStyleSheet.idl \
    $(CyberCore)/css/CSSSupportsRule.idl \
    $(CyberCore)/css/CSSUnknownRule.idl \
    $(CyberCore)/css/DocumentOrShadowRoot+CSSOM.idl \
    $(CyberCore)/css/DOMCSSCustomPropertyDescriptor.idl \
    $(CyberCore)/css/DOMCSSNamespace.idl \
    $(CyberCore)/css/DOMCSSNamespace+CSSNumericFactory.idl \
    $(CyberCore)/css/DOMCSSNamespace+CSSPainting.idl \
    $(CyberCore)/css/DOMCSSNamespace+CSSPropertiesandValues.idl \
    $(CyberCore)/css/DOMMatrix.idl \
    $(CyberCore)/css/DOMMatrix2DInit.idl \
    $(CyberCore)/css/DOMMatrixInit.idl \
    $(CyberCore)/css/DOMMatrixReadOnly.idl \
    $(CyberCore)/css/DeprecatedCSSOMCounter.idl \
    $(CyberCore)/css/DeprecatedCSSOMPrimitiveValue.idl \
    $(CyberCore)/css/DeprecatedCSSOMRGBColor.idl \
    $(CyberCore)/css/DeprecatedCSSOMRect.idl \
    $(CyberCore)/css/DeprecatedCSSOMValue.idl \
    $(CyberCore)/css/DeprecatedCSSOMValueList.idl \
    $(CyberCore)/css/ElementCSSInlineStyle.idl \
    $(CyberCore)/css/FontFace.idl \
    $(CyberCore)/css/FontFaceSet.idl \
    $(CyberCore)/css/FontFaceSource.idl \
    $(CyberCore)/css/LinkStyle.idl \
    $(CyberCore)/css/MediaList.idl \
    $(CyberCore)/css/MediaQueryList.idl \
    $(CyberCore)/css/MediaQueryListEvent.idl \
    $(CyberCore)/css/StyleMedia.idl \
    $(CyberCore)/css/StyleSheet.idl \
    $(CyberCore)/css/StyleSheetList.idl \
    $(CyberCore)/css/typedom/StylePropertyMap.idl \
    $(CyberCore)/css/typedom/StylePropertyMapReadOnly.idl \
	$(CyberCore)/css/typedom/CSSKeywordValue.idl \
    $(CyberCore)/css/typedom/CSSStyleImageValue.idl \
    $(CyberCore)/css/typedom/CSSNumericValue.idl \
    $(CyberCore)/css/typedom/CSSStyleValue.idl \
    $(CyberCore)/css/typedom/CSSUnitValue.idl \
    $(CyberCore)/css/typedom/CSSUnparsedValue.idl \
    $(CyberCore)/css/typedom/CSSOMVariableReferenceValue.idl \
    $(CyberCore)/css/typedom/color/CSSColor.idl \
    $(CyberCore)/css/typedom/color/CSSColorValue.idl \
    $(CyberCore)/css/typedom/color/CSSHSL.idl \
    $(CyberCore)/css/typedom/color/CSSHWB.idl \
    $(CyberCore)/css/typedom/color/CSSLCH.idl \
    $(CyberCore)/css/typedom/color/CSSLab.idl \
    $(CyberCore)/css/typedom/color/CSSOKLCH.idl \
    $(CyberCore)/css/typedom/color/CSSOKLab.idl \
    $(CyberCore)/css/typedom/color/CSSRGB.idl \
    $(CyberCore)/css/typedom/numeric/CSSMathClamp.idl \
    $(CyberCore)/css/typedom/numeric/CSSMathInvert.idl \
    $(CyberCore)/css/typedom/numeric/CSSMathMax.idl \
    $(CyberCore)/css/typedom/numeric/CSSMathMin.idl \
	$(CyberCore)/css/typedom/numeric/CSSMathNegate.idl \
    $(CyberCore)/css/typedom/numeric/CSSMathOperator.idl \
    $(CyberCore)/css/typedom/numeric/CSSMathProduct.idl \
    $(CyberCore)/css/typedom/numeric/CSSMathSum.idl \
    $(CyberCore)/css/typedom/numeric/CSSMathValue.idl \
    $(CyberCore)/css/typedom/numeric/CSSNumericArray.idl \
    $(CyberCore)/css/typedom/numeric/CSSNumericBaseType.idl \
    $(CyberCore)/css/typedom/numeric/CSSNumericType.idl \
	$(CyberCore)/css/typedom/transform/CSSMatrixComponent.idl \
	$(CyberCore)/css/typedom/transform/CSSMatrixComponentOptions.idl \
	$(CyberCore)/css/typedom/transform/CSSPerspective.idl \
	$(CyberCore)/css/typedom/transform/CSSRotate.idl \
	$(CyberCore)/css/typedom/transform/CSSScale.idl \
	$(CyberCore)/css/typedom/transform/CSSSkew.idl \
	$(CyberCore)/css/typedom/transform/CSSSkewX.idl \
	$(CyberCore)/css/typedom/transform/CSSSkewY.idl \
	$(CyberCore)/css/typedom/transform/CSSTransformComponent.idl \
	$(CyberCore)/css/typedom/transform/CSSTransformValue.idl \
	$(CyberCore)/css/typedom/transform/CSSTranslate.idl \
    $(CyberCore)/dom/AbortAlgorithm.idl \
    $(CyberCore)/dom/AbortController.idl \
    $(CyberCore)/dom/AbortSignal.idl \
    $(CyberCore)/dom/AbstractRange.idl \
    $(CyberCore)/dom/AddEventListenerOptions.idl \
    $(CyberCore)/dom/Attr.idl \
    $(CyberCore)/dom/BeforeUnloadEvent.idl \
    $(CyberCore)/dom/BroadcastChannel.idl \
    $(CyberCore)/dom/CDATASection.idl \
    $(CyberCore)/dom/CharacterData.idl \
    $(CyberCore)/dom/ChildNode.idl \
    $(CyberCore)/dom/ClipboardEvent.idl \
    $(CyberCore)/dom/Comment.idl \
    $(CyberCore)/dom/CompositionEvent.idl \
    $(CyberCore)/dom/CustomElementRegistry.idl \
    $(CyberCore)/dom/CustomEvent.idl \
    $(CyberCore)/dom/DOMException.idl \
    $(CyberCore)/dom/DOMImplementation.idl \
    $(CyberCore)/dom/DOMPoint.idl \
    $(CyberCore)/dom/DOMPointInit.idl \
    $(CyberCore)/dom/DOMPointReadOnly.idl \
    $(CyberCore)/dom/DOMQuad.idl \
    $(CyberCore)/dom/DOMQuadInit.idl \
    $(CyberCore)/dom/DOMRect.idl \
    $(CyberCore)/dom/DOMRectInit.idl \
    $(CyberCore)/dom/DOMRectList.idl \
    $(CyberCore)/dom/DOMRectReadOnly.idl \
    $(CyberCore)/dom/DOMStringList.idl \
    $(CyberCore)/dom/DOMStringMap.idl \
    $(CyberCore)/dom/DataTransfer.idl \
    $(CyberCore)/dom/DataTransferItem.idl \
    $(CyberCore)/dom/DataTransferItemList.idl \
    $(CyberCore)/dom/DeviceMotionEvent.idl \
    $(CyberCore)/dom/DeviceOrientationEvent.idl \
    $(CyberCore)/dom/DeviceOrientationOrMotionPermissionState.idl \
    $(CyberCore)/dom/Document.idl \
    $(CyberCore)/dom/Document+CSSOMView.idl \
    $(CyberCore)/dom/Document+Fullscreen.idl \
    $(CyberCore)/dom/Document+HTML.idl \
    $(CyberCore)/dom/Document+HTMLObsolete.idl \
    $(CyberCore)/dom/Document+PageVisibility.idl \
    $(CyberCore)/dom/Document+PointerLock.idl \
    $(CyberCore)/dom/Document+Selection.idl \
    $(CyberCore)/dom/Document+StorageAccess.idl \
    $(CyberCore)/dom/Document+UndoMananger.idl \
    $(CyberCore)/dom/DocumentAndElementEventHandlers.idl \
    $(CyberCore)/dom/DocumentFragment.idl \
    $(CyberCore)/dom/DocumentOrShadowRoot.idl \
    $(CyberCore)/dom/DocumentOrShadowRoot+Fullscreen.idl \
    $(CyberCore)/dom/DocumentOrShadowRoot+PointerLock.idl \
    $(CyberCore)/dom/DocumentType.idl \
    $(CyberCore)/dom/DragEvent.idl \
    $(CyberCore)/dom/Element+CSSOMView.idl \
    $(CyberCore)/dom/Element+ComputedStyleMap.idl \
    $(CyberCore)/dom/Element+DOMParsing.idl \
    $(CyberCore)/dom/Element+Fullscreen.idl \
    $(CyberCore)/dom/Element+PointerEvents.idl \
    $(CyberCore)/dom/Element+PointerLock.idl \
    $(CyberCore)/dom/Element.idl \
    $(CyberCore)/dom/ElementContentEditable.idl \
    $(CyberCore)/dom/ElementInternals.idl \
    $(CyberCore)/dom/ErrorEvent.idl \
    $(CyberCore)/dom/Event.idl \
    $(CyberCore)/dom/EventInit.idl \
    $(CyberCore)/dom/EventListener.idl \
    $(CyberCore)/dom/EventListenerOptions.idl \
    $(CyberCore)/dom/EventModifierInit.idl \
    $(CyberCore)/dom/EventTarget.idl \
    $(CyberCore)/dom/FocusEvent.idl \
    $(CyberCore)/dom/FocusOptions.idl \
    $(CyberCore)/dom/FormDataEvent.idl \
    $(CyberCore)/dom/FullscreenOptions.idl \
    $(CyberCore)/dom/GlobalEventHandlers+PointerEvents.idl \
    $(CyberCore)/dom/GlobalEventHandlers+Selection.idl \
    $(CyberCore)/dom/GlobalEventHandlers.idl \
    $(CyberCore)/dom/HashChangeEvent.idl \
    $(CyberCore)/dom/IdleDeadline.idl \
    $(CyberCore)/dom/IdleRequestCallback.idl \
    $(CyberCore)/dom/IdleRequestOptions.idl \
    $(CyberCore)/dom/InnerHTML.idl \
    $(CyberCore)/dom/InputEvent.idl \
    $(CyberCore)/dom/KeyboardEvent.idl \
    $(CyberCore)/dom/MessageChannel.idl \
    $(CyberCore)/dom/MessageEvent.idl \
    $(CyberCore)/dom/MessagePort.idl \
    $(CyberCore)/dom/MouseEvent.idl \
    $(CyberCore)/dom/MouseEventInit.idl \
    $(CyberCore)/dom/MutationCallback.idl \
    $(CyberCore)/dom/MutationEvent.idl \
    $(CyberCore)/dom/MutationObserver.idl \
    $(CyberCore)/dom/MutationRecord.idl \
    $(CyberCore)/dom/NamedNodeMap.idl \
    $(CyberCore)/dom/NavigatorMaxTouchPoints.idl \
    $(CyberCore)/dom/Node.idl \
    $(CyberCore)/dom/NodeFilter.idl \
    $(CyberCore)/dom/NodeIterator.idl \
    $(CyberCore)/dom/NodeList.idl \
    $(CyberCore)/dom/NonDocumentTypeChildNode.idl \
    $(CyberCore)/dom/NonElementParentNode.idl \
    $(CyberCore)/dom/OverflowEvent.idl \
    $(CyberCore)/dom/PageTransitionEvent.idl \
    $(CyberCore)/dom/ParentNode.idl \
    $(CyberCore)/dom/PointerEvent.idl \
    $(CyberCore)/dom/PopStateEvent.idl \
    $(CyberCore)/dom/ProcessingInstruction.idl \
    $(CyberCore)/dom/ProgressEvent.idl \
    $(CyberCore)/dom/PromiseRejectionEvent.idl \
    $(CyberCore)/dom/Range+CSSOMView.idl \
    $(CyberCore)/dom/Range+DOMParsing.idl \
    $(CyberCore)/dom/Range.idl \
    $(CyberCore)/dom/RequestAnimationFrameCallback.idl \
    $(CyberCore)/dom/SecurityPolicyViolationEvent.idl \
    $(CyberCore)/dom/SecurityPolicyViolationEventDisposition.idl \
    $(CyberCore)/dom/ShadowRoot.idl \
    $(CyberCore)/dom/ShadowRootInit.idl \
    $(CyberCore)/dom/ShadowRootMode.idl \
    $(CyberCore)/dom/SlotAssignmentMode.idl \
    $(CyberCore)/dom/Slotable.idl \
    $(CyberCore)/dom/StaticRange.idl \
    $(CyberCore)/dom/StringCallback.idl \
    $(CyberCore)/dom/Text.idl \
    $(CyberCore)/dom/TextDecoder.idl \
    $(CyberCore)/dom/TextDecoderStream.idl \
    $(CyberCore)/dom/TextDecoderStreamDecoder.idl \
    $(CyberCore)/dom/TextEncoder.idl \
    $(CyberCore)/dom/TextEncoderStream.idl \
    $(CyberCore)/dom/TextEncoderStreamEncoder.idl \
    $(CyberCore)/dom/TextEvent.idl \
    $(CyberCore)/dom/ToggleEvent.idl \
    $(CyberCore)/dom/TreeWalker.idl \
    $(CyberCore)/dom/UIEvent.idl \
    $(CyberCore)/dom/UIEventInit.idl \
    $(CyberCore)/dom/ValidityStateFlags.idl \
    $(CyberCore)/dom/VisibilityState.idl \
    $(CyberCore)/dom/WheelEvent.idl \
    $(CyberCore)/dom/XMLDocument.idl \
    $(CyberCore)/fileapi/Blob.idl \
    $(CyberCore)/fileapi/BlobCallback.idl \
    $(CyberCore)/fileapi/BlobPropertyBag.idl \
    $(CyberCore)/fileapi/EndingType.idl \
    $(CyberCore)/fileapi/File.idl \
    $(CyberCore)/fileapi/FileList.idl \
    $(CyberCore)/fileapi/FileReader.idl \
    $(CyberCore)/fileapi/FileReaderSync.idl \
    $(CyberCore)/html/DOMFormData.idl \
    $(CyberCore)/html/DOMTokenList.idl \
    $(CyberCore)/html/DOMURL.idl \
    $(CyberCore)/html/HTMLAllCollection.idl \
    $(CyberCore)/html/HTMLAnchorElement.idl \
    $(CyberCore)/html/HTMLAreaElement.idl \
    $(CyberCore)/html/HTMLAttachmentElement.idl \
    $(CyberCore)/html/HTMLAudioElement.idl \
    $(CyberCore)/html/HTMLBRElement.idl \
    $(CyberCore)/html/HTMLBaseElement.idl \
    $(CyberCore)/html/HTMLBodyElement+Compat.idl \
    $(CyberCore)/html/HTMLBodyElement.idl \
    $(CyberCore)/html/HTMLButtonElement.idl \
    $(CyberCore)/html/HTMLCanvasElement.idl \
    $(CyberCore)/html/HTMLCollection.idl \
    $(CyberCore)/html/HTMLDListElement.idl \
    $(CyberCore)/html/HTMLDataElement.idl \
    $(CyberCore)/html/HTMLDataListElement.idl \
    $(CyberCore)/html/HTMLDetailsElement.idl \
    $(CyberCore)/html/HTMLDialogElement.idl \
    $(CyberCore)/html/HTMLDirectoryElement.idl \
    $(CyberCore)/html/HTMLDivElement.idl \
    $(CyberCore)/html/HTMLDocument.idl \
    $(CyberCore)/html/HTMLElement+CSSOMView.idl \
    $(CyberCore)/html/HTMLElement.idl \
    $(CyberCore)/html/HTMLEmbedElement.idl \
    $(CyberCore)/html/HTMLFieldSetElement.idl \
    $(CyberCore)/html/HTMLFontElement.idl \
    $(CyberCore)/html/HTMLFormControlsCollection.idl \
    $(CyberCore)/html/HTMLFormElement.idl \
    $(CyberCore)/html/HTMLFrameElement.idl \
    $(CyberCore)/html/HTMLFrameSetElement.idl \
    $(CyberCore)/html/HTMLHRElement.idl \
    $(CyberCore)/html/HTMLHeadElement.idl \
    $(CyberCore)/html/HTMLHeadingElement.idl \
    $(CyberCore)/html/HTMLHtmlElement.idl \
    $(CyberCore)/html/HTMLHyperlinkElementUtils.idl \
    $(CyberCore)/html/HTMLIFrameElement.idl \
    $(CyberCore)/html/HTMLImageElement+CSSOMView.idl \
    $(CyberCore)/html/HTMLImageElement.idl \
    $(CyberCore)/html/HTMLInputElement.idl \
    $(CyberCore)/html/HTMLLIElement.idl \
    $(CyberCore)/html/HTMLLabelElement.idl \
    $(CyberCore)/html/HTMLLegendElement.idl \
    $(CyberCore)/html/HTMLLinkElement.idl \
    $(CyberCore)/html/HTMLMapElement.idl \
    $(CyberCore)/html/HTMLMarqueeElement.idl \
    $(CyberCore)/html/HTMLMediaElement+AudioOutput.idl \
    $(CyberCore)/html/HTMLMediaElement.idl \
    $(CyberCore)/html/HTMLMenuElement.idl \
    $(CyberCore)/html/HTMLMenuItemElement.idl \
    $(CyberCore)/html/HTMLMetaElement.idl \
    $(CyberCore)/html/HTMLMeterElement.idl \
    $(CyberCore)/html/HTMLModElement.idl \
    $(CyberCore)/html/HTMLOListElement.idl \
    $(CyberCore)/html/HTMLObjectElement.idl \
    $(CyberCore)/html/HTMLOptGroupElement.idl \
    $(CyberCore)/html/HTMLOptionElement.idl \
    $(CyberCore)/html/HTMLOptionsCollection.idl \
    $(CyberCore)/html/HTMLOrForeignElement.idl \
    $(CyberCore)/html/HTMLOutputElement.idl \
    $(CyberCore)/html/HTMLParagraphElement.idl \
    $(CyberCore)/html/HTMLParamElement.idl \
    $(CyberCore)/html/HTMLPictureElement.idl \
    $(CyberCore)/html/HTMLPreElement.idl \
    $(CyberCore)/html/HTMLProgressElement.idl \
    $(CyberCore)/html/HTMLQuoteElement.idl \
    $(CyberCore)/html/HTMLScriptElement.idl \
    $(CyberCore)/html/HTMLSelectElement.idl \
    $(CyberCore)/html/HTMLSlotElement.idl \
    $(CyberCore)/html/HTMLSourceElement.idl \
    $(CyberCore)/html/HTMLSpanElement.idl \
    $(CyberCore)/html/HTMLStyleElement.idl \
    $(CyberCore)/html/HTMLTableCaptionElement.idl \
    $(CyberCore)/html/HTMLTableCellElement.idl \
    $(CyberCore)/html/HTMLTableColElement.idl \
    $(CyberCore)/html/HTMLTableElement.idl \
    $(CyberCore)/html/HTMLTableRowElement.idl \
    $(CyberCore)/html/HTMLTableSectionElement.idl \
    $(CyberCore)/html/HTMLTemplateElement.idl \
    $(CyberCore)/html/HTMLTextAreaElement.idl \
    $(CyberCore)/html/HTMLTimeElement.idl \
    $(CyberCore)/html/HTMLTitleElement.idl \
    $(CyberCore)/html/HTMLTrackElement.idl \
    $(CyberCore)/html/HTMLUListElement.idl \
    $(CyberCore)/html/HTMLUnknownElement.idl \
    $(CyberCore)/html/HTMLVideoElement.idl \
    $(CyberCore)/html/HTMLVideoElement+RequestVideoFrameCallback.idl \
    $(CyberCore)/html/ImageBitmap.idl \
    $(CyberCore)/html/ImageBitmapOptions.idl \
    $(CyberCore)/html/ImageData.idl \
    $(CyberCore)/html/ImageDataSettings.idl \
    $(CyberCore)/html/MediaController.idl \
    $(CyberCore)/html/MediaEncryptedEvent.idl \
    $(CyberCore)/html/MediaError.idl \
    $(CyberCore)/html/OffscreenCanvas.idl \
    $(CyberCore)/html/RadioNodeList.idl \
    $(CyberCore)/html/SubmitEvent.idl \
    $(CyberCore)/html/TextMetrics.idl \
    $(CyberCore)/html/TimeRanges.idl \
    $(CyberCore)/html/URLSearchParams.idl \
    $(CyberCore)/html/UserActivation.idl \
    $(CyberCore)/html/ValidityState.idl \
    $(CyberCore)/html/VideoFrameMetadata.idl \
    $(CyberCore)/html/VideoFrameRequestCallback.idl \
    $(CyberCore)/html/VoidCallback.idl \
    $(CyberCore)/html/CyberKitMediaKeyError.idl \
    $(CyberCore)/html/canvas/ANGLEInstancedArrays.idl \
    $(CyberCore)/html/canvas/CanvasCompositing.idl \
    $(CyberCore)/html/canvas/CanvasDirection.idl \
    $(CyberCore)/html/canvas/CanvasDrawImage.idl \
    $(CyberCore)/html/canvas/CanvasDrawPath.idl \
    $(CyberCore)/html/canvas/CanvasFillRule.idl \
    $(CyberCore)/html/canvas/CanvasFillStrokeStyles.idl \
    $(CyberCore)/html/canvas/CanvasFilters.idl \
    $(CyberCore)/html/canvas/CanvasGradient.idl \
    $(CyberCore)/html/canvas/CanvasImageData.idl \
    $(CyberCore)/html/canvas/CanvasImageSmoothing.idl \
    $(CyberCore)/html/canvas/CanvasLineCap.idl \
    $(CyberCore)/html/canvas/CanvasLineJoin.idl \
    $(CyberCore)/html/canvas/CanvasPath.idl \
    $(CyberCore)/html/canvas/CanvasPathDrawingStyles.idl \
    $(CyberCore)/html/canvas/CanvasPattern.idl \
    $(CyberCore)/html/canvas/CanvasRect.idl \
    $(CyberCore)/html/canvas/CanvasRenderingContext2D.idl \
    $(CyberCore)/html/canvas/CanvasRenderingContext2DSettings.idl \
    $(CyberCore)/html/canvas/CanvasShadowStyles.idl \
    $(CyberCore)/html/canvas/CanvasState.idl \
    $(CyberCore)/html/canvas/CanvasText.idl \
    $(CyberCore)/html/canvas/CanvasTextAlign.idl \
    $(CyberCore)/html/canvas/CanvasTextBaseline.idl \
    $(CyberCore)/html/canvas/CanvasTextDrawingStyles.idl \
    $(CyberCore)/html/canvas/CanvasTransform.idl \
    $(CyberCore)/html/canvas/CanvasUserInterface.idl \
    $(CyberCore)/html/canvas/EXTBlendMinMax.idl \
    $(CyberCore)/html/canvas/EXTColorBufferFloat.idl \
    $(CyberCore)/html/canvas/EXTColorBufferHalfFloat.idl \
    $(CyberCore)/html/canvas/EXTDisjointTimerQuery.idl \
    $(CyberCore)/html/canvas/EXTDisjointTimerQueryWebGL2.idl \
    $(CyberCore)/html/canvas/EXTFloatBlend.idl \
    $(CyberCore)/html/canvas/EXTFragDepth.idl \
    $(CyberCore)/html/canvas/EXTShaderTextureLOD.idl \
    $(CyberCore)/html/canvas/EXTTextureCompressionBPTC.idl \
    $(CyberCore)/html/canvas/EXTTextureCompressionRGTC.idl \
    $(CyberCore)/html/canvas/EXTTextureFilterAnisotropic.idl \
    $(CyberCore)/html/canvas/EXTTextureNorm16.idl \
    $(CyberCore)/html/canvas/EXTsRGB.idl \
    $(CyberCore)/html/canvas/GPUCanvasContext.idl \
    $(CyberCore)/html/canvas/ImageBitmapRenderingContext.idl \
    $(CyberCore)/html/canvas/ImageBitmapRenderingContextSettings.idl \
    $(CyberCore)/html/canvas/ImageSmoothingQuality.idl \
    $(CyberCore)/html/canvas/KHRParallelShaderCompile.idl \
    $(CyberCore)/html/canvas/OESDrawBuffersIndexed.idl \
    $(CyberCore)/html/canvas/OESElementIndexUint.idl \
    $(CyberCore)/html/canvas/OESFBORenderMipmap.idl \
    $(CyberCore)/html/canvas/OESStandardDerivatives.idl \
    $(CyberCore)/html/canvas/OESTextureFloat.idl \
    $(CyberCore)/html/canvas/OESTextureFloatLinear.idl \
    $(CyberCore)/html/canvas/OESTextureHalfFloat.idl \
    $(CyberCore)/html/canvas/OESTextureHalfFloatLinear.idl \
    $(CyberCore)/html/canvas/OESVertexArrayObject.idl \
    $(CyberCore)/html/canvas/OffscreenCanvasRenderingContext2D.idl \
    $(CyberCore)/html/canvas/PaintRenderingContext2D.idl \
    $(CyberCore)/html/canvas/Path2D.idl \
    $(CyberCore)/html/canvas/PredefinedColorSpace.idl \
    $(CyberCore)/html/canvas/WebGL2RenderingContext.idl \
    $(CyberCore)/html/canvas/WebGLActiveInfo.idl \
    $(CyberCore)/html/canvas/WebGLBuffer.idl \
    $(CyberCore)/html/canvas/WebGLClipCullDistance.idl \
    $(CyberCore)/html/canvas/WebGLColorBufferFloat.idl \
    $(CyberCore)/html/canvas/WebGLCompressedTextureASTC.idl \
    $(CyberCore)/html/canvas/WebGLCompressedTextureETC.idl \
    $(CyberCore)/html/canvas/WebGLCompressedTextureETC1.idl \
    $(CyberCore)/html/canvas/WebGLCompressedTexturePVRTC.idl \
    $(CyberCore)/html/canvas/WebGLCompressedTextureS3TC.idl \
    $(CyberCore)/html/canvas/WebGLCompressedTextureS3TCsRGB.idl \
    $(CyberCore)/html/canvas/WebGLContextAttributes.idl \
    $(CyberCore)/html/canvas/WebGLContextEvent.idl \
    $(CyberCore)/html/canvas/WebGLDebugRendererInfo.idl \
    $(CyberCore)/html/canvas/WebGLDebugShaders.idl \
    $(CyberCore)/html/canvas/WebGLDepthTexture.idl \
    $(CyberCore)/html/canvas/WebGLDrawBuffers.idl \
    $(CyberCore)/html/canvas/WebGLDrawInstancedBaseVertexBaseInstance.idl \
    $(CyberCore)/html/canvas/WebGLFramebuffer.idl \
    $(CyberCore)/html/canvas/WebGLLoseContext.idl \
    $(CyberCore)/html/canvas/WebGLMultiDraw.idl \
    $(CyberCore)/html/canvas/WebGLMultiDrawInstancedBaseVertexBaseInstance.idl \
    $(CyberCore)/html/canvas/WebGLProgram.idl \
    $(CyberCore)/html/canvas/WebGLProvokingVertex.idl \
    $(CyberCore)/html/canvas/WebGLQuery.idl \
    $(CyberCore)/html/canvas/WebGLRenderbuffer.idl \
    $(CyberCore)/html/canvas/WebGLRenderingContext.idl \
    $(CyberCore)/html/canvas/WebGLRenderingContextBase.idl \
    $(CyberCore)/html/canvas/WebGLSampler.idl \
    $(CyberCore)/html/canvas/WebGLShader.idl \
    $(CyberCore)/html/canvas/WebGLShaderPrecisionFormat.idl \
    $(CyberCore)/html/canvas/WebGLSync.idl \
    $(CyberCore)/html/canvas/WebGLTexture.idl \
    $(CyberCore)/html/canvas/WebGLTimerQueryEXT.idl \
    $(CyberCore)/html/canvas/WebGLTransformFeedback.idl \
    $(CyberCore)/html/canvas/WebGLUniformLocation.idl \
    $(CyberCore)/html/canvas/WebGLVertexArrayObject.idl \
    $(CyberCore)/html/canvas/WebGLVertexArrayObjectOES.idl \
    $(CyberCore)/html/track/AudioTrack.idl \
    $(CyberCore)/html/track/AudioTrackConfiguration.idl \
    $(CyberCore)/html/track/AudioTrackList.idl \
    $(CyberCore)/html/track/DataCue.idl \
    $(CyberCore)/html/track/TextTrack.idl \
    $(CyberCore)/html/track/TextTrackCue.idl \
    $(CyberCore)/html/track/TextTrackCueGeneric.idl \
    $(CyberCore)/html/track/TextTrackCueList.idl \
    $(CyberCore)/html/track/TextTrackList.idl \
    $(CyberCore)/html/track/TrackEvent.idl \
    $(CyberCore)/html/track/VTTCue.idl \
    $(CyberCore)/html/track/VTTRegion.idl \
    $(CyberCore)/html/track/VTTRegionList.idl \
    $(CyberCore)/html/track/VideoTrack.idl \
    $(CyberCore)/html/track/VideoTrackConfiguration.idl \
    $(CyberCore)/html/track/VideoTrackList.idl \
    $(CyberCore)/mathml/MathMLElement.idl \
    $(CyberCore)/mathml/MathMLMathElement.idl \
    $(CyberCore)/inspector/CommandLineAPIHost.idl \
    $(CyberCore)/inspector/InspectorAuditAccessibilityObject.idl \
    $(CyberCore)/inspector/InspectorAuditDOMObject.idl \
    $(CyberCore)/inspector/InspectorAuditResourcesObject.idl \
    $(CyberCore)/inspector/InspectorFrontendHost.idl \
    $(CyberCore)/loader/COEPInheritenceViolationReportBody.idl \
    $(CyberCore)/loader/CORPViolationReportBody.idl \
    $(CyberCore)/loader/appcache/DOMApplicationCache.idl \
    $(CyberCore)/page/BarProp.idl \
    $(CyberCore)/page/Crypto.idl \
    $(CyberCore)/page/DOMSelection.idl \
    $(CyberCore)/page/DOMWindow+CSSOM.idl \
    $(CyberCore)/page/DOMWindow+CSSOMView.idl \
    $(CyberCore)/page/DOMWindow+Compat.idl \
    $(CyberCore)/page/DOMWindow+DeviceMotion.idl \
    $(CyberCore)/page/DOMWindow+DeviceOrientation.idl \
    $(CyberCore)/page/DOMWindow+RequestIdleCallback.idl \
    $(CyberCore)/page/DOMWindow+Selection.idl \
    $(CyberCore)/page/DOMWindow+VisualViewport.idl \
    $(CyberCore)/page/DOMWindow.idl \
    $(CyberCore)/page/EventSource.idl \
    $(CyberCore)/page/History.idl \
    $(CyberCore)/page/IntersectionObserver.idl \
    $(CyberCore)/page/IntersectionObserverCallback.idl \
    $(CyberCore)/page/IntersectionObserverEntry.idl \
    $(CyberCore)/page/Location.idl \
    $(CyberCore)/page/Navigator.idl \
    $(CyberCore)/page/Navigator+IsLoggedIn.idl \
    $(CyberCore)/page/Navigator+UserActivation.idl \
    $(CyberCore)/page/NavigatorCookies.idl \
    $(CyberCore)/page/NavigatorID.idl \
    $(CyberCore)/page/NavigatorLanguage.idl \
    $(CyberCore)/page/NavigatorOnLine.idl \
    $(CyberCore)/page/NavigatorPlugins.idl \
    $(CyberCore)/page/NavigatorServiceWorker.idl \
    $(CyberCore)/page/NavigatorShare.idl \
    $(CyberCore)/page/NavigatorStorage.idl \
    $(CyberCore)/page/Performance+NavigationTiming.idl \
    $(CyberCore)/page/Performance+PerformanceTimeline.idl \
    $(CyberCore)/page/Performance+ResourceTiming.idl \
    $(CyberCore)/page/Performance+UserTiming.idl \
    $(CyberCore)/page/Performance.idl \
    $(CyberCore)/page/PerformanceEntry.idl \
    $(CyberCore)/page/PerformanceMark.idl \
    $(CyberCore)/page/PerformanceMarkOptions.idl \
    $(CyberCore)/page/PerformanceMeasure.idl \
    $(CyberCore)/page/PerformanceMeasureOptions.idl \
    $(CyberCore)/page/PerformanceNavigation.idl \
    $(CyberCore)/page/PerformanceNavigationTiming.idl \
    $(CyberCore)/page/PerformanceObserver.idl \
    $(CyberCore)/page/PerformanceObserverCallback.idl \
    $(CyberCore)/page/PerformanceObserverEntryList.idl \
    $(CyberCore)/page/PerformancePaintTiming.idl \
    $(CyberCore)/page/PerformanceResourceTiming.idl \
    $(CyberCore)/page/PerformanceServerTiming.idl \
    $(CyberCore)/page/PerformanceTiming.idl \
    $(CyberCore)/page/RemoteDOMWindow.idl \
    $(CyberCore)/page/ResizeObserver.idl \
    $(CyberCore)/page/ResizeObserverBoxOptions.idl \
    $(CyberCore)/page/ResizeObserverCallback.idl \
    $(CyberCore)/page/ResizeObserverEntry.idl \
    $(CyberCore)/page/ResizeObserverOptions.idl \
    $(CyberCore)/page/ResizeObserverSize.idl \
    $(CyberCore)/page/Screen.idl \
    $(CyberCore)/page/ScreenOrientation.idl \
    $(CyberCore)/page/ScrollBehavior.idl \
    $(CyberCore)/page/ScrollIntoViewOptions.idl \
    $(CyberCore)/page/ScrollLogicalPosition.idl \
    $(CyberCore)/page/ScrollOptions.idl \
    $(CyberCore)/page/ScrollToOptions.idl \
    $(CyberCore)/page/ShadowRealmGlobalScope.idl \
    $(CyberCore)/page/ShareData.idl \
    $(CyberCore)/page/StructuredSerializeOptions.idl \
    $(CyberCore)/page/UndoItem.idl \
    $(CyberCore)/page/UndoManager.idl \
    $(CyberCore)/page/UserMessageHandler.idl \
    $(CyberCore)/page/UserMessageHandlersNamespace.idl \
    $(CyberCore)/page/VisualViewport.idl \
    $(CyberCore)/page/CyberKitNamespace.idl \
    $(CyberCore)/page/CyberKitPoint.idl \
    $(CyberCore)/page/WindowEventHandlers.idl \
    $(CyberCore)/page/WindowLocalStorage.idl \
    $(CyberCore)/page/WindowOrWorkerGlobalScope+Crypto.idl \
    $(CyberCore)/page/WindowOrWorkerGlobalScope+Performance.idl \
    $(CyberCore)/page/WindowOrWorkerGlobalScope.idl \
    $(CyberCore)/page/WindowSessionStorage.idl \
    $(CyberCore)/page/WorkerNavigator.idl \
    $(CyberCore)/page/csp/CSPViolationReportBody.idl \
    $(CyberCore)/plugins/DOMMimeType.idl \
    $(CyberCore)/plugins/DOMMimeTypeArray.idl \
    $(CyberCore)/plugins/DOMPlugin.idl \
    $(CyberCore)/plugins/DOMPluginArray.idl \
    $(CyberCore)/storage/Storage.idl \
    $(CyberCore)/storage/StorageEvent.idl \
    $(CyberCore)/svg/Document+SVG.idl \
    $(CyberCore)/svg/SVGAElement.idl \
    $(CyberCore)/svg/SVGAngle.idl \
    $(CyberCore)/svg/SVGAnimateColorElement.idl \
    $(CyberCore)/svg/SVGAnimateElement.idl \
    $(CyberCore)/svg/SVGAnimateMotionElement.idl \
    $(CyberCore)/svg/SVGAnimateTransformElement.idl \
    $(CyberCore)/svg/SVGAnimatedAngle.idl \
    $(CyberCore)/svg/SVGAnimatedBoolean.idl \
    $(CyberCore)/svg/SVGAnimatedEnumeration.idl \
    $(CyberCore)/svg/SVGAnimatedInteger.idl \
    $(CyberCore)/svg/SVGAnimatedLength.idl \
    $(CyberCore)/svg/SVGAnimatedLengthList.idl \
    $(CyberCore)/svg/SVGAnimatedNumber.idl \
    $(CyberCore)/svg/SVGAnimatedNumberList.idl \
    $(CyberCore)/svg/SVGAnimatedPreserveAspectRatio.idl \
    $(CyberCore)/svg/SVGAnimatedRect.idl \
    $(CyberCore)/svg/SVGAnimatedString.idl \
    $(CyberCore)/svg/SVGAnimatedTransformList.idl \
    $(CyberCore)/svg/SVGAnimationElement.idl \
    $(CyberCore)/svg/SVGCircleElement.idl \
    $(CyberCore)/svg/SVGClipPathElement.idl \
    $(CyberCore)/svg/SVGComponentTransferFunctionElement.idl \
    $(CyberCore)/svg/SVGDefsElement.idl \
    $(CyberCore)/svg/SVGDescElement.idl \
    $(CyberCore)/svg/SVGElement.idl \
    $(CyberCore)/svg/SVGEllipseElement.idl \
    $(CyberCore)/svg/SVGFEBlendElement.idl \
    $(CyberCore)/svg/SVGFEColorMatrixElement.idl \
    $(CyberCore)/svg/SVGFEComponentTransferElement.idl \
    $(CyberCore)/svg/SVGFECompositeElement.idl \
    $(CyberCore)/svg/SVGFEConvolveMatrixElement.idl \
    $(CyberCore)/svg/SVGFEDiffuseLightingElement.idl \
    $(CyberCore)/svg/SVGFEDisplacementMapElement.idl \
    $(CyberCore)/svg/SVGFEDistantLightElement.idl \
    $(CyberCore)/svg/SVGFEDropShadowElement.idl \
    $(CyberCore)/svg/SVGFEFloodElement.idl \
    $(CyberCore)/svg/SVGFEFuncAElement.idl \
    $(CyberCore)/svg/SVGFEFuncBElement.idl \
    $(CyberCore)/svg/SVGFEFuncGElement.idl \
    $(CyberCore)/svg/SVGFEFuncRElement.idl \
    $(CyberCore)/svg/SVGFEGaussianBlurElement.idl \
    $(CyberCore)/svg/SVGFEImageElement.idl \
    $(CyberCore)/svg/SVGFEMergeElement.idl \
    $(CyberCore)/svg/SVGFEMergeNodeElement.idl \
    $(CyberCore)/svg/SVGFEMorphologyElement.idl \
    $(CyberCore)/svg/SVGFEOffsetElement.idl \
    $(CyberCore)/svg/SVGFEPointLightElement.idl \
    $(CyberCore)/svg/SVGFESpecularLightingElement.idl \
    $(CyberCore)/svg/SVGFESpotLightElement.idl \
    $(CyberCore)/svg/SVGFETileElement.idl \
    $(CyberCore)/svg/SVGFETurbulenceElement.idl \
    $(CyberCore)/svg/SVGFilterElement.idl \
    $(CyberCore)/svg/SVGFilterPrimitiveStandardAttributes.idl \
    $(CyberCore)/svg/SVGFitToViewBox.idl \
    $(CyberCore)/svg/SVGForeignObjectElement.idl \
    $(CyberCore)/svg/SVGGElement.idl \
    $(CyberCore)/svg/SVGGeometryElement.idl \
    $(CyberCore)/svg/SVGGradientElement.idl \
    $(CyberCore)/svg/SVGGraphicsElement.idl \
    $(CyberCore)/svg/SVGImageElement.idl \
    $(CyberCore)/svg/SVGLength.idl \
    $(CyberCore)/svg/SVGLengthList.idl \
    $(CyberCore)/svg/SVGLineElement.idl \
    $(CyberCore)/svg/SVGLinearGradientElement.idl \
    $(CyberCore)/svg/SVGMPathElement.idl \
    $(CyberCore)/svg/SVGMarkerElement.idl \
    $(CyberCore)/svg/SVGMaskElement.idl \
    $(CyberCore)/svg/SVGMatrix.idl \
    $(CyberCore)/svg/SVGMetadataElement.idl \
    $(CyberCore)/svg/SVGNumber.idl \
    $(CyberCore)/svg/SVGNumberList.idl \
    $(CyberCore)/svg/SVGPathElement.idl \
    $(CyberCore)/svg/SVGPathSeg.idl \
    $(CyberCore)/svg/SVGPathSegArcAbs.idl \
    $(CyberCore)/svg/SVGPathSegArcRel.idl \
    $(CyberCore)/svg/SVGPathSegClosePath.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoCubicAbs.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoCubicRel.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoCubicSmoothAbs.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoCubicSmoothRel.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoQuadraticAbs.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoQuadraticRel.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoQuadraticSmoothAbs.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoQuadraticSmoothRel.idl \
    $(CyberCore)/svg/SVGPathSegLinetoAbs.idl \
    $(CyberCore)/svg/SVGPathSegLinetoHorizontalAbs.idl \
    $(CyberCore)/svg/SVGPathSegLinetoHorizontalRel.idl \
    $(CyberCore)/svg/SVGPathSegLinetoRel.idl \
    $(CyberCore)/svg/SVGPathSegLinetoVerticalAbs.idl \
    $(CyberCore)/svg/SVGPathSegLinetoVerticalRel.idl \
    $(CyberCore)/svg/SVGPathSegList.idl \
    $(CyberCore)/svg/SVGPathSegMovetoAbs.idl \
    $(CyberCore)/svg/SVGPathSegMovetoRel.idl \
    $(CyberCore)/svg/SVGPatternElement.idl \
    $(CyberCore)/svg/SVGPoint.idl \
    $(CyberCore)/svg/SVGPointList.idl \
    $(CyberCore)/svg/SVGPolygonElement.idl \
    $(CyberCore)/svg/SVGPolylineElement.idl \
    $(CyberCore)/svg/SVGPreserveAspectRatio.idl \
    $(CyberCore)/svg/SVGRadialGradientElement.idl \
    $(CyberCore)/svg/SVGRect.idl \
    $(CyberCore)/svg/SVGRectElement.idl \
    $(CyberCore)/svg/SVGRenderingIntent.idl \
    $(CyberCore)/svg/SVGSVGElement.idl \
    $(CyberCore)/svg/SVGScriptElement.idl \
    $(CyberCore)/svg/SVGSetElement.idl \
    $(CyberCore)/svg/SVGStopElement.idl \
    $(CyberCore)/svg/SVGStringList.idl \
    $(CyberCore)/svg/SVGStyleElement.idl \
    $(CyberCore)/svg/SVGSwitchElement.idl \
    $(CyberCore)/svg/SVGSymbolElement.idl \
    $(CyberCore)/svg/SVGTSpanElement.idl \
    $(CyberCore)/svg/SVGTests.idl \
    $(CyberCore)/svg/SVGTextContentElement.idl \
    $(CyberCore)/svg/SVGTextElement.idl \
    $(CyberCore)/svg/SVGTextPathElement.idl \
    $(CyberCore)/svg/SVGTextPositioningElement.idl \
    $(CyberCore)/svg/SVGTitleElement.idl \
    $(CyberCore)/svg/SVGTransform.idl \
    $(CyberCore)/svg/SVGTransformList.idl \
    $(CyberCore)/svg/SVGURIReference.idl \
    $(CyberCore)/svg/SVGUnitTypes.idl \
    $(CyberCore)/svg/SVGUseElement.idl \
    $(CyberCore)/svg/SVGViewElement.idl \
    $(CyberCore)/svg/SVGViewSpec.idl \
    $(CyberCore)/svg/SVGZoomAndPan.idl \
    $(CyberCore)/testing/GCObservation.idl \
    $(CyberCore)/testing/InternalSettings.idl \
    $(CyberCore)/testing/Internals.idl \
    $(CyberCore)/testing/InternalsMapLike.idl \
    $(CyberCore)/testing/InternalsSetLike.idl \
    $(CyberCore)/testing/MallocStatistics.idl \
    $(CyberCore)/testing/MemoryInfo.idl \
    $(CyberCore)/testing/MockCDMFactory.idl \
    $(CyberCore)/testing/MockContentFilterSettings.idl \
    $(CyberCore)/testing/MockPageOverlay.idl \
    $(CyberCore)/testing/MockPaymentAddress.idl \
    $(CyberCore)/testing/MockPaymentContactFields.idl \
    $(CyberCore)/testing/MockPaymentCoordinator.idl \
    $(CyberCore)/testing/MockPaymentError.idl \
    $(CyberCore)/testing/MockWebAuthenticationConfiguration.idl \
    $(CyberCore)/testing/ServiceWorkerInternals.idl \
    $(CyberCore)/testing/TypeConversions.idl \
    $(CyberCore)/testing/FakeXRBoundsPoint.idl \
    $(CyberCore)/testing/FakeXRButtonStateInit.idl \
    $(CyberCore)/testing/FakeXRInputSourceInit.idl \
    $(CyberCore)/testing/FakeXRJointStateInit.idl \
    $(CyberCore)/testing/FakeXRRigidTransformInit.idl \
    $(CyberCore)/testing/FakeXRViewInit.idl \
    $(CyberCore)/testing/WebFakeXRDevice.idl \
    $(CyberCore)/testing/WebFakeXRInputController.idl \
    $(CyberCore)/testing/WebXRTest.idl \
    $(CyberCore)/testing/XRSimulateUserActivationFunction.idl \
    $(CyberCore)/workers/AbstractWorker.idl \
    $(CyberCore)/workers/DedicatedWorkerGlobalScope.idl \
    $(CyberCore)/workers/Worker.idl \
    $(CyberCore)/workers/WorkerGlobalScope.idl \
    $(CyberCore)/workers/WorkerLocation.idl \
    $(CyberCore)/workers/WorkerOptions.idl \
    $(CyberCore)/workers/WorkerType.idl \
    $(CyberCore)/workers/service/ExtendableEvent.idl \
    $(CyberCore)/workers/service/ExtendableEventInit.idl \
    $(CyberCore)/workers/service/ExtendableMessageEvent.idl \
    $(CyberCore)/workers/service/FetchEvent.idl \
    $(CyberCore)/workers/service/NavigationPreloadManager.idl \
    $(CyberCore)/workers/service/NavigationPreloadState.idl \
    $(CyberCore)/workers/service/ServiceWorker.idl \
    $(CyberCore)/workers/service/ServiceWorkerClient.idl \
    $(CyberCore)/workers/service/ServiceWorkerClientType.idl \
    $(CyberCore)/workers/service/ServiceWorkerClients.idl \
    $(CyberCore)/workers/service/ServiceWorkerContainer.idl \
    $(CyberCore)/workers/service/ServiceWorkerGlobalScope.idl \
    $(CyberCore)/workers/service/ServiceWorkerRegistration.idl \
    $(CyberCore)/workers/service/ServiceWorkerUpdateViaCache.idl \
    $(CyberCore)/workers/service/ServiceWorkerWindowClient.idl \
    $(CyberCore)/workers/service/background-fetch/BackgroundFetchEvent.idl \
    $(CyberCore)/workers/service/background-fetch/BackgroundFetchEventInit.idl \
    $(CyberCore)/workers/service/background-fetch/BackgroundFetchFailureReason.idl \
    $(CyberCore)/workers/service/background-fetch/BackgroundFetchManager.idl \
    $(CyberCore)/workers/service/background-fetch/BackgroundFetchOptions.idl \
    $(CyberCore)/workers/service/background-fetch/BackgroundFetchRecord.idl \
    $(CyberCore)/workers/service/background-fetch/BackgroundFetchRegistration.idl \
    $(CyberCore)/workers/service/background-fetch/BackgroundFetchResult.idl \
    $(CyberCore)/workers/service/background-fetch/BackgroundFetchUIOptions.idl \
    $(CyberCore)/workers/service/background-fetch/BackgroundFetchUpdateUIEvent.idl \
    $(CyberCore)/workers/service/background-fetch/ImageResource.idl \
    $(CyberCore)/workers/service/background-fetch/ServiceWorkerGlobalScope+BackgroundFetchAPI.idl \
    $(CyberCore)/workers/service/background-fetch/ServiceWorkerRegistration+BackgroundFetchAPI.idl \
    $(CyberCore)/workers/shared/SharedWorker.idl \
    $(CyberCore)/workers/shared/SharedWorkerGlobalScope.idl \
    $(CyberCore)/worklets/PaintWorkletGlobalScope.idl \
    $(CyberCore)/worklets/Worklet.idl \
    $(CyberCore)/worklets/WorkletGlobalScope.idl \
    $(CyberCore)/worklets/WorkletOptions.idl \
    $(CyberCore)/xml/CustomXPathNSResolver.idl \
    $(CyberCore)/xml/DOMParser.idl \
    $(CyberCore)/xml/ParseFromStringOptions.idl \
    $(CyberCore)/xml/XMLHttpRequest.idl \
    $(CyberCore)/xml/XMLHttpRequestEventTarget.idl \
    $(CyberCore)/xml/XMLHttpRequestProgressEvent.idl \
    $(CyberCore)/xml/XMLHttpRequestUpload.idl \
    $(CyberCore)/xml/XMLSerializer.idl \
    $(CyberCore)/xml/XPathEvaluator.idl \
    $(CyberCore)/xml/XPathEvaluatorBase.idl \
    $(CyberCore)/xml/XPathExpression.idl \
    $(CyberCore)/xml/XPathNSResolver.idl \
    $(CyberCore)/xml/XPathResult.idl \
    $(CyberCore)/xml/XSLTProcessor.idl \
    InternalSettingsGenerated.idl \
    CSSStyleDeclaration+PropertyNames.idl \
#

# --------

ADDITIONAL_BINDING_IDLS = \
    DocumentTouch.idl \
    GestureEvent.idl \
    Internals+Additions.idl \
    InternalsAdditions.idl \
    Touch.idl \
    TouchEvent.idl \
    TouchList.idl \
#

vpath %.in $(WEBKITADDITIONS_HEADER_SEARCH_PATHS)

ADDITIONAL_EVENT_NAMES =
ADDITIONAL_EVENT_TARGET_FACTORY =

-include CyberCoreDerivedSourcesAdditions.make

# Convert the "bare" IDL names in ADDITIONAL_BINDING_IDLS to full paths by
# looking for those files in the expected locations: in /usr/local/include in
# the build output directory, in /usr/local/include in the SDK, or in any of
# the paths indicated in JS_BINDING_IDLS.
#
# The first step is to get the paths of the IDL files in JS_BINDING_IDLS. We
# take just the directory parts, convert them to something that has symlinks and
# other non-canonical parts resolved, and then sort them (which removes
# duplicates).
#
# Next we prepend the "bare" IDL names with the locations in
# BUILD_PRODUCTS_DIR, SDKROOT, and the paths generated from JS_BINDING_IDLS
# until we find each file. The resulting full path is added to JS_BINDING_IDLS.

IDL_PATHS := $(sort $(foreach IDL_FILE, $(JS_BINDING_IDLS), $(realpath $(dir $(IDL_FILE)))))

ADDITIONS_PATHS = \
    $(BUILT_PRODUCTS_DIR)$(WK_LIBRARY_HEADERS_FOLDER_PATH)/CyberKitAdditions \
    $(SDKROOT)$(WK_LIBRARY_HEADERS_FOLDER_PATH)/CyberKitAdditions

ADDITIONAL_BINDING_IDLS_PATHS = \
    $(ADDITIONS_PATHS) \
    $(IDL_PATHS)

JS_BINDING_IDLS += \
    $(foreach \
        IDL_FILE, \
        $(ADDITIONAL_BINDING_IDLS), \
        $(firstword $(realpath $(foreach \
            IDL_PATH, \
            $(ADDITIONAL_BINDING_IDLS_PATHS), \
            $(IDL_PATH)/$(IDL_FILE)))))

.PHONY : all

get_bare_name = $(basename $(notdir $(1)))
JS_DOM_CLASSES=$(call get_bare_name,$(JS_BINDING_IDLS))

JS_DOM_HEADERS=$(filter-out JSEventListener.h, $(JS_DOM_CLASSES:%=JS%.h))
JS_DOM_IMPLEMENTATIONS=$(filter-out JSEventListener.cpp, $(JS_DOM_CLASSES:%=JS%.cpp))

WEB_DOM_HEADERS :=

all : \
    $(JS_DOM_HEADERS) \
    $(JS_DOM_IMPLEMENTATIONS) \
    $(WEB_DOM_HEADERS) \
    \
    CSSPropertyNames.cpp \
    CSSPropertyNames.h \
    CSSPropertyParsing.cpp \
    CSSPropertyParsing.h \
    CSSValueKeywords.cpp \
    CSSValueKeywords.h \
    ColorData.cpp \
    DOMJITAbstractHeapRepository.h \
    ElementName.cpp \
    ElementName.h \
    EventInterfaces.h \
    EventTargetInterfaces.h \
    HTMLElementFactory.cpp \
    HTMLElementFactory.h \
    HTMLElementTypeHelpers.h \
    HTMLEntityTable.cpp \
    HTMLNames.cpp \
    HTMLNames.h \
    JSHTMLElementWrapperFactory.cpp \
    JSHTMLElementWrapperFactory.h \
    JSMathMLElementWrapperFactory.cpp \
    JSMathMLElementWrapperFactory.h \
    JSSVGElementWrapperFactory.cpp \
    JSSVGElementWrapperFactory.h \
    LocalizableAdditions.strings.out \
    Namespace.cpp \
    Namespace.h \
    SVGElementFactory.cpp \
    SVGElementFactory.h \
    SVGElementTypeHelpers.h \
    SVGNames.cpp \
    SVGNames.h \
    SelectorPseudoClassAndCompatibilityElementMap.cpp \
    SelectorPseudoElementTypeMap.cpp \
    StyleBuilderGenerated.cpp \
    StylePropertyShorthandFunctions.cpp \
    StylePropertyShorthandFunctions.h \
    TagName.cpp \
    TagName.h \
    CSSStyleDeclaration+PropertyNames.idl \
    CyberKitFontFamilyNames.cpp \
    CyberKitFontFamilyNames.h \
    MathMLElementFactory.cpp \
    MathMLElementFactory.h \
    MathMLElementTypeHelpers.h \
    MathMLNames.cpp \
    MathMLNames.h \
#

# --------

# CSS property names and value keywords

WEBCORE_CSS_PROPERTY_NAMES := $(CyberCore)/css/CSSProperties.json
WEBCORE_CSS_VALUE_KEYWORDS := $(CyberCore)/css/CSSValueKeywords.in
WEBCORE_CSS_VALUE_KEYWORDS := $(WEBCORE_CSS_VALUE_KEYWORDS) $(CyberCore)/css/SVGCSSValueKeywords.in

CSS_PROPERTY_NAME_FILES = \
    CSSPropertyNames.cpp \
    CSSPropertyNames.h \
    CSSPropertyParsing.cpp \
    CSSPropertyParsing.h \
    CSSStyleDeclaration+PropertyNames.idl \
    StyleBuilderGenerated.cpp \
    StylePropertyShorthandFunctions.cpp \
    StylePropertyShorthandFunctions.h \
#
CSS_PROPERTY_NAME_FILES_PATTERNS = $(subst .,%,$(CSS_PROPERTY_NAME_FILES))

all : $(CSS_PROPERTY_NAME_FILES)
$(CSS_PROPERTY_NAME_FILES_PATTERNS) : $(WEBCORE_CSS_PROPERTY_NAMES) $(CyberCore)/css/process-css-properties.py $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PERL) -pe '' $(WEBCORE_CSS_PROPERTY_NAMES) > CSSProperties.json
	$(PYTHON) "$(CyberCore)/css/process-css-properties.py" --gperf-executable gperf --defines "$(FEATURE_AND_PLATFORM_DEFINES)"

CSS_VALUE_KEYWORD_FILES = \
    CSSValueKeywords.h \
    CSSValueKeywords.cpp \
#
CSS_VALUE_KEYWORD_FILES_PATTERNS = $(subst .,%,$(CSS_VALUE_KEYWORD_FILES))

all : $(CSS_VALUE_KEYWORD_FILES)
$(CSS_VALUE_KEYWORD_FILES_PATTERNS) : $(WEBCORE_CSS_VALUE_KEYWORDS) $(CyberCore)/css/process-css-values.py $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PERL) -pe '' $(WEBCORE_CSS_VALUE_KEYWORDS) > CSSValueKeywords.in
	$(PYTHON) "$(CyberCore)/css/process-css-values.py" --gperf-executable gperf --defines "$(FEATURE_AND_PLATFORM_DEFINES)"

# --------

# CSS Selector pseudo type name to value map.

SelectorPseudoClassAndCompatibilityElementMap.cpp : $(CyberCore)/css/makeSelectorPseudoClassAndCompatibilityElementMap.py $(CyberCore)/css/SelectorPseudoClassAndCompatibilityElementMap.in $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PYTHON) "$(CyberCore)/css/makeSelectorPseudoClassAndCompatibilityElementMap.py" $(CyberCore)/css/SelectorPseudoClassAndCompatibilityElementMap.in gperf "$(FEATURE_AND_PLATFORM_DEFINES)"

SelectorPseudoElementTypeMap.cpp : $(CyberCore)/css/makeSelectorPseudoElementsMap.py $(CyberCore)/css/SelectorPseudoElementTypeMap.in $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PYTHON) "$(CyberCore)/css/makeSelectorPseudoElementsMap.py" $(CyberCore)/css/SelectorPseudoElementTypeMap.in gperf "$(FEATURE_AND_PLATFORM_DEFINES)"

# --------

# DOMJIT Abstract Heap

all : DOMJITAbstractHeapRepository.h

DOMJITAbstractHeapRepository.h : $(CyberCore)/domjit/generate-abstract-heap.rb $(CyberCore)/domjit/DOMJITAbstractHeapRepository.yaml
	$(RUBY) "$(CyberCore)/domjit/generate-abstract-heap.rb" $(CyberCore)/domjit/DOMJITAbstractHeapRepository.yaml DOMJITAbstractHeapRepository.h

# --------

# XMLViewer CSS

all : XMLViewerCSS.h

XMLViewerCSS.h : $(CyberCore)/xml/XMLViewer.css
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/cssmin.py < "$(CyberCore)/xml/XMLViewer.css" > XMLViewer.min.css
	$(PERL) $(CyberScriptCore_SCRIPTS_DIR)/xxd.pl XMLViewer_css XMLViewer.min.css XMLViewerCSS.h
	$(DELETE) XMLViewer.min.css

# --------

# XMLViewer JS

all : XMLViewerJS.h

XMLViewerJS.h : $(CyberCore)/xml/XMLViewer.js
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/jsmin.py < "$(CyberCore)/xml/XMLViewer.js" > XMLViewer.min.js
	$(PERL) $(CyberScriptCore_SCRIPTS_DIR)/xxd.pl XMLViewer_js XMLViewer.min.js XMLViewerJS.h
	$(DELETE) XMLViewer.min.js

# --------

# HTML entity names

HTMLEntityTable.cpp : $(CyberCore)/html/parser/HTMLEntityNames.in $(CyberCore)/html/parser/create-html-entity-table
	$(PYTHON) $(CyberCore)/html/parser/create-html-entity-table -o HTMLEntityTable.cpp $(CyberCore)/html/parser/HTMLEntityNames.in

# --------

# HTTP header names

HTTP_HEADER_NAMES_FILES = \
    HTTPHeaderNames.cpp \
    HTTPHeaderNames.gperf \
    HTTPHeaderNames.h \
#
HTTP_HEADER_NAMES_FILES_PATTERNS = $(subst .,%,$(HTTP_HEADER_NAMES_FILES))

all : $(HTTP_HEADER_NAMES_FILES)
$(HTTP_HEADER_NAMES_FILES_PATTERNS) : $(CyberCore)/platform/network/HTTPHeaderNames.in $(CyberCore)/platform/network/create-http-header-name-table
	$(PYTHON) $(CyberCore)/platform/network/create-http-header-name-table $(CyberCore)/platform/network/HTTPHeaderNames.in gperf

# --------

# color names

ColorData.cpp : $(CyberCore)/platform/ColorData.gperf $(CyberCore)/make-hash-tools.pl
	$(PERL) $(CyberCore)/make-hash-tools.pl . $(CyberCore)/platform/ColorData.gperf

# --------

# .strings files

POSSIBLE_LOCALIZABLE_STRINGS_FILES := $(wildcard $(foreach ADDITIONS_PATH,$(ADDITIONS_PATHS),$(ADDITIONS_PATH)/LocalizableAdditions.strings.txt))

LOCALIZABLE_STRINGS_FILE = $(word 1,$(POSSIBLE_LOCALIZABLE_STRINGS_FILES))

LocalizableAdditions.strings.out : $(CyberCore)/preprocess-localizable-strings.pl $(CyberCore)/bindings/scripts/preprocessor.pm $(LOCALIZABLE_STRINGS_FILE) $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PERL) $< --defines "$(FEATURE_AND_PLATFORM_DEFINES)" $@ $(LOCALIZABLE_STRINGS_FILE)

# --------

# modern media controls

POSSIBLE_ADDITIONAL_MODERN_MEDIA_CONTROLS_STYLE_SHEETS = \
    $(foreach \
        STYLE_SHEET, \
        $(ADDITIONAL_MODERN_MEDIA_CONTROLS_STYLE_SHEETS), \
        $(firstword $(realpath $(foreach \
            ADDITIONS_PATH, \
            $(ADDITIONS_PATHS), \
            $(ADDITIONS_PATH)/$(STYLE_SHEET)))))

MODERN_MEDIA_CONTROLS_STYLE_SHEETS = \
    $(CyberCore)/Modules/modern-media-controls/controls/activity-indicator.css \
    $(CyberCore)/Modules/modern-media-controls/controls/airplay-button.css \
    $(CyberCore)/Modules/modern-media-controls/controls/background-tint.css \
    $(CyberCore)/Modules/modern-media-controls/controls/button.css \
    $(CyberCore)/Modules/modern-media-controls/controls/buttons-container.css \
    $(CyberCore)/Modules/modern-media-controls/controls/controls-bar.css \
    $(CyberCore)/Modules/modern-media-controls/controls/inline-media-controls.css \
    $(CyberCore)/Modules/modern-media-controls/controls/ios-inline-media-controls.css \
    $(CyberCore)/Modules/modern-media-controls/controls/macos-fullscreen-media-controls.css \
    $(CyberCore)/Modules/modern-media-controls/controls/macos-inline-media-controls.css \
    $(CyberCore)/Modules/modern-media-controls/controls/media-controls.css \
    $(CyberCore)/Modules/modern-media-controls/controls/media-document.css \
    $(CyberCore)/Modules/modern-media-controls/controls/placard.css \
    $(CyberCore)/Modules/modern-media-controls/controls/slider-base.css \
    $(CyberCore)/Modules/modern-media-controls/controls/slider.css \
    $(CyberCore)/Modules/modern-media-controls/controls/status-label.css \
    $(CyberCore)/Modules/modern-media-controls/controls/text-tracks.css \
    $(CyberCore)/Modules/modern-media-controls/controls/time-label.css \
    $(CyberCore)/Modules/modern-media-controls/controls/watchos-activity-indicator.css \
    $(CyberCore)/Modules/modern-media-controls/controls/watchos-media-controls.css \
    $(POSSIBLE_ADDITIONAL_MODERN_MEDIA_CONTROLS_STYLE_SHEETS) \
#

all : ModernMediaControls.css

ModernMediaControls.css : $(MODERN_MEDIA_CONTROLS_STYLE_SHEETS)
	cat $^ > ModernMediaControls.css

# user agent style sheets

POSSIBLE_ADDITIONAL_USER_AGENT_STYLE_SHEETS = \
    $(foreach \
        STYLE_SHEET, \
        $(ADDITIONAL_USER_AGENT_STYLE_SHEETS), \
        $(firstword $(realpath $(foreach \
            ADDITIONS_PATH, \
            $(ADDITIONS_PATHS), \
            $(ADDITIONS_PATH)/$(STYLE_SHEET)))))

USER_AGENT_STYLE_SHEETS = \
    $(CyberCore)/css/counterStyles.css \
    $(CyberCore)/css/dialog.css \
    $(CyberCore)/css/fullscreen.css \
    $(CyberCore)/css/horizontalFormControls.css \
    $(CyberCore)/css/html.css \
    $(CyberCore)/css/legacyFormControlsIOS.css \
    $(CyberCore)/css/mathml.css \
    $(CyberCore)/css/mediaControls.css \
    $(CyberCore)/css/plugIns.css \
    $(CyberCore)/css/popover.css \
    $(CyberCore)/css/quirks.css \
    $(CyberCore)/css/svg.css \
    $(CyberCore)/html/shadow/mac/imageControlsMac.css \
    $(CyberCore)/html/shadow/attachmentElementShadow.css \
    $(CyberCore)/html/shadow/imageOverlay.css \
    $(CyberCore)/html/shadow/meterElementShadow.css \
    ModernMediaControls.css \
    $(POSSIBLE_ADDITIONAL_USER_AGENT_STYLE_SHEETS) \
#

USER_AGENT_STYLE_SHEETS_FILES = UserAgentStyleSheets.h UserAgentStyleSheetsData.cpp
USER_AGENT_STYLE_SHEETS_FILES_PATTERNS = $(subst .,%,$(USER_AGENT_STYLE_SHEETS_FILES))

all : $(USER_AGENT_STYLE_SHEETS_FILES)

$(USER_AGENT_STYLE_SHEETS_FILES_PATTERNS) : $(CyberCore)/css/make-css-file-arrays.pl $(CyberCore)/bindings/scripts/preprocessor.pm $(USER_AGENT_STYLE_SHEETS) $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PERL) $< --defines "$(FEATURE_AND_PLATFORM_DEFINES)" $(USER_AGENT_STYLE_SHEETS_FILES) $(USER_AGENT_STYLE_SHEETS)

# --------

# modern media controls

POSSIBLE_ADDITIONAL_MODERN_MEDIA_CONTROLS_SCRIPTS = \
    $(foreach \
        SCRIPT, \
        $(ADDITIONAL_MODERN_MEDIA_CONTROLS_SCRIPTS), \
        $(firstword $(realpath $(foreach \
            ADDITIONS_PATH, \
            $(ADDITIONS_PATHS), \
            $(ADDITIONS_PATH)/$(SCRIPT)))))

MODERN_MEDIA_CONTROLS_SCRIPTS = \
    $(CyberCore)/Modules/modern-media-controls/main.js \
    $(CyberCore)/Modules/modern-media-controls/gesture-recognizers/gesture-recognizer.js \
    $(CyberCore)/Modules/modern-media-controls/gesture-recognizers/tap.js \
    $(CyberCore)/Modules/modern-media-controls/gesture-recognizers/pinch.js \
    $(CyberCore)/Modules/modern-media-controls/controls/scheduler.js \
    $(CyberCore)/Modules/modern-media-controls/controls/layout-traits.js \
    $(CyberCore)/Modules/modern-media-controls/controls/layout-node.js \
    $(CyberCore)/Modules/modern-media-controls/controls/layout-item.js \
    $(CyberCore)/Modules/modern-media-controls/controls/icon-service.js \
    $(CyberCore)/Modules/modern-media-controls/controls/background-tint.js \
    $(CyberCore)/Modules/modern-media-controls/controls/time-control.js \
    $(CyberCore)/Modules/modern-media-controls/controls/time-label.js \
    $(CyberCore)/Modules/modern-media-controls/controls/slider-base.js \
    $(CyberCore)/Modules/modern-media-controls/controls/slider.js \
    $(CyberCore)/Modules/modern-media-controls/controls/button.js \
    $(CyberCore)/Modules/modern-media-controls/controls/play-pause-button.js \
    $(CyberCore)/Modules/modern-media-controls/controls/skip-back-button.js \
    $(CyberCore)/Modules/modern-media-controls/controls/skip-forward-button.js \
    $(CyberCore)/Modules/modern-media-controls/controls/mute-button.js \
    $(CyberCore)/Modules/modern-media-controls/controls/airplay-button.js \
    $(CyberCore)/Modules/modern-media-controls/controls/pip-button.js \
    $(CyberCore)/Modules/modern-media-controls/controls/tracks-button.js \
    $(CyberCore)/Modules/modern-media-controls/controls/fullscreen-button.js \
    $(CyberCore)/Modules/modern-media-controls/controls/seek-button.js \
    $(CyberCore)/Modules/modern-media-controls/controls/rewind-button.js \
    $(CyberCore)/Modules/modern-media-controls/controls/forward-button.js \
    $(CyberCore)/Modules/modern-media-controls/controls/overflow-button.js \
    $(CyberCore)/Modules/modern-media-controls/controls/close-button.js \
    $(CyberCore)/Modules/modern-media-controls/controls/buttons-container.js \
    $(CyberCore)/Modules/modern-media-controls/controls/status-label.js \
    $(CyberCore)/Modules/modern-media-controls/controls/controls-bar.js \
    $(CyberCore)/Modules/modern-media-controls/controls/auto-hide-controller.js \
    $(CyberCore)/Modules/modern-media-controls/controls/media-controls.js \
    $(CyberCore)/Modules/modern-media-controls/controls/background-click-delegate-notifier.js \
    $(CyberCore)/Modules/modern-media-controls/controls/inline-media-controls.js \
    $(CyberCore)/Modules/modern-media-controls/controls/ios-inline-media-controls.js \
    $(CyberCore)/Modules/modern-media-controls/controls/ios-layout-traits.js \
    $(CyberCore)/Modules/modern-media-controls/controls/macos-inline-media-controls.js \
    $(CyberCore)/Modules/modern-media-controls/controls/macos-fullscreen-media-controls.js \
    $(CyberCore)/Modules/modern-media-controls/controls/macos-layout-traits.js \
    $(CyberCore)/Modules/modern-media-controls/controls/placard.js \
    $(CyberCore)/Modules/modern-media-controls/controls/airplay-placard.js \
    $(CyberCore)/Modules/modern-media-controls/controls/invalid-placard.js \
    $(CyberCore)/Modules/modern-media-controls/controls/pip-placard.js \
    $(CyberCore)/Modules/modern-media-controls/controls/watchos-activity-indicator.js \
    $(CyberCore)/Modules/modern-media-controls/controls/watchos-media-controls.js \
    $(CyberCore)/Modules/modern-media-controls/controls/watchos-layout-traits.js \
    $(CyberCore)/Modules/modern-media-controls/media/media-controller-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/airplay-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/audio-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/close-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/controls-visibility-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/fullscreen-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/mute-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/overflow-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/pip-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/placard-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/playback-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/scrubbing-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/seek-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/seek-backward-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/seek-forward-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/skip-back-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/skip-forward-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/start-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/status-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/time-control-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/tracks-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/volume-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/media-document-controller.js \
    $(CyberCore)/Modules/modern-media-controls/media/watchos-media-controls-support.js \
    $(CyberCore)/Modules/modern-media-controls/media/media-controller.js \
    $(POSSIBLE_ADDITIONAL_MODERN_MEDIA_CONTROLS_SCRIPTS) \
#

all : ModernMediaControls.js

ModernMediaControls.js : $(MODERN_MEDIA_CONTROLS_SCRIPTS)
	cat $^ > ModernMediaControls.js

# user agent scripts

USER_AGENT_SCRIPTS = \
    ModernMediaControls.js \
#

USER_AGENT_SCRIPTS_FILES = \
    UserAgentScripts.h \
    UserAgentScriptsData.cpp \
#
USER_AGENT_SCRIPTS_FILES_PATTERNS = $(subst .,%,$(USER_AGENT_SCRIPTS_FILES))

all : $(USER_AGENT_SCRIPTS_FILES)

$(USER_AGENT_SCRIPTS_FILES_PATTERNS) : $(CyberScriptCore_SCRIPTS_DIR)/make-js-file-arrays.py $(USER_AGENT_SCRIPTS)
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/make-js-file-arrays.py -n CyberCore --fail-if-non-ascii $(USER_AGENT_SCRIPTS_FILES) $(USER_AGENT_SCRIPTS)

# --------

# plug-ins resources

PLUG_INS_RESOURCES = $(CyberCore)/Resources/plugIns.js

# order matters -- make-css-file-arrays.pl takes the header and then the source file path
PLUG_INS_RESOURCES_FILES = PlugInsResources.h PlugInsResourcesData.cpp
PLUG_INS_RESOURCES_FILES_PATTERNS = $(subst .,%,$(PLUG_INS_RESOURCES_FILES))

all : $(PLUG_INS_RESOURCES_FILES)

$(PLUG_INS_RESOURCES_FILES_PATTERNS) : $(CyberCore)/css/make-css-file-arrays.pl $(CyberCore)/bindings/scripts/preprocessor.pm $(PLUG_INS_RESOURCES) $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PERL) $< --defines "$(FEATURE_AND_PLATFORM_DEFINES)" $(PLUG_INS_RESOURCES_FILES) $(PLUG_INS_RESOURCES)

# --------

WEBKIT_FONT_FAMILY_NAME_FILES = \
    CyberKitFontFamilyNames.cpp \
    CyberKitFontFamilyNames.h \
#
WEBKIT_FONT_FAMILY_NAME_FILES_PATTERNS = $(subst .,%,$(WEBKIT_FONT_FAMILY_NAME_FILES))

all : $(WEBKIT_FONT_FAMILY_NAME_FILES)
$(WEBKIT_FONT_FAMILY_NAME_FILES_PATTERNS): $(CyberCore)/dom/make_names.pl $(CyberCore)/bindings/scripts/Hasher.pm $(CyberCore)/bindings/scripts/StaticString.pm $(CyberCore)/css/CyberKitFontFamilyNames.in
	$(PERL) $< --fonts $(CyberCore)/css/CyberKitFontFamilyNames.in

# HTML tag and attribute names

HTML_TAG_FILES = \
    JSHTMLElementWrapperFactory.cpp \
    JSHTMLElementWrapperFactory.h \
    HTMLElementFactory.cpp \
    HTMLElementFactory.h \
    HTMLElementTypeHelpers.h \
    HTMLNames.cpp \
    HTMLNames.h \
#
HTML_TAG_FILES_PATTERNS = $(subst .,%,$(HTML_TAG_FILES))

all : $(HTML_TAG_FILES)

$(HTML_TAG_FILES_PATTERNS) : $(CyberCore)/dom/make_names.pl $(CyberCore)/bindings/scripts/Hasher.pm $(CyberCore)/bindings/scripts/StaticString.pm $(CyberCore)/html/HTMLTagNames.in $(CyberCore)/html/HTMLAttributeNames.in $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PERL) $< --elements $(CyberCore)/html/HTMLTagNames.in --attrs $(CyberCore)/html/HTMLAttributeNames.in --factory --wrapperFactory

XML_NS_NAMES_FILES = XMLNSNames.cpp XMLNSNames.h
XML_NS_NAMES_FILES_PATTERNS = $(subst .,%,$(XML_NS_NAMES_FILES))

all : $(XML_NS_NAMES_FILES)

$(XML_NS_NAMES_FILES_PATTERNS) : $(CyberCore)/dom/make_names.pl $(CyberCore)/bindings/scripts/Hasher.pm $(CyberCore)/bindings/scripts/StaticString.pm $(CyberCore)/xml/xmlnsattrs.in
	$(PERL) $< --attrs $(CyberCore)/xml/xmlnsattrs.in
	
XML_NAMES_FILES = XMLNames.cpp XMLNames.h
XML_NAMES_FILES_PATTERNS = $(subst .,%,$(XML_NAMES_FILES))

all : $(XML_NAMES_FILES)

$(XML_NAMES_FILES_PATTERNS) : $(CyberCore)/dom/make_names.pl $(CyberCore)/bindings/scripts/Hasher.pm $(CyberCore)/bindings/scripts/StaticString.pm $(CyberCore)/xml/xmlattrs.in
	$(PERL) $< --attrs $(CyberCore)/xml/xmlattrs.in

# --------

# SVG tag and attribute names, and element factory

SVG_TAG_FILES = \
    JSSVGElementWrapperFactory.cpp \
    JSSVGElementWrapperFactory.h \
    SVGElementFactory.cpp \
    SVGElementFactory.h \
    SVGElementTypeHelpers.h \
    SVGNames.cpp \
    SVGNames.h \
#
SVG_TAG_FILES_PATTERNS = $(subst .,%,$(SVG_TAG_FILES))

all : $(SVG_TAG_FILES)

$(SVG_TAG_FILES_PATTERNS) : $(CyberCore)/dom/make_names.pl $(CyberCore)/bindings/scripts/Hasher.pm $(CyberCore)/bindings/scripts/StaticString.pm $(CyberCore)/svg/svgtags.in $(CyberCore)/svg/svgattrs.in $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PERL) $< --elements $(CyberCore)/svg/svgtags.in --attrs $(CyberCore)/svg/svgattrs.in --factory --wrapperFactory

XLINK_NAMES_FILES = XLinkNames.cpp XLinkNames.h
XLINK_NAMES_FILES_PATTERNS = $(subst .,%,$(XLINK_NAMES_FILES))

all : $(XLINK_NAMES_FILES)

$(XLINK_NAMES_FILES_PATTERNS) : $(CyberCore)/dom/make_names.pl $(CyberCore)/bindings/scripts/Hasher.pm $(CyberCore)/bindings/scripts/StaticString.pm $(CyberCore)/svg/xlinkattrs.in
	$(PERL) $< --attrs $(CyberCore)/svg/xlinkattrs.in

# --------

# Register event constructors and targets

EVENT_NAMES = $(CyberCore)/dom/EventNames.in $(ADDITIONAL_EVENT_NAMES)

EVENT_FACTORY_FILES = \
    EventFactory.cpp \
    EventHeaders.h \
    EventInterfaces.h \
#
EVENT_FACTORY_PATTERNS = $(subst .,%,$(EVENT_FACTORY_FILES))

all : $(EVENT_FACTORY_FILES)
$(EVENT_FACTORY_PATTERNS) : $(CyberCore)/dom/make_event_factory.pl $(EVENT_NAMES)
	$(PERL) $< $(addprefix --input , $(filter-out $(CyberCore)/dom/make_event_factory.pl, $^))

EVENT_TARGET_FACTORY = $(CyberCore)/dom/EventTargetFactory.in $(ADDITIONAL_EVENT_TARGET_FACTORY)

EVENT_TARGET_FACTORY_FILES = \
    EventTargetFactory.cpp \
    EventTargetHeaders.h \
    EventTargetInterfaces.h \
#
EVENT_TARGET_FACTORY_PATTERNS = $(subst .,%,$(EVENT_TARGET_FACTORY_FILES))

all : $(EVENT_TARGET_FACTORY_FILES)
$(EVENT_TARGET_FACTORY_PATTERNS) : $(CyberCore)/dom/make_event_factory.pl $(EVENT_TARGET_FACTORY)
	$(PERL) $< $(addprefix --input , $(filter-out $(CyberCore)/dom/make_event_factory.pl, $^))

# --------

# MathML tag and attribute names, and element factory

MATH_ML_GENERATED_FILES = \
    JSMathMLElementWrapperFactory.cpp \
    JSMathMLElementWrapperFactory.h \
    MathMLElementFactory.cpp \
    MathMLElementFactory.h \
    MathMLElementTypeHelpers.h \
    MathMLNames.cpp \
    MathMLNames.h \
#
MATH_ML_GENERATED_PATTERNS = $(subst .,%,$(MATH_ML_GENERATED_FILES))

all : $(MATH_ML_GENERATED_FILES)
$(MATH_ML_GENERATED_PATTERNS) : $(CyberCore)/dom/make_names.pl $(CyberCore)/bindings/scripts/Hasher.pm $(CyberCore)/bindings/scripts/StaticString.pm $(CyberCore)/mathml/mathtags.in $(CyberCore)/mathml/mathattrs.in
	$(PERL) $< --elements $(CyberCore)/mathml/mathtags.in --attrs $(CyberCore)/mathml/mathattrs.in --factory --wrapperFactory

# --------

# TagName, ElementName, and Namespace enums

DOM_NAME_ENUM_DEPS = \
    $(CyberCore)/dom/make_names.pl \
    $(CyberCore)/bindings/scripts/Hasher.pm \
    $(CyberCore)/bindings/scripts/StaticString.pm \
    $(CyberCore)/html/HTMLTagNames.in \
    $(CyberCore)/svg/svgtags.in \
    $(CyberCore)/mathml/mathtags.in \
    $(CyberCore)/html/HTMLAttributeNames.in \
    $(CyberCore)/mathml/mathattrs.in \
    $(CyberCore)/svg/svgattrs.in \
    $(CyberCore)/svg/xlinkattrs.in \
    $(CyberCore)/xml/xmlattrs.in \
    $(CyberCore)/xml/xmlnsattrs.in \
#

DOM_NAME_ENUM_ARGUMENTS = \
    --elements $(CyberCore)/html/HTMLTagNames.in \
    --elements $(CyberCore)/svg/svgtags.in \
    --elements $(CyberCore)/mathml/mathtags.in \
    --attrs $(CyberCore)/html/HTMLAttributeNames.in \
    --attrs $(CyberCore)/mathml/mathattrs.in \
    --attrs $(CyberCore)/svg/svgattrs.in \
    --attrs $(CyberCore)/svg/xlinkattrs.in \
    --attrs $(CyberCore)/xml/xmlattrs.in \
    --attrs $(CyberCore)/xml/xmlnsattrs.in \
#

TAG_NAME_GENERATED_FILES = \
    TagName.cpp \
    TagName.h \
#
TAG_NAME_GENERATED_PATTERNS = $(subst .,%,$(TAG_NAME_GENERATED_FILES))

all : $(TAG_NAME_GENERATED_FILES)
$(TAG_NAME_GENERATED_PATTERNS) : $(DOM_NAME_ENUM_DEPS)
	$(PERL) $< --enum TagName $(DOM_NAME_ENUM_ARGUMENTS)

ELEMENT_NAME_GENERATED_FILES = \
    ElementName.cpp \
    ElementName.h \
#
ELEMENT_NAME_GENERATED_PATTERNS = $(subst .,%,$(ELEMENT_NAME_GENERATED_FILES))

all : $(ELEMENT_NAME_GENERATED_FILES)
$(ELEMENT_NAME_GENERATED_PATTERNS) : $(DOM_NAME_ENUM_DEPS)
	$(PERL) $< --enum ElementName $(DOM_NAME_ENUM_ARGUMENTS)

NAMESPACE_GENERATED_FILES = \
    Namespace.cpp \
    Namespace.h \
#
NAMESPACE_GENERATED_PATTERNS = $(subst .,%,$(NAMESPACE_GENERATED_FILES))

all : $(NAMESPACE_GENERATED_FILES)
$(NAMESPACE_GENERATED_PATTERNS) : $(DOM_NAME_ENUM_DEPS)
	$(PERL) $< --enum Namespace $(DOM_NAME_ENUM_ARGUMENTS)

# --------

# Internal Settings

WEB_PREFERENCES_INPUT_FILES = \
    ${WTF_BUILD_SCRIPTS_DIR}/Preferences/UnifiedWebPreferences.yaml \
    ${CyberCore}/page/Settings.yaml \
#

GENERATE_SETTINGS = \
    InternalSettingsGenerated.cpp \
    InternalSettingsGenerated.idl \
    InternalSettingsGenerated.h \
    Settings.cpp \
    Settings.h \
#

all : $(GENERATE_SETTINGS)

$(GENERATE_SETTINGS) : % : $(CyberCore)/Scripts/SettingsTemplates/%.erb $(WEB_PREFERENCES_INPUT_FILES) $(CyberCore)/Scripts/GenerateSettings.rb
	$(RUBY) $(CyberCore)/Scripts/GenerateSettings.rb $(WEB_PREFERENCES_INPUT_FILES) --template $<

# --------

# Common generator things

COMMON_BINDINGS_SCRIPTS = \
    $(CyberCore)/bindings/scripts/CodeGenerator.pm \
    $(CyberCore)/bindings/scripts/IDLParser.pm \
    $(CyberCore)/bindings/scripts/generate-bindings.pl \
    $(CyberCore)/bindings/scripts/preprocessor.pm

PREPROCESS_IDLS_SCRIPTS = \
    $(CyberCore)/bindings/scripts/preprocess-idls.pl

# JS bindings generator

IDL_INCLUDES = \
    $(CyberCore)/Modules \
    $(ADDITIONAL_BINDING_IDLS_PATHS)

IDL_COMMON_ARGS = $(IDL_INCLUDES:%=--include %) --write-dependencies --outputDir .

JS_BINDINGS_SCRIPTS = $(COMMON_BINDINGS_SCRIPTS) $(CyberCore)/bindings/scripts/CodeGeneratorJS.pm

SUPPLEMENTAL_DEPENDENCY_FILE = SupplementalDependencies.txt
SUPPLEMENTAL_MAKEFILE_DEPS = SupplementalDependencies.dep
ISO_SUBSPACES_HEADER_FILE = DOMIsoSubspaces.h
CLIENT_ISO_SUBSPACES_HEADER_FILE = DOMClientIsoSubspaces.h
CONSTRUCTORS_HEADER_FILE = DOMConstructors.h
WINDOW_CONSTRUCTORS_FILE = DOMWindowConstructors.idl
WORKERGLOBALSCOPE_CONSTRUCTORS_FILE = WorkerGlobalScopeConstructors.idl
SHADOWREALMGLOBALSCOPE_CONSTRUCTORS_FILE = ShadowRealmGlobalScopeConstructors.idl
DEDICATEDWORKERGLOBALSCOPE_CONSTRUCTORS_FILE = DedicatedWorkerGlobalScopeConstructors.idl
SERVICEWORKERGLOBALSCOPE_CONSTRUCTORS_FILE = ServiceWorkerGlobalScopeConstructors.idl
SHAREDWORKERGLOBALSCOPE_CONSTRUCTORS_FILE = SharedWorkerGlobalScopeConstructors.idl
WORKLETGLOBALSCOPE_CONSTRUCTORS_FILE = WorkletGlobalScopeConstructors.idl
PAINTWORKLETGLOBALSCOPE_CONSTRUCTORS_FILE = PaintWorkletGlobalScopeConstructors.idl
AUDIOWORKLETGLOBALSCOPE_CONSTRUCTORS_FILE = AudioWorkletGlobalScopeConstructors.idl
IDL_ATTRIBUTES_FILE = $(CyberCore)/bindings/scripts/IDLAttributes.json

IDL_INTERMEDIATE_FILES = \
    $(SUPPLEMENTAL_MAKEFILE_DEPS) \
    $(SUPPLEMENTAL_DEPENDENCY_FILE) \
    $(ISO_SUBSPACES_HEADER_FILE) \
    $(CLIENT_ISO_SUBSPACES_HEADER_FILE) \
    $(CONSTRUCTORS_HEADER_FILE) \
    $(WINDOW_CONSTRUCTORS_FILE) \
    $(WORKERGLOBALSCOPE_CONSTRUCTORS_FILE) \
    $(SHADOWREALMGLOBALSCOPE_CONSTRUCTORS_FILE) \
    $(DEDICATEDWORKERGLOBALSCOPE_CONSTRUCTORS_FILE) \
    $(SERVICEWORKERGLOBALSCOPE_CONSTRUCTORS_FILE) \
    $(WORKLETGLOBALSCOPE_CONSTRUCTORS_FILE) \
    $(PAINTWORKLETGLOBALSCOPE_CONSTRUCTORS_FILE) \
    $(AUDIOWORKLETGLOBALSCOPE_CONSTRUCTORS_FILE)
#
IDL_INTERMEDIATE_PATTERNS = $(subst .,%,$(IDL_INTERMEDIATE_FILES))

IDL_FILE_NAMES_LIST = IDLFileNamesList.txt
$(IDL_FILE_NAMES_LIST) : $(JS_BINDING_IDLS)
	echo $(JS_BINDING_IDLS) | tr " " "\n" > $@

$(IDL_INTERMEDIATE_PATTERNS) : $(PREPROCESS_IDLS_SCRIPTS) $(IDL_ATTRIBUTES_FILE) $(IDL_FILE_NAMES_LIST) $(JS_BINDING_IDLS) $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PERL) $(CyberCore)/bindings/scripts/preprocess-idls.pl --defines "$(FEATURE_AND_PLATFORM_DEFINES) LANGUAGE_JAVASCRIPT" --idlFileNamesList $(IDL_FILE_NAMES_LIST) --idlAttributesFile $(IDL_ATTRIBUTES_FILE) --supplementalDependencyFile $(SUPPLEMENTAL_DEPENDENCY_FILE) --isoSubspacesHeaderFile $(ISO_SUBSPACES_HEADER_FILE) --clientISOSubspacesHeaderFile $(CLIENT_ISO_SUBSPACES_HEADER_FILE) --constructorsHeaderFile $(CONSTRUCTORS_HEADER_FILE) --windowConstructorsFile $(WINDOW_CONSTRUCTORS_FILE) --workerGlobalScopeConstructorsFile $(WORKERGLOBALSCOPE_CONSTRUCTORS_FILE) --shadowRealmGlobalScopeConstructorsFile $(SHADOWREALMGLOBALSCOPE_CONSTRUCTORS_FILE) --dedicatedWorkerGlobalScopeConstructorsFile $(DEDICATEDWORKERGLOBALSCOPE_CONSTRUCTORS_FILE) --serviceWorkerGlobalScopeConstructorsFile $(SERVICEWORKERGLOBALSCOPE_CONSTRUCTORS_FILE) --sharedWorkerGlobalScopeConstructorsFile $(SHAREDWORKERGLOBALSCOPE_CONSTRUCTORS_FILE) --workletGlobalScopeConstructorsFile $(WORKLETGLOBALSCOPE_CONSTRUCTORS_FILE) --paintWorkletGlobalScopeConstructorsFile $(PAINTWORKLETGLOBALSCOPE_CONSTRUCTORS_FILE) --audioWorkletGlobalScopeConstructorsFile $(AUDIOWORKLETGLOBALSCOPE_CONSTRUCTORS_FILE) --supplementalMakefileDeps $(SUPPLEMENTAL_MAKEFILE_DEPS)

#
# Emit the rules to generate bindings from IDL files. Note that there are
# several scenarios we need to support:
#
# * Files such as GestureEvent.idl can be provided by some facility external to
#   the CyberKit repository. The file can be made available by appearing in
#   BUILT_PRODUCTS_DIR or SDKROOT. If found in either of those locations, the
#   file will be used, otherwise, there will be no GestureEvent facility.
#
# * Files such as TouchEvent.idl can also be found in BUILT_PRODUCTS_DIR or
#   SDKROOT. However, there is also a version of the file in CyberCore/dom. If
#   the file can't be found in either of the first two locations, the version
#   in CyberCore/dom will be used.
#
# * Files such as ApplePaySetup.idl used to be provided externally in
#   BUILT_PRODUCTS_DIR or SDKROOT, but are now "upstreamed" into
#   CyberCore/Modules/applepay. Because they were previously found in SDKROOT,
#   building against old SDKs will cause the build system to be exposed to old,
#   out-of-date versions of ApplePaySetup.idl. In these cases, we want to
#   *ignore* the externally-provided versions and use the versions found in
#   CyberCore.
#
# To build the IDL files, we used to have a rule like the following:
#
#   JS%.cpp JS%.h : %.idl ...
#       ...
#
# This rule made use of vpath to find the %.idl files in all their myriad
# locations. However, because of the three scenarios previously outlined, that
# approach is problematic. In some cases, we want to prefer the files in
# BUILT_PRODUCTS_DIR or SDKROOT over those in CyberCore. In other cases, it's the
# other way around. So we can't use vpath to find the files indicated by %.idl.
#
# Instead, since the IDL files come from many different sources, we need a
# separate rule for each one, with each rule incorporating the full path of the
# file. Fortunately, we can do this pretty simply with the $(foreach) function.
#
# All that said, we do still need to invoke vpath. There are some
# auto-generated dependency files (such as SupplementalDependencies.dep) that
# generate file information without paths, and those still need to benefit from
# setting search paths with vpath.

vpath %.idl $(ADDITIONAL_BINDING_IDLS_PATHS) $(CyberCore)/bindings/scripts

# -------------------------------------------------
define GENERATE_BINDINGS_template

JS$(call get_bare_name,$(1)).cpp JS$(call get_bare_name,$(1)).h: $(1) $$(JS_BINDINGS_SCRIPTS) $$(IDL_ATTRIBUTES_FILE) $$(IDL_INTERMEDIATE_FILES) $$(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$$(PERL) $$(CyberCore)/bindings/scripts/generate-bindings.pl \
		$$(IDL_COMMON_ARGS) \
		--defines "$$(FEATURE_AND_PLATFORM_DEFINES) LANGUAGE_JAVASCRIPT" \
		--generator JS \
		--idlAttributesFile $$(IDL_ATTRIBUTES_FILE) \
		--supplementalDependencyFile $$(SUPPLEMENTAL_DEPENDENCY_FILE) \
		$$<

endef
# -------------------------------------------------

$(foreach IDL_FILE,$(JS_BINDING_IDLS),$(eval $(call GENERATE_BINDINGS_template,$(IDL_FILE))))

ifneq ($(NO_SUPPLEMENTAL_FILES),1)
-include $(SUPPLEMENTAL_MAKEFILE_DEPS)
endif

ifneq ($(NO_SUPPLEMENTAL_FILES),1)
-include $(JS_DOM_HEADERS:.h=.dep)
endif

# CyberCore JS Builtins

CyberCore_BUILTINS_SOURCES = \
    $(CyberCore)/Modules/compression/CompressionStream.js \
    $(CyberCore)/Modules/compression/DecompressionStream.js \
    $(CyberCore)/Modules/streams/ByteLengthQueuingStrategy.js \
    $(CyberCore)/Modules/streams/CountQueuingStrategy.js \
    $(CyberCore)/Modules/streams/ReadableByteStreamController.js \
    $(CyberCore)/Modules/streams/ReadableByteStreamInternals.js \
    $(CyberCore)/Modules/streams/ReadableStream.js \
    $(CyberCore)/Modules/streams/ReadableStreamBYOBRequest.js \
    $(CyberCore)/Modules/streams/ReadableStreamDefaultController.js \
    $(CyberCore)/Modules/streams/ReadableStreamInternals.js \
    $(CyberCore)/Modules/streams/ReadableStreamBYOBReader.js \
    $(CyberCore)/Modules/streams/ReadableStreamDefaultReader.js \
    $(CyberCore)/Modules/streams/StreamInternals.js \
    $(CyberCore)/Modules/streams/TransformStream.js \
    $(CyberCore)/Modules/streams/TransformStreamDefaultController.js \
    $(CyberCore)/Modules/streams/TransformStreamInternals.js \
    $(CyberCore)/Modules/streams/WritableStreamDefaultController.js \
    $(CyberCore)/Modules/streams/WritableStreamDefaultWriter.js \
    $(CyberCore)/Modules/streams/WritableStreamInternals.js \
    $(CyberCore)/dom/TextDecoderStream.js \
    $(CyberCore)/dom/TextEncoderStream.js \
    $(CyberCore)/bindings/js/JSDOMBindingInternals.js \
    $(CyberCore)/inspector/CommandLineAPIModuleSource.js \
#

BUILTINS_GENERATOR_SCRIPTS = \
    $(CyberScriptCore_SCRIPTS_DIR)/wkbuiltins.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generator.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_model.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_templates.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_combined_header.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_combined_implementation.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_separate_header.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_separate_implementation.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_internals_wrapper_header.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_internals_wrapper_implementation.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_wrapper_header.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_wrapper_implementation.py \
    $(CyberScriptCore_SCRIPTS_DIR)/generate-js-builtins.py \
    $(CyberScriptCore_SCRIPTS_DIR)/lazywriter.py \
#

CyberCore_BUILTINS_WRAPPERS = $(addsuffix Builtins.cpp, $(notdir $(basename $(CyberCore_BUILTINS_SOURCES))))
CyberCore_BUILTINS_WRAPPERS += \
    CyberCoreJSBuiltins.h \
    CyberCoreJSBuiltins.cpp \
    CyberCoreJSBuiltinInternals.h \
    CyberCoreJSBuiltinInternals.cpp \
#
CyberCore_BUILTINS_WRAPPERS_PATTERNS = $(subst .,%,$(CyberCore_BUILTINS_WRAPPERS))

# Adding/removing scripts should trigger regeneration, but changing which builtins are
# generated should not affect other builtins when not passing '--combined' to the generator.

CyberCore_BUILTINS_SOURCES_LIST : $(CyberScriptCore_SCRIPTS_DIR)/UpdateContents.py $(CyberCore)/DerivedSources.make
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/UpdateContents.py '$(CyberCore_BUILTINS_SOURCES)' $@

CyberCore_BUILTINS_DEPENDENCIES_LIST : $(CyberScriptCore_SCRIPTS_DIR)/UpdateContents.py $(CyberCore)/DerivedSources.make
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/UpdateContents.py '$(BUILTINS_GENERATOR_SCRIPTS)' $@

$(CyberCore_BUILTINS_WRAPPERS_PATTERNS) : $(CyberCore_BUILTINS_SOURCES) CyberCore_BUILTINS_SOURCES_LIST $(BUILTINS_GENERATOR_SCRIPTS) CyberCore_BUILTINS_DEPENDENCIES_LIST
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/generate-js-builtins.py --wrappers-only --output-directory . --framework CyberCore $(CyberCore_BUILTINS_SOURCES)

# See comments for IDL_PATHS for a description of what this is for.
vpath %.js $(sort $(foreach f,$(CyberCore_BUILTINS_SOURCES),$(realpath $(dir $(f)))))

%Builtins.h: %.js $(BUILTINS_GENERATOR_SCRIPTS) CyberCore_BUILTINS_DEPENDENCIES_LIST
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/generate-js-builtins.py --output-directory . --framework CyberCore $<

all : $(notdir $(CyberCore_BUILTINS_SOURCES:%.js=%Builtins.h)) $(CyberCore_BUILTINS_WRAPPERS)

# ------------------------

