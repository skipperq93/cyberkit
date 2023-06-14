/*
 * Copyright (C) 2012 Igalia S.L.
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

#ifndef CyberKitWebExtension_h
#define CyberKitWebExtension_h

#include <glib-object.h>
#include <webkit2/CyberKitDefines.h>
#include <webkit2/CyberKitWebPage.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_WEB_EXTENSION            (webkit_web_extension_get_type())
#define WEBKIT_WEB_EXTENSION(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_WEB_EXTENSION, CyberKitWebExtension))
#define WEBKIT_IS_WEB_EXTENSION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_WEB_EXTENSION))
#define WEBKIT_WEB_EXTENSION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_WEB_EXTENSION, CyberKitWebExtensionClass))
#define WEBKIT_IS_WEB_EXTENSION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_WEB_EXTENSION))
#define WEBKIT_WEB_EXTENSION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_WEB_EXTENSION, CyberKitWebExtensionClass))

typedef struct _CyberKitWebExtension        CyberKitWebExtension;
typedef struct _CyberKitWebExtensionClass   CyberKitWebExtensionClass;
typedef struct _CyberKitWebExtensionPrivate CyberKitWebExtensionPrivate;

/**
 * CyberKitWebExtensionInitializeFunction:
 * @extension: a #CyberKitWebExtension
 *
 * Type definition for a function that will be called to initialize
 * the web extension when the web process starts.
 */
typedef void (* CyberKitWebExtensionInitializeFunction) (CyberKitWebExtension *extension);

/**
 * CyberKitWebExtensionInitializeWithUserDataFunction:
 * @extension: a #CyberKitWebExtension
 * @user_data: a #GVariant
 *
 * Type definition for a function that will be called to initialize
 * the web extensions when the web process starts, and which receives
 * as additional argument the user data set with
 * webkit_web_context_set_web_extensions_initialization_user_data().
 *
 * Since: 2.4
 */
typedef void (* CyberKitWebExtensionInitializeWithUserDataFunction) (CyberKitWebExtension *extension,
                                                                   const GVariant     *user_data);

struct _CyberKitWebExtension {
    GObject parent;

    CyberKitWebExtensionPrivate *priv;
};

struct _CyberKitWebExtensionClass {
    GObjectClass parent_class;
};

WEBKIT_API GType
webkit_web_extension_get_type (void);

WEBKIT_API CyberKitWebPage *
webkit_web_extension_get_page (CyberKitWebExtension *extension,
                               guint64             page_id);

G_END_DECLS

#endif
