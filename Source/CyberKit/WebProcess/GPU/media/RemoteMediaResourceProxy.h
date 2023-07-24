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

#pragma once

#if ENABLE(GPU_PROCESS)

#include "Connection.h"
#include "RemoteMediaResourceIdentifier.h"
#include <CyberCore/PlatformMediaResourceLoader.h>
#include <CyberCore/PolicyChecker.h>
#include <CyberCore/ResourceResponse.h>

namespace CyberKit {

class RemoteMediaResourceProxy final : public CyberCore::PlatformMediaResourceClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    RemoteMediaResourceProxy(Ref<IPC::Connection>&&, CyberCore::PlatformMediaResource&, RemoteMediaResourceIdentifier);
    ~RemoteMediaResourceProxy();

private:
    // PlatformMediaResourceClient
    void responseReceived(CyberCore::PlatformMediaResource&, const CyberCore::ResourceResponse&, CompletionHandler<void(CyberCore::ShouldContinuePolicyCheck)>&&) final;
    void redirectReceived(CyberCore::PlatformMediaResource&, CyberCore::ResourceRequest&&, const CyberCore::ResourceResponse&, CompletionHandler<void(CyberCore::ResourceRequest&&)>&&) final;
    bool shouldCacheResponse(CyberCore::PlatformMediaResource&, const CyberCore::ResourceResponse&) final;
    void dataSent(CyberCore::PlatformMediaResource&, unsigned long long, unsigned long long) final;
    void dataReceived(CyberCore::PlatformMediaResource&, const char*, int) final;
    void accessControlCheckFailed(CyberCore::PlatformMediaResource&, const CyberCore::ResourceError&) final;
    void loadFailed(CyberCore::PlatformMediaResource&, const CyberCore::ResourceError&) final;
    void loadFinished(CyberCore::PlatformMediaResource&, const CyberCore::NetworkLoadMetrics&) final;

    Ref<IPC::Connection> m_connection;
    CyberCore::PlatformMediaResource& m_platformMediaResource;
    RemoteMediaResourceIdentifier m_id;
};

} // namespace CyberKit

#endif
