/*
 * Copyright (C) 2019 Apple Inc. All rights reserved.
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

#include "NetworkCacheEntry.h"
#include "NetworkLoadClient.h"
#include <CyberCore/ContentSecurityPolicyResponseHeaders.h>
#include <CyberCore/CrossOriginEmbedderPolicy.h>
#include <CyberCore/FetchOptions.h>
#include <CyberCore/ServiceWorkerJobData.h>
#include <wtf/CompletionHandler.h>
#include <wtf/WeakPtr.h>

namespace CyberCore {
struct ServiceWorkerJobData;
struct WorkerFetchResult;
class TextResourceDecoder;
}

namespace WebKit {

class NetworkLoad;
class NetworkSession;

class ServiceWorkerSoftUpdateLoader final : public NetworkLoadClient, public CanMakeWeakPtr<ServiceWorkerSoftUpdateLoader> {
    WTF_MAKE_FAST_ALLOCATED;
public:
    using Handler = CompletionHandler<void(CyberCore::WorkerFetchResult&&)>;
    ServiceWorkerSoftUpdateLoader(NetworkSession&, CyberCore::ServiceWorkerJobData&&, bool shouldRefreshCache, CyberCore::ResourceRequest&&, Handler&&);
    ~ServiceWorkerSoftUpdateLoader();
    
private:
    // NetworkLoadClient.
    void didSendData(uint64_t bytesSent, uint64_t totalBytesToBeSent) final { }
    bool isSynchronous() const final { return false; }
    bool isAllowedToAskUserForCredentials() const final { return false; }
    void willSendRedirectedRequest(CyberCore::ResourceRequest&&, CyberCore::ResourceRequest&& redirectRequest, CyberCore::ResourceResponse&& redirectResponse) final;
    void didReceiveResponse(CyberCore::ResourceResponse&&, PrivateRelayed, ResponseCompletionHandler&&) final;
    void didReceiveBuffer(const CyberCore::FragmentedSharedBuffer&, uint64_t reportedEncodedDataLength) final;
    void didFinishLoading(const CyberCore::NetworkLoadMetrics&) final;
    void didFailLoading(const CyberCore::ResourceError&) final;

    void loadWithCacheEntry(NetworkCache::Entry&);
    void loadFromNetwork(NetworkSession&, CyberCore::ResourceRequest&&);
    void fail(CyberCore::ResourceError&&);
    void didComplete();
    CyberCore::ResourceError processResponse(const CyberCore::ResourceResponse&);

    Handler m_completionHandler;
    CyberCore::ServiceWorkerJobData m_jobData;
    std::unique_ptr<NetworkLoad> m_networkLoad;
    WeakPtr<NetworkSession> m_session;

    String m_responseEncoding;
    String m_referrerPolicy;
    CyberCore::ContentSecurityPolicyResponseHeaders m_contentSecurityPolicy;
    CyberCore::CrossOriginEmbedderPolicy m_crossOriginEmbedderPolicy;

    std::unique_ptr<NetworkCache::Entry> m_cacheEntry;
    RefPtr<CyberCore::TextResourceDecoder> m_decoder;
    StringBuilder m_script;
    CyberCore::CertificateInfo m_certificateInfo;
};

} // namespace WebKit

#endif // ENABLE(SERVICE_WORKER)

