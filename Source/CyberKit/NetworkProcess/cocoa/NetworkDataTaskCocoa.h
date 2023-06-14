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

#include "NetworkActivityTracker.h"
#include "NetworkDataTask.h"
#include "NetworkLoadParameters.h"
#include "NetworkProximityAssertion.h"
#include <CyberCore/NetworkLoadMetrics.h>
#include <wtf/RetainPtr.h>

OBJC_CLASS NSHTTPCookieStorage;
OBJC_CLASS NSURLSessionDataTask;

namespace CyberKit {

class NetworkSessionCocoa;

class NetworkDataTaskCocoa final : public NetworkDataTask {
    friend class NetworkSessionCocoa;
public:
    static Ref<NetworkDataTask> create(NetworkSession& session, NetworkDataTaskClient& client, const CyberCore::ResourceRequest& request, uint64_t frameID, uint64_t pageID, CyberCore::StoredCredentialsPolicy storedCredentialsPolicy, CyberCore::ContentSniffingPolicy shouldContentSniff, CyberCore::ContentEncodingSniffingPolicy shouldContentEncodingSniff, bool shouldClearReferrerOnHTTPSToHTTPRedirect, PreconnectOnly shouldPreconnectOnly, bool dataTaskIsForMainFrameNavigation, Optional<NetworkActivityTracker> networkActivityTracker)
    {
        return adoptRef(*new NetworkDataTaskCocoa(session, client, request, frameID, pageID, storedCredentialsPolicy, shouldContentSniff, shouldContentEncodingSniff, shouldClearReferrerOnHTTPSToHTTPRedirect, shouldPreconnectOnly, dataTaskIsForMainFrameNavigation, networkActivityTracker));
    }

    ~NetworkDataTaskCocoa();

    typedef uint64_t TaskIdentifier;

    void didSendData(uint64_t totalBytesSent, uint64_t totalBytesExpectedToSend);
    void didReceiveChallenge(CyberCore::AuthenticationChallenge&&, ChallengeCompletionHandler&&);
    void didCompleteWithError(const CyberCore::ResourceError&, const CyberCore::NetworkLoadMetrics&);
    void didReceiveData(Ref<CyberCore::SharedBuffer>&&);

    void willPerformHTTPRedirection(CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&, RedirectCompletionHandler&&);
    void transferSandboxExtensionToDownload(Download&);

    void suspend() override;
    void cancel() override;
    void resume() override;
    void invalidateAndCancel() override { }
    NetworkDataTask::State state() const override;

    void setPendingDownloadLocation(const String&, SandboxExtension::Handle&&, bool /*allowOverwrite*/) override;
    String suggestedFilename() const override;

    CyberCore::NetworkLoadMetrics& networkLoadMetrics() { return m_networkLoadMetrics; }

    uint64_t frameID() const { return m_frameID; };
    uint64_t pageID() const { return m_pageID; };

#if ENABLE(PROXIMITY_NETWORKING)
    void holdProximityAssertion(NetworkProximityAssertion& assertion)
    {
        ASSERT(!m_proximityAssertionToken);
        m_proximityAssertionToken.emplace(assertion);
    }
#endif

    String description() const override;

private:
    NetworkDataTaskCocoa(NetworkSession&, NetworkDataTaskClient&, const CyberCore::ResourceRequest&, uint64_t frameID, uint64_t pageID, CyberCore::StoredCredentialsPolicy, CyberCore::ContentSniffingPolicy, CyberCore::ContentEncodingSniffingPolicy, bool shouldClearReferrerOnHTTPSToHTTPRedirect, PreconnectOnly, bool dataTaskIsForMainFrameNavigation, Optional<NetworkActivityTracker>);

    bool tryPasswordBasedAuthentication(const CyberCore::AuthenticationChallenge&, ChallengeCompletionHandler&);
    void applySniffingPoliciesAndBindRequestToInferfaceIfNeeded(__strong NSURLRequest*&, bool shouldContentSniff, bool shouldContentEncodingSniff);

#if ENABLE(RESOURCE_LOAD_STATISTICS)
    static NSHTTPCookieStorage *statelessCookieStorage();
    void applyCookieBlockingPolicy(bool shouldBlock);
#endif
    bool isThirdPartyRequest(const CyberCore::ResourceRequest&);
    bool isAlwaysOnLoggingAllowed() const;

    RefPtr<SandboxExtension> m_sandboxExtension;
    RetainPtr<NSURLSessionDataTask> m_task;
    CyberCore::NetworkLoadMetrics m_networkLoadMetrics;
    uint64_t m_frameID;
    uint64_t m_pageID;

#if ENABLE(RESOURCE_LOAD_STATISTICS)
    bool m_hasBeenSetToUseStatelessCookieStorage { false };
#endif

#if ENABLE(PROXIMITY_NETWORKING)
    Optional<NetworkProximityAssertion::Token> m_proximityAssertionToken;
#endif
};

CyberCore::Credential serverTrustCredential(const CyberCore::AuthenticationChallenge&);

} // namespace CyberKit
