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
#include "CyberKitDOMHTMLMetaElement.h"

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
#include "CyberKitDOMHTMLMetaElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLMetaElement* kit(CyberCore::HTMLMetaElement* obj)
{
    return WEBKIT_DOM_HTML_META_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::HTMLMetaElement* core(CyberKitDOMHTMLMetaElement* request)
{
    return request ? static_cast<CyberCore::HTMLMetaElement*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLMetaElement* wrapHTMLMetaElement(CyberCore::HTMLMetaElement* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_META_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_HTML_META_ELEMENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_html_meta_element_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::HTMLMetaElement* coreTarget = static_cast<CyberCore::HTMLMetaElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_html_meta_element_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLMetaElement* coreTarget = static_cast<CyberCore::HTMLMetaElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_html_meta_element_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLMetaElement* coreTarget = static_cast<CyberCore::HTMLMetaElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_html_meta_element_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_html_meta_element_dispatch_event;
    iface->add_event_listener = webkit_dom_html_meta_element_add_event_listener;
    iface->remove_event_listener = webkit_dom_html_meta_element_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMHTMLMetaElement, webkit_dom_html_meta_element, WEBKIT_DOM_TYPE_HTML_ELEMENT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_html_meta_element_dom_event_target_init))

enum {
    DOM_HTML_META_ELEMENT_PROP_0,
    DOM_HTML_META_ELEMENT_PROP_CONTENT,
    DOM_HTML_META_ELEMENT_PROP_HTTP_EQUIV,
    DOM_HTML_META_ELEMENT_PROP_NAME,
    DOM_HTML_META_ELEMENT_PROP_SCHEME,
};

static void webkit_dom_html_meta_element_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLMetaElement* self = WEBKIT_DOM_HTML_META_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_META_ELEMENT_PROP_CONTENT:
        webkit_dom_html_meta_element_set_content(self, g_value_get_string(value));
        break;
    case DOM_HTML_META_ELEMENT_PROP_HTTP_EQUIV:
        webkit_dom_html_meta_element_set_http_equiv(self, g_value_get_string(value));
        break;
    case DOM_HTML_META_ELEMENT_PROP_NAME:
        webkit_dom_html_meta_element_set_name(self, g_value_get_string(value));
        break;
    case DOM_HTML_META_ELEMENT_PROP_SCHEME:
        webkit_dom_html_meta_element_set_scheme(self, g_value_get_string(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_meta_element_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLMetaElement* self = WEBKIT_DOM_HTML_META_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_META_ELEMENT_PROP_CONTENT:
        g_value_take_string(value, webkit_dom_html_meta_element_get_content(self));
        break;
    case DOM_HTML_META_ELEMENT_PROP_HTTP_EQUIV:
        g_value_take_string(value, webkit_dom_html_meta_element_get_http_equiv(self));
        break;
    case DOM_HTML_META_ELEMENT_PROP_NAME:
        g_value_take_string(value, webkit_dom_html_meta_element_get_name(self));
        break;
    case DOM_HTML_META_ELEMENT_PROP_SCHEME:
        g_value_take_string(value, webkit_dom_html_meta_element_get_scheme(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_meta_element_class_init(CyberKitDOMHTMLMetaElementClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_html_meta_element_set_property;
    gobjectClass->get_property = webkit_dom_html_meta_element_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_META_ELEMENT_PROP_CONTENT,
        g_param_spec_string(
            "content",
            "HTMLMetaElement:content",
            "read-write gchar* HTMLMetaElement:content",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_META_ELEMENT_PROP_HTTP_EQUIV,
        g_param_spec_string(
            "http-equiv",
            "HTMLMetaElement:http-equiv",
            "read-write gchar* HTMLMetaElement:http-equiv",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_META_ELEMENT_PROP_NAME,
        g_param_spec_string(
            "name",
            "HTMLMetaElement:name",
            "read-write gchar* HTMLMetaElement:name",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_META_ELEMENT_PROP_SCHEME,
        g_param_spec_string(
            "scheme",
            "HTMLMetaElement:scheme",
            "read-write gchar* HTMLMetaElement:scheme",
            "",
            WEBKIT_PARAM_READWRITE));

}

static void webkit_dom_html_meta_element_init(CyberKitDOMHTMLMetaElement* request)
{
    UNUSED_PARAM(request);
}

gchar* webkit_dom_html_meta_element_get_content(CyberKitDOMHTMLMetaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_META_ELEMENT(self), 0);
    CyberCore::HTMLMetaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::contentAttr));
    return result;
}

void webkit_dom_html_meta_element_set_content(CyberKitDOMHTMLMetaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_META_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLMetaElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::contentAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_meta_element_get_http_equiv(CyberKitDOMHTMLMetaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_META_ELEMENT(self), 0);
    CyberCore::HTMLMetaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::http_equivAttr));
    return result;
}

void webkit_dom_html_meta_element_set_http_equiv(CyberKitDOMHTMLMetaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_META_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLMetaElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::http_equivAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_meta_element_get_name(CyberKitDOMHTMLMetaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_META_ELEMENT(self), 0);
    CyberCore::HTMLMetaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->getNameAttribute());
    return result;
}

void webkit_dom_html_meta_element_set_name(CyberKitDOMHTMLMetaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_META_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLMetaElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::nameAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_meta_element_get_scheme(CyberKitDOMHTMLMetaElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_META_ELEMENT(self), 0);
    CyberCore::HTMLMetaElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::schemeAttr));
    return result;
}

void webkit_dom_html_meta_element_set_scheme(CyberKitDOMHTMLMetaElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_META_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLMetaElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::schemeAttr, WTF::AtomString::fromUTF8(value));
}

G_GNUC_END_IGNORE_DEPRECATIONS;
