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

#ifndef CyberKitDOMElement_h
#define CyberKitDOMElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMNode.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_ELEMENT            (webkit_dom_element_get_type())
#define WEBKIT_DOM_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_ELEMENT, CyberKitDOMElement))
#define WEBKIT_DOM_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_ELEMENT, CyberKitDOMElementClass)
#define WEBKIT_DOM_IS_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_ELEMENT))
#define WEBKIT_DOM_IS_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_ELEMENT))
#define WEBKIT_DOM_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_ELEMENT, CyberKitDOMElementClass))

struct _CyberKitDOMElement {
    CyberKitDOMNode parent_instance;
};

struct _CyberKitDOMElementClass {
    CyberKitDOMNodeClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_element_get_type                             (void);

WEBKIT_DEPRECATED_FOR(webkit_web_form_manager_input_element_is_user_edited) gboolean
webkit_dom_element_html_input_element_is_user_edited    (CyberKitDOMElement *element);

WEBKIT_DEPRECATED_FOR(webkit_web_form_manager_input_element_is_auto_filled) gboolean
webkit_dom_element_html_input_element_get_auto_filled   (CyberKitDOMElement *element);

WEBKIT_DEPRECATED_FOR(webkit_web_form_manager_input_element_auto_fill) void
webkit_dom_element_html_input_element_set_auto_filled   (CyberKitDOMElement *element,
                                                         gboolean          auto_filled);
WEBKIT_DEPRECATED_FOR(webkit_web_form_manager_input_element_auto_fill) void
webkit_dom_element_html_input_element_set_editing_value (CyberKitDOMElement *element,
                                                         const char       *value);

#ifndef WEBKIT_DISABLE_DEPRECATED

/**
 * WEBKIT_DOM_ELEMENT_ALLOW_KEYBOARD_INPUT:
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
#define WEBKIT_DOM_ELEMENT_ALLOW_KEYBOARD_INPUT 1

#endif

/**
 * webkit_dom_element_get_attribute:
 * @self: A #CyberKitDOMElement
 * @name: A #gchar
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_element_get_attribute(CyberKitDOMElement* self, const gchar* name);

/**
 * webkit_dom_element_set_attribute:
 * @self: A #CyberKitDOMElement
 * @name: A #gchar
 * @value: A #gchar
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_set_attribute(CyberKitDOMElement* self, const gchar* name, const gchar* value, GError** error);

/**
 * webkit_dom_element_remove_attribute:
 * @self: A #CyberKitDOMElement
 * @name: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_remove_attribute(CyberKitDOMElement* self, const gchar* name);

/**
 * webkit_dom_element_get_attribute_node:
 * @self: A #CyberKitDOMElement
 * @name: A #gchar
 *
 * Returns: (transfer none): A #CyberKitDOMAttr
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMAttr*
webkit_dom_element_get_attribute_node(CyberKitDOMElement* self, const gchar* name);

/**
 * webkit_dom_element_set_attribute_node:
 * @self: A #CyberKitDOMElement
 * @newAttr: A #CyberKitDOMAttr
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMAttr
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMAttr*
webkit_dom_element_set_attribute_node(CyberKitDOMElement* self, CyberKitDOMAttr* newAttr, GError** error);

/**
 * webkit_dom_element_remove_attribute_node:
 * @self: A #CyberKitDOMElement
 * @oldAttr: A #CyberKitDOMAttr
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMAttr
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMAttr*
webkit_dom_element_remove_attribute_node(CyberKitDOMElement* self, CyberKitDOMAttr* oldAttr, GError** error);

/**
 * webkit_dom_element_get_elements_by_tag_name_as_html_collection:
 * @self: A #CyberKitDOMElement
 * @name: A #gchar
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Since: 2.12
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_element_get_elements_by_tag_name_as_html_collection(CyberKitDOMElement* self, const gchar* name);

/**
 * webkit_dom_element_has_attributes:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_element_has_attributes(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_attribute_ns:
 * @self: A #CyberKitDOMElement
 * @namespaceURI: A #gchar
 * @localName: A #gchar
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_element_get_attribute_ns(CyberKitDOMElement* self, const gchar* namespaceURI, const gchar* localName);

/**
 * webkit_dom_element_set_attribute_ns:
 * @self: A #CyberKitDOMElement
 * @namespaceURI: (allow-none): A #gchar
 * @qualifiedName: A #gchar
 * @value: A #gchar
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_set_attribute_ns(CyberKitDOMElement* self, const gchar* namespaceURI, const gchar* qualifiedName, const gchar* value, GError** error);

/**
 * webkit_dom_element_remove_attribute_ns:
 * @self: A #CyberKitDOMElement
 * @namespaceURI: A #gchar
 * @localName: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_remove_attribute_ns(CyberKitDOMElement* self, const gchar* namespaceURI, const gchar* localName);

/**
 * webkit_dom_element_get_elements_by_tag_name_ns_as_html_collection:
 * @self: A #CyberKitDOMElement
 * @namespaceURI: A #gchar
 * @localName: A #gchar
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Since: 2.12
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_element_get_elements_by_tag_name_ns_as_html_collection(CyberKitDOMElement* self, const gchar* namespaceURI, const gchar* localName);

/**
 * webkit_dom_element_get_attribute_node_ns:
 * @self: A #CyberKitDOMElement
 * @namespaceURI: A #gchar
 * @localName: A #gchar
 *
 * Returns: (transfer none): A #CyberKitDOMAttr
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMAttr*
webkit_dom_element_get_attribute_node_ns(CyberKitDOMElement* self, const gchar* namespaceURI, const gchar* localName);

/**
 * webkit_dom_element_set_attribute_node_ns:
 * @self: A #CyberKitDOMElement
 * @newAttr: A #CyberKitDOMAttr
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMAttr
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMAttr*
webkit_dom_element_set_attribute_node_ns(CyberKitDOMElement* self, CyberKitDOMAttr* newAttr, GError** error);

/**
 * webkit_dom_element_has_attribute:
 * @self: A #CyberKitDOMElement
 * @name: A #gchar
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_element_has_attribute(CyberKitDOMElement* self, const gchar* name);

/**
 * webkit_dom_element_has_attribute_ns:
 * @self: A #CyberKitDOMElement
 * @namespaceURI: A #gchar
 * @localName: A #gchar
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_element_has_attribute_ns(CyberKitDOMElement* self, const gchar* namespaceURI, const gchar* localName);

/**
 * webkit_dom_element_focus:
 * @self: A #CyberKitDOMElement
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_focus(CyberKitDOMElement* self);

/**
 * webkit_dom_element_blur:
 * @self: A #CyberKitDOMElement
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_blur(CyberKitDOMElement* self);

/**
 * webkit_dom_element_scroll_into_view:
 * @self: A #CyberKitDOMElement
 * @alignWithTop: A #gboolean
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_scroll_into_view(CyberKitDOMElement* self, gboolean alignWithTop);

/**
 * webkit_dom_element_scroll_into_view_if_needed:
 * @self: A #CyberKitDOMElement
 * @centerIfNeeded: A #gboolean
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_scroll_into_view_if_needed(CyberKitDOMElement* self, gboolean centerIfNeeded);

/**
 * webkit_dom_element_scroll_by_lines:
 * @self: A #CyberKitDOMElement
 * @lines: A #glong
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_scroll_by_lines(CyberKitDOMElement* self, glong lines);

/**
 * webkit_dom_element_scroll_by_pages:
 * @self: A #CyberKitDOMElement
 * @pages: A #glong
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_scroll_by_pages(CyberKitDOMElement* self, glong pages);

/**
 * webkit_dom_element_get_elements_by_class_name_as_html_collection:
 * @self: A #CyberKitDOMElement
 * @name: A #gchar
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Since: 2.12
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_element_get_elements_by_class_name_as_html_collection(CyberKitDOMElement* self, const gchar* name);

/**
 * webkit_dom_element_query_selector:
 * @self: A #CyberKitDOMElement
 * @selectors: A #gchar
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_element_query_selector(CyberKitDOMElement* self, const gchar* selectors, GError** error);

/**
 * webkit_dom_element_query_selector_all:
 * @self: A #CyberKitDOMElement
 * @selectors: A #gchar
 * @error: #GError
 *
 * Returns: (transfer full): A #CyberKitDOMNodeList
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNodeList*
webkit_dom_element_query_selector_all(CyberKitDOMElement* self, const gchar* selectors, GError** error);

/**
 * webkit_dom_element_get_tag_name:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_element_get_tag_name(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_attributes:
 * @self: A #CyberKitDOMElement
 *
 * Returns: (transfer full): A #CyberKitDOMNamedNodeMap
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNamedNodeMap*
webkit_dom_element_get_attributes(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_style:
 * @self: A #CyberKitDOMElement
 *
 * Returns: (transfer full): A #CyberKitDOMCSSStyleDeclaration
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMCSSStyleDeclaration*
webkit_dom_element_get_style(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_id:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_element_get_id(CyberKitDOMElement* self);

/**
 * webkit_dom_element_set_id:
 * @self: A #CyberKitDOMElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_set_id(CyberKitDOMElement* self, const gchar* value);

/**
 * webkit_dom_element_get_namespace_uri:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gchar
 *
 * Since: 2.14
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_element_get_namespace_uri(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_prefix:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gchar
 *
 * Since: 2.14
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_element_get_prefix(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_local_name:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gchar
 *
 * Since: 2.14
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_element_get_local_name(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_offset_left:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gdouble
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gdouble
webkit_dom_element_get_offset_left(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_offset_top:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gdouble
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gdouble
webkit_dom_element_get_offset_top(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_offset_width:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gdouble
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gdouble
webkit_dom_element_get_offset_width(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_offset_height:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gdouble
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gdouble
webkit_dom_element_get_offset_height(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_client_left:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gdouble
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gdouble
webkit_dom_element_get_client_left(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_client_top:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gdouble
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gdouble
webkit_dom_element_get_client_top(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_client_width:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gdouble
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gdouble
webkit_dom_element_get_client_width(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_client_height:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gdouble
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gdouble
webkit_dom_element_get_client_height(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_scroll_left:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_element_get_scroll_left(CyberKitDOMElement* self);

/**
 * webkit_dom_element_set_scroll_left:
 * @self: A #CyberKitDOMElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_set_scroll_left(CyberKitDOMElement* self, glong value);

/**
 * webkit_dom_element_get_scroll_top:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_element_get_scroll_top(CyberKitDOMElement* self);

/**
 * webkit_dom_element_set_scroll_top:
 * @self: A #CyberKitDOMElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_set_scroll_top(CyberKitDOMElement* self, glong value);

/**
 * webkit_dom_element_get_scroll_width:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_element_get_scroll_width(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_scroll_height:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_element_get_scroll_height(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_bounding_client_rect:
 * @self: A #CyberKitDOMElement
 *
 * Returns a #CyberKitDOMClientRect representing the size and position of @self
 * relative to the viewport.
 *
 * Returns: (transfer full): A #CyberKitDOMClientRect
 *
 * Since: 2.18
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMClientRect*
webkit_dom_element_get_bounding_client_rect(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_client_rects:
 * @self: A #CyberKitDOMElement
 *
 * Returns a collection of #CyberKitDOMClientRect objects, each of which describe
 * the size and position of a CSS border box relative to the viewport.
 *
 * Returns: (transfer full): A #CyberKitDOMClientRectList
 *
 * Since: 2.18
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMClientRectList*
webkit_dom_element_get_client_rects(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_offset_parent:
 * @self: A #CyberKitDOMElement
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_element_get_offset_parent(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_inner_html:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gchar
 *
 * Since: 2.8
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_element_get_inner_html(CyberKitDOMElement* self);

/**
 * webkit_dom_element_set_inner_html:
 * @self: A #CyberKitDOMElement
 * @value: A #gchar
 * @error: #GError
 *
 * Since: 2.8
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_set_inner_html(CyberKitDOMElement* self, const gchar* value, GError** error);

/**
 * webkit_dom_element_get_outer_html:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gchar
 *
 * Since: 2.8
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_element_get_outer_html(CyberKitDOMElement* self);

/**
 * webkit_dom_element_set_outer_html:
 * @self: A #CyberKitDOMElement
 * @value: A #gchar
 * @error: #GError
 *
 * Since: 2.8
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_set_outer_html(CyberKitDOMElement* self, const gchar* value, GError** error);

/**
 * webkit_dom_element_get_class_name:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_element_get_class_name(CyberKitDOMElement* self);

/**
 * webkit_dom_element_set_class_name:
 * @self: A #CyberKitDOMElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_set_class_name(CyberKitDOMElement* self, const gchar* value);

/**
 * webkit_dom_element_get_previous_element_sibling:
 * @self: A #CyberKitDOMElement
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_element_get_previous_element_sibling(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_next_element_sibling:
 * @self: A #CyberKitDOMElement
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_element_get_next_element_sibling(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_children:
 * @self: A #CyberKitDOMElement
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Since: 2.10
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_element_get_children(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_first_element_child:
 * @self: A #CyberKitDOMElement
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_element_get_first_element_child(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_last_element_child:
 * @self: A #CyberKitDOMElement
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_element_get_last_element_child(CyberKitDOMElement* self);

/**
 * webkit_dom_element_get_child_element_count:
 * @self: A #CyberKitDOMElement
 *
 * Returns: A #gulong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gulong
webkit_dom_element_get_child_element_count(CyberKitDOMElement* self);

/**
 * webkit_dom_element_matches:
 * @self: A #CyberKitDOMElement
 * @selectors: A #gchar
 * @error: #GError
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_element_matches(CyberKitDOMElement* self, const gchar* selectors, GError** error);

/**
 * webkit_dom_element_closest:
 * @self: A #CyberKitDOMElement
 * @selectors: A #gchar
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_element_closest(CyberKitDOMElement* self, const gchar* selectors, GError** error);

/**
 * webkit_dom_element_webkit_matches_selector:
 * @self: A #CyberKitDOMElement
 * @selectors: A #gchar
 * @error: #GError
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_element_webkit_matches_selector(CyberKitDOMElement* self, const gchar* selectors, GError** error);

/**
 * webkit_dom_element_webkit_request_fullscreen:
 * @self: A #CyberKitDOMElement
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_webkit_request_fullscreen(CyberKitDOMElement* self);

/**
 * webkit_dom_element_insert_adjacent_element:
 * @self: A #CyberKitDOMElement
 * @where: A #gchar
 * @element: A #CyberKitDOMElement
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_element_insert_adjacent_element(CyberKitDOMElement* self, const gchar* where, CyberKitDOMElement* element, GError** error);

/**
 * webkit_dom_element_insert_adjacent_html:
 * @self: A #CyberKitDOMElement
 * @where: A #gchar
 * @html: A #gchar
 * @error: #GError
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_insert_adjacent_html(CyberKitDOMElement* self, const gchar* where, const gchar* html, GError** error);

/**
 * webkit_dom_element_insert_adjacent_text:
 * @self: A #CyberKitDOMElement
 * @where: A #gchar
 * @text: A #gchar
 * @error: #GError
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_insert_adjacent_text(CyberKitDOMElement* self, const gchar* where, const gchar* text, GError** error);

/**
 * webkit_dom_element_request_pointer_lock:
 * @self: A #CyberKitDOMElement
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_request_pointer_lock(CyberKitDOMElement* self);

/**
 * webkit_dom_element_remove:
 * @self: A #CyberKitDOMElement
 * @error: #GError
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_element_remove(CyberKitDOMElement* self, GError** error);

/**
 * webkit_dom_element_get_class_list:
 * @self: A #CyberKitDOMElement
 *
 * Returns: (transfer full): A #CyberKitDOMDOMTokenList
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMDOMTokenList*
webkit_dom_element_get_class_list(CyberKitDOMElement* self);

G_END_DECLS

#endif /* CyberKitDOMElement_h */
