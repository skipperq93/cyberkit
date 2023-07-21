/*
 * Copyright (C) 2017 Igalia S.L.
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
#include "CyberKitWebView.h"

#include "CyberKitAuthenticationDialog.h"
#include "CyberKitScriptDialogImpl.h"
#include "CyberKitWebViewBasePrivate.h"
#include "CyberKitWebViewPrivate.h"
#include <CyberCore/Color.h>
#include <CyberCore/GtkUtilities.h>
#include <CyberCore/PlatformDisplay.h>
#include <CyberCore/PlatformScreen.h>
#include <glib/gi18n-lib.h>
#include <gtk/gtk.h>

gboolean webkitWebViewAuthenticate(CyberKitWebView* webView, CyberKitAuthenticationRequest* request)
{
    CredentialStorageMode credentialStorageMode = webkit_authentication_request_can_save_credentials(request) ? AllowPersistentStorage : DisallowPersistentStorage;
    webkitWebViewBaseAddDialog(WEBKIT_WEB_VIEW_BASE(webView), webkitAuthenticationDialogNew(request, credentialStorageMode));

    return TRUE;
}

gboolean webkitWebViewScriptDialog(CyberKitWebView* webView, CyberKitScriptDialog* scriptDialog)
{
    GUniquePtr<char> title(g_strdup_printf("JavaScript - %s", webkitWebViewGetPage(webView).pageLoadState().url().utf8().data()));
    // Limit script dialog size to 80% of the web view size.
    GtkRequisition maxSize = { static_cast<int>(gtk_widget_get_allocated_width(GTK_WIDGET(webView)) * 0.80), static_cast<int>(gtk_widget_get_allocated_height(GTK_WIDGET(webView)) * 0.80) };
    webkitWebViewBaseAddDialog(WEBKIT_WEB_VIEW_BASE(webView), webkitScriptDialogImplNew(scriptDialog, title.get(), &maxSize));

    return TRUE;
}

static void fileChooserDialogResponseCallback(GtkFileChooser* dialog, gint responseID, CyberKitFileChooserRequest* request)
{
    GRefPtr<CyberKitFileChooserRequest> adoptedRequest = adoptGRef(request);
    if (responseID == GTK_RESPONSE_ACCEPT) {
        GUniquePtr<GSList> filesList(gtk_file_chooser_get_filenames(dialog));
        GRefPtr<GPtrArray> filesArray = adoptGRef(g_ptr_array_new());
        for (GSList* file = filesList.get(); file; file = g_slist_next(file))
            g_ptr_array_add(filesArray.get(), file->data);
        g_ptr_array_add(filesArray.get(), 0);
        webkit_file_chooser_request_select_files(adoptedRequest.get(), reinterpret_cast<const gchar* const*>(filesArray->pdata));
    } else
        webkit_file_chooser_request_cancel(adoptedRequest.get());

    g_object_unref(dialog);
}

gboolean webkitWebViewRunFileChooser(CyberKitWebView* webView, CyberKitFileChooserRequest* request)
{
    GtkWidget* toplevel = gtk_widget_get_toplevel(GTK_WIDGET(webView));
    if (!CyberCore::widgetIsOnscreenToplevelWindow(toplevel))
        toplevel = 0;

    gboolean allowsMultipleSelection = webkit_file_chooser_request_get_select_multiple(request);

    GtkFileChooserNative* dialog = gtk_file_chooser_native_new(allowsMultipleSelection ? _("Select Files") : _("Select File"),
        toplevel ? GTK_WINDOW(toplevel) : nullptr, GTK_FILE_CHOOSER_ACTION_OPEN, nullptr, nullptr);
    if (toplevel)
        gtk_native_dialog_set_modal(GTK_NATIVE_DIALOG(dialog), TRUE);

    if (GtkFileFilter* filter = webkit_file_chooser_request_get_mime_types_filter(request))
        gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);
    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), allowsMultipleSelection);

    if (const gchar* const* selectedFiles = webkit_file_chooser_request_get_selected_files(request))
        gtk_file_chooser_select_filename(GTK_FILE_CHOOSER(dialog), selectedFiles[0]);

    g_signal_connect(dialog, "response", G_CALLBACK(fileChooserDialogResponseCallback), g_object_ref(request));

    gtk_native_dialog_show(GTK_NATIVE_DIALOG(dialog));

    return TRUE;
}

struct WindowStateEvent {
    enum class Type { Maximize, Minimize, Restore };

    WindowStateEvent(Type type, CompletionHandler<void()>&& completionHandler)
        : type(type)
        , completionHandler(WTFMove(completionHandler))
        , completeTimer(RunLoop::main(), this, &WindowStateEvent::complete)
    {
        // Complete the event if not done after one second.
        completeTimer.startOneShot(1_s);
    }

    ~WindowStateEvent()
    {
        complete();
    }

    void complete()
    {
        if (auto handler = std::exchange(completionHandler, nullptr))
            handler();
    }

    Type type;
    CompletionHandler<void()> completionHandler;
    RunLoop::Timer<WindowStateEvent> completeTimer;
};

static const char* gWindowStateEventID = "wk-window-state-event";

static gboolean windowStateEventCallback(GtkWidget* window, GdkEventWindowState* event, CyberKitWebView* view)
{
    auto* state = static_cast<WindowStateEvent*>(g_object_get_data(G_OBJECT(view), gWindowStateEventID));
    if (!state) {
        g_signal_handlers_disconnect_by_func(window, reinterpret_cast<gpointer>(windowStateEventCallback), view);
        return FALSE;
    }

    bool eventCompleted = false;
    switch (state->type) {
    case WindowStateEvent::Type::Maximize:
        if (event->new_window_state & GDK_WINDOW_STATE_MAXIMIZED)
            eventCompleted = true;
        break;
    case WindowStateEvent::Type::Minimize:
        if ((event->new_window_state & GDK_WINDOW_STATE_ICONIFIED) || !gtk_widget_get_mapped(window))
            eventCompleted = true;
        break;
    case WindowStateEvent::Type::Restore:
        if (!(event->new_window_state & GDK_WINDOW_STATE_MAXIMIZED) && !(event->new_window_state & GDK_WINDOW_STATE_ICONIFIED))
            eventCompleted = true;
        break;
    }

    if (eventCompleted) {
        g_signal_handlers_disconnect_by_func(window, reinterpret_cast<gpointer>(windowStateEventCallback), view);
        g_object_set_data(G_OBJECT(view), gWindowStateEventID, nullptr);
    }

    return FALSE;
}

void webkitWebViewMaximizeWindow(CyberKitWebView* view, CompletionHandler<void()>&& completionHandler)
{
    auto* topLevel = gtk_widget_get_toplevel(GTK_WIDGET(view));
    if (!gtk_widget_is_toplevel(topLevel)) {
        completionHandler();
        return;
    }

    auto* window = GTK_WINDOW(topLevel);
    if (gtk_window_is_maximized(window)) {
        completionHandler();
        return;
    }

    g_object_set_data_full(G_OBJECT(view), gWindowStateEventID, new WindowStateEvent(WindowStateEvent::Type::Maximize, WTFMove(completionHandler)), [](gpointer userData) {
        delete static_cast<WindowStateEvent*>(userData);
    });
    g_signal_connect_object(window, "window-state-event", G_CALLBACK(windowStateEventCallback), view, G_CONNECT_AFTER);
    gtk_window_maximize(window);
#if ENABLE(DEVELOPER_MODE)
    // Xvfb doesn't support maximize, so we resize the window to the screen size.
    if (CyberCore::PlatformDisplay::sharedDisplay().type() == CyberCore::PlatformDisplay::Type::X11) {
        const char* underXvfb = g_getenv("UNDER_XVFB");
        if (!g_strcmp0(underXvfb, "yes")) {
            auto screenRect = CyberCore::screenAvailableRect(nullptr);
            gtk_window_move(window, screenRect.x(), screenRect.y());
            gtk_window_resize(window, screenRect.width(), screenRect.height());
        }
    }
#endif
    gtk_widget_show(topLevel);
}

void webkitWebViewMinimizeWindow(CyberKitWebView* view, CompletionHandler<void()>&& completionHandler)
{
    auto* topLevel = gtk_widget_get_toplevel(GTK_WIDGET(view));
    if (!gtk_widget_is_toplevel(topLevel)) {
        completionHandler();
        return;
    }

    auto* window = GTK_WINDOW(topLevel);
    g_object_set_data_full(G_OBJECT(view), gWindowStateEventID, new WindowStateEvent(WindowStateEvent::Type::Minimize, WTFMove(completionHandler)), [](gpointer userData) {
        delete static_cast<WindowStateEvent*>(userData);
    });
    g_signal_connect_object(window, "window-state-event", G_CALLBACK(windowStateEventCallback), view, G_CONNECT_AFTER);
    gtk_window_iconify(window);
    gtk_widget_hide(topLevel);
}

void webkitWebViewRestoreWindow(CyberKitWebView* view, CompletionHandler<void()>&& completionHandler)
{
    auto* topLevel = gtk_widget_get_toplevel(GTK_WIDGET(view));
    if (!gtk_widget_is_toplevel(topLevel)) {
        completionHandler();
        return;
    }

    auto* window = GTK_WINDOW(topLevel);
    if (gtk_widget_get_mapped(topLevel) && !gtk_window_is_maximized(window)) {
        completionHandler();
        return;
    }

    g_object_set_data_full(G_OBJECT(view), gWindowStateEventID, new WindowStateEvent(WindowStateEvent::Type::Restore, WTFMove(completionHandler)), [](gpointer userData) {
        delete static_cast<WindowStateEvent*>(userData);
    });
    g_signal_connect_object(window, "window-state-event", G_CALLBACK(windowStateEventCallback), view, G_CONNECT_AFTER);
    if (gtk_window_is_maximized(window))
        gtk_window_unmaximize(window);
    if (!gtk_widget_get_mapped(topLevel))
        gtk_window_deiconify(window);
#if ENABLE(DEVELOPER_MODE)
    // Xvfb doesn't support maximize, so we resize the window to the default size.
    if (CyberCore::PlatformDisplay::sharedDisplay().type() == CyberCore::PlatformDisplay::Type::X11) {
        const char* underXvfb = g_getenv("UNDER_XVFB");
        if (!g_strcmp0(underXvfb, "yes")) {
            int x, y;
            gtk_window_get_default_size(window, &x, &y);
            gtk_window_resize(window, x, y);
        }
    }
#endif
    gtk_widget_show(topLevel);
}

/**
 * webkit_web_view_new:
 *
 * Creates a new #CyberKitWebView with the default #CyberKitWebContext and
 * no #CyberKitUserContentManager associated with it.
 * See also webkit_web_view_new_with_context(),
 * webkit_web_view_new_with_user_content_manager(), and
 * webkit_web_view_new_with_settings().
 *
 * Returns: The newly created #CyberKitWebView widget
 */
GtkWidget* webkit_web_view_new()
{
    return webkit_web_view_new_with_context(webkit_web_context_get_default());
}

/**
 * webkit_web_view_new_with_context:
 * @context: the #CyberKitWebContext to be used by the #CyberKitWebView
 *
 * Creates a new #CyberKitWebView with the given #CyberKitWebContext and
 * no #CyberKitUserContentManager associated with it.
 * See also webkit_web_view_new_with_user_content_manager() and
 * webkit_web_view_new_with_settings().
 *
 * Returns: The newly created #CyberKitWebView widget
 */
GtkWidget* webkit_web_view_new_with_context(CyberKitWebContext* context)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_CONTEXT(context), 0);

    return GTK_WIDGET(g_object_new(WEBKIT_TYPE_WEB_VIEW,
        "is-ephemeral", webkit_web_context_is_ephemeral(context),
        "web-context", context,
        nullptr));
}

/**
 * webkit_web_view_new_with_related_view: (constructor)
 * @web_view: the related #CyberKitWebView
 *
 * Creates a new #CyberKitWebView sharing the same web process with @web_view.
 * This method doesn't have any effect when %WEBKIT_PROCESS_MODEL_SHARED_SECONDARY_PROCESS
 * process model is used, because a single web process is shared for all the web views in the
 * same #CyberKitWebContext. When using %WEBKIT_PROCESS_MODEL_MULTIPLE_SECONDARY_PROCESSES process model,
 * this method should always be used when creating the #CyberKitWebView in the #CyberKitWebView::create signal.
 * You can also use this method to implement other process models based on %WEBKIT_PROCESS_MODEL_MULTIPLE_SECONDARY_PROCESSES,
 * like for example, sharing the same web process for all the views in the same security domain.
 *
 * The newly created #CyberKitWebView will also have the same #CyberKitUserContentManager
 * and #CyberKitSettings as @web_view.
 *
 * Returns: (transfer full): The newly created #CyberKitWebView widget
 *
 * Since: 2.4
 */
GtkWidget* webkit_web_view_new_with_related_view(CyberKitWebView* webView)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), nullptr);

    return GTK_WIDGET(g_object_new(WEBKIT_TYPE_WEB_VIEW,
        "user-content-manager", webkit_web_view_get_user_content_manager(webView),
        "settings", webkit_web_view_get_settings(webView),
        "related-view", webView,
        nullptr));
}

/**
 * webkit_web_view_new_with_settings:
 * @settings: a #CyberKitSettings
 *
 * Creates a new #CyberKitWebView with the given #CyberKitSettings.
 * See also webkit_web_view_new_with_context(), and
 * webkit_web_view_new_with_user_content_manager().
 *
 * Returns: The newly created #CyberKitWebView widget
 *
 * Since: 2.6
 */
GtkWidget* webkit_web_view_new_with_settings(CyberKitSettings* settings)
{
    g_return_val_if_fail(WEBKIT_IS_SETTINGS(settings), nullptr);
    return GTK_WIDGET(g_object_new(WEBKIT_TYPE_WEB_VIEW, "settings", settings, nullptr));
}

/**
 * webkit_web_view_new_with_user_content_manager:
 * @user_content_manager: a #CyberKitUserContentManager.
 *
 * Creates a new #CyberKitWebView with the given #CyberKitUserContentManager.
 * The content loaded in the view may be affected by the content injected
 * in the view by the user content manager.
 *
 * Returns: The newly created #CyberKitWebView widget
 *
 * Since: 2.6
 */
GtkWidget* webkit_web_view_new_with_user_content_manager(CyberKitUserContentManager* userContentManager)
{
    g_return_val_if_fail(WEBKIT_IS_USER_CONTENT_MANAGER(userContentManager), nullptr);

    return GTK_WIDGET(g_object_new(WEBKIT_TYPE_WEB_VIEW, "user-content-manager", userContentManager, nullptr));
}

/**
 * webkit_web_view_set_background_color:
 * @web_view: a #CyberKitWebView
 * @rgba: a #GdkRGBA
 *
 * Sets the color that will be used to draw the @web_view background before
 * the actual contents are rendered. Note that if the web page loaded in @web_view
 * specifies a background color, it will take precedence over the @rgba color.
 * By default the @web_view background color is opaque white.
 * Note that the parent window must have a RGBA visual and
 * #GtkWidget:app-paintable property set to %TRUE for backgrounds colors to work.
 *
 * <informalexample><programlisting>
 * static void browser_window_set_background_color (BrowserWindow *window,
 *                                                  const GdkRGBA *rgba)
 * {
 *     CyberKitWebView *web_view;
 *     GdkScreen *screen = gtk_window_get_screen (GTK_WINDOW (window));
 *     GdkVisual *rgba_visual = gdk_screen_get_rgba_visual (screen);
 *
 *     if (!rgba_visual)
 *          return;
 *
 *     gtk_widget_set_visual (GTK_WIDGET (window), rgba_visual);
 *     gtk_widget_set_app_paintable (GTK_WIDGET (window), TRUE);
 *
 *     web_view = browser_window_get_web_view (window);
 *     webkit_web_view_set_background_color (web_view, rgba);
 * }
 * </programlisting></informalexample>
 *
 * Since: 2.8
 */
void webkit_web_view_set_background_color(CyberKitWebView* webView, const GdkRGBA* rgba)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));
    g_return_if_fail(rgba);

    auto& page = *webkitWebViewBaseGetPage(reinterpret_cast<CyberKitWebViewBase*>(webView));
    page.setBackgroundColor(CyberCore::Color(*rgba));
}

/**
 * webkit_web_view_get_background_color:
 * @web_view: a #CyberKitWebView
 * @rgba: (out): a #GdkRGBA to fill in with the background color
 *
 * Gets the color that is used to draw the @web_view background before
 * the actual contents are rendered.
 * For more information see also webkit_web_view_set_background_color()
 *
 * Since: 2.8
 */
void webkit_web_view_get_background_color(CyberKitWebView* webView, GdkRGBA* rgba)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));
    g_return_if_fail(rgba);

    auto& page = *webkitWebViewBaseGetPage(reinterpret_cast<CyberKitWebViewBase*>(webView));
    *rgba = page.backgroundColor().valueOr(CyberCore::Color::white);
}
