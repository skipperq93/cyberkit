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

#ifndef CyberKitDOMHTMLCollection_h
#define CyberKitDOMHTMLCollection_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMObject.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_COLLECTION            (webkit_dom_html_collection_get_type())
#define WEBKIT_DOM_HTML_COLLECTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_COLLECTION, CyberKitDOMHTMLCollection))
#define WEBKIT_DOM_HTML_COLLECTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_COLLECTION, CyberKitDOMHTMLCollectionClass)
#define WEBKIT_DOM_IS_HTML_COLLECTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_COLLECTION))
#define WEBKIT_DOM_IS_HTML_COLLECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_COLLECTION))
#define WEBKIT_DOM_HTML_COLLECTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_COLLECTION, CyberKitDOMHTMLCollectionClass))

struct _CyberKitDOMHTMLCollection {
    CyberKitDOMObject parent_instance;
};

struct _CyberKitDOMHTMLCollectionClass {
    CyberKitDOMObjectClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_collection_get_type(void);

/**
 * webkit_dom_html_collection_item:
 * @self: A #CyberKitDOMHTMLCollection
 * @index: A #gulong
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_html_collection_item(CyberKitDOMHTMLCollection* self, gulong index);

/**
 * webkit_dom_html_collection_named_item:
 * @self: A #CyberKitDOMHTMLCollection
 * @name: A #gchar
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_html_collection_named_item(CyberKitDOMHTMLCollection* self, const gchar* name);

/**
 * webkit_dom_html_collection_get_length:
 * @self: A #CyberKitDOMHTMLCollection
 *
 * Returns: A #gulong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gulong
webkit_dom_html_collection_get_length(CyberKitDOMHTMLCollection* self);

G_END_DECLS

#endif /* CyberKitDOMHTMLCollection_h */
