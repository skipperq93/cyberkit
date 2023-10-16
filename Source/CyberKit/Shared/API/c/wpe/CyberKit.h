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

#ifndef WPE_CyberKit_h
#define WPE_CyberKit_h

#define BUILDING_WPE__

#include <CyberKit/WKBase.h>
#include <CyberKit/WKType.h>

// From Source/CyberKit/Shared/API/c/
#include <CyberKit/WKArray.h>
#include <CyberKit/WKData.h>
#include <CyberKit/WKDeclarationSpecifiers.h>
#include <CyberKit/WKDictionary.h>
#include <CyberKit/WKErrorRef.h>
#include <CyberKit/WKGeometry.h>
#include <CyberKit/WKMutableArray.h>
#include <CyberKit/WKMutableDictionary.h>
#include <CyberKit/WKNumber.h>
#include <CyberKit/WKSecurityOriginRef.h>
#include <CyberKit/WKString.h>
#include <CyberKit/WKURL.h>
#include <CyberKit/WKURLRequest.h>
#include <CyberKit/WKURLResponse.h>
#include <CyberKit/WKUserContentInjectedFrames.h>
#include <CyberKit/WKUserScriptInjectionTime.h>

// From Source/CyberKit/WebProcess/InjectedBundle/API/c/
#include <CyberKit/WKBundle.h>
#include <CyberKit/WKBundleBackForwardList.h>
#include <CyberKit/WKBundleBackForwardListItem.h>
#include <CyberKit/WKBundleFileHandleRef.h>
#include <CyberKit/WKBundleFrame.h>
#include <CyberKit/WKBundleHitTestResult.h>
#include <CyberKit/WKBundleInitialize.h>
#include <CyberKit/WKBundleInspector.h>
#include <CyberKit/WKBundleNavigationAction.h>
#include <CyberKit/WKBundleNodeHandle.h>
#include <CyberKit/WKBundlePage.h>
#include <CyberKit/WKBundlePageBanner.h>
#include <CyberKit/WKBundlePageContextMenuClient.h>
#include <CyberKit/WKBundlePageEditorClient.h>
#include <CyberKit/WKBundlePageFormClient.h>
#include <CyberKit/WKBundlePageFullScreenClient.h>
#include <CyberKit/WKBundlePageLoaderClient.h>
#include <CyberKit/WKBundlePageOverlay.h>
#include <CyberKit/WKBundlePagePolicyClient.h>
#include <CyberKit/WKBundlePageResourceLoadClient.h>
#include <CyberKit/WKBundlePageUIClient.h>
#include <CyberKit/WKBundleRangeHandle.h>
#include <CyberKit/WKBundleScriptWorld.h>

// From Source/CyberKit/UIProcess/API/C
#include <CyberKit/WKBackForwardListItemRef.h>
#include <CyberKit/WKBackForwardListRef.h>
#include <CyberKit/WKContext.h>
#include <CyberKit/WKContextConfigurationRef.h>
#include <CyberKit/WKCredential.h>
#include <CyberKit/WKCredentialTypes.h>
#include <CyberKit/WKFrame.h>
#include <CyberKit/WKFrameInfoRef.h>
#include <CyberKit/WKFramePolicyListener.h>
#include <CyberKit/WKHitTestResult.h>
#include <CyberKit/WKNavigationActionRef.h>
#include <CyberKit/WKNavigationDataRef.h>
#include <CyberKit/WKNavigationRef.h>
#include <CyberKit/WKNavigationResponseRef.h>
#include <CyberKit/WKPage.h>
#include <CyberKit/WKPageConfigurationRef.h>
#include <CyberKit/WKPageGroup.h>
#include <CyberKit/WKPreferencesRef.h>
#include <CyberKit/WKSessionStateRef.h>
#include <CyberKit/WKUserContentControllerRef.h>
#include <CyberKit/WKUserScriptRef.h>
#include <CyberKit/WKView.h>
#include <CyberKit/WKViewportAttributes.h>
#include <CyberKit/WKWindowFeaturesRef.h>

#endif // WPE_CyberKit_h
