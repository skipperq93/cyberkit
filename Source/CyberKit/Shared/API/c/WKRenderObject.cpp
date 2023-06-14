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
#include "WKRenderObject.h"

#include "APIArray.h"
#include "WKAPICast.h"
#include "WebRenderObject.h"

WKTypeID WKRenderObjectGetTypeID()
{
    return CyberKit::toAPI(CyberKit::WebRenderObject::APIType);
}

WKStringRef WKRenderObjectCopyName(WKRenderObjectRef renderObjectRef)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(renderObjectRef)->name());
}

WKStringRef WKRenderObjectCopyTextSnippet(WKRenderObjectRef renderObjectRef)
{
    CyberKit::WebRenderObject* renderObject = CyberKit::toImpl(renderObjectRef);
    if (!renderObject->textSnippet().isNull())
        return CyberKit::toCopiedAPI(renderObject->textSnippet());

    return nullptr;
}

unsigned WKRenderObjectGetTextLength(WKRenderObjectRef renderObjectRef)
{
    return CyberKit::toImpl(renderObjectRef)->textLength();
}

WKStringRef WKRenderObjectCopyElementTagName(WKRenderObjectRef renderObjectRef)
{
    CyberKit::WebRenderObject* renderObject = CyberKit::toImpl(renderObjectRef);
    if (!renderObject->elementTagName().isNull())
        return CyberKit::toCopiedAPI(renderObject->elementTagName());

    return nullptr;
}

WKStringRef WKRenderObjectCopyElementID(WKRenderObjectRef renderObjectRef)
{
    CyberKit::WebRenderObject* renderObject = CyberKit::toImpl(renderObjectRef);
    if (!renderObject->elementID().isNull())
        return CyberKit::toCopiedAPI(renderObject->elementID());

    return nullptr;
}

WKArrayRef WKRenderObjectGetElementClassNames(WKRenderObjectRef renderObjectRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(renderObjectRef)->elementClassNames());
}

WKPoint WKRenderObjectGetAbsolutePosition(WKRenderObjectRef renderObjectRef)
{
    CyberCore::IntPoint absolutePosition = CyberKit::toImpl(renderObjectRef)->absolutePosition();
    return WKPointMake(absolutePosition.x(), absolutePosition.y());
}

WKRect WKRenderObjectGetFrameRect(WKRenderObjectRef renderObjectRef)
{
    CyberCore::IntRect frameRect = CyberKit::toImpl(renderObjectRef)->frameRect();
    return WKRectMake(frameRect.x(), frameRect.y(), frameRect.width(), frameRect.height());
}

WKArrayRef WKRenderObjectGetChildren(WKRenderObjectRef renderObjectRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(renderObjectRef)->children());
}
