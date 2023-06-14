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

#import "WKDOMNode.h"
#import "WKDOMRange.h"
#import <CyberCore/Node.h>
#import <CyberCore/Range.h>
#import <wtf/HashMap.h>

#if WK_API_ENABLED

namespace CyberCore {
class Element;
class Document;
}

@class WKDOMElement;
@class WKDOMDocument;
@class WKDOMText;

@interface WKDOMNode () {
@package
    RefPtr<CyberCore::Node> _impl;
}

- (id)_initWithImpl:(CyberCore::Node*)impl;
@end

@interface WKDOMRange () {
@package
    RefPtr<CyberCore::Range> _impl;
}

- (id)_initWithImpl:(CyberCore::Range*)impl;
@end

namespace CyberKit {

template<typename CyberCoreType, typename WKDOMType>
class DOMCache {
public:
    DOMCache()
    {
    }

    void add(CyberCoreType core, WKDOMType kit)
    {
        m_map.add(core, kit);
    }
    
    WKDOMType get(CyberCoreType core)
    {
        return m_map.get(core);
    }

    void remove(CyberCoreType core)
    {
        m_map.remove(core);
    }

private:
    // This class should only ever be used as a singleton.
    ~DOMCache() = delete;

    HashMap<CyberCoreType, WKDOMType> m_map;
};

// -- Caches --

DOMCache<CyberCore::Node*, __unsafe_unretained WKDOMNode *>& WKDOMNodeCache();
DOMCache<CyberCore::Range*, __unsafe_unretained WKDOMRange *>& WKDOMRangeCache();

// -- Node and classes derived from Node. --

CyberCore::Node* toCyberCoreNode(WKDOMNode *);
WKDOMNode *toWKDOMNode(CyberCore::Node*);

CyberCore::Element* toCyberCoreElement(WKDOMElement *);
WKDOMElement *toWKDOMElement(CyberCore::Element*);

CyberCore::Document* toCyberCoreDocument(WKDOMDocument *);
WKDOMDocument *toWKDOMDocument(CyberCore::Document*);

CyberCore::Text* toCyberCoreText(WKDOMText *);
WKDOMText *toWKDOMText(CyberCore::Text*);

// -- Range. --

CyberCore::Range* toCyberCoreRange(WKDOMRange *);
WKDOMRange *toWKDOMRange(CyberCore::Range*);

// -- Helpers --

NSArray *toNSArray(const Vector<CyberCore::IntRect>&);

} // namespace CyberKit

#endif // WK_API_ENABLED
