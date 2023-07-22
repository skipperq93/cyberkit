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

#include <CyberCore/ResourceError.h>
#include <CyberCore/ResourceRequest.h>
#include <CyberCore/ResourceResponse.h>

namespace CyberCore {
class NetworkLoadMetrics;
class SharedBuffer;
enum class PolicyAction : uint8_t;
}

namespace CyberKit {

using ResponseCompletionHandler = CompletionHandler<void(CyberCore::PolicyAction)>;

class NetworkLoadClient {
public:
    virtual ~NetworkLoadClient() { }

    virtual bool isSynchronous() const = 0;

    virtual bool isAllowedToAskUserForCredentials() const = 0;

    virtual void didSendData(unsigned long long bytesSent, unsigned long long totalBytesToBeSent) = 0;
    virtual void willSendRedirectedRequest(CyberCore::ResourceRequest&&, CyberCore::ResourceRequest&& redirectRequest, CyberCore::ResourceResponse&& redirectResponse) = 0;
    virtual void didReceiveResponse(CyberCore::ResourceResponse&&, ResponseCompletionHandler&&) = 0;
    virtual void didReceiveBuffer(Ref<CyberCore::SharedBuffer>&&, int reportedEncodedDataLength) = 0;
    virtual void didFinishLoading(const CyberCore::NetworkLoadMetrics&) = 0;
    virtual void didFailLoading(const CyberCore::ResourceError&) = 0;
    virtual void didBlockAuthenticationChallenge() { };
    virtual bool shouldCaptureExtraNetworkLoadMetrics() const { return false; }
};

} // namespace CyberKit
