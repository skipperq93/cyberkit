/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
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

#ifndef CyberKit2_C_h
#define CyberKit2_C_h

#include <CyberKit/WKBase.h>
#include <CyberKit/WKType.h>

#include <CyberKit/WKArray.h>
#include <CyberKit/WKBackForwardListRef.h>
#include <CyberKit/WKBackForwardListItemRef.h>
#include <CyberKit/WKConnectionRef.h>
#include <CyberKit/WKContext.h>
#include <CyberKit/WKData.h>
#include <CyberKit/WKDictionary.h>
#include <CyberKit/WKErrorRef.h>
#include <CyberKit/WKFeature.h>
#include <CyberKit/WKFormSubmissionListener.h>
#include <CyberKit/WKFrame.h>
#include <CyberKit/WKFramePolicyListener.h>
#include <CyberKit/WKGeolocationManager.h>
#include <CyberKit/WKGeolocationPermissionRequest.h>
#include <CyberKit/WKGeolocationPosition.h>
#include <CyberKit/WKHitTestResult.h>
#include <CyberKit/WKMutableArray.h>
#include <CyberKit/WKMutableDictionary.h>
#include <CyberKit/WKNavigationDataRef.h>
#include <CyberKit/WKNumber.h>
#include <CyberKit/WKOpenPanelParametersRef.h>
#include <CyberKit/WKOpenPanelResultListener.h>
#include <CyberKit/WKPage.h>
#include <CyberKit/WKPageConfigurationRef.h>
#include <CyberKit/WKPageGroup.h>
#include <CyberKit/WKPreferencesRef.h>
#include <CyberKit/WKString.h>
#include <CyberKit/WKURL.h>
#include <CyberKit/WKURLRequest.h>
#include <CyberKit/WKURLResponse.h>
#include <CyberKit/WKUserContentControllerRef.h>
#include <CyberKit/WKUserMediaPermissionRequest.h>
#include <CyberKit/WKUserScriptRef.h>

#if defined(__OBJC__) && __OBJC__
#import <CyberKit/WKView.h>
#elif !(defined(__APPLE__) && __APPLE__)
#include <CyberKit/WKView.h>
#endif

#endif /* CyberKit2_C_h */
