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

#if ENABLE(WEB_AUTHN)

#include "APIObject.h"
#include <CyberCore/AuthenticatorTransport.h>
#include <variant>
#include <wtf/UniqueRef.h>
#include <wtf/WeakPtr.h>
#include <wtf/text/WTFString.h>

namespace CyberCore {
enum class ClientDataType : bool;

class AuthenticatorResponse;

struct ExceptionData;
struct MockWebAuthenticationConfiguration;
}

namespace CyberKit {
class AuthenticatorManager;

struct WebAuthenticationRequestData;
}

namespace API {

class WebAuthenticationPanelClient;

class WebAuthenticationPanel final : public ObjectImpl<Object::Type::WebAuthenticationPanel>, public CanMakeWeakPtr<WebAuthenticationPanel> {
public:
    using Response = std::variant<Ref<CyberCore::AuthenticatorResponse>, CyberCore::ExceptionData>;
    using Callback = CompletionHandler<void(Response&&)>;

    WebAuthenticationPanel();
    ~WebAuthenticationPanel();

    void handleRequest(CyberKit::WebAuthenticationRequestData&&, Callback&&);
    void cancel() const;
    void setMockConfiguration(CyberCore::MockWebAuthenticationConfiguration&&);

    const WebAuthenticationPanelClient& client() const { return m_client.get(); }
    void setClient(UniqueRef<WebAuthenticationPanelClient>&&);

    // FIXME: <rdar://problem/71509848> Remove the following deprecated methods.
    using TransportSet = HashSet<CyberCore::AuthenticatorTransport, WTF::IntHash<CyberCore::AuthenticatorTransport>, WTF::StrongEnumHashTraits<CyberCore::AuthenticatorTransport>>;
    static Ref<WebAuthenticationPanel> create(const CyberKit::AuthenticatorManager&, const WTF::String& rpId, const TransportSet&, CyberCore::ClientDataType, const WTF::String& userName);
    WTF::String rpId() const { return m_rpId; }
    const Vector<CyberCore::AuthenticatorTransport>& transports() const { return m_transports; }
    CyberCore::ClientDataType clientDataType() const { return m_clientDataType; }
    WTF::String userName() const { return m_userName; }

private:
    // FIXME: <rdar://problem/71509848> Remove the following deprecated method.
    WebAuthenticationPanel(const CyberKit::AuthenticatorManager&, const WTF::String& rpId, const TransportSet&, CyberCore::ClientDataType, const WTF::String& userName);

    std::unique_ptr<CyberKit::AuthenticatorManager> m_manager; // FIXME: <rdar://problem/71509848> Change to UniqueRef.
    UniqueRef<WebAuthenticationPanelClient> m_client;

    // FIXME: <rdar://problem/71509848> Remove the following deprecated fields.
    WeakPtr<CyberKit::AuthenticatorManager> m_weakManager;
    WTF::String m_rpId;
    Vector<CyberCore::AuthenticatorTransport> m_transports;
    CyberCore::ClientDataType m_clientDataType;
    WTF::String m_userName;
};

} // namespace API

#endif // ENABLE(WEB_AUTHN)
