/*
 * Copyright (C) 2012 Igalia S.L.
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

#if !defined(__WEBKIT_H_INSIDE__) && !defined(WEBKIT2_COMPILATION) && !defined(__WEBKIT_WEB_EXTENSION_H_INSIDE__)
#error "Only <wpe/webkit.h> can be included directly."
#endif

#ifndef CyberKitContextMenuItem_h
#define CyberKitContextMenuItem_h

#include <gio/gio.h>
#include <wpe/CyberKitDefines.h>
#include <wpe/CyberKitContextMenu.h>
#include <wpe/CyberKitContextMenuActions.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_CONTEXT_MENU_ITEM            (webkit_context_menu_item_get_type())
#define WEBKIT_CONTEXT_MENU_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_CONTEXT_MENU_ITEM, CyberKitContextMenuItem))
#define WEBKIT_IS_CONTEXT_MENU_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_CONTEXT_MENU_ITEM))
#define WEBKIT_CONTEXT_MENU_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_CONTEXT_MENU_ITEM, CyberKitContextMenuItemClass))
#define WEBKIT_IS_CONTEXT_MENU_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_CONTEXT_MENU_ITEM))
#define WEBKIT_CONTEXT_MENU_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_CONTEXT_MENU_ITEM, CyberKitContextMenuItemClass))

typedef struct _CyberKitContextMenuItem        CyberKitContextMenuItem;
typedef struct _CyberKitContextMenuItemClass   CyberKitContextMenuItemClass;
typedef struct _CyberKitContextMenuItemPrivate CyberKitContextMenuItemPrivate;

struct _CyberKitContextMenuItem {
    GInitiallyUnowned parent;

    CyberKitContextMenuItemPrivate *priv;
};

struct _CyberKitContextMenuItemClass {
    GInitiallyUnownedClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_context_menu_item_get_type                         (void);

WEBKIT_API CyberKitContextMenuItem *
webkit_context_menu_item_new_from_gaction                 (GAction                *action,
                                                           const gchar            *label,
                                                           GVariant               *target);

WEBKIT_API CyberKitContextMenuItem *
webkit_context_menu_item_new_from_stock_action            (CyberKitContextMenuAction action);

WEBKIT_API CyberKitContextMenuItem *
webkit_context_menu_item_new_from_stock_action_with_label (CyberKitContextMenuAction action,
                                                           const gchar            *label);

WEBKIT_API CyberKitContextMenuItem *
webkit_context_menu_item_new_with_submenu                 (const gchar            *label,
                                                           CyberKitContextMenu      *submenu);

WEBKIT_API CyberKitContextMenuItem *
webkit_context_menu_item_new_separator                    (void);

WEBKIT_API GAction *
webkit_context_menu_item_get_gaction                      (CyberKitContextMenuItem  *item);

WEBKIT_API CyberKitContextMenuAction
webkit_context_menu_item_get_stock_action                 (CyberKitContextMenuItem  *item);

WEBKIT_API gboolean
webkit_context_menu_item_is_separator                     (CyberKitContextMenuItem  *item);

WEBKIT_API void
webkit_context_menu_item_set_submenu                      (CyberKitContextMenuItem  *item,
                                                           CyberKitContextMenu      *submenu);

WEBKIT_API CyberKitContextMenu *
webkit_context_menu_item_get_submenu                      (CyberKitContextMenuItem  *item);

G_END_DECLS

#endif
