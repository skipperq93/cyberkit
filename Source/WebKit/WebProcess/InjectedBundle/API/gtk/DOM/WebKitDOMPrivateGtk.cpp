/*
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 *  Copyright (C) 2007 Samuel Weinig <sam@webkit.org>
 *  Copyright (C) 2008 Luke Kenneth Casson Leighton <lkcl@lkcl.net>
 *  Copyright (C) 2008 Martin Soto <soto@freedesktop.org>
 *  Copyright (C) 2009-2013 Igalia S.L.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "CyberKitDOMPrivate.h"

#include <CyberCore/Blob.h>
#include <CyberCore/Element.h>
#include <CyberCore/Event.h>
#include <CyberCore/EventTarget.h>
#include <CyberCore/File.h>
#include <CyberCore/HTMLElement.h>
#include <CyberCore/HTMLNames.h>
#include <CyberCore/KeyboardEvent.h>
#include <CyberCore/MouseEvent.h>
#include <CyberCore/StyleSheet.h>
#include <CyberCore/UIEvent.h>
#include "CyberKitDOMAttrPrivate.h"
#include "CyberKitDOMBlobPrivate.h"
#include "CyberKitDOMCDATASectionPrivate.h"
#include "CyberKitDOMCSSStyleSheetPrivate.h"
#include "CyberKitDOMCommentPrivate.h"
#include "CyberKitDOMDOMWindowPrivate.h"
#include "CyberKitDOMDocumentFragmentPrivate.h"
#include "CyberKitDOMDocumentPrivate.h"
#include "CyberKitDOMDocumentTypePrivate.h"
#include "CyberKitDOMElementPrivate.h"
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTargetPrivate.h"
#include "CyberKitDOMFilePrivate.h"
#include "CyberKitDOMHTMLCollectionPrivate.h"
#include "CyberKitDOMHTMLDocumentPrivate.h"
#include "CyberKitDOMHTMLOptionsCollectionPrivate.h"
#include "CyberKitDOMHTMLPrivate.h"
#include "CyberKitDOMKeyboardEventPrivate.h"
#include "CyberKitDOMMouseEventPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMProcessingInstructionPrivate.h"
#include "CyberKitDOMStyleSheetPrivate.h"
#include "CyberKitDOMTextPrivate.h"
#include "CyberKitDOMUIEventPrivate.h"
#include "CyberKitDOMWheelEventPrivate.h"

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

using namespace CyberCore;
using namespace CyberCore::HTMLNames;

CyberKitDOMNode* wrapNodeGtk(Node* node)
{
    ASSERT(node);
    ASSERT(node->nodeType());

    switch (node->nodeType()) {
    case Node::ELEMENT_NODE:
        if (is<HTMLElement>(*node))
            return WEBKIT_DOM_NODE(wrap(downcast<HTMLElement>(node)));
        return nullptr;
    case Node::ATTRIBUTE_NODE:
        return WEBKIT_DOM_NODE(wrapAttr(static_cast<Attr*>(node)));
    case Node::TEXT_NODE:
        return WEBKIT_DOM_NODE(wrapText(downcast<Text>(node)));
    case Node::CDATA_SECTION_NODE:
        return WEBKIT_DOM_NODE(wrapCDATASection(static_cast<CDATASection*>(node)));
    case Node::PROCESSING_INSTRUCTION_NODE:
        return WEBKIT_DOM_NODE(wrapProcessingInstruction(static_cast<ProcessingInstruction*>(node)));
    case Node::COMMENT_NODE:
        return WEBKIT_DOM_NODE(wrapComment(static_cast<Comment*>(node)));
    case Node::DOCUMENT_NODE:
        if (is<HTMLDocument>(*node))
            return WEBKIT_DOM_NODE(wrapHTMLDocument(downcast<HTMLDocument>(node)));
        return nullptr;
    case Node::DOCUMENT_TYPE_NODE:
        return WEBKIT_DOM_NODE(wrapDocumentType(static_cast<DocumentType*>(node)));
    case Node::DOCUMENT_FRAGMENT_NODE:
        return WEBKIT_DOM_NODE(wrapDocumentFragment(static_cast<DocumentFragment*>(node)));
    }

    return wrapNode(node);
}

CyberKitDOMEvent* wrap(Event* event)
{
    ASSERT(event);

    if (is<UIEvent>(*event)) {
        if (is<MouseEvent>(*event))
            return WEBKIT_DOM_EVENT(wrapMouseEvent(&downcast<MouseEvent>(*event)));
        if (is<KeyboardEvent>(*event))
            return WEBKIT_DOM_EVENT(wrapKeyboardEvent(&downcast<KeyboardEvent>(*event)));
        if (is<WheelEvent>(*event))
            return WEBKIT_DOM_EVENT(wrapWheelEvent(&downcast<WheelEvent>(*event)));
        return WEBKIT_DOM_EVENT(wrapUIEvent(&downcast<UIEvent>(*event)));
    }

    return wrapEvent(event);
}

CyberKitDOMStyleSheet* wrap(StyleSheet* styleSheet)
{
    ASSERT(styleSheet);

    if (is<CSSStyleSheet>(*styleSheet))
        return WEBKIT_DOM_STYLE_SHEET(wrapCSSStyleSheet(downcast<CSSStyleSheet>(styleSheet)));
    return wrapStyleSheet(styleSheet);
}

CyberKitDOMHTMLCollection* wrap(HTMLCollection* collection)
{
    ASSERT(collection);

    if (is<HTMLOptionsCollection>(*collection))
        return WEBKIT_DOM_HTML_COLLECTION(wrapHTMLOptionsCollection(downcast<HTMLOptionsCollection>(collection)));
    return wrapHTMLCollection(collection);
}

CyberKitDOMEventTarget* wrap(EventTarget* eventTarget)
{
    ASSERT(eventTarget);

    if (is<Node>(*eventTarget))
        return WEBKIT_DOM_EVENT_TARGET(kit(&downcast<Node>(*eventTarget)));
    if (is<DOMWindow>(*eventTarget))
        return WEBKIT_DOM_EVENT_TARGET(kit(&downcast<DOMWindow>(*eventTarget)));
    return nullptr;
}

CyberKitDOMBlob* wrap(Blob* blob)
{
    ASSERT(blob);

    if (blob->isFile())
        return WEBKIT_DOM_BLOB(wrapFile(static_cast<File*>(blob)));
    return wrapBlob(blob);
}

} // namespace CyberKit
G_GNUC_END_IGNORE_DEPRECATIONS;
