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

#ifndef CyberKitDOMHTMLDocument_h
#define CyberKitDOMHTMLDocument_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMDocument.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_DOCUMENT            (webkit_dom_html_document_get_type())
#define WEBKIT_DOM_HTML_DOCUMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_DOCUMENT, CyberKitDOMHTMLDocument))
#define WEBKIT_DOM_HTML_DOCUMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_DOCUMENT, CyberKitDOMHTMLDocumentClass)
#define WEBKIT_DOM_IS_HTML_DOCUMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_DOCUMENT))
#define WEBKIT_DOM_IS_HTML_DOCUMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_DOCUMENT))
#define WEBKIT_DOM_HTML_DOCUMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_DOCUMENT, CyberKitDOMHTMLDocumentClass))

struct _CyberKitDOMHTMLDocument {
    CyberKitDOMDocument parent_instance;
};

struct _CyberKitDOMHTMLDocumentClass {
    CyberKitDOMDocumentClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_document_get_type(void);

/**
 * webkit_dom_html_document_close:
 * @self: A #CyberKitDOMHTMLDocument
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_document_close(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_clear:
 * @self: A #CyberKitDOMHTMLDocument
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_document_clear(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_capture_events:
 * @self: A #CyberKitDOMHTMLDocument
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_document_capture_events(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_release_events:
 * @self: A #CyberKitDOMHTMLDocument
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_document_release_events(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_get_width:
 * @self: A #CyberKitDOMHTMLDocument
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_document_get_width(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_get_height:
 * @self: A #CyberKitDOMHTMLDocument
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_document_get_height(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_get_dir:
 * @self: A #CyberKitDOMHTMLDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_document_get_dir(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_set_dir:
 * @self: A #CyberKitDOMHTMLDocument
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_document_set_dir(CyberKitDOMHTMLDocument* self, const gchar* value);

/**
 * webkit_dom_html_document_get_bg_color:
 * @self: A #CyberKitDOMHTMLDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_document_get_bg_color(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_set_bg_color:
 * @self: A #CyberKitDOMHTMLDocument
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_document_set_bg_color(CyberKitDOMHTMLDocument* self, const gchar* value);

/**
 * webkit_dom_html_document_get_fg_color:
 * @self: A #CyberKitDOMHTMLDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_document_get_fg_color(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_set_fg_color:
 * @self: A #CyberKitDOMHTMLDocument
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_document_set_fg_color(CyberKitDOMHTMLDocument* self, const gchar* value);

/**
 * webkit_dom_html_document_get_alink_color:
 * @self: A #CyberKitDOMHTMLDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_document_get_alink_color(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_set_alink_color:
 * @self: A #CyberKitDOMHTMLDocument
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_document_set_alink_color(CyberKitDOMHTMLDocument* self, const gchar* value);

/**
 * webkit_dom_html_document_get_link_color:
 * @self: A #CyberKitDOMHTMLDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_document_get_link_color(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_set_link_color:
 * @self: A #CyberKitDOMHTMLDocument
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_document_set_link_color(CyberKitDOMHTMLDocument* self, const gchar* value);

/**
 * webkit_dom_html_document_get_vlink_color:
 * @self: A #CyberKitDOMHTMLDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_document_get_vlink_color(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_set_vlink_color:
 * @self: A #CyberKitDOMHTMLDocument
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_document_set_vlink_color(CyberKitDOMHTMLDocument* self, const gchar* value);

G_END_DECLS

#endif /* CyberKitDOMHTMLDocument_h */
