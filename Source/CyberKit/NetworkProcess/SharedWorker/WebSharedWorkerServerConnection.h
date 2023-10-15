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

#include "MessageReceiver.h"
#include "MessageSender.h"
#include <CyberCore/ProcessIdentifier.h>
#include <CyberCore/SharedWorkerObjectIdentifier.h>
#include <CyberCore/TransferredMessagePort.h>
#include <CyberCore/WorkerInitializationData.h>
#include <pal/SessionID.h>

namespace CyberCore {
class ResourceError;
struct SharedWorkerKey;
struct WorkerFetchResult;
struct WorkerOptions;
}

namespace CyberKit {

class NetworkProcess;
class NetworkSession;
class WebSharedWorker;
class WebSharedWorkerServer;
class WebSharedWorkerServerToContextConnection;
class NetworkSession;

class WebSharedWorkerServerConnection : public IPC::MessageSender, public IPC::MessageReceiver {
    WTF_MAKE_FAST_ALLOCATED;
public:
    WebSharedWorkerServerConnection(NetworkProcess&, WebSharedWorkerServer&, IPC::Connection&, CyberCore::ProcessIdentifier);
    ~WebSharedWorkerServerConnection();

    WebSharedWorkerServer& server() { return m_server; }
    const WebSharedWorkerServer& server() const { return m_server; }

    PAL::SessionID sessionID();
    NetworkSession* session();
    CyberCore::ProcessIdentifier webProcessIdentifier() const { return m_webProcessIdentifier; }

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

    void fetchScriptInClient(const WebSharedWorker&, CyberCore::SharedWorkerObjectIdentifier, CompletionHandler<void(CyberCore::WorkerFetchResult&&, CyberCore::WorkerInitializationData&&)>&&);
    void notifyWorkerObjectOfLoadCompletion(CyberCore::SharedWorkerObjectIdentifier, const CyberCore::ResourceError&);
    void postExceptionToWorkerObject(CyberCore::SharedWorkerObjectIdentifier, const String& errorMessage, int lineNumber, int columnNumber, const String& sourceURL);

private:
    // IPC::MessageSender.
    IPC::Connection* messageSenderConnection() const final;
    uint64_t messageSenderDestinationID() const final { return 0; }

    // IPC messages.
    void requestSharedWorker(CyberCore::SharedWorkerKey&&, CyberCore::SharedWorkerObjectIdentifier, CyberCore::TransferredMessagePort&&, CyberCore::WorkerOptions&&);
    void sharedWorkerObjectIsGoingAway(CyberCore::SharedWorkerKey&&, CyberCore::SharedWorkerObjectIdentifier);
    void suspendForBackForwardCache(CyberCore::SharedWorkerKey&&, CyberCore::SharedWorkerObjectIdentifier);
    void resumeForBackForwardCache(CyberCore::SharedWorkerKey&&, CyberCore::SharedWorkerObjectIdentifier);

    Ref<IPC::Connection> m_contentConnection;
    Ref<NetworkProcess> m_networkProcess;
    WebSharedWorkerServer& m_server;
    CyberCore::ProcessIdentifier m_webProcessIdentifier;
};

} // namespace CyberKit
