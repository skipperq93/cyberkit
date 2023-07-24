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

#import "DOMHTMLSelectElementInternal.h"

#import "DOMHTMLCollectionInternal.h"
#import "DOMHTMLElementInternal.h"
#import "DOMHTMLFormElementInternal.h"
#import "DOMHTMLOptionsCollectionInternal.h"
#import "DOMNodeInternal.h"
#import "DOMNodeListInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/HTMLCollection.h>
#import <CyberCore/HTMLElement.h>
#import <CyberCore/HTMLFormElement.h>
#import <CyberCore/HTMLNames.h>
#import <CyberCore/HTMLOptGroupElement.h>
#import <CyberCore/HTMLOptionsCollection.h>
#import <CyberCore/HTMLSelectElement.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/NameNodeList.h>
#import <CyberCore/Node.h>
#import <CyberCore/NodeList.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLSelectElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLSelectElement

- (BOOL)autofocus
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::autofocusAttr);
}

- (void)setAutofocus:(BOOL)newAutofocus
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::autofocusAttr, newAutofocus);
}

- (BOOL)disabled
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::disabledAttr);
}

- (void)setDisabled:(BOOL)newDisabled
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::disabledAttr, newDisabled);
}

- (DOMHTMLFormElement *)form
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->form()));
}

- (BOOL)multiple
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->multiple();
}

- (void)setMultiple:(BOOL)newMultiple
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setMultiple(newMultiple);
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

- (int)size
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->size();
}

- (void)setSize:(int)newSize
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setSize(newSize);
}

- (NSString *)type
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->type();
}

- (DOMHTMLOptionsCollection *)options
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->options()));
}

- (int)length
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->length();
}

- (int)selectedIndex
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->selectedIndex();
}

- (void)setSelectedIndex:(int)newSelectedIndex
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setSelectedIndex(newSelectedIndex);
}

- (NSString *)value
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->value();
}

- (void)setValue:(NSString *)newValue
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setValue(newValue);
}

- (BOOL)willValidate
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->willValidate();
}

- (DOMNode *)item:(unsigned)index
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->item(index)));
}

- (DOMNode *)namedItem:(NSString *)inName
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->namedItem(inName)));
}

- (void)add:(DOMHTMLElement *)element before:(DOMHTMLElement *)before
{
    CyberCore::JSMainThreadNullState state;
    if (!element)
        raiseTypeErrorException();

    auto& coreElement = *core(element);
    Variant<RefPtr<CyberCore::HTMLOptionElement>, RefPtr<CyberCore::HTMLOptGroupElement>> variantElement;
    if (is<CyberCore::HTMLOptionElement>(coreElement))
        variantElement = &downcast<CyberCore::HTMLOptionElement>(coreElement);
    else if (is<CyberCore::HTMLOptGroupElement>(coreElement))
        variantElement = &downcast<CyberCore::HTMLOptGroupElement>(coreElement);
    else
        raiseTypeErrorException();
    raiseOnDOMError(IMPL->add(WTFMove(variantElement), CyberCore::HTMLSelectElement::HTMLElementOrInt(core(before))));
}

- (void)remove:(int)index
{
    CyberCore::JSMainThreadNullState state;
    IMPL->remove(index);
}

@end

@implementation DOMHTMLSelectElement (DOMHTMLSelectElementDeprecated)

- (void)add:(DOMHTMLElement *)element :(DOMHTMLElement *)before
{
    [self add:element before:before];
}

@end

CyberCore::HTMLSelectElement* core(DOMHTMLSelectElement *wrapper)
{
    return wrapper ? reinterpret_cast<CyberCore::HTMLSelectElement*>(wrapper->_internal) : 0;
}

#undef IMPL
