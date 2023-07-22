/*
 * Copyright (C) 2016-2018 Apple Inc. All rights reserved.
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
#include "NetworkResourceLoadParameters.h"
#include <CyberCore/ResourceError.h>
#include <CyberCore/ResourceResponse.h>
#include <wtf/CompletionHandler.h>
#include <wtf/UniqueRef.h>
#include <wtf/WeakPtr.h>

namespace CyberKit {

class NetworkConnectionToWebProcess;
class NetworkLoadChecker;
class NetworkProcess;
class NetworkSchemeRegistry;

class PingLoad final : public CanMakeWeakPtr<PingLoad>, private NetworkDataTaskClient {
public:
    PingLoad(NetworkProcess&, PAL::SessionID, NetworkResourceLoadParameters&&, CompletionHandler<void(const CyberCore::ResourceError&, const CyberCore::ResourceResponse&)>&&);
    PingLoad(NetworkConnectionToWebProcess&, NetworkResourceLoadParameters&&, CompletionHandler<void(const CyberCore::ResourceError&, const CyberCore::ResourceResponse&)>&&);

private:
    ~PingLoad();
    void initialize(NetworkProcess&);

    const URL& currentURL() const;

    void willPerformHTTPRedirection(CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&, RedirectCompletionHandler&&) final;
    void didReceiveChallenge(CyberCore::AuthenticationChallenge&&, NegotiatedLegacyTLS, ChallengeCompletionHandler&&) final;
    void didReceiveResponse(CyberCore::ResourceResponse&&, NegotiatedLegacyTLS, ResponseCompletionHandler&&) final;
    void didReceiveData(Ref<CyberCore::SharedBuffer>&&) final;
    void didCompleteWithError(const CyberCore::ResourceError&, const CyberCore::NetworkLoadMetrics&) final;
    void didSendData(uint64_t totalBytesSent, uint64_t totalBytesExpectedToSend) final;
    void wasBlocked() final;
    void cannotShowURL() final;
    void wasBlockedByRestrictions() final;
    void timeoutTimerFired();

    void loadRequest(NetworkProcess&, CyberCore::ResourceRequest&&);

    void didFinish(const CyberCore::ResourceError& = { }, const CyberCore::ResourceResponse& response = { });
    
    PAL::SessionID m_sessionID;
    NetworkResourceLoadParameters m_parameters;
    CompletionHandler<void(const CyberCore::ResourceError&, const CyberCore::ResourceResponse&)> m_completionHandler;
    RefPtr<NetworkDataTask> m_task;
    CyberCore::Timer m_timeoutTimer;
    UniqueRef<NetworkLoadChecker> m_networkLoadChecker;
    Vector<RefPtr<CyberCore::BlobDataFileReference>> m_blobFiles;
};

}
