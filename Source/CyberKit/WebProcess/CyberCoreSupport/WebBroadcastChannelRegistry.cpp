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

#include "config.h"
#include "WebBroadcastChannelRegistry.h"

#include "NetworkBroadcastChannelRegistryMessages.h"
#include "NetworkProcessConnection.h"
#include "WebProcess.h"
#include <CyberCore/BroadcastChannel.h>
#include <CyberCore/MessageWithMessagePorts.h>
#include <wtf/CallbackAggregator.h>

namespace CyberKit {

static inline IPC::Connection& networkProcessConnection()
{
    return WebProcess::singleton().ensureNetworkProcessConnection().connection();
}

// Opaque origins are only stored in process in m_channelsPerOrigin and never sent to the NetworkProcess as a ClientOrigin.
// The identity of opaque origins wouldn't be preserved when serializing them as a SecurityOriginData (via ClientOrigin).
// Since BroadcastChannels from an opaque origin can only communicate with other BroadcastChannels from the same opaque origin,
// the destination channels have to be within the same WebProcess anyway.
static std::optional<CyberCore::ClientOrigin> toClientOrigin(const CyberCore::PartitionedSecurityOrigin& origin)
{
    if (origin.topOrigin->isOpaque() || origin.clientOrigin->isOpaque())
        return std::nullopt;
    return CyberCore::ClientOrigin { origin.topOrigin->data(), origin.clientOrigin->data() };
}

void WebBroadcastChannelRegistry::registerChannel(const CyberCore::PartitionedSecurityOrigin& origin, const String& name, CyberCore::BroadcastChannelIdentifier identifier)
{
    auto& channelsForOrigin = m_channelsPerOrigin.ensure(origin, [] { return HashMap<String, Vector<CyberCore::BroadcastChannelIdentifier>> { }; }).iterator->value;
    auto& channelsForName = channelsForOrigin.ensure(name, [] { return Vector<CyberCore::BroadcastChannelIdentifier> { }; }).iterator->value;
    channelsForName.append(identifier);

    if (channelsForName.size() == 1) {
        if (auto clientOrigin = toClientOrigin(origin))
            networkProcessConnection().send(Messages::NetworkBroadcastChannelRegistry::RegisterChannel { *clientOrigin, name }, 0);
    }
}

void WebBroadcastChannelRegistry::unregisterChannel(const CyberCore::PartitionedSecurityOrigin& origin, const String& name, CyberCore::BroadcastChannelIdentifier identifier)
{
    auto channelsPerOriginIterator = m_channelsPerOrigin.find(origin);
    if (channelsPerOriginIterator == m_channelsPerOrigin.end())
        return;

    auto& channelsForOrigin = channelsPerOriginIterator->value;
    auto channelsForOriginIterator = channelsForOrigin.find(name);
    if (channelsForOriginIterator == channelsForOrigin.end())
        return;

    auto& channelIdentifiersForName = channelsForOriginIterator->value;
    if (!channelIdentifiersForName.removeFirst(identifier))
        return;
    if (!channelIdentifiersForName.isEmpty())
        return;

    channelsForOrigin.remove(channelsForOriginIterator);
    if (auto clientOrigin = toClientOrigin(origin))
        networkProcessConnection().send(Messages::NetworkBroadcastChannelRegistry::UnregisterChannel { *clientOrigin, name }, 0);

    if (channelsForOrigin.isEmpty())
        m_channelsPerOrigin.remove(channelsPerOriginIterator);
}

void WebBroadcastChannelRegistry::postMessage(const CyberCore::PartitionedSecurityOrigin& origin, const String& name, CyberCore::BroadcastChannelIdentifier source, Ref<CyberCore::SerializedScriptValue>&& message, CompletionHandler<void()>&& completionHandler)
{
    auto callbackAggregator = CallbackAggregator::create(WTFMove(completionHandler));
    postMessageLocally(origin, name, source, message.copyRef(), callbackAggregator.copyRef());
    if (auto clientOrigin = toClientOrigin(origin))
        networkProcessConnection().sendWithAsyncReply(Messages::NetworkBroadcastChannelRegistry::PostMessage { *clientOrigin, name, CyberCore::MessageWithMessagePorts { WTFMove(message), { } } }, [callbackAggregator] { }, 0);
}

void WebBroadcastChannelRegistry::postMessageLocally(const CyberCore::PartitionedSecurityOrigin& origin, const String& name, std::optional<CyberCore::BroadcastChannelIdentifier> sourceInProcess, Ref<CyberCore::SerializedScriptValue>&& message, Ref<WTF::CallbackAggregator>&& callbackAggregator)
{
    auto channelsPerOriginIterator = m_channelsPerOrigin.find(origin);
    if (channelsPerOriginIterator == m_channelsPerOrigin.end())
        return;

    auto& channelsForOrigin = channelsPerOriginIterator->value;
    auto channelsForOriginIterator = channelsForOrigin.find(name);
    if (channelsForOriginIterator == channelsForOrigin.end())
        return;

    auto channelIdentifiersForName = channelsForOriginIterator->value;
    for (auto& channelIdentifier : channelIdentifiersForName) {
        if (channelIdentifier == sourceInProcess)
            continue;
        CyberCore::BroadcastChannel::dispatchMessageTo(channelIdentifier, message.copyRef(), [callbackAggregator] { });
    }
}

void WebBroadcastChannelRegistry::postMessageToRemote(const CyberCore::ClientOrigin& clientOrigin, const String& name, CyberCore::MessageWithMessagePorts&& message, CompletionHandler<void()>&& completionHandler)
{
    auto callbackAggregator = CallbackAggregator::create(WTFMove(completionHandler));
    CyberCore::PartitionedSecurityOrigin origin { clientOrigin.topOrigin.securityOrigin(), clientOrigin.clientOrigin.securityOrigin() };
    postMessageLocally(origin, name, std::nullopt, *message.message, callbackAggregator.copyRef());
}

void WebBroadcastChannelRegistry::networkProcessCrashed()
{
    for (auto& [origin, channelsForOrigin] : m_channelsPerOrigin) {
        auto clientOrigin = toClientOrigin(origin);
        if (!clientOrigin)
            continue;
        for (auto& name : channelsForOrigin.keys())
            networkProcessConnection().send(Messages::NetworkBroadcastChannelRegistry::RegisterChannel { *clientOrigin, name }, 0);
    }
}

} // namespace CyberKit
