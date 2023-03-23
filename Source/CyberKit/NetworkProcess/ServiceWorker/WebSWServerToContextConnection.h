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

#include "MessageReceiver.h"
#include "MessageSender.h"
#include "ServiceWorkerDownloadTask.h"
#include "ServiceWorkerFetchTask.h"
#include "WebPageProxyIdentifier.h"
#include <CyberCore/SWServerToContextConnection.h>
#include <wtf/ThreadSafeWeakPtr.h>
#include <wtf/URLHash.h>
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

class WebSWServerToContextConnection final: public CyberCore::SWServerToContextConnection, public IPC::MessageSender, public IPC::MessageReceiver {
public:
    WebSWServerToContextConnection(NetworkConnectionToWebProcess&, WebPageProxyIdentifier, CyberCore::RegistrableDomain&&, std::optional<CyberCore::ScriptExecutionContextIdentifier> serviceWorkerPageIdentifier, CyberCore::SWServer&);
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
    void registerDownload(ServiceWorkerDownloadTask&);
    void unregisterDownload(ServiceWorkerDownloadTask&);

    CyberCore::ProcessIdentifier webProcessIdentifier() const final;
    NetworkProcess& networkProcess() { return m_connection.networkProcess(); }

private:
    // IPC::MessageSender
    IPC::Connection* messageSenderConnection() const final;
    uint64_t messageSenderDestinationID() const final;

    void postMessageToServiceWorkerClient(const CyberCore::ScriptExecutionContextIdentifier& destinationIdentifier, const CyberCore::MessageWithMessagePorts&, CyberCore::ServiceWorkerIdentifier sourceIdentifier, const String& sourceOrigin);
    void skipWaiting(uint64_t requestIdentifier, CyberCore::ServiceWorkerIdentifier);

    // Messages to the SW host WebProcess
    void installServiceWorkerContext(const CyberCore::ServiceWorkerContextData&, const CyberCore::ServiceWorkerData&, const String& userAgent, CyberCore::WorkerThreadMode) final;
    void updateAppInitiatedValue(CyberCore::ServiceWorkerIdentifier, CyberCore::LastNavigationWasAppInitiated) final;
    void fireInstallEvent(CyberCore::ServiceWorkerIdentifier) final;
    void fireActivateEvent(CyberCore::ServiceWorkerIdentifier) final;
    void terminateWorker(CyberCore::ServiceWorkerIdentifier) final;
    void didSaveScriptsToDisk(CyberCore::ServiceWorkerIdentifier, const CyberCore::ScriptBuffer&, const MemoryCompactRobinHoodHashMap<URL, CyberCore::ScriptBuffer>& importedScripts) final;
    void matchAllCompleted(uint64_t requestIdentifier, const Vector<CyberCore::ServiceWorkerClientData>&) final;
    void firePushEvent(CyberCore::ServiceWorkerIdentifier, const std::optional<Vector<uint8_t>>&, CompletionHandler<void(bool)>&&) final;
    void fireNotificationEvent(CyberCore::ServiceWorkerIdentifier, const CyberCore::NotificationData&, CyberCore::NotificationEventType, CompletionHandler<void(bool)>&&) final;
    void close() final;
    void focus(CyberCore::ScriptExecutionContextIdentifier, CompletionHandler<void(std::optional<CyberCore::ServiceWorkerClientData>&&)>&&);
    void navigate(CyberCore::ScriptExecutionContextIdentifier, CyberCore::ServiceWorkerIdentifier, const URL&, CompletionHandler<void(Expected<std::optional<CyberCore::ServiceWorkerClientData>, CyberCore::ExceptionData>&&)>&&);

    void connectionIsNoLongerNeeded() final;
    void terminateDueToUnresponsiveness() final;
    void openWindow(CyberCore::ServiceWorkerIdentifier, const URL&, OpenWindowCallback&&) final;

    void connectionClosed();

    NetworkConnectionToWebProcess& m_connection;
    HashMap<CyberCore::FetchIdentifier, WeakPtr<ServiceWorkerFetchTask>> m_ongoingFetches;
    HashMap<CyberCore::FetchIdentifier, ThreadSafeWeakPtr<ServiceWorkerDownloadTask>> m_ongoingDownloads;
    bool m_isThrottleable { true };
    WebPageProxyIdentifier m_webPageProxyID;
    size_t m_processingFunctionalEventCount { 0 };
}; // class WebSWServerToContextConnection

} // namespace CyberKit

#endif // ENABLE(SERVICE_WORKER)

