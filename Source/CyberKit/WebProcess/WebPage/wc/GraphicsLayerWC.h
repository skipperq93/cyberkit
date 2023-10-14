/*
 * Copyright (C) 2021 Sony Interactive Entertainment Inc.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if USE(GRAPHICS_LAYER_WC)

#include "WCUpateInfo.h"
#include <CyberCore/GraphicsLayerContentsDisplayDelegate.h>
#include <wtf/DoublyLinkedList.h>

namespace CyberCore {
class TransformState;
}

namespace CyberKit {
class WCTiledBacking;

class GraphicsLayerWC final : public CyberCore::GraphicsLayer, public DoublyLinkedListNode<GraphicsLayerWC> {
public:
    struct Observer {
        virtual void graphicsLayerAdded(GraphicsLayerWC&) = 0;
        virtual void graphicsLayerRemoved(GraphicsLayerWC&) = 0;
        virtual void commitLayerUpateInfo(WCLayerUpateInfo&&) = 0;
        virtual RefPtr<CyberCore::ImageBuffer> createImageBuffer(CyberCore::FloatSize) = 0;
    };

    GraphicsLayerWC(Type layerType, CyberCore::GraphicsLayerClient&, Observer&);
    ~GraphicsLayerWC() override;

    void clearObserver() { m_observer = nullptr; }

    // GraphicsLayer
    CyberCore::PlatformLayerIdentifier primaryLayerID() const override;
    void setNeedsDisplay() override;
    void setNeedsDisplayInRect(const CyberCore::FloatRect&, ShouldClipToLayer) override;
    void setContentsNeedsDisplay() override;
    bool setChildren(Vector<Ref<GraphicsLayer>>&&) override;
    void addChild(Ref<GraphicsLayer>&&) override;
    void addChildAtIndex(Ref<GraphicsLayer>&&, int index) override;
    void addChildAbove(Ref<GraphicsLayer>&&, GraphicsLayer* sibling) override;
    void addChildBelow(Ref<GraphicsLayer>&&, GraphicsLayer* sibling) override;
    bool replaceChild(GraphicsLayer* oldChild, Ref<GraphicsLayer>&& newChild) override;
    void removeFromParent() override;
    void setMaskLayer(RefPtr<GraphicsLayer>&&) override;
    void setReplicatedLayer(GraphicsLayer*) override;
    void setReplicatedByLayer(RefPtr<GraphicsLayer>&&) override;
    void setPosition(const CyberCore::FloatPoint&) override;
    void setAnchorPoint(const CyberCore::FloatPoint3D&) override;
    void setSize(const CyberCore::FloatSize&) override;
    void setBoundsOrigin(const CyberCore::FloatPoint&) override;
    void setTransform(const CyberCore::TransformationMatrix&) override;
    void setChildrenTransform(const CyberCore::TransformationMatrix&) override;
    void setPreserves3D(bool) override;
    void setMasksToBounds(bool) override;
    void setBackgroundColor(const CyberCore::Color&) override;
    void setOpacity(float) override;
    void setContentsRect(const CyberCore::FloatRect&) override;
    void setContentsClippingRect(const CyberCore::FloatRoundedRect&) override;
    void setContentsRectClipsDescendants(bool) override;
    void setDrawsContent(bool) override;
    void setContentsVisible(bool) override;
    void setBackfaceVisibility(bool) override;
    void setContentsToSolidColor(const CyberCore::Color&) override;
    void setContentsToPlatformLayer(PlatformLayer*, ContentsLayerPurpose) override;
    void setContentsDisplayDelegate(RefPtr<CyberCore::GraphicsLayerContentsDisplayDelegate>&&, ContentsLayerPurpose) override;
    bool shouldDirectlyCompositeImage(CyberCore::Image*) const override { return false; }
    bool usesContentsLayer() const override;
    void setShowDebugBorder(bool) override;
    void setDebugBorder(const CyberCore::Color&, float width) override;
    void setShowRepaintCounter(bool) override;
    bool setFilters(const CyberCore::FilterOperations&) override;
    bool setBackdropFilters(const CyberCore::FilterOperations&) override;
    void setBackdropFiltersRect(const CyberCore::FloatRoundedRect&) override;
    void flushCompositingState(const CyberCore::FloatRect& clipRect) override;
    void flushCompositingStateForThisLayerOnly() override;
    CyberCore::TiledBacking* tiledBacking() const override;

protected:
    friend WCTiledBacking;

    RefPtr<CyberCore::ImageBuffer> createImageBuffer(CyberCore::FloatSize);
    
private:
    struct VisibleAndCoverageRects {
        WTF_MAKE_STRUCT_FAST_ALLOCATED;
        CyberCore::FloatRect visibleRect;
        CyberCore::FloatRect coverageRect;
        CyberCore::TransformationMatrix animatingTransform;
    };

    enum ScheduleFlushOrNot { ScheduleFlush, DontScheduleFlush };
    void noteLayerPropertyChanged(OptionSet<WCLayerChange>, ScheduleFlushOrNot = ScheduleFlush);
    CyberCore::TransformationMatrix transformByApplyingAnchorPoint(const CyberCore::TransformationMatrix&) const;
    CyberCore::TransformationMatrix layerTransform(const CyberCore::FloatPoint&, const CyberCore::TransformationMatrix* = nullptr) const;
    VisibleAndCoverageRects computeVisibleAndCoverageRect(CyberCore::TransformState&, bool preserves3D) const;
    void recursiveCommitChanges(const CyberCore::TransformState&);

    friend class WTF::DoublyLinkedListNode<GraphicsLayerWC>;

    GraphicsLayerWC* m_prev;
    GraphicsLayerWC* m_next;
    CyberCore::PlatformLayerIdentifier m_layerID { CyberCore::PlatformLayerIdentifier::generate() };
    Observer* m_observer;
    std::unique_ptr<WCTiledBacking> m_tiledBacking;
    PlatformLayer* m_platformLayer { nullptr };
    CyberCore::Color m_solidColor;
    CyberCore::Color m_debugBorderColor;
    OptionSet<WCLayerChange> m_uncommittedChanges;
    float m_debugBorderWidth { 0 };
};

} // namespace CyberKit

#endif // USE(GRAPHICS_LAYER_WC)
