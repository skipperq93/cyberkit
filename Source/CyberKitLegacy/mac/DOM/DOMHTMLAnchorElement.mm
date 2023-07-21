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

#import "DOMHTMLAnchorElement.h"

#import "DOMNodeInternal.h"
#import "DOMPrivate.h"
#import "DOMTokenListInternal.h"
#import <CyberCore/DOMTokenList.h>
#import "ExceptionHandlers.h"
#import <CyberCore/HTMLAnchorElement.h>
#import <CyberCore/HTMLNames.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLAnchorElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLAnchorElement

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

- (NSString *)coords
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::coordsAttr);
}

- (void)setCoords:(NSString *)newCoords
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::coordsAttr, newCoords);
}

#if ENABLE(DOWNLOAD_ATTRIBUTE)
- (NSString *)download
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::downloadAttr);
}

- (void)setDownload:(NSString *)newDownload
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::downloadAttr, newDownload);
}
#endif

- (NSString *)hreflang
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::hreflangAttr);
}

- (void)setHreflang:(NSString *)newHreflang
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::hreflangAttr, newHreflang);
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

- (NSString *)ping
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::pingAttr);
}

- (void)setPing:(NSString *)newPing
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::pingAttr, newPing);
}

- (NSString *)rel
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::relAttr);
}

- (void)setRel:(NSString *)newRel
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::relAttr, newRel);
}

- (NSString *)rev
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::revAttr);
}

- (void)setRev:(NSString *)newRev
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::revAttr, newRev);
}

- (NSString *)shape
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::shapeAttr);
}

- (void)setShape:(NSString *)newShape
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::shapeAttr, newShape);
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

- (NSString *)accessKey
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::accesskeyAttr);
}

- (void)setAccessKey:(NSString *)newAccessKey
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::accesskeyAttr, newAccessKey);
}

- (NSString *)text
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->text();
}

- (NSURL *)absoluteLinkURL
{
    CyberCore::JSMainThreadNullState state;
    return [self _getURLAttribute:@"href"];
}

- (DOMTokenList *)relList
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->relList()));
}

- (NSString *)href
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getURLAttribute(CyberCore::HTMLNames::hrefAttr);
}

- (void)setHref:(NSString *)newHref
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::hrefAttr, newHref);
}

- (NSString *)origin
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->origin();
}

- (NSString *)protocol
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->protocol();
}

- (NSString *)host
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->host();
}

- (NSString *)hostname
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hostname();
}

- (NSString *)port
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->port();
}

- (NSString *)pathname
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->pathname();
}

- (NSString *)search
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->search();
}

- (NSString *)hashName
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hash();
}

@end
