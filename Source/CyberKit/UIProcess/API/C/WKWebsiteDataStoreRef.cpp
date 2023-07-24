/*
 * Copyright (C) 2015-2019 Apple Inc. All rights reserved.
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
#include "WKWebsiteDataStoreRef.h"

#include "APIArray.h"
#include "APIHTTPCookieStore.h"
#include "ShouldGrandfatherStatistics.h"
#include "WKAPICast.h"
#include "WKDictionary.h"
#include "WKMutableArray.h"
#include "WKNumber.h"
#include "WKRetainPtr.h"
#include "WKSecurityOriginRef.h"
#include "WKString.h"
#include "WebDeviceOrientationAndMotionAccessController.h"
#include "WebPageProxy.h"
#include "WebResourceLoadStatisticsStore.h"
#include "WebsiteData.h"
#include "WebsiteDataFetchOption.h"
#include "WebsiteDataRecord.h"
#include "WebsiteDataStore.h"
#include "WebsiteDataType.h"
#include <CyberCore/RegistrableDomain.h>
#include <wtf/CallbackAggregator.h>
#include <wtf/URL.h>

WKTypeID WKWebsiteDataStoreGetTypeID()
{
    return CyberKit::toAPI(CyberKit::WebsiteDataStore::APIType);
}

WKWebsiteDataStoreRef WKWebsiteDataStoreGetDefaultDataStore()
{
    return CyberKit::toAPI(CyberKit::WebsiteDataStore::defaultDataStore().ptr());
}

WKWebsiteDataStoreRef WKWebsiteDataStoreCreateNonPersistentDataStore()
{
    return CyberKit::toAPI(&CyberKit::WebsiteDataStore::createNonPersistent().leakRef());
}

WKWebsiteDataStoreRef WKWebsiteDataStoreCreateWithConfiguration(WKWebsiteDataStoreConfigurationRef configuration)
{
    auto sessionID = CyberKit::toImpl(configuration)->isPersistent() ? PAL::SessionID::generatePersistentSessionID() : PAL::SessionID::generateEphemeralSessionID();
    return CyberKit::toAPI(&CyberKit::WebsiteDataStore::create(*CyberKit::toImpl(configuration), sessionID).leakRef());
}

void WKWebsiteDataStoreRemoveITPDataForDomain(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, void* context, WKWebsiteDataStoreRemoveITPDataForDomainFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::WebsiteDataRecord dataRecord;
    dataRecord.types.add(CyberKit::WebsiteDataType::ResourceLoadStatistics);
    dataRecord.addResourceLoadStatisticsRegistrableDomain(CyberCore::RegistrableDomain::uncheckedCreateFromHost(CyberKit::toImpl(host)->string()));
    Vector<CyberKit::WebsiteDataRecord> dataRecords = { WTFMove(dataRecord) };

    OptionSet<CyberKit::WebsiteDataType> dataTypes = CyberKit::WebsiteDataType::ResourceLoadStatistics;
    CyberKit::toImpl(dataStoreRef)->removeData(dataTypes, dataRecords, [context, callback] {
        callback(context);
    });
#else
    callback(context);
#endif
}

void WKWebsiteDataStoreDoesStatisticsDomainIDExistInDatabase(WKWebsiteDataStoreRef dataStoreRef, int domainID, void* context, WKWebsiteDataStoreDoesStatisticsDomainIDExistInDatabaseFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->domainIDExistsInDatabase(domainID, [context, callback](bool exists) {
        callback(exists, context);
    });
#else
    callback(false, context);
#endif
}

void WKWebsiteDataStoreSetResourceLoadStatisticsEnabled(WKWebsiteDataStoreRef dataStoreRef, bool enable)
{
    auto* websiteDataStore = CyberKit::toImpl(dataStoreRef);
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    websiteDataStore->useExplicitITPState();
#endif
    websiteDataStore->setResourceLoadStatisticsEnabled(enable);
}

void WKWebsiteDataStoreIsStatisticsEphemeral(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreStatisticsEphemeralFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->isResourceLoadStatisticsEphemeral([context, completionHandler](bool isEphemeral) {
        completionHandler(isEphemeral, context);
    });
#else
    completionHandler(false, context);
#endif
}

bool WKWebsiteDataStoreGetResourceLoadStatisticsEnabled(WKWebsiteDataStoreRef dataStoreRef)
{
    return CyberKit::toImpl(dataStoreRef)->resourceLoadStatisticsEnabled();
}

void WKWebsiteDataStoreSetResourceLoadStatisticsDebugMode(WKWebsiteDataStoreRef dataStoreRef, bool enable)
{
    CyberKit::toImpl(dataStoreRef)->setResourceLoadStatisticsDebugMode(enable);
}

WKHTTPCookieStoreRef WKWebsiteDataStoreGetHTTPCookieStore(WKWebsiteDataStoreRef dataStoreRef)
{
    return CyberKit::toAPI(&CyberKit::toImpl(dataStoreRef)->cookieStore());
}

void WKWebsiteDataStoreSetResourceLoadStatisticsDebugModeWithCompletionHandler(WKWebsiteDataStoreRef dataStoreRef, bool enable, void* context, WKWebsiteDataStoreStatisticsDebugModeFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setResourceLoadStatisticsDebugMode(enable, [context, completionHandler] {
        completionHandler(context);
    });
#else
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreSetResourceLoadStatisticsPrevalentResourceForDebugMode(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, void* context, WKWebsiteDataStoreStatisticsDebugModeFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setPrevalentResourceForDebugMode(URL(URL(), CyberKit::toImpl(host)->string()), [context, completionHandler] {
        completionHandler(context);
    });
#else
    completionHandler(context);
#endif
}
void WKWebsiteDataStoreSetStatisticsLastSeen(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, double seconds, void* context, WKWebsiteDataStoreStatisticsLastSeenFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setLastSeen(URL(URL(), CyberKit::toImpl(host)->string()), Seconds { seconds }, [context, completionHandler] {
        completionHandler(context);
    });
#else
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreSetStatisticsMergeStatistic(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, WKStringRef topFrameDomain1, WKStringRef topFrameDomain2, double lastSeen, bool hadUserInteraction, double mostRecentUserInteraction, bool isGrandfathered, bool isPrevalent, bool isVeryPrevalent, unsigned dataRecordsRemoved, void* context, WKWebsiteDataStoreStatisticsMergeStatisticFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->mergeStatisticForTesting(URL(URL(), CyberKit::toImpl(host)->string()), URL(URL(), CyberKit::toImpl(topFrameDomain1)->string()), URL(URL(), CyberKit::toImpl(topFrameDomain2)->string()), Seconds { lastSeen }, hadUserInteraction, Seconds { mostRecentUserInteraction }, isGrandfathered, isPrevalent, isVeryPrevalent, dataRecordsRemoved, [context, completionHandler] {
        completionHandler(context);
    });
#else
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreSetStatisticsExpiredStatistic(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, bool hadUserInteraction, bool isScheduledForAllButCookieDataRemoval, bool isPrevalent, void* context, WKWebsiteDataStoreStatisticsMergeStatisticFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->insertExpiredStatisticForTesting(URL(URL(), CyberKit::toImpl(host)->string()), hadUserInteraction, isScheduledForAllButCookieDataRemoval, isPrevalent, [context, completionHandler] {
        completionHandler(context);
    });
#else
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreSetStatisticsPrevalentResource(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, bool value, void* context, WKWebsiteDataStoreStatisticsPrevalentResourceFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    auto& websiteDataStore = *CyberKit::toImpl(dataStoreRef);

    if (value)
        websiteDataStore.setPrevalentResource(URL(URL(), CyberKit::toImpl(host)->string()), [context, completionHandler] {
            completionHandler(context);
        });
    else
        websiteDataStore.clearPrevalentResource(URL(URL(), CyberKit::toImpl(host)->string()), [context, completionHandler] {
            completionHandler(context);
        });
#else
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreSetStatisticsVeryPrevalentResource(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, bool value, void* context, WKWebsiteDataStoreStatisticsVeryPrevalentResourceFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    auto& websiteDataStore = *CyberKit::toImpl(dataStoreRef);

    if (value)
        websiteDataStore.setVeryPrevalentResource(URL(URL(), CyberKit::toImpl(host)->string()), [context, completionHandler] {
            completionHandler(context);
        });
    else
        websiteDataStore.clearPrevalentResource(URL(URL(), CyberKit::toImpl(host)->string()), [context, completionHandler] {
            completionHandler(context);
        });
#else
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreDumpResourceLoadStatistics(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreDumpResourceLoadStatisticsFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->dumpResourceLoadStatistics([context, callback] (const String& resourceLoadStatistics) {
        callback(CyberKit::toAPI(resourceLoadStatistics.impl()), context);
    });
#else
    callback(CyberKit::toAPI(emptyString().impl()), context);
#endif
}

void WKWebsiteDataStoreIsStatisticsPrevalentResource(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, void* context, WKWebsiteDataStoreIsStatisticsPrevalentResourceFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->isPrevalentResource(URL(URL(), CyberKit::toImpl(host)->string()), [context, callback](bool isPrevalentResource) {
        callback(isPrevalentResource, context);
    });
#else
    callback(false, context);
#endif
}

void WKWebsiteDataStoreIsStatisticsVeryPrevalentResource(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, void* context, WKWebsiteDataStoreIsStatisticsPrevalentResourceFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->isVeryPrevalentResource(URL(URL(), CyberKit::toImpl(host)->string()), [context, callback](bool isVeryPrevalentResource) {
        callback(isVeryPrevalentResource, context);
    });
#else
    callback(false, context);
#endif
}

void WKWebsiteDataStoreIsStatisticsRegisteredAsSubresourceUnder(WKWebsiteDataStoreRef dataStoreRef, WKStringRef subresourceHost, WKStringRef topFrameHost, void* context, WKWebsiteDataStoreIsStatisticsRegisteredAsSubresourceUnderFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->isRegisteredAsSubresourceUnder(URL(URL(), CyberKit::toImpl(subresourceHost)->string()), URL(URL(), CyberKit::toImpl(topFrameHost)->string()), [context, callback](bool isRegisteredAsSubresourceUnder) {
        callback(isRegisteredAsSubresourceUnder, context);
    });
#else
    callback(false, context);
#endif
}

void WKWebsiteDataStoreIsStatisticsRegisteredAsSubFrameUnder(WKWebsiteDataStoreRef dataStoreRef, WKStringRef subFrameHost, WKStringRef topFrameHost, void* context, WKWebsiteDataStoreIsStatisticsRegisteredAsSubFrameUnderFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->isRegisteredAsSubFrameUnder(URL(URL(), CyberKit::toImpl(subFrameHost)->string()), URL(URL(), CyberKit::toImpl(topFrameHost)->string()), [context, callback](bool isRegisteredAsSubFrameUnder) {
        callback(isRegisteredAsSubFrameUnder, context);
    });
#else
    callback(false, context);
#endif
}

void WKWebsiteDataStoreIsStatisticsRegisteredAsRedirectingTo(WKWebsiteDataStoreRef dataStoreRef, WKStringRef hostRedirectedFrom, WKStringRef hostRedirectedTo, void* context, WKWebsiteDataStoreIsStatisticsRegisteredAsRedirectingToFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->isRegisteredAsRedirectingTo(URL(URL(), CyberKit::toImpl(hostRedirectedFrom)->string()), URL(URL(), CyberKit::toImpl(hostRedirectedTo)->string()), [context, callback](bool isRegisteredAsRedirectingTo) {
        callback(isRegisteredAsRedirectingTo, context);
    });
#else
    callback(false, context);
#endif
}

void WKWebsiteDataStoreSetStatisticsHasHadUserInteraction(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, bool value, void* context, WKWebsiteDataStoreStatisticsHasHadUserInteractionFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    auto& dataStore = *CyberKit::toImpl(dataStoreRef);

    if (value)
        dataStore.logUserInteraction(URL(URL(), CyberKit::toImpl(host)->string()), [context, completionHandler] {
            completionHandler(context);
        });
    else
        dataStore.clearUserInteraction(URL(URL(), CyberKit::toImpl(host)->string()), [context, completionHandler] {
            completionHandler(context);
        });
#else
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreIsStatisticsHasHadUserInteraction(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, void* context, WKWebsiteDataStoreIsStatisticsHasHadUserInteractionFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->hasHadUserInteraction(URL(URL(), CyberKit::toImpl(host)->string()), [context, callback](bool hasHadUserInteraction) {
        callback(hasHadUserInteraction, context);
    });
#else
    callback(false, context);
#endif
}

void WKWebsiteDataStoreIsStatisticsOnlyInDatabaseOnce(WKWebsiteDataStoreRef dataStoreRef, WKStringRef subHost, WKStringRef topHost, void* context, WKWebsiteDataStoreIsStatisticsOnlyInDatabaseOnceFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->isRelationshipOnlyInDatabaseOnce(URL(URL(), CyberKit::toImpl(subHost)->string()), URL(URL(), CyberKit::toImpl(topHost)->string()), [context, callback](bool onlyInDatabaseOnce) {
        callback(onlyInDatabaseOnce, context);
    });
#else
    callback(false, context);
#endif
}

void WKWebsiteDataStoreSetStatisticsGrandfathered(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, bool value)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setGrandfathered(URL(URL(), CyberKit::toImpl(host)->string()), value, [] { });
#endif
}

void WKWebsiteDataStoreIsStatisticsGrandfathered(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, void* context, WKWebsiteDataStoreIsStatisticsGrandfatheredFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->isGrandfathered(URL(URL(), CyberKit::toImpl(host)->string()), [context, callback](bool isGrandfathered) {
        callback(isGrandfathered, context);
    });
#else
    callback(false, context);
#endif
}

void WKWebsiteDataStoreSetUseITPDatabase(WKWebsiteDataStoreRef dataStoreRef, bool value, void* context, WKWebsiteDataStoreSetUseITPDatabaseFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setUseITPDatabase(value, [context, completionHandler] {
        completionHandler(context);
    });
#else
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreSetStatisticsSubframeUnderTopFrameOrigin(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, WKStringRef topFrameHost)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setSubframeUnderTopFrameDomain(URL(URL(), CyberKit::toImpl(host)->string()), URL(URL(), CyberKit::toImpl(topFrameHost)->string()), [] { });
#endif
}

void WKWebsiteDataStoreSetStatisticsSubresourceUnderTopFrameOrigin(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, WKStringRef topFrameHost)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setSubresourceUnderTopFrameDomain(URL(URL(), CyberKit::toImpl(host)->string()), URL(URL(), CyberKit::toImpl(topFrameHost)->string()), [] { });
#endif
}

void WKWebsiteDataStoreSetStatisticsSubresourceUniqueRedirectTo(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, WKStringRef hostRedirectedTo)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setSubresourceUniqueRedirectTo(URL(URL(), CyberKit::toImpl(host)->string()), URL(URL(), CyberKit::toImpl(hostRedirectedTo)->string()), [] { });
#endif
}

void WKWebsiteDataStoreSetStatisticsSubresourceUniqueRedirectFrom(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, WKStringRef hostRedirectedFrom)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setSubresourceUniqueRedirectFrom(URL(URL(), CyberKit::toImpl(host)->string()), URL(URL(), CyberKit::toImpl(hostRedirectedFrom)->string()), [] { });
#endif
}

void WKWebsiteDataStoreSetStatisticsTopFrameUniqueRedirectTo(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, WKStringRef hostRedirectedTo)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setTopFrameUniqueRedirectTo(URL(URL(), CyberKit::toImpl(host)->string()), URL(URL(), CyberKit::toImpl(hostRedirectedTo)->string()), [] { });
#endif
}

void WKWebsiteDataStoreSetStatisticsTopFrameUniqueRedirectFrom(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, WKStringRef hostRedirectedFrom)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setTopFrameUniqueRedirectFrom(URL(URL(), CyberKit::toImpl(host)->string()), URL(URL(), CyberKit::toImpl(hostRedirectedFrom)->string()), [] { });
#endif
}

void WKWebsiteDataStoreSetStatisticsCrossSiteLoadWithLinkDecoration(WKWebsiteDataStoreRef dataStoreRef, WKStringRef fromHost, WKStringRef toHost, void* context, WKWebsiteDataStoreSetStatisticsCrossSiteLoadWithLinkDecorationFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setCrossSiteLoadWithLinkDecorationForTesting(URL(URL(), CyberKit::toImpl(fromHost)->string()), URL(URL(), CyberKit::toImpl(toHost)->string()), [context, callback] {
        callback(context);
    });
#else
    callback(context);
#endif
}

void WKWebsiteDataStoreSetStatisticsTimeToLiveUserInteraction(WKWebsiteDataStoreRef dataStoreRef, double seconds, void* context, WKWebsiteDataStoreSetStatisticsTimeToLiveUserInteractionFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setTimeToLiveUserInteraction(Seconds { seconds }, [context, callback] {
        callback(context);
    });
#else
    callback(context);
#endif
}

void WKWebsiteDataStoreStatisticsProcessStatisticsAndDataRecords(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreStatisticsProcessStatisticsAndDataRecordsFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->scheduleStatisticsAndDataRecordsProcessing([context, callback] {
        callback(context);
    });
#else
    callback(context);
#endif
}

void WKWebsiteDataStoreStatisticsUpdateCookieBlocking(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreStatisticsUpdateCookieBlockingFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->scheduleCookieBlockingUpdate([context, completionHandler]() {
        completionHandler(context);
    });
#else
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreStatisticsSubmitTelemetry(WKWebsiteDataStoreRef dataStoreRef)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->submitTelemetry();
#endif
}

void WKWebsiteDataStoreSetStatisticsNotifyPagesWhenDataRecordsWereScanned(WKWebsiteDataStoreRef dataStoreRef, bool value)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setNotifyPagesWhenDataRecordsWereScanned(value, [] { });
#endif
}

void WKWebsiteDataStoreSetStatisticsIsRunningTest(WKWebsiteDataStoreRef dataStoreRef, bool value, void* context, WKWebsiteDataStoreSetStatisticsIsRunningTestFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setIsRunningResourceLoadStatisticsTest(value, [context, callback] {
        callback(context);
    });
#else
    callback(context);
#endif
}

void WKWebsiteDataStoreSetStatisticsShouldClassifyResourcesBeforeDataRecordsRemoval(WKWebsiteDataStoreRef dataStoreRef, bool value)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setShouldClassifyResourcesBeforeDataRecordsRemoval(value, []() { });
#endif
}

void WKWebsiteDataStoreSetStatisticsMinimumTimeBetweenDataRecordsRemoval(WKWebsiteDataStoreRef dataStoreRef, double seconds)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setMinimumTimeBetweenDataRecordsRemoval(Seconds { seconds }, []() { });
#endif
}

void WKWebsiteDataStoreSetStatisticsGrandfatheringTime(WKWebsiteDataStoreRef dataStoreRef, double seconds)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setGrandfatheringTime(Seconds { seconds }, []() { });
#endif
}

void WKWebsiteDataStoreSetStatisticsMaxStatisticsEntries(WKWebsiteDataStoreRef dataStoreRef, unsigned entries)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setMaxStatisticsEntries(entries, []() { });
#endif
}

void WKWebsiteDataStoreSetStatisticsPruneEntriesDownTo(WKWebsiteDataStoreRef dataStoreRef, unsigned entries)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setPruneEntriesDownTo(entries, []() { });
#endif
}

void WKWebsiteDataStoreStatisticsClearInMemoryAndPersistentStore(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreStatisticsClearInMemoryAndPersistentStoreFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->scheduleClearInMemoryAndPersistent(CyberKit::ShouldGrandfatherStatistics::Yes, [context, callback]() {
        callback(context);
    });
#else
    callback(context);
#endif
}

void WKWebsiteDataStoreStatisticsClearInMemoryAndPersistentStoreModifiedSinceHours(WKWebsiteDataStoreRef dataStoreRef, unsigned hours, void* context, WKWebsiteDataStoreStatisticsClearInMemoryAndPersistentStoreModifiedSinceHoursFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->scheduleClearInMemoryAndPersistent(WallTime::now() - Seconds::fromHours(hours), CyberKit::ShouldGrandfatherStatistics::Yes, [context, callback]() {
        callback(context);
    });
#else
    callback(context);
#endif
}

void WKWebsiteDataStoreStatisticsClearThroughWebsiteDataRemoval(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreStatisticsClearThroughWebsiteDataRemovalFunction callback)
{
    OptionSet<CyberKit::WebsiteDataType> dataTypes = CyberKit::WebsiteDataType::ResourceLoadStatistics;
    CyberKit::toImpl(dataStoreRef)->removeData(dataTypes, WallTime::fromRawSeconds(0), [context, callback] {
        callback(context);
    });
}

void WKWebsiteDataStoreStatisticsDeleteCookiesForTesting(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, bool includeHttpOnlyCookies, void* context, WKWebsiteDataStoreStatisticsDeleteCookiesForTestingFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->deleteCookiesForTesting(URL(URL(), CyberKit::toImpl(host)->string()), includeHttpOnlyCookies, [context, callback] {
        callback(context);
    });
#else
    callback(context);
#endif
}

void WKWebsiteDataStoreStatisticsHasLocalStorage(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, void* context, WKWebsiteDataStoreStatisticsHasLocalStorageFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->hasLocalStorageForTesting(URL(URL(), CyberKit::toImpl(host)->string()), [context, callback](bool hasLocalStorage) {
        callback(hasLocalStorage, context);
    });
#else
    callback(false, context);
#endif
}

void WKWebsiteDataStoreSetStatisticsCacheMaxAgeCap(WKWebsiteDataStoreRef dataStoreRef, double seconds, void* context, WKWebsiteDataStoreSetStatisticsCacheMaxAgeCapFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setCacheMaxAgeCapForPrevalentResources(Seconds { seconds }, [context, callback] {
        callback(context);
    });
#else
    callback(context);
#endif
}

void WKWebsiteDataStoreStatisticsHasIsolatedSession(WKWebsiteDataStoreRef dataStoreRef, WKStringRef host, void* context, WKWebsiteDataStoreStatisticsHasIsolatedSessionFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->hasIsolatedSessionForTesting(URL(URL(), CyberKit::toImpl(host)->string()), [context, callback](bool hasIsolatedSession) {
        callback(hasIsolatedSession, context);
    });
#else
    callback(false, context);
#endif
}

void WKWebsiteDataStoreHasAppBoundSession(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreHasAppBoundSessionFunction callback)
{
    CyberKit::toImpl(dataStoreRef)->hasAppBoundSession([context, callback](bool hasAppBoundSession) {
        callback(hasAppBoundSession, context);
    });
}

void WKWebsiteDataStoreSetResourceLoadStatisticsShouldDowngradeReferrerForTesting(WKWebsiteDataStoreRef dataStoreRef, bool enabled, void* context, WKWebsiteDataStoreSetResourceLoadStatisticsShouldDowngradeReferrerForTestingFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setResourceLoadStatisticsShouldDowngradeReferrerForTesting(enabled, [context, completionHandler] {
        completionHandler(context);
    });
#else
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreSetResourceLoadStatisticsShouldBlockThirdPartyCookiesForTesting(WKWebsiteDataStoreRef dataStoreRef, bool enabled, bool onlyOnSitesWithoutUserInteraction, void* context, WKWebsiteDataStoreSetResourceLoadStatisticsShouldBlockThirdPartyCookiesForTestingFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setResourceLoadStatisticsShouldBlockThirdPartyCookiesForTesting(enabled, onlyOnSitesWithoutUserInteraction, [context, completionHandler] {
        completionHandler(context);
    });
#else
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreSetResourceLoadStatisticsFirstPartyWebsiteDataRemovalModeForTesting(WKWebsiteDataStoreRef dataStoreRef, bool enabled, void* context, WKWebsiteDataStoreSetResourceLoadStatisticsFirstPartyWebsiteDataRemovalModeForTestingFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setResourceLoadStatisticsFirstPartyWebsiteDataRemovalModeForTesting(enabled, [context, completionHandler] {
        completionHandler(context);
    });
#else
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreSetResourceLoadStatisticsToSameSiteStrictCookiesForTesting(WKWebsiteDataStoreRef dataStoreRef, WKStringRef hostName, void* context, WKWebsiteDataStoreSetResourceLoadStatisticsToSameSiteStrictCookiesForTestingFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setResourceLoadStatisticsToSameSiteStrictCookiesForTesting(URL(URL(), CyberKit::toImpl(hostName)->string()), [context, completionHandler] {
        completionHandler(context);
    });
#else
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreSetResourceLoadStatisticsFirstPartyHostCNAMEDomainForTesting(WKWebsiteDataStoreRef dataStoreRef, WKStringRef firstPartyURLString, WKStringRef cnameURLString, void* context, WKWebsiteDataStoreSetResourceLoadStatisticsFirstPartyHostCNAMEDomainForTestingFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setResourceLoadStatisticsFirstPartyHostCNAMEDomainForTesting(URL(URL(), CyberKit::toImpl(firstPartyURLString)->string()), URL(URL(), CyberKit::toImpl(cnameURLString)->string()), [context, completionHandler] {
        completionHandler(context);
    });
#else
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreSetResourceLoadStatisticsThirdPartyCNAMEDomainForTesting(WKWebsiteDataStoreRef dataStoreRef, WKStringRef cnameURLString, void* context, WKWebsiteDataStoreSetResourceLoadStatisticsThirdPartyCNAMEDomainForTestingFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->setResourceLoadStatisticsThirdPartyCNAMEDomainForTesting(URL(URL(), CyberKit::toImpl(cnameURLString)->string()), [context, completionHandler] {
        completionHandler(context);
    });
#else
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreSetAppBoundDomainsForTesting(WKArrayRef originURLsRef, void* context, WKWebsiteDataStoreSetAppBoundDomainsForTestingFunction completionHandler)
{
#if PLATFORM(COCOA)
    RefPtr<API::Array> originURLsArray = toImpl(originURLsRef);
    size_t newSize = originURLsArray ? originURLsArray->size() : 0;
    HashSet<CyberCore::RegistrableDomain> domains;
    domains.reserveInitialCapacity(newSize);
    for (size_t i = 0; i < newSize; ++i) {
        auto* originURL = originURLsArray->at<API::URL>(i);
        if (!originURL)
            continue;
        
        domains.add(CyberCore::RegistrableDomain { URL(URL(), originURL->string()) });
    }

    CyberKit::WebsiteDataStore::setAppBoundDomainsForTesting(WTFMove(domains), [context, completionHandler] {
        completionHandler(context);
    });
#else
    UNUSED_PARAM(originURLsRef);
    UNUSED_PARAM(context);
    UNUSED_PARAM(completionHandler);
#endif
}

void WKWebsiteDataStoreStatisticsResetToConsistentState(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreStatisticsResetToConsistentStateFunction completionHandler)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    auto callbackAggregator = CallbackAggregator::create([context, completionHandler]() {
        completionHandler(context);
    });

    auto& store = *CyberKit::toImpl(dataStoreRef);
    store.clearResourceLoadStatisticsInWebProcesses([callbackAggregator] { });
    store.resetCacheMaxAgeCapForPrevalentResources([callbackAggregator] { });
    store.resetCrossSiteLoadsWithLinkDecorationForTesting([callbackAggregator] { });
    store.setResourceLoadStatisticsShouldDowngradeReferrerForTesting(true, [callbackAggregator] { });
    store.setResourceLoadStatisticsShouldBlockThirdPartyCookiesForTesting(false, false, [callbackAggregator] { });
    store.setResourceLoadStatisticsShouldEnbleSameSiteStrictEnforcementForTesting(true, [callbackAggregator] { });
    store.setResourceLoadStatisticsFirstPartyWebsiteDataRemovalModeForTesting(false, [callbackAggregator] { });
    store.resetParametersToDefaultValues([callbackAggregator] { });
    store.scheduleClearInMemoryAndPersistent(CyberKit::ShouldGrandfatherStatistics::No, [callbackAggregator] { });
    store.setUseITPDatabase(false, [callbackAggregator] { });
#else
    UNUSED_PARAM(dataStoreRef);
    completionHandler(context);
#endif
}

void WKWebsiteDataStoreRemoveAllFetchCaches(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreRemoveFetchCacheRemovalFunction callback)
{
    OptionSet<CyberKit::WebsiteDataType> dataTypes = CyberKit::WebsiteDataType::DOMCache;
    CyberKit::toImpl(dataStoreRef)->removeData(dataTypes, -WallTime::infinity(), [context, callback] {
        callback(context);
    });
}

void WKWebsiteDataStoreRemoveFetchCacheForOrigin(WKWebsiteDataStoreRef dataStoreRef, WKSecurityOriginRef origin, void* context, WKWebsiteDataStoreRemoveFetchCacheRemovalFunction callback)
{
    CyberKit::WebsiteDataRecord dataRecord;
    dataRecord.add(CyberKit::WebsiteDataType::DOMCache, CyberKit::toImpl(origin)->securityOrigin().data());
    Vector<CyberKit::WebsiteDataRecord> dataRecords = { WTFMove(dataRecord) };

    OptionSet<CyberKit::WebsiteDataType> dataTypes = CyberKit::WebsiteDataType::DOMCache;
    CyberKit::toImpl(dataStoreRef)->removeData(dataTypes, dataRecords, [context, callback] {
        callback(context);
    });
}

void WKWebsiteDataStoreRemoveAllIndexedDatabases(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreRemoveAllIndexedDatabasesCallback callback)
{
    OptionSet<CyberKit::WebsiteDataType> dataTypes = CyberKit::WebsiteDataType::IndexedDBDatabases;
    CyberKit::toImpl(dataStoreRef)->removeData(dataTypes, -WallTime::infinity(), [context, callback] {
    if (callback)
        callback(context);
    });
}

void WKWebsiteDataStoreRemoveLocalStorage(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreRemoveLocalStorageCallback callback)
{
    OptionSet<CyberKit::WebsiteDataType> dataTypes = CyberKit::WebsiteDataType::LocalStorage;
    CyberKit::toImpl(dataStoreRef)->removeData(dataTypes, -WallTime::infinity(), [context, callback] {
        if (callback)
            callback(context);
    });
}

void WKWebsiteDataStoreRemoveAllServiceWorkerRegistrations(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreRemoveAllServiceWorkerRegistrationsCallback callback)
{
#if ENABLE(SERVICE_WORKER)
    OptionSet<CyberKit::WebsiteDataType> dataTypes = CyberKit::WebsiteDataType::ServiceWorkerRegistrations;
    CyberKit::toImpl(dataStoreRef)->removeData(dataTypes, -WallTime::infinity(), [context, callback] {
        callback(context);
    });
#else
    UNUSED_PARAM(dataStoreRef);
    callback(context);
#endif
}

void WKWebsiteDataStoreGetFetchCacheOrigins(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreGetFetchCacheOriginsFunction callback)
{
    CyberKit::toImpl(dataStoreRef)->fetchData(CyberKit::WebsiteDataType::DOMCache, { }, [context, callback] (auto dataRecords) {
        Vector<RefPtr<API::Object>> securityOrigins;
        for (const auto& dataRecord : dataRecords) {
            for (const auto& origin : dataRecord.origins)
                securityOrigins.append(API::SecurityOrigin::create(origin.securityOrigin()));
        }
        callback(CyberKit::toAPI(API::Array::create(WTFMove(securityOrigins)).ptr()), context);
    });
}

void WKWebsiteDataStoreGetFetchCacheSizeForOrigin(WKWebsiteDataStoreRef dataStoreRef, WKStringRef origin, void* context, WKWebsiteDataStoreGetFetchCacheSizeForOriginFunction callback)
{
    OptionSet<CyberKit::WebsiteDataFetchOption> fetchOptions = CyberKit::WebsiteDataFetchOption::ComputeSizes;

    CyberKit::toImpl(dataStoreRef)->fetchData(CyberKit::WebsiteDataType::DOMCache, fetchOptions, [origin, context, callback] (auto dataRecords) {
        auto originData = CyberCore::SecurityOrigin::createFromString(CyberKit::toImpl(origin)->string())->data();
        for (auto& dataRecord : dataRecords) {
            for (const auto& recordOrigin : dataRecord.origins) {
                if (originData == recordOrigin) {
                    callback(dataRecord.size ? dataRecord.size->totalSize : 0, context);
                    return;
                }

            }
        }
        callback(0, context);
    });
}

void WKWebsiteDataStoreSetPerOriginStorageQuota(WKWebsiteDataStoreRef, uint64_t)
{
}

void WKWebsiteDataStoreClearAllDeviceOrientationPermissions(WKWebsiteDataStoreRef dataStoreRef)
{
#if ENABLE(DEVICE_ORIENTATION)
    CyberKit::toImpl(dataStoreRef)->deviceOrientationAndMotionAccessController().clearPermissions();
#endif
}

void WKWebsiteDataStoreClearAdClickAttributionsThroughWebsiteDataRemoval(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreClearAdClickAttributionsThroughWebsiteDataRemovalFunction callback)
{
    OptionSet<CyberKit::WebsiteDataType> dataTypes = CyberKit::WebsiteDataType::AdClickAttributions;
    CyberKit::toImpl(dataStoreRef)->removeData(dataTypes, WallTime::fromRawSeconds(0), [context, callback] {
        callback(context);
    });
}

void WKWebsiteDataStoreSetCacheModelSynchronouslyForTesting(WKWebsiteDataStoreRef dataStoreRef, WKCacheModel cacheModel)
{
    CyberKit::toImpl(dataStoreRef)->setCacheModelSynchronouslyForTesting(CyberKit::toCacheModel(cacheModel));
}

void WKWebsiteDataStoreResetQuota(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreResetQuotaCallback callback)
{
    CyberKit::toImpl(dataStoreRef)->resetQuota([context, callback] {
        if (callback)
            callback(context);
    });
}

void WKWebsiteDataStoreClearAppBoundSession(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreClearAppBoundSessionFunction completionHandler)
{
    CyberKit::toImpl(dataStoreRef)->clearAppBoundSession([context, completionHandler] {
        completionHandler(context);
    });
}

void WKWebsiteDataStoreReinitializeAppBoundDomains(WKWebsiteDataStoreRef dataStoreRef)
{
#if PLATFORM(COCOA)
    CyberKit::toImpl(dataStoreRef)->reinitializeAppBoundDomains();
#endif
}

void WKWebsiteDataStoreUpdateBundleIdentifierInNetworkProcess(WKWebsiteDataStoreRef dataStoreRef, const WKStringRef bundleIdentifier, void* context, WKWebsiteDataStoreUpdateBundleIdentifierInNetworkProcessFunction completionHandler)
{
    CyberKit::toImpl(dataStoreRef)->updateBundleIdentifierInNetworkProcess(CyberKit::toImpl(bundleIdentifier)->string(), [context, completionHandler] {
        completionHandler(context);
    });
}

void WKWebsiteDataStoreClearBundleIdentifierInNetworkProcess(WKWebsiteDataStoreRef dataStoreRef, void* context, WKWebsiteDataStoreClearBundleIdentifierInNetworkProcessFunction completionHandler)
{
    CyberKit::toImpl(dataStoreRef)->clearBundleIdentifierInNetworkProcess([context, completionHandler] {
        completionHandler(context);
    });
}

void WKWebsiteDataStoreGetAllStorageAccessEntries(WKWebsiteDataStoreRef dataStoreRef, WKPageRef pageRef, void* context, WKWebsiteDataStoreGetAllStorageAccessEntriesFunction callback)
{
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    CyberKit::toImpl(dataStoreRef)->getAllStorageAccessEntries(CyberKit::toImpl(pageRef)->identifier(), [context, callback] (Vector<String>&& domains) {
        auto domainArrayRef = WKMutableArrayCreate();
        for (auto domain : domains)
            WKArrayAppendItem(domainArrayRef, adoptWK(WKStringCreateWithUTF8CString(domain.utf8().data())).get());

        callback(context, domainArrayRef);
    });
#else
    callback(context, WKMutableArrayCreate());
#endif
}
