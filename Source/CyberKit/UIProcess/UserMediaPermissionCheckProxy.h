/*
 * Copyright (C) 2015-2018 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "APIObject.h"
#include <CyberCore/MediaConstraints.h>
#include <wtf/CompletionHandler.h>
#include <wtf/text/WTFString.h>

namespace CyberCore {
class SecurityOrigin;
}

namespace CyberKit {

class UserMediaPermissionCheckProxy : public API::ObjectImpl<API::Object::Type::UserMediaPermissionCheck> {
public:
    enum class PermissionInfo { Error, Unknown, Granted };
    using CompletionHandler = WTF::CompletionHandler<void(PermissionInfo)>;

    static Ref<UserMediaPermissionCheckProxy> create(uint64_t frameID, CompletionHandler&& handler, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin)
    {
        return adoptRef(*new UserMediaPermissionCheckProxy(frameID, WTFMove(handler), WTFMove(userMediaDocumentOrigin), WTFMove(topLevelDocumentOrigin)));
    }

    void deny() { setUserMediaAccessInfo(false); }
    void setUserMediaAccessInfo(bool);
    void invalidate() { complete(PermissionInfo::Error); }

    uint64_t frameID() const { return m_frameID; }
    CyberCore::SecurityOrigin& userMediaDocumentSecurityOrigin() { return m_userMediaDocumentSecurityOrigin.get(); }
    CyberCore::SecurityOrigin& topLevelDocumentSecurityOrigin() { return m_topLevelDocumentSecurityOrigin.get(); }
    
private:
    UserMediaPermissionCheckProxy(uint64_t frameID, CompletionHandler&&, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin);
    ~UserMediaPermissionCheckProxy();

    void complete(PermissionInfo);
    
    uint64_t m_frameID;
    CompletionHandler m_completionHandler;
    Ref<CyberCore::SecurityOrigin> m_userMediaDocumentSecurityOrigin;
    Ref<CyberCore::SecurityOrigin> m_topLevelDocumentSecurityOrigin;
};

} // namespace CyberKit
