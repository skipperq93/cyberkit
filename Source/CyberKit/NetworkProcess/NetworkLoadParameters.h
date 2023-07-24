/*
 * Copyright (C) 2015 Apple Inc. All rights reserved.
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
#include "PolicyDecision.h"
#include "WebPageProxyIdentifier.h"
#include <CyberCore/BlobDataFileReference.h>
#include <CyberCore/FrameIdentifier.h>
#include <CyberCore/PageIdentifier.h>
#include <CyberCore/ResourceLoaderOptions.h>
#include <CyberCore/ResourceRequest.h>
#include <CyberCore/SecurityOrigin.h>
#include <CyberCore/ShouldRelaxThirdPartyCookieBlocking.h>
#include <wtf/EnumTraits.h>
#include <wtf/ProcessID.h>

namespace CyberKit {

enum class PreconnectOnly : bool { No, Yes };

class NetworkLoadParameters {
public:
    WebPageProxyIdentifier webPageProxyID;
    CyberCore::PageIdentifier webPageID;
    CyberCore::FrameIdentifier webFrameID;
    RefPtr<CyberCore::SecurityOrigin> topOrigin;
    WTF::ProcessID parentPID { 0 };
#if HAVE(AUDIT_TOKEN)
    Optional<audit_token_t> networkProcessAuditToken;
#endif
    CyberCore::ResourceRequest request;
    CyberCore::ContentSniffingPolicy contentSniffingPolicy { CyberCore::ContentSniffingPolicy::SniffContent };
    CyberCore::ContentEncodingSniffingPolicy contentEncodingSniffingPolicy { CyberCore::ContentEncodingSniffingPolicy::Sniff };
    CyberCore::StoredCredentialsPolicy storedCredentialsPolicy { CyberCore::StoredCredentialsPolicy::DoNotUse };
    CyberCore::ClientCredentialPolicy clientCredentialPolicy { CyberCore::ClientCredentialPolicy::CannotAskClientForCredentials };
    bool shouldClearReferrerOnHTTPSToHTTPRedirect { true };
    bool needsCertificateInfo { false };
    bool isMainFrameNavigation { false };
    bool isMainResourceNavigationForAnyFrame { false };
    CyberCore::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking { CyberCore::ShouldRelaxThirdPartyCookieBlocking::No };
    Vector<RefPtr<CyberCore::BlobDataFileReference>> blobFileReferences;
    PreconnectOnly shouldPreconnectOnly { PreconnectOnly::No };
    Optional<NetworkActivityTracker> networkActivityTracker;
    Optional<NavigatingToAppBoundDomain> isNavigatingToAppBoundDomain { NavigatingToAppBoundDomain::No };
};

} // namespace CyberKit
