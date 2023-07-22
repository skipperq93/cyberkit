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

#import "DOMHTMLElementInternal.h"

#import "DOMElementInternal.h"
#import "DOMHTMLCollectionInternal.h"
#import "DOMNodeInternal.h"
#import <CyberCore/Element.h>
#import "ExceptionHandlers.h"
#import <CyberCore/HTMLCollection.h>
#import <CyberCore/HTMLElement.h>
#import <CyberCore/HTMLNames.h>
#import <CyberCore/HitTestResult.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLElement

- (NSString *)title
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::titleAttr);
}

- (void)setTitle:(NSString *)newTitle
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::titleAttr, newTitle);
}

- (NSString *)lang
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::langAttr);
}

- (void)setLang:(NSString *)newLang
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::langAttr, newLang);
}

- (BOOL)translate
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->translate();
}

- (void)setTranslate:(BOOL)newTranslate
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setTranslate(newTranslate);
}

- (NSString *)dir
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->dir();
}

- (void)setDir:(NSString *)newDir
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setDir(newDir);
}

- (int)tabIndex
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->tabIndexForBindings();
}

- (void)setTabIndex:(int)newTabIndex
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setTabIndexForBindings(newTabIndex);
}

- (BOOL)draggable
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->draggable();
}

- (void)setDraggable:(BOOL)newDraggable
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setDraggable(newDraggable);
}

- (NSString *)webkitdropzone
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::webkitdropzoneAttr);
}

- (void)setWebkitdropzone:(NSString *)newWebkitdropzone
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::webkitdropzoneAttr, newWebkitdropzone);
}

- (BOOL)hidden
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::hiddenAttr);
}

- (void)setHidden:(BOOL)newHidden
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::hiddenAttr, newHidden);
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

- (NSString *)innerText
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->innerText();
}

- (void)setInnerText:(NSString *)newInnerText
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setInnerText(newInnerText));
}

- (NSString *)outerText
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->outerText();
}

- (void)setOuterText:(NSString *)newOuterText
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setOuterText(newOuterText));
}

- (NSString *)contentEditable
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->contentEditable();
}

- (void)setContentEditable:(NSString *)newContentEditable
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setContentEditable(newContentEditable));
}

- (BOOL)isContentEditable
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->isContentEditable();
}

- (BOOL)spellcheck
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->spellcheck();
}

- (void)setSpellcheck:(BOOL)newSpellcheck
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setSpellcheck(newSpellcheck);
}

- (NSString *)idName
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getIdAttribute();
}

- (void)setIdName:(NSString *)newIdName
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::idAttr, newIdName);
}

- (DOMHTMLCollection *)children
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->children()));
}

- (NSString *)titleDisplayString
{
    CyberCore::JSMainThreadNullState state;
    return CyberCore::displayString(IMPL->title(), core(self));
}

- (DOMElement *)insertAdjacentElement:(NSString *)where element:(DOMElement *)element
{
    CyberCore::JSMainThreadNullState state;
    if (!element)
        raiseTypeErrorException();
    return kit(raiseOnDOMError(IMPL->insertAdjacentElement(where, *core(element))));
}

- (void)insertAdjacentHTML:(NSString *)where html:(NSString *)html
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->insertAdjacentHTML(where, html));
}

- (void)insertAdjacentText:(NSString *)where text:(NSString *)text
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->insertAdjacentText(where, text));
}

- (void)click
{
    CyberCore::JSMainThreadNullState state;
    IMPL->click();
}

#if ENABLE(IOS_AUTOCORRECT_AND_AUTOCAPITALIZE)

- (BOOL)autocorrect
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->shouldAutocorrect();
}

- (void)setAutocorrect:(BOOL)newAutocorrect
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAutocorrect(newAutocorrect);
}

- (NSString *)autocapitalize
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->autocapitalize();
}

- (void)setAutocapitalize:(NSString *)newAutocapitalize
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAutocapitalize(newAutocapitalize);
}

#endif

@end

CyberCore::HTMLElement* core(DOMHTMLElement *wrapper)
{
    return wrapper ? reinterpret_cast<CyberCore::HTMLElement*>(wrapper->_internal) : 0;
}

DOMHTMLElement *kit(CyberCore::HTMLElement* value)
{
    CyberCoreThreadViolationCheckRoundOne();
    return static_cast<DOMHTMLElement*>(kit(static_cast<CyberCore::Node*>(value)));
}

#undef IMPL
