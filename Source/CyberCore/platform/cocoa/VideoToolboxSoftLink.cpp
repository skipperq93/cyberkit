/*
 * Copyright (C) 2017-2019 Apple Inc. All rights reserved.
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

#include "config.h"

#if USE(VIDEOTOOLBOX)

#include <VideoToolbox/VideoToolbox.h>
#include <wtf/SoftLinking.h>

typedef struct OpaqueVTVideoDecoder VTVideoDecoderRef;
typedef struct OpaqueVTImageRotationSession* VTImageRotationSessionRef;
typedef struct OpaqueVTPixelBufferConformer* VTPixelBufferConformerRef;
typedef struct OpaqueVTPixelTransferSession* VTPixelTransferSessionRef;

SOFT_LINK_FRAMEWORK_FOR_SOURCE(CyberCore, VideoToolbox)

SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTSessionCopyProperty, OSStatus, (VTSessionRef session, CFStringRef propertyKey, CFAllocatorRef allocator, void* propertyValueOut), (session, propertyKey, allocator, propertyValueOut))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTSessionSetProperties, OSStatus, (VTSessionRef session, CFDictionaryRef propertyDictionary), (session, propertyDictionary))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTDecompressionSessionCreate, OSStatus, (CFAllocatorRef allocator, CMVideoFormatDescriptionRef videoFormatDescription, CFDictionaryRef videoDecoderSpecification, CFDictionaryRef destinationImageBufferAttributes, const VTDecompressionOutputCallbackRecord* outputCallback, VTDecompressionSessionRef* decompressionSessionOut), (allocator, videoFormatDescription, videoDecoderSpecification, destinationImageBufferAttributes, outputCallback, decompressionSessionOut))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTDecompressionSessionCanAcceptFormatDescription, Boolean, (VTDecompressionSessionRef session, CMFormatDescriptionRef newFormatDesc), (session, newFormatDesc))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTDecompressionSessionWaitForAsynchronousFrames, OSStatus, (VTDecompressionSessionRef session), (session))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTDecompressionSessionDecodeFrameWithOutputHandler, OSStatus, (VTDecompressionSessionRef session, CMSampleBufferRef sampleBuffer, VTDecodeFrameFlags decodeFlags, VTDecodeInfoFlags *infoFlagsOut, VTDecompressionOutputHandler outputHandler), (session, sampleBuffer, decodeFlags, infoFlagsOut, outputHandler))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTImageRotationSessionCreate, OSStatus, (CFAllocatorRef allocator, uint32_t rotationDegrees, VTImageRotationSessionRef* imageRotationSessionOut), (allocator, rotationDegrees, imageRotationSessionOut))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTImageRotationSessionSetProperty, OSStatus, (VTImageRotationSessionRef session, CFStringRef propertyKey, CFTypeRef propertyValue), (session, propertyKey, propertyValue))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTImageRotationSessionTransferImage, OSStatus, (VTImageRotationSessionRef session, CVPixelBufferRef sourceBuffer, CVPixelBufferRef destinationBuffer), (session, sourceBuffer, destinationBuffer))
SOFT_LINK_FUNCTION_MAY_FAIL_FOR_SOURCE(CyberCore, VideoToolbox, VTIsHardwareDecodeSupported, Boolean, (CMVideoCodecType codecType), (codecType))
SOFT_LINK_FUNCTION_MAY_FAIL_FOR_SOURCE(CyberCore, VideoToolbox, VTGetGVADecoderAvailability, OSStatus, (uint32_t* totalInstanceCountOut, uint32_t* freeInstanceCountOut), (totalInstanceCountOut, freeInstanceCountOut))
SOFT_LINK_FUNCTION_MAY_FAIL_FOR_SOURCE(CyberCore, VideoToolbox, VTCreateCGImageFromCVPixelBuffer, OSStatus, (CVPixelBufferRef pixelBuffer, CFDictionaryRef options, CGImageRef* imageOut), (pixelBuffer, options, imageOut))
SOFT_LINK_FUNCTION_MAY_FAIL_FOR_SOURCE(CyberCore, VideoToolbox, VTCopyHEVCDecoderCapabilitiesDictionary, CFDictionaryRef, (), ())
SOFT_LINK_FUNCTION_MAY_FAIL_FOR_SOURCE(CyberCore, VideoToolbox, VTGetHEVCCapabilitesForFormatDescription, OSStatus, (CMVideoFormatDescriptionRef formatDescription, CFDictionaryRef decoderCapabilitiesDict, Boolean* isDecodable, Boolean* mayBePlayable), (formatDescription, decoderCapabilitiesDict, isDecodable, mayBePlayable))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTSelectAndCreateVideoDecoderInstance, OSStatus, (CMVideoCodecType codecType, CFAllocatorRef allocator, CFDictionaryRef videoDecoderSpecification, VTVideoDecoderRef *decoderInstanceOut), (codecType, allocator, videoDecoderSpecification, decoderInstanceOut))
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, VideoToolbox, kVTVideoDecoderSpecification_EnableHardwareAcceleratedVideoDecoder, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, VideoToolbox, kVTDecompressionPropertyKey_PixelBufferPool, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, VideoToolbox, kVTDecompressionPropertyKey_SuggestedQualityOfServiceTiers, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, VideoToolbox, kVTImageRotationPropertyKey_EnableHighSpeedTransfer, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, VideoToolbox, kVTImageRotationPropertyKey_FlipHorizontalOrientation, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, VideoToolbox, kVTImageRotationPropertyKey_FlipVerticalOrientation, CFStringRef)

SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTPixelTransferSessionCreate, OSStatus, (CFAllocatorRef allocator, VTPixelTransferSessionRef* pixelTransferSessionOut), (allocator, pixelTransferSessionOut))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTPixelTransferSessionTransferImage, OSStatus, (VTPixelTransferSessionRef session, CVPixelBufferRef sourceBuffer, CVPixelBufferRef destinationBuffer), (session, sourceBuffer, destinationBuffer))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTSessionSetProperty, OSStatus, (VTSessionRef session, CFStringRef propertyKey, CFTypeRef propertyValue), (session, propertyKey, propertyValue))
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, VideoToolbox, kVTPixelTransferPropertyKey_ScalingMode, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, VideoToolbox, kVTScalingMode_Letterbox, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, VideoToolbox, kVTScalingMode_Trim, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, VideoToolbox, kVTPixelTransferPropertyKey_EnableHardwareAcceleratedTransfer, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, VideoToolbox, kVTPixelTransferPropertyKey_EnableHighSpeedTransfer, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, VideoToolbox, kVTPixelTransferPropertyKey_RealTime, CFStringRef)

SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, VideoToolbox, kVTHEVCDecoderCapability_SupportedProfiles, CFStringRef)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, VideoToolbox, kVTHEVCDecoderCapability_PerProfileSupport, CFStringRef)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, VideoToolbox, kVTHEVCDecoderProfileCapability_IsHardwareAccelerated, CFStringRef)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, VideoToolbox, kVTHEVCDecoderProfileCapability_MaxDecodeLevel, CFStringRef)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, VideoToolbox, kVTHEVCDecoderProfileCapability_MaxPlaybackLevel, CFStringRef)

SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTPixelBufferConformerCreateWithAttributes, OSStatus, (CFAllocatorRef allocator, CFDictionaryRef attributes, VTPixelBufferConformerRef* conformerOut), (allocator, attributes, conformerOut));
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTPixelBufferConformerIsConformantPixelBuffer, Boolean, (VTPixelBufferConformerRef conformer, CVPixelBufferRef pixBuf), (conformer, pixBuf))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, VideoToolbox, VTPixelBufferConformerCopyConformedPixelBuffer, OSStatus, (VTPixelBufferConformerRef conformer, CVPixelBufferRef sourceBuffer, Boolean ensureModifiable, CVPixelBufferRef* conformedBufferOut), (conformer, sourceBuffer, ensureModifiable, conformedBufferOut))

#endif // USE(VIDEOTOOLBOX)
