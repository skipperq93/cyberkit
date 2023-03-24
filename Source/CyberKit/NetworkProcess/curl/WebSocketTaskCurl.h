/*
 * Copyright (C) 2022 Sony Interactive Entertainment Inc.
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
#include "WebSocketTask.h"
#include <CyberCore/CurlStream.h>
#include <CyberCore/ResourceRequest.h>
#include <CyberCore/WebSocketChannel.h>
#include <CyberCore/WebSocketDeflateFramer.h>

namespace CyberCore {
class CurlStreamScheduler;
class SharedBuffer;
}

namespace CyberKit {

class NetworkSocketChannel;
struct SessionSet;

class WebSocketTask : public CanMakeWeakPtr<WebSocketTask>, public CyberCore::CurlStream::Client {
    WTF_MAKE_FAST_ALLOCATED;
public:
    WebSocketTask(NetworkSocketChannel&, const CyberCore::ResourceRequest&, const String& protocol);
    ~WebSocketTask();

    void sendString(const IPC::DataReference&, CompletionHandler<void()>&&);
    void sendData(const IPC::DataReference&, CompletionHandler<void()>&&);
    void close(int32_t code, const String& reason);

    void cancel();
    void resume();

    SessionSet* sessionSet() { return nullptr; }

private:
    enum class State : uint8_t {
        Connecting,
        Handshaking,
        Opened,
        Closing,
        Closed
    };

    void didOpen(CyberCore::CurlStreamID) final;
    void didSendData(CyberCore::CurlStreamID, size_t) final { };
    void didReceiveData(CyberCore::CurlStreamID, const CyberCore::SharedBuffer&) final;
    void didFail(CyberCore::CurlStreamID, CURLcode) final;

    bool appendReceivedBuffer(const CyberCore::SharedBuffer&);
    void skipReceivedBuffer(size_t len);

    Expected<bool, String> validateOpeningHandshake();
    std::optional<String> receiveFrames(Function<void(CyberCore::WebSocketFrame::OpCode, const uint8_t*, size_t)>&&);
    std::optional<String> validateFrame(const CyberCore::WebSocketFrame&);

    bool sendFrame(CyberCore::WebSocketFrame::OpCode, const uint8_t* data, size_t dataLength);
    void sendClosingHandshakeIfNeeded(int32_t, const String& reason);

    void didFail(String&& reason);
    void didClose(int32_t code, const String& reason);

    bool isStreamInvalidated() { return m_streamID == CyberCore::invalidCurlStreamID; }
    void destructStream();

    NetworkSocketChannel& m_channel;
    CyberCore::ResourceRequest m_request;
    String m_protocol;

    CyberCore::CurlStreamScheduler& m_scheduler;
    CyberCore::CurlStreamID m_streamID { CyberCore::invalidCurlStreamID };

    State m_state { State::Connecting };

    std::unique_ptr<CyberCore::WebSocketHandshake> m_handshake;
    CyberCore::WebSocketDeflateFramer m_deflateFramer;

    bool m_didCompleteOpeningHandshake { false };

    bool m_shouldDiscardReceivedData { false };
    Vector<uint8_t> m_receiveBuffer;

    bool m_hasContinuousFrame { false };
    CyberCore::WebSocketFrame::OpCode m_continuousFrameOpCode { CyberCore::WebSocketFrame::OpCode::OpCodeInvalid };
    Vector<uint8_t> m_continuousFrameData;

    bool m_receivedClosingHandshake { false };
    int32_t m_closeEventCode { CyberCore::WebSocketChannel::CloseEventCode::CloseEventCodeNotSpecified };
    String m_closeEventReason;

    bool m_didSendClosingHandshake { false };
    bool m_receivedDidFail { false };
};

} // namespace CyberKit
