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

#ifndef CyberKitDOMRange_h
#define CyberKitDOMRange_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMObject.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_RANGE            (webkit_dom_range_get_type())
#define WEBKIT_DOM_RANGE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_RANGE, CyberKitDOMRange))
#define WEBKIT_DOM_RANGE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_RANGE, CyberKitDOMRangeClass)
#define WEBKIT_DOM_IS_RANGE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_RANGE))
#define WEBKIT_DOM_IS_RANGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_RANGE))
#define WEBKIT_DOM_RANGE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_RANGE, CyberKitDOMRangeClass))

#ifndef WEBKIT_DISABLE_DEPRECATED

/**
 * WEBKIT_DOM_RANGE_START_TO_START:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_RANGE_START_TO_START 0

/**
 * WEBKIT_DOM_RANGE_START_TO_END:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_RANGE_START_TO_END 1

/**
 * WEBKIT_DOM_RANGE_END_TO_END:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_RANGE_END_TO_END 2

/**
 * WEBKIT_DOM_RANGE_END_TO_START:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_RANGE_END_TO_START 3

/**
 * WEBKIT_DOM_RANGE_NODE_BEFORE:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_RANGE_NODE_BEFORE 0

/**
 * WEBKIT_DOM_RANGE_NODE_AFTER:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_RANGE_NODE_AFTER 1

/**
 * WEBKIT_DOM_RANGE_NODE_BEFORE_AND_AFTER:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_RANGE_NODE_BEFORE_AND_AFTER 2

/**
 * WEBKIT_DOM_RANGE_NODE_INSIDE:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_RANGE_NODE_INSIDE 3

#endif /* WEBKIT_DISABLE_DEPRECATED */

struct _CyberKitDOMRange {
    CyberKitDOMObject parent_instance;
};

struct _CyberKitDOMRangeClass {
    CyberKitDOMObjectClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_range_get_type(void);

/**
 * webkit_dom_range_set_start:
 * @self: A #CyberKitDOMRange
 * @refNode: A #CyberKitDOMNode
 * @offset: A #glong
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_range_set_start(CyberKitDOMRange* self, CyberKitDOMNode* refNode, glong offset, GError** error);

/**
 * webkit_dom_range_set_end:
 * @self: A #CyberKitDOMRange
 * @refNode: A #CyberKitDOMNode
 * @offset: A #glong
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_range_set_end(CyberKitDOMRange* self, CyberKitDOMNode* refNode, glong offset, GError** error);

/**
 * webkit_dom_range_set_start_before:
 * @self: A #CyberKitDOMRange
 * @refNode: A #CyberKitDOMNode
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_range_set_start_before(CyberKitDOMRange* self, CyberKitDOMNode* refNode, GError** error);

/**
 * webkit_dom_range_set_start_after:
 * @self: A #CyberKitDOMRange
 * @refNode: A #CyberKitDOMNode
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_range_set_start_after(CyberKitDOMRange* self, CyberKitDOMNode* refNode, GError** error);

/**
 * webkit_dom_range_set_end_before:
 * @self: A #CyberKitDOMRange
 * @refNode: A #CyberKitDOMNode
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_range_set_end_before(CyberKitDOMRange* self, CyberKitDOMNode* refNode, GError** error);

/**
 * webkit_dom_range_set_end_after:
 * @self: A #CyberKitDOMRange
 * @refNode: A #CyberKitDOMNode
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_range_set_end_after(CyberKitDOMRange* self, CyberKitDOMNode* refNode, GError** error);

/**
 * webkit_dom_range_collapse:
 * @self: A #CyberKitDOMRange
 * @toStart: A #gboolean
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_range_collapse(CyberKitDOMRange* self, gboolean toStart, GError** error);

/**
 * webkit_dom_range_select_node:
 * @self: A #CyberKitDOMRange
 * @refNode: A #CyberKitDOMNode
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_range_select_node(CyberKitDOMRange* self, CyberKitDOMNode* refNode, GError** error);

/**
 * webkit_dom_range_select_node_contents:
 * @self: A #CyberKitDOMRange
 * @refNode: A #CyberKitDOMNode
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_range_select_node_contents(CyberKitDOMRange* self, CyberKitDOMNode* refNode, GError** error);

/**
 * webkit_dom_range_compare_boundary_points:
 * @self: A #CyberKitDOMRange
 * @how: A #gushort
 * @sourceRange: A #CyberKitDOMRange
 * @error: #GError
 *
 * Returns: A #gshort
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gshort
webkit_dom_range_compare_boundary_points(CyberKitDOMRange* self, gushort how, CyberKitDOMRange* sourceRange, GError** error);

/**
 * webkit_dom_range_delete_contents:
 * @self: A #CyberKitDOMRange
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_range_delete_contents(CyberKitDOMRange* self, GError** error);

/**
 * webkit_dom_range_extract_contents:
 * @self: A #CyberKitDOMRange
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMDocumentFragment
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMDocumentFragment*
webkit_dom_range_extract_contents(CyberKitDOMRange* self, GError** error);

/**
 * webkit_dom_range_clone_contents:
 * @self: A #CyberKitDOMRange
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMDocumentFragment
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMDocumentFragment*
webkit_dom_range_clone_contents(CyberKitDOMRange* self, GError** error);

/**
 * webkit_dom_range_insert_node:
 * @self: A #CyberKitDOMRange
 * @newNode: A #CyberKitDOMNode
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_range_insert_node(CyberKitDOMRange* self, CyberKitDOMNode* newNode, GError** error);

/**
 * webkit_dom_range_surround_contents:
 * @self: A #CyberKitDOMRange
 * @newParent: A #CyberKitDOMNode
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_range_surround_contents(CyberKitDOMRange* self, CyberKitDOMNode* newParent, GError** error);

/**
 * webkit_dom_range_clone_range:
 * @self: A #CyberKitDOMRange
 * @error: #GError
 *
 * Returns: (transfer full): A #CyberKitDOMRange
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMRange*
webkit_dom_range_clone_range(CyberKitDOMRange* self, GError** error);

/**
 * webkit_dom_range_to_string:
 * @self: A #CyberKitDOMRange
 * @error: #GError
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_range_to_string(CyberKitDOMRange* self, GError** error);

/**
 * webkit_dom_range_detach:
 * @self: A #CyberKitDOMRange
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_range_detach(CyberKitDOMRange* self, GError** error);

/**
 * webkit_dom_range_create_contextual_fragment:
 * @self: A #CyberKitDOMRange
 * @html: A #gchar
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMDocumentFragment
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMDocumentFragment*
webkit_dom_range_create_contextual_fragment(CyberKitDOMRange* self, const gchar* html, GError** error);

/**
 * webkit_dom_range_compare_node:
 * @self: A #CyberKitDOMRange
 * @refNode: A #CyberKitDOMNode
 * @error: #GError
 *
 * Returns: A #gshort
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gshort
webkit_dom_range_compare_node(CyberKitDOMRange* self, CyberKitDOMNode* refNode, GError** error);

/**
 * webkit_dom_range_intersects_node:
 * @self: A #CyberKitDOMRange
 * @refNode: A #CyberKitDOMNode
 * @error: #GError
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_range_intersects_node(CyberKitDOMRange* self, CyberKitDOMNode* refNode, GError** error);

/**
 * webkit_dom_range_compare_point:
 * @self: A #CyberKitDOMRange
 * @refNode: A #CyberKitDOMNode
 * @offset: A #glong
 * @error: #GError
 *
 * Returns: A #gshort
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gshort
webkit_dom_range_compare_point(CyberKitDOMRange* self, CyberKitDOMNode* refNode, glong offset, GError** error);

/**
 * webkit_dom_range_is_point_in_range:
 * @self: A #CyberKitDOMRange
 * @refNode: A #CyberKitDOMNode
 * @offset: A #glong
 * @error: #GError
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_range_is_point_in_range(CyberKitDOMRange* self, CyberKitDOMNode* refNode, glong offset, GError** error);

/**
 * webkit_dom_range_get_start_container:
 * @self: A #CyberKitDOMRange
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_range_get_start_container(CyberKitDOMRange* self, GError** error);

/**
 * webkit_dom_range_get_start_offset:
 * @self: A #CyberKitDOMRange
 * @error: #GError
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_range_get_start_offset(CyberKitDOMRange* self, GError** error);

/**
 * webkit_dom_range_get_end_container:
 * @self: A #CyberKitDOMRange
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_range_get_end_container(CyberKitDOMRange* self, GError** error);

/**
 * webkit_dom_range_get_end_offset:
 * @self: A #CyberKitDOMRange
 * @error: #GError
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_range_get_end_offset(CyberKitDOMRange* self, GError** error);

/**
 * webkit_dom_range_get_collapsed:
 * @self: A #CyberKitDOMRange
 * @error: #GError
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_range_get_collapsed(CyberKitDOMRange* self, GError** error);

/**
 * webkit_dom_range_get_common_ancestor_container:
 * @self: A #CyberKitDOMRange
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_range_get_common_ancestor_container(CyberKitDOMRange* self, GError** error);

/**
 * webkit_dom_range_get_text:
 * @self: A #CyberKitDOMRange
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_range_get_text(CyberKitDOMRange* self);

/**
 * webkit_dom_range_expand:
 * @self: A #CyberKitDOMRange
 * @unit: A #gchar
 * @error: #GError
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_range_expand(CyberKitDOMRange* self, const gchar* unit, GError** error);

G_END_DECLS

#endif /* CyberKitDOMRange_h */
