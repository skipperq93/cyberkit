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

#import "DOMHTMLTextAreaElementInternal.h"

#import "DOMHTMLFormElementInternal.h"
#import "DOMNodeInternal.h"
#import "DOMNodeListInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/HTMLTextAreaElement.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/NodeList.h>
#import <CyberCore/ThreadCheck.h>

static inline CyberCore::HTMLTextAreaElement& unwrap(DOMHTMLTextAreaElement& wrapper)
{
    ASSERT(wrapper._internal);
    return downcast<CyberCore::HTMLTextAreaElement>(reinterpret_cast<CyberCore::Node&>(*wrapper._internal));
}

CyberCore::HTMLTextAreaElement* core(DOMHTMLTextAreaElement *wrapper)
{
    return wrapper ? &unwrap(*wrapper) : nullptr;
}

DOMHTMLTextAreaElement *kit(CyberCore::HTMLTextAreaElement* value)
{
    CyberCoreThreadViolationCheckRoundOne();
    return static_cast<DOMHTMLTextAreaElement*>(kit(static_cast<CyberCore::Node*>(value)));
}

@implementation DOMHTMLTextAreaElement

- (BOOL)autofocus
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).hasAttributeWithoutSynchronization(CyberCore::HTMLNames::autofocusAttr);
}

- (void)setAutofocus:(BOOL)newAutofocus
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setBooleanAttribute(CyberCore::HTMLNames::autofocusAttr, newAutofocus);
}

- (NSString *)dirName
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).getAttribute(CyberCore::HTMLNames::dirnameAttr);
}

- (void)setDirName:(NSString *)newDirName
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setAttributeWithoutSynchronization(CyberCore::HTMLNames::dirnameAttr, newDirName);
}

- (BOOL)disabled
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).hasAttributeWithoutSynchronization(CyberCore::HTMLNames::disabledAttr);
}

- (void)setDisabled:(BOOL)newDisabled
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setBooleanAttribute(CyberCore::HTMLNames::disabledAttr, newDisabled);
}

- (DOMHTMLFormElement *)form
{
    CyberCore::JSMainThreadNullState state;
    return kit(unwrap(*self).form());
}

- (int)maxLength
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).maxLength();
}

- (void)setMaxLength:(int)newMaxLength
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(unwrap(*self).setMaxLength(newMaxLength));
}

- (NSString *)name
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).getNameAttribute();
}

- (void)setName:(NSString *)newName
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setAttributeWithoutSynchronization(CyberCore::HTMLNames::nameAttr, newName);
}

- (NSString *)placeholder
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).getAttribute(CyberCore::HTMLNames::placeholderAttr);
}

- (void)setPlaceholder:(NSString *)newPlaceholder
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setAttributeWithoutSynchronization(CyberCore::HTMLNames::placeholderAttr, newPlaceholder);
}

- (BOOL)readOnly
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).hasAttributeWithoutSynchronization(CyberCore::HTMLNames::readonlyAttr);
}

- (void)setReadOnly:(BOOL)newReadOnly
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setBooleanAttribute(CyberCore::HTMLNames::readonlyAttr, newReadOnly);
}

- (BOOL)required
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).hasAttributeWithoutSynchronization(CyberCore::HTMLNames::requiredAttr);
}

- (void)setRequired:(BOOL)newRequired
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setBooleanAttribute(CyberCore::HTMLNames::requiredAttr, newRequired);
}

- (int)rows
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).rows();
}

- (void)setRows:(int)newRows
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setRows(newRows);
}

- (int)cols
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).cols();
}

- (void)setCols:(int)newCols
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setCols(newCols);
}

- (NSString *)wrap
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).getAttribute(CyberCore::HTMLNames::wrapAttr);
}

- (void)setWrap:(NSString *)newWrap
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setAttributeWithoutSynchronization(CyberCore::HTMLNames::wrapAttr, newWrap);
}

- (NSString *)type
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).type();
}

- (NSString *)defaultValue
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).defaultValue();
}

- (void)setDefaultValue:(NSString *)newDefaultValue
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setDefaultValue(newDefaultValue);
}

- (NSString *)value
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).value();
}

- (void)setValue:(NSString *)newValue
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setValue(newValue);
}

- (unsigned)textLength
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).textLength();
}

- (BOOL)willValidate
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).willValidate();
}

- (DOMNodeList *)labels
{
    CyberCore::JSMainThreadNullState state;
    return kit(unwrap(*self).labels().get());
}

- (int)selectionStart
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).selectionStart();
}

- (void)setSelectionStart:(int)newSelectionStart
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setSelectionStart(newSelectionStart);
}

- (int)selectionEnd
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).selectionEnd();
}

- (void)setSelectionEnd:(int)newSelectionEnd
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setSelectionEnd(newSelectionEnd);
}

- (NSString *)selectionDirection
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).selectionDirection();
}

- (void)setSelectionDirection:(NSString *)newSelectionDirection
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setSelectionDirection(newSelectionDirection);
}

- (NSString *)accessKey
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).getAttribute(CyberCore::HTMLNames::accesskeyAttr);
}

- (void)setAccessKey:(NSString *)newAccessKey
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setAttributeWithoutSynchronization(CyberCore::HTMLNames::accesskeyAttr, newAccessKey);
}

- (NSString *)autocomplete
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).autocomplete();
}

- (void)setAutocomplete:(NSString *)newAutocomplete
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setAutocomplete(newAutocomplete);
}

- (void)select
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).select();
}

- (void)setRangeText:(NSString *)replacement
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(unwrap(*self).setRangeText(replacement));
}

- (void)setRangeText:(NSString *)replacement start:(unsigned)start end:(unsigned)end selectionMode:(NSString *)selectionMode
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(unwrap(*self).setRangeText(replacement, start, end, selectionMode));
}

- (void)setSelectionRange:(int)start end:(int)end
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setSelectionRange(start, end);
}

@end
