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
#include "CyberKitDOMHTMLEmbedElement.h"

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
#include "CyberKitDOMHTMLEmbedElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLEmbedElement* kit(CyberCore::HTMLEmbedElement* obj)
{
    return WEBKIT_DOM_HTML_EMBED_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::HTMLEmbedElement* core(CyberKitDOMHTMLEmbedElement* request)
{
    return request ? static_cast<CyberCore::HTMLEmbedElement*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLEmbedElement* wrapHTMLEmbedElement(CyberCore::HTMLEmbedElement* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_EMBED_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_HTML_EMBED_ELEMENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_html_embed_element_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::HTMLEmbedElement* coreTarget = static_cast<CyberCore::HTMLEmbedElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_html_embed_element_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLEmbedElement* coreTarget = static_cast<CyberCore::HTMLEmbedElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_html_embed_element_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLEmbedElement* coreTarget = static_cast<CyberCore::HTMLEmbedElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_html_embed_element_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_html_embed_element_dispatch_event;
    iface->add_event_listener = webkit_dom_html_embed_element_add_event_listener;
    iface->remove_event_listener = webkit_dom_html_embed_element_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMHTMLEmbedElement, webkit_dom_html_embed_element, WEBKIT_DOM_TYPE_HTML_ELEMENT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_html_embed_element_dom_event_target_init))

enum {
    DOM_HTML_EMBED_ELEMENT_PROP_0,
    DOM_HTML_EMBED_ELEMENT_PROP_ALIGN,
    DOM_HTML_EMBED_ELEMENT_PROP_HEIGHT,
    DOM_HTML_EMBED_ELEMENT_PROP_NAME,
    DOM_HTML_EMBED_ELEMENT_PROP_SRC,
    DOM_HTML_EMBED_ELEMENT_PROP_TYPE,
    DOM_HTML_EMBED_ELEMENT_PROP_WIDTH,
};

static void webkit_dom_html_embed_element_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLEmbedElement* self = WEBKIT_DOM_HTML_EMBED_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_EMBED_ELEMENT_PROP_ALIGN:
        webkit_dom_html_embed_element_set_align(self, g_value_get_string(value));
        break;
    case DOM_HTML_EMBED_ELEMENT_PROP_HEIGHT:
        webkit_dom_html_embed_element_set_height(self, g_value_get_long(value));
        break;
    case DOM_HTML_EMBED_ELEMENT_PROP_NAME:
        webkit_dom_html_embed_element_set_name(self, g_value_get_string(value));
        break;
    case DOM_HTML_EMBED_ELEMENT_PROP_SRC:
        webkit_dom_html_embed_element_set_src(self, g_value_get_string(value));
        break;
    case DOM_HTML_EMBED_ELEMENT_PROP_TYPE:
        webkit_dom_html_embed_element_set_type_attr(self, g_value_get_string(value));
        break;
    case DOM_HTML_EMBED_ELEMENT_PROP_WIDTH:
        webkit_dom_html_embed_element_set_width(self, g_value_get_long(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_embed_element_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLEmbedElement* self = WEBKIT_DOM_HTML_EMBED_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_EMBED_ELEMENT_PROP_ALIGN:
        g_value_take_string(value, webkit_dom_html_embed_element_get_align(self));
        break;
    case DOM_HTML_EMBED_ELEMENT_PROP_HEIGHT:
        g_value_set_long(value, webkit_dom_html_embed_element_get_height(self));
        break;
    case DOM_HTML_EMBED_ELEMENT_PROP_NAME:
        g_value_take_string(value, webkit_dom_html_embed_element_get_name(self));
        break;
    case DOM_HTML_EMBED_ELEMENT_PROP_SRC:
        g_value_take_string(value, webkit_dom_html_embed_element_get_src(self));
        break;
    case DOM_HTML_EMBED_ELEMENT_PROP_TYPE:
        g_value_take_string(value, webkit_dom_html_embed_element_get_type_attr(self));
        break;
    case DOM_HTML_EMBED_ELEMENT_PROP_WIDTH:
        g_value_set_long(value, webkit_dom_html_embed_element_get_width(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_embed_element_class_init(CyberKitDOMHTMLEmbedElementClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_html_embed_element_set_property;
    gobjectClass->get_property = webkit_dom_html_embed_element_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_EMBED_ELEMENT_PROP_ALIGN,
        g_param_spec_string(
            "align",
            "HTMLEmbedElement:align",
            "read-write gchar* HTMLEmbedElement:align",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_EMBED_ELEMENT_PROP_HEIGHT,
        g_param_spec_long(
            "height",
            "HTMLEmbedElement:height",
            "read-write glong HTMLEmbedElement:height",
            G_MINLONG, G_MAXLONG, 0,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_EMBED_ELEMENT_PROP_NAME,
        g_param_spec_string(
            "name",
            "HTMLEmbedElement:name",
            "read-write gchar* HTMLEmbedElement:name",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_EMBED_ELEMENT_PROP_SRC,
        g_param_spec_string(
            "src",
            "HTMLEmbedElement:src",
            "read-write gchar* HTMLEmbedElement:src",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_EMBED_ELEMENT_PROP_TYPE,
        g_param_spec_string(
            "type",
            "HTMLEmbedElement:type",
            "read-write gchar* HTMLEmbedElement:type",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_EMBED_ELEMENT_PROP_WIDTH,
        g_param_spec_long(
            "width",
            "HTMLEmbedElement:width",
            "read-write glong HTMLEmbedElement:width",
            G_MINLONG, G_MAXLONG, 0,
            WEBKIT_PARAM_READWRITE));

}

static void webkit_dom_html_embed_element_init(CyberKitDOMHTMLEmbedElement* request)
{
    UNUSED_PARAM(request);
}

gchar* webkit_dom_html_embed_element_get_align(CyberKitDOMHTMLEmbedElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_EMBED_ELEMENT(self), 0);
    CyberCore::HTMLEmbedElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr));
    return result;
}

void webkit_dom_html_embed_element_set_align(CyberKitDOMHTMLEmbedElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_EMBED_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLEmbedElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr, WTF::AtomString::fromUTF8(value));
}

glong webkit_dom_html_embed_element_get_height(CyberKitDOMHTMLEmbedElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_EMBED_ELEMENT(self), 0);
    CyberCore::HTMLEmbedElement* item = CyberKit::core(self);
    glong result = item->getIntegralAttribute(CyberCore::HTMLNames::heightAttr);
    return result;
}

void webkit_dom_html_embed_element_set_height(CyberKitDOMHTMLEmbedElement* self, glong value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_EMBED_ELEMENT(self));
    CyberCore::HTMLEmbedElement* item = CyberKit::core(self);
    item->setIntegralAttribute(CyberCore::HTMLNames::heightAttr, value);
}

gchar* webkit_dom_html_embed_element_get_name(CyberKitDOMHTMLEmbedElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_EMBED_ELEMENT(self), 0);
    CyberCore::HTMLEmbedElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->getNameAttribute());
    return result;
}

void webkit_dom_html_embed_element_set_name(CyberKitDOMHTMLEmbedElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_EMBED_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLEmbedElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::nameAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_embed_element_get_src(CyberKitDOMHTMLEmbedElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_EMBED_ELEMENT(self), 0);
    CyberCore::HTMLEmbedElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->getURLAttribute(CyberCore::HTMLNames::srcAttr));
    return result;
}

void webkit_dom_html_embed_element_set_src(CyberKitDOMHTMLEmbedElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_EMBED_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLEmbedElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::srcAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_embed_element_get_type_attr(CyberKitDOMHTMLEmbedElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_EMBED_ELEMENT(self), 0);
    CyberCore::HTMLEmbedElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::typeAttr));
    return result;
}

void webkit_dom_html_embed_element_set_type_attr(CyberKitDOMHTMLEmbedElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_EMBED_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLEmbedElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::typeAttr, WTF::AtomString::fromUTF8(value));
}

glong webkit_dom_html_embed_element_get_width(CyberKitDOMHTMLEmbedElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_EMBED_ELEMENT(self), 0);
    CyberCore::HTMLEmbedElement* item = CyberKit::core(self);
    glong result = item->getIntegralAttribute(CyberCore::HTMLNames::widthAttr);
    return result;
}

void webkit_dom_html_embed_element_set_width(CyberKitDOMHTMLEmbedElement* self, glong value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_EMBED_ELEMENT(self));
    CyberCore::HTMLEmbedElement* item = CyberKit::core(self);
    item->setIntegralAttribute(CyberCore::HTMLNames::widthAttr, value);
}

G_GNUC_END_IGNORE_DEPRECATIONS;
