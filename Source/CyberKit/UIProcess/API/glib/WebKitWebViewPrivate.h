/*
 * Copyright (C) 2011, 2017 Igalia S.L.
 * Portions Copyright (c) 2011 Motorola Mobility, Inc.  All rights reserved.
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

#pragma once

#include "APIPageConfiguration.h"
#include "InstallMissingMediaPluginsPermissionRequest.h"
#include "WebContextMenuItemData.h"
#include "WebEvent.h"
#include "WebHitTestResultData.h"
#include "WebImage.h"
#include "CyberKitWebView.h"
#include "WebPageProxy.h"
#include <CyberCore/IntRect.h>
#include <CyberCore/LinkIcon.h>
#include <wtf/CompletionHandler.h>
#include <wtf/text/CString.h>

void webkitWebViewCreatePage(CyberKitWebView*, Ref<API::PageConfiguration>&&);
CyberKit::WebPageProxy& webkitWebViewGetPage(CyberKitWebView*);
void webkitWebViewWillStartLoad(CyberKitWebView*);
void webkitWebViewLoadChanged(CyberKitWebView*, CyberKitLoadEvent);
void webkitWebViewLoadFailed(CyberKitWebView*, CyberKitLoadEvent, const char* failingURI, GError*);
void webkitWebViewLoadFailedWithTLSErrors(CyberKitWebView*, const char* failingURI, GError*, GTlsCertificateFlags, GTlsCertificate*);
#if PLATFORM(GTK)
void webkitWebViewGetLoadDecisionForIcon(CyberKitWebView*, const CyberCore::LinkIcon&, Function<void(bool)>&&);
void webkitWebViewSetIcon(CyberKitWebView*, const CyberCore::LinkIcon&, API::Data&);
#endif
CyberKit::WebPageProxy* webkitWebViewCreateNewPage(CyberKitWebView*, const CyberCore::WindowFeatures&, CyberKitNavigationAction*);
void webkitWebViewReadyToShowPage(CyberKitWebView*);
void webkitWebViewRunAsModal(CyberKitWebView*);
void webkitWebViewClosePage(CyberKitWebView*);
void webkitWebViewRunJavaScriptAlert(CyberKitWebView*, const CString& message, Function<void()>&& completionHandler);
void webkitWebViewRunJavaScriptConfirm(CyberKitWebView*, const CString& message, Function<void(bool)>&& completionHandler);
void webkitWebViewRunJavaScriptPrompt(CyberKitWebView*, const CString& message, const CString& defaultText, Function<void(const String&)>&& completionHandler);
void webkitWebViewRunJavaScriptBeforeUnloadConfirm(CyberKitWebView*, const CString& message, Function<void(bool)>&& completionHandler);
bool webkitWebViewIsShowingScriptDialog(CyberKitWebView*);
bool webkitWebViewIsScriptDialogRunning(CyberKitWebView*, CyberKitScriptDialog*);
String webkitWebViewGetCurrentScriptDialogMessage(CyberKitWebView*);
void webkitWebViewSetCurrentScriptDialogUserInput(CyberKitWebView*, const String&);
void webkitWebViewAcceptCurrentScriptDialog(CyberKitWebView*);
void webkitWebViewDismissCurrentScriptDialog(CyberKitWebView*);
Optional<CyberKitScriptDialogType> webkitWebViewGetCurrentScriptDialogType(CyberKitWebView*);
void webkitWebViewMakePermissionRequest(CyberKitWebView*, CyberKitPermissionRequest*);
void webkitWebViewMakePolicyDecision(CyberKitWebView*, CyberKitPolicyDecisionType, CyberKitPolicyDecision*);
void webkitWebViewMouseTargetChanged(CyberKitWebView*, const CyberKit::WebHitTestResultData&, OptionSet<CyberKit::WebEvent::Modifier>);
void webkitWebViewHandleDownloadRequest(CyberKitWebView*, CyberKit::DownloadProxy*);
void webkitWebViewPrintFrame(CyberKitWebView*, CyberKit::WebFrameProxy*);
void webkitWebViewResourceLoadStarted(CyberKitWebView*, CyberKit::WebFrameProxy*, uint64_t resourceIdentifier, CyberKitURIRequest*);
void webkitWebViewRunFileChooserRequest(CyberKitWebView*, CyberKitFileChooserRequest*);
CyberKitWebResource* webkitWebViewGetLoadingWebResource(CyberKitWebView*, uint64_t resourceIdentifier);
#if PLATFORM(GTK)
void webKitWebViewDidReceiveSnapshot(CyberKitWebView*, uint64_t callbackID, CyberKit::WebImage*);
#endif
void webkitWebViewRemoveLoadingWebResource(CyberKitWebView*, uint64_t resourceIdentifier);
void webkitWebViewMaximizeWindow(CyberKitWebView*, CompletionHandler<void()>&&);
void webkitWebViewMinimizeWindow(CyberKitWebView*, CompletionHandler<void()>&&);
void webkitWebViewRestoreWindow(CyberKitWebView*, CompletionHandler<void()>&&);
void webkitWebViewEnterFullScreen(CyberKitWebView*);
void webkitWebViewExitFullScreen(CyberKitWebView*);
void webkitWebViewPopulateContextMenu(CyberKitWebView*, const Vector<CyberKit::WebContextMenuItemData>& proposedMenu, const CyberKit::WebHitTestResultData&, GVariant*);
void webkitWebViewSubmitFormRequest(CyberKitWebView*, CyberKitFormSubmissionRequest*);
void webkitWebViewHandleAuthenticationChallenge(CyberKitWebView*, CyberKit::AuthenticationChallengeProxy*);
void webkitWebViewInsecureContentDetected(CyberKitWebView*, CyberKitInsecureContentEvent);
bool webkitWebViewEmitShowNotification(CyberKitWebView*, CyberKitNotification*);
void webkitWebViewWebProcessTerminated(CyberKitWebView*, CyberKitWebProcessTerminationReason);
void webkitWebViewIsPlayingAudioChanged(CyberKitWebView*);
void webkitWebViewSelectionDidChange(CyberKitWebView*);
void webkitWebViewRequestInstallMissingMediaPlugins(CyberKitWebView*, CyberKit::InstallMissingMediaPluginsPermissionRequest&);
CyberKitWebsiteDataManager* webkitWebViewGetWebsiteDataManager(CyberKitWebView*);

#if PLATFORM(GTK)
bool webkitWebViewEmitRunColorChooser(CyberKitWebView*, CyberKitColorChooserRequest*);
bool webkitWebViewShowOptionMenu(CyberKitWebView*, const CyberCore::IntRect&, CyberKitOptionMenu*, const GdkEvent*);
#endif

gboolean webkitWebViewAuthenticate(CyberKitWebView*, CyberKitAuthenticationRequest*);
gboolean webkitWebViewScriptDialog(CyberKitWebView*, CyberKitScriptDialog*);
gboolean webkitWebViewRunFileChooser(CyberKitWebView*, CyberKitFileChooserRequest*);
