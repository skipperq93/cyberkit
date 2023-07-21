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
#include "CyberKitDOMHTMLDivElement.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include "GObjectEventListener.h"
#include <CyberCore/HTMLNames.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMHTMLDivElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLDivElement* kit(CyberCore::HTMLDivElement* obj)
{
    return WEBKIT_DOM_HTML_DIV_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::HTMLDivElement* core(CyberKitDOMHTMLDivElement* request)
{
    return request ? static_cast<CyberCore::HTMLDivElement*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLDivElement* wrapHTMLDivElement(CyberCore::HTMLDivElement* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_DIV_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_HTML_DIV_ELEMENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_html_div_element_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::HTMLDivElement* coreTarget = static_cast<CyberCore::HTMLDivElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_html_div_element_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLDivElement* coreTarget = static_cast<CyberCore::HTMLDivElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_html_div_element_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLDivElement* coreTarget = static_cast<CyberCore::HTMLDivElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_html_div_element_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_html_div_element_dispatch_event;
    iface->add_event_listener = webkit_dom_html_div_element_add_event_listener;
    iface->remove_event_listener = webkit_dom_html_div_element_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMHTMLDivElement, webkit_dom_html_div_element, WEBKIT_DOM_TYPE_HTML_ELEMENT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_html_div_element_dom_event_target_init))

enum {
    DOM_HTML_DIV_ELEMENT_PROP_0,
    DOM_HTML_DIV_ELEMENT_PROP_ALIGN,
};

static void webkit_dom_html_div_element_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLDivElement* self = WEBKIT_DOM_HTML_DIV_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_DIV_ELEMENT_PROP_ALIGN:
        webkit_dom_html_div_element_set_align(self, g_value_get_string(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_div_element_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLDivElement* self = WEBKIT_DOM_HTML_DIV_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_DIV_ELEMENT_PROP_ALIGN:
        g_value_take_string(value, webkit_dom_html_div_element_get_align(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_div_element_class_init(CyberKitDOMHTMLDivElementClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_html_div_element_set_property;
    gobjectClass->get_property = webkit_dom_html_div_element_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_DIV_ELEMENT_PROP_ALIGN,
        g_param_spec_string(
            "align",
            "HTMLDivElement:align",
            "read-write gchar* HTMLDivElement:align",
            "",
            WEBKIT_PARAM_READWRITE));

}

static void webkit_dom_html_div_element_init(CyberKitDOMHTMLDivElement* request)
{
    UNUSED_PARAM(request);
}

gchar* webkit_dom_html_div_element_get_align(CyberKitDOMHTMLDivElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_DIV_ELEMENT(self), 0);
    CyberCore::HTMLDivElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr));
    return result;
}

void webkit_dom_html_div_element_set_align(CyberKitDOMHTMLDivElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_DIV_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLDivElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr, convertedValue);
}

G_GNUC_END_IGNORE_DEPRECATIONS;
