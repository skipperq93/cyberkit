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

#if !defined(__WEBKITDOM_H_INSIDE__) && !defined(BUILDING_WEBKIT) && !defined(WEBKIT_DOM_USE_UNSTABLE_API)
#error "Only <webkitdom/webkitdom.h> can be included directly."
#endif

#ifndef CyberKitDOMDOMSelection_h
#define CyberKitDOMDOMSelection_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMObject.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_DOM_SELECTION            (webkit_dom_dom_selection_get_type())
#define WEBKIT_DOM_DOM_SELECTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_DOM_SELECTION, CyberKitDOMDOMSelection))
#define WEBKIT_DOM_DOM_SELECTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_DOM_SELECTION, CyberKitDOMDOMSelectionClass)
#define WEBKIT_DOM_IS_DOM_SELECTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_DOM_SELECTION))
#define WEBKIT_DOM_IS_DOM_SELECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_DOM_SELECTION))
#define WEBKIT_DOM_DOM_SELECTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_DOM_SELECTION, CyberKitDOMDOMSelectionClass))

struct _CyberKitDOMDOMSelection {
    CyberKitDOMObject parent_instance;
};

struct _CyberKitDOMDOMSelectionClass {
    CyberKitDOMObjectClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_dom_selection_get_type(void);

/**
 * webkit_dom_dom_selection_collapse:
 * @self: A #CyberKitDOMDOMSelection
 * @node: A #CyberKitDOMNode
 * @offset: A #gulong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_selection_collapse(CyberKitDOMDOMSelection* self, CyberKitDOMNode* node, gulong offset);

/**
 * webkit_dom_dom_selection_collapse_to_end:
 * @self: A #CyberKitDOMDOMSelection
 * @error: #GError
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_selection_collapse_to_end(CyberKitDOMDOMSelection* self, GError** error);

/**
 * webkit_dom_dom_selection_collapse_to_start:
 * @self: A #CyberKitDOMDOMSelection
 * @error: #GError
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_selection_collapse_to_start(CyberKitDOMDOMSelection* self, GError** error);

/**
 * webkit_dom_dom_selection_delete_from_document:
 * @self: A #CyberKitDOMDOMSelection
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_selection_delete_from_document(CyberKitDOMDOMSelection* self);

/**
 * webkit_dom_dom_selection_contains_node:
 * @self: A #CyberKitDOMDOMSelection
 * @node: A #CyberKitDOMNode
 * @allowPartial: A #gboolean
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED gboolean
webkit_dom_dom_selection_contains_node(CyberKitDOMDOMSelection* self, CyberKitDOMNode* node, gboolean allowPartial);

/**
 * webkit_dom_dom_selection_select_all_children:
 * @self: A #CyberKitDOMDOMSelection
 * @node: A #CyberKitDOMNode
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_selection_select_all_children(CyberKitDOMDOMSelection* self, CyberKitDOMNode* node);

/**
 * webkit_dom_dom_selection_extend:
 * @self: A #CyberKitDOMDOMSelection
 * @node: A #CyberKitDOMNode
 * @offset: A #gulong
 * @error: #GError
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_selection_extend(CyberKitDOMDOMSelection* self, CyberKitDOMNode* node, gulong offset, GError** error);

/**
 * webkit_dom_dom_selection_get_range_at:
 * @self: A #CyberKitDOMDOMSelection
 * @index: A #gulong
 * @error: #GError
 *
 * Returns: (transfer full): A #CyberKitDOMRange
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED CyberKitDOMRange*
webkit_dom_dom_selection_get_range_at(CyberKitDOMDOMSelection* self, gulong index, GError** error);

/**
 * webkit_dom_dom_selection_remove_all_ranges:
 * @self: A #CyberKitDOMDOMSelection
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_selection_remove_all_ranges(CyberKitDOMDOMSelection* self);

/**
 * webkit_dom_dom_selection_add_range:
 * @self: A #CyberKitDOMDOMSelection
 * @range: A #CyberKitDOMRange
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_selection_add_range(CyberKitDOMDOMSelection* self, CyberKitDOMRange* range);

/**
 * webkit_dom_dom_selection_set_base_and_extent:
 * @self: A #CyberKitDOMDOMSelection
 * @baseNode: A #CyberKitDOMNode
 * @baseOffset: A #gulong
 * @extentNode: A #CyberKitDOMNode
 * @extentOffset: A #gulong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_selection_set_base_and_extent(CyberKitDOMDOMSelection* self, CyberKitDOMNode* baseNode, gulong baseOffset, CyberKitDOMNode* extentNode, gulong extentOffset);

/**
 * webkit_dom_dom_selection_set_position:
 * @self: A #CyberKitDOMDOMSelection
 * @node: A #CyberKitDOMNode
 * @offset: A #gulong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_selection_set_position(CyberKitDOMDOMSelection* self, CyberKitDOMNode* node, gulong offset);

/**
 * webkit_dom_dom_selection_empty:
 * @self: A #CyberKitDOMDOMSelection
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_selection_empty(CyberKitDOMDOMSelection* self);

/**
 * webkit_dom_dom_selection_modify:
 * @self: A #CyberKitDOMDOMSelection
 * @alter: A #gchar
 * @direction: A #gchar
 * @granularity: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_selection_modify(CyberKitDOMDOMSelection* self, const gchar* alter, const gchar* direction, const gchar* granularity);

/**
 * webkit_dom_dom_selection_get_anchor_node:
 * @self: A #CyberKitDOMDOMSelection
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_dom_selection_get_anchor_node(CyberKitDOMDOMSelection* self);

/**
 * webkit_dom_dom_selection_get_anchor_offset:
 * @self: A #CyberKitDOMDOMSelection
 *
 * Returns: A #gulong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED gulong
webkit_dom_dom_selection_get_anchor_offset(CyberKitDOMDOMSelection* self);

/**
 * webkit_dom_dom_selection_get_focus_node:
 * @self: A #CyberKitDOMDOMSelection
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_dom_selection_get_focus_node(CyberKitDOMDOMSelection* self);

/**
 * webkit_dom_dom_selection_get_focus_offset:
 * @self: A #CyberKitDOMDOMSelection
 *
 * Returns: A #gulong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED gulong
webkit_dom_dom_selection_get_focus_offset(CyberKitDOMDOMSelection* self);

/**
 * webkit_dom_dom_selection_get_is_collapsed:
 * @self: A #CyberKitDOMDOMSelection
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED gboolean
webkit_dom_dom_selection_get_is_collapsed(CyberKitDOMDOMSelection* self);

/**
 * webkit_dom_dom_selection_get_range_count:
 * @self: A #CyberKitDOMDOMSelection
 *
 * Returns: A #gulong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED gulong
webkit_dom_dom_selection_get_range_count(CyberKitDOMDOMSelection* self);

/**
 * webkit_dom_dom_selection_get_selection_type:
 * @self: A #CyberKitDOMDOMSelection
 *
 * Returns: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED gchar*
webkit_dom_dom_selection_get_selection_type(CyberKitDOMDOMSelection* self);

/**
 * webkit_dom_dom_selection_get_base_node:
 * @self: A #CyberKitDOMDOMSelection
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_dom_selection_get_base_node(CyberKitDOMDOMSelection* self);

/**
 * webkit_dom_dom_selection_get_base_offset:
 * @self: A #CyberKitDOMDOMSelection
 *
 * Returns: A #gulong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED gulong
webkit_dom_dom_selection_get_base_offset(CyberKitDOMDOMSelection* self);

/**
 * webkit_dom_dom_selection_get_extent_node:
 * @self: A #CyberKitDOMDOMSelection
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_dom_selection_get_extent_node(CyberKitDOMDOMSelection* self);

/**
 * webkit_dom_dom_selection_get_extent_offset:
 * @self: A #CyberKitDOMDOMSelection
 *
 * Returns: A #gulong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED gulong
webkit_dom_dom_selection_get_extent_offset(CyberKitDOMDOMSelection* self);

G_END_DECLS

#endif /* CyberKitDOMDOMSelection_h */
