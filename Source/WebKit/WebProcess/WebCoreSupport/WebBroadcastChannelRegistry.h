/*
 * Copyright (C) 2021 Apple Inc. All rights reserved.
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
#include <CyberCore/BroadcastChannelRegistry.h>
#include <CyberCore/ClientOrigin.h>
#include <CyberCore/PartitionedSecurityOrigin.h>
#include <CyberCore/SecurityOrigin.h>
#include <wtf/CallbackAggregator.h>
#include <wtf/HashMap.h>
#include <wtf/Vector.h>

namespace CyberCore {
struct MessageWithMessagePorts;
}

namespace CyberKit {

class WebBroadcastChannelRegistry final : public CyberCore::BroadcastChannelRegistry {
public:
    static Ref<WebBroadcastChannelRegistry> create()
    {
        return adoptRef(*new WebBroadcastChannelRegistry);
    }

    void registerChannel(const CyberCore::PartitionedSecurityOrigin&, const String& name, CyberCore::BroadcastChannelIdentifier) final;
    void unregisterChannel(const CyberCore::PartitionedSecurityOrigin&, const String& name, CyberCore::BroadcastChannelIdentifier) final;
    void postMessage(const CyberCore::PartitionedSecurityOrigin&, const String& name, CyberCore::BroadcastChannelIdentifier source, Ref<CyberCore::SerializedScriptValue>&&, CompletionHandler<void()>&&) final;

    void networkProcessCrashed();

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&);

private:
    WebBroadcastChannelRegistry() = default;

    void postMessageToRemote(const CyberCore::ClientOrigin&, const String& name, CyberCore::MessageWithMessagePorts&&, CompletionHandler<void()>&&);
    void postMessageLocally(const CyberCore::PartitionedSecurityOrigin&, const String& name, std::optional<CyberCore::BroadcastChannelIdentifier> sourceInProcess, Ref<CyberCore::SerializedScriptValue>&&, Ref<WTF::CallbackAggregator>&&);

    HashMap<CyberCore::PartitionedSecurityOrigin, HashMap<String, Vector<CyberCore::BroadcastChannelIdentifier>>> m_channelsPerOrigin;
};

} // namespace CyberKit
