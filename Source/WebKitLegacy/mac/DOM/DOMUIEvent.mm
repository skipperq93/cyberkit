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

#import "DOMUIEvent.h"

#import "DOMAbstractViewInternal.h"
#import "DOMEventInternal.h"
#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/DOMWindow.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/KeyboardEvent.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/UIEvent.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::UIEvent*>(reinterpret_cast<CyberCore::Event*>(_internal))

@implementation DOMUIEvent

- (DOMAbstractView *)view
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->view()));
}

- (int)detail
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->detail();
}

- (int)keyCode
{
    CyberCore::JSMainThreadNullState state;
    if (is<CyberCore::KeyboardEvent>(*IMPL))
        return downcast<CyberCore::KeyboardEvent>(*IMPL).keyCode();
    return 0;
}

- (int)charCode
{
    CyberCore::JSMainThreadNullState state;
    if (is<CyberCore::KeyboardEvent>(*IMPL))
        return downcast<CyberCore::KeyboardEvent>(*IMPL).charCode();
    return 0;
}

- (int)layerX
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->layerX();
}

- (int)layerY
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->layerY();
}

- (int)pageX
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->pageX();
}

- (int)pageY
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->pageY();
}

- (int)which
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->which();
}

- (void)initUIEvent:(NSString *)type canBubble:(BOOL)canBubble cancelable:(BOOL)cancelable view:(DOMAbstractView *)inView detail:(int)inDetail
{
    CyberCore::JSMainThreadNullState state;
    IMPL->initUIEvent(type, canBubble, cancelable, toWindowProxy(inView), inDetail);
}

@end

@implementation DOMUIEvent (DOMUIEventDeprecated)

- (void)initUIEvent:(NSString *)type :(BOOL)canBubble :(BOOL)cancelable :(DOMAbstractView *)inView :(int)inDetail
{
    [self initUIEvent:type canBubble:canBubble cancelable:cancelable view:inView detail:inDetail];
}

@end

#undef IMPL
