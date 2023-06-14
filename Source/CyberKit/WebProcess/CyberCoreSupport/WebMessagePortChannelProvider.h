/*
 * Copyright (C) 2018 Apple Inc. All rights reserved.
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

#include <CyberCore/MessagePortChannelProvider.h>
#include <wtf/CompletionHandler.h>
#include <wtf/HashMap.h>

namespace CyberKit {

class WebMessagePortChannelProvider : public CyberCore::MessagePortChannelProvider {
public:
    static WebMessagePortChannelProvider& singleton();

    void didTakeAllMessagesForPort(Vector<CyberCore::MessageWithMessagePorts>&& messages, uint64_t messageCallbackIdentifier, uint64_t messageBatchIdentifier);
    void checkProcessLocalPortForActivity(const CyberCore::MessagePortIdentifier&, uint64_t callbackIdentifier);
    void didCheckRemotePortForActivity(uint64_t callbackIdentifier, bool hasActivity);
    
private:
    WebMessagePortChannelProvider();
    ~WebMessagePortChannelProvider() final;

    void createNewMessagePortChannel(const CyberCore::MessagePortIdentifier& local, const CyberCore::MessagePortIdentifier& remote) final;
    void entangleLocalPortInThisProcessToRemote(const CyberCore::MessagePortIdentifier& local, const CyberCore::MessagePortIdentifier& remote) final;
    void messagePortDisentangled(const CyberCore::MessagePortIdentifier& local) final;
    void messagePortClosed(const CyberCore::MessagePortIdentifier& local) final;
    void takeAllMessagesForPort(const CyberCore::MessagePortIdentifier&, Function<void(Vector<CyberCore::MessageWithMessagePorts>&&, Function<void()>&&)>&&) final;
    void postMessageToRemote(CyberCore::MessageWithMessagePorts&&, const CyberCore::MessagePortIdentifier& remoteTarget) final;
    void checkProcessLocalPortForActivity(const CyberCore::MessagePortIdentifier&, CyberCore::ProcessIdentifier, CompletionHandler<void(HasActivity)>&&) final;

    // To be called only in the UI process
    void checkRemotePortForActivity(const CyberCore::MessagePortIdentifier& remoteTarget, CompletionHandler<void(HasActivity)>&& callback) final;

    Lock m_takeAllMessagesCallbackLock;
    HashMap<uint64_t, CompletionHandler<void(Vector<CyberCore::MessageWithMessagePorts>&&, Function<void()>&&)>> m_takeAllMessagesCallbacks;
    Lock m_remoteActivityCallbackLock;
    HashMap<uint64_t, CompletionHandler<void(HasActivity)>> m_remoteActivityCallbacks;
};

} // namespace CyberKit
