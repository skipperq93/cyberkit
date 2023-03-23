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

#ifndef CyberKitDOMNode_h
#define CyberKitDOMNode_h

#include <glib-object.h>
#include <jsc/jsc.h>
#include <webkitdom/CyberKitDOMObject.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_NODE            (webkit_dom_node_get_type())
#define WEBKIT_DOM_NODE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_NODE, CyberKitDOMNode))
#define WEBKIT_DOM_NODE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_NODE, CyberKitDOMNodeClass)
#define WEBKIT_DOM_IS_NODE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_NODE))
#define WEBKIT_DOM_IS_NODE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_NODE))
#define WEBKIT_DOM_NODE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_NODE, CyberKitDOMNodeClass))

struct _CyberKitDOMNode {
    CyberKitDOMObject parent_instance;
};

struct _CyberKitDOMNodeClass {
    CyberKitDOMObjectClass parent_class;
};

#ifndef WEBKIT_DISABLE_DEPRECATED

/**
 * WEBKIT_DOM_NODE_ELEMENT_NODE:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_ELEMENT_NODE 1

/**
 * WEBKIT_DOM_NODE_ATTRIBUTE_NODE:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_ATTRIBUTE_NODE 2

/**
 * WEBKIT_DOM_NODE_TEXT_NODE:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_TEXT_NODE 3

/**
 * WEBKIT_DOM_NODE_CDATA_SECTION_NODE:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_CDATA_SECTION_NODE 4

/**
 * WEBKIT_DOM_NODE_ENTITY_REFERENCE_NODE:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_ENTITY_REFERENCE_NODE 5

/**
 * WEBKIT_DOM_NODE_ENTITY_NODE:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_ENTITY_NODE 6

/**
 * WEBKIT_DOM_NODE_PROCESSING_INSTRUCTION_NODE:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_PROCESSING_INSTRUCTION_NODE 7

/**
 * WEBKIT_DOM_NODE_COMMENT_NODE:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_COMMENT_NODE 8

/**
 * WEBKIT_DOM_NODE_DOCUMENT_NODE:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_DOCUMENT_NODE 9

/**
 * WEBKIT_DOM_NODE_DOCUMENT_TYPE_NODE:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_DOCUMENT_TYPE_NODE 10

/**
 * WEBKIT_DOM_NODE_DOCUMENT_FRAGMENT_NODE:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_DOCUMENT_FRAGMENT_NODE 11

/**
 * WEBKIT_DOM_NODE_DOCUMENT_POSITION_DISCONNECTED:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_DOCUMENT_POSITION_DISCONNECTED 0x01

/**
 * WEBKIT_DOM_NODE_DOCUMENT_POSITION_PRECEDING:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_DOCUMENT_POSITION_PRECEDING 0x02

/**
 * WEBKIT_DOM_NODE_DOCUMENT_POSITION_FOLLOWING:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_DOCUMENT_POSITION_FOLLOWING 0x04

/**
 * WEBKIT_DOM_NODE_DOCUMENT_POSITION_CONTAINS:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_DOCUMENT_POSITION_CONTAINS 0x08

/**
 * WEBKIT_DOM_NODE_DOCUMENT_POSITION_CONTAINED_BY:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_DOCUMENT_POSITION_CONTAINED_BY 0x10

/**
 * WEBKIT_DOM_NODE_DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_NODE_DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC 0x20

#endif /* WEBKIT_DISABLE_DEPRECATED */

WEBKIT_DEPRECATED GType
webkit_dom_node_get_type(void);

/**
 * webkit_dom_node_insert_before:
 * @self: A #CyberKitDOMNode
 * @newChild: A #CyberKitDOMNode
 * @refChild: (allow-none): A #CyberKitDOMNode
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_node_insert_before(CyberKitDOMNode* self, CyberKitDOMNode* newChild, CyberKitDOMNode* refChild, GError** error);

/**
 * webkit_dom_node_replace_child:
 * @self: A #CyberKitDOMNode
 * @newChild: A #CyberKitDOMNode
 * @oldChild: A #CyberKitDOMNode
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_node_replace_child(CyberKitDOMNode* self, CyberKitDOMNode* newChild, CyberKitDOMNode* oldChild, GError** error);

/**
 * webkit_dom_node_remove_child:
 * @self: A #CyberKitDOMNode
 * @oldChild: A #CyberKitDOMNode
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_node_remove_child(CyberKitDOMNode* self, CyberKitDOMNode* oldChild, GError** error);

/**
 * webkit_dom_node_append_child:
 * @self: A #CyberKitDOMNode
 * @newChild: A #CyberKitDOMNode
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_node_append_child(CyberKitDOMNode* self, CyberKitDOMNode* newChild, GError** error);

/**
 * webkit_dom_node_has_child_nodes:
 * @self: A #CyberKitDOMNode
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_node_has_child_nodes(CyberKitDOMNode* self);

/**
 * webkit_dom_node_clone_node_with_error:
 * @self: A #CyberKitDOMNode
 * @deep: A #gboolean
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Since: 2.14
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_node_clone_node_with_error(CyberKitDOMNode* self, gboolean deep, GError** error);

/**
 * webkit_dom_node_normalize:
 * @self: A #CyberKitDOMNode
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_node_normalize(CyberKitDOMNode* self);

/**
 * webkit_dom_node_is_supported:
 * @self: A #CyberKitDOMNode
 * @feature: A #gchar
 * @version: A #gchar
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_node_is_supported(CyberKitDOMNode* self, const gchar* feature, const gchar* version);

/**
 * webkit_dom_node_is_same_node:
 * @self: A #CyberKitDOMNode
 * @other: A #CyberKitDOMNode
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_node_is_same_node(CyberKitDOMNode* self, CyberKitDOMNode* other);

/**
 * webkit_dom_node_is_equal_node:
 * @self: A #CyberKitDOMNode
 * @other: A #CyberKitDOMNode
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_node_is_equal_node(CyberKitDOMNode* self, CyberKitDOMNode* other);

/**
 * webkit_dom_node_lookup_prefix:
 * @self: A #CyberKitDOMNode
 * @namespaceURI: A #gchar
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_node_lookup_prefix(CyberKitDOMNode* self, const gchar* namespaceURI);

/**
 * webkit_dom_node_lookup_namespace_uri:
 * @self: A #CyberKitDOMNode
 * @prefix: A #gchar
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_node_lookup_namespace_uri(CyberKitDOMNode* self, const gchar* prefix);

/**
 * webkit_dom_node_is_default_namespace:
 * @self: A #CyberKitDOMNode
 * @namespaceURI: A #gchar
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_node_is_default_namespace(CyberKitDOMNode* self, const gchar* namespaceURI);

/**
 * webkit_dom_node_compare_document_position:
 * @self: A #CyberKitDOMNode
 * @other: A #CyberKitDOMNode
 *
 * Returns: A #gushort
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gushort
webkit_dom_node_compare_document_position(CyberKitDOMNode* self, CyberKitDOMNode* other);

/**
 * webkit_dom_node_contains:
 * @self: A #CyberKitDOMNode
 * @other: A #CyberKitDOMNode
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_node_contains(CyberKitDOMNode* self, CyberKitDOMNode* other);

/**
 * webkit_dom_node_get_node_name:
 * @self: A #CyberKitDOMNode
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_node_get_node_name(CyberKitDOMNode* self);

/**
 * webkit_dom_node_get_node_value:
 * @self: A #CyberKitDOMNode
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_node_get_node_value(CyberKitDOMNode* self);

/**
 * webkit_dom_node_set_node_value:
 * @self: A #CyberKitDOMNode
 * @value: A #gchar
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_node_set_node_value(CyberKitDOMNode* self, const gchar* value, GError** error);

/**
 * webkit_dom_node_get_node_type:
 * @self: A #CyberKitDOMNode
 *
 * Returns: A #gushort
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gushort
webkit_dom_node_get_node_type(CyberKitDOMNode* self);

/**
 * webkit_dom_node_get_parent_node:
 * @self: A #CyberKitDOMNode
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_node_get_parent_node(CyberKitDOMNode* self);

/**
 * webkit_dom_node_get_child_nodes:
 * @self: A #CyberKitDOMNode
 *
 * Returns: (transfer full): A #CyberKitDOMNodeList
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNodeList*
webkit_dom_node_get_child_nodes(CyberKitDOMNode* self);

/**
 * webkit_dom_node_get_first_child:
 * @self: A #CyberKitDOMNode
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_node_get_first_child(CyberKitDOMNode* self);

/**
 * webkit_dom_node_get_last_child:
 * @self: A #CyberKitDOMNode
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_node_get_last_child(CyberKitDOMNode* self);

/**
 * webkit_dom_node_get_previous_sibling:
 * @self: A #CyberKitDOMNode
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_node_get_previous_sibling(CyberKitDOMNode* self);

/**
 * webkit_dom_node_get_next_sibling:
 * @self: A #CyberKitDOMNode
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_node_get_next_sibling(CyberKitDOMNode* self);

/**
 * webkit_dom_node_get_owner_document:
 * @self: A #CyberKitDOMNode
 *
 * Returns: (transfer none): A #CyberKitDOMDocument
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMDocument*
webkit_dom_node_get_owner_document(CyberKitDOMNode* self);

/**
 * webkit_dom_node_get_base_uri:
 * @self: A #CyberKitDOMNode
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_node_get_base_uri(CyberKitDOMNode* self);

/**
 * webkit_dom_node_get_text_content:
 * @self: A #CyberKitDOMNode
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_node_get_text_content(CyberKitDOMNode* self);

/**
 * webkit_dom_node_set_text_content:
 * @self: A #CyberKitDOMNode
 * @value: A #gchar
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_node_set_text_content(CyberKitDOMNode* self, const gchar* value, GError** error);

/**
 * webkit_dom_node_get_parent_element:
 * @self: A #CyberKitDOMNode
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_node_get_parent_element(CyberKitDOMNode* self);

WEBKIT_DEPRECATED CyberKitDOMNode *
webkit_dom_node_for_js_value(JSCValue* value);

G_END_DECLS

#endif /* CyberKitDOMNode_h */
