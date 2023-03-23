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

#include "UserMediaPermissionCheckProxy.h"
#include "UserMediaPermissionRequestProxy.h"
#include <CyberCore/MediaProducer.h>
#include <CyberCore/PermissionDescriptor.h>
#include <CyberCore/PermissionState.h>
#include <CyberCore/RealtimeMediaSourceCenter.h>
#include <CyberCore/RealtimeMediaSourceFactory.h>
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
struct ClientOrigin;
struct MediaConstraints;
struct MediaStreamRequest;
class SecurityOrigin;
};

namespace WebKit {

class WebPageProxy;

enum MediaDevicePermissionRequestIdentifierType { };
using MediaDevicePermissionRequestIdentifier = ObjectIdentifier<MediaDevicePermissionRequestIdentifierType>;

class UserMediaPermissionRequestManagerProxy
#if ENABLE(MEDIA_STREAM)
    : public CyberCore::AudioCaptureFactory::ExtensiveObserver
#else
    : public CanMakeWeakPtr<UserMediaPermissionRequestManagerProxy>
#endif
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
    static bool permittedToCaptureAudio();
    static bool permittedToCaptureVideo();

    void invalidatePendingRequests();

    void requestUserMediaPermissionForFrame(CyberCore::UserMediaRequestIdentifier, CyberCore::FrameIdentifier, Ref<CyberCore::SecurityOrigin>&&  userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin, CyberCore::MediaStreamRequest&&);

    void resetAccess(std::optional<CyberCore::FrameIdentifier> mainFrameID = { });
    void didCommitLoadForFrame(CyberCore::FrameIdentifier);
    void viewIsBecomingVisible();

    void grantRequest(UserMediaPermissionRequestProxy&);
    void denyRequest(UserMediaPermissionRequestProxy&, UserMediaPermissionRequestProxy::UserMediaAccessDenialReason, const String& invalidConstraint = { });

    void enumerateMediaDevicesForFrame(CyberCore::FrameIdentifier, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin, CompletionHandler<void(const Vector<CyberCore::CaptureDevice>&, CyberCore::MediaDeviceHashSalts&&)>&&);

    void stopCapture();
    void scheduleNextRejection();
    void rejectionTimerFired();
    void clearCachedState();
    void captureDevicesChanged();

    void captureStateChanged(CyberCore::MediaProducerMediaStateFlags oldState, CyberCore::MediaProducerMediaStateFlags newState);
    void syncWithCyberCorePrefs() const;

    enum class RequestAction {
        Deny,
        Grant,
        Prompt
    };

    bool canAudioCaptureSucceed() const;
    bool canVideoCaptureSucceed() const;
    void setMockCaptureDevicesEnabledOverride(std::optional<bool>);
    bool hasPendingCapture() const { return m_hasPendingCapture; }

    void checkUserMediaPermissionForSpeechRecognition(CyberCore::FrameIdentifier, const CyberCore::SecurityOrigin&, const CyberCore::SecurityOrigin&, const CyberCore::CaptureDevice&, CompletionHandler<void(bool)>&&);

    struct DeniedRequest {
        CyberCore::FrameIdentifier mainFrameID;
        Ref<CyberCore::SecurityOrigin> userMediaDocumentOrigin;
        Ref<CyberCore::SecurityOrigin> topLevelDocumentOrigin;
        bool isAudioDenied;
        bool isVideoDenied;
        bool isScreenCaptureDenied;
    };

    std::optional<CyberCore::PermissionState> filterPermissionQuery(const CyberCore::ClientOrigin&, const CyberCore::PermissionDescriptor&, CyberCore::PermissionState);

    bool shouldChangeDeniedToPromptForCamera(const CyberCore::ClientOrigin&) const;
    bool shouldChangeDeniedToPromptForMicrophone(const CyberCore::ClientOrigin&) const;
    bool shouldChangePromptToGrantForCamera(const CyberCore::ClientOrigin&) const;
    bool shouldChangePromptToGrantForMicrophone(const CyberCore::ClientOrigin&) const;

private:
#if !RELEASE_LOG_DISABLED
    const Logger& logger() const final;
    const void* logIdentifier() const final { return m_logIdentifier; }
    const char* logClassName() const override { return "UserMediaPermissionRequestManagerProxy"; }
    WTFLogChannel& logChannel() const final;
#endif

    Ref<UserMediaPermissionRequestProxy> createPermissionRequest(CyberCore::UserMediaRequestIdentifier, CyberCore::FrameIdentifier mainFrameID, CyberCore::FrameIdentifier, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin, Vector<CyberCore::CaptureDevice>&& audioDevices, Vector<CyberCore::CaptureDevice>&& videoDevices, CyberCore::MediaStreamRequest&&);
#if ENABLE(MEDIA_STREAM)
    void finishGrantingRequest(UserMediaPermissionRequestProxy&);

    const UserMediaPermissionRequestProxy* searchForGrantedRequest(CyberCore::FrameIdentifier, const CyberCore::SecurityOrigin& userMediaDocumentOrigin, const CyberCore::SecurityOrigin& topLevelDocumentOrigin, bool needsAudio, bool needsVideo) const;
    bool wasRequestDenied(const UserMediaPermissionRequestProxy&, bool needsAudio, bool needsVideo, bool needsScreenCapture);

    using PermissionInfo = UserMediaPermissionCheckProxy::PermissionInfo;
    void getUserMediaPermissionInfo(CyberCore::FrameIdentifier, Ref<CyberCore::SecurityOrigin>&& userMediaDocumentOrigin, Ref<CyberCore::SecurityOrigin>&& topLevelDocumentOrigin, CompletionHandler<void(PermissionInfo)>&&);
    void captureDevicesChanged(PermissionInfo);

    RequestAction getRequestAction(const UserMediaPermissionRequestProxy&);

    bool wasGrantedVideoOrAudioAccess(CyberCore::FrameIdentifier);

    void computeFilteredDeviceList(bool revealIdsAndLabels, CompletionHandler<void(Vector<CyberCore::CaptureDevice>&&)>&&);
    void platformGetMediaStreamDevices(CompletionHandler<void(Vector<CyberCore::CaptureDevice>&&)>&&);

    void processUserMediaPermissionRequest();
    void processUserMediaPermissionInvalidRequest(const String& invalidConstraint);
    void processUserMediaPermissionValidRequest(Vector<CyberCore::CaptureDevice>&& audioDevices, Vector<CyberCore::CaptureDevice>&& videoDevices, CyberCore::MediaDeviceHashSalts&&);
    void startProcessingUserMediaPermissionRequest(Ref<UserMediaPermissionRequestProxy>&&);

    static void requestSystemValidation(const WebPageProxy&, UserMediaPermissionRequestProxy&, CompletionHandler<void(bool)>&&);

    void platformValidateUserMediaRequestConstraints(CyberCore::RealtimeMediaSourceCenter::ValidConstraintsHandler&& validHandler, CyberCore::RealtimeMediaSourceCenter::InvalidConstraintsHandler&& invalidHandler, CyberCore::MediaDeviceHashSalts&&);
#endif

    bool mockCaptureDevicesEnabled() const;

    void watchdogTimerFired();

    void processNextUserMediaRequestIfNeeded();
    void decidePolicyForUserMediaPermissionRequest();
    void updateStoredRequests(UserMediaPermissionRequestProxy&);

    String ephemeralDeviceHashSaltForFrame(CyberCore::FrameIdentifier);

    RefPtr<UserMediaPermissionRequestProxy> m_currentUserMediaRequest;
    Deque<Ref<UserMediaPermissionRequestProxy>> m_pendingUserMediaRequests;
    HashSet<MediaDevicePermissionRequestIdentifier> m_pendingDeviceRequests;

    WebPageProxy& m_page;

    RunLoop::Timer m_rejectionTimer;
    Deque<Ref<UserMediaPermissionRequestProxy>> m_pendingRejections;

    Vector<Ref<UserMediaPermissionRequestProxy>> m_pregrantedRequests;
    Vector<Ref<UserMediaPermissionRequestProxy>> m_grantedRequests;
    HashMap<CyberCore::FrameIdentifier, String> m_frameEphemeralHashSalts;

    Vector<DeniedRequest> m_deniedRequests;

    CyberCore::MediaProducerMediaStateFlags m_captureState;
    RunLoop::Timer m_watchdogTimer;
    Seconds m_currentWatchdogInterval;
#if !RELEASE_LOG_DISABLED
    Ref<const Logger> m_logger;
    const void* m_logIdentifier;
#endif
    bool m_hasFilteredDeviceList { false };
#if PLATFORM(COCOA)
    bool m_hasCreatedSandboxExtensionForTCCD { false };
#endif
    uint64_t m_hasPendingCapture { 0 };
    std::optional<bool> m_mockDevicesEnabledOverride;
    HashSet<CyberCore::FrameIdentifier> m_grantedFrames;
};

String convertEnumerationToString(UserMediaPermissionRequestManagerProxy::RequestAction);

} // namespace WebKit

#if ENABLE(MEDIA_STREAM)
namespace WTF {

template<typename Type>
struct LogArgument;

template <>
struct LogArgument<WebKit::UserMediaPermissionRequestManagerProxy::RequestAction> {
    static String toString(const WebKit::UserMediaPermissionRequestManagerProxy::RequestAction type)
    {
        return convertEnumerationToString(type);
    }
};

}; // namespace WTF
#endif
