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

#ifndef CyberKitDOMHTMLMetaElement_h
#define CyberKitDOMHTMLMetaElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_META_ELEMENT            (webkit_dom_html_meta_element_get_type())
#define WEBKIT_DOM_HTML_META_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_META_ELEMENT, CyberKitDOMHTMLMetaElement))
#define WEBKIT_DOM_HTML_META_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_META_ELEMENT, CyberKitDOMHTMLMetaElementClass)
#define WEBKIT_DOM_IS_HTML_META_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_META_ELEMENT))
#define WEBKIT_DOM_IS_HTML_META_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_META_ELEMENT))
#define WEBKIT_DOM_HTML_META_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_META_ELEMENT, CyberKitDOMHTMLMetaElementClass))

struct _CyberKitDOMHTMLMetaElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLMetaElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_meta_element_get_type(void);

/**
 * webkit_dom_html_meta_element_get_content:
 * @self: A #CyberKitDOMHTMLMetaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_meta_element_get_content(CyberKitDOMHTMLMetaElement* self);

/**
 * webkit_dom_html_meta_element_set_content:
 * @self: A #CyberKitDOMHTMLMetaElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_meta_element_set_content(CyberKitDOMHTMLMetaElement* self, const gchar* value);

/**
 * webkit_dom_html_meta_element_get_http_equiv:
 * @self: A #CyberKitDOMHTMLMetaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_meta_element_get_http_equiv(CyberKitDOMHTMLMetaElement* self);

/**
 * webkit_dom_html_meta_element_set_http_equiv:
 * @self: A #CyberKitDOMHTMLMetaElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_meta_element_set_http_equiv(CyberKitDOMHTMLMetaElement* self, const gchar* value);

/**
 * webkit_dom_html_meta_element_get_name:
 * @self: A #CyberKitDOMHTMLMetaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_meta_element_get_name(CyberKitDOMHTMLMetaElement* self);

/**
 * webkit_dom_html_meta_element_set_name:
 * @self: A #CyberKitDOMHTMLMetaElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_meta_element_set_name(CyberKitDOMHTMLMetaElement* self, const gchar* value);

/**
 * webkit_dom_html_meta_element_get_scheme:
 * @self: A #CyberKitDOMHTMLMetaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_meta_element_get_scheme(CyberKitDOMHTMLMetaElement* self);

/**
 * webkit_dom_html_meta_element_set_scheme:
 * @self: A #CyberKitDOMHTMLMetaElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_meta_element_set_scheme(CyberKitDOMHTMLMetaElement* self, const gchar* value);

G_END_DECLS

#endif /* CyberKitDOMHTMLMetaElement_h */
