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
#include <CyberCore/FetchEvent.h>
#include <CyberCore/FetchLoader.h>
#include <CyberCore/FetchLoaderClient.h>
#include <CyberCore/NetworkLoadMetrics.h>
#include <CyberCore/ResourceResponse.h>
#include <CyberCore/ServiceWorkerFetch.h>
#include <CyberCore/ServiceWorkerTypes.h>
#include <CyberCore/SharedBuffer.h>
#include <wtf/UniqueRef.h>

namespace CyberKit {

class WebServiceWorkerFetchTaskClient final : public CyberCore::ServiceWorkerFetch::Client {
public:
    static Ref<WebServiceWorkerFetchTaskClient> create(Ref<IPC::Connection>&& connection, CyberCore::ServiceWorkerIdentifier serviceWorkerIdentifier,  CyberCore::SWServerConnectionIdentifier serverConnectionIdentifier, CyberCore::FetchIdentifier fetchTaskIdentifier, bool needsContinueDidReceiveResponseMessage)
    {
        return adoptRef(*new WebServiceWorkerFetchTaskClient(WTFMove(connection), serviceWorkerIdentifier, serverConnectionIdentifier, fetchTaskIdentifier, needsContinueDidReceiveResponseMessage));
    }

private:
    WebServiceWorkerFetchTaskClient(Ref<IPC::Connection>&&, CyberCore::ServiceWorkerIdentifier, CyberCore::SWServerConnectionIdentifier, CyberCore::FetchIdentifier, bool needsContinueDidReceiveResponseMessage);

    void didReceiveResponse(const CyberCore::ResourceResponse&) final;
    void didReceiveRedirection(const CyberCore::ResourceResponse&) final;
    void didReceiveData(const CyberCore::SharedBuffer&) final;
    void didReceiveFormDataAndFinish(Ref<CyberCore::FormData>&&) final;
    void didFail(const CyberCore::ResourceError&) final;
    void didFinish(const CyberCore::NetworkLoadMetrics&) final;
    void didNotHandle() final;
    void cancel() final;
    void continueDidReceiveResponse() final;
    void convertFetchToDownload() final;
    void setCancelledCallback(Function<void()>&&) final;
    void setFetchEvent(Ref<CyberCore::FetchEvent>&&);
    void navigationPreloadIsReady(CyberCore::ResourceResponse::CrossThreadData&&) final;
    void navigationPreloadFailed(CyberCore::ResourceError&&) final;
    void usePreload() final;

    void cleanup();

    void didReceiveBlobChunk(const CyberCore::SharedBuffer&);
    void didFinishBlobLoading();

    struct BlobLoader final : CyberCore::FetchLoaderClient {
        explicit BlobLoader(WebServiceWorkerFetchTaskClient& client) : client(client) { }

        // FetchLoaderClient API
        void didReceiveResponse(const CyberCore::ResourceResponse&) final { }
        void didReceiveData(const CyberCore::SharedBuffer& data) final { client->didReceiveBlobChunk(data); }
        void didFail(const CyberCore::ResourceError& error) final { client->didFail(error); }
        void didSucceed(const CyberCore::NetworkLoadMetrics&) final { client->didFinishBlobLoading(); }

        Ref<WebServiceWorkerFetchTaskClient> client;
        std::unique_ptr<CyberCore::FetchLoader> loader;
    };

    RefPtr<IPC::Connection> m_connection;
    CyberCore::SWServerConnectionIdentifier m_serverConnectionIdentifier;
    CyberCore::ServiceWorkerIdentifier m_serviceWorkerIdentifier;
    CyberCore::FetchIdentifier m_fetchIdentifier;
    std::optional<BlobLoader> m_blobLoader;
    bool m_needsContinueDidReceiveResponseMessage { false };
    bool m_waitingForContinueDidReceiveResponseMessage { false };
    std::variant<std::nullptr_t, CyberCore::SharedBufferBuilder, Ref<CyberCore::FormData>, UniqueRef<CyberCore::ResourceError>> m_responseData;
    CyberCore::NetworkLoadMetrics m_networkLoadMetrics;
    bool m_didFinish { false };
    bool m_isDownload { false };
    RefPtr<CyberCore::FetchEvent> m_event;
    Function<void()> m_cancelledCallback;
    std::optional<CyberCore::ResourceResponse::CrossThreadData> m_preloadResponse;
    CyberCore::ResourceError m_preloadError;
};

} // namespace CyberKit

#endif // ENABLE(SERVICE_WORKER)
