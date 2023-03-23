/*
 * Copyright (C) 2014 Apple, Inc.
 * Copyright (C) 2020 Sony Interactive Entertainment Inc.
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

#if USE(GRAPHICS_LAYER_TEXTURE_MAPPER)

#include "CallbackID.h"
#include "LayerTreeContext.h"
#include <CyberCore/DisplayRefreshMonitor.h>
#include <CyberCore/GLContext.h>
#include <CyberCore/GraphicsLayerClient.h>
#include <CyberCore/PlatformScreen.h>
#include <CyberCore/TextureMapperFPSCounter.h>
#include <CyberCore/Timer.h>
#include <wtf/Forward.h>

namespace CyberCore {
class GraphicsLayer;
class GraphicsLayerFactory;
class IntRect;
class IntSize;
class Page;
struct ViewportAttributes;
}

namespace CyberKit {

class WebPage;

class LayerTreeHost : public CyberCore::GraphicsLayerClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    explicit LayerTreeHost(WebPage&);
    ~LayerTreeHost();

    const LayerTreeContext& layerTreeContext() const { return m_layerTreeContext; }
    void setLayerFlushSchedulingEnabled(bool);
    void setShouldNotifyAfterNextScheduledLayerFlush(bool);
    void scheduleLayerFlush();
    void cancelPendingLayerFlush();
    void setRootCompositingLayer(CyberCore::GraphicsLayer*);
    void setViewOverlayRootLayer(CyberCore::GraphicsLayer*);
    void setNonCompositedContentsNeedDisplay(const CyberCore::IntRect&);
    void scrollNonCompositedContents(const CyberCore::IntRect&);
    void forceRepaint();
    void forceRepaintAsync(CompletionHandler<void()>&&);
    void sizeDidChange(const CyberCore::IntSize& newSize);
    void pauseRendering();
    void resumeRendering();
    CyberCore::GraphicsLayerFactory* graphicsLayerFactory();
    void contentsSizeChanged(const CyberCore::IntSize&);
    void didChangeViewportAttributes(CyberCore::ViewportAttributes&&);
    void setIsDiscardable(bool);
    void deviceOrPageScaleFactorChanged();
    RefPtr<CyberCore::DisplayRefreshMonitor> createDisplayRefreshMonitor(CyberCore::PlatformDisplayID);
    CyberCore::PlatformDisplayID displayID() const { return m_displayID; }

private:
    // GraphicsLayerClient
    void paintContents(const CyberCore::GraphicsLayer*, CyberCore::GraphicsContext&, const CyberCore::FloatRect& rectToPaint, CyberCore::GraphicsLayerPaintBehavior) override;
    float deviceScaleFactor() const override;

    void initialize();
    GLNativeWindowType window();
    bool enabled();
    void compositeLayersToContext();
    void flushAndRenderLayers();
    bool flushPendingLayerChanges();
    void scrollNonCompositedContents(const CyberCore::IntRect& scrollRect, const CyberCore::IntSize& scrollOffset);
    void layerFlushTimerFired();
    bool prepareForRendering();
    void applyDeviceScaleFactor();

    WebPage& m_webPage;
    std::unique_ptr<CyberCore::GLContext> m_context;
    LayerTreeContext m_layerTreeContext;
    CyberCore::PlatformDisplayID m_displayID;
    RefPtr<CyberCore::GraphicsLayer> m_rootLayer;
    CyberCore::GraphicsLayer* m_rootCompositingLayer { nullptr };
    CyberCore::GraphicsLayer* m_overlayCompositingLayer { nullptr };
    std::unique_ptr<CyberCore::TextureMapper> m_textureMapper;
    CyberCore::TextureMapperFPSCounter m_fpsCounter;
    CyberCore::Timer m_layerFlushTimer;
    bool m_notifyAfterScheduledLayerFlush { false };
    bool m_isSuspended { false };
};

} // namespace CyberKit

#endif // USE(GRAPHICS_LAYER_TEXTURE_MAPPER)
