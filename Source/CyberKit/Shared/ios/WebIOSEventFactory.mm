/*
 * Copyright (C) 2013-2022 Apple Inc. All rights reserved.
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

#import "config.h"
#import "WebIOSEventFactory.h"

#if PLATFORM(IOS_FAMILY)

#import "UIKitSPI.h"
#import <CyberCore/KeyEventCodesIOS.h>
#import <CyberCore/PlatformEventFactoryIOS.h>
#import <CyberCore/Scrollbar.h>

OptionSet<CyberKit::WebEventModifier> WebIOSEventFactory::webEventModifiersForUIKeyModifierFlags(UIKeyModifierFlags modifierFlags)
{
    OptionSet<CyberKit::WebEventModifier> modifiers;
    if (modifierFlags & UIKeyModifierShift)
        modifiers.add(CyberKit::WebEventModifier::ShiftKey);
    if (modifierFlags & UIKeyModifierControl)
        modifiers.add(CyberKit::WebEventModifier::ControlKey);
    if (modifierFlags & UIKeyModifierAlternate)
        modifiers.add(CyberKit::WebEventModifier::AltKey);
    if (modifierFlags & UIKeyModifierCommand)
        modifiers.add(CyberKit::WebEventModifier::MetaKey);
    if (modifierFlags & UIKeyModifierAlphaShift)
        modifiers.add(CyberKit::WebEventModifier::CapsLockKey);
    return modifiers;
}

UIKeyModifierFlags WebIOSEventFactory::toUIKeyModifierFlags(OptionSet<CyberKit::WebEventModifier> modifiers)
{
    UIKeyModifierFlags modifierFlags = 0;
    if (modifiers.contains(CyberKit::WebEventModifier::ShiftKey))
        modifierFlags |= UIKeyModifierShift;
    if (modifiers.contains(CyberKit::WebEventModifier::ControlKey))
        modifierFlags |= UIKeyModifierControl;
    if (modifiers.contains(CyberKit::WebEventModifier::AltKey))
        modifierFlags |= UIKeyModifierAlternate;
    if (modifiers.contains(CyberKit::WebEventModifier::MetaKey))
        modifierFlags |= UIKeyModifierCommand;
    if (modifiers.contains(CyberKit::WebEventModifier::CapsLockKey))
        modifierFlags |= UIKeyModifierAlphaShift;
    return modifierFlags;
}

static OptionSet<CyberKit::WebEventModifier> modifiersForEvent(::WebEvent *event)
{
    OptionSet<CyberKit::WebEventModifier> modifiers;
    WebEventFlags eventModifierFlags = event.modifierFlags;
    if (eventModifierFlags & WebEventFlagMaskShiftKey)
        modifiers.add(CyberKit::WebEventModifier::ShiftKey);
    if (eventModifierFlags & WebEventFlagMaskControlKey)
        modifiers.add(CyberKit::WebEventModifier::ControlKey);
    if (eventModifierFlags & WebEventFlagMaskOptionKey)
        modifiers.add(CyberKit::WebEventModifier::AltKey);
    if (eventModifierFlags & WebEventFlagMaskCommandKey)
        modifiers.add(CyberKit::WebEventModifier::MetaKey);
    if (eventModifierFlags & WebEventFlagMaskLeftCapsLockKey)
        modifiers.add(CyberKit::WebEventModifier::CapsLockKey);
    return modifiers;
}

CyberKit::WebKeyboardEvent WebIOSEventFactory::createWebKeyboardEvent(::WebEvent *event, bool handledByInputMethod)
{
    CyberKit::WebEventType type = (event.type == WebEventKeyUp) ? CyberKit::WebEventType::KeyUp : CyberKit::WebEventType::KeyDown;
    String text;
    String unmodifiedText;
    bool autoRepeat;
    if (event.keyboardFlags & WebEventKeyboardInputModifierFlagsChanged) {
        text = emptyString();
        unmodifiedText = emptyString();
        autoRepeat = false;
    } else {
        text = event.characters;
        unmodifiedText = event.charactersIgnoringModifiers;
        autoRepeat = event.isKeyRepeating;
    }
    String key = CyberCore::keyForKeyEvent(event);
    String code = CyberCore::codeForKeyEvent(event);
    String keyIdentifier = CyberCore::keyIdentifierForKeyEvent(event);
    int windowsVirtualKeyCode = CyberCore::windowsKeyCodeForKeyEvent(event);
    // FIXME: This is not correct. WebEvent.keyCode represents the Windows native virtual key code.
    int nativeVirtualKeyCode = event.keyCode;
    int macCharCode = 0;
    bool isKeypad = false;
    bool isSystemKey = false;
    auto modifiers = modifiersForEvent(event);
    double timestamp = event.timestamp;

    if (windowsVirtualKeyCode == '\r') {
        text = "\r"_s;
        unmodifiedText = text;
    }

    // The adjustments below are only needed in backward compatibility mode, but we cannot tell what mode we are in from here.

    // Turn 0x7F into 8, because backspace needs to always be 8.
    if (text == "\x7F"_s)
        text = "\x8"_s;
    if (unmodifiedText == "\x7F"_s)
        unmodifiedText = "\x8"_s;
    // Always use 9 for tab.
    if (windowsVirtualKeyCode == 9) {
        text = "\x9"_s;
        unmodifiedText = text;
    }

    return CyberKit::WebKeyboardEvent { { type, modifiers, WallTime::fromRawSeconds(timestamp) }, text, unmodifiedText, key, code, keyIdentifier, windowsVirtualKeyCode, nativeVirtualKeyCode, macCharCode, handledByInputMethod, autoRepeat, isKeypad, isSystemKey };
}

CyberKit::WebMouseEvent WebIOSEventFactory::createWebMouseEvent(::WebEvent *event)
{
    // This currently only supports synthetic mouse moved events with no button pressed.
    ASSERT_ARG(event, event.type == WebEventMouseMoved);

    auto type = CyberKit::WebEventType::MouseMove;
    auto button = CyberKit::WebMouseEventButton::NoButton;
    unsigned short buttons = 0;
    auto position = CyberCore::IntPoint(event.locationInWindow);
    float deltaX = 0;
    float deltaY = 0;
    float deltaZ = 0;
    int clickCount = 0;
    double timestamp = event.timestamp;

    return CyberKit::WebMouseEvent({ type, OptionSet<CyberKit::WebEventModifier> { }, WallTime::fromRawSeconds(timestamp) }, button, buttons, position, position, deltaX, deltaY, deltaZ, clickCount);
}

#if HAVE(UISCROLLVIEW_ASYNCHRONOUS_SCROLL_EVENT_HANDLING)
static CyberKit::WebWheelEvent::Phase toWebPhase(UIScrollPhase phase)
{
    switch (phase) {
    case UIScrollPhaseNone:
        return CyberKit::WebWheelEvent::PhaseNone;
    case UIScrollPhaseMayBegin:
        return CyberKit::WebWheelEvent::PhaseMayBegin;
    case UIScrollPhaseBegan:
        return CyberKit::WebWheelEvent::PhaseBegan;
    case UIScrollPhaseChanged:
        return CyberKit::WebWheelEvent::PhaseChanged;
    case UIScrollPhaseEnded:
        return CyberKit::WebWheelEvent::PhaseEnded;
    case UIScrollPhaseCancelled:
        return CyberKit::WebWheelEvent::PhaseCancelled;
    default:
        ASSERT_NOT_REACHED();
        return CyberKit::WebWheelEvent::PhaseNone;
    }
}

CyberKit::WebWheelEvent WebIOSEventFactory::createWebWheelEvent(UIScrollEvent *event, UIView *contentView, std::optional<CyberKit::WebWheelEvent::Phase> overridePhase)
{
    CyberCore::IntPoint scrollLocation = CyberCore::roundedIntPoint([event locationInView:contentView]);
    CGVector deltaVector = [event _adjustedAcceleratedDeltaInView:contentView];
    CyberCore::FloatSize delta(deltaVector.dx, deltaVector.dy);
    CyberCore::FloatSize wheelTicks = delta;
    wheelTicks.scale(1. / static_cast<float>(CyberCore::Scrollbar::pixelsPerLineStep()));
    auto timestamp = MonotonicTime::fromRawSeconds(event.timestamp).approximateWallTime();
    return {
        { CyberKit::WebEventType::Wheel, OptionSet<CyberKit::WebEventModifier> { }, timestamp },
        scrollLocation,
        scrollLocation,
        delta,
        wheelTicks,
        CyberKit::WebWheelEvent::Granularity::ScrollByPixelWheelEvent,
        false,
        overridePhase.value_or(toWebPhase(event.phase)),
        CyberKit::WebWheelEvent::PhaseNone,
        true,
        1,
        delta,
        timestamp,
        { },
        CyberKit::WebWheelEvent::MomentumEndType::Unknown
    };
}
#endif

#endif // PLATFORM(IOS_FAMILY)
