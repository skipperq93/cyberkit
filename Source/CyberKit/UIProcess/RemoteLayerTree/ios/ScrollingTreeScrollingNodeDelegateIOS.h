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

#if PLATFORM(IOS_FAMILY) && ENABLE(ASYNC_SCROLLING)

#import <UIKit/UIScrollView.h>
#import <CyberCore/ScrollingCoordinator.h>
#import <CyberCore/ScrollingTreeScrollingNode.h>
#import <CyberCore/ScrollingTreeScrollingNodeDelegate.h>

@class CALayer;
@class UIScrollEvent;
@class UIScrollView;
@class WKScrollingNodeScrollViewDelegate;

namespace CyberCore {

class FloatPoint;
class FloatRect;
class FloatSize;
class ScrollingTreeScrollingNode;

}

namespace CyberKit {

class ScrollingTreeScrollingNodeDelegateIOS final : public CyberCore::ScrollingTreeScrollingNodeDelegate {
    WTF_MAKE_FAST_ALLOCATED;
public:
    
    enum class AllowOverscrollToPreventScrollPropagation : bool { No, Yes };
    
    explicit ScrollingTreeScrollingNodeDelegateIOS(CyberCore::ScrollingTreeScrollingNode&);
    ~ScrollingTreeScrollingNodeDelegateIOS() final;

    void scrollWillStart() const;
    void scrollDidEnd() const;
    void scrollViewWillStartPanGesture() const;
    void scrollViewDidScroll(const CyberCore::FloatPoint& scrollOffset, bool inUserInteraction);

    void currentSnapPointIndicesDidChange(std::optional<unsigned> horizontal, std::optional<unsigned> vertical) const;
    CALayer *scrollLayer() const { return m_scrollLayer.get(); }

    void resetScrollViewDelegate();
    void commitStateBeforeChildren(const CyberCore::ScrollingStateScrollingNode&);
    void commitStateAfterChildren(const CyberCore::ScrollingStateScrollingNode&);

    void repositionScrollingLayers();

#if HAVE(UISCROLLVIEW_ASYNCHRONOUS_SCROLL_EVENT_HANDLING)
    void handleAsynchronousCancelableScrollEvent(UIScrollView *, UIScrollEvent *, void (^completion)(BOOL handled));
#endif

    OptionSet<CyberCore::TouchAction> activeTouchActions() const { return m_activeTouchActions; }
    void computeActiveTouchActionsForGestureRecognizer(UIGestureRecognizer*);
    void clearActiveTouchActions() { m_activeTouchActions = { }; }
    void cancelPointersForGestureRecognizer(UIGestureRecognizer*);

    UIScrollView *findActingScrollParent(UIScrollView *);
    UIScrollView *scrollView() const;

    bool startAnimatedScrollToPosition(CyberCore::FloatPoint) final;
    void stopAnimatedScroll() final;

    void serviceScrollAnimation(MonotonicTime) final { }
    
    static void updateScrollViewForOverscrollBehavior(UIScrollView *, const CyberCore::OverscrollBehavior, const CyberCore::OverscrollBehavior, AllowOverscrollToPreventScrollPropagation);

private:
    RetainPtr<CALayer> m_scrollLayer;
#if ENABLE(INTERACTION_REGIONS_IN_EVENT_REGION)
    RetainPtr<CALayer> m_interactionRegionsLayer;
#endif
    RetainPtr<CALayer> m_scrolledContentsLayer;
    RetainPtr<WKScrollingNodeScrollViewDelegate> m_scrollViewDelegate;
    OptionSet<CyberCore::TouchAction> m_activeTouchActions { };
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
