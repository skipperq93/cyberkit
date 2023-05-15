/*
 * Copyright (C) 2012-2015 Apple Inc. All rights reserved.
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

#include "DownloadID.h"
#include "MessageSender.h"
#include "NetworkCache.h"
#include "NetworkConnectionToWebProcess.h"
#include "NetworkLoadClient.h"
#include "NetworkResourceLoadIdentifier.h"
#include "NetworkResourceLoadParameters.h"
#include "PrivateRelayed.h"
#include <CyberCore/ContentFilterClient.h>
#include <CyberCore/ContentFilterUnblockHandler.h>
#include <CyberCore/ContentSecurityPolicyClient.h>
#include <CyberCore/CrossOriginAccessControl.h>
#include <CyberCore/PrivateClickMeasurement.h>
#include <CyberCore/ReportingClient.h>
#include <CyberCore/ResourceResponse.h>
#include <CyberCore/SWServerRegistration.h>
#include <CyberCore/SecurityPolicyViolationEvent.h>
#include <CyberCore/SharedBuffer.h>
#include <CyberCore/Timer.h>
#include <wtf/MonotonicTime.h>
#include <wtf/WeakPtr.h>

namespace CyberCore {
class BlobDataFileReference;
class ContentFilter;
class FormData;
class LinkHeader;
class NetworkStorageSession;
class Report;
class ResourceRequest;
}

namespace CyberKit {

class EarlyHintsResourceLoader;
class NetworkConnectionToWebProcess;
class NetworkLoad;
class NetworkLoadChecker;
class ServiceWorkerFetchTask;
class WebSWServerConnection;

enum class NegotiatedLegacyTLS : bool;
enum class ViolationReportType : uint8_t;

struct ResourceLoadInfo;

namespace NetworkCache {
class Entry;
}

class NetworkResourceLoader final
    : public RefCounted<NetworkResourceLoader>
    , public NetworkLoadClient
    , public IPC::MessageSender
    , public CyberCore::ContentSecurityPolicyClient
    , public CyberCore::CrossOriginAccessControlCheckDisabler
#if ENABLE(CONTENT_FILTERING_IN_NETWORKING_PROCESS)
    , public CyberCore::ContentFilterClient
#endif
    , public CyberCore::ReportingClient
    , public CanMakeWeakPtr<NetworkResourceLoader> {
public:
    static Ref<NetworkResourceLoader> create(NetworkResourceLoadParameters&& parameters, NetworkConnectionToWebProcess& connection, CompletionHandler<void(const CyberCore::ResourceError&, const CyberCore::ResourceResponse, Vector<uint8_t>&&)>&& reply = nullptr)
    {
        return adoptRef(*new NetworkResourceLoader(WTFMove(parameters), connection, WTFMove(reply)));
    }
    virtual ~NetworkResourceLoader();

    const CyberCore::ResourceRequest& originalRequest() const { return m_parameters.request; }

    NetworkLoad* networkLoad() const { return m_networkLoad.get(); }

    void start();
    void abort();

    void transferToNewWebProcess(NetworkConnectionToWebProcess&, const NetworkResourceLoadParameters&);

    // Message handlers.
    void didReceiveNetworkResourceLoaderMessage(IPC::Connection&, IPC::Decoder&);

    void continueWillSendRequest(CyberCore::ResourceRequest&&, bool isAllowedToAskUserForCredentials);

    void setResponse(CyberCore::ResourceResponse&& response) { m_response = WTFMove(response); }
    const CyberCore::ResourceResponse& response() const { return m_response; }

    NetworkConnectionToWebProcess& connectionToWebProcess() const { return m_connection; }
    PAL::SessionID sessionID() const { return m_connection->sessionID(); }
    CyberCore::ResourceLoaderIdentifier coreIdentifier() const { return m_parameters.identifier; }
    CyberCore::FrameIdentifier frameID() const { return m_parameters.webFrameID; }
    CyberCore::PageIdentifier pageID() const { return m_parameters.webPageID; }
    const NetworkResourceLoadParameters& parameters() const { return m_parameters; }
    NetworkResourceLoadIdentifier identifier() const { return m_resourceLoadID; }
    const URL& firstResponseURL() const { return m_firstResponseURL; }

    NetworkCache::GlobalFrameID globalFrameID() { return { m_parameters.webPageProxyID, pageID(), frameID() }; }

    struct SynchronousLoadData;

    // NetworkLoadClient.
    void didSendData(uint64_t bytesSent, uint64_t totalBytesToBeSent) final;
    bool isSynchronous() const final;
    bool isAllowedToAskUserForCredentials() const final { return m_isAllowedToAskUserForCredentials; }
    void willSendRedirectedRequest(CyberCore::ResourceRequest&&, CyberCore::ResourceRequest&& redirectRequest, CyberCore::ResourceResponse&&) final;
    void didReceiveInformationalResponse(CyberCore::ResourceResponse&&) final;
    void didReceiveResponse(CyberCore::ResourceResponse&&, PrivateRelayed, ResponseCompletionHandler&&) final;
    void didReceiveBuffer(const CyberCore::FragmentedSharedBuffer&, uint64_t reportedEncodedDataLength) final;
    void didFinishLoading(const CyberCore::NetworkLoadMetrics&) final;
    void didFailLoading(const CyberCore::ResourceError&) final;
    void didBlockAuthenticationChallenge() final;
    void didReceiveChallenge(const CyberCore::AuthenticationChallenge&) final;
    bool shouldCaptureExtraNetworkLoadMetrics() const final;

    // CrossOriginAccessControlCheckDisabler
    bool crossOriginAccessControlCheckEnabled() const override;
        
    void convertToDownload(DownloadID, const CyberCore::ResourceRequest&, const CyberCore::ResourceResponse&);

    bool isMainResource() const { return m_parameters.request.requester() == CyberCore::ResourceRequestRequester::Main; }
    bool isMainFrameLoad() const { return isMainResource() && m_parameters.frameAncestorOrigins.isEmpty(); }
    bool isCrossOriginPrefetch() const;

#if ENABLE(TRACKING_PREVENTION) && !RELEASE_LOG_DISABLED
    static bool shouldLogCookieInformation(NetworkConnectionToWebProcess&, PAL::SessionID);
    static void logCookieInformation(NetworkConnectionToWebProcess&, ASCIILiteral label, const void* loggedObject, const CyberCore::NetworkStorageSession&, const URL& firstParty, const CyberCore::SameSiteInfo&, const URL&, const String& referrer, std::optional<CyberCore::FrameIdentifier>, std::optional<CyberCore::PageIdentifier>, std::optional<CyberCore::ResourceLoaderIdentifier>);
#endif

    void disableExtraNetworkLoadMetricsCapture() { m_shouldCaptureExtraNetworkLoadMetrics = false; }

    bool isKeptAlive() const { return m_isKeptAlive; }

    void consumeSandboxExtensionsIfNeeded();

#if ENABLE(SERVICE_WORKER)
    void startWithServiceWorker();
    void serviceWorkerDidNotHandle(ServiceWorkerFetchTask*);
    void setResultingClientIdentifier(String&& identifier) { m_resultingClientIdentifier = WTFMove(identifier); }
    const String& resultingClientIdentifier() const { return m_resultingClientIdentifier; }
    void setServiceWorkerRegistration(CyberCore::SWServerRegistration& serviceWorkerRegistration) { m_serviceWorkerRegistration = serviceWorkerRegistration; }
    void setWorkerStart(MonotonicTime);
    MonotonicTime workerStart() const { return m_workerStart; }
#endif

    std::optional<CyberCore::ResourceError> doCrossOriginOpenerHandlingOfResponse(const CyberCore::ResourceResponse&);
    void sendDidReceiveResponsePotentiallyInNewBrowsingContextGroup(const CyberCore::ResourceResponse&, PrivateRelayed, bool needsContinueDidReceiveResponseMessage);

    bool isAppInitiated();

#if ENABLE(CONTENT_FILTERING_IN_NETWORKING_PROCESS)
    void ref() const final { RefCounted<NetworkResourceLoader>::ref(); }
    void deref() const final { RefCounted<NetworkResourceLoader>::deref(); }
    bool continueAfterServiceWorkerReceivedData(const CyberCore::SharedBuffer&, uint64_t encodedDataLength);
    bool continueAfterServiceWorkerReceivedResponse(const CyberCore::ResourceResponse&);
    void serviceWorkerDidFinish();
#endif

    void willSendServiceWorkerRedirectedRequest(CyberCore::ResourceRequest&&, CyberCore::ResourceRequest&& redirectRequest, CyberCore::ResourceResponse&&);

private:
    NetworkResourceLoader(NetworkResourceLoadParameters&&, NetworkConnectionToWebProcess&, CompletionHandler<void(const CyberCore::ResourceError&, const CyberCore::ResourceResponse, Vector<uint8_t>&&)>&&);

    // IPC::MessageSender
    IPC::Connection* messageSenderConnection() const override;
    uint64_t messageSenderDestinationID() const override { return m_parameters.identifier.toUInt64(); }

#if ENABLE(CONTENT_FILTERING_IN_NETWORKING_PROCESS)
    // ContentFilterClient
    void dataReceivedThroughContentFilter(const CyberCore::SharedBuffer&, size_t) final;
    CyberCore::ResourceError contentFilterDidBlock(CyberCore::ContentFilterUnblockHandler, String&& unblockRequestDeniedScript) final;
    void cancelMainResourceLoadForContentFilter(const CyberCore::ResourceError&) final;
    void handleProvisionalLoadFailureFromContentFilter(const URL& blockedPageURL, CyberCore::SubstituteData&) final;
#endif

    void processClearSiteDataHeader(const CyberCore::ResourceResponse&, CompletionHandler<void()>&&);

    bool canUseCache(const CyberCore::ResourceRequest&) const;
    bool canUseCachedRedirect(const CyberCore::ResourceRequest&) const;

    void tryStoreAsCacheEntry();
    void retrieveCacheEntry(const CyberCore::ResourceRequest&);
    void retrieveCacheEntryInternal(std::unique_ptr<NetworkCache::Entry>&&, CyberCore::ResourceRequest&&);
    void didRetrieveCacheEntry(std::unique_ptr<NetworkCache::Entry>);
    void sendResultForCacheEntry(std::unique_ptr<NetworkCache::Entry>);
    void validateCacheEntry(std::unique_ptr<NetworkCache::Entry>);
    void dispatchWillSendRequestForCacheEntry(CyberCore::ResourceRequest&&, std::unique_ptr<NetworkCache::Entry>&&);

    bool shouldInterruptLoadForXFrameOptions(const String&, const URL&);
    bool shouldInterruptLoadForCSPFrameAncestorsOrXFrameOptions(const CyberCore::ResourceResponse&);
    bool shouldInterruptNavigationForCrossOriginEmbedderPolicy(const CyberCore::ResourceResponse&);
    bool shouldInterruptWorkerLoadForCrossOriginEmbedderPolicy(const CyberCore::ResourceResponse&);

    enum class FirstLoad : bool { No, Yes };
    void startNetworkLoad(CyberCore::ResourceRequest&&, FirstLoad);
    void restartNetworkLoad(CyberCore::ResourceRequest&&);
    void continueDidReceiveResponse();
    void didReceiveMainResourceResponse(const CyberCore::ResourceResponse&);

    enum class LoadResult {
        Unknown,
        Success,
        Failure,
        Cancel
    };
    void cleanup(LoadResult);
    
    void platformDidReceiveResponse(const CyberCore::ResourceResponse&);

    void startBufferingTimerIfNeeded();
    void bufferingTimerFired();
    void sendBuffer(const CyberCore::FragmentedSharedBuffer&, size_t encodedDataLength);

    void consumeSandboxExtensions();
    void invalidateSandboxExtensions();

#if ENABLE(TRACKING_PREVENTION) && !RELEASE_LOG_DISABLED
    void logCookieInformation() const;
#endif

    void continueWillSendRedirectedRequest(CyberCore::ResourceRequest&&, CyberCore::ResourceRequest&& redirectRequest, CyberCore::ResourceResponse&&, std::optional<CyberCore::PCM::AttributionTriggerData>&&);
    void didFinishWithRedirectResponse(CyberCore::ResourceRequest&&, CyberCore::ResourceRequest&& redirectRequest, CyberCore::ResourceResponse&&);
    CyberCore::ResourceResponse sanitizeResponseIfPossible(CyberCore::ResourceResponse&&, CyberCore::ResourceResponse::SanitizationType);

    // ContentSecurityPolicyClient
    void addConsoleMessage(MessageSource, MessageLevel, const String&, unsigned long requestIdentifier = 0) final;
    void enqueueSecurityPolicyViolationEvent(CyberCore::SecurityPolicyViolationEventInit&&) final;

    void logSlowCacheRetrieveIfNeeded(const NetworkCache::Cache::RetrieveInfo&);

    std::optional<Seconds> validateCacheEntryForMaxAgeCapValidation(const CyberCore::ResourceRequest&, const CyberCore::ResourceRequest& redirectRequest, const CyberCore::ResourceResponse&);

    ResourceLoadInfo resourceLoadInfo();

#if ENABLE(CONTENT_FILTERING_IN_NETWORKING_PROCESS)
    bool startContentFiltering(CyberCore::ResourceRequest&);
#endif

    // ReportingClient
    void notifyReportObservers(Ref<CyberCore::Report>&&) final;
    String endpointURIForToken(const String&) const final;
    void sendReportToEndpoints(const URL& baseURL, const Vector<String>& endpointURIs, const Vector<String>& endpointTokens, Ref<CyberCore::FormData>&& report, CyberCore::ViolationReportType) final;
    String httpUserAgent() const final { return originalRequest().httpUserAgent(); }
    void initializeReportingEndpoints(const CyberCore::ResourceResponse&);
    CyberCore::FrameIdentifier frameIdentifierForReport() const;

    enum class IsFromServiceWorker : bool { No, Yes };
    void willSendRedirectedRequestInternal(CyberCore::ResourceRequest&&, CyberCore::ResourceRequest&& redirectRequest, CyberCore::ResourceResponse&&, IsFromServiceWorker);
    std::optional<CyberCore::NetworkLoadMetrics> computeResponseMetrics(const CyberCore::ResourceResponse&) const;

    void startRequest(const CyberCore::ResourceRequest&);
    bool abortIfServiceWorkersOnly();

    NetworkResourceLoadParameters m_parameters;

    Ref<NetworkConnectionToWebProcess> m_connection;

    std::unique_ptr<NetworkLoad> m_networkLoad;

    CyberCore::ResourceResponse m_response;

    size_t m_bufferedDataEncodedDataLength { 0 };
    CyberCore::SharedBufferBuilder m_bufferedData;
    unsigned m_redirectCount { 0 };

    std::unique_ptr<SynchronousLoadData> m_synchronousLoadData;
    Vector<RefPtr<CyberCore::BlobDataFileReference>> m_fileReferences;

    bool m_wasStarted { false };
    bool m_didConsumeSandboxExtensions { false };
    bool m_isAllowedToAskUserForCredentials { false };
    size_t m_numBytesReceived { 0 };

    unsigned m_retrievedDerivedDataCount { 0 };

    CyberCore::Timer m_bufferingTimer;
    RefPtr<NetworkCache::Cache> m_cache;
    CyberCore::SharedBufferBuilder m_bufferedDataForCache;
    std::unique_ptr<NetworkCache::Entry> m_cacheEntryForValidation;
    std::unique_ptr<NetworkCache::Entry> m_cacheEntryForMaxAgeCapValidation;
    bool m_isWaitingContinueWillSendRequestForCachedRedirect { false };
    std::unique_ptr<NetworkCache::Entry> m_cacheEntryWaitingForContinueDidReceiveResponse;
    std::unique_ptr<NetworkLoadChecker> m_networkLoadChecker;
    bool m_shouldRestartLoad { false };
    ResponseCompletionHandler m_responseCompletionHandler;
    bool m_shouldCaptureExtraNetworkLoadMetrics { false };
    bool m_isKeptAlive { false };
    std::unique_ptr<EarlyHintsResourceLoader> m_earlyHintsResourceLoader;

    std::optional<NetworkActivityTracker> m_networkActivityTracker;
#if ENABLE(SERVICE_WORKER)
    std::unique_ptr<ServiceWorkerFetchTask> m_serviceWorkerFetchTask;
    String m_resultingClientIdentifier;
    WeakPtr<CyberCore::SWServerRegistration> m_serviceWorkerRegistration;
    MonotonicTime m_workerStart;
#endif
    NetworkResourceLoadIdentifier m_resourceLoadID;
    CyberCore::ResourceResponse m_redirectResponse;
    URL m_firstResponseURL; // First URL in response's URL list (https://fetch.spec.whatwg.org/#concept-response-url-list).
    std::optional<CyberCore::CrossOriginOpenerPolicyEnforcementResult> m_currentCoopEnforcementResult;

#if ENABLE(CONTENT_FILTERING_IN_NETWORKING_PROCESS)
    std::unique_ptr<CyberCore::ContentFilter> m_contentFilter;
    CyberCore::ContentFilterUnblockHandler m_unblockHandler;
    String m_unblockRequestDeniedScript;
#endif

    PrivateRelayed m_privateRelayed { PrivateRelayed::No };
    MemoryCompactRobinHoodHashMap<String, String> m_reportingEndpoints;
};

} // namespace CyberKit
