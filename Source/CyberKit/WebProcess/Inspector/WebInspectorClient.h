/*
 * Copyright (C) 2010, 2015 Apple Inc. All rights reserved.
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

#include <CyberCore/InspectorClient.h>
#include <CyberCore/PageOverlay.h>
#include <wtf/HashSet.h>

namespace CyberCore {
class GraphicsContext;
class GraphicsLayer;
class IntRect;
class PageOverlay;
}

namespace CyberKit {

class WebPage;
class RepaintIndicatorLayerClient;

class WebInspectorClient : public CyberCore::InspectorClient, private CyberCore::PageOverlay::Client {
    WTF_MAKE_FAST_ALLOCATED;
friend class RepaintIndicatorLayerClient;
public:
    WebInspectorClient(WebPage*);
    virtual ~WebInspectorClient();

private:
    // CyberCore::InspectorClient
    void inspectedPageDestroyed() override;
    void frontendCountChanged(unsigned) override;

    Inspector::FrontendChannel* openLocalFrontend(CyberCore::InspectorController*) override;
    void bringFrontendToFront() override;
    void didResizeMainFrame(CyberCore::Frame*) override;

    void highlight() override;
    void hideHighlight() override;

#if PLATFORM(IOS_FAMILY)
    void showInspectorIndication() override;
    void hideInspectorIndication() override;

    void didSetSearchingForNode(bool) override;
#endif

    void elementSelectionChanged(bool) override;
    void timelineRecordingChanged(bool) override;

    bool overridesShowPaintRects() const override { return true; }
    void showPaintRect(const CyberCore::FloatRect&) override;

    void setDeveloperPreferenceOverride(CyberCore::InspectorClient::DeveloperPreference, Optional<bool>) final;

    // PageOverlay::Client
    void willMoveToPage(CyberCore::PageOverlay&, CyberCore::Page*) override;
    void didMoveToPage(CyberCore::PageOverlay&, CyberCore::Page*) override;
    void drawRect(CyberCore::PageOverlay&, CyberCore::GraphicsContext&, const CyberCore::IntRect&) override;
    bool mouseEvent(CyberCore::PageOverlay&, const CyberCore::PlatformMouseEvent&) override;

    void animationEndedForLayer(const CyberCore::GraphicsLayer*);

    WebPage* m_page;
    CyberCore::PageOverlay* m_highlightOverlay;
    
    RefPtr<CyberCore::PageOverlay> m_paintRectOverlay;
    std::unique_ptr<RepaintIndicatorLayerClient> m_paintIndicatorLayerClient;
    HashSet<Ref<CyberCore::GraphicsLayer>> m_paintRectLayers;
};

} // namespace CyberKit
