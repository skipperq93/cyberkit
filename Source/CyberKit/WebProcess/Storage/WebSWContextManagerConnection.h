/*
 * Copyright (C) 2017-2022 Apple Inc. All rights reserved.
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

#if ENABLE(SERVICE_WORKER)

#include "Connection.h"
#include "DataReference.h"
#include "IdentifierTypes.h"
#include "MessageReceiver.h"
#include "NavigatingToAppBoundDomain.h"
#include "ShareableResource.h"
#include "UserContentControllerIdentifier.h"
#include "WebPageProxyIdentifier.h"
#include "WebPreferencesStore.h"
#include "WorkQueueMessageReceiver.h"
#include <CyberCore/SWContextManager.h>
#include <CyberCore/ServiceWorkerClientData.h>
#include <CyberCore/ServiceWorkerTypes.h>
#include <wtf/URLHash.h>

namespace IPC {
class FormDataReference;
}

namespace CyberCore {
struct FetchOptions;
class ResourceRequest;
struct ServiceWorkerContextData;
enum class WorkerThreadMode : bool;
}

namespace CyberKit {

class RemoteWorkerFrameLoaderClient;
class WebUserContentController;
struct RemoteWorkerInitializationData;

class WebSWContextManagerConnection final : public CyberCore::SWContextManager::Connection, public IPC::WorkQueueMessageReceiver {
public:
    static Ref<WebSWContextManagerConnection> create(Ref<IPC::Connection>&& connection, CyberCore::RegistrableDomain&& registrableDomain, std::optional<CyberCore::ScriptExecutionContextIdentifier> serviceWorkerPageIdentifier, PageGroupIdentifier pageGroupID, WebPageProxyIdentifier webPageProxyID, CyberCore::PageIdentifier pageID, const WebPreferencesStore& store, RemoteWorkerInitializationData&& initializationData) { return adoptRef(*new WebSWContextManagerConnection(WTFMove(connection), WTFMove(registrableDomain), serviceWorkerPageIdentifier, pageGroupID, webPageProxyID, pageID, store, WTFMove(initializationData))); }
    ~WebSWContextManagerConnection();

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

    CyberCore::PageIdentifier pageIdentifier() const final { return m_pageID; }

    void ref() const final { IPC::WorkQueueMessageReceiver::ref(); }
    void deref() const final { IPC::WorkQueueMessageReceiver ::deref(); }

private:
    WebSWContextManagerConnection(Ref<IPC::Connection>&&, CyberCore::RegistrableDomain&&, std::optional<CyberCore::ScriptExecutionContextIdentifier> serviceWorkerPageIdentifier, PageGroupIdentifier, WebPageProxyIdentifier, CyberCore::PageIdentifier, const WebPreferencesStore&, RemoteWorkerInitializationData&&);

    // CyberCore::SWContextManager::Connection.
    void establishConnection(CompletionHandler<void()>&&) final;
    void postMessageToServiceWorkerClient(const CyberCore::ScriptExecutionContextIdentifier& destinationIdentifier, const CyberCore::MessageWithMessagePorts&, CyberCore::ServiceWorkerIdentifier sourceIdentifier, const String& sourceOrigin) final;
    void didFinishInstall(std::optional<CyberCore::ServiceWorkerJobDataIdentifier>, CyberCore::ServiceWorkerIdentifier, bool wasSuccessful) final;
    void didFinishActivation(CyberCore::ServiceWorkerIdentifier) final;
    void setServiceWorkerHasPendingEvents(CyberCore::ServiceWorkerIdentifier, bool) final;
    void workerTerminated(CyberCore::ServiceWorkerIdentifier) final;
    void findClientByVisibleIdentifier(CyberCore::ServiceWorkerIdentifier, const String&, FindClientByIdentifierCallback&&) final;
    void matchAll(CyberCore::ServiceWorkerIdentifier, const CyberCore::ServiceWorkerClientQueryOptions&, CyberCore::ServiceWorkerClientsMatchAllCallback&&) final;
    void claim(CyberCore::ServiceWorkerIdentifier, CompletionHandler<void(CyberCore::ExceptionOr<void>&&)>&&) final;
    void focus(CyberCore::ScriptExecutionContextIdentifier, CompletionHandler<void(std::optional<CyberCore::ServiceWorkerClientData>&&)>&&) final;
    void navigate(CyberCore::ScriptExecutionContextIdentifier, CyberCore::ServiceWorkerIdentifier, const URL&, NavigateCallback&&) final;
    void skipWaiting(CyberCore::ServiceWorkerIdentifier, CompletionHandler<void()>&&) final;
    void setScriptResource(CyberCore::ServiceWorkerIdentifier, const URL&, const CyberCore::ServiceWorkerContextData::ImportedScript&) final;
    bool isThrottleable() const final;
    void didFailHeartBeatCheck(CyberCore::ServiceWorkerIdentifier) final;
    void setAsInspected(CyberCore::ServiceWorkerIdentifier, bool) final;
    void openWindow(CyberCore::ServiceWorkerIdentifier, const URL&, OpenWindowCallback&&) final;
    void stop() final;

    // IPC messages.
    void updatePreferencesStore(WebPreferencesStore&&);
    void serviceWorkerStarted(std::optional<CyberCore::ServiceWorkerJobDataIdentifier>, CyberCore::ServiceWorkerIdentifier, bool doesHandleFetch) final;
    void serviceWorkerFailedToStart(std::optional<CyberCore::ServiceWorkerJobDataIdentifier>, CyberCore::ServiceWorkerIdentifier, const String& exceptionMessage) final;
    void installServiceWorker(CyberCore::ServiceWorkerContextData&&, CyberCore::ServiceWorkerData&&, String&& userAgent, CyberCore::WorkerThreadMode);
    void updateAppInitiatedValue(CyberCore::ServiceWorkerIdentifier, CyberCore::LastNavigationWasAppInitiated);
    void startFetch(CyberCore::SWServerConnectionIdentifier, CyberCore::ServiceWorkerIdentifier, CyberCore::FetchIdentifier, CyberCore::ResourceRequest&&, CyberCore::FetchOptions&&, IPC::FormDataReference&&, String&& referrer, bool isServiceWorkerNavigationPreloadEnabled, String&& clientIdentifier, String&& resultingClientIdentifier);
    void cancelFetch(CyberCore::SWServerConnectionIdentifier, CyberCore::ServiceWorkerIdentifier, CyberCore::FetchIdentifier);
    void continueDidReceiveFetchResponse(CyberCore::SWServerConnectionIdentifier, CyberCore::ServiceWorkerIdentifier, CyberCore::FetchIdentifier);
    void postMessageToServiceWorker(CyberCore::ServiceWorkerIdentifier destinationIdentifier, CyberCore::MessageWithMessagePorts&&, CyberCore::ServiceWorkerOrClientData&& sourceData);
    void fireInstallEvent(CyberCore::ServiceWorkerIdentifier);
    void fireActivateEvent(CyberCore::ServiceWorkerIdentifier);
    void firePushEvent(CyberCore::ServiceWorkerIdentifier, const std::optional<IPC::DataReference>&, CompletionHandler<void(bool)>&&);
    void fireNotificationEvent(CyberCore::ServiceWorkerIdentifier, CyberCore::NotificationData&&, CyberCore::NotificationEventType, CompletionHandler<void(bool)>&&);
    void terminateWorker(CyberCore::ServiceWorkerIdentifier);
#if ENABLE(SHAREABLE_RESOURCE) && PLATFORM(COCOA)
    void didSaveScriptsToDisk(CyberCore::ServiceWorkerIdentifier, CyberCore::ScriptBuffer&&, HashMap<URL, CyberCore::ScriptBuffer>&& importedScripts);
#endif
    void matchAllCompleted(uint64_t matchAllRequestIdentifier, Vector<CyberCore::ServiceWorkerClientData>&&);
    void skipWaitingCompleted(uint64_t matchAllRequestIdentifier);
    void setUserAgent(String&& userAgent);
    void close();
    void setThrottleState(bool isThrottleable);
    void convertFetchToDownload(CyberCore::SWServerConnectionIdentifier, CyberCore::ServiceWorkerIdentifier, CyberCore::FetchIdentifier);
    void cancelFetchDownload(CyberCore::ServiceWorkerIdentifier, CyberCore::FetchIdentifier);
    void navigationPreloadIsReady(CyberCore::SWServerConnectionIdentifier, CyberCore::ServiceWorkerIdentifier, CyberCore::FetchIdentifier, CyberCore::ResourceResponse&&);
    void navigationPreloadFailed(CyberCore::SWServerConnectionIdentifier, CyberCore::ServiceWorkerIdentifier, CyberCore::FetchIdentifier, CyberCore::ResourceError&&);

    Ref<IPC::Connection> m_connectionToNetworkProcess;
    CyberCore::RegistrableDomain m_registrableDomain;
    std::optional<CyberCore::ScriptExecutionContextIdentifier> m_serviceWorkerPageIdentifier;
    PageGroupIdentifier m_pageGroupID;
    WebPageProxyIdentifier m_webPageProxyID;
    CyberCore::PageIdentifier m_pageID;

    HashSet<std::unique_ptr<RemoteWorkerFrameLoaderClient>> m_loaders;
    HashMap<uint64_t, CyberCore::ServiceWorkerClientsMatchAllCallback> m_matchAllRequests;
    HashMap<uint64_t, Function<void()>> m_skipWaitingCallbacks;
    uint64_t m_previousRequestIdentifier { 0 };
    String m_userAgent;
    bool m_isThrottleable { true };
    Ref<WebUserContentController> m_userContentController;
    std::optional<WebPreferencesStore> m_preferencesStore;
    Ref<WorkQueue> m_queue;
};

} // namespace CyberKit

#endif // ENABLE(SERVICE_WORKER)
