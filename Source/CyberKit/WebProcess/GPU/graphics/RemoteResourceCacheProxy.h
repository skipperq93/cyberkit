/*
 * Copyright (C) 2020-2023 Apple Inc.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(GPU_PROCESS)

#include "RenderingUpdateID.h"
#include <CyberCore/RenderingResource.h>
#include <wtf/HashMap.h>

namespace CyberCore {
class DecomposedGlyphs;
class Filter;
class Font;
class Gradient;
class ImageBuffer;
class NativeImage;
struct FontCustomPlatformData;
}

namespace CyberKit {

class RemoteImageBufferProxy;
class RemoteRenderingBackendProxy;

class RemoteResourceCacheProxy : public CyberCore::RenderingResource::Observer {
public:
    RemoteResourceCacheProxy(RemoteRenderingBackendProxy&);
    ~RemoteResourceCacheProxy();

    void cacheImageBuffer(RemoteImageBufferProxy&);
    RemoteImageBufferProxy* cachedImageBuffer(CyberCore::RenderingResourceIdentifier) const;
    void releaseImageBuffer(RemoteImageBufferProxy&);
    void forgetImageBuffer(CyberCore::RenderingResourceIdentifier);

    CyberCore::NativeImage* cachedNativeImage(CyberCore::RenderingResourceIdentifier) const;

    void recordNativeImageUse(CyberCore::NativeImage&);
    void recordFontUse(CyberCore::Font&);
    void recordImageBufferUse(CyberCore::ImageBuffer&);
    void recordDecomposedGlyphsUse(CyberCore::DecomposedGlyphs&);
    void recordGradientUse(CyberCore::Gradient&);
    void recordFilterUse(CyberCore::Filter&);
    void recordFontCustomPlatformDataUse(const CyberCore::FontCustomPlatformData&);

    void didPaintLayers();

    void remoteResourceCacheWasDestroyed();
    void releaseMemory();
    void releaseAllImageResources();
    
    unsigned imagesCount() const;

    void clear();

private:
    using ImageBufferHashMap = HashMap<CyberCore::RenderingResourceIdentifier, WeakPtr<RemoteImageBufferProxy>>;
    using RenderingResourceHashMap = HashMap<CyberCore::RenderingResourceIdentifier, ThreadSafeWeakPtr<CyberCore::RenderingResource>>;
    using FontHashMap = HashMap<CyberCore::RenderingResourceIdentifier, uint64_t>;

    void releaseRenderingResource(CyberCore::RenderingResourceIdentifier) override;
    void clearRenderingResourceMap();
    void clearNativeImageMap();

    void finalizeRenderingUpdateForFonts();
    void prepareForNextRenderingUpdate();
    void clearFontMap();
    void clearFontCustomPlatformDataMap();
    void clearImageBufferBackends();

    ImageBufferHashMap m_imageBuffers;
    RenderingResourceHashMap m_renderingResources;
    FontHashMap m_fonts;
    FontHashMap m_fontCustomPlatformDatas;

    unsigned m_numberOfFontsUsedInCurrentRenderingUpdate { 0 };
    unsigned m_numberOfFontCustomPlatformDatasUsedInCurrentRenderingUpdate { 0 };

    RemoteRenderingBackendProxy& m_remoteRenderingBackendProxy;
    uint64_t m_renderingUpdateID;
};

} // namespace CyberKit

#endif // ENABLE(GPU_PROCESS)
