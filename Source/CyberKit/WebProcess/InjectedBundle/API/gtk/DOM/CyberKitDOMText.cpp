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
#include "CyberKitDOMText.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include "GObjectEventListener.h"
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "CyberKitDOMTextPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMText* kit(CyberCore::Text* obj)
{
    return WEBKIT_DOM_TEXT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::Text* core(CyberKitDOMText* request)
{
    return request ? static_cast<CyberCore::Text*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMText* wrapText(CyberCore::Text* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_TEXT(g_object_new(WEBKIT_DOM_TYPE_TEXT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_text_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::Text* coreTarget = static_cast<CyberCore::Text*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_text_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::Text* coreTarget = static_cast<CyberCore::Text*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_text_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::Text* coreTarget = static_cast<CyberCore::Text*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_text_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_text_dispatch_event;
    iface->add_event_listener = webkit_dom_text_add_event_listener;
    iface->remove_event_listener = webkit_dom_text_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMText, webkit_dom_text, WEBKIT_DOM_TYPE_CHARACTER_DATA, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_text_dom_event_target_init))

enum {
    DOM_TEXT_PROP_0,
    DOM_TEXT_PROP_WHOLE_TEXT,
};

static void webkit_dom_text_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMText* self = WEBKIT_DOM_TEXT(object);

    switch (propertyId) {
    case DOM_TEXT_PROP_WHOLE_TEXT:
        g_value_take_string(value, webkit_dom_text_get_whole_text(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_text_class_init(CyberKitDOMTextClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->get_property = webkit_dom_text_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_TEXT_PROP_WHOLE_TEXT,
        g_param_spec_string(
            "whole-text",
            "Text:whole-text",
            "read-only gchar* Text:whole-text",
            "",
            WEBKIT_PARAM_READABLE));

}

static void webkit_dom_text_init(CyberKitDOMText* request)
{
    UNUSED_PARAM(request);
}

CyberKitDOMText* webkit_dom_text_split_text(CyberKitDOMText* self, gulong offset, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEXT(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Text* item = CyberKit::core(self);
    auto result = item->splitText(offset);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

gchar* webkit_dom_text_get_whole_text(CyberKitDOMText* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_TEXT(self), 0);
    CyberCore::Text* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->wholeText());
    return result;
}

G_GNUC_END_IGNORE_DEPRECATIONS;
