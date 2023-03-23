/*
 * Copyright (C) 2012-2019 Apple Inc. All rights reserved.
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

#include "CacheStorageEngineConnection.h"
#include "Connection.h"
#include "DownloadID.h"
#include "NetworkActivityTracker.h"
#include "NetworkMDNSRegister.h"
#include "NetworkRTCProvider.h"
#include "NetworkResourceLoadIdentifier.h"
#include "NetworkResourceLoadMap.h"
#include "PolicyDecision.h"
#include "SandboxExtension.h"
#include "WebPageProxyIdentifier.h"
#include "WebPaymentCoordinatorProxy.h"
#include "WebResourceLoadObserver.h"
#include <CyberScriptCore/ConsoleTypes.h>
#include <CyberCore/FrameIdentifier.h>
#include <CyberCore/LayoutMilestone.h>
#include <CyberCore/LoadSchedulingMode.h>
#include <CyberCore/MessagePortChannelProvider.h>
#include <CyberCore/MessagePortIdentifier.h>
#include <CyberCore/NetworkLoadInformation.h>
#include <CyberCore/NetworkStorageSession.h>
#include <CyberCore/PageIdentifier.h>
#include <CyberCore/ProcessIdentifier.h>
#include <CyberCore/RTCDataChannelIdentifier.h>
#include <CyberCore/RegistrableDomain.h>
#include <CyberCore/WebSocketIdentifier.h>
#include <wtf/OptionSet.h>
#include <wtf/RefCounted.h>
#include <wtf/URLHash.h>

#if ENABLE(IPC_TESTING_API)
#include "IPCTester.h"
#endif

namespace PAL {
class SessionID;
}

namespace CyberCore {
class BlobDataFileReference;
class BlobPart;
class BlobRegistryImpl;
class MockContentFilterSettings;
enum class NetworkConnectionIntegrity : uint16_t;
class ResourceError;
class ResourceRequest;
enum class ApplyTrackingPrevention : bool;
enum class StorageAccessScope : bool;
struct ClientOrigin;
struct PolicyContainer;
struct RequestStorageAccessResult;
struct SameSiteInfo;

enum class HTTPCookieAcceptPolicy : uint8_t;
enum class IncludeSecureCookies : bool;
}

namespace CyberKit {

class NetworkSchemeRegistry;
class NetworkProcess;
class NetworkResourceLoader;
class NetworkResourceLoadParameters;
class NetworkSession;
class NetworkSocketChannel;
class NetworkSocketStream;
class ServiceWorkerFetchTask;
class WebSWServerConnection;
class WebSWServerToContextConnection;
class WebSharedWorkerServerConnection;
class WebSharedWorkerServerToContextConnection;

enum class PrivateRelayed : bool;

namespace NetworkCache {
struct DataKey;
}

class NetworkConnectionToWebProcess
    : public RefCounted<NetworkConnectionToWebProcess>
#if ENABLE(APPLE_PAY_REMOTE_UI)
    , public WebPaymentCoordinatorProxy::Client
#endif
#if HAVE(COOKIE_CHANGE_LISTENER_API)
    , public CyberCore::CookieChangeObserver
#endif
    , IPC::Connection::Client {
public:
    using RegistrableDomain = CyberCore::RegistrableDomain;

    static Ref<NetworkConnectionToWebProcess> create(NetworkProcess&, CyberCore::ProcessIdentifier, PAL::SessionID, IPC::Connection::Identifier);
    virtual ~NetworkConnectionToWebProcess();
    
    PAL::SessionID sessionID() const { return m_sessionID; }
    NetworkSession* networkSession();

    IPC::Connection& connection() { return m_connection.get(); }
    NetworkProcess& networkProcess() { return m_networkProcess.get(); }

    void didCleanupResourceLoader(NetworkResourceLoader&);
    void transferKeptAliveLoad(NetworkResourceLoader&);
    void setOnLineState(bool);

    bool captureExtraNetworkLoadMetricsEnabled() const { return m_captureExtraNetworkLoadMetricsEnabled; }

    RefPtr<CyberCore::BlobDataFileReference> getBlobDataFileReferenceForPath(const String& path);

    void endSuspension();

    void getNetworkLoadInformationResponse(CyberCore::ResourceLoaderIdentifier identifier, CompletionHandler<void(const CyberCore::ResourceResponse&)>&& completionHandler)
    {
        if (auto* info = m_networkLoadInformationByID.get(identifier))
            return completionHandler(info->response);
        completionHandler({ });
    }

    void getNetworkLoadIntermediateInformation(CyberCore::ResourceLoaderIdentifier identifier, CompletionHandler<void(const Vector<CyberCore::NetworkTransactionInformation>&)>&& completionHandler)
    {
        if (auto* info = m_networkLoadInformationByID.get(identifier))
            return completionHandler(info->transactions);
        completionHandler({ });
    }

    void takeNetworkLoadInformationMetrics(CyberCore::ResourceLoaderIdentifier identifier, CompletionHandler<void(const CyberCore::NetworkLoadMetrics&)>&& completionHandler)
    {
        if (auto info = m_networkLoadInformationByID.take(identifier))
            return completionHandler(info->metrics);
        completionHandler({ });
    }

    void addNetworkLoadInformation(CyberCore::ResourceLoaderIdentifier identifier, CyberCore::NetworkLoadInformation&& information)
    {
        ASSERT(!m_networkLoadInformationByID.contains(identifier));
        m_networkLoadInformationByID.add(identifier, makeUnique<CyberCore::NetworkLoadInformation>(WTFMove(information)));
    }

    void addNetworkLoadInformationMetrics(CyberCore::ResourceLoaderIdentifier identifier, const CyberCore::NetworkLoadMetrics& metrics)
    {
        ASSERT(m_networkLoadInformationByID.contains(identifier));
        m_networkLoadInformationByID.ensure(identifier, [] {
            return makeUnique<CyberCore::NetworkLoadInformation>();
        }).iterator->value->metrics = metrics;
    }

    void removeNetworkLoadInformation(CyberCore::ResourceLoaderIdentifier identifier)
    {
        m_networkLoadInformationByID.remove(identifier);
    }

    std::optional<NetworkActivityTracker> startTrackingResourceLoad(CyberCore::PageIdentifier, CyberCore::ResourceLoaderIdentifier resourceID, bool isTopResource);
    void stopTrackingResourceLoad(CyberCore::ResourceLoaderIdentifier resourceID, NetworkActivityTracker::CompletionCode);

    Vector<RefPtr<CyberCore::BlobDataFileReference>> resolveBlobReferences(const NetworkResourceLoadParameters&);

    void removeSocketChannel(CyberCore::WebSocketIdentifier);

    CyberCore::ProcessIdentifier webProcessIdentifier() const { return m_webProcessIdentifier; }

#if ENABLE(SERVICE_WORKER)
    void serviceWorkerServerToContextConnectionNoLongerNeeded();
    WebSWServerConnection* swConnection();
    std::unique_ptr<ServiceWorkerFetchTask> createFetchTask(NetworkResourceLoader&, const CyberCore::ResourceRequest&);
#endif
    void sharedWorkerServerToContextConnectionIsNoLongerNeeded();

    WebSharedWorkerServerConnection* sharedWorkerConnection();

    NetworkSchemeRegistry& schemeRegistry() { return m_schemeRegistry.get(); }

    void cookieAcceptPolicyChanged(CyberCore::HTTPCookieAcceptPolicy);

    void broadcastConsoleMessage(JSC::MessageSource, JSC::MessageLevel, const String& message);
    RefPtr<NetworkResourceLoader> takeNetworkResourceLoader(CyberCore::ResourceLoaderIdentifier);

#if ENABLE(CONTENT_FILTERING_IN_NETWORKING_PROCESS)
    void installMockContentFilter(CyberCore::MockContentFilterSettings&&);
#endif

private:
    NetworkConnectionToWebProcess(NetworkProcess&, CyberCore::ProcessIdentifier, PAL::SessionID, IPC::Connection::Identifier);

    void didFinishPreconnection(CyberCore::ResourceLoaderIdentifier preconnectionIdentifier, const CyberCore::ResourceError&);
    CyberCore::NetworkStorageSession* storageSession();

    // IPC::Connection::Client
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;
    bool didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, UniqueRef<IPC::Encoder>&) override;
    void didClose(IPC::Connection&) override;
    void didReceiveInvalidMessage(IPC::Connection&, IPC::MessageName) override;

    // Message handlers.
    void didReceiveNetworkConnectionToWebProcessMessage(IPC::Connection&, IPC::Decoder&);
    bool didReceiveSyncNetworkConnectionToWebProcessMessage(IPC::Connection&, IPC::Decoder&, UniqueRef<IPC::Encoder>&);

    void scheduleResourceLoad(NetworkResourceLoadParameters&&, std::optional<NetworkResourceLoadIdentifier> existingLoaderToResume);
    void performSynchronousLoad(NetworkResourceLoadParameters&&, CompletionHandler<void(const CyberCore::ResourceError&, const CyberCore::ResourceResponse, Vector<uint8_t>&&)>&&);
    void testProcessIncomingSyncMessagesWhenWaitingForSyncReply(WebPageProxyIdentifier, CompletionHandler<void(bool)>&&);
    void loadPing(NetworkResourceLoadParameters&&);
    void prefetchDNS(const String&);
    void sendH2Ping(NetworkResourceLoadParameters&&, CompletionHandler<void(Expected<WTF::Seconds, CyberCore::ResourceError>&&)>&&);
    void preconnectTo(std::optional<CyberCore::ResourceLoaderIdentifier> preconnectionIdentifier, NetworkResourceLoadParameters&&);
    void isResourceLoadFinished(CyberCore::ResourceLoaderIdentifier, CompletionHandler<void(bool)>&&);

    void removeLoadIdentifier(CyberCore::ResourceLoaderIdentifier);
    void pageLoadCompleted(CyberCore::PageIdentifier);
    void browsingContextRemoved(WebPageProxyIdentifier, CyberCore::PageIdentifier, CyberCore::FrameIdentifier);
    void crossOriginRedirectReceived(CyberCore::ResourceLoaderIdentifier, const URL& redirectURL);
    void startDownload(DownloadID, const CyberCore::ResourceRequest&, std::optional<NavigatingToAppBoundDomain>, const String& suggestedName = { });
    void convertMainResourceLoadToDownload(std::optional<CyberCore::ResourceLoaderIdentifier> mainResourceLoadIdentifier, DownloadID, const CyberCore::ResourceRequest&, const CyberCore::ResourceResponse&, std::optional<NavigatingToAppBoundDomain>);

    void registerURLSchemesAsCORSEnabled(Vector<String>&& schemes);

    void cookiesForDOM(const URL& firstParty, const CyberCore::SameSiteInfo&, const URL&, CyberCore::FrameIdentifier, CyberCore::PageIdentifier, CyberCore::IncludeSecureCookies, CyberCore::ApplyTrackingPrevention, CyberCore::ShouldRelaxThirdPartyCookieBlocking, CompletionHandler<void(String cookieString, bool secureCookiesAccessed)>&&);
    void setCookiesFromDOM(const URL& firstParty, const CyberCore::SameSiteInfo&, const URL&, CyberCore::FrameIdentifier, CyberCore::PageIdentifier, CyberCore::ApplyTrackingPrevention, const String&, CyberCore::ShouldRelaxThirdPartyCookieBlocking);
    void cookieRequestHeaderFieldValue(const URL& firstParty, const CyberCore::SameSiteInfo&, const URL&, std::optional<CyberCore::FrameIdentifier>, std::optional<CyberCore::PageIdentifier>, CyberCore::IncludeSecureCookies, CyberCore::ApplyTrackingPrevention, CyberCore::ShouldRelaxThirdPartyCookieBlocking, CompletionHandler<void(String cookieString, bool secureCookiesAccessed)>&&);
    void getRawCookies(const URL& firstParty, const CyberCore::SameSiteInfo&, const URL&, std::optional<CyberCore::FrameIdentifier>, std::optional<CyberCore::PageIdentifier>, CyberCore::ApplyTrackingPrevention, CyberCore::ShouldRelaxThirdPartyCookieBlocking, CompletionHandler<void(Vector<CyberCore::Cookie>&&)>&&);
    void setRawCookie(const CyberCore::Cookie&);
    void deleteCookie(const URL&, const String& cookieName, CompletionHandler<void()>&&);

    void registerFileBlobURL(const URL&, const String& path, const String& replacementPath, SandboxExtension::Handle&&, const String& contentType);
    void registerBlobURL(const URL&, Vector<CyberCore::BlobPart>&&, const String& contentType);
    void registerBlobURLFromURL(const URL&, const URL& srcURL, CyberCore::PolicyContainer&&);
    void registerBlobURLOptionallyFileBacked(const URL&, const URL& srcURL, const String& fileBackedPath, const String& contentType);
    void registerBlobURLForSlice(const URL&, const URL& srcURL, int64_t start, int64_t end, const String& contentType);
    void blobSize(const URL&, CompletionHandler<void(uint64_t)>&&);
    void unregisterBlobURL(const URL&);
    void writeBlobsToTemporaryFilesForIndexedDB(const Vector<String>& blobURLs, CompletionHandler<void(Vector<String>&&)>&&);

    void registerBlobURLHandle(const URL&);
    void unregisterBlobURLHandle(const URL&);

    void setCaptureExtraNetworkLoadMetricsEnabled(bool);

    void createSocketStream(URL&&, String cachePartition, CyberCore::WebSocketIdentifier);

    void createSocketChannel(const CyberCore::ResourceRequest&, const String& protocol, CyberCore::WebSocketIdentifier, WebPageProxyIdentifier, const CyberCore::ClientOrigin&, bool hadMainFrameMainResourcePrivateRelayed, bool allowPrivacyProxy, OptionSet<CyberCore::NetworkConnectionIntegrity> networkConnectionIntegrityPolicy);
    void updateQuotaBasedOnSpaceUsageForTesting(CyberCore::ClientOrigin&&);

    void establishSharedWorkerServerConnection();
    void unregisterSharedWorkerConnection();

#if ENABLE(SERVICE_WORKER)
    void establishSWServerConnection();
    void establishSWContextConnection(WebPageProxyIdentifier, CyberCore::RegistrableDomain&&, std::optional<CyberCore::ScriptExecutionContextIdentifier> serviceWorkerPageIdentifier, CompletionHandler<void()>&&);
    void closeSWContextConnection();
    void unregisterSWConnection();
#endif

    void establishSharedWorkerContextConnection(WebPageProxyIdentifier, CyberCore::RegistrableDomain&&, CompletionHandler<void()>&&);
    void closeSharedWorkerContextConnection();

    void createRTCProvider(CompletionHandler<void()>&&);
#if ENABLE(WEB_RTC)
    void connectToRTCDataChannelRemoteSource(CyberCore::RTCDataChannelIdentifier source, CyberCore::RTCDataChannelIdentifier handler, CompletionHandler<void(std::optional<bool>)>&&);
#endif

    void createNewMessagePortChannel(const CyberCore::MessagePortIdentifier& port1, const CyberCore::MessagePortIdentifier& port2);
    void entangleLocalPortInThisProcessToRemote(const CyberCore::MessagePortIdentifier& local, const CyberCore::MessagePortIdentifier& remote);
    void messagePortDisentangled(const CyberCore::MessagePortIdentifier&);
    void messagePortClosed(const CyberCore::MessagePortIdentifier&);
    void takeAllMessagesForPort(const CyberCore::MessagePortIdentifier&, CompletionHandler<void(Vector<CyberCore::MessageWithMessagePorts>&&, uint64_t)>&&);
    void postMessageToRemote(CyberCore::MessageWithMessagePorts&&, const CyberCore::MessagePortIdentifier&);
    void didDeliverMessagePortMessages(uint64_t messageBatchIdentifier);

    void setCORSDisablingPatterns(CyberCore::PageIdentifier, Vector<String>&&);

#if PLATFORM(MAC)
    void updateActivePages(const String& name, const Vector<String>& activePagesOrigins, audit_token_t);
    void getProcessDisplayName(audit_token_t, CompletionHandler<void(const String&)>&&);
#endif

#if USE(LIBWEBRTC)
    NetworkRTCProvider& rtcProvider();
#endif
#if ENABLE(WEB_RTC)
    NetworkMDNSRegister& mdnsRegister() { return m_mdnsRegister; }
    void registerToRTCDataChannelProxy();
    void unregisterToRTCDataChannelProxy();
#endif

    CacheStorageEngineConnection& cacheStorageConnection();

    void clearPageSpecificData(CyberCore::PageIdentifier);

#if ENABLE(TRACKING_PREVENTION)
    void removeStorageAccessForFrame(CyberCore::FrameIdentifier, CyberCore::PageIdentifier);

    void logUserInteraction(RegistrableDomain&&);
    void resourceLoadStatisticsUpdated(Vector<CyberCore::ResourceLoadStatistics>&&, CompletionHandler<void()>&&);
    void hasStorageAccess(RegistrableDomain&& subFrameDomain, RegistrableDomain&& topFrameDomain, CyberCore::FrameIdentifier, CyberCore::PageIdentifier, CompletionHandler<void(bool)>&&);
    void requestStorageAccess(RegistrableDomain&& subFrameDomain, RegistrableDomain&& topFrameDomain, CyberCore::FrameIdentifier, CyberCore::PageIdentifier, WebPageProxyIdentifier, CyberCore::StorageAccessScope, CompletionHandler<void(CyberCore::RequestStorageAccessResult)>&&);
    void requestStorageAccessUnderOpener(CyberCore::RegistrableDomain&& domainInNeedOfStorageAccess, CyberCore::PageIdentifier openerPageID, CyberCore::RegistrableDomain&& openerDomain);
#endif

    void addOriginAccessAllowListEntry(const String& sourceOrigin, const String& destinationProtocol, const String& destinationHost, bool allowDestinationSubdomains);
    void removeOriginAccessAllowListEntry(const String& sourceOrigin, const String& destinationProtocol, const String& destinationHost, bool allowDestinationSubdomains);
    void resetOriginAccessAllowLists();

    uint64_t nextMessageBatchIdentifier(CompletionHandler<void()>&&);

    void domCookiesForHost(const URL& host, bool subscribeToCookieChangeNotifications, CompletionHandler<void(const Vector<CyberCore::Cookie>&)>&&);

#if HAVE(COOKIE_CHANGE_LISTENER_API)
    void unsubscribeFromCookieChangeNotifications(const HashSet<String>& hosts);

    // CyberCore::CookieChangeObserver.
    void cookiesAdded(const String& host, const Vector<CyberCore::Cookie>&) final;
    void cookiesDeleted(const String& host, const Vector<CyberCore::Cookie>&) final;
    void allCookiesDeleted() final;
#endif

    struct ResourceNetworkActivityTracker {
        ResourceNetworkActivityTracker() = default;
        ResourceNetworkActivityTracker(const ResourceNetworkActivityTracker&) = default;
        ResourceNetworkActivityTracker(ResourceNetworkActivityTracker&&) = default;
        ResourceNetworkActivityTracker(CyberCore::PageIdentifier pageID)
            : pageID { pageID }
            , isRootActivity { true }
            , networkActivity { NetworkActivityTracker::Label::LoadPage }
        {
        }

        ResourceNetworkActivityTracker(CyberCore::PageIdentifier pageID, CyberCore::ResourceLoaderIdentifier resourceID)
            : pageID { pageID }
            , resourceID { resourceID }
            , networkActivity { NetworkActivityTracker::Label::LoadResource }
        {
        }

        CyberCore::PageIdentifier pageID;
        CyberCore::ResourceLoaderIdentifier resourceID;
        bool isRootActivity { false };
        NetworkActivityTracker networkActivity;
    };

    void stopAllNetworkActivityTracking();
    void stopAllNetworkActivityTrackingForPage(CyberCore::PageIdentifier);
    size_t findRootNetworkActivity(CyberCore::PageIdentifier);
    size_t findNetworkActivityTracker(CyberCore::ResourceLoaderIdentifier resourceID);

    void hasUploadStateChanged(bool);

    void setResourceLoadSchedulingMode(CyberCore::PageIdentifier, CyberCore::LoadSchedulingMode);
    void prioritizeResourceLoads(const Vector<CyberCore::ResourceLoaderIdentifier>&);

#if ENABLE(APPLE_PAY_REMOTE_UI)
    WebPaymentCoordinatorProxy& paymentCoordinator();

    // WebPaymentCoordinatorProxy::Client
    IPC::Connection* paymentCoordinatorConnection(const WebPaymentCoordinatorProxy&) final;
    UIViewController *paymentCoordinatorPresentingViewController(const WebPaymentCoordinatorProxy&) final;
#if ENABLE(APPLE_PAY_REMOTE_UI_USES_SCENE)
    void getWindowSceneIdentifierForPaymentPresentation(WebPageProxyIdentifier, CompletionHandler<void(const String&)>&&) final;
#endif
    const String& paymentCoordinatorBoundInterfaceIdentifier(const WebPaymentCoordinatorProxy&) final;
    const String& paymentCoordinatorCTDataConnectionServiceType(const WebPaymentCoordinatorProxy&) final;
    const String& paymentCoordinatorSourceApplicationBundleIdentifier(const WebPaymentCoordinatorProxy&) final;
    const String& paymentCoordinatorSourceApplicationSecondaryIdentifier(const WebPaymentCoordinatorProxy&) final;
    std::unique_ptr<PaymentAuthorizationPresenter> paymentCoordinatorAuthorizationPresenter(WebPaymentCoordinatorProxy&, PKPaymentRequest *) final;
    void paymentCoordinatorAddMessageReceiver(WebPaymentCoordinatorProxy&, IPC::ReceiverName, IPC::MessageReceiver&) final;
    void paymentCoordinatorRemoveMessageReceiver(WebPaymentCoordinatorProxy&, IPC::ReceiverName) final;
#endif

    Ref<IPC::Connection> m_connection;
    Ref<NetworkProcess> m_networkProcess;
    PAL::SessionID m_sessionID;

    HashMap<CyberCore::WebSocketIdentifier, RefPtr<NetworkSocketStream>> m_networkSocketStreams;
    HashMap<CyberCore::WebSocketIdentifier, std::unique_ptr<NetworkSocketChannel>> m_networkSocketChannels;
    NetworkResourceLoadMap m_networkResourceLoaders;
    HashMap<String, RefPtr<CyberCore::BlobDataFileReference>> m_blobDataFileReferences;
    Vector<ResourceNetworkActivityTracker> m_networkActivityTrackers;

    HashMap<CyberCore::ResourceLoaderIdentifier, std::unique_ptr<CyberCore::NetworkLoadInformation>> m_networkLoadInformationByID;


#if USE(LIBWEBRTC)
    RefPtr<NetworkRTCProvider> m_rtcProvider;
#endif
#if ENABLE(WEB_RTC)
    NetworkMDNSRegister m_mdnsRegister;
#endif
#if HAVE(COOKIE_CHANGE_LISTENER_API)
    HashSet<String> m_hostsWithCookieListeners;
#endif

    bool m_captureExtraNetworkLoadMetricsEnabled { false };

    RefPtr<CacheStorageEngineConnection> m_cacheStorageConnection;

#if ENABLE(SERVICE_WORKER)
    WeakPtr<WebSWServerConnection> m_swConnection;
    std::unique_ptr<WebSWServerToContextConnection> m_swContextConnection;
#endif
    WeakPtr<WebSharedWorkerServerConnection> m_sharedWorkerConnection;
    std::unique_ptr<WebSharedWorkerServerToContextConnection> m_sharedWorkerContextConnection;

#if ENABLE(WEB_RTC)
    bool m_isRegisteredToRTCDataChannelProxy { false };
#endif

#if ENABLE(APPLE_PAY_REMOTE_UI)
    std::unique_ptr<WebPaymentCoordinatorProxy> m_paymentCoordinator;
#endif
    const CyberCore::ProcessIdentifier m_webProcessIdentifier;

    HashSet<CyberCore::MessagePortIdentifier> m_processEntangledPorts;
    HashMap<uint64_t, CompletionHandler<void()>> m_messageBatchDeliveryCompletionHandlers;
    Ref<NetworkSchemeRegistry> m_schemeRegistry;
        
    HashSet<URL> m_blobURLs;
    HashCountedSet<URL> m_blobURLHandles;
#if ENABLE(IPC_TESTING_API)
    IPCTester m_ipcTester;
#endif
};

} // namespace CyberKit
