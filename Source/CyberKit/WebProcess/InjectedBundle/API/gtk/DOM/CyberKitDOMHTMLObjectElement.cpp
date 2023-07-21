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
#include "CyberKitDOMHTMLObjectElement.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include "GObjectEventListener.h"
#include <CyberCore/HTMLNames.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMDocumentPrivate.h"
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMHTMLFormElementPrivate.h"
#include "CyberKitDOMHTMLObjectElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLObjectElement* kit(CyberCore::HTMLObjectElement* obj)
{
    return WEBKIT_DOM_HTML_OBJECT_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::HTMLObjectElement* core(CyberKitDOMHTMLObjectElement* request)
{
    return request ? static_cast<CyberCore::HTMLObjectElement*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLObjectElement* wrapHTMLObjectElement(CyberCore::HTMLObjectElement* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_OBJECT_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_HTML_OBJECT_ELEMENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_html_object_element_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::HTMLObjectElement* coreTarget = static_cast<CyberCore::HTMLObjectElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_html_object_element_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLObjectElement* coreTarget = static_cast<CyberCore::HTMLObjectElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_html_object_element_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLObjectElement* coreTarget = static_cast<CyberCore::HTMLObjectElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_html_object_element_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_html_object_element_dispatch_event;
    iface->add_event_listener = webkit_dom_html_object_element_add_event_listener;
    iface->remove_event_listener = webkit_dom_html_object_element_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMHTMLObjectElement, webkit_dom_html_object_element, WEBKIT_DOM_TYPE_HTML_ELEMENT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_html_object_element_dom_event_target_init))

enum {
    DOM_HTML_OBJECT_ELEMENT_PROP_0,
    DOM_HTML_OBJECT_ELEMENT_PROP_FORM,
    DOM_HTML_OBJECT_ELEMENT_PROP_CODE,
    DOM_HTML_OBJECT_ELEMENT_PROP_ALIGN,
    DOM_HTML_OBJECT_ELEMENT_PROP_ARCHIVE,
    DOM_HTML_OBJECT_ELEMENT_PROP_BORDER,
    DOM_HTML_OBJECT_ELEMENT_PROP_CODE_BASE,
    DOM_HTML_OBJECT_ELEMENT_PROP_CODE_TYPE,
    DOM_HTML_OBJECT_ELEMENT_PROP_DATA,
    DOM_HTML_OBJECT_ELEMENT_PROP_DECLARE,
    DOM_HTML_OBJECT_ELEMENT_PROP_HEIGHT,
    DOM_HTML_OBJECT_ELEMENT_PROP_HSPACE,
    DOM_HTML_OBJECT_ELEMENT_PROP_NAME,
    DOM_HTML_OBJECT_ELEMENT_PROP_STANDBY,
    DOM_HTML_OBJECT_ELEMENT_PROP_TYPE,
    DOM_HTML_OBJECT_ELEMENT_PROP_USE_MAP,
    DOM_HTML_OBJECT_ELEMENT_PROP_VSPACE,
    DOM_HTML_OBJECT_ELEMENT_PROP_WIDTH,
    DOM_HTML_OBJECT_ELEMENT_PROP_CONTENT_DOCUMENT,
};

static void webkit_dom_html_object_element_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLObjectElement* self = WEBKIT_DOM_HTML_OBJECT_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_OBJECT_ELEMENT_PROP_CODE:
        webkit_dom_html_object_element_set_code(self, g_value_get_string(value));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_ALIGN:
        webkit_dom_html_object_element_set_align(self, g_value_get_string(value));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_ARCHIVE:
        webkit_dom_html_object_element_set_archive(self, g_value_get_string(value));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_BORDER:
        webkit_dom_html_object_element_set_border(self, g_value_get_string(value));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_CODE_BASE:
        webkit_dom_html_object_element_set_code_base(self, g_value_get_string(value));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_CODE_TYPE:
        webkit_dom_html_object_element_set_code_type(self, g_value_get_string(value));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_DATA:
        webkit_dom_html_object_element_set_data(self, g_value_get_string(value));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_DECLARE:
        webkit_dom_html_object_element_set_declare(self, g_value_get_boolean(value));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_HEIGHT:
        webkit_dom_html_object_element_set_height(self, g_value_get_string(value));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_HSPACE:
        webkit_dom_html_object_element_set_hspace(self, g_value_get_long(value));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_NAME:
        webkit_dom_html_object_element_set_name(self, g_value_get_string(value));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_STANDBY:
        webkit_dom_html_object_element_set_standby(self, g_value_get_string(value));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_TYPE:
        webkit_dom_html_object_element_set_type_attr(self, g_value_get_string(value));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_USE_MAP:
        webkit_dom_html_object_element_set_use_map(self, g_value_get_string(value));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_VSPACE:
        webkit_dom_html_object_element_set_vspace(self, g_value_get_long(value));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_WIDTH:
        webkit_dom_html_object_element_set_width(self, g_value_get_string(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_object_element_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLObjectElement* self = WEBKIT_DOM_HTML_OBJECT_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_OBJECT_ELEMENT_PROP_FORM:
        g_value_set_object(value, webkit_dom_html_object_element_get_form(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_CODE:
        g_value_take_string(value, webkit_dom_html_object_element_get_code(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_ALIGN:
        g_value_take_string(value, webkit_dom_html_object_element_get_align(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_ARCHIVE:
        g_value_take_string(value, webkit_dom_html_object_element_get_archive(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_BORDER:
        g_value_take_string(value, webkit_dom_html_object_element_get_border(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_CODE_BASE:
        g_value_take_string(value, webkit_dom_html_object_element_get_code_base(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_CODE_TYPE:
        g_value_take_string(value, webkit_dom_html_object_element_get_code_type(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_DATA:
        g_value_take_string(value, webkit_dom_html_object_element_get_data(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_DECLARE:
        g_value_set_boolean(value, webkit_dom_html_object_element_get_declare(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_HEIGHT:
        g_value_take_string(value, webkit_dom_html_object_element_get_height(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_HSPACE:
        g_value_set_long(value, webkit_dom_html_object_element_get_hspace(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_NAME:
        g_value_take_string(value, webkit_dom_html_object_element_get_name(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_STANDBY:
        g_value_take_string(value, webkit_dom_html_object_element_get_standby(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_TYPE:
        g_value_take_string(value, webkit_dom_html_object_element_get_type_attr(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_USE_MAP:
        g_value_take_string(value, webkit_dom_html_object_element_get_use_map(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_VSPACE:
        g_value_set_long(value, webkit_dom_html_object_element_get_vspace(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_WIDTH:
        g_value_take_string(value, webkit_dom_html_object_element_get_width(self));
        break;
    case DOM_HTML_OBJECT_ELEMENT_PROP_CONTENT_DOCUMENT:
        g_value_set_object(value, webkit_dom_html_object_element_get_content_document(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_object_element_class_init(CyberKitDOMHTMLObjectElementClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_html_object_element_set_property;
    gobjectClass->get_property = webkit_dom_html_object_element_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_FORM,
        g_param_spec_object(
            "form",
            "HTMLObjectElement:form",
            "read-only CyberKitDOMHTMLFormElement* HTMLObjectElement:form",
            WEBKIT_DOM_TYPE_HTML_FORM_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_CODE,
        g_param_spec_string(
            "code",
            "HTMLObjectElement:code",
            "read-write gchar* HTMLObjectElement:code",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_ALIGN,
        g_param_spec_string(
            "align",
            "HTMLObjectElement:align",
            "read-write gchar* HTMLObjectElement:align",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_ARCHIVE,
        g_param_spec_string(
            "archive",
            "HTMLObjectElement:archive",
            "read-write gchar* HTMLObjectElement:archive",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_BORDER,
        g_param_spec_string(
            "border",
            "HTMLObjectElement:border",
            "read-write gchar* HTMLObjectElement:border",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_CODE_BASE,
        g_param_spec_string(
            "code-base",
            "HTMLObjectElement:code-base",
            "read-write gchar* HTMLObjectElement:code-base",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_CODE_TYPE,
        g_param_spec_string(
            "code-type",
            "HTMLObjectElement:code-type",
            "read-write gchar* HTMLObjectElement:code-type",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_DATA,
        g_param_spec_string(
            "data",
            "HTMLObjectElement:data",
            "read-write gchar* HTMLObjectElement:data",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_DECLARE,
        g_param_spec_boolean(
            "declare",
            "HTMLObjectElement:declare",
            "read-write gboolean HTMLObjectElement:declare",
            FALSE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_HEIGHT,
        g_param_spec_string(
            "height",
            "HTMLObjectElement:height",
            "read-write gchar* HTMLObjectElement:height",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_HSPACE,
        g_param_spec_long(
            "hspace",
            "HTMLObjectElement:hspace",
            "read-write glong HTMLObjectElement:hspace",
            G_MINLONG, G_MAXLONG, 0,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_NAME,
        g_param_spec_string(
            "name",
            "HTMLObjectElement:name",
            "read-write gchar* HTMLObjectElement:name",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_STANDBY,
        g_param_spec_string(
            "standby",
            "HTMLObjectElement:standby",
            "read-write gchar* HTMLObjectElement:standby",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_TYPE,
        g_param_spec_string(
            "type",
            "HTMLObjectElement:type",
            "read-write gchar* HTMLObjectElement:type",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_USE_MAP,
        g_param_spec_string(
            "use-map",
            "HTMLObjectElement:use-map",
            "read-write gchar* HTMLObjectElement:use-map",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_VSPACE,
        g_param_spec_long(
            "vspace",
            "HTMLObjectElement:vspace",
            "read-write glong HTMLObjectElement:vspace",
            G_MINLONG, G_MAXLONG, 0,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_WIDTH,
        g_param_spec_string(
            "width",
            "HTMLObjectElement:width",
            "read-write gchar* HTMLObjectElement:width",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_OBJECT_ELEMENT_PROP_CONTENT_DOCUMENT,
        g_param_spec_object(
            "content-document",
            "HTMLObjectElement:content-document",
            "read-only CyberKitDOMDocument* HTMLObjectElement:content-document",
            WEBKIT_DOM_TYPE_DOCUMENT,
            WEBKIT_PARAM_READABLE));

}

static void webkit_dom_html_object_element_init(CyberKitDOMHTMLObjectElement* request)
{
    UNUSED_PARAM(request);
}

CyberKitDOMHTMLFormElement* webkit_dom_html_object_element_get_form(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLFormElement> gobjectResult = WTF::getPtr(item->form());
    return CyberKit::kit(gobjectResult.get());
}

gchar* webkit_dom_html_object_element_get_code(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::codeAttr));
    return result;
}

void webkit_dom_html_object_element_set_code(CyberKitDOMHTMLObjectElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::codeAttr, convertedValue);
}

gchar* webkit_dom_html_object_element_get_align(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr));
    return result;
}

void webkit_dom_html_object_element_set_align(CyberKitDOMHTMLObjectElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr, convertedValue);
}

gchar* webkit_dom_html_object_element_get_archive(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::archiveAttr));
    return result;
}

void webkit_dom_html_object_element_set_archive(CyberKitDOMHTMLObjectElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::archiveAttr, convertedValue);
}

gchar* webkit_dom_html_object_element_get_border(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::borderAttr));
    return result;
}

void webkit_dom_html_object_element_set_border(CyberKitDOMHTMLObjectElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::borderAttr, convertedValue);
}

gchar* webkit_dom_html_object_element_get_code_base(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::codebaseAttr));
    return result;
}

void webkit_dom_html_object_element_set_code_base(CyberKitDOMHTMLObjectElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::codebaseAttr, convertedValue);
}

gchar* webkit_dom_html_object_element_get_code_type(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::codetypeAttr));
    return result;
}

void webkit_dom_html_object_element_set_code_type(CyberKitDOMHTMLObjectElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::codetypeAttr, convertedValue);
}

gchar* webkit_dom_html_object_element_get_data(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->getURLAttribute(CyberCore::HTMLNames::dataAttr));
    return result;
}

void webkit_dom_html_object_element_set_data(CyberKitDOMHTMLObjectElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::dataAttr, convertedValue);
}

gboolean webkit_dom_html_object_element_get_declare(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), FALSE);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    gboolean result = item->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::declareAttr);
    return result;
}

void webkit_dom_html_object_element_set_declare(CyberKitDOMHTMLObjectElement* self, gboolean value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    item->setBooleanAttribute(CyberCore::HTMLNames::declareAttr, value);
}

gchar* webkit_dom_html_object_element_get_height(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::heightAttr));
    return result;
}

void webkit_dom_html_object_element_set_height(CyberKitDOMHTMLObjectElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::heightAttr, convertedValue);
}

glong webkit_dom_html_object_element_get_hspace(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    glong result = item->getIntegralAttribute(CyberCore::HTMLNames::hspaceAttr);
    return result;
}

void webkit_dom_html_object_element_set_hspace(CyberKitDOMHTMLObjectElement* self, glong value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    item->setIntegralAttribute(CyberCore::HTMLNames::hspaceAttr, value);
}

gchar* webkit_dom_html_object_element_get_name(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->getNameAttribute());
    return result;
}

void webkit_dom_html_object_element_set_name(CyberKitDOMHTMLObjectElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::nameAttr, convertedValue);
}

gchar* webkit_dom_html_object_element_get_standby(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::standbyAttr));
    return result;
}

void webkit_dom_html_object_element_set_standby(CyberKitDOMHTMLObjectElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::standbyAttr, convertedValue);
}

gchar* webkit_dom_html_object_element_get_type_attr(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::typeAttr));
    return result;
}

void webkit_dom_html_object_element_set_type_attr(CyberKitDOMHTMLObjectElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::typeAttr, convertedValue);
}

gchar* webkit_dom_html_object_element_get_use_map(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::usemapAttr));
    return result;
}

void webkit_dom_html_object_element_set_use_map(CyberKitDOMHTMLObjectElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::usemapAttr, convertedValue);
}

glong webkit_dom_html_object_element_get_vspace(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    glong result = item->getIntegralAttribute(CyberCore::HTMLNames::vspaceAttr);
    return result;
}

void webkit_dom_html_object_element_set_vspace(CyberKitDOMHTMLObjectElement* self, glong value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    item->setIntegralAttribute(CyberCore::HTMLNames::vspaceAttr, value);
}

gchar* webkit_dom_html_object_element_get_width(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::widthAttr));
    return result;
}

void webkit_dom_html_object_element_set_width(CyberKitDOMHTMLObjectElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::widthAttr, convertedValue);
}

CyberKitDOMDocument* webkit_dom_html_object_element_get_content_document(CyberKitDOMHTMLObjectElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(self), 0);
    CyberCore::HTMLObjectElement* item = CyberKit::core(self);
    RefPtr<CyberCore::Document> gobjectResult = WTF::getPtr(item->contentDocument());
    return CyberKit::kit(gobjectResult.get());
}

G_GNUC_END_IGNORE_DEPRECATIONS;
