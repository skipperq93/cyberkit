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
#include "CyberKitDOMHTMLInputElement.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include "GObjectEventListener.h"
#include <CyberCore/HTMLNames.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMFileListPrivate.h"
#include "CyberKitDOMHTMLElementPrivate.h"
#include "CyberKitDOMHTMLFormElementPrivate.h"
#include "CyberKitDOMHTMLInputElementPrivate.h"
#include "CyberKitDOMNodeListPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLInputElement* kit(CyberCore::HTMLInputElement* obj)
{
    return WEBKIT_DOM_HTML_INPUT_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::HTMLInputElement* core(CyberKitDOMHTMLInputElement* request)
{
    return request ? static_cast<CyberCore::HTMLInputElement*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLInputElement* wrapHTMLInputElement(CyberCore::HTMLInputElement* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_INPUT_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_HTML_INPUT_ELEMENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_html_input_element_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::HTMLInputElement* coreTarget = static_cast<CyberCore::HTMLInputElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_html_input_element_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLInputElement* coreTarget = static_cast<CyberCore::HTMLInputElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_html_input_element_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLInputElement* coreTarget = static_cast<CyberCore::HTMLInputElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_html_input_element_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_html_input_element_dispatch_event;
    iface->add_event_listener = webkit_dom_html_input_element_add_event_listener;
    iface->remove_event_listener = webkit_dom_html_input_element_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMHTMLInputElement, webkit_dom_html_input_element, WEBKIT_DOM_TYPE_HTML_ELEMENT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_html_input_element_dom_event_target_init))

enum {
    DOM_HTML_INPUT_ELEMENT_PROP_0,
    DOM_HTML_INPUT_ELEMENT_PROP_ACCEPT,
    DOM_HTML_INPUT_ELEMENT_PROP_ALT,
    DOM_HTML_INPUT_ELEMENT_PROP_AUTOFOCUS,
    DOM_HTML_INPUT_ELEMENT_PROP_DEFAULT_CHECKED,
    DOM_HTML_INPUT_ELEMENT_PROP_CHECKED,
    DOM_HTML_INPUT_ELEMENT_PROP_DISABLED,
    DOM_HTML_INPUT_ELEMENT_PROP_FORM,
    DOM_HTML_INPUT_ELEMENT_PROP_FILES,
    DOM_HTML_INPUT_ELEMENT_PROP_HEIGHT,
    DOM_HTML_INPUT_ELEMENT_PROP_INDETERMINATE,
    DOM_HTML_INPUT_ELEMENT_PROP_MAX_LENGTH,
    DOM_HTML_INPUT_ELEMENT_PROP_MULTIPLE,
    DOM_HTML_INPUT_ELEMENT_PROP_NAME,
    DOM_HTML_INPUT_ELEMENT_PROP_READ_ONLY,
    DOM_HTML_INPUT_ELEMENT_PROP_SIZE,
    DOM_HTML_INPUT_ELEMENT_PROP_SRC,
    DOM_HTML_INPUT_ELEMENT_PROP_TYPE,
    DOM_HTML_INPUT_ELEMENT_PROP_DEFAULT_VALUE,
    DOM_HTML_INPUT_ELEMENT_PROP_VALUE,
    DOM_HTML_INPUT_ELEMENT_PROP_WIDTH,
    DOM_HTML_INPUT_ELEMENT_PROP_WILL_VALIDATE,
    DOM_HTML_INPUT_ELEMENT_PROP_ALIGN,
    DOM_HTML_INPUT_ELEMENT_PROP_USE_MAP,
    DOM_HTML_INPUT_ELEMENT_PROP_CAPTURE,
};

static void webkit_dom_html_input_element_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLInputElement* self = WEBKIT_DOM_HTML_INPUT_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_INPUT_ELEMENT_PROP_ACCEPT:
        webkit_dom_html_input_element_set_accept(self, g_value_get_string(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_ALT:
        webkit_dom_html_input_element_set_alt(self, g_value_get_string(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_AUTOFOCUS:
        webkit_dom_html_input_element_set_autofocus(self, g_value_get_boolean(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_DEFAULT_CHECKED:
        webkit_dom_html_input_element_set_default_checked(self, g_value_get_boolean(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_CHECKED:
        webkit_dom_html_input_element_set_checked(self, g_value_get_boolean(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_DISABLED:
        webkit_dom_html_input_element_set_disabled(self, g_value_get_boolean(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_HEIGHT:
        webkit_dom_html_input_element_set_height(self, g_value_get_ulong(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_INDETERMINATE:
        webkit_dom_html_input_element_set_indeterminate(self, g_value_get_boolean(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_MAX_LENGTH:
        webkit_dom_html_input_element_set_max_length(self, g_value_get_long(value), nullptr);
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_MULTIPLE:
        webkit_dom_html_input_element_set_multiple(self, g_value_get_boolean(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_NAME:
        webkit_dom_html_input_element_set_name(self, g_value_get_string(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_READ_ONLY:
        webkit_dom_html_input_element_set_read_only(self, g_value_get_boolean(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_SIZE:
        webkit_dom_html_input_element_set_size(self, g_value_get_ulong(value), nullptr);
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_SRC:
        webkit_dom_html_input_element_set_src(self, g_value_get_string(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_TYPE:
        webkit_dom_html_input_element_set_input_type(self, g_value_get_string(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_DEFAULT_VALUE:
        webkit_dom_html_input_element_set_default_value(self, g_value_get_string(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_VALUE:
        webkit_dom_html_input_element_set_value(self, g_value_get_string(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_WIDTH:
        webkit_dom_html_input_element_set_width(self, g_value_get_ulong(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_ALIGN:
        webkit_dom_html_input_element_set_align(self, g_value_get_string(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_USE_MAP:
        webkit_dom_html_input_element_set_use_map(self, g_value_get_string(value));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_CAPTURE:
        webkit_dom_html_input_element_set_capture_type(self, g_value_get_string(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_input_element_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLInputElement* self = WEBKIT_DOM_HTML_INPUT_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_INPUT_ELEMENT_PROP_ACCEPT:
        g_value_take_string(value, webkit_dom_html_input_element_get_accept(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_ALT:
        g_value_take_string(value, webkit_dom_html_input_element_get_alt(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_AUTOFOCUS:
        g_value_set_boolean(value, webkit_dom_html_input_element_get_autofocus(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_DEFAULT_CHECKED:
        g_value_set_boolean(value, webkit_dom_html_input_element_get_default_checked(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_CHECKED:
        g_value_set_boolean(value, webkit_dom_html_input_element_get_checked(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_DISABLED:
        g_value_set_boolean(value, webkit_dom_html_input_element_get_disabled(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_FORM:
        g_value_set_object(value, webkit_dom_html_input_element_get_form(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_FILES:
        g_value_set_object(value, webkit_dom_html_input_element_get_files(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_HEIGHT:
        g_value_set_ulong(value, webkit_dom_html_input_element_get_height(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_INDETERMINATE:
        g_value_set_boolean(value, webkit_dom_html_input_element_get_indeterminate(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_MAX_LENGTH:
        g_value_set_long(value, webkit_dom_html_input_element_get_max_length(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_MULTIPLE:
        g_value_set_boolean(value, webkit_dom_html_input_element_get_multiple(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_NAME:
        g_value_take_string(value, webkit_dom_html_input_element_get_name(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_READ_ONLY:
        g_value_set_boolean(value, webkit_dom_html_input_element_get_read_only(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_SIZE:
        g_value_set_ulong(value, webkit_dom_html_input_element_get_size(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_SRC:
        g_value_take_string(value, webkit_dom_html_input_element_get_src(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_TYPE:
        g_value_take_string(value, webkit_dom_html_input_element_get_input_type(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_DEFAULT_VALUE:
        g_value_take_string(value, webkit_dom_html_input_element_get_default_value(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_VALUE:
        g_value_take_string(value, webkit_dom_html_input_element_get_value(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_WIDTH:
        g_value_set_ulong(value, webkit_dom_html_input_element_get_width(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_WILL_VALIDATE:
        g_value_set_boolean(value, webkit_dom_html_input_element_get_will_validate(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_ALIGN:
        g_value_take_string(value, webkit_dom_html_input_element_get_align(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_USE_MAP:
        g_value_take_string(value, webkit_dom_html_input_element_get_use_map(self));
        break;
    case DOM_HTML_INPUT_ELEMENT_PROP_CAPTURE:
        g_value_take_string(value, webkit_dom_html_input_element_get_capture_type(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_input_element_class_init(CyberKitDOMHTMLInputElementClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_html_input_element_set_property;
    gobjectClass->get_property = webkit_dom_html_input_element_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_ACCEPT,
        g_param_spec_string(
            "accept",
            "HTMLInputElement:accept",
            "read-write gchar* HTMLInputElement:accept",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_ALT,
        g_param_spec_string(
            "alt",
            "HTMLInputElement:alt",
            "read-write gchar* HTMLInputElement:alt",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_AUTOFOCUS,
        g_param_spec_boolean(
            "autofocus",
            "HTMLInputElement:autofocus",
            "read-write gboolean HTMLInputElement:autofocus",
            FALSE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_DEFAULT_CHECKED,
        g_param_spec_boolean(
            "default-checked",
            "HTMLInputElement:default-checked",
            "read-write gboolean HTMLInputElement:default-checked",
            FALSE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_CHECKED,
        g_param_spec_boolean(
            "checked",
            "HTMLInputElement:checked",
            "read-write gboolean HTMLInputElement:checked",
            FALSE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_DISABLED,
        g_param_spec_boolean(
            "disabled",
            "HTMLInputElement:disabled",
            "read-write gboolean HTMLInputElement:disabled",
            FALSE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_FORM,
        g_param_spec_object(
            "form",
            "HTMLInputElement:form",
            "read-only CyberKitDOMHTMLFormElement* HTMLInputElement:form",
            WEBKIT_DOM_TYPE_HTML_FORM_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_FILES,
        g_param_spec_object(
            "files",
            "HTMLInputElement:files",
            "read-only CyberKitDOMFileList* HTMLInputElement:files",
            WEBKIT_DOM_TYPE_FILE_LIST,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_HEIGHT,
        g_param_spec_ulong(
            "height",
            "HTMLInputElement:height",
            "read-write gulong HTMLInputElement:height",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_INDETERMINATE,
        g_param_spec_boolean(
            "indeterminate",
            "HTMLInputElement:indeterminate",
            "read-write gboolean HTMLInputElement:indeterminate",
            FALSE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_MAX_LENGTH,
        g_param_spec_long(
            "max-length",
            "HTMLInputElement:max-length",
            "read-write glong HTMLInputElement:max-length",
            G_MINLONG, G_MAXLONG, 0,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_MULTIPLE,
        g_param_spec_boolean(
            "multiple",
            "HTMLInputElement:multiple",
            "read-write gboolean HTMLInputElement:multiple",
            FALSE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_NAME,
        g_param_spec_string(
            "name",
            "HTMLInputElement:name",
            "read-write gchar* HTMLInputElement:name",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_READ_ONLY,
        g_param_spec_boolean(
            "read-only",
            "HTMLInputElement:read-only",
            "read-write gboolean HTMLInputElement:read-only",
            FALSE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_SIZE,
        g_param_spec_ulong(
            "size",
            "HTMLInputElement:size",
            "read-write gulong HTMLInputElement:size",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_SRC,
        g_param_spec_string(
            "src",
            "HTMLInputElement:src",
            "read-write gchar* HTMLInputElement:src",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_TYPE,
        g_param_spec_string(
            "type",
            "HTMLInputElement:type",
            "read-write gchar* HTMLInputElement:type",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_DEFAULT_VALUE,
        g_param_spec_string(
            "default-value",
            "HTMLInputElement:default-value",
            "read-write gchar* HTMLInputElement:default-value",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_VALUE,
        g_param_spec_string(
            "value",
            "HTMLInputElement:value",
            "read-write gchar* HTMLInputElement:value",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_WIDTH,
        g_param_spec_ulong(
            "width",
            "HTMLInputElement:width",
            "read-write gulong HTMLInputElement:width",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_WILL_VALIDATE,
        g_param_spec_boolean(
            "will-validate",
            "HTMLInputElement:will-validate",
            "read-only gboolean HTMLInputElement:will-validate",
            FALSE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_ALIGN,
        g_param_spec_string(
            "align",
            "HTMLInputElement:align",
            "read-write gchar* HTMLInputElement:align",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_USE_MAP,
        g_param_spec_string(
            "use-map",
            "HTMLInputElement:use-map",
            "read-write gchar* HTMLInputElement:use-map",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_INPUT_ELEMENT_PROP_CAPTURE,
        g_param_spec_string(
            "capture",
            "HTMLInputElement:capture",
            "read-write gchar* HTMLInputElement:capture",
            "",
            WEBKIT_PARAM_READWRITE));

}

static void webkit_dom_html_input_element_init(CyberKitDOMHTMLInputElement* request)
{
    UNUSED_PARAM(request);
}

void webkit_dom_html_input_element_select(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    item->select();
}

gchar* webkit_dom_html_input_element_get_accept(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::acceptAttr));
    return result;
}

void webkit_dom_html_input_element_set_accept(CyberKitDOMHTMLInputElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::acceptAttr, convertedValue);
}

gchar* webkit_dom_html_input_element_get_alt(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::altAttr));
    return result;
}

void webkit_dom_html_input_element_set_alt(CyberKitDOMHTMLInputElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::altAttr, convertedValue);
}

gboolean webkit_dom_html_input_element_get_autofocus(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), FALSE);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gboolean result = item->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::autofocusAttr);
    return result;
}

void webkit_dom_html_input_element_set_autofocus(CyberKitDOMHTMLInputElement* self, gboolean value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    item->setBooleanAttribute(CyberCore::HTMLNames::autofocusAttr, value);
}

gboolean webkit_dom_html_input_element_get_default_checked(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), FALSE);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gboolean result = item->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::checkedAttr);
    return result;
}

void webkit_dom_html_input_element_set_default_checked(CyberKitDOMHTMLInputElement* self, gboolean value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    item->setBooleanAttribute(CyberCore::HTMLNames::checkedAttr, value);
}

gboolean webkit_dom_html_input_element_get_checked(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), FALSE);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gboolean result = item->checked();
    return result;
}

void webkit_dom_html_input_element_set_checked(CyberKitDOMHTMLInputElement* self, gboolean value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    item->setChecked(value);
}

gboolean webkit_dom_html_input_element_get_disabled(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), FALSE);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gboolean result = item->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::disabledAttr);
    return result;
}

void webkit_dom_html_input_element_set_disabled(CyberKitDOMHTMLInputElement* self, gboolean value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    item->setBooleanAttribute(CyberCore::HTMLNames::disabledAttr, value);
}

CyberKitDOMHTMLFormElement* webkit_dom_html_input_element_get_form(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLFormElement> gobjectResult = WTF::getPtr(item->form());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMFileList* webkit_dom_html_input_element_get_files(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    RefPtr<CyberCore::FileList> gobjectResult = WTF::getPtr(item->files());
    return CyberKit::kit(gobjectResult.get());
}

void webkit_dom_html_input_element_set_files(CyberKitDOMHTMLInputElement* self, CyberKitDOMFileList* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    g_return_if_fail(WEBKIT_DOM_IS_FILE_LIST(value));
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    CyberCore::FileList* convertedValue = CyberKit::core(value);
    item->setFiles(convertedValue);
}

gulong webkit_dom_html_input_element_get_height(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gulong result = item->height();
    return result;
}

void webkit_dom_html_input_element_set_height(CyberKitDOMHTMLInputElement* self, gulong value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    item->setHeight(value);
}

gboolean webkit_dom_html_input_element_get_indeterminate(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), FALSE);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gboolean result = item->indeterminate();
    return result;
}

void webkit_dom_html_input_element_set_indeterminate(CyberKitDOMHTMLInputElement* self, gboolean value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    item->setIndeterminate(value);
}

glong webkit_dom_html_input_element_get_max_length(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    glong result = item->maxLength();
    return result;
}

void webkit_dom_html_input_element_set_max_length(CyberKitDOMHTMLInputElement* self, glong value, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    g_return_if_fail(!error || !*error);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    auto result = item->setMaxLength(value);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

gboolean webkit_dom_html_input_element_get_multiple(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), FALSE);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gboolean result = item->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::multipleAttr);
    return result;
}

void webkit_dom_html_input_element_set_multiple(CyberKitDOMHTMLInputElement* self, gboolean value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    item->setBooleanAttribute(CyberCore::HTMLNames::multipleAttr, value);
}

gchar* webkit_dom_html_input_element_get_name(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->getNameAttribute());
    return result;
}

void webkit_dom_html_input_element_set_name(CyberKitDOMHTMLInputElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::nameAttr, convertedValue);
}

gboolean webkit_dom_html_input_element_get_read_only(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), FALSE);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gboolean result = item->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::readonlyAttr);
    return result;
}

void webkit_dom_html_input_element_set_read_only(CyberKitDOMHTMLInputElement* self, gboolean value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    item->setBooleanAttribute(CyberCore::HTMLNames::readonlyAttr, value);
}

gulong webkit_dom_html_input_element_get_size(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gulong result = item->size();
    return result;
}

void webkit_dom_html_input_element_set_size(CyberKitDOMHTMLInputElement* self, gulong value, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    g_return_if_fail(!error || !*error);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    auto result = item->setSize(value);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

gchar* webkit_dom_html_input_element_get_src(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->getURLAttribute(CyberCore::HTMLNames::srcAttr));
    return result;
}

void webkit_dom_html_input_element_set_src(CyberKitDOMHTMLInputElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::srcAttr, convertedValue);
}

gchar* webkit_dom_html_input_element_get_input_type(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->type());
    return result;
}

void webkit_dom_html_input_element_set_input_type(CyberKitDOMHTMLInputElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setType(convertedValue);
}

gchar* webkit_dom_html_input_element_get_default_value(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->defaultValue());
    return result;
}

void webkit_dom_html_input_element_set_default_value(CyberKitDOMHTMLInputElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setDefaultValue(convertedValue);
}

gchar* webkit_dom_html_input_element_get_value(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->value());
    return result;
}

void webkit_dom_html_input_element_set_value(CyberKitDOMHTMLInputElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setValue(convertedValue);
}

gulong webkit_dom_html_input_element_get_width(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gulong result = item->width();
    return result;
}

void webkit_dom_html_input_element_set_width(CyberKitDOMHTMLInputElement* self, gulong value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    item->setWidth(value);
}

gboolean webkit_dom_html_input_element_get_will_validate(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), FALSE);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gboolean result = item->willValidate();
    return result;
}

gchar* webkit_dom_html_input_element_get_align(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr));
    return result;
}

void webkit_dom_html_input_element_set_align(CyberKitDOMHTMLInputElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr, convertedValue);
}

gchar* webkit_dom_html_input_element_get_use_map(CyberKitDOMHTMLInputElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::usemapAttr));
    return result;
}

void webkit_dom_html_input_element_set_use_map(CyberKitDOMHTMLInputElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::usemapAttr, convertedValue);
}

gchar* webkit_dom_html_input_element_get_capture_type(CyberKitDOMHTMLInputElement* self)
{
#if ENABLE(MEDIA_CAPTURE)
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), 0);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::captureAttr));
    return result;
#else
    UNUSED_PARAM(self);
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Media Capture")
    return 0;
#endif /* ENABLE(MEDIA_CAPTURE) */
}

void webkit_dom_html_input_element_set_capture_type(CyberKitDOMHTMLInputElement* self, const gchar* value)
{
#if ENABLE(MEDIA_CAPTURE)
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::captureAttr, convertedValue);
#else
    UNUSED_PARAM(self);
    UNUSED_PARAM(value);
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Media Capture")
#endif /* ENABLE(MEDIA_CAPTURE) */
}

gboolean webkit_dom_html_input_element_is_edited(CyberKitDOMHTMLInputElement* input)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(input), FALSE);

    return CyberKit::core(input)->lastChangeWasUserEdit();
}

gboolean webkit_dom_html_input_element_get_auto_filled(CyberKitDOMHTMLInputElement* self)
{
  g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), FALSE);

  return CyberKit::core(self)->isAutoFilled();
}

void webkit_dom_html_input_element_set_auto_filled(CyberKitDOMHTMLInputElement* self, gboolean value)
{
  g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));

  CyberKit::core(self)->setAutoFilled(value);
}

void webkit_dom_html_input_element_set_editing_value(CyberKitDOMHTMLInputElement* self, const gchar* value)
{
  g_return_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self));
  g_return_if_fail(value);

  CyberKit::core(self)->setEditingValue(WTF::String::fromUTF8(value));
}
G_GNUC_END_IGNORE_DEPRECATIONS;
