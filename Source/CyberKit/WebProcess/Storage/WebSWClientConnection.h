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
#include "ServiceWorkerClientFetch.h"
#include "SharedMemory.h"
#include <CyberCore/MessageWithMessagePorts.h>
#include <CyberCore/SWClientConnection.h>
#include <pal/SessionID.h>
#include <wtf/UniqueRef.h>

namespace CyberCore {
struct ExceptionData;
class ResourceLoader;
}

namespace CyberKit {

class WebSWOriginTable;
class WebServiceWorkerProvider;

class WebSWClientConnection final : public CyberCore::SWClientConnection, private IPC::MessageSender, public IPC::MessageReceiver {
public:
    static Ref<WebSWClientConnection> create(PAL::SessionID sessionID) { return adoptRef(*new WebSWClientConnection { sessionID }); }
    ~WebSWClientConnection();

    CyberCore::SWServerConnectionIdentifier serverConnectionIdentifier() const final;

    void addServiceWorkerRegistrationInServer(CyberCore::ServiceWorkerRegistrationIdentifier) final;
    void removeServiceWorkerRegistrationInServer(CyberCore::ServiceWorkerRegistrationIdentifier) final;

    void disconnectedFromWebProcess();
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

    bool mayHaveServiceWorkerRegisteredForOrigin(const CyberCore::SecurityOriginData&) const final;
    void startFetch(CyberCore::FetchIdentifier, CyberCore::ServiceWorkerRegistrationIdentifier, const CyberCore::ResourceRequest&, const CyberCore::FetchOptions&, const String& referrer);
    void cancelFetch(CyberCore::FetchIdentifier, CyberCore::ServiceWorkerRegistrationIdentifier);
    void continueDidReceiveFetchResponse(CyberCore::FetchIdentifier, CyberCore::ServiceWorkerRegistrationIdentifier);

    void connectionToServerLost();

    void syncTerminateWorker(CyberCore::ServiceWorkerIdentifier) final;

    PAL::SessionID sessionID() const { return m_sessionID; }

private:
    explicit WebSWClientConnection(PAL::SessionID);

    void initializeConnectionIfNeeded();

    void scheduleJobInServer(const CyberCore::ServiceWorkerJobData&) final;
    void finishFetchingScriptInServer(const CyberCore::ServiceWorkerFetchResult&) final;
    void postMessageToServiceWorker(CyberCore::ServiceWorkerIdentifier destinationIdentifier, CyberCore::MessageWithMessagePorts&&, const CyberCore::ServiceWorkerOrClientIdentifier& source) final;
    void registerServiceWorkerClient(const CyberCore::SecurityOrigin& topOrigin, const CyberCore::ServiceWorkerClientData&, const Optional<CyberCore::ServiceWorkerRegistrationIdentifier>&, const String& userAgent) final;
    void unregisterServiceWorkerClient(CyberCore::DocumentIdentifier) final;

    void matchRegistration(CyberCore::SecurityOriginData&& topOrigin, const URL& clientURL, RegistrationCallback&&) final;
    void didMatchRegistration(uint64_t matchRequestIdentifier, Optional<CyberCore::ServiceWorkerRegistrationData>&&);
    void didGetRegistrations(uint64_t matchRequestIdentifier, Vector<CyberCore::ServiceWorkerRegistrationData>&&);
    void whenRegistrationReady(const CyberCore::SecurityOrigin& topOrigin, const URL& clientURL, WhenRegistrationReadyCallback&&) final;
    void registrationReady(uint64_t callbackID, CyberCore::ServiceWorkerRegistrationData&&);

    void getRegistrations(CyberCore::SecurityOriginData&& topOrigin, const URL& clientURL, GetRegistrationsCallback&&) final;

    void didResolveRegistrationPromise(const CyberCore::ServiceWorkerRegistrationKey&) final;
    void updateThrottleState() final;
    bool isThrottleable() const final { return m_isThrottleable; }
    void storeRegistrationsOnDiskForTesting(CompletionHandler<void()>&&) final;

    void scheduleStorageJob(const CyberCore::ServiceWorkerJobData&);

    void runOrDelayTaskForImport(WTF::Function<void()>&& task);

    IPC::Connection* messageSenderConnection() const final { return m_connection.get(); }
    uint64_t messageSenderDestinationID() const final { return m_identifier.toUInt64(); }

    void setSWOriginTableSharedMemory(const SharedMemory::Handle&);
    void setSWOriginTableIsImported();

    template<typename U> void ensureConnectionAndSend(const U& message);

    PAL::SessionID m_sessionID;
    CyberCore::SWServerConnectionIdentifier m_identifier;

    RefPtr<IPC::Connection> m_connection;
    UniqueRef<WebSWOriginTable> m_swOriginTable;

    uint64_t m_previousCallbackIdentifier { 0 };
    HashMap<uint64_t, RegistrationCallback> m_ongoingMatchRegistrationTasks;
    HashMap<uint64_t, GetRegistrationsCallback> m_ongoingGetRegistrationsTasks;
    HashMap<uint64_t, WhenRegistrationReadyCallback> m_ongoingRegistrationReadyTasks;
    Deque<WTF::Function<void()>> m_tasksPendingOriginImport;
    bool m_isThrottleable { true };
}; // class WebSWServerConnection

} // namespace CyberKit

#endif // ENABLE(SERVICE_WORKER)
