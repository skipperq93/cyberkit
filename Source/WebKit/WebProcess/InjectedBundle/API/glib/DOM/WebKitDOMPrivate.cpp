/*
 *  Copyright (C) 2018 Igalia S.L.
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

#include "CyberKitDOMDocumentPrivate.h"
#include "CyberKitDOMElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include <CyberCore/HTMLFormElement.h>

#if PLATFORM(GTK)
#include "CyberKitDOMPrivateGtk.h"
#endif

namespace CyberKit {

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

CyberKitDOMNode* wrap(CyberCore::Node* node)
{
    ASSERT(node);
    ASSERT(node->nodeType());

#if PLATFORM(GTK)
    if (auto* wrapper = wrapNodeGtk(node))
        return wrapper;
#endif

    switch (node->nodeType()) {
    case CyberCore::Node::ELEMENT_NODE:
        return WEBKIT_DOM_NODE(wrapElement(downcast<CyberCore::Element>(node)));
    case CyberCore::Node::DOCUMENT_NODE:
        return WEBKIT_DOM_NODE(wrapDocument(downcast<CyberCore::Document>(node)));
    case CyberCore::Node::ATTRIBUTE_NODE:
    case CyberCore::Node::TEXT_NODE:
    case CyberCore::Node::CDATA_SECTION_NODE:
    case CyberCore::Node::PROCESSING_INSTRUCTION_NODE:
    case CyberCore::Node::COMMENT_NODE:
    case CyberCore::Node::DOCUMENT_TYPE_NODE:
    case CyberCore::Node::DOCUMENT_FRAGMENT_NODE:
        break;
    }

    return wrapNode(node);
}

G_GNUC_END_IGNORE_DEPRECATIONS;

} // namespace CyberKit
