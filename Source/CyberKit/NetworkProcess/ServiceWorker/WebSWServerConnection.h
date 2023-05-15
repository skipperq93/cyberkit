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
#include "MessageSender.h"
#include "RetrieveRecordResponseBodyCallbackIdentifier.h"
#include "ServiceWorkerFetchTask.h"
#include <CyberCore/ExceptionOr.h>
#include <CyberCore/FetchIdentifier.h>
#include <CyberCore/NavigationPreloadState.h>
#include <CyberCore/ProcessIdentifier.h>
#include <CyberCore/PushPermissionState.h>
#include <CyberCore/PushSubscriptionData.h>
#include <CyberCore/SWServer.h>
#include <pal/SessionID.h>
#include <wtf/HashMap.h>
#include <wtf/WeakPtr.h>

namespace IPC {
class FormDataReference;

template<> struct AsyncReplyError<CyberCore::ExceptionOr<bool>> {
    static CyberCore::ExceptionOr<bool> create() { return CyberCore::Exception { CyberCore::TypeError, "Internal error"_s }; }
};

}

namespace CyberCore {
class ServiceWorkerRegistrationKey;
struct ClientOrigin;
struct ExceptionData;
struct MessageWithMessagePorts;
struct ServiceWorkerClientData;
}

namespace CyberKit {

class NetworkProcess;
class NetworkResourceLoadParameters;
class NetworkResourceLoader;
class ServiceWorkerFetchTask;

class WebSWServerConnection final : public CyberCore::SWServer::Connection, public IPC::MessageSender, public IPC::MessageReceiver {
public:
    WebSWServerConnection(NetworkProcess&, CyberCore::SWServer&, IPC::Connection&, CyberCore::ProcessIdentifier);
    WebSWServerConnection(const WebSWServerConnection&) = delete;
    ~WebSWServerConnection() final;

    using CyberCore::SWServer::Connection::weakPtrFactory;
    using CyberCore::SWServer::Connection::WeakValueType;
    using CyberCore::SWServer::Connection::WeakPtrImplType;

    IPC::Connection& ipcConnection() const { return m_contentConnection.get(); }

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

    NetworkSession* session();
    PAL::SessionID sessionID() const;

    std::unique_ptr<ServiceWorkerFetchTask> createFetchTask(NetworkResourceLoader&, const CyberCore::ResourceRequest&);
    void fetchTaskTimedOut(CyberCore::ServiceWorkerIdentifier);

    void transferServiceWorkerLoadToNewWebProcess(NetworkResourceLoader&, CyberCore::SWServerRegistration&, CyberCore::ProcessIdentifier);
    std::optional<CyberCore::SWServer::GatheredClientData> gatherClientData(CyberCore::ScriptExecutionContextIdentifier);

    void registerServiceWorkerClient(CyberCore::ClientOrigin&&, CyberCore::ServiceWorkerClientData&&, const std::optional<CyberCore::ServiceWorkerRegistrationIdentifier>&, String&& userAgent);
    void registerServiceWorkerClientInternal(CyberCore::ClientOrigin&&, CyberCore::ServiceWorkerClientData&&, const std::optional<CyberCore::ServiceWorkerRegistrationIdentifier>&, String&& userAgent, CyberCore::SWServer::IsBeingCreatedClient);
    void unregisterServiceWorkerClient(const CyberCore::ScriptExecutionContextIdentifier&);

private:
    // Implement SWServer::Connection (Messages to the client WebProcess)
    void rejectJobInClient(CyberCore::ServiceWorkerJobIdentifier, const CyberCore::ExceptionData&) final;
    void resolveRegistrationJobInClient(CyberCore::ServiceWorkerJobIdentifier, const CyberCore::ServiceWorkerRegistrationData&, CyberCore::ShouldNotifyWhenResolved) final;
    void resolveUnregistrationJobInClient(CyberCore::ServiceWorkerJobIdentifier, const CyberCore::ServiceWorkerRegistrationKey&, bool unregistrationResult) final;
    void startScriptFetchInClient(CyberCore::ServiceWorkerJobIdentifier, const CyberCore::ServiceWorkerRegistrationKey&, CyberCore::FetchOptions::Cache) final;
    void updateRegistrationStateInClient(CyberCore::ServiceWorkerRegistrationIdentifier, CyberCore::ServiceWorkerRegistrationState, const std::optional<CyberCore::ServiceWorkerData>&) final;
    void updateWorkerStateInClient(CyberCore::ServiceWorkerIdentifier, CyberCore::ServiceWorkerState) final;
    void fireUpdateFoundEvent(CyberCore::ServiceWorkerRegistrationIdentifier) final;
    void setRegistrationLastUpdateTime(CyberCore::ServiceWorkerRegistrationIdentifier, WallTime) final;
    void setRegistrationUpdateViaCache(CyberCore::ServiceWorkerRegistrationIdentifier, CyberCore::ServiceWorkerUpdateViaCache) final;
    void notifyClientsOfControllerChange(const HashSet<CyberCore::ScriptExecutionContextIdentifier>& contextIdentifiers, const CyberCore::ServiceWorkerData& newController);
    void focusServiceWorkerClient(CyberCore::ScriptExecutionContextIdentifier, CompletionHandler<void(std::optional<CyberCore::ServiceWorkerClientData>&&)>&&) final;

    void scheduleJobInServer(CyberCore::ServiceWorkerJobData&&);

    using UnregisterJobResult = Expected<bool, CyberCore::ExceptionData>;
    void scheduleUnregisterJobInServer(CyberCore::ServiceWorkerJobIdentifier, CyberCore::ServiceWorkerRegistrationIdentifier, CyberCore::ServiceWorkerOrClientIdentifier, CompletionHandler<void(UnregisterJobResult&&)>&&);

    void startFetch(ServiceWorkerFetchTask&, CyberCore::SWServerWorker&);

    void matchRegistration(const CyberCore::SecurityOriginData& topOrigin, const URL& clientURL, CompletionHandler<void(std::optional<CyberCore::ServiceWorkerRegistrationData>&&)>&&);
    void getRegistrations(const CyberCore::SecurityOriginData& topOrigin, const URL& clientURL, CompletionHandler<void(const Vector<CyberCore::ServiceWorkerRegistrationData>&)>&&);

    void terminateWorkerFromClient(CyberCore::ServiceWorkerIdentifier, CompletionHandler<void()>&&);
    void whenServiceWorkerIsTerminatedForTesting(CyberCore::ServiceWorkerIdentifier, CompletionHandler<void()>&&);

    void postMessageToServiceWorkerClient(CyberCore::ScriptExecutionContextIdentifier destinationContextIdentifier, const CyberCore::MessageWithMessagePorts&, CyberCore::ServiceWorkerIdentifier sourceServiceWorkerIdentifier, const String& sourceOrigin) final;

    void contextConnectionCreated(CyberCore::SWServerToContextConnection&) final;
    void updateBackgroundFetchRegistration(const CyberCore::BackgroundFetchInformation&) final;

    bool isThrottleable() const { return m_isThrottleable; }
    bool hasMatchingClient(const CyberCore::RegistrableDomain&) const;
    bool computeThrottleState(const CyberCore::RegistrableDomain&) const;
    void setThrottleState(bool isThrottleable);
    void updateThrottleState();

    void subscribeToPushService(CyberCore::ServiceWorkerRegistrationIdentifier, Vector<uint8_t>&& applicationServerKey, CompletionHandler<void(Expected<CyberCore::PushSubscriptionData, CyberCore::ExceptionData>&&)>&&);
    void unsubscribeFromPushService(CyberCore::ServiceWorkerRegistrationIdentifier, CyberCore::PushSubscriptionIdentifier,  CompletionHandler<void(Expected<bool, CyberCore::ExceptionData>&&)>&&);
    void getPushSubscription(CyberCore::ServiceWorkerRegistrationIdentifier, CompletionHandler<void(Expected<std::optional<CyberCore::PushSubscriptionData>, CyberCore::ExceptionData>&&)>&&);
    void getPushPermissionState(CyberCore::ServiceWorkerRegistrationIdentifier, CompletionHandler<void(Expected<uint8_t, CyberCore::ExceptionData>&&)>&&);

    void postMessageToServiceWorker(CyberCore::ServiceWorkerIdentifier destination, CyberCore::MessageWithMessagePorts&&, const CyberCore::ServiceWorkerOrClientIdentifier& source);
    void controlClient(const NetworkResourceLoadParameters&, CyberCore::SWServerRegistration&, const CyberCore::ResourceRequest&, CyberCore::ProcessIdentifier);

    using ExceptionOrVoidCallback = CompletionHandler<void(std::optional<CyberCore::ExceptionData>&&)>;
    void enableNavigationPreload(CyberCore::ServiceWorkerRegistrationIdentifier, ExceptionOrVoidCallback&&);
    void disableNavigationPreload(CyberCore::ServiceWorkerRegistrationIdentifier, ExceptionOrVoidCallback&&);
    void setNavigationPreloadHeaderValue(CyberCore::ServiceWorkerRegistrationIdentifier, String&&, ExceptionOrVoidCallback&&);
    using ExceptionOrNavigationPreloadStateCallback = CompletionHandler<void(Expected<CyberCore::NavigationPreloadState, CyberCore::ExceptionData>&&)>;
    void getNavigationPreloadState(CyberCore::ServiceWorkerRegistrationIdentifier, ExceptionOrNavigationPreloadStateCallback&&);

    void retrieveRecordResponseBody(CyberCore::BackgroundFetchRecordIdentifier, RetrieveRecordResponseBodyCallbackIdentifier);

    URL clientURLFromIdentifier(CyberCore::ServiceWorkerOrClientIdentifier);

    IPC::Connection* messageSenderConnection() const final { return m_contentConnection.ptr(); }
    uint64_t messageSenderDestinationID() const final { return 0; }
    
    template<typename U> static void sendToContextProcess(CyberCore::SWServerToContextConnection&, U&& message);

    Ref<IPC::Connection> m_contentConnection;
    Ref<NetworkProcess> m_networkProcess;
    HashMap<CyberCore::ScriptExecutionContextIdentifier, CyberCore::ClientOrigin> m_clientOrigins;
    HashMap<CyberCore::ServiceWorkerJobIdentifier, CompletionHandler<void(UnregisterJobResult&&)>> m_unregisterJobs;
    bool m_isThrottleable { true };
};

} // namespace CyberKit

#endif // ENABLE(SERVICE_WORKER)
