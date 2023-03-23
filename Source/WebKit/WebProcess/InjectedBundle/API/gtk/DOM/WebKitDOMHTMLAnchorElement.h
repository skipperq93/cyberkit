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

#ifndef CyberKitDOMHTMLAnchorElement_h
#define CyberKitDOMHTMLAnchorElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_ANCHOR_ELEMENT            (webkit_dom_html_anchor_element_get_type())
#define WEBKIT_DOM_HTML_ANCHOR_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_ANCHOR_ELEMENT, CyberKitDOMHTMLAnchorElement))
#define WEBKIT_DOM_HTML_ANCHOR_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_ANCHOR_ELEMENT, CyberKitDOMHTMLAnchorElementClass)
#define WEBKIT_DOM_IS_HTML_ANCHOR_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_ANCHOR_ELEMENT))
#define WEBKIT_DOM_IS_HTML_ANCHOR_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_ANCHOR_ELEMENT))
#define WEBKIT_DOM_HTML_ANCHOR_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_ANCHOR_ELEMENT, CyberKitDOMHTMLAnchorElementClass))

struct _CyberKitDOMHTMLAnchorElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLAnchorElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_anchor_element_get_type(void);

/**
 * webkit_dom_html_anchor_element_get_charset:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_charset(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_charset:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_charset(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_coords:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_coords(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_coords:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_coords(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_hreflang:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_hreflang(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_hreflang:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_hreflang(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_name:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_name(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_name:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_name(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_rel:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_rel(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_rel:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_rel(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_rev:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_rev(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_rev:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_rev(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_shape:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_shape(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_shape:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_shape(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_target:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_target(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_target:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_target(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_type_attr:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_type_attr(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_type_attr:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_type_attr(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_text:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_text(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_text:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_text(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_href:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_href(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_href:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_href(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_protocol:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_protocol(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_protocol:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_protocol(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_host:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_host(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_host:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_host(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_hostname:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_hostname(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_hostname:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_hostname(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_port:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_port(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_port:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_port(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_pathname:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_pathname(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_pathname:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_pathname(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_search:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_search(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_search:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_search(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

/**
 * webkit_dom_html_anchor_element_get_hash:
 * @self: A #CyberKitDOMHTMLAnchorElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_anchor_element_get_hash(CyberKitDOMHTMLAnchorElement* self);

/**
 * webkit_dom_html_anchor_element_set_hash:
 * @self: A #CyberKitDOMHTMLAnchorElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_anchor_element_set_hash(CyberKitDOMHTMLAnchorElement* self, const gchar* value);

G_END_DECLS

#endif /* CyberKitDOMHTMLAnchorElement_h */
