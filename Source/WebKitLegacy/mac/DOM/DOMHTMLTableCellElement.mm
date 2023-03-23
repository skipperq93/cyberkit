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

#import "DOMHTMLTableCellElementInternal.h"

#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/HTMLNames.h>
#import <CyberCore/HTMLTableCellElement.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLTableCellElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLTableCellElement

- (int)cellIndex
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->cellIndex();
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

- (NSString *)axis
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::axisAttr);
}

- (void)setAxis:(NSString *)newAxis
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::axisAttr, newAxis);
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

- (int)colSpan
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->colSpan();
}

- (void)setColSpan:(int)newColSpan
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setColSpan(newColSpan);
}

- (int)rowSpan
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->rowSpanForBindings();
}

- (void)setRowSpan:(int)newRowSpan
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setRowSpanForBindings(newRowSpan);
}

- (NSString *)headers
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::headersAttr);
}

- (void)setHeaders:(NSString *)newHeaders
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::headersAttr, newHeaders);
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

- (BOOL)noWrap
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::nowrapAttr);
}

- (void)setNoWrap:(BOOL)newNoWrap
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::nowrapAttr, newNoWrap);
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

- (NSString *)abbr
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::abbrAttr);
}

- (void)setAbbr:(NSString *)newAbbr
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::abbrAttr, newAbbr);
}

- (NSString *)scope
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->scope();
}

- (void)setScope:(NSString *)newScope
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setScope(newScope);
}

@end

CyberCore::HTMLTableCellElement* core(DOMHTMLTableCellElement *wrapper)
{
    return wrapper ? reinterpret_cast<CyberCore::HTMLTableCellElement*>(wrapper->_internal) : 0;
}

DOMHTMLTableCellElement *kit(CyberCore::HTMLTableCellElement* value)
{
    CyberCoreThreadViolationCheckRoundOne();
    return static_cast<DOMHTMLTableCellElement*>(kit(static_cast<CyberCore::Node*>(value)));
}

#undef IMPL
