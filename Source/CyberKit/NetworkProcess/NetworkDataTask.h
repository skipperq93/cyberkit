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

#include "DownloadID.h"
#include "SandboxExtension.h"
#include <CyberCore/Credential.h>
#include <CyberCore/FrameLoaderTypes.h>
#include <CyberCore/NetworkLoadMetrics.h>
#include <CyberCore/ResourceLoaderOptions.h>
#include <CyberCore/ResourceRequest.h>
#include <CyberCore/StoredCredentialsPolicy.h>
#include <CyberCore/Timer.h>
#include <pal/SessionID.h>
#include <wtf/CompletionHandler.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/text/WTFString.h>

namespace CyberCore {
class AuthenticationChallenge;
class ResourceError;
class ResourceResponse;
class SharedBuffer;
}

namespace CyberKit {

class NetworkLoadParameters;
class NetworkSession;
class PendingDownload;
enum class AuthenticationChallengeDisposition : uint8_t;
enum class NegotiatedLegacyTLS : bool;

using RedirectCompletionHandler = CompletionHandler<void(CyberCore::ResourceRequest&&)>;
using ChallengeCompletionHandler = CompletionHandler<void(AuthenticationChallengeDisposition, const CyberCore::Credential&)>;
using ResponseCompletionHandler = CompletionHandler<void(CyberCore::PolicyAction)>;

class NetworkDataTaskClient {
public:
    virtual void willPerformHTTPRedirection(CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&, RedirectCompletionHandler&&) = 0;
    virtual void didReceiveChallenge(CyberCore::AuthenticationChallenge&&, NegotiatedLegacyTLS, ChallengeCompletionHandler&&) = 0;
    virtual void didReceiveResponse(CyberCore::ResourceResponse&&, NegotiatedLegacyTLS, ResponseCompletionHandler&&) = 0;
    virtual void didReceiveData(Ref<CyberCore::SharedBuffer>&&) = 0;
    virtual void didCompleteWithError(const CyberCore::ResourceError&, const CyberCore::NetworkLoadMetrics&) = 0;
    virtual void didSendData(uint64_t totalBytesSent, uint64_t totalBytesExpectedToSend) = 0;
    virtual void wasBlocked() = 0;
    virtual void cannotShowURL() = 0;
    virtual void wasBlockedByRestrictions() = 0;

    virtual bool shouldCaptureExtraNetworkLoadMetrics() const { return false; }

    void didCompleteWithError(const CyberCore::ResourceError& error)
    {
        CyberCore::NetworkLoadMetrics emptyMetrics;
        didCompleteWithError(error, emptyMetrics);
    }

    virtual ~NetworkDataTaskClient() { }
};

class NetworkDataTask : public ThreadSafeRefCounted<NetworkDataTask, WTF::DestructionThread::Main>, public CanMakeWeakPtr<NetworkDataTask> {
public:
    static Ref<NetworkDataTask> create(NetworkSession&, NetworkDataTaskClient&, const NetworkLoadParameters&);

    virtual ~NetworkDataTask();

    virtual void cancel() = 0;
    virtual void resume() = 0;
    virtual void invalidateAndCancel() = 0;

    void didReceiveResponse(CyberCore::ResourceResponse&&, NegotiatedLegacyTLS, ResponseCompletionHandler&&);
    bool shouldCaptureExtraNetworkLoadMetrics() const;

    enum class State {
        Running,
        Suspended,
        Canceling,
        Completed
    };
    virtual State state() const = 0;

    NetworkDataTaskClient* client() const { return m_client; }
    void clearClient() { m_client = nullptr; }

    DownloadID pendingDownloadID() const { return m_pendingDownloadID; }
    PendingDownload* pendingDownload() const { return m_pendingDownload; }
    void setPendingDownloadID(DownloadID downloadID)
    {
        ASSERT(!m_pendingDownloadID.downloadID());
        ASSERT(downloadID.downloadID());
        m_pendingDownloadID = downloadID;
    }
    void setPendingDownload(PendingDownload& pendingDownload)
    {
        ASSERT(!m_pendingDownload);
        m_pendingDownload = &pendingDownload;
    }

    virtual void setPendingDownloadLocation(const String& filename, SandboxExtension::Handle&&, bool /*allowOverwrite*/) { m_pendingDownloadLocation = filename; }
    const String& pendingDownloadLocation() const { return m_pendingDownloadLocation; }
    bool isDownload() const { return !!m_pendingDownloadID.downloadID(); }

    const CyberCore::ResourceRequest& firstRequest() const { return m_firstRequest; }
    virtual String suggestedFilename() const { return String(); }
    void setSuggestedFilename(const String& suggestedName) { m_suggestedFilename = suggestedName; }
    const String& partition() { return m_partition; }

    bool isTopLevelNavigation() const { return m_dataTaskIsForMainFrameNavigation; }

    virtual String description() const;

    PAL::SessionID sessionID() const;

    NetworkSession* networkSession();

protected:
    NetworkDataTask(NetworkSession&, NetworkDataTaskClient&, const CyberCore::ResourceRequest&, CyberCore::StoredCredentialsPolicy, bool shouldClearReferrerOnHTTPSToHTTPRedirect, bool dataTaskIsForMainFrameNavigation);

    enum FailureType {
        NoFailure,
        BlockedFailure,
        InvalidURLFailure,
        RestrictedURLFailure
    };
    void failureTimerFired();
    void scheduleFailure(FailureType);

    FailureType m_scheduledFailureType { NoFailure };
    CyberCore::Timer m_failureTimer;
    WeakPtr<NetworkSession> m_session;
    NetworkDataTaskClient* m_client { nullptr };
    PendingDownload* m_pendingDownload { nullptr };
    DownloadID m_pendingDownloadID;
    String m_user;
    String m_password;
    String m_partition;
#if USE(CREDENTIAL_STORAGE_WITH_NETWORK_SESSION)
    CyberCore::Credential m_initialCredential;
#endif
    CyberCore::StoredCredentialsPolicy m_storedCredentialsPolicy { CyberCore::StoredCredentialsPolicy::DoNotUse };
    String m_lastHTTPMethod;
    String m_pendingDownloadLocation;
    CyberCore::ResourceRequest m_firstRequest;
    bool m_shouldClearReferrerOnHTTPSToHTTPRedirect { true };
    String m_suggestedFilename;
    bool m_dataTaskIsForMainFrameNavigation { false };
};

} // namespace CyberKit
