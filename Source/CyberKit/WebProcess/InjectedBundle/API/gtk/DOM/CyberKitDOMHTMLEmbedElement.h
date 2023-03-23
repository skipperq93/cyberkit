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

#ifndef CyberKitDOMHTMLEmbedElement_h
#define CyberKitDOMHTMLEmbedElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_EMBED_ELEMENT            (webkit_dom_html_embed_element_get_type())
#define WEBKIT_DOM_HTML_EMBED_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_EMBED_ELEMENT, CyberKitDOMHTMLEmbedElement))
#define WEBKIT_DOM_HTML_EMBED_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_EMBED_ELEMENT, CyberKitDOMHTMLEmbedElementClass)
#define WEBKIT_DOM_IS_HTML_EMBED_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_EMBED_ELEMENT))
#define WEBKIT_DOM_IS_HTML_EMBED_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_EMBED_ELEMENT))
#define WEBKIT_DOM_HTML_EMBED_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_EMBED_ELEMENT, CyberKitDOMHTMLEmbedElementClass))

struct _CyberKitDOMHTMLEmbedElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLEmbedElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_embed_element_get_type(void);

/**
 * webkit_dom_html_embed_element_get_align:
 * @self: A #CyberKitDOMHTMLEmbedElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_embed_element_get_align(CyberKitDOMHTMLEmbedElement* self);

/**
 * webkit_dom_html_embed_element_set_align:
 * @self: A #CyberKitDOMHTMLEmbedElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_embed_element_set_align(CyberKitDOMHTMLEmbedElement* self, const gchar* value);

/**
 * webkit_dom_html_embed_element_get_height:
 * @self: A #CyberKitDOMHTMLEmbedElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_embed_element_get_height(CyberKitDOMHTMLEmbedElement* self);

/**
 * webkit_dom_html_embed_element_set_height:
 * @self: A #CyberKitDOMHTMLEmbedElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_embed_element_set_height(CyberKitDOMHTMLEmbedElement* self, glong value);

/**
 * webkit_dom_html_embed_element_get_name:
 * @self: A #CyberKitDOMHTMLEmbedElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_embed_element_get_name(CyberKitDOMHTMLEmbedElement* self);

/**
 * webkit_dom_html_embed_element_set_name:
 * @self: A #CyberKitDOMHTMLEmbedElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_embed_element_set_name(CyberKitDOMHTMLEmbedElement* self, const gchar* value);

/**
 * webkit_dom_html_embed_element_get_src:
 * @self: A #CyberKitDOMHTMLEmbedElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_embed_element_get_src(CyberKitDOMHTMLEmbedElement* self);

/**
 * webkit_dom_html_embed_element_set_src:
 * @self: A #CyberKitDOMHTMLEmbedElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_embed_element_set_src(CyberKitDOMHTMLEmbedElement* self, const gchar* value);

/**
 * webkit_dom_html_embed_element_get_type_attr:
 * @self: A #CyberKitDOMHTMLEmbedElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_embed_element_get_type_attr(CyberKitDOMHTMLEmbedElement* self);

/**
 * webkit_dom_html_embed_element_set_type_attr:
 * @self: A #CyberKitDOMHTMLEmbedElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_embed_element_set_type_attr(CyberKitDOMHTMLEmbedElement* self, const gchar* value);

/**
 * webkit_dom_html_embed_element_get_width:
 * @self: A #CyberKitDOMHTMLEmbedElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_embed_element_get_width(CyberKitDOMHTMLEmbedElement* self);

/**
 * webkit_dom_html_embed_element_set_width:
 * @self: A #CyberKitDOMHTMLEmbedElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_embed_element_set_width(CyberKitDOMHTMLEmbedElement* self, glong value);

G_END_DECLS

#endif /* CyberKitDOMHTMLEmbedElement_h */
