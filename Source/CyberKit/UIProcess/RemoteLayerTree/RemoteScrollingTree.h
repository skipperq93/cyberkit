/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
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

#if ENABLE(ASYNC_SCROLLING)

#include "RemoteScrollingCoordinator.h"
#include <CyberCore/ScrollingConstraints.h>
#include <CyberCore/ScrollingTree.h>

namespace CyberCore {
class PlatformMouseEvent;
};

namespace CyberKit {

class RemoteScrollingCoordinatorProxy;

class RemoteScrollingTree final : public CyberCore::ScrollingTree {
public:
    static Ref<RemoteScrollingTree> create(RemoteScrollingCoordinatorProxy&);
    virtual ~RemoteScrollingTree();

    bool isRemoteScrollingTree() const override { return true; }
    CyberCore::ScrollingEventResult tryToHandleWheelEvent(const CyberCore::PlatformWheelEvent&) override;

    void handleMouseEvent(const CyberCore::PlatformMouseEvent&);

    const RemoteScrollingCoordinatorProxy& scrollingCoordinatorProxy() const { return m_scrollingCoordinatorProxy; }

    void scrollingTreeNodeDidScroll(CyberCore::ScrollingTreeScrollingNode&, CyberCore::ScrollingLayerPositionAction = CyberCore::ScrollingLayerPositionAction::Sync) override;
    void scrollingTreeNodeRequestsScroll(CyberCore::ScrollingNodeID, const CyberCore::FloatPoint& scrollPosition, bool representsProgrammaticScroll) override;

    void currentSnapPointIndicesDidChange(CyberCore::ScrollingNodeID, unsigned horizontal, unsigned vertical) override;

private:
    explicit RemoteScrollingTree(RemoteScrollingCoordinatorProxy&);

#if PLATFORM(MAC)
    void handleWheelEventPhase(CyberCore::PlatformWheelEventPhase) override;
#endif

#if PLATFORM(IOS_FAMILY)
    void scrollingTreeNodeWillStartPanGesture() override;
    void scrollingTreeNodeWillStartScroll() override;
    void scrollingTreeNodeDidEndScroll() override;
#endif

    Ref<CyberCore::ScrollingTreeNode> createScrollingTreeNode(CyberCore::ScrollingNodeType, CyberCore::ScrollingNodeID) override;
    
    RemoteScrollingCoordinatorProxy& m_scrollingCoordinatorProxy;
};

} // namespace CyberKit

SPECIALIZE_TYPE_TRAITS_SCROLLING_TREE(CyberKit::RemoteScrollingTree, isRemoteScrollingTree());

#endif // ENABLE(ASYNC_SCROLLING)
