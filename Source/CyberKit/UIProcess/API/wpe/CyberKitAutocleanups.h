/*
 * Copyright (C) 2015 Igalia S.L.
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

#ifndef CyberKitAutocleanups_h
#define CyberKitAutocleanups_h

#ifdef G_DEFINE_AUTOPTR_CLEANUP_FUNC
#ifndef __GI_SCANNER__

G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitAuthenticationRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitAutomationSession, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitBackForwardList, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitBackForwardListItem, g_object_unref)
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
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitInputMethodContext, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitInstallMissingMediaPluginsPermissionRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitNavigationPolicyDecision, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitNotification, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitNotificationPermissionRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitPermissionRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitPlugin, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitPolicyDecision, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitResponsePolicyDecision, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitSecurityManager, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitSettings, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitURIRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitURIResponse, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitURISchemeRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitUserContentFilterStore, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitUserContentManager, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitUserMediaPermissionRequest, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitUserMessage, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWebContext, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWebResource, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWebView, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWebsiteDataManager, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWebsitePolicies, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWindowProperties, g_object_unref)

G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitApplicationInfo, webkit_application_info_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitCredential, webkit_credential_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitITPFirstParty, webkit_itp_first_party_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitITPThirdParty, webkit_itp_third_party_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitJavascriptResult, webkit_javascript_result_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitMimeInfo, webkit_mime_info_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitNavigationAction, webkit_navigation_action_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitNetworkProxySettings, webkit_network_proxy_settings_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitSecurityOrigin, webkit_security_origin_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitUserScript, webkit_user_script_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitUserStyleSheet, webkit_user_style_sheet_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitUserContentFilter, webkit_user_content_filter_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWebsiteData, webkit_website_data_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (CyberKitWebViewSessionState, webkit_web_view_session_state_unref)

#endif /* __GI_SCANNER__ */
#endif /* G_DEFINE_AUTOPTR_CLEANUP_FUNC */

#endif /* CyberKitAutocleanups_h */
