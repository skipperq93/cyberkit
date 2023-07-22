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

#import "DOMHTMLAppletElement.h"

#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/HTMLAppletElement.h>
#import <CyberCore/HTMLNames.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLAppletElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLAppletElement

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

- (NSString *)alt
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::altAttr);
}

- (void)setAlt:(NSString *)newAlt
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::altAttr, newAlt);
}

- (NSString *)archive
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::archiveAttr);
}

- (void)setArchive:(NSString *)newArchive
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::archiveAttr, newArchive);
}

- (NSString *)code
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::codeAttr);
}

- (void)setCode:(NSString *)newCode
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::codeAttr, newCode);
}

- (NSString *)codeBase
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::codebaseAttr);
}

- (void)setCodeBase:(NSString *)newCodeBase
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::codebaseAttr, newCodeBase);
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

- (int)hspace
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getIntegralAttribute(CyberCore::HTMLNames::hspaceAttr);
}

- (void)setHspace:(int)newHspace
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setIntegralAttribute(CyberCore::HTMLNames::hspaceAttr, newHspace);
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

- (NSString *)object
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::objectAttr);
}

- (void)setObject:(NSString *)newObject
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::objectAttr, newObject);
}

- (int)vspace
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getIntegralAttribute(CyberCore::HTMLNames::vspaceAttr);
}

- (void)setVspace:(int)newVspace
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setIntegralAttribute(CyberCore::HTMLNames::vspaceAttr, newVspace);
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

@end

#undef IMPL
