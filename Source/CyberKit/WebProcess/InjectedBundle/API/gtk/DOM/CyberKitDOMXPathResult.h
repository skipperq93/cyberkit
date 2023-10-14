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

#ifndef CyberKitDOMXPathResult_h
#define CyberKitDOMXPathResult_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMObject.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_XPATH_RESULT            (webkit_dom_xpath_result_get_type())
#define WEBKIT_DOM_XPATH_RESULT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_XPATH_RESULT, CyberKitDOMXPathResult))
#define WEBKIT_DOM_XPATH_RESULT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_XPATH_RESULT, CyberKitDOMXPathResultClass)
#define WEBKIT_DOM_IS_XPATH_RESULT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_XPATH_RESULT))
#define WEBKIT_DOM_IS_XPATH_RESULT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_XPATH_RESULT))
#define WEBKIT_DOM_XPATH_RESULT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_XPATH_RESULT, CyberKitDOMXPathResultClass))

#ifndef WEBKIT_DISABLE_DEPRECATED

/**
 * WEBKIT_DOM_XPATH_RESULT_ANY_TYPE:
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_XPATH_RESULT_ANY_TYPE 0

/**
 * WEBKIT_DOM_XPATH_RESULT_NUMBER_TYPE:
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_XPATH_RESULT_NUMBER_TYPE 1

/**
 * WEBKIT_DOM_XPATH_RESULT_STRING_TYPE:
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_XPATH_RESULT_STRING_TYPE 2

/**
 * WEBKIT_DOM_XPATH_RESULT_BOOLEAN_TYPE:
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_XPATH_RESULT_BOOLEAN_TYPE 3

/**
 * WEBKIT_DOM_XPATH_RESULT_UNORDERED_NODE_ITERATOR_TYPE:
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_XPATH_RESULT_UNORDERED_NODE_ITERATOR_TYPE 4

/**
 * WEBKIT_DOM_XPATH_RESULT_ORDERED_NODE_ITERATOR_TYPE:
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_XPATH_RESULT_ORDERED_NODE_ITERATOR_TYPE 5

/**
 * WEBKIT_DOM_XPATH_RESULT_UNORDERED_NODE_SNAPSHOT_TYPE:
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_XPATH_RESULT_UNORDERED_NODE_SNAPSHOT_TYPE 6

/**
 * WEBKIT_DOM_XPATH_RESULT_ORDERED_NODE_SNAPSHOT_TYPE:
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_XPATH_RESULT_ORDERED_NODE_SNAPSHOT_TYPE 7

/**
 * WEBKIT_DOM_XPATH_RESULT_ANY_UNORDERED_NODE_TYPE:
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_XPATH_RESULT_ANY_UNORDERED_NODE_TYPE 8

/**
 * WEBKIT_DOM_XPATH_RESULT_FIRST_ORDERED_NODE_TYPE:
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
#define WEBKIT_DOM_XPATH_RESULT_FIRST_ORDERED_NODE_TYPE 9

#endif /* WEBKIT_DISABLE_DEPRECATED */

struct _CyberKitDOMXPathResult {
    CyberKitDOMObject parent_instance;
};

struct _CyberKitDOMXPathResultClass {
    CyberKitDOMObjectClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_xpath_result_get_type(void);

/**
 * webkit_dom_xpath_result_iterate_next:
 * @self: A #CyberKitDOMXPathResult
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_xpath_result_iterate_next(CyberKitDOMXPathResult* self, GError** error);

/**
 * webkit_dom_xpath_result_snapshot_item:
 * @self: A #CyberKitDOMXPathResult
 * @index: A #gulong
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_xpath_result_snapshot_item(CyberKitDOMXPathResult* self, gulong index, GError** error);

/**
 * webkit_dom_xpath_result_get_result_type:
 * @self: A #CyberKitDOMXPathResult
 *
 * Returns: A #gushort
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gushort
webkit_dom_xpath_result_get_result_type(CyberKitDOMXPathResult* self);

/**
 * webkit_dom_xpath_result_get_number_value:
 * @self: A #CyberKitDOMXPathResult
 * @error: #GError
 *
 * Returns: A #gdouble
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gdouble
webkit_dom_xpath_result_get_number_value(CyberKitDOMXPathResult* self, GError** error);

/**
 * webkit_dom_xpath_result_get_string_value:
 * @self: A #CyberKitDOMXPathResult
 * @error: #GError
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_xpath_result_get_string_value(CyberKitDOMXPathResult* self, GError** error);

/**
 * webkit_dom_xpath_result_get_boolean_value:
 * @self: A #CyberKitDOMXPathResult
 * @error: #GError
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_xpath_result_get_boolean_value(CyberKitDOMXPathResult* self, GError** error);

/**
 * webkit_dom_xpath_result_get_single_node_value:
 * @self: A #CyberKitDOMXPathResult
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_xpath_result_get_single_node_value(CyberKitDOMXPathResult* self, GError** error);

/**
 * webkit_dom_xpath_result_get_invalid_iterator_state:
 * @self: A #CyberKitDOMXPathResult
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_xpath_result_get_invalid_iterator_state(CyberKitDOMXPathResult* self);

/**
 * webkit_dom_xpath_result_get_snapshot_length:
 * @self: A #CyberKitDOMXPathResult
 * @error: #GError
 *
 * Returns: A #gulong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gulong
webkit_dom_xpath_result_get_snapshot_length(CyberKitDOMXPathResult* self, GError** error);

G_END_DECLS

#endif /* CyberKitDOMXPathResult_h */
