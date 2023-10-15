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

#import "DOMEventInternal.h"

#import "DOMEventTarget.h"
#import "DOMInternal.h"
#import "DOMNodeInternal.h"
#import <CyberCore/Event.h>
#import "ExceptionHandlers.h"
#import <CyberCore/JSExecState.h>
#import <CyberCore/Node.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/CyberCoreObjCExtras.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL reinterpret_cast<CyberCore::Event*>(_internal)

@implementation DOMEvent

- (void)dealloc
{
    if (CyberCoreObjCScheduleDeallocateOnMainThread([DOMEvent class], self))
        return;

    if (_internal)
        IMPL->deref();
    [super dealloc];
}

- (NSString *)type
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->type();
}

- (id <DOMEventTarget>)target
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->target()));
}

- (id <DOMEventTarget>)currentTarget
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->currentTarget()));
}

- (unsigned short)eventPhase
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->eventPhase();
}

- (BOOL)bubbles
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->bubbles();
}

- (BOOL)cancelable
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->cancelable();
}

- (BOOL)composed
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->composed();
}

- (DOMTimeStamp)timeStamp
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->timeStamp().approximateWallTime().secondsSinceEpoch().milliseconds();
}

- (BOOL)defaultPrevented
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->defaultPrevented();
}

- (BOOL)isTrusted
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->isTrusted();
}

- (id <DOMEventTarget>)srcElement
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->target()));
}

- (BOOL)returnValue
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->legacyReturnValue();
}

- (void)setReturnValue:(BOOL)newReturnValue
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setLegacyReturnValue(newReturnValue);
}

- (BOOL)cancelBubble
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->cancelBubble();
}

- (void)setCancelBubble:(BOOL)newCancelBubble
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setCancelBubble(newCancelBubble);
}

- (void)stopPropagation
{
    CyberCore::JSMainThreadNullState state;
    IMPL->stopPropagation();
}

- (void)preventDefault
{
    CyberCore::JSMainThreadNullState state;
    IMPL->preventDefault();
}

- (void)initEvent:(NSString *)eventTypeArg canBubbleArg:(BOOL)canBubbleArg cancelableArg:(BOOL)cancelableArg
{
    CyberCore::JSMainThreadNullState state;
    IMPL->initEvent(eventTypeArg, canBubbleArg, cancelableArg);
}

- (void)stopImmediatePropagation
{
    CyberCore::JSMainThreadNullState state;
    IMPL->stopImmediatePropagation();
}

@end

@implementation DOMEvent (DOMEventDeprecated)

- (void)initEvent:(NSString *)eventTypeArg :(BOOL)canBubbleArg :(BOOL)cancelableArg
{
    [self initEvent:eventTypeArg canBubbleArg:canBubbleArg cancelableArg:cancelableArg];
}

@end

CyberCore::Event* core(DOMEvent *wrapper)
{
    return wrapper ? reinterpret_cast<CyberCore::Event*>(wrapper->_internal) : 0;
}

DOMEvent *kit(CyberCore::Event* value)
{
    CyberCoreThreadViolationCheckRoundOne();
    if (!value)
        return nil;
    if (DOMEvent *wrapper = getDOMWrapper(value))
        return retainPtr(wrapper).autorelease();
    RetainPtr<DOMEvent> wrapper = adoptNS([[kitClass(value) alloc] _init]);
    if (!wrapper)
        return nil;
    wrapper->_internal = reinterpret_cast<DOMObjectInternal*>(value);
    value->ref();
    addDOMWrapper(wrapper.get(), value);
    return wrapper.autorelease();
}

#undef IMPL
