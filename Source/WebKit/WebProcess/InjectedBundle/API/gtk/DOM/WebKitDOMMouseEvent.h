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

#ifndef CyberKitDOMMouseEvent_h
#define CyberKitDOMMouseEvent_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMUIEvent.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_MOUSE_EVENT            (webkit_dom_mouse_event_get_type())
#define WEBKIT_DOM_MOUSE_EVENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_MOUSE_EVENT, CyberKitDOMMouseEvent))
#define WEBKIT_DOM_MOUSE_EVENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_MOUSE_EVENT, CyberKitDOMMouseEventClass)
#define WEBKIT_DOM_IS_MOUSE_EVENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_MOUSE_EVENT))
#define WEBKIT_DOM_IS_MOUSE_EVENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_MOUSE_EVENT))
#define WEBKIT_DOM_MOUSE_EVENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_MOUSE_EVENT, CyberKitDOMMouseEventClass))

struct _CyberKitDOMMouseEvent {
    CyberKitDOMUIEvent parent_instance;
};

struct _CyberKitDOMMouseEventClass {
    CyberKitDOMUIEventClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_mouse_event_get_type(void);

/**
 * webkit_dom_mouse_event_init_mouse_event:
 * @self: A #CyberKitDOMMouseEvent
 * @type: A #gchar
 * @canBubble: A #gboolean
 * @cancelable: A #gboolean
 * @view: A #CyberKitDOMDOMWindow
 * @detail: A #glong
 * @screenX: A #glong
 * @screenY: A #glong
 * @clientX: A #glong
 * @clientY: A #glong
 * @ctrlKey: A #gboolean
 * @altKey: A #gboolean
 * @shiftKey: A #gboolean
 * @metaKey: A #gboolean
 * @button: A #gushort
 * @relatedTarget: A #CyberKitDOMEventTarget
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_mouse_event_init_mouse_event(CyberKitDOMMouseEvent* self, const gchar* type, gboolean canBubble, gboolean cancelable, CyberKitDOMDOMWindow* view, glong detail, glong screenX, glong screenY, glong clientX, glong clientY, gboolean ctrlKey, gboolean altKey, gboolean shiftKey, gboolean metaKey, gushort button, CyberKitDOMEventTarget* relatedTarget);

/**
 * webkit_dom_mouse_event_get_screen_x:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_mouse_event_get_screen_x(CyberKitDOMMouseEvent* self);

/**
 * webkit_dom_mouse_event_get_screen_y:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_mouse_event_get_screen_y(CyberKitDOMMouseEvent* self);

/**
 * webkit_dom_mouse_event_get_client_x:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_mouse_event_get_client_x(CyberKitDOMMouseEvent* self);

/**
 * webkit_dom_mouse_event_get_client_y:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_mouse_event_get_client_y(CyberKitDOMMouseEvent* self);

/**
 * webkit_dom_mouse_event_get_ctrl_key:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_mouse_event_get_ctrl_key(CyberKitDOMMouseEvent* self);

/**
 * webkit_dom_mouse_event_get_shift_key:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_mouse_event_get_shift_key(CyberKitDOMMouseEvent* self);

/**
 * webkit_dom_mouse_event_get_alt_key:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_mouse_event_get_alt_key(CyberKitDOMMouseEvent* self);

/**
 * webkit_dom_mouse_event_get_meta_key:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_mouse_event_get_meta_key(CyberKitDOMMouseEvent* self);

/**
 * webkit_dom_mouse_event_get_button:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: A #gushort
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gushort
webkit_dom_mouse_event_get_button(CyberKitDOMMouseEvent* self);

/**
 * webkit_dom_mouse_event_get_related_target:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: (transfer full): A #CyberKitDOMEventTarget
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMEventTarget*
webkit_dom_mouse_event_get_related_target(CyberKitDOMMouseEvent* self);

/**
 * webkit_dom_mouse_event_get_offset_x:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_mouse_event_get_offset_x(CyberKitDOMMouseEvent* self);

/**
 * webkit_dom_mouse_event_get_offset_y:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_mouse_event_get_offset_y(CyberKitDOMMouseEvent* self);

/**
 * webkit_dom_mouse_event_get_x:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_mouse_event_get_x(CyberKitDOMMouseEvent* self);

/**
 * webkit_dom_mouse_event_get_y:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_mouse_event_get_y(CyberKitDOMMouseEvent* self);

/**
 * webkit_dom_mouse_event_get_from_element:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_mouse_event_get_from_element(CyberKitDOMMouseEvent* self);

/**
 * webkit_dom_mouse_event_get_to_element:
 * @self: A #CyberKitDOMMouseEvent
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_mouse_event_get_to_element(CyberKitDOMMouseEvent* self);

G_END_DECLS

#endif /* CyberKitDOMMouseEvent_h */
