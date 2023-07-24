/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
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
#include "MessageReceiver.h"
#include "UserContentControllerIdentifier.h"
#include "WebPageProxyIdentifier.h"
#include "WebSWContextManagerConnectionMessagesReplies.h"
#include <CyberCore/EmptyFrameLoaderClient.h>
#include <CyberCore/SWContextManager.h>
#include <CyberCore/ServiceWorkerClientData.h>
#include <CyberCore/ServiceWorkerTypes.h>

namespace IPC {
class FormDataReference;
}

namespace CyberCore {
struct FetchOptions;
class ResourceRequest;
struct ServiceWorkerContextData;
}

namespace CyberKit {

class ServiceWorkerFrameLoaderClient;
struct ServiceWorkerInitializationData;
struct WebPreferencesStore;
class WebUserContentController;

class WebSWContextManagerConnection final : public CyberCore::SWContextManager::Connection, public IPC::MessageReceiver {
public:
    WebSWContextManagerConnection(Ref<IPC::Connection>&&, CyberCore::RegistrableDomain&&, uint64_t pageGroupID, WebPageProxyIdentifier, CyberCore::PageIdentifier, const WebPreferencesStore&, ServiceWorkerInitializationData&&);
    ~WebSWContextManagerConnection();

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

private:
    void updatePreferencesStore(const WebPreferencesStore&);

    // CyberCore::SWContextManager::Connection.
    void establishConnection(CompletionHandler<void()>&&) final;
    void postMessageToServiceWorkerClient(const CyberCore::ServiceWorkerClientIdentifier& destinationIdentifier, const CyberCore::MessageWithMessagePorts&, CyberCore::ServiceWorkerIdentifier sourceIdentifier, const String& sourceOrigin) final;
    void didFinishInstall(Optional<CyberCore::ServiceWorkerJobDataIdentifier>, CyberCore::ServiceWorkerIdentifier, bool wasSuccessful) final;
    void didFinishActivation(CyberCore::ServiceWorkerIdentifier) final;
    void setServiceWorkerHasPendingEvents(CyberCore::ServiceWorkerIdentifier, bool) final;
    void workerTerminated(CyberCore::ServiceWorkerIdentifier) final;
    void findClientByIdentifier(CyberCore::ServiceWorkerIdentifier, CyberCore::ServiceWorkerClientIdentifier, FindClientByIdentifierCallback&&) final;
    void matchAll(CyberCore::ServiceWorkerIdentifier, const CyberCore::ServiceWorkerClientQueryOptions&, CyberCore::ServiceWorkerClientsMatchAllCallback&&) final;
    void claim(CyberCore::ServiceWorkerIdentifier, CompletionHandler<void(CyberCore::ExceptionOr<void>&&)>&&) final;
    void skipWaiting(CyberCore::ServiceWorkerIdentifier, CompletionHandler<void()>&&) final;
    void setScriptResource(CyberCore::ServiceWorkerIdentifier, const URL&, const CyberCore::ServiceWorkerContextData::ImportedScript&) final;
    bool isThrottleable() const final;
    void didFailHeartBeatCheck(CyberCore::ServiceWorkerIdentifier) final;

    // IPC messages.
    void serviceWorkerStarted(Optional<CyberCore::ServiceWorkerJobDataIdentifier>, CyberCore::ServiceWorkerIdentifier, bool doesHandleFetch) final;
    void serviceWorkerFailedToStart(Optional<CyberCore::ServiceWorkerJobDataIdentifier>, CyberCore::ServiceWorkerIdentifier, const String& exceptionMessage) final;
    void installServiceWorker(const CyberCore::ServiceWorkerContextData&, String&& userAgent);
    void startFetch(CyberCore::SWServerConnectionIdentifier, CyberCore::ServiceWorkerIdentifier, CyberCore::FetchIdentifier, CyberCore::ResourceRequest&&, CyberCore::FetchOptions&&, IPC::FormDataReference&&, String&& referrer);
    void cancelFetch(CyberCore::SWServerConnectionIdentifier, CyberCore::ServiceWorkerIdentifier, CyberCore::FetchIdentifier);
    void continueDidReceiveFetchResponse(CyberCore::SWServerConnectionIdentifier, CyberCore::ServiceWorkerIdentifier, CyberCore::FetchIdentifier);
    void postMessageToServiceWorker(CyberCore::ServiceWorkerIdentifier destinationIdentifier, CyberCore::MessageWithMessagePorts&&, CyberCore::ServiceWorkerOrClientData&& sourceData);
    void fireInstallEvent(CyberCore::ServiceWorkerIdentifier);
    void fireActivateEvent(CyberCore::ServiceWorkerIdentifier);
    void terminateWorker(CyberCore::ServiceWorkerIdentifier);
    void findClientByIdentifierCompleted(uint64_t requestIdentifier, Optional<CyberCore::ServiceWorkerClientData>&&, bool hasSecurityError);
    void matchAllCompleted(uint64_t matchAllRequestIdentifier, Vector<CyberCore::ServiceWorkerClientData>&&);
    void setUserAgent(String&& userAgent);
    void close();
    void setThrottleState(bool isThrottleable);

    Ref<IPC::Connection> m_connectionToNetworkProcess;
    CyberCore::RegistrableDomain m_registrableDomain;
    uint64_t m_pageGroupID;
    WebPageProxyIdentifier m_webPageProxyID;
    CyberCore::PageIdentifier m_pageID;

    CyberCore::SecurityOrigin::StorageBlockingPolicy m_storageBlockingPolicy { CyberCore::SecurityOrigin::StorageBlockingPolicy::AllowAllStorage };

    HashSet<std::unique_ptr<ServiceWorkerFrameLoaderClient>> m_loaders;
    HashMap<uint64_t, FindClientByIdentifierCallback> m_findClientByIdentifierRequests;
    HashMap<uint64_t, CyberCore::ServiceWorkerClientsMatchAllCallback> m_matchAllRequests;
    uint64_t m_previousRequestIdentifier { 0 };
    String m_userAgent;
    bool m_isThrottleable { true };
    RefPtr<WebUserContentController> m_userContentController;
};

class ServiceWorkerFrameLoaderClient final : public CyberCore::EmptyFrameLoaderClient {
public:
    ServiceWorkerFrameLoaderClient(WebPageProxyIdentifier, CyberCore::PageIdentifier, CyberCore::FrameIdentifier, const String& userAgent);

    WebPageProxyIdentifier webPageProxyID() const { return m_webPageProxyID; }

    void setUserAgent(String&& userAgent) { m_userAgent = WTFMove(userAgent); }

private:
    Ref<CyberCore::DocumentLoader> createDocumentLoader(const CyberCore::ResourceRequest&, const CyberCore::SubstituteData&) final;

    Optional<CyberCore::PageIdentifier> pageID() const final { return m_pageID; }
    Optional<CyberCore::FrameIdentifier> frameID() const final { return m_frameID; }

    bool shouldUseCredentialStorage(CyberCore::DocumentLoader*, unsigned long) final { return true; }
    bool isServiceWorkerFrameLoaderClient() const final { return true; }

    String userAgent(const URL&) const final { return m_userAgent; }

    WebPageProxyIdentifier m_webPageProxyID;
    CyberCore::PageIdentifier m_pageID;
    CyberCore::FrameIdentifier m_frameID;
    String m_userAgent;
};

} // namespace CyberKit

SPECIALIZE_TYPE_TRAITS_BEGIN(CyberKit::ServiceWorkerFrameLoaderClient)
    static bool isType(const CyberCore::FrameLoaderClient& frameLoaderClient) { return frameLoaderClient.isServiceWorkerFrameLoaderClient(); }
SPECIALIZE_TYPE_TRAITS_END()

#endif // ENABLE(SERVICE_WORKER)
