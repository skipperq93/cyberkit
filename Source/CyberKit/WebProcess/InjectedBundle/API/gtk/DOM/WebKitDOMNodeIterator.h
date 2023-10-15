/*
 *  This file is part of the CyberKit open source project.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#if !defined(__WEBKITDOM_H_INSIDE__) && !defined(BUILDING_WEBKIT)
#error "Only <webkitdom/webkitdom.h> can be included directly."
#endif

#ifndef CyberKitDOMNodeIterator_h
#define CyberKitDOMNodeIterator_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMObject.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_NODE_ITERATOR            (webkit_dom_node_iterator_get_type())
#define WEBKIT_DOM_NODE_ITERATOR(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_NODE_ITERATOR, CyberKitDOMNodeIterator))
#define WEBKIT_DOM_NODE_ITERATOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_NODE_ITERATOR, CyberKitDOMNodeIteratorClass)
#define WEBKIT_DOM_IS_NODE_ITERATOR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_NODE_ITERATOR))
#define WEBKIT_DOM_IS_NODE_ITERATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_NODE_ITERATOR))
#define WEBKIT_DOM_NODE_ITERATOR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_NODE_ITERATOR, CyberKitDOMNodeIteratorClass))

struct _CyberKitDOMNodeIterator {
    CyberKitDOMObject parent_instance;
};

struct _CyberKitDOMNodeIteratorClass {
    CyberKitDOMObjectClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_node_iterator_get_type(void);

/**
 * webkit_dom_node_iterator_next_node:
 * @self: A #CyberKitDOMNodeIterator
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_node_iterator_next_node(CyberKitDOMNodeIterator* self, GError** error);

/**
 * webkit_dom_node_iterator_previous_node:
 * @self: A #CyberKitDOMNodeIterator
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_node_iterator_previous_node(CyberKitDOMNodeIterator* self, GError** error);

/**
 * webkit_dom_node_iterator_detach:
 * @self: A #CyberKitDOMNodeIterator
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_node_iterator_detach(CyberKitDOMNodeIterator* self);

/**
 * webkit_dom_node_iterator_get_root:
 * @self: A #CyberKitDOMNodeIterator
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_node_iterator_get_root(CyberKitDOMNodeIterator* self);

/**
 * webkit_dom_node_iterator_get_what_to_show:
 * @self: A #CyberKitDOMNodeIterator
 *
 * Returns: A #gulong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gulong
webkit_dom_node_iterator_get_what_to_show(CyberKitDOMNodeIterator* self);

/**
 * webkit_dom_node_iterator_get_filter:
 * @self: A #CyberKitDOMNodeIterator
 *
 * Returns: (transfer full): A #CyberKitDOMNodeFilter
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNodeFilter*
webkit_dom_node_iterator_get_filter(CyberKitDOMNodeIterator* self);

/**
 * webkit_dom_node_iterator_get_reference_node:
 * @self: A #CyberKitDOMNodeIterator
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_node_iterator_get_reference_node(CyberKitDOMNodeIterator* self);

/**
 * webkit_dom_node_iterator_get_pointer_before_reference_node:
 * @self: A #CyberKitDOMNodeIterator
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_node_iterator_get_pointer_before_reference_node(CyberKitDOMNodeIterator* self);

G_END_DECLS

#endif /* CyberKitDOMNodeIterator_h */
