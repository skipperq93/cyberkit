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

#include "Connection.h"
#include "IdentifierTypes.h"
#include "MessageReceiver.h"
#include "WebPageProxyIdentifier.h"
#include "WebPreferencesStore.h"
#include <CyberCore/PageIdentifier.h>
#include <CyberCore/RegistrableDomain.h>
#include <CyberCore/SharedWorkerContextManager.h>
#include <CyberCore/SharedWorkerIdentifier.h>

namespace CyberCore {
struct ClientOrigin;
struct WorkerFetchResult;
struct WorkerInitializationData;
struct WorkerOptions;
}

namespace CyberKit {

class WebUserContentController;
struct RemoteWorkerInitializationData;

class WebSharedWorkerContextManagerConnection final : public CyberCore::SharedWorkerContextManager::Connection, public IPC::MessageReceiver {
public:
    WebSharedWorkerContextManagerConnection(Ref<IPC::Connection>&&, CyberCore::RegistrableDomain&&, PageGroupIdentifier, WebPageProxyIdentifier, CyberCore::PageIdentifier, const WebPreferencesStore&, RemoteWorkerInitializationData&&);
    ~WebSharedWorkerContextManagerConnection();

    void establishConnection(CompletionHandler<void()>&&) final;
    void postExceptionToWorkerObject(CyberCore::SharedWorkerIdentifier, const String& errorMessage, int lineNumber, int columnNumber, const String& sourceURL) final;
    void sharedWorkerTerminated(CyberCore::SharedWorkerIdentifier) final;

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

private:
    // IPC Messages.
    void launchSharedWorker(CyberCore::ClientOrigin&&, CyberCore::SharedWorkerIdentifier, CyberCore::WorkerOptions&&, CyberCore::WorkerFetchResult&&, CyberCore::WorkerInitializationData&&);
    void updatePreferencesStore(const WebPreferencesStore&);
    void setUserAgent(String&& userAgent) { m_userAgent = WTFMove(userAgent); }
    void close();

    Ref<IPC::Connection> m_connectionToNetworkProcess;
    CyberCore::RegistrableDomain m_registrableDomain;
    PageGroupIdentifier m_pageGroupID;
    WebPageProxyIdentifier m_webPageProxyID;
    CyberCore::PageIdentifier m_pageID;
    String m_userAgent;
    Ref<WebUserContentController> m_userContentController;
    std::optional<WebPreferencesStore> m_preferencesStore;
};

} // namespace CyberKit
