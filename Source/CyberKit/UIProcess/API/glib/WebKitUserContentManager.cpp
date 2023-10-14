/*
 * Copyright (C) 2014, 2020 Igalia S.L.
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
#include "CyberKitInitialize.h"
#include "CyberKitUserContentManagerPrivate.h"
#include "CyberKitUserContentPrivate.h"
#include "CyberKitWebContextPrivate.h"
#include "WebScriptMessageHandler.h"
#include <wtf/CompletionHandler.h>
#include <wtf/glib/GRefPtr.h>
#include <wtf/glib/WTFGType.h>

#if !ENABLE(2022_GLIB_API)
#include "CyberKitJavascriptResultPrivate.h"
#endif

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
 * CyberKitUserContentManager:
 *
 * Manages user-defined content which affects web pages.
 *
 * Using a #CyberKitUserContentManager user CSS style sheets can be set to
 * be injected in the web pages loaded by a #CyberKitWebView, by
 * webkit_user_content_manager_add_style_sheet().
 *
 * To use a #CyberKitUserContentManager, it must be created using
 * webkit_user_content_manager_new(), and then used to construct
 * a #CyberKitWebView. User style sheets can be created with
 * webkit_user_style_sheet_new().
 *
 * User style sheets can be added and removed at any time, but
 * they will affect the web pages loaded afterwards.
 *
 * Since: 2.6
 */

WEBKIT_DEFINE_FINAL_TYPE(CyberKitUserContentManager, webkit_user_content_manager, G_TYPE_OBJECT, GObject)

enum {
    SCRIPT_MESSAGE_RECEIVED,
    SCRIPT_MESSAGE_WITH_REPLY_RECEIVED,

    LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0, };

static void webkit_user_content_manager_class_init(CyberKitUserContentManagerClass* klass)
{
    webkitInitialize();

    GObjectClass* gObjectClass = G_OBJECT_CLASS(klass);

    /**
     * CyberKitUserContentManager::script-message-received:
     * @manager: the #CyberKitUserContentManager
     * @value: the value received from the JavaScript world.
     *
     * This signal is emitted when JavaScript in a web view calls
     * <code>window.webkit.messageHandlers.<name>.postMessage()</code>, after registering
     * <code><name></code> using
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
#if ENABLE(2022_GLIB_API)
            g_cclosure_marshal_VOID__OBJECT,
#else
            g_cclosure_marshal_VOID__BOXED,
#endif
            G_TYPE_NONE, 1,
#if ENABLE(2022_GLIB_API)
            JSC_TYPE_VALUE);
#else
            WEBKIT_TYPE_JAVASCRIPT_RESULT);
#endif

    /**
     * CyberKitUserContentManager::script-message-with-reply-received:
     * @manager: the #CyberKitUserContentManager
     * @value: the value received from the JavaScript world.
     * @reply: the #CyberKitScriptMessageReply to send the reply to the script message.
     *
     * This signal is emitted when JavaScript in a web view calls
     * <code>window.webkit.messageHandlers.<name>.postMessage()</code>, after registering
     * <code><name></code> using
     * webkit_user_content_manager_register_script_message_handler_with_reply()
     *
     * The given @reply can be used to send a return value with
     * webkit_script_message_reply_return_value() or an error message with
     * webkit_script_message_reply_return_error_message(). If none of them are
     * called, an automatic reply with an undefined value will be sent.
     *
     * It is possible to handle the reply asynchronously, by simply calling
     * g_object_ref() on the @reply and returning %TRUE.
     *
     * Returns: %TRUE to stop other handlers from being invoked for the event.
     *    %FALSE to propagate the event further.
     *
     * Since: 2.40
     */
    signals[SCRIPT_MESSAGE_WITH_REPLY_RECEIVED] =
        g_signal_new(
            "script-message-with-reply-received",
            G_TYPE_FROM_CLASS(gObjectClass),
            static_cast<GSignalFlags>(G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED),
            0, g_signal_accumulator_true_handled, nullptr,
            nullptr,
            G_TYPE_BOOLEAN, 2,
            JSC_TYPE_VALUE,
            WEBKIT_TYPE_SCRIPT_MESSAGE_REPLY);
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
 *
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
 * webkit_user_content_manager_remove_style_sheet:
 * @manager: A #CyberKitUserContentManager
 * @stylesheet: A #CyberKitUserStyleSheet
 *
 * Removes a #CyberKitUserStyleSheet from the given #CyberKitUserContentManager.
 *
 * See also webkit_user_content_manager_remove_all_style_sheets().
 *
 * Since: 2.32
 */
void webkit_user_content_manager_remove_style_sheet(CyberKitUserContentManager* manager, CyberKitUserStyleSheet* styleSheet)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    g_return_if_fail(styleSheet);
    manager->priv->userContentController->removeUserStyleSheet(webkitUserStyleSheetGetUserStyleSheet(styleSheet));
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
 *
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
 * webkit_user_content_manager_remove_script:
 * @manager: A #CyberKitUserContentManager
 * @script: A #CyberKitUserScript
 *
 * Removes a #CyberKitUserScript from the given #CyberKitUserContentManager.
 *
 * See also webkit_user_content_manager_remove_all_scripts().
 *
 * Since: 2.32
 */
void webkit_user_content_manager_remove_script(CyberKitUserContentManager* manager, CyberKitUserScript* script)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    g_return_if_fail(script);
    manager->priv->userContentController->removeUserScript(webkitUserScriptGetUserScript(script));
}

/**
 * webkit_user_content_manager_remove_all_scripts:
 * @manager: A #CyberKitUserContentManager
 *
 * Removes all user scripts from the given #CyberKitUserContentManager
 *
 * See also webkit_user_content_manager_remove_script().
 *
 * Since: 2.6
 */
void webkit_user_content_manager_remove_all_scripts(CyberKitUserContentManager* manager)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    manager->priv->userContentController->removeAllUserScripts();
}

/**
 * CyberKitScriptMessageReply: (ref-func webkit_script_message_reply_ref) (unref-func webkit_script_message_reply_unref)
 *
 * A reply for a script message received.
 * If no reply has been sent by the user, an automatically generated reply with
 * undefined value with be sent.
 *
 * Since: 2.40
 */
struct _CyberKitScriptMessageReply {
    _CyberKitScriptMessageReply(WTF::Function<void(API::SerializedScriptValue*, const String&)>&& completionHandler)
        : completionHandler(WTFMove(completionHandler))
        , referenceCount(1)
    {
    }

    void sendValue(JSCValue* value)
    {
        auto serializedValue = API::SerializedScriptValue::createFromJSCValue(value);
        completionHandler(serializedValue.get(), { });
    }

    void sendErrorMessage(const char* errorMessage)
    {
        completionHandler(nullptr, String::fromUTF8(errorMessage));
    }

    ~_CyberKitScriptMessageReply()
    {
        if (completionHandler) {
            auto value = adoptGRef(jsc_value_new_undefined(API::SerializedScriptValue::sharedJSCContext()));
            sendValue(value.get());
        }
    }

    WTF::CompletionHandler<void(API::SerializedScriptValue*, const String&)> completionHandler;
    int referenceCount;
};

G_DEFINE_BOXED_TYPE(CyberKitScriptMessageReply, webkit_script_message_reply, webkit_script_message_reply_ref, webkit_script_message_reply_unref)

/**
 * webkit_script_message_reply_ref:
 * @script_message_reply: A #CyberKitScriptMessageReply
 *
 * Atomically increments the reference count of @script_message_reply by one.
 *
 * Returns: the @script_message_reply passed in.
 *
 * Since: 2.40
 */
CyberKitScriptMessageReply*
webkit_script_message_reply_ref(CyberKitScriptMessageReply* scriptMessageReply)
{
    g_return_val_if_fail(scriptMessageReply, nullptr);
    g_atomic_int_inc(&scriptMessageReply->referenceCount);
    return scriptMessageReply;
}

/**
 * webkit_script_message_reply_unref:
 * @script_message_reply: A #CyberKitScriptMessageReply
 *
 * Atomically decrements the reference count of @script_message_reply by one.
 *
 * If the reference count drops to 0, all the memory allocated by the
 * #CyberKitScriptMessageReply is released. This function is MT-safe and may
 * be called from any thread.
 *
 * Since: 2.40
 */
void webkit_script_message_reply_unref(CyberKitScriptMessageReply* scriptMessageReply)
{
    g_return_if_fail(scriptMessageReply);
    if (g_atomic_int_dec_and_test(&scriptMessageReply->referenceCount)) {
        scriptMessageReply->~CyberKitScriptMessageReply();
        fastFree(scriptMessageReply);
    }
}

CyberKitScriptMessageReply* webKitScriptMessageReplyCreate(WTF::Function<void(API::SerializedScriptValue*, const String&)>&& completionHandler)
{
    CyberKitScriptMessageReply* scriptMessageReply = static_cast<CyberKitScriptMessageReply*>(fastMalloc(sizeof(CyberKitScriptMessageReply)));
    new (scriptMessageReply) CyberKitScriptMessageReply(WTFMove(completionHandler));
    return scriptMessageReply;
}

/**
 * webkit_script_message_reply_return_value:
 * @script_message_reply: A #CyberKitScriptMessageReply
 * @reply_value: Reply value of the provided script message
 *
 * Reply to a script message with a value.
 *
 * This function can be called twice for passing the reply value in.
 *
 * Since: 2.40
 */
void webkit_script_message_reply_return_value(CyberKitScriptMessageReply* message, JSCValue* replyValue)
{
    g_return_if_fail(message != nullptr);
    g_return_if_fail(message->completionHandler);

    message->sendValue(replyValue);
}

/**
 * webkit_script_message_reply_return_error_message:
 * @script_message_reply: A #CyberKitScriptMessageReply
 * @error_message: An error message to return as specified by the user's script message
 *
 * Reply to a script message with an error message.
 *
 * Since: 2.40
 */
void
webkit_script_message_reply_return_error_message(CyberKitScriptMessageReply* message, const char* errorMessage)
{
    g_return_if_fail(message != nullptr);
    g_return_if_fail(errorMessage != nullptr);
    g_return_if_fail(message->completionHandler);

    message->sendErrorMessage(errorMessage);
}

class ScriptMessageClientGtk final : public WebScriptMessageHandler::Client {
    WTF_MAKE_FAST_ALLOCATED;
public:
    ScriptMessageClientGtk(CyberKitUserContentManager* manager, const char* handlerName, bool supportsAsyncReply)
        : m_handlerName(g_quark_from_string(handlerName))
        , m_manager(manager)
        , m_supportsAsyncReply(supportsAsyncReply)
    {
    }

    void didPostMessage(WebPageProxy&, FrameInfoData&&, API::ContentWorld&, CyberCore::SerializedScriptValue& serializedScriptValue) override
    {
#if ENABLE(2022_GLIB_API)
        GRefPtr<JSCValue> value = API::SerializedScriptValue::deserialize(serializedScriptValue);
        g_signal_emit(m_manager, signals[SCRIPT_MESSAGE_RECEIVED], m_handlerName, value.get());
#else
        CyberKitJavascriptResult* jsResult = webkitJavascriptResultCreate(serializedScriptValue);
        g_signal_emit(m_manager, signals[SCRIPT_MESSAGE_RECEIVED], m_handlerName, jsResult);
        webkit_javascript_result_unref(jsResult);
#endif
    }

    bool supportsAsyncReply() override
    {
        return m_supportsAsyncReply;
    }

    void didPostMessageWithAsyncReply(WebPageProxy&, FrameInfoData&&, API::ContentWorld&, CyberCore::SerializedScriptValue& serializedScriptValue, WTF::Function<void(API::SerializedScriptValue*, const String&)>&& completionHandler) override
    {
        CyberKitScriptMessageReply* message = webKitScriptMessageReplyCreate(WTFMove(completionHandler));
        GRefPtr<JSCValue> value = API::SerializedScriptValue::deserialize(serializedScriptValue);
        gboolean returnValue;
        g_signal_emit(m_manager, signals[SCRIPT_MESSAGE_WITH_REPLY_RECEIVED], m_handlerName, value.get(), message, &returnValue);
        webkit_script_message_reply_unref(message);
    }

    virtual ~ScriptMessageClientGtk() { }

private:
    GQuark m_handlerName;
    CyberKitUserContentManager* m_manager;
    bool m_supportsAsyncReply;
};

#if !ENABLE(2022_GLIB_API)
gboolean webkit_user_content_manager_register_script_message_handler(CyberKitUserContentManager* manager, const char* name)
{
    g_return_val_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager), FALSE);
    g_return_val_if_fail(name, FALSE);

    Ref<WebScriptMessageHandler> handler =
        WebScriptMessageHandler::create(makeUnique<ScriptMessageClientGtk>(manager, name, false), AtomString::fromUTF8(name), API::ContentWorld::pageContentWorld());
    return manager->priv->userContentController->addUserScriptMessageHandler(handler.get());
}

void webkit_user_content_manager_unregister_script_message_handler(CyberKitUserContentManager* manager, const char* name)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    g_return_if_fail(name);
    manager->priv->userContentController->removeUserMessageHandlerForName(String::fromUTF8(name), API::ContentWorld::pageContentWorld());
}
#else
gboolean webkit_user_content_manager_register_script_message_handler(CyberKitUserContentManager* manager, const char* name, const char* worldName)
{
    g_return_val_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager), FALSE);
    g_return_val_if_fail(name, FALSE);

    Ref<WebScriptMessageHandler> handler =
        WebScriptMessageHandler::create(makeUnique<ScriptMessageClientGtk>(manager, name, false), AtomString::fromUTF8(name), worldName ? webkitContentWorld(worldName) : API::ContentWorld::pageContentWorld());
    return manager->priv->userContentController->addUserScriptMessageHandler(handler.get());
}

void webkit_user_content_manager_unregister_script_message_handler(CyberKitUserContentManager* manager, const char* name, const char* worldName)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    g_return_if_fail(name);

    manager->priv->userContentController->removeUserMessageHandlerForName(String::fromUTF8(name), worldName ? webkitContentWorld(worldName) : API::ContentWorld::pageContentWorld());
}
#endif

/**
 * webkit_user_content_manager_register_script_message_handler_with_reply:
 * @manager: A #CyberKitUserContentManager
 * @name: Name of the script message channel
 * @world_name (nullable): the name of a #CyberKitScriptWorld
 *
 * Registers a new user script message handler in script world with name @world_name.
 *
 * Different from webkit_user_content_manager_register_script_message_handler(),
 * when using this function to register the handler, the connected signal is
 * script-message-with-reply-received, and a reply provided by the user is expected.
 * Otherwise, the user will receive a default undefined value.
 *
 * If %NULL is passed as the @world_name, the default world will be used.
 * See webkit_user_content_manager_register_script_message_handler() for full description.
 *
 * Registering a script message handler will fail if the requested
 * name has been already registered before.
 *
 * The registered handler can be unregistered by using
 * webkit_user_content_manager_unregister_script_message_handler().
 *
 * Returns: %TRUE if message handler was registered successfully, or %FALSE otherwise.
 *
 * Since: 2.40
 */
gboolean webkit_user_content_manager_register_script_message_handler_with_reply(CyberKitUserContentManager* manager, const char* name, const char* worldName)
{
    g_return_val_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager), FALSE);
    g_return_val_if_fail(name, FALSE);

    auto handler = WebScriptMessageHandler::create(makeUnique<ScriptMessageClientGtk>(manager, name, true), AtomString::fromUTF8(name), worldName ? webkitContentWorld(worldName) : API::ContentWorld::pageContentWorld());
    return manager->priv->userContentController->addUserScriptMessageHandler(handler.get());
}

#if !ENABLE(2022_GLIB_API)
gboolean webkit_user_content_manager_register_script_message_handler_in_world(CyberKitUserContentManager* manager, const char* name, const char* worldName)
{
    g_return_val_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager), FALSE);
    g_return_val_if_fail(name, FALSE);
    g_return_val_if_fail(worldName, FALSE);

    Ref<WebScriptMessageHandler> handler =
        WebScriptMessageHandler::create(makeUnique<ScriptMessageClientGtk>(manager, name, false), AtomString::fromUTF8(name), webkitContentWorld(worldName));
    return manager->priv->userContentController->addUserScriptMessageHandler(handler.get());
}

void webkit_user_content_manager_unregister_script_message_handler_in_world(CyberKitUserContentManager* manager, const char* name, const char* worldName)
{
    g_return_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(manager));
    g_return_if_fail(name);
    g_return_if_fail(worldName);

    manager->priv->userContentController->removeUserMessageHandlerForName(String::fromUTF8(name), webkitContentWorld(worldName));
}
#endif

/**
 * webkit_user_content_manager_add_filter:
 * @manager: A #CyberKitUserContentManager
 * @filter: A #CyberKitUserContentFilter
 *
 * Adds a #CyberKitUserContentFilter to the given #CyberKitUserContentManager.
 *
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
#if ENABLE(CONTENT_EXTENSIONS)
    manager->priv->userContentController->addContentRuleList(webkitUserContentFilterGetContentRuleList(filter));
#endif
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
#if ENABLE(CONTENT_EXTENSIONS)
    manager->priv->userContentController->removeContentRuleList(webkitUserContentFilterGetContentRuleList(filter).name());
#endif
}

/**
 * webkit_user_content_manager_remove_filter_by_id:
 * @manager: A #CyberKitUserContentManager
 * @filter_id: Filter identifier
 *
 * Removes a filter by the given identifier.
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
#if ENABLE(CONTENT_EXTENSIONS)
    manager->priv->userContentController->removeContentRuleList(String::fromUTF8(filterId));
#endif
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
#if ENABLE(CONTENT_EXTENSIONS)
    manager->priv->userContentController->removeAllContentRuleLists();
#endif
}

WebUserContentControllerProxy* webkitUserContentManagerGetUserContentControllerProxy(CyberKitUserContentManager* manager)
{
    return manager->priv->userContentController.get();
}
