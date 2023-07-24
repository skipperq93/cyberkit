/*
 * Copyright (C) 2019-2020 Apple Inc. All rights reserved.
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

#if ENABLE(GPU_PROCESS)

#include "GPUProcessConnection.h"
#include "MediaPlayerPrivateRemoteIdentifier.h"
#include "MessageReceiver.h"
#include "RemoteMediaPlayerState.h"
#include "RemoteMediaResourceIdentifier.h"
#include "SharedMemory.h"
#include "TrackPrivateRemoteIdentifier.h"
#include "WebProcessSupplement.h"
#include <CyberCore/MediaPlayer.h>
#include <wtf/HashMap.h>

namespace CyberCore {
class MediaPlayerPrivateInterface;
class Settings;
}

namespace CyberKit {

class MediaPlayerPrivateRemote;
class RemoteMediaPlayerMIMETypeCache;
class WebProcess;
struct TrackPrivateRemoteConfiguration;

class RemoteMediaPlayerManager
    : public WebProcessSupplement
    , public CanMakeWeakPtr<RemoteMediaPlayerManager> {
    WTF_MAKE_FAST_ALLOCATED;
public:
    explicit RemoteMediaPlayerManager(WebProcess&);
    ~RemoteMediaPlayerManager();

    static const char* supplementName();
    WebProcess& parentProcess() const { return m_process; }

    void updatePreferences(const CyberCore::Settings&);

    GPUProcessConnection& gpuProcessConnection() const;

    void didReceivePlayerMessage(IPC::Connection&, IPC::Decoder&);

    void deleteRemoteMediaPlayer(MediaPlayerPrivateRemoteIdentifier);

    MediaPlayerPrivateRemoteIdentifier findRemotePlayerId(const CyberCore::MediaPlayerPrivateInterface*);

private:
    std::unique_ptr<CyberCore::MediaPlayerPrivateInterface> createRemoteMediaPlayer(CyberCore::MediaPlayer*, CyberCore::MediaPlayerEnums::MediaEngineIdentifier);

    // WebProcessSupplement
    void initialize(const WebProcessCreationParameters&) final;

    friend class MediaPlayerRemoteFactory;
    void getSupportedTypes(CyberCore::MediaPlayerEnums::MediaEngineIdentifier, HashSet<String, ASCIICaseInsensitiveHash>&);
    CyberCore::MediaPlayer::SupportsType supportsTypeAndCodecs(CyberCore::MediaPlayerEnums::MediaEngineIdentifier, const CyberCore::MediaEngineSupportParameters&);
    bool supportsKeySystem(CyberCore::MediaPlayerEnums::MediaEngineIdentifier, const String& keySystem, const String& mimeType);
    HashSet<RefPtr<CyberCore::SecurityOrigin>> originsInMediaCache(CyberCore::MediaPlayerEnums::MediaEngineIdentifier, const String&);
    void clearMediaCache(CyberCore::MediaPlayerEnums::MediaEngineIdentifier, const String&, WallTime modifiedSince);
    void clearMediaCacheForOrigins(CyberCore::MediaPlayerEnums::MediaEngineIdentifier, const String&, const HashSet<RefPtr<CyberCore::SecurityOrigin>>&);

    RemoteMediaPlayerMIMETypeCache& typeCache(CyberCore::MediaPlayerEnums::MediaEngineIdentifier);

    HashMap<MediaPlayerPrivateRemoteIdentifier, WeakPtr<MediaPlayerPrivateRemote>> m_players;
    WebProcess& m_process;
    mutable GPUProcessConnection* m_gpuProcessConnection { nullptr };
};

} // namespace CyberKit

#endif
