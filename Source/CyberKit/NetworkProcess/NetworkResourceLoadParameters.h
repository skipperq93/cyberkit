/*
 * Copyright (C) 2012, 2013 Apple Inc. All rights reserved.
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

#include "NetworkLoadParameters.h"
#include "PolicyDecision.h"
#include "SandboxExtension.h"
#include "UserContentControllerIdentifier.h"
#include <CyberCore/ContentSecurityPolicyResponseHeaders.h>
#include <CyberCore/CrossOriginAccessControl.h>
#include <CyberCore/CrossOriginEmbedderPolicy.h>
#include <CyberCore/FetchOptions.h>
#include <CyberCore/NavigationRequester.h>
#include <CyberCore/ResourceLoaderIdentifier.h>
#include <CyberCore/SecurityContext.h>
#include <wtf/Seconds.h>

namespace IPC {
class Decoder;
class Encoder;
}

namespace CyberKit {

class NetworkResourceLoadParameters : public NetworkLoadParameters {
public:
    NetworkResourceLoadParameters() = default;
    NetworkResourceLoadParameters(
        NetworkLoadParameters&&
        , CyberCore::ResourceLoaderIdentifier
        , RefPtr<CyberCore::FormData>&& httpBody
        , std::optional<Vector<SandboxExtension::Handle>>&& sandboxExtensionIfHttpBody
        , std::optional<SandboxExtension::Handle>&& sandboxExtensionIflocalFile
        , Seconds maximumBufferingTime
        , CyberCore::FetchOptions&&
        , std::optional<CyberCore::ContentSecurityPolicyResponseHeaders>&& cspResponseHeaders
        , URL&& parentFrameURL
        , URL&& frameURL
        , CyberCore::CrossOriginEmbedderPolicy parentCrossOriginEmbedderPolicy
        , CyberCore::CrossOriginEmbedderPolicy
        , CyberCore::HTTPHeaderMap&& originalRequestHeaders
        , bool shouldRestrictHTTPResponseAccess
        , CyberCore::PreflightPolicy
        , bool shouldEnableCrossOriginResourcePolicy
        , Vector<RefPtr<CyberCore::SecurityOrigin>>&& frameAncestorOrigins
        , bool pageHasResourceLoadClient
        , std::optional<CyberCore::FrameIdentifier> parentFrameID
        , bool crossOriginAccessControlCheckEnabled
        , URL&& documentURL
        , bool isCrossOriginOpenerPolicyEnabled
        , bool isClearSiteDataHeaderEnabled
        , bool isDisplayingInitialEmptyDocument
        , CyberCore::SandboxFlags effectiveSandboxFlags
        , URL&& openerURL
        , CyberCore::CrossOriginOpenerPolicy&& sourceCrossOriginOpenerPolicy
        , uint64_t navigationID
        , std::optional<CyberCore::NavigationRequester>&&
#if ENABLE(SERVICE_WORKER)
        , CyberCore::ServiceWorkersMode
        , std::optional<CyberCore::ServiceWorkerRegistrationIdentifier>
        , OptionSet<CyberCore::HTTPHeadersToKeepFromCleaning>
        , std::optional<CyberCore::FetchIdentifier> navigationPreloadIdentifier
#endif
#if ENABLE(CONTENT_EXTENSIONS)
        , URL&& mainDocumentURL
        , std::optional<UserContentControllerIdentifier>
#endif
    );
    
    std::optional<Vector<SandboxExtension::Handle>> sandboxExtensionsIfHttpBody() const;
    std::optional<SandboxExtension::Handle> sandboxExtensionIflocalFile() const;

    RefPtr<CyberCore::SecurityOrigin> parentOrigin() const;

    CyberCore::ResourceLoaderIdentifier identifier;
    Vector<RefPtr<SandboxExtension>> requestBodySandboxExtensions; // Created automatically for the sender.
    RefPtr<SandboxExtension> resourceSandboxExtension; // Created automatically for the sender.
    Seconds maximumBufferingTime;
    CyberCore::FetchOptions options;
    std::optional<CyberCore::ContentSecurityPolicyResponseHeaders> cspResponseHeaders;
    URL parentFrameURL;
    URL frameURL;
    CyberCore::CrossOriginEmbedderPolicy parentCrossOriginEmbedderPolicy;
    CyberCore::CrossOriginEmbedderPolicy crossOriginEmbedderPolicy;
    CyberCore::HTTPHeaderMap originalRequestHeaders;
    bool shouldRestrictHTTPResponseAccess { false };
    CyberCore::PreflightPolicy preflightPolicy { CyberCore::PreflightPolicy::Consider };
    bool shouldEnableCrossOriginResourcePolicy { false };
    Vector<RefPtr<CyberCore::SecurityOrigin>> frameAncestorOrigins;
    bool pageHasResourceLoadClient { false };
    std::optional<CyberCore::FrameIdentifier> parentFrameID;
    bool crossOriginAccessControlCheckEnabled { true };
    URL documentURL;

    bool isCrossOriginOpenerPolicyEnabled { false };
    bool isClearSiteDataHeaderEnabled { false };
    bool isDisplayingInitialEmptyDocument { false };
    CyberCore::SandboxFlags effectiveSandboxFlags { CyberCore::SandboxNone };
    URL openerURL;
    CyberCore::CrossOriginOpenerPolicy sourceCrossOriginOpenerPolicy;
    uint64_t navigationID { 0 };
    std::optional<CyberCore::NavigationRequester> navigationRequester;

#if ENABLE(SERVICE_WORKER)
    CyberCore::ServiceWorkersMode serviceWorkersMode { CyberCore::ServiceWorkersMode::None };
    std::optional<CyberCore::ServiceWorkerRegistrationIdentifier> serviceWorkerRegistrationIdentifier;
    OptionSet<CyberCore::HTTPHeadersToKeepFromCleaning> httpHeadersToKeep;
    std::optional<CyberCore::FetchIdentifier> navigationPreloadIdentifier;
#endif

#if ENABLE(CONTENT_EXTENSIONS)
    URL mainDocumentURL;
    std::optional<UserContentControllerIdentifier> userContentControllerIdentifier;
#endif
};

} // namespace CyberKit
