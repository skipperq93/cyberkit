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
#include "CyberKitDOMHTMLTableColElement.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include "GObjectEventListener.h"
#include <CyberCore/HTMLNames.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMHTMLTableColElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLTableColElement* kit(CyberCore::HTMLTableColElement* obj)
{
    return WEBKIT_DOM_HTML_TABLE_COL_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::HTMLTableColElement* core(CyberKitDOMHTMLTableColElement* request)
{
    return request ? static_cast<CyberCore::HTMLTableColElement*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLTableColElement* wrapHTMLTableColElement(CyberCore::HTMLTableColElement* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_TABLE_COL_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_HTML_TABLE_COL_ELEMENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_html_table_col_element_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::HTMLTableColElement* coreTarget = static_cast<CyberCore::HTMLTableColElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_html_table_col_element_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLTableColElement* coreTarget = static_cast<CyberCore::HTMLTableColElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_html_table_col_element_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLTableColElement* coreTarget = static_cast<CyberCore::HTMLTableColElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_html_table_col_element_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_html_table_col_element_dispatch_event;
    iface->add_event_listener = webkit_dom_html_table_col_element_add_event_listener;
    iface->remove_event_listener = webkit_dom_html_table_col_element_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMHTMLTableColElement, webkit_dom_html_table_col_element, WEBKIT_DOM_TYPE_HTML_ELEMENT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_html_table_col_element_dom_event_target_init))

enum {
    DOM_HTML_TABLE_COL_ELEMENT_PROP_0,
    DOM_HTML_TABLE_COL_ELEMENT_PROP_ALIGN,
    DOM_HTML_TABLE_COL_ELEMENT_PROP_CH,
    DOM_HTML_TABLE_COL_ELEMENT_PROP_CH_OFF,
    DOM_HTML_TABLE_COL_ELEMENT_PROP_SPAN,
    DOM_HTML_TABLE_COL_ELEMENT_PROP_V_ALIGN,
    DOM_HTML_TABLE_COL_ELEMENT_PROP_WIDTH,
};

static void webkit_dom_html_table_col_element_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLTableColElement* self = WEBKIT_DOM_HTML_TABLE_COL_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_TABLE_COL_ELEMENT_PROP_ALIGN:
        webkit_dom_html_table_col_element_set_align(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_COL_ELEMENT_PROP_CH:
        webkit_dom_html_table_col_element_set_ch(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_COL_ELEMENT_PROP_CH_OFF:
        webkit_dom_html_table_col_element_set_ch_off(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_COL_ELEMENT_PROP_SPAN:
        webkit_dom_html_table_col_element_set_span(self, g_value_get_long(value));
        break;
    case DOM_HTML_TABLE_COL_ELEMENT_PROP_V_ALIGN:
        webkit_dom_html_table_col_element_set_v_align(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_COL_ELEMENT_PROP_WIDTH:
        webkit_dom_html_table_col_element_set_width(self, g_value_get_string(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_table_col_element_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLTableColElement* self = WEBKIT_DOM_HTML_TABLE_COL_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_TABLE_COL_ELEMENT_PROP_ALIGN:
        g_value_take_string(value, webkit_dom_html_table_col_element_get_align(self));
        break;
    case DOM_HTML_TABLE_COL_ELEMENT_PROP_CH:
        g_value_take_string(value, webkit_dom_html_table_col_element_get_ch(self));
        break;
    case DOM_HTML_TABLE_COL_ELEMENT_PROP_CH_OFF:
        g_value_take_string(value, webkit_dom_html_table_col_element_get_ch_off(self));
        break;
    case DOM_HTML_TABLE_COL_ELEMENT_PROP_SPAN:
        g_value_set_long(value, webkit_dom_html_table_col_element_get_span(self));
        break;
    case DOM_HTML_TABLE_COL_ELEMENT_PROP_V_ALIGN:
        g_value_take_string(value, webkit_dom_html_table_col_element_get_v_align(self));
        break;
    case DOM_HTML_TABLE_COL_ELEMENT_PROP_WIDTH:
        g_value_take_string(value, webkit_dom_html_table_col_element_get_width(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_table_col_element_class_init(CyberKitDOMHTMLTableColElementClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_html_table_col_element_set_property;
    gobjectClass->get_property = webkit_dom_html_table_col_element_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_COL_ELEMENT_PROP_ALIGN,
        g_param_spec_string(
            "align",
            "HTMLTableColElement:align",
            "read-write gchar* HTMLTableColElement:align",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_COL_ELEMENT_PROP_CH,
        g_param_spec_string(
            "ch",
            "HTMLTableColElement:ch",
            "read-write gchar* HTMLTableColElement:ch",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_COL_ELEMENT_PROP_CH_OFF,
        g_param_spec_string(
            "ch-off",
            "HTMLTableColElement:ch-off",
            "read-write gchar* HTMLTableColElement:ch-off",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_COL_ELEMENT_PROP_SPAN,
        g_param_spec_long(
            "span",
            "HTMLTableColElement:span",
            "read-write glong HTMLTableColElement:span",
            G_MINLONG, G_MAXLONG, 0,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_COL_ELEMENT_PROP_V_ALIGN,
        g_param_spec_string(
            "v-align",
            "HTMLTableColElement:v-align",
            "read-write gchar* HTMLTableColElement:v-align",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_COL_ELEMENT_PROP_WIDTH,
        g_param_spec_string(
            "width",
            "HTMLTableColElement:width",
            "read-write gchar* HTMLTableColElement:width",
            "",
            WEBKIT_PARAM_READWRITE));

}

static void webkit_dom_html_table_col_element_init(CyberKitDOMHTMLTableColElement* request)
{
    UNUSED_PARAM(request);
}

gchar* webkit_dom_html_table_col_element_get_align(CyberKitDOMHTMLTableColElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_COL_ELEMENT(self), 0);
    CyberCore::HTMLTableColElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr));
    return result;
}

void webkit_dom_html_table_col_element_set_align(CyberKitDOMHTMLTableColElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_COL_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableColElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr, convertedValue);
}

gchar* webkit_dom_html_table_col_element_get_ch(CyberKitDOMHTMLTableColElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_COL_ELEMENT(self), 0);
    CyberCore::HTMLTableColElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::charAttr));
    return result;
}

void webkit_dom_html_table_col_element_set_ch(CyberKitDOMHTMLTableColElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_COL_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableColElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::charAttr, convertedValue);
}

gchar* webkit_dom_html_table_col_element_get_ch_off(CyberKitDOMHTMLTableColElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_COL_ELEMENT(self), 0);
    CyberCore::HTMLTableColElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::charoffAttr));
    return result;
}

void webkit_dom_html_table_col_element_set_ch_off(CyberKitDOMHTMLTableColElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_COL_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableColElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::charoffAttr, convertedValue);
}

glong webkit_dom_html_table_col_element_get_span(CyberKitDOMHTMLTableColElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_COL_ELEMENT(self), 0);
    CyberCore::HTMLTableColElement* item = CyberKit::core(self);
    glong result = item->span();
    return result;
}

void webkit_dom_html_table_col_element_set_span(CyberKitDOMHTMLTableColElement* self, glong value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_COL_ELEMENT(self));
    CyberCore::HTMLTableColElement* item = CyberKit::core(self);
    item->setSpan(value);
}

gchar* webkit_dom_html_table_col_element_get_v_align(CyberKitDOMHTMLTableColElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_COL_ELEMENT(self), 0);
    CyberCore::HTMLTableColElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::valignAttr));
    return result;
}

void webkit_dom_html_table_col_element_set_v_align(CyberKitDOMHTMLTableColElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_COL_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableColElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::valignAttr, convertedValue);
}

gchar* webkit_dom_html_table_col_element_get_width(CyberKitDOMHTMLTableColElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_COL_ELEMENT(self), 0);
    CyberCore::HTMLTableColElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::widthAttr));
    return result;
}

void webkit_dom_html_table_col_element_set_width(CyberKitDOMHTMLTableColElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_COL_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableColElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::widthAttr, convertedValue);
}

G_GNUC_END_IGNORE_DEPRECATIONS;
