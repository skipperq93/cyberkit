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

#include "RemoteCaptureSampleManager.h"
#include "SharedMemory.h"
#include "WebProcessSupplement.h"
#include <CyberCore/CaptureDeviceManager.h>
#include <CyberCore/RealtimeMediaSource.h>
#include <CyberCore/RealtimeMediaSourceFactory.h>
#include <CyberCore/RealtimeMediaSourceIdentifier.h>
#include <wtf/HashMap.h>

namespace CyberCore {
class CAAudioStreamDescription;
class RemoteVideoSample;
}

namespace CyberKit {

class RemoteRealtimeMediaSource;
class WebProcess;

class UserMediaCaptureManager : public WebProcessSupplement, public IPC::MessageReceiver {
    WTF_MAKE_FAST_ALLOCATED;
public:
    explicit UserMediaCaptureManager(WebProcess&);
    ~UserMediaCaptureManager();

    static const char* supplementName();

    void didReceiveMessageFromGPUProcess(IPC::Connection& connection, IPC::Decoder& decoder) { didReceiveMessage(connection, decoder); }
    void setupCaptureProcesses(bool shouldCaptureAudioInUIProcess, bool shouldCaptureAudioInGPUProcess, bool shouldCaptureVideoInUIProcess, bool shouldCaptureVideoInGPUProcess, bool shouldCaptureDisplayInUIProcess);

    void addSource(Ref<RemoteRealtimeMediaSource>&&);
    void removeSource(CyberCore::RealtimeMediaSourceIdentifier);

private:
    // CyberCore::RealtimeMediaSource factories
    class AudioFactory : public CyberCore::AudioCaptureFactory {
    public:
        explicit AudioFactory(UserMediaCaptureManager& manager) : m_manager(manager) { }
        void setShouldCaptureInGPUProcess(bool);

    private:
        CyberCore::CaptureSourceOrError createAudioCaptureSource(const CyberCore::CaptureDevice&, String&& hashSalt, const CyberCore::MediaConstraints*) final;
        CyberCore::CaptureDeviceManager& audioCaptureDeviceManager() final { return m_manager.m_noOpCaptureDeviceManager; }

        UserMediaCaptureManager& m_manager;
        bool m_shouldCaptureInGPUProcess { false };
    };
    class VideoFactory : public CyberCore::VideoCaptureFactory {
    public:
        explicit VideoFactory(UserMediaCaptureManager& manager) : m_manager(manager) { }
        void setShouldCaptureInGPUProcess(bool value) { m_shouldCaptureInGPUProcess = value; }

    private:
        CyberCore::CaptureSourceOrError createVideoCaptureSource(const CyberCore::CaptureDevice&, String&& hashSalt, const CyberCore::MediaConstraints*) final;
        CyberCore::CaptureDeviceManager& videoCaptureDeviceManager() final { return m_manager.m_noOpCaptureDeviceManager; }
#if PLATFORM(IOS_FAMILY)
        void setActiveSource(CyberCore::RealtimeMediaSource&) final;
#endif

        UserMediaCaptureManager& m_manager;
        bool m_shouldCaptureInGPUProcess { false };
    };
    class DisplayFactory : public CyberCore::DisplayCaptureFactory {
    public:
        explicit DisplayFactory(UserMediaCaptureManager& manager) : m_manager(manager) { }

    private:
        CyberCore::CaptureSourceOrError createDisplayCaptureSource(const CyberCore::CaptureDevice&, const CyberCore::MediaConstraints*) final;
        CyberCore::CaptureDeviceManager& displayCaptureDeviceManager() final { return m_manager.m_noOpCaptureDeviceManager; }

        UserMediaCaptureManager& m_manager;
    };

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
    void captureFailed(CyberCore::RealtimeMediaSourceIdentifier);
    void sourceStopped(CyberCore::RealtimeMediaSourceIdentifier);
    void sourceEnded(CyberCore::RealtimeMediaSourceIdentifier identifier) { removeSource(identifier); }
    void sourceMutedChanged(CyberCore::RealtimeMediaSourceIdentifier, bool muted);
    void sourceSettingsChanged(CyberCore::RealtimeMediaSourceIdentifier, const CyberCore::RealtimeMediaSourceSettings&);
    void remoteVideoSampleAvailable(CyberCore::RealtimeMediaSourceIdentifier, CyberCore::RemoteVideoSample&&);
    void applyConstraintsSucceeded(CyberCore::RealtimeMediaSourceIdentifier, const CyberCore::RealtimeMediaSourceSettings&);
    void applyConstraintsFailed(CyberCore::RealtimeMediaSourceIdentifier, String&&, String&&);

    Ref<CyberCore::RealtimeMediaSource> cloneVideoSource(RemoteRealtimeMediaSource&);

    HashMap<CyberCore::RealtimeMediaSourceIdentifier, Ref<RemoteRealtimeMediaSource>> m_sources;
    WebProcess& m_process;
    NoOpCaptureDeviceManager m_noOpCaptureDeviceManager;
    AudioFactory m_audioFactory;
    VideoFactory m_videoFactory;
    DisplayFactory m_displayFactory;
    RemoteCaptureSampleManager m_remoteCaptureSampleManager;
};

} // namespace CyberKit

#endif
