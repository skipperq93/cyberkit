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

#import "DOMKeyboardEvent.h"

#import "DOMAbstractViewInternal.h"
#import "DOMEventInternal.h"
#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/JSExecState.h>
#import <CyberCore/KeyboardEvent.h>
#import <CyberCore/LocalDOMWindow.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::KeyboardEvent*>(reinterpret_cast<CyberCore::Event*>(_internal))

@implementation DOMKeyboardEvent

- (NSString *)keyIdentifier
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->keyIdentifier();
}

- (unsigned)location
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->location();
}

- (unsigned)keyLocation
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->location();
}

- (BOOL)ctrlKey
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->ctrlKey();
}

- (BOOL)shiftKey
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->shiftKey();
}

- (BOOL)altKey
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->altKey();
}

- (BOOL)metaKey
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->metaKey();
}

- (BOOL)altGraphKey
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->altGraphKey();
}

- (int)keyCode
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->keyCode();
}

- (int)charCode
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->charCode();
}

- (BOOL)getModifierState:(NSString *)keyIdentifierArg
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getModifierState(keyIdentifierArg);
}

- (void)initKeyboardEvent:(NSString *)type canBubble:(BOOL)canBubble cancelable:(BOOL)cancelable view:(DOMAbstractView *)view keyIdentifier:(NSString *)inKeyIdentifier location:(unsigned)inLocation ctrlKey:(BOOL)inCtrlKey altKey:(BOOL)inAltKey shiftKey:(BOOL)inShiftKey metaKey:(BOOL)inMetaKey altGraphKey:(BOOL)inAltGraphKey
{
    CyberCore::JSMainThreadNullState state;
    IMPL->initKeyboardEvent(type, canBubble, cancelable, toWindowProxy(view), inKeyIdentifier, inLocation, inCtrlKey, inAltKey, inShiftKey, inMetaKey, inAltGraphKey);
}

- (void)initKeyboardEvent:(NSString *)type canBubble:(BOOL)canBubble cancelable:(BOOL)cancelable view:(DOMAbstractView *)view keyIdentifier:(NSString *)inKeyIdentifier location:(unsigned)inLocation ctrlKey:(BOOL)inCtrlKey altKey:(BOOL)inAltKey shiftKey:(BOOL)inShiftKey metaKey:(BOOL)inMetaKey
{
    CyberCore::JSMainThreadNullState state;
    IMPL->initKeyboardEvent(type, canBubble, cancelable, toWindowProxy(view), inKeyIdentifier, inLocation, inCtrlKey, inAltKey, inShiftKey, inMetaKey);
}

- (void)initKeyboardEvent:(NSString *)type canBubble:(BOOL)canBubble cancelable:(BOOL)cancelable view:(DOMAbstractView *)view keyIdentifier:(NSString *)inKeyIdentifier keyLocation:(unsigned)inKeyLocation ctrlKey:(BOOL)inCtrlKey altKey:(BOOL)inAltKey shiftKey:(BOOL)inShiftKey metaKey:(BOOL)inMetaKey altGraphKey:(BOOL)inAltGraphKey
{
    CyberCore::JSMainThreadNullState state;
    IMPL->initKeyboardEvent(type, canBubble, cancelable, toWindowProxy(view), inKeyIdentifier, inKeyLocation, inCtrlKey, inAltKey, inShiftKey, inMetaKey, inAltGraphKey);
}

- (void)initKeyboardEvent:(NSString *)type canBubble:(BOOL)canBubble cancelable:(BOOL)cancelable view:(DOMAbstractView *)view keyIdentifier:(NSString *)inKeyIdentifier keyLocation:(unsigned)inKeyLocation ctrlKey:(BOOL)inCtrlKey altKey:(BOOL)inAltKey shiftKey:(BOOL)inShiftKey metaKey:(BOOL)inMetaKey
{
    CyberCore::JSMainThreadNullState state;
    IMPL->initKeyboardEvent(type, canBubble, cancelable, toWindowProxy(view), inKeyIdentifier, inKeyLocation, inCtrlKey, inAltKey, inShiftKey, inMetaKey);
}

@end

#undef IMPL
