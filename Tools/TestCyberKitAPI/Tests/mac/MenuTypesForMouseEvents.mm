/*
 * Copyright (C) 2015 Apple Inc. All rights reserved.
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

#import "PlatformUtilities.h"
#import <Carbon/Carbon.h> // For GetCurrentEventTime
#import <CyberCore/PlatformEventFactoryMac.h>
#import <pal/spi/mac/NSMenuSPI.h>
#import <wtf/RetainPtr.h>

namespace TestCyberKitAPI {

static bool canCallMenuTypeForEvent()
{
    return [NSMenu respondsToSelector:@selector(menuTypeForEvent:)];
}

static void buildAndPerformTest(NSEventType buttonEvent, NSEventModifierFlags modifierFlags, CyberCore::MouseButton expectedButton, NSMenuType expectedMenu)
{
    @autoreleasepool {
        auto webView = adoptNS([[WebView alloc] init]);
        NSEvent *event = [NSEvent mouseEventWithType:buttonEvent
                                            location:NSMakePoint(100, 100)
                                       modifierFlags:modifierFlags
                                           timestamp:GetCurrentEventTime()
                                        windowNumber:[[webView window] windowNumber]
                                             context:[NSGraphicsContext currentContext]
                                         eventNumber:0
                                          clickCount:0
                                            pressure:0];

        auto pme = CyberCore::PlatformEventFactory::createPlatformMouseEvent(event, nil, webView.get());

        EXPECT_EQ(expectedButton, pme.button());
        EXPECT_TRUE(!modifierFlags || pme.modifierFlags() & modifierFlags);
        EXPECT_EQ(expectedMenu, pme.menuTypeForEvent());
        if (canCallMenuTypeForEvent())
            EXPECT_EQ(expectedMenu, [NSMenu menuTypeForEvent:event]);
    }
}

TEST(CyberKitLegacy, MenuAndButtonForNormalLeftClick)
{
    buildAndPerformTest(NSEventTypeLeftMouseDown, 0, CyberCore::LeftButton, NSMenuTypeNone);
}

TEST(CyberKitLegacy, MenuAndButtonForNormalRightClick)
{
    buildAndPerformTest(NSEventTypeRightMouseDown, 0, CyberCore::RightButton, NSMenuTypeContextMenu);
}

TEST(CyberKitLegacy, MenuAndButtonForNormalMiddleClick)
{
    buildAndPerformTest(NSEventTypeOtherMouseDown, 0, CyberCore::MiddleButton, NSMenuTypeNone);
}

TEST(CyberKitLegacy, MenuAndButtonForControlLeftClick)
{
    buildAndPerformTest(NSEventTypeLeftMouseDown, NSEventModifierFlagControl, CyberCore::LeftButton, NSMenuTypeContextMenu);
}

TEST(CyberKitLegacy, MenuAndButtonForControlRightClick)
{
    buildAndPerformTest(NSEventTypeRightMouseDown, NSEventModifierFlagControl, CyberCore::RightButton, NSMenuTypeContextMenu);
}

TEST(CyberKitLegacy, MenuAndButtonForControlMiddleClick)
{
    buildAndPerformTest(NSEventTypeOtherMouseDown, NSEventModifierFlagControl, CyberCore::MiddleButton, NSMenuTypeNone);
}
    
TEST(CyberKitLegacy, MenuAndButtonForShiftLeftClick)
{
    buildAndPerformTest(NSEventTypeLeftMouseDown, NSEventModifierFlagShift, CyberCore::LeftButton, NSMenuTypeNone);
}

TEST(CyberKitLegacy, MenuAndButtonForShiftRightClick)
{
    buildAndPerformTest(NSEventTypeRightMouseDown, NSEventModifierFlagShift, CyberCore::RightButton, NSMenuTypeContextMenu);
}

TEST(CyberKitLegacy, MenuAndButtonForShiftMiddleClick)
{
    buildAndPerformTest(NSEventTypeOtherMouseDown, NSEventModifierFlagShift, CyberCore::MiddleButton, NSMenuTypeNone);
}

TEST(CyberKitLegacy, MenuAndButtonForCommandLeftClick)
{
    buildAndPerformTest(NSEventTypeLeftMouseDown, NSEventModifierFlagCommand, CyberCore::LeftButton, NSMenuTypeNone);
}

TEST(CyberKitLegacy, MenuAndButtonForCommandRightClick)
{
    buildAndPerformTest(NSEventTypeRightMouseDown, NSEventModifierFlagCommand, CyberCore::RightButton, NSMenuTypeContextMenu);
}

TEST(CyberKitLegacy, MenuAndButtonForCommandMiddleClick)
{
    buildAndPerformTest(NSEventTypeOtherMouseDown, NSEventModifierFlagCommand, CyberCore::MiddleButton, NSMenuTypeNone);
}

TEST(CyberKitLegacy, MenuAndButtonForAltLeftClick)
{
    buildAndPerformTest(NSEventTypeLeftMouseDown, NSEventModifierFlagOption, CyberCore::LeftButton, NSMenuTypeNone);
}

TEST(CyberKitLegacy, MenuAndButtonForAltRightClick)
{
    buildAndPerformTest(NSEventTypeRightMouseDown, NSEventModifierFlagOption, CyberCore::RightButton, NSMenuTypeContextMenu);
}

TEST(CyberKitLegacy, MenuAndButtonForAltMiddleClick)
{
    buildAndPerformTest(NSEventTypeOtherMouseDown, NSEventModifierFlagOption, CyberCore::MiddleButton, NSMenuTypeNone);
}


} // namespace TestCyberKitAPI
