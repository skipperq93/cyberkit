/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
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

#if ENABLE(VIDEO_PRESENTATION_MODE)

#include "LayerHostingContext.h"
#include "MessageReceiver.h"
#include "PlaybackSessionContextIdentifier.h"
#include <CyberCore/AudioSession.h>
#include <CyberCore/GraphicsLayer.h>
#include <CyberCore/PlatformView.h>
#include <CyberCore/VideoFullscreenChangeObserver.h>
#include <CyberCore/VideoFullscreenModel.h>
#include <wtf/HashMap.h>
#include <wtf/HashSet.h>
#include <wtf/RefCounted.h>
#include <wtf/RefPtr.h>
#include <wtf/text/WTFString.h>

#if PLATFORM(IOS_FAMILY)
#include <CyberCore/VideoFullscreenInterfaceAVKit.h>
#else
#include <CyberCore/VideoFullscreenInterfaceMac.h>
#endif

#if PLATFORM(IOS_FAMILY)
typedef CyberCore::VideoFullscreenInterfaceAVKit PlatformVideoFullscreenInterface;
#else
typedef CyberCore::VideoFullscreenInterfaceMac PlatformVideoFullscreenInterface;
#endif

namespace CyberKit {

constexpr size_t DefaultMockPictureInPictureWindowWidth = 100;
constexpr size_t DefaultMockPictureInPictureWindowHeight = 100;

class WebPageProxy;
class PlaybackSessionManagerProxy;
class PlaybackSessionModelContext;
class VideoFullscreenManagerProxy;

class VideoFullscreenModelContext final
    : public RefCounted<VideoFullscreenModelContext>
    , public CyberCore::VideoFullscreenModel
    , public CyberCore::VideoFullscreenChangeObserver  {
public:
    static Ref<VideoFullscreenModelContext> create(VideoFullscreenManagerProxy& manager, PlaybackSessionModelContext& playbackSessionModel, PlaybackSessionContextIdentifier contextId)
    {
        return adoptRef(*new VideoFullscreenModelContext(manager, playbackSessionModel, contextId));
    }
    virtual ~VideoFullscreenModelContext();

    void invalidate() { m_manager = nullptr; }

    PlatformView *layerHostView() const { return m_layerHostView.get(); }
    void setLayerHostView(RetainPtr<PlatformView>&& layerHostView) { m_layerHostView = WTFMove(layerHostView); }

private:
    VideoFullscreenModelContext(VideoFullscreenManagerProxy&, PlaybackSessionModelContext&, PlaybackSessionContextIdentifier);

    // VideoFullscreenModel
    void addClient(CyberCore::VideoFullscreenModelClient&) override;
    void removeClient(CyberCore::VideoFullscreenModelClient&) override;
    void requestFullscreenMode(CyberCore::HTMLMediaElementEnums::VideoFullscreenMode, bool finishedWithMedia = false) override;
    void setVideoLayerFrame(CyberCore::FloatRect) override;
    void setVideoLayerGravity(CyberCore::MediaPlayerEnums::VideoGravity) override;
    void fullscreenModeChanged(CyberCore::HTMLMediaElementEnums::VideoFullscreenMode) override;
    bool hasVideo() const override { return m_hasVideo; }
    CyberCore::FloatSize videoDimensions() const override { return m_videoDimensions; }
#if PLATFORM(IOS_FAMILY)
    UIViewController *presentingViewController() final;
    UIViewController *createVideoFullscreenViewController(AVPlayerViewController*) final;
#endif
    void willEnterPictureInPicture() final;
    void didEnterPictureInPicture() final;
    void failedToEnterPictureInPicture() final;
    void willExitPictureInPicture() final;
    void didExitPictureInPicture() final;
    void requestRouteSharingPolicyAndContextUID(CompletionHandler<void(CyberCore::RouteSharingPolicy, String)>&&) final;

    // VideoFullscreenChangeObserver
    void requestUpdateInlineRect() final;
    void requestVideoContentLayer() final;
    void returnVideoContentLayer() final;
    void didSetupFullscreen() final;
    void didEnterFullscreen() final;
    void willExitFullscreen() final;
    void didExitFullscreen() final;
    void didCleanupFullscreen() final;
    void prepareToExitFullscreen() final;
    void fullscreenMayReturnToInline() final;
    void fullscreenWillReturnToInline() final;

    VideoFullscreenManagerProxy* m_manager;
    Ref<PlaybackSessionModelContext> m_playbackSessionModel;
    PlaybackSessionContextIdentifier m_contextId;
    RetainPtr<PlatformView> m_layerHostView;
    HashSet<CyberCore::VideoFullscreenModelClient*> m_clients;
    CyberCore::FloatSize m_videoDimensions;
    bool m_hasVideo { false };
};

class VideoFullscreenManagerProxy : public RefCounted<VideoFullscreenManagerProxy>, private IPC::MessageReceiver {
public:
    static Ref<VideoFullscreenManagerProxy> create(WebPageProxy&, PlaybackSessionManagerProxy&);
    virtual ~VideoFullscreenManagerProxy();

    void invalidate();

    void requestHideAndExitFullscreen();
    bool hasMode(CyberCore::HTMLMediaElementEnums::VideoFullscreenMode) const;
    bool mayAutomaticallyShowVideoPictureInPicture() const;
    void applicationDidBecomeActive();
    bool isVisible() const;

    void setMockVideoPresentationModeEnabled(bool enabled) { m_mockVideoPresentationModeEnabled = enabled; }

    void requestRouteSharingPolicyAndContextUID(PlaybackSessionContextIdentifier, CompletionHandler<void(CyberCore::RouteSharingPolicy, String)>&&);

    bool isPlayingVideoInEnhancedFullscreen() const;

    PlatformVideoFullscreenInterface* controlsManagerInterface();

    void forEachSession(Function<void(CyberCore::VideoFullscreenModel&, PlatformVideoFullscreenInterface&)>&&);

private:
    friend class VideoFullscreenModelContext;

    explicit VideoFullscreenManagerProxy(WebPageProxy&, PlaybackSessionManagerProxy&);
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;

    typedef std::tuple<RefPtr<VideoFullscreenModelContext>, RefPtr<PlatformVideoFullscreenInterface>> ModelInterfaceTuple;
    ModelInterfaceTuple createModelAndInterface(PlaybackSessionContextIdentifier);
    ModelInterfaceTuple& ensureModelAndInterface(PlaybackSessionContextIdentifier);
    VideoFullscreenModelContext& ensureModel(PlaybackSessionContextIdentifier);
    PlatformVideoFullscreenInterface& ensureInterface(PlaybackSessionContextIdentifier);
    PlatformVideoFullscreenInterface* findInterface(PlaybackSessionContextIdentifier);
    void addClientForContext(PlaybackSessionContextIdentifier);
    void removeClientForContext(PlaybackSessionContextIdentifier);

    void hasVideoInPictureInPictureDidChange(bool);

    // Messages from VideoFullscreenManager
    void setupFullscreenWithID(PlaybackSessionContextIdentifier, CyberKit::LayerHostingContextID videoLayerID, const CyberCore::IntRect& initialRect, const CyberCore::FloatSize& videoDimensions, float hostingScaleFactor, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode, bool allowsPictureInPicture, bool standby);
    void setInlineRect(PlaybackSessionContextIdentifier, const CyberCore::IntRect& inlineRect, bool visible);
    void setHasVideoContentLayer(PlaybackSessionContextIdentifier, bool value);
    void setHasVideo(PlaybackSessionContextIdentifier, bool);
    void setVideoDimensions(PlaybackSessionContextIdentifier, const CyberCore::FloatSize&);
    void enterFullscreen(PlaybackSessionContextIdentifier);
    void exitFullscreen(PlaybackSessionContextIdentifier, CyberCore::IntRect finalRect);
    void cleanupFullscreen(PlaybackSessionContextIdentifier);
    void preparedToReturnToInline(PlaybackSessionContextIdentifier, bool visible, CyberCore::IntRect inlineRect);
    void preparedToExitFullscreen(PlaybackSessionContextIdentifier);
    void exitFullscreenWithoutAnimationToMode(PlaybackSessionContextIdentifier, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode);

    // Messages to VideoFullscreenManager
    void requestFullscreenMode(PlaybackSessionContextIdentifier, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode, bool finishedWithMedia = false);
    void requestUpdateInlineRect(PlaybackSessionContextIdentifier);
    void requestVideoContentLayer(PlaybackSessionContextIdentifier);
    void returnVideoContentLayer(PlaybackSessionContextIdentifier);
    void didSetupFullscreen(PlaybackSessionContextIdentifier);
    void willExitFullscreen(PlaybackSessionContextIdentifier);
    void didExitFullscreen(PlaybackSessionContextIdentifier);
    void didEnterFullscreen(PlaybackSessionContextIdentifier);
    void didCleanupFullscreen(PlaybackSessionContextIdentifier);
    void setVideoLayerFrame(PlaybackSessionContextIdentifier, CyberCore::FloatRect);
    void setVideoLayerGravity(PlaybackSessionContextIdentifier, CyberCore::MediaPlayerEnums::VideoGravity);
    void fullscreenModeChanged(PlaybackSessionContextIdentifier, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode);
    void fullscreenMayReturnToInline(PlaybackSessionContextIdentifier);
    void fullscreenWillReturnToInline(PlaybackSessionContextIdentifier);

    bool m_mockVideoPresentationModeEnabled { false };
    CyberCore::FloatSize m_mockPictureInPictureWindowSize { DefaultMockPictureInPictureWindowWidth, DefaultMockPictureInPictureWindowHeight };

    WebPageProxy* m_page;
    Ref<PlaybackSessionManagerProxy> m_playbackSessionManagerProxy;
    HashMap<PlaybackSessionContextIdentifier, ModelInterfaceTuple> m_contextMap;
    PlaybackSessionContextIdentifier m_controlsManagerContextId;
    HashMap<PlaybackSessionContextIdentifier, int> m_clientCounts;
};

} // namespace CyberKit

#endif // ENABLE(VIDEO_PRESENTATION_MODE)
