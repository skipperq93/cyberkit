/*
 * Copyright (C) 2014 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
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
#include "CyberKitUserContentManager.h"

#include "APISerializedScriptValue.h"
#include "InjectUserScriptImmediately.h"
#include "CyberKitJavascriptResultPrivate.h"
#include "CyberKitUserContentManagerPrivate.h"
#include "CyberKitUserContentPrivate.h"
#include "CyberKitWebContextPrivate.h"
#include "WebScriptMessageHandler.h"
#include <wtf/glib/GRefPtr.h>
#include <wtf/glib/WTFGType.h>

#if PLATFORM(WPE)
#include "WPEView.h"
#endif

using namespace CyberCore;
using namespace CyberKit;

struct _CyberKitUserContentManagerPrivate {
    _CyberKitUserContentManagerPrivate()
        : userContentController(adoptRef(new WebUserContentControllerProxy))
    {
    }

    RefPtr<WebUserContentControllerProxy> userContentController;
};

/**
 * SECTION:CyberKitUserContentManager
 * @short_description: Manages user-defined content which affects web pages.
 * @title: CyberKitUserContentManager
 *
 * Using a #CyberKitUserContentManager user CSS style sheets can be set to
 * be injected in the web pages loaded by a #CyberKitWebView, by
 * webkit_user_content_manager_add_style_sheet().
 *
 * To use a #CyberKitUserContentManager, it must be created using
 * webkit_user_content_manager_new(), and then passed to
 * webkit_web_view_new_with_user_content_manager(). User style
 * sheets can be created with webkit_user_style_sheet_new().
 *
 * User style sheets can be added and removed at any time, but
 * they will affect the web pages loaded afterwards.
 *
 * Since: 2.6
 */

WEBKIT_DEFINE_TYPE(CyberKitUserContentManager, webkit_user_content_manager, G_TYPE_OBJECT)

enum {
    SCRIPT_MESSAGE_RECEIVED,

    LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0, };

static void webkit_user_content_manager_class_init(CyberKitUserContentManagerClass* klass)
{
    GObjectClass* gObjectClass = G_OBJECT_CLASS(klass);

    /**
     * CyberKitUserContentManager::script-message-received:
     * @manager: the #CyberKitUserContentManager
     * @js_result: the #CyberKitJavascriptResult holding the value received from the JavaScript world.
     *
     * This signal is emitted when JavaScript in a web view calls
     * <code>window.webkit.messageHandlers.&lt;name&gt;.postMessage()</code>, after registering
     * <code>&lt;name&gt;</code> using
     * webkit_user_content_manager_register_script_message_handler()
     *
     * Since: 2.8
     */
    signals[SCRIPT_MESSAGE_RECEIVED] =
        g_signal_new(
            "script-message-received",
            G_TYPE_FROM_CLASS(gObjectClass),
            static_cast<GSignalFlags>(G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED),
            0, nullptr, nullptr,
            g_cclosure_marshal_VOID__BOXED,
            G_TYPE_NONE, 1,
            WEBKIT_TYPE_JAVASCRIPT_RESULT);
}

/**
 * webkit_user_content_manager_new:
 *
 * Creates a new user content manager.
 *
 * Returns: A #CyberKitUserContentManager
 *
 * Since: 2.6
 */
CyberKitUserContentManager* webkit_user_content_manager_new()
{
    return WEBKIT_USER_CONTENT_MANAGER(g_object_new(WEBKIT_TYPE_USER_CONTENT_MANAGER, nullptr));
}

/**
 * webkit_user_content_manager_add_style_sheet:
 * @manager: A #CyberKitUserContentManager
 * @stylesheet: A #CyberKitUserStyleSheet
 *
 * Adds a #CyberKitUserStyleSheet to the given #CyberKitUserContentManager.
 * The same #CyberKitUserStyleSheet can be reused with multiple
 * #CyberKitUserContentManager instances.
 *
 * Since: 2.6
 */
void webkit_user_content_manager_add_style_sheet(CyberKitUserContentManager* manager, CyberKitUserStyleSheet* styleSheet)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    g_return_if_fail(styleSheet);
    manager->priv->userContentController->addUserStyleSheet(webkitUserStyleSheetGetUserStyleSheet(styleSheet));
}

/**
 * webkit_user_content_manager_remove_all_style_sheets:
 * @manager: A #CyberKitUserContentManager
 *
 * Removes all user style sheets from the given #CyberKitUserContentManager.
 *
 * Since: 2.6
 */
void webkit_user_content_manager_remove_all_style_sheets(CyberKitUserContentManager* manager)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    manager->priv->userContentController->removeAllUserStyleSheets();
}

/**
 * webkit_user_content_manager_add_script:
 * @manager: A #CyberKitUserContentManager
 * @script: A #CyberKitUserScript
 *
 * Adds a #CyberKitUserScript to the given #CyberKitUserContentManager.
 * The same #CyberKitUserScript can be reused with multiple
 * #CyberKitUserContentManager instances.
 *
 * Since: 2.6
 */
void webkit_user_content_manager_add_script(CyberKitUserContentManager* manager, CyberKitUserScript* script)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    g_return_if_fail(script);
    manager->priv->userContentController->addUserScript(webkitUserScriptGetUserScript(script), InjectUserScriptImmediately::No);
}

/**
 * webkit_user_content_manager_remove_all_scripts:
 * @manager: A #CyberKitUserContentManager
 *
 * Removes all user scripts from the given #CyberKitUserContentManager
 *
 * Since: 2.6
 */
void webkit_user_content_manager_remove_all_scripts(CyberKitUserContentManager* manager)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    manager->priv->userContentController->removeAllUserScripts();
}

class ScriptMessageClientGtk final : public WebScriptMessageHandler::Client {
    WTF_MAKE_FAST_ALLOCATED;
public:
    ScriptMessageClientGtk(CyberKitUserContentManager* manager, const char* handlerName)
        : m_handlerName(g_quark_from_string(handlerName))
        , m_manager(manager)
    {
    }

    void didPostMessage(WebPageProxy&, FrameInfoData&&, CyberCore::SerializedScriptValue& serializedScriptValue) override
    {
        CyberKitJavascriptResult* jsResult = webkitJavascriptResultCreate(serializedScriptValue);
        g_signal_emit(m_manager, signals[SCRIPT_MESSAGE_RECEIVED], m_handlerName, jsResult);
        webkit_javascript_result_unref(jsResult);
    }

    virtual ~ScriptMessageClientGtk() { }

private:
    GQuark m_handlerName;
    CyberKitUserContentManager* m_manager;
};

/**
 * webkit_user_content_manager_register_script_message_handler:
 * @manager: A #CyberKitUserContentManager
 * @name: Name of the script message channel
 *
 * Registers a new user script message handler. After it is registered,
 * scripts can use `window.webkit.messageHandlers.&lt;name&gt;.postMessage(value)`
 * to send messages. Those messages are received by connecting handlers
 * to the #CyberKitUserContentManager::script-message-received signal. The
 * handler name is used as the detail of the signal. To avoid race
 * conditions between registering the handler name, and starting to
 * receive the signals, it is recommended to connect to the signal
 * *before* registering the handler name:
 *
 * <informalexample><programlisting>
 * CyberKitWebView *view = webkit_web_view_new ();
 * CyberKitUserContentManager *manager = webkit_web_view_get_user_content_manager ();
 * g_signal_connect (manager, "script-message-received::foobar",
 *                   G_CALLBACK (handle_script_message), NULL);
 * webkit_user_content_manager_register_script_message_handler (manager, "foobar");
 * </programlisting></informalexample>
 *
 * Registering a script message handler will fail if the requested
 * name has been already registered before.
 *
 * Returns: %TRUE if message handler was registered successfully, or %FALSE otherwise.
 *
 * Since: 2.8
 */
gboolean webkit_user_content_manager_register_script_message_handler(CyberKitUserContentManager* manager, const char* name)
{
    g_return_val_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager), FALSE);
    g_return_val_if_fail(name, FALSE);

    Ref<WebScriptMessageHandler> handler =
        WebScriptMessageHandler::create(makeUnique<ScriptMessageClientGtk>(manager, name), String::fromUTF8(name), API::UserContentWorld::normalWorld());
    return manager->priv->userContentController->addUserScriptMessageHandler(handler.get());
}

/**
 * webkit_user_content_manager_unregister_script_message_handler:
 * @manager: A #CyberKitUserContentManager
 * @name: Name of the script message channel
 *
 * Unregisters a previously registered message handler.
 *
 * Note that this does *not* disconnect handlers for the
 * #CyberKitUserContentManager::script-message-received signal;
 * they will be kept connected, but the signal will not be emitted
 * unless the handler name is registered again.
 *
 * See also webkit_user_content_manager_register_script_message_handler().
 *
 * Since: 2.8
 */
void webkit_user_content_manager_unregister_script_message_handler(CyberKitUserContentManager* manager, const char* name)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    g_return_if_fail(name);
    manager->priv->userContentController->removeUserMessageHandlerForName(String::fromUTF8(name), API::UserContentWorld::normalWorld());
}

/**
 * webkit_user_content_manager_register_script_message_handler_in_world:
 * @manager: A #CyberKitUserContentManager
 * @name: Name of the script message channel
 * @world_name: the name of a #CyberKitScriptWorld
 *
 * Registers a new user script message handler in script world with name @world_name.
 * See webkit_user_content_manager_register_script_message_handler() for full description.
 *
 * Registering a script message handler will fail if the requested
 * name has been already registered before.
 *
 * Returns: %TRUE if message handler was registered successfully, or %FALSE otherwise.
 *
 * Since: 2.22
 */
gboolean webkit_user_content_manager_register_script_message_handler_in_world(CyberKitUserContentManager* manager, const char* name, const char* worldName)
{
    g_return_val_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager), FALSE);
    g_return_val_if_fail(name, FALSE);
    g_return_val_if_fail(worldName, FALSE);

    Ref<WebScriptMessageHandler> handler =
        WebScriptMessageHandler::create(makeUnique<ScriptMessageClientGtk>(manager, name), String::fromUTF8(name), webkitUserContentWorld(worldName));
    return manager->priv->userContentController->addUserScriptMessageHandler(handler.get());
}

/**
 * webkit_user_content_manager_unregister_script_message_handler_in_world:
 * @manager: A #CyberKitUserContentManager
 * @name: Name of the script message channel
 * @world_name: the name of a #CyberKitScriptWorld
 *
 * Unregisters a previously registered message handler in script world with name @world_name.
 *
 * Note that this does *not* disconnect handlers for the
 * #CyberKitUserContentManager::script-message-received signal;
 * they will be kept connected, but the signal will not be emitted
 * unless the handler name is registered again.
 *
 * See also webkit_user_content_manager_register_script_message_handler_in_world().
 *
 * Since: 2.22
 */
void webkit_user_content_manager_unregister_script_message_handler_in_world(CyberKitUserContentManager* manager, const char* name, const char* worldName)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    g_return_if_fail(name);
    g_return_if_fail(worldName);

    manager->priv->userContentController->removeUserMessageHandlerForName(String::fromUTF8(name), webkitUserContentWorld(worldName));
}

/**
 * webkit_user_content_manager_add_filter:
 * @manager: A #CyberKitUserContentManager
 * @filter: A #CyberKitUserContentFilter
 *
 * Adds a #CyberKitUserContentFilter to the given #CyberKitUserContentManager.
 * The same #CyberKitUserContentFilter can be reused with multiple
 * #CyberKitUserContentManager instances.
 *
 * Filters need to be saved and loaded from #CyberKitUserContentFilterStore.
 *
 * Since: 2.24
 */
void webkit_user_content_manager_add_filter(CyberKitUserContentManager* manager, CyberKitUserContentFilter* filter)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    g_return_if_fail(filter);
    manager->priv->userContentController->addContentRuleList(webkitUserContentFilterGetContentRuleList(filter));
}

/**
 * webkit_user_content_manager_remove_filter:
 * @manager: A #CyberKitUserContentManager
 * @filter: A #CyberKitUserContentFilter
 *
 * Removes a filter from the given #CyberKitUserContentManager.
 *
 * Since 2.24
 */
void webkit_user_content_manager_remove_filter(CyberKitUserContentManager* manager, CyberKitUserContentFilter* filter)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    g_return_if_fail(filter);
    manager->priv->userContentController->removeContentRuleList(webkitUserContentFilterGetContentRuleList(filter).name());
}

/**
 * webkit_user_content_manager_remove_filter_by_id:
 * @manager: A #CyberKitUserContentManager
 * @filter_id: Filter identifier
 *
 * Removes a filter from the given #CyberKitUserContentManager given the
 * identifier of a #CyberKitUserContentFilter as returned by
 * webkit_user_content_filter_get_identifier().
 *
 * Since: 2.26
 */
void webkit_user_content_manager_remove_filter_by_id(CyberKitUserContentManager* manager, const char* filterId)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    g_return_if_fail(filterId);
    manager->priv->userContentController->removeContentRuleList(String::fromUTF8(filterId));
}

/**
 * webkit_user_content_manager_remove_all_filters:
 * @manager: A #CyberKitUserContentManager
 *
 * Removes all content filters from the given #CyberKitUserContentManager.
 *
 * Since: 2.24
 */
void webkit_user_content_manager_remove_all_filters(CyberKitUserContentManager* manager)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    manager->priv->userContentController->removeAllContentRuleLists();
}

WebUserContentControllerProxy* webkitUserContentManagerGetUserContentControllerProxy(CyberKitUserContentManager* manager)
{
    return manager->priv->userContentController.get();
}
