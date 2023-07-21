/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
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

#import <CyberKit/NSAttributedString.h>
#import <CyberKit/WKBackForwardList.h>
#import <CyberKit/WKBackForwardListItem.h>
#import <CyberKit/WKContentRuleList.h>
#import <CyberKit/WKContentRuleListStore.h>
#import <CyberKit/WKContextMenuElementInfo.h>
#import <CyberKit/WKError.h>
#import <CyberKit/WKFoundation.h>
#import <CyberKit/WKFrameInfo.h>
#import <CyberKit/WKHTTPCookieStore.h>
#import <CyberKit/WKNavigation.h>
#import <CyberKit/WKNavigationAction.h>
#import <CyberKit/WKNavigationDelegate.h>
#import <CyberKit/WKNavigationResponse.h>
#import <CyberKit/WKOpenPanelParameters.h>
#import <CyberKit/WKPreferences.h>
#import <CyberKit/WKPreviewActionItem.h>
#import <CyberKit/WKPreviewActionItemIdentifiers.h>
#import <CyberKit/WKPreviewElementInfo.h>
#import <CyberKit/WKProcessPool.h>
#import <CyberKit/WKScriptMessage.h>
#import <CyberKit/WKScriptMessageHandler.h>
#import <CyberKit/WKSecurityOrigin.h>
#import <CyberKit/WKSnapshotConfiguration.h>
#import <CyberKit/WKUIDelegate.h>
#import <CyberKit/WKURLSchemeHandler.h>
#import <CyberKit/WKURLSchemeTask.h>
#import <CyberKit/WKUserContentController.h>
#import <CyberKit/WKUserScript.h>
#import <CyberKit/WKWebView.h>
#import <CyberKit/WKWebViewConfiguration.h>
#import <CyberKit/WKWebpagePreferences.h>
#import <CyberKit/WKWebsiteDataRecord.h>
#import <CyberKit/WKWebsiteDataStore.h>
#import <CyberKit/WKWindowFeatures.h>

#if !defined(TARGET_OS_IOSMAC) || !TARGET_OS_IOSMAC
#import <CyberKit/CyberKitLegacy.h>
#endif
