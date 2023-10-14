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

#ifndef CyberKitDOMHTMLFrameElement_h
#define CyberKitDOMHTMLFrameElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_FRAME_ELEMENT            (webkit_dom_html_frame_element_get_type())
#define WEBKIT_DOM_HTML_FRAME_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_FRAME_ELEMENT, CyberKitDOMHTMLFrameElement))
#define WEBKIT_DOM_HTML_FRAME_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_FRAME_ELEMENT, CyberKitDOMHTMLFrameElementClass)
#define WEBKIT_DOM_IS_HTML_FRAME_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_FRAME_ELEMENT))
#define WEBKIT_DOM_IS_HTML_FRAME_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_FRAME_ELEMENT))
#define WEBKIT_DOM_HTML_FRAME_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_FRAME_ELEMENT, CyberKitDOMHTMLFrameElementClass))

struct _CyberKitDOMHTMLFrameElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLFrameElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_frame_element_get_type(void);

/**
 * webkit_dom_html_frame_element_get_frame_border:
 * @self: A #CyberKitDOMHTMLFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_frame_element_get_frame_border(CyberKitDOMHTMLFrameElement* self);

/**
 * webkit_dom_html_frame_element_set_frame_border:
 * @self: A #CyberKitDOMHTMLFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_frame_element_set_frame_border(CyberKitDOMHTMLFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_frame_element_get_long_desc:
 * @self: A #CyberKitDOMHTMLFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_frame_element_get_long_desc(CyberKitDOMHTMLFrameElement* self);

/**
 * webkit_dom_html_frame_element_set_long_desc:
 * @self: A #CyberKitDOMHTMLFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_frame_element_set_long_desc(CyberKitDOMHTMLFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_frame_element_get_margin_height:
 * @self: A #CyberKitDOMHTMLFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_frame_element_get_margin_height(CyberKitDOMHTMLFrameElement* self);

/**
 * webkit_dom_html_frame_element_set_margin_height:
 * @self: A #CyberKitDOMHTMLFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_frame_element_set_margin_height(CyberKitDOMHTMLFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_frame_element_get_margin_width:
 * @self: A #CyberKitDOMHTMLFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_frame_element_get_margin_width(CyberKitDOMHTMLFrameElement* self);

/**
 * webkit_dom_html_frame_element_set_margin_width:
 * @self: A #CyberKitDOMHTMLFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_frame_element_set_margin_width(CyberKitDOMHTMLFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_frame_element_get_name:
 * @self: A #CyberKitDOMHTMLFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_frame_element_get_name(CyberKitDOMHTMLFrameElement* self);

/**
 * webkit_dom_html_frame_element_set_name:
 * @self: A #CyberKitDOMHTMLFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_frame_element_set_name(CyberKitDOMHTMLFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_frame_element_get_no_resize:
 * @self: A #CyberKitDOMHTMLFrameElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_frame_element_get_no_resize(CyberKitDOMHTMLFrameElement* self);

/**
 * webkit_dom_html_frame_element_set_no_resize:
 * @self: A #CyberKitDOMHTMLFrameElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_frame_element_set_no_resize(CyberKitDOMHTMLFrameElement* self, gboolean value);

/**
 * webkit_dom_html_frame_element_get_scrolling:
 * @self: A #CyberKitDOMHTMLFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_frame_element_get_scrolling(CyberKitDOMHTMLFrameElement* self);

/**
 * webkit_dom_html_frame_element_set_scrolling:
 * @self: A #CyberKitDOMHTMLFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_frame_element_set_scrolling(CyberKitDOMHTMLFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_frame_element_get_src:
 * @self: A #CyberKitDOMHTMLFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_frame_element_get_src(CyberKitDOMHTMLFrameElement* self);

/**
 * webkit_dom_html_frame_element_set_src:
 * @self: A #CyberKitDOMHTMLFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_frame_element_set_src(CyberKitDOMHTMLFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_frame_element_get_content_document:
 * @self: A #CyberKitDOMHTMLFrameElement
 *
 * Returns: (transfer none): A #CyberKitDOMDocument
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMDocument*
webkit_dom_html_frame_element_get_content_document(CyberKitDOMHTMLFrameElement* self);

/**
 * webkit_dom_html_frame_element_get_content_window:
 * @self: A #CyberKitDOMHTMLFrameElement
 *
 * Returns: (transfer full): A #CyberKitDOMDOMWindow
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMDOMWindow*
webkit_dom_html_frame_element_get_content_window(CyberKitDOMHTMLFrameElement* self);

/**
 * webkit_dom_html_frame_element_get_width:
 * @self: A #CyberKitDOMHTMLFrameElement
 *
 * Returns: 0
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_frame_element_get_width(CyberKitDOMHTMLFrameElement* self);

/**
 * webkit_dom_html_frame_element_get_height:
 * @self: A #CyberKitDOMHTMLFrameElement
 *
 * Returns: 0
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_frame_element_get_height(CyberKitDOMHTMLFrameElement* self);

G_END_DECLS

#endif /* CyberKitDOMHTMLFrameElement_h */
