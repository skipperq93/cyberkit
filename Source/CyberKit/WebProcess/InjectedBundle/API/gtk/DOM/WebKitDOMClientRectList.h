/*
 *  Copyright (C) 2017 Aidan Holm <aidanholm@gmail.com>
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

#ifndef CyberKitDOMClientRectList_h
#define CyberKitDOMClientRectList_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMObject.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_CLIENT_RECT_LIST            (webkit_dom_client_rect_list_get_type())
#define WEBKIT_DOM_CLIENT_RECT_LIST(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_CLIENT_RECT_LIST, CyberKitDOMClientRectList))
#define WEBKIT_DOM_CLIENT_RECT_LIST_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_CLIENT_RECT_LIST, CyberKitDOMClientRectListClass)
#define WEBKIT_DOM_IS_CLIENT_RECT_LIST(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_CLIENT_RECT_LIST))
#define WEBKIT_DOM_IS_CLIENT_RECT_LIST_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_CLIENT_RECT_LIST))
#define WEBKIT_DOM_CLIENT_RECT_LIST_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_CLIENT_RECT_LIST, CyberKitDOMClientRectListClass))

struct _CyberKitDOMClientRectList {
    CyberKitDOMObject parent_instance;
};

struct _CyberKitDOMClientRectListClass {
    CyberKitDOMObjectClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_client_rect_list_get_type(void);

/**
 * webkit_dom_client_rect_list_get_length:
 * @self: A #CyberKitDOMClientRectList
 *
 * Returns the number of #CyberKitDOMClientRect objects that @self contains.
 *
 * Returns: A #gulong
 *
 * Since: 2.18
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gulong
webkit_dom_client_rect_list_get_length(CyberKitDOMClientRectList* self);

/**
 * webkit_dom_client_rect_list_item:
 * @self: A #CyberKitDOMClientRectList
 * @index: A #gulong
 *
 * Returns the #CyberKitDOMClientRect object that @self contains at @index.
 *
 * Returns: (transfer full): A #CyberKitDOMClientRect
 *
 * Since: 2.18
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMClientRect* webkit_dom_client_rect_list_item(CyberKitDOMClientRectList* self, gulong index);

G_END_DECLS

#endif /* CyberKitDOMClientRectList_h */
