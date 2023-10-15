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

#include "RemoteScrollingTree.h"

#if PLATFORM(MAC) && ENABLE(UI_SIDE_COMPOSITING)

#include "RemoteScrollingCoordinator.h"
#include <CyberCore/ScrollingConstraints.h>
#include <CyberCore/ScrollingTree.h>

namespace CyberCore {
class PlatformMouseEvent;
};

namespace CyberKit {

class RemoteScrollingCoordinatorProxy;

class RemoteScrollingTreeMac final : public RemoteScrollingTree {
public:
    explicit RemoteScrollingTreeMac(RemoteScrollingCoordinatorProxy&);
    virtual ~RemoteScrollingTreeMac();

    void scrollingTreeNodeScrollbarVisibilityDidChange(CyberCore::ScrollingNodeID, ScrollbarOrientation, bool) override;

private:
    void handleWheelEventPhase(CyberCore::ScrollingNodeID, CyberCore::PlatformWheelEventPhase) override;
    RefPtr<CyberCore::ScrollingTreeNode> scrollingNodeForPoint(CyberCore::FloatPoint) override;
#if ENABLE(WHEEL_EVENT_REGIONS)
    OptionSet<CyberCore::EventListenerRegionType> eventListenerRegionTypesForPoint(CyberCore::FloatPoint) const override;
#endif

    void didCommitTree() override;

    void scrollingTreeNodeDidScroll(CyberCore::ScrollingTreeScrollingNode&, CyberCore::ScrollingLayerPositionAction) override;
    void scrollingTreeNodeDidStopAnimatedScroll(CyberCore::ScrollingTreeScrollingNode&) override;
    bool scrollingTreeNodeRequestsScroll(CyberCore::ScrollingNodeID, const CyberCore::RequestedScrollData&) override;
    bool scrollingTreeNodeRequestsKeyboardScroll(CyberCore::ScrollingNodeID, const CyberCore::RequestedKeyboardScrollData&) override;
    void currentSnapPointIndicesDidChange(CyberCore::ScrollingNodeID, std::optional<unsigned> horizontal, std::optional<unsigned> vertical) override;
    void reportExposedUnfilledArea(MonotonicTime, unsigned unfilledArea) override;
    void reportSynchronousScrollingReasonsChanged(MonotonicTime, OptionSet<CyberCore::SynchronousScrollingReason>) override;
    void receivedWheelEventWithPhases(CyberCore::PlatformWheelEventPhase, CyberCore::PlatformWheelEventPhase momentumPhase) override;

    // "Default handling" here refers to sending the event to the web process for synchronous scrolling, and DOM event handling.
    void willSendEventForDefaultHandling(const CyberCore::PlatformWheelEvent&) override;
    void waitForEventDefaultHandlingCompletion(const CyberCore::PlatformWheelEvent&) override;
    void wheelEventDefaultHandlingCompleted(const CyberCore::PlatformWheelEvent&, CyberCore::ScrollingNodeID, std::optional<CyberCore::WheelScrollGestureState>) override;

    void deferWheelEventTestCompletionForReason(CyberCore::ScrollingNodeID, CyberCore::WheelEventTestMonitor::DeferReason) override;
    void removeWheelEventTestCompletionDeferralForReason(CyberCore::ScrollingNodeID, CyberCore::WheelEventTestMonitor::DeferReason) override;

    void hasNodeWithAnimatedScrollChanged(bool) override;
    void displayDidRefresh(CyberCore::PlatformDisplayID) override;

    void lockLayersForHitTesting() final WTF_ACQUIRES_LOCK(m_layerHitTestMutex);
    void unlockLayersForHitTesting() final WTF_RELEASES_LOCK(m_layerHitTestMutex);

    void beginTransactionOnScrollingThread() final;
    void commitTransactionOnScrollingThread() final;

    void startPendingScrollAnimations() WTF_REQUIRES_LOCK(m_treeLock);

    void scrollingTreeNodeWillStartScroll(CyberCore::ScrollingNodeID) override;
    void scrollingTreeNodeDidEndScroll(CyberCore::ScrollingNodeID) override;

    void scrollingTreeNodeDidBeginScrollSnapping(ScrollingNodeID) override;
    void scrollingTreeNodeDidEndScrollSnapping(ScrollingNodeID) override;

    Ref<CyberCore::ScrollingTreeNode> createScrollingTreeNode(CyberCore::ScrollingNodeType, CyberCore::ScrollingNodeID) override;

    HashMap<CyberCore::ScrollingNodeID, CyberCore::RequestedScrollData> m_nodesWithPendingScrollAnimations; // Guarded by m_treeLock but used via call chains that can't be annotated.
    HashMap<CyberCore::ScrollingNodeID, CyberCore::RequestedKeyboardScrollData> m_nodesWithPendingKeyboardScrollAnimations; // Guarded by m_treeLock but used via call chains that can't be annotated.

    mutable Lock m_layerHitTestMutex;

    Condition m_waitingForBeganEventCondition;
    bool m_receivedBeganEventFromMainThread WTF_GUARDED_BY_LOCK(m_treeLock) { false };
};

} // namespace CyberKit

#endif // PLATFORM(MAC) && ENABLE(UI_SIDE_COMPOSITING)
