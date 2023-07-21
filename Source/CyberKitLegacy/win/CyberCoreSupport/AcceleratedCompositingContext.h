/*
 * Copyright (C) 2014 Apple, Inc.
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

#ifndef AcceleratedCompositingContext_h
#define AcceleratedCompositingContext_h

#if USE(TEXTURE_MAPPER_GL)

#include <CyberCore/FloatRect.h>
#include <CyberCore/GLContext.h>
#include <CyberCore/GraphicsLayer.h>
#include <CyberCore/GraphicsLayerClient.h>
#include <CyberCore/IntRect.h>
#include <CyberCore/IntSize.h>
#include <CyberCore/TextureMapperFPSCounter.h>
#include <CyberCore/Timer.h>

class WebView;

class AcceleratedCompositingContext : public CyberCore::GraphicsLayerClient {
    WTF_MAKE_NONCOPYABLE(AcceleratedCompositingContext);
public:
    explicit AcceleratedCompositingContext(WebView&);
    virtual ~AcceleratedCompositingContext();

    void setRootCompositingLayer(CyberCore::GraphicsLayer*);
    void setNonCompositedContentsNeedDisplay(const CyberCore::IntRect&);
    void scheduleLayerFlush();
    void resizeRootLayer(const CyberCore::IntSize&);
    bool enabled();

    // GraphicsLayerClient
    void paintContents(const CyberCore::GraphicsLayer*, CyberCore::GraphicsContext&, OptionSet<CyberCore::GraphicsLayerPaintingPhase>, const CyberCore::FloatRect& rectToPaint, CyberCore::GraphicsLayerPaintBehavior) override;
    float deviceScaleFactor() const override;

    void initialize();

    enum CompositePurpose { ForResize, NotForResize };
    void compositeLayersToContext(CompositePurpose = NotForResize);

    void flushAndRenderLayers();
    bool flushPendingLayerChanges();
    bool flushPendingLayerChangesSoon();
    void scrollNonCompositedContents(const CyberCore::IntRect& scrollRect, const CyberCore::IntSize& scrollOffset);

    static bool acceleratedCompositingAvailable();

private:
    WebView& m_webView;
    std::unique_ptr<CyberCore::GLContext> m_context;
    HWND m_window;
    RefPtr<CyberCore::GraphicsLayer> m_rootLayer;
    RefPtr<CyberCore::GraphicsLayer> m_nonCompositedContentLayer;
    std::unique_ptr<CyberCore::TextureMapper> m_textureMapper;
    CyberCore::TextureMapperFPSCounter m_fpsCounter;

    class LayerFlushTimer : public CyberCore::TimerBase {
    public:
        LayerFlushTimer(AcceleratedCompositingContext& context)
            : m_context(context)
        {
        }

    private:
        virtual void fired()
        {
            m_context.layerFlushTimerFired();
        }

        AcceleratedCompositingContext& m_context;
    } m_layerFlushTimer;

    void layerFlushTimerFired();
    void stopAnyPendingLayerFlush();
    bool prepareForRendering();
    bool startedAnimation(CyberCore::GraphicsLayer*);
    void applyDeviceScaleFactor();
};

#endif // TEXTURE_MAPPER_GL

#endif // AcceleratedCompositingContext_h
