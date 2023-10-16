/*
 * Copyright (C) 2023 Apple Inc. All rights reserved.
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

#if ENABLE(GAMEPAD) && PLATFORM(COCOA)

#import <CoreHaptics/CoreHaptics.h>
#import <wtf/SoftLinking.h>

SOFT_LINK_FRAMEWORK_FOR_HEADER(CyberCore, CoreHaptics)

SOFT_LINK_CLASS_FOR_HEADER(CyberCore, CHHapticEngine)
SOFT_LINK_CLASS_FOR_HEADER(CyberCore, CHHapticPattern)

SOFT_LINK_CONSTANT_FOR_HEADER(CyberCore, CoreHaptics, CHHapticPatternKeyEvent, NSString *);
#define CHHapticPatternKeyEvent CyberCore::get_CoreHaptics_CHHapticPatternKeyEvent()
SOFT_LINK_CONSTANT_FOR_HEADER(CyberCore, CoreHaptics, CHHapticPatternKeyEventType, NSString *);
#define CHHapticPatternKeyEventType CyberCore::get_CoreHaptics_CHHapticPatternKeyEventType()
SOFT_LINK_CONSTANT_FOR_HEADER(CyberCore, CoreHaptics, CHHapticEventTypeHapticTransient, NSString *);
#define CHHapticEventTypeHapticTransient CyberCore::get_CoreHaptics_CHHapticEventTypeHapticTransient()
SOFT_LINK_CONSTANT_FOR_HEADER(CyberCore, CoreHaptics, CHHapticEventTypeHapticContinuous, NSString *);
#define CHHapticEventTypeHapticContinuous CyberCore::get_CoreHaptics_CHHapticEventTypeHapticContinuous()
SOFT_LINK_CONSTANT_FOR_HEADER(CyberCore, CoreHaptics, CHHapticPatternKeyTime, NSString *);
#define CHHapticPatternKeyTime CyberCore::get_CoreHaptics_CHHapticPatternKeyTime()
SOFT_LINK_CONSTANT_FOR_HEADER(CyberCore, CoreHaptics, CHHapticPatternKeyEventParameters, NSString *);
#define CHHapticPatternKeyEventParameters CyberCore::get_CoreHaptics_CHHapticPatternKeyEventParameters()
SOFT_LINK_CONSTANT_FOR_HEADER(CyberCore, CoreHaptics, CHHapticPatternKeyParameterID, NSString *);
#define CHHapticPatternKeyParameterID CyberCore::get_CoreHaptics_CHHapticPatternKeyParameterID()
SOFT_LINK_CONSTANT_FOR_HEADER(CyberCore, CoreHaptics, CHHapticPatternKeyParameterValue, NSString *);
#define CHHapticPatternKeyParameterValue CyberCore::get_CoreHaptics_CHHapticPatternKeyParameterValue()
SOFT_LINK_CONSTANT_FOR_HEADER(CyberCore, CoreHaptics, CHHapticEventParameterIDHapticIntensity, NSString *);
#define CHHapticEventParameterIDHapticIntensity CyberCore::get_CoreHaptics_CHHapticEventParameterIDHapticIntensity()
SOFT_LINK_CONSTANT_FOR_HEADER(CyberCore, CoreHaptics, CHHapticPatternKeyEventDuration, NSString *);
#define CHHapticPatternKeyEventDuration CyberCore::get_CoreHaptics_CHHapticPatternKeyEventDuration()
SOFT_LINK_CONSTANT_FOR_HEADER(CyberCore, CoreHaptics, CHHapticPatternKeyPattern, NSString *);
#define CHHapticPatternKeyPattern CyberCore::get_CoreHaptics_CHHapticPatternKeyPattern()

#endif // ENABLE(GAMEPAD) && PLATFORM(COCOA)
