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

#ifndef CyberKitDOMHTMLIFrameElement_h
#define CyberKitDOMHTMLIFrameElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_IFRAME_ELEMENT            (webkit_dom_html_iframe_element_get_type())
#define WEBKIT_DOM_HTML_IFRAME_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_IFRAME_ELEMENT, CyberKitDOMHTMLIFrameElement))
#define WEBKIT_DOM_HTML_IFRAME_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_IFRAME_ELEMENT, CyberKitDOMHTMLIFrameElementClass)
#define WEBKIT_DOM_IS_HTML_IFRAME_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_IFRAME_ELEMENT))
#define WEBKIT_DOM_IS_HTML_IFRAME_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_IFRAME_ELEMENT))
#define WEBKIT_DOM_HTML_IFRAME_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_IFRAME_ELEMENT, CyberKitDOMHTMLIFrameElementClass))

struct _CyberKitDOMHTMLIFrameElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLIFrameElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_iframe_element_get_type(void);

/**
 * webkit_dom_html_iframe_element_get_align:
 * @self: A #CyberKitDOMHTMLIFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_iframe_element_get_align(CyberKitDOMHTMLIFrameElement* self);

/**
 * webkit_dom_html_iframe_element_set_align:
 * @self: A #CyberKitDOMHTMLIFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_iframe_element_set_align(CyberKitDOMHTMLIFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_iframe_element_get_frame_border:
 * @self: A #CyberKitDOMHTMLIFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_iframe_element_get_frame_border(CyberKitDOMHTMLIFrameElement* self);

/**
 * webkit_dom_html_iframe_element_set_frame_border:
 * @self: A #CyberKitDOMHTMLIFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_iframe_element_set_frame_border(CyberKitDOMHTMLIFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_iframe_element_get_height:
 * @self: A #CyberKitDOMHTMLIFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_iframe_element_get_height(CyberKitDOMHTMLIFrameElement* self);

/**
 * webkit_dom_html_iframe_element_set_height:
 * @self: A #CyberKitDOMHTMLIFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_iframe_element_set_height(CyberKitDOMHTMLIFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_iframe_element_get_long_desc:
 * @self: A #CyberKitDOMHTMLIFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_iframe_element_get_long_desc(CyberKitDOMHTMLIFrameElement* self);

/**
 * webkit_dom_html_iframe_element_set_long_desc:
 * @self: A #CyberKitDOMHTMLIFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_iframe_element_set_long_desc(CyberKitDOMHTMLIFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_iframe_element_get_margin_height:
 * @self: A #CyberKitDOMHTMLIFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_iframe_element_get_margin_height(CyberKitDOMHTMLIFrameElement* self);

/**
 * webkit_dom_html_iframe_element_set_margin_height:
 * @self: A #CyberKitDOMHTMLIFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_iframe_element_set_margin_height(CyberKitDOMHTMLIFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_iframe_element_get_margin_width:
 * @self: A #CyberKitDOMHTMLIFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_iframe_element_get_margin_width(CyberKitDOMHTMLIFrameElement* self);

/**
 * webkit_dom_html_iframe_element_set_margin_width:
 * @self: A #CyberKitDOMHTMLIFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_iframe_element_set_margin_width(CyberKitDOMHTMLIFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_iframe_element_get_name:
 * @self: A #CyberKitDOMHTMLIFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_iframe_element_get_name(CyberKitDOMHTMLIFrameElement* self);

/**
 * webkit_dom_html_iframe_element_set_name:
 * @self: A #CyberKitDOMHTMLIFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_iframe_element_set_name(CyberKitDOMHTMLIFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_iframe_element_get_scrolling:
 * @self: A #CyberKitDOMHTMLIFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_iframe_element_get_scrolling(CyberKitDOMHTMLIFrameElement* self);

/**
 * webkit_dom_html_iframe_element_set_scrolling:
 * @self: A #CyberKitDOMHTMLIFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_iframe_element_set_scrolling(CyberKitDOMHTMLIFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_iframe_element_get_src:
 * @self: A #CyberKitDOMHTMLIFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_iframe_element_get_src(CyberKitDOMHTMLIFrameElement* self);

/**
 * webkit_dom_html_iframe_element_set_src:
 * @self: A #CyberKitDOMHTMLIFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_iframe_element_set_src(CyberKitDOMHTMLIFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_iframe_element_get_width:
 * @self: A #CyberKitDOMHTMLIFrameElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_iframe_element_get_width(CyberKitDOMHTMLIFrameElement* self);

/**
 * webkit_dom_html_iframe_element_set_width:
 * @self: A #CyberKitDOMHTMLIFrameElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_iframe_element_set_width(CyberKitDOMHTMLIFrameElement* self, const gchar* value);

/**
 * webkit_dom_html_iframe_element_get_content_document:
 * @self: A #CyberKitDOMHTMLIFrameElement
 *
 * Returns: (transfer none): A #CyberKitDOMDocument
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMDocument*
webkit_dom_html_iframe_element_get_content_document(CyberKitDOMHTMLIFrameElement* self);

/**
 * webkit_dom_html_iframe_element_get_content_window:
 * @self: A #CyberKitDOMHTMLIFrameElement
 *
 * Returns: (transfer full): A #CyberKitDOMDOMWindow
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMDOMWindow*
webkit_dom_html_iframe_element_get_content_window(CyberKitDOMHTMLIFrameElement* self);

G_END_DECLS

#endif /* CyberKitDOMHTMLIFrameElement_h */
