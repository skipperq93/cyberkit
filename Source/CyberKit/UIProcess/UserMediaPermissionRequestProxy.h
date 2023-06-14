/*
 * Copyright (C) 2014 Igalia S.L.
 * Copyright (C) 2016 Apple Inc. All rights reserved.
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
    static Ref<UserMediaPermissionRequestProxy> create(UserMediaPermissionRequestManagerProxy& manager, uint64_t userMediaID, uint64_t mainFrameID, uint64_t frameID, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin, Vector<CyberCore::CaptureDevice>&& audioDevices, Vector<CyberCore::CaptureDevice>&& videoDevices, CyberCore::MediaStreamRequest&& request)
    {
        return adoptRef(*new UserMediaPermissionRequestProxy(manager, userMediaID, mainFrameID, frameID, WTFMove(userMediaDocumentOrigin), WTFMove(topLevelDocumentOrigin), WTFMove(audioDevices), WTFMove(videoDevices), WTFMove(request)));
    }

    void allow(const String& audioDeviceUID, const String& videoDeviceUID);
    void allow(CyberCore::CaptureDevice&& audioDevice, CyberCore::CaptureDevice&& videoDevice);
    void allow();

    enum class UserMediaAccessDenialReason { NoConstraints, UserMediaDisabled, NoCaptureDevices, InvalidConstraint, HardwareError, PermissionDenied, OtherFailure };
    void deny(UserMediaAccessDenialReason);

    void invalidate();

    bool requiresAudioCapture() const { return m_eligibleAudioDevices.size(); }
    bool requiresVideoCapture() const { return !requiresDisplayCapture() && m_eligibleVideoDevices.size(); }
    bool requiresDisplayCapture() const { return m_request.type == CyberCore::MediaStreamRequest::Type::DisplayMedia && m_eligibleVideoDevices.size(); }

    Vector<String> videoDeviceUIDs() const;
    Vector<String> audioDeviceUIDs() const;

    uint64_t mainFrameID() const { return m_mainFrameID; }
    uint64_t frameID() const { return m_frameID; }
    CyberCore::SecurityOrigin& topLevelDocumentSecurityOrigin() { return m_topLevelDocumentSecurityOrigin.get(); }
    CyberCore::SecurityOrigin& userMediaDocumentSecurityOrigin() { return m_userMediaDocumentSecurityOrigin.get(); }

    CyberCore::MediaStreamRequest::Type requestType() const { return m_request.type; }

private:
    UserMediaPermissionRequestProxy(UserMediaPermissionRequestManagerProxy&, uint64_t userMediaID, uint64_t mainFrameID, uint64_t frameID, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin, Vector<CyberCore::CaptureDevice>&& audioDevices, Vector<CyberCore::CaptureDevice>&& videoDevices, CyberCore::MediaStreamRequest&&);

    UserMediaPermissionRequestManagerProxy* m_manager;
    uint64_t m_userMediaID;
    uint64_t m_mainFrameID;
    uint64_t m_frameID;
    Ref<CyberCore::SecurityOrigin> m_userMediaDocumentSecurityOrigin;
    Ref<CyberCore::SecurityOrigin> m_topLevelDocumentSecurityOrigin;
    Vector<CyberCore::CaptureDevice> m_eligibleVideoDevices;
    Vector<CyberCore::CaptureDevice> m_eligibleAudioDevices;
    CyberCore::MediaStreamRequest m_request;
};

} // namespace CyberKit
