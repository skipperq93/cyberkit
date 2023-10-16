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

#ifndef CyberKitDOMMediaList_h
#define CyberKitDOMMediaList_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMObject.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_MEDIA_LIST            (webkit_dom_media_list_get_type())
#define WEBKIT_DOM_MEDIA_LIST(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_MEDIA_LIST, CyberKitDOMMediaList))
#define WEBKIT_DOM_MEDIA_LIST_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_MEDIA_LIST, CyberKitDOMMediaListClass)
#define WEBKIT_DOM_IS_MEDIA_LIST(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_MEDIA_LIST))
#define WEBKIT_DOM_IS_MEDIA_LIST_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_MEDIA_LIST))
#define WEBKIT_DOM_MEDIA_LIST_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_MEDIA_LIST, CyberKitDOMMediaListClass))

struct _CyberKitDOMMediaList {
    CyberKitDOMObject parent_instance;
};

struct _CyberKitDOMMediaListClass {
    CyberKitDOMObjectClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_media_list_get_type(void);

/**
 * webkit_dom_media_list_item:
 * @self: A #CyberKitDOMMediaList
 * @index: A #gulong
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_media_list_item(CyberKitDOMMediaList* self, gulong index);

/**
 * webkit_dom_media_list_delete_medium:
 * @self: A #CyberKitDOMMediaList
 * @oldMedium: A #gchar
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_media_list_delete_medium(CyberKitDOMMediaList* self, const gchar* oldMedium, GError** error);

/**
 * webkit_dom_media_list_append_medium:
 * @self: A #CyberKitDOMMediaList
 * @newMedium: A #gchar
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_media_list_append_medium(CyberKitDOMMediaList* self, const gchar* newMedium, GError** error);

/**
 * webkit_dom_media_list_get_media_text:
 * @self: A #CyberKitDOMMediaList
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_media_list_get_media_text(CyberKitDOMMediaList* self);

/**
 * webkit_dom_media_list_set_media_text:
 * @self: A #CyberKitDOMMediaList
 * @value: A #gchar
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_media_list_set_media_text(CyberKitDOMMediaList* self, const gchar* value, GError** error);

/**
 * webkit_dom_media_list_get_length:
 * @self: A #CyberKitDOMMediaList
 *
 * Returns: A #gulong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gulong
webkit_dom_media_list_get_length(CyberKitDOMMediaList* self);

G_END_DECLS

#endif /* CyberKitDOMMediaList_h */
