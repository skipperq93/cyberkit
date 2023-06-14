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

#include "UserMediaPermissionCheckProxy.h"
#include "UserMediaPermissionRequestProxy.h"
#include <CyberCore/MediaProducer.h>
#include <CyberCore/SecurityOrigin.h>
#include <wtf/HashMap.h>
#include <wtf/RunLoop.h>
#include <wtf/Seconds.h>
#include <wtf/WeakPtr.h>

namespace CyberCore {
class CaptureDevice;
struct MediaConstraints;
struct MediaStreamRequest;
class SecurityOrigin;
};

namespace CyberKit {

class WebPageProxy;

class UserMediaPermissionRequestManagerProxy : public CanMakeWeakPtr<UserMediaPermissionRequestManagerProxy> {
public:
    explicit UserMediaPermissionRequestManagerProxy(WebPageProxy&);
    ~UserMediaPermissionRequestManagerProxy();

    WebPageProxy& page() const { return m_page; }

    void invalidatePendingRequests();

    void requestUserMediaPermissionForFrame(uint64_t userMediaID, uint64_t frameID, Ref<CyberCore::SecurityOrigin>&&  userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin, const CyberCore::MediaStreamRequest&);

    void resetAccess(uint64_t mainFrameID);
    void viewIsBecomingVisible();

    void userMediaAccessWasGranted(uint64_t, CyberCore::CaptureDevice&& audioDevice, CyberCore::CaptureDevice&& videoDevice);
    void userMediaAccessWasDenied(uint64_t, UserMediaPermissionRequestProxy::UserMediaAccessDenialReason);

    void enumerateMediaDevicesForFrame(uint64_t userMediaID, uint64_t frameID, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin);

    void stopCapture();
    void scheduleNextRejection();
    void rejectionTimerFired();
    void clearCachedState();
    void captureDevicesChanged();

    void captureStateChanged(CyberCore::MediaProducer::MediaStateFlags oldState, CyberCore::MediaProducer::MediaStateFlags newState);
    void syncWithCyberCorePrefs() const;

private:
    Ref<UserMediaPermissionRequestProxy> createPermissionRequest(uint64_t userMediaID, uint64_t mainFrameID, uint64_t frameID, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin, Vector<CyberCore::CaptureDevice>&& audioDevices, Vector<CyberCore::CaptureDevice>&& videoDevices, CyberCore::MediaStreamRequest&&);
    void denyRequest(uint64_t userMediaID, UserMediaPermissionRequestProxy::UserMediaAccessDenialReason, const String& invalidConstraint);
#if ENABLE(MEDIA_STREAM)
    bool grantAccess(uint64_t userMediaID, const CyberCore::CaptureDevice audioDevice, const CyberCore::CaptureDevice videoDevice, const String& deviceIdentifierHashSalt);

    const UserMediaPermissionRequestProxy* searchForGrantedRequest(uint64_t frameID, const CyberCore::SecurityOrigin& userMediaDocumentOrigin, const CyberCore::SecurityOrigin& topLevelDocumentOrigin, bool needsAudio, bool needsVideo) const;
    bool wasRequestDenied(uint64_t mainFrameID, const CyberCore::SecurityOrigin& userMediaDocumentOrigin, const CyberCore::SecurityOrigin& topLevelDocumentOrigin, bool needsAudio, bool needsVideo, bool needsScreenCapture);

    void getUserMediaPermissionInfo(uint64_t requestID, uint64_t frameID, UserMediaPermissionCheckProxy::CompletionHandler&&, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin);

    enum class RequestAction {
        Deny,
        Grant,
        Prompt
    };
    RequestAction getRequestAction(uint64_t frameID, CyberCore::SecurityOrigin& userMediaDocumentOrigin, CyberCore::SecurityOrigin& topLevelDocumentOrigin, const CyberCore::MediaStreamRequest&, Vector<CyberCore::CaptureDevice>& audioDevices, Vector<CyberCore::CaptureDevice>& videoDevices);

    bool wasGrantedVideoOrAudioAccess(uint64_t, const CyberCore::SecurityOrigin& userMediaDocumentOrigin, const CyberCore::SecurityOrigin& topLevelDocumentOrigin);
#endif

    void watchdogTimerFired();

    HashMap<uint64_t, RefPtr<UserMediaPermissionRequestProxy>> m_pendingUserMediaRequests;
    HashMap<uint64_t, Ref<UserMediaPermissionCheckProxy>> m_pendingDeviceRequests;

    WebPageProxy& m_page;

    RunLoop::Timer<UserMediaPermissionRequestManagerProxy> m_rejectionTimer;
    Vector<uint64_t> m_pendingRejections;

    Vector<Ref<UserMediaPermissionRequestProxy>> m_pregrantedRequests;
    Vector<Ref<UserMediaPermissionRequestProxy>> m_grantedRequests;

    struct DeniedRequest {
        uint64_t mainFrameID;
        Ref<CyberCore::SecurityOrigin> userMediaDocumentOrigin;
        Ref<CyberCore::SecurityOrigin> topLevelDocumentOrigin;
        bool isAudioDenied;
        bool isVideoDenied;
        bool isScreenCaptureDenied;
    };
    Vector<DeniedRequest> m_deniedRequests;

    CyberCore::MediaProducer::MediaStateFlags m_captureState { CyberCore::MediaProducer::IsNotPlaying };
    RunLoop::Timer<UserMediaPermissionRequestManagerProxy> m_watchdogTimer;
    Seconds m_currentWatchdogInterval;
    bool m_hasFilteredDeviceList { false };
};

} // namespace CyberKit

