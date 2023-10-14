/*
 * Copyright (C) 2012 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "config.h"
#import "WKDOMInternals.h"

#import <CyberCore/Document.h>
#import <CyberCore/Element.h>
#import <CyberCore/Node.h>
#import <CyberCore/Range.h>
#import <CyberCore/Text.h>
#import <wtf/NeverDestroyed.h>

// Classes to instantiate.
#import "WKDOMElement.h"
#import "WKDOMDocument.h"
#import "WKDOMText.h"

#if PLATFORM(IOS_FAMILY)
#import <CyberCore/WAKAppKitStubs.h>
#endif

namespace CyberKit {

template<typename CyberCoreType, typename WKDOMType>
static WKDOMType toWKDOMType(CyberCoreType impl, DOMCache<CyberCoreType, WKDOMType>& cache);

// -- Caches -- 

DOMCache<CyberCore::Node*, __unsafe_unretained WKDOMNode *>& WKDOMNodeCache()
{
    static NeverDestroyed<DOMCache<CyberCore::Node*, __unsafe_unretained WKDOMNode *>> cache;
    return cache;
}

DOMCache<CyberCore::Range*, __unsafe_unretained WKDOMRange *>& WKDOMRangeCache()
{
    static NeverDestroyed<DOMCache<CyberCore::Range*, __unsafe_unretained WKDOMRange *>> cache;
    return cache;
}

// -- Node and classes derived from Node. --

static Class WKDOMNodeClass(CyberCore::Node* impl)
{
    switch (impl->nodeType()) {
    case CyberCore::Node::ELEMENT_NODE:
        return [WKDOMElement class];
    case CyberCore::Node::DOCUMENT_NODE:
        return [WKDOMDocument class];
    case CyberCore::Node::TEXT_NODE:
        return [WKDOMText class];
    case CyberCore::Node::ATTRIBUTE_NODE:
    case CyberCore::Node::CDATA_SECTION_NODE:
    case CyberCore::Node::PROCESSING_INSTRUCTION_NODE:
    case CyberCore::Node::COMMENT_NODE:
    case CyberCore::Node::DOCUMENT_TYPE_NODE:
    case CyberCore::Node::DOCUMENT_FRAGMENT_NODE:
        return [WKDOMNode class];
    }
    ASSERT_NOT_REACHED();
    return nil;
}

static RetainPtr<WKDOMNode> createWrapper(CyberCore::Node* impl)
{
    return adoptNS([[WKDOMNodeClass(impl) alloc] _initWithImpl:impl]);
}

CyberCore::Node* toCyberCoreNode(WKDOMNode *wrapper)
{
    return wrapper ? wrapper->_impl.get() : 0;
}

WKDOMNode *toWKDOMNode(CyberCore::Node* impl)
{
    return toWKDOMType<CyberCore::Node*, __unsafe_unretained WKDOMNode *>(impl, WKDOMNodeCache());
}

CyberCore::Element* toCyberCoreElement(WKDOMElement *wrapper)
{
    return wrapper ? reinterpret_cast<CyberCore::Element*>(wrapper->_impl.get()) : 0;
}

WKDOMElement *toWKDOMElement(CyberCore::Element* impl)
{
    return static_cast<WKDOMElement*>(toWKDOMNode(static_cast<CyberCore::Node*>(impl)));
}

CyberCore::Document* toCyberCoreDocument(WKDOMDocument *wrapper)
{
    return wrapper ? reinterpret_cast<CyberCore::Document*>(wrapper->_impl.get()) : 0;
}

WKDOMDocument *toWKDOMDocument(CyberCore::Document* impl)
{
    return static_cast<WKDOMDocument*>(toWKDOMNode(static_cast<CyberCore::Node*>(impl)));
}

CyberCore::Text* toCyberCoreText(WKDOMText *wrapper)
{
    return wrapper ? reinterpret_cast<CyberCore::Text*>(wrapper->_impl.get()) : 0;
}

WKDOMText *toWKDOMText(CyberCore::Text* impl)
{
    return static_cast<WKDOMText*>(toWKDOMNode(static_cast<CyberCore::Node*>(impl)));
}

// -- Range. --

static RetainPtr<WKDOMRange> createWrapper(CyberCore::Range* impl)
{
    return adoptNS([[WKDOMRange alloc] _initWithImpl:impl]);
}

CyberCore::Range* toCyberCoreRange(WKDOMRange * wrapper)
{
    return wrapper ? wrapper->_impl.get() : 0;
}

WKDOMRange *toWKDOMRange(CyberCore::Range* impl)
{
    return toWKDOMType<CyberCore::Range*, __unsafe_unretained WKDOMRange *>(impl, WKDOMRangeCache());
}

// -- Helpers --

template<typename CyberCoreType, typename WKDOMType>
static WKDOMType toWKDOMType(CyberCoreType impl, DOMCache<CyberCoreType, WKDOMType>& cache)
{
    if (!impl)
        return nil;
    if (WKDOMType wrapper = cache.get(impl))
        return retainPtr(wrapper).autorelease();
    auto wrapper = createWrapper(impl);
    if (!wrapper)
        return nil;
    return wrapper.autorelease();
}

} // namespace CyberKit
