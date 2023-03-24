/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
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

#include "config.h"
#include "WebMediaStrategy.h"

#include "GPUConnectionToWebProcess.h"
#include "GPUProcessConnection.h"
#include "RemoteAudioDestinationProxy.h"
#include "RemoteCDMFactory.h"
#include "CyberCoreArgumentCoders.h"
#include "WebProcess.h"
#include <CyberCore/AudioDestination.h>
#include <CyberCore/AudioIOCallback.h>
#include <CyberCore/CDMFactory.h>
#include <CyberCore/NowPlayingManager.h>

#if PLATFORM(COCOA)
#include <CyberCore/MediaSessionManagerCocoa.h>
#endif

namespace CyberKit {

WebMediaStrategy::~WebMediaStrategy() = default;

#if ENABLE(WEB_AUDIO)
Ref<CyberCore::AudioDestination> WebMediaStrategy::createAudioDestination(CyberCore::AudioIOCallback& callback, const String& inputDeviceId,
    unsigned numberOfInputChannels, unsigned numberOfOutputChannels, float sampleRate)
{
#if ENABLE(GPU_PROCESS)
    if (m_useGPUProcess)
        return RemoteAudioDestinationProxy::create(callback, inputDeviceId, numberOfInputChannels, numberOfOutputChannels, sampleRate);
#endif
    return CyberCore::AudioDestination::create(callback, inputDeviceId, numberOfInputChannels, numberOfOutputChannels, sampleRate);
}
#endif

std::unique_ptr<CyberCore::NowPlayingManager> WebMediaStrategy::createNowPlayingManager() const
{
#if ENABLE(GPU_PROCESS)
    if (m_useGPUProcess) {
        class NowPlayingInfoForGPUManager : public CyberCore::NowPlayingManager {
            void clearNowPlayingInfoPrivate() final
            {
                if (auto* connection = WebProcess::singleton().existingGPUProcessConnection())
                    connection->connection().send(Messages::GPUConnectionToWebProcess::ClearNowPlayingInfo { }, 0);
            }

            void setNowPlayingInfoPrivate(const CyberCore::NowPlayingInfo& nowPlayingInfo) final
            {
                auto& connection = WebProcess::singleton().ensureGPUProcessConnection().connection();
                connection.send(Messages::GPUConnectionToWebProcess::SetNowPlayingInfo { nowPlayingInfo }, 0);
            }
        };
        return makeUnique<NowPlayingInfoForGPUManager>();
    }
#endif
    return CyberCore::MediaStrategy::createNowPlayingManager();
}

} // namespace CyberKit
