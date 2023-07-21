/*
 * Copyright (C) 2018 Apple Inc. All rights reserved.
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

#include <CyberCore/GraphicsLayer.h>
#include <wtf/RetainPtr.h>
#include <wtf/Vector.h>

OBJC_CLASS CALayer;
#if PLATFORM(IOS_FAMILY)
OBJC_CLASS UIView;
#endif

namespace CyberKit {

class RemoteLayerTreeScrollbars;

class RemoteLayerTreeNode {
    WTF_MAKE_FAST_ALLOCATED;
public:
    RemoteLayerTreeNode(CyberCore::GraphicsLayer::PlatformLayerID, RetainPtr<CALayer>);
#if PLATFORM(IOS_FAMILY)
    RemoteLayerTreeNode(CyberCore::GraphicsLayer::PlatformLayerID, RetainPtr<UIView>);
#endif
    ~RemoteLayerTreeNode();

    static std::unique_ptr<RemoteLayerTreeNode> createWithPlainLayer(CyberCore::GraphicsLayer::PlatformLayerID);

    CALayer *layer() const { return m_layer.get(); }
#if PLATFORM(IOS_FAMILY)
    UIView *uiView() const { return m_uiView.get(); }
#endif

    CyberCore::GraphicsLayer::PlatformLayerID layerID() const { return m_layerID; }

    const CyberCore::EventRegion& eventRegion() const { return m_eventRegion; }
    void setEventRegion(const CyberCore::EventRegion&);

    // Non-ancestor scroller that controls positioning of the layer.
    CyberCore::GraphicsLayer::PlatformLayerID actingScrollContainerID() const { return m_actingScrollContainerID; }
    // Ancestor scrollers that don't affect positioning of the layer.
    const Vector<CyberCore::GraphicsLayer::PlatformLayerID>& stationaryScrollContainerIDs() const { return m_stationaryScrollContainerIDs; }

    void setActingScrollContainerID(CyberCore::GraphicsLayer::PlatformLayerID value) { m_actingScrollContainerID = value; }
    void setStationaryScrollContainerIDs(Vector<CyberCore::GraphicsLayer::PlatformLayerID>&& value) { m_stationaryScrollContainerIDs = WTFMove(value); }

    void detachFromParent();

    static CyberCore::GraphicsLayer::PlatformLayerID layerID(CALayer *);
    static RemoteLayerTreeNode* forCALayer(CALayer *);

    static NSString *appendLayerDescription(NSString *description, CALayer *);

private:
    void initializeLayer();

    CyberCore::GraphicsLayer::PlatformLayerID m_layerID;

    RetainPtr<CALayer> m_layer;
#if PLATFORM(IOS_FAMILY)
    RetainPtr<UIView> m_uiView;
#endif

    CyberCore::EventRegion m_eventRegion;

    CyberCore::GraphicsLayer::PlatformLayerID m_actingScrollContainerID { 0 };
    Vector<CyberCore::GraphicsLayer::PlatformLayerID> m_stationaryScrollContainerIDs;
};

}
