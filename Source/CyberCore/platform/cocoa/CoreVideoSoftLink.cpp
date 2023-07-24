/*
 * Copyright (C) 2016 Apple Inc. All rights reserved.
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

#include <CoreVideo/CoreVideo.h>
#include <wtf/SoftLinking.h>

typedef struct __IOSurface* IOSurfaceRef;

SOFT_LINK_FRAMEWORK_FOR_SOURCE(CyberCore, CoreVideo)

SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVBufferGetAttachment, CFTypeRef, (CVBufferRef buffer, CFStringRef key, CVAttachmentMode* attachmentMode), (buffer, key, attachmentMode))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferGetTypeID, CFTypeID, (), ())
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferGetWidth, size_t, (CVPixelBufferRef pixelBuffer), (pixelBuffer))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferGetHeight, size_t, (CVPixelBufferRef pixelBuffer), (pixelBuffer))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferGetBaseAddress, void*, (CVPixelBufferRef pixelBuffer), (pixelBuffer))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferGetBytesPerRow, size_t, (CVPixelBufferRef pixelBuffer), (pixelBuffer))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferGetBytesPerRowOfPlane, size_t, (CVPixelBufferRef pixelBuffer, size_t planeIndex), (pixelBuffer, planeIndex))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferGetPixelFormatType, OSType, (CVPixelBufferRef pixelBuffer), (pixelBuffer))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferGetBaseAddressOfPlane, void *, (CVPixelBufferRef pixelBuffer, size_t planeIndex), (pixelBuffer, planeIndex));
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferLockBaseAddress, CVReturn, (CVPixelBufferRef pixelBuffer, CVOptionFlags lockFlags), (pixelBuffer, lockFlags))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferUnlockBaseAddress, CVReturn, (CVPixelBufferRef pixelBuffer, CVOptionFlags lockFlags), (pixelBuffer, lockFlags))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferPoolCreate, CVReturn,(CFAllocatorRef allocator, CFDictionaryRef poolAttributes, CFDictionaryRef pixelBufferAttributes, CVPixelBufferPoolRef* poolOut), (allocator, poolAttributes, pixelBufferAttributes, poolOut))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferPoolCreatePixelBuffer, CVReturn, (CFAllocatorRef allocator, CVPixelBufferPoolRef pixelBufferPool, CVPixelBufferRef* pixelBufferOut), (allocator, pixelBufferPool, pixelBufferOut))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferGetIOSurface, IOSurfaceRef, (CVPixelBufferRef pixelBuffer), (pixelBuffer))
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVPixelBufferPixelFormatTypeKey, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVPixelBufferCGBitmapContextCompatibilityKey, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVPixelBufferCGImageCompatibilityKey, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVPixelBufferIOSurfaceOpenGLESFBOCompatibilityKey, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVPixelBufferWidthKey, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVPixelBufferHeightKey, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVPixelFormatOpenGLCompatibility, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVPixelFormatOpenGLESCompatibility, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVPixelBufferIOSurfacePropertiesKey, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVPixelBufferPoolMinimumBufferCountKey, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVImageBufferYCbCrMatrixKey, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVImageBufferYCbCrMatrix_ITU_R_709_2, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVImageBufferYCbCrMatrix_ITU_R_601_4, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVImageBufferYCbCrMatrix_SMPTE_240M_1995, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVImageBufferColorPrimaries_EBU_3213, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVImageBufferColorPrimaries_ITU_R_709_2, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVImageBufferColorPrimaries_SMPTE_C, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVImageBufferColorPrimariesKey, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVImageBufferTransferFunctionKey, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVImageBufferTransferFunction_ITU_R_709_2, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVImageBufferTransferFunction_SMPTE_240M_1995, CFStringRef)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, CoreVideo, kCVImageBufferYCbCrMatrix_DCI_P3, CFStringRef)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, CoreVideo, kCVImageBufferYCbCrMatrix_P3_D65, CFStringRef)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, CoreVideo, kCVImageBufferYCbCrMatrix_ITU_R_2020, CFStringRef)

#if USE(OPENGL_ES)
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVOpenGLESTextureCacheCreate, CVReturn, (CFAllocatorRef allocator, CFDictionaryRef cacheAttributes, CVEAGLContext eaglContext, CFDictionaryRef textureAttributes, CVOpenGLESTextureCacheRef* cacheOut), (allocator, cacheAttributes, eaglContext, textureAttributes, cacheOut))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVOpenGLESTextureCacheCreateTextureFromImage, CVReturn, (CFAllocatorRef allocator, CVOpenGLESTextureCacheRef textureCache, CVImageBufferRef sourceImage, CFDictionaryRef textureAttributes, GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, size_t planeIndex, CVOpenGLESTextureRef* textureOut), (allocator, textureCache, sourceImage, textureAttributes, target, internalFormat, width, height, format, type, planeIndex, textureOut))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVOpenGLESTextureCacheFlush, void, (CVOpenGLESTextureCacheRef textureCache, CVOptionFlags options), (textureCache, options))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVOpenGLESTextureGetTarget, GLenum, (CVOpenGLESTextureRef image), (image))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVOpenGLESTextureGetName, GLuint, (CVOpenGLESTextureRef image), (image))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVOpenGLESTextureGetCleanTexCoords, void, (CVOpenGLESTextureRef image, GLfloat lowerLeft[2], GLfloat lowerRight[2], GLfloat upperLeft[2], GLfloat upperRight[2]), (image, lowerLeft, lowerRight, upperLeft, upperRight))
#elif USE(OPENGL)
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVOpenGLTextureCacheCreate, CVReturn, (CFAllocatorRef allocator, CFDictionaryRef cacheAttributes, CGLContextObj cglContext, CGLPixelFormatObj cglPixelFormat, CFDictionaryRef textureAttributes, CVOpenGLTextureCacheRef* cacheOut), (allocator, cacheAttributes, cglContext, cglPixelFormat, textureAttributes, cacheOut))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVOpenGLTextureCacheCreateTextureFromImage, CVReturn, (CFAllocatorRef allocator, CVOpenGLTextureCacheRef textureCache, CVImageBufferRef sourceImage, CFDictionaryRef attributes, CVOpenGLTextureRef* textureOut), (allocator, textureCache, sourceImage, attributes, textureOut))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVOpenGLTextureCacheFlush, void, (CVOpenGLTextureCacheRef textureCache, CVOptionFlags options), (textureCache, options))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVOpenGLTextureGetTarget, GLenum, (CVOpenGLTextureRef image), (image))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVOpenGLTextureGetName, GLuint, (CVOpenGLTextureRef image), (image))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVOpenGLTextureGetCleanTexCoords, void, (CVOpenGLTextureRef image, GLfloat lowerLeft[2], GLfloat lowerRight[2], GLfloat upperLeft[2], GLfloat upperRight[2]), (image, lowerLeft, lowerRight, upperLeft, upperRight))
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVPixelBufferIOSurfaceOpenGLFBOCompatibilityKey, CFStringRef)
#endif

#if PLATFORM(MAC)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVPixelBufferExtendedPixelsRightKey, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVPixelBufferExtendedPixelsBottomKey, CFStringRef)
SOFT_LINK_CONSTANT_FOR_SOURCE(CyberCore, CoreVideo, kCVPixelBufferOpenGLCompatibilityKey, CFStringRef)
#endif

SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferCreate, CVReturn, (CFAllocatorRef allocator, size_t width, size_t height, OSType pixelFormatType, CFDictionaryRef pixelBufferAttributes, CVPixelBufferRef *pixelBufferOut), (allocator, width, height, pixelFormatType, pixelBufferAttributes, pixelBufferOut))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferCreateWithBytes, CVReturn, (CFAllocatorRef allocator, size_t width, size_t height, OSType pixelFormatType, void* data, size_t bytesPerRow, void (*releaseCallback)(void*, const void*), void* releasePointer, CFDictionaryRef pixelBufferAttributes, CVPixelBufferRef *pixelBufferOut), (allocator, width, height, pixelFormatType, data, bytesPerRow, releaseCallback, releasePointer, pixelBufferAttributes, pixelBufferOut))
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, CoreVideo, CVPixelBufferCreateWithIOSurface, CVReturn, (CFAllocatorRef allocator, IOSurfaceRef surface, CFDictionaryRef pixelBufferAttributes, CVPixelBufferRef * pixelBufferOut), (allocator, surface, pixelBufferAttributes, pixelBufferOut))

