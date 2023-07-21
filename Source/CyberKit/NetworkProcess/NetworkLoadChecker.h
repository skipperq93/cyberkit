/*
 * Copyright (C) 2018-2019 Apple Inc. All rights reserved.
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

#include "UserContentControllerIdentifier.h"
#include <CyberCore/ContentExtensionActions.h>
#include <CyberCore/ContentSecurityPolicyResponseHeaders.h>
#include <CyberCore/FetchOptions.h>
#include <CyberCore/NetworkLoadInformation.h>
#include <CyberCore/ResourceError.h>
#include <wtf/CompletionHandler.h>
#include <wtf/Expected.h>
#include <wtf/Variant.h>
#include <wtf/WeakPtr.h>

namespace CyberCore {
class ContentSecurityPolicy;
struct ContentSecurityPolicyClient;
class SecurityOrigin;
enum class PreflightPolicy : uint8_t;
enum class StoredCredentialsPolicy : uint8_t;
}

namespace CyberKit {

class NetworkCORSPreflightChecker;
class NetworkProcess;

class NetworkLoadChecker : public CanMakeWeakPtr<NetworkLoadChecker> {
    WTF_MAKE_FAST_ALLOCATED;
public:
    enum class LoadType : bool { MainFrame, Other };

    NetworkLoadChecker(NetworkProcess&, CyberCore::FetchOptions&&, PAL::SessionID, CyberCore::PageIdentifier, uint64_t frameID, CyberCore::HTTPHeaderMap&&, URL&&, RefPtr<CyberCore::SecurityOrigin>&&, CyberCore::PreflightPolicy, String&& referrer, bool isHTTPSUpgradeEnabled = false, bool shouldCaptureExtraNetworkLoadMetrics = false, LoadType requestLoadType = LoadType::Other);
    ~NetworkLoadChecker();

    struct RedirectionTriplet {
        CyberCore::ResourceRequest request;
        CyberCore::ResourceRequest redirectRequest;
        CyberCore::ResourceResponse redirectResponse;
    };

    using RequestOrRedirectionTripletOrError = Variant<CyberCore::ResourceRequest, RedirectionTriplet, CyberCore::ResourceError>;
    using ValidationHandler = CompletionHandler<void(RequestOrRedirectionTripletOrError&&)>;
    void check(CyberCore::ResourceRequest&&, CyberCore::ContentSecurityPolicyClient*, ValidationHandler&&);

    using RedirectionRequestOrError = Expected<RedirectionTriplet, CyberCore::ResourceError>;
    using RedirectionValidationHandler = CompletionHandler<void(RedirectionRequestOrError&&)>;
    void checkRedirection(CyberCore::ResourceRequest&& request, CyberCore::ResourceRequest&& redirectRequest, CyberCore::ResourceResponse&& redirectResponse, CyberCore::ContentSecurityPolicyClient*, RedirectionValidationHandler&&);

    CyberCore::ResourceError validateResponse(CyberCore::ResourceResponse&);

    void setCSPResponseHeaders(CyberCore::ContentSecurityPolicyResponseHeaders&& headers) { m_cspResponseHeaders = WTFMove(headers); }
#if ENABLE(CONTENT_EXTENSIONS)
    void setContentExtensionController(URL&& mainDocumentURL, Optional<UserContentControllerIdentifier> identifier)
    {
        m_mainDocumentURL = WTFMove(mainDocumentURL);
        m_userContentControllerIdentifier = identifier;
    }
#endif

    const URL& url() const { return m_url; }
    CyberCore::StoredCredentialsPolicy storedCredentialsPolicy() const { return m_storedCredentialsPolicy; }

    CyberCore::NetworkLoadInformation takeNetworkLoadInformation() { return WTFMove(m_loadInformation); }
    void storeRedirectionIfNeeded(const CyberCore::ResourceRequest&, const CyberCore::ResourceResponse&);

    void enableContentExtensionsCheck() { m_checkContentExtensions = true; }

private:
    CyberCore::ContentSecurityPolicy* contentSecurityPolicy();
    bool isChecking() const { return !!m_corsPreflightChecker; }
    bool isRedirected() const { return m_redirectCount; }

    void checkRequest(CyberCore::ResourceRequest&&, CyberCore::ContentSecurityPolicyClient*, ValidationHandler&&);

    bool isAllowedByContentSecurityPolicy(const CyberCore::ResourceRequest&, CyberCore::ContentSecurityPolicyClient*);

    void continueCheckingRequest(CyberCore::ResourceRequest&&, ValidationHandler&&);
    void continueCheckingRequestOrDoSyntheticRedirect(CyberCore::ResourceRequest&& originalRequest, CyberCore::ResourceRequest&& currentRequest, ValidationHandler&&);

    bool doesNotNeedCORSCheck(const URL&) const;
    void checkCORSRequest(CyberCore::ResourceRequest&&, ValidationHandler&&);
    void checkCORSRedirectedRequest(CyberCore::ResourceRequest&&, ValidationHandler&&);
    void checkCORSRequestWithPreflight(CyberCore::ResourceRequest&&, ValidationHandler&&);

    RequestOrRedirectionTripletOrError accessControlErrorForValidationHandler(String&&);

#if ENABLE(CONTENT_EXTENSIONS)
    struct ContentExtensionResult {
        CyberCore::ResourceRequest request;
        const CyberCore::ContentRuleListResults& results;
    };
    using ContentExtensionResultOrError = Expected<ContentExtensionResult, CyberCore::ResourceError>;
    using ContentExtensionCallback = CompletionHandler<void(ContentExtensionResultOrError)>;
    void processContentRuleListsForLoad(CyberCore::ResourceRequest&&, ContentExtensionCallback&&);
#endif

    void applyHTTPSUpgradeIfNeeded(CyberCore::ResourceRequest&&, CompletionHandler<void(CyberCore::ResourceRequest&&)>&&) const;

    CyberCore::FetchOptions m_options;
    CyberCore::StoredCredentialsPolicy m_storedCredentialsPolicy;
    PAL::SessionID m_sessionID;
    Ref<NetworkProcess> m_networkProcess;
    CyberCore::PageIdentifier m_pageID;
    uint64_t m_frameID;
    CyberCore::HTTPHeaderMap m_originalRequestHeaders; // Needed for CORS checks.
    CyberCore::HTTPHeaderMap m_firstRequestHeaders; // Needed for CORS checks.
    URL m_url;
    RefPtr<CyberCore::SecurityOrigin> m_origin;
    Optional<CyberCore::ContentSecurityPolicyResponseHeaders> m_cspResponseHeaders;
#if ENABLE(CONTENT_EXTENSIONS)
    URL m_mainDocumentURL;
    Optional<UserContentControllerIdentifier> m_userContentControllerIdentifier;
#endif

    std::unique_ptr<NetworkCORSPreflightChecker> m_corsPreflightChecker;
    bool m_isSameOriginRequest { true };
    bool m_isSimpleRequest { true };
    std::unique_ptr<CyberCore::ContentSecurityPolicy> m_contentSecurityPolicy;
    size_t m_redirectCount { 0 };
    URL m_previousURL;
    CyberCore::PreflightPolicy m_preflightPolicy;
    String m_referrer;
    bool m_checkContentExtensions { false };
    bool m_shouldCaptureExtraNetworkLoadMetrics { false };
    bool m_isHTTPSUpgradeEnabled { false };
    CyberCore::NetworkLoadInformation m_loadInformation;

    LoadType m_requestLoadType;
};

}
