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
#include <wtf/CompletionHandler.h>
#include <wtf/Deque.h>
#include <wtf/HashMap.h>
#include <wtf/LoggerHelper.h>
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

class UserMediaPermissionRequestManagerProxy
    : public CanMakeWeakPtr<UserMediaPermissionRequestManagerProxy>
#if !RELEASE_LOG_DISABLED
    , private LoggerHelper
#endif
{
    WTF_MAKE_FAST_ALLOCATED;
public:
    explicit UserMediaPermissionRequestManagerProxy(WebPageProxy&);
    ~UserMediaPermissionRequestManagerProxy();

    WebPageProxy& page() const { return m_page; }

#if ENABLE(MEDIA_STREAM)
    static void forEach(const WTF::Function<void(UserMediaPermissionRequestManagerProxy&)>&);
#endif

    void invalidatePendingRequests();

    void requestUserMediaPermissionForFrame(uint64_t userMediaID, CyberCore::FrameIdentifier, Ref<CyberCore::SecurityOrigin>&&  userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin, CyberCore::MediaStreamRequest&&);

    void resetAccess(Optional<CyberCore::FrameIdentifier> mainFrameID = { });
    void viewIsBecomingVisible();

    void grantRequest(UserMediaPermissionRequestProxy&);
    void denyRequest(UserMediaPermissionRequestProxy&, UserMediaPermissionRequestProxy::UserMediaAccessDenialReason, const String& invalidConstraint = { });

    void enumerateMediaDevicesForFrame(CyberCore::FrameIdentifier, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin, CompletionHandler<void(const Vector<CyberCore::CaptureDevice>&, const String&)>&&);

    void stopCapture();
    void scheduleNextRejection();
    void rejectionTimerFired();
    void clearCachedState();
    void captureDevicesChanged();

    void captureStateChanged(CyberCore::MediaProducer::MediaStateFlags oldState, CyberCore::MediaProducer::MediaStateFlags newState);
    void syncWithCyberCorePrefs() const;

    enum class RequestAction {
        Deny,
        Grant,
        Prompt
    };

    void setMockCaptureDevicesEnabledOverride(Optional<bool>);
    bool hasPendingCapture() const { return m_hasPendingCapture; }

private:
#if !RELEASE_LOG_DISABLED
    const Logger& logger() const final;
    const void* logIdentifier() const final { return m_logIdentifier; }
    const char* logClassName() const override { return "UserMediaPermissionRequestManagerProxy"; }
    WTFLogChannel& logChannel() const final;
#endif

    Ref<UserMediaPermissionRequestProxy> createPermissionRequest(uint64_t userMediaID, CyberCore::FrameIdentifier mainFrameID, CyberCore::FrameIdentifier, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin, Vector<CyberCore::CaptureDevice>&& audioDevices, Vector<CyberCore::CaptureDevice>&& videoDevices, CyberCore::MediaStreamRequest&&);
#if ENABLE(MEDIA_STREAM)
    void finishGrantingRequest(UserMediaPermissionRequestProxy&);

    const UserMediaPermissionRequestProxy* searchForGrantedRequest(CyberCore::FrameIdentifier, const CyberCore::SecurityOrigin& userMediaDocumentOrigin, const CyberCore::SecurityOrigin& topLevelDocumentOrigin, bool needsAudio, bool needsVideo) const;
    bool wasRequestDenied(CyberCore::FrameIdentifier mainFrameID, const CyberCore::SecurityOrigin& userMediaDocumentOrigin, const CyberCore::SecurityOrigin& topLevelDocumentOrigin, bool needsAudio, bool needsVideo, bool needsScreenCapture);

    using PermissionInfo = UserMediaPermissionCheckProxy::PermissionInfo;
    void getUserMediaPermissionInfo(CyberCore::FrameIdentifier, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin, CompletionHandler<void(PermissionInfo)>&&);
    void captureDevicesChanged(PermissionInfo);

    RequestAction getRequestAction(const UserMediaPermissionRequestProxy&);

    bool wasGrantedVideoOrAudioAccess(CyberCore::FrameIdentifier, const CyberCore::SecurityOrigin& userMediaDocumentOrigin, const CyberCore::SecurityOrigin& topLevelDocumentOrigin);

    Vector<CyberCore::CaptureDevice> computeFilteredDeviceList(bool revealIdsAndLabels, const String& deviceIDHashSalt);

    void processUserMediaPermissionRequest();
    void processUserMediaPermissionInvalidRequest(const String& invalidConstraint);
    void processUserMediaPermissionValidRequest(Vector<CyberCore::CaptureDevice>&& audioDevices, Vector<CyberCore::CaptureDevice>&& videoDevices, String&& deviceIdentifierHashSalt);
    void startProcessingUserMediaPermissionRequest(Ref<UserMediaPermissionRequestProxy>&&);
#endif

    void watchdogTimerFired();

    void processNextUserMediaRequestIfNeeded();

    RefPtr<UserMediaPermissionRequestProxy> m_currentUserMediaRequest;
    Deque<Ref<UserMediaPermissionRequestProxy>> m_pendingUserMediaRequests;
    HashSet<uint64_t> m_pendingDeviceRequests;

    WebPageProxy& m_page;

    RunLoop::Timer<UserMediaPermissionRequestManagerProxy> m_rejectionTimer;
    Deque<Ref<UserMediaPermissionRequestProxy>> m_pendingRejections;

    Vector<Ref<UserMediaPermissionRequestProxy>> m_pregrantedRequests;
    Vector<Ref<UserMediaPermissionRequestProxy>> m_grantedRequests;

    struct DeniedRequest {
        CyberCore::FrameIdentifier mainFrameID;
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
#if !RELEASE_LOG_DISABLED
    Ref<const Logger> m_logger;
    const void* m_logIdentifier;
#endif
    bool m_hasFilteredDeviceList { false };
#if PLATFORM(IOS)
    bool m_hasCreatedSandboxExtensionForTCCD { false };
#endif
    uint64_t m_hasPendingCapture { 0 };
    Optional<bool> m_mockDevicesEnabledOverride;
};

String convertEnumerationToString(UserMediaPermissionRequestManagerProxy::RequestAction);

} // namespace CyberKit

#if ENABLE(MEDIA_STREAM)
namespace WTF {

template<typename Type>
struct LogArgument;

template <>
struct LogArgument<CyberKit::UserMediaPermissionRequestManagerProxy::RequestAction> {
    static String toString(const CyberKit::UserMediaPermissionRequestManagerProxy::RequestAction type)
    {
        return convertEnumerationToString(type);
    }
};

}; // namespace WTF
#endif
