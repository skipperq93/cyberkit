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
#include "CyberKitDOMAttr.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include "GObjectEventListener.h"
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMAttrPrivate.h"
#include "CyberKitDOMElementPrivate.h"
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMAttr* kit(CyberCore::Attr* obj)
{
    return WEBKIT_DOM_ATTR(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::Attr* core(CyberKitDOMAttr* request)
{
    return request ? static_cast<CyberCore::Attr*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMAttr* wrapAttr(CyberCore::Attr* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_ATTR(g_object_new(WEBKIT_DOM_TYPE_ATTR, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_attr_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::Attr* coreTarget = static_cast<CyberCore::Attr*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_attr_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::Attr* coreTarget = static_cast<CyberCore::Attr*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_attr_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::Attr* coreTarget = static_cast<CyberCore::Attr*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_attr_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_attr_dispatch_event;
    iface->add_event_listener = webkit_dom_attr_add_event_listener;
    iface->remove_event_listener = webkit_dom_attr_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMAttr, webkit_dom_attr, WEBKIT_DOM_TYPE_NODE, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_attr_dom_event_target_init))

enum {
    DOM_ATTR_PROP_0,
    DOM_ATTR_PROP_NAME,
    DOM_ATTR_PROP_SPECIFIED,
    DOM_ATTR_PROP_VALUE,
    DOM_ATTR_PROP_OWNER_ELEMENT,
    DOM_ATTR_PROP_NAMESPACE_URI,
    DOM_ATTR_PROP_PREFIX,
    DOM_ATTR_PROP_LOCAL_NAME,
};

static void webkit_dom_attr_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMAttr* self = WEBKIT_DOM_ATTR(object);

    switch (propertyId) {
    case DOM_ATTR_PROP_VALUE:
        webkit_dom_attr_set_value(self, g_value_get_string(value), nullptr);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_attr_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMAttr* self = WEBKIT_DOM_ATTR(object);

    switch (propertyId) {
    case DOM_ATTR_PROP_NAME:
        g_value_take_string(value, webkit_dom_attr_get_name(self));
        break;
    case DOM_ATTR_PROP_SPECIFIED:
        g_value_set_boolean(value, webkit_dom_attr_get_specified(self));
        break;
    case DOM_ATTR_PROP_VALUE:
        g_value_take_string(value, webkit_dom_attr_get_value(self));
        break;
    case DOM_ATTR_PROP_OWNER_ELEMENT:
        g_value_set_object(value, webkit_dom_attr_get_owner_element(self));
        break;
    case DOM_ATTR_PROP_NAMESPACE_URI:
        g_value_take_string(value, webkit_dom_attr_get_namespace_uri(self));
        break;
    case DOM_ATTR_PROP_PREFIX:
        g_value_take_string(value, webkit_dom_attr_get_prefix(self));
        break;
    case DOM_ATTR_PROP_LOCAL_NAME:
        g_value_take_string(value, webkit_dom_attr_get_local_name(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_attr_class_init(CyberKitDOMAttrClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_attr_set_property;
    gobjectClass->get_property = webkit_dom_attr_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_ATTR_PROP_NAME,
        g_param_spec_string(
            "name",
            "Attr:name",
            "read-only gchar* Attr:name",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_ATTR_PROP_SPECIFIED,
        g_param_spec_boolean(
            "specified",
            "Attr:specified",
            "read-only gboolean Attr:specified",
            FALSE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_ATTR_PROP_VALUE,
        g_param_spec_string(
            "value",
            "Attr:value",
            "read-write gchar* Attr:value",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_ATTR_PROP_OWNER_ELEMENT,
        g_param_spec_object(
            "owner-element",
            "Attr:owner-element",
            "read-only CyberKitDOMElement* Attr:owner-element",
            WEBKIT_DOM_TYPE_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_ATTR_PROP_NAMESPACE_URI,
        g_param_spec_string(
            "namespace-uri",
            "Attr:namespace-uri",
            "read-only gchar* Attr:namespace-uri",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_ATTR_PROP_PREFIX,
        g_param_spec_string(
            "prefix",
            "Attr:prefix",
            "read-only gchar* Attr:prefix",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_ATTR_PROP_LOCAL_NAME,
        g_param_spec_string(
            "local-name",
            "Attr:local-name",
            "read-only gchar* Attr:local-name",
            "",
            WEBKIT_PARAM_READABLE));

}

static void webkit_dom_attr_init(CyberKitDOMAttr* request)
{
    UNUSED_PARAM(request);
}

gchar* webkit_dom_attr_get_name(CyberKitDOMAttr* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_ATTR(self), 0);
    CyberCore::Attr* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->name());
    return result;
}

gboolean webkit_dom_attr_get_specified(CyberKitDOMAttr* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_ATTR(self), FALSE);
    CyberCore::Attr* item = CyberKit::core(self);
    gboolean result = item->specified();
    return result;
}

gchar* webkit_dom_attr_get_value(CyberKitDOMAttr* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_ATTR(self), 0);
    CyberCore::Attr* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->value());
    return result;
}

void webkit_dom_attr_set_value(CyberKitDOMAttr* self, const gchar* value, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_ATTR(self));
    g_return_if_fail(value);
    UNUSED_PARAM(error);
    CyberCore::Attr* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setValue(convertedValue);
}

CyberKitDOMElement* webkit_dom_attr_get_owner_element(CyberKitDOMAttr* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_ATTR(self), 0);
    CyberCore::Attr* item = CyberKit::core(self);
    RefPtr<CyberCore::Element> gobjectResult = WTF::getPtr(item->ownerElement());
    return CyberKit::kit(gobjectResult.get());
}

gchar* webkit_dom_attr_get_namespace_uri(CyberKitDOMAttr* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_ATTR(self), 0);
    CyberCore::Attr* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->namespaceURI());
    return result;
}

gchar* webkit_dom_attr_get_prefix(CyberKitDOMAttr* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_ATTR(self), 0);
    CyberCore::Attr* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->prefix());
    return result;
}

gchar* webkit_dom_attr_get_local_name(CyberKitDOMAttr* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_ATTR(self), 0);
    CyberCore::Attr* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->localName());
    return result;
}

G_GNUC_END_IGNORE_DEPRECATIONS;
