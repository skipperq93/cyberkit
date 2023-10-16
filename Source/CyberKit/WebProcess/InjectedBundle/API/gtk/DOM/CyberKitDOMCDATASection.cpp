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
#include "CyberKitDOMCDATASection.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include "GObjectEventListener.h"
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMCDATASectionPrivate.h"
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMCDATASection* kit(CyberCore::CDATASection* obj)
{
    return WEBKIT_DOM_CDATA_SECTION(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::CDATASection* core(CyberKitDOMCDATASection* request)
{
    return request ? static_cast<CyberCore::CDATASection*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMCDATASection* wrapCDATASection(CyberCore::CDATASection* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_CDATA_SECTION(g_object_new(WEBKIT_DOM_TYPE_CDATA_SECTION, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_cdata_section_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::CDATASection* coreTarget = static_cast<CyberCore::CDATASection*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_cdata_section_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::CDATASection* coreTarget = static_cast<CyberCore::CDATASection*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_cdata_section_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::CDATASection* coreTarget = static_cast<CyberCore::CDATASection*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_cdata_section_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_cdata_section_dispatch_event;
    iface->add_event_listener = webkit_dom_cdata_section_add_event_listener;
    iface->remove_event_listener = webkit_dom_cdata_section_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMCDATASection, webkit_dom_cdata_section, WEBKIT_DOM_TYPE_TEXT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_cdata_section_dom_event_target_init))

static void webkit_dom_cdata_section_class_init(CyberKitDOMCDATASectionClass* requestClass)
{
    UNUSED_PARAM(requestClass);
}

static void webkit_dom_cdata_section_init(CyberKitDOMCDATASection* request)
{
    UNUSED_PARAM(request);
}

G_GNUC_END_IGNORE_DEPRECATIONS;
