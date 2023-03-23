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
#include "CyberKitDOMHTMLMarqueeElement.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include "GObjectEventListener.h"
#include <CyberCore/HTMLNames.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMHTMLMarqueeElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLMarqueeElement* kit(CyberCore::HTMLMarqueeElement* obj)
{
    return WEBKIT_DOM_HTML_MARQUEE_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::HTMLMarqueeElement* core(CyberKitDOMHTMLMarqueeElement* request)
{
    return request ? static_cast<CyberCore::HTMLMarqueeElement*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLMarqueeElement* wrapHTMLMarqueeElement(CyberCore::HTMLMarqueeElement* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_MARQUEE_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_HTML_MARQUEE_ELEMENT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

static gboolean webkit_dom_html_marquee_element_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::HTMLMarqueeElement* coreTarget = static_cast<CyberCore::HTMLMarqueeElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_html_marquee_element_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLMarqueeElement* coreTarget = static_cast<CyberCore::HTMLMarqueeElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_html_marquee_element_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::HTMLMarqueeElement* coreTarget = static_cast<CyberCore::HTMLMarqueeElement*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static void webkit_dom_html_marquee_element_dom_event_target_init(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_html_marquee_element_dispatch_event;
    iface->add_event_listener = webkit_dom_html_marquee_element_add_event_listener;
    iface->remove_event_listener = webkit_dom_html_marquee_element_remove_event_listener;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMHTMLMarqueeElement, webkit_dom_html_marquee_element, WEBKIT_DOM_TYPE_HTML_ELEMENT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkit_dom_html_marquee_element_dom_event_target_init))

static void webkit_dom_html_marquee_element_class_init(CyberKitDOMHTMLMarqueeElementClass* requestClass)
{
}

static void webkit_dom_html_marquee_element_init(CyberKitDOMHTMLMarqueeElement* request)
{
    UNUSED_PARAM(request);
}

void webkit_dom_html_marquee_element_start(CyberKitDOMHTMLMarqueeElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_MARQUEE_ELEMENT(self));
    CyberCore::HTMLMarqueeElement* item = CyberKit::core(self);
    item->start();
}

void webkit_dom_html_marquee_element_stop(CyberKitDOMHTMLMarqueeElement* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_HTML_MARQUEE_ELEMENT(self));
    CyberCore::HTMLMarqueeElement* item = CyberKit::core(self);
    item->stop();
}
G_GNUC_END_IGNORE_DEPRECATIONS;
