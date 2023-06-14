/*
 * Copyright (C) 2015 Red Hat Inc.
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
#error "Only <ape/webkit-web-extension.h> can be included directly."
#endif

#ifndef CyberKitWebEditor_h
#define CyberKitWebEditor_h

#include <glib-object.h>
#include <wpe/CyberKitDefines.h>
#include <wpe/CyberKitWebPage.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_WEB_EDITOR               (webkit_web_editor_get_type())
#define WEBKIT_WEB_EDITOR(obj)               (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_WEB_EDITOR, CyberKitWebEditor))
#define WEBKIT_IS_WEB_EDITOR(obj)            (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_WEB_EDITOR))
#define WEBKIT_WEB_EDITOR_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_WEB_EDITOR, CyberKitWebEditorClass))
#define WEBKIT_IS_WEB_EDITOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_WEB_EDITOR))
#define WEBKIT_WEB_EDITOR_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_WEB_EDITOR, CyberKitWebEditorClass))

typedef struct _CyberKitWebEditor        CyberKitWebEditor;
typedef struct _CyberKitWebEditorClass   CyberKitWebEditorClass;
typedef struct _CyberKitWebEditorPrivate CyberKitWebEditorPrivate;

/* Forward declarations */
typedef struct _CyberKitWebPage          CyberKitWebPage;

struct _CyberKitWebEditor {
    GObject parent;

    CyberKitWebEditorPrivate *priv;
};

struct _CyberKitWebEditorClass {
    GObjectClass parent_class;
};

WEBKIT_API GType
webkit_web_editor_get_type (void);

WEBKIT_API CyberKitWebPage *
webkit_web_editor_get_page (CyberKitWebEditor *editor);

G_END_DECLS

#endif // CyberKitWebEditor_h
