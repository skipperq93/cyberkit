/*
 * Copyright (C) 2013 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2,1 of the License, or (at your option) any later version.
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

#if !defined(__WEBKIT_WEB_EXTENSION_H_INSIDE__) && !defined(WEBKIT2_COMPILATION)
#error "Only <webkit2/webkit-web-extension.h> can be included directly."
#endif

#ifndef CyberKitScriptWorld_h
#define CyberKitScriptWorld_h

#include <glib-object.h>
#include <webkit2/CyberKitDefines.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_SCRIPT_WORLD            (webkit_script_world_get_type())
#define WEBKIT_SCRIPT_WORLD(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_SCRIPT_WORLD, CyberKitScriptWorld))
#define WEBKIT_IS_SCRIPT_WORLD(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_SCRIPT_WORLD))
#define WEBKIT_SCRIPT_WORLD_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_SCRIPT_WORLD, CyberKitScriptWorldClass))
#define WEBKIT_IS_SCRIPT_WORLD_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_SCRIPT_WORLD))
#define WEBKIT_SCRIPT_WORLD_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_SCRIPT_WORLD, CyberKitScriptWorldClass))

typedef struct _CyberKitScriptWorld        CyberKitScriptWorld;
typedef struct _CyberKitScriptWorldClass   CyberKitScriptWorldClass;
typedef struct _CyberKitScriptWorldPrivate CyberKitScriptWorldPrivate;

struct _CyberKitScriptWorld {
    GObject parent;

    CyberKitScriptWorldPrivate *priv;
};

struct _CyberKitScriptWorldClass {
    GObjectClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_script_world_get_type      (void);

WEBKIT_API CyberKitScriptWorld *
webkit_script_world_get_default   (void);

WEBKIT_API CyberKitScriptWorld *
webkit_script_world_new           (void);

WEBKIT_API CyberKitScriptWorld *
webkit_script_world_new_with_name (const char        *name);

WEBKIT_API const char *
webkit_script_world_get_name      (CyberKitScriptWorld *world);

G_END_DECLS

#endif
