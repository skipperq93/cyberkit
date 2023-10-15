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

#import "DOMHTMLTableElement.h"

#import "DOMHTMLCollectionInternal.h"
#import "DOMHTMLElementInternal.h"
#import "DOMHTMLTableCaptionElementInternal.h"
#import "DOMHTMLTableSectionElementInternal.h"
#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/HTMLCollection.h>
#import <CyberCore/HTMLElement.h>
#import <CyberCore/HTMLNames.h>
#import <CyberCore/HTMLTableCaptionElement.h>
#import <CyberCore/HTMLTableElement.h>
#import <CyberCore/HTMLTableSectionElement.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLTableElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLTableElement

- (DOMHTMLTableCaptionElement *)caption
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->caption()));
}

- (void)setCaption:(DOMHTMLTableCaptionElement *)newCaption
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setCaption(core(newCaption)));
}

- (DOMHTMLTableSectionElement *)tHead
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->tHead()));
}

- (void)setTHead:(DOMHTMLTableSectionElement *)newTHead
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setTHead(core(newTHead)));
}

- (DOMHTMLTableSectionElement *)tFoot
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->tFoot()));
}

- (void)setTFoot:(DOMHTMLTableSectionElement *)newTFoot
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setTFoot(core(newTFoot)));
}

- (DOMHTMLCollection *)rows
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->rows()));
}

- (DOMHTMLCollection *)tBodies
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->tBodies()));
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

- (NSString *)border
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::borderAttr);
}

- (void)setBorder:(NSString *)newBorder
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::borderAttr, newBorder);
}

- (NSString *)cellPadding
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::cellpaddingAttr);
}

- (void)setCellPadding:(NSString *)newCellPadding
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::cellpaddingAttr, newCellPadding);
}

- (NSString *)cellSpacing
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::cellspacingAttr);
}

- (void)setCellSpacing:(NSString *)newCellSpacing
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::cellspacingAttr, newCellSpacing);
}

- (NSString *)frameBorders
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::frameAttr);
}

- (void)setFrameBorders:(NSString *)newFrameBorders
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::frameAttr, newFrameBorders);
}

- (NSString *)rules
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::rulesAttr);
}

- (void)setRules:(NSString *)newRules
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::rulesAttr, newRules);
}

- (NSString *)summary
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::summaryAttr);
}

- (void)setSummary:(NSString *)newSummary
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::summaryAttr, newSummary);
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

- (DOMHTMLElement *)createTHead
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->createTHead()));
}

- (void)deleteTHead
{
    CyberCore::JSMainThreadNullState state;
    IMPL->deleteTHead();
}

- (DOMHTMLElement *)createTFoot
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->createTFoot()));
}

- (void)deleteTFoot
{
    CyberCore::JSMainThreadNullState state;
    IMPL->deleteTFoot();
}

- (DOMHTMLElement *)createTBody
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->createTBody()));
}

- (DOMHTMLElement *)createCaption
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->createCaption()));
}

- (void)deleteCaption
{
    CyberCore::JSMainThreadNullState state;
    IMPL->deleteCaption();
}

- (DOMHTMLElement *)insertRow:(int)index
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->insertRow(index)).ptr());
}

- (void)deleteRow:(int)index
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->deleteRow(index));
}

@end

#undef IMPL
