/*
 * Copyright (C) 2012 Apple Inc. All rights reserved.
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
#include "WKRenderLayer.h"

#include "APIArray.h"
#include "WKAPICast.h"
#include "WebRenderLayer.h"

WKTypeID WKRenderLayerGetTypeID()
{
    return CyberKit::toAPI(CyberKit::WebRenderLayer::APIType);
}

WKRenderObjectRef WKRenderLayerGetRenderer(WKRenderLayerRef renderLayerRef)
{
    return toAPI(CyberKit::toImpl(renderLayerRef)->renderer());
}

WKStringRef WKRenderLayerCopyRendererName(WKRenderLayerRef renderLayerRef)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(renderLayerRef)->renderer()->name());
}

WKStringRef WKRenderLayerCopyElementTagName(WKRenderLayerRef renderLayerRef)
{
    CyberKit::WebRenderLayer* renderLayer = CyberKit::toImpl(renderLayerRef);
    if (!renderLayer->renderer()->elementTagName().isNull())
        return CyberKit::toCopiedAPI(renderLayer->renderer()->elementTagName());

    return nullptr;
}

WKStringRef WKRenderLayerCopyElementID(WKRenderLayerRef renderLayerRef)
{
    CyberKit::WebRenderLayer* renderLayer = CyberKit::toImpl(renderLayerRef);
    if (!renderLayer->renderer()->elementID().isNull())
        return CyberKit::toCopiedAPI(renderLayer->renderer()->elementID());

    return nullptr;
}

WKArrayRef WKRenderLayerGetElementClassNames(WKRenderLayerRef renderLayerRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(renderLayerRef)->renderer()->elementClassNames());
}

WKRect WKRenderLayerGetAbsoluteBounds(WKRenderLayerRef renderLayerRef)
{
    CyberCore::IntRect bounds = CyberKit::toImpl(renderLayerRef)->absoluteBoundingBox();
    return WKRectMake(bounds.x(), bounds.y(), bounds.width(), bounds.height());
}

bool WKRenderLayerIsClipping(WKRenderLayerRef renderLayerRef)
{
    return CyberKit::toImpl(renderLayerRef)->isClipping();
}

bool WKRenderLayerIsClipped(WKRenderLayerRef renderLayerRef)
{
    return CyberKit::toImpl(renderLayerRef)->isClipped();
}

bool WKRenderLayerIsReflection(WKRenderLayerRef renderLayerRef)
{
    return CyberKit::toImpl(renderLayerRef)->isReflection();
}

WKCompositingLayerType WKRenderLayerGetCompositingLayerType(WKRenderLayerRef renderLayerRef)
{
    switch (CyberKit::toImpl(renderLayerRef)->compositingLayerType()) {
    case CyberKit::WebRenderLayer::None:
        return kWKCompositingLayerTypeNone;
    case CyberKit::WebRenderLayer::Normal:
        return kWKCompositingLayerTypeNormal;
    case CyberKit::WebRenderLayer::Tiled:
        return kWKCompositingLayerTypeTiled;
    case CyberKit::WebRenderLayer::Media:
        return kWKCompositingLayerTypeMedia;
    case CyberKit::WebRenderLayer::Container:
        return kWKCompositingLayerTypeContainer;
    }

    ASSERT_NOT_REACHED();
    return kWKCompositingLayerTypeNone;
}

WK_EXPORT double WKRenderLayerGetBackingStoreMemoryEstimate(WKRenderLayerRef renderLayerRef)
{
    return CyberKit::toImpl(renderLayerRef)->backingStoreMemoryEstimate();
}

WKArrayRef WKRenderLayerGetNegativeZOrderList(WKRenderLayerRef renderLayerRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(renderLayerRef)->negativeZOrderList());
}

WKArrayRef WKRenderLayerGetNormalFlowList(WKRenderLayerRef renderLayerRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(renderLayerRef)->normalFlowList());
}

WKArrayRef WKRenderLayerGetPositiveZOrderList(WKRenderLayerRef renderLayerRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(renderLayerRef)->positiveZOrderList());
}

WKRenderLayerRef WKRenderLayerGetFrameContentsLayer(WKRenderLayerRef renderLayerRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(renderLayerRef)->frameContentsLayer());
}
