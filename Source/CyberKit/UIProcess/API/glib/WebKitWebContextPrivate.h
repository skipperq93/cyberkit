/*
 * Copyright (C) 2011 Igalia S.L.
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

#include "DownloadProxy.h"
#include "LegacyCustomProtocolManagerProxy.h"
#include "CyberKitUserContentManager.h"
#include "CyberKitWebContext.h"
#include "WebProcessPool.h"
#include <CyberCore/ResourceRequest.h>

CyberKit::WebProcessPool& webkitWebContextGetProcessPool(CyberKitWebContext*);
CyberKitDownload* webkitWebContextGetOrCreateDownload(CyberKit::DownloadProxy*);
CyberKitDownload* webkitWebContextStartDownload(CyberKitWebContext*, const char* uri, CyberKit::WebPageProxy*);
void webkitWebContextRemoveDownload(CyberKit::DownloadProxy*);
void webkitWebContextDownloadStarted(CyberKitWebContext*, CyberKitDownload*);
void webkitWebContextStartLoadingCustomProtocol(CyberKitWebContext*, uint64_t customProtocolID, const CyberCore::ResourceRequest&, CyberKit::LegacyCustomProtocolManagerProxy&);
void webkitWebContextStopLoadingCustomProtocol(CyberKitWebContext*, uint64_t customProtocolID);
void webkitWebContextInvalidateCustomProtocolRequests(CyberKitWebContext*, CyberKit::LegacyCustomProtocolManagerProxy&);
void webkitWebContextDidFinishLoadingCustomProtocol(CyberKitWebContext*, uint64_t customProtocolID);
bool webkitWebContextIsLoadingCustomProtocol(CyberKitWebContext*, uint64_t customProtocolID);
void webkitWebContextCreatePageForWebView(CyberKitWebContext*, CyberKitWebView*, CyberKitUserContentManager*, CyberKitWebView*);
void webkitWebContextWebViewDestroyed(CyberKitWebContext*, CyberKitWebView*);
CyberKitWebView* webkitWebContextGetWebViewForPage(CyberKitWebContext*, CyberKit::WebPageProxy*);
GVariant* webkitWebContextInitializeWebExtensions(CyberKitWebContext*);
void webkitWebContextInitializeNotificationPermissions(CyberKitWebContext*);
#if ENABLE(REMOTE_INSPECTOR)
void webkitWebContextWillCloseAutomationSession(CyberKitWebContext*);
#endif
