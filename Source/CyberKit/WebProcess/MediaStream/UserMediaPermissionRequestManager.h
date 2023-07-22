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

#include "SandboxExtension.h"
#include <CyberCore/MediaCanStartListener.h>
#include <CyberCore/MediaConstraints.h>
#include <CyberCore/UserMediaClient.h>
#include <CyberCore/UserMediaRequest.h>
#include <wtf/HashMap.h>
#include <wtf/Ref.h>
#include <wtf/RefPtr.h>

namespace CyberKit {

class WebPage;

class UserMediaPermissionRequestManager : public CanMakeWeakPtr<UserMediaPermissionRequestManager>, private CyberCore::MediaCanStartListener {
    WTF_MAKE_FAST_ALLOCATED;
public:
    explicit UserMediaPermissionRequestManager(WebPage&);
    ~UserMediaPermissionRequestManager() = default;

    void startUserMediaRequest(CyberCore::UserMediaRequest&);
    void cancelUserMediaRequest(CyberCore::UserMediaRequest&);
    void userMediaAccessWasGranted(uint64_t, CyberCore::CaptureDevice&& audioDevice, CyberCore::CaptureDevice&& videoDevice, String&& deviceIdentifierHashSalt, CompletionHandler<void()>&&);
    void userMediaAccessWasDenied(uint64_t, CyberCore::UserMediaRequest::MediaAccessDenialReason, String&&);

    void enumerateMediaDevices(CyberCore::Document&, CompletionHandler<void(const Vector<CyberCore::CaptureDevice>&, const String&)>&&);

    CyberCore::UserMediaClient::DeviceChangeObserverToken addDeviceChangeObserver(WTF::Function<void()>&&);
    void removeDeviceChangeObserver(CyberCore::UserMediaClient::DeviceChangeObserverToken);

    void captureDevicesChanged();

private:
    void sendUserMediaRequest(CyberCore::UserMediaRequest&);

    // CyberCore::MediaCanStartListener
    void mediaCanStart(CyberCore::Document&) final;

    WebPage& m_page;

    HashMap<CyberCore::UserMediaRequestIdentifier, Ref<CyberCore::UserMediaRequest>> m_ongoingUserMediaRequests;
    HashMap<RefPtr<CyberCore::Document>, Vector<Ref<CyberCore::UserMediaRequest>>> m_pendingUserMediaRequests;

    HashMap<CyberCore::UserMediaClient::DeviceChangeObserverToken, Function<void()>> m_deviceChangeObserverMap;
    bool m_monitoringDeviceChange { false };
};

} // namespace CyberKit

namespace WTF {

} // namespace WTF

#endif // ENABLE(MEDIA_STREAM)
