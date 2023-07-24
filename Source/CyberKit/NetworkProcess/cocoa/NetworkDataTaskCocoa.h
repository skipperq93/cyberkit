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
#include <CyberCore/NetworkLoadMetrics.h>
#include <wtf/RetainPtr.h>

OBJC_CLASS NSHTTPCookieStorage;
OBJC_CLASS NSURLSessionDataTask;

namespace CyberCore {
class RegistrableDomain;
}

namespace CyberKit {

class Download;
class NetworkSessionCocoa;
struct SessionWrapper;

class NetworkDataTaskCocoa final : public NetworkDataTask {
public:
    static Ref<NetworkDataTask> create(NetworkSession& session, NetworkDataTaskClient& client, const CyberCore::ResourceRequest& request, CyberCore::FrameIdentifier frameID, CyberCore::PageIdentifier pageID, CyberCore::StoredCredentialsPolicy storedCredentialsPolicy, CyberCore::ContentSniffingPolicy shouldContentSniff, CyberCore::ContentEncodingSniffingPolicy shouldContentEncodingSniff, bool shouldClearReferrerOnHTTPSToHTTPRedirect, PreconnectOnly shouldPreconnectOnly, bool dataTaskIsForMainFrameNavigation, bool dataTaskIsForMainResourceNavigationForAnyFrame, Optional<NetworkActivityTracker> networkActivityTracker, Optional<NavigatingToAppBoundDomain> isNavigatingToAppBoundDomain, CyberCore::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking)
    {
        return adoptRef(*new NetworkDataTaskCocoa(session, client, request, frameID, pageID, storedCredentialsPolicy, shouldContentSniff, shouldContentEncodingSniff, shouldClearReferrerOnHTTPSToHTTPRedirect, shouldPreconnectOnly, dataTaskIsForMainFrameNavigation, dataTaskIsForMainResourceNavigationForAnyFrame, networkActivityTracker, isNavigatingToAppBoundDomain, shouldRelaxThirdPartyCookieBlocking));
    }

    ~NetworkDataTaskCocoa();

    using TaskIdentifier = uint64_t;

    void didSendData(uint64_t totalBytesSent, uint64_t totalBytesExpectedToSend);
    void didReceiveChallenge(CyberCore::AuthenticationChallenge&&, NegotiatedLegacyTLS, ChallengeCompletionHandler&&);
    void didNegotiateModernTLS(const CyberCore::AuthenticationChallenge&);
    void didCompleteWithError(const CyberCore::ResourceError&, const CyberCore::NetworkLoadMetrics&);
    void didReceiveResponse(CyberCore::ResourceResponse&&, NegotiatedLegacyTLS, ResponseCompletionHandler&&);
    void didReceiveData(Ref<CyberCore::SharedBuffer>&&);

    void willPerformHTTPRedirection(CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&, RedirectCompletionHandler&&);
    void transferSandboxExtensionToDownload(Download&);

    void cancel() override;
    void resume() override;
    void invalidateAndCancel() override { }
    NetworkDataTask::State state() const override;

    void setPendingDownloadLocation(const String&, SandboxExtension::Handle&&, bool /*allowOverwrite*/) override;
    String suggestedFilename() const override;

    CyberCore::NetworkLoadMetrics& networkLoadMetrics() { return m_networkLoadMetrics; }

    CyberCore::FrameIdentifier frameID() const { return m_frameID; };
    CyberCore::PageIdentifier pageID() const { return m_pageID; };
    CyberCore::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking() const { return m_shouldRelaxThirdPartyCookieBlocking; }

    String description() const override;

    void setH2PingCallback(const URL&, CompletionHandler<void(Expected<WTF::Seconds, CyberCore::ResourceError>&&)>&&) override;

private:
    NetworkDataTaskCocoa(NetworkSession&, NetworkDataTaskClient&, const CyberCore::ResourceRequest&, CyberCore::FrameIdentifier, CyberCore::PageIdentifier, CyberCore::StoredCredentialsPolicy, CyberCore::ContentSniffingPolicy, CyberCore::ContentEncodingSniffingPolicy, bool shouldClearReferrerOnHTTPSToHTTPRedirect, PreconnectOnly, bool dataTaskIsForMainFrameNavigation, bool dataTaskIsForMainResourceNavigationForAnyFrame, Optional<NetworkActivityTracker>, Optional<NavigatingToAppBoundDomain> isNavigatingToAppBoundDomain, CyberCore::ShouldRelaxThirdPartyCookieBlocking);

    bool tryPasswordBasedAuthentication(const CyberCore::AuthenticationChallenge&, ChallengeCompletionHandler&);
    void applySniffingPoliciesAndBindRequestToInferfaceIfNeeded(__strong NSURLRequest*&, bool shouldContentSniff, bool shouldContentEncodingSniff);

#if ENABLE(RESOURCE_LOAD_STATISTICS)
    static NSHTTPCookieStorage *statelessCookieStorage();
#if HAVE(CFNETWORK_CNAME_AND_COOKIE_TRANSFORM_SPI)
    void updateFirstPartyInfoForSession(const URL&);
    void applyCookiePolicyForThirdPartyCNAMECloaking(const CyberCore::ResourceRequest&);
#endif
    void blockCookies();
    void unblockCookies();
    bool needsFirstPartyCookieBlockingLatchModeQuirk(const URL& firstPartyURL, const URL& requestURL, const URL& redirectingURL) const;
#endif
    bool isAlwaysOnLoggingAllowed() const;

    WeakPtr<SessionWrapper> m_sessionWrapper;
    RefPtr<SandboxExtension> m_sandboxExtension;
    RetainPtr<NSURLSessionDataTask> m_task;
    CyberCore::NetworkLoadMetrics m_networkLoadMetrics;
    CyberCore::FrameIdentifier m_frameID;
    CyberCore::PageIdentifier m_pageID;

#if ENABLE(RESOURCE_LOAD_STATISTICS)
    bool m_hasBeenSetToUseStatelessCookieStorage { false };
#if HAVE(CFNETWORK_CNAME_AND_COOKIE_TRANSFORM_SPI)
    Seconds m_ageCapForCNAMECloakedCookies { 24_h * 7 };
#endif
#endif

    bool m_isForMainResourceNavigationForAnyFrame { false };
    bool m_isAlwaysOnLoggingAllowed { false };
    CyberCore::ShouldRelaxThirdPartyCookieBlocking m_shouldRelaxThirdPartyCookieBlocking { CyberCore::ShouldRelaxThirdPartyCookieBlocking::No };
};

CyberCore::Credential serverTrustCredential(const CyberCore::AuthenticationChallenge&);

} // namespace CyberKit
