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

#if (PLATFORM(IOS_FAMILY) && HAVE(AVKIT)) || (PLATFORM(MAC) && ENABLE(VIDEO_PRESENTATION_MODE))

#include "MessageReceiver.h"
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

class WebPageProxy;
class PlaybackSessionManagerProxy;
class PlaybackSessionModelContext;
class VideoFullscreenManagerProxy;

class VideoFullscreenModelContext final
    : public RefCounted<VideoFullscreenModelContext>
    , public CyberCore::VideoFullscreenModel
    , public CyberCore::VideoFullscreenChangeObserver  {
public:
    static Ref<VideoFullscreenModelContext> create(VideoFullscreenManagerProxy& manager, PlaybackSessionModelContext& playbackSessionModel, uint64_t contextId)
    {
        return adoptRef(*new VideoFullscreenModelContext(manager, playbackSessionModel, contextId));
    }
    virtual ~VideoFullscreenModelContext();

    void invalidate() { m_manager = nullptr; }

    PlatformView *layerHostView() const { return m_layerHostView.get(); }
    void setLayerHostView(RetainPtr<PlatformView>&& layerHostView) { m_layerHostView = WTFMove(layerHostView); }

private:
    VideoFullscreenModelContext(VideoFullscreenManagerProxy&, PlaybackSessionModelContext&, uint64_t);

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
    void fullscreenMayReturnToInline() final;

    VideoFullscreenManagerProxy* m_manager;
    Ref<PlaybackSessionModelContext> m_playbackSessionModel;
    uint64_t m_contextId;
    RetainPtr<PlatformView *> m_layerHostView;
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

    void requestRouteSharingPolicyAndContextUID(uint64_t contextId, CompletionHandler<void(CyberCore::RouteSharingPolicy, String)>&&);

#if ENABLE(VIDEO_PRESENTATION_MODE)
    bool isPlayingVideoInEnhancedFullscreen() const;
#endif

    PlatformVideoFullscreenInterface* controlsManagerInterface();

private:
    friend class VideoFullscreenModelContext;

    explicit VideoFullscreenManagerProxy(WebPageProxy&, PlaybackSessionManagerProxy&);
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;

    typedef std::tuple<RefPtr<VideoFullscreenModelContext>, RefPtr<PlatformVideoFullscreenInterface>> ModelInterfaceTuple;
    ModelInterfaceTuple createModelAndInterface(uint64_t contextId);
    ModelInterfaceTuple& ensureModelAndInterface(uint64_t contextId);
    VideoFullscreenModelContext& ensureModel(uint64_t contextId);
    PlatformVideoFullscreenInterface& ensureInterface(uint64_t contextId);
    void addClientForContext(uint64_t contextId);
    void removeClientForContext(uint64_t contextId);

    // Messages from VideoFullscreenManager
    void setupFullscreenWithID(uint64_t contextId, uint32_t videoLayerID, const CyberCore::IntRect& initialRect, float hostingScaleFactor, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode, bool allowsPictureInPicture, bool standby);
    void setInlineRect(uint64_t contextId, const CyberCore::IntRect& inlineRect, bool visible);
    void setHasVideoContentLayer(uint64_t contextId, bool value);
    void setHasVideo(uint64_t contextId, bool);
    void setVideoDimensions(uint64_t contextId, const CyberCore::FloatSize&);
    void enterFullscreen(uint64_t contextId);
    void exitFullscreen(uint64_t contextId, CyberCore::IntRect finalRect);
    void cleanupFullscreen(uint64_t contextId);
    void preparedToReturnToInline(uint64_t contextId, bool visible, CyberCore::IntRect inlineRect);
    void preparedToExitFullscreen(uint64_t contextId);
#if PLATFORM(MAC) && ENABLE(VIDEO_PRESENTATION_MODE)
    void exitFullscreenWithoutAnimationToMode(uint64_t contextId, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode);
#endif

    // Messages to VideoFullscreenManager
    void requestFullscreenMode(uint64_t contextId, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode, bool finishedWithMedia = false);
    void requestUpdateInlineRect(uint64_t contextId);
    void requestVideoContentLayer(uint64_t contextId);
    void returnVideoContentLayer(uint64_t contextId);
    void didSetupFullscreen(uint64_t contextId);
    void willExitFullscreen(uint64_t contextId);
    void didExitFullscreen(uint64_t contextId);
    void didEnterFullscreen(uint64_t contextId);
    void didCleanupFullscreen(uint64_t contextId);
    void setVideoLayerFrame(uint64_t contextId, CyberCore::FloatRect);
    void setVideoLayerGravity(uint64_t contextId, CyberCore::MediaPlayerEnums::VideoGravity);
    void fullscreenModeChanged(uint64_t contextId, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode);
    void fullscreenMayReturnToInline(uint64_t contextId);

    WebPageProxy* m_page;
    Ref<PlaybackSessionManagerProxy> m_playbackSessionManagerProxy;
    HashMap<uint64_t, ModelInterfaceTuple> m_contextMap;
    uint64_t m_controlsManagerContextId { 0 };
    HashMap<uint64_t, int> m_clientCounts;
};
    
} // namespace CyberKit

#endif // PLATFORM(IOS_FAMILY) || (PLATFORM(MAC) && ENABLE(VIDEO_PRESENTATION_MODE))

