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

#import "DOMHTMLFormElementInternal.h"

#import "DOMHTMLCollectionInternal.h"
#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/HTMLCollection.h>
#import <CyberCore/HTMLFormElement.h>
#import <CyberCore/HTMLNames.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLFormElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLFormElement

- (NSString *)acceptCharset
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::accept_charsetAttr);
}

- (void)setAcceptCharset:(NSString *)newAcceptCharset
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::accept_charsetAttr, newAcceptCharset);
}

- (NSString *)action
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getURLAttribute(CyberCore::HTMLNames::actionAttr);
}

- (void)setAction:(NSString *)newAction
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::actionAttr, newAction);
}

- (NSString *)autocomplete
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->autocomplete();
}

- (void)setAutocomplete:(NSString *)newAutocomplete
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAutocomplete(newAutocomplete);
}

- (NSString *)enctype
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->enctype();
}

- (void)setEnctype:(NSString *)newEnctype
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setEnctype(newEnctype);
}

- (NSString *)encoding
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->enctype();
}

- (void)setEncoding:(NSString *)newEncoding
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setEnctype(newEncoding);
}

- (NSString *)method
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->method();
}

- (void)setMethod:(NSString *)newMethod
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setMethod(newMethod);
}

- (NSString *)name
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getNameAttribute();
}

- (void)setName:(NSString *)newName
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::nameAttr, newName);
}

- (BOOL)noValidate
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::novalidateAttr);
}

- (void)setNoValidate:(BOOL)newNoValidate
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::novalidateAttr, newNoValidate);
}

- (NSString *)target
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::targetAttr);
}

- (void)setTarget:(NSString *)newTarget
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::targetAttr, newTarget);
}

- (DOMHTMLCollection *)elements
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->elementsForNativeBindings()));
}

- (int)length
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->length();
}

- (void)submit
{
    CyberCore::JSMainThreadNullState state;
    IMPL->submit();
}

- (void)reset
{
    CyberCore::JSMainThreadNullState state;
    IMPL->reset();
}

- (BOOL)checkValidity
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->checkValidity();
}

@end

DOMHTMLFormElement *kit(CyberCore::HTMLFormElement* value)
{
    CyberCoreThreadViolationCheckRoundOne();
    return static_cast<DOMHTMLFormElement*>(kit(static_cast<CyberCore::Node*>(value)));
}

#undef IMPL
