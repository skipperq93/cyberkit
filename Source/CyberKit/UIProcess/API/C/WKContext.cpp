/*
 * Copyright (C) 2010, 2011, 2012 Apple Inc. All rights reserved.
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
#include "WKContextPrivate.h"

#include "APIArray.h"
#include "APIClient.h"
#include "APIDownloadClient.h"
#include "APILegacyContextHistoryClient.h"
#include "APINavigationData.h"
#include "APIProcessPoolConfiguration.h"
#include "APIURLRequest.h"
#include "AuthenticationChallengeProxy.h"
#include "DownloadProxy.h"
#include "WKAPICast.h"
#include "WKArray.h"
#include "WKContextConfigurationRef.h"
#include "WKRetainPtr.h"
#include "WKString.h"
#include "WebCertificateInfo.h"
#include "WebContextInjectedBundleClient.h"
#include "WebProcessPool.h"
#include <wtf/RefPtr.h>
#include <wtf/text/WTFString.h>

// Supplements
#include "WebCookieManagerProxy.h"
#include "WebGeolocationManagerProxy.h"
#include "WebNotificationManagerProxy.h"

namespace API {
template<> struct ClientTraits<WKContextDownloadClientBase> {
    typedef std::tuple<WKContextDownloadClientV0, WKContextDownloadClientV1> Versions;
};
template<> struct ClientTraits<WKContextHistoryClientBase> {
    typedef std::tuple<WKContextHistoryClientV0> Versions;
};
}

WKTypeID WKContextGetTypeID()
{
    return CyberKit::toAPI(CyberKit::WebProcessPool::APIType);
}

WKContextRef WKContextCreate()
{
    auto configuration = API::ProcessPoolConfiguration::createWithLegacyOptions();
    return CyberKit::toAPI(&CyberKit::WebProcessPool::create(configuration).leakRef());
}

WKContextRef WKContextCreateWithInjectedBundlePath(WKStringRef pathRef)
{
    auto configuration = API::ProcessPoolConfiguration::createWithLegacyOptions();
    configuration->setInjectedBundlePath(CyberKit::toWTFString(pathRef));

    return CyberKit::toAPI(&CyberKit::WebProcessPool::create(configuration).leakRef());
}

WKContextRef WKContextCreateWithConfiguration(WKContextConfigurationRef configuration)
{
    RefPtr<API::ProcessPoolConfiguration> apiConfiguration = CyberKit::toImpl(configuration);
    if (!apiConfiguration)
        apiConfiguration = API::ProcessPoolConfiguration::create();
    return CyberKit::toAPI(&CyberKit::WebProcessPool::create(*apiConfiguration).leakRef());
}

void WKContextSetClient(WKContextRef contextRef, const WKContextClientBase* wkClient)
{
    CyberKit::toImpl(contextRef)->initializeClient(wkClient);
}

void WKContextSetInjectedBundleClient(WKContextRef contextRef, const WKContextInjectedBundleClientBase* wkClient)
{
    CyberKit::toImpl(contextRef)->setInjectedBundleClient(std::make_unique<CyberKit::WebContextInjectedBundleClient>(wkClient));
}

void WKContextSetHistoryClient(WKContextRef contextRef, const WKContextHistoryClientBase* wkClient)
{
    class HistoryClient final : public API::Client<WKContextHistoryClientBase>, public API::LegacyContextHistoryClient {
    public:
        explicit HistoryClient(const WKContextHistoryClientBase* client)
        {
            initialize(client);
        }

    private:
        void didNavigateWithNavigationData(CyberKit::WebProcessPool& processPool, CyberKit::WebPageProxy& page, const CyberKit::WebNavigationDataStore& navigationDataStore, CyberKit::WebFrameProxy& frame) override
        {
            if (!m_client.didNavigateWithNavigationData)
                return;

            RefPtr<API::NavigationData> navigationData = API::NavigationData::create(navigationDataStore);
            m_client.didNavigateWithNavigationData(CyberKit::toAPI(&processPool), CyberKit::toAPI(&page), CyberKit::toAPI(navigationData.get()), CyberKit::toAPI(&frame), m_client.base.clientInfo);
        }

        void didPerformClientRedirect(CyberKit::WebProcessPool& processPool, CyberKit::WebPageProxy& page, const String& sourceURL, const String& destinationURL, CyberKit::WebFrameProxy& frame) override
        {
            if (!m_client.didPerformClientRedirect)
                return;

            m_client.didPerformClientRedirect(CyberKit::toAPI(&processPool), CyberKit::toAPI(&page), CyberKit::toURLRef(sourceURL.impl()), CyberKit::toURLRef(destinationURL.impl()), CyberKit::toAPI(&frame), m_client.base.clientInfo);
        }

        void didPerformServerRedirect(CyberKit::WebProcessPool& processPool, CyberKit::WebPageProxy& page, const String& sourceURL, const String& destinationURL, CyberKit::WebFrameProxy& frame) override
        {
            if (!m_client.didPerformServerRedirect)
                return;

            m_client.didPerformServerRedirect(CyberKit::toAPI(&processPool), CyberKit::toAPI(&page), CyberKit::toURLRef(sourceURL.impl()), CyberKit::toURLRef(destinationURL.impl()), CyberKit::toAPI(&frame), m_client.base.clientInfo);
        }

        void didUpdateHistoryTitle(CyberKit::WebProcessPool& processPool, CyberKit::WebPageProxy& page, const String& title, const String& url, CyberKit::WebFrameProxy& frame) override
        {
            if (!m_client.didUpdateHistoryTitle)
                return;

            m_client.didUpdateHistoryTitle(CyberKit::toAPI(&processPool), CyberKit::toAPI(&page), CyberKit::toAPI(title.impl()), CyberKit::toURLRef(url.impl()), CyberKit::toAPI(&frame), m_client.base.clientInfo);
        }

        void populateVisitedLinks(CyberKit::WebProcessPool& processPool) override
        {
            if (!m_client.populateVisitedLinks)
                return;

            m_client.populateVisitedLinks(CyberKit::toAPI(&processPool), m_client.base.clientInfo);
        }

        bool addsVisitedLinks() const override
        {
            return m_client.populateVisitedLinks;
        }
    };

    CyberKit::WebProcessPool& processPool = *CyberKit::toImpl(contextRef);
    processPool.setHistoryClient(std::make_unique<HistoryClient>(wkClient));

    bool addsVisitedLinks = processPool.historyClient().addsVisitedLinks();

    for (auto& process : processPool.processes()) {
        for (auto& page : process->pages())
            page->setAddsVisitedLinks(addsVisitedLinks);
    }
}

void WKContextSetDownloadClient(WKContextRef contextRef, const WKContextDownloadClientBase* wkClient)
{
    class DownloadClient final : public API::Client<WKContextDownloadClientBase>, public API::DownloadClient {
    public:
        explicit DownloadClient(const WKContextDownloadClientBase* client)
        {
            initialize(client);
        }
    private:
        void didStart(CyberKit::WebProcessPool& processPool, CyberKit::DownloadProxy& downloadProxy) final
        {
            if (!m_client.didStart)
                return;

            m_client.didStart(CyberKit::toAPI(&processPool), CyberKit::toAPI(&downloadProxy), m_client.base.clientInfo);
        }

        void didReceiveAuthenticationChallenge(CyberKit::WebProcessPool& processPool, CyberKit::DownloadProxy& downloadProxy, CyberKit::AuthenticationChallengeProxy& authenticationChallengeProxy) final
        {
            if (!m_client.didReceiveAuthenticationChallenge)
                return;

            m_client.didReceiveAuthenticationChallenge(CyberKit::toAPI(&processPool), CyberKit::toAPI(&downloadProxy), CyberKit::toAPI(&authenticationChallengeProxy), m_client.base.clientInfo);
        }

        void didReceiveResponse(CyberKit::WebProcessPool& processPool, CyberKit::DownloadProxy& downloadProxy, const CyberCore::ResourceResponse& response) final
        {
            if (!m_client.didReceiveResponse)
                return;

            m_client.didReceiveResponse(CyberKit::toAPI(&processPool), CyberKit::toAPI(&downloadProxy), CyberKit::toAPI(API::URLResponse::create(response).ptr()), m_client.base.clientInfo);
        }

        void didReceiveData(CyberKit::WebProcessPool& processPool, CyberKit::DownloadProxy& downloadProxy, uint64_t length) final
        {
            if (!m_client.didReceiveData)
                return;

            m_client.didReceiveData(CyberKit::toAPI(&processPool), CyberKit::toAPI(&downloadProxy), length, m_client.base.clientInfo);
        }

        void decideDestinationWithSuggestedFilename(CyberKit::WebProcessPool& processPool, CyberKit::DownloadProxy& downloadProxy, const String& filename, Function<void(CyberKit::AllowOverwrite, WTF::String)>&& completionHandler) final
        {
            if (!m_client.decideDestinationWithSuggestedFilename)
                return completionHandler(CyberKit::AllowOverwrite::No, { });

            bool allowOverwrite = false;
            auto destination = adoptWK(m_client.decideDestinationWithSuggestedFilename(CyberKit::toAPI(&processPool), CyberKit::toAPI(&downloadProxy), CyberKit::toAPI(filename.impl()), &allowOverwrite, m_client.base.clientInfo));
            completionHandler(allowOverwrite ? CyberKit::AllowOverwrite::Yes : CyberKit::AllowOverwrite::No, CyberKit::toWTFString(destination.get()));
        }

        void didCreateDestination(CyberKit::WebProcessPool& processPool, CyberKit::DownloadProxy& downloadProxy, const String& path) final
        {
            if (!m_client.didCreateDestination)
                return;

            m_client.didCreateDestination(CyberKit::toAPI(&processPool), CyberKit::toAPI(&downloadProxy), CyberKit::toAPI(path.impl()), m_client.base.clientInfo);
        }

        void didFinish(CyberKit::WebProcessPool& processPool, CyberKit::DownloadProxy& downloadProxy) final
        {
            if (!m_client.didFinish)
                return;

            m_client.didFinish(CyberKit::toAPI(&processPool), CyberKit::toAPI(&downloadProxy), m_client.base.clientInfo);
        }

        void didFail(CyberKit::WebProcessPool& processPool, CyberKit::DownloadProxy& downloadProxy, const CyberCore::ResourceError& error) final
        {
            if (!m_client.didFail)
                return;

            m_client.didFail(CyberKit::toAPI(&processPool), CyberKit::toAPI(&downloadProxy), CyberKit::toAPI(error), m_client.base.clientInfo);
        }
        
        void didCancel(CyberKit::WebProcessPool& processPool, CyberKit::DownloadProxy& downloadProxy) final
        {
            if (!m_client.didCancel)
                return;
            
            m_client.didCancel(CyberKit::toAPI(&processPool), CyberKit::toAPI(&downloadProxy), m_client.base.clientInfo);
        }
        
        void processDidCrash(CyberKit::WebProcessPool& processPool, CyberKit::DownloadProxy& downloadProxy) final
        {
            if (!m_client.processDidCrash)
                return;
            
            m_client.processDidCrash(CyberKit::toAPI(&processPool), CyberKit::toAPI(&downloadProxy), m_client.base.clientInfo);
        }

        void willSendRequest(CyberKit::WebProcessPool& processPool, CyberKit::DownloadProxy& downloadProxy, CyberCore::ResourceRequest&& request, const CyberCore::ResourceResponse&, CompletionHandler<void(CyberCore::ResourceRequest&&)>&& completionHandler) final
        {
            if (m_client.didReceiveServerRedirect)
                m_client.didReceiveServerRedirect(CyberKit::toAPI(&processPool), CyberKit::toAPI(&downloadProxy), CyberKit::toURLRef(request.url().string().impl()), m_client.base.clientInfo);

            completionHandler(WTFMove(request));
        }
    };

    CyberKit::toImpl(contextRef)->setDownloadClient(std::make_unique<DownloadClient>(wkClient));
}

void WKContextSetConnectionClient(WKContextRef contextRef, const WKContextConnectionClientBase* wkClient)
{
    CyberKit::toImpl(contextRef)->initializeConnectionClient(wkClient);
}

WKDownloadRef WKContextDownloadURLRequest(WKContextRef contextRef, WKURLRequestRef requestRef)
{
    return CyberKit::toAPI(&CyberKit::toImpl(contextRef)->download(0, CyberKit::toImpl(requestRef)->resourceRequest()));
}

WKDownloadRef WKContextResumeDownload(WKContextRef contextRef, WKDataRef resumeData, WKStringRef path)
{
    return CyberKit::toAPI(&CyberKit::toImpl(contextRef)->resumeDownload(nullptr, CyberKit::toImpl(resumeData), CyberKit::toWTFString(path)));
}

void WKContextSetInitializationUserDataForInjectedBundle(WKContextRef contextRef,  WKTypeRef userDataRef)
{
    CyberKit::toImpl(contextRef)->setInjectedBundleInitializationUserData(CyberKit::toImpl(userDataRef));
}

void WKContextPostMessageToInjectedBundle(WKContextRef contextRef, WKStringRef messageNameRef, WKTypeRef messageBodyRef)
{
    CyberKit::toImpl(contextRef)->postMessageToInjectedBundle(CyberKit::toImpl(messageNameRef)->string(), CyberKit::toImpl(messageBodyRef));
}

void WKContextGetGlobalStatistics(WKContextStatistics* statistics)
{
    const CyberKit::WebProcessPool::Statistics& webContextStatistics = CyberKit::WebProcessPool::statistics();

    statistics->wkViewCount = webContextStatistics.wkViewCount;
    statistics->wkPageCount = webContextStatistics.wkPageCount;
    statistics->wkFrameCount = webContextStatistics.wkFrameCount;
}

void WKContextAddVisitedLink(WKContextRef contextRef, WKStringRef visitedURL)
{
    String visitedURLString = CyberKit::toImpl(visitedURL)->string();
    if (visitedURLString.isEmpty())
        return;

    CyberKit::toImpl(contextRef)->visitedLinkStore().addVisitedLinkHash(CyberCore::computeSharedStringHash(visitedURLString));
}

void WKContextClearVisitedLinks(WKContextRef contextRef)
{
    CyberKit::toImpl(contextRef)->visitedLinkStore().removeAll();
}

void WKContextSetCacheModel(WKContextRef contextRef, WKCacheModel cacheModel)
{
    CyberKit::toImpl(contextRef)->setCacheModel(CyberKit::toCacheModel(cacheModel));
}

WKCacheModel WKContextGetCacheModel(WKContextRef contextRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(contextRef)->cacheModel());
}

void WKContextSetMaximumNumberOfProcesses(WKContextRef, unsigned)
{
    // Deprecated.
}

unsigned WKContextGetMaximumNumberOfProcesses(WKContextRef)
{
    // Deprecated.
    return std::numeric_limits<unsigned>::max();
}

void WKContextSetAlwaysUsesComplexTextCodePath(WKContextRef contextRef, bool alwaysUseComplexTextCodePath)
{
    CyberKit::toImpl(contextRef)->setAlwaysUsesComplexTextCodePath(alwaysUseComplexTextCodePath);
}

void WKContextSetShouldUseFontSmoothing(WKContextRef contextRef, bool useFontSmoothing)
{
    CyberKit::toImpl(contextRef)->setShouldUseFontSmoothing(useFontSmoothing);
}

void WKContextSetAdditionalPluginsDirectory(WKContextRef contextRef, WKStringRef pluginsDirectory)
{
#if ENABLE(NETSCAPE_PLUGIN_API)
    CyberKit::toImpl(contextRef)->setAdditionalPluginsDirectory(CyberKit::toImpl(pluginsDirectory)->string());
#else
    UNUSED_PARAM(contextRef);
    UNUSED_PARAM(pluginsDirectory);
#endif
}

void WKContextRefreshPlugIns(WKContextRef context)
{
#if ENABLE(NETSCAPE_PLUGIN_API)
    CyberKit::toImpl(context)->refreshPlugins();
#else
    UNUSED_PARAM(context);
#endif
}

void WKContextRegisterURLSchemeAsEmptyDocument(WKContextRef contextRef, WKStringRef urlScheme)
{
    CyberKit::toImpl(contextRef)->registerURLSchemeAsEmptyDocument(CyberKit::toImpl(urlScheme)->string());
}

void WKContextRegisterURLSchemeAsSecure(WKContextRef contextRef, WKStringRef urlScheme)
{
    CyberKit::toImpl(contextRef)->registerURLSchemeAsSecure(CyberKit::toImpl(urlScheme)->string());
}

void WKContextRegisterURLSchemeAsBypassingContentSecurityPolicy(WKContextRef contextRef, WKStringRef urlScheme)
{
    CyberKit::toImpl(contextRef)->registerURLSchemeAsBypassingContentSecurityPolicy(CyberKit::toImpl(urlScheme)->string());
}

void WKContextRegisterURLSchemeAsCachePartitioned(WKContextRef contextRef, WKStringRef urlScheme)
{
    CyberKit::toImpl(contextRef)->registerURLSchemeAsCachePartitioned(CyberKit::toImpl(urlScheme)->string());
}

void WKContextRegisterURLSchemeAsCanDisplayOnlyIfCanRequest(WKContextRef contextRef, WKStringRef urlScheme)
{
    CyberKit::toImpl(contextRef)->registerURLSchemeAsCanDisplayOnlyIfCanRequest(CyberKit::toImpl(urlScheme)->string());
}

void WKContextSetDomainRelaxationForbiddenForURLScheme(WKContextRef contextRef, WKStringRef urlScheme)
{
    CyberKit::toImpl(contextRef)->setDomainRelaxationForbiddenForURLScheme(CyberKit::toImpl(urlScheme)->string());
}

void WKContextSetCanHandleHTTPSServerTrustEvaluation(WKContextRef contextRef, bool value)
{
    CyberKit::toImpl(contextRef)->setCanHandleHTTPSServerTrustEvaluation(value);
}

void WKContextSetPrewarmsProcessesAutomatically(WKContextRef contextRef, bool value)
{
    CyberKit::toImpl(contextRef)->configuration().setIsAutomaticProcessWarmingEnabled(value);
}

void WKContextSetUsesSingleWebProcess(WKContextRef contextRef, bool value)
{
    CyberKit::toImpl(contextRef)->configuration().setUsesSingleWebProcess(value);
}

bool WKContextGetUsesSingleWebProcess(WKContextRef contextRef)
{
    return CyberKit::toImpl(contextRef)->configuration().usesSingleWebProcess();
}

void WKContextSetCustomWebContentServiceBundleIdentifier(WKContextRef contextRef, WKStringRef name)
{
    CyberKit::toImpl(contextRef)->setCustomWebContentServiceBundleIdentifier(CyberKit::toImpl(name)->string());
}

void WKContextSetServiceWorkerFetchTimeoutForTesting(WKContextRef contextRef, double seconds)
{
    CyberKit::toImpl(contextRef)->setServiceWorkerTimeoutForTesting((Seconds)seconds);
}

void WKContextResetServiceWorkerFetchTimeoutForTesting(WKContextRef contextRef)
{
    CyberKit::toImpl(contextRef)->resetServiceWorkerTimeoutForTesting();
}

void WKContextSetDiskCacheSpeculativeValidationEnabled(WKContextRef contextRef, bool value)
{
    CyberKit::toImpl(contextRef)->configuration().setDiskCacheSpeculativeValidationEnabled(value);
}

void WKContextPreconnectToServer(WKContextRef contextRef, WKURLRef serverURLRef)
{
    CyberKit::toImpl(contextRef)->preconnectToServer(URL(URL(), CyberKit::toWTFString(serverURLRef)));
}

WKCookieManagerRef WKContextGetCookieManager(WKContextRef contextRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(contextRef)->supplement<CyberKit::WebCookieManagerProxy>());
}

WKWebsiteDataStoreRef WKContextGetWebsiteDataStore(WKContextRef context)
{
    auto* dataStore = CyberKit::toImpl(context)->websiteDataStore();
    if (!dataStore) {
        auto defaultDataStore = API::WebsiteDataStore::defaultDataStore();
        CyberKit::toImpl(context)->setPrimaryDataStore(defaultDataStore.get());
        dataStore = defaultDataStore.ptr();
    }

    return CyberKit::toAPI(dataStore);
}

WKApplicationCacheManagerRef WKContextGetApplicationCacheManager(WKContextRef context)
{
    return reinterpret_cast<WKApplicationCacheManagerRef>(WKContextGetWebsiteDataStore(context));
}

WKGeolocationManagerRef WKContextGetGeolocationManager(WKContextRef contextRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(contextRef)->supplement<CyberKit::WebGeolocationManagerProxy>());
}

WKIconDatabaseRef WKContextGetIconDatabase(WKContextRef)
{
    return nullptr;
}

WKKeyValueStorageManagerRef WKContextGetKeyValueStorageManager(WKContextRef context)
{
    return reinterpret_cast<WKKeyValueStorageManagerRef>(WKContextGetWebsiteDataStore(context));
}

WKMediaSessionFocusManagerRef WKContextGetMediaSessionFocusManager(WKContextRef context)
{
#if ENABLE(MEDIA_SESSION)
    return CyberKit::toAPI(CyberKit::toImpl(context)->supplement<CyberKit::WebMediaSessionFocusManager>());
#else
    UNUSED_PARAM(context);
    return nullptr;
#endif
}

WKNotificationManagerRef WKContextGetNotificationManager(WKContextRef contextRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(contextRef)->supplement<CyberKit::WebNotificationManagerProxy>());
}

WKResourceCacheManagerRef WKContextGetResourceCacheManager(WKContextRef context)
{
    return reinterpret_cast<WKResourceCacheManagerRef>(WKContextGetWebsiteDataStore(context));
}

void WKContextStartMemorySampler(WKContextRef contextRef, WKDoubleRef interval)
{
    CyberKit::toImpl(contextRef)->startMemorySampler(CyberKit::toImpl(interval)->value());
}

void WKContextStopMemorySampler(WKContextRef contextRef)
{
    CyberKit::toImpl(contextRef)->stopMemorySampler();
}

void WKContextSetIconDatabasePath(WKContextRef, WKStringRef)
{
}

void WKContextAllowSpecificHTTPSCertificateForHost(WKContextRef contextRef, WKCertificateInfoRef certificateRef, WKStringRef hostRef)
{
    CyberKit::toImpl(contextRef)->allowSpecificHTTPSCertificateForHost(CyberKit::toImpl(certificateRef), CyberKit::toImpl(hostRef)->string());
}

void WKContextDisableProcessTermination(WKContextRef contextRef)
{
    CyberKit::toImpl(contextRef)->disableProcessTermination();
}

void WKContextEnableProcessTermination(WKContextRef contextRef)
{
    CyberKit::toImpl(contextRef)->enableProcessTermination();
}

void WKContextSetHTTPPipeliningEnabled(WKContextRef contextRef, bool enabled)
{
    CyberKit::toImpl(contextRef)->setHTTPPipeliningEnabled(enabled);
}

void WKContextWarmInitialProcess(WKContextRef contextRef)
{
    CyberKit::toImpl(contextRef)->prewarmProcess();
}

void WKContextGetStatistics(WKContextRef contextRef, void* context, WKContextGetStatisticsFunction callback)
{
    CyberKit::toImpl(contextRef)->getStatistics(0xFFFFFFFF, CyberKit::toGenericCallbackFunction(context, callback));
}

void WKContextGetStatisticsWithOptions(WKContextRef contextRef, WKStatisticsOptions optionsMask, void* context, WKContextGetStatisticsFunction callback)
{
    CyberKit::toImpl(contextRef)->getStatistics(optionsMask, CyberKit::toGenericCallbackFunction(context, callback));
}

bool WKContextJavaScriptConfigurationFileEnabled(WKContextRef contextRef)
{
    return CyberKit::toImpl(contextRef)->javaScriptConfigurationFileEnabled();
}

void WKContextSetJavaScriptConfigurationFileEnabled(WKContextRef contextRef, bool enable)
{
    CyberKit::toImpl(contextRef)->setJavaScriptConfigurationFileEnabled(enable);
}

void WKContextGarbageCollectJavaScriptObjects(WKContextRef contextRef)
{
    CyberKit::toImpl(contextRef)->garbageCollectJavaScriptObjects();
}

void WKContextSetJavaScriptGarbageCollectorTimerEnabled(WKContextRef contextRef, bool enable)
{
    CyberKit::toImpl(contextRef)->setJavaScriptGarbageCollectorTimerEnabled(enable);
}

void WKContextUseTestingNetworkSession(WKContextRef context)
{
    CyberKit::toImpl(context)->useTestingNetworkSession();
}

void WKContextSetAllowsAnySSLCertificateForWebSocketTesting(WKContextRef context, bool allows)
{
    CyberKit::toImpl(context)->setAllowsAnySSLCertificateForWebSocket(allows);
}

void WKContextSetAllowsAnySSLCertificateForServiceWorkerTesting(WKContextRef context, bool allows)
{
#if ENABLE(SERVICE_WORKER)
    CyberKit::toImpl(context)->setAllowsAnySSLCertificateForServiceWorker(allows);
#endif
}

void WKContextClearCachedCredentials(WKContextRef context)
{
    CyberKit::toImpl(context)->clearCachedCredentials();
}

WKDictionaryRef WKContextCopyPlugInAutoStartOriginHashes(WKContextRef contextRef)
{
    return CyberKit::toAPI(&CyberKit::toImpl(contextRef)->plugInAutoStartOriginHashes().leakRef());
}

void WKContextSetPlugInAutoStartOriginHashes(WKContextRef contextRef, WKDictionaryRef dictionaryRef)
{
    if (!dictionaryRef)
        return;
    CyberKit::toImpl(contextRef)->setPlugInAutoStartOriginHashes(*CyberKit::toImpl(dictionaryRef));
}

void WKContextSetPlugInAutoStartOriginsFilteringOutEntriesAddedAfterTime(WKContextRef contextRef, WKDictionaryRef dictionaryRef, double time)
{
    if (!dictionaryRef)
        return;
    CyberKit::toImpl(contextRef)->setPlugInAutoStartOriginsFilteringOutEntriesAddedAfterTime(*CyberKit::toImpl(dictionaryRef), WallTime::fromRawSeconds(time));
}

void WKContextSetPlugInAutoStartOrigins(WKContextRef contextRef, WKArrayRef arrayRef)
{
    if (!arrayRef)
        return;
    CyberKit::toImpl(contextRef)->setPlugInAutoStartOrigins(*CyberKit::toImpl(arrayRef));
}

void WKContextSetInvalidMessageFunction(WKContextInvalidMessageFunction invalidMessageFunction)
{
    CyberKit::WebProcessPool::setInvalidMessageCallback(invalidMessageFunction);
}

void WKContextSetMemoryCacheDisabled(WKContextRef contextRef, bool disabled)
{
    CyberKit::toImpl(contextRef)->setMemoryCacheDisabled(disabled);
}

void WKContextSetFontWhitelist(WKContextRef contextRef, WKArrayRef arrayRef)
{
    CyberKit::toImpl(contextRef)->setFontWhitelist(CyberKit::toImpl(arrayRef));
}

void WKContextTerminateNetworkProcess(WKContextRef context)
{
    CyberKit::toImpl(context)->terminateNetworkProcess();
}

void WKContextTerminateServiceWorkerProcess(WKContextRef context)
{
    CyberKit::toImpl(context)->terminateServiceWorkerProcesses();
}

ProcessID WKContextGetNetworkProcessIdentifier(WKContextRef contextRef)
{
    return CyberKit::toImpl(contextRef)->networkProcessIdentifier();
}

void WKContextAddSupportedPlugin(WKContextRef contextRef, WKStringRef domainRef, WKStringRef nameRef, WKArrayRef mimeTypesRef, WKArrayRef extensionsRef)
{
#if ENABLE(NETSCAPE_PLUGIN_API)
    HashSet<String> mimeTypes;
    HashSet<String> extensions;

    size_t count = WKArrayGetSize(mimeTypesRef);
    for (size_t i = 0; i < count; ++i)
        mimeTypes.add(CyberKit::toWTFString(static_cast<WKStringRef>(WKArrayGetItemAtIndex(mimeTypesRef, i))));
    count = WKArrayGetSize(extensionsRef);
    for (size_t i = 0; i < count; ++i)
        extensions.add(CyberKit::toWTFString(static_cast<WKStringRef>(WKArrayGetItemAtIndex(extensionsRef, i))));

    CyberKit::toImpl(contextRef)->addSupportedPlugin(CyberKit::toWTFString(domainRef), CyberKit::toWTFString(nameRef), WTFMove(mimeTypes), WTFMove(extensions));
#endif
}

void WKContextClearSupportedPlugins(WKContextRef contextRef)
{
#if ENABLE(NETSCAPE_PLUGIN_API)
    CyberKit::toImpl(contextRef)->clearSupportedPlugins();
#endif
}

void WKContextSetIDBPerOriginQuota(WKContextRef contextRef, uint64_t quota)
{
    CyberKit::toImpl(contextRef)->setIDBPerOriginQuota(quota);
}

void WKContextClearCurrentModifierStateForTesting(WKContextRef contextRef)
{
    CyberKit::toImpl(contextRef)->clearCurrentModifierStateForTesting();
}
