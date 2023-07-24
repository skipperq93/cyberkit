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

#ifndef CyberKitOptionMenu_h
#define CyberKitOptionMenu_h

#include <glib-object.h>
#include <wpe/CyberKitDefines.h>
#include <wpe/CyberKitOptionMenuItem.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_OPTION_MENU            (webkit_option_menu_get_type())
#define WEBKIT_OPTION_MENU(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_OPTION_MENU, CyberKitOptionMenu))
#define WEBKIT_IS_OPTION_MENU(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_OPTION_MENU))
#define WEBKIT_OPTION_MENU_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_OPTION_MENU, CyberKitOptionMenuClass))
#define WEBKIT_IS_OPTION_MENU_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_OPTION_MENU))
#define WEBKIT_OPTION_MENU_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_OPTION_MENU, CyberKitOptionMenuClass))

typedef struct _CyberKitOptionMenu        CyberKitOptionMenu;
typedef struct _CyberKitOptionMenuClass   CyberKitOptionMenuClass;
typedef struct _CyberKitOptionMenuPrivate CyberKitOptionMenuPrivate;

struct _CyberKitOptionMenu {
    GObject parent;

    CyberKitOptionMenuPrivate *priv;
};

struct _CyberKitOptionMenuClass {
    GObjectClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_option_menu_get_type       (void);

WEBKIT_API guint
webkit_option_menu_get_n_items    (CyberKitOptionMenu *menu);

WEBKIT_API CyberKitOptionMenuItem *
webkit_option_menu_get_item       (CyberKitOptionMenu *menu,
                                   guint             index);

WEBKIT_API void
webkit_option_menu_select_item    (CyberKitOptionMenu *menu,
                                   guint             index);

WEBKIT_API void
webkit_option_menu_activate_item  (CyberKitOptionMenu *menu,
                                   guint             index);

WEBKIT_API void
webkit_option_menu_close          (CyberKitOptionMenu *menu);

G_END_DECLS

#endif
