/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if PLATFORM(COCOA) && ENABLE(MEDIA_STREAM)

#include "MessageReceiver.h"
#include "SharedMemory.h"
#include "WebProcessSupplement.h"
#include <CyberCore/CaptureDeviceManager.h>
#include <CyberCore/RealtimeMediaSource.h>
#include <CyberCore/RealtimeMediaSourceFactory.h>
#include <wtf/HashMap.h>

namespace CyberCore {
class CAAudioStreamDescription;
class RemoteVideoSample;
}

namespace CyberKit {

class CrossProcessRealtimeAudioSource;
class WebProcess;

class UserMediaCaptureManager : public WebProcessSupplement, public IPC::MessageReceiver {
    WTF_MAKE_FAST_ALLOCATED;
public:
    explicit UserMediaCaptureManager(WebProcess&);
    ~UserMediaCaptureManager();

    static const char* supplementName();

    void didReceiveMessageFromGPUProcess(IPC::Connection& connection, IPC::Decoder& decoder) { didReceiveMessage(connection, decoder); }

private:
    // WebProcessSupplement
    void initialize(const WebProcessCreationParameters&) final;

    // CyberCore::RealtimeMediaSource factories
    class AudioFactory : public CyberCore::AudioCaptureFactory {
    public:
        explicit AudioFactory(UserMediaCaptureManager& manager) : m_manager(manager) { }
        void setShouldCaptureInGPUProcess(bool value) { m_shouldCaptureInGPUProcess = value; }
    private:
        CyberCore::CaptureSourceOrError createAudioCaptureSource(const CyberCore::CaptureDevice&, String&& hashSalt, const CyberCore::MediaConstraints*) final;
        CyberCore::CaptureDeviceManager& audioCaptureDeviceManager() final { return m_manager.m_noOpCaptureDeviceManager; }
#if PLATFORM(IOS_FAMILY)
        void setAudioCapturePageState(bool interrupted, bool pageMuted) final;
#endif

        UserMediaCaptureManager& m_manager;
        bool m_shouldCaptureInGPUProcess { false };
    };
    class VideoFactory : public CyberCore::VideoCaptureFactory {
    public:
        explicit VideoFactory(UserMediaCaptureManager& manager) : m_manager(manager) { }

    private:
        CyberCore::CaptureSourceOrError createVideoCaptureSource(const CyberCore::CaptureDevice& device, String&& hashSalt, const CyberCore::MediaConstraints* constraints) final { return m_manager.createCaptureSource(device, WTFMove(hashSalt), constraints); }
        CyberCore::CaptureDeviceManager& videoCaptureDeviceManager() final { return m_manager.m_noOpCaptureDeviceManager; }
#if PLATFORM(IOS_FAMILY)
        void setVideoCapturePageState(bool interrupted, bool pageMuted) final;
#endif

        UserMediaCaptureManager& m_manager;
    };
    class DisplayFactory : public CyberCore::DisplayCaptureFactory {
    public:
        explicit DisplayFactory(UserMediaCaptureManager& manager) : m_manager(manager) { }

    private:
        CyberCore::CaptureSourceOrError createDisplayCaptureSource(const CyberCore::CaptureDevice& device, const CyberCore::MediaConstraints* constraints) final  { return m_manager.createCaptureSource(device, { }, constraints); }
        CyberCore::CaptureDeviceManager& displayCaptureDeviceManager() final { return m_manager.m_noOpCaptureDeviceManager; }

        UserMediaCaptureManager& m_manager;
    };

    CyberCore::CaptureSourceOrError createCaptureSource(const CyberCore::CaptureDevice&, String&&, const CyberCore::MediaConstraints*, bool shouldCaptureInGPUProcess = false);

    class NoOpCaptureDeviceManager : public CyberCore::CaptureDeviceManager {
    public:
        NoOpCaptureDeviceManager() = default;

    private:
        const Vector<CyberCore::CaptureDevice>& captureDevices() final
        {
            ASSERT_NOT_REACHED();
            return m_emptyDevices;
        }
        Vector<CyberCore::CaptureDevice> m_emptyDevices;
    };

    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

    // Messages::UserMediaCaptureManager
    void captureFailed(uint64_t id);
    void sourceStopped(uint64_t id);
    void sourceEnded(uint64_t id);
    void sourceMutedChanged(uint64_t id, bool muted);
    void sourceSettingsChanged(uint64_t id, const CyberCore::RealtimeMediaSourceSettings&);
    void storageChanged(uint64_t id, const SharedMemory::Handle&, const CyberCore::CAAudioStreamDescription&, uint64_t numberOfFrames);
    void ringBufferFrameBoundsChanged(uint64_t id, uint64_t startFrame, uint64_t endFrame);
    void audioSamplesAvailable(uint64_t id, MediaTime, uint64_t numberOfFrames, uint64_t startFrame, uint64_t endFrame);
    void remoteVideoSampleAvailable(uint64_t id, CyberCore::RemoteVideoSample&&);

    void startProducingData(uint64_t);
    void stopProducingData(uint64_t);
    CyberCore::RealtimeMediaSourceCapabilities capabilities(uint64_t);
    void applyConstraints(uint64_t, const CyberCore::MediaConstraints&);
    void applyConstraintsSucceeded(uint64_t, const CyberCore::RealtimeMediaSourceSettings&);
    void applyConstraintsFailed(uint64_t, String&&, String&&);

    class Source;
    friend class Source;

    void requestToEnd(uint64_t sourceID);
    Ref<CyberCore::RealtimeMediaSource> cloneSource(Source&);
    Ref<CyberCore::RealtimeMediaSource> cloneVideoSource(Source&);

    HashMap<uint64_t, RefPtr<Source>> m_sources;
    WebProcess& m_process;
    NoOpCaptureDeviceManager m_noOpCaptureDeviceManager;
    AudioFactory m_audioFactory;
    VideoFactory m_videoFactory;
    DisplayFactory m_displayFactory;
};

} // namespace CyberKit

#endif
