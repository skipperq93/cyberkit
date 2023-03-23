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
#include "CyberKitDOMHTMLTableElement.h"

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
#include "CyberKitDOMHTMLCollectionPrivate.h"
#include "CyberKitDOMHTMLElementPrivate.h"
#include "CyberKitDOMHTMLTableCaptionElementPrivate.h"
#include "CyberKitDOMHTMLTableElementPrivate.h"
#include "CyberKitDOMHTMLTableSectionElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLTableElement* kit(CyberCore::HTMLTableElement* obj)
{
    return WEBKIT_DOM_HTML_TABLE_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::HTMLTableElement* core(CyberKitDOMHTMLTableElement* request)
{
    return request ? static_cast<CyberCore::HTMLTableElement*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLTableElement* wrapHTMLTableElement(CyberCore::HTMLTableElement* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_TABLE_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_HTML_TABLE_ELEMENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_html_table_element_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::HTMLTableElement* coreTarget = static_cast<CyberCore::HTMLTableElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_html_table_element_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLTableElement* coreTarget = static_cast<CyberCore::HTMLTableElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_html_table_element_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLTableElement* coreTarget = static_cast<CyberCore::HTMLTableElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_html_table_element_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_html_table_element_dispatch_event;
    iface->add_event_listener = webkit_dom_html_table_element_add_event_listener;
    iface->remove_event_listener = webkit_dom_html_table_element_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMHTMLTableElement, webkit_dom_html_table_element, WEBKIT_DOM_TYPE_HTML_ELEMENT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_html_table_element_dom_event_target_init))

enum {
    DOM_HTML_TABLE_ELEMENT_PROP_0,
    DOM_HTML_TABLE_ELEMENT_PROP_CAPTION,
    DOM_HTML_TABLE_ELEMENT_PROP_T_HEAD,
    DOM_HTML_TABLE_ELEMENT_PROP_T_FOOT,
    DOM_HTML_TABLE_ELEMENT_PROP_ROWS,
    DOM_HTML_TABLE_ELEMENT_PROP_T_BODIES,
    DOM_HTML_TABLE_ELEMENT_PROP_ALIGN,
    DOM_HTML_TABLE_ELEMENT_PROP_BG_COLOR,
    DOM_HTML_TABLE_ELEMENT_PROP_BORDER,
    DOM_HTML_TABLE_ELEMENT_PROP_CELL_PADDING,
    DOM_HTML_TABLE_ELEMENT_PROP_CELL_SPACING,
    DOM_HTML_TABLE_ELEMENT_PROP_RULES,
    DOM_HTML_TABLE_ELEMENT_PROP_SUMMARY,
    DOM_HTML_TABLE_ELEMENT_PROP_WIDTH,
};

static void webkit_dom_html_table_element_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLTableElement* self = WEBKIT_DOM_HTML_TABLE_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_TABLE_ELEMENT_PROP_ALIGN:
        webkit_dom_html_table_element_set_align(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_BG_COLOR:
        webkit_dom_html_table_element_set_bg_color(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_BORDER:
        webkit_dom_html_table_element_set_border(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_CELL_PADDING:
        webkit_dom_html_table_element_set_cell_padding(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_CELL_SPACING:
        webkit_dom_html_table_element_set_cell_spacing(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_RULES:
        webkit_dom_html_table_element_set_rules(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_SUMMARY:
        webkit_dom_html_table_element_set_summary(self, g_value_get_string(value));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_WIDTH:
        webkit_dom_html_table_element_set_width(self, g_value_get_string(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_table_element_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLTableElement* self = WEBKIT_DOM_HTML_TABLE_ELEMENT(object);

    switch (propertyId) {
    case DOM_HTML_TABLE_ELEMENT_PROP_CAPTION:
        g_value_set_object(value, webkit_dom_html_table_element_get_caption(self));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_T_HEAD:
        g_value_set_object(value, webkit_dom_html_table_element_get_t_head(self));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_T_FOOT:
        g_value_set_object(value, webkit_dom_html_table_element_get_t_foot(self));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_ROWS:
        g_value_set_object(value, webkit_dom_html_table_element_get_rows(self));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_T_BODIES:
        g_value_set_object(value, webkit_dom_html_table_element_get_t_bodies(self));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_ALIGN:
        g_value_take_string(value, webkit_dom_html_table_element_get_align(self));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_BG_COLOR:
        g_value_take_string(value, webkit_dom_html_table_element_get_bg_color(self));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_BORDER:
        g_value_take_string(value, webkit_dom_html_table_element_get_border(self));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_CELL_PADDING:
        g_value_take_string(value, webkit_dom_html_table_element_get_cell_padding(self));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_CELL_SPACING:
        g_value_take_string(value, webkit_dom_html_table_element_get_cell_spacing(self));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_RULES:
        g_value_take_string(value, webkit_dom_html_table_element_get_rules(self));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_SUMMARY:
        g_value_take_string(value, webkit_dom_html_table_element_get_summary(self));
        break;
    case DOM_HTML_TABLE_ELEMENT_PROP_WIDTH:
        g_value_take_string(value, webkit_dom_html_table_element_get_width(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_html_table_element_class_init(CyberKitDOMHTMLTableElementClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_html_table_element_set_property;
    gobjectClass->get_property = webkit_dom_html_table_element_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ELEMENT_PROP_CAPTION,
        g_param_spec_object(
            "caption",
            "HTMLTableElement:caption",
            "read-only CyberKitDOMHTMLTableCaptionElement* HTMLTableElement:caption",
            WEBKIT_DOM_TYPE_HTML_TABLE_CAPTION_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ELEMENT_PROP_T_HEAD,
        g_param_spec_object(
            "t-head",
            "HTMLTableElement:t-head",
            "read-only CyberKitDOMHTMLTableSectionElement* HTMLTableElement:t-head",
            WEBKIT_DOM_TYPE_HTML_TABLE_SECTION_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ELEMENT_PROP_T_FOOT,
        g_param_spec_object(
            "t-foot",
            "HTMLTableElement:t-foot",
            "read-only CyberKitDOMHTMLTableSectionElement* HTMLTableElement:t-foot",
            WEBKIT_DOM_TYPE_HTML_TABLE_SECTION_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ELEMENT_PROP_ROWS,
        g_param_spec_object(
            "rows",
            "HTMLTableElement:rows",
            "read-only CyberKitDOMHTMLCollection* HTMLTableElement:rows",
            WEBKIT_DOM_TYPE_HTML_COLLECTION,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ELEMENT_PROP_T_BODIES,
        g_param_spec_object(
            "t-bodies",
            "HTMLTableElement:t-bodies",
            "read-only CyberKitDOMHTMLCollection* HTMLTableElement:t-bodies",
            WEBKIT_DOM_TYPE_HTML_COLLECTION,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ELEMENT_PROP_ALIGN,
        g_param_spec_string(
            "align",
            "HTMLTableElement:align",
            "read-write gchar* HTMLTableElement:align",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ELEMENT_PROP_BG_COLOR,
        g_param_spec_string(
            "bg-color",
            "HTMLTableElement:bg-color",
            "read-write gchar* HTMLTableElement:bg-color",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ELEMENT_PROP_BORDER,
        g_param_spec_string(
            "border",
            "HTMLTableElement:border",
            "read-write gchar* HTMLTableElement:border",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ELEMENT_PROP_CELL_PADDING,
        g_param_spec_string(
            "cell-padding",
            "HTMLTableElement:cell-padding",
            "read-write gchar* HTMLTableElement:cell-padding",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ELEMENT_PROP_CELL_SPACING,
        g_param_spec_string(
            "cell-spacing",
            "HTMLTableElement:cell-spacing",
            "read-write gchar* HTMLTableElement:cell-spacing",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ELEMENT_PROP_RULES,
        g_param_spec_string(
            "rules",
            "HTMLTableElement:rules",
            "read-write gchar* HTMLTableElement:rules",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ELEMENT_PROP_SUMMARY,
        g_param_spec_string(
            "summary",
            "HTMLTableElement:summary",
            "read-write gchar* HTMLTableElement:summary",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_TABLE_ELEMENT_PROP_WIDTH,
        g_param_spec_string(
            "width",
            "HTMLTableElement:width",
            "read-write gchar* HTMLTableElement:width",
            "",
            WEBKIT_PARAM_READWRITE));

}

static void webkit_dom_html_table_element_init(CyberKitDOMHTMLTableElement* request)
{
    UNUSED_PARAM(request);
}

CyberKitDOMHTMLElement* webkit_dom_html_table_element_create_t_head(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLElement> gobjectResult = WTF::getPtr(item->createTHead());
    return CyberKit::kit(gobjectResult.get());
}

void webkit_dom_html_table_element_delete_t_head(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self));
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    item->deleteTHead();
}

CyberKitDOMHTMLElement* webkit_dom_html_table_element_create_t_foot(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLElement> gobjectResult = WTF::getPtr(item->createTFoot());
    return CyberKit::kit(gobjectResult.get());
}

void webkit_dom_html_table_element_delete_t_foot(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self));
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    item->deleteTFoot();
}

CyberKitDOMHTMLElement* webkit_dom_html_table_element_create_caption(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLElement> gobjectResult = WTF::getPtr(item->createCaption());
    return CyberKit::kit(gobjectResult.get());
}

void webkit_dom_html_table_element_delete_caption(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self));
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    item->deleteCaption();
}

CyberKitDOMHTMLElement* webkit_dom_html_table_element_insert_row(CyberKitDOMHTMLTableElement* self, glong index, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    auto result = item->insertRow(index);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

void webkit_dom_html_table_element_delete_row(CyberKitDOMHTMLTableElement* self, glong index, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self));
    g_return_if_fail(!error || !*error);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    auto result = item->deleteRow(index);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

CyberKitDOMHTMLTableCaptionElement* webkit_dom_html_table_element_get_caption(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLTableCaptionElement> gobjectResult = WTF::getPtr(item->caption());
    return CyberKit::kit(gobjectResult.get());
}

void webkit_dom_html_table_element_set_caption(CyberKitDOMHTMLTableElement* self, CyberKitDOMHTMLTableCaptionElement* value, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self));
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_CAPTION_ELEMENT(value));
    g_return_if_fail(!error || !*error);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    CyberCore::HTMLTableCaptionElement* convertedValue = CyberKit::core(value);
    auto result = item->setCaption(convertedValue);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

CyberKitDOMHTMLTableSectionElement* webkit_dom_html_table_element_get_t_head(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLTableSectionElement> gobjectResult = WTF::getPtr(item->tHead());
    return CyberKit::kit(gobjectResult.get());
}

void webkit_dom_html_table_element_set_t_head(CyberKitDOMHTMLTableElement* self, CyberKitDOMHTMLTableSectionElement* value, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self));
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_SECTION_ELEMENT(value));
    g_return_if_fail(!error || !*error);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    CyberCore::HTMLTableSectionElement* convertedValue = CyberKit::core(value);
    auto result = item->setTHead(convertedValue);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

CyberKitDOMHTMLTableSectionElement* webkit_dom_html_table_element_get_t_foot(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLTableSectionElement> gobjectResult = WTF::getPtr(item->tFoot());
    return CyberKit::kit(gobjectResult.get());
}

void webkit_dom_html_table_element_set_t_foot(CyberKitDOMHTMLTableElement* self, CyberKitDOMHTMLTableSectionElement* value, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self));
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_SECTION_ELEMENT(value));
    g_return_if_fail(!error || !*error);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    CyberCore::HTMLTableSectionElement* convertedValue = CyberKit::core(value);
    auto result = item->setTFoot(convertedValue);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

CyberKitDOMHTMLCollection* webkit_dom_html_table_element_get_rows(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->rows());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMHTMLCollection* webkit_dom_html_table_element_get_t_bodies(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->tBodies());
    return CyberKit::kit(gobjectResult.get());
}

gchar* webkit_dom_html_table_element_get_align(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr));
    return result;
}

void webkit_dom_html_table_element_set_align(CyberKitDOMHTMLTableElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_table_element_get_bg_color(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::bgcolorAttr));
    return result;
}

void webkit_dom_html_table_element_set_bg_color(CyberKitDOMHTMLTableElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::bgcolorAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_table_element_get_border(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::borderAttr));
    return result;
}

void webkit_dom_html_table_element_set_border(CyberKitDOMHTMLTableElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::borderAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_table_element_get_cell_padding(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::cellpaddingAttr));
    return result;
}

void webkit_dom_html_table_element_set_cell_padding(CyberKitDOMHTMLTableElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::cellpaddingAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_table_element_get_cell_spacing(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::cellspacingAttr));
    return result;
}

void webkit_dom_html_table_element_set_cell_spacing(CyberKitDOMHTMLTableElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::cellspacingAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_table_element_get_rules(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::rulesAttr));
    return result;
}

void webkit_dom_html_table_element_set_rules(CyberKitDOMHTMLTableElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::rulesAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_table_element_get_summary(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::summaryAttr));
    return result;
}

void webkit_dom_html_table_element_set_summary(CyberKitDOMHTMLTableElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::summaryAttr, WTF::AtomString::fromUTF8(value));
}

gchar* webkit_dom_html_table_element_get_width(CyberKitDOMHTMLTableElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self), 0);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->attributeWithoutSynchronization(CyberCore::HTMLNames::widthAttr));
    return result;
}

void webkit_dom_html_table_element_set_width(CyberKitDOMHTMLTableElement* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(self));
    g_return_if_fail(value);
    CyberCore::HTMLTableElement* item = CyberKit::core(self);
    item->setAttributeWithoutSynchronization(CyberCore::HTMLNames::widthAttr, WTF::AtomString::fromUTF8(value));
}

G_GNUC_END_IGNORE_DEPRECATIONS;
