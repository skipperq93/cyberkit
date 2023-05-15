/*
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2013 Company 100, Inc. All rights reserved.
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

#if USE(COORDINATED_GRAPHICS)

#include "WebPage.h"
#include <CyberCore/CoordinatedGraphicsLayer.h>
#include <CyberCore/FloatPoint.h>
#include <CyberCore/GraphicsLayerClient.h>
#include <CyberCore/GraphicsLayerFactory.h>
#include <CyberCore/IntRect.h>
#include <CyberCore/NicosiaBuffer.h>
#include <CyberCore/NicosiaPlatformLayer.h>
#include <CyberCore/NicosiaScene.h>
#include <CyberCore/NicosiaSceneIntegration.h>

namespace Nicosia {
class ImageBackingStore;
class PaintingEngine;
class SceneIntegration;
}

namespace CyberCore {
class GraphicsContext;
class GraphicsLayer;
class Image;
}

namespace CyberKit {

class CompositingCoordinator final : public CyberCore::GraphicsLayerClient
    , public CyberCore::CoordinatedGraphicsLayerClient
    , public CyberCore::GraphicsLayerFactory
    , public Nicosia::SceneIntegration::Client {
    WTF_MAKE_NONCOPYABLE(CompositingCoordinator);
public:
    class Client {
    public:
        virtual void didFlushRootLayer(const CyberCore::FloatRect& visibleContentRect) = 0;
        virtual void notifyFlushRequired() = 0;
        virtual void commitSceneState(const RefPtr<Nicosia::Scene>&) = 0;
        virtual void updateScene() = 0;
    };

    CompositingCoordinator(WebPage&, CompositingCoordinator::Client&);
    virtual ~CompositingCoordinator();

    void invalidate();

    void setRootCompositingLayer(CyberCore::GraphicsLayer*);
    void setViewOverlayRootLayer(CyberCore::GraphicsLayer*);
    void sizeDidChange(const CyberCore::IntSize&);
    void deviceOrPageScaleFactorChanged();

    void setVisibleContentsRect(const CyberCore::FloatRect&);
    void renderNextFrame();

    CyberCore::GraphicsLayer* rootLayer() const { return m_rootLayer.get(); }
    CyberCore::GraphicsLayer* rootCompositingLayer() const { return m_rootCompositingLayer; }

    void forceFrameSync() { m_shouldSyncFrame = true; }

    bool flushPendingLayerChanges(OptionSet<CyberCore::FinalizeRenderingUpdateFlags>);
    void syncDisplayState();

    double nextAnimationServiceTime() const;

private:
    // GraphicsLayerClient
    void notifyFlushRequired(const CyberCore::GraphicsLayer*) override;
    float deviceScaleFactor() const override;
    float pageScaleFactor() const override;

    // CoordinatedGraphicsLayerClient
    bool isFlushingLayerChanges() const override { return m_isFlushingLayerChanges; }
    CyberCore::FloatRect visibleContentsRect() const override;
    void detachLayer(CyberCore::CoordinatedGraphicsLayer*) override;
    void attachLayer(CyberCore::CoordinatedGraphicsLayer*) override;
    Nicosia::PaintingEngine& paintingEngine() override;
    RefPtr<Nicosia::ImageBackingStore> imageBackingStore(uint64_t, Function<RefPtr<Nicosia::Buffer>()>) override;
    void syncLayerState() override;

    // GraphicsLayerFactory
    Ref<CyberCore::GraphicsLayer> createGraphicsLayer(CyberCore::GraphicsLayer::Type, CyberCore::GraphicsLayerClient&) override;

    // Nicosia::SceneIntegration::Client
    void requestUpdate() override;

    void initializeRootCompositingLayerIfNeeded();

    void purgeBackingStores();

    double timestamp() const;

    WebPage& m_page;
    CompositingCoordinator::Client& m_client;

    RefPtr<CyberCore::GraphicsLayer> m_rootLayer;
    CyberCore::GraphicsLayer* m_rootCompositingLayer { nullptr };
    CyberCore::GraphicsLayer* m_overlayCompositingLayer { nullptr };

    struct {
        RefPtr<Nicosia::Scene> scene;
        RefPtr<Nicosia::SceneIntegration> sceneIntegration;
        Nicosia::Scene::State state;
    } m_nicosia;

    HashMap<Nicosia::PlatformLayer::LayerID, CyberCore::CoordinatedGraphicsLayer*> m_registeredLayers;

    std::unique_ptr<Nicosia::PaintingEngine> m_paintingEngine;
    HashMap<uint64_t, Ref<Nicosia::ImageBackingStore>> m_imageBackingStores;

    // We don't send the messages related to releasing resources to renderer during purging, because renderer already had removed all resources.
    bool m_isPurging { false };
    bool m_isFlushingLayerChanges { false };
    bool m_shouldSyncFrame { false };
    bool m_didInitializeRootCompositingLayer { false };

    CyberCore::FloatRect m_visibleContentsRect;

    double m_lastAnimationServiceTime { 0 };
};

}

#endif // namespace CyberKit
