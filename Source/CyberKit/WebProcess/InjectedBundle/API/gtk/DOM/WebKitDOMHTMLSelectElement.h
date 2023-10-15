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

#ifndef CyberKitDOMHTMLSelectElement_h
#define CyberKitDOMHTMLSelectElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_SELECT_ELEMENT            (webkit_dom_html_select_element_get_type())
#define WEBKIT_DOM_HTML_SELECT_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_SELECT_ELEMENT, CyberKitDOMHTMLSelectElement))
#define WEBKIT_DOM_HTML_SELECT_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_SELECT_ELEMENT, CyberKitDOMHTMLSelectElementClass)
#define WEBKIT_DOM_IS_HTML_SELECT_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_SELECT_ELEMENT))
#define WEBKIT_DOM_IS_HTML_SELECT_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_SELECT_ELEMENT))
#define WEBKIT_DOM_HTML_SELECT_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_SELECT_ELEMENT, CyberKitDOMHTMLSelectElementClass))

struct _CyberKitDOMHTMLSelectElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLSelectElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_select_element_get_type(void);

/**
 * webkit_dom_html_select_element_item:
 * @self: A #CyberKitDOMHTMLSelectElement
 * @index: A #gulong
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_html_select_element_item(CyberKitDOMHTMLSelectElement* self, gulong index);

/**
 * webkit_dom_html_select_element_named_item:
 * @self: A #CyberKitDOMHTMLSelectElement
 * @name: A #gchar
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_html_select_element_named_item(CyberKitDOMHTMLSelectElement* self, const gchar* name);

/**
 * webkit_dom_html_select_element_add:
 * @self: A #CyberKitDOMHTMLSelectElement
 * @element: A #CyberKitDOMHTMLElement
 * @before: A #CyberKitDOMHTMLElement
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_select_element_add(CyberKitDOMHTMLSelectElement* self, CyberKitDOMHTMLElement* element, CyberKitDOMHTMLElement* before, GError** error);

/**
 * webkit_dom_html_select_element_remove:
 * @self: A #CyberKitDOMHTMLSelectElement
 * @index: A #glong
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_select_element_remove(CyberKitDOMHTMLSelectElement* self, glong index);

/**
 * webkit_dom_html_select_element_get_autofocus:
 * @self: A #CyberKitDOMHTMLSelectElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_select_element_get_autofocus(CyberKitDOMHTMLSelectElement* self);

/**
 * webkit_dom_html_select_element_set_autofocus:
 * @self: A #CyberKitDOMHTMLSelectElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_select_element_set_autofocus(CyberKitDOMHTMLSelectElement* self, gboolean value);

/**
 * webkit_dom_html_select_element_get_disabled:
 * @self: A #CyberKitDOMHTMLSelectElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_select_element_get_disabled(CyberKitDOMHTMLSelectElement* self);

/**
 * webkit_dom_html_select_element_set_disabled:
 * @self: A #CyberKitDOMHTMLSelectElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_select_element_set_disabled(CyberKitDOMHTMLSelectElement* self, gboolean value);

/**
 * webkit_dom_html_select_element_get_form:
 * @self: A #CyberKitDOMHTMLSelectElement
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLFormElement
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLFormElement*
webkit_dom_html_select_element_get_form(CyberKitDOMHTMLSelectElement* self);

/**
 * webkit_dom_html_select_element_get_multiple:
 * @self: A #CyberKitDOMHTMLSelectElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_select_element_get_multiple(CyberKitDOMHTMLSelectElement* self);

/**
 * webkit_dom_html_select_element_set_multiple:
 * @self: A #CyberKitDOMHTMLSelectElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_select_element_set_multiple(CyberKitDOMHTMLSelectElement* self, gboolean value);

/**
 * webkit_dom_html_select_element_get_name:
 * @self: A #CyberKitDOMHTMLSelectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_select_element_get_name(CyberKitDOMHTMLSelectElement* self);

/**
 * webkit_dom_html_select_element_set_name:
 * @self: A #CyberKitDOMHTMLSelectElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_select_element_set_name(CyberKitDOMHTMLSelectElement* self, const gchar* value);

/**
 * webkit_dom_html_select_element_get_size:
 * @self: A #CyberKitDOMHTMLSelectElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_select_element_get_size(CyberKitDOMHTMLSelectElement* self);

/**
 * webkit_dom_html_select_element_set_size:
 * @self: A #CyberKitDOMHTMLSelectElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_select_element_set_size(CyberKitDOMHTMLSelectElement* self, glong value);

/**
 * webkit_dom_html_select_element_get_select_type:
 * @self: A #CyberKitDOMHTMLSelectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_select_element_get_select_type(CyberKitDOMHTMLSelectElement* self);

/**
 * webkit_dom_html_select_element_get_options:
 * @self: A #CyberKitDOMHTMLSelectElement
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLOptionsCollection
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLOptionsCollection*
webkit_dom_html_select_element_get_options(CyberKitDOMHTMLSelectElement* self);

/**
 * webkit_dom_html_select_element_get_length:
 * @self: A #CyberKitDOMHTMLSelectElement
 *
 * Returns: A #gulong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gulong
webkit_dom_html_select_element_get_length(CyberKitDOMHTMLSelectElement* self);

/**
 * webkit_dom_html_select_element_set_length:
 * @self: A #CyberKitDOMHTMLSelectElement
 * @value: A #gulong
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_select_element_set_length(CyberKitDOMHTMLSelectElement* self, gulong value, GError** error);

/**
 * webkit_dom_html_select_element_get_selected_index:
 * @self: A #CyberKitDOMHTMLSelectElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_select_element_get_selected_index(CyberKitDOMHTMLSelectElement* self);

/**
 * webkit_dom_html_select_element_set_selected_index:
 * @self: A #CyberKitDOMHTMLSelectElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_select_element_set_selected_index(CyberKitDOMHTMLSelectElement* self, glong value);

/**
 * webkit_dom_html_select_element_get_value:
 * @self: A #CyberKitDOMHTMLSelectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_select_element_get_value(CyberKitDOMHTMLSelectElement* self);

/**
 * webkit_dom_html_select_element_set_value:
 * @self: A #CyberKitDOMHTMLSelectElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_select_element_set_value(CyberKitDOMHTMLSelectElement* self, const gchar* value);

/**
 * webkit_dom_html_select_element_get_will_validate:
 * @self: A #CyberKitDOMHTMLSelectElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_select_element_get_will_validate(CyberKitDOMHTMLSelectElement* self);

G_END_DECLS

#endif /* CyberKitDOMHTMLSelectElement_h */
