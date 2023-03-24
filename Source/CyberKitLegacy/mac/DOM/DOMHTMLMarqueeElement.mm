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

#import "DOMHTMLMarqueeElement.h"

#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/ElementInlines.h>
#import <CyberCore/HTMLMarqueeElement.h>
#import <CyberCore/HTMLNames.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLMarqueeElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLMarqueeElement

- (NSString *)behavior
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::behaviorAttr);
}

- (void)setBehavior:(NSString *)newBehavior
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::behaviorAttr, newBehavior);
}

- (NSString *)bgColor
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::bgcolorAttr);
}

- (void)setBgColor:(NSString *)newBgColor
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::bgcolorAttr, newBgColor);
}

- (NSString *)direction
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::directionAttr);
}

- (void)setDirection:(NSString *)newDirection
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::directionAttr, newDirection);
}

- (NSString *)height
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::heightAttr);
}

- (void)setHeight:(NSString *)newHeight
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::heightAttr, newHeight);
}

- (unsigned)hspace
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getUnsignedIntegralAttribute(CyberCore::HTMLNames::hspaceAttr);
}

- (void)setHspace:(unsigned)newHspace
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setUnsignedIntegralAttribute(CyberCore::HTMLNames::hspaceAttr, newHspace);
}

- (int)loop
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->loop();
}

- (void)setLoop:(int)newLoop
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setLoop(newLoop));
}

- (unsigned)scrollAmount
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->scrollAmount();
}

- (void)setScrollAmount:(unsigned)newScrollAmount
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setScrollAmount(newScrollAmount);
}

- (unsigned)scrollDelay
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->scrollDelay();
}

- (void)setScrollDelay:(unsigned)newScrollDelay
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setScrollDelay(newScrollDelay);
}

- (BOOL)trueSpeed
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::truespeedAttr);
}

- (void)setTrueSpeed:(BOOL)newTrueSpeed
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::truespeedAttr, newTrueSpeed);
}

- (unsigned)vspace
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getUnsignedIntegralAttribute(CyberCore::HTMLNames::vspaceAttr);
}

- (void)setVspace:(unsigned)newVspace
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setUnsignedIntegralAttribute(CyberCore::HTMLNames::vspaceAttr, newVspace);
}

- (NSString *)width
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::widthAttr);
}

- (void)setWidth:(NSString *)newWidth
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::widthAttr, newWidth);
}

- (void)start
{
    CyberCore::JSMainThreadNullState state;
    IMPL->start();
}

- (void)stop
{
    CyberCore::JSMainThreadNullState state;
    IMPL->stop();
}

@end

#undef IMPL
