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
#include "ServiceWorkerFetchTask.h"
#include <CyberCore/SWServerToContextConnection.h>
#include <wtf/WeakPtr.h>

namespace CyberCore {
struct FetchOptions;
struct MessageWithMessagePorts;
class ResourceRequest;
}

namespace IPC {
class FormDataReference;
}

namespace PAL {
class SessionID;
}

namespace CyberKit {

class NetworkConnectionToWebProcess;
class WebSWServerConnection;

class WebSWServerToContextConnection: public CanMakeWeakPtr<WebSWServerToContextConnection>, public CyberCore::SWServerToContextConnection, public IPC::MessageSender, public IPC::MessageReceiver {
public:
    WebSWServerToContextConnection(NetworkConnectionToWebProcess&, CyberCore::RegistrableDomain&&, CyberCore::SWServer&);
    ~WebSWServerToContextConnection();

    IPC::Connection& ipcConnection() const;

    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

    void startFetch(ServiceWorkerFetchTask&);
    void cancelFetch(CyberCore::SWServerConnectionIdentifier, CyberCore::FetchIdentifier, CyberCore::ServiceWorkerIdentifier);

    void didReceiveFetchTaskMessage(IPC::Connection&, IPC::Decoder&);

    void setThrottleState(bool isThrottleable);
    bool isThrottleable() const { return m_isThrottleable; }

    void registerFetch(ServiceWorkerFetchTask&);
    void unregisterFetch(ServiceWorkerFetchTask&);

    CyberCore::ProcessIdentifier webProcessIdentifier() const;

private:
    // IPC::MessageSender
    IPC::Connection* messageSenderConnection() const final;
    uint64_t messageSenderDestinationID() const final;

    void postMessageToServiceWorkerClient(const CyberCore::ServiceWorkerClientIdentifier& destinationIdentifier, const CyberCore::MessageWithMessagePorts&, CyberCore::ServiceWorkerIdentifier sourceIdentifier, const String& sourceOrigin);

    // Messages to the SW host WebProcess
    void installServiceWorkerContext(const CyberCore::ServiceWorkerContextData&, const String& userAgent) final;
    void fireInstallEvent(CyberCore::ServiceWorkerIdentifier) final;
    void fireActivateEvent(CyberCore::ServiceWorkerIdentifier) final;
    void terminateWorker(CyberCore::ServiceWorkerIdentifier) final;
    void syncTerminateWorker(CyberCore::ServiceWorkerIdentifier) final;
    void findClientByIdentifierCompleted(uint64_t requestIdentifier, const Optional<CyberCore::ServiceWorkerClientData>&, bool hasSecurityError) final;
    void matchAllCompleted(uint64_t requestIdentifier, const Vector<CyberCore::ServiceWorkerClientData>&) final;

    void connectionIsNoLongerNeeded() final;

    void connectionClosed();

    NetworkConnectionToWebProcess& m_connection;
    WeakPtr<CyberCore::SWServer> m_server;
    HashMap<CyberCore::FetchIdentifier, WeakPtr<ServiceWorkerFetchTask>> m_ongoingFetches;
    bool m_isThrottleable { true };
}; // class WebSWServerToContextConnection

} // namespace CyberKit

#endif // ENABLE(SERVICE_WORKER)

