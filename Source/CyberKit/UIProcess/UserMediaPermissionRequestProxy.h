/*
 * Copyright (C) 2014 Igalia S.L.
 * Copyright (C) 2016-2019 Apple Inc. All rights reserved.
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
#include <CyberCore/MediaStreamRequest.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace CyberCore {
class SecurityOrigin;
}

namespace CyberKit {

class UserMediaPermissionRequestManagerProxy;

class UserMediaPermissionRequestProxy : public API::ObjectImpl<API::Object::Type::UserMediaPermissionRequest> {
public:
    static Ref<UserMediaPermissionRequestProxy> create(UserMediaPermissionRequestManagerProxy& manager, uint64_t userMediaID, CyberCore::FrameIdentifier mainFrameID, CyberCore::FrameIdentifier frameID, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin, Vector<CyberCore::CaptureDevice>&& audioDevices, Vector<CyberCore::CaptureDevice>&& videoDevices, CyberCore::MediaStreamRequest&& request)
    {
        return adoptRef(*new UserMediaPermissionRequestProxy(manager, userMediaID, mainFrameID, frameID, WTFMove(userMediaDocumentOrigin), WTFMove(topLevelDocumentOrigin), WTFMove(audioDevices), WTFMove(videoDevices), WTFMove(request)));
    }

    void allow(const String& audioDeviceUID, const String& videoDeviceUID);
    void allow();

    enum class UserMediaAccessDenialReason { NoConstraints, UserMediaDisabled, NoCaptureDevices, InvalidConstraint, HardwareError, PermissionDenied, OtherFailure };
    void deny(UserMediaAccessDenialReason = UserMediaAccessDenialReason::UserMediaDisabled);

    void doDefaultAction();

    void invalidate();
    bool isPending() const { return m_manager; }

    bool requiresAudioCapture() const { return m_eligibleAudioDevices.size(); }
    bool requiresVideoCapture() const { return !requiresDisplayCapture() && m_eligibleVideoDevices.size(); }
    bool requiresDisplayCapture() const { return m_request.type == CyberCore::MediaStreamRequest::Type::DisplayMedia && m_eligibleVideoDevices.size(); }

    void setEligibleVideoDeviceUIDs(Vector<CyberCore::CaptureDevice>&& devices) { m_eligibleVideoDevices = WTFMove(devices); }
    void setEligibleAudioDeviceUIDs(Vector<CyberCore::CaptureDevice>&& devices) { m_eligibleAudioDevices = WTFMove(devices); }

    Vector<String> videoDeviceUIDs() const;
    Vector<String> audioDeviceUIDs() const;
    bool hasAudioDevice() const { return !m_eligibleAudioDevices.isEmpty(); }
    bool hasVideoDevice() const { return !m_eligibleVideoDevices.isEmpty(); }

    bool hasPersistentAccess() const { return m_hasPersistentAccess; }
    void setHasPersistentAccess() { m_hasPersistentAccess = true; }

    uint64_t userMediaID() const { return m_userMediaID; }
    CyberCore::FrameIdentifier mainFrameID() const { return m_mainFrameID; }
    CyberCore::FrameIdentifier frameID() const { return m_frameID; }

    CyberCore::SecurityOrigin& topLevelDocumentSecurityOrigin() { return m_topLevelDocumentSecurityOrigin.get(); }
    CyberCore::SecurityOrigin& userMediaDocumentSecurityOrigin() { return m_userMediaDocumentSecurityOrigin.get(); }
    const CyberCore::SecurityOrigin& topLevelDocumentSecurityOrigin() const { return m_topLevelDocumentSecurityOrigin.get(); }
    const CyberCore::SecurityOrigin& userMediaDocumentSecurityOrigin() const { return m_userMediaDocumentSecurityOrigin.get(); }

    const CyberCore::MediaStreamRequest& userRequest() const { return m_request; }

    CyberCore::MediaStreamRequest::Type requestType() const { return m_request.type; }

    void setDeviceIdentifierHashSalt(String&& salt) { m_deviceIdentifierHashSalt = WTFMove(salt); }
    const String& deviceIdentifierHashSalt() const { return m_deviceIdentifierHashSalt; }

    CyberCore::CaptureDevice audioDevice() const { return m_eligibleAudioDevices.isEmpty() ? CyberCore::CaptureDevice { } : m_eligibleAudioDevices[0]; }
    CyberCore::CaptureDevice videoDevice() const { return m_eligibleVideoDevices.isEmpty() ? CyberCore::CaptureDevice { } : m_eligibleVideoDevices[0]; }

#if ENABLE(MEDIA_STREAM)
    bool isUserGesturePriviledged() const { return m_request.isUserGesturePriviledged; }
#endif

private:
    UserMediaPermissionRequestProxy(UserMediaPermissionRequestManagerProxy&, uint64_t userMediaID, CyberCore::FrameIdentifier mainFrameID, CyberCore::FrameIdentifier, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin, Vector<CyberCore::CaptureDevice>&& audioDevices, Vector<CyberCore::CaptureDevice>&& videoDevices, CyberCore::MediaStreamRequest&&);

    UserMediaPermissionRequestManagerProxy* m_manager;
    uint64_t m_userMediaID;
    CyberCore::FrameIdentifier m_mainFrameID;
    CyberCore::FrameIdentifier m_frameID;
    Ref<CyberCore::SecurityOrigin> m_userMediaDocumentSecurityOrigin;
    Ref<CyberCore::SecurityOrigin> m_topLevelDocumentSecurityOrigin;
    Vector<CyberCore::CaptureDevice> m_eligibleVideoDevices;
    Vector<CyberCore::CaptureDevice> m_eligibleAudioDevices;
    CyberCore::MediaStreamRequest m_request;
    bool m_hasPersistentAccess { false };
    String m_deviceIdentifierHashSalt;
};

String convertEnumerationToString(UserMediaPermissionRequestProxy::UserMediaAccessDenialReason);

} // namespace CyberKit

namespace WTF {

template<typename Type>
struct LogArgument;

template <>
struct LogArgument<CyberKit::UserMediaPermissionRequestProxy::UserMediaAccessDenialReason> {
    static String toString(const CyberKit::UserMediaPermissionRequestProxy::UserMediaAccessDenialReason type)
    {
        return convertEnumerationToString(type);
    }
};

}; // namespace WTF

