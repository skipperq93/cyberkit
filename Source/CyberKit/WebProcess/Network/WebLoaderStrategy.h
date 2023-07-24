/*
 * Copyright (C) 2012, 2015 Apple Inc. All rights reserved.
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

#include "WebResourceLoader.h"
#include <CyberCore/LoaderStrategy.h>
#include <CyberCore/ResourceError.h>
#include <CyberCore/ResourceLoader.h>
#include <CyberCore/ResourceResponse.h>
#include <wtf/HashSet.h>
#include <wtf/RunLoop.h>

namespace CyberCore {
struct FetchOptions;
}

namespace CyberKit {

class NetworkProcessConnection;
class WebFrame;
class WebPage;
class WebURLSchemeTaskProxy;
typedef uint64_t ResourceLoadIdentifier;

class WebLoaderStrategy final : public CyberCore::LoaderStrategy {
    WTF_MAKE_NONCOPYABLE(WebLoaderStrategy); WTF_MAKE_FAST_ALLOCATED;
public:
    WebLoaderStrategy();
    ~WebLoaderStrategy() final;
    
    void loadResource(CyberCore::Frame&, CyberCore::CachedResource&, CyberCore::ResourceRequest&&, const CyberCore::ResourceLoaderOptions&, CompletionHandler<void(RefPtr<CyberCore::SubresourceLoader>&&)>&&) final;
    void loadResourceSynchronously(CyberCore::FrameLoader&, unsigned long resourceLoadIdentifier, const CyberCore::ResourceRequest&, CyberCore::ClientCredentialPolicy, const CyberCore::FetchOptions&, const CyberCore::HTTPHeaderMap&, CyberCore::ResourceError&, CyberCore::ResourceResponse&, Vector<char>& data) final;
    void pageLoadCompleted(CyberCore::Page&) final;
    void browsingContextRemoved(CyberCore::Frame&) final;

    void remove(CyberCore::ResourceLoader*) final;
    void setDefersLoading(CyberCore::ResourceLoader&, bool) final;
    void crossOriginRedirectReceived(CyberCore::ResourceLoader*, const URL& redirectURL) final;
    
    void servePendingRequests(CyberCore::ResourceLoadPriority minimumPriority) final;

    void suspendPendingRequests() final;
    void resumePendingRequests() final;

    bool usePingLoad() const final;
    void startPingLoad(CyberCore::Frame&, CyberCore::ResourceRequest&, const CyberCore::HTTPHeaderMap& originalRequestHeaders, const CyberCore::FetchOptions&, CyberCore::ContentSecurityPolicyImposition, PingLoadCompletionHandler&&) final;
    void didFinishPingLoad(uint64_t pingLoadIdentifier, CyberCore::ResourceError&&, CyberCore::ResourceResponse&&);

    void preconnectTo(CyberCore::ResourceRequest&&, WebPage&, WebFrame&, CyberCore::StoredCredentialsPolicy, PreconnectCompletionHandler&& = nullptr);
    void preconnectTo(CyberCore::FrameLoader&, const URL&, CyberCore::StoredCredentialsPolicy, PreconnectCompletionHandler&&) final;
    void didFinishPreconnection(uint64_t preconnectionIdentifier, CyberCore::ResourceError&&);

    void setCaptureExtraNetworkLoadMetricsEnabled(bool) final;

    WebResourceLoader* webResourceLoaderForIdentifier(ResourceLoadIdentifier identifier) const { return m_webResourceLoaders.get(identifier); }
    void schedulePluginStreamLoad(CyberCore::Frame&, CyberCore::NetscapePlugInStreamLoaderClient&, CyberCore::ResourceRequest&&, CompletionHandler<void(RefPtr<CyberCore::NetscapePlugInStreamLoader>&&)>&&);

    void networkProcessCrashed();

    void addURLSchemeTaskProxy(WebURLSchemeTaskProxy&);
    void removeURLSchemeTaskProxy(WebURLSchemeTaskProxy&);

    void scheduleLoadFromNetworkProcess(CyberCore::ResourceLoader&, const CyberCore::ResourceRequest&, const WebResourceLoader::TrackingParameters&, bool shouldClearReferrerOnHTTPSToHTTPRedirect, Seconds maximumBufferingTime);

    bool isOnLine() const final;
    void addOnlineStateChangeListener(Function<void(bool)>&&) final;
    void setOnLineState(bool);

    static uint64_t generateLoadIdentifier();

private:
    void scheduleLoad(CyberCore::ResourceLoader&, CyberCore::CachedResource*, bool shouldClearReferrerOnHTTPSToHTTPRedirect);
    void scheduleInternallyFailedLoad(CyberCore::ResourceLoader&);
    void internallyFailedLoadTimerFired();
    void startLocalLoad(CyberCore::ResourceLoader&);
    bool tryLoadingUsingURLSchemeHandler(CyberCore::ResourceLoader&, const WebResourceLoader::TrackingParameters&);
    
    struct SyncLoadResult {
        CyberCore::ResourceResponse response;
        CyberCore::ResourceError error;
        Vector<char> data;
    };
    Optional<SyncLoadResult> tryLoadingSynchronouslyUsingURLSchemeHandler(CyberCore::FrameLoader&, ResourceLoadIdentifier, const CyberCore::ResourceRequest&);

    CyberCore::ResourceResponse responseFromResourceLoadIdentifier(uint64_t resourceLoadIdentifier) final;
    Vector<CyberCore::NetworkTransactionInformation> intermediateLoadInformationFromResourceLoadIdentifier(uint64_t resourceLoadIdentifier) final;
    CyberCore::NetworkLoadMetrics networkMetricsFromResourceLoadIdentifier(uint64_t resourceLoadIdentifier) final;

    bool shouldPerformSecurityChecks() const final;
    bool havePerformedSecurityChecks(const CyberCore::ResourceResponse&) const final;

    Vector<uint64_t> ongoingLoads() const final
    {
        return WTF::map(m_webResourceLoaders, [](auto&& keyValue) -> uint64_t {
            return keyValue.key;
        });
    }

    HashSet<RefPtr<CyberCore::ResourceLoader>> m_internallyFailedResourceLoaders;
    RunLoop::Timer<WebLoaderStrategy> m_internallyFailedLoadTimer;
    
    HashMap<unsigned long, RefPtr<WebResourceLoader>> m_webResourceLoaders;
    HashMap<unsigned long, WebURLSchemeTaskProxy*> m_urlSchemeTasks;
    HashMap<unsigned long, PingLoadCompletionHandler> m_pingLoadCompletionHandlers;
    HashMap<unsigned long, PreconnectCompletionHandler> m_preconnectCompletionHandlers;
    Vector<Function<void(bool)>> m_onlineStateChangeListeners;
    bool m_isOnLine { true };
};

} // namespace CyberKit
