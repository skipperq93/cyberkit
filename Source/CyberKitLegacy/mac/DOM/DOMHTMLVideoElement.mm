/*
 * Copyright (C) 2004-2016 Apple Inc. All rights reserved.
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


#if ENABLE(VIDEO)

#import "DOMHTMLVideoElement.h"

#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/HTMLNames.h>
#import <CyberCore/HTMLVideoElement.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLVideoElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLVideoElement

- (unsigned)width
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getUnsignedIntegralAttribute(CyberCore::HTMLNames::widthAttr);
}

- (void)setWidth:(unsigned)newWidth
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setUnsignedIntegralAttribute(CyberCore::HTMLNames::widthAttr, newWidth);
}

- (unsigned)height
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getUnsignedIntegralAttribute(CyberCore::HTMLNames::heightAttr);
}

- (void)setHeight:(unsigned)newHeight
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setUnsignedIntegralAttribute(CyberCore::HTMLNames::heightAttr, newHeight);
}

- (unsigned)videoWidth
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->videoWidth();
}

- (unsigned)videoHeight
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->videoHeight();
}

- (NSString *)poster
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getURLAttribute(CyberCore::HTMLNames::posterAttr);
}

- (void)setPoster:(NSString *)newPoster
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::posterAttr, newPoster);
}

- (BOOL)playsInline
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::playsinlineAttr);
}

- (void)setPlaysInline:(BOOL)newPlaysInline
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::playsinlineAttr, newPlaysInline);
}

- (BOOL)webkitSupportsFullscreen
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->webkitSupportsFullscreen();
}

- (BOOL)webkitDisplayingFullscreen
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->webkitDisplayingFullscreen();
}

- (void)webkitEnterFullscreen
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->webkitEnterFullscreen());
}

- (void)webkitExitFullscreen
{
    CyberCore::JSMainThreadNullState state;
    IMPL->webkitExitFullscreen();
}

- (void)webkitEnterFullScreen
{
    [self webkitEnterFullscreen];
}

- (void)webkitExitFullScreen
{
    [self webkitExitFullscreen];
}

@end

#endif // ENABLE(VIDEO)

#undef IMPL
