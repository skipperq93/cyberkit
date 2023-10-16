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
#include "CyberKitDOMHTMLTableSectionElement.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include <CyberCore/ElementInlines.h>
#include <CyberCore/HTMLNames.h>
#include <CyberCore/HTMLTableRowElement.h>
#include <CyberCore/JSExecState.h>
#include "GObjectEventListener.h"
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMHTMLCollectionPrivate.h"
#include "CyberKitDOMHTMLElementPrivate.h"
#include "CyberKitDOMHTMLTableSectionElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLTableSectionElement* kit(CyberCore::HTMLTableSectionElement* obj)
{
    return WEBKIT_DOM_HTML_TABLE_SECTION_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::HTMLTableSectionElement* core(CyberKitDOMHTMLTableSectionElement* request)
{
    return request ? static_cast<CyberCore::HTMLTableSectionElement*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLTableSectionElement* wrapHTMLTableSectionElement(CyberCore::HTMLTableSectionElement* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_TABLE_SECTION_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_HTML_TABLE_SECTION_ELEMENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_html_table_section_element_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::HTMLTableSectionElement* coreTarget = static_cast<CyberCore::HTMLTableSectionElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_html_table_section_element_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLTableSectionElement* coreTarget = static_cast<CyberCore::HTMLTableSectionElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_html_table_section_element_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLTableSectionElement* coreTarget = static_cast<CyberCore::HTMLTableSectionElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_html_table_section_element_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_html_table_section_element_dispatch_event;
    iface->add_event_listener = webkit_dom_html_table_section_element_add_event_listener;
    iface->remove_event_listener = webkit_dom_html_table_section_element_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMHTMLTableSectionElement, webkit_dom_html_table_section_element, WEBKIT_DOM_TYPE_HTML_ELEMENT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_html_table_section_element_dom_event_target_init))

enum {
    DOM_HTML_TABLE_SECTION_ELEMENT_PROP_0,
    DOM_HTML_TABLE_SECTION_ELEMENT_PROP_ALIGN,
    DOM_HTML_TABLE_SECTION_ELEMENT_PROP_CH,
    DOM_HTML_TABLE_SECTION_ELEMENT_PROP_CH_OFF,
    DOM_HTML_TABLE_SECTION_ELEMENT_PROP_V_ALIGN,
    DOM_HTML_TABLE_SECTION_ELEMENT_PROP_ROWS,
};

static void webkit_dom_html_table_section_element_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLTableSectionElement* self = WEBKIT_DOM_HTML_TABLE_SECTION_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_TABLE_SECTION_ELEMENT_PROP_ALIGN:
        webkit_dom_html_table_section_element_set_align(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_SECTION_ELEMENT_PROP_CH:
        webkit_dom_html_table_section_element_set_ch(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_SECTION_ELEMENT_PROP_CH_OFF:
        webkit_dom_html_table_section_element_set_ch_off(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_SECTION_ELEMENT_PROP_V_ALIGN:
        webkit_dom_html_table_section_element_set_v_align(self, g_value_get_string(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_table_section_element_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLTableSectionElement* self = WEBKIT_DOM_HTML_TABLE_SECTION_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_TABLE_SECTION_ELEMENT_PROP_ALIGN:
        g_value_take_string(value, webkit_dom_html_table_section_element_get_align(self));
        break;
    case DOM_HTML_TABLE_SECTION_ELEMENT_PROP_CH:
        g_value_take_string(value, webkit_dom_html_table_section_element_get_ch(self));
        break;
    case DOM_HTML_TABLE_SECTION_ELEMENT_PROP_CH_OFF:
        g_value_take_string(value, webkit_dom_html_table_section_element_get_ch_off(self));
        break;
    case DOM_HTML_TABLE_SECTION_ELEMENT_PROP_V_ALIGN:
        g_value_take_string(value, webkit_dom_html_table_section_element_get_v_align(self));
        break;
    case DOM_HTML_TABLE_SECTION_ELEMENT_PROP_ROWS:
        g_value_set_object(value, webkit_dom_html_table_section_element_get_rows(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_table_section_element_class_init(CyberKitDOMHTMLTableSectionElementClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_html_table_section_element_set_property;
    gobjectClass->get_property = webkit_dom_html_table_section_element_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_SECTION_ELEMENT_PROP_ALIGN,
        g_param_spec_string(
            "align",
            "HTMLTableSectionElement:align",
            "read-write gchar* HTMLTableSectionElement:align",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_SECTION_ELEMENT_PROP_CH,
        g_param_spec_string(
            "ch",
            "HTMLTableSectionElement:ch",
            "read-write gchar* HTMLTableSectionElement:ch",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_SECTION_ELEMENT_PROP_CH_OFF,
        g_param_spec_string(
            "ch-off",
            "HTMLTableSectionElement:ch-off",
            "read-write gchar* HTMLTableSectionElement:ch-off",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_SECTION_ELEMENT_PROP_V_ALIGN,
        g_param_spec_string(
            "v-align",
            "HTMLTableSectionElement:v-align",
            "read-write gchar* HTMLTableSectionElement:v-align",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_SECTION_ELEMENT_PROP_ROWS,
        g_param_spec_object(
            "rows",
            "HTMLTableSectionElement:rows",
            "read-only CyberKitDOMHTMLCollection* HTMLTableSectionElement:rows",
            WEBKIT_DOM_TYPE_HTML_COLLECTION,
            WEBKIT_PARAM_READABLE));

}

static void webkit_dom_html_table_section_element_init(CyberKitDOMHTMLTableSectionElement* request)
{
    UNUSED_PARAM(request);
}

CyberKitDOMHTMLElement* webkit_dom_html_table_section_element_insert_row(CyberKitDOMHTMLTableSectionElement* self, glong index, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_SECTION_ELEMENT(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::HTMLTableSectionElement* item = CyberKit::core(self);
    auto result = item->insertRow(index);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
    CyberCore::HTMLElement* resultElement = result.releaseReturnValue().ptr();
    return CyberKit::kit(resultElement);
}

void webkit_dom_html_table_section_element_delete_row(CyberKitDOMHTMLTableSectionElement* self, glong index, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_SECTION_ELEMENT(self));
    g_return_if_fail(!error || !*error);
    CyberCore::HTMLTableSectionElement* item = CyberKit::core(self);
    auto result = item->deleteRow(index);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

gchar* webkit_dom_html_table_section_element_get_align(CyberKitDOMHTMLTableSectionElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_SECTION_ELEMENT(self), 0);
    CyberCore::HTMLTableSectionElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr));
    return result;
}

void webkit_dom_html_table_section_element_set_align(CyberKitDOMHTMLTableSectionElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_SECTION_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableSectionElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_table_section_element_get_ch(CyberKitDOMHTMLTableSectionElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_SECTION_ELEMENT(self), 0);
    CyberCore::HTMLTableSectionElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::charAttr));
    return result;
}

void webkit_dom_html_table_section_element_set_ch(CyberKitDOMHTMLTableSectionElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_SECTION_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableSectionElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::charAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_table_section_element_get_ch_off(CyberKitDOMHTMLTableSectionElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_SECTION_ELEMENT(self), 0);
    CyberCore::HTMLTableSectionElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::charoffAttr));
    return result;
}

void webkit_dom_html_table_section_element_set_ch_off(CyberKitDOMHTMLTableSectionElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_SECTION_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableSectionElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::charoffAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_table_section_element_get_v_align(CyberKitDOMHTMLTableSectionElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_SECTION_ELEMENT(self), 0);
    CyberCore::HTMLTableSectionElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::valignAttr));
    return result;
}

void webkit_dom_html_table_section_element_set_v_align(CyberKitDOMHTMLTableSectionElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_SECTION_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableSectionElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::valignAttr, WTF::AtomString::fromUTF8(value));
}

CyberKitDOMHTMLCollection* webkit_dom_html_table_section_element_get_rows(CyberKitDOMHTMLTableSectionElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_SECTION_ELEMENT(self), 0);
    CyberCore::HTMLTableSectionElement* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->rows());
    return CyberKit::kit(gobjectResult.get());
}

G_GNUC_END_IGNORE_DEPRECATIONS;
