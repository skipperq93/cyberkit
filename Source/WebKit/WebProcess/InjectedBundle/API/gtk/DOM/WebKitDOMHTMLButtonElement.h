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

#ifndef CyberKitDOMHTMLButtonElement_h
#define CyberKitDOMHTMLButtonElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_BUTTON_ELEMENT            (webkit_dom_html_button_element_get_type())
#define WEBKIT_DOM_HTML_BUTTON_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_BUTTON_ELEMENT, CyberKitDOMHTMLButtonElement))
#define WEBKIT_DOM_HTML_BUTTON_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_BUTTON_ELEMENT, CyberKitDOMHTMLButtonElementClass)
#define WEBKIT_DOM_IS_HTML_BUTTON_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_BUTTON_ELEMENT))
#define WEBKIT_DOM_IS_HTML_BUTTON_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_BUTTON_ELEMENT))
#define WEBKIT_DOM_HTML_BUTTON_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_BUTTON_ELEMENT, CyberKitDOMHTMLButtonElementClass))

struct _CyberKitDOMHTMLButtonElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLButtonElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_button_element_get_type(void);

/**
 * webkit_dom_html_button_element_get_autofocus:
 * @self: A #CyberKitDOMHTMLButtonElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_button_element_get_autofocus(CyberKitDOMHTMLButtonElement* self);

/**
 * webkit_dom_html_button_element_set_autofocus:
 * @self: A #CyberKitDOMHTMLButtonElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_button_element_set_autofocus(CyberKitDOMHTMLButtonElement* self, gboolean value);

/**
 * webkit_dom_html_button_element_get_disabled:
 * @self: A #CyberKitDOMHTMLButtonElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_button_element_get_disabled(CyberKitDOMHTMLButtonElement* self);

/**
 * webkit_dom_html_button_element_set_disabled:
 * @self: A #CyberKitDOMHTMLButtonElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_button_element_set_disabled(CyberKitDOMHTMLButtonElement* self, gboolean value);

/**
 * webkit_dom_html_button_element_get_form:
 * @self: A #CyberKitDOMHTMLButtonElement
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLFormElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLFormElement*
webkit_dom_html_button_element_get_form(CyberKitDOMHTMLButtonElement* self);

/**
 * webkit_dom_html_button_element_get_button_type:
 * @self: A #CyberKitDOMHTMLButtonElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_button_element_get_button_type(CyberKitDOMHTMLButtonElement* self);

/**
 * webkit_dom_html_button_element_set_button_type:
 * @self: A #CyberKitDOMHTMLButtonElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_button_element_set_button_type(CyberKitDOMHTMLButtonElement* self, const gchar* value);

/**
 * webkit_dom_html_button_element_get_name:
 * @self: A #CyberKitDOMHTMLButtonElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_button_element_get_name(CyberKitDOMHTMLButtonElement* self);

/**
 * webkit_dom_html_button_element_set_name:
 * @self: A #CyberKitDOMHTMLButtonElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_button_element_set_name(CyberKitDOMHTMLButtonElement* self, const gchar* value);

/**
 * webkit_dom_html_button_element_get_value:
 * @self: A #CyberKitDOMHTMLButtonElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_button_element_get_value(CyberKitDOMHTMLButtonElement* self);

/**
 * webkit_dom_html_button_element_set_value:
 * @self: A #CyberKitDOMHTMLButtonElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_button_element_set_value(CyberKitDOMHTMLButtonElement* self, const gchar* value);

/**
 * webkit_dom_html_button_element_get_will_validate:
 * @self: A #CyberKitDOMHTMLButtonElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_button_element_get_will_validate(CyberKitDOMHTMLButtonElement* self);

G_END_DECLS

#endif /* CyberKitDOMHTMLButtonElement_h */
