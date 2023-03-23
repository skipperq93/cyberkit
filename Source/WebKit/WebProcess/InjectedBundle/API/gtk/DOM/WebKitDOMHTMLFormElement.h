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

#ifndef CyberKitDOMHTMLFormElement_h
#define CyberKitDOMHTMLFormElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_FORM_ELEMENT            (webkit_dom_html_form_element_get_type())
#define WEBKIT_DOM_HTML_FORM_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_FORM_ELEMENT, CyberKitDOMHTMLFormElement))
#define WEBKIT_DOM_HTML_FORM_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_FORM_ELEMENT, CyberKitDOMHTMLFormElementClass)
#define WEBKIT_DOM_IS_HTML_FORM_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_FORM_ELEMENT))
#define WEBKIT_DOM_IS_HTML_FORM_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_FORM_ELEMENT))
#define WEBKIT_DOM_HTML_FORM_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_FORM_ELEMENT, CyberKitDOMHTMLFormElementClass))

struct _CyberKitDOMHTMLFormElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLFormElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_form_element_get_type(void);

/**
 * webkit_dom_html_form_element_submit:
 * @self: A #CyberKitDOMHTMLFormElement
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_form_element_submit(CyberKitDOMHTMLFormElement* self);

/**
 * webkit_dom_html_form_element_reset:
 * @self: A #CyberKitDOMHTMLFormElement
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_form_element_reset(CyberKitDOMHTMLFormElement* self);

/**
 * webkit_dom_html_form_element_get_accept_charset:
 * @self: A #CyberKitDOMHTMLFormElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_form_element_get_accept_charset(CyberKitDOMHTMLFormElement* self);

/**
 * webkit_dom_html_form_element_set_accept_charset:
 * @self: A #CyberKitDOMHTMLFormElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_form_element_set_accept_charset(CyberKitDOMHTMLFormElement* self, const gchar* value);

/**
 * webkit_dom_html_form_element_get_action:
 * @self: A #CyberKitDOMHTMLFormElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_form_element_get_action(CyberKitDOMHTMLFormElement* self);

/**
 * webkit_dom_html_form_element_set_action:
 * @self: A #CyberKitDOMHTMLFormElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_form_element_set_action(CyberKitDOMHTMLFormElement* self, const gchar* value);

/**
 * webkit_dom_html_form_element_get_enctype:
 * @self: A #CyberKitDOMHTMLFormElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_form_element_get_enctype(CyberKitDOMHTMLFormElement* self);

/**
 * webkit_dom_html_form_element_set_enctype:
 * @self: A #CyberKitDOMHTMLFormElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_form_element_set_enctype(CyberKitDOMHTMLFormElement* self, const gchar* value);

/**
 * webkit_dom_html_form_element_get_encoding:
 * @self: A #CyberKitDOMHTMLFormElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_form_element_get_encoding(CyberKitDOMHTMLFormElement* self);

/**
 * webkit_dom_html_form_element_set_encoding:
 * @self: A #CyberKitDOMHTMLFormElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_form_element_set_encoding(CyberKitDOMHTMLFormElement* self, const gchar* value);

/**
 * webkit_dom_html_form_element_get_method:
 * @self: A #CyberKitDOMHTMLFormElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_form_element_get_method(CyberKitDOMHTMLFormElement* self);

/**
 * webkit_dom_html_form_element_set_method:
 * @self: A #CyberKitDOMHTMLFormElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_form_element_set_method(CyberKitDOMHTMLFormElement* self, const gchar* value);

/**
 * webkit_dom_html_form_element_get_name:
 * @self: A #CyberKitDOMHTMLFormElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_form_element_get_name(CyberKitDOMHTMLFormElement* self);

/**
 * webkit_dom_html_form_element_set_name:
 * @self: A #CyberKitDOMHTMLFormElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_form_element_set_name(CyberKitDOMHTMLFormElement* self, const gchar* value);

/**
 * webkit_dom_html_form_element_get_target:
 * @self: A #CyberKitDOMHTMLFormElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_form_element_get_target(CyberKitDOMHTMLFormElement* self);

/**
 * webkit_dom_html_form_element_set_target:
 * @self: A #CyberKitDOMHTMLFormElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_form_element_set_target(CyberKitDOMHTMLFormElement* self, const gchar* value);

/**
 * webkit_dom_html_form_element_get_elements:
 * @self: A #CyberKitDOMHTMLFormElement
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_html_form_element_get_elements(CyberKitDOMHTMLFormElement* self);

/**
 * webkit_dom_html_form_element_get_length:
 * @self: A #CyberKitDOMHTMLFormElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_form_element_get_length(CyberKitDOMHTMLFormElement* self);

G_END_DECLS

#endif /* CyberKitDOMHTMLFormElement_h */
