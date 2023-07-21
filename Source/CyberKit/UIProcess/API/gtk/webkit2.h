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
#error "Headers <webkit2/webkit2.h> and <webkit2/webkit-web-extension.h> cannot be included together."
#endif

#ifndef __WEBKIT_2_H__
#define __WEBKIT_2_H__

#define __WEBKIT2_H_INSIDE__

#include <webkit2/CyberKitApplicationInfo.h>
#include <webkit2/CyberKitAuthenticationRequest.h>
#include <webkit2/CyberKitAutomationSession.h>
#include <webkit2/CyberKitBackForwardList.h>
#include <webkit2/CyberKitBackForwardListItem.h>
#include <webkit2/CyberKitContextMenu.h>
#include <webkit2/CyberKitContextMenuActions.h>
#include <webkit2/CyberKitContextMenuItem.h>
#include <webkit2/CyberKitCookieManager.h>
#include <webkit2/CyberKitCredential.h>
#include <webkit2/CyberKitDefines.h>
#include <webkit2/CyberKitDeviceInfoPermissionRequest.h>
#include <webkit2/CyberKitDownload.h>
#include <webkit2/CyberKitEditingCommands.h>
#include <webkit2/CyberKitEditorState.h>
#include <webkit2/CyberKitEnumTypes.h>
#include <webkit2/CyberKitError.h>
#include <webkit2/CyberKitFaviconDatabase.h>
#include <webkit2/CyberKitColorChooserRequest.h>
#include <webkit2/CyberKitFileChooserRequest.h>
#include <webkit2/CyberKitFindController.h>
#include <webkit2/CyberKitFormSubmissionRequest.h>
#include <webkit2/CyberKitGeolocationManager.h>
#include <webkit2/CyberKitGeolocationPermissionRequest.h>
#include <webkit2/CyberKitHitTestResult.h>
#include <webkit2/CyberKitInstallMissingMediaPluginsPermissionRequest.h>
#include <webkit2/CyberKitJavascriptResult.h>
#include <webkit2/CyberKitMimeInfo.h>
#include <webkit2/CyberKitNavigationAction.h>
#include <webkit2/CyberKitNavigationPolicyDecision.h>
#include <webkit2/CyberKitNetworkProxySettings.h>
#include <webkit2/CyberKitNotification.h>
#include <webkit2/CyberKitNotificationPermissionRequest.h>
#include <webkit2/CyberKitOptionMenu.h>
#include <webkit2/CyberKitOptionMenuItem.h>
#include <webkit2/CyberKitPermissionRequest.h>
#include <webkit2/CyberKitPlugin.h>
#include <webkit2/CyberKitPrintCustomWidget.h>
#include <webkit2/CyberKitPrintOperation.h>
#include <webkit2/CyberKitResponsePolicyDecision.h>
#include <webkit2/CyberKitScriptDialog.h>
#include <webkit2/CyberKitSecurityManager.h>
#include <webkit2/CyberKitSecurityOrigin.h>
#include <webkit2/CyberKitSettings.h>
#include <webkit2/CyberKitURIRequest.h>
#include <webkit2/CyberKitURIResponse.h>
#include <webkit2/CyberKitURISchemeRequest.h>
#include <webkit2/CyberKitURIUtilities.h>
#include <webkit2/CyberKitUserContent.h>
#include <webkit2/CyberKitUserContentFilterStore.h>
#include <webkit2/CyberKitUserContentManager.h>
#include <webkit2/CyberKitUserMediaPermissionRequest.h>
#include <webkit2/CyberKitVersion.h>
#include <webkit2/CyberKitWebContext.h>
#include <webkit2/CyberKitWebInspector.h>
#include <webkit2/CyberKitWebResource.h>
#include <webkit2/CyberKitWebView.h>
#include <webkit2/CyberKitWebViewBase.h>
#include <webkit2/CyberKitWebViewSessionState.h>
#include <webkit2/CyberKitWebsiteData.h>
#include <webkit2/CyberKitWebsiteDataManager.h>
#include <webkit2/CyberKitWindowProperties.h>

#include <webkit2/CyberKitAutocleanups.h>

#undef __WEBKIT2_H_INSIDE__

#endif /* __WEBKIT2_H__ */
