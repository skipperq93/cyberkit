/*
 * Copyright (C) 2014-2015 Apple Inc. All rights reserved.
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

#include "MessageReceiver.h"
#include "RemoteScrollingCoordinator.h"
#include "RemoteScrollingTree.h"
#include <wtf/Noncopyable.h>
#include <wtf/RefPtr.h>

namespace CyberCore {
class FloatPoint;
class PlatformWheelEvent;
}

namespace CyberKit {

class RemoteLayerTreeHost;
class RemoteScrollingCoordinatorTransaction;
class RemoteScrollingTree;
class WebPageProxy;

class RemoteScrollingCoordinatorProxy {
    WTF_MAKE_NONCOPYABLE(RemoteScrollingCoordinatorProxy);
public:
    explicit RemoteScrollingCoordinatorProxy(WebPageProxy&);
    virtual ~RemoteScrollingCoordinatorProxy();

    bool visualViewportEnabled() const { return m_scrollingTree && m_scrollingTree->visualViewportEnabled(); }

    // Inform the web process that the scroll position changed (called from the scrolling tree)
    void scrollingTreeNodeDidScroll(CyberCore::ScrollingNodeID, const CyberCore::FloatPoint& newScrollPosition, const Optional<CyberCore::FloatPoint>& layoutViewportOrigin, CyberCore::ScrollingLayerPositionAction);
    void scrollingTreeNodeRequestsScroll(CyberCore::ScrollingNodeID, const CyberCore::FloatPoint& scrollPosition, bool representsProgrammaticScroll);

    CyberCore::TrackingType eventTrackingTypeForPoint(const AtomicString& eventName, CyberCore::IntPoint) const;

    // Called externally when native views move around.
    void viewportChangedViaDelegatedScrolling(CyberCore::ScrollingNodeID, const CyberCore::FloatRect& fixedPositionRect, double scale);

    void currentSnapPointIndicesDidChange(CyberCore::ScrollingNodeID, unsigned horizontal, unsigned vertical);

    // FIXME: expose the tree and pass this to that?
    bool handleWheelEvent(const CyberCore::PlatformWheelEvent&);
    
    CyberCore::ScrollingNodeID rootScrollingNodeID() const;

    const RemoteLayerTreeHost* layerTreeHost() const;

    struct RequestedScrollInfo {
        bool requestsScrollPositionUpdate { };
        bool requestIsProgrammaticScroll { };
        CyberCore::FloatPoint requestedScrollPosition;
    };
    void commitScrollingTreeState(const RemoteScrollingCoordinatorTransaction&, RequestedScrollInfo&);

    void setPropagatesMainFrameScrolls(bool propagatesMainFrameScrolls) { m_propagatesMainFrameScrolls = propagatesMainFrameScrolls; }
    bool propagatesMainFrameScrolls() const { return m_propagatesMainFrameScrolls; }
    bool hasFixedOrSticky() const { return m_scrollingTree->hasFixedOrSticky(); }

#if PLATFORM(IOS_FAMILY)
    CyberCore::FloatRect customFixedPositionRect() const;
    void scrollingTreeNodeWillStartPanGesture();
    void scrollingTreeNodeWillStartScroll();
    void scrollingTreeNodeDidEndScroll();
#if ENABLE(CSS_SCROLL_SNAP)
    void adjustTargetContentOffsetForSnapping(CGSize maxScrollDimensions, CGPoint velocity, CGFloat topInset, CGPoint* targetContentOffset);
    bool hasActiveSnapPoint() const;
    CGPoint nearestActiveContentInsetAdjustedSnapPoint(CGFloat topInset, const CGPoint&) const;
    bool shouldSetScrollViewDecelerationRateFast() const;
#endif
#endif

    String scrollingTreeAsText() const;

private:
    void connectStateNodeLayers(CyberCore::ScrollingStateTree&, const RemoteLayerTreeHost&);
#if ENABLE(CSS_SCROLL_SNAP)
    bool shouldSnapForMainFrameScrolling(CyberCore::ScrollEventAxis) const;
    float closestSnapOffsetForMainFrameScrolling(CyberCore::ScrollEventAxis, float scrollDestination, float velocity, unsigned& closestIndex) const;
#endif

    WebPageProxy& m_webPageProxy;
    RefPtr<RemoteScrollingTree> m_scrollingTree;
    RequestedScrollInfo* m_requestedScrollInfo;
#if ENABLE(CSS_SCROLL_SNAP)
    unsigned m_currentHorizontalSnapPointIndex { 0 };
    unsigned m_currentVerticalSnapPointIndex { 0 };
#endif
    bool m_propagatesMainFrameScrolls;
};

} // namespace CyberKit

#endif // ENABLE(ASYNC_SCROLLING)
