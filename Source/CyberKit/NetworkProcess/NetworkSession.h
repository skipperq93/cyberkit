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

#pragma once

#include "PrefetchCache.h"
#include "SandboxExtension.h"
#include "ServiceWorkerSoftUpdateLoader.h"
#include "WebResourceLoadStatisticsStore.h"
#include <CyberCore/AdClickAttribution.h>
#include <CyberCore/BlobRegistryImpl.h>
#include <CyberCore/NetworkStorageSession.h>
#include <CyberCore/RegistrableDomain.h>
#include <pal/SessionID.h>
#include <wtf/HashSet.h>
#include <wtf/Ref.h>
#include <wtf/Seconds.h>
#include <wtf/UniqueRef.h>
#include <wtf/WeakPtr.h>
#include <wtf/text/WTFString.h>

namespace CyberCore {
class NetworkStorageSession;
class ResourceRequest;
enum class IncludeHttpOnlyCookies : bool;
enum class ShouldSample : bool;
}

namespace CyberKit {

class AdClickAttributionManager;
class NetworkDataTask;
class NetworkProcess;
class NetworkResourceLoader;
class NetworkSocketChannel;
class WebResourceLoadStatisticsStore;
class WebSocketTask;
struct NetworkSessionCreationParameters;

enum class WebsiteDataType;

namespace NetworkCache {
class Cache;
}

class NetworkSession : public CanMakeWeakPtr<NetworkSession> {
    WTF_MAKE_FAST_ALLOCATED;
public:
    static std::unique_ptr<NetworkSession> create(NetworkProcess&, NetworkSessionCreationParameters&&);
    virtual ~NetworkSession();

    virtual void invalidateAndCancel();
    virtual void clearCredentials() { };
    virtual bool shouldLogCookieInformation() const { return false; }
    virtual Seconds loadThrottleLatency() const { return { }; }

    PAL::SessionID sessionID() const { return m_sessionID; }
    NetworkProcess& networkProcess() { return m_networkProcess; }
    CyberCore::NetworkStorageSession* networkStorageSession() const;

    void registerNetworkDataTask(NetworkDataTask& task) { m_dataTaskSet.add(&task); }
    void unregisterNetworkDataTask(NetworkDataTask& task) { m_dataTaskSet.remove(&task); }

#if ENABLE(RESOURCE_LOAD_STATISTICS)
    WebResourceLoadStatisticsStore* resourceLoadStatistics() const { return m_resourceLoadStatistics.get(); }
    void setResourceLoadStatisticsEnabled(bool);
    void recreateResourceLoadStatisticStore(CompletionHandler<void()>&&);
    bool isResourceLoadStatisticsEnabled() const;
    void notifyResourceLoadStatisticsProcessed();
    void deleteWebsiteDataForRegistrableDomains(OptionSet<WebsiteDataType>, Vector<std::pair<CyberCore::RegistrableDomain, WebsiteDataToRemove>>&&, bool shouldNotifyPage, CompletionHandler<void(const HashSet<CyberCore::RegistrableDomain>&)>&&);
    void registrableDomainsWithWebsiteData(OptionSet<WebsiteDataType>, bool shouldNotifyPage, CompletionHandler<void(HashSet<CyberCore::RegistrableDomain>&&)>&&);
    void logDiagnosticMessageWithValue(const String& message, const String& description, unsigned value, unsigned significantFigures, CyberCore::ShouldSample);
    void notifyPageStatisticsTelemetryFinished(unsigned numberOfPrevalentResources, unsigned numberOfPrevalentResourcesWithUserInteraction, unsigned numberOfPrevalentResourcesWithoutUserInteraction, unsigned topPrevalentResourceWithUserInteractionDaysSinceUserInteraction, unsigned medianDaysSinceUserInteractionPrevalentResourceWithUserInteraction, unsigned top3NumberOfPrevalentResourcesWithUI, unsigned top3MedianSubFrameWithoutUI, unsigned top3MedianSubResourceWithoutUI, unsigned top3MedianUniqueRedirectsWithoutUI, unsigned top3MedianDataRecordsRemovedWithoutUI);
    bool enableResourceLoadStatisticsLogTestingEvent() const { return m_enableResourceLoadStatisticsLogTestingEvent; }
    void setResourceLoadStatisticsLogTestingEvent(bool log) { m_enableResourceLoadStatisticsLogTestingEvent = log; }
    virtual bool hasIsolatedSession(const CyberCore::RegistrableDomain) const { return false; }
    virtual void clearIsolatedSessions() { }
    void setShouldDowngradeReferrerForTesting(bool);
    bool shouldDowngradeReferrer() const;
    void setThirdPartyCookieBlockingMode(CyberCore::ThirdPartyCookieBlockingMode);
#endif
    void storeAdClickAttribution(CyberCore::AdClickAttribution&&);
    void handleAdClickAttributionConversion(CyberCore::AdClickAttribution::Conversion&&, const URL& requestURL, const CyberCore::ResourceRequest& redirectRequest);
    void dumpAdClickAttribution(CompletionHandler<void(String)>&&);
    void clearAdClickAttribution();
    void clearAdClickAttributionForRegistrableDomain(CyberCore::RegistrableDomain&&);
    void setAdClickAttributionOverrideTimerForTesting(bool value);
    void setAdClickAttributionConversionURLForTesting(URL&&);
    void markAdClickAttributionsAsExpiredForTesting();

    void addKeptAliveLoad(Ref<NetworkResourceLoader>&&);
    void removeKeptAliveLoad(NetworkResourceLoader&);

    NetworkCache::Cache* cache() { return m_cache.get(); }

    PrefetchCache& prefetchCache() { return m_prefetchCache; }
    void clearPrefetchCache() { m_prefetchCache.clear(); }

    virtual std::unique_ptr<WebSocketTask> createWebSocketTask(NetworkSocketChannel&, const CyberCore::ResourceRequest&, const String& protocol);
    virtual void removeWebSocketTask(WebSocketTask&) { }
    virtual void addWebSocketTask(WebSocketTask&) { }

    CyberCore::BlobRegistryImpl& blobRegistry() { return m_blobRegistry; }

    unsigned testSpeedMultiplier() const { return m_testSpeedMultiplier; }
    bool allowsServerPreconnect() const { return m_allowsServerPreconnect; }

    bool isStaleWhileRevalidateEnabled() const { return m_isStaleWhileRevalidateEnabled; }

#if ENABLE(SERVICE_WORKER)
    void addSoftUpdateLoader(std::unique_ptr<ServiceWorkerSoftUpdateLoader>&& loader) { m_softUpdateLoaders.add(WTFMove(loader)); }
    void removeSoftUpdateLoader(ServiceWorkerSoftUpdateLoader* loader) { m_softUpdateLoaders.remove(loader); }
#endif

protected:
    NetworkSession(NetworkProcess&, const NetworkSessionCreationParameters&);

#if ENABLE(RESOURCE_LOAD_STATISTICS)
    void destroyResourceLoadStatistics();
    void forwardResourceLoadStatisticsSettings();
#endif

    PAL::SessionID m_sessionID;
    Ref<NetworkProcess> m_networkProcess;
    HashSet<NetworkDataTask*> m_dataTaskSet;
    String m_resourceLoadStatisticsDirectory;
#if ENABLE(RESOURCE_LOAD_STATISTICS)
    RefPtr<WebResourceLoadStatisticsStore> m_resourceLoadStatistics;
    ShouldIncludeLocalhost m_shouldIncludeLocalhostInResourceLoadStatistics { ShouldIncludeLocalhost::Yes };
    EnableResourceLoadStatisticsDebugMode m_enableResourceLoadStatisticsDebugMode { EnableResourceLoadStatisticsDebugMode::No };
    CyberCore::RegistrableDomain m_resourceLoadStatisticsManualPrevalentResource;
    bool m_enableResourceLoadStatisticsLogTestingEvent;
    bool m_downgradeReferrer { true };
    CyberCore::ThirdPartyCookieBlockingMode m_thirdPartyCookieBlockingMode { CyberCore::ThirdPartyCookieBlockingMode::All };
    CyberCore::FirstPartyWebsiteDataRemovalMode m_firstPartyWebsiteDataRemovalMode { CyberCore::FirstPartyWebsiteDataRemovalMode::AllButCookies };
#endif
    bool m_isStaleWhileRevalidateEnabled { false };
    UniqueRef<AdClickAttributionManager> m_adClickAttribution;

    HashSet<Ref<NetworkResourceLoader>> m_keptAliveLoads;

    PrefetchCache m_prefetchCache;

#if !ASSERT_DISABLED
    bool m_isInvalidated { false };
#endif
    RefPtr<NetworkCache::Cache> m_cache;
    CyberCore::BlobRegistryImpl m_blobRegistry;
    unsigned m_testSpeedMultiplier { 1 };
    bool m_allowsServerPreconnect { true };

#if ENABLE(SERVICE_WORKER)
    HashSet<std::unique_ptr<ServiceWorkerSoftUpdateLoader>> m_softUpdateLoaders;
#endif
};

} // namespace CyberKit
