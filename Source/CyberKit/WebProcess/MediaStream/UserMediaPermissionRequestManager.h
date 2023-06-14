/*
 * Copyright (C) 2014 Igalia S.L.
 * Copyright (C) 2016-2018 Apple Inc. All rights reserved.
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

#if ENABLE(MEDIA_STREAM)

#include "MediaDeviceSandboxExtensions.h"
#include "SandboxExtension.h"
#include <CyberCore/MediaCanStartListener.h>
#include <CyberCore/MediaConstraints.h>
#include <CyberCore/MediaDevicesEnumerationRequest.h>
#include <CyberCore/UserMediaClient.h>
#include <CyberCore/UserMediaRequest.h>
#include <wtf/HashMap.h>
#include <wtf/Ref.h>
#include <wtf/RefPtr.h>

namespace CyberKit {

class WebPage;

class UserMediaPermissionRequestManager : public CanMakeWeakPtr<UserMediaPermissionRequestManager>, private CyberCore::MediaCanStartListener {
public:
    explicit UserMediaPermissionRequestManager(WebPage&);
    ~UserMediaPermissionRequestManager();

    void startUserMediaRequest(CyberCore::UserMediaRequest&);
    void cancelUserMediaRequest(CyberCore::UserMediaRequest&);
    void userMediaAccessWasGranted(uint64_t, CyberCore::CaptureDevice&& audioDevice, CyberCore::CaptureDevice&& videoDevice, String&& deviceIdentifierHashSalt);
    void userMediaAccessWasDenied(uint64_t, CyberCore::UserMediaRequest::MediaAccessDenialReason, String&&);

    void enumerateMediaDevices(CyberCore::MediaDevicesEnumerationRequest&);
    void cancelMediaDevicesEnumeration(CyberCore::MediaDevicesEnumerationRequest&);
    void didCompleteMediaDeviceEnumeration(uint64_t, const Vector<CyberCore::CaptureDevice>& deviceList, String&& deviceIdentifierHashSalt, bool originHasPersistentAccess);

    void grantUserMediaDeviceSandboxExtensions(MediaDeviceSandboxExtensions&&);
    void revokeUserMediaDeviceSandboxExtensions(const Vector<String>&);

    CyberCore::UserMediaClient::DeviceChangeObserverToken addDeviceChangeObserver(WTF::Function<void()>&&);
    void removeDeviceChangeObserver(CyberCore::UserMediaClient::DeviceChangeObserverToken);

    void captureDevicesChanged();
    void clear();

private:
    void sendUserMediaRequest(CyberCore::UserMediaRequest&);

    // CyberCore::MediaCanStartListener
    void mediaCanStart(CyberCore::Document&) final;

    void removeMediaRequestFromMaps(CyberCore::UserMediaRequest&);

    WebPage& m_page;

    HashMap<uint64_t, RefPtr<CyberCore::UserMediaRequest>> m_idToUserMediaRequestMap;
    HashMap<RefPtr<CyberCore::UserMediaRequest>, uint64_t> m_userMediaRequestToIDMap;
    HashMap<RefPtr<CyberCore::Document>, Vector<RefPtr<CyberCore::UserMediaRequest>>> m_blockedUserMediaRequests;

    HashMap<uint64_t, RefPtr<CyberCore::MediaDevicesEnumerationRequest>> m_idToMediaDevicesEnumerationRequestMap;
    HashMap<RefPtr<CyberCore::MediaDevicesEnumerationRequest>, uint64_t> m_mediaDevicesEnumerationRequestToIDMap;

    HashMap<String, RefPtr<SandboxExtension>> m_userMediaDeviceSandboxExtensions;

    HashMap<CyberCore::UserMediaClient::DeviceChangeObserverToken, WTF::Function<void()>> m_deviceChangeObserverMap;
    bool m_monitoringDeviceChange { false };
};

} // namespace CyberKit

namespace WTF {

} // namespace WTF

#endif // ENABLE(MEDIA_STREAM)
