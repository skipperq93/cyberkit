/*
 * Copyright (C) 2012-2018 Apple Inc. All rights reserved.
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

#include "CallbackID.h"
#include "DrawingArea.h"
#include "GraphicsLayerCARemote.h"
#include "RemoteLayerTreeTransaction.h"
#include <CyberCore/AnimationFrameRate.h>
#include <CyberCore/GraphicsLayerClient.h>
#include <CyberCore/Timer.h>
#include <atomic>
#include <dispatch/dispatch.h>
#include <wtf/HashMap.h>
#include <wtf/OSObjectPtr.h>
#include <wtf/WeakPtr.h>

namespace CyberCore {
class PlatformCALayer;
class ThreadSafeImageBufferFlusher;
}

namespace CyberKit {

class RemoteLayerTreeContext;
class RemoteLayerTreeDisplayRefreshMonitor;

class RemoteLayerTreeDrawingArea : public DrawingArea, public CyberCore::GraphicsLayerClient {
    friend class RemoteLayerTreeDisplayRefreshMonitor;
public:
    RemoteLayerTreeDrawingArea(WebPage&, const WebPageCreationParameters&);
    virtual ~RemoteLayerTreeDrawingArea();

    TransactionID nextTransactionID() const { return m_currentTransactionID.next(); }
    TransactionID lastCommittedTransactionID() const { return m_currentTransactionID; }

protected:
    void updateRendering();

private:
    // DrawingArea
    void setNeedsDisplay() override;
    void setNeedsDisplayInRect(const CyberCore::IntRect&) override;
    void scroll(const CyberCore::IntRect& scrollRect, const CyberCore::IntSize& scrollDelta) override;
    void updateGeometry(const CyberCore::IntSize& viewSize, bool flushSynchronously, const WTF::MachSendRight& fencePort, CompletionHandler<void()>&&) override;
    void adoptDisplayRefreshMonitorsFromDrawingArea(DrawingArea&) override;

    CyberCore::GraphicsLayerFactory* graphicsLayerFactory() override;
    void setRootCompositingLayer(CyberCore::GraphicsLayer*) override;
    void triggerRenderingUpdate() override;
    void attachViewOverlayGraphicsLayer(CyberCore::GraphicsLayer*) override;

    void dispatchAfterEnsuringDrawing(IPC::AsyncReplyID) final;
    virtual void willCommitLayerTree(RemoteLayerTreeTransaction&) { };

    RefPtr<CyberCore::DisplayRefreshMonitor> createDisplayRefreshMonitor(CyberCore::PlatformDisplayID) final;
    void willDestroyDisplayRefreshMonitor(CyberCore::DisplayRefreshMonitor*);
    void setPreferredFramesPerSecond(CyberCore::FramesPerSecond);

    bool shouldUseTiledBackingForFrameView(const CyberCore::LocalFrameView&) const override;

    void updatePreferences(const WebPreferencesStore&) override;

    bool supportsAsyncScrolling() const override { return true; }
    bool usesDelegatedPageScaling() const override { return true; }
    CyberCore::DelegatedScrollingMode delegatedScrollingMode() const override;

    void setLayerTreeStateIsFrozen(bool) override;
    bool layerTreeStateIsFrozen() const override { return m_isRenderingSuspended; }

    void forceRepaint() override;
    void forceRepaintAsync(WebPage&, CompletionHandler<void()>&&) override;

    void setViewExposedRect(std::optional<CyberCore::FloatRect>) override;
    std::optional<CyberCore::FloatRect> viewExposedRect() const override { return m_viewExposedRect; }

    void acceleratedAnimationDidStart(CyberCore::GraphicsLayer::PlatformLayerID, const String& key, MonotonicTime startTime) override;
    void acceleratedAnimationDidEnd(CyberCore::GraphicsLayer::PlatformLayerID, const String& key) override;

    CyberCore::FloatRect exposedContentRect() const override;
    void setExposedContentRect(const CyberCore::FloatRect&) override;

    void displayDidRefresh() override;

    void setDeviceScaleFactor(float) override;

    void mainFrameContentSizeChanged(const CyberCore::IntSize&) override;

    void activityStateDidChange(OptionSet<CyberCore::ActivityState::Flag> changed, ActivityStateChangeID, CompletionHandler<void()>&&) override;

    bool addMilestonesToDispatch(OptionSet<CyberCore::LayoutMilestone>) override;

    void updateRootLayers();

    void addCommitHandlers();
    void startRenderingUpdateTimer();
    void didCompleteRenderingUpdateDisplay() override;

    TransactionID takeNextTransactionID() { return m_currentTransactionID.increment(); }

    void tryMarkLayersVolatile(CompletionHandler<void(bool succeeded)>&&) final;

    void adoptLayersFromDrawingArea(DrawingArea&) final;

    class BackingStoreFlusher : public ThreadSafeRefCounted<BackingStoreFlusher> {
    public:
        static Ref<BackingStoreFlusher> create(IPC::Connection*, UniqueRef<IPC::Encoder>&&, Vector<std::unique_ptr<CyberCore::ThreadSafeImageBufferFlusher>>);

        void flush();
        bool hasFlushed() const { return m_hasFlushed; }

    private:
        BackingStoreFlusher(IPC::Connection*, UniqueRef<IPC::Encoder>&&, Vector<std::unique_ptr<CyberCore::ThreadSafeImageBufferFlusher>>);

        RefPtr<IPC::Connection> m_connection;
        std::unique_ptr<IPC::Encoder> m_commitEncoder;
        Vector<std::unique_ptr<CyberCore::ThreadSafeImageBufferFlusher>> m_flushers;

        std::atomic<bool> m_hasFlushed;
    };

    std::unique_ptr<RemoteLayerTreeContext> m_remoteLayerTreeContext;
    Ref<CyberCore::GraphicsLayer> m_rootLayer;

    std::optional<CyberCore::FloatRect> m_viewExposedRect;

    CyberCore::Timer m_updateRenderingTimer;
    bool m_isRenderingSuspended { false };
    bool m_hasDeferredRenderingUpdate { false };
    bool m_nextRenderingUpdateRequiresSynchronousImageDecoding { false };
    bool m_inUpdateRendering { false };

    bool m_waitingForBackingStoreSwap { false };
    bool m_deferredRenderingUpdateWhileWaitingForBackingStoreSwap { false };

    OSObjectPtr<dispatch_queue_t> m_commitQueue;
    RefPtr<BackingStoreFlusher> m_pendingBackingStoreFlusher;

    HashSet<RemoteLayerTreeDisplayRefreshMonitor*> m_displayRefreshMonitors;
    HashSet<RemoteLayerTreeDisplayRefreshMonitor*>* m_displayRefreshMonitorsToNotify { nullptr };

    TransactionID m_currentTransactionID;
    Vector<IPC::AsyncReplyID> m_pendingCallbackIDs;
    ActivityStateChangeID m_activityStateChangeID { ActivityStateChangeAsynchronous };

    OptionSet<CyberCore::LayoutMilestone> m_pendingNewlyReachedPaintingMilestones;

    RefPtr<CyberCore::GraphicsLayer> m_contentLayer;
    RefPtr<CyberCore::GraphicsLayer> m_viewOverlayRootLayer;
};

inline bool RemoteLayerTreeDrawingArea::addMilestonesToDispatch(OptionSet<CyberCore::LayoutMilestone> paintMilestones)
{
    m_pendingNewlyReachedPaintingMilestones.add(paintMilestones);
    return true;
}

} // namespace CyberKit

SPECIALIZE_TYPE_TRAITS_DRAWING_AREA(RemoteLayerTreeDrawingArea, DrawingAreaType::RemoteLayerTree)
