/*
 * Copyright (C) 2011 Igalia S.L.
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

#ifndef CyberKitWindowProperties_h
#define CyberKitWindowProperties_h

#include <glib-object.h>
#include <wpe/CyberKitDefines.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_WINDOW_PROPERTIES            (webkit_window_properties_get_type())
#define WEBKIT_WINDOW_PROPERTIES(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_WINDOW_PROPERTIES, CyberKitWindowProperties))
#define WEBKIT_IS_WINDOW_PROPERTIES(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_WINDOW_PROPERTIES))
#define WEBKIT_WINDOW_PROPERTIES_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_WINDOW_PROPERTIES, CyberKitWindowPropertiesClass))
#define WEBKIT_IS_WINDOW_PROPERTIES_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_WINDOW_PROPERTIES))
#define WEBKIT_WINDOW_PROPERTIES_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_WINDOW_PROPERTIES, CyberKitWindowPropertiesClass))

typedef struct _CyberKitWindowProperties CyberKitWindowProperties;
typedef struct _CyberKitWindowPropertiesClass CyberKitWindowPropertiesClass;
typedef struct _CyberKitWindowPropertiesPrivate CyberKitWindowPropertiesPrivate;

struct _CyberKitWindowProperties {
    GObject parent;

    /*< private >*/
    CyberKitWindowPropertiesPrivate *priv;
};

struct _CyberKitWindowPropertiesClass {
    GObjectClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_window_properties_get_type                (void);

WEBKIT_API gboolean
webkit_window_properties_get_toolbar_visible     (CyberKitWindowProperties *window_properties);

WEBKIT_API gboolean
webkit_window_properties_get_statusbar_visible   (CyberKitWindowProperties *window_properties);

WEBKIT_API gboolean
webkit_window_properties_get_scrollbars_visible  (CyberKitWindowProperties *window_properties);

WEBKIT_API gboolean
webkit_window_properties_get_menubar_visible     (CyberKitWindowProperties *window_properties);

WEBKIT_API gboolean
webkit_window_properties_get_locationbar_visible (CyberKitWindowProperties *window_properties);

WEBKIT_API gboolean
webkit_window_properties_get_resizable           (CyberKitWindowProperties *window_properties);

WEBKIT_API gboolean
webkit_window_properties_get_fullscreen          (CyberKitWindowProperties *window_properties);

G_END_DECLS

#endif
