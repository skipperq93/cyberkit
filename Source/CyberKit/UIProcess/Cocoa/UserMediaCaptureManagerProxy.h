/*
 * Copyright (C) 2017-2020 Apple Inc. All rights reserved.
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
#include "UserMediaCaptureManager.h"
#include <CyberCore/CaptureDevice.h>
#include <CyberCore/OrientationNotifier.h>
#include <CyberCore/RealtimeMediaSource.h>
#include <CyberCore/RealtimeMediaSourceIdentifier.h>
#include <wtf/UniqueRef.h>

namespace CyberCore {
class PlatformMediaSessionManager;
}

namespace CyberKit {

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
    };
    explicit UserMediaCaptureManagerProxy(UniqueRef<ConnectionProxy>&&);
    ~UserMediaCaptureManagerProxy();

    void clear();

    void setOrientation(uint64_t);

    void didReceiveMessageFromGPUProcess(IPC::Connection& connection, IPC::Decoder& decoder) { didReceiveMessage(connection, decoder); }

private:
    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

    void createMediaSourceForCaptureDeviceWithConstraints(CyberCore::RealtimeMediaSourceIdentifier, const CyberCore::CaptureDevice& deviceID, String&&, const CyberCore::MediaConstraints&, CompletionHandler<void(bool succeeded, String invalidConstraints, CyberCore::RealtimeMediaSourceSettings&&, CyberCore::RealtimeMediaSourceCapabilities&&)>&&);
    void startProducingData(CyberCore::RealtimeMediaSourceIdentifier);
    void stopProducingData(CyberCore::RealtimeMediaSourceIdentifier);
    void end(CyberCore::RealtimeMediaSourceIdentifier);
    void capabilities(CyberCore::RealtimeMediaSourceIdentifier, CompletionHandler<void(CyberCore::RealtimeMediaSourceCapabilities&&)>&&);
    void applyConstraints(CyberCore::RealtimeMediaSourceIdentifier, const CyberCore::MediaConstraints&);
    void clone(CyberCore::RealtimeMediaSourceIdentifier clonedID, CyberCore::RealtimeMediaSourceIdentifier cloneID);
    void requestToEnd(CyberCore::RealtimeMediaSourceIdentifier);
    void setShouldApplyRotation(CyberCore::RealtimeMediaSourceIdentifier, bool shouldApplyRotation);

    class SourceProxy;
    friend class SourceProxy;
    HashMap<CyberCore::RealtimeMediaSourceIdentifier, std::unique_ptr<SourceProxy>> m_proxies;
    UniqueRef<ConnectionProxy> m_connectionProxy;
    CyberCore::OrientationNotifier m_orientationNotifier { 0 };
};

}

#endif
