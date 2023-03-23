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

#import "DOMElementInternal.h"

#import "DOMAttrInternal.h"
#import "DOMCSSStyleDeclarationInternal.h"
#import "DOMHTMLCollectionInternal.h"
#import "DOMNodeInternal.h"
#import "DOMNodeListInternal.h"
#import "DOMTokenListInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/Attr.h>
#import <CyberCore/CSSStyleDeclaration.h>
#import <CyberCore/Element.h>
#import <CyberCore/HTMLCollection.h>
#import <CyberCore/HTMLNames.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/NameNodeList.h>
#import <CyberCore/NodeList.h>
#import <CyberCore/ScrollIntoViewOptions.h>
#import <CyberCore/StyleProperties.h>
#import <CyberCore/StyledElement.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>

static inline CyberCore::Element& unwrap(DOMElement& wrapper)
{
    ASSERT(wrapper._internal);
    return downcast<CyberCore::Element>(reinterpret_cast<CyberCore::Node&>(*wrapper._internal));
}

CyberCore::Element* core(DOMElement *wrapper)
{
    return wrapper ? &unwrap(*wrapper) : nullptr;
}

DOMElement *kit(CyberCore::Element* value)
{
    CyberCoreThreadViolationCheckRoundOne();
    return static_cast<DOMElement*>(kit(static_cast<CyberCore::Node*>(value)));
}

@implementation DOMElement

- (NSString *)tagName
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).tagName();
}

- (DOMCSSStyleDeclaration *)style
{
    CyberCore::JSMainThreadNullState state;
    auto& element = unwrap(*self);
    return is<CyberCore::StyledElement>(element) ? kit(&downcast<CyberCore::StyledElement>(element).cssomStyle()) : nullptr;
}

- (int)offsetLeft
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).offsetLeftForBindings();
}

- (int)offsetTop
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).offsetTopForBindings();
}

- (int)offsetWidth
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).offsetWidth();
}

- (int)offsetHeight
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).offsetHeight();
}

- (int)clientLeft
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).clientLeft();
}

- (int)clientTop
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).clientTop();
}

- (int)clientWidth
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).clientWidth();
}

- (int)clientHeight
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).clientHeight();
}

- (int)scrollLeft
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).scrollLeft();
}

- (void)setScrollLeft:(int)newScrollLeft
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setScrollLeft(newScrollLeft);
}

- (int)scrollTop
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).scrollTop();
}

- (void)setScrollTop:(int)newScrollTop
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setScrollTop(newScrollTop);
}

- (int)scrollWidth
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).scrollWidth();
}

- (int)scrollHeight
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).scrollHeight();
}

- (DOMElement *)offsetParent
{
    CyberCore::JSMainThreadNullState state;
    return kit(unwrap(*self).offsetParentForBindings());
}

- (NSString *)innerHTML
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).innerHTML();
}

- (void)setInnerHTML:(NSString *)newInnerHTML
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(unwrap(*self).setInnerHTML(newInnerHTML));
}

- (NSString *)outerHTML
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).outerHTML();
}

- (void)setOuterHTML:(NSString *)newOuterHTML
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(unwrap(*self).setOuterHTML(newOuterHTML));
}

- (NSString *)className
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).getAttribute(CyberCore::HTMLNames::classAttr);
}

- (void)setClassName:(NSString *)newClassName
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setAttributeWithoutSynchronization(CyberCore::HTMLNames::classAttr, newClassName);
}

- (DOMTokenList *)classList
{
    CyberCore::JSMainThreadNullState state;
    return kit(&unwrap(*self).classList());
}

- (NSString *)innerText
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).innerText();
}

- (NSString *)uiactions
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).getAttribute(CyberCore::HTMLNames::uiactionsAttr);
}

- (void)setUiactions:(NSString *)newUiactions
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).setAttributeWithoutSynchronization(CyberCore::HTMLNames::uiactionsAttr, newUiactions);
}

- (DOMElement *)previousElementSibling
{
    CyberCore::JSMainThreadNullState state;
    return kit(unwrap(*self).previousElementSibling());
}

- (DOMElement *)nextElementSibling
{
    CyberCore::JSMainThreadNullState state;
    return kit(unwrap(*self).nextElementSibling());
}

- (DOMHTMLCollection *)children
{
    CyberCore::JSMainThreadNullState state;
    return kit(unwrap(*self).children().ptr());
}

- (DOMElement *)firstElementChild
{
    CyberCore::JSMainThreadNullState state;
    return kit(unwrap(*self).firstElementChild());
}

- (DOMElement *)lastElementChild
{
    CyberCore::JSMainThreadNullState state;
    return kit(unwrap(*self).lastElementChild());
}

- (unsigned)childElementCount
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).childElementCount();
}

#if PLATFORM(IOS_FAMILY)
- (CGRect)boundsInRootViewSpace
{
    CyberCore::JSMainThreadNullState state;
    auto bounds = unwrap(*self).boundsInRootViewSpace();
    return CGRectMake(bounds.x(), bounds.y(), bounds.width(), bounds.height());
}
#endif

- (NSString *)getAttribute:(NSString *)name
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).getAttribute(name);
}

- (void)setAttribute:(NSString *)name value:(NSString *)value
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(unwrap(*self).setAttribute(name, value));
}

- (void)removeAttribute:(NSString *)name
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).removeAttribute(name);
}

- (DOMAttr *)getAttributeNode:(NSString *)name
{
    CyberCore::JSMainThreadNullState state;
    return kit(unwrap(*self).getAttributeNode(name).get());
}

- (DOMAttr *)setAttributeNode:(DOMAttr *)newAttr
{
    CyberCore::JSMainThreadNullState state;
    if (!newAttr)
        raiseTypeErrorException();
    return kit(raiseOnDOMError(unwrap(*self).setAttributeNode(*core(newAttr))).get());
}

- (DOMAttr *)removeAttributeNode:(DOMAttr *)oldAttr
{
    CyberCore::JSMainThreadNullState state;
    if (!oldAttr)
        raiseTypeErrorException();
    return kit(raiseOnDOMError(unwrap(*self).removeAttributeNode(*core(oldAttr))).ptr());
}

- (DOMNodeList *)getElementsByTagName:(NSString *)name
{
    if (!name)
        return nullptr;

    CyberCore::JSMainThreadNullState state;
    Ref<CyberCore::NodeList> result = unwrap(*self).getElementsByTagName(name);
    return kit(result.ptr());
}

- (NSString *)getAttributeNS:(NSString *)namespaceURI localName:(NSString *)localName
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).getAttributeNS(namespaceURI, localName);
}

- (void)setAttributeNS:(NSString *)namespaceURI qualifiedName:(NSString *)qualifiedName value:(NSString *)value
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(unwrap(*self).setAttributeNS(namespaceURI, qualifiedName, value));
}

- (void)removeAttributeNS:(NSString *)namespaceURI localName:(NSString *)localName
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).removeAttributeNS(namespaceURI, localName);
}

- (DOMNodeList *)getElementsByTagNameNS:(NSString *)namespaceURI localName:(NSString *)localName
{
    if (!localName)
        return nullptr;

    CyberCore::JSMainThreadNullState state;
    Ref<CyberCore::NodeList> result = unwrap(*self).getElementsByTagNameNS(namespaceURI, localName);
    return kit(result.ptr());
}

- (DOMAttr *)getAttributeNodeNS:(NSString *)namespaceURI localName:(NSString *)localName
{
    CyberCore::JSMainThreadNullState state;
    return kit(unwrap(*self).getAttributeNodeNS(namespaceURI, localName).get());
}

- (DOMAttr *)setAttributeNodeNS:(DOMAttr *)newAttr
{
    CyberCore::JSMainThreadNullState state;
    if (!newAttr)
        raiseTypeErrorException();
    return kit(raiseOnDOMError(unwrap(*self).setAttributeNodeNS(*core(newAttr))).get());
}

- (BOOL)hasAttribute:(NSString *)name
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).hasAttribute(name);
}

- (BOOL)hasAttributeNS:(NSString *)namespaceURI localName:(NSString *)localName
{
    CyberCore::JSMainThreadNullState state;
    return unwrap(*self).hasAttributeNS(namespaceURI, localName);
}

- (void)focus
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).focus();
}

- (void)blur
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).blur();
}

- (void)scrollIntoView:(BOOL)alignWithTop
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).scrollIntoView(alignWithTop);
}

- (void)scrollIntoViewIfNeeded:(BOOL)centerIfNeeded
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).scrollIntoViewIfNeeded(centerIfNeeded);
}

- (void)scrollByLines:(int)lines
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).scrollByLines(lines);
}

- (void)scrollByPages:(int)pages
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).scrollByPages(pages);
}

- (DOMNodeList *)getElementsByClassName:(NSString *)name
{
    CyberCore::JSMainThreadNullState state;
    Ref<CyberCore::NodeList> result = unwrap(*self).getElementsByClassName(name);
    return kit(result.ptr());
}

- (BOOL)matches:(NSString *)selectors
{
    CyberCore::JSMainThreadNullState state;
    return raiseOnDOMError(unwrap(*self).matches(selectors));
}

- (DOMElement *)closest:(NSString *)selectors
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(unwrap(*self).closest(selectors)));
}

- (BOOL)webkitMatchesSelector:(NSString *)selectors
{
    CyberCore::JSMainThreadNullState state;
    return raiseOnDOMError(unwrap(*self).matches(selectors));
}

#if ENABLE(FULLSCREEN_API)

- (void)webkitRequestFullScreen:(unsigned short)flags
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).webkitRequestFullscreen();
}

- (void)webkitRequestFullscreen
{
    CyberCore::JSMainThreadNullState state;
    unwrap(*self).webkitRequestFullscreen();
}

#endif

- (void)remove
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(unwrap(*self).remove());
}

- (DOMElement *)querySelector:(NSString *)selectors
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(unwrap(*self).querySelector(selectors)));
}

- (DOMNodeList *)querySelectorAll:(NSString *)selectors
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(unwrap(*self).querySelectorAll(selectors)).ptr());
}

@end

@implementation DOMElement (DOMElementDeprecated)

- (void)setAttribute:(NSString *)name :(NSString *)value
{
    [self setAttribute:name value:value];
}

- (NSString *)getAttributeNS:(NSString *)namespaceURI :(NSString *)localName
{
    return [self getAttributeNS:namespaceURI localName:localName];
}

- (void)setAttributeNS:(NSString *)namespaceURI :(NSString *)qualifiedName :(NSString *)value
{
    [self setAttributeNS:namespaceURI qualifiedName:qualifiedName value:value];
}

- (void)removeAttributeNS:(NSString *)namespaceURI :(NSString *)localName
{
    [self removeAttributeNS:namespaceURI localName:localName];
}

- (DOMNodeList *)getElementsByTagNameNS:(NSString *)namespaceURI :(NSString *)localName
{
    return [self getElementsByTagNameNS:namespaceURI localName:localName];
}

- (DOMAttr *)getAttributeNodeNS:(NSString *)namespaceURI :(NSString *)localName
{
    return [self getAttributeNodeNS:namespaceURI localName:localName];
}

- (BOOL)hasAttributeNS:(NSString *)namespaceURI :(NSString *)localName
{
    return [self hasAttributeNS:namespaceURI localName:localName];
}

@end
