/*
 * Copyright (C) 2019-2023 Apple Inc. All rights reserved.
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

#if ENABLE(GPU_PROCESS) && ENABLE(VIDEO)

#include "Connection.h"
#include "MessageReceiver.h"
#include "RemoteLegacyCDMSessionIdentifier.h"
#include "RemoteMediaPlayerConfiguration.h"
#include "RemoteMediaPlayerProxyConfiguration.h"
#include "RemoteMediaPlayerState.h"
#include "RemoteMediaResourceIdentifier.h"
#include "RemoteVideoFrameProxy.h"
#include "SandboxExtension.h"
#include "ScopedRenderingResourcesRequest.h"
#include "TrackPrivateRemoteIdentifier.h"
#include <CyberCore/Cookie.h>
#include <CyberCore/InbandTextTrackPrivate.h>
#include <CyberCore/MediaPlayer.h>
#include <CyberCore/MediaPlayerIdentifier.h>
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

#if ENABLE(MEDIA_SOURCE)
#include "RemoteMediaSourceIdentifier.h"
#include "RemoteMediaSourceProxy.h"
#endif

#if PLATFORM(COCOA)
#include "SharedCARingBuffer.h"
#endif

#if USE(AVFOUNDATION)
#include <wtf/RetainPtr.h>
#endif

namespace WTF {
class MachSendRight;
}

namespace CyberCore {
class AudioTrackPrivate;
#if ENABLE(WIRELESS_PLAYBACK_TARGET)
class MediaPlaybackTargetContext;
#endif
class SecurityOriginData;
class VideoTrackPrivate;

struct FourCC;

class VideoFrame;

#if PLATFORM(COCOA)
class VideoFrameCV;
#endif
}

#if USE(AVFOUNDATION)
typedef struct __CVBuffer* CVPixelBufferRef;
#endif

namespace CyberKit {

using LayerHostingContextID = uint32_t;
class LayerHostingContext;
class RemoteAudioTrackProxy;
class RemoteAudioSourceProviderProxy;
class RemoteMediaPlayerManagerProxy;
class RemoteTextTrackProxy;
class RemoteVideoFrameObjectHeap;
class RemoteVideoTrackProxy;

class RemoteMediaPlayerProxy final
    : public RefCounted<RemoteMediaPlayerProxy>
    , public CyberCore::MediaPlayerClient
    , public IPC::MessageReceiver {
    WTF_MAKE_FAST_ALLOCATED;
public:
    static Ref<RemoteMediaPlayerProxy> create(RemoteMediaPlayerManagerProxy&, CyberCore::MediaPlayerIdentifier, Ref<IPC::Connection>&&, CyberCore::MediaPlayerEnums::MediaEngineIdentifier, RemoteMediaPlayerProxyConfiguration&&, RemoteVideoFrameObjectHeap&, const CyberCore::ProcessIdentity&);
    ~RemoteMediaPlayerProxy();

    CyberCore::MediaPlayerIdentifier identifier() const { return m_id; }
    void invalidate();

#if ENABLE(VIDEO_PRESENTATION_MODE)
    void updateVideoFullscreenInlineImage();
    void setVideoFullscreenMode(CyberCore::MediaPlayer::VideoFullscreenMode);
    void videoFullscreenStandbyChanged(bool);
#endif

    void setBufferingPolicy(CyberCore::MediaPlayer::BufferingPolicy);

#if PLATFORM(IOS_FAMILY)
    void accessLog(CompletionHandler<void(String)>&&);
    void errorLog(CompletionHandler<void(String)>&&);
#endif

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;
    bool didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, UniqueRef<IPC::Encoder>&);

    void getConfiguration(RemoteMediaPlayerConfiguration&);

    void prepareForPlayback(bool privateMode, CyberCore::MediaPlayerEnums::Preload, bool preservesPitch, bool prepareForRendering, CyberCore::IntSize presentationSize, float videoContentScale, CyberCore::DynamicRangeMode);
    void prepareForRendering();

    void load(URL&&, std::optional<SandboxExtension::Handle>&&, const CyberCore::ContentType&, const String&, bool, CompletionHandler<void(RemoteMediaPlayerConfiguration&&)>&&);
#if ENABLE(MEDIA_SOURCE)
    void loadMediaSource(URL&&, const CyberCore::ContentType&, bool webMParserEnabled, RemoteMediaSourceIdentifier, CompletionHandler<void(RemoteMediaPlayerConfiguration&&)>&&);
#endif
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
    void setPitchCorrectionAlgorithm(CyberCore::MediaPlayer::PitchCorrectionAlgorithm);

    void setPageIsVisible(bool);
    void setShouldMaintainAspectRatio(bool);
#if ENABLE(VIDEO_PRESENTATION_MODE)
    void setVideoFullscreenGravity(CyberCore::MediaPlayerEnums::VideoGravity);
#endif
    void acceleratedRenderingStateChanged(bool);
    void setShouldDisableSleep(bool);
    void setRate(double);
    void didLoadingProgress(CompletionHandler<void(bool)>&&);

    void setPresentationSize(const CyberCore::IntSize&);

#if PLATFORM(COCOA)
    void setVideoInlineSizeFenced(const CyberCore::FloatSize&, const WTF::MachSendRight&);
#endif

#if ENABLE(WIRELESS_PLAYBACK_TARGET)
    void setWirelessVideoPlaybackDisabled(bool);
    void setShouldPlayToPlaybackTarget(bool);
    void setWirelessPlaybackTarget(CyberCore::MediaPlaybackTargetContext&&);
    void mediaPlayerCurrentPlaybackTargetIsWirelessChanged(bool) final;
#endif

#if ENABLE(LEGACY_ENCRYPTED_MEDIA)
    void setLegacyCDMSession(std::optional<RemoteLegacyCDMSessionIdentifier>&& instanceId);
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

    void audioTrackSetEnabled(const TrackPrivateRemoteIdentifier&, bool);
    void videoTrackSetSelected(const TrackPrivateRemoteIdentifier&, bool);
    void textTrackSetMode(const TrackPrivateRemoteIdentifier&, CyberCore::InbandTextTrackPrivate::Mode);

    using PerformTaskAtMediaTimeCompletionHandler = CompletionHandler<void(std::optional<MediaTime>, std::optional<MonotonicTime>)>;
    void performTaskAtMediaTime(const MediaTime&, MonotonicTime, PerformTaskAtMediaTimeCompletionHandler&&);
    void isCrossOrigin(CyberCore::SecurityOriginData, CompletionHandler<void(std::optional<bool>)>&&);

    void setVideoPlaybackMetricsUpdateInterval(double);

    void setPreferredDynamicRangeMode(CyberCore::DynamicRangeMode);

    RefPtr<CyberCore::PlatformMediaResource> requestResource(CyberCore::ResourceRequest&&, CyberCore::PlatformMediaResourceLoader::LoadOptions);
    void sendH2Ping(const URL&, CompletionHandler<void(Expected<WTF::Seconds, CyberCore::ResourceError>&&)>&&);
    void removeResource(RemoteMediaResourceIdentifier);

    RefPtr<CyberCore::MediaPlayer> mediaPlayer() { return m_player; }

    TrackPrivateRemoteIdentifier addRemoteAudioTrackProxy(CyberCore::AudioTrackPrivate&);
    TrackPrivateRemoteIdentifier addRemoteVideoTrackProxy(CyberCore::VideoTrackPrivate&);
    TrackPrivateRemoteIdentifier addRemoteTextTrackProxy(CyberCore::InbandTextTrackPrivate&);

private:
    RemoteMediaPlayerProxy(RemoteMediaPlayerManagerProxy&, CyberCore::MediaPlayerIdentifier, Ref<IPC::Connection>&&, CyberCore::MediaPlayerEnums::MediaEngineIdentifier, RemoteMediaPlayerProxyConfiguration&&, RemoteVideoFrameObjectHeap&, const CyberCore::ProcessIdentity&);

    // MediaPlayerClient
    void mediaPlayerCharacteristicChanged() final;
    void mediaPlayerRenderingModeChanged() final;
    void mediaPlayerNetworkStateChanged() final;
    void mediaPlayerReadyStateChanged() final;
    void mediaPlayerVolumeChanged() final;
    void mediaPlayerMuteChanged() final;
    void mediaPlayerTimeChanged() final;
    void mediaPlayerDurationChanged() final;
    void mediaPlayerSizeChanged() final;
    void mediaPlayerRateChanged() final;
    void mediaPlayerPlaybackStateChanged() final;
    void mediaPlayerResourceNotSupported() final;
    void mediaPlayerEngineFailedToLoad() const final;
    void mediaPlayerActiveSourceBuffersChanged() final;
    void mediaPlayerBufferedTimeRangesChanged() final;
    void mediaPlayerSeekableTimeRangesChanged() final;
    bool mediaPlayerRenderingCanBeAccelerated() final;

    void mediaPlayerDidAddAudioTrack(CyberCore::AudioTrackPrivate&) final;
    void mediaPlayerDidRemoveAudioTrack(CyberCore::AudioTrackPrivate&) final;
    void mediaPlayerDidAddVideoTrack(CyberCore::VideoTrackPrivate&) final;
    void mediaPlayerDidRemoveVideoTrack(CyberCore::VideoTrackPrivate&) final;
    void mediaPlayerDidAddTextTrack(CyberCore::InbandTextTrackPrivate&) final;
    void mediaPlayerDidRemoveTextTrack(CyberCore::InbandTextTrackPrivate&) final;

    // Not implemented
    void mediaPlayerFirstVideoFrameAvailable() final;

#if ENABLE(LEGACY_ENCRYPTED_MEDIA)
    RefPtr<ArrayBuffer> mediaPlayerCachedKeyForKeyId(const String&) const final;
    void mediaPlayerKeyNeeded(const CyberCore::SharedBuffer&) final;
    String mediaPlayerMediaKeysStorageDirectory() const final;
#endif

#if ENABLE(ENCRYPTED_MEDIA)
    void mediaPlayerInitializationDataEncountered(const String&, RefPtr<ArrayBuffer>&&) final;
    void mediaPlayerWaitingForKeyChanged() final;
#endif

    String mediaPlayerReferrer() const final;
    String mediaPlayerUserAgent() const final;
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
    CyberCore::LayoutRect mediaPlayerContentBoxRect() const final;

    void textTrackRepresentationBoundsChanged(const CyberCore::IntRect&) final;

#if ENABLE(AVF_CAPTIONS)
    Vector<RefPtr<CyberCore::PlatformTextTrack>> outOfBandTrackSources() final;
#endif

#if PLATFORM(IOS_FAMILY)
    String mediaPlayerNetworkInterfaceName() const final;
    void mediaPlayerGetRawCookies(const URL&, CyberCore::MediaPlayerClient::GetRawCookiesCallback&&) const final;
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

    const std::optional<Vector<String>>& allowedMediaContainerTypes() const final { return m_configuration.allowedMediaContainerTypes; };
    const std::optional<Vector<String>>& allowedMediaCodecTypes() const final { return m_configuration.allowedMediaCodecTypes; };
    const std::optional<Vector<CyberCore::FourCC>>& allowedMediaVideoCodecIDs() const final { return m_configuration.allowedMediaVideoCodecIDs; };
    const std::optional<Vector<CyberCore::FourCC>>& allowedMediaAudioCodecIDs() const final { return m_configuration.allowedMediaAudioCodecIDs; };
    const std::optional<Vector<CyberCore::FourCC>>& allowedMediaCaptionFormatTypes() const final { return m_configuration.allowedMediaCaptionFormatTypes; };

    bool mediaPlayerPrefersSandboxedParsing() const final { return m_configuration.prefersSandboxedParsing; }
    bool mediaPlayerShouldDisableHDR() const final { return m_configuration.shouldDisableHDR; }

    void startUpdateCachedStateMessageTimer();
    void updateCachedState(bool = false);
    void sendCachedState();
    void timerFired();

    void maybeUpdateCachedVideoMetrics();
    void updateCachedVideoMetrics();

    void createAudioSourceProvider();
    void setShouldEnableAudioSourceProvider(bool);

    void playAtHostTime(MonotonicTime);
    void pauseAtHostTime(MonotonicTime);

    void startVideoFrameMetadataGathering();
    void stopVideoFrameMetadataGathering();
#if PLATFORM(COCOA)
    void mediaPlayerOnNewVideoFrameMetadata(CyberCore::VideoFrameMetadata&&, RetainPtr<CVPixelBufferRef>&&);
#endif

    void playerContentBoxRectChanged(const CyberCore::LayoutRect&);

    bool mediaPlayerPausedOrStalled() const;
    void currentTimeChanged(const MediaTime&);

#if PLATFORM(COCOA)
    void setVideoInlineSizeIfPossible(const CyberCore::FloatSize&);
    void nativeImageForCurrentTime(CompletionHandler<void(std::optional<WTF::MachSendRight>&&, CyberCore::DestinationColorSpace)>&&);
    void colorSpace(CompletionHandler<void(CyberCore::DestinationColorSpace)>&&);
#if !HAVE(AVSAMPLEBUFFERDISPLAYLAYER_COPYDISPLAYEDPIXELBUFFER)
    void willBeAskedToPaintGL();
#endif
#endif
    void videoFrameForCurrentTimeIfChanged(CompletionHandler<void(std::optional<RemoteVideoFrameProxy::Properties>&&, bool)>&&);

    void setShouldDisableHDR(bool);

#if !RELEASE_LOG_DISABLED
    const Logger& mediaPlayerLogger() final { return m_logger; }
    const void* mediaPlayerLogIdentifier() { return reinterpret_cast<const void*>(m_configuration.logIdentifier); }
    const Logger& logger() { return mediaPlayerLogger(); }
    const void* logIdentifier() { return mediaPlayerLogIdentifier(); }
    const char* logClassName() const { return "RemoteMediaPlayerProxy"; }
    WTFLogChannel& logChannel() const;
#endif

    HashMap<Ref<CyberCore::AudioTrackPrivate>, Ref<RemoteAudioTrackProxy>> m_audioTracks;
    HashMap<Ref<CyberCore::VideoTrackPrivate>, Ref<RemoteVideoTrackProxy>> m_videoTracks;
    HashMap<Ref<CyberCore::InbandTextTrackPrivate>, Ref<RemoteTextTrackProxy>> m_textTracks;

    CyberCore::MediaPlayerIdentifier m_id;
    RefPtr<SandboxExtension> m_sandboxExtension;
    Ref<IPC::Connection> m_webProcessConnection;
    RefPtr<CyberCore::MediaPlayer> m_player;
    std::unique_ptr<LayerHostingContext> m_inlineLayerHostingContext;
#if ENABLE(VIDEO_PRESENTATION_MODE)
    std::unique_ptr<LayerHostingContext> m_fullscreenLayerHostingContext;
#endif
    WeakPtr<RemoteMediaPlayerManagerProxy> m_manager;
    CyberCore::MediaPlayerEnums::MediaEngineIdentifier m_engineIdentifier;
    Vector<CyberCore::ContentType> m_typesRequiringHardwareSupport;
    RunLoop::Timer m_updateCachedStateMessageTimer;
    RemoteMediaPlayerState m_cachedState;
    RemoteMediaPlayerProxyConfiguration m_configuration;
    PerformTaskAtMediaTimeCompletionHandler m_performTaskAtMediaTimeCompletionHandler;
#if ENABLE(MEDIA_SOURCE)
    RefPtr<RemoteMediaSourceProxy> m_mediaSourceProxy;
#endif

    Seconds m_videoPlaybackMetricsUpdateInterval;
    MonotonicTime m_nextPlaybackQualityMetricsUpdateTime;

    CyberCore::FloatSize m_videoInlineSize;
    float m_videoContentScale { 1.0 };
    CyberCore::LayoutRect m_playerContentBoxRect;

    bool m_bufferedChanged { true };
    bool m_renderingCanBeAccelerated { false };
    CyberCore::MediaPlayer::VideoFullscreenMode m_fullscreenMode { CyberCore::MediaPlayer::VideoFullscreenModeNone };
    bool m_videoFullscreenStandby { false };

#if ENABLE(LEGACY_ENCRYPTED_MEDIA) && ENABLE(ENCRYPTED_MEDIA)
    bool m_shouldContinueAfterKeyNeeded { false };
    std::optional<RemoteLegacyCDMSessionIdentifier> m_legacySession;
#endif

#if ENABLE(WEB_AUDIO) && PLATFORM(COCOA)
    RefPtr<RemoteAudioSourceProviderProxy> m_remoteAudioSourceProvider;
#endif
    ScopedRenderingResourcesRequest m_renderingResourcesRequest;

    bool m_observingTimeChanges { false };
    Ref<RemoteVideoFrameObjectHeap> m_videoFrameObjectHeap;
    RefPtr<CyberCore::VideoFrame> m_videoFrameForCurrentTime;
#if !RELEASE_LOG_DISABLED
    const Logger& m_logger;
#endif
};

} // namespace CyberKit

#endif // ENABLE(GPU_PROCESS) && ENABLE(VIDEO)
