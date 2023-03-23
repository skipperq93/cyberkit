/*
 *  Copyright (C) 2014 Igalia S.L.
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

#ifndef CyberKitDOMNodeFilter_h
#define CyberKitDOMNodeFilter_h

#include <glib-object.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_NODE_FILTER            (webkit_dom_node_filter_get_type ())
#define WEBKIT_DOM_NODE_FILTER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WEBKIT_DOM_TYPE_NODE_FILTER, CyberKitDOMNodeFilter))
#define WEBKIT_DOM_NODE_FILTER_CLASS(obj)      (G_TYPE_CHECK_CLASS_CAST ((obj), WEBKIT_DOM_TYPE_NODE_FILTER, CyberKitDOMNodeFilterIface))
#define WEBKIT_DOM_IS_NODE_FILTER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WEBKIT_DOM_TYPE_NODE_FILTER))
#define WEBKIT_DOM_NODE_FILTER_GET_IFACE(obj)  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), WEBKIT_DOM_TYPE_NODE_FILTER, CyberKitDOMNodeFilterIface))

#ifndef WEBKIT_DISABLE_DEPRECATED

/**
 * WEBKIT_DOM_NODE_FILTER_ACCEPT:
 *
 * Accept the node. Use this macro as return value of webkit_dom_node_filter_accept_node()
 * implementation to accept the given #CyberKitDOMNode
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_ACCEPT 1

/**
 * WEBKIT_DOM_NODE_FILTER_REJECT:
 *
 * Reject the node. Use this macro as return value of webkit_dom_node_filter_accept_node()
 * implementation to reject the given #CyberKitDOMNode. The children of the given node will
 * be rejected too.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_REJECT 2

/**
 * WEBKIT_DOM_NODE_FILTER_SKIP:
 *
 * Skip the node. Use this macro as return value of webkit_dom_node_filter_accept_node()
 * implementation to skip the given #CyberKitDOMNode. The children of the given node will
 * not be skipped.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_SKIP   3

/**
 * WEBKIT_DOM_NODE_FILTER_SHOW_ALL:
 *
 * Show all nodes.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_SHOW_ALL                    0xFFFFFFFF

/**
 * WEBKIT_DOM_NODE_FILTER_SHOW_ELEMENT:
 *
 * Show #CyberKitDOMElement nodes.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_SHOW_ELEMENT                0x00000001

/**
 * WEBKIT_DOM_NODE_FILTER_SHOW_ATTRIBUTE:
 *
 * Show #CyberKitDOMAttr nodes.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_SHOW_ATTRIBUTE              0x00000002

/**
 * WEBKIT_DOM_NODE_FILTER_SHOW_TEXT:
 *
 * Show #CyberKitDOMText nodes.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_SHOW_TEXT                   0x00000004

/**
 * WEBKIT_DOM_NODE_FILTER_SHOW_CDATA_SECTION:
 *
 * Show #CyberKitDOMCDataSection nodes.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_SHOW_CDATA_SECTION          0x00000008

/**
 * WEBKIT_DOM_NODE_FILTER_SHOW_ENTITY_REFERENCE:
 *
 * Show #CyberKitDOMEntityReference nodes.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_SHOW_ENTITY_REFERENCE       0x00000010

/**
 * WEBKIT_DOM_NODE_FILTER_SHOW_ENTITY:
 *
 * Show #CyberKitDOMEntity nodes.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_SHOW_ENTITY                 0x00000020

/**
 * WEBKIT_DOM_NODE_FILTER_SHOW_PROCESSING_INSTRUCTION:
 *
 * Show #CyberKitDOMProcessingInstruction nodes.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_SHOW_PROCESSING_INSTRUCTION 0x00000040

/**
 * WEBKIT_DOM_NODE_FILTER_SHOW_COMMENT:
 *
 * Show #CyberKitDOMComment nodes.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_SHOW_COMMENT                0x00000080

/**
 * WEBKIT_DOM_NODE_FILTER_SHOW_DOCUMENT:
 *
 * Show #CyberKitDOMDocument nodes.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_SHOW_DOCUMENT               0x00000100

/**
 * WEBKIT_DOM_NODE_FILTER_SHOW_DOCUMENT_TYPE:
 *
 * Show #CyberKitDOMDocumentType nodes.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_SHOW_DOCUMENT_TYPE          0x00000200

/**
 * WEBKIT_DOM_NODE_FILTER_SHOW_DOCUMENT_FRAGMENT:
 *
 * Show #CyberKitDOMDocumentFragment nodes.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_SHOW_DOCUMENT_FRAGMENT      0x00000400

/**
 * WEBKIT_DOM_NODE_FILTER_SHOW_NOTATION:
 *
 * Show #CyberKitDOMNotation nodes.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_NODE_FILTER_SHOW_NOTATION               0x00000800

#endif /* WEBKIT_DISABLE_DEPRECATED */

struct _CyberKitDOMNodeFilterIface {
    GTypeInterface gIface;

    /* virtual table */
    gshort (* accept_node)(CyberKitDOMNodeFilter *filter,
                           CyberKitDOMNode       *node);

    void (*_webkitdom_reserved0) (void);
    void (*_webkitdom_reserved1) (void);
    void (*_webkitdom_reserved2) (void);
    void (*_webkitdom_reserved3) (void);
};


WEBKIT_DEPRECATED GType webkit_dom_node_filter_get_type(void) G_GNUC_CONST;

/**
 * webkit_dom_node_filter_accept_node:
 * @filter: A #CyberKitDOMNodeFilter
 * @node: A #CyberKitDOMNode
 *
 * Returns: a #gshort
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED gshort webkit_dom_node_filter_accept_node(CyberKitDOMNodeFilter *filter,
                                                     CyberKitDOMNode       *node);

G_END_DECLS

#endif /* CyberKitDOMNodeFilter_h */
