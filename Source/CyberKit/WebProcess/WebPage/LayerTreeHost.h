/*
 * Copyright (C) 2011, 2012 Apple Inc. All rights reserved.
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

#ifndef LayerTreeHost_h
#define LayerTreeHost_h

#if USE(COORDINATED_GRAPHICS) || USE(TEXTURE_MAPPER)

#include "CallbackID.h"
#include "LayerTreeContext.h"
#include <CyberCore/DisplayRefreshMonitor.h>
#include <CyberCore/PlatformScreen.h>
#include <wtf/Forward.h>
#include <wtf/RefCounted.h>
#include <wtf/RefPtr.h>

namespace IPC {
class Connection;
}

namespace CyberCore {
class IntRect;
class IntSize;
class GraphicsLayer;
class GraphicsLayerFactory;
#if USE(COORDINATED_GRAPHICS_THREADED)
struct ViewportAttributes;
#endif
}

namespace CyberKit {

class WebPage;

class LayerTreeHost : public RefCounted<LayerTreeHost> {
public:
    static RefPtr<LayerTreeHost> create(WebPage&);
    virtual ~LayerTreeHost();

    const LayerTreeContext& layerTreeContext() const { return m_layerTreeContext; }
    void setLayerFlushSchedulingEnabled(bool);
    void setShouldNotifyAfterNextScheduledLayerFlush(bool notifyAfterScheduledLayerFlush) { m_notifyAfterScheduledLayerFlush = notifyAfterScheduledLayerFlush; }

    virtual void scheduleLayerFlush() = 0;
    virtual void cancelPendingLayerFlush() = 0;
    virtual void setRootCompositingLayer(CyberCore::GraphicsLayer*) = 0;
    virtual void invalidate();

    virtual void setNonCompositedContentsNeedDisplay() { };
    virtual void setNonCompositedContentsNeedDisplayInRect(const CyberCore::IntRect&) { };
    virtual void scrollNonCompositedContents(const CyberCore::IntRect&) { };
    virtual void forceRepaint() = 0;
    virtual bool forceRepaintAsync(CallbackID) { return false; }
    virtual void sizeDidChange(const CyberCore::IntSize& newSize) = 0;
    virtual void pageBackgroundTransparencyChanged() = 0;

    virtual void pauseRendering();
    virtual void resumeRendering();

    virtual CyberCore::GraphicsLayerFactory* graphicsLayerFactory() { return nullptr; }

#if USE(COORDINATED_GRAPHICS_THREADED)
    virtual void contentsSizeChanged(const CyberCore::IntSize&) { };
    virtual void didChangeViewportAttributes(CyberCore::ViewportAttributes&&) { };
#endif

#if USE(COORDINATED_GRAPHICS)
    virtual void scheduleAnimation() = 0;
    virtual void setIsDiscardable(bool) { };
#endif

#if USE(TEXTURE_MAPPER_GL) && PLATFORM(GTK)
    virtual void setNativeSurfaceHandleForCompositing(uint64_t) { };
#endif

#if USE(COORDINATED_GRAPHICS) || USE(TEXTURE_MAPPER)
    virtual void deviceOrPageScaleFactorChanged() = 0;
#endif

#if USE(REQUEST_ANIMATION_FRAME_DISPLAY_MONITOR)
    virtual RefPtr<CyberCore::DisplayRefreshMonitor> createDisplayRefreshMonitor(CyberCore::PlatformDisplayID) { return nullptr; }
#endif

    virtual void setViewOverlayRootLayer(CyberCore::GraphicsLayer* viewOverlayRootLayer) { m_viewOverlayRootLayer = viewOverlayRootLayer; }

protected:
    explicit LayerTreeHost(WebPage&);

    WebPage& m_webPage;
    LayerTreeContext m_layerTreeContext;
    bool m_layerFlushSchedulingEnabled { true };
    bool m_notifyAfterScheduledLayerFlush { false };
    bool m_isSuspended { false };
    bool m_isValid { true };
    CyberCore::GraphicsLayer* m_viewOverlayRootLayer { nullptr };
};

} // namespace CyberKit

#endif // USE(COORDINATED_GRAPHICS) || USE(TEXTURE_MAPPER)

#endif // LayerTreeHost_h
