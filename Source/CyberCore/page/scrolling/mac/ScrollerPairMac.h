/*
 * Copyright (C) 2019 Apple Inc. All rights reserved.
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

#if PLATFORM(MAC)

#include "ScrollerMac.h"
#include <CyberCore/FloatRect.h>
#include <CyberCore/FloatSize.h>
#include <CyberCore/PlatformWheelEvent.h>

OBJC_CLASS NSScrollerImpPair;
OBJC_CLASS WebScrollerImpPairDelegateMac;

namespace CyberCore {
class PlatformMouseEvent;
class PlatformWheelEvent;
class ScrollingTreeScrollingNode;
}

namespace CyberCore {

// Controls a pair of NSScrollerImps via a pair of ScrollerMac. The NSScrollerImps need to remain internal to this class.
class ScrollerPairMac {
    WTF_MAKE_FAST_ALLOCATED;
    friend class ScrollerMac;
public:
    ScrollerPairMac(CyberCore::ScrollingTreeScrollingNode&);
    void init();

    ~ScrollerPairMac();

    void handleWheelEventPhase(PlatformWheelEventPhase);
    bool handleMouseEvent(const CyberCore::PlatformMouseEvent&);
    
    void setUsePresentationValues(bool);
    bool isUsingPresentationValues() const { return m_usingPresentationValues; }
    
    void setVerticalScrollbarPresentationValue(float);
    void setHorizontalScrollbarPresentationValue(float);

    void updateValues();

    CyberCore::FloatSize visibleSize() const;
    CyberCore::IntPoint lastKnownMousePosition() const { return m_lastKnownMousePosition; }
    bool useDarkAppearance() const;

    struct Values {
        float value;
        float proportion;
    };
    Values valuesForOrientation(ScrollbarOrientation);

    void releaseReferencesToScrollerImpsOnTheMainThread();
    
    bool hasScrollerImp();

    // Only for use by WebScrollerImpPairDelegateMac. Do not use elsewhere!
    ScrollerMac& verticalScroller() { return m_verticalScroller; }
    ScrollerMac& horizontalScroller() { return m_horizontalScroller; }

private:

    NSScrollerImp *scrollerImpHorizontal() { return horizontalScroller().scrollerImp(); }
    NSScrollerImp *scrollerImpVertical() { return verticalScroller().scrollerImp(); }
    
    NSScrollerImpPair *scrollerImpPair() { return m_scrollerImpPair.get(); }
    Lock& scrollerImpPairLock() WTF_RETURNS_LOCK(m_scrollerImpPairLock) { return m_scrollerImpPairLock; }

    CyberCore::ScrollingTreeScrollingNode& m_scrollingNode;

    ScrollerMac m_verticalScroller;
    ScrollerMac m_horizontalScroller;

    CyberCore::IntPoint m_lastKnownMousePosition;
    std::optional<CyberCore::FloatPoint> m_lastScrollOffset;

    mutable Lock m_scrollerImpPairLock;
    RetainPtr<NSScrollerImpPair> m_scrollerImpPair;
    RetainPtr<WebScrollerImpPairDelegateMac> m_scrollerImpPairDelegate;
    
    std::atomic<bool> m_usingPresentationValues { false };
};

}

#endif
