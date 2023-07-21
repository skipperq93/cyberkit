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

namespace CyberCore {
struct FetchOptions;
class ResourceRequest;
}

namespace IPC {
class FormDataReference;
}

namespace PAL {
class SessionID;
}

namespace CyberKit {

class NetworkProcess;
class WebSWServerConnection;

class WebSWServerToContextConnection : public CyberCore::SWServerToContextConnection, public IPC::MessageSender, public IPC::MessageReceiver {
public:
    template <typename... Args> static Ref<WebSWServerToContextConnection> create(Args&&... args)
    {
        return adoptRef(*new WebSWServerToContextConnection(std::forward<Args>(args)...));
    }

    void connectionClosed();

    IPC::Connection* ipcConnection() const { return m_ipcConnection.ptr(); }

    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

    void terminate();

    void startFetch(PAL::SessionID, WebSWServerConnection&, CyberCore::FetchIdentifier, CyberCore::ServiceWorkerIdentifier, const CyberCore::ResourceRequest&, const CyberCore::FetchOptions&, const IPC::FormDataReference&, const String&);
    void cancelFetch(CyberCore::SWServerConnectionIdentifier, CyberCore::FetchIdentifier, CyberCore::ServiceWorkerIdentifier);
    void continueDidReceiveFetchResponse(CyberCore::SWServerConnectionIdentifier, CyberCore::FetchIdentifier, CyberCore::ServiceWorkerIdentifier);

    void didReceiveFetchTaskMessage(IPC::Connection&, IPC::Decoder&);

    void setThrottleState(bool isThrottleable);
    bool isThrottleable() const { return m_isThrottleable; }

    void fetchTaskTimedOut(ServiceWorkerFetchTask&);

private:
    WebSWServerToContextConnection(NetworkProcess&, const CyberCore::RegistrableDomain&, Ref<IPC::Connection>&&);
    ~WebSWServerToContextConnection();

    // IPC::MessageSender
    IPC::Connection* messageSenderConnection() const final;
    uint64_t messageSenderDestinationID() const final;

    // Messages to the SW host WebProcess
    void installServiceWorkerContext(const CyberCore::ServiceWorkerContextData&, PAL::SessionID, const String& userAgent) final;
    void fireInstallEvent(CyberCore::ServiceWorkerIdentifier) final;
    void fireActivateEvent(CyberCore::ServiceWorkerIdentifier) final;
    void terminateWorker(CyberCore::ServiceWorkerIdentifier) final;
    void syncTerminateWorker(CyberCore::ServiceWorkerIdentifier) final;
    void findClientByIdentifierCompleted(uint64_t requestIdentifier, const Optional<CyberCore::ServiceWorkerClientData>&, bool hasSecurityError) final;
    void matchAllCompleted(uint64_t requestIdentifier, const Vector<CyberCore::ServiceWorkerClientData>&) final;
    void claimCompleted(uint64_t requestIdentifier) final;
    void didFinishSkipWaiting(uint64_t callbackID) final;

    void connectionMayNoLongerBeNeeded() final;

    Ref<IPC::Connection> m_ipcConnection;
    Ref<NetworkProcess> m_networkProcess;
    
    HashMap<ServiceWorkerFetchTask::Identifier, CyberCore::FetchIdentifier> m_ongoingFetchIdentifiers;
    HashMap<CyberCore::FetchIdentifier, Ref<ServiceWorkerFetchTask>> m_ongoingFetches;
    bool m_isThrottleable { true };
}; // class WebSWServerToContextConnection

} // namespace CyberKit

#endif // ENABLE(SERVICE_WORKER)

