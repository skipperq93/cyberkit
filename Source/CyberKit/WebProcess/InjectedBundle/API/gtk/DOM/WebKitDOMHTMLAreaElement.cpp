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
#include "CyberKitDOMHTMLAreaElement.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include <CyberCore/ElementInlines.h>
#include <CyberCore/HTMLNames.h>
#include <CyberCore/JSExecState.h>
#include "GObjectEventListener.h"
#include "CyberKitDOMDOMTokenListPrivate.h"
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMHTMLAreaElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLAreaElement* kit(CyberCore::HTMLAreaElement* obj)
{
    return WEBKIT_DOM_HTML_AREA_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::HTMLAreaElement* core(CyberKitDOMHTMLAreaElement* request)
{
    return request ? static_cast<CyberCore::HTMLAreaElement*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLAreaElement* wrapHTMLAreaElement(CyberCore::HTMLAreaElement* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_AREA_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_HTML_AREA_ELEMENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_html_area_element_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::HTMLAreaElement* coreTarget = static_cast<CyberCore::HTMLAreaElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_html_area_element_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLAreaElement* coreTarget = static_cast<CyberCore::HTMLAreaElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_html_area_element_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLAreaElement* coreTarget = static_cast<CyberCore::HTMLAreaElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_html_area_element_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_html_area_element_dispatch_event;
    iface->add_event_listener = webkit_dom_html_area_element_add_event_listener;
    iface->remove_event_listener = webkit_dom_html_area_element_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMHTMLAreaElement, webkit_dom_html_area_element, WEBKIT_DOM_TYPE_HTML_ELEMENT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_html_area_element_dom_event_target_init))

enum {
    DOM_HTML_AREA_ELEMENT_PROP_0,
    DOM_HTML_AREA_ELEMENT_PROP_ALT,
    DOM_HTML_AREA_ELEMENT_PROP_COORDS,
    DOM_HTML_AREA_ELEMENT_PROP_NO_HREF,
    DOM_HTML_AREA_ELEMENT_PROP_SHAPE,
    DOM_HTML_AREA_ELEMENT_PROP_TARGET,
    DOM_HTML_AREA_ELEMENT_PROP_HREF,
    DOM_HTML_AREA_ELEMENT_PROP_PROTOCOL,
    DOM_HTML_AREA_ELEMENT_PROP_HOST,
    DOM_HTML_AREA_ELEMENT_PROP_HOSTNAME,
    DOM_HTML_AREA_ELEMENT_PROP_PORT,
    DOM_HTML_AREA_ELEMENT_PROP_PATHNAME,
    DOM_HTML_AREA_ELEMENT_PROP_SEARCH,
    DOM_HTML_AREA_ELEMENT_PROP_HASH,
};

static void webkit_dom_html_area_element_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLAreaElement* self = WEBKIT_DOM_HTML_AREA_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_AREA_ELEMENT_PROP_ALT:
        webkit_dom_html_area_element_set_alt(self, g_value_get_string(value));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_COORDS:
        webkit_dom_html_area_element_set_coords(self, g_value_get_string(value));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_NO_HREF:
        webkit_dom_html_area_element_set_no_href(self, g_value_get_boolean(value));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_SHAPE:
        webkit_dom_html_area_element_set_shape(self, g_value_get_string(value));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_TARGET:
        webkit_dom_html_area_element_set_target(self, g_value_get_string(value));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_HREF:
        webkit_dom_html_area_element_set_href(self, g_value_get_string(value));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_PROTOCOL:
        webkit_dom_html_area_element_set_protocol(self, g_value_get_string(value));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_HOST:
        webkit_dom_html_area_element_set_host(self, g_value_get_string(value));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_HOSTNAME:
        webkit_dom_html_area_element_set_hostname(self, g_value_get_string(value));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_PORT:
        webkit_dom_html_area_element_set_port(self, g_value_get_string(value));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_PATHNAME:
        webkit_dom_html_area_element_set_pathname(self, g_value_get_string(value));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_SEARCH:
        webkit_dom_html_area_element_set_search(self, g_value_get_string(value));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_HASH:
        webkit_dom_html_area_element_set_hash(self, g_value_get_string(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_area_element_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLAreaElement* self = WEBKIT_DOM_HTML_AREA_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_AREA_ELEMENT_PROP_ALT:
        g_value_take_string(value, webkit_dom_html_area_element_get_alt(self));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_COORDS:
        g_value_take_string(value, webkit_dom_html_area_element_get_coords(self));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_NO_HREF:
        g_value_set_boolean(value, webkit_dom_html_area_element_get_no_href(self));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_SHAPE:
        g_value_take_string(value, webkit_dom_html_area_element_get_shape(self));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_TARGET:
        g_value_take_string(value, webkit_dom_html_area_element_get_target(self));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_HREF:
        g_value_take_string(value, webkit_dom_html_area_element_get_href(self));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_PROTOCOL:
        g_value_take_string(value, webkit_dom_html_area_element_get_protocol(self));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_HOST:
        g_value_take_string(value, webkit_dom_html_area_element_get_host(self));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_HOSTNAME:
        g_value_take_string(value, webkit_dom_html_area_element_get_hostname(self));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_PORT:
        g_value_take_string(value, webkit_dom_html_area_element_get_port(self));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_PATHNAME:
        g_value_take_string(value, webkit_dom_html_area_element_get_pathname(self));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_SEARCH:
        g_value_take_string(value, webkit_dom_html_area_element_get_search(self));
        break;
    case DOM_HTML_AREA_ELEMENT_PROP_HASH:
        g_value_take_string(value, webkit_dom_html_area_element_get_hash(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_area_element_class_init(CyberKitDOMHTMLAreaElementClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_html_area_element_set_property;
    gobjectClass->get_property = webkit_dom_html_area_element_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_AREA_ELEMENT_PROP_ALT,
        g_param_spec_string(
            "alt",
            "HTMLAreaElement:alt",
            "read-write gchar* HTMLAreaElement:alt",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_AREA_ELEMENT_PROP_COORDS,
        g_param_spec_string(
            "coords",
            "HTMLAreaElement:coords",
            "read-write gchar* HTMLAreaElement:coords",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_AREA_ELEMENT_PROP_NO_HREF,
        g_param_spec_boolean(
            "no-href",
            "HTMLAreaElement:no-href",
            "read-write gboolean HTMLAreaElement:no-href",
            FALSE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_AREA_ELEMENT_PROP_SHAPE,
        g_param_spec_string(
            "shape",
            "HTMLAreaElement:shape",
            "read-write gchar* HTMLAreaElement:shape",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_AREA_ELEMENT_PROP_TARGET,
        g_param_spec_string(
            "target",
            "HTMLAreaElement:target",
            "read-write gchar* HTMLAreaElement:target",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_AREA_ELEMENT_PROP_HREF,
        g_param_spec_string(
            "href",
            "HTMLAreaElement:href",
            "read-write gchar* HTMLAreaElement:href",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_AREA_ELEMENT_PROP_PROTOCOL,
        g_param_spec_string(
            "protocol",
            "HTMLAreaElement:protocol",
            "read-write gchar* HTMLAreaElement:protocol",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_AREA_ELEMENT_PROP_HOST,
        g_param_spec_string(
            "host",
            "HTMLAreaElement:host",
            "read-write gchar* HTMLAreaElement:host",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_AREA_ELEMENT_PROP_HOSTNAME,
        g_param_spec_string(
            "hostname",
            "HTMLAreaElement:hostname",
            "read-write gchar* HTMLAreaElement:hostname",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_AREA_ELEMENT_PROP_PORT,
        g_param_spec_string(
            "port",
            "HTMLAreaElement:port",
            "read-write gchar* HTMLAreaElement:port",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_AREA_ELEMENT_PROP_PATHNAME,
        g_param_spec_string(
            "pathname",
            "HTMLAreaElement:pathname",
            "read-write gchar* HTMLAreaElement:pathname",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_AREA_ELEMENT_PROP_SEARCH,
        g_param_spec_string(
            "search",
            "HTMLAreaElement:search",
            "read-write gchar* HTMLAreaElement:search",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_AREA_ELEMENT_PROP_HASH,
        g_param_spec_string(
            "hash",
            "HTMLAreaElement:hash",
            "read-write gchar* HTMLAreaElement:hash",
            "",
            WEBKIT_PARAM_READWRITE));

}

static void webkit_dom_html_area_element_init(CyberKitDOMHTMLAreaElement* request)
{
    UNUSED_PARAM(request);
}

gchar* webkit_dom_html_area_element_get_alt(CyberKitDOMHTMLAreaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self), 0);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::altAttr));
    return result;
}

void webkit_dom_html_area_element_set_alt(CyberKitDOMHTMLAreaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::altAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_area_element_get_coords(CyberKitDOMHTMLAreaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self), 0);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::coordsAttr));
    return result;
}

void webkit_dom_html_area_element_set_coords(CyberKitDOMHTMLAreaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::coordsAttr, WTF::AtomString::fromUTF8(value));
}

gboolean webkit_dom_html_area_element_get_no_href(CyberKitDOMHTMLAreaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self), FALSE);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    gboolean result = item->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::nohrefAttr);
    return result;
}

void webkit_dom_html_area_element_set_no_href(CyberKitDOMHTMLAreaElement* self, gboolean value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self));
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    item->setBooleanAttribute(CyberCore::HTMLNames::nohrefAttr, value);
}

gchar* webkit_dom_html_area_element_get_shape(CyberKitDOMHTMLAreaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self), 0);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::shapeAttr));
    return result;
}

void webkit_dom_html_area_element_set_shape(CyberKitDOMHTMLAreaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::shapeAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_area_element_get_target(CyberKitDOMHTMLAreaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self), 0);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::targetAttr));
    return result;
}

void webkit_dom_html_area_element_set_target(CyberKitDOMHTMLAreaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::targetAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_area_element_get_href(CyberKitDOMHTMLAreaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self), 0);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->getURLAttribute(CyberCore::HTMLNames::hrefAttr));
    return result;
}

void webkit_dom_html_area_element_set_href(CyberKitDOMHTMLAreaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::hrefAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_area_element_get_protocol(CyberKitDOMHTMLAreaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self), 0);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->protocol());
    return result;
}

void webkit_dom_html_area_element_set_protocol(CyberKitDOMHTMLAreaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setProtocol(convertedValue);
}

gchar* webkit_dom_html_area_element_get_host(CyberKitDOMHTMLAreaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self), 0);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->host());
    return result;
}

void webkit_dom_html_area_element_set_host(CyberKitDOMHTMLAreaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setHost(convertedValue);
}

gchar* webkit_dom_html_area_element_get_hostname(CyberKitDOMHTMLAreaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self), 0);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->hostname());
    return result;
}

void webkit_dom_html_area_element_set_hostname(CyberKitDOMHTMLAreaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setHostname(convertedValue);
}

gchar* webkit_dom_html_area_element_get_port(CyberKitDOMHTMLAreaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self), 0);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->port());
    return result;
}

void webkit_dom_html_area_element_set_port(CyberKitDOMHTMLAreaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setPort(convertedValue);
}

gchar* webkit_dom_html_area_element_get_pathname(CyberKitDOMHTMLAreaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self), 0);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->pathname());
    return result;
}

void webkit_dom_html_area_element_set_pathname(CyberKitDOMHTMLAreaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setPathname(convertedValue);
}

gchar* webkit_dom_html_area_element_get_search(CyberKitDOMHTMLAreaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self), 0);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->search());
    return result;
}

void webkit_dom_html_area_element_set_search(CyberKitDOMHTMLAreaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setSearch(convertedValue);
}

gchar* webkit_dom_html_area_element_get_hash(CyberKitDOMHTMLAreaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self), 0);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->hash());
    return result;
}

void webkit_dom_html_area_element_set_hash(CyberKitDOMHTMLAreaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_AREA_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLAreaElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setHash(convertedValue);
}

G_GNUC_END_IGNORE_DEPRECATIONS;
