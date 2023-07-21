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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <CyberCore/ResourceError.h>
#include <CyberCore/ResourceHandle.h>
#include <CyberCore/ResourceHandleClient.h>
#include <CyberCore/ResourceRequest.h>
#include <CyberCore/ResourceResponse.h>
#include <CyberCore/Timer.h>
#include <wtf/CompletionHandler.h>

// This class triggers asynchronous loads independent of the networking context staying alive (i.e., auditing pingbacks).
// The object just needs to live long enough to ensure the message was actually sent.
// As soon as any callback is received from the ResourceHandle, this class will cancel the load and delete itself.

class PingHandle final : private CyberCore::ResourceHandleClient {
    WTF_MAKE_NONCOPYABLE(PingHandle); WTF_MAKE_FAST_ALLOCATED;
public:
    PingHandle(CyberCore::NetworkingContext* networkingContext, const CyberCore::ResourceRequest& request, bool shouldUseCredentialStorage, bool shouldFollowRedirects, CompletionHandler<void(const CyberCore::ResourceError&, const CyberCore::ResourceResponse&)>&& completionHandler)
        : m_currentRequest(request)
        , m_timeoutTimer(*this, &PingHandle::timeoutTimerFired)
        , m_shouldUseCredentialStorage(shouldUseCredentialStorage)
        , m_shouldFollowRedirects(shouldFollowRedirects)
        , m_completionHandler(WTFMove(completionHandler))
    {
        bool defersLoading = false;
        bool shouldContentSniff = false;
        bool shouldContentEncodingSniff = true;
        m_handle = CyberCore::ResourceHandle::create(networkingContext, request, this, defersLoading, shouldContentSniff, shouldContentEncodingSniff);

        // If the server never responds, this object will hang around forever.
        // Set a very generous timeout, just in case.
        m_timeoutTimer.startOneShot(60000_s);
    }

private:
    void willSendRequestAsync(CyberCore::ResourceHandle*, CyberCore::ResourceRequest&& request, CyberCore::ResourceResponse&&, CompletionHandler<void(CyberCore::ResourceRequest&&)>&& completionHandler) final
    {
        m_currentRequest = WTFMove(request);
        if (m_shouldFollowRedirects) {
            completionHandler(CyberCore::ResourceRequest { m_currentRequest });
            return;
        }
        completionHandler({ });
        pingLoadComplete(CyberCore::ResourceError { String(), 0, m_currentRequest.url(), "Not allowed to follow redirects"_s, CyberCore::ResourceError::Type::AccessControl });
    }
    void didReceiveResponseAsync(CyberCore::ResourceHandle*, CyberCore::ResourceResponse&& response, CompletionHandler<void()>&& completionHandler) final
    {
        completionHandler();
        pingLoadComplete({ }, response);
    }
    void didReceiveBuffer(CyberCore::ResourceHandle*, Ref<CyberCore::SharedBuffer>&&, int) final { pingLoadComplete(); }
    void didFinishLoading(CyberCore::ResourceHandle*) final { pingLoadComplete(); }
    void didFail(CyberCore::ResourceHandle*, const CyberCore::ResourceError& error) final { pingLoadComplete(error); }
    bool shouldUseCredentialStorage(CyberCore::ResourceHandle*) final { return m_shouldUseCredentialStorage; }
    void timeoutTimerFired() { pingLoadComplete(CyberCore::ResourceError { String(), 0, m_currentRequest.url(), "Load timed out"_s, CyberCore::ResourceError::Type::Timeout }); }
#if USE(PROTECTION_SPACE_AUTH_CALLBACK)
    void canAuthenticateAgainstProtectionSpaceAsync(CyberCore::ResourceHandle*, const CyberCore::ProtectionSpace&, CompletionHandler<void(bool)>&& completionHandler)
    {
        completionHandler(false);
        pingLoadComplete(CyberCore::ResourceError { String { }, 0, m_currentRequest.url(), "Not allowed to authenticate"_s, CyberCore::ResourceError::Type::AccessControl });
    }
#endif

    void pingLoadComplete(const CyberCore::ResourceError& error = { }, const CyberCore::ResourceResponse& response = { })
    {
        if (auto completionHandler = std::exchange(m_completionHandler, nullptr))
            completionHandler(error, response);
        delete this;
    }

    virtual ~PingHandle()
    {
        ASSERT(!m_completionHandler);
        if (m_handle) {
            ASSERT(m_handle->client() == this);
            m_handle->clearClient();
            m_handle->cancel();
        }
    }

    RefPtr<CyberCore::ResourceHandle> m_handle;
    CyberCore::ResourceRequest m_currentRequest;
    CyberCore::Timer m_timeoutTimer;
    bool m_shouldUseCredentialStorage;
    bool m_shouldFollowRedirects;
    CompletionHandler<void(const CyberCore::ResourceError&, const CyberCore::ResourceResponse&)> m_completionHandler;
};
