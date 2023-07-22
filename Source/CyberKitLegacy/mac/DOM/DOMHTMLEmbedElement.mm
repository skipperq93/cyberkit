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

#import "DOMHTMLEmbedElement.h"

#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/HTMLEmbedElement.h>
#import <CyberCore/HTMLNames.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLEmbedElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLEmbedElement

- (NSString *)align
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::alignAttr);
}

- (void)setAlign:(NSString *)newAlign
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr, newAlign);
}

- (int)height
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getIntegralAttribute(CyberCore::HTMLNames::heightAttr);
}

- (void)setHeight:(int)newHeight
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setIntegralAttribute(CyberCore::HTMLNames::heightAttr, newHeight);
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

- (NSString *)src
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getURLAttribute(CyberCore::HTMLNames::srcAttr);
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

- (int)width
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getIntegralAttribute(CyberCore::HTMLNames::widthAttr);
}

- (void)setWidth:(int)newWidth
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setIntegralAttribute(CyberCore::HTMLNames::widthAttr, newWidth);
}

@end

#undef IMPL
