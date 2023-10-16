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
#include "NetworkTaskCocoa.h"
#include <CyberCore/NetworkLoadMetrics.h>
#include <CyberCore/PrivateClickMeasurement.h>
#include <wtf/RetainPtr.h>

OBJC_CLASS NSHTTPCookieStorage;
OBJC_CLASS NSURLSessionDataTask;
OBJC_CLASS NSMutableURLRequest;

namespace CyberCore {
class RegistrableDomain;
class SharedBuffer;
enum class AdvancedPrivacyProtections : uint16_t;
}

namespace CyberKit {

class Download;
class NetworkSessionCocoa;
struct SessionWrapper;

class NetworkDataTaskCocoa final : public NetworkDataTask, public NetworkTaskCocoa {
public:
    static Ref<NetworkDataTask> create(NetworkSession& session, NetworkDataTaskClient& client, const NetworkLoadParameters& parameters)
    {
        return adoptRef(*new NetworkDataTaskCocoa(session, client, parameters));
    }

    ~NetworkDataTaskCocoa();

    using TaskIdentifier = uint64_t;

    void didSendData(uint64_t totalBytesSent, uint64_t totalBytesExpectedToSend);
    void didReceiveChallenge(CyberCore::AuthenticationChallenge&&, NegotiatedLegacyTLS, ChallengeCompletionHandler&&);
    void didNegotiateModernTLS(const URL&);
    void didCompleteWithError(const CyberCore::ResourceError&, const CyberCore::NetworkLoadMetrics&);
    void didReceiveResponse(CyberCore::ResourceResponse&&, NegotiatedLegacyTLS, PrivateRelayed, ResponseCompletionHandler&&);
    void didReceiveData(const CyberCore::SharedBuffer&);

    void willPerformHTTPRedirection(CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&, RedirectCompletionHandler&&);
    void transferSandboxExtensionToDownload(Download&);

    void cancel() override;
    void resume() override;
    void invalidateAndCancel() override { }
    NetworkDataTask::State state() const override;

    void setPendingDownloadLocation(const String&, SandboxExtension::Handle&&, bool /*allowOverwrite*/) override;
    String suggestedFilename() const override;

    CyberCore::NetworkLoadMetrics& networkLoadMetrics() { return m_networkLoadMetrics; }

    std::optional<CyberCore::FrameIdentifier> frameID() const final { return m_frameID; };
    std::optional<CyberCore::PageIdentifier> pageID() const final { return m_pageID; };

    String description() const override;

    void setH2PingCallback(const URL&, CompletionHandler<void(Expected<WTF::Seconds, CyberCore::ResourceError>&&)>&&) override;
    void setPriority(CyberCore::ResourceLoadPriority) override;
#if ENABLE(INSPECTOR_NETWORK_THROTTLING)
    void setEmulatedConditions(const std::optional<int64_t>& bytesPerSecondLimit) override;
#endif

    void checkTAO(const CyberCore::ResourceResponse&);

private:
    NetworkDataTaskCocoa(NetworkSession&, NetworkDataTaskClient&, const NetworkLoadParameters&);

    bool tryPasswordBasedAuthentication(const CyberCore::AuthenticationChallenge&, ChallengeCompletionHandler&);
    void applySniffingPoliciesAndBindRequestToInferfaceIfNeeded(RetainPtr<NSURLRequest>&, bool shouldContentSniff, CyberCore::ContentEncodingSniffingPolicy);

#if ENABLE(TRACKING_PREVENTION)
    void updateFirstPartyInfoForSession(const URL&);
#endif

    NSURLSessionTask* task() const final;
    CyberCore::StoredCredentialsPolicy storedCredentialsPolicy() const final { return m_storedCredentialsPolicy; }

    WeakPtr<SessionWrapper> m_sessionWrapper;
    RefPtr<SandboxExtension> m_sandboxExtension;
    RetainPtr<NSURLSessionDataTask> m_task;
    CyberCore::NetworkLoadMetrics m_networkLoadMetrics;
    CyberCore::FrameIdentifier m_frameID;
    CyberCore::PageIdentifier m_pageID;
    WebPageProxyIdentifier m_webPageProxyID;

    bool m_isForMainResourceNavigationForAnyFrame { false };
    RefPtr<CyberCore::SecurityOrigin> m_sourceOrigin;
};

CyberCore::Credential serverTrustCredential(const CyberCore::AuthenticationChallenge&);
void setPCMDataCarriedOnRequest(CyberCore::PrivateClickMeasurement::PcmDataCarried, NSMutableURLRequest *);

void enableAdvancedPrivacyProtections(NSMutableURLRequest *, OptionSet<CyberCore::AdvancedPrivacyProtections>);

} // namespace CyberKit
