/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
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
#include <CyberCore/ResourceRequest.h>
#include <wtf/Deque.h>
#include <wtf/RefCounted.h>

namespace CyberCore {
class ResourceError;
class ResourceLoader;
class ResourceResponse;
class SharedBuffer;
}

namespace CyberKit {

class WebFrame;
class WebURLSchemeHandlerProxy;

class WebURLSchemeTaskProxy : public RefCounted<WebURLSchemeTaskProxy> {
public:
    static Ref<WebURLSchemeTaskProxy> create(WebURLSchemeHandlerProxy& handler, CyberCore::ResourceLoader& loader, WebFrame& webFrame)
    {
        return adoptRef(*new WebURLSchemeTaskProxy(handler, loader, webFrame));
    }
    
    const CyberCore::ResourceRequest& request() const { return m_request; }

    void startLoading();
    void stopLoading();

    void didPerformRedirection(CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&, CompletionHandler<void(CyberCore::ResourceRequest&&)>&&);
    void didReceiveResponse(const CyberCore::ResourceResponse&);
    void didReceiveData(const CyberCore::SharedBuffer&);
    void didComplete(const CyberCore::ResourceError&);

    CyberCore::ResourceLoaderIdentifier identifier() const { return m_identifier; }

private:
    WebURLSchemeTaskProxy(WebURLSchemeHandlerProxy&, CyberCore::ResourceLoader&, WebFrame&);
    bool hasLoader();

    void queueTask(Function<void()>&& task) { m_queuedTasks.append(WTFMove(task)); }
    void processNextPendingTask();

    WebURLSchemeHandlerProxy& m_urlSchemeHandler;
    RefPtr<CyberCore::ResourceLoader> m_coreLoader;
    RefPtr<WebFrame> m_frame;
    CyberCore::ResourceRequest m_request;
    CyberCore::ResourceLoaderIdentifier m_identifier;
    bool m_waitingForCompletionHandler { false };
    Deque<Function<void()>> m_queuedTasks;
};

} // namespace CyberKit
