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
#include "NetworkLoadParameters.h"
#include <CyberCore/CurlRequestClient.h>
#include <CyberCore/FrameIdentifier.h>
#include <CyberCore/PageIdentifier.h>
#include <CyberCore/ProtectionSpace.h>
#include <CyberCore/ResourceResponse.h>
#include <CyberCore/ShouldRelaxThirdPartyCookieBlocking.h>
#include <wtf/FileSystem.h>
#include <wtf/MonotonicTime.h>

namespace CyberCore {
class CurlRequest;
class SharedBuffer;
}

namespace WebKit {

class NetworkDataTaskCurl final : public NetworkDataTask, public CyberCore::CurlRequestClient {
public:
    static Ref<NetworkDataTask> create(NetworkSession& session, NetworkDataTaskClient& client, const NetworkLoadParameters& parameters)
    {
        return adoptRef(*new NetworkDataTaskCurl(session, client, parameters));
    }

    ~NetworkDataTaskCurl();

    void ref() override { NetworkDataTask::ref(); }
    void deref() override { NetworkDataTask::deref(); }

private:
    enum class RequestStatus {
        NewRequest,
        ReusedRequest
    };

    NetworkDataTaskCurl(NetworkSession&, NetworkDataTaskClient&, const NetworkLoadParameters&);

    void cancel() override;
    void resume() override;
    void invalidateAndCancel() override;
    NetworkDataTask::State state() const override;

    Ref<CyberCore::CurlRequest> createCurlRequest(CyberCore::ResourceRequest&&, RequestStatus = RequestStatus::NewRequest);
    void curlDidSendData(CyberCore::CurlRequest&, unsigned long long, unsigned long long) override;
    void curlDidReceiveResponse(CyberCore::CurlRequest&, CyberCore::CurlResponse&&) override;
    void curlDidReceiveData(CyberCore::CurlRequest&, const CyberCore::SharedBuffer&) override;
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

    bool isThirdPartyRequest(const CyberCore::ResourceRequest&);
    bool shouldBlockCookies(const CyberCore::ResourceRequest&);
    void blockCookies();
    void unblockCookies();

    void updateNetworkLoadMetrics(CyberCore::NetworkLoadMetrics&);

    String suggestedFilename() const override;
    void deleteDownloadFile();

    CyberCore::FrameIdentifier m_frameID;
    CyberCore::PageIdentifier m_pageID;
    CyberCore::ShouldRelaxThirdPartyCookieBlocking m_shouldRelaxThirdPartyCookieBlocking { CyberCore::ShouldRelaxThirdPartyCookieBlocking::No };
    RefPtr<CyberCore::SecurityOrigin> m_sourceOrigin;

    State m_state { State::Suspended };

    RefPtr<CyberCore::CurlRequest> m_curlRequest;
    CyberCore::ResourceResponse m_response;
    unsigned m_redirectCount { 0 };
    unsigned m_authFailureCount { 0 };

    FileSystem::PlatformFileHandle m_downloadDestinationFile { FileSystem::invalidPlatformFileHandle };

    bool m_blockingCookies { false };

    MonotonicTime m_startTime;
    bool m_failsTAOCheck { false };
    bool m_hasCrossOriginRedirect { false };
};

} // namespace WebKit
