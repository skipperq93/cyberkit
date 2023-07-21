/*
 * Copyright (C) 2018 Sony Interactive Entertainment Inc.
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

#include "NetworkDataTask.h"
#include <CyberCore/CurlRequestClient.h>
#include <CyberCore/ProtectionSpace.h>
#include <CyberCore/ResourceResponse.h>

namespace CyberCore {
class CurlRequest;
}

namespace CyberKit {

class NetworkDataTaskCurl final : public NetworkDataTask, public CyberCore::CurlRequestClient {
public:
    static Ref<NetworkDataTask> create(NetworkSession& session, NetworkDataTaskClient& client, const CyberCore::ResourceRequest& request, CyberCore::StoredCredentialsPolicy storedCredentialsPolicy, CyberCore::ContentSniffingPolicy shouldContentSniff, CyberCore::ContentEncodingSniffingPolicy shouldContentEncodingSniff, bool shouldClearReferrerOnHTTPSToHTTPRedirect, bool dataTaskIsForMainFrameNavigation)
    {
        return adoptRef(*new NetworkDataTaskCurl(session, client, request, storedCredentialsPolicy, shouldContentSniff, shouldContentEncodingSniff, shouldClearReferrerOnHTTPSToHTTPRedirect, dataTaskIsForMainFrameNavigation));
    }

    ~NetworkDataTaskCurl();

    void ref() override { RefCounted<NetworkDataTask>::ref(); }
    void deref() override { RefCounted<NetworkDataTask>::deref(); }

private:
    enum class RequestStatus {
        NewRequest,
        ReusedRequest
    };

    NetworkDataTaskCurl(NetworkSession&, NetworkDataTaskClient&, const CyberCore::ResourceRequest&, CyberCore::StoredCredentialsPolicy, CyberCore::ContentSniffingPolicy, CyberCore::ContentEncodingSniffingPolicy, bool shouldClearReferrerOnHTTPSToHTTPRedirect, bool dataTaskIsForMainFrameNavigation);

    void cancel() override;
    void resume() override;
    void invalidateAndCancel() override;
    NetworkDataTask::State state() const override;

    Ref<CyberCore::CurlRequest> createCurlRequest(CyberCore::ResourceRequest&&, RequestStatus = RequestStatus::NewRequest);
    void curlDidSendData(CyberCore::CurlRequest&, unsigned long long, unsigned long long) override;
    void curlDidReceiveResponse(CyberCore::CurlRequest&, CyberCore::CurlResponse&&) override;
    void curlDidReceiveBuffer(CyberCore::CurlRequest&, Ref<CyberCore::SharedBuffer>&&) override;
    void curlDidComplete(CyberCore::CurlRequest&, CyberCore::NetworkLoadMetrics&&) override;
    void curlDidFailWithError(CyberCore::CurlRequest&, CyberCore::ResourceError&&, CyberCore::CertificateInfo&&) override;

    void invokeDidReceiveResponse();

    bool shouldRedirectAsGET(const CyberCore::ResourceRequest&, bool crossOrigin);
    void willPerformHTTPRedirection();

    void tryHttpAuthentication(CyberCore::AuthenticationChallenge&&);
    void tryProxyAuthentication(CyberCore::AuthenticationChallenge&&);
    void restartWithCredential(const CyberCore::ProtectionSpace&, const CyberCore::Credential&);

    void tryServerTrustEvaluation(CyberCore::AuthenticationChallenge&&);

    void appendCookieHeader(CyberCore::ResourceRequest&);
    void handleCookieHeaders(const CyberCore::ResourceRequest&, const CyberCore::CurlResponse&);

    State m_state { State::Suspended };

    RefPtr<CyberCore::CurlRequest> m_curlRequest;
    CyberCore::ResourceResponse m_response;
    unsigned m_redirectCount { 0 };
    unsigned m_authFailureCount { 0 };
    MonotonicTime m_startTime;
};

} // namespace CyberKit
