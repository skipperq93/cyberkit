/*
 * Copyright (C) 2010-2016 Apple Inc. All rights reserved.
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

#include "config.h"
#include "WKBundle.h"

#include "APIArray.h"
#include "APIData.h"
#include "InjectedBundle.h"
#include "InjectedBundleClient.h"
#include "InjectedBundleScriptWorld.h"
#include "WKAPICast.h"
#include "WKBundleAPICast.h"
#include "WKBundlePrivate.h"
#include "WKMutableArray.h"
#include "WKMutableDictionary.h"
#include "WKNumber.h"
#include "WKRetainPtr.h"
#include "WKString.h"
#include "WebConnection.h"
#include "WebFrame.h"
#include "WebPage.h"
#include "WebPageGroupProxy.h"
#include <CyberCore/DatabaseTracker.h>
#include <CyberCore/MemoryRelease.h>
#include <CyberCore/ResourceLoadObserver.h>
#include <CyberCore/ServiceWorkerThreadProxy.h>

WKTypeID WKBundleGetTypeID()
{
    return CyberKit::toAPI(CyberKit::InjectedBundle::APIType);
}

void WKBundleSetClient(WKBundleRef bundleRef, WKBundleClientBase *wkClient)
{
    CyberKit::toImpl(bundleRef)->setClient(std::make_unique<CyberKit::InjectedBundleClient>(wkClient));
}

void WKBundleSetServiceWorkerProxyCreationCallback(WKBundleRef bundleRef, void (*callback)(uint64_t))
{
    CyberKit::toImpl(bundleRef)->setServiceWorkerProxyCreationCallback(callback);
}

void WKBundlePostMessage(WKBundleRef bundleRef, WKStringRef messageNameRef, WKTypeRef messageBodyRef)
{
    CyberKit::toImpl(bundleRef)->postMessage(CyberKit::toWTFString(messageNameRef), CyberKit::toImpl(messageBodyRef));
}

void WKBundlePostSynchronousMessage(WKBundleRef bundleRef, WKStringRef messageNameRef, WKTypeRef messageBodyRef, WKTypeRef* returnRetainedDataRef)
{
    RefPtr<API::Object> returnData;
    CyberKit::toImpl(bundleRef)->postSynchronousMessage(CyberKit::toWTFString(messageNameRef), CyberKit::toImpl(messageBodyRef), returnData);
    if (returnRetainedDataRef)
        *returnRetainedDataRef = CyberKit::toAPI(returnData.leakRef());
}

WKConnectionRef WKBundleGetApplicationConnection(WKBundleRef bundleRef)
{
    return toAPI(CyberKit::toImpl(bundleRef)->webConnectionToUIProcess());
}

void WKBundleGarbageCollectJavaScriptObjects(WKBundleRef bundleRef)
{
    CyberKit::toImpl(bundleRef)->garbageCollectJavaScriptObjects();
}

void WKBundleGarbageCollectJavaScriptObjectsOnAlternateThreadForDebugging(WKBundleRef bundleRef, bool waitUntilDone)
{
    CyberKit::toImpl(bundleRef)->garbageCollectJavaScriptObjectsOnAlternateThreadForDebugging(waitUntilDone);
}

size_t WKBundleGetJavaScriptObjectsCount(WKBundleRef bundleRef)
{
    return CyberKit::toImpl(bundleRef)->javaScriptObjectsCount();
}

void WKBundleAddUserScript(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, WKBundleScriptWorldRef scriptWorldRef, WKStringRef sourceRef, WKURLRef urlRef, WKArrayRef whitelistRef, WKArrayRef blacklistRef, _WKUserScriptInjectionTime injectionTimeRef, WKUserContentInjectedFrames injectedFramesRef)
{
    CyberKit::toImpl(bundleRef)->addUserScript(CyberKit::toImpl(pageGroupRef), CyberKit::toImpl(scriptWorldRef), CyberKit::toWTFString(sourceRef), CyberKit::toWTFString(urlRef), CyberKit::toImpl(whitelistRef), CyberKit::toImpl(blacklistRef), CyberKit::toUserScriptInjectionTime(injectionTimeRef), CyberKit::toUserContentInjectedFrames(injectedFramesRef));
}

void WKBundleAddUserStyleSheet(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, WKBundleScriptWorldRef scriptWorldRef, WKStringRef sourceRef, WKURLRef urlRef, WKArrayRef whitelistRef, WKArrayRef blacklistRef, WKUserContentInjectedFrames injectedFramesRef)
{
    CyberKit::toImpl(bundleRef)->addUserStyleSheet(CyberKit::toImpl(pageGroupRef), CyberKit::toImpl(scriptWorldRef), CyberKit::toWTFString(sourceRef), CyberKit::toWTFString(urlRef), CyberKit::toImpl(whitelistRef), CyberKit::toImpl(blacklistRef), CyberKit::toUserContentInjectedFrames(injectedFramesRef));
}

void WKBundleRemoveUserScript(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, WKBundleScriptWorldRef scriptWorldRef, WKURLRef urlRef)
{
    CyberKit::toImpl(bundleRef)->removeUserScript(CyberKit::toImpl(pageGroupRef), CyberKit::toImpl(scriptWorldRef), CyberKit::toWTFString(urlRef));
}

void WKBundleRemoveUserStyleSheet(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, WKBundleScriptWorldRef scriptWorldRef, WKURLRef urlRef)
{
    CyberKit::toImpl(bundleRef)->removeUserStyleSheet(CyberKit::toImpl(pageGroupRef), CyberKit::toImpl(scriptWorldRef), CyberKit::toWTFString(urlRef));
}

void WKBundleRemoveUserScripts(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, WKBundleScriptWorldRef scriptWorldRef)
{
    CyberKit::toImpl(bundleRef)->removeUserScripts(CyberKit::toImpl(pageGroupRef), CyberKit::toImpl(scriptWorldRef));
}

void WKBundleRemoveUserStyleSheets(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, WKBundleScriptWorldRef scriptWorldRef)
{
    CyberKit::toImpl(bundleRef)->removeUserStyleSheets(CyberKit::toImpl(pageGroupRef), CyberKit::toImpl(scriptWorldRef));
}

void WKBundleRemoveAllUserContent(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef)
{
    CyberKit::toImpl(bundleRef)->removeAllUserContent(CyberKit::toImpl(pageGroupRef));
}

void WKBundleOverrideBoolPreferenceForTestRunner(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, WKStringRef preference, bool enabled)
{
    CyberKit::toImpl(bundleRef)->overrideBoolPreferenceForTestRunner(CyberKit::toImpl(pageGroupRef), CyberKit::toWTFString(preference), enabled);
}

void WKBundleSetAllowUniversalAccessFromFileURLs(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, bool enabled)
{
    CyberKit::toImpl(bundleRef)->setAllowUniversalAccessFromFileURLs(CyberKit::toImpl(pageGroupRef), enabled);
}

void WKBundleSetAllowFileAccessFromFileURLs(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, bool enabled)
{
    CyberKit::toImpl(bundleRef)->setAllowFileAccessFromFileURLs(CyberKit::toImpl(pageGroupRef), enabled);
}

void WKBundleSetAllowStorageAccessFromFileURLS(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, bool needsQuirk)
{
    CyberKit::toImpl(bundleRef)->setNeedsStorageAccessFromFileURLsQuirk(CyberKit::toImpl(pageGroupRef), needsQuirk);
}

void WKBundleSetMinimumLogicalFontSize(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, int size)
{
    CyberKit::toImpl(bundleRef)->setMinimumLogicalFontSize(CyberKit::toImpl(pageGroupRef), size);
}

void WKBundleSetFrameFlatteningEnabled(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, bool enabled)
{
    CyberKit::toImpl(bundleRef)->setFrameFlatteningEnabled(CyberKit::toImpl(pageGroupRef), enabled);
}

void WKBundleSetAsyncFrameScrollingEnabled(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, bool enabled)
{
    CyberKit::toImpl(bundleRef)->setAsyncFrameScrollingEnabled(CyberKit::toImpl(pageGroupRef), enabled);
}

void WKBundleSetJavaScriptCanAccessClipboard(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, bool enabled)
{
    CyberKit::toImpl(bundleRef)->setJavaScriptCanAccessClipboard(CyberKit::toImpl(pageGroupRef), enabled);
}

void WKBundleSetPrivateBrowsingEnabled(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, bool enabled)
{
    CyberKit::toImpl(bundleRef)->setPrivateBrowsingEnabled(CyberKit::toImpl(pageGroupRef), enabled);
}

void WKBundleSetUseDashboardCompatibilityMode(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, bool enabled)
{
    CyberKit::toImpl(bundleRef)->setUseDashboardCompatibilityMode(CyberKit::toImpl(pageGroupRef), enabled);
}

void WKBundleSetPopupBlockingEnabled(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, bool enabled)
{
    CyberKit::toImpl(bundleRef)->setPopupBlockingEnabled(CyberKit::toImpl(pageGroupRef), enabled);
}

void WKBundleSetAuthorAndUserStylesEnabled(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, bool enabled)
{
    CyberKit::toImpl(bundleRef)->setAuthorAndUserStylesEnabled(CyberKit::toImpl(pageGroupRef), enabled);
}

void WKBundleSetSpatialNavigationEnabled(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, bool enabled)
{
    CyberKit::toImpl(bundleRef)->setSpatialNavigationEnabled(CyberKit::toImpl(pageGroupRef), enabled);
}

void WKBundleAddOriginAccessWhitelistEntry(WKBundleRef bundleRef, WKStringRef sourceOrigin, WKStringRef destinationProtocol, WKStringRef destinationHost, bool allowDestinationSubdomains)
{
    CyberKit::toImpl(bundleRef)->addOriginAccessWhitelistEntry(CyberKit::toWTFString(sourceOrigin), CyberKit::toWTFString(destinationProtocol), CyberKit::toWTFString(destinationHost), allowDestinationSubdomains);
}

void WKBundleRemoveOriginAccessWhitelistEntry(WKBundleRef bundleRef, WKStringRef sourceOrigin, WKStringRef destinationProtocol, WKStringRef destinationHost, bool allowDestinationSubdomains)
{
    CyberKit::toImpl(bundleRef)->removeOriginAccessWhitelistEntry(CyberKit::toWTFString(sourceOrigin), CyberKit::toWTFString(destinationProtocol), CyberKit::toWTFString(destinationHost), allowDestinationSubdomains);
}

void WKBundleResetOriginAccessWhitelists(WKBundleRef bundleRef)
{
    CyberKit::toImpl(bundleRef)->resetOriginAccessWhitelists();
}

void WKBundleSetAsynchronousSpellCheckingEnabled(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, bool enabled)
{
    CyberKit::toImpl(bundleRef)->setAsynchronousSpellCheckingEnabled(CyberKit::toImpl(pageGroupRef), enabled);
}

WKArrayRef WKBundleGetLiveDocumentURLs(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, bool excludeDocumentsInPageGroupPages)
{
    auto liveDocuments = CyberKit::toImpl(bundleRef)->liveDocumentURLs(CyberKit::toImpl(pageGroupRef), excludeDocumentsInPageGroupPages);

    auto liveURLs = adoptWK(WKMutableArrayCreate());

    for (const auto& it : liveDocuments) {
        auto urlInfo = adoptWK(WKMutableDictionaryCreate());

        auto documentIDKey = adoptWK(WKStringCreateWithUTF8CString("id"));
        auto documentURLKey = adoptWK(WKStringCreateWithUTF8CString("url"));

        auto documentIDValue = adoptWK(WKUInt64Create(it.key));
        auto documentURLValue = adoptWK(CyberKit::toCopiedAPI(it.value));

        WKDictionarySetItem(urlInfo.get(), documentIDKey.get(), documentIDValue.get());
        WKDictionarySetItem(urlInfo.get(), documentURLKey.get(), documentURLValue.get());

        WKArrayAppendItem(liveURLs.get(), urlInfo.get());
    }
    
    return liveURLs.leakRef();
}

void WKBundleReportException(JSContextRef context, JSValueRef exception)
{
    CyberKit::InjectedBundle::reportException(context, exception);
}

void WKBundleClearAllDatabases(WKBundleRef)
{
    CyberCore::DatabaseTracker::singleton().deleteAllDatabasesImmediately();
}

void WKBundleSetDatabaseQuota(WKBundleRef bundleRef, uint64_t quota)
{
    // Historically, we've used the following (somewhat nonsensical) string for the databaseIdentifier of local files.
    CyberCore::DatabaseTracker::singleton().setQuota(*CyberKit::SecurityOriginData::fromDatabaseIdentifier("file__0"), quota);
}

void WKBundleReleaseMemory(WKBundleRef)
{
    CyberCore::releaseMemory(WTF::Critical::Yes, WTF::Synchronous::Yes);
}

WKDataRef WKBundleCreateWKDataFromUInt8Array(WKBundleRef bundle, JSContextRef context, JSValueRef data)
{
    return CyberKit::toAPI(&CyberKit::toImpl(bundle)->createWebDataFromUint8Array(context, data).leakRef());
}

int WKBundleNumberOfPages(WKBundleRef bundleRef, WKBundleFrameRef frameRef, double pageWidthInPixels, double pageHeightInPixels)
{
    return CyberKit::toImpl(bundleRef)->numberOfPages(CyberKit::toImpl(frameRef), pageWidthInPixels, pageHeightInPixels);
}

int WKBundlePageNumberForElementById(WKBundleRef bundleRef, WKBundleFrameRef frameRef, WKStringRef idRef, double pageWidthInPixels, double pageHeightInPixels)
{
    return CyberKit::toImpl(bundleRef)->pageNumberForElementById(CyberKit::toImpl(frameRef), CyberKit::toWTFString(idRef), pageWidthInPixels, pageHeightInPixels);
}

WKStringRef WKBundlePageSizeAndMarginsInPixels(WKBundleRef bundleRef, WKBundleFrameRef frameRef, int pageIndex, int width, int height, int marginTop, int marginRight, int marginBottom, int marginLeft)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(bundleRef)->pageSizeAndMarginsInPixels(CyberKit::toImpl(frameRef), pageIndex, width, height, marginTop, marginRight, marginBottom, marginLeft));
}

bool WKBundleIsPageBoxVisible(WKBundleRef bundleRef, WKBundleFrameRef frameRef, int pageIndex)
{
    return CyberKit::toImpl(bundleRef)->isPageBoxVisible(CyberKit::toImpl(frameRef), pageIndex);
}

bool WKBundleIsProcessingUserGesture(WKBundleRef)
{
    return CyberKit::InjectedBundle::isProcessingUserGesture();
}

void WKBundleSetUserStyleSheetLocation(WKBundleRef bundleRef, WKBundlePageGroupRef pageGroupRef, WKStringRef location)
{
    CyberKit::toImpl(bundleRef)->setUserStyleSheetLocation(CyberKit::toImpl(pageGroupRef), CyberKit::toWTFString(location));
}

void WKBundleSetWebNotificationPermission(WKBundleRef bundleRef, WKBundlePageRef pageRef, WKStringRef originStringRef, bool allowed)
{
    CyberKit::toImpl(bundleRef)->setWebNotificationPermission(CyberKit::toImpl(pageRef), CyberKit::toWTFString(originStringRef), allowed);
}

void WKBundleRemoveAllWebNotificationPermissions(WKBundleRef bundleRef, WKBundlePageRef pageRef)
{
    CyberKit::toImpl(bundleRef)->removeAllWebNotificationPermissions(CyberKit::toImpl(pageRef));
}

uint64_t WKBundleGetWebNotificationID(WKBundleRef bundleRef, JSContextRef context, JSValueRef notification)
{
    return CyberKit::toImpl(bundleRef)->webNotificationID(context, notification);
}

void WKBundleSetTabKeyCyclesThroughElements(WKBundleRef bundleRef, WKBundlePageRef pageRef, bool enabled)
{
    CyberKit::toImpl(bundleRef)->setTabKeyCyclesThroughElements(CyberKit::toImpl(pageRef), enabled);
}

void WKBundleClearResourceLoadStatistics(WKBundleRef)
{
    CyberCore::ResourceLoadObserver::shared().clearState();
}

void WKBundleResourceLoadStatisticsNotifyObserver(WKBundleRef)
{
    CyberCore::ResourceLoadObserver::shared().notifyObserver();
}


void WKBundleExtendClassesForParameterCoder(WKBundleRef bundle, WKArrayRef classes)
{
#if PLATFORM(COCOA) && WK_API_ENABLED
    auto classList = CyberKit::toImpl(classes);
    if (!classList)
        return;

    CyberKit::toImpl(bundle)->extendClassesForParameterCoder(*classList);
#endif
}
