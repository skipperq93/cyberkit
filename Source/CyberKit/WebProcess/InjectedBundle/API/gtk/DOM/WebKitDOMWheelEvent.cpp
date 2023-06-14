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

#include "config.h"
#include "CyberKitDOMWheelEvent.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/Document.h>
#include <CyberCore/ExceptionCode.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMDOMWindowPrivate.h"
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMPrivate.h"
#include "CyberKitDOMWheelEventPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMWheelEvent* kit(CyberCore::WheelEvent* obj)
{
    return WEBKIT_DOM_WHEEL_EVENT(kit(static_cast<CyberCore::Event*>(obj)));
}

CyberCore::WheelEvent* core(CyberKitDOMWheelEvent* request)
{
    return request ? static_cast<CyberCore::WheelEvent*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMWheelEvent* wrapWheelEvent(CyberCore::WheelEvent* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_WHEEL_EVENT(g_object_new(WEBKIT_DOM_TYPE_WHEEL_EVENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

G_DEFINE_TYPE(CyberKitDOMWheelEvent, webkit_dom_wheel_event, WEBKIT_DOM_TYPE_MOUSE_EVENT)

enum {
    DOM_WHEEL_EVENT_PROP_0,
    DOM_WHEEL_EVENT_PROP_WHEEL_DELTA_X,
    DOM_WHEEL_EVENT_PROP_WHEEL_DELTA_Y,
    DOM_WHEEL_EVENT_PROP_WHEEL_DELTA,
};

static void webkit_dom_wheel_event_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMWheelEvent* self = WEBKIT_DOM_WHEEL_EVENT(object);

    switch (propertyId) {
    case DOM_WHEEL_EVENT_PROP_WHEEL_DELTA_X:
        g_value_set_long(value, webkit_dom_wheel_event_get_wheel_delta_x(self));
        break;
    case DOM_WHEEL_EVENT_PROP_WHEEL_DELTA_Y:
        g_value_set_long(value, webkit_dom_wheel_event_get_wheel_delta_y(self));
        break;
    case DOM_WHEEL_EVENT_PROP_WHEEL_DELTA:
        g_value_set_long(value, webkit_dom_wheel_event_get_wheel_delta(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_wheel_event_class_init(CyberKitDOMWheelEventClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->get_property = webkit_dom_wheel_event_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_WHEEL_EVENT_PROP_WHEEL_DELTA_X,
        g_param_spec_long(
            "wheel-delta-x",
            "WheelEvent:wheel-delta-x",
            "read-only glong WheelEvent:wheel-delta-x",
            G_MINLONG, G_MAXLONG, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_WHEEL_EVENT_PROP_WHEEL_DELTA_Y,
        g_param_spec_long(
            "wheel-delta-y",
            "WheelEvent:wheel-delta-y",
            "read-only glong WheelEvent:wheel-delta-y",
            G_MINLONG, G_MAXLONG, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_WHEEL_EVENT_PROP_WHEEL_DELTA,
        g_param_spec_long(
            "wheel-delta",
            "WheelEvent:wheel-delta",
            "read-only glong WheelEvent:wheel-delta",
            G_MINLONG, G_MAXLONG, 0,
            WEBKIT_PARAM_READABLE));
}

static void webkit_dom_wheel_event_init(CyberKitDOMWheelEvent* request)
{
    UNUSED_PARAM(request);
}

void webkit_dom_wheel_event_init_wheel_event(CyberKitDOMWheelEvent* self, glong wheelDeltaX, glong wheelDeltaY, CyberKitDOMDOMWindow* view, glong screenX, glong screenY, glong clientX, glong clientY, gboolean ctrlKey, gboolean altKey, gboolean shiftKey, gboolean metaKey)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_WHEEL_EVENT(self));
    g_return_if_fail(WEBKIT_DOM_IS_DOM_WINDOW(view));
    CyberCore::WheelEvent* item = CyberKit::core(self);
    item->initCyberKitWheelEvent(wheelDeltaX, wheelDeltaY, CyberKit::toWindowProxy(view), screenX, screenY, clientX, clientY, ctrlKey, altKey, shiftKey, metaKey);
}

glong webkit_dom_wheel_event_get_wheel_delta_x(CyberKitDOMWheelEvent* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_WHEEL_EVENT(self), 0);
    CyberCore::WheelEvent* item = CyberKit::core(self);
    glong result = item->wheelDeltaX();
    return result;
}

glong webkit_dom_wheel_event_get_wheel_delta_y(CyberKitDOMWheelEvent* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_WHEEL_EVENT(self), 0);
    CyberCore::WheelEvent* item = CyberKit::core(self);
    glong result = item->wheelDeltaY();
    return result;
}

glong webkit_dom_wheel_event_get_wheel_delta(CyberKitDOMWheelEvent* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_WHEEL_EVENT(self), 0);
    CyberCore::WheelEvent* item = CyberKit::core(self);
    glong result = item->wheelDelta();
    return result;
}
G_GNUC_END_IGNORE_DEPRECATIONS;
