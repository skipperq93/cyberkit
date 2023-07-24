/*
 * Copyright (C) 2010-2018 Apple Inc. All rights reserved.
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
#include "DrawingAreaInfo.h"
#include "LayerTreeContext.h"
#include "MessageReceiver.h"
#include "WebPage.h"
#include <CyberCore/ActivityState.h>
#include <CyberCore/FloatRect.h>
#include <CyberCore/IntRect.h>
#include <CyberCore/LayoutMilestone.h>
#include <CyberCore/PlatformScreen.h>
#include <wtf/Forward.h>
#include <wtf/Noncopyable.h>
#include <wtf/TypeCasts.h>

namespace WTF {
class MachSendRight;
}

namespace IPC {
class Connection;
class Decoder;
}

namespace CyberCore {
class DisplayRefreshMonitor;
class Frame;
class FrameView;
class GraphicsLayer;
class GraphicsLayerFactory;
struct ViewportAttributes;
}

namespace CyberKit {

struct ColorSpaceData;
class LayerTreeHost;
struct WebPageCreationParameters;
struct WebPreferencesStore;

class DrawingArea : public IPC::MessageReceiver {
    WTF_MAKE_FAST_ALLOCATED;
    WTF_MAKE_NONCOPYABLE(DrawingArea);

public:
    static std::unique_ptr<DrawingArea> create(WebPage&, const WebPageCreationParameters&);
    virtual ~DrawingArea();
    
    DrawingAreaType type() const { return m_type; }
    DrawingAreaIdentifier identifier() const { return m_identifier; }

    virtual void setNeedsDisplay() = 0;
    virtual void setNeedsDisplayInRect(const CyberCore::IntRect&) = 0;
    virtual void scroll(const CyberCore::IntRect& scrollRect, const CyberCore::IntSize& scrollDelta) = 0;

    virtual void sendEnterAcceleratedCompositingModeIfNeeded() { }

    // FIXME: These should be pure virtual.
    virtual void forceRepaint() { }
    virtual bool forceRepaintAsync(CallbackID) { return false; }
    virtual void setLayerTreeStateIsFrozen(bool) { }
    virtual bool layerTreeStateIsFrozen() const { return false; }

    virtual void updatePreferences(const WebPreferencesStore&) { }
    virtual void enablePainting() { }
    virtual void mainFrameContentSizeChanged(const CyberCore::IntSize&) { }

#if PLATFORM(COCOA)
    virtual void setViewExposedRect(Optional<CyberCore::FloatRect>) = 0;
    virtual Optional<CyberCore::FloatRect> viewExposedRect() const = 0;

    virtual void acceleratedAnimationDidStart(uint64_t /*layerID*/, const String& /*key*/, MonotonicTime /*startTime*/) { }
    virtual void acceleratedAnimationDidEnd(uint64_t /*layerID*/, const String& /*key*/) { }
    virtual void addFence(const WTF::MachSendRight&) { }

    virtual CyberCore::FloatRect exposedContentRect() const = 0;
    virtual void setExposedContentRect(const CyberCore::FloatRect&) = 0;
#endif

    virtual void mainFrameScrollabilityChanged(bool) { }

    virtual bool supportsAsyncScrolling() const { return false; }
    virtual bool usesDelegatedScrolling() const { return false; }
    virtual bool usesDelegatedPageScaling() const { return false; }

    virtual bool shouldUseTiledBackingForFrameView(const CyberCore::FrameView&) const { return false; }

    virtual CyberCore::GraphicsLayerFactory* graphicsLayerFactory() { return nullptr; }
    virtual void setRootCompositingLayer(CyberCore::GraphicsLayer*) = 0;
    virtual void scheduleRenderingUpdate() = 0;
    virtual void scheduleImmediateRenderingUpdate() = 0;

    virtual RefPtr<CyberCore::DisplayRefreshMonitor> createDisplayRefreshMonitor(CyberCore::PlatformDisplayID);

    virtual void dispatchAfterEnsuringUpdatedScrollPosition(WTF::Function<void ()>&&);

    virtual void activityStateDidChange(OptionSet<CyberCore::ActivityState::Flag>, ActivityStateChangeID, const Vector<CallbackID>&) { }
    virtual void setLayerHostingMode(LayerHostingMode) { }

    virtual bool markLayersVolatileImmediatelyIfPossible() { return true; }

    virtual void attachViewOverlayGraphicsLayer(CyberCore::GraphicsLayer*) { }

    virtual void setShouldScaleViewToFitDocument(bool) { }

    virtual bool addMilestonesToDispatch(OptionSet<CyberCore::LayoutMilestone>) { return false; }

#if PLATFORM(COCOA)
    // Used by TiledCoreAnimationDrawingArea.
    virtual void updateGeometry(const CyberCore::IntSize& viewSize, bool flushSynchronously, const WTF::MachSendRight& fencePort) { }
#endif

#if USE(COORDINATED_GRAPHICS)
    virtual void layerHostDidFlushLayers() { }
#endif

#if USE(COORDINATED_GRAPHICS) || USE(TEXTURE_MAPPER)
    virtual void didChangeViewportAttributes(CyberCore::ViewportAttributes&&) = 0;
    virtual void deviceOrPageScaleFactorChanged() = 0;
#endif

    virtual void adoptLayersFromDrawingArea(DrawingArea&) { }
    virtual void adoptDisplayRefreshMonitorsFromDrawingArea(DrawingArea&) { }

    void removeMessageReceiverIfNeeded();

protected:
    DrawingArea(DrawingAreaType, DrawingAreaIdentifier, WebPage&);

    template<typename U> bool send(const U& message)
    {
        return m_webPage.send(message, m_identifier.toUInt64(), { });
    }

    DrawingAreaType m_type;
    DrawingAreaIdentifier m_identifier;
    WebPage& m_webPage;

private:
    // IPC::MessageReceiver.
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;

    // Message handlers.
    // FIXME: These should be pure virtual.
#if USE(COORDINATED_GRAPHICS) || USE(TEXTURE_MAPPER)
    virtual void updateBackingStoreState(uint64_t /*backingStoreStateID*/, bool /*respondImmediately*/, float /*deviceScaleFactor*/, const CyberCore::IntSize& /*size*/,
                                         const CyberCore::IntSize& /*scrollOffset*/) { }
#endif
    virtual void didUpdate() { }

#if PLATFORM(COCOA)
    // Used by TiledCoreAnimationDrawingArea.
    virtual void setDeviceScaleFactor(float) { }
    virtual void setColorSpace(const ColorSpaceData&) { }

    virtual void adjustTransientZoom(double scale, CyberCore::FloatPoint origin) { }
    virtual void commitTransientZoom(double scale, CyberCore::FloatPoint origin) { }

    virtual void addTransactionCallbackID(CyberKit::CallbackID) { ASSERT_NOT_REACHED(); }
#endif

    bool m_hasRemovedMessageReceiver { false };
};

} // namespace CyberKit

#define SPECIALIZE_TYPE_TRAITS_DRAWING_AREA(ToValueTypeName, AreaType) \
SPECIALIZE_TYPE_TRAITS_BEGIN(CyberKit::ToValueTypeName) \
    static bool isType(const CyberKit::DrawingArea& area) { return area.type() == CyberKit::AreaType; } \
SPECIALIZE_TYPE_TRAITS_END()
