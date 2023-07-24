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
#include "CyberKitDOMHTMLBodyElement.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include "GObjectEventListener.h"
#include <CyberCore/HTMLNames.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMHTMLBodyElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLBodyElement* kit(CyberCore::HTMLBodyElement* obj)
{
    return WEBKIT_DOM_HTML_BODY_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::HTMLBodyElement* core(CyberKitDOMHTMLBodyElement* request)
{
    return request ? static_cast<CyberCore::HTMLBodyElement*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLBodyElement* wrapHTMLBodyElement(CyberCore::HTMLBodyElement* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_BODY_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_HTML_BODY_ELEMENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_html_body_element_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::HTMLBodyElement* coreTarget = static_cast<CyberCore::HTMLBodyElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_html_body_element_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLBodyElement* coreTarget = static_cast<CyberCore::HTMLBodyElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_html_body_element_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLBodyElement* coreTarget = static_cast<CyberCore::HTMLBodyElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_html_body_element_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_html_body_element_dispatch_event;
    iface->add_event_listener = webkit_dom_html_body_element_add_event_listener;
    iface->remove_event_listener = webkit_dom_html_body_element_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMHTMLBodyElement, webkit_dom_html_body_element, WEBKIT_DOM_TYPE_HTML_ELEMENT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_html_body_element_dom_event_target_init))

enum {
    DOM_HTML_BODY_ELEMENT_PROP_0,
    DOM_HTML_BODY_ELEMENT_PROP_A_LINK,
    DOM_HTML_BODY_ELEMENT_PROP_BACKGROUND,
    DOM_HTML_BODY_ELEMENT_PROP_BG_COLOR,
    DOM_HTML_BODY_ELEMENT_PROP_LINK,
    DOM_HTML_BODY_ELEMENT_PROP_TEXT,
    DOM_HTML_BODY_ELEMENT_PROP_V_LINK,
};

static void webkit_dom_html_body_element_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLBodyElement* self = WEBKIT_DOM_HTML_BODY_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_BODY_ELEMENT_PROP_A_LINK:
        webkit_dom_html_body_element_set_a_link(self, g_value_get_string(value));
        break;
    case DOM_HTML_BODY_ELEMENT_PROP_BACKGROUND:
        webkit_dom_html_body_element_set_background(self, g_value_get_string(value));
        break;
    case DOM_HTML_BODY_ELEMENT_PROP_BG_COLOR:
        webkit_dom_html_body_element_set_bg_color(self, g_value_get_string(value));
        break;
    case DOM_HTML_BODY_ELEMENT_PROP_LINK:
        webkit_dom_html_body_element_set_link(self, g_value_get_string(value));
        break;
    case DOM_HTML_BODY_ELEMENT_PROP_TEXT:
        webkit_dom_html_body_element_set_text(self, g_value_get_string(value));
        break;
    case DOM_HTML_BODY_ELEMENT_PROP_V_LINK:
        webkit_dom_html_body_element_set_v_link(self, g_value_get_string(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_body_element_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLBodyElement* self = WEBKIT_DOM_HTML_BODY_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_BODY_ELEMENT_PROP_A_LINK:
        g_value_take_string(value, webkit_dom_html_body_element_get_a_link(self));
        break;
    case DOM_HTML_BODY_ELEMENT_PROP_BACKGROUND:
        g_value_take_string(value, webkit_dom_html_body_element_get_background(self));
        break;
    case DOM_HTML_BODY_ELEMENT_PROP_BG_COLOR:
        g_value_take_string(value, webkit_dom_html_body_element_get_bg_color(self));
        break;
    case DOM_HTML_BODY_ELEMENT_PROP_LINK:
        g_value_take_string(value, webkit_dom_html_body_element_get_link(self));
        break;
    case DOM_HTML_BODY_ELEMENT_PROP_TEXT:
        g_value_take_string(value, webkit_dom_html_body_element_get_text(self));
        break;
    case DOM_HTML_BODY_ELEMENT_PROP_V_LINK:
        g_value_take_string(value, webkit_dom_html_body_element_get_v_link(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_body_element_class_init(CyberKitDOMHTMLBodyElementClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_html_body_element_set_property;
    gobjectClass->get_property = webkit_dom_html_body_element_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_BODY_ELEMENT_PROP_A_LINK,
        g_param_spec_string(
            "a-link",
            "HTMLBodyElement:a-link",
            "read-write gchar* HTMLBodyElement:a-link",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_BODY_ELEMENT_PROP_BACKGROUND,
        g_param_spec_string(
            "background",
            "HTMLBodyElement:background",
            "read-write gchar* HTMLBodyElement:background",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_BODY_ELEMENT_PROP_BG_COLOR,
        g_param_spec_string(
            "bg-color",
            "HTMLBodyElement:bg-color",
            "read-write gchar* HTMLBodyElement:bg-color",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_BODY_ELEMENT_PROP_LINK,
        g_param_spec_string(
            "link",
            "HTMLBodyElement:link",
            "read-write gchar* HTMLBodyElement:link",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_BODY_ELEMENT_PROP_TEXT,
        g_param_spec_string(
            "text",
            "HTMLBodyElement:text",
            "read-write gchar* HTMLBodyElement:text",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_BODY_ELEMENT_PROP_V_LINK,
        g_param_spec_string(
            "v-link",
            "HTMLBodyElement:v-link",
            "read-write gchar* HTMLBodyElement:v-link",
            "",
            WEBKIT_PARAM_READWRITE));

}

static void webkit_dom_html_body_element_init(CyberKitDOMHTMLBodyElement* request)
{
    UNUSED_PARAM(request);
}

gchar* webkit_dom_html_body_element_get_a_link(CyberKitDOMHTMLBodyElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_BODY_ELEMENT(self), 0);
    CyberCore::HTMLBodyElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::alinkAttr));
    return result;
}

void webkit_dom_html_body_element_set_a_link(CyberKitDOMHTMLBodyElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_BODY_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLBodyElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::alinkAttr, convertedValue);
}

gchar* webkit_dom_html_body_element_get_background(CyberKitDOMHTMLBodyElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_BODY_ELEMENT(self), 0);
    CyberCore::HTMLBodyElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::backgroundAttr));
    return result;
}

void webkit_dom_html_body_element_set_background(CyberKitDOMHTMLBodyElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_BODY_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLBodyElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::backgroundAttr, convertedValue);
}

gchar* webkit_dom_html_body_element_get_bg_color(CyberKitDOMHTMLBodyElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_BODY_ELEMENT(self), 0);
    CyberCore::HTMLBodyElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::bgcolorAttr));
    return result;
}

void webkit_dom_html_body_element_set_bg_color(CyberKitDOMHTMLBodyElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_BODY_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLBodyElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::bgcolorAttr, convertedValue);
}

gchar* webkit_dom_html_body_element_get_link(CyberKitDOMHTMLBodyElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_BODY_ELEMENT(self), 0);
    CyberCore::HTMLBodyElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::linkAttr));
    return result;
}

void webkit_dom_html_body_element_set_link(CyberKitDOMHTMLBodyElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_BODY_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLBodyElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::linkAttr, convertedValue);
}

gchar* webkit_dom_html_body_element_get_text(CyberKitDOMHTMLBodyElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_BODY_ELEMENT(self), 0);
    CyberCore::HTMLBodyElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::textAttr));
    return result;
}

void webkit_dom_html_body_element_set_text(CyberKitDOMHTMLBodyElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_BODY_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLBodyElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::textAttr, convertedValue);
}

gchar* webkit_dom_html_body_element_get_v_link(CyberKitDOMHTMLBodyElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_BODY_ELEMENT(self), 0);
    CyberCore::HTMLBodyElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::vlinkAttr));
    return result;
}

void webkit_dom_html_body_element_set_v_link(CyberKitDOMHTMLBodyElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_BODY_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLBodyElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::vlinkAttr, convertedValue);
}

G_GNUC_END_IGNORE_DEPRECATIONS;
