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

#include "DataReference.h"
#include "FormDataReference.h"
#include "MessageReceiver.h"
#include "MessageSender.h"
#include <CyberCore/FetchIdentifier.h>
#include <CyberCore/ResourceError.h>
#include <CyberCore/ResourceLoader.h>
#include <wtf/CompletionHandler.h>

namespace CyberKit {

class WebSWClientConnection;
class WebServiceWorkerProvider;

class ServiceWorkerClientFetch final : public RefCounted<ServiceWorkerClientFetch>, public IPC::MessageReceiver {
public:
    enum class Result { Succeeded, Cancelled, Unhandled };
    using Callback = WTF::CompletionHandler<void(Result)>;

    static Ref<ServiceWorkerClientFetch> create(WebServiceWorkerProvider&, Ref<CyberCore::ResourceLoader>&&, CyberCore::FetchIdentifier, Ref<WebSWClientConnection>&&, bool shouldClearReferrerOnHTTPSToHTTPRedirect, Callback&&);
    ~ServiceWorkerClientFetch();

    void start();

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;
    void cancel();

    bool isOngoing() const { return !!m_callback; }

private:
    ServiceWorkerClientFetch(WebServiceWorkerProvider&, Ref<CyberCore::ResourceLoader>&&, CyberCore::FetchIdentifier, Ref<WebSWClientConnection>&&, bool shouldClearReferrerOnHTTPSToHTTPRedirect, Callback&&);

    Optional<CyberCore::ResourceError> validateResponse(const CyberCore::ResourceResponse&);

    void didReceiveResponse(CyberCore::ResourceResponse&&, bool needsContinueDidReceiveResponseMessage);
    void didReceiveRedirectResponse(CyberCore::ResourceResponse&&);
    void didReceiveData(const IPC::DataReference&, int64_t encodedDataLength);
    void didReceiveFormData(const IPC::FormDataReference&);
    void didFinish();
    void didFail(CyberCore::ResourceError&&);
    void didNotHandle();

    WebServiceWorkerProvider& m_serviceWorkerProvider;
    RefPtr<CyberCore::ResourceLoader> m_loader;
    CyberCore::FetchIdentifier m_identifier;
    Ref<WebSWClientConnection> m_connection;
    Callback m_callback;
    bool m_shouldClearReferrerOnHTTPSToHTTPRedirect { true };
    int64_t m_encodedDataLength { 0 };
    bool m_didFail { false };
    CyberCore::ServiceWorkerRegistrationIdentifier m_serviceWorkerRegistrationIdentifier;
};

} // namespace CyberKit

#endif // ENABLE(SERVICE_WORKER)
