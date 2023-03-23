/*
 * Copyright (C) 2013 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2,1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "CyberKitFrame.h"

#include "CyberKitFramePrivate.h"
#include "CyberKitScriptWorldPrivate.h"
#include "CyberKitWebFormManagerPrivate.h"
#include <CyberScriptCore/APICast.h>
#include <CyberScriptCore/JSGlobalObjectInlines.h>
#include <CyberScriptCore/JSLock.h>
#include <CyberCore/Frame.h>
#include <CyberCore/FrameLoader.h>
#include <CyberCore/JSNode.h>
#include <CyberCore/ScriptController.h>
#include <jsc/JSCContextPrivate.h>
#include <wtf/glib/WTFGType.h>
#include <wtf/text/CString.h>

#if !ENABLE(2022_GLIB_API)
#include "CyberKitDOMNodePrivate.h"
#endif

using namespace CyberKit;
using namespace CyberCore;

/**
 * CyberKitFrame:
 *
 * A web page frame.
 *
 * Each `CyberKitWebPage` has at least one main frame, and can have any number
 * of subframes.
 *
 * Since: 2.26
 */

struct _CyberKitFramePrivate {
    RefPtr<WebFrame> webFrame;

    CString uri;
};

WEBKIT_DEFINE_FINAL_TYPE(CyberKitFrame, webkit_frame, G_TYPE_OBJECT, GObject)

static void webkit_frame_class_init(CyberKitFrameClass*)
{
}

static CString getURL(WebFrame* webFrame)
{
    auto* documentLoader = webFrame->coreFrame()->loader().provisionalDocumentLoader();
    if (!documentLoader)
        documentLoader = webFrame->coreFrame()->loader().documentLoader();

    ASSERT(documentLoader);

    if (!documentLoader->unreachableURL().isEmpty())
        return documentLoader->unreachableURL().string().utf8();

    return documentLoader->url().string().utf8();
}

CyberKitFrame* webkitFrameCreate(WebFrame* webFrame)
{
    CyberKitFrame* frame = WEBKIT_FRAME(g_object_new(WEBKIT_TYPE_FRAME, NULL));
    frame->priv->webFrame = webFrame;

    frame->priv->uri = getURL(webFrame);

    return frame;
}

WebFrame* webkitFrameGetWebFrame(CyberKitFrame* frame)
{
    return frame->priv->webFrame.get();
}

GRefPtr<JSCValue> webkitFrameGetJSCValueForElementInWorld(CyberKitFrame* frame, Element& element, CyberKitScriptWorld* world)
{
    Vector<RefPtr<Element>> elements = { RefPtr<Element>(&element) };
    auto values = webkitFrameGetJSCValuesForElementsInWorld(frame, elements, world);
    return values.takeLast();
}

Vector<GRefPtr<JSCValue>> webkitFrameGetJSCValuesForElementsInWorld(CyberKitFrame* frame, const Vector<RefPtr<Element>>& elements, CyberKitScriptWorld* world)
{
    auto* wkWorld = webkitScriptWorldGetInjectedBundleScriptWorld(world);
    auto jsContext = jscContextGetOrCreate(frame->priv->webFrame->jsContextForWorld(wkWorld));
    JSDOMWindow* globalObject = frame->priv->webFrame->coreFrame()->script().globalObject(wkWorld->coreWorld());
    return elements.map([&jsContext, globalObject](auto& element) -> GRefPtr<JSCValue> {
        JSValueRef jsValue = nullptr;
        {
            JSC::JSLockHolder lock(globalObject);
            jsValue = toRef(globalObject, toJS(globalObject, globalObject, element.get()));
        }
        return jsValue ? jscContextGetOrCreateValue(jsContext.get(), jsValue) : nullptr;
    });
}

void webkitFrameSetURI(CyberKitFrame* frame, const CString& uri)
{
    if (frame->priv->uri == uri)
        return;

    frame->priv->uri = uri;
}

/**
 * webkit_frame_get_id:
 * @frame: a #CyberKitFrame
 *
 * Gets the process-unique identifier of this #CyberKitFrame. No other
 * frame in the same web process will have the same ID; however, frames
 * in other web processes may.
 *
 * Returns: the identifier of @frame
 *
 * Since: 2.26
 */
guint64 webkit_frame_get_id(CyberKitFrame* frame)
{
    g_return_val_if_fail(WEBKIT_IS_FRAME(frame), 0);

    return frame->priv->webFrame->frameID().object().toUInt64();
}

/**
 * webkit_frame_is_main_frame:
 * @frame: a #CyberKitFrame
 *
 * Gets whether @frame is the main frame of a #CyberKitWebPage
 *
 * Returns: %TRUE if @frame is a main frame or %FALSE otherwise
 *
 * Since: 2.2
 */
gboolean webkit_frame_is_main_frame(CyberKitFrame* frame)
{
    g_return_val_if_fail(WEBKIT_IS_FRAME(frame), FALSE);

    return frame->priv->webFrame->isMainFrame();
}

/**
 * webkit_frame_get_uri:
 * @frame: a #CyberKitFrame
 *
 * Gets the current active URI of @frame.
 *
 * Returns: the current active URI of @frame or %NULL if nothing has been
 *    loaded yet.
 *
 * Since: 2.2
 */
const gchar* webkit_frame_get_uri(CyberKitFrame* frame)
{
    g_return_val_if_fail(WEBKIT_IS_FRAME(frame), 0);

    if (frame->priv->uri.isNull())
        frame->priv->uri = frame->priv->webFrame->url().string().utf8();

    return frame->priv->uri.data();
}

#if PLATFORM(GTK) && !USE(GTK4)
/**
 * webkit_frame_get_javascript_global_context: (skip)
 * @frame: a #CyberKitFrame
 *
 * Gets the global JavaScript execution context. Use this function to bridge
 * between the CyberKit and CyberScriptCore APIs.
 *
 * Returns: (transfer none): the global JavaScript context of @frame
 *
 * Since: 2.2
 *
 * Deprecated: 2.22: Use webkit_frame_get_js_context() instead.
 */
JSGlobalContextRef webkit_frame_get_javascript_global_context(CyberKitFrame* frame)
{
    g_return_val_if_fail(WEBKIT_IS_FRAME(frame), 0);

    return frame->priv->webFrame->jsContext();
}

/**
 * webkit_frame_get_javascript_context_for_script_world: (skip)
 * @frame: a #CyberKitFrame
 * @world: a #CyberKitScriptWorld
 *
 * Gets the JavaScript execution context of @frame for the given #CyberKitScriptWorld.
 *
 * Returns: (transfer none): the JavaScript context of @frame for @world
 *
 * Since: 2.2
 *
 * Deprecated: 2.22: Use webkit_frame_get_js_context_for_script_world() instead.
 */
JSGlobalContextRef webkit_frame_get_javascript_context_for_script_world(CyberKitFrame* frame, CyberKitScriptWorld* world)
{
    g_return_val_if_fail(WEBKIT_IS_FRAME(frame), 0);
    g_return_val_if_fail(WEBKIT_IS_SCRIPT_WORLD(world), 0);

    return frame->priv->webFrame->jsContextForWorld(webkitScriptWorldGetInjectedBundleScriptWorld(world));
}
#endif

/**
 * webkit_frame_get_js_context:
 * @frame: a #CyberKitFrame
 *
 * Get the JavaScript execution context of @frame. Use this function to bridge
 * between the CyberKit and CyberScriptCore APIs.
 *
 * Returns: (transfer full): the #JSCContext for the JavaScript execution context of @frame.
 *
 * Since: 2.22
 */
JSCContext* webkit_frame_get_js_context(CyberKitFrame* frame)
{
    g_return_val_if_fail(WEBKIT_IS_FRAME(frame), nullptr);

    return jscContextGetOrCreate(frame->priv->webFrame->jsContext()).leakRef();
}

/**
 * webkit_frame_get_js_context_for_script_world:
 * @frame: a #CyberKitFrame
 * @world: a #CyberKitScriptWorld
 *
 * Get the JavaScript execution context of @frame for the given #CyberKitScriptWorld.
 *
 * Returns: (transfer full): the #JSCContext for the JavaScript execution context of @frame for @world.
 *
 * Since: 2.22
 */
JSCContext* webkit_frame_get_js_context_for_script_world(CyberKitFrame* frame, CyberKitScriptWorld* world)
{
    g_return_val_if_fail(WEBKIT_IS_FRAME(frame), nullptr);
    g_return_val_if_fail(WEBKIT_IS_SCRIPT_WORLD(world), nullptr);

    return jscContextGetOrCreate(frame->priv->webFrame->jsContextForWorld(webkitScriptWorldGetInjectedBundleScriptWorld(world))).leakRef();
}

#if !ENABLE(2022_GLIB_API)
/**
 * webkit_frame_get_js_value_for_dom_object:
 * @frame: a #CyberKitFrame
 * @dom_object: a #CyberKitDOMObject
 *
 * Get a #JSCValue referencing the given DOM object. The value is created in the JavaScript execution
 * context of @frame.
 *
 * Returns: (transfer full): the #JSCValue referencing @dom_object.
 *
 * Since: 2.22
 *
 * Deprecated: 2.40
 */
JSCValue* webkit_frame_get_js_value_for_dom_object(CyberKitFrame* frame, CyberKitDOMObject* domObject)
{
    G_GNUC_BEGIN_IGNORE_DEPRECATIONS;
    return webkit_frame_get_js_value_for_dom_object_in_script_world(frame, domObject, webkit_script_world_get_default());
    G_GNUC_END_IGNORE_DEPRECATIONS;
}

/**
 * webkit_frame_get_js_value_for_dom_object_in_script_world:
 * @frame: a #CyberKitFrame
 * @dom_object: a #CyberKitDOMObject
 * @world: a #CyberKitScriptWorld
 *
 * Get a #JSCValue referencing the given DOM object. The value is created in the JavaScript execution
 * context of @frame for the given #CyberKitScriptWorld.
 *
 * Returns: (transfer full): the #JSCValue referencing @dom_object
 *
 * Since: 2.22
 *
 * Deprecated: 2.40
 */
JSCValue* webkit_frame_get_js_value_for_dom_object_in_script_world(CyberKitFrame* frame, CyberKitDOMObject* domObject, CyberKitScriptWorld* world)
{
    g_return_val_if_fail(WEBKIT_IS_FRAME(frame), nullptr);
    G_GNUC_BEGIN_IGNORE_DEPRECATIONS;
    g_return_val_if_fail(WEBKIT_DOM_IS_OBJECT(domObject), nullptr);
    G_GNUC_END_IGNORE_DEPRECATIONS;
    g_return_val_if_fail(WEBKIT_IS_SCRIPT_WORLD(world), nullptr);

    auto* wkWorld = webkitScriptWorldGetInjectedBundleScriptWorld(world);
    auto jsContext = jscContextGetOrCreate(frame->priv->webFrame->jsContextForWorld(wkWorld));
    JSDOMWindow* globalObject = frame->priv->webFrame->coreFrame()->script().globalObject(wkWorld->coreWorld());
    JSValueRef jsValue = nullptr;
    {
        JSC::JSLockHolder lock(globalObject);
        G_GNUC_BEGIN_IGNORE_DEPRECATIONS;
        if (WEBKIT_DOM_IS_NODE(domObject))
            jsValue = toRef(globalObject, toJS(globalObject, globalObject, CyberKit::core(WEBKIT_DOM_NODE(domObject))));
        G_GNUC_END_IGNORE_DEPRECATIONS;
    }

    return jsValue ? jscContextGetOrCreateValue(jsContext.get(), jsValue).leakRef() : nullptr;
}
#endif
