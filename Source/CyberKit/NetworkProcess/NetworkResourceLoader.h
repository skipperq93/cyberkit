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
#include "NetworkConnectionToWebProcessMessages.h"
#include "NetworkLoadClient.h"
#include "NetworkResourceLoadParameters.h"
#include <CyberCore/AdClickAttribution.h>
#include <CyberCore/ContentSecurityPolicyClient.h>
#include <CyberCore/ResourceResponse.h>
#include <CyberCore/SecurityPolicyViolationEvent.h>
#include <CyberCore/Timer.h>
#include <wtf/WeakPtr.h>

namespace CyberCore {
class BlobDataFileReference;
class FormData;
class NetworkStorageSession;
class ResourceRequest;
}

namespace CyberKit {

class NetworkConnectionToWebProcess;
class NetworkLoad;
class NetworkLoadChecker;

namespace NetworkCache {
class Entry;
}

class NetworkResourceLoader final
    : public RefCounted<NetworkResourceLoader>
    , public NetworkLoadClient
    , public IPC::MessageSender
    , public CyberCore::ContentSecurityPolicyClient
    , public CanMakeWeakPtr<NetworkResourceLoader> {
public:
    static Ref<NetworkResourceLoader> create(NetworkResourceLoadParameters&& parameters, NetworkConnectionToWebProcess& connection, Messages::NetworkConnectionToWebProcess::PerformSynchronousLoad::DelayedReply&& reply = nullptr)
    {
        return adoptRef(*new NetworkResourceLoader(WTFMove(parameters), connection, WTFMove(reply)));
    }
    virtual ~NetworkResourceLoader();

    const CyberCore::ResourceRequest& originalRequest() const { return m_parameters.request; }

    NetworkLoad* networkLoad() const { return m_networkLoad.get(); }

    void start();
    void abort();

    // Message handlers.
    void didReceiveNetworkResourceLoaderMessage(IPC::Connection&, IPC::Decoder&);

    void continueWillSendRequest(CyberCore::ResourceRequest&& newRequest, bool isAllowedToAskUserForCredentials);

    const CyberCore::ResourceResponse& response() const { return m_response; }

    NetworkConnectionToWebProcess& connectionToWebProcess() const { return m_connection; }
    PAL::SessionID sessionID() const { return m_parameters.sessionID; }
    ResourceLoadIdentifier identifier() const { return m_parameters.identifier; }
    uint64_t frameID() const { return m_parameters.webFrameID; }
    CyberCore::PageIdentifier pageID() const { return m_parameters.webPageID; }

    struct SynchronousLoadData;

    // NetworkLoadClient.
    void didSendData(unsigned long long bytesSent, unsigned long long totalBytesToBeSent) override;
    bool isSynchronous() const override;
    bool isAllowedToAskUserForCredentials() const override { return m_isAllowedToAskUserForCredentials; }
    void willSendRedirectedRequest(CyberCore::ResourceRequest&&, CyberCore::ResourceRequest&& redirectRequest, CyberCore::ResourceResponse&&) override;
    void didReceiveResponse(CyberCore::ResourceResponse&&, ResponseCompletionHandler&&) override;
    void didReceiveBuffer(Ref<CyberCore::SharedBuffer>&&, int reportedEncodedDataLength) override;
    void didFinishLoading(const CyberCore::NetworkLoadMetrics&) override;
    void didFailLoading(const CyberCore::ResourceError&) override;
    void didBlockAuthenticationChallenge() override;
    bool shouldCaptureExtraNetworkLoadMetrics() const override;

    void convertToDownload(DownloadID, const CyberCore::ResourceRequest&, const CyberCore::ResourceResponse&);

    bool isMainResource() const { return m_parameters.request.requester() == CyberCore::ResourceRequest::Requester::Main; }
    bool isMainFrameLoad() const { return isMainResource() && m_parameters.frameAncestorOrigins.isEmpty(); }
    bool isCrossOriginPrefetch() const;

    bool isAlwaysOnLoggingAllowed() const;

#if ENABLE(RESOURCE_LOAD_STATISTICS) && !RELEASE_LOG_DISABLED
    static bool shouldLogCookieInformation(NetworkConnectionToWebProcess&, const PAL::SessionID&);
    static void logCookieInformation(NetworkConnectionToWebProcess&, const String& label, const void* loggedObject, const CyberCore::NetworkStorageSession&, const URL& firstParty, const CyberCore::SameSiteInfo&, const URL&, const String& referrer, Optional<uint64_t> frameID, Optional<CyberCore::PageIdentifier>, Optional<uint64_t> identifier);
#endif

    void disableExtraNetworkLoadMetricsCapture() { m_shouldCaptureExtraNetworkLoadMetrics = false; }

    bool isKeptAlive() const { return m_isKeptAlive; }

private:
    NetworkResourceLoader(NetworkResourceLoadParameters&&, NetworkConnectionToWebProcess&, Messages::NetworkConnectionToWebProcess::PerformSynchronousLoad::DelayedReply&&);

    // IPC::MessageSender
    IPC::Connection* messageSenderConnection() const override;
    uint64_t messageSenderDestinationID() const override { return m_parameters.identifier; }

    bool canUseCache(const CyberCore::ResourceRequest&) const;
    bool canUseCachedRedirect(const CyberCore::ResourceRequest&) const;

    void tryStoreAsCacheEntry();
    void retrieveCacheEntry(const CyberCore::ResourceRequest&);
    void didRetrieveCacheEntry(std::unique_ptr<NetworkCache::Entry>);
    void sendResultForCacheEntry(std::unique_ptr<NetworkCache::Entry>);
    void validateCacheEntry(std::unique_ptr<NetworkCache::Entry>);
    void dispatchWillSendRequestForCacheEntry(CyberCore::ResourceRequest&&, std::unique_ptr<NetworkCache::Entry>&&);

    bool shouldInterruptLoadForXFrameOptions(const String&, const URL&);
    bool shouldInterruptLoadForCSPFrameAncestorsOrXFrameOptions(const CyberCore::ResourceResponse&);

    enum class FirstLoad { No, Yes };
    void startNetworkLoad(CyberCore::ResourceRequest&&, FirstLoad);
    void restartNetworkLoad(CyberCore::ResourceRequest&&);
    void continueDidReceiveResponse();

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
    void sendBuffer(CyberCore::SharedBuffer&, size_t encodedDataLength);

    void consumeSandboxExtensions();
    void invalidateSandboxExtensions();

#if ENABLE(RESOURCE_LOAD_STATISTICS) && !RELEASE_LOG_DISABLED
    void logCookieInformation() const;
#endif

    void continueWillSendRedirectedRequest(CyberCore::ResourceRequest&&, CyberCore::ResourceRequest&& redirectRequest, CyberCore::ResourceResponse&&, Optional<CyberCore::AdClickAttribution::Conversion>&&);
    void didFinishWithRedirectResponse(CyberCore::ResourceRequest&&, CyberCore::ResourceRequest&& redirectRequest, CyberCore::ResourceResponse&&);
    CyberCore::ResourceResponse sanitizeResponseIfPossible(CyberCore::ResourceResponse&&, CyberCore::ResourceResponse::SanitizationType);

    // ContentSecurityPolicyClient
    void addConsoleMessage(MessageSource, MessageLevel, const String&, unsigned long) final;
    void sendCSPViolationReport(URL&&, Ref<CyberCore::FormData>&&) final;
    void enqueueSecurityPolicyViolationEvent(CyberCore::SecurityPolicyViolationEvent::Init&&) final;

    void logSlowCacheRetrieveIfNeeded(const NetworkCache::Cache::RetrieveInfo&);

    void handleAdClickAttributionConversion(CyberCore::AdClickAttribution::Conversion&&, const URL&, const CyberCore::ResourceRequest&);

    Optional<Seconds> validateCacheEntryForMaxAgeCapValidation(const CyberCore::ResourceRequest&, const CyberCore::ResourceRequest& redirectRequest, const CyberCore::ResourceResponse&);

    const NetworkResourceLoadParameters m_parameters;

    Ref<NetworkConnectionToWebProcess> m_connection;

    std::unique_ptr<NetworkLoad> m_networkLoad;

    CyberCore::ResourceResponse m_response;

    size_t m_bufferedDataEncodedDataLength { 0 };
    RefPtr<CyberCore::SharedBuffer> m_bufferedData;
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
    RefPtr<CyberCore::SharedBuffer> m_bufferedDataForCache;
    std::unique_ptr<NetworkCache::Entry> m_cacheEntryForValidation;
    std::unique_ptr<NetworkCache::Entry> m_cacheEntryForMaxAgeCapValidation;
    bool m_isWaitingContinueWillSendRequestForCachedRedirect { false };
    std::unique_ptr<NetworkCache::Entry> m_cacheEntryWaitingForContinueDidReceiveResponse;
    std::unique_ptr<NetworkLoadChecker> m_networkLoadChecker;
    bool m_shouldRestartLoad { false };
    ResponseCompletionHandler m_responseCompletionHandler;
    bool m_shouldCaptureExtraNetworkLoadMetrics { false };
    bool m_isKeptAlive { false };

    Optional<NetworkActivityTracker> m_networkActivityTracker;
};

} // namespace CyberKit
