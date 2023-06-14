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

#import "DOMMouseEvent.h"

#import "DOMAbstractViewInternal.h"
#import "DOMEventInternal.h"
#import "DOMEventTarget.h"
#import "DOMNode.h"
#import "DOMNodeInternal.h"
#import <CyberCore/DOMWindow.h>
#import "ExceptionHandlers.h"
#import <CyberCore/JSExecState.h>
#import <CyberCore/MouseEvent.h>
#import <CyberCore/Node.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::MouseEvent*>(reinterpret_cast<CyberCore::Event*>(_internal))

@implementation DOMMouseEvent

- (int)screenX
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->screenX();
}

- (int)screenY
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->screenY();
}

- (int)clientX
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->clientX();
}

- (int)clientY
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->clientY();
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

- (unsigned short)button
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->button();
}

- (id <DOMEventTarget>)relatedTarget
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->relatedTarget()));
}

- (int)offsetX
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->offsetX();
}

- (int)offsetY
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->offsetY();
}

- (int)x
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->x();
}

- (int)y
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->y();
}

- (DOMNode *)fromElement
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->fromElement()));
}

- (DOMNode *)toElement
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->toElement()));
}

- (void)initMouseEvent:(NSString *)type canBubble:(BOOL)canBubble cancelable:(BOOL)cancelable view:(DOMAbstractView *)view detail:(int)detail screenX:(int)inScreenX screenY:(int)inScreenY clientX:(int)inClientX clientY:(int)inClientY ctrlKey:(BOOL)inCtrlKey altKey:(BOOL)inAltKey shiftKey:(BOOL)inShiftKey metaKey:(BOOL)inMetaKey button:(unsigned short)inButton relatedTarget:(id <DOMEventTarget>)inRelatedTarget
{
    CyberCore::JSMainThreadNullState state;
    DOMNode* inRelatedTargetObjC = inRelatedTarget;
    CyberCore::Node* inRelatedTargetNode = core(inRelatedTargetObjC);
    IMPL->initMouseEvent(type, canBubble, cancelable, toWindowProxy(view), detail, inScreenX, inScreenY, inClientX, inClientY, inCtrlKey, inAltKey, inShiftKey, inMetaKey, inButton, inRelatedTargetNode);
}

@end

@implementation DOMMouseEvent (DOMMouseEventDeprecated)

- (void)initMouseEvent:(NSString *)type :(BOOL)canBubble :(BOOL)cancelable :(DOMAbstractView *)view :(int)detail :(int)inScreenX :(int)inScreenY :(int)inClientX :(int)inClientY :(BOOL)inCtrlKey :(BOOL)inAltKey :(BOOL)inShiftKey :(BOOL)inMetaKey :(unsigned short)inButton :(id <DOMEventTarget>)inRelatedTarget
{
    [self initMouseEvent:type canBubble:canBubble cancelable:cancelable view:view detail:detail screenX:inScreenX screenY:inScreenY clientX:inClientX clientY:inClientY ctrlKey:inCtrlKey altKey:inAltKey shiftKey:inShiftKey metaKey:inMetaKey button:inButton relatedTarget:inRelatedTarget];
}

@end
