/*
 * Copyright (C) 2023 Apple Inc. All rights reserved.
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
#include <CyberCore/FrameIdentifier.h>
#include <CyberCore/PageIdentifier.h>
#include <CyberCore/ResourceRequest.h>
#include <CyberCore/ResourceResponse.h>
#include <CyberCore/ShouldRelaxThirdPartyCookieBlocking.h>

OBJC_CLASS NSArray;
OBJC_CLASS NSString;
OBJC_CLASS NSURLSessionTask;

namespace CyberCore {
class RegistrableDomain;
}

namespace CyberKit {

class NetworkTaskCocoa {
public:
    virtual ~NetworkTaskCocoa() = default;

    void willPerformHTTPRedirection(CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&, RedirectCompletionHandler&&);
    virtual std::optional<CyberCore::FrameIdentifier> frameID() const = 0;
    virtual std::optional<CyberCore::PageIdentifier> pageID() const = 0;

    CyberCore::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking() const { return m_shouldRelaxThirdPartyCookieBlocking; }

protected:
    NetworkTaskCocoa(NetworkSession&, CyberCore::ShouldRelaxThirdPartyCookieBlocking);

#if ENABLE(TRACKING_PREVENTION)
    static NSHTTPCookieStorage *statelessCookieStorage();
    bool shouldApplyCookiePolicyForThirdPartyCloaking() const;
    void applyCookiePolicyForThirdPartyCloaking(const CyberCore::ResourceRequest&);
    void blockCookies();
    void unblockCookies();
    static void updateTaskWithFirstPartyForSameSiteCookies(NSURLSessionTask*, const CyberCore::ResourceRequest&);
    bool needsFirstPartyCookieBlockingLatchModeQuirk(const URL& firstPartyURL, const URL& requestURL, const URL& redirectingURL) const;
    static NSString *lastRemoteIPAddress(NSURLSessionTask *);
    static CyberCore::RegistrableDomain lastCNAMEDomain(String);
#endif

    bool isAlwaysOnLoggingAllowed() const { return m_isAlwaysOnLoggingAllowed; }
    virtual NSURLSessionTask* task() const = 0;
    virtual CyberCore::StoredCredentialsPolicy storedCredentialsPolicy() const = 0;

private:
    WeakPtr<NetworkSession> m_networkSession;
#if ENABLE(TRACKING_PREVENTION)
    bool m_hasBeenSetToUseStatelessCookieStorage { false };
    Seconds m_ageCapForCNAMECloakedCookies { 24_h * 7 };
#endif
    bool m_isAlwaysOnLoggingAllowed { false };
    CyberCore::ShouldRelaxThirdPartyCookieBlocking m_shouldRelaxThirdPartyCookieBlocking { CyberCore::ShouldRelaxThirdPartyCookieBlocking::No };
};

} // namespace CyberKit
