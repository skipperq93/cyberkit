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

#ifndef CyberKitDOMWheelEvent_h
#define CyberKitDOMWheelEvent_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMMouseEvent.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_WHEEL_EVENT            (webkit_dom_wheel_event_get_type())
#define WEBKIT_DOM_WHEEL_EVENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_WHEEL_EVENT, CyberKitDOMWheelEvent))
#define WEBKIT_DOM_WHEEL_EVENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_WHEEL_EVENT, CyberKitDOMWheelEventClass)
#define WEBKIT_DOM_IS_WHEEL_EVENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_WHEEL_EVENT))
#define WEBKIT_DOM_IS_WHEEL_EVENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_WHEEL_EVENT))
#define WEBKIT_DOM_WHEEL_EVENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_WHEEL_EVENT, CyberKitDOMWheelEventClass))

struct _CyberKitDOMWheelEvent {
    CyberKitDOMMouseEvent parent_instance;
};

struct _CyberKitDOMWheelEventClass {
    CyberKitDOMMouseEventClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_wheel_event_get_type(void);

/**
 * webkit_dom_wheel_event_init_wheel_event:
 * @self: A #CyberKitDOMWheelEvent
 * @wheelDeltaX: A #glong
 * @wheelDeltaY: A #glong
 * @view: A #CyberKitDOMDOMWindow
 * @screenX: A #glong
 * @screenY: A #glong
 * @clientX: A #glong
 * @clientY: A #glong
 * @ctrlKey: A #gboolean
 * @altKey: A #gboolean
 * @shiftKey: A #gboolean
 * @metaKey: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_wheel_event_init_wheel_event(CyberKitDOMWheelEvent* self, glong wheelDeltaX, glong wheelDeltaY, CyberKitDOMDOMWindow* view, glong screenX, glong screenY, glong clientX, glong clientY, gboolean ctrlKey, gboolean altKey, gboolean shiftKey, gboolean metaKey);

/**
 * webkit_dom_wheel_event_get_wheel_delta_x:
 * @self: A #CyberKitDOMWheelEvent
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_wheel_event_get_wheel_delta_x(CyberKitDOMWheelEvent* self);

/**
 * webkit_dom_wheel_event_get_wheel_delta_y:
 * @self: A #CyberKitDOMWheelEvent
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_wheel_event_get_wheel_delta_y(CyberKitDOMWheelEvent* self);

/**
 * webkit_dom_wheel_event_get_wheel_delta:
 * @self: A #CyberKitDOMWheelEvent
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_wheel_event_get_wheel_delta(CyberKitDOMWheelEvent* self);

G_END_DECLS

#endif /* CyberKitDOMWheelEvent_h */
