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

#ifndef CyberKitDOMHTMLMapElement_h
#define CyberKitDOMHTMLMapElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_MAP_ELEMENT            (webkit_dom_html_map_element_get_type())
#define WEBKIT_DOM_HTML_MAP_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_MAP_ELEMENT, CyberKitDOMHTMLMapElement))
#define WEBKIT_DOM_HTML_MAP_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_MAP_ELEMENT, CyberKitDOMHTMLMapElementClass)
#define WEBKIT_DOM_IS_HTML_MAP_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_MAP_ELEMENT))
#define WEBKIT_DOM_IS_HTML_MAP_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_MAP_ELEMENT))
#define WEBKIT_DOM_HTML_MAP_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_MAP_ELEMENT, CyberKitDOMHTMLMapElementClass))

struct _CyberKitDOMHTMLMapElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLMapElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_map_element_get_type(void);

/**
 * webkit_dom_html_map_element_get_areas:
 * @self: A #CyberKitDOMHTMLMapElement
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_html_map_element_get_areas(CyberKitDOMHTMLMapElement* self);

/**
 * webkit_dom_html_map_element_get_name:
 * @self: A #CyberKitDOMHTMLMapElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_map_element_get_name(CyberKitDOMHTMLMapElement* self);

/**
 * webkit_dom_html_map_element_set_name:
 * @self: A #CyberKitDOMHTMLMapElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_map_element_set_name(CyberKitDOMHTMLMapElement* self, const gchar* value);

G_END_DECLS

#endif /* CyberKitDOMHTMLMapElement_h */
