/*
 * Copyright (C) 2011, 2014 Apple Inc. All rights reserved.
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

#include "DisplayLinkObserverID.h"
#include "MessageReceiver.h"
#include "MomentumEventDispatcher.h"
#include "WebEvent.h"
#include <CyberCore/PageIdentifier.h>
#include <CyberCore/PlatformWheelEvent.h>
#include <CyberCore/RectEdges.h>
#include <CyberCore/ScrollingCoordinatorTypes.h>
#include <CyberCore/WheelEventDeltaFilter.h>
#include <memory>
#include <wtf/HashMap.h>
#include <wtf/Lock.h>
#include <wtf/Noncopyable.h>
#include <wtf/RefPtr.h>
#include <wtf/ThreadingPrimitives.h>
#include <wtf/WorkQueue.h>

#if ENABLE(MAC_GESTURE_EVENTS)
#include "WebGestureEvent.h"
#endif

namespace CyberCore {
struct DisplayUpdate;
class ThreadedScrollingTree;
using PlatformDisplayID = uint32_t;
}

namespace CyberKit {

class MomentumEventDispatcher;
class ScrollingAccelerationCurve;
class WebPage;
class WebWheelEvent;

#if ENABLE(IOS_TOUCH_EVENTS)
class WebTouchEvent;
#endif

class EventDispatcher final :
#if ENABLE(MOMENTUM_EVENT_DISPATCHER)
    public MomentumEventDispatcher::Client,
#endif
    private IPC::MessageReceiver {
public:
    EventDispatcher();
    ~EventDispatcher();

    enum class WheelEventOrigin : bool { UIProcess, MomentumEventDispatcher };

    WorkQueue& queue() { return m_queue.get(); }

#if ENABLE(SCROLLING_THREAD)
    void addScrollingTreeForPage(WebPage&);
    void removeScrollingTreeForPage(WebPage&);
#endif

#if ENABLE(IOS_TOUCH_EVENTS)
    using TouchEventQueue = Vector<std::pair<WebTouchEvent, CompletionHandler<void(bool)>>, 1>;
    void takeQueuedTouchEventsForPage(const WebPage&, TouchEventQueue&);
#endif

    void initializeConnection(IPC::Connection&);

    void notifyScrollingTreesDisplayDidRefresh(CyberCore::PlatformDisplayID);

private:
    // IPC::MessageReceiver overrides.
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;

    // Message handlers
    void wheelEvent(CyberCore::PageIdentifier, const WebWheelEvent&, CyberCore::RectEdges<bool> rubberBandableEdges);
#if ENABLE(MOMENTUM_EVENT_DISPATCHER)
    void setScrollingAccelerationCurve(CyberCore::PageIdentifier, std::optional<ScrollingAccelerationCurve>);
#endif
#if ENABLE(IOS_TOUCH_EVENTS)
    void touchEvent(CyberCore::PageIdentifier, const WebTouchEvent&, CompletionHandler<void(bool)>&&);
    void touchEventWithoutCallback(CyberCore::PageIdentifier, const WebTouchEvent&);
#endif
#if ENABLE(MAC_GESTURE_EVENTS)
    void gestureEvent(CyberCore::PageIdentifier, const WebGestureEvent&);
#endif

    // This is called on the main thread.
    void dispatchWheelEvent(CyberCore::PageIdentifier, const WebWheelEvent&, OptionSet<CyberCore::WheelEventProcessingSteps>, WheelEventOrigin);
    void dispatchWheelEventViaMainThread(CyberCore::PageIdentifier, const WebWheelEvent&, OptionSet<CyberCore::WheelEventProcessingSteps>, WheelEventOrigin);

    void internalWheelEvent(CyberCore::PageIdentifier, const WebWheelEvent&, CyberCore::RectEdges<bool> rubberBandableEdges, WheelEventOrigin);

#if ENABLE(IOS_TOUCH_EVENTS)
    void dispatchTouchEvents();
#endif
#if ENABLE(MAC_GESTURE_EVENTS)
    void dispatchGestureEvent(CyberCore::PageIdentifier, const WebGestureEvent&);
#endif

    static void sendDidReceiveEvent(CyberCore::PageIdentifier, WebEventType, bool didHandleEvent);

#if PLATFORM(MAC)
    void displayDidRefresh(CyberCore::PlatformDisplayID, const CyberCore::DisplayUpdate&, bool sendToMainThread);
#endif

#if ENABLE(SCROLLING_THREAD)
    void displayDidRefreshOnScrollingThread(CyberCore::PlatformDisplayID);
#endif

#if ENABLE(MOMENTUM_EVENT_DISPATCHER)
    // EventDispatcher::Client
    void handleSyntheticWheelEvent(CyberCore::PageIdentifier, const WebWheelEvent&, CyberCore::RectEdges<bool> rubberBandableEdges) override;
    void startDisplayDidRefreshCallbacks(CyberCore::PlatformDisplayID) override;
    void stopDisplayDidRefreshCallbacks(CyberCore::PlatformDisplayID) override;

#if ENABLE(MOMENTUM_EVENT_DISPATCHER_TEMPORARY_LOGGING)
    void flushMomentumEventLoggingSoon() override;
#endif
#endif

    void pageScreenDidChange(CyberCore::PageIdentifier, CyberCore::PlatformDisplayID, std::optional<unsigned> nominalFramesPerSecond);

    Ref<WorkQueue> m_queue;

#if ENABLE(ASYNC_SCROLLING) && ENABLE(SCROLLING_THREAD)
    Lock m_scrollingTreesLock;
    HashMap<CyberCore::PageIdentifier, RefPtr<CyberCore::ThreadedScrollingTree>> m_scrollingTrees WTF_GUARDED_BY_LOCK(m_scrollingTreesLock);
#endif
    std::unique_ptr<CyberCore::WheelEventDeltaFilter> m_recentWheelEventDeltaFilter;
#if ENABLE(IOS_TOUCH_EVENTS)
    Lock m_touchEventsLock;
    HashMap<CyberCore::PageIdentifier, TouchEventQueue> m_touchEvents WTF_GUARDED_BY_LOCK(m_touchEventsLock);
#endif

#if ENABLE(MOMENTUM_EVENT_DISPATCHER)
    std::unique_ptr<MomentumEventDispatcher> m_momentumEventDispatcher;
    DisplayLinkObserverID m_observerID;
#endif
};

} // namespace CyberKit
