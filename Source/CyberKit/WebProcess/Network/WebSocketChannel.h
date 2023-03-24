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

#include "DataReference.h"
#include "MessageReceiver.h"
#include "MessageSender.h"
#include "WebPageProxyIdentifier.h"
#include <CyberCore/NetworkSendQueue.h>
#include <CyberCore/ResourceRequest.h>
#include <CyberCore/ResourceResponse.h>
#include <CyberCore/ThreadableWebSocketChannel.h>
#include <CyberCore/WebSocketChannelInspector.h>
#include <CyberCore/WebSocketFrame.h>
#include <wtf/WeakPtr.h>

namespace IPC {
class Connection;
class Decoder;
}

namespace CyberCore {
class WeakPtrImplWithEventTargetData;
}

namespace CyberKit {

class WebSocketChannel : public IPC::MessageSender, public IPC::MessageReceiver, public CyberCore::ThreadableWebSocketChannel, public RefCounted<WebSocketChannel> {
public:
    static Ref<WebSocketChannel> create(WebPageProxyIdentifier, CyberCore::Document&, CyberCore::WebSocketChannelClient&);
    ~WebSocketChannel();

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&);

    void networkProcessCrashed();

    using RefCounted<WebSocketChannel>::ref;
    using RefCounted<WebSocketChannel>::deref;

private:
    WebSocketChannel(WebPageProxyIdentifier, CyberCore::Document&, CyberCore::WebSocketChannelClient&);

    static CyberCore::NetworkSendQueue createMessageQueue(CyberCore::Document&, WebSocketChannel&);

    // ThreadableWebSocketChannel
    ConnectStatus connect(const URL&, const String& protocol) final;
    String subprotocol() final;
    String extensions() final;
    SendResult send(CString&&) final;
    SendResult send(const JSC::ArrayBuffer&, unsigned byteOffset, unsigned byteLength) final;
    SendResult send(CyberCore::Blob&) final;
    unsigned bufferedAmount() const final;
    void close(int code, const String& reason) final;
    void fail(String&& reason) final;
    void disconnect() final;
    void suspend() final;
    void resume() final;
    void refThreadableWebSocketChannel() final { ref(); }
    void derefThreadableWebSocketChannel() final { deref(); }

    void notifySendFrame(CyberCore::WebSocketFrame::OpCode, const uint8_t* data, size_t length);
    void logErrorMessage(const String&);

    // Message receivers
    void didConnect(String&& subprotocol, String&& extensions);
    void didReceiveText(String&&);
    void didReceiveBinaryData(IPC::DataReference&&);
    void didClose(unsigned short code, String&&);
    void didReceiveMessageError(String&&);
    void didSendHandshakeRequest(CyberCore::ResourceRequest&&);
    void didReceiveHandshakeResponse(CyberCore::ResourceResponse&&);

    // MessageSender
    IPC::Connection* messageSenderConnection() const final;
    uint64_t messageSenderDestinationID() const final;

    bool increaseBufferedAmount(size_t);
    void decreaseBufferedAmount(size_t);
    template<typename T> void sendMessage(T&&, size_t byteLength);

    const CyberCore::WebSocketChannelInspector* channelInspector() const final { return &m_inspector; }
    CyberCore::WebSocketChannelIdentifier progressIdentifier() const final { return m_inspector.progressIdentifier(); }
    bool hasCreatedHandshake() const final { return !m_url.isNull(); }
    bool isConnected() const final { return !m_handshakeResponse.isNull(); }
    CyberCore::ResourceRequest clientHandshakeRequest(const CookieGetter&) const final { return m_handshakeRequest; }
    const CyberCore::ResourceResponse& serverHandshakeResponse() const final { return m_handshakeResponse; }

    WeakPtr<CyberCore::Document, CyberCore::WeakPtrImplWithEventTargetData> m_document;
    WeakPtr<CyberCore::WebSocketChannelClient> m_client;
    URL m_url;
    String m_subprotocol;
    String m_extensions;
    size_t m_bufferedAmount { 0 };
    bool m_isClosing { false };
    CyberCore::NetworkSendQueue m_messageQueue;
    CyberCore::WebSocketChannelInspector m_inspector;
    CyberCore::ResourceRequest m_handshakeRequest;
    CyberCore::ResourceResponse m_handshakeResponse;
    WebPageProxyIdentifier m_webPageProxyID;
};

} // namespace CyberKit
