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

#import "DOMHTMLTableRowElement.h"

#import "DOMHTMLCollectionInternal.h"
#import "DOMHTMLElementInternal.h"
#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/HTMLCollection.h>
#import <CyberCore/HTMLElement.h>
#import <CyberCore/HTMLNames.h>
#import <CyberCore/HTMLTableRowElement.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLTableRowElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLTableRowElement

- (int)rowIndex
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->rowIndex();
}

- (int)sectionRowIndex
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->sectionRowIndex();
}

- (DOMHTMLCollection *)cells
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->cells()));
}

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

- (NSString *)ch
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::charAttr);
}

- (void)setCh:(NSString *)newCh
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::charAttr, newCh);
}

- (NSString *)chOff
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::charoffAttr);
}

- (void)setChOff:(NSString *)newChOff
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::charoffAttr, newChOff);
}

- (NSString *)vAlign
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::valignAttr);
}

- (void)setVAlign:(NSString *)newVAlign
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::valignAttr, newVAlign);
}

- (DOMHTMLElement *)insertCell:(int)index
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->insertCell(index)).ptr());
}

- (void)deleteCell:(int)index
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->deleteCell(index));
}

@end
