/*
 * Copyright (C) 2021 Apple Inc. All rights reserved.
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

#if ENABLE(MOMENTUM_EVENT_DISPATCHER)

#define ENABLE_MOMENTUM_EVENT_DISPATCHER_TEMPORARY_LOGGING 0

#include "ScrollingAccelerationCurve.h"
#include "WebWheelEvent.h"
#include <CyberCore/FloatSize.h>
#include <CyberCore/PageIdentifier.h>
#include <CyberCore/RectEdges.h>
#include <memory>
#include <wtf/Deque.h>
#include <wtf/MonotonicTime.h>
#include <wtf/Noncopyable.h>

namespace CyberCore {
struct DisplayUpdate;
using FramesPerSecond = unsigned;
using PlatformDisplayID = uint32_t;
}

namespace CyberKit {

class MomentumEventDispatcher {
    WTF_MAKE_NONCOPYABLE(MomentumEventDispatcher);
    WTF_MAKE_FAST_ALLOCATED;
public:
    class Client {
    friend class MomentumEventDispatcher;
    public:
        virtual ~Client() = default;

    private:
        virtual void handleSyntheticWheelEvent(CyberCore::PageIdentifier, const WebWheelEvent&, CyberCore::RectEdges<bool> rubberBandableEdges) = 0;
        
        virtual void startDisplayDidRefreshCallbacks(CyberCore::PlatformDisplayID) = 0;
        virtual void stopDisplayDidRefreshCallbacks(CyberCore::PlatformDisplayID) = 0;

#if ENABLE(MOMENTUM_EVENT_DISPATCHER_TEMPORARY_LOGGING)
        virtual void flushMomentumEventLoggingSoon() = 0;
#endif
    };

    MomentumEventDispatcher(Client&);
    ~MomentumEventDispatcher();

    bool handleWheelEvent(CyberCore::PageIdentifier, const WebWheelEvent&, CyberCore::RectEdges<bool> rubberBandableEdges);

    void setScrollingAccelerationCurve(CyberCore::PageIdentifier, std::optional<ScrollingAccelerationCurve>);

    void displayDidRefresh(CyberCore::PlatformDisplayID);

    void pageScreenDidChange(CyberCore::PageIdentifier, CyberCore::PlatformDisplayID, std::optional<unsigned> nominalFramesPerSecond);

#if ENABLE(MOMENTUM_EVENT_DISPATCHER_TEMPORARY_LOGGING)
    void flushLog();
#endif

private:
    void didStartMomentumPhase(CyberCore::PageIdentifier, const WebWheelEvent&);
    void didEndMomentumPhase();

    bool eventShouldStartSyntheticMomentumPhase(CyberCore::PageIdentifier, const WebWheelEvent&) const;

    std::optional<ScrollingAccelerationCurve> scrollingAccelerationCurveForPage(CyberCore::PageIdentifier) const;

    void startDisplayLink();
    void stopDisplayLink();

    struct DisplayProperties {
        CyberCore::PlatformDisplayID displayID;
        CyberCore::FramesPerSecond nominalFrameRate;
    };
    std::optional<DisplayProperties> displayProperties(CyberCore::PageIdentifier) const;

    void dispatchSyntheticMomentumEvent(WebWheelEvent::Phase, CyberCore::FloatSize delta);

    void buildOffsetTableWithInitialDelta(CyberCore::FloatSize);
    void equalizeTailGaps();

    // Once consumed, this delta *must* be dispatched in an event.
    std::optional<CyberCore::FloatSize> consumeDeltaForCurrentTime();

    CyberCore::FloatSize offsetAtTime(Seconds);
    std::pair<CyberCore::FloatSize, CyberCore::FloatSize> computeNextDelta(CyberCore::FloatSize currentUnacceleratedDelta);

    void didReceiveScrollEventWithInterval(CyberCore::FloatSize, Seconds);
    void didReceiveScrollEvent(const WebWheelEvent&);

#if ENABLE(MOMENTUM_EVENT_DISPATCHER_TEMPORARY_LOGGING)
    void pushLogEntry(uint32_t generatedPhase, uint32_t eventPhase);

    CyberCore::FloatSize m_lastActivePhaseDelta;

    struct LogEntry {
        MonotonicTime time;

        float totalGeneratedOffset { 0 };
        float totalEventOffset { 0 };

        uint32_t generatedPhase { 0 };
        uint32_t eventPhase { 0 };
    };
    LogEntry m_currentLogState;
    Vector<LogEntry> m_log;
#endif

    struct Delta {
        float rawPlatformDelta;
        Seconds frameInterval;
    };
    static constexpr unsigned deltaHistoryQueueSize = 9;
    typedef Deque<Delta, deltaHistoryQueueSize> HistoricalDeltas;
    HistoricalDeltas m_deltaHistoryX;
    HistoricalDeltas m_deltaHistoryY;

    Markable<WallTime> m_lastScrollTimestamp;
    std::optional<WebWheelEvent> m_lastIncomingEvent;
    CyberCore::RectEdges<bool> m_lastRubberBandableEdges;
    bool m_isInOverriddenPlatformMomentumGesture { false };

    struct {
        bool active { false };

        CyberCore::PageIdentifier pageIdentifier;
        std::optional<ScrollingAccelerationCurve> accelerationCurve;
        std::optional<WebWheelEvent> initiatingEvent;

        CyberCore::FloatSize currentOffset;
        MonotonicTime startTime;

        Vector<CyberCore::FloatSize> offsetTable; // Always at 60Hz intervals.
        Vector<CyberCore::FloatSize> tailDeltaTable; // Always at event dispatch intervals.
        Seconds tailStartDelay;
        unsigned currentTailDeltaIndex { 0 };

        CyberCore::FramesPerSecond displayNominalFrameRate { 0 };

#if ENABLE(MOMENTUM_EVENT_DISPATCHER_TEMPORARY_LOGGING)
        CyberCore::FloatSize accumulatedEventOffset;
        bool didLogInitialQueueState { false };
#endif

#if ENABLE(MOMENTUM_EVENT_DISPATCHER_PREMATURE_ROUNDING)
        CyberCore::FloatSize carryOffset;
#endif
    } m_currentGesture;

    HashMap<CyberCore::PageIdentifier, DisplayProperties> m_displayProperties;

    mutable Lock m_accelerationCurvesLock;
    HashMap<CyberCore::PageIdentifier, std::optional<ScrollingAccelerationCurve>> m_accelerationCurves WTF_GUARDED_BY_LOCK(m_accelerationCurvesLock);
    Client& m_client;
};

} // namespace CyberKit

#endif
