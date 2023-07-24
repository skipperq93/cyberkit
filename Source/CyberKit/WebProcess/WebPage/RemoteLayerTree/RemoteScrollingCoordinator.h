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
#include <CyberCore/AsyncScrollingCoordinator.h>
#include <CyberCore/ScrollTypes.h>
#include <CyberCore/ScrollingConstraints.h>
#include <CyberCore/Timer.h>

namespace IPC {
class Decoder;
class Encoder;
}

namespace CyberKit {

class WebPage;
class RemoteScrollingCoordinatorTransaction;
class RemoteScrollingUIState;

class RemoteScrollingCoordinator : public CyberCore::AsyncScrollingCoordinator, public IPC::MessageReceiver {
public:
    static Ref<RemoteScrollingCoordinator> create(WebPage* page)
    {
        return adoptRef(*new RemoteScrollingCoordinator(page));
    }

    void buildTransaction(RemoteScrollingCoordinatorTransaction&);

    void scrollingStateInUIProcessChanged(const RemoteScrollingUIState&);

private:
    RemoteScrollingCoordinator(WebPage*);
    virtual ~RemoteScrollingCoordinator();

    bool isRemoteScrollingCoordinator() const override { return true; }
    
    // ScrollingCoordinator
    bool coordinatesScrollingForFrameView(const CyberCore::FrameView&) const override;
    void scheduleTreeStateCommit() override;

    bool isRubberBandInProgress(CyberCore::ScrollingNodeID) const final;
    bool isUserScrollInProgress(CyberCore::ScrollingNodeID) const final;
#if ENABLE(CSS_SCROLL_SNAP)
    bool isScrollSnapInProgress(CyberCore::ScrollingNodeID) const final;
#endif

    void setScrollPinningBehavior(CyberCore::ScrollPinningBehavior) override;

    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;
    
    // Respond to UI process changes.
    void scrollPositionChangedForNode(CyberCore::ScrollingNodeID, const CyberCore::FloatPoint& scrollPosition, bool syncLayerPosition);
    void currentSnapPointIndicesChangedForNode(CyberCore::ScrollingNodeID, unsigned horizontal, unsigned vertical);

    WebPage* m_webPage;

    HashSet<CyberCore::ScrollingNodeID> m_nodesWithActiveRubberBanding;
    HashSet<CyberCore::ScrollingNodeID> m_nodesWithActiveScrollSnap;
    HashSet<CyberCore::ScrollingNodeID> m_nodesWithActiveUserScrolls;
};

} // namespace CyberKit

SPECIALIZE_TYPE_TRAITS_SCROLLING_COORDINATOR(CyberKit::RemoteScrollingCoordinator, isRemoteScrollingCoordinator());

#endif // ENABLE(ASYNC_SCROLLING)
