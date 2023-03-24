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

#ifndef CyberKitDOMHTMLImageElement_h
#define CyberKitDOMHTMLImageElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_IMAGE_ELEMENT            (webkit_dom_html_image_element_get_type())
#define WEBKIT_DOM_HTML_IMAGE_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_IMAGE_ELEMENT, CyberKitDOMHTMLImageElement))
#define WEBKIT_DOM_HTML_IMAGE_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_IMAGE_ELEMENT, CyberKitDOMHTMLImageElementClass)
#define WEBKIT_DOM_IS_HTML_IMAGE_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_IMAGE_ELEMENT))
#define WEBKIT_DOM_IS_HTML_IMAGE_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_IMAGE_ELEMENT))
#define WEBKIT_DOM_HTML_IMAGE_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_IMAGE_ELEMENT, CyberKitDOMHTMLImageElementClass))

struct _CyberKitDOMHTMLImageElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLImageElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_image_element_get_type(void);

/**
 * webkit_dom_html_image_element_get_name:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_image_element_get_name(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_set_name:
 * @self: A #CyberKitDOMHTMLImageElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_image_element_set_name(CyberKitDOMHTMLImageElement* self, const gchar* value);

/**
 * webkit_dom_html_image_element_get_align:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_image_element_get_align(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_set_align:
 * @self: A #CyberKitDOMHTMLImageElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_image_element_set_align(CyberKitDOMHTMLImageElement* self, const gchar* value);

/**
 * webkit_dom_html_image_element_get_alt:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_image_element_get_alt(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_set_alt:
 * @self: A #CyberKitDOMHTMLImageElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_image_element_set_alt(CyberKitDOMHTMLImageElement* self, const gchar* value);

/**
 * webkit_dom_html_image_element_get_border:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_image_element_get_border(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_set_border:
 * @self: A #CyberKitDOMHTMLImageElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_image_element_set_border(CyberKitDOMHTMLImageElement* self, const gchar* value);

/**
 * webkit_dom_html_image_element_get_height:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_image_element_get_height(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_set_height:
 * @self: A #CyberKitDOMHTMLImageElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_image_element_set_height(CyberKitDOMHTMLImageElement* self, glong value);

/**
 * webkit_dom_html_image_element_get_hspace:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_image_element_get_hspace(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_set_hspace:
 * @self: A #CyberKitDOMHTMLImageElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_image_element_set_hspace(CyberKitDOMHTMLImageElement* self, glong value);

/**
 * webkit_dom_html_image_element_get_is_map:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_image_element_get_is_map(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_set_is_map:
 * @self: A #CyberKitDOMHTMLImageElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_image_element_set_is_map(CyberKitDOMHTMLImageElement* self, gboolean value);

/**
 * webkit_dom_html_image_element_get_long_desc:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_image_element_get_long_desc(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_set_long_desc:
 * @self: A #CyberKitDOMHTMLImageElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_image_element_set_long_desc(CyberKitDOMHTMLImageElement* self, const gchar* value);

/**
 * webkit_dom_html_image_element_get_src:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_image_element_get_src(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_set_src:
 * @self: A #CyberKitDOMHTMLImageElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_image_element_set_src(CyberKitDOMHTMLImageElement* self, const gchar* value);

/**
 * webkit_dom_html_image_element_get_use_map:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_image_element_get_use_map(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_set_use_map:
 * @self: A #CyberKitDOMHTMLImageElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_image_element_set_use_map(CyberKitDOMHTMLImageElement* self, const gchar* value);

/**
 * webkit_dom_html_image_element_get_vspace:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_image_element_get_vspace(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_set_vspace:
 * @self: A #CyberKitDOMHTMLImageElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_image_element_set_vspace(CyberKitDOMHTMLImageElement* self, glong value);

/**
 * webkit_dom_html_image_element_get_width:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_image_element_get_width(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_set_width:
 * @self: A #CyberKitDOMHTMLImageElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_image_element_set_width(CyberKitDOMHTMLImageElement* self, glong value);

/**
 * webkit_dom_html_image_element_get_complete:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_image_element_get_complete(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_get_lowsrc:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_image_element_get_lowsrc(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_set_lowsrc:
 * @self: A #CyberKitDOMHTMLImageElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_image_element_set_lowsrc(CyberKitDOMHTMLImageElement* self, const gchar* value);

/**
 * webkit_dom_html_image_element_get_natural_height:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_image_element_get_natural_height(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_get_natural_width:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_image_element_get_natural_width(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_get_x:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_image_element_get_x(CyberKitDOMHTMLImageElement* self);

/**
 * webkit_dom_html_image_element_get_y:
 * @self: A #CyberKitDOMHTMLImageElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_image_element_get_y(CyberKitDOMHTMLImageElement* self);

G_END_DECLS

#endif /* CyberKitDOMHTMLImageElement_h */
