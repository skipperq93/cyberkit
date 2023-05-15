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

#pragma once

#include "Connection.h"
#include "DataReference.h"
#include "MessageSender.h"
#include "ShareableResource.h"
#include "WebPageProxyIdentifier.h"
#include "WebResourceInterceptController.h"
#include <CyberCore/FrameIdentifier.h>
#include <CyberCore/PageIdentifier.h>
#include <CyberCore/ResourceLoaderIdentifier.h>
#include <wtf/MonotonicTime.h>
#include <wtf/RefCounted.h>
#include <wtf/RefPtr.h>

namespace IPC {
class FormDataReference;
class SharedBufferReference;
}

namespace CyberCore {
class ContentFilterUnblockHandler;
class NetworkLoadMetrics;
class ResourceError;
class ResourceLoader;
class ResourceRequest;
class ResourceResponse;
class SubstituteData;
enum class MainFrameMainResource : bool;
}

namespace CyberKit {

enum class PrivateRelayed : bool;

class WebResourceLoader : public RefCounted<WebResourceLoader>, public IPC::MessageSender {
public:
    struct TrackingParameters {
        WebPageProxyIdentifier webPageProxyID;
        CyberCore::PageIdentifier pageID;
        CyberCore::FrameIdentifier frameID;
        CyberCore::ResourceLoaderIdentifier resourceID;
    };

    static Ref<WebResourceLoader> create(Ref<CyberCore::ResourceLoader>&&, const TrackingParameters&);

    ~WebResourceLoader();

    void didReceiveWebResourceLoaderMessage(IPC::Connection&, IPC::Decoder&);

    CyberCore::ResourceLoader* resourceLoader() const { return m_coreLoader.get(); }

    void detachFromCoreLoader();

private:
    WebResourceLoader(Ref<CyberCore::ResourceLoader>&&, const TrackingParameters&);

    // IPC::MessageSender
    IPC::Connection* messageSenderConnection() const override;
    uint64_t messageSenderDestinationID() const override;

    void willSendRequest(CyberCore::ResourceRequest&&, IPC::FormDataReference&& requestBody, CyberCore::ResourceResponse&&);
    void didSendData(uint64_t bytesSent, uint64_t totalBytesToBeSent);
    void didReceiveResponse(CyberCore::ResourceResponse&&, PrivateRelayed, bool needsContinueDidReceiveResponseMessage, std::optional<CyberCore::NetworkLoadMetrics>&&);
    void didReceiveData(IPC::SharedBufferReference&& data, uint64_t encodedDataLength);
    void didFinishResourceLoad(CyberCore::NetworkLoadMetrics&&);
    void didFailResourceLoad(const CyberCore::ResourceError&);
    void didFailServiceWorkerLoad(const CyberCore::ResourceError&);
    void serviceWorkerDidNotHandle();
    void didBlockAuthenticationChallenge();
    void setWorkerStart(MonotonicTime value) { m_workerStart = value; }

    void stopLoadingAfterXFrameOptionsOrContentSecurityPolicyDenied(const CyberCore::ResourceResponse&);

    CyberCore::MainFrameMainResource mainFrameMainResource() const;
    
#if ENABLE(SHAREABLE_RESOURCE)
    void didReceiveResource(const ShareableResource::Handle&);
#endif

#if ENABLE(CONTENT_FILTERING_IN_NETWORKING_PROCESS)
    void contentFilterDidBlockLoad(const CyberCore::ContentFilterUnblockHandler&, String&& unblockRequestDeniedScript, const CyberCore::ResourceError&, const URL& blockedPageURL, CyberCore::SubstituteData&&);
#endif
    
    RefPtr<CyberCore::ResourceLoader> m_coreLoader;
    TrackingParameters m_trackingParameters;
    WebResourceInterceptController m_interceptController;
    size_t m_numBytesReceived { 0 };

#if ASSERT_ENABLED
    bool m_isProcessingNetworkResponse { false };
#endif

    MonotonicTime m_workerStart;
};

} // namespace CyberKit
