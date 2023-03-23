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

#if ENABLE(UI_SIDE_COMPOSITING)

#include "RemoteScrollingCoordinator.h"
#include <CyberCore/ScrollingConstraints.h>
#include <CyberCore/ScrollingTree.h>
#include <CyberCore/WheelEventTestMonitor.h>

namespace CyberCore {
class PlatformMouseEvent;
};

namespace CyberKit {

class RemoteScrollingCoordinatorProxy;

class RemoteScrollingTree : public CyberCore::ScrollingTree {
public:
    static Ref<RemoteScrollingTree> create(RemoteScrollingCoordinatorProxy&);
    virtual ~RemoteScrollingTree();

    bool isRemoteScrollingTree() const final { return true; }

    void invalidate() final;

    virtual void handleMouseEvent(const CyberCore::PlatformMouseEvent&) { }

    const RemoteScrollingCoordinatorProxy& scrollingCoordinatorProxy() const { return m_scrollingCoordinatorProxy; }

    void scrollingTreeNodeDidScroll(CyberCore::ScrollingTreeScrollingNode&, CyberCore::ScrollingLayerPositionAction = CyberCore::ScrollingLayerPositionAction::Sync) final;
    void scrollingTreeNodeDidStopAnimatedScroll(CyberCore::ScrollingTreeScrollingNode&) final;
    bool scrollingTreeNodeRequestsScroll(CyberCore::ScrollingNodeID, const CyberCore::RequestedScrollData&) final;

    void currentSnapPointIndicesDidChange(CyberCore::ScrollingNodeID, std::optional<unsigned> horizontal, std::optional<unsigned> vertical) final;
    void reportExposedUnfilledArea(MonotonicTime, unsigned unfilledArea) override;
    void reportSynchronousScrollingReasonsChanged(MonotonicTime, OptionSet<CyberCore::SynchronousScrollingReason>) override;

protected:
    explicit RemoteScrollingTree(RemoteScrollingCoordinatorProxy&);

    Ref<CyberCore::ScrollingTreeNode> createScrollingTreeNode(CyberCore::ScrollingNodeType, CyberCore::ScrollingNodeID) override;

    void receivedWheelEventWithPhases(CyberCore::PlatformWheelEventPhase phase, CyberCore::PlatformWheelEventPhase momentumPhase) override;
    void deferWheelEventTestCompletionForReason(CyberCore::ScrollingNodeID, CyberCore::WheelEventTestMonitor::DeferReason) override;
    void removeWheelEventTestCompletionDeferralForReason(CyberCore::ScrollingNodeID, CyberCore::WheelEventTestMonitor::DeferReason) override;

    RemoteScrollingCoordinatorProxy& m_scrollingCoordinatorProxy;
};

} // namespace CyberKit

SPECIALIZE_TYPE_TRAITS_SCROLLING_TREE(CyberKit::RemoteScrollingTree, isRemoteScrollingTree());

#endif // ENABLE(UI_SIDE_COMPOSITING)
