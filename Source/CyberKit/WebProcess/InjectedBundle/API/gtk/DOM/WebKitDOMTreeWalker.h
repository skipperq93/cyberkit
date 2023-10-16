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

#ifndef CyberKitDOMTreeWalker_h
#define CyberKitDOMTreeWalker_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMObject.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_TREE_WALKER            (webkit_dom_tree_walker_get_type())
#define WEBKIT_DOM_TREE_WALKER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_TREE_WALKER, CyberKitDOMTreeWalker))
#define WEBKIT_DOM_TREE_WALKER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_TREE_WALKER, CyberKitDOMTreeWalkerClass)
#define WEBKIT_DOM_IS_TREE_WALKER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_TREE_WALKER))
#define WEBKIT_DOM_IS_TREE_WALKER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_TREE_WALKER))
#define WEBKIT_DOM_TREE_WALKER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_TREE_WALKER, CyberKitDOMTreeWalkerClass))

struct _CyberKitDOMTreeWalker {
    CyberKitDOMObject parent_instance;
};

struct _CyberKitDOMTreeWalkerClass {
    CyberKitDOMObjectClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_tree_walker_get_type(void);

/**
 * webkit_dom_tree_walker_parent_node:
 * @self: A #CyberKitDOMTreeWalker
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_tree_walker_parent_node(CyberKitDOMTreeWalker* self);

/**
 * webkit_dom_tree_walker_first_child:
 * @self: A #CyberKitDOMTreeWalker
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_tree_walker_first_child(CyberKitDOMTreeWalker* self);

/**
 * webkit_dom_tree_walker_last_child:
 * @self: A #CyberKitDOMTreeWalker
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_tree_walker_last_child(CyberKitDOMTreeWalker* self);

/**
 * webkit_dom_tree_walker_previous_sibling:
 * @self: A #CyberKitDOMTreeWalker
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_tree_walker_previous_sibling(CyberKitDOMTreeWalker* self);

/**
 * webkit_dom_tree_walker_next_sibling:
 * @self: A #CyberKitDOMTreeWalker
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_tree_walker_next_sibling(CyberKitDOMTreeWalker* self);

/**
 * webkit_dom_tree_walker_previous_node:
 * @self: A #CyberKitDOMTreeWalker
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_tree_walker_previous_node(CyberKitDOMTreeWalker* self);

/**
 * webkit_dom_tree_walker_next_node:
 * @self: A #CyberKitDOMTreeWalker
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_tree_walker_next_node(CyberKitDOMTreeWalker* self);

/**
 * webkit_dom_tree_walker_get_root:
 * @self: A #CyberKitDOMTreeWalker
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_tree_walker_get_root(CyberKitDOMTreeWalker* self);

/**
 * webkit_dom_tree_walker_get_what_to_show:
 * @self: A #CyberKitDOMTreeWalker
 *
 * Returns: A #gulong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gulong
webkit_dom_tree_walker_get_what_to_show(CyberKitDOMTreeWalker* self);

/**
 * webkit_dom_tree_walker_get_filter:
 * @self: A #CyberKitDOMTreeWalker
 *
 * Returns: (transfer full): A #CyberKitDOMNodeFilter
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNodeFilter*
webkit_dom_tree_walker_get_filter(CyberKitDOMTreeWalker* self);

/**
 * webkit_dom_tree_walker_get_current_node:
 * @self: A #CyberKitDOMTreeWalker
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_tree_walker_get_current_node(CyberKitDOMTreeWalker* self);

/**
 * webkit_dom_tree_walker_set_current_node:
 * @self: A #CyberKitDOMTreeWalker
 * @value: A #CyberKitDOMNode
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_tree_walker_set_current_node(CyberKitDOMTreeWalker* self, CyberKitDOMNode* value, GError** error);

G_END_DECLS

#endif /* CyberKitDOMTreeWalker_h */
