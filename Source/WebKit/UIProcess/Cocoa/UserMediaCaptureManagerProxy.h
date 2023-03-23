/*
 * Copyright (C) 2017-2022 Apple Inc. All rights reserved.
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

#include "Connection.h"
#include "MessageReceiver.h"
#include "RemoteVideoFrameObjectHeap.h"
#include "UserMediaCaptureManager.h"
#include <CyberCore/CaptureDevice.h>
#include <CyberCore/OrientationNotifier.h>
#include <CyberCore/ProcessIdentity.h>
#include <CyberCore/RealtimeMediaSource.h>
#include <CyberCore/RealtimeMediaSourceIdentifier.h>
#include <pal/spi/cocoa/TCCSPI.h>
#include <wtf/UniqueRef.h>

namespace CyberCore {
class PlatformMediaSessionManager;
struct VideoPresetData;
}

namespace WebKit {

class SharedMemory;
class WebProcessProxy;

class UserMediaCaptureManagerProxy : private IPC::MessageReceiver {
    WTF_MAKE_FAST_ALLOCATED;
public:
    class ConnectionProxy {
    public:
        virtual ~ConnectionProxy() = default;
        virtual void addMessageReceiver(IPC::ReceiverName, IPC::MessageReceiver&) = 0;
        virtual void removeMessageReceiver(IPC::ReceiverName) = 0;
        virtual IPC::Connection& connection() = 0;
        virtual bool willStartCapture(CyberCore::CaptureDevice::DeviceType) const = 0;
        virtual Logger& logger() = 0;
        virtual bool setCaptureAttributionString() { return true; }
        virtual const CyberCore::ProcessIdentity& resourceOwner() const = 0;
#if ENABLE(APP_PRIVACY_REPORT)
        virtual void setTCCIdentity() { }
#endif
        virtual void startProducingData(CyberCore::RealtimeMediaSource::Type) { }
        virtual RemoteVideoFrameObjectHeap* remoteVideoFrameObjectHeap() { return nullptr; }
    };
    explicit UserMediaCaptureManagerProxy(UniqueRef<ConnectionProxy>&&);
    ~UserMediaCaptureManagerProxy();

    void clear();

    void setOrientation(uint64_t);

    void didReceiveMessageFromGPUProcess(IPC::Connection& connection, IPC::Decoder& decoder) { didReceiveMessage(connection, decoder); }

    bool hasSourceProxies() const;

private:
    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

    using CreateSourceCallback = CompletionHandler<void(bool succeeded, String invalidConstraints, CyberCore::RealtimeMediaSourceSettings&&, CyberCore::RealtimeMediaSourceCapabilities&&, Vector<CyberCore::VideoPresetData>&&, CyberCore::IntSize, double)>;
    void createMediaSourceForCaptureDeviceWithConstraints(CyberCore::RealtimeMediaSourceIdentifier, const CyberCore::CaptureDevice& deviceID, CyberCore::MediaDeviceHashSalts&&, const CyberCore::MediaConstraints&, bool shouldUseGPUProcessRemoteFrames, CyberCore::PageIdentifier, CreateSourceCallback&&);
    void startProducingData(CyberCore::RealtimeMediaSourceIdentifier);
    void stopProducingData(CyberCore::RealtimeMediaSourceIdentifier);
    void removeSource(CyberCore::RealtimeMediaSourceIdentifier);
    void capabilities(CyberCore::RealtimeMediaSourceIdentifier, CompletionHandler<void(CyberCore::RealtimeMediaSourceCapabilities&&)>&&);
    void applyConstraints(CyberCore::RealtimeMediaSourceIdentifier, const CyberCore::MediaConstraints&);
    void clone(CyberCore::RealtimeMediaSourceIdentifier clonedID, CyberCore::RealtimeMediaSourceIdentifier cloneID, CyberCore::PageIdentifier);
    void endProducingData(CyberCore::RealtimeMediaSourceIdentifier);
    void setShouldApplyRotation(CyberCore::RealtimeMediaSourceIdentifier, bool shouldApplyRotation);
    void setIsInBackground(CyberCore::RealtimeMediaSourceIdentifier, bool);

    CyberCore::CaptureSourceOrError createMicrophoneSource(const CyberCore::CaptureDevice&, CyberCore::MediaDeviceHashSalts&&, const CyberCore::MediaConstraints*, CyberCore::PageIdentifier);
    CyberCore::CaptureSourceOrError createCameraSource(const CyberCore::CaptureDevice&, CyberCore::MediaDeviceHashSalts&&, const CyberCore::MediaConstraints*, CyberCore::PageIdentifier);

    class SourceProxy;
    friend class SourceProxy;
    HashMap<CyberCore::RealtimeMediaSourceIdentifier, std::unique_ptr<SourceProxy>> m_proxies;
    UniqueRef<ConnectionProxy> m_connectionProxy;
    CyberCore::OrientationNotifier m_orientationNotifier { 0 };

    struct PageSources {
        WeakPtr<CyberCore::RealtimeMediaSource> microphoneSource;
        WeakHashSet<CyberCore::RealtimeMediaSource> cameraSources;
    };
    HashMap<CyberCore::PageIdentifier, PageSources> m_pageSources;
};

}

#endif
