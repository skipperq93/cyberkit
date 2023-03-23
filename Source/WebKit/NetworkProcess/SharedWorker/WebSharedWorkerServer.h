/*
 * Copyright (C) 2022 Apple Inc. All rights reserved.
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

#include <CyberCore/ProcessIdentifier.h>
#include <CyberCore/RegistrableDomain.h>
#include <CyberCore/ScriptExecutionContextIdentifier.h>
#include <CyberCore/SharedWorkerIdentifier.h>
#include <CyberCore/SharedWorkerKey.h>
#include <CyberCore/SharedWorkerObjectIdentifier.h>
#include <CyberCore/TransferredMessagePort.h>
#include <wtf/WeakPtr.h>

namespace PAL {
class SessionID;
}

namespace CyberCore {

struct ClientOrigin;
struct WorkerFetchResult;
struct WorkerInitializationData;
struct WorkerOptions;
}

namespace WebKit {

class NetworkSession;
class WebSharedWorker;
class WebSharedWorkerServerConnection;
class WebSharedWorkerServerToContextConnection;

class WebSharedWorkerServer : public CanMakeWeakPtr<WebSharedWorkerServer> {
    WTF_MAKE_FAST_ALLOCATED;
public:
    explicit WebSharedWorkerServer(NetworkSession&);
    ~WebSharedWorkerServer();

    PAL::SessionID sessionID();
    WebSharedWorkerServerToContextConnection* contextConnectionForRegistrableDomain(const CyberCore::RegistrableDomain&) const;

    void requestSharedWorker(CyberCore::SharedWorkerKey&&, CyberCore::SharedWorkerObjectIdentifier, CyberCore::TransferredMessagePort&&, CyberCore::WorkerOptions&&);
    void sharedWorkerObjectIsGoingAway(const CyberCore::SharedWorkerKey&, CyberCore::SharedWorkerObjectIdentifier);
    void suspendForBackForwardCache(const CyberCore::SharedWorkerKey&, CyberCore::SharedWorkerObjectIdentifier);
    void resumeForBackForwardCache(const CyberCore::SharedWorkerKey&, CyberCore::SharedWorkerObjectIdentifier);
    void postExceptionToWorkerObject(CyberCore::SharedWorkerIdentifier, const String& errorMessage, int lineNumber, int columnNumber, const String& sourceURL);
    void sharedWorkerTerminated(CyberCore::SharedWorkerIdentifier);

    void terminateContextConnectionWhenPossible(const CyberCore::RegistrableDomain&, CyberCore::ProcessIdentifier);
    void addContextConnection(WebSharedWorkerServerToContextConnection&);
    void removeContextConnection(WebSharedWorkerServerToContextConnection&);
    void addConnection(std::unique_ptr<WebSharedWorkerServerConnection>&&);
    void removeConnection(CyberCore::ProcessIdentifier);

private:
    void createContextConnection(const CyberCore::RegistrableDomain&, std::optional<CyberCore::ProcessIdentifier> requestingProcessIdentifier);
    bool needsContextConnectionForRegistrableDomain(const CyberCore::RegistrableDomain&) const;
    void contextConnectionCreated(WebSharedWorkerServerToContextConnection&);
    void didFinishFetchingSharedWorkerScript(WebSharedWorker&, CyberCore::WorkerFetchResult&&, CyberCore::WorkerInitializationData&&);
    void shutDownSharedWorker(const CyberCore::SharedWorkerKey&);

    NetworkSession& m_session;
    HashMap<CyberCore::ProcessIdentifier, std::unique_ptr<WebSharedWorkerServerConnection>> m_connections;
    HashMap<CyberCore::RegistrableDomain, WebSharedWorkerServerToContextConnection*> m_contextConnections;
    HashSet<CyberCore::RegistrableDomain> m_pendingContextConnectionDomains;
    HashMap<CyberCore::SharedWorkerKey, std::unique_ptr<WebSharedWorker>> m_sharedWorkers;
};

} // namespace WebKit
