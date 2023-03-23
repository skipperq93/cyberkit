/*
 * Copyright (C) 2012 Apple Inc. All rights reserved.
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

#include "config.h"
#include "NetworkResourceLoadParameters.h"

#include "CyberCoreArgumentCoders.h"

namespace CyberKit {
using namespace CyberCore;

NetworkResourceLoadParameters::NetworkResourceLoadParameters(
    NetworkLoadParameters&& networkLoadParameters
    , CyberCore::ResourceLoaderIdentifier identifier
    , RefPtr<CyberCore::FormData>&& httpBody
    , std::optional<Vector<SandboxExtension::Handle>>&& sandboxExtensionIfHttpBody
    , std::optional<SandboxExtension::Handle>&& sandboxExtensionIflocalFile
    , Seconds maximumBufferingTime
    , CyberCore::FetchOptions&& options
    , std::optional<CyberCore::ContentSecurityPolicyResponseHeaders>&& cspResponseHeaders
    , URL&& parentFrameURL
    , URL&& frameURL
    , CyberCore::CrossOriginEmbedderPolicy parentCrossOriginEmbedderPolicy
    , CyberCore::CrossOriginEmbedderPolicy crossOriginEmbedderPolicy
    , CyberCore::HTTPHeaderMap&& originalRequestHeaders
    , bool shouldRestrictHTTPResponseAccess
    , CyberCore::PreflightPolicy preflightPolicy
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
    , std::optional<CyberCore::NavigationRequester>&& navigationRequester
#if ENABLE(SERVICE_WORKER)
    , CyberCore::ServiceWorkersMode serviceWorkersMode
    , std::optional<CyberCore::ServiceWorkerRegistrationIdentifier> serviceWorkerRegistrationIdentifier
    , OptionSet<CyberCore::HTTPHeadersToKeepFromCleaning> httpHeadersToKeep
    , std::optional<CyberCore::FetchIdentifier> navigationPreloadIdentifier
#endif
#if ENABLE(CONTENT_EXTENSIONS)
    , URL&& mainDocumentURL
    , std::optional<UserContentControllerIdentifier> userContentControllerIdentifier
#endif
    ) : NetworkLoadParameters(WTFMove(networkLoadParameters))
        , identifier(identifier)
        , maximumBufferingTime(maximumBufferingTime)
        , options(WTFMove(options))
        , cspResponseHeaders(WTFMove(cspResponseHeaders))
        , parentFrameURL(WTFMove(parentFrameURL))
        , frameURL(WTFMove(frameURL))
        , parentCrossOriginEmbedderPolicy(parentCrossOriginEmbedderPolicy)
        , crossOriginEmbedderPolicy(crossOriginEmbedderPolicy)
        , originalRequestHeaders(WTFMove(originalRequestHeaders))
        , shouldRestrictHTTPResponseAccess(shouldRestrictHTTPResponseAccess)
        , preflightPolicy(preflightPolicy)
        , shouldEnableCrossOriginResourcePolicy(shouldEnableCrossOriginResourcePolicy)
        , frameAncestorOrigins(WTFMove(frameAncestorOrigins))
        , pageHasResourceLoadClient(pageHasResourceLoadClient)
        , parentFrameID(parentFrameID)
        , crossOriginAccessControlCheckEnabled(crossOriginAccessControlCheckEnabled)
        , documentURL(WTFMove(documentURL))
        , isCrossOriginOpenerPolicyEnabled(isCrossOriginOpenerPolicyEnabled)
        , isClearSiteDataHeaderEnabled(isClearSiteDataHeaderEnabled)
        , isDisplayingInitialEmptyDocument(isDisplayingInitialEmptyDocument)
        , effectiveSandboxFlags(effectiveSandboxFlags)
        , openerURL(WTFMove(openerURL))
        , sourceCrossOriginOpenerPolicy(WTFMove(sourceCrossOriginOpenerPolicy))
        , navigationID(navigationID)
        , navigationRequester(WTFMove(navigationRequester))
#if ENABLE(SERVICE_WORKER)
        , serviceWorkersMode(serviceWorkersMode)
        , serviceWorkerRegistrationIdentifier(serviceWorkerRegistrationIdentifier)
        , httpHeadersToKeep(httpHeadersToKeep)
        , navigationPreloadIdentifier(navigationPreloadIdentifier)
#endif
#if ENABLE(CONTENT_EXTENSIONS)
        , mainDocumentURL(WTFMove(mainDocumentURL))
        , userContentControllerIdentifier(userContentControllerIdentifier)
#endif
{
    if (httpBody) {
        request.setHTTPBody(WTFMove(httpBody));

        if (!sandboxExtensionIfHttpBody)
            RELEASE_ASSERT_NOT_REACHED_WITH_MESSAGE("NetworkResourceLoadParameters which specify a httpBody should have sandboxExtensionIfHttpBody");
        for (auto& handle : WTFMove(*sandboxExtensionIfHttpBody)) {
            if (auto extension = SandboxExtension::create(WTFMove(handle)))
                requestBodySandboxExtensions.append(WTFMove(extension));
        }
    }
    
    if (request.url().isLocalFile()) {
        if (!sandboxExtensionIflocalFile)
            RELEASE_ASSERT_NOT_REACHED_WITH_MESSAGE("NetworkResourceLoadParameters which specify a URL of a local file should have sandboxExtensionIflocalFile");
        resourceSandboxExtension = SandboxExtension::create(WTFMove(*sandboxExtensionIflocalFile));
    }
}

RefPtr<SecurityOrigin> NetworkResourceLoadParameters::parentOrigin() const
{
    if (frameAncestorOrigins.isEmpty())
        return nullptr;
    return frameAncestorOrigins.first();
}

std::optional<Vector<SandboxExtension::Handle>> NetworkResourceLoadParameters::sandboxExtensionsIfHttpBody() const
{
    if (!request.httpBody())
        return std::nullopt;
    
    Vector<SandboxExtension::Handle> requestBodySandboxExtensions;
    for (const FormDataElement& element : request.httpBody()->elements()) {
        if (auto* fileData = std::get_if<FormDataElement::EncodedFileData>(&element.data)) {
            const String& path = fileData->filename;
            if (auto handle = SandboxExtension::createHandle(path, SandboxExtension::Type::ReadOnly))
                requestBodySandboxExtensions.append(WTFMove(*handle));
        }
    }
    return requestBodySandboxExtensions;
    
}

std::optional<SandboxExtension::Handle> NetworkResourceLoadParameters::sandboxExtensionIflocalFile() const
{
    if (!request.url().isLocalFile())
        return std::nullopt;
    
    SandboxExtension::Handle requestSandboxExtension;
#if HAVE(AUDIT_TOKEN)
    if (networkProcessAuditToken) {
        if (auto handle = SandboxExtension::createHandleForReadByAuditToken(request.url().fileSystemPath(), *networkProcessAuditToken))
            requestSandboxExtension = WTFMove(*handle);
    } else
#endif
    {
        if (auto handle = SandboxExtension::createHandle(request.url().fileSystemPath(), SandboxExtension::Type::ReadOnly))
            requestSandboxExtension = WTFMove(*handle);
    }

    return requestSandboxExtension;
}
    
} // namespace CyberKit
