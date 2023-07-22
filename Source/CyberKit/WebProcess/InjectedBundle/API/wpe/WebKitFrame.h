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
#error "Only <wpe/webkit-web-extension.h> can be included directly."
#endif

#ifndef CyberKitFrame_h
#define CyberKitFrame_h

#include <glib-object.h>
#include <jsc/jsc.h>
#include <wpe/CyberKitDefines.h>
#include <wpe/CyberKitScriptWorld.h>
#include <wpe/webkitdom.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_FRAME            (webkit_frame_get_type())
#define WEBKIT_FRAME(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_FRAME, CyberKitFrame))
#define WEBKIT_IS_FRAME(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_FRAME))
#define WEBKIT_FRAME_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_FRAME, CyberKitFrameClass))
#define WEBKIT_IS_FRAME_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_FRAME))
#define WEBKIT_FRAME_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_FRAME, CyberKitFrameClass))

typedef struct _CyberKitFrame        CyberKitFrame;
typedef struct _CyberKitFrameClass   CyberKitFrameClass;
typedef struct _CyberKitFramePrivate CyberKitFramePrivate;

struct _CyberKitFrame {
    GObject parent;

    CyberKitFramePrivate *priv;
};

struct _CyberKitFrameClass {
    GObjectClass parent_class;
};

WEBKIT_API GType
webkit_frame_get_type                                    (void);

WEBKIT_API guint64
webkit_frame_get_id                                      (CyberKitFrame       *frame);

WEBKIT_API gboolean
webkit_frame_is_main_frame                               (CyberKitFrame       *frame);

WEBKIT_API const gchar *
webkit_frame_get_uri                                     (CyberKitFrame       *frame);

WEBKIT_API JSCContext *
webkit_frame_get_js_context                              (CyberKitFrame       *frame);

WEBKIT_API JSCContext *
webkit_frame_get_js_context_for_script_world             (CyberKitFrame       *frame,
                                                          CyberKitScriptWorld *world);

WEBKIT_API JSCValue *
webkit_frame_get_js_value_for_dom_object                 (CyberKitFrame       *frame,
                                                          CyberKitDOMObject   *dom_object);

WEBKIT_API JSCValue *
webkit_frame_get_js_value_for_dom_object_in_script_world (CyberKitFrame       *frame,
                                                          CyberKitDOMObject   *dom_object,
                                                          CyberKitScriptWorld *world);

G_END_DECLS

#endif
