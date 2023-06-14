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
#include "CyberKitDOMHTMLButtonElement.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include "GObjectEventListener.h"
#include <CyberCore/HTMLNames.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMHTMLButtonElementPrivate.h"
#include "CyberKitDOMHTMLFormElementPrivate.h"
#include "CyberKitDOMNodeListPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLButtonElement* kit(CyberCore::HTMLButtonElement* obj)
{
    return WEBKIT_DOM_HTML_BUTTON_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::HTMLButtonElement* core(CyberKitDOMHTMLButtonElement* request)
{
    return request ? static_cast<CyberCore::HTMLButtonElement*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLButtonElement* wrapHTMLButtonElement(CyberCore::HTMLButtonElement* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_BUTTON_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_HTML_BUTTON_ELEMENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_html_button_element_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::HTMLButtonElement* coreTarget = static_cast<CyberCore::HTMLButtonElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_html_button_element_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLButtonElement* coreTarget = static_cast<CyberCore::HTMLButtonElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_html_button_element_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLButtonElement* coreTarget = static_cast<CyberCore::HTMLButtonElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_html_button_element_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_html_button_element_dispatch_event;
    iface->add_event_listener = webkit_dom_html_button_element_add_event_listener;
    iface->remove_event_listener = webkit_dom_html_button_element_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMHTMLButtonElement, webkit_dom_html_button_element, WEBKIT_DOM_TYPE_HTML_ELEMENT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_html_button_element_dom_event_target_init))

enum {
    DOM_HTML_BUTTON_ELEMENT_PROP_0,
    DOM_HTML_BUTTON_ELEMENT_PROP_AUTOFOCUS,
    DOM_HTML_BUTTON_ELEMENT_PROP_DISABLED,
    DOM_HTML_BUTTON_ELEMENT_PROP_FORM,
    DOM_HTML_BUTTON_ELEMENT_PROP_TYPE,
    DOM_HTML_BUTTON_ELEMENT_PROP_NAME,
    DOM_HTML_BUTTON_ELEMENT_PROP_VALUE,
    DOM_HTML_BUTTON_ELEMENT_PROP_WILL_VALIDATE,
};

static void webkit_dom_html_button_element_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLButtonElement* self = WEBKIT_DOM_HTML_BUTTON_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_BUTTON_ELEMENT_PROP_AUTOFOCUS:
        webkit_dom_html_button_element_set_autofocus(self, g_value_get_boolean(value));
        break;
    case DOM_HTML_BUTTON_ELEMENT_PROP_DISABLED:
        webkit_dom_html_button_element_set_disabled(self, g_value_get_boolean(value));
        break;
    case DOM_HTML_BUTTON_ELEMENT_PROP_TYPE:
        webkit_dom_html_button_element_set_button_type(self, g_value_get_string(value));
        break;
    case DOM_HTML_BUTTON_ELEMENT_PROP_NAME:
        webkit_dom_html_button_element_set_name(self, g_value_get_string(value));
        break;
    case DOM_HTML_BUTTON_ELEMENT_PROP_VALUE:
        webkit_dom_html_button_element_set_value(self, g_value_get_string(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_button_element_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLButtonElement* self = WEBKIT_DOM_HTML_BUTTON_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_BUTTON_ELEMENT_PROP_AUTOFOCUS:
        g_value_set_boolean(value, webkit_dom_html_button_element_get_autofocus(self));
        break;
    case DOM_HTML_BUTTON_ELEMENT_PROP_DISABLED:
        g_value_set_boolean(value, webkit_dom_html_button_element_get_disabled(self));
        break;
    case DOM_HTML_BUTTON_ELEMENT_PROP_FORM:
        g_value_set_object(value, webkit_dom_html_button_element_get_form(self));
        break;
    case DOM_HTML_BUTTON_ELEMENT_PROP_TYPE:
        g_value_take_string(value, webkit_dom_html_button_element_get_button_type(self));
        break;
    case DOM_HTML_BUTTON_ELEMENT_PROP_NAME:
        g_value_take_string(value, webkit_dom_html_button_element_get_name(self));
        break;
    case DOM_HTML_BUTTON_ELEMENT_PROP_VALUE:
        g_value_take_string(value, webkit_dom_html_button_element_get_value(self));
        break;
    case DOM_HTML_BUTTON_ELEMENT_PROP_WILL_VALIDATE:
        g_value_set_boolean(value, webkit_dom_html_button_element_get_will_validate(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_button_element_class_init(CyberKitDOMHTMLButtonElementClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_html_button_element_set_property;
    gobjectClass->get_property = webkit_dom_html_button_element_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_BUTTON_ELEMENT_PROP_AUTOFOCUS,
        g_param_spec_boolean(
            "autofocus",
            "HTMLButtonElement:autofocus",
            "read-write gboolean HTMLButtonElement:autofocus",
            FALSE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_BUTTON_ELEMENT_PROP_DISABLED,
        g_param_spec_boolean(
            "disabled",
            "HTMLButtonElement:disabled",
            "read-write gboolean HTMLButtonElement:disabled",
            FALSE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_BUTTON_ELEMENT_PROP_FORM,
        g_param_spec_object(
            "form",
            "HTMLButtonElement:form",
            "read-only CyberKitDOMHTMLFormElement* HTMLButtonElement:form",
            WEBKIT_DOM_TYPE_HTML_FORM_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_BUTTON_ELEMENT_PROP_TYPE,
        g_param_spec_string(
            "type",
            "HTMLButtonElement:type",
            "read-write gchar* HTMLButtonElement:type",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_BUTTON_ELEMENT_PROP_NAME,
        g_param_spec_string(
            "name",
            "HTMLButtonElement:name",
            "read-write gchar* HTMLButtonElement:name",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_BUTTON_ELEMENT_PROP_VALUE,
        g_param_spec_string(
            "value",
            "HTMLButtonElement:value",
            "read-write gchar* HTMLButtonElement:value",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_BUTTON_ELEMENT_PROP_WILL_VALIDATE,
        g_param_spec_boolean(
            "will-validate",
            "HTMLButtonElement:will-validate",
            "read-only gboolean HTMLButtonElement:will-validate",
            FALSE,
            WEBKIT_PARAM_READABLE));
}

static void webkit_dom_html_button_element_init(CyberKitDOMHTMLButtonElement* request)
{
    UNUSED_PARAM(request);
}

gboolean webkit_dom_html_button_element_get_autofocus(CyberKitDOMHTMLButtonElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_BUTTON_ELEMENT(self), FALSE);
    CyberCore::HTMLButtonElement* item = CyberKit::core(self);
    gboolean result = item->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::autofocusAttr);
    return result;
}

void webkit_dom_html_button_element_set_autofocus(CyberKitDOMHTMLButtonElement* self, gboolean value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_BUTTON_ELEMENT(self));
    CyberCore::HTMLButtonElement* item = CyberKit::core(self);
    item->setBooleanAttribute(CyberCore::HTMLNames::autofocusAttr, value);
}

gboolean webkit_dom_html_button_element_get_disabled(CyberKitDOMHTMLButtonElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_BUTTON_ELEMENT(self), FALSE);
    CyberCore::HTMLButtonElement* item = CyberKit::core(self);
    gboolean result = item->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::disabledAttr);
    return result;
}

void webkit_dom_html_button_element_set_disabled(CyberKitDOMHTMLButtonElement* self, gboolean value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_BUTTON_ELEMENT(self));
    CyberCore::HTMLButtonElement* item = CyberKit::core(self);
    item->setBooleanAttribute(CyberCore::HTMLNames::disabledAttr, value);
}

CyberKitDOMHTMLFormElement* webkit_dom_html_button_element_get_form(CyberKitDOMHTMLButtonElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_BUTTON_ELEMENT(self), 0);
    CyberCore::HTMLButtonElement* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLFormElement> gobjectResult = WTF::getPtr(item->form());
    return CyberKit::kit(gobjectResult.get());
}

gchar* webkit_dom_html_button_element_get_button_type(CyberKitDOMHTMLButtonElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_BUTTON_ELEMENT(self), 0);
    CyberCore::HTMLButtonElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->type());
    return result;
}

void webkit_dom_html_button_element_set_button_type(CyberKitDOMHTMLButtonElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_BUTTON_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLButtonElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setType(convertedValue);
}

gchar* webkit_dom_html_button_element_get_name(CyberKitDOMHTMLButtonElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_BUTTON_ELEMENT(self), 0);
    CyberCore::HTMLButtonElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->getNameAttribute());
    return result;
}

void webkit_dom_html_button_element_set_name(CyberKitDOMHTMLButtonElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_BUTTON_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLButtonElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::nameAttr, convertedValue);
}

gchar* webkit_dom_html_button_element_get_value(CyberKitDOMHTMLButtonElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_BUTTON_ELEMENT(self), 0);
    CyberCore::HTMLButtonElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::valueAttr));
    return result;
}

void webkit_dom_html_button_element_set_value(CyberKitDOMHTMLButtonElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_BUTTON_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLButtonElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::valueAttr, convertedValue);
}

gboolean webkit_dom_html_button_element_get_will_validate(CyberKitDOMHTMLButtonElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_BUTTON_ELEMENT(self), FALSE);
    CyberCore::HTMLButtonElement* item = CyberKit::core(self);
    gboolean result = item->willValidate();
    return result;
}
G_GNUC_END_IGNORE_DEPRECATIONS;
