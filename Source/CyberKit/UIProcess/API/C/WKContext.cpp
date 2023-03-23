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
#include "GPUProcessProxy.h"
#include "LegacyGlobalSettings.h"
#include "WKAPICast.h"
#include "WKArray.h"
#include "WKContextConfigurationRef.h"
#include "WKRetainPtr.h"
#include "WKString.h"
#include "WKWebsiteDataStoreRef.h"
#include "WebContextInjectedBundleClient.h"
#include "WebPageProxy.h"
#include "WebProcessPool.h"
#include <CyberCore/GamepadProvider.h>
#include <wtf/RefPtr.h>
#include <wtf/text/WTFString.h>

// Supplements
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
    auto configuration = API::ProcessPoolConfiguration::create();
    return CyberKit::toAPI(&CyberKit::WebProcessPool::create(configuration).leakRef());
}

WKContextRef WKContextCreateWithInjectedBundlePath(WKStringRef pathRef)
{
    auto configuration = API::ProcessPoolConfiguration::create();
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
    CyberKit::toImpl(contextRef)->setInjectedBundleClient(makeUnique<CyberKit::WebContextInjectedBundleClient>(wkClient));
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
    processPool.setHistoryClient(makeUnique<HistoryClient>(wkClient));

    bool addsVisitedLinks = processPool.historyClient().addsVisitedLinks();

    for (auto& process : processPool.processes()) {
        for (auto& page : process->pages())
            page->setAddsVisitedLinks(addsVisitedLinks);
    }
}

void WKContextSetDownloadClient(WKContextRef context, const WKContextDownloadClientBase* wkClient)
{
    class LegacyDownloadClient final : public API::Client<WKContextDownloadClientBase>, public API::DownloadClient {
    public:
        explicit LegacyDownloadClient(const WKContextDownloadClientBase* client, WKContextRef context)
            : m_context(context)
        {
            initialize(client);
        }
    private:
        void legacyDidStart(CyberKit::DownloadProxy& downloadProxy) final
        {
            if (!m_client.didStart)
                return;
            m_client.didStart(m_context, CyberKit::toAPI(&downloadProxy), m_client.base.clientInfo);
        }
        void didReceiveAuthenticationChallenge(CyberKit::DownloadProxy& downloadProxy, CyberKit::AuthenticationChallengeProxy& authenticationChallengeProxy) final
        {
            if (!m_client.didReceiveAuthenticationChallenge)
                return;
            m_client.didReceiveAuthenticationChallenge(m_context, CyberKit::toAPI(&downloadProxy), CyberKit::toAPI(&authenticationChallengeProxy), m_client.base.clientInfo);
        }
        void didReceiveResponse(CyberKit::DownloadProxy& downloadProxy, const CyberCore::ResourceResponse& response)
        {
            if (!m_client.didReceiveResponse)
                return;
            m_client.didReceiveResponse(m_context, CyberKit::toAPI(&downloadProxy), CyberKit::toAPI(API::URLResponse::create(response).ptr()), m_client.base.clientInfo);
        }
        void didReceiveData(CyberKit::DownloadProxy& downloadProxy, uint64_t length, uint64_t, uint64_t) final
        {
            if (!m_client.didReceiveData)
                return;
            m_client.didReceiveData(m_context, CyberKit::toAPI(&downloadProxy), length, m_client.base.clientInfo);
        }
        void decideDestinationWithSuggestedFilename(CyberKit::DownloadProxy& downloadProxy, const CyberCore::ResourceResponse& response, const String& filename, CompletionHandler<void(CyberKit::AllowOverwrite, WTF::String)>&& completionHandler) final
        {
            didReceiveResponse(downloadProxy, response);
            if (!m_client.decideDestinationWithSuggestedFilename)
                return completionHandler(CyberKit::AllowOverwrite::No, { });
            bool allowOverwrite = false;
            auto destination = adoptWK(m_client.decideDestinationWithSuggestedFilename(m_context, CyberKit::toAPI(&downloadProxy), CyberKit::toAPI(filename.impl()), &allowOverwrite, m_client.base.clientInfo));
            completionHandler(allowOverwrite ? CyberKit::AllowOverwrite::Yes : CyberKit::AllowOverwrite::No, CyberKit::toWTFString(destination.get()));
        }
        void didCreateDestination(CyberKit::DownloadProxy& downloadProxy, const String& path) final
        {
            if (!m_client.didCreateDestination)
                return;
            m_client.didCreateDestination(m_context, CyberKit::toAPI(&downloadProxy), CyberKit::toAPI(path.impl()), m_client.base.clientInfo);
        }
        void didFinish(CyberKit::DownloadProxy& downloadProxy) final
        {
            if (!m_client.didFinish)
                return;
            m_client.didFinish(m_context, CyberKit::toAPI(&downloadProxy), m_client.base.clientInfo);
        }
        void didFail(CyberKit::DownloadProxy& downloadProxy, const CyberCore::ResourceError& error, API::Data*) final
        {
            if (!m_client.didFail)
                return;
            m_client.didFail(m_context, CyberKit::toAPI(&downloadProxy), CyberKit::toAPI(error), m_client.base.clientInfo);
        }
        void legacyDidCancel(CyberKit::DownloadProxy& downloadProxy) final
        {
            if (!m_client.didCancel)
                return;
            m_client.didCancel(m_context, CyberKit::toAPI(&downloadProxy), m_client.base.clientInfo);
        }
        void processDidCrash(CyberKit::DownloadProxy& downloadProxy) final
        {
            if (!m_client.processDidCrash)
                return;
            m_client.processDidCrash(m_context, CyberKit::toAPI(&downloadProxy), m_client.base.clientInfo);
        }
        void willSendRequest(CyberKit::DownloadProxy& downloadProxy, CyberCore::ResourceRequest&& request, const CyberCore::ResourceResponse&, CompletionHandler<void(CyberCore::ResourceRequest&&)>&& completionHandler) final
        {
            if (m_client.didReceiveServerRedirect)
                m_client.didReceiveServerRedirect(m_context, CyberKit::toAPI(&downloadProxy), CyberKit::toURLRef(request.url().string().impl()), m_client.base.clientInfo);
            completionHandler(WTFMove(request));
        }
        WKContextRef m_context;
    };
    CyberKit::toImpl(context)->setLegacyDownloadClient(adoptRef(*new LegacyDownloadClient(wkClient, context)));
}

void WKContextSetConnectionClient(WKContextRef contextRef, const WKContextConnectionClientBase* wkClient)
{
    CyberKit::toImpl(contextRef)->initializeConnectionClient(wkClient);
}

WKDownloadRef WKContextDownloadURLRequest(WKContextRef, WKURLRequestRef)
{
    return nullptr;
}

WKDownloadRef WKContextResumeDownload(WKContextRef, WKDataRef, WKStringRef)
{
    return nullptr;
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
    CyberKit::LegacyGlobalSettings::singleton().setCacheModel(CyberKit::toCacheModel(cacheModel));
}

WKCacheModel WKContextGetCacheModel(WKContextRef contextRef)
{
    return CyberKit::toAPI(CyberKit::LegacyGlobalSettings::singleton().cacheModel());
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

void WKContextSetDisableFontSubpixelAntialiasingForTesting(WKContextRef contextRef, bool disable)
{
    CyberKit::toImpl(contextRef)->setDisableFontSubpixelAntialiasingForTesting(disable);
}

void WKContextSetAdditionalPluginsDirectory(WKContextRef contextRef, WKStringRef pluginsDirectory)
{
    UNUSED_PARAM(contextRef);
    UNUSED_PARAM(pluginsDirectory);
}

void WKContextRefreshPlugIns(WKContextRef context)
{
    UNUSED_PARAM(context);
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

void WKContextSetDiskCacheSpeculativeValidationEnabled(WKContextRef, bool)
{
}

void WKContextPreconnectToServer(WKContextRef, WKURLRef)
{
}

WKWebsiteDataStoreRef WKContextGetWebsiteDataStore(WKContextRef)
{
    return WKWebsiteDataStoreGetDefaultDataStore();
}

WKApplicationCacheManagerRef WKContextGetApplicationCacheManager(WKContextRef context)
{
    return reinterpret_cast<WKApplicationCacheManagerRef>(WKWebsiteDataStoreGetDefaultDataStore());
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
    return reinterpret_cast<WKKeyValueStorageManagerRef>(WKWebsiteDataStoreGetDefaultDataStore());
}

WKNotificationManagerRef WKContextGetNotificationManager(WKContextRef contextRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(contextRef)->supplement<CyberKit::WebNotificationManagerProxy>());
}

WKResourceCacheManagerRef WKContextGetResourceCacheManager(WKContextRef context)
{
    return reinterpret_cast<WKResourceCacheManagerRef>(WKWebsiteDataStoreGetDefaultDataStore());
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

void WKContextAllowSpecificHTTPSCertificateForHost(WKContextRef, WKCertificateInfoRef, WKStringRef)
{
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
}

void WKContextGetStatisticsWithOptions(WKContextRef contextRef, WKStatisticsOptions optionsMask, void* context, WKContextGetStatisticsFunction callback)
{
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

WKDictionaryRef WKContextCopyPlugInAutoStartOriginHashes(WKContextRef)
{
    return nullptr;
}

void WKContextSetPlugInAutoStartOriginHashes(WKContextRef, WKDictionaryRef)
{
}

void WKContextSetPlugInAutoStartOriginsFilteringOutEntriesAddedAfterTime(WKContextRef, WKDictionaryRef, double)
{
}

void WKContextSetPlugInAutoStartOrigins(WKContextRef, WKArrayRef)
{
}

void WKContextSetInvalidMessageFunction(WKContextInvalidMessageFunction invalidMessageFunction)
{
    CyberKit::WebProcessPool::setInvalidMessageCallback(invalidMessageFunction);
}

void WKContextSetMemoryCacheDisabled(WKContextRef contextRef, bool disabled)
{
    CyberKit::toImpl(contextRef)->setMemoryCacheDisabled(disabled);
}

void WKContextSetFontAllowList(WKContextRef contextRef, WKArrayRef arrayRef)
{
    CyberKit::toImpl(contextRef)->setFontAllowList(CyberKit::toImpl(arrayRef));
}

void WKContextTerminateGPUProcess(WKContextRef)
{
#if ENABLE(GPU_PROCESS)
    if (auto* gpuProcess = CyberKit::GPUProcessProxy::singletonIfCreated())
        gpuProcess->terminateForTesting();
#endif
}

void WKContextTerminateServiceWorkers(WKContextRef context)
{
    CyberKit::toImpl(context)->terminateServiceWorkers();
}

void WKContextAddSupportedPlugin(WKContextRef contextRef, WKStringRef domainRef, WKStringRef nameRef, WKArrayRef mimeTypesRef, WKArrayRef extensionsRef)
{
}

void WKContextClearSupportedPlugins(WKContextRef contextRef)
{
}

void WKContextClearCurrentModifierStateForTesting(WKContextRef contextRef)
{
    CyberKit::toImpl(contextRef)->clearCurrentModifierStateForTesting();
}

void WKContextSetUseSeparateServiceWorkerProcess(WKContextRef, bool useSeparateServiceWorkerProcess)
{
    CyberKit::WebProcessPool::setUseSeparateServiceWorkerProcess(useSeparateServiceWorkerProcess);
}

void WKContextSetPrimaryWebsiteDataStore(WKContextRef, WKWebsiteDataStoreRef)
{
}

WKArrayRef WKContextCopyLocalhostAliases(WKContextRef)
{
    return CyberKit::toAPI(&API::Array::createStringArray(copyToVector(CyberKit::LegacyGlobalSettings::singleton().hostnamesToRegisterAsLocal())).leakRef());
}

void WKContextSetLocalhostAliases(WKContextRef, WKArrayRef localhostAliases)
{
    for (const auto& hostname : CyberKit::toImpl(localhostAliases)->toStringVector())
        CyberKit::LegacyGlobalSettings::singleton().registerHostnameAsLocal(hostname);
}

void WKContextClearMockGamepadsForTesting(WKContextRef)
{
#if ENABLE(GAMEPAD)
    if (CyberCore::GamepadProvider::singleton().isMockGamepadProvider())
        CyberCore::GamepadProvider::singleton().clearGamepadsForTesting();
#endif
}
