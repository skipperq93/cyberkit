/*
 * Copyright (C) 2011-2018 Apple Inc. All rights reserved.
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

#if PLATFORM(MAC)

#include "CallbackID.h"
#include "DrawingArea.h"
#include "LayerTreeContext.h"
#include <CyberCore/FloatRect.h>
#include <CyberCore/TransformationMatrix.h>
#include <wtf/HashMap.h>
#include <wtf/RetainPtr.h>
#include <wtf/RunLoop.h>

OBJC_CLASS CALayer;

namespace CyberCore {
class FrameView;
class PlatformCALayer;
class RunLoopObserver;
class TiledBacking;
}

namespace CyberKit {

class LayerHostingContext;

class TiledCoreAnimationDrawingArea : public DrawingArea, public CanMakeWeakPtr<TiledCoreAnimationDrawingArea> {
public:
    TiledCoreAnimationDrawingArea(WebPage&, const WebPageCreationParameters&);
    virtual ~TiledCoreAnimationDrawingArea();

private:
    // DrawingArea
    void setNeedsDisplay() override;
    void setNeedsDisplayInRect(const CyberCore::IntRect&) override;
    void scroll(const CyberCore::IntRect& scrollRect, const CyberCore::IntSize& scrollDelta) override { }

    void forceRepaint() override;
    bool forceRepaintAsync(CallbackID) override;
    void setLayerTreeStateIsFrozen(bool) override;
    bool layerTreeStateIsFrozen() const override;
    void setRootCompositingLayer(CyberCore::GraphicsLayer*) override;
    void scheduleRenderingUpdate() override;
    void scheduleImmediateRenderingUpdate() override;

    void updatePreferences(const WebPreferencesStore&) override;
    void mainFrameContentSizeChanged(const CyberCore::IntSize&) override;

    void setViewExposedRect(Optional<CyberCore::FloatRect>) override;
    Optional<CyberCore::FloatRect> viewExposedRect() const override { return m_viewExposedRect; }

    CyberCore::FloatRect exposedContentRect() const override;
    void setExposedContentRect(const CyberCore::FloatRect&) override;

    bool supportsAsyncScrolling() const override { return true; }

    void dispatchAfterEnsuringUpdatedScrollPosition(WTF::Function<void ()>&&) override;

    bool shouldUseTiledBackingForFrameView(const CyberCore::FrameView&) const override;

    void activityStateDidChange(OptionSet<CyberCore::ActivityState::Flag> changed, ActivityStateChangeID, const Vector<CallbackID>&) override;
    void didUpdateActivityStateTimerFired();

    void attachViewOverlayGraphicsLayer(CyberCore::GraphicsLayer*) override;

    bool addMilestonesToDispatch(OptionSet<CyberCore::LayoutMilestone> paintMilestones) override;

    void addCommitHandlers();
    enum class UpdateRenderingType { Normal, TransientZoom };
    void updateRendering(UpdateRenderingType = UpdateRenderingType::Normal);

    // Message handlers.
    void updateGeometry(const CyberCore::IntSize& viewSize, bool flushSynchronously, const WTF::MachSendRight& fencePort) override;
    void setDeviceScaleFactor(float) override;
    void suspendPainting();
    void resumePainting();
    void setLayerHostingMode(LayerHostingMode) override;
    void setColorSpace(const ColorSpaceData&) override;
    void addFence(const WTF::MachSendRight&) override;

    void addTransactionCallbackID(CallbackID) override;
    void setShouldScaleViewToFitDocument(bool) override;

    void sendEnterAcceleratedCompositingModeIfNeeded() override;
    void sendDidFirstLayerFlushIfNeeded();

    void adjustTransientZoom(double scale, CyberCore::FloatPoint origin) override;
    void commitTransientZoom(double scale, CyberCore::FloatPoint origin) override;
    void applyTransientZoomToPage(double scale, CyberCore::FloatPoint origin);
    CyberCore::PlatformCALayer* layerForTransientZoom() const;
    CyberCore::PlatformCALayer* shadowLayerForTransientZoom() const;

    void applyTransientZoomToLayers(double scale, CyberCore::FloatPoint origin);

    void updateLayerHostingContext();

    void setRootCompositingLayer(CALayer *);
    void updateRootLayers();

    CyberCore::TiledBacking* mainFrameTiledBacking() const;
    void updateDebugInfoLayer(bool showLayer);

    void scaleViewToFitDocumentIfNeeded();

    void sendPendingNewlyReachedPaintingMilestones();

    void updateRenderingRunLoopCallback();
    void invalidateRenderingUpdateRunLoopObserver();
    void scheduleRenderingUpdateRunLoopObserver();

    void startRenderThrottlingTimer();
    void renderThrottlingTimerFired();

    std::unique_ptr<LayerHostingContext> m_layerHostingContext;

    RetainPtr<CALayer> m_hostingLayer;
    RetainPtr<CALayer> m_rootLayer;
    RetainPtr<CALayer> m_debugInfoLayer;
    RetainPtr<CALayer> m_pendingRootLayer;

    Optional<CyberCore::FloatRect> m_viewExposedRect;

    CyberCore::IntSize m_lastViewSizeForScaleToFit;
    CyberCore::IntSize m_lastDocumentSizeForScaleToFit;

    double m_transientZoomScale { 1 };
    CyberCore::FloatPoint m_transientZoomOrigin;

    RunLoop::Timer<TiledCoreAnimationDrawingArea> m_sendDidUpdateActivityStateTimer;
    Vector<CallbackID> m_nextActivityStateChangeCallbackIDs;
    ActivityStateChangeID m_activityStateChangeID { ActivityStateChangeAsynchronous };

    RefPtr<CyberCore::GraphicsLayer> m_viewOverlayRootLayer;

    OptionSet<CyberCore::LayoutMilestone> m_pendingNewlyReachedPaintingMilestones;
    Vector<CallbackID> m_pendingCallbackIDs;

    std::unique_ptr<CyberCore::RunLoopObserver> m_renderUpdateRunLoopObserver;

    bool m_isPaintingSuspended { false };
    bool m_inUpdateGeometry { false };
    bool m_layerTreeStateIsFrozen { false };
    bool m_shouldScaleViewToFitDocument { false };
    bool m_isScalingViewToFitDocument { false };
    bool m_needsSendEnterAcceleratedCompositingMode { true };
    bool m_needsSendDidFirstLayerFlush { true };
};

inline bool TiledCoreAnimationDrawingArea::addMilestonesToDispatch(OptionSet<CyberCore::LayoutMilestone> paintMilestones)
{
    m_pendingNewlyReachedPaintingMilestones.add(paintMilestones);
    return true;
}

} // namespace CyberKit

SPECIALIZE_TYPE_TRAITS_DRAWING_AREA(TiledCoreAnimationDrawingArea, DrawingAreaTypeTiledCoreAnimation)

#endif // PLATFORM(MAC)
