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

OBJC_CLASS NSScrollerImpPair;
OBJC_CLASS WKScrollerImpPairDelegate;

namespace CyberCore {
class PlatformMouseEvent;
class PlatformWheelEvent;
class ScrollingTreeScrollingNode;
}

namespace CyberKit {

class ScrollerPairMac {
    WTF_MAKE_FAST_ALLOCATED;
public:
    ScrollerPairMac(CyberCore::ScrollingTreeScrollingNode&);

    ~ScrollerPairMac();

    ScrollerMac& verticalScroller() { return m_verticalScroller; }
    ScrollerMac& horizontalScroller() { return m_horizontalScroller; }

    bool handleWheelEvent(const CyberCore::PlatformWheelEvent&);
    bool handleMouseEvent(const CyberCore::PlatformMouseEvent&);

    void updateValues();

    CyberCore::FloatSize visibleSize() const;
    CyberCore::IntPoint lastKnownMousePosition() const { return m_lastKnownMousePosition; }
    bool useDarkAppearance() const;

    struct Values {
        float value;
        float proportion;
    };
    Values valuesForOrientation(ScrollerMac::Orientation);

    NSScrollerImpPair *scrollerImpPair() { return m_scrollerImpPair.get(); }

private:
    CyberCore::ScrollingTreeScrollingNode& m_scrollingNode;

    ScrollerMac m_verticalScroller;
    ScrollerMac m_horizontalScroller;

    CyberCore::FloatSize m_contentSize;
    CyberCore::FloatRect m_visibleContentRect;

    CyberCore::IntPoint m_lastKnownMousePosition;
    Optional<CyberCore::FloatPoint> m_lastScrollPosition;

    RetainPtr<NSScrollerImpPair> m_scrollerImpPair;
    RetainPtr<WKScrollerImpPairDelegate> m_scrollerImpPairDelegate;
};

}

#endif
