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
#include "CyberKitDOMHTMLBaseElement.h"

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
#include "CyberKitDOMHTMLBaseElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLBaseElement* kit(CyberCore::HTMLBaseElement* obj)
{
    return WEBKIT_DOM_HTML_BASE_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::HTMLBaseElement* core(CyberKitDOMHTMLBaseElement* request)
{
    return request ? static_cast<CyberCore::HTMLBaseElement*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLBaseElement* wrapHTMLBaseElement(CyberCore::HTMLBaseElement* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_BASE_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_HTML_BASE_ELEMENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_html_base_element_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::HTMLBaseElement* coreTarget = static_cast<CyberCore::HTMLBaseElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_html_base_element_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLBaseElement* coreTarget = static_cast<CyberCore::HTMLBaseElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_html_base_element_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLBaseElement* coreTarget = static_cast<CyberCore::HTMLBaseElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_html_base_element_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_html_base_element_dispatch_event;
    iface->add_event_listener = webkit_dom_html_base_element_add_event_listener;
    iface->remove_event_listener = webkit_dom_html_base_element_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMHTMLBaseElement, webkit_dom_html_base_element, WEBKIT_DOM_TYPE_HTML_ELEMENT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_html_base_element_dom_event_target_init))

enum {
    DOM_HTML_BASE_ELEMENT_PROP_0,
    DOM_HTML_BASE_ELEMENT_PROP_HREF,
    DOM_HTML_BASE_ELEMENT_PROP_TARGET,
};

static void webkit_dom_html_base_element_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLBaseElement* self = WEBKIT_DOM_HTML_BASE_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_BASE_ELEMENT_PROP_HREF:
        webkit_dom_html_base_element_set_href(self, g_value_get_string(value));
        break;
    case DOM_HTML_BASE_ELEMENT_PROP_TARGET:
        webkit_dom_html_base_element_set_target(self, g_value_get_string(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_base_element_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLBaseElement* self = WEBKIT_DOM_HTML_BASE_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_BASE_ELEMENT_PROP_HREF:
        g_value_take_string(value, webkit_dom_html_base_element_get_href(self));
        break;
    case DOM_HTML_BASE_ELEMENT_PROP_TARGET:
        g_value_take_string(value, webkit_dom_html_base_element_get_target(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_base_element_class_init(CyberKitDOMHTMLBaseElementClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_html_base_element_set_property;
    gobjectClass->get_property = webkit_dom_html_base_element_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_BASE_ELEMENT_PROP_HREF,
        g_param_spec_string(
            "href",
            "HTMLBaseElement:href",
            "read-write gchar* HTMLBaseElement:href",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_BASE_ELEMENT_PROP_TARGET,
        g_param_spec_string(
            "target",
            "HTMLBaseElement:target",
            "read-write gchar* HTMLBaseElement:target",
            "",
            WEBKIT_PARAM_READWRITE));

}

static void webkit_dom_html_base_element_init(CyberKitDOMHTMLBaseElement* request)
{
    UNUSED_PARAM(request);
}

gchar* webkit_dom_html_base_element_get_href(CyberKitDOMHTMLBaseElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_BASE_ELEMENT(self), 0);
    CyberCore::HTMLBaseElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->href());
    return result;
}

void webkit_dom_html_base_element_set_href(CyberKitDOMHTMLBaseElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_BASE_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLBaseElement* item = CyberKit::core(self);
    item->setHref(WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_base_element_get_target(CyberKitDOMHTMLBaseElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_BASE_ELEMENT(self), 0);
    CyberCore::HTMLBaseElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::targetAttr));
    return result;
}

void webkit_dom_html_base_element_set_target(CyberKitDOMHTMLBaseElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_BASE_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLBaseElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::targetAttr, WTF::AtomString::fromUTF8(value));
}

G_GNUC_END_IGNORE_DEPRECATIONS;
