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
#include "SharedMemory.h"
#include <CyberCore/MessageWithMessagePorts.h>
#include <CyberCore/SWClientConnection.h>
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
    static Ref<WebSWClientConnection> create() { return adoptRef(*new WebSWClientConnection); }
    ~WebSWClientConnection();

    CyberCore::SWServerConnectionIdentifier serverConnectionIdentifier() const final { return m_identifier; }

    void addServiceWorkerRegistrationInServer(CyberCore::ServiceWorkerRegistrationIdentifier) final;
    void removeServiceWorkerRegistrationInServer(CyberCore::ServiceWorkerRegistrationIdentifier) final;

    void disconnectedFromWebProcess();
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

    bool mayHaveServiceWorkerRegisteredForOrigin(const CyberCore::SecurityOriginData&) const final;

    void connectionToServerLost();

    bool isThrottleable() const { return m_isThrottleable; }
    void updateThrottleState();

    void terminateWorkerForTesting(CyberCore::ServiceWorkerIdentifier, CompletionHandler<void()>&&);

private:
    WebSWClientConnection();

    void scheduleJobInServer(const CyberCore::ServiceWorkerJobData&) final;
    void finishFetchingScriptInServer(const CyberCore::ServiceWorkerFetchResult&) final;
    void postMessageToServiceWorker(CyberCore::ServiceWorkerIdentifier destinationIdentifier, CyberCore::MessageWithMessagePorts&&, const CyberCore::ServiceWorkerOrClientIdentifier& source) final;
    void registerServiceWorkerClient(const CyberCore::SecurityOrigin& topOrigin, const CyberCore::ServiceWorkerClientData&, const Optional<CyberCore::ServiceWorkerRegistrationIdentifier>&, const String& userAgent) final;
    void unregisterServiceWorkerClient(CyberCore::DocumentIdentifier) final;
    void scheduleUnregisterJobInServer(CyberCore::ServiceWorkerRegistrationIdentifier, CyberCore::DocumentOrWorkerIdentifier, CompletionHandler<void(CyberCore::ExceptionOr<bool>&&)>&&) final;

    void matchRegistration(CyberCore::SecurityOriginData&& topOrigin, const URL& clientURL, RegistrationCallback&&) final;
    void didMatchRegistration(uint64_t matchRequestIdentifier, Optional<CyberCore::ServiceWorkerRegistrationData>&&);
    void didGetRegistrations(uint64_t matchRequestIdentifier, Vector<CyberCore::ServiceWorkerRegistrationData>&&);
    void whenRegistrationReady(const CyberCore::SecurityOriginData& topOrigin, const URL& clientURL, WhenRegistrationReadyCallback&&) final;
    void registrationReady(uint64_t callbackID, CyberCore::ServiceWorkerRegistrationData&&);

    void setDocumentIsControlled(CyberCore::DocumentIdentifier, CyberCore::ServiceWorkerRegistrationData&&, CompletionHandler<void(bool)>&&);

    void getRegistrations(CyberCore::SecurityOriginData&& topOrigin, const URL& clientURL, GetRegistrationsCallback&&) final;
    void whenServiceWorkerIsTerminatedForTesting(CyberCore::ServiceWorkerIdentifier, CompletionHandler<void()>&&) final;

    void didResolveRegistrationPromise(const CyberCore::ServiceWorkerRegistrationKey&) final;
    void storeRegistrationsOnDiskForTesting(CompletionHandler<void()>&&) final;

    void scheduleStorageJob(const CyberCore::ServiceWorkerJobData&);

    void runOrDelayTaskForImport(Function<void()>&& task);

    IPC::Connection* messageSenderConnection() const final;
    uint64_t messageSenderDestinationID() const final { return 0; }

    void setSWOriginTableSharedMemory(const SharedMemory::IPCHandle&);
    void setSWOriginTableIsImported();

    void clear();

    CyberCore::SWServerConnectionIdentifier m_identifier;

    UniqueRef<WebSWOriginTable> m_swOriginTable;

    uint64_t m_previousCallbackIdentifier { 0 };
    HashMap<uint64_t, RegistrationCallback> m_ongoingMatchRegistrationTasks;
    HashMap<uint64_t, GetRegistrationsCallback> m_ongoingGetRegistrationsTasks;
    HashMap<uint64_t, WhenRegistrationReadyCallback> m_ongoingRegistrationReadyTasks;
    Deque<Function<void()>> m_tasksPendingOriginImport;
    bool m_isThrottleable { true };
}; // class WebSWServerConnection

} // namespace CyberKit

#endif // ENABLE(SERVICE_WORKER)
