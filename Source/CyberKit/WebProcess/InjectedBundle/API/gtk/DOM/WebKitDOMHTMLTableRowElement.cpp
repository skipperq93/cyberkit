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
#include "CyberKitDOMHTMLTableRowElement.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include "GObjectEventListener.h"
#include <CyberCore/HTMLNames.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMHTMLCollectionPrivate.h"
#include "CyberKitDOMHTMLElementPrivate.h"
#include "CyberKitDOMHTMLTableRowElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLTableRowElement* kit(CyberCore::HTMLTableRowElement* obj)
{
    return WEBKIT_DOM_HTML_TABLE_ROW_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::HTMLTableRowElement* core(CyberKitDOMHTMLTableRowElement* request)
{
    return request ? static_cast<CyberCore::HTMLTableRowElement*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLTableRowElement* wrapHTMLTableRowElement(CyberCore::HTMLTableRowElement* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_TABLE_ROW_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_HTML_TABLE_ROW_ELEMENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_html_table_row_element_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::HTMLTableRowElement* coreTarget = static_cast<CyberCore::HTMLTableRowElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_html_table_row_element_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLTableRowElement* coreTarget = static_cast<CyberCore::HTMLTableRowElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_html_table_row_element_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLTableRowElement* coreTarget = static_cast<CyberCore::HTMLTableRowElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_html_table_row_element_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_html_table_row_element_dispatch_event;
    iface->add_event_listener = webkit_dom_html_table_row_element_add_event_listener;
    iface->remove_event_listener = webkit_dom_html_table_row_element_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMHTMLTableRowElement, webkit_dom_html_table_row_element, WEBKIT_DOM_TYPE_HTML_ELEMENT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_html_table_row_element_dom_event_target_init))

enum {
    DOM_HTML_TABLE_ROW_ELEMENT_PROP_0,
    DOM_HTML_TABLE_ROW_ELEMENT_PROP_ROW_INDEX,
    DOM_HTML_TABLE_ROW_ELEMENT_PROP_SECTION_ROW_INDEX,
    DOM_HTML_TABLE_ROW_ELEMENT_PROP_CELLS,
    DOM_HTML_TABLE_ROW_ELEMENT_PROP_ALIGN,
    DOM_HTML_TABLE_ROW_ELEMENT_PROP_BG_COLOR,
    DOM_HTML_TABLE_ROW_ELEMENT_PROP_CH,
    DOM_HTML_TABLE_ROW_ELEMENT_PROP_CH_OFF,
    DOM_HTML_TABLE_ROW_ELEMENT_PROP_V_ALIGN,
};

static void webkit_dom_html_table_row_element_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLTableRowElement* self = WEBKIT_DOM_HTML_TABLE_ROW_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_TABLE_ROW_ELEMENT_PROP_ALIGN:
        webkit_dom_html_table_row_element_set_align(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_ROW_ELEMENT_PROP_BG_COLOR:
        webkit_dom_html_table_row_element_set_bg_color(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_ROW_ELEMENT_PROP_CH:
        webkit_dom_html_table_row_element_set_ch(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_ROW_ELEMENT_PROP_CH_OFF:
        webkit_dom_html_table_row_element_set_ch_off(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_ROW_ELEMENT_PROP_V_ALIGN:
        webkit_dom_html_table_row_element_set_v_align(self, g_value_get_string(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_table_row_element_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLTableRowElement* self = WEBKIT_DOM_HTML_TABLE_ROW_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_TABLE_ROW_ELEMENT_PROP_ROW_INDEX:
        g_value_set_long(value, webkit_dom_html_table_row_element_get_row_index(self));
        break;
    case DOM_HTML_TABLE_ROW_ELEMENT_PROP_SECTION_ROW_INDEX:
        g_value_set_long(value, webkit_dom_html_table_row_element_get_section_row_index(self));
        break;
    case DOM_HTML_TABLE_ROW_ELEMENT_PROP_CELLS:
        g_value_set_object(value, webkit_dom_html_table_row_element_get_cells(self));
        break;
    case DOM_HTML_TABLE_ROW_ELEMENT_PROP_ALIGN:
        g_value_take_string(value, webkit_dom_html_table_row_element_get_align(self));
        break;
    case DOM_HTML_TABLE_ROW_ELEMENT_PROP_BG_COLOR:
        g_value_take_string(value, webkit_dom_html_table_row_element_get_bg_color(self));
        break;
    case DOM_HTML_TABLE_ROW_ELEMENT_PROP_CH:
        g_value_take_string(value, webkit_dom_html_table_row_element_get_ch(self));
        break;
    case DOM_HTML_TABLE_ROW_ELEMENT_PROP_CH_OFF:
        g_value_take_string(value, webkit_dom_html_table_row_element_get_ch_off(self));
        break;
    case DOM_HTML_TABLE_ROW_ELEMENT_PROP_V_ALIGN:
        g_value_take_string(value, webkit_dom_html_table_row_element_get_v_align(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_table_row_element_class_init(CyberKitDOMHTMLTableRowElementClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_html_table_row_element_set_property;
    gobjectClass->get_property = webkit_dom_html_table_row_element_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ROW_ELEMENT_PROP_ROW_INDEX,
        g_param_spec_long(
            "row-index",
            "HTMLTableRowElement:row-index",
            "read-only glong HTMLTableRowElement:row-index",
            G_MINLONG, G_MAXLONG, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ROW_ELEMENT_PROP_SECTION_ROW_INDEX,
        g_param_spec_long(
            "section-row-index",
            "HTMLTableRowElement:section-row-index",
            "read-only glong HTMLTableRowElement:section-row-index",
            G_MINLONG, G_MAXLONG, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ROW_ELEMENT_PROP_CELLS,
        g_param_spec_object(
            "cells",
            "HTMLTableRowElement:cells",
            "read-only CyberKitDOMHTMLCollection* HTMLTableRowElement:cells",
            WEBKIT_DOM_TYPE_HTML_COLLECTION,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ROW_ELEMENT_PROP_ALIGN,
        g_param_spec_string(
            "align",
            "HTMLTableRowElement:align",
            "read-write gchar* HTMLTableRowElement:align",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ROW_ELEMENT_PROP_BG_COLOR,
        g_param_spec_string(
            "bg-color",
            "HTMLTableRowElement:bg-color",
            "read-write gchar* HTMLTableRowElement:bg-color",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ROW_ELEMENT_PROP_CH,
        g_param_spec_string(
            "ch",
            "HTMLTableRowElement:ch",
            "read-write gchar* HTMLTableRowElement:ch",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ROW_ELEMENT_PROP_CH_OFF,
        g_param_spec_string(
            "ch-off",
            "HTMLTableRowElement:ch-off",
            "read-write gchar* HTMLTableRowElement:ch-off",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ROW_ELEMENT_PROP_V_ALIGN,
        g_param_spec_string(
            "v-align",
            "HTMLTableRowElement:v-align",
            "read-write gchar* HTMLTableRowElement:v-align",
            "",
            WEBKIT_PARAM_READWRITE));

}

static void webkit_dom_html_table_row_element_init(CyberKitDOMHTMLTableRowElement* request)
{
    UNUSED_PARAM(request);
}

CyberKitDOMHTMLElement* webkit_dom_html_table_row_element_insert_cell(CyberKitDOMHTMLTableRowElement* self, glong index, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::HTMLTableRowElement* item = CyberKit::core(self);
    auto result = item->insertCell(index);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return WEBKIT_DOM_HTML_ELEMENT(CyberKit::kit(result.releaseReturnValue().ptr()));
}

void webkit_dom_html_table_row_element_delete_cell(CyberKitDOMHTMLTableRowElement* self, glong index, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(self));
    g_return_if_fail(!error || !*error);
    CyberCore::HTMLTableRowElement* item = CyberKit::core(self);
    auto result = item->deleteCell(index);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

glong webkit_dom_html_table_row_element_get_row_index(CyberKitDOMHTMLTableRowElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(self), 0);
    CyberCore::HTMLTableRowElement* item = CyberKit::core(self);
    glong result = item->rowIndex();
    return result;
}

glong webkit_dom_html_table_row_element_get_section_row_index(CyberKitDOMHTMLTableRowElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(self), 0);
    CyberCore::HTMLTableRowElement* item = CyberKit::core(self);
    glong result = item->sectionRowIndex();
    return result;
}

CyberKitDOMHTMLCollection* webkit_dom_html_table_row_element_get_cells(CyberKitDOMHTMLTableRowElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(self), 0);
    CyberCore::HTMLTableRowElement* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->cells());
    return CyberKit::kit(gobjectResult.get());
}

gchar* webkit_dom_html_table_row_element_get_align(CyberKitDOMHTMLTableRowElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(self), 0);
    CyberCore::HTMLTableRowElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr));
    return result;
}

void webkit_dom_html_table_row_element_set_align(CyberKitDOMHTMLTableRowElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableRowElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr, convertedValue);
}

gchar* webkit_dom_html_table_row_element_get_bg_color(CyberKitDOMHTMLTableRowElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(self), 0);
    CyberCore::HTMLTableRowElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::bgcolorAttr));
    return result;
}

void webkit_dom_html_table_row_element_set_bg_color(CyberKitDOMHTMLTableRowElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableRowElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::bgcolorAttr, convertedValue);
}

gchar* webkit_dom_html_table_row_element_get_ch(CyberKitDOMHTMLTableRowElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(self), 0);
    CyberCore::HTMLTableRowElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::charAttr));
    return result;
}

void webkit_dom_html_table_row_element_set_ch(CyberKitDOMHTMLTableRowElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableRowElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::charAttr, convertedValue);
}

gchar* webkit_dom_html_table_row_element_get_ch_off(CyberKitDOMHTMLTableRowElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(self), 0);
    CyberCore::HTMLTableRowElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::charoffAttr));
    return result;
}

void webkit_dom_html_table_row_element_set_ch_off(CyberKitDOMHTMLTableRowElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableRowElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::charoffAttr, convertedValue);
}

gchar* webkit_dom_html_table_row_element_get_v_align(CyberKitDOMHTMLTableRowElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(self), 0);
    CyberCore::HTMLTableRowElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::valignAttr));
    return result;
}

void webkit_dom_html_table_row_element_set_v_align(CyberKitDOMHTMLTableRowElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableRowElement* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::valignAttr, convertedValue);
}

G_GNUC_END_IGNORE_DEPRECATIONS;
