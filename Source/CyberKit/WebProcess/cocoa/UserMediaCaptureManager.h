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
#include <CyberCore/DisplayCaptureManager.h>
#include <CyberCore/RealtimeMediaSource.h>
#include <CyberCore/RealtimeMediaSourceFactory.h>
#include <CyberCore/RealtimeMediaSourceIdentifier.h>
#include <wtf/HashMap.h>

namespace CyberCore {
class CAAudioStreamDescription;
}

namespace CyberKit {

class RemoteRealtimeAudioSource;
class RemoteRealtimeVideoSource;
class WebProcess;

class UserMediaCaptureManager : public WebProcessSupplement, public IPC::MessageReceiver {
    WTF_MAKE_FAST_ALLOCATED;
public:
    explicit UserMediaCaptureManager(WebProcess&);
    ~UserMediaCaptureManager();

    static const char* supplementName();

    void didReceiveMessageFromGPUProcess(IPC::Connection& connection, IPC::Decoder& decoder) { didReceiveMessage(connection, decoder); }
    void setupCaptureProcesses(bool shouldCaptureAudioInUIProcess, bool shouldCaptureAudioInGPUProcess, bool shouldCaptureVideoInUIProcess, bool shouldCaptureVideoInGPUProcess, bool shouldCaptureDisplayInUIProcess, bool shouldCaptureDisplayInGPUProcess, bool shouldUseGPUProcessRemoteFrames);

    void addSource(Ref<RemoteRealtimeAudioSource>&&);
    void addSource(Ref<RemoteRealtimeVideoSource>&&);
    void removeSource(CyberCore::RealtimeMediaSourceIdentifier);

    RemoteCaptureSampleManager& remoteCaptureSampleManager() { return m_remoteCaptureSampleManager; }
    bool shouldUseGPUProcessRemoteFrames() const { return m_shouldUseGPUProcessRemoteFrames; }

private:
    // CyberCore::RealtimeMediaSource factories
    class AudioFactory : public CyberCore::AudioCaptureFactory {
    public:
        explicit AudioFactory(UserMediaCaptureManager& manager) : m_manager(manager) { }
        void setShouldCaptureInGPUProcess(bool);

    private:
        CyberCore::CaptureSourceOrError createAudioCaptureSource(const CyberCore::CaptureDevice&, CyberCore::MediaDeviceHashSalts&&, const CyberCore::MediaConstraints*, CyberCore::PageIdentifier) final;
        CyberCore::CaptureDeviceManager& audioCaptureDeviceManager() final { return m_manager.m_noOpCaptureDeviceManager; }
        const Vector<CyberCore::CaptureDevice>& speakerDevices() const final { return m_speakerDevices; }

        UserMediaCaptureManager& m_manager;
        bool m_shouldCaptureInGPUProcess { false };
        Vector<CyberCore::CaptureDevice> m_speakerDevices;
    };
    class VideoFactory : public CyberCore::VideoCaptureFactory {
    public:
        explicit VideoFactory(UserMediaCaptureManager& manager) : m_manager(manager) { }
        void setShouldCaptureInGPUProcess(bool);

    private:
        CyberCore::CaptureSourceOrError createVideoCaptureSource(const CyberCore::CaptureDevice&, CyberCore::MediaDeviceHashSalts&&, const CyberCore::MediaConstraints*, CyberCore::PageIdentifier) final;
        CyberCore::CaptureDeviceManager& videoCaptureDeviceManager() final { return m_manager.m_noOpCaptureDeviceManager; }

        UserMediaCaptureManager& m_manager;
        bool m_shouldCaptureInGPUProcess { false };
    };
    class DisplayFactory : public CyberCore::DisplayCaptureFactory {
    public:
        explicit DisplayFactory(UserMediaCaptureManager& manager) : m_manager(manager) { }
        void setShouldCaptureInGPUProcess(bool);

    private:
        CyberCore::CaptureSourceOrError createDisplayCaptureSource(const CyberCore::CaptureDevice&, CyberCore::MediaDeviceHashSalts&&, const CyberCore::MediaConstraints*, CyberCore::PageIdentifier) final;
        CyberCore::DisplayCaptureManager& displayCaptureDeviceManager() final { return m_manager.m_noOpCaptureDeviceManager; }

        UserMediaCaptureManager& m_manager;
        bool m_shouldCaptureInGPUProcess { false };
    };

    class NoOpCaptureDeviceManager : public CyberCore::DisplayCaptureManager {
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
    void sourceStopped(CyberCore::RealtimeMediaSourceIdentifier, bool didFail);
    void sourceMutedChanged(CyberCore::RealtimeMediaSourceIdentifier, bool muted, bool interrupted);

    void sourceSettingsChanged(CyberCore::RealtimeMediaSourceIdentifier, CyberCore::RealtimeMediaSourceSettings&&);
    void sourceConfigurationChanged(CyberCore::RealtimeMediaSourceIdentifier, String&&, CyberCore::RealtimeMediaSourceSettings&&, CyberCore::RealtimeMediaSourceCapabilities&&);

    void applyConstraintsSucceeded(CyberCore::RealtimeMediaSourceIdentifier, CyberCore::RealtimeMediaSourceSettings&&);
    void applyConstraintsFailed(CyberCore::RealtimeMediaSourceIdentifier, String&&, String&&);

    using Source = std::variant<std::nullptr_t, Ref<RemoteRealtimeAudioSource>, Ref<RemoteRealtimeVideoSource>>;
    HashMap<CyberCore::RealtimeMediaSourceIdentifier, Source> m_sources;
    WebProcess& m_process;
    NoOpCaptureDeviceManager m_noOpCaptureDeviceManager;
    AudioFactory m_audioFactory;
    VideoFactory m_videoFactory;
    DisplayFactory m_displayFactory;
    RemoteCaptureSampleManager m_remoteCaptureSampleManager;
    bool m_shouldUseGPUProcessRemoteFrames { false };
};

} // namespace CyberKit

#endif
