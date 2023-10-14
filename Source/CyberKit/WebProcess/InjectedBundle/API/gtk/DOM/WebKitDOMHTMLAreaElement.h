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

#ifndef CyberKitDOMHTMLAreaElement_h
#define CyberKitDOMHTMLAreaElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_AREA_ELEMENT            (webkit_dom_html_area_element_get_type())
#define WEBKIT_DOM_HTML_AREA_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_AREA_ELEMENT, CyberKitDOMHTMLAreaElement))
#define WEBKIT_DOM_HTML_AREA_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_AREA_ELEMENT, CyberKitDOMHTMLAreaElementClass)
#define WEBKIT_DOM_IS_HTML_AREA_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_AREA_ELEMENT))
#define WEBKIT_DOM_IS_HTML_AREA_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_AREA_ELEMENT))
#define WEBKIT_DOM_HTML_AREA_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_AREA_ELEMENT, CyberKitDOMHTMLAreaElementClass))

struct _CyberKitDOMHTMLAreaElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLAreaElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_area_element_get_type(void);

/**
 * webkit_dom_html_area_element_get_alt:
 * @self: A #CyberKitDOMHTMLAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_area_element_get_alt(CyberKitDOMHTMLAreaElement* self);

/**
 * webkit_dom_html_area_element_set_alt:
 * @self: A #CyberKitDOMHTMLAreaElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_area_element_set_alt(CyberKitDOMHTMLAreaElement* self, const gchar* value);

/**
 * webkit_dom_html_area_element_get_coords:
 * @self: A #CyberKitDOMHTMLAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_area_element_get_coords(CyberKitDOMHTMLAreaElement* self);

/**
 * webkit_dom_html_area_element_set_coords:
 * @self: A #CyberKitDOMHTMLAreaElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_area_element_set_coords(CyberKitDOMHTMLAreaElement* self, const gchar* value);

/**
 * webkit_dom_html_area_element_get_no_href:
 * @self: A #CyberKitDOMHTMLAreaElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_area_element_get_no_href(CyberKitDOMHTMLAreaElement* self);

/**
 * webkit_dom_html_area_element_set_no_href:
 * @self: A #CyberKitDOMHTMLAreaElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_area_element_set_no_href(CyberKitDOMHTMLAreaElement* self, gboolean value);

/**
 * webkit_dom_html_area_element_get_shape:
 * @self: A #CyberKitDOMHTMLAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_area_element_get_shape(CyberKitDOMHTMLAreaElement* self);

/**
 * webkit_dom_html_area_element_set_shape:
 * @self: A #CyberKitDOMHTMLAreaElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_area_element_set_shape(CyberKitDOMHTMLAreaElement* self, const gchar* value);

/**
 * webkit_dom_html_area_element_get_target:
 * @self: A #CyberKitDOMHTMLAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_area_element_get_target(CyberKitDOMHTMLAreaElement* self);

/**
 * webkit_dom_html_area_element_set_target:
 * @self: A #CyberKitDOMHTMLAreaElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_area_element_set_target(CyberKitDOMHTMLAreaElement* self, const gchar* value);

/**
 * webkit_dom_html_area_element_get_href:
 * @self: A #CyberKitDOMHTMLAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_area_element_get_href(CyberKitDOMHTMLAreaElement* self);

/**
 * webkit_dom_html_area_element_set_href:
 * @self: A #CyberKitDOMHTMLAreaElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_area_element_set_href(CyberKitDOMHTMLAreaElement* self, const gchar* value);

/**
 * webkit_dom_html_area_element_get_protocol:
 * @self: A #CyberKitDOMHTMLAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_area_element_get_protocol(CyberKitDOMHTMLAreaElement* self);

/**
 * webkit_dom_html_area_element_set_protocol:
 * @self: A #CyberKitDOMHTMLAreaElement
 * @value: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_area_element_set_protocol(CyberKitDOMHTMLAreaElement* self, const gchar* value);

/**
 * webkit_dom_html_area_element_get_host:
 * @self: A #CyberKitDOMHTMLAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_area_element_get_host(CyberKitDOMHTMLAreaElement* self);

/**
 * webkit_dom_html_area_element_set_host:
 * @self: A #CyberKitDOMHTMLAreaElement
 * @value: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_area_element_set_host(CyberKitDOMHTMLAreaElement* self, const gchar* value);

/**
 * webkit_dom_html_area_element_get_hostname:
 * @self: A #CyberKitDOMHTMLAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_area_element_get_hostname(CyberKitDOMHTMLAreaElement* self);

/**
 * webkit_dom_html_area_element_set_hostname:
 * @self: A #CyberKitDOMHTMLAreaElement
 * @value: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_area_element_set_hostname(CyberKitDOMHTMLAreaElement* self, const gchar* value);

/**
 * webkit_dom_html_area_element_get_port:
 * @self: A #CyberKitDOMHTMLAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_area_element_get_port(CyberKitDOMHTMLAreaElement* self);

/**
 * webkit_dom_html_area_element_set_port:
 * @self: A #CyberKitDOMHTMLAreaElement
 * @value: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_area_element_set_port(CyberKitDOMHTMLAreaElement* self, const gchar* value);

/**
 * webkit_dom_html_area_element_get_pathname:
 * @self: A #CyberKitDOMHTMLAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_area_element_get_pathname(CyberKitDOMHTMLAreaElement* self);

/**
 * webkit_dom_html_area_element_set_pathname:
 * @self: A #CyberKitDOMHTMLAreaElement
 * @value: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_area_element_set_pathname(CyberKitDOMHTMLAreaElement* self, const gchar* value);

/**
 * webkit_dom_html_area_element_get_search:
 * @self: A #CyberKitDOMHTMLAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_area_element_get_search(CyberKitDOMHTMLAreaElement* self);

/**
 * webkit_dom_html_area_element_set_search:
 * @self: A #CyberKitDOMHTMLAreaElement
 * @value: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_area_element_set_search(CyberKitDOMHTMLAreaElement* self, const gchar* value);

/**
 * webkit_dom_html_area_element_get_hash:
 * @self: A #CyberKitDOMHTMLAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_area_element_get_hash(CyberKitDOMHTMLAreaElement* self);

/**
 * webkit_dom_html_area_element_set_hash:
 * @self: A #CyberKitDOMHTMLAreaElement
 * @value: A #gchar
 *
 * Stability: Unstable
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_area_element_set_hash(CyberKitDOMHTMLAreaElement* self, const gchar* value);

G_END_DECLS

#endif /* CyberKitDOMHTMLAreaElement_h */
