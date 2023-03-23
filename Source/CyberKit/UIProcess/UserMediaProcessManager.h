/*
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

#include "UserMediaPermissionRequestManagerProxy.h"
#include <CyberCore/CaptureDevice.h>
#include <CyberCore/PageIdentifier.h>
#include <CyberCore/RealtimeMediaSourceCenter.h>
#include <CyberCore/UserMediaClient.h>
#include <wtf/WeakPtr.h>

namespace CyberKit {

class UserMediaPermissionRequestProxy;
class WebProcessProxy;

class UserMediaProcessManager : public CyberCore::RealtimeMediaSourceCenter::Observer {
public:

    static UserMediaProcessManager& singleton();

    UserMediaProcessManager();

    bool willCreateMediaStream(UserMediaPermissionRequestManagerProxy&, const UserMediaPermissionRequestProxy&);

    void revokeSandboxExtensionsIfNeeded(WebProcessProxy&);

    void setCaptureEnabled(bool);
    bool captureEnabled() const { return m_captureEnabled; }

    void denyNextUserMediaRequest() { m_denyNextRequest = true; }

    void beginMonitoringCaptureDevices();

private:
    enum class ShouldNotify : bool { No, Yes };
    void updateCaptureDevices(ShouldNotify);
    void captureDevicesChanged();

    // RealtimeMediaSourceCenter::Observer
    void devicesChanged() final;

    Vector<CyberCore::CaptureDevice> m_captureDevices;
    bool m_captureEnabled { true };
    bool m_denyNextRequest { false };
};

} // namespace CyberKit

#endif
