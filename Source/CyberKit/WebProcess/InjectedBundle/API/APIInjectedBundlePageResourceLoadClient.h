/*
 * Copyright (C) 2017 Igalia S.L.
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

#include <CyberCore/ResourceLoaderIdentifier.h>

namespace CyberKit {
class WebFrame;
class WebPage;
}

namespace CyberCore {
class ResourceError;
class ResourceRequest;
class ResourceResponse;
}

namespace API {

namespace InjectedBundle {

class ResourceLoadClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    virtual ~ResourceLoadClient() = default;

    virtual void didInitiateLoadForResource(CyberKit::WebPage&, CyberKit::WebFrame&, CyberCore::ResourceLoaderIdentifier, const CyberCore::ResourceRequest&, bool /*pageIsProvisionallyLoading*/) { }
    virtual void willSendRequestForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, CyberCore::ResourceLoaderIdentifier, CyberCore::ResourceRequest&, const CyberCore::ResourceResponse&) { }
    virtual void didReceiveResponseForResource(CyberKit::WebPage&, CyberKit::WebFrame&, CyberCore::ResourceLoaderIdentifier, const CyberCore::ResourceResponse&) { }
    virtual void didReceiveContentLengthForResource(CyberKit::WebPage&, CyberKit::WebFrame&, CyberCore::ResourceLoaderIdentifier, uint64_t contentLength) { }
    virtual void didFinishLoadForResource(CyberKit::WebPage&, CyberKit::WebFrame&, CyberCore::ResourceLoaderIdentifier) { }
    virtual void didFailLoadForResource(CyberKit::WebPage&, CyberKit::WebFrame&, CyberCore::ResourceLoaderIdentifier, const CyberCore::ResourceError&) { }
    virtual bool shouldCacheResponse(CyberKit::WebPage&, CyberKit::WebFrame&, CyberCore::ResourceLoaderIdentifier) { return true; }
    virtual bool shouldUseCredentialStorage(CyberKit::WebPage&, CyberKit::WebFrame&, CyberCore::ResourceLoaderIdentifier) { return true; }
};

} // namespace InjectedBundle

} // namespace API
