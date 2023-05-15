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
#include "CyberKitDOMEvent.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/Document.h>
#include <CyberCore/ExceptionCode.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTargetPrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_EVENT_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_DOM_TYPE_EVENT, CyberKitDOMEventPrivate)

typedef struct _CyberKitDOMEventPrivate {
    RefPtr<CyberCore::Event> coreObject;
} CyberKitDOMEventPrivate;

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMEvent* kit(CyberCore::Event* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_EVENT(ret);

    return wrap(obj);
}

CyberCore::Event* core(CyberKitDOMEvent* request)
{
    return request ? static_cast<CyberCore::Event*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMEvent* wrapEvent(CyberCore::Event* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_EVENT(g_object_new(WEBKIT_DOM_TYPE_EVENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

G_DEFINE_TYPE(CyberKitDOMEvent, webkit_dom_event, WEBKIT_DOM_TYPE_OBJECT)

enum {
    DOM_EVENT_PROP_0,
    DOM_EVENT_PROP_TYPE,
    DOM_EVENT_PROP_TARGET,
    DOM_EVENT_PROP_CURRENT_TARGET,
    DOM_EVENT_PROP_EVENT_PHASE,
    DOM_EVENT_PROP_BUBBLES,
    DOM_EVENT_PROP_CANCELABLE,
    DOM_EVENT_PROP_TIME_STAMP,
    DOM_EVENT_PROP_SRC_ELEMENT,
    DOM_EVENT_PROP_RETURN_VALUE,
    DOM_EVENT_PROP_CANCEL_BUBBLE,
};

static void webkit_dom_event_finalize(GObject* object)
{
    CyberKitDOMEventPrivate* priv = WEBKIT_DOM_EVENT_GET_PRIVATE(object);

    CyberKit::DOMObjectCache::forget(priv->coreObject.get());

    priv->~CyberKitDOMEventPrivate();
    G_OBJECT_CLASS(webkit_dom_event_parent_class)->finalize(object);
}

static void webkit_dom_event_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMEvent* self = WEBKIT_DOM_EVENT(object);

    switch (propertyId) {
    case DOM_EVENT_PROP_RETURN_VALUE:
        webkit_dom_event_set_return_value(self, g_value_get_boolean(value));
        break;
    case DOM_EVENT_PROP_CANCEL_BUBBLE:
        webkit_dom_event_set_cancel_bubble(self, g_value_get_boolean(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_event_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMEvent* self = WEBKIT_DOM_EVENT(object);

    switch (propertyId) {
    case DOM_EVENT_PROP_TYPE:
        g_value_take_string(value, webkit_dom_event_get_event_type(self));
        break;
    case DOM_EVENT_PROP_TARGET:
        g_value_set_object(value, webkit_dom_event_get_target(self));
        break;
    case DOM_EVENT_PROP_CURRENT_TARGET:
        g_value_set_object(value, webkit_dom_event_get_current_target(self));
        break;
    case DOM_EVENT_PROP_EVENT_PHASE:
        g_value_set_uint(value, webkit_dom_event_get_event_phase(self));
        break;
    case DOM_EVENT_PROP_BUBBLES:
        g_value_set_boolean(value, webkit_dom_event_get_bubbles(self));
        break;
    case DOM_EVENT_PROP_CANCELABLE:
        g_value_set_boolean(value, webkit_dom_event_get_cancelable(self));
        break;
    case DOM_EVENT_PROP_TIME_STAMP:
        g_value_set_uint(value, webkit_dom_event_get_time_stamp(self));
        break;
    case DOM_EVENT_PROP_SRC_ELEMENT:
        g_value_set_object(value, webkit_dom_event_get_src_element(self));
        break;
    case DOM_EVENT_PROP_RETURN_VALUE:
        g_value_set_boolean(value, webkit_dom_event_get_return_value(self));
        break;
    case DOM_EVENT_PROP_CANCEL_BUBBLE:
        g_value_set_boolean(value, webkit_dom_event_get_cancel_bubble(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static GObject* webkit_dom_event_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_event_parent_class)->constructor(type, constructPropertiesCount, constructProperties);

    CyberKitDOMEventPrivate* priv = WEBKIT_DOM_EVENT_GET_PRIVATE(object);
    priv->coreObject = static_cast<CyberCore::Event*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    CyberKit::DOMObjectCache::put(priv->coreObject.get(), object);

    return object;
}

static void webkit_dom_event_class_init(CyberKitDOMEventClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(CyberKitDOMEventPrivate));
    gobjectClass->constructor = webkit_dom_event_constructor;
    gobjectClass->finalize = webkit_dom_event_finalize;
    gobjectClass->set_property = webkit_dom_event_set_property;
    gobjectClass->get_property = webkit_dom_event_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_EVENT_PROP_TYPE,
        g_param_spec_string(
            "type",
            "Event:type",
            "read-only gchar* Event:type",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_EVENT_PROP_TARGET,
        g_param_spec_object(
            "target",
            "Event:target",
            "read-only CyberKitDOMEventTarget* Event:target",
            WEBKIT_DOM_TYPE_EVENT_TARGET,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_EVENT_PROP_CURRENT_TARGET,
        g_param_spec_object(
            "current-target",
            "Event:current-target",
            "read-only CyberKitDOMEventTarget* Event:current-target",
            WEBKIT_DOM_TYPE_EVENT_TARGET,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_EVENT_PROP_EVENT_PHASE,
        g_param_spec_uint(
            "event-phase",
            "Event:event-phase",
            "read-only gushort Event:event-phase",
            0, G_MAXUINT, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_EVENT_PROP_BUBBLES,
        g_param_spec_boolean(
            "bubbles",
            "Event:bubbles",
            "read-only gboolean Event:bubbles",
            FALSE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_EVENT_PROP_CANCELABLE,
        g_param_spec_boolean(
            "cancelable",
            "Event:cancelable",
            "read-only gboolean Event:cancelable",
            FALSE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_EVENT_PROP_TIME_STAMP,
        g_param_spec_uint(
            "time-stamp",
            "Event:time-stamp",
            "read-only guint32 Event:time-stamp",
            0, G_MAXUINT, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_EVENT_PROP_SRC_ELEMENT,
        g_param_spec_object(
            "src-element",
            "Event:src-element",
            "read-only CyberKitDOMEventTarget* Event:src-element",
            WEBKIT_DOM_TYPE_EVENT_TARGET,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_EVENT_PROP_RETURN_VALUE,
        g_param_spec_boolean(
            "return-value",
            "Event:return-value",
            "read-write gboolean Event:return-value",
            FALSE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_EVENT_PROP_CANCEL_BUBBLE,
        g_param_spec_boolean(
            "cancel-bubble",
            "Event:cancel-bubble",
            "read-write gboolean Event:cancel-bubble",
            FALSE,
            WEBKIT_PARAM_READWRITE));

}

static void webkit_dom_event_init(CyberKitDOMEvent* request)
{
    CyberKitDOMEventPrivate* priv = WEBKIT_DOM_EVENT_GET_PRIVATE(request);
    new (priv) CyberKitDOMEventPrivate();
}

void webkit_dom_event_stop_propagation(CyberKitDOMEvent* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_EVENT(self));
    CyberCore::Event* item = CyberKit::core(self);
    item->stopPropagation();
}

void webkit_dom_event_prevent_default(CyberKitDOMEvent* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_EVENT(self));
    CyberCore::Event* item = CyberKit::core(self);
    item->preventDefault();
}

void webkit_dom_event_init_event(CyberKitDOMEvent* self, const gchar* eventTypeArg, gboolean canBubbleArg, gboolean cancelableArg)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_EVENT(self));
    g_return_if_fail(eventTypeArg);
    CyberCore::Event* item = CyberKit::core(self);
    item->initEvent(WTF::AtomString::fromUTF8(eventTypeArg), canBubbleArg, cancelableArg);
}

gchar* webkit_dom_event_get_event_type(CyberKitDOMEvent* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_EVENT(self), 0);
    CyberCore::Event* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->type());
    return result;
}

CyberKitDOMEventTarget* webkit_dom_event_get_target(CyberKitDOMEvent* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_EVENT(self), 0);
    CyberCore::Event* item = CyberKit::core(self);
    RefPtr<CyberCore::EventTarget> gobjectResult = WTF::getPtr(item->target());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMEventTarget* webkit_dom_event_get_current_target(CyberKitDOMEvent* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_EVENT(self), 0);
    CyberCore::Event* item = CyberKit::core(self);
    RefPtr<CyberCore::EventTarget> gobjectResult = WTF::getPtr(item->currentTarget());
    return CyberKit::kit(gobjectResult.get());
}

gushort webkit_dom_event_get_event_phase(CyberKitDOMEvent* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_EVENT(self), 0);
    CyberCore::Event* item = CyberKit::core(self);
    gushort result = item->eventPhase();
    return result;
}

gboolean webkit_dom_event_get_bubbles(CyberKitDOMEvent* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_EVENT(self), FALSE);
    CyberCore::Event* item = CyberKit::core(self);
    gboolean result = item->bubbles();
    return result;
}

gboolean webkit_dom_event_get_cancelable(CyberKitDOMEvent* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_EVENT(self), FALSE);
    CyberCore::Event* item = CyberKit::core(self);
    gboolean result = item->cancelable();
    return result;
}

guint32 webkit_dom_event_get_time_stamp(CyberKitDOMEvent* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_EVENT(self), 0);
    CyberCore::Event* item = CyberKit::core(self);
    guint32 result = item->timeStamp().approximateWallTime().secondsSinceEpoch().milliseconds();
    return result;
}

CyberKitDOMEventTarget* webkit_dom_event_get_src_element(CyberKitDOMEvent* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_EVENT(self), 0);
    CyberCore::Event* item = CyberKit::core(self);
    RefPtr<CyberCore::EventTarget> gobjectResult = WTF::getPtr(item->target());
    return CyberKit::kit(gobjectResult.get());
}

gboolean webkit_dom_event_get_return_value(CyberKitDOMEvent* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_EVENT(self), FALSE);
    CyberCore::Event* item = CyberKit::core(self);
    gboolean result = item->legacyReturnValue();
    return result;
}

void webkit_dom_event_set_return_value(CyberKitDOMEvent* self, gboolean value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_EVENT(self));
    CyberCore::Event* item = CyberKit::core(self);
    item->setLegacyReturnValue(value);
}

gboolean webkit_dom_event_get_cancel_bubble(CyberKitDOMEvent* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_EVENT(self), FALSE);
    CyberCore::Event* item = CyberKit::core(self);
    gboolean result = item->cancelBubble();
    return result;
}

void webkit_dom_event_set_cancel_bubble(CyberKitDOMEvent* self, gboolean value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_EVENT(self));
    CyberCore::Event* item = CyberKit::core(self);
    item->setCancelBubble(value);
}

G_GNUC_END_IGNORE_DEPRECATIONS;
