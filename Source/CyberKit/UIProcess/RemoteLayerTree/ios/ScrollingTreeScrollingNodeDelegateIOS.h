/*
 * Copyright (C) 2017 Igalia S.L. All rights reserved.
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

#if PLATFORM(IOS_FAMILY) && ENABLE(ASYNC_SCROLLING)

#include <UIKit/UIScrollView.h>
#include <CyberCore/ScrollingCoordinator.h>
#include <CyberCore/ScrollingTreeScrollingNode.h>
#include <CyberCore/ScrollingTreeScrollingNodeDelegate.h>

OBJC_CLASS CALayer;
OBJC_CLASS WKScrollingNodeScrollViewDelegate;

namespace CyberCore {

class FloatPoint;
class FloatRect;
class FloatSize;
class ScrollingTreeScrollingNode;

}

namespace CyberKit {

class ScrollingTreeScrollingNodeDelegateIOS : public CyberCore::ScrollingTreeScrollingNodeDelegate {
public:
    explicit ScrollingTreeScrollingNodeDelegateIOS(CyberCore::ScrollingTreeScrollingNode&);
    ~ScrollingTreeScrollingNodeDelegateIOS() final;

    void scrollWillStart() const;
    void scrollDidEnd() const;
    void scrollViewWillStartPanGesture() const;
    void scrollViewDidScroll(const CyberCore::FloatPoint& scrollPosition, bool inUserInteraction) const;
    void currentSnapPointIndicesDidChange(unsigned horizontal, unsigned vertical) const;
    CALayer *scrollLayer() const { return m_scrollLayer.get(); }

    void resetScrollViewDelegate();
    void commitStateBeforeChildren(const CyberCore::ScrollingStateScrollingNode&);
    void commitStateAfterChildren(const CyberCore::ScrollingStateScrollingNode&);
    void updateLayersAfterAncestorChange(const CyberCore::ScrollingTreeNode& changedNode, const CyberCore::FloatRect& fixedPositionRect, const CyberCore::FloatSize& cumulativeDelta);
    CyberCore::FloatPoint scrollPosition() const;
    void setScrollLayerPosition(const CyberCore::FloatPoint&);
    void updateChildNodesAfterScroll(const CyberCore::FloatPoint& scrollPosition);

private:
    RetainPtr<CALayer> m_scrollLayer;
    RetainPtr<CALayer> m_scrolledContentsLayer;
    RetainPtr<WKScrollingNodeScrollViewDelegate> m_scrollViewDelegate;
    bool m_updatingFromStateNode { false };
};

} // namespace CyberKit

@interface WKScrollingNodeScrollViewDelegate : NSObject <UIScrollViewDelegate> {
    CyberKit::ScrollingTreeScrollingNodeDelegateIOS* _scrollingTreeNodeDelegate;
}

@property (nonatomic, getter=_isInUserInteraction) BOOL inUserInteraction;

- (instancetype)initWithScrollingTreeNodeDelegate:(CyberKit::ScrollingTreeScrollingNodeDelegateIOS*)delegate;

@end

#endif // PLATFORM(IOS_FAMILY) && ENABLE(ASYNC_SCROLLING)
