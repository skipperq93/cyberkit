/*
 * Copyright (C) 2021 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(WEB_RTC)

#include "DataReference.h"
#include "WorkQueueMessageReceiver.h"
#include <CyberCore/ProcessQualified.h>
#include <CyberCore/RTCDataChannelRemoteHandler.h>
#include <CyberCore/RTCDataChannelRemoteHandlerConnection.h>
#include <CyberCore/RTCDataChannelRemoteSource.h>
#include <CyberCore/RTCDataChannelRemoteSourceConnection.h>
#include <wtf/WorkQueue.h>

namespace CyberKit {

class RTCDataChannelRemoteManager final : private IPC::MessageReceiver {
public:
    static RTCDataChannelRemoteManager& sharedManager();

    CyberCore::RTCDataChannelRemoteHandlerConnection& remoteHandlerConnection();
    bool connectToRemoteSource(CyberCore::RTCDataChannelIdentifier source, CyberCore::RTCDataChannelIdentifier handler);

private:
    RTCDataChannelRemoteManager();
    void initialize();

    // IPC::MessageReceiver overrides.
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

    // Messages
    void sendData(CyberCore::RTCDataChannelIdentifier, bool isRaw, const IPC::DataReference&);
    void close(CyberCore::RTCDataChannelIdentifier);

    // To handler
    void changeReadyState(CyberCore::RTCDataChannelIdentifier, CyberCore::RTCDataChannelState);
    void receiveData(CyberCore::RTCDataChannelIdentifier, bool isRaw, const IPC::DataReference&);
    void detectError(CyberCore::RTCDataChannelIdentifier, CyberCore::RTCErrorDetailType, String&&);
    void bufferedAmountIsDecreasing(CyberCore::RTCDataChannelIdentifier, size_t);

    CyberCore::RTCDataChannelRemoteSourceConnection& remoteSourceConnection();
    void postTaskToHandler(CyberCore::RTCDataChannelIdentifier, Function<void(CyberCore::RTCDataChannelRemoteHandler&)>&&);
    CyberCore::RTCDataChannelRemoteSource* sourceFromIdentifier(CyberCore::RTCDataChannelIdentifier);

    class RemoteHandlerConnection : public CyberCore::RTCDataChannelRemoteHandlerConnection {
    public:
        static Ref<RemoteHandlerConnection> create(Ref<WorkQueue>&&);

        void connectToSource(CyberCore::RTCDataChannelRemoteHandler&, CyberCore::ScriptExecutionContextIdentifier, CyberCore::RTCDataChannelIdentifier, CyberCore::RTCDataChannelIdentifier) final;
        void sendData(CyberCore::RTCDataChannelIdentifier, bool isRaw, const unsigned char*, size_t) final;
        void close(CyberCore::RTCDataChannelIdentifier) final;

    private:
        explicit RemoteHandlerConnection(Ref<WorkQueue>&&);

        Ref<IPC::Connection> m_connection;
        Ref<WorkQueue> m_queue;
    };

    class RemoteSourceConnection : public CyberCore::RTCDataChannelRemoteSourceConnection {
    public:
        static Ref<RemoteSourceConnection> create();

    private:
        RemoteSourceConnection();

        void didChangeReadyState(CyberCore::RTCDataChannelIdentifier, CyberCore::RTCDataChannelState) final;
        void didReceiveStringData(CyberCore::RTCDataChannelIdentifier, const String&) final;
        void didReceiveRawData(CyberCore::RTCDataChannelIdentifier, const uint8_t*, size_t) final;
        void didDetectError(CyberCore::RTCDataChannelIdentifier, CyberCore::RTCErrorDetailType, const String&) final;
        void bufferedAmountIsDecreasing(CyberCore::RTCDataChannelIdentifier, size_t) final;

        Ref<IPC::Connection> m_connection;
    };

    struct RemoteHandler {
        WeakPtr<CyberCore::RTCDataChannelRemoteHandler> handler;
        CyberCore::ScriptExecutionContextIdentifier contextIdentifier;
    };

    Ref<WorkQueue> m_queue;
    RefPtr<IPC::Connection> m_connection;
    RefPtr<RemoteHandlerConnection> m_remoteHandlerConnection;
    RefPtr<RemoteSourceConnection> m_remoteSourceConnection;
    HashMap<CyberCore::RTCDataChannelLocalIdentifier, UniqueRef<CyberCore::RTCDataChannelRemoteSource>> m_sources;
    HashMap<CyberCore::RTCDataChannelLocalIdentifier, RemoteHandler> m_handlers;
};

} // namespace CyberKit

#endif // ENABLE(WEB_RTC)
