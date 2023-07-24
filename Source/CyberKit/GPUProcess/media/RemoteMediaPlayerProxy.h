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

#include "Connection.h"
#include "MediaPlayerPrivateRemoteIdentifier.h"
#include "MessageReceiver.h"
#include "RemoteLegacyCDMSessionIdentifier.h"
#include "RemoteMediaPlayerConfiguration.h"
#include "RemoteMediaPlayerProxyConfiguration.h"
#include "RemoteMediaPlayerState.h"
#include "RemoteMediaResourceIdentifier.h"
#include "SandboxExtension.h"
#include "TrackPrivateRemoteIdentifier.h"
#include <CyberCore/Cookie.h>
#include <CyberCore/InbandTextTrackPrivate.h>
#include <CyberCore/MediaPlayer.h>
#include <CyberCore/PlatformMediaResourceLoader.h>
#include <wtf/LoggerHelper.h>
#include <wtf/RefPtr.h>
#include <wtf/RunLoop.h>
#include <wtf/Vector.h>
#include <wtf/WeakPtr.h>

#if ENABLE(ENCRYPTED_MEDIA)
#include "RemoteCDMInstanceIdentifier.h"
#include "RemoteCDMInstanceProxy.h"
#endif

namespace WTF {
class MachSendRight;
}

namespace CyberCore {
class AudioTrackPrivate;
class VideoTrackPrivate;
}

namespace CyberKit {

using LayerHostingContextID = uint32_t;
class LayerHostingContext;
class RemoteAudioTrackProxy;
class RemoteMediaPlayerManagerProxy;
class RemoteTextTrackProxy;
class RemoteVideoTrackProxy;

class RemoteMediaPlayerProxy final
    : public CanMakeWeakPtr<RemoteMediaPlayerProxy>
    , public CyberCore::MediaPlayerClient
    , private IPC::MessageReceiver {
    WTF_MAKE_FAST_ALLOCATED;
public:
    RemoteMediaPlayerProxy(RemoteMediaPlayerManagerProxy&, MediaPlayerPrivateRemoteIdentifier, Ref<IPC::Connection>&&, CyberCore::MediaPlayerEnums::MediaEngineIdentifier, RemoteMediaPlayerProxyConfiguration&&);
    ~RemoteMediaPlayerProxy();

    MediaPlayerPrivateRemoteIdentifier idendifier() const { return m_id; }
    void invalidate();

#if ENABLE(VIDEO_PRESENTATION_MODE)
    void updateVideoFullscreenInlineImage();
    void setVideoFullscreenMode(CyberCore::MediaPlayer::VideoFullscreenMode);
    void videoFullscreenStandbyChanged();
#endif

    void setBufferingPolicy(CyberCore::MediaPlayer::BufferingPolicy);

#if PLATFORM(IOS_FAMILY)
    void accessLog(CompletionHandler<void(String)>&&);
    void errorLog(CompletionHandler<void(String)>&&);
#endif

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;
    void didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, std::unique_ptr<IPC::Encoder>&);

    void getConfiguration(RemoteMediaPlayerConfiguration&);

    void prepareForPlayback(bool privateMode, CyberCore::MediaPlayerEnums::Preload, bool preservesPitch, bool prepareForRendering, float videoContentScale, CompletionHandler<void(Optional<LayerHostingContextID>&& inlineLayerHostingContextId, Optional<LayerHostingContextID>&& fullscreenLayerHostingContextId)>&&);
    void prepareForRendering();

    void load(URL&&, Optional<SandboxExtension::Handle>&&, const CyberCore::ContentType&, const String&, CompletionHandler<void(RemoteMediaPlayerConfiguration&&)>&&);
    void cancelLoad();

    void prepareToPlay();

    void play();
    void pause();

    void seek(const MediaTime&);
    void seekWithTolerance(const MediaTime&, const MediaTime& negativeTolerance, const MediaTime& positiveTolerance);

    void setVolume(double);
    void setMuted(bool);

    void setPreload(CyberCore::MediaPlayerEnums::Preload);
    void setPrivateBrowsingMode(bool);
    void setPreservesPitch(bool);

    void setVisible(bool);
    void setShouldMaintainAspectRatio(bool);
#if ENABLE(VIDEO_PRESENTATION_MODE)
    void enterFullscreen(CompletionHandler<void()>&&);
    void exitFullscreen(CompletionHandler<void()>&&);
    void setVideoFullscreenGravity(CyberCore::MediaPlayerEnums::VideoGravity);
#endif
    void acceleratedRenderingStateChanged(bool);
    void setShouldDisableSleep(bool);
    void setRate(double);

    void audioTrackSetEnabled(TrackPrivateRemoteIdentifier, bool);
    void videoTrackSetSelected(TrackPrivateRemoteIdentifier, bool);
    void textTrackSetMode(TrackPrivateRemoteIdentifier, CyberCore::InbandTextTrackPrivate::Mode);

#if PLATFORM(COCOA)
    void setVideoInlineSizeFenced(const CyberCore::IntSize&, const WTF::MachSendRight&);
#if ENABLE(VIDEO_PRESENTATION_MODE)
    void setVideoFullscreenFrameFenced(const CyberCore::FloatRect&, const WTF::MachSendRight&);
#endif
#endif

#if ENABLE(WIRELESS_PLAYBACK_TARGET)
    void setWirelessVideoPlaybackDisabled(bool);
    void setShouldPlayToPlaybackTarget(bool);
    void setWirelessPlaybackTarget(const CyberCore::MediaPlaybackTargetContext&);
    void mediaPlayerCurrentPlaybackTargetIsWirelessChanged(bool) final;
#endif

#if ENABLE(LEGACY_ENCRYPTED_MEDIA)
    void setLegacyCDMSession(RemoteLegacyCDMSessionIdentifier&& instanceId);
    void keyAdded();
#endif

#if ENABLE(ENCRYPTED_MEDIA)
    void cdmInstanceAttached(RemoteCDMInstanceIdentifier&&);
    void cdmInstanceDetached(RemoteCDMInstanceIdentifier&&);
    void attemptToDecryptWithInstance(RemoteCDMInstanceIdentifier&&);
#endif

#if ENABLE(LEGACY_ENCRYPTED_MEDIA) && ENABLE(ENCRYPTED_MEDIA)
    void setShouldContinueAfterKeyNeeded(bool);
#endif

    void beginSimulatedHDCPError();
    void endSimulatedHDCPError();

    void notifyActiveSourceBuffersChanged();

    void applicationWillResignActive();
    void applicationDidBecomeActive();

    void notifyTrackModeChanged();
    void tracksChanged();
    void syncTextTrackBounds();

    void performTaskAtMediaTime(const MediaTime&, WallTime, CompletionHandler<void(Optional<MediaTime>)>&&);
    void wouldTaintOrigin(struct CyberCore::SecurityOriginData, CompletionHandler<void(Optional<bool>)>&&);

    Ref<CyberCore::PlatformMediaResource> requestResource(CyberCore::ResourceRequest&&, CyberCore::PlatformMediaResourceLoader::LoadOptions);
    void sendH2Ping(const URL&, CompletionHandler<void(Expected<WTF::Seconds, CyberCore::ResourceError>&&)>&&);
    void removeResource(RemoteMediaResourceIdentifier);

    RefPtr<CyberCore::MediaPlayer> mediaPlayer() { return m_player; }

private:
    // MediaPlayerClient
    void mediaPlayerNetworkStateChanged() final;
    void mediaPlayerReadyStateChanged() final;
    void mediaPlayerVolumeChanged() final;
    void mediaPlayerMuteChanged() final;
    void mediaPlayerTimeChanged() final;
    void mediaPlayerDurationChanged() final;
    void mediaPlayerRateChanged() final;
    void mediaPlayerPlaybackStateChanged() final;
    void mediaPlayerEngineFailedToLoad() const final;
    void mediaPlayerBufferedTimeRangesChanged() final;
    void mediaPlayerSeekableTimeRangesChanged() final;
    bool mediaPlayerRenderingCanBeAccelerated() final;

    void mediaPlayerDidAddAudioTrack(CyberCore::AudioTrackPrivate&) final;
    void mediaPlayerDidRemoveAudioTrack(CyberCore::AudioTrackPrivate&) final;
    void mediaPlayerDidAddVideoTrack(CyberCore::VideoTrackPrivate&) final;
    void mediaPlayerDidRemoveVideoTrack(CyberCore::VideoTrackPrivate&) final;

    // Not implemented
    void mediaPlayerResourceNotSupported() final;
    void mediaPlayerSizeChanged() final;
    void mediaPlayerEngineUpdated() final;
    void mediaPlayerFirstVideoFrameAvailable() final;
    void mediaPlayerCharacteristicChanged() final;
    void mediaPlayerRenderingModeChanged() final;
    void mediaPlayerActiveSourceBuffersChanged() final;

#if ENABLE(LEGACY_ENCRYPTED_MEDIA)
    RefPtr<ArrayBuffer> mediaPlayerCachedKeyForKeyId(const String&) const final;
    void mediaPlayerKeyNeeded(Uint8Array*) final;
    String mediaPlayerMediaKeysStorageDirectory() const final;
#endif

#if ENABLE(ENCRYPTED_MEDIA)
    void mediaPlayerInitializationDataEncountered(const String&, RefPtr<ArrayBuffer>&&) final;
    void mediaPlayerWaitingForKeyChanged() final;
#endif

    String mediaPlayerReferrer() const final;
    String mediaPlayerUserAgent() const final;
    void mediaPlayerEnterFullscreen() final;
    void mediaPlayerExitFullscreen() final;
    bool mediaPlayerIsFullscreen() const final;
    bool mediaPlayerIsFullscreenPermitted() const final;
    bool mediaPlayerIsVideo() const final;
    float mediaPlayerContentsScale() const final;
    bool mediaPlayerPlatformVolumeConfigurationRequired() const final;
    CyberCore::CachedResourceLoader* mediaPlayerCachedResourceLoader() final;
    RefPtr<CyberCore::PlatformMediaResourceLoader> mediaPlayerCreateResourceLoader() final;
    bool doesHaveAttribute(const AtomString&, AtomString* = nullptr) const final;
    bool mediaPlayerShouldUsePersistentCache() const final;
    const String& mediaPlayerMediaCacheDirectory() const final;

    void mediaPlayerDidAddTextTrack(CyberCore::InbandTextTrackPrivate&) final;
    void mediaPlayerDidRemoveTextTrack(CyberCore::InbandTextTrackPrivate&) final;

    void textTrackRepresentationBoundsChanged(const CyberCore::IntRect&) final;

#if ENABLE(AVF_CAPTIONS)
    Vector<RefPtr<CyberCore::PlatformTextTrack>> outOfBandTrackSources() final;
#endif

#if PLATFORM(IOS_FAMILY)
    String mediaPlayerNetworkInterfaceName() const final;
    bool mediaPlayerGetRawCookies(const URL&, Vector<CyberCore::Cookie>&) const final;
#endif

    String mediaPlayerSourceApplicationIdentifier() const final;

    double mediaPlayerRequestedPlaybackRate() const final;
#if ENABLE(VIDEO_PRESENTATION_MODE)
    CyberCore::MediaPlayerEnums::VideoFullscreenMode mediaPlayerFullscreenMode() const final;
    bool mediaPlayerIsVideoFullscreenStandby() const final;
#endif
    Vector<String> mediaPlayerPreferredAudioCharacteristics() const final;

    bool mediaPlayerShouldDisableSleep() const final;
    const Vector<CyberCore::ContentType>& mediaContentTypesRequiringHardwareSupport() const final;
    bool mediaPlayerShouldCheckHardwareSupport() const final;

    void startUpdateCachedStateMessageTimer();
    void updateCachedState();
    void sendCachedState();
    void timerFired();

#if !RELEASE_LOG_DISABLED
    const Logger& mediaPlayerLogger() final { return m_logger; }
    const void* mediaPlayerLogIdentifier() { return reinterpret_cast<const void*>(m_configuration.logIdentifier); }
#endif

    HashMap<CyberCore::AudioTrackPrivate*, Ref<RemoteAudioTrackProxy>> m_audioTracks;
    HashMap<CyberCore::VideoTrackPrivate*, Ref<RemoteVideoTrackProxy>> m_videoTracks;
    HashMap<CyberCore::InbandTextTrackPrivate*, Ref<RemoteTextTrackProxy>> m_textTracks;

    MediaPlayerPrivateRemoteIdentifier m_id;
    RefPtr<SandboxExtension> m_sandboxExtension;
    Ref<IPC::Connection> m_webProcessConnection;
    RefPtr<CyberCore::MediaPlayer> m_player;
    std::unique_ptr<LayerHostingContext> m_inlineLayerHostingContext;
#if ENABLE(VIDEO_PRESENTATION_MODE)
    std::unique_ptr<LayerHostingContext> m_fullscreenLayerHostingContext;
#endif
    RemoteMediaPlayerManagerProxy& m_manager;
    CyberCore::MediaPlayerEnums::MediaEngineIdentifier m_engineIdentifier;
    Vector<CyberCore::ContentType> m_typesRequiringHardwareSupport;
    RunLoop::Timer<RemoteMediaPlayerProxy> m_updateCachedStateMessageTimer;
    RemoteMediaPlayerState m_cachedState;
    RemoteMediaPlayerProxyConfiguration m_configuration;
    CompletionHandler<void(Optional<MediaTime>)> m_performTaskAtMediaTimeCompletionHandler;

    CyberCore::LayoutRect m_videoContentBoxRect;
    float m_videoContentScale { 1.0 };

    bool m_bufferedChanged { true };
    bool m_renderingCanBeAccelerated { true };

#if ENABLE(LEGACY_ENCRYPTED_MEDIA) && ENABLE(ENCRYPTED_MEDIA)
    bool m_shouldContinueAfterKeyNeeded { false };
    RemoteLegacyCDMSessionIdentifier m_legacySession;
#endif

#if !RELEASE_LOG_DISABLED
    const Logger& m_logger;
#endif
};

} // namespace CyberKit

#endif
