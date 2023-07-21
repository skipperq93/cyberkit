/*
 * Copyright (C) 2011 Igalia S.L.
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

#ifdef __WEBKIT_WEB_EXTENSION_H__
#error "Headers <wpe/webkit.h> and <wpe/webkit-web-extension.h> cannot be included together."
#endif

#ifndef __WEBKIT_H__
#define __WEBKIT_H__

#define __WEBKIT_H_INSIDE__

#include <wpe/CyberKitApplicationInfo.h>
#include <wpe/CyberKitAuthenticationRequest.h>
#include <wpe/CyberKitAutomationSession.h>
#include <wpe/CyberKitBackForwardList.h>
#include <wpe/CyberKitBackForwardListItem.h>
#include <wpe/CyberKitContextMenu.h>
#include <wpe/CyberKitContextMenuActions.h>
#include <wpe/CyberKitContextMenuItem.h>
#include <wpe/CyberKitCookieManager.h>
#include <wpe/CyberKitCredential.h>
#include <wpe/CyberKitDefines.h>
#include <wpe/CyberKitDeviceInfoPermissionRequest.h>
#include <wpe/CyberKitDownload.h>
#include <wpe/CyberKitEditingCommands.h>
#include <wpe/CyberKitEditorState.h>
#include <wpe/CyberKitEnumTypes.h>
#include <wpe/CyberKitError.h>
#include <wpe/CyberKitFaviconDatabase.h>
#include <wpe/CyberKitFindController.h>
#include <wpe/CyberKitFormSubmissionRequest.h>
#include <wpe/CyberKitGeolocationManager.h>
#include <wpe/CyberKitGeolocationPermissionRequest.h>
#include <wpe/CyberKitHitTestResult.h>
#include <wpe/CyberKitInstallMissingMediaPluginsPermissionRequest.h>
#include <wpe/CyberKitJavascriptResult.h>
#include <wpe/CyberKitMimeInfo.h>
#include <wpe/CyberKitNavigationAction.h>
#include <wpe/CyberKitNavigationPolicyDecision.h>
#include <wpe/CyberKitNetworkProxySettings.h>
#include <wpe/CyberKitNotification.h>
#include <wpe/CyberKitNotificationPermissionRequest.h>
#include <wpe/CyberKitPermissionRequest.h>
#include <wpe/CyberKitPlugin.h>
#include <wpe/CyberKitResponsePolicyDecision.h>
#include <wpe/CyberKitScriptDialog.h>
#include <wpe/CyberKitSecurityManager.h>
#include <wpe/CyberKitSecurityOrigin.h>
#include <wpe/CyberKitSettings.h>
#include <wpe/CyberKitURIRequest.h>
#include <wpe/CyberKitURIResponse.h>
#include <wpe/CyberKitURISchemeRequest.h>
#include <wpe/CyberKitURIUtilities.h>
#include <wpe/CyberKitUserContent.h>
#include <wpe/CyberKitUserContentFilterStore.h>
#include <wpe/CyberKitUserContentManager.h>
#include <wpe/CyberKitUserMediaPermissionRequest.h>
#include <wpe/CyberKitVersion.h>
#include <wpe/CyberKitWebContext.h>
#include <wpe/CyberKitWebResource.h>
#include <wpe/CyberKitWebView.h>
#include <wpe/CyberKitWebViewBackend.h>
#include <wpe/CyberKitWebViewSessionState.h>
#include <wpe/CyberKitWebsiteData.h>
#include <wpe/CyberKitWebsiteDataManager.h>
#include <wpe/CyberKitWindowProperties.h>

#include <wpe/CyberKitAutocleanups.h>

#undef __WEBKIT_H_INSIDE__

#endif /* __WEBKIT_H__ */
