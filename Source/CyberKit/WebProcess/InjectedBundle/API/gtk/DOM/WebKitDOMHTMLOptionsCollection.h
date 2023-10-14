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

#ifndef CyberKitDOMHTMLOptionsCollection_h
#define CyberKitDOMHTMLOptionsCollection_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLCollection.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_OPTIONS_COLLECTION            (webkit_dom_html_options_collection_get_type())
#define WEBKIT_DOM_HTML_OPTIONS_COLLECTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_OPTIONS_COLLECTION, CyberKitDOMHTMLOptionsCollection))
#define WEBKIT_DOM_HTML_OPTIONS_COLLECTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_OPTIONS_COLLECTION, CyberKitDOMHTMLOptionsCollectionClass)
#define WEBKIT_DOM_IS_HTML_OPTIONS_COLLECTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_OPTIONS_COLLECTION))
#define WEBKIT_DOM_IS_HTML_OPTIONS_COLLECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_OPTIONS_COLLECTION))
#define WEBKIT_DOM_HTML_OPTIONS_COLLECTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_OPTIONS_COLLECTION, CyberKitDOMHTMLOptionsCollectionClass))

struct _CyberKitDOMHTMLOptionsCollection {
    CyberKitDOMHTMLCollection parent_instance;
};

struct _CyberKitDOMHTMLOptionsCollectionClass {
    CyberKitDOMHTMLCollectionClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_options_collection_get_type(void);

/**
 * webkit_dom_html_options_collection_named_item:
 * @self: A #CyberKitDOMHTMLOptionsCollection
 * @name: A #gchar
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_html_options_collection_named_item(CyberKitDOMHTMLOptionsCollection* self, const gchar* name);

/**
 * webkit_dom_html_options_collection_get_selected_index:
 * @self: A #CyberKitDOMHTMLOptionsCollection
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_options_collection_get_selected_index(CyberKitDOMHTMLOptionsCollection* self);

/**
 * webkit_dom_html_options_collection_set_selected_index:
 * @self: A #CyberKitDOMHTMLOptionsCollection
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_options_collection_set_selected_index(CyberKitDOMHTMLOptionsCollection* self, glong value);

/**
 * webkit_dom_html_options_collection_get_length:
 * @self: A #CyberKitDOMHTMLOptionsCollection
 *
 * Returns: A #gulong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gulong
webkit_dom_html_options_collection_get_length(CyberKitDOMHTMLOptionsCollection* self);

G_END_DECLS

#endif /* CyberKitDOMHTMLOptionsCollection_h */
