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
#include "MessageReceiver.h"
#include "RemoteCDMInstanceIdentifier.h"
#include "RemoteCDMInstanceSessionIdentifier.h"
#include "RemoteCDMProxy.h"
#include <CyberCore/CDMInstance.h>
#include <wtf/Ref.h>
#include <wtf/UniqueRef.h>

namespace CyberCore {
class CDMInstance;
struct CDMKeySystemConfiguration;
}

namespace IPC {
class SharedBufferCopy;
}

namespace CyberKit {

struct RemoteCDMInstanceConfiguration;
class RemoteCDMInstanceSessionProxy;

class RemoteCDMInstanceProxy : private IPC::MessageReceiver  {
public:
    static std::unique_ptr<RemoteCDMInstanceProxy> create(WeakPtr<RemoteCDMProxy>&&, Ref<CyberCore::CDMInstance>&&);
    ~RemoteCDMInstanceProxy();

    const RemoteCDMInstanceConfiguration& configuration() const { return m_configuration.get(); }
    CyberCore::CDMInstance& instance() { return m_instance; }

private:
    friend class RemoteCDMFactoryProxy;
    RemoteCDMInstanceProxy(WeakPtr<RemoteCDMProxy>&&, Ref<CyberCore::CDMInstance>&&, UniqueRef<RemoteCDMInstanceConfiguration>&&);

    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;
    void didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, std::unique_ptr<IPC::Encoder>&) final;

    using SuccessValue = CyberCore::CDMInstance::SuccessValue;
    using AllowDistinctiveIdentifiers = CyberCore::CDMInstance::AllowDistinctiveIdentifiers;
    using AllowPersistentState = CyberCore::CDMInstance::AllowPersistentState;

    // Messages
    void initializeWithConfiguration(const CyberCore::CDMKeySystemConfiguration&, AllowDistinctiveIdentifiers, AllowPersistentState, CompletionHandler<void(SuccessValue)>&&);
    void setServerCertificate(IPC::SharedBufferCopy&&, CompletionHandler<void(SuccessValue)>&&);
    void setStorageDirectory(const String&);
    void createSession(CompletionHandler<void(const RemoteCDMInstanceSessionIdentifier&)>&&);

    WeakPtr<RemoteCDMProxy> m_cdm;
    Ref<CyberCore::CDMInstance> m_instance;
    UniqueRef<RemoteCDMInstanceConfiguration> m_configuration;
    HashMap<RemoteCDMInstanceSessionIdentifier, std::unique_ptr<RemoteCDMInstanceSessionProxy>> m_sessions;
};

}

#endif
