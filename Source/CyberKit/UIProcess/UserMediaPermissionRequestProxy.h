/*
 * Copyright (C) 2014 Igalia S.L.
 * Copyright (C) 2016-2022 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

#include "APIObject.h"
#include <CyberCore/CaptureDevice.h>
#include <CyberCore/FrameIdentifier.h>
#include <CyberCore/MediaDeviceHashSalts.h>
#include <CyberCore/MediaStreamRequest.h>
#include <CyberCore/UserMediaRequestIdentifier.h>
#include <wtf/CompletionHandler.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace CyberCore {
class SecurityOrigin;
}

namespace WebKit {

class UserMediaPermissionRequestManagerProxy;

class UserMediaPermissionRequestProxy : public API::ObjectImpl<API::Object::Type::UserMediaPermissionRequest> {
public:
    static Ref<UserMediaPermissionRequestProxy> create(UserMediaPermissionRequestManagerProxy&, CyberCore::UserMediaRequestIdentifier, CyberCore::FrameIdentifier, CyberCore::FrameIdentifier, Ref<CyberCore::SecurityOrigin>&&, Ref<CyberCore::SecurityOrigin>&&, Vector<CyberCore::CaptureDevice>&&, Vector<CyberCore::CaptureDevice>&&, CyberCore::MediaStreamRequest&&, CompletionHandler<void(bool)>&& = { });

    ~UserMediaPermissionRequestProxy() = default;

    void allow(const String& audioDeviceUID, const String& videoDeviceUID);
    void allow();
    void promptForGetUserMedia();

    enum class UserMediaDisplayCapturePromptType { Window, Screen, UserChoose };
    virtual void promptForGetDisplayMedia(UserMediaDisplayCapturePromptType = UserMediaDisplayCapturePromptType::UserChoose);
    virtual bool canPromptForGetDisplayMedia();

    void doDefaultAction();
    enum class UserMediaAccessDenialReason { NoConstraints, UserMediaDisabled, NoCaptureDevices, InvalidConstraint, HardwareError, PermissionDenied, OtherFailure };
    void deny(UserMediaAccessDenialReason = UserMediaAccessDenialReason::UserMediaDisabled);

    void invalidate();
    bool isPending() const { return m_manager; }

    bool requiresAudioCapture() const { return m_eligibleAudioDevices.size(); }
    bool requiresVideoCapture() const { return !requiresDisplayCapture() && m_eligibleVideoDevices.size(); }
    bool requiresDisplayCapture() const { return m_request.type == CyberCore::MediaStreamRequest::Type::DisplayMedia || m_request.type == CyberCore::MediaStreamRequest::Type::DisplayMediaWithAudio; }
    bool requiresDisplayCaptureWithAudio() const { return m_request.type == CyberCore::MediaStreamRequest::Type::DisplayMediaWithAudio; }

    void setEligibleVideoDeviceUIDs(Vector<CyberCore::CaptureDevice>&& devices) { m_eligibleVideoDevices = WTFMove(devices); }
    void setEligibleAudioDeviceUIDs(Vector<CyberCore::CaptureDevice>&& devices) { m_eligibleAudioDevices = WTFMove(devices); }

    Vector<String> videoDeviceUIDs() const;
    Vector<String> audioDeviceUIDs() const;
    bool hasAudioDevice() const { return !m_eligibleAudioDevices.isEmpty(); }
    bool hasVideoDevice() const { return !m_eligibleVideoDevices.isEmpty(); }

    bool hasPersistentAccess() const { return m_hasPersistentAccess; }
    void setHasPersistentAccess() { m_hasPersistentAccess = true; }

    CyberCore::UserMediaRequestIdentifier userMediaID() const { return m_userMediaID; }
    CyberCore::FrameIdentifier mainFrameID() const { return m_mainFrameID; }
    CyberCore::FrameIdentifier frameID() const { return m_frameID; }

    CyberCore::SecurityOrigin& topLevelDocumentSecurityOrigin() { return m_topLevelDocumentSecurityOrigin.get(); }
    CyberCore::SecurityOrigin& userMediaDocumentSecurityOrigin() { return m_userMediaDocumentSecurityOrigin.get(); }
    const CyberCore::SecurityOrigin& topLevelDocumentSecurityOrigin() const { return m_topLevelDocumentSecurityOrigin.get(); }
    const CyberCore::SecurityOrigin& userMediaDocumentSecurityOrigin() const { return m_userMediaDocumentSecurityOrigin.get(); }

    const CyberCore::MediaStreamRequest& userRequest() const { return m_request; }

    CyberCore::MediaStreamRequest::Type requestType() const { return m_request.type; }

    void setDeviceIdentifierHashSalts(CyberCore::MediaDeviceHashSalts&& salts) { m_deviceIdentifierHashSalts = WTFMove(salts); }
    const CyberCore::MediaDeviceHashSalts& deviceIdentifierHashSalts() const { return m_deviceIdentifierHashSalts; }

    CyberCore::CaptureDevice audioDevice() const { return m_eligibleAudioDevices.isEmpty() ? CyberCore::CaptureDevice { } : m_eligibleAudioDevices[0]; }
    CyberCore::CaptureDevice videoDevice() const { return m_eligibleVideoDevices.isEmpty() ? CyberCore::CaptureDevice { } : m_eligibleVideoDevices[0]; }

#if ENABLE(MEDIA_STREAM)
    bool isUserGesturePriviledged() const { return m_request.isUserGesturePriviledged; }
#endif

    CompletionHandler<void(bool)> decisionCompletionHandler() { return std::exchange(m_decisionCompletionHandler, { }); }

protected:
    UserMediaPermissionRequestProxy(UserMediaPermissionRequestManagerProxy&, CyberCore::UserMediaRequestIdentifier, CyberCore::FrameIdentifier mainFrameID, CyberCore::FrameIdentifier, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin, Vector<CyberCore::CaptureDevice>&& audioDevices, Vector<CyberCore::CaptureDevice>&& videoDevices, CyberCore::MediaStreamRequest&&, CompletionHandler<void(bool)>&&);

    UserMediaPermissionRequestManagerProxy* manager() const { return m_manager; }

private:
    UserMediaPermissionRequestManagerProxy* m_manager;
    CyberCore::UserMediaRequestIdentifier m_userMediaID;
    CyberCore::FrameIdentifier m_mainFrameID;
    CyberCore::FrameIdentifier m_frameID;
    Ref<CyberCore::SecurityOrigin> m_userMediaDocumentSecurityOrigin;
    Ref<CyberCore::SecurityOrigin> m_topLevelDocumentSecurityOrigin;
    Vector<CyberCore::CaptureDevice> m_eligibleVideoDevices;
    Vector<CyberCore::CaptureDevice> m_eligibleAudioDevices;
    CyberCore::MediaStreamRequest m_request;
    bool m_hasPersistentAccess { false };
    CyberCore::MediaDeviceHashSalts m_deviceIdentifierHashSalts;
    CompletionHandler<void(bool)> m_decisionCompletionHandler;
};

String convertEnumerationToString(UserMediaPermissionRequestProxy::UserMediaAccessDenialReason);

} // namespace WebKit

namespace WTF {

template<typename Type>
struct LogArgument;

template <>
struct LogArgument<WebKit::UserMediaPermissionRequestProxy::UserMediaAccessDenialReason> {
    static String toString(const WebKit::UserMediaPermissionRequestProxy::UserMediaAccessDenialReason type)
    {
        return convertEnumerationToString(type);
    }
};

}; // namespace WTF

