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

#ifndef CyberKitDOMBlob_h
#define CyberKitDOMBlob_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMObject.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_BLOB            (webkit_dom_blob_get_type())
#define WEBKIT_DOM_BLOB(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_BLOB, CyberKitDOMBlob))
#define WEBKIT_DOM_BLOB_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_BLOB, CyberKitDOMBlobClass)
#define WEBKIT_DOM_IS_BLOB(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_BLOB))
#define WEBKIT_DOM_IS_BLOB_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_BLOB))
#define WEBKIT_DOM_BLOB_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_BLOB, CyberKitDOMBlobClass))

struct _CyberKitDOMBlob {
    CyberKitDOMObject parent_instance;
};

struct _CyberKitDOMBlobClass {
    CyberKitDOMObjectClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_blob_get_type(void);

/**
 * webkit_dom_blob_get_size:
 * @self: A #CyberKitDOMBlob
 *
 * Returns: A #guint64
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED guint64
webkit_dom_blob_get_size(CyberKitDOMBlob* self);

G_END_DECLS

#endif /* CyberKitDOMBlob_h */
