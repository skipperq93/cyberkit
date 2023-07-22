/*
 * Copyright (C) 2007 Holger Hans Peter Freyther
 * Copyright (C) 2007, 2008 Alp Toker <alp@atoker.com>
 * Copyright (C) 2008 Collabora Ltd.
 * Copyright (C) 2011, 2017 Igalia S.L.
 * Portions Copyright (c) 2011 Motorola Mobility, Inc.  All rights reserved.
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

#if !defined(__WEBKIT_H_INSIDE__) && !defined(WEBKIT2_COMPILATION)
#error "Only <wpe/webkit.h> can be included directly."
#endif

#ifndef CyberKitWebView_h
#define CyberKitWebView_h

#include <wpe/CyberKitAuthenticationRequest.h>
#include <wpe/CyberKitBackForwardList.h>
#include <wpe/CyberKitContextMenu.h>
#include <wpe/CyberKitDefines.h>
#include <wpe/CyberKitEditorState.h>
#include <wpe/CyberKitFileChooserRequest.h>
#include <wpe/CyberKitFindController.h>
#include <wpe/CyberKitFormSubmissionRequest.h>
#include <wpe/CyberKitHitTestResult.h>
#include <wpe/CyberKitInputMethodContext.h>
#include <wpe/CyberKitJavascriptResult.h>
#include <wpe/CyberKitNavigationAction.h>
#include <wpe/CyberKitNotification.h>
#include <wpe/CyberKitPermissionRequest.h>
#include <wpe/CyberKitPolicyDecision.h>
#include <wpe/CyberKitScriptDialog.h>
#include <wpe/CyberKitSettings.h>
#include <wpe/CyberKitURIRequest.h>
#include <wpe/CyberKitUserContentManager.h>
#include <wpe/CyberKitUserMessage.h>
#include <wpe/CyberKitWebContext.h>
#include <wpe/CyberKitWebResource.h>
#include <wpe/CyberKitWebViewBackend.h>
#include <wpe/CyberKitColor.h>
#include <wpe/CyberKitWebViewSessionState.h>
#include <wpe/CyberKitWindowProperties.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_WEB_VIEW            (webkit_web_view_get_type())
#define WEBKIT_WEB_VIEW(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_WEB_VIEW, CyberKitWebView))
#define WEBKIT_IS_WEB_VIEW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_WEB_VIEW))
#define WEBKIT_WEB_VIEW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_WEB_VIEW, CyberKitWebViewClass))
#define WEBKIT_IS_WEB_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_WEB_VIEW))
#define WEBKIT_WEB_VIEW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_WEB_VIEW, CyberKitWebViewClass))

typedef struct _CyberKitWebView        CyberKitWebView;
typedef struct _CyberKitWebViewClass   CyberKitWebViewClass;
typedef struct _CyberKitWebViewPrivate CyberKitWebViewPrivate;

/**
 * CyberKitPolicyDecisionType:
 * @WEBKIT_POLICY_DECISION_TYPE_NAVIGATION_ACTION: This type of policy decision
 *   is requested when CyberKit is about to navigate to a new page in either the
 *   main frame or a subframe. Acceptable policy decisions are either
 *   webkit_policy_decision_use() or webkit_policy_decision_ignore(). This
 *   type of policy decision is always a #CyberKitNavigationPolicyDecision.
 * @WEBKIT_POLICY_DECISION_TYPE_NEW_WINDOW_ACTION: This type of policy decision
 *   is requested when CyberKit is about to create a new window. Acceptable policy
 *   decisions are either webkit_policy_decision_use() or
 *   webkit_policy_decision_ignore(). This type of policy decision is always
 *   a #CyberKitNavigationPolicyDecision. These decisions are useful for implementing
 *   special actions for new windows, such as forcing the new window to open
 *   in a tab when a keyboard modifier is active or handling a special
 *   target attribute on &lt;a&gt; elements.
 * @WEBKIT_POLICY_DECISION_TYPE_RESPONSE: This type of decision is used when CyberKit has
 *   received a response for a network resource and is about to start the load.
 *   Note that these resources include all subresources of a page such as images
 *   and stylesheets as well as main documents. Appropriate policy responses to
 *   this decision are webkit_policy_decision_use(), webkit_policy_decision_ignore(),
 *   or webkit_policy_decision_download(). This type of policy decision is always
 *   a #CyberKitResponsePolicyDecision. This decision is useful for forcing
 *   some types of resources to be downloaded rather than rendered in the WebView
 *   or to block the transfer of resources entirely.
 *
 * Enum values used for determining the type of a policy decision during
 * #CyberKitWebView::decide-policy.
 */
typedef enum {
    WEBKIT_POLICY_DECISION_TYPE_NAVIGATION_ACTION,
    WEBKIT_POLICY_DECISION_TYPE_NEW_WINDOW_ACTION,
    WEBKIT_POLICY_DECISION_TYPE_RESPONSE,
} CyberKitPolicyDecisionType;

/**
 * CyberKitLoadEvent:
 * @WEBKIT_LOAD_STARTED: A new load request has been made.
 * No data has been received yet, empty structures have
 * been allocated to perform the load; the load may still
 * fail due to transport issues such as not being able to
 * resolve a name, or connect to a port.
 * @WEBKIT_LOAD_REDIRECTED: A provisional data source received
 * a server redirect.
 * @WEBKIT_LOAD_COMMITTED: The content started arriving for a page load.
 * The necessary transport requirements are established, and the
 * load is being performed.
 * @WEBKIT_LOAD_FINISHED: Load completed. All resources are done loading
 * or there was an error during the load operation.
 *
 * Enum values used to denote the different events that happen during a
 * #CyberKitWebView load operation.
 */
typedef enum {
    WEBKIT_LOAD_STARTED,
    WEBKIT_LOAD_REDIRECTED,
    WEBKIT_LOAD_COMMITTED,
    WEBKIT_LOAD_FINISHED
} CyberKitLoadEvent;

/**
 * CyberKitSaveMode:
 * @WEBKIT_SAVE_MODE_MHTML: Save the current page using the MHTML format.
 *
 * Enum values to specify the different ways in which a #CyberKitWebView
 * can save its current web page into a self-contained file.
 */
typedef enum {
    WEBKIT_SAVE_MODE_MHTML
} CyberKitSaveMode;

/**
 * CyberKitInsecureContentEvent:
 * @WEBKIT_INSECURE_CONTENT_RUN: Insecure content has been detected by
 * trying to execute any kind of logic (e.g. a script) from an
 * untrusted source.
 * @WEBKIT_INSECURE_CONTENT_DISPLAYED: Insecure content has been
 * detected by trying to display any kind of resource (e.g. an image)
 * from an untrusted source.
 *
 * Enum values used to denote the different events which can trigger
 * the detection of insecure content.
 */
typedef enum {
    WEBKIT_INSECURE_CONTENT_RUN,
    WEBKIT_INSECURE_CONTENT_DISPLAYED
} CyberKitInsecureContentEvent;

/**
 * CyberKitWebProcessTerminationReason:
 * @WEBKIT_WEB_PROCESS_CRASHED: the web process crashed.
 * @WEBKIT_WEB_PROCESS_EXCEEDED_MEMORY_LIMIT: the web process exceeded the memory limit.
 *
 * Enum values used to specify the reason why the web process terminated abnormally.
 *
 * Since: 2.20
 */
typedef enum {
    WEBKIT_WEB_PROCESS_CRASHED,
    WEBKIT_WEB_PROCESS_EXCEEDED_MEMORY_LIMIT
} CyberKitWebProcessTerminationReason;

/**
 * CyberKitFrameDisplayedCallback:
 * @web_view: a #CyberKitWebView
 * @user_data: user data
 *
 * Callback to be called when a frame is displayed in a #webKitWebView.
 *
 * Since: 2.24
 */
typedef void (* CyberKitFrameDisplayedCallback) (CyberKitWebView *web_view,
                                               gpointer       user_data);

struct _CyberKitWebView {
    GObject parent;

    /*< private >*/
    CyberKitWebViewPrivate *priv;
};

struct _CyberKitWebViewClass {
    GObjectClass parent;

    void           (* load_changed)                (CyberKitWebView               *web_view,
                                                    CyberKitLoadEvent              load_event);
    gboolean       (* load_failed)                 (CyberKitWebView               *web_view,
                                                    CyberKitLoadEvent              load_event,
                                                    const gchar                 *failing_uri,
                                                    GError                      *error);

    CyberKitWebView *(* create)                      (CyberKitWebView               *web_view,
                                                    CyberKitNavigationAction      *navigation_action);
    void           (* ready_to_show)               (CyberKitWebView               *web_view);
    void           (* run_as_modal)                (CyberKitWebView               *web_view);
    void           (* close)                       (CyberKitWebView               *web_view);

    gboolean       (* script_dialog)               (CyberKitWebView               *web_view,
                                                    CyberKitScriptDialog          *dialog);

    gboolean       (* decide_policy)               (CyberKitWebView               *web_view,
                                                    CyberKitPolicyDecision        *decision,
                                                    CyberKitPolicyDecisionType     type);
    gboolean       (* permission_request)          (CyberKitWebView               *web_view,
                                                    CyberKitPermissionRequest     *permission_request);
    void           (* mouse_target_changed)        (CyberKitWebView               *web_view,
                                                    CyberKitHitTestResult         *hit_test_result,
                                                    guint                        modifiers);
    void           (* resource_load_started)       (CyberKitWebView               *web_view,
                                                    CyberKitWebResource           *resource,
                                                    CyberKitURIRequest            *request);
    gboolean       (* enter_fullscreen)            (CyberKitWebView               *web_view);
    gboolean       (* leave_fullscreen)            (CyberKitWebView               *web_view);
    gboolean       (* run_file_chooser)            (CyberKitWebView               *web_view,
                                                    CyberKitFileChooserRequest    *request);
    gboolean       (* context_menu)                (CyberKitWebView               *web_view,
                                                    CyberKitContextMenu           *context_menu,
                                                    void                        *event, /* FIXME: Use a wpe thing here. I'm not sure we want to expose libwpe in the API. */
                                                    CyberKitHitTestResult         *hit_test_result);
    void           (* context_menu_dismissed)      (CyberKitWebView               *web_view);
    void           (* submit_form)                 (CyberKitWebView               *web_view,
                                                    CyberKitFormSubmissionRequest *request);
    void           (* insecure_content_detected)   (CyberKitWebView               *web_view,
                                                    CyberKitInsecureContentEvent   event);
    gboolean       (* web_process_crashed)         (CyberKitWebView               *web_view);

    gboolean       (* authenticate)                (CyberKitWebView               *web_view,
                                                    CyberKitAuthenticationRequest *request);
    gboolean       (* load_failed_with_tls_errors) (CyberKitWebView               *web_view,
                                                    const gchar                 *failing_uri,
                                                    GTlsCertificate             *certificate,
                                                    GTlsCertificateFlags         errors);
    gboolean       (* show_notification)           (CyberKitWebView               *web_view,
                                                    CyberKitNotification          *notification);
    void           (* web_process_terminated)      (CyberKitWebView               *web_view,
                                                    CyberKitWebProcessTerminationReason reason);
    gboolean       (* user_message_received)       (CyberKitWebView               *web_view,
                                                    CyberKitUserMessage           *message);


    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
    void (*_webkit_reserved4) (void);
    void (*_webkit_reserved5) (void);
    void (*_webkit_reserved6) (void);
};

WEBKIT_API GType
webkit_web_view_get_type                             (void);

WEBKIT_API CyberKitWebView *
webkit_web_view_new                                  (CyberKitWebViewBackend      *backend);

WEBKIT_API CyberKitWebView *
webkit_web_view_new_with_context                     (CyberKitWebViewBackend      *backend,
                                                      CyberKitWebContext          *context);

WEBKIT_API CyberKitWebView *
webkit_web_view_new_with_settings                    (CyberKitWebViewBackend      *backend,
                                                      CyberKitSettings            *settings);

WEBKIT_API CyberKitWebView *
webkit_web_view_new_with_related_view                (CyberKitWebViewBackend      *backend,
                                                      CyberKitWebView             *web_view);

WEBKIT_API CyberKitWebView *
webkit_web_view_new_with_user_content_manager        (CyberKitWebViewBackend      *backend,
                                                      CyberKitUserContentManager  *user_content_manager);

WEBKIT_API CyberKitWebViewBackend *
webkit_web_view_get_backend                          (CyberKitWebView             *web_view);

WEBKIT_API gboolean
webkit_web_view_is_ephemeral                         (CyberKitWebView             *web_view);

WEBKIT_API gboolean
webkit_web_view_is_controlled_by_automation          (CyberKitWebView             *web_view);

WEBKIT_API CyberKitAutomationBrowsingContextPresentation
webkit_web_view_get_automation_presentation_type     (CyberKitWebView             *web_view);

WEBKIT_API CyberKitWebsiteDataManager *
webkit_web_view_get_website_data_manager             (CyberKitWebView             *web_view);

WEBKIT_API CyberKitWebContext *
webkit_web_view_get_context                          (CyberKitWebView             *web_view);

WEBKIT_API void
webkit_web_view_try_close                            (CyberKitWebView             *web_view);

WEBKIT_API void
webkit_web_view_load_uri                             (CyberKitWebView             *web_view,
                                                      const gchar               *uri);

WEBKIT_API void
webkit_web_view_load_html                            (CyberKitWebView             *web_view,
                                                      const gchar               *content,
                                                      const gchar               *base_uri);
WEBKIT_API void
webkit_web_view_load_alternate_html                  (CyberKitWebView             *web_view,
                                                      const gchar               *content,
                                                      const gchar               *content_uri,
                                                      const gchar               *base_uri);
WEBKIT_API void
webkit_web_view_load_plain_text                      (CyberKitWebView             *web_view,
                                                      const gchar               *plain_text);

WEBKIT_API void
webkit_web_view_load_bytes                           (CyberKitWebView             *web_view,
                                                      GBytes                    *bytes,
                                                      const gchar               *mime_type,
                                                      const gchar               *encoding,
                                                      const gchar               *base_uri);

WEBKIT_API void
webkit_web_view_load_request                         (CyberKitWebView             *web_view,
                                                      CyberKitURIRequest          *request);

WEBKIT_API void
webkit_web_view_stop_loading                         (CyberKitWebView             *web_view);

WEBKIT_API gboolean
webkit_web_view_is_loading                           (CyberKitWebView             *web_view);

WEBKIT_API gboolean
webkit_web_view_is_playing_audio                     (CyberKitWebView             *web_view);

WEBKIT_API guint64
webkit_web_view_get_page_id                          (CyberKitWebView             *web_view);

WEBKIT_API const gchar *
webkit_web_view_get_title                            (CyberKitWebView             *web_view);

WEBKIT_API void
webkit_web_view_reload                               (CyberKitWebView             *web_view);

WEBKIT_API void
webkit_web_view_reload_bypass_cache                  (CyberKitWebView             *web_view);

WEBKIT_API gdouble
webkit_web_view_get_estimated_load_progress          (CyberKitWebView             *web_view);

WEBKIT_API void
webkit_web_view_go_back                              (CyberKitWebView             *web_view);

WEBKIT_API gboolean
webkit_web_view_can_go_back                          (CyberKitWebView             *web_view);

WEBKIT_API void
webkit_web_view_go_forward                           (CyberKitWebView             *web_view);

WEBKIT_API gboolean
webkit_web_view_can_go_forward                       (CyberKitWebView             *web_view);

WEBKIT_API CyberKitBackForwardList *
webkit_web_view_get_back_forward_list                (CyberKitWebView             *web_view);

WEBKIT_API void
webkit_web_view_go_to_back_forward_list_item         (CyberKitWebView             *web_view,
                                                      CyberKitBackForwardListItem *list_item);
WEBKIT_API const gchar *
webkit_web_view_get_uri                              (CyberKitWebView             *web_view);

WEBKIT_API const gchar *
webkit_web_view_get_custom_charset                   (CyberKitWebView             *web_view);

WEBKIT_API void
webkit_web_view_set_custom_charset                   (CyberKitWebView             *web_view,
                                                      const gchar               *charset);

WEBKIT_API void
webkit_web_view_set_settings                         (CyberKitWebView             *web_view,
                                                      CyberKitSettings            *settings);

WEBKIT_API CyberKitSettings *
webkit_web_view_get_settings                         (CyberKitWebView             *web_view);

WEBKIT_API CyberKitWindowProperties *
webkit_web_view_get_window_properties                (CyberKitWebView             *web_view);

WEBKIT_API void
webkit_web_view_set_zoom_level                       (CyberKitWebView             *web_view,
                                                      gdouble                    zoom_level);
WEBKIT_API gdouble
webkit_web_view_get_zoom_level                       (CyberKitWebView             *web_view);

WEBKIT_API void
webkit_web_view_can_execute_editing_command          (CyberKitWebView             *web_view,
                                                      const gchar               *command,
                                                      GCancellable              *cancellable,
                                                      GAsyncReadyCallback        callback,
                                                      gpointer                   user_data);

WEBKIT_API gboolean
webkit_web_view_can_execute_editing_command_finish   (CyberKitWebView             *web_view,
                                                      GAsyncResult              *result,
                                                      GError                   **error);

WEBKIT_API void
webkit_web_view_execute_editing_command              (CyberKitWebView             *web_view,
                                                      const gchar               *command);

WEBKIT_API void
webkit_web_view_execute_editing_command_with_argument(CyberKitWebView             *web_view,
                                                      const char                *command,
                                                      const char                *argument);

WEBKIT_API CyberKitFindController *
webkit_web_view_get_find_controller                  (CyberKitWebView             *web_view);

WEBKIT_API void
webkit_web_view_run_javascript                       (CyberKitWebView             *web_view,
                                                      const gchar               *script,
                                                      GCancellable              *cancellable,
                                                      GAsyncReadyCallback        callback,
                                                      gpointer                   user_data);
WEBKIT_API CyberKitJavascriptResult *
webkit_web_view_run_javascript_finish                (CyberKitWebView             *web_view,
                                                      GAsyncResult              *result,
                                                      GError                   **error);

WEBKIT_API void
webkit_web_view_run_javascript_in_world              (CyberKitWebView             *web_view,
                                                      const gchar               *script,
                                                      const gchar               *world_name,
                                                      GCancellable              *cancellable,
                                                      GAsyncReadyCallback        callback,
                                                      gpointer                   user_data);
WEBKIT_API CyberKitJavascriptResult *
webkit_web_view_run_javascript_in_world_finish       (CyberKitWebView             *web_view,
                                                      GAsyncResult              *result,
                                                      GError                   **error);

WEBKIT_API void
webkit_web_view_run_javascript_from_gresource        (CyberKitWebView             *web_view,
                                                      const gchar               *resource,
                                                      GCancellable              *cancellable,
                                                      GAsyncReadyCallback        callback,
                                                      gpointer                   user_data);

WEBKIT_API CyberKitJavascriptResult *
webkit_web_view_run_javascript_from_gresource_finish (CyberKitWebView             *web_view,
                                                      GAsyncResult              *result,
                                                      GError                   **error);

WEBKIT_API CyberKitWebResource *
webkit_web_view_get_main_resource                    (CyberKitWebView             *web_view);

WEBKIT_API gboolean
webkit_web_view_can_show_mime_type                   (CyberKitWebView             *web_view,
                                                      const gchar               *mime_type);

WEBKIT_API void
webkit_web_view_save                                 (CyberKitWebView             *web_view,
                                                      CyberKitSaveMode             save_mode,
                                                      GCancellable              *cancellable,
                                                      GAsyncReadyCallback        callback,
                                                      gpointer                   user_data);

WEBKIT_API GInputStream *
webkit_web_view_save_finish                          (CyberKitWebView             *web_view,
                                                      GAsyncResult              *result,
                                                      GError                   **error);

WEBKIT_API void
webkit_web_view_save_to_file                         (CyberKitWebView             *web_view,
                                                      GFile                     *file,
                                                      CyberKitSaveMode             save_mode,
                                                      GCancellable              *cancellable,
                                                      GAsyncReadyCallback        callback,
                                                      gpointer                   user_data);

WEBKIT_API gboolean
webkit_web_view_save_to_file_finish                  (CyberKitWebView             *web_view,
                                                      GAsyncResult              *result,
                                                      GError                   **error);

WEBKIT_API CyberKitDownload *
webkit_web_view_download_uri                         (CyberKitWebView             *web_view,
                                                      const char                *uri);

WEBKIT_API gboolean
webkit_web_view_get_tls_info                         (CyberKitWebView             *web_view,
                                                      GTlsCertificate          **certificate,
                                                      GTlsCertificateFlags      *errors);

WEBKIT_API CyberKitUserContentManager *
webkit_web_view_get_user_content_manager             (CyberKitWebView             *web_view);

WEBKIT_API gboolean
webkit_web_view_is_editable                          (CyberKitWebView             *web_view);

WEBKIT_API void
webkit_web_view_set_editable                         (CyberKitWebView             *web_view,
                                                      gboolean                  editable);

WEBKIT_API CyberKitEditorState *
webkit_web_view_get_editor_state                     (CyberKitWebView             *web_view);

WEBKIT_API CyberKitWebViewSessionState *
webkit_web_view_get_session_state                    (CyberKitWebView             *web_view);

WEBKIT_API void
webkit_web_view_restore_session_state                (CyberKitWebView             *web_view,
                                                      CyberKitWebViewSessionState *state);

WEBKIT_API guint
webkit_web_view_add_frame_displayed_callback         (CyberKitWebView               *web_view,
                                                      CyberKitFrameDisplayedCallback callback,
                                                      gpointer                     user_data,
                                                      GDestroyNotify               destroy_notify);

WEBKIT_API void
webkit_web_view_remove_frame_displayed_callback      (CyberKitWebView               *web_view,
                                                      guint                        id);

WEBKIT_API void
webkit_web_view_set_background_color                 (CyberKitWebView               *web_view,
                                                      CyberKitColor                 *color);
WEBKIT_API void
webkit_web_view_get_background_color                 (CyberKitWebView               *web_view,
                                                      CyberKitColor                 *color);

WEBKIT_API void
webkit_web_view_send_message_to_page                 (CyberKitWebView               *web_view,
                                                      CyberKitUserMessage           *message,
                                                      GCancellable                *cancellable,
                                                      GAsyncReadyCallback          callback,
                                                      gpointer                     user_data);

WEBKIT_API CyberKitUserMessage *
webkit_web_view_send_message_to_page_finish          (CyberKitWebView               *web_view,
                                                      GAsyncResult                *result,
                                                      GError                     **error);

WEBKIT_API void
webkit_web_view_set_input_method_context             (CyberKitWebView               *web_view,
                                                      CyberKitInputMethodContext    *context);

WEBKIT_API CyberKitInputMethodContext *
webkit_web_view_get_input_method_context             (CyberKitWebView               *web_view);

G_END_DECLS

#endif
