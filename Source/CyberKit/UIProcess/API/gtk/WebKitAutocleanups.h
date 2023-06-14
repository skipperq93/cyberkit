/*
 * Copyright (C) 2015 Igalia S.L.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined(__WEBKIT2_H_INSIDE__) && !defined(WEBKIT2_COMPILATION)
#error "Only <webkit2/webkit2.h> can be included directly."
#endif

#ifndef CyberKitAutocleanups_h
#define CyberKitAutocleanups_h

#ifdef G_DEFINE_AUTOPTR_CLEANUP_FUNC
#ifndef __GI_SCANNER__

G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitAuthenticationRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitBackForwardList, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitBackForwardListItem, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitColorChooserRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitContextMenu, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitContextMenuItem, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitCookieManager, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitDownload, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitEditorState, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitFaviconDatabase, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitFileChooserRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitFindController, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitFormSubmissionRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitGeolocationPermissionRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitHitTestResult, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitInstallMissingMediaPluginsPermissionRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitNavigationPolicyDecision, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitNotification, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitNotificationPermissionRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitPermissionRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitPlugin, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitPolicyDecision, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitPrintCustomWidget, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitPrintOperation, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitResponsePolicyDecision, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitSecurityManager, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitSettings, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitURIRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitURIResponse, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitURISchemeRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitUserContentManager, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitUserMediaPermissionRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWebContext, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWebInspector, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWebResource, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWebView, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWebsiteDataManager, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWindowProperties, g_object_unref)

G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitCredential, webkit_credential_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitJavascriptResult, webkit_javascript_result_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitMimeInfo, webkit_mime_info_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitNavigationAction, webkit_navigation_action_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitNetworkProxySettings, webkit_network_proxy_settings_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitSecurityOrigin, webkit_security_origin_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitUserScript, webkit_user_script_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitUserStyleSheet, webkit_user_style_sheet_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWebsiteData, webkit_website_data_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWebViewSessionState, webkit_web_view_session_state_unref)

#endif // __GI_SCANNER__
#endif // G_DEFINE_AUTOPTR_CLEANUP_FUNC

#endif // CyberKitAutocleanups_h
