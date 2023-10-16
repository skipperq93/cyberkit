/*
 *  This file is part of the CyberKit open source project.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#if !defined(__WEBKITDOM_H_INSIDE__) && !defined(BUILDING_WEBKIT)
#error "Only <webkitdom/webkitdom.h> can be included directly."
#endif

#ifndef CyberKitDOMDOMWindow_h
#define CyberKitDOMDOMWindow_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMObject.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_DOM_WINDOW            (webkit_dom_dom_window_get_type())
#define WEBKIT_DOM_DOM_WINDOW(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_DOM_WINDOW, CyberKitDOMDOMWindow))
#define WEBKIT_DOM_DOM_WINDOW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_DOM_WINDOW, CyberKitDOMDOMWindowClass)
#define WEBKIT_DOM_IS_DOM_WINDOW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_DOM_WINDOW))
#define WEBKIT_DOM_IS_DOM_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_DOM_WINDOW))
#define WEBKIT_DOM_DOM_WINDOW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_DOM_WINDOW, CyberKitDOMDOMWindowClass))

struct _CyberKitDOMDOMWindow {
    CyberKitDOMObject parent_instance;
};

struct _CyberKitDOMDOMWindowClass {
    CyberKitDOMObjectClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_dom_window_get_type(void);

/**
 * webkit_dom_dom_window_get_selection:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: (transfer full): A #CyberKitDOMDOMSelection
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED CyberKitDOMDOMSelection*
webkit_dom_dom_window_get_selection(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_focus:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_focus(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_blur:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_blur(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_close:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_close(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_print:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_print(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_stop:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_stop(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_alert:
 * @self: A #CyberKitDOMDOMWindow
 * @message: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_alert(CyberKitDOMDOMWindow* self, const gchar* message);

/**
 * webkit_dom_dom_window_confirm:
 * @self: A #CyberKitDOMDOMWindow
 * @message: A #gchar
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED gboolean
webkit_dom_dom_window_confirm(CyberKitDOMDOMWindow* self, const gchar* message);

/**
 * webkit_dom_dom_window_prompt:
 * @self: A #CyberKitDOMDOMWindow
 * @message: A #gchar
 * @defaultValue: A #gchar
 *
 * Returns: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED gchar*
webkit_dom_dom_window_prompt(CyberKitDOMDOMWindow* self, const gchar* message, const gchar* defaultValue);

/**
 * webkit_dom_dom_window_find:
 * @self: A #CyberKitDOMDOMWindow
 * @string: A #gchar
 * @caseSensitive: A #gboolean
 * @backwards: A #gboolean
 * @wrap: A #gboolean
 * @wholeWord: A #gboolean
 * @searchInFrames: A #gboolean
 * @showDialog: A #gboolean
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED gboolean
webkit_dom_dom_window_find(CyberKitDOMDOMWindow* self, const gchar* string, gboolean caseSensitive, gboolean backwards, gboolean wrap, gboolean wholeWord, gboolean searchInFrames, gboolean showDialog);

/**
 * webkit_dom_dom_window_scroll_by:
 * @self: A #CyberKitDOMDOMWindow
 * @x: A #gdouble
 * @y: A #gdouble
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_scroll_by(CyberKitDOMDOMWindow* self, gdouble x, gdouble y);

/**
 * webkit_dom_dom_window_scroll_to:
 * @self: A #CyberKitDOMDOMWindow
 * @x: A #gdouble
 * @y: A #gdouble
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_scroll_to(CyberKitDOMDOMWindow* self, gdouble x, gdouble y);

/**
 * webkit_dom_dom_window_move_by:
 * @self: A #CyberKitDOMDOMWindow
 * @x: A #gfloat
 * @y: A #gfloat
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_move_by(CyberKitDOMDOMWindow* self, gfloat x, gfloat y);

/**
 * webkit_dom_dom_window_move_to:
 * @self: A #CyberKitDOMDOMWindow
 * @x: A #gfloat
 * @y: A #gfloat
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_move_to(CyberKitDOMDOMWindow* self, gfloat x, gfloat y);

/**
 * webkit_dom_dom_window_resize_by:
 * @self: A #CyberKitDOMDOMWindow
 * @x: A #gfloat
 * @y: A #gfloat
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_resize_by(CyberKitDOMDOMWindow* self, gfloat x, gfloat y);

/**
 * webkit_dom_dom_window_resize_to:
 * @self: A #CyberKitDOMDOMWindow
 * @width: A #gfloat
 * @height: A #gfloat
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_resize_to(CyberKitDOMDOMWindow* self, gfloat width, gfloat height);

/**
 * webkit_dom_dom_window_get_computed_style:
 * @self: A #CyberKitDOMDOMWindow
 * @element: A #CyberKitDOMElement
 * @pseudoElement: (allow-none): A #gchar
 *
 * Returns: (transfer full): A #CyberKitDOMCSSStyleDeclaration
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED CyberKitDOMCSSStyleDeclaration*
webkit_dom_dom_window_get_computed_style(CyberKitDOMDOMWindow* self, CyberKitDOMElement* element, const gchar* pseudoElement);

/**
 * webkit_dom_dom_window_capture_events:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_capture_events(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_release_events:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_release_events(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_frame_element:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_dom_window_get_frame_element(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_offscreen_buffering:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED gboolean
webkit_dom_dom_window_get_offscreen_buffering(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_outer_height:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #glong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED glong
webkit_dom_dom_window_get_outer_height(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_outer_width:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #glong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED glong
webkit_dom_dom_window_get_outer_width(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_inner_height:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #glong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED glong
webkit_dom_dom_window_get_inner_height(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_inner_width:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #glong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED glong
webkit_dom_dom_window_get_inner_width(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_screen_x:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #glong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED glong
webkit_dom_dom_window_get_screen_x(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_screen_y:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #glong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED glong
webkit_dom_dom_window_get_screen_y(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_screen_left:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #glong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED glong
webkit_dom_dom_window_get_screen_left(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_screen_top:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #glong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED glong
webkit_dom_dom_window_get_screen_top(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_scroll_x:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #glong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED glong
webkit_dom_dom_window_get_scroll_x(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_scroll_y:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #glong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED glong
webkit_dom_dom_window_get_scroll_y(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_page_x_offset:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #glong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED glong
webkit_dom_dom_window_get_page_x_offset(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_page_y_offset:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #glong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED glong
webkit_dom_dom_window_get_page_y_offset(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_closed:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED gboolean
webkit_dom_dom_window_get_closed(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_length:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #gulong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED gulong
webkit_dom_dom_window_get_length(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_name:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED gchar*
webkit_dom_dom_window_get_name(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_set_name:
 * @self: A #CyberKitDOMDOMWindow
 * @value: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_set_name(CyberKitDOMDOMWindow* self, const gchar* value);

/**
 * webkit_dom_dom_window_get_status:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED gchar*
webkit_dom_dom_window_get_status(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_set_status:
 * @self: A #CyberKitDOMDOMWindow
 * @value: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_set_status(CyberKitDOMDOMWindow* self, const gchar* value);

/**
 * webkit_dom_dom_window_get_default_status:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED gchar*
webkit_dom_dom_window_get_default_status(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_set_default_status:
 * @self: A #CyberKitDOMDOMWindow
 * @value: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED void
webkit_dom_dom_window_set_default_status(CyberKitDOMDOMWindow* self, const gchar* value);

/**
 * webkit_dom_dom_window_get_self:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: (transfer full): A #CyberKitDOMDOMWindow
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED CyberKitDOMDOMWindow*
webkit_dom_dom_window_get_self(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_window:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: (transfer full): A #CyberKitDOMDOMWindow
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED CyberKitDOMDOMWindow*
webkit_dom_dom_window_get_window(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_frames:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: (transfer full): A #CyberKitDOMDOMWindow
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED CyberKitDOMDOMWindow*
webkit_dom_dom_window_get_frames(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_opener:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: (transfer full): A #CyberKitDOMDOMWindow
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED CyberKitDOMDOMWindow*
webkit_dom_dom_window_get_opener(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_parent:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: (transfer full): A #CyberKitDOMDOMWindow
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED CyberKitDOMDOMWindow*
webkit_dom_dom_window_get_parent(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_top:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: (transfer full): A #CyberKitDOMDOMWindow
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED CyberKitDOMDOMWindow*
webkit_dom_dom_window_get_top(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_document:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: (transfer none): A #CyberKitDOMDocument
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED CyberKitDOMDocument*
webkit_dom_dom_window_get_document(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_device_pixel_ratio:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #gdouble
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED gdouble
webkit_dom_dom_window_get_device_pixel_ratio(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_get_orientation:
 * @self: A #CyberKitDOMDOMWindow
 *
 * Returns: A #glong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED glong
webkit_dom_dom_window_get_orientation(CyberKitDOMDOMWindow* self);

/**
 * webkit_dom_dom_window_post_user_message:
 * @window: A #CyberKitDOMDOMWindow
 * @handler: Name of the user message handler.
 * @message: JavaScript value to be sent.
 *
 * Returns: Whether the message was successfully sent.
 *
 * Since: 2.8
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
 */
WEBKIT_DEPRECATED gboolean
webkit_dom_dom_window_webkit_message_handlers_post_message(CyberKitDOMDOMWindow* window, const gchar* handler, const gchar* message);

G_END_DECLS

#endif /* CyberKitDOMDOMWindow_h */
