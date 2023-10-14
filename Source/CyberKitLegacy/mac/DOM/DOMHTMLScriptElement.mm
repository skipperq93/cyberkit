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

#import "DOMHTMLScriptElementInternal.h"

#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/ElementInlines.h>
#import <CyberCore/HTMLNames.h>
#import <CyberCore/HTMLScriptElement.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLScriptElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLScriptElement

- (NSString *)text
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->text();
}

- (void)setText:(NSString *)newText
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setText(newText);
}

- (NSString *)htmlFor
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::forAttr);
}

- (void)setHtmlFor:(NSString *)newHtmlFor
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::forAttr, newHtmlFor);
}

- (NSString *)event
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::eventAttr);
}

- (void)setEvent:(NSString *)newEvent
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::eventAttr, newEvent);
}

- (NSString *)charset
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::charsetAttr);
}

- (void)setCharset:(NSString *)newCharset
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::charsetAttr, newCharset);
}

- (BOOL)async
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->async();
}

- (void)setAsync:(BOOL)newAsync
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAsync(newAsync);
}

- (BOOL)defer
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::deferAttr);
}

- (void)setDefer:(BOOL)newDefer
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::deferAttr, newDefer);
}

- (NSString *)src
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getURLAttribute(CyberCore::HTMLNames::srcAttr).string();
}

- (void)setSrc:(NSString *)newSrc
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::srcAttr, newSrc);
}

- (NSString *)type
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::typeAttr);
}

- (void)setType:(NSString *)newType
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::typeAttr, newType);
}

- (NSString *)crossOrigin
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->crossOrigin();
}

- (void)setCrossOrigin:(NSString *)newCrossOrigin
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setCrossOrigin(newCrossOrigin);
}

- (NSString *)nonce
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::nonceAttr);
}

- (void)setNonce:(NSString *)newNonce
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::nonceAttr, newNonce);
}

@end

DOMHTMLScriptElement *kit(CyberCore::HTMLScriptElement* value)
{
    CyberCoreThreadViolationCheckRoundOne();
    return static_cast<DOMHTMLScriptElement*>(kit(static_cast<CyberCore::Node*>(value)));
}

#undef IMPL
