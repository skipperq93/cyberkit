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

#include "MessageReceiver.h"
#include "MessageSender.h"
#include <CyberCore/FetchIdentifier.h>
#include <CyberCore/SWServer.h>
#include <pal/SessionID.h>
#include <wtf/HashMap.h>

namespace IPC {
class FormDataReference;
}

namespace CyberCore {
class ServiceWorkerRegistrationKey;
struct ClientOrigin;
struct ExceptionData;
struct MessageWithMessagePorts;
struct ServiceWorkerClientData;
}

namespace CyberKit {

class WebSWServerConnection : public CyberCore::SWServer::Connection, public IPC::MessageSender, public IPC::MessageReceiver {
public:
    WebSWServerConnection(CyberCore::SWServer&, IPC::Connection&, PAL::SessionID);
    WebSWServerConnection(const WebSWServerConnection&) = delete;
    ~WebSWServerConnection() final;

    IPC::Connection& ipcConnection() const { return m_contentConnection.get(); }

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;
    void didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, std::unique_ptr<IPC::Encoder>&);

    PAL::SessionID sessionID() const { return m_sessionID; }

    void didReceiveFetchResponse(CyberCore::FetchIdentifier, const CyberCore::ResourceResponse&);
    void didReceiveFetchData(CyberCore::FetchIdentifier, const IPC::DataReference&, int64_t encodedDataLength);
    void didReceiveFetchFormData(CyberCore::FetchIdentifier, const IPC::FormDataReference&);
    void didFinishFetch(CyberCore::FetchIdentifier);
    void didFailFetch(CyberCore::FetchIdentifier, const CyberCore::ResourceError&);
    void didNotHandleFetch(CyberCore::FetchIdentifier);

    void postMessageToServiceWorkerClient(CyberCore::DocumentIdentifier destinationContextIdentifier, CyberCore::MessageWithMessagePorts&&, CyberCore::ServiceWorkerIdentifier sourceServiceWorkerIdentifier, const String& sourceOrigin);
    void postMessageToServiceWorker(CyberCore::ServiceWorkerIdentifier destination, CyberCore::MessageWithMessagePorts&&, const CyberCore::ServiceWorkerOrClientIdentifier& source);

private:
    // Implement SWServer::Connection (Messages to the client WebProcess)
    void rejectJobInClient(CyberCore::ServiceWorkerJobIdentifier, const CyberCore::ExceptionData&) final;
    void resolveRegistrationJobInClient(CyberCore::ServiceWorkerJobIdentifier, const CyberCore::ServiceWorkerRegistrationData&, CyberCore::ShouldNotifyWhenResolved) final;
    void resolveUnregistrationJobInClient(CyberCore::ServiceWorkerJobIdentifier, const CyberCore::ServiceWorkerRegistrationKey&, bool unregistrationResult) final;
    void startScriptFetchInClient(CyberCore::ServiceWorkerJobIdentifier, const CyberCore::ServiceWorkerRegistrationKey&, CyberCore::FetchOptions::Cache) final;
    void updateRegistrationStateInClient(CyberCore::ServiceWorkerRegistrationIdentifier, CyberCore::ServiceWorkerRegistrationState, const Optional<CyberCore::ServiceWorkerData>&) final;
    void updateWorkerStateInClient(CyberCore::ServiceWorkerIdentifier, CyberCore::ServiceWorkerState) final;
    void fireUpdateFoundEvent(CyberCore::ServiceWorkerRegistrationIdentifier) final;
    void setRegistrationLastUpdateTime(CyberCore::ServiceWorkerRegistrationIdentifier, WallTime) final;
    void setRegistrationUpdateViaCache(CyberCore::ServiceWorkerRegistrationIdentifier, CyberCore::ServiceWorkerUpdateViaCache) final;
    void notifyClientsOfControllerChange(const HashSet<CyberCore::DocumentIdentifier>& contextIdentifiers, const CyberCore::ServiceWorkerData& newController);
    void registrationReady(uint64_t registrationReadyRequestIdentifier, CyberCore::ServiceWorkerRegistrationData&&) final;

    void scheduleJobInServer(CyberCore::ServiceWorkerJobData&&);

    void startFetch(CyberCore::ServiceWorkerRegistrationIdentifier, CyberCore::FetchIdentifier, CyberCore::ResourceRequest&&, CyberCore::FetchOptions&&, IPC::FormDataReference&&, String&& referrer);
    void cancelFetch(CyberCore::ServiceWorkerRegistrationIdentifier, CyberCore::FetchIdentifier);

    void matchRegistration(uint64_t registrationMatchRequestIdentifier, const CyberCore::SecurityOriginData& topOrigin, const URL& clientURL);
    void getRegistrations(uint64_t registrationMatchRequestIdentifier, const CyberCore::SecurityOriginData& topOrigin, const URL& clientURL);

    void registerServiceWorkerClient(CyberCore::SecurityOriginData&& topOrigin, CyberCore::ServiceWorkerClientData&&, const Optional<CyberCore::ServiceWorkerRegistrationIdentifier>&, String&& userAgent);
    void unregisterServiceWorkerClient(const CyberCore::ServiceWorkerClientIdentifier&);

    IPC::Connection* messageSenderConnection() final { return m_contentConnection.ptr(); }
    uint64_t messageSenderDestinationID() final { return identifier().toUInt64(); }
    
    template<typename U> static void sendToContextProcess(CyberCore::SWServerToContextConnection&, U&& message);

    PAL::SessionID m_sessionID;
    Ref<IPC::Connection> m_contentConnection;
    HashMap<CyberCore::ServiceWorkerClientIdentifier, CyberCore::ClientOrigin> m_clientOrigins;
};

} // namespace CyberKit

#endif // ENABLE(SERVICE_WORKER)
