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
#include "CyberKitDOMHTMLScriptElement.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include <CyberCore/ElementInlines.h>
#include <CyberCore/HTMLNames.h>
#include <CyberCore/JSExecState.h>
#include "GObjectEventListener.h"
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMHTMLScriptElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLScriptElement* kit(CyberCore::HTMLScriptElement* obj)
{
    return WEBKIT_DOM_HTML_SCRIPT_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::HTMLScriptElement* core(CyberKitDOMHTMLScriptElement* request)
{
    return request ? static_cast<CyberCore::HTMLScriptElement*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLScriptElement* wrapHTMLScriptElement(CyberCore::HTMLScriptElement* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_SCRIPT_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_HTML_SCRIPT_ELEMENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_html_script_element_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::HTMLScriptElement* coreTarget = static_cast<CyberCore::HTMLScriptElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_html_script_element_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLScriptElement* coreTarget = static_cast<CyberCore::HTMLScriptElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_html_script_element_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLScriptElement* coreTarget = static_cast<CyberCore::HTMLScriptElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_html_script_element_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_html_script_element_dispatch_event;
    iface->add_event_listener = webkit_dom_html_script_element_add_event_listener;
    iface->remove_event_listener = webkit_dom_html_script_element_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMHTMLScriptElement, webkit_dom_html_script_element, WEBKIT_DOM_TYPE_HTML_ELEMENT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_html_script_element_dom_event_target_init))

enum {
    DOM_HTML_SCRIPT_ELEMENT_PROP_0,
    DOM_HTML_SCRIPT_ELEMENT_PROP_TEXT,
    DOM_HTML_SCRIPT_ELEMENT_PROP_HTML_FOR,
    DOM_HTML_SCRIPT_ELEMENT_PROP_EVENT,
    DOM_HTML_SCRIPT_ELEMENT_PROP_CHARSET,
    DOM_HTML_SCRIPT_ELEMENT_PROP_DEFER,
    DOM_HTML_SCRIPT_ELEMENT_PROP_SRC,
    DOM_HTML_SCRIPT_ELEMENT_PROP_TYPE,
};

static void webkit_dom_html_script_element_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLScriptElement* self = WEBKIT_DOM_HTML_SCRIPT_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_SCRIPT_ELEMENT_PROP_TEXT:
        webkit_dom_html_script_element_set_text(self, g_value_get_string(value));
        break;
    case DOM_HTML_SCRIPT_ELEMENT_PROP_HTML_FOR:
        webkit_dom_html_script_element_set_html_for(self, g_value_get_string(value));
        break;
    case DOM_HTML_SCRIPT_ELEMENT_PROP_EVENT:
        webkit_dom_html_script_element_set_event(self, g_value_get_string(value));
        break;
    case DOM_HTML_SCRIPT_ELEMENT_PROP_CHARSET:
        webkit_dom_html_script_element_set_charset(self, g_value_get_string(value));
        break;
    case DOM_HTML_SCRIPT_ELEMENT_PROP_DEFER:
        webkit_dom_html_script_element_set_defer(self, g_value_get_boolean(value));
        break;
    case DOM_HTML_SCRIPT_ELEMENT_PROP_SRC:
        webkit_dom_html_script_element_set_src(self, g_value_get_string(value));
        break;
    case DOM_HTML_SCRIPT_ELEMENT_PROP_TYPE:
        webkit_dom_html_script_element_set_type_attr(self, g_value_get_string(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_script_element_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLScriptElement* self = WEBKIT_DOM_HTML_SCRIPT_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_SCRIPT_ELEMENT_PROP_TEXT:
        g_value_take_string(value, webkit_dom_html_script_element_get_text(self));
        break;
    case DOM_HTML_SCRIPT_ELEMENT_PROP_HTML_FOR:
        g_value_take_string(value, webkit_dom_html_script_element_get_html_for(self));
        break;
    case DOM_HTML_SCRIPT_ELEMENT_PROP_EVENT:
        g_value_take_string(value, webkit_dom_html_script_element_get_event(self));
        break;
    case DOM_HTML_SCRIPT_ELEMENT_PROP_CHARSET:
        g_value_take_string(value, webkit_dom_html_script_element_get_charset(self));
        break;
    case DOM_HTML_SCRIPT_ELEMENT_PROP_DEFER:
        g_value_set_boolean(value, webkit_dom_html_script_element_get_defer(self));
        break;
    case DOM_HTML_SCRIPT_ELEMENT_PROP_SRC:
        g_value_take_string(value, webkit_dom_html_script_element_get_src(self));
        break;
    case DOM_HTML_SCRIPT_ELEMENT_PROP_TYPE:
        g_value_take_string(value, webkit_dom_html_script_element_get_type_attr(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_script_element_class_init(CyberKitDOMHTMLScriptElementClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_html_script_element_set_property;
    gobjectClass->get_property = webkit_dom_html_script_element_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_SCRIPT_ELEMENT_PROP_TEXT,
        g_param_spec_string(
            "text",
            "HTMLScriptElement:text",
            "read-write gchar* HTMLScriptElement:text",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_SCRIPT_ELEMENT_PROP_HTML_FOR,
        g_param_spec_string(
            "html-for",
            "HTMLScriptElement:html-for",
            "read-write gchar* HTMLScriptElement:html-for",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_SCRIPT_ELEMENT_PROP_EVENT,
        g_param_spec_string(
            "event",
            "HTMLScriptElement:event",
            "read-write gchar* HTMLScriptElement:event",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_SCRIPT_ELEMENT_PROP_CHARSET,
        g_param_spec_string(
            "charset",
            "HTMLScriptElement:charset",
            "read-write gchar* HTMLScriptElement:charset",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_SCRIPT_ELEMENT_PROP_DEFER,
        g_param_spec_boolean(
            "defer",
            "HTMLScriptElement:defer",
            "read-write gboolean HTMLScriptElement:defer",
            FALSE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_SCRIPT_ELEMENT_PROP_SRC,
        g_param_spec_string(
            "src",
            "HTMLScriptElement:src",
            "read-write gchar* HTMLScriptElement:src",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_SCRIPT_ELEMENT_PROP_TYPE,
        g_param_spec_string(
            "type",
            "HTMLScriptElement:type",
            "read-write gchar* HTMLScriptElement:type",
            "",
            WEBKIT_PARAM_READWRITE));
}

static void webkit_dom_html_script_element_init(CyberKitDOMHTMLScriptElement* request)
{
    UNUSED_PARAM(request);
}

gchar* webkit_dom_html_script_element_get_text(CyberKitDOMHTMLScriptElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT(self), 0);
    CyberCore::HTMLScriptElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->text());
    return result;
}

void webkit_dom_html_script_element_set_text(CyberKitDOMHTMLScriptElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLScriptElement* item = CyberKit::core(self);
    item->setText(WTF::String::fromUTF8(value));
}

gchar* webkit_dom_html_script_element_get_html_for(CyberKitDOMHTMLScriptElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT(self), 0);
    CyberCore::HTMLScriptElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::forAttr));
    return result;
}

void webkit_dom_html_script_element_set_html_for(CyberKitDOMHTMLScriptElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLScriptElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::forAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_script_element_get_event(CyberKitDOMHTMLScriptElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT(self), 0);
    CyberCore::HTMLScriptElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::eventAttr));
    return result;
}

void webkit_dom_html_script_element_set_event(CyberKitDOMHTMLScriptElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLScriptElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::eventAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_script_element_get_charset(CyberKitDOMHTMLScriptElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT(self), 0);
    CyberCore::HTMLScriptElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::charsetAttr));
    return result;
}

void webkit_dom_html_script_element_set_charset(CyberKitDOMHTMLScriptElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLScriptElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::charsetAttr, WTF::AtomString::fromUTF8(value));
}

gboolean webkit_dom_html_script_element_get_defer(CyberKitDOMHTMLScriptElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT(self), FALSE);
    CyberCore::HTMLScriptElement* item = CyberKit::core(self);
    gboolean result = item->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::deferAttr);
    return result;
}

void webkit_dom_html_script_element_set_defer(CyberKitDOMHTMLScriptElement* self, gboolean value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT(self));
    CyberCore::HTMLScriptElement* item = CyberKit::core(self);
    item->setBooleanAttribute(CyberCore::HTMLNames::deferAttr, value);
}

gchar* webkit_dom_html_script_element_get_src(CyberKitDOMHTMLScriptElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT(self), 0);
    CyberCore::HTMLScriptElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->getURLAttribute(CyberCore::HTMLNames::srcAttr));
    return result;
}

void webkit_dom_html_script_element_set_src(CyberKitDOMHTMLScriptElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLScriptElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::srcAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_script_element_get_type_attr(CyberKitDOMHTMLScriptElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT(self), 0);
    CyberCore::HTMLScriptElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::typeAttr));
    return result;
}

void webkit_dom_html_script_element_set_type_attr(CyberKitDOMHTMLScriptElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_SCRIPT_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLScriptElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::typeAttr, WTF::AtomString::fromUTF8(value));
}
G_GNUC_END_IGNORE_DEPRECATIONS;
