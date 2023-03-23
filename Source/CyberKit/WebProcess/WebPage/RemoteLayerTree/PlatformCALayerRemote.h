/*
 * Copyright (C) 2013-2021 Apple Inc. All rights reserved.
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

#include "RemoteLayerTreeTransaction.h"
#include <CyberCore/PlatformCALayer.h>
#include <CyberCore/PlatformLayer.h>

namespace CyberCore {
class LayerPool;
}

namespace CyberKit {

class RemoteLayerTreeContext;

class PlatformCALayerRemote : public CyberCore::PlatformCALayer {
public:
    static Ref<PlatformCALayerRemote> create(CyberCore::PlatformCALayer::LayerType, CyberCore::PlatformCALayerClient*, RemoteLayerTreeContext&);
    static Ref<PlatformCALayerRemote> create(PlatformLayer *, CyberCore::PlatformCALayerClient*, RemoteLayerTreeContext&);
#if ENABLE(MODEL_ELEMENT)
    static Ref<PlatformCALayerRemote> create(Ref<CyberCore::Model>, CyberCore::PlatformCALayerClient*, RemoteLayerTreeContext&);
#endif
    static Ref<PlatformCALayerRemote> create(const PlatformCALayerRemote&, CyberCore::PlatformCALayerClient*, RemoteLayerTreeContext&);

    virtual ~PlatformCALayerRemote();

    PlatformLayer* platformLayer() const override { return nullptr; }

    void recursiveBuildTransaction(RemoteLayerTreeContext&, RemoteLayerTreeTransaction&);

    void setNeedsDisplayInRect(const CyberCore::FloatRect& dirtyRect) override;
    void setNeedsDisplay() override;

    void copyContentsFromLayer(PlatformCALayer*) override;

    CyberCore::PlatformCALayer* superlayer() const override;
    void removeFromSuperlayer() override;
    void setSublayers(const CyberCore::PlatformCALayerList&) override;
    CyberCore::PlatformCALayerList sublayersForLogging() const override { return m_children; }
    void removeAllSublayers() override;
    void appendSublayer(CyberCore::PlatformCALayer&) override;
    void insertSublayer(CyberCore::PlatformCALayer&, size_t index) override;
    void replaceSublayer(CyberCore::PlatformCALayer& reference, CyberCore::PlatformCALayer&) override;
    const CyberCore::PlatformCALayerList* customSublayers() const override { return nullptr; }
    void adoptSublayers(CyberCore::PlatformCALayer& source) override;

    void addAnimationForKey(const String& key, CyberCore::PlatformCAAnimation&) override;
    void removeAnimationForKey(const String& key) override;
    RefPtr<CyberCore::PlatformCAAnimation> animationForKey(const String& key) override;
    void animationStarted(const String& key, MonotonicTime beginTime) override;
    void animationEnded(const String& key) override;

    void setMask(CyberCore::PlatformCALayer*) override;

    bool isOpaque() const override;
    void setOpaque(bool) override;

    CyberCore::FloatRect bounds() const override;
    void setBounds(const CyberCore::FloatRect&) override;

    CyberCore::FloatPoint3D position() const override;
    void setPosition(const CyberCore::FloatPoint3D&) override;

    CyberCore::FloatPoint3D anchorPoint() const override;
    void setAnchorPoint(const CyberCore::FloatPoint3D&) override;

    CyberCore::TransformationMatrix transform() const override;
    void setTransform(const CyberCore::TransformationMatrix&) override;

    CyberCore::TransformationMatrix sublayerTransform() const override;
    void setSublayerTransform(const CyberCore::TransformationMatrix&) override;

    bool isHidden() const override;
    void setHidden(bool) override;

    bool contentsHidden() const override;
    void setContentsHidden(bool) override;

    bool userInteractionEnabled() const override;
    void setUserInteractionEnabled(bool) override;

    void setBackingStoreAttached(bool) override;
    bool backingStoreAttached() const override;

    bool geometryFlipped() const override;
    void setGeometryFlipped(bool) override;

    bool isDoubleSided() const override;
    void setDoubleSided(bool) override;

    bool masksToBounds() const override;
    void setMasksToBounds(bool) override;

    bool acceleratesDrawing() const override;
    void setAcceleratesDrawing(bool) override;

    bool wantsDeepColorBackingStore() const override;
    void setWantsDeepColorBackingStore(bool) override;

    bool hasContents() const override;
    CFTypeRef contents() const override;
    void setContents(CFTypeRef) override;
#if HAVE(IOSURFACE)
    void setContents(const CyberCore::IOSurface&) override;
    void setContents(const WTF::MachSendRight&) override;
#endif
    void setContentsRect(const CyberCore::FloatRect&) override;

    void setMinificationFilter(CyberCore::PlatformCALayer::FilterType) override;
    void setMagnificationFilter(CyberCore::PlatformCALayer::FilterType) override;

    CyberCore::Color backgroundColor() const override;
    void setBackgroundColor(const CyberCore::Color&) override;

    void setBorderWidth(float) override;
    void setBorderColor(const CyberCore::Color&) override;

    float opacity() const override;
    void setOpacity(float) override;

    void setFilters(const CyberCore::FilterOperations&) override;
    static bool filtersCanBeComposited(const CyberCore::FilterOperations&);
    void copyFiltersFrom(const CyberCore::PlatformCALayer&) override;

#if ENABLE(CSS_COMPOSITING)
    void setBlendMode(CyberCore::BlendMode) override;
#endif

    void setName(const String&) override;

    void setSpeed(float) override;

    void setTimeOffset(CFTimeInterval) override;

    float contentsScale() const override;
    void setContentsScale(float) override;

    float cornerRadius() const override;
    void setCornerRadius(float) override;

    void setAntialiasesEdges(bool) override;

    // FIXME: Having both shapeRoundedRect and shapePath is redundant. We could use shapePath for everything.
    CyberCore::FloatRoundedRect shapeRoundedRect() const override;
    void setShapeRoundedRect(const CyberCore::FloatRoundedRect&) override;

    CyberCore::Path shapePath() const override;
    void setShapePath(const CyberCore::Path&) override;

    CyberCore::WindRule shapeWindRule() const override;
    void setShapeWindRule(CyberCore::WindRule) override;

    CyberCore::GraphicsLayer::CustomAppearance customAppearance() const override;
    void updateCustomAppearance(CyberCore::GraphicsLayer::CustomAppearance) override;

    void setEventRegion(const CyberCore::EventRegion&) override;

#if ENABLE(SCROLLING_THREAD)
    CyberCore::ScrollingNodeID scrollingNodeID() const override;
    void setScrollingNodeID(CyberCore::ScrollingNodeID) override;
#endif

#if HAVE(CORE_ANIMATION_SEPARATED_LAYERS)
    bool isSeparated() const override;
    void setIsSeparated(bool) override;

#if HAVE(CORE_ANIMATION_SEPARATED_PORTALS)
    bool isSeparatedPortal() const override;
    void setIsSeparatedPortal(bool) override;

    bool isDescendentOfSeparatedPortal() const override;
    void setIsDescendentOfSeparatedPortal(bool) override;
#endif
#endif

    CyberCore::TiledBacking* tiledBacking() override { return nullptr; }

    Ref<CyberCore::PlatformCALayer> clone(CyberCore::PlatformCALayerClient* owner) const override;

    Ref<PlatformCALayer> createCompatibleLayer(CyberCore::PlatformCALayer::LayerType, CyberCore::PlatformCALayerClient*) const override;

    void enumerateRectsBeingDrawn(CyberCore::GraphicsContext&, void (^block)(CyberCore::FloatRect)) override;

    virtual uint32_t hostingContextID();

    unsigned backingStoreBytesPerPixel() const override;

    void setClonedLayer(const PlatformCALayer*);

    RemoteLayerTreeTransaction::LayerProperties& properties() { return m_properties; }
    const RemoteLayerTreeTransaction::LayerProperties& properties() const { return m_properties; }

    void didCommit();

    void moveToContext(RemoteLayerTreeContext&);
    void clearContext() { m_context = nullptr; }
    RemoteLayerTreeContext* context() const { return m_context; }
    
    virtual void populateCreationProperties(RemoteLayerTreeTransaction::LayerCreationProperties&, const RemoteLayerTreeContext&, CyberCore::PlatformCALayer::LayerType);

protected:
    PlatformCALayerRemote(CyberCore::PlatformCALayer::LayerType, CyberCore::PlatformCALayerClient* owner, RemoteLayerTreeContext&);
    PlatformCALayerRemote(const PlatformCALayerRemote&, CyberCore::PlatformCALayerClient*, RemoteLayerTreeContext&);

    void updateClonedLayerProperties(PlatformCALayerRemote& clone, bool copyContents = true) const;

private:
    Type type() const override { return Type::Remote; }
    void ensureBackingStore();
    void updateBackingStore();
    void removeSublayer(PlatformCALayerRemote*);

#if ENABLE(CG_DISPLAY_LIST_BACKED_IMAGE_BUFFER)
    RemoteLayerBackingStore::IncludeDisplayList shouldIncludeDisplayListInBackingStore() const;
#endif

    bool requiresCustomAppearanceUpdateOnBoundsChange() const;

    CyberCore::LayerPool& layerPool() override;

    RemoteLayerTreeTransaction::LayerProperties m_properties;
    CyberCore::PlatformCALayerList m_children;
    PlatformCALayerRemote* m_superlayer { nullptr };
    PlatformCALayerRemote* m_maskLayer { nullptr };
    HashMap<String, RefPtr<CyberCore::PlatformCAAnimation>> m_animations;

    bool m_acceleratesDrawing { false };
    bool m_wantsDeepColorBackingStore { false };

    RemoteLayerTreeContext* m_context;
};

} // namespace CyberKit

SPECIALIZE_TYPE_TRAITS_BEGIN(CyberKit::PlatformCALayerRemote)
static bool isType(const CyberCore::PlatformCALayer& layer)
{
    switch (layer.type()) {
    case CyberCore::PlatformCALayer::Type::Cocoa:
        break;
    case CyberCore::PlatformCALayer::Type::Remote:
    case CyberCore::PlatformCALayer::Type::RemoteCustom:
    case CyberCore::PlatformCALayer::Type::RemoteHost:
    case CyberCore::PlatformCALayer::Type::RemoteModel:
        return true;
    };
    return false;
}
SPECIALIZE_TYPE_TRAITS_END()
