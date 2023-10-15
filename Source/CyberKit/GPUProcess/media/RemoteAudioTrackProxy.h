/*
* Copyright (C) 2020 Apple Inc. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#if ENABLE(GPU_PROCESS) && ENABLE(VIDEO)

#include "MessageReceiver.h"
#include "TrackPrivateRemoteIdentifier.h"
#include <CyberCore/AudioTrackPrivate.h>
#include <CyberCore/MediaPlayerIdentifier.h>
#include <CyberCore/TrackBase.h>
#include <wtf/Ref.h>
#include <wtf/WeakPtr.h>

namespace IPC {
class Connection;
class Decoder;
}

namespace CyberKit {

class GPUConnectionToWebProcess;
struct AudioTrackPrivateRemoteConfiguration;

class RemoteAudioTrackProxy final
    : public ThreadSafeRefCounted<RemoteAudioTrackProxy, WTF::DestructionThread::Main>
    , private CyberCore::AudioTrackPrivateClient {
public:
    static Ref<RemoteAudioTrackProxy> create(GPUConnectionToWebProcess& connectionToWebProcess, TrackPrivateRemoteIdentifier identifier, CyberCore::AudioTrackPrivate& trackPrivate, CyberCore::MediaPlayerIdentifier mediaPlayerIdentifier)
    {
        return adoptRef(*new RemoteAudioTrackProxy(connectionToWebProcess, identifier, trackPrivate, mediaPlayerIdentifier));
    }

    virtual ~RemoteAudioTrackProxy();

    TrackPrivateRemoteIdentifier identifier() const { return m_identifier; };
    void setEnabled(bool enabled)
    {
        m_enabled = enabled;
        m_trackPrivate->setEnabled(enabled);
    }

private:
    RemoteAudioTrackProxy(GPUConnectionToWebProcess&, TrackPrivateRemoteIdentifier, CyberCore::AudioTrackPrivate&, CyberCore::MediaPlayerIdentifier);

    // AudioTrackPrivateClient
    void enabledChanged(bool) final;
    void configurationChanged(const CyberCore::PlatformAudioTrackConfiguration&) final;

    // TrackPrivateBaseClient
    void idChanged(const AtomString&) final;
    void labelChanged(const AtomString&) final;
    void languageChanged(const AtomString&) final;
    void willRemove() final;

    AudioTrackPrivateRemoteConfiguration configuration();
    void configurationChanged();

    WeakPtr<GPUConnectionToWebProcess> m_connectionToWebProcess;
    TrackPrivateRemoteIdentifier m_identifier;
    Ref<CyberCore::AudioTrackPrivate> m_trackPrivate;
    CyberCore::MediaPlayerIdentifier m_mediaPlayerIdentifier;
    bool m_enabled { false };
};

} // namespace CyberKit

#endif // ENABLE(GPU_PROCESS) && ENABLE(VIDEO)
