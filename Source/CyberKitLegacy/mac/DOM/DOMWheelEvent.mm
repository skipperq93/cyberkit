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

#import "DOMWheelEventInternal.h"

#import "DOMAbstractViewInternal.h"
#import "DOMEventInternal.h"
#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/JSExecState.h>
#import <CyberCore/LocalDOMWindow.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <CyberCore/WheelEvent.h>
#import <wtf/GetPtr.h>

#define IMPL static_cast<CyberCore::WheelEvent*>(reinterpret_cast<CyberCore::Event*>(_internal))

@implementation DOMWheelEvent

- (double)deltaX
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->deltaX();
}

- (double)deltaY
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->deltaY();
}

- (double)deltaZ
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->deltaZ();
}

- (unsigned)deltaMode
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->deltaMode();
}

- (int)wheelDeltaX
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->wheelDeltaX();
}

- (int)wheelDeltaY
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->wheelDeltaY();
}

- (int)wheelDelta
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->wheelDelta();
}

- (BOOL)webkitDirectionInvertedFromDevice
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->webkitDirectionInvertedFromDevice();
}

- (BOOL)isHorizontal
{
    return !!self.wheelDeltaX;
}

- (void)initWheelEvent:(int)inWheelDeltaX wheelDeltaY:(int)inWheelDeltaY view:(DOMAbstractView *)view screenX:(int)screenX screenY:(int)screenY clientX:(int)clientX clientY:(int)clientY ctrlKey:(BOOL)ctrlKey altKey:(BOOL)altKey shiftKey:(BOOL)shiftKey metaKey:(BOOL)metaKey
{
    CyberCore::JSMainThreadNullState state;
    IMPL->initCyberKitWheelEvent(inWheelDeltaX, inWheelDeltaY, toWindowProxy(view), screenX, screenY, clientX, clientY, ctrlKey, altKey, shiftKey, metaKey);
}

@end

CyberCore::WheelEvent* core(DOMWheelEvent *wrapper)
{
    return wrapper ? reinterpret_cast<CyberCore::WheelEvent*>(wrapper->_internal) : 0;
}

#undef IMPL
