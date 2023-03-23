/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
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

#if ENABLE(GAMEPAD) && PLATFORM(COCOA)

#import "GameControllerSPI.h"
#import <wtf/SoftLinking.h>

SOFT_LINK_FRAMEWORK_FOR_SOURCE(CyberCore, GameController)
SOFT_LINK_CLASS_FOR_SOURCE_OPTIONAL_WITH_EXPORT(CyberCore, GameController, GCController, WEBCORE_EXPORT)
SOFT_LINK_CLASS_FOR_SOURCE_OPTIONAL(CyberCore, GameController, GCControllerButtonInput)

SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputButtonA, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputButtonB, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputButtonX, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputButtonY, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputButtonHome, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputButtonMenu, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputButtonOptions, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputDirectionPad, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputLeftShoulder, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputLeftTrigger, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputLeftThumbstick, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputLeftThumbstickButton, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputRightShoulder, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputRightTrigger, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputRightThumbstick, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCInputRightThumbstickButton, NSString *)

SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCControllerDidConnectNotification, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCControllerDidDisconnectNotification, NSString *)

SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCHapticsLocalityLeftHandle, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCHapticsLocalityRightHandle, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCHapticsLocalityLeftTrigger, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(CyberCore, GameController, GCHapticsLocalityRightTrigger, NSString *)

#if HAVE(MULTIGAMEPADPROVIDER_SUPPORT)
SOFT_LINK_FUNCTION_FOR_SOURCE(CyberCore, GameController, ControllerClassForService, Class, (IOHIDServiceClientRef service), (service))
#endif

#endif // ENABLE(GAMEPAD) && PLATFORM(COCOA)
