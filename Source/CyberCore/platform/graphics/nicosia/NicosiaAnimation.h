/*
 Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies)

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public License
 along with this library; see the file COPYING.LIB.  If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
 */

#pragma once

#include "Animation.h"
#include "GraphicsLayer.h"

namespace CyberCore {
class TransformationMatrix;
}

namespace Nicosia {

class Animation {
public:
    enum class AnimationState { Playing, Paused, Stopped };

    struct ApplicationResult {
        Optional<CyberCore::TransformationMatrix> transform;
        Optional<double> opacity;
        Optional<CyberCore::FilterOperations> filters;
        bool hasRunningAnimations { false };
    };

    Animation()
        : m_keyframes(CyberCore::AnimatedPropertyInvalid)
    { }
    Animation(const String&, const CyberCore::KeyframeValueList&, const CyberCore::FloatSize&, const CyberCore::Animation&, bool, MonotonicTime, Seconds, AnimationState);

    WEBCORE_EXPORT Animation(const Animation&);
    Animation& operator=(const Animation&);
    Animation(Animation&&) = default;
    Animation& operator=(Animation&&) = default;

    void apply(ApplicationResult&, MonotonicTime);
    void applyKeepingInternalState(ApplicationResult&, MonotonicTime);
    void pause(Seconds);
    void resume();
    bool isActive() const;

    const String& name() const { return m_name; }
    const CyberCore::KeyframeValueList& keyframes() const { return m_keyframes; }
    AnimationState state() const { return m_state; }
    CyberCore::TimingFunction* timingFunction() const { return m_timingFunction.get(); }

private:
    void applyInternal(ApplicationResult&, const CyberCore::AnimationValue& from, const CyberCore::AnimationValue& to, float progress);
    Seconds computeTotalRunningTime(MonotonicTime);

    String m_name;
    CyberCore::KeyframeValueList m_keyframes;
    CyberCore::FloatSize m_boxSize;
    RefPtr<CyberCore::TimingFunction> m_timingFunction;
    double m_iterationCount;
    double m_duration;
    CyberCore::Animation::AnimationDirection m_direction;
    bool m_fillsForwards;
    bool m_listsMatch;
    MonotonicTime m_startTime;
    Seconds m_pauseTime;
    Seconds m_totalRunningTime;
    MonotonicTime m_lastRefreshedTime;
    AnimationState m_state;
};

class Animations {
public:
    Animations() = default;

    void add(const Animation&);
    void remove(const String& name);
    void remove(const String& name, CyberCore::AnimatedPropertyID);
    void pause(const String&, Seconds);
    void suspend(MonotonicTime);
    void resume();

    void apply(Animation::ApplicationResult&, MonotonicTime);
    void applyKeepingInternalState(Animation::ApplicationResult&, MonotonicTime);

    bool isEmpty() const { return m_animations.isEmpty(); }
    size_t size() const { return m_animations.size(); }
    const Vector<Animation>& animations() const { return m_animations; }
    Vector<Animation>& animations() { return m_animations; }

    bool hasRunningAnimations() const;
    bool hasActiveAnimationsOfType(CyberCore::AnimatedPropertyID type) const;
    Animations getActiveAnimations() const;

private:
    Vector<Animation> m_animations;
};

}
