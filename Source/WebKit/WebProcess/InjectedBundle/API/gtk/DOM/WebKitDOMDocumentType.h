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

#ifndef CyberKitDOMDocumentType_h
#define CyberKitDOMDocumentType_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMNode.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_DOCUMENT_TYPE            (webkit_dom_document_type_get_type())
#define WEBKIT_DOM_DOCUMENT_TYPE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_DOCUMENT_TYPE, CyberKitDOMDocumentType))
#define WEBKIT_DOM_DOCUMENT_TYPE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_DOCUMENT_TYPE, CyberKitDOMDocumentTypeClass)
#define WEBKIT_DOM_IS_DOCUMENT_TYPE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_DOCUMENT_TYPE))
#define WEBKIT_DOM_IS_DOCUMENT_TYPE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_DOCUMENT_TYPE))
#define WEBKIT_DOM_DOCUMENT_TYPE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_DOCUMENT_TYPE, CyberKitDOMDocumentTypeClass))

struct _CyberKitDOMDocumentType {
    CyberKitDOMNode parent_instance;
};

struct _CyberKitDOMDocumentTypeClass {
    CyberKitDOMNodeClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_document_type_get_type(void);

/**
 * webkit_dom_document_type_get_name:
 * @self: A #CyberKitDOMDocumentType
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_type_get_name(CyberKitDOMDocumentType* self);

/**
 * webkit_dom_document_type_get_entities:
 * @self: A #CyberKitDOMDocumentType
 *
 * Returns: (transfer full): A #CyberKitDOMNamedNodeMap
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNamedNodeMap*
webkit_dom_document_type_get_entities(CyberKitDOMDocumentType* self);

/**
 * webkit_dom_document_type_get_notations:
 * @self: A #CyberKitDOMDocumentType
 *
 * Returns: (transfer full): A #CyberKitDOMNamedNodeMap
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNamedNodeMap*
webkit_dom_document_type_get_notations(CyberKitDOMDocumentType* self);

/**
 * webkit_dom_document_type_get_internal_subset:
 * @self: A #CyberKitDOMDocumentType
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_type_get_internal_subset(CyberKitDOMDocumentType* self);

/**
 * webkit_dom_document_type_get_public_id:
 * @self: A #CyberKitDOMDocumentType
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_type_get_public_id(CyberKitDOMDocumentType* self);

/**
 * webkit_dom_document_type_get_system_id:
 * @self: A #CyberKitDOMDocumentType
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_type_get_system_id(CyberKitDOMDocumentType* self);

G_END_DECLS

#endif /* CyberKitDOMDocumentType_h */
