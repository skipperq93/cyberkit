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
#include "CyberKitDOMCharacterData.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include "GObjectEventListener.h"
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMCharacterDataPrivate.h"
#include "CyberKitDOMElementPrivate.h"
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMCharacterData* kit(CyberCore::CharacterData* obj)
{
    return WEBKIT_DOM_CHARACTER_DATA(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::CharacterData* core(CyberKitDOMCharacterData* request)
{
    return request ? static_cast<CyberCore::CharacterData*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMCharacterData* wrapCharacterData(CyberCore::CharacterData* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_CHARACTER_DATA(g_object_new(WEBKIT_DOM_TYPE_CHARACTER_DATA, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_character_data_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::CharacterData* coreTarget = static_cast<CyberCore::CharacterData*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_character_data_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::CharacterData* coreTarget = static_cast<CyberCore::CharacterData*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_character_data_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::CharacterData* coreTarget = static_cast<CyberCore::CharacterData*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_character_data_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_character_data_dispatch_event;
    iface->add_event_listener = webkit_dom_character_data_add_event_listener;
    iface->remove_event_listener = webkit_dom_character_data_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMCharacterData, webkit_dom_character_data, WEBKIT_DOM_TYPE_NODE, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_character_data_dom_event_target_init))

enum {
    DOM_CHARACTER_DATA_PROP_0,
    DOM_CHARACTER_DATA_PROP_DATA,
    DOM_CHARACTER_DATA_PROP_LENGTH,
};

static void webkit_dom_character_data_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMCharacterData* self = WEBKIT_DOM_CHARACTER_DATA(object);

    switch (propertyId) {
    case DOM_CHARACTER_DATA_PROP_DATA:
        webkit_dom_character_data_set_data(self, g_value_get_string(value), nullptr);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_character_data_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMCharacterData* self = WEBKIT_DOM_CHARACTER_DATA(object);

    switch (propertyId) {
    case DOM_CHARACTER_DATA_PROP_DATA:
        g_value_take_string(value, webkit_dom_character_data_get_data(self));
        break;
    case DOM_CHARACTER_DATA_PROP_LENGTH:
        g_value_set_ulong(value, webkit_dom_character_data_get_length(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_character_data_class_init(CyberKitDOMCharacterDataClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->set_property = webkit_dom_character_data_set_property;
    gobjectClass->get_property = webkit_dom_character_data_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_CHARACTER_DATA_PROP_DATA,
        g_param_spec_string(
            "data",
            "CharacterData:data",
            "read-write gchar* CharacterData:data",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_CHARACTER_DATA_PROP_LENGTH,
        g_param_spec_ulong(
            "length",
            "CharacterData:length",
            "read-only gulong CharacterData:length",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READABLE));
}

static void webkit_dom_character_data_init(CyberKitDOMCharacterData* request)
{
    UNUSED_PARAM(request);
}

gchar* webkit_dom_character_data_substring_data(CyberKitDOMCharacterData* self, gulong offset, gulong length, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CHARACTER_DATA(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::CharacterData* item = CyberKit::core(self);
    auto result = item->substringData(offset, length);
    if (result.hasException())
        return nullptr;
    return convertToUTF8String(result.releaseReturnValue());
}

void webkit_dom_character_data_append_data(CyberKitDOMCharacterData* self, const gchar* data, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_CHARACTER_DATA(self));
    g_return_if_fail(data);
    UNUSED_PARAM(error);
    CyberCore::CharacterData* item = CyberKit::core(self);
    WTF::String convertedData = WTF::String::fromUTF8(data);
    item->appendData(convertedData);
}

void webkit_dom_character_data_insert_data(CyberKitDOMCharacterData* self, gulong offset, const gchar* data, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_CHARACTER_DATA(self));
    g_return_if_fail(data);
    g_return_if_fail(!error || !*error);
    CyberCore::CharacterData* item = CyberKit::core(self);
    WTF::String convertedData = WTF::String::fromUTF8(data);
    auto result = item->insertData(offset, convertedData);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

void webkit_dom_character_data_delete_data(CyberKitDOMCharacterData* self, gulong offset, gulong length, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_CHARACTER_DATA(self));
    g_return_if_fail(!error || !*error);
    CyberCore::CharacterData* item = CyberKit::core(self);
    auto result = item->deleteData(offset, length);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

void webkit_dom_character_data_replace_data(CyberKitDOMCharacterData* self, gulong offset, gulong length, const gchar* data, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_CHARACTER_DATA(self));
    g_return_if_fail(data);
    g_return_if_fail(!error || !*error);
    CyberCore::CharacterData* item = CyberKit::core(self);
    WTF::String convertedData = WTF::String::fromUTF8(data);
    auto result = item->replaceData(offset, length, convertedData);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

gchar* webkit_dom_character_data_get_data(CyberKitDOMCharacterData* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CHARACTER_DATA(self), 0);
    CyberCore::CharacterData* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->data());
    return result;
}

void webkit_dom_character_data_set_data(CyberKitDOMCharacterData* self, const gchar* value, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_CHARACTER_DATA(self));
    g_return_if_fail(value);
    UNUSED_PARAM(error);
    CyberCore::CharacterData* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setData(convertedValue);
}

gulong webkit_dom_character_data_get_length(CyberKitDOMCharacterData* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CHARACTER_DATA(self), 0);
    CyberCore::CharacterData* item = CyberKit::core(self);
    gulong result = item->length();
    return result;
}
G_GNUC_END_IGNORE_DEPRECATIONS;
