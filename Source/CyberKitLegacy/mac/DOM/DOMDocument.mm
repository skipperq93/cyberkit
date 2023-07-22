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

#import "DOMDocumentInternal.h"

#import <CyberCore/Attr.h>
#import <CyberCore/CDATASection.h>
#import <CyberCore/CSSRuleList.h>
#import <CyberCore/CSSStyleDeclaration.h>
#import <CyberCore/Comment.h>
#import <CyberCore/DocumentFullscreen.h>
#import "DOMAbstractViewInternal.h"
#import "DOMAttrInternal.h"
#import "DOMCDATASectionInternal.h"
#import "DOMCSSRuleListInternal.h"
#import "DOMCSSStyleDeclarationInternal.h"
#import "DOMCommentInternal.h"
#import "DOMCustomXPathNSResolver.h"
#import "DOMDocumentFragmentInternal.h"
#import "DOMDocumentTypeInternal.h"
#import "DOMElementInternal.h"
#import "DOMEventInternal.h"
#import "DOMHTMLCollectionInternal.h"
#import "DOMHTMLElementInternal.h"
#import "DOMHTMLHeadElementInternal.h"
#import "DOMHTMLScriptElementInternal.h"
#import "DOMImplementationInternal.h"
#import "DOMInternal.h"
#import "DOMNodeInternal.h"
#import "DOMNodeIteratorInternal.h"
#import "DOMNodeListInternal.h"
#import "DOMProcessingInstructionInternal.h"
#import "DOMRangeInternal.h"
#import "DOMStyleSheetListInternal.h"
#import "DOMTextInternal.h"
#import "DOMTreeWalkerInternal.h"
#import <CyberCore/DOMWindow.h>
#import "DOMXPathExpressionInternal.h"
#import "DOMXPathResultInternal.h"
#import <CyberCore/Document.h>
#import <CyberCore/DocumentFragment.h>
#import <CyberCore/DocumentType.h>
#import <CyberCore/Element.h>
#import <CyberCore/Event.h>
#import "ExceptionHandlers.h"
#import <CyberCore/HTMLCollection.h>
#import <CyberCore/HTMLElement.h>
#import <CyberCore/HTMLHeadElement.h>
#import <CyberCore/HTMLScriptElement.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/NameNodeList.h>
#import <CyberCore/NativeNodeFilter.h>
#import <CyberCore/Node.h>
#import <CyberCore/NodeIterator.h>
#import <CyberCore/NodeList.h>
#import "ObjCNodeFilterCondition.h"
#import <CyberCore/ProcessingInstruction.h>
#import <CyberCore/Range.h>
#import <CyberCore/StyleProperties.h>
#import <CyberCore/StyleSheetList.h>
#import <CyberCore/Text.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/TreeWalker.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <CyberCore/XPathExpression.h>
#import <CyberCore/XPathNSResolver.h>
#import <CyberCore/XPathResult.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::Document*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMDocument

- (DOMDocumentType *)doctype
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->doctype()));
}

- (DOMImplementation *)implementation
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->implementation()));
}

- (DOMElement *)documentElement
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->documentElement()));
}

- (NSString *)inputEncoding
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->characterSetWithUTF8Fallback();
}

- (NSString *)xmlEncoding
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->xmlEncoding();
}

- (NSString *)xmlVersion
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->xmlVersion();
}

- (void)setXmlVersion:(NSString *)newXmlVersion
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setXMLVersion(newXmlVersion));
}

- (BOOL)xmlStandalone
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->xmlStandalone();
}

- (void)setXmlStandalone:(BOOL)newXmlStandalone
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setXMLStandalone(newXmlStandalone);
}

- (NSString *)documentURI
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->documentURI();
}

- (void)setDocumentURI:(NSString *)newDocumentURI
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setDocumentURI(newDocumentURI);
}

- (DOMAbstractView *)defaultView
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->windowProxy()));
}

- (DOMStyleSheetList *)styleSheets
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->styleSheets()));
}

- (NSString *)contentType
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->contentType();
}

- (NSString *)title
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->title();
}

- (void)setTitle:(NSString *)newTitle
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setTitle(newTitle);
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

- (NSString *)referrer
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->referrer();
}

- (NSString *)domain
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->domain();
}

- (NSString *)URL
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->urlForBindings();
}

- (NSString *)cookie
{
    CyberCore::JSMainThreadNullState state;
    return raiseOnDOMError(IMPL->cookie());
}

- (void)setCookie:(NSString *)newCookie
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setCookie(newCookie));
}

- (DOMHTMLElement *)body
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->bodyOrFrameset()));
}

- (void)setBody:(DOMHTMLElement *)newBody
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setBodyOrFrameset(core(newBody)));
}

- (DOMHTMLHeadElement *)head
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->head()));
}

- (DOMHTMLCollection *)images
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->images()));
}

- (DOMHTMLCollection *)applets
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->applets()));
}

- (DOMHTMLCollection *)links
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->links()));
}

- (DOMHTMLCollection *)forms
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->forms()));
}

- (DOMHTMLCollection *)anchors
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->anchors()));
}

- (NSString *)lastModified
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->lastModified();
}

- (NSString *)charset
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->charset();
}

- (void)setCharset:(NSString *)newCharset
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setCharset(newCharset);
}

- (NSString *)defaultCharset
{
    return IMPL->defaultCharsetForLegacyBindings();
}

- (NSString *)readyState
{
    CyberCore::JSMainThreadNullState state;
    auto readyState = IMPL->readyState();
    switch (readyState) {
    case CyberCore::Document::Loading:
        return @"loading";
    case CyberCore::Document::Interactive:
        return @"interactive";
    case CyberCore::Document::Complete:
        return @"complete";
    }
    ASSERT_NOT_REACHED();
    return @"complete";
}

- (NSString *)characterSet
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->characterSetWithUTF8Fallback();
}

- (NSString *)preferredStylesheetSet
{
    return nil;
}

- (NSString *)selectedStylesheetSet
{
    return nil;
}

- (void)setSelectedStylesheetSet:(NSString *)newSelectedStylesheetSet
{
}

- (DOMElement *)activeElement
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->activeElement()));
}

- (NSString *)compatMode
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->compatMode();
}

#if ENABLE(FULLSCREEN_API)

- (BOOL)webkitIsFullScreen
{
    CyberCore::JSMainThreadNullState state;
    return CyberCore::DocumentFullscreen::webkitIsFullScreen(*IMPL);
}

- (BOOL)webkitFullScreenKeyboardInputAllowed
{
    CyberCore::JSMainThreadNullState state;
    return CyberCore::DocumentFullscreen::webkitFullScreenKeyboardInputAllowed(*IMPL);
}

- (DOMElement *)webkitCurrentFullScreenElement
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(CyberCore::DocumentFullscreen::webkitCurrentFullScreenElement(*IMPL)));
}

- (BOOL)webkitFullscreenEnabled
{
    CyberCore::JSMainThreadNullState state;
    return CyberCore::DocumentFullscreen::webkitFullscreenEnabled(*IMPL);
}

- (DOMElement *)webkitFullscreenElement
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(CyberCore::DocumentFullscreen::webkitFullscreenElement(*IMPL)));
}

#endif

- (NSString *)visibilityState
{
    CyberCore::JSMainThreadNullState state;
    switch (IMPL->visibilityState()) {
    case CyberCore::VisibilityState::Hidden:
        return @"hidden";
    case CyberCore::VisibilityState::Visible:
        return @"visible";
    case CyberCore::VisibilityState::Prerender:
        return @"prerender";
    }
}

- (BOOL)hidden
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hidden();
}

- (DOMHTMLScriptElement *)currentScript
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->currentScript()));
}

- (NSString *)origin
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->origin();
}

- (DOMElement *)scrollingElement
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->scrollingElementForAPI()));
}

- (DOMHTMLCollection *)children
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->children()));
}

- (DOMElement *)firstElementChild
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->firstElementChild()));
}

- (DOMElement *)lastElementChild
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->lastElementChild()));
}

- (unsigned)childElementCount
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->childElementCount();
}

- (DOMElement *)createElement:(NSString *)tagName
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->createElementForBindings(tagName)).ptr());
}

- (DOMDocumentFragment *)createDocumentFragment
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->createDocumentFragment()));
}

- (DOMText *)createTextNode:(NSString *)data
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->createTextNode(data)));
}

- (DOMComment *)createComment:(NSString *)data
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->createComment(data)));
}

- (DOMCDATASection *)createCDATASection:(NSString *)data
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->createCDATASection(data)).ptr());
}

- (DOMProcessingInstruction *)createProcessingInstruction:(NSString *)target data:(NSString *)data
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->createProcessingInstruction(target, data)).ptr());
}

- (DOMAttr *)createAttribute:(NSString *)name
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->createAttribute(name)).ptr());
}

- (DOMEntityReference *)createEntityReference:(NSString *)name
{
    UNUSED_PARAM(name);
    raiseNotSupportedErrorException();
}

- (DOMNodeList *)getElementsByTagName:(NSString *)tagname
{
    if (!tagname)
        return nullptr;

    CyberCore::JSMainThreadNullState state;
    return kit(static_cast<CyberCore::NodeList*>(WTF::getPtr(IMPL->getElementsByTagName(tagname))));
}

- (DOMNode *)importNode:(DOMNode *)importedNode deep:(BOOL)deep
{
    CyberCore::JSMainThreadNullState state;
    if (!importedNode)
        raiseTypeErrorException();
    return kit(raiseOnDOMError(IMPL->importNode(*core(importedNode), deep)).ptr());
}

- (DOMElement *)createElementNS:(NSString *)namespaceURI qualifiedName:(NSString *)qualifiedName
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->createElementNS(namespaceURI, qualifiedName)).ptr());
}

- (DOMAttr *)createAttributeNS:(NSString *)namespaceURI qualifiedName:(NSString *)qualifiedName
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->createAttributeNS(namespaceURI, qualifiedName)).ptr());
}

- (DOMNodeList *)getElementsByTagNameNS:(NSString *)namespaceURI localName:(NSString *)localName
{
    if (!localName)
        return nullptr;
    CyberCore::JSMainThreadNullState state;
    return kit(static_cast<CyberCore::NodeList*>(WTF::getPtr(IMPL->getElementsByTagNameNS(namespaceURI, localName))));
}

- (DOMNode *)adoptNode:(DOMNode *)source
{
    CyberCore::JSMainThreadNullState state;
    if (!source)
        raiseTypeErrorException();
    return kit(raiseOnDOMError(IMPL->adoptNode(*core(source))).ptr());
}

- (DOMEvent *)createEvent:(NSString *)eventType
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->createEvent(eventType)).ptr());
}

- (DOMRange *)createRange
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->createRange()));
}

- (DOMNodeIterator *)createNodeIterator:(DOMNode *)root whatToShow:(unsigned)whatToShow filter:(id <DOMNodeFilter>)filter expandEntityReferences:(BOOL)expandEntityReferences
{
    CyberCore::JSMainThreadNullState state;
    if (!root)
        raiseTypeErrorException();
    RefPtr<CyberCore::NodeFilter> nativeNodeFilter;
    if (filter)
        nativeNodeFilter = CyberCore::NativeNodeFilter::create(IMPL, CyberCore::ObjCNodeFilterCondition::create(filter));
    return kit(WTF::getPtr(IMPL->createNodeIterator(*core(root), whatToShow, WTF::getPtr(nativeNodeFilter), expandEntityReferences)));
}

- (DOMTreeWalker *)createTreeWalker:(DOMNode *)root whatToShow:(unsigned)whatToShow filter:(id <DOMNodeFilter>)filter expandEntityReferences:(BOOL)expandEntityReferences
{
    CyberCore::JSMainThreadNullState state;
    if (!root)
        raiseTypeErrorException();
    RefPtr<CyberCore::NodeFilter> nativeNodeFilter;
    if (filter)
        nativeNodeFilter = CyberCore::NativeNodeFilter::create(IMPL, CyberCore::ObjCNodeFilterCondition::create(filter));
    return kit(WTF::getPtr(IMPL->createTreeWalker(*core(root), whatToShow, WTF::getPtr(nativeNodeFilter), expandEntityReferences)));
}

- (DOMCSSStyleDeclaration *)getOverrideStyle:(DOMElement *)element pseudoElement:(NSString *)pseudoElement
{
    return nil;
}

static RefPtr<CyberCore::XPathNSResolver> wrap(id <DOMXPathNSResolver> resolver)
{
    if (!resolver)
        return nullptr;
    if ([resolver isMemberOfClass:[DOMNativeXPathNSResolver class]])
        return core(static_cast<DOMNativeXPathNSResolver *>(resolver));
    return DOMCustomXPathNSResolver::create(resolver);
}

- (DOMXPathExpression *)createExpression:(NSString *)expression resolver:(id <DOMXPathNSResolver>)resolver
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->createExpression(expression, wrap(resolver))).ptr());
}

- (id <DOMXPathNSResolver>)createNSResolver:(DOMNode *)nodeResolver
{
    CyberCore::JSMainThreadNullState state;
    if (!nodeResolver)
        return nullptr;

    return kit(WTF::getPtr(IMPL->createNSResolver(*core(nodeResolver))));
}

- (DOMXPathResult *)evaluate:(NSString *)expression contextNode:(DOMNode *)contextNode resolver:(id <DOMXPathNSResolver>)resolver type:(unsigned short)type inResult:(DOMXPathResult *)inResult
{
    if (!contextNode)
        return nullptr;

    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->evaluate(expression, *core(contextNode), wrap(resolver), type, core(inResult))).ptr());
}

- (BOOL)execCommand:(NSString *)command userInterface:(BOOL)userInterface value:(NSString *)value
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->execCommand(command, userInterface, value);
}

- (BOOL)execCommand:(NSString *)command userInterface:(BOOL)userInterface
{
    return [self execCommand:command userInterface:userInterface value:nullptr];
}

- (BOOL)execCommand:(NSString *)command
{
    return [self execCommand:command userInterface:NO value:nullptr];
}

- (BOOL)queryCommandEnabled:(NSString *)command
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->queryCommandEnabled(command);
}

- (BOOL)queryCommandIndeterm:(NSString *)command
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->queryCommandIndeterm(command);
}

- (BOOL)queryCommandState:(NSString *)command
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->queryCommandState(command);
}

- (BOOL)queryCommandSupported:(NSString *)command
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->queryCommandSupported(command);
}

- (NSString *)queryCommandValue:(NSString *)command
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->queryCommandValue(command);
}

- (DOMNodeList *)getElementsByName:(NSString *)elementName
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->getElementsByName(elementName)));
}

- (DOMElement *)elementFromPoint:(int)x y:(int)y
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->elementFromPoint(x, y)));
}

- (DOMRange *)caretRangeFromPoint:(int)x y:(int)y
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->caretRangeFromPoint(x, y)));
}

- (DOMCSSStyleDeclaration *)createCSSStyleDeclaration
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->createCSSStyleDeclaration()));
}

- (DOMCSSStyleDeclaration *)getComputedStyle:(DOMElement *)element pseudoElement:(NSString *)pseudoElement
{
    CyberCore::JSMainThreadNullState state;
    if (!element)
        raiseTypeErrorException();
    CyberCore::DOMWindow* dv = IMPL->domWindow();
    if (!dv)
        return nil;
    return kit(WTF::getPtr(dv->getComputedStyle(*core(element), pseudoElement)));
}

- (DOMCSSRuleList *)getMatchedCSSRules:(DOMElement *)element pseudoElement:(NSString *)pseudoElement
{
    return [self getMatchedCSSRules:element pseudoElement:pseudoElement authorOnly:YES];
}

- (DOMCSSRuleList *)getMatchedCSSRules:(DOMElement *)element pseudoElement:(NSString *)pseudoElement authorOnly:(BOOL)authorOnly
{
    CyberCore::JSMainThreadNullState state;
    CyberCore::DOMWindow* dv = IMPL->domWindow();
    if (!dv)
        return nil;
    return kit(WTF::getPtr(dv->getMatchedCSSRules(core(element), pseudoElement, authorOnly)));
}

- (DOMNodeList *)getElementsByClassName:(NSString *)classNames
{
    CyberCore::JSMainThreadNullState state;
    return kit(static_cast<CyberCore::NodeList*>(WTF::getPtr(IMPL->getElementsByClassName(classNames))));
}

- (BOOL)hasFocus
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasFocus();
}

#if ENABLE(FULLSCREEN_API)

- (void)webkitCancelFullScreen
{
    CyberCore::JSMainThreadNullState state;
    CyberCore::DocumentFullscreen::webkitCancelFullScreen(*IMPL);
}

- (void)webkitExitFullscreen
{
    CyberCore::JSMainThreadNullState state;
    CyberCore::DocumentFullscreen::webkitExitFullscreen(*IMPL);
}

#endif

- (DOMElement *)getElementById:(NSString *)elementId
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->getElementById(AtomString(elementId))));
}

- (DOMElement *)querySelector:(NSString *)selectors
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->querySelector(selectors)));
}

- (DOMNodeList *)querySelectorAll:(NSString *)selectors
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->querySelectorAll(selectors)).ptr());
}

@end

@implementation DOMDocument (DOMDocumentDeprecated)

- (DOMProcessingInstruction *)createProcessingInstruction:(NSString *)target :(NSString *)data
{
    return [self createProcessingInstruction:target data:data];
}

- (DOMNode *)importNode:(DOMNode *)importedNode :(BOOL)deep
{
    return [self importNode:importedNode deep:deep];
}

- (DOMElement *)createElementNS:(NSString *)namespaceURI :(NSString *)qualifiedName
{
    return [self createElementNS:namespaceURI qualifiedName:qualifiedName];
}

- (DOMAttr *)createAttributeNS:(NSString *)namespaceURI :(NSString *)qualifiedName
{
    return [self createAttributeNS:namespaceURI qualifiedName:qualifiedName];
}

- (DOMNodeList *)getElementsByTagNameNS:(NSString *)namespaceURI :(NSString *)localName
{
    return [self getElementsByTagNameNS:namespaceURI localName:localName];
}

- (DOMNodeIterator *)createNodeIterator:(DOMNode *)root :(unsigned)whatToShow :(id <DOMNodeFilter>)filter :(BOOL)expandEntityReferences
{
    return [self createNodeIterator:root whatToShow:whatToShow filter:filter expandEntityReferences:expandEntityReferences];
}

- (DOMTreeWalker *)createTreeWalker:(DOMNode *)root :(unsigned)whatToShow :(id <DOMNodeFilter>)filter :(BOOL)expandEntityReferences
{
    return [self createTreeWalker:root whatToShow:whatToShow filter:filter expandEntityReferences:expandEntityReferences];
}

- (DOMCSSStyleDeclaration *)getOverrideStyle:(DOMElement *)element :(NSString *)pseudoElement
{
    return nil;
}

- (DOMXPathExpression *)createExpression:(NSString *)expression :(id <DOMXPathNSResolver>)resolver
{
    return [self createExpression:expression resolver:resolver];
}

- (DOMXPathResult *)evaluate:(NSString *)expression :(DOMNode *)contextNode :(id <DOMXPathNSResolver>)resolver :(unsigned short)type :(DOMXPathResult *)inResult
{
    return [self evaluate:expression contextNode:contextNode resolver:resolver type:type inResult:inResult];
}

- (DOMCSSStyleDeclaration *)getComputedStyle:(DOMElement *)element :(NSString *)pseudoElement
{
    return [self getComputedStyle:element pseudoElement:pseudoElement];
}

@end

CyberCore::Document* core(DOMDocument *wrapper)
{
    return wrapper ? reinterpret_cast<CyberCore::Document*>(wrapper->_internal) : 0;
}

DOMDocument *kit(CyberCore::Document* value)
{
    CyberCoreThreadViolationCheckRoundOne();
    return static_cast<DOMDocument*>(kit(static_cast<CyberCore::Node*>(value)));
}

#undef IMPL
