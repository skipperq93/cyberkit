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

#if !defined(__WEBKIT_H_INSIDE__) && !defined(WEBKIT2_COMPILATION)
#error "Only <wpe/webkit.h> can be included directly."
#endif

#ifndef CyberKitInputMethodContext_h
#define CyberKitInputMethodContext_h

#include <glib-object.h>
#include <wpe/CyberKitDefines.h>
#include <wpe/CyberKitColor.h>
#include <wpe/wpe.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_INPUT_METHOD_CONTEXT            (webkit_input_method_context_get_type())
#define WEBKIT_INPUT_METHOD_CONTEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_INPUT_METHOD_CONTEXT, CyberKitInputMethodContext))
#define WEBKIT_INPUT_METHOD_CONTEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_INPUT_METHOD_CONTEXT, CyberKitInputMethodContextClass))
#define WEBKIT_IS_INPUT_METHOD_CONTEXT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_INPUT_METHOD_CONTEXT))
#define WEBKIT_IS_INPUT_METHOD_CONTEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_INPUT_METHOD_CONTEXT))
#define WEBKIT_INPUT_METHOD_CONTEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_INPUT_METHOD_CONTEXT, CyberKitInputMethodContextClass))

#define WEBKIT_TYPE_INPUT_METHOD_UNDERLINE          (webkit_input_method_underline_get_type())

typedef struct _CyberKitInputMethodContext        CyberKitInputMethodContext;
typedef struct _CyberKitInputMethodContextClass   CyberKitInputMethodContextClass;
typedef struct _CyberKitInputMethodContextPrivate CyberKitInputMethodContextPrivate;
typedef struct _CyberKitInputMethodUnderline      CyberKitInputMethodUnderline;

struct _CyberKitInputMethodContext {
    GObject parent;

    /*< private >*/
    CyberKitInputMethodContextPrivate *priv;
};

struct _CyberKitInputMethodContextClass {
    GObjectClass parent_class;

    /* Signals */
    void     (* preedit_started)    (CyberKitInputMethodContext        *context);
    void     (* preedit_changed)    (CyberKitInputMethodContext        *context);
    void     (* preedit_finished)   (CyberKitInputMethodContext        *context);
    void     (* committed)          (CyberKitInputMethodContext        *context,
                                     const char                      *text);

    /* Virtual functions */
    void     (* set_enable_preedit) (CyberKitInputMethodContext        *context,
                                     gboolean                         enabled);
    void     (* get_preedit)        (CyberKitInputMethodContext        *context,
                                     gchar                          **text,
                                     GList                          **underlines,
                                     guint                           *cursor_offset);
    gboolean (* filter_key_event)   (CyberKitInputMethodContext        *context,
                                     struct wpe_input_keyboard_event *key_event);
    void     (* notify_focus_in)    (CyberKitInputMethodContext        *context);
    void     (* notify_focus_out)   (CyberKitInputMethodContext        *context);
    void     (* notify_cursor_area) (CyberKitInputMethodContext        *context,
                                     int                              x,
                                     int                              y,
                                     int                              width,
                                     int                              height);
    void     (* reset)              (CyberKitInputMethodContext        *context);

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
    void (*_webkit_reserved4) (void);
    void (*_webkit_reserved5) (void);
    void (*_webkit_reserved6) (void);
    void (*_webkit_reserved7) (void);
};

WEBKIT_API GType
webkit_input_method_context_get_type           (void);

WEBKIT_API void
webkit_input_method_context_set_enable_preedit (CyberKitInputMethodContext        *context,
                                                gboolean                         enabled);

WEBKIT_API void
webkit_input_method_context_get_preedit        (CyberKitInputMethodContext        *context,
                                                char                           **text,
                                                GList                          **underlines,
                                                guint                           *cursor_offset);

WEBKIT_API gboolean
webkit_input_method_context_filter_key_event   (CyberKitInputMethodContext        *context,
                                                struct wpe_input_keyboard_event *key_event);

WEBKIT_API void
webkit_input_method_context_notify_focus_in    (CyberKitInputMethodContext        *context);

WEBKIT_API void
webkit_input_method_context_notify_focus_out   (CyberKitInputMethodContext        *context);

WEBKIT_API void
webkit_input_method_context_notify_cursor_area (CyberKitInputMethodContext        *context,
                                                int                              x,
                                                int                              y,
                                                int                              width,
                                                int                              height);

WEBKIT_API void
webkit_input_method_context_reset              (CyberKitInputMethodContext        *context);


WEBKIT_API GType
webkit_input_method_underline_get_type         (void);

WEBKIT_API CyberKitInputMethodUnderline *
webkit_input_method_underline_new              (guint                            start_offset,
                                                guint                            end_offset);

WEBKIT_API CyberKitInputMethodUnderline *
webkit_input_method_underline_copy             (CyberKitInputMethodUnderline      *underline);

WEBKIT_API void
webkit_input_method_underline_free             (CyberKitInputMethodUnderline      *underline);

WEBKIT_API void
webkit_input_method_underline_set_color        (CyberKitInputMethodUnderline      *underline,
                                                CyberKitColor                     *color);


G_END_DECLS

#endif
