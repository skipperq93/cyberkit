/*
 * Copyright (C) 2019 Apple Inc. All rights reserved.
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
#include "RemoteMediaResourceProxy.h"

#if ENABLE(GPU_PROCESS)

#include "DataReference.h"
#include "RemoteMediaResourceManagerMessages.h"
#include "CyberCoreArgumentCoders.h"
#include <wtf/CompletionHandler.h>

namespace CyberKit {

RemoteMediaResourceProxy::RemoteMediaResourceProxy(Ref<IPC::Connection>&& connection, CyberCore::PlatformMediaResource& platformMediaResource, RemoteMediaResourceIdentifier id)
    : m_connection(WTFMove(connection))
    , m_platformMediaResource(platformMediaResource)
    , m_id(id)
{
}

RemoteMediaResourceProxy::~RemoteMediaResourceProxy()
{
}

void RemoteMediaResourceProxy::responseReceived(CyberCore::PlatformMediaResource&, const CyberCore::ResourceResponse& response, CompletionHandler<void(CyberCore::PolicyChecker::ShouldContinue)>&& completionHandler)
{
    m_connection->sendWithAsyncReply(Messages::RemoteMediaResourceManager::ResponseReceived(m_id, response, m_platformMediaResource.didPassAccessControlCheck()), [completionHandler = WTFMove(completionHandler)](auto shouldContinue) mutable {
        completionHandler(shouldContinue);
    });
}

void RemoteMediaResourceProxy::redirectReceived(CyberCore::PlatformMediaResource&, CyberCore::ResourceRequest&& request, const CyberCore::ResourceResponse& response, CompletionHandler<void(CyberCore::ResourceRequest&&)>&& completionHandler)
{
    m_connection->sendWithAsyncReply(Messages::RemoteMediaResourceManager::RedirectReceived(m_id, request, response), [completionHandler = WTFMove(completionHandler)](auto&& request) mutable {
        completionHandler(WTFMove(request));
    });
}

bool RemoteMediaResourceProxy::shouldCacheResponse(CyberCore::PlatformMediaResource&, const CyberCore::ResourceResponse&)
{
    // TODO: need to check CyberCoreNSURLSessionDataTaskClient::shouldCacheResponse()
    return false;
}

void RemoteMediaResourceProxy::dataSent(CyberCore::PlatformMediaResource&, unsigned long long bytesSent, unsigned long long totalBytesToBeSent)
{
    m_connection->send(Messages::RemoteMediaResourceManager::DataSent(m_id, bytesSent, totalBytesToBeSent), 0);
}

void RemoteMediaResourceProxy::dataReceived(CyberCore::PlatformMediaResource&, const char* data, int length)
{
    m_connection->send(Messages::RemoteMediaResourceManager::DataReceived(m_id, IPC::DataReference(reinterpret_cast<const uint8_t*>(data), length)), 0);
}

void RemoteMediaResourceProxy::accessControlCheckFailed(CyberCore::PlatformMediaResource&, const CyberCore::ResourceError& error)
{
    m_connection->send(Messages::RemoteMediaResourceManager::AccessControlCheckFailed(m_id, error), 0);
}

void RemoteMediaResourceProxy::loadFailed(CyberCore::PlatformMediaResource&, const CyberCore::ResourceError& error)
{
    m_connection->send(Messages::RemoteMediaResourceManager::LoadFailed(m_id, error), 0);
}

void RemoteMediaResourceProxy::loadFinished(CyberCore::PlatformMediaResource&)
{
    m_connection->send(Messages::RemoteMediaResourceManager::LoadFinished(m_id), 0);
}

}

#endif
