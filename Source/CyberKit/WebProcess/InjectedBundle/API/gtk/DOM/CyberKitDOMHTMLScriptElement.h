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

#ifndef CyberKitDOMHTMLScriptElement_h
#define CyberKitDOMHTMLScriptElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_SCRIPT_ELEMENT            (webkit_dom_html_script_element_get_type())
#define WEBKIT_DOM_HTML_SCRIPT_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_SCRIPT_ELEMENT, CyberKitDOMHTMLScriptElement))
#define WEBKIT_DOM_HTML_SCRIPT_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_SCRIPT_ELEMENT, CyberKitDOMHTMLScriptElementClass)
#define WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_SCRIPT_ELEMENT))
#define WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_SCRIPT_ELEMENT))
#define WEBKIT_DOM_HTML_SCRIPT_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_SCRIPT_ELEMENT, CyberKitDOMHTMLScriptElementClass))

struct _CyberKitDOMHTMLScriptElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLScriptElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_script_element_get_type(void);

/**
 * webkit_dom_html_script_element_get_text:
 * @self: A #CyberKitDOMHTMLScriptElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_script_element_get_text(CyberKitDOMHTMLScriptElement* self);

/**
 * webkit_dom_html_script_element_set_text:
 * @self: A #CyberKitDOMHTMLScriptElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_script_element_set_text(CyberKitDOMHTMLScriptElement* self, const gchar* value);

/**
 * webkit_dom_html_script_element_get_html_for:
 * @self: A #CyberKitDOMHTMLScriptElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_script_element_get_html_for(CyberKitDOMHTMLScriptElement* self);

/**
 * webkit_dom_html_script_element_set_html_for:
 * @self: A #CyberKitDOMHTMLScriptElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_script_element_set_html_for(CyberKitDOMHTMLScriptElement* self, const gchar* value);

/**
 * webkit_dom_html_script_element_get_event:
 * @self: A #CyberKitDOMHTMLScriptElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_script_element_get_event(CyberKitDOMHTMLScriptElement* self);

/**
 * webkit_dom_html_script_element_set_event:
 * @self: A #CyberKitDOMHTMLScriptElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_script_element_set_event(CyberKitDOMHTMLScriptElement* self, const gchar* value);

/**
 * webkit_dom_html_script_element_get_charset:
 * @self: A #CyberKitDOMHTMLScriptElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_script_element_get_charset(CyberKitDOMHTMLScriptElement* self);

/**
 * webkit_dom_html_script_element_set_charset:
 * @self: A #CyberKitDOMHTMLScriptElement
 * @value: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_script_element_set_charset(CyberKitDOMHTMLScriptElement* self, const gchar* value);

/**
 * webkit_dom_html_script_element_get_defer:
 * @self: A #CyberKitDOMHTMLScriptElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_script_element_get_defer(CyberKitDOMHTMLScriptElement* self);

/**
 * webkit_dom_html_script_element_set_defer:
 * @self: A #CyberKitDOMHTMLScriptElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_script_element_set_defer(CyberKitDOMHTMLScriptElement* self, gboolean value);

/**
 * webkit_dom_html_script_element_get_src:
 * @self: A #CyberKitDOMHTMLScriptElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_script_element_get_src(CyberKitDOMHTMLScriptElement* self);

/**
 * webkit_dom_html_script_element_set_src:
 * @self: A #CyberKitDOMHTMLScriptElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_script_element_set_src(CyberKitDOMHTMLScriptElement* self, const gchar* value);

/**
 * webkit_dom_html_script_element_get_type_attr:
 * @self: A #CyberKitDOMHTMLScriptElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_script_element_get_type_attr(CyberKitDOMHTMLScriptElement* self);

/**
 * webkit_dom_html_script_element_set_type_attr:
 * @self: A #CyberKitDOMHTMLScriptElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_script_element_set_type_attr(CyberKitDOMHTMLScriptElement* self, const gchar* value);

G_END_DECLS

#endif /* CyberKitDOMHTMLScriptElement_h */
