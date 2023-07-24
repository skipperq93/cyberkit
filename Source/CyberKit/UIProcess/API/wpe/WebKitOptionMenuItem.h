/*
 * Copyright (C) 2020 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#if !defined(__WEBKIT_H_INSIDE__) && !defined(WEBKIT2_COMPILATION)
#error "Only <wpe/webkit.h> can be included directly."
#endif

#ifndef CyberKitOptionMenuItem_h
#define CyberKitOptionMenuItem_h

#include <glib-object.h>
#include <wpe/CyberKitDefines.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_OPTION_MENU_ITEM (webkit_option_menu_item_get_type())

typedef struct _CyberKitOptionMenuItem CyberKitOptionMenuItem;

WEBKIT_API GType
webkit_option_menu_item_get_type       (void);

WEBKIT_API CyberKitOptionMenuItem *
webkit_option_menu_item_copy           (CyberKitOptionMenuItem *item);

WEBKIT_API void
webkit_option_menu_item_free           (CyberKitOptionMenuItem *item);

WEBKIT_API const gchar *
webkit_option_menu_item_get_label      (CyberKitOptionMenuItem *item);

WEBKIT_API const gchar *
webkit_option_menu_item_get_tooltip    (CyberKitOptionMenuItem *item);

WEBKIT_API gboolean
webkit_option_menu_item_is_group_label (CyberKitOptionMenuItem *item);

WEBKIT_API gboolean
webkit_option_menu_item_is_group_child (CyberKitOptionMenuItem *item);

WEBKIT_API gboolean
webkit_option_menu_item_is_enabled     (CyberKitOptionMenuItem *item);

WEBKIT_API gboolean
webkit_option_menu_item_is_selected    (CyberKitOptionMenuItem *item);

G_END_DECLS

#endif
