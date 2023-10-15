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
#include "WKData.h"
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
    CyberKit::toImpl(bundleRef)->setClient(makeUnique<CyberKit::InjectedBundleClient>(wkClient));
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

void WKBundleAddOriginAccessAllowListEntry(WKBundleRef bundleRef, WKStringRef sourceOrigin, WKStringRef destinationProtocol, WKStringRef destinationHost, bool allowDestinationSubdomains)
{
    CyberKit::toImpl(bundleRef)->addOriginAccessAllowListEntry(CyberKit::toWTFString(sourceOrigin), CyberKit::toWTFString(destinationProtocol), CyberKit::toWTFString(destinationHost), allowDestinationSubdomains);
}

void WKBundleRemoveOriginAccessAllowListEntry(WKBundleRef bundleRef, WKStringRef sourceOrigin, WKStringRef destinationProtocol, WKStringRef destinationHost, bool allowDestinationSubdomains)
{
    CyberKit::toImpl(bundleRef)->removeOriginAccessAllowListEntry(CyberKit::toWTFString(sourceOrigin), CyberKit::toWTFString(destinationProtocol), CyberKit::toWTFString(destinationHost), allowDestinationSubdomains);
}

void WKBundleResetOriginAccessAllowLists(WKBundleRef bundleRef)
{
    CyberKit::toImpl(bundleRef)->resetOriginAccessAllowLists();
}

void WKBundleSetAsynchronousSpellCheckingEnabledForTesting(WKBundleRef bundleRef, bool enabled)
{
    CyberKit::toImpl(bundleRef)->setAsynchronousSpellCheckingEnabled(enabled);
}

WKArrayRef WKBundleGetLiveDocumentURLsForTesting(WKBundleRef bundleRef, bool excludeDocumentsInPageGroupPages)
{
    auto liveDocuments = CyberKit::toImpl(bundleRef)->liveDocumentURLs(excludeDocumentsInPageGroupPages);

    auto liveURLs = adoptWK(WKMutableArrayCreate());

    for (const auto& it : liveDocuments) {
        auto urlInfo = adoptWK(WKMutableDictionaryCreate());

        auto documentIDKey = adoptWK(WKStringCreateWithUTF8CString("id"));
        auto documentURLKey = adoptWK(WKStringCreateWithUTF8CString("url"));

        auto documentIDValue = adoptWK(CyberKit::toCopiedAPI(it.key.toString()));
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

void WKBundleSetDatabaseQuota(WKBundleRef bundleRef, uint64_t quota)
{
    // Historically, we've used the following (somewhat nonsensical) string for the databaseIdentifier of local files.
    CyberCore::DatabaseTracker::singleton().setQuota(*CyberCore::SecurityOriginData::fromDatabaseIdentifier("file__0"_s), quota);
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

void WKBundleSetUserStyleSheetLocationForTesting(WKBundleRef bundleRef, WKStringRef location)
{
    CyberKit::toImpl(bundleRef)->setUserStyleSheetLocation(CyberKit::toWTFString(location));
}

void WKBundleSetWebNotificationPermission(WKBundleRef bundleRef, WKBundlePageRef pageRef, WKStringRef originStringRef, bool allowed)
{
    CyberKit::toImpl(bundleRef)->setWebNotificationPermission(CyberKit::toImpl(pageRef), CyberKit::toWTFString(originStringRef), allowed);
}

void WKBundleRemoveAllWebNotificationPermissions(WKBundleRef bundleRef, WKBundlePageRef pageRef)
{
    CyberKit::toImpl(bundleRef)->removeAllWebNotificationPermissions(CyberKit::toImpl(pageRef));
}

WKDataRef WKBundleCopyWebNotificationID(WKBundleRef bundleRef, JSContextRef context, JSValueRef notification)
{
    auto identifier = CyberKit::toImpl(bundleRef)->webNotificationID(context, notification);
    if (!identifier)
        return nullptr;

    auto span = identifier->toSpan();
    return WKDataCreate(span.data(), span.size());
}

void WKBundleSetTabKeyCyclesThroughElements(WKBundleRef bundleRef, WKBundlePageRef pageRef, bool enabled)
{
    CyberKit::toImpl(bundleRef)->setTabKeyCyclesThroughElements(CyberKit::toImpl(pageRef), enabled);
}

void WKBundleClearResourceLoadStatistics(WKBundleRef)
{
    CyberCore::ResourceLoadObserver::shared().clearState();
}

bool WKBundleResourceLoadStatisticsNotifyObserver(WKBundleRef)
{
    if (!CyberCore::ResourceLoadObserver::shared().hasStatistics())
        return false;

    CyberCore::ResourceLoadObserver::shared().updateCentralStatisticsStore([] { });
    return true;
}


void WKBundleExtendClassesForParameterCoder(WKBundleRef bundle, WKArrayRef classes)
{
#if PLATFORM(COCOA)
    auto classList = CyberKit::toImpl(classes);
    if (!classList)
        return;

    CyberKit::toImpl(bundle)->extendClassesForParameterCoder(*classList);
#endif
}
