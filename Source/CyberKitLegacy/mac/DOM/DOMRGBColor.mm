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

#import "DOMRGBColorInternal.h"

#import "DOMCSSPrimitiveValueInternal.h"
#import "DOMInternal.h"
#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/DeprecatedCSSOMPrimitiveValue.h>
#import <CyberCore/DeprecatedCSSOMRGBColor.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/CyberCoreObjCExtras.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>

#if PLATFORM(MAC)
#import <CyberCore/ColorMac.h>
#else
#import <CyberCore/ColorSpace.h>
#endif

#define IMPL reinterpret_cast<CyberCore::DeprecatedCSSOMRGBColor*>(_internal)

@implementation DOMRGBColor

- (void)dealloc
{
    if (CyberCoreObjCScheduleDeallocateOnMainThread([DOMRGBColor class], self))
        return;

    if (_internal)
        IMPL->deref();
    [super dealloc];
}

- (DOMCSSPrimitiveValue *)red
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->red()));
}

- (DOMCSSPrimitiveValue *)green
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->green()));
}

- (DOMCSSPrimitiveValue *)blue
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->blue()));
}

- (DOMCSSPrimitiveValue *)alpha
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->alpha()));
}

#if PLATFORM(MAC)
- (NSColor *)color
{
    CyberCore::JSMainThreadNullState state;
    return CyberCore::nsColor(IMPL->color());
}
#else
- (CGColorRef)color
{
    CyberCore::JSMainThreadNullState state;
    return CyberCore::cachedCGColor(IMPL->color());
}
#endif

@end

DOMRGBColor *kit(CyberCore::DeprecatedCSSOMRGBColor* value)
{
    CyberCoreThreadViolationCheckRoundOne();
    if (!value)
        return nil;
    if (DOMRGBColor *wrapper = getDOMWrapper(value))
        return [[wrapper retain] autorelease];
    DOMRGBColor *wrapper = [[DOMRGBColor alloc] _init];
    wrapper->_internal = reinterpret_cast<DOMObjectInternal*>(value);
    value->ref();
    addDOMWrapper(wrapper, value);
    return [wrapper autorelease];
}

#undef IMPL
