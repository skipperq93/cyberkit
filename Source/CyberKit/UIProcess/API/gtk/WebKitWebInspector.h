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

#if !defined(__WEBKIT2_H_INSIDE__) && !defined(WEBKIT2_COMPILATION)
#error "Only <webkit2/webkit2.h> can be included directly."
#endif

#ifndef CyberKitWebInspector_h
#define CyberKitWebInspector_h

#include <glib-object.h>
#include <webkit2/CyberKitDefines.h>
#include <webkit2/CyberKitWebViewBase.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_WEB_INSPECTOR            (webkit_web_inspector_get_type())
#define WEBKIT_WEB_INSPECTOR(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_WEB_INSPECTOR, CyberKitWebInspector))
#define WEBKIT_IS_WEB_INSPECTOR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_WEB_INSPECTOR))
#define WEBKIT_WEB_INSPECTOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_WEB_INSPECTOR, CyberKitWebInspectorClass))
#define WEBKIT_IS_WEB_INSPECTOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_WEB_INSPECTOR))
#define WEBKIT_WEB_INSPECTOR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_WEB_INSPECTOR, CyberKitWebInspectorClass))

typedef struct _CyberKitWebInspector        CyberKitWebInspector;
typedef struct _CyberKitWebInspectorClass   CyberKitWebInspectorClass;
typedef struct _CyberKitWebInspectorPrivate CyberKitWebInspectorPrivate;

struct _CyberKitWebInspector {
    GObject parent;

    CyberKitWebInspectorPrivate *priv;
};

struct _CyberKitWebInspectorClass {
    GObjectClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_web_inspector_get_type            (void);

WEBKIT_API CyberKitWebViewBase *
webkit_web_inspector_get_web_view        (CyberKitWebInspector *inspector);

WEBKIT_API const char *
webkit_web_inspector_get_inspected_uri   (CyberKitWebInspector *inspector);

WEBKIT_API gboolean
webkit_web_inspector_is_attached         (CyberKitWebInspector *inspector);

WEBKIT_API void
webkit_web_inspector_attach              (CyberKitWebInspector *inspector);

WEBKIT_API void
webkit_web_inspector_detach              (CyberKitWebInspector *inspector);

WEBKIT_API void
webkit_web_inspector_show                (CyberKitWebInspector *inspector);

WEBKIT_API void
webkit_web_inspector_close               (CyberKitWebInspector *inspector);

WEBKIT_API guint
webkit_web_inspector_get_attached_height (CyberKitWebInspector *inspector);

WEBKIT_API gboolean
webkit_web_inspector_get_can_attach      (CyberKitWebInspector* inspector);

G_END_DECLS

#endif
