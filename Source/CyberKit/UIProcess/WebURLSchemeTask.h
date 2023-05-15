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

#include "APIObject.h"
#include "WebProcessProxy.h"
#include <CyberCore/ResourceRequest.h>
#include <CyberCore/ResourceResponse.h>
#include <CyberCore/SharedBuffer.h>
#include <wtf/CompletionHandler.h>
#include <wtf/InstanceCounted.h>
#include <wtf/Lock.h>
#include <wtf/RefCounted.h>
#include <wtf/RefPtr.h>

namespace API {
class FrameInfo;
}

namespace CyberCore {
class ResourceError;
class ResourceResponse;
}

namespace CyberKit {

struct URLSchemeTaskParameters;
class WebURLSchemeHandler;
class WebPageProxy;

using SyncLoadCompletionHandler = CompletionHandler<void(const CyberCore::ResourceResponse&, const CyberCore::ResourceError&, Vector<uint8_t>&&)>;

class WebURLSchemeTask : public API::ObjectImpl<API::Object::Type::URLSchemeTask>, public InstanceCounted<WebURLSchemeTask> {
    WTF_MAKE_NONCOPYABLE(WebURLSchemeTask);
public:
    static Ref<WebURLSchemeTask> create(WebURLSchemeHandler&, WebPageProxy&, WebProcessProxy&, CyberCore::PageIdentifier, URLSchemeTaskParameters&&, SyncLoadCompletionHandler&&);

    ~WebURLSchemeTask();

    CyberCore::ResourceLoaderIdentifier resourceLoaderID() const { ASSERT(RunLoop::isMain()); return m_resourceLoaderID; }
    WebPageProxyIdentifier pageProxyID() const { ASSERT(RunLoop::isMain()); return m_pageProxyID; }
    CyberCore::PageIdentifier webPageID() const { ASSERT(RunLoop::isMain()); return m_webPageID; }
    WebProcessProxy* process() { ASSERT(RunLoop::isMain()); return m_process.get(); }
    CyberCore::ResourceRequest request() const;
    API::FrameInfo& frameInfo() const { return m_frameInfo.get(); }

#if PLATFORM(COCOA)
    NSURLRequest *nsRequest() const;
#endif

    enum class ExceptionType {
        DataAlreadySent,
        CompleteAlreadyCalled,
        RedirectAfterResponse,
        TaskAlreadyStopped,
        NoResponseSent,
        WaitingForRedirectCompletionHandler,
        None,
    };
    ExceptionType willPerformRedirection(CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&,  Function<void(CyberCore::ResourceRequest&&)>&&);
    ExceptionType didPerformRedirection(CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&);
    ExceptionType didReceiveResponse(const CyberCore::ResourceResponse&);
    ExceptionType didReceiveData(Ref<CyberCore::SharedBuffer>&&);
    ExceptionType didComplete(const CyberCore::ResourceError&);

    void stop();
    void pageDestroyed();

    void suppressTaskStoppedExceptions() { m_shouldSuppressTaskStoppedExceptions = true; }

    bool waitingForRedirectCompletionHandlerCallback() const { return m_waitingForRedirectCompletionHandlerCallback; }

private:
    WebURLSchemeTask(WebURLSchemeHandler&, WebPageProxy&, WebProcessProxy&, CyberCore::PageIdentifier, URLSchemeTaskParameters&&, SyncLoadCompletionHandler&&);

    bool isSync() const { return !!m_syncCompletionHandler; }

    Ref<WebURLSchemeHandler> m_urlSchemeHandler;
    RefPtr<WebProcessProxy> m_process;
    CyberCore::ResourceLoaderIdentifier m_resourceLoaderID;
    WebPageProxyIdentifier m_pageProxyID;
    CyberCore::PageIdentifier m_webPageID;
    CyberCore::ResourceRequest m_request WTF_GUARDED_BY_LOCK(m_requestLock);
    Ref<API::FrameInfo> m_frameInfo;
    mutable Lock m_requestLock;
    bool m_stopped { false };
    bool m_responseSent { false };
    bool m_dataSent { false };
    bool m_completed { false };
    bool m_shouldSuppressTaskStoppedExceptions { false };

    SyncLoadCompletionHandler m_syncCompletionHandler;
    CyberCore::ResourceResponse m_syncResponse;
    CyberCore::SharedBufferBuilder m_syncData;

    bool m_waitingForRedirectCompletionHandlerCallback { false };
};

} // namespace CyberKit
