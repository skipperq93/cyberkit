/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
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

#if ENABLE(GPU_PROCESS) && ENABLE(ENCRYPTED_MEDIA)

#include "Connection.h"
#include "DataReference.h"
#include "MessageReceiver.h"
#include "RemoteCDMInstanceSessionIdentifier.h"
#include "RemoteCDMProxy.h"
#include <CyberCore/CDMInstanceSession.h>
#include <wtf/CompletionHandler.h>
#include <wtf/WeakPtr.h>

namespace IPC {
class SharedBufferCopy;
}

namespace CyberKit {

class RemoteCDMInstanceSessionProxy final : private IPC::MessageReceiver, private CyberCore::CDMInstanceSessionClient {
public:
    static std::unique_ptr<RemoteCDMInstanceSessionProxy> create(WeakPtr<RemoteCDMProxy>&&, Ref<CyberCore::CDMInstanceSession>&&, RemoteCDMInstanceSessionIdentifier);
    virtual ~RemoteCDMInstanceSessionProxy();

private:
    friend class RemoteCDMFactoryProxy;
    RemoteCDMInstanceSessionProxy(WeakPtr<RemoteCDMProxy>&&, Ref<CyberCore::CDMInstanceSession>&&, RemoteCDMInstanceSessionIdentifier);

    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

    // Types
    using LicenseType = CyberCore::CDMInstanceSession::LicenseType;
    using KeyStatusVector = CyberCore::CDMInstanceSession::KeyStatusVector;
    using Message = CyberCore::CDMInstanceSession::Message;
    using SessionLoadFailure = CyberCore::CDMInstanceSession::SessionLoadFailure;
    using LicenseCallback = CompletionHandler<void(IPC::SharedBufferCopy&&, const String& sessionId, bool, bool)>;
    using LicenseUpdateCallback = CompletionHandler<void(bool, Optional<KeyStatusVector>&&, Optional<double>&&, Optional<Message>&&, bool)>;
    using LoadSessionCallback = CompletionHandler<void(Optional<KeyStatusVector>&&, Optional<double>&&, Optional<Message>&&, bool, SessionLoadFailure)>;
    using CloseSessionCallback = CompletionHandler<void()>;
    using RemoveSessionDataCallback = CompletionHandler<void(KeyStatusVector&&, Optional<IPC::SharedBufferCopy>&&, bool)>;
    using StoreRecordCallback = CompletionHandler<void()>;

    // Messages
    void requestLicense(LicenseType, AtomString initDataType, IPC::SharedBufferCopy&& initData, LicenseCallback&&);
    void updateLicense(String sessionId, LicenseType, IPC::SharedBufferCopy&& response, LicenseUpdateCallback&&);
    void loadSession(LicenseType, String sessionId, String origin, LoadSessionCallback&&);
    void closeSession(const String& sessionId, CloseSessionCallback&&);
    void removeSessionData(const String& sessionId, LicenseType, RemoveSessionDataCallback&&);
    void storeRecordOfKeyUsage(const String& sessionId);
    void displayIDChanged(PlatformDisplayID);

    // CDMInstanceSessionClient
    void updateKeyStatuses(KeyStatusVector&&) final;
    void sendMessage(CyberCore::CDMMessageType, Ref<CyberCore::SharedBuffer>&& message) final;
    void sessionIdChanged(const String&) final;
    PlatformDisplayID displayID() final { return m_displayID; }

    WeakPtr<RemoteCDMProxy> m_cdm;
    Ref<CyberCore::CDMInstanceSession> m_session;
    RemoteCDMInstanceSessionIdentifier m_identifier;
    PlatformDisplayID m_displayID { 0 };
};

}

#endif
