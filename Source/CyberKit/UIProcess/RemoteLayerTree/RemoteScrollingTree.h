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

#include <CyberCore/ScrollingConstraints.h>
#include <CyberCore/ScrollingTree.h>
#include <CyberCore/WheelEventTestMonitor.h>
#include <wtf/WeakPtr.h>

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

    RemoteScrollingCoordinatorProxy* scrollingCoordinatorProxy() const;

    void scrollingTreeNodeDidScroll(CyberCore::ScrollingTreeScrollingNode&, CyberCore::ScrollingLayerPositionAction = CyberCore::ScrollingLayerPositionAction::Sync) override;
    void scrollingTreeNodeDidStopAnimatedScroll(CyberCore::ScrollingTreeScrollingNode&) override;
    bool scrollingTreeNodeRequestsScroll(CyberCore::ScrollingNodeID, const CyberCore::RequestedScrollData&) override;
    bool scrollingTreeNodeRequestsKeyboardScroll(CyberCore::ScrollingNodeID, const CyberCore::RequestedKeyboardScrollData&) override;

    void scrollingTreeNodeWillStartScroll(CyberCore::ScrollingNodeID) override;
    void scrollingTreeNodeDidEndScroll(CyberCore::ScrollingNodeID) override;

    void currentSnapPointIndicesDidChange(CyberCore::ScrollingNodeID, std::optional<unsigned> horizontal, std::optional<unsigned> vertical) override;
    void reportExposedUnfilledArea(MonotonicTime, unsigned unfilledArea) override;
    void reportSynchronousScrollingReasonsChanged(MonotonicTime, OptionSet<CyberCore::SynchronousScrollingReason>) override;

protected:
    explicit RemoteScrollingTree(RemoteScrollingCoordinatorProxy&);

    Ref<CyberCore::ScrollingTreeNode> createScrollingTreeNode(CyberCore::ScrollingNodeType, CyberCore::ScrollingNodeID) override;

    void receivedWheelEventWithPhases(CyberCore::PlatformWheelEventPhase phase, CyberCore::PlatformWheelEventPhase momentumPhase) override;
    void deferWheelEventTestCompletionForReason(CyberCore::ScrollingNodeID, CyberCore::WheelEventTestMonitor::DeferReason) override;
    void removeWheelEventTestCompletionDeferralForReason(CyberCore::ScrollingNodeID, CyberCore::WheelEventTestMonitor::DeferReason) override;

    // This gets nulled out via invalidate(), since the scrolling thread can hold a ref to the ScrollingTree after the RemoteScrollingCoordinatorProxy has gone away.
    WeakPtr<RemoteScrollingCoordinatorProxy> m_scrollingCoordinatorProxy;
};

class RemoteLayerTreeHitTestLocker {
public:
    RemoteLayerTreeHitTestLocker(RemoteScrollingTree& scrollingTree)
        : m_scrollingTree(scrollingTree)
    {
        m_scrollingTree->lockLayersForHitTesting();
    }
    
    ~RemoteLayerTreeHitTestLocker()
    {
        m_scrollingTree->unlockLayersForHitTesting();
    }

private:
    Ref<RemoteScrollingTree> m_scrollingTree;
};

} // namespace CyberKit

SPECIALIZE_TYPE_TRAITS_SCROLLING_TREE(CyberKit::RemoteScrollingTree, isRemoteScrollingTree());

#endif // ENABLE(UI_SIDE_COMPOSITING)
