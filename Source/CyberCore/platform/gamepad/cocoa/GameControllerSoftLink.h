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

#pragma once

#if ENABLE(GAMEPAD) && PLATFORM(COCOA)

#import "GameControllerSPI.h"
#import <wtf/SoftLinking.h>

SOFT_LINK_FRAMEWORK_FOR_HEADER(CyberCore, GameController)
SOFT_LINK_CLASS_FOR_HEADER(CyberCore, GCController)
SOFT_LINK_CLASS_FOR_HEADER(CyberCore, GCControllerButtonInput)

SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputButtonA, NSString *)
#define GCInputButtonA CyberCore::get_GameController_GCInputButtonA()
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputButtonB, NSString *)
#define GCInputButtonB CyberCore::get_GameController_GCInputButtonB()
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputButtonX, NSString *)
#define GCInputButtonX CyberCore::get_GameController_GCInputButtonX()
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputButtonY, NSString *)
#define GCInputButtonY CyberCore::get_GameController_GCInputButtonY()
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputButtonHome, NSString *)
#define GCInputButtonHome CyberCore::get_GameController_GCInputButtonHome()
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputButtonMenu, NSString *)
#define GCInputButtonMenu CyberCore::get_GameController_GCInputButtonMenu()
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputButtonOptions, NSString *)
#define GCInputButtonOptions CyberCore::get_GameController_GCInputButtonOptions()
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputDirectionPad, NSString *)
#define GCInputDirectionPad CyberCore::get_GameController_GCInputDirectionPad()
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputLeftShoulder, NSString *)
#define GCInputLeftShoulder CyberCore::get_GameController_GCInputLeftShoulder()
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputLeftTrigger, NSString *)
#define GCInputLeftTrigger CyberCore::get_GameController_GCInputLeftTrigger()
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputLeftThumbstick, NSString *)
#define GCInputLeftThumbstick CyberCore::get_GameController_GCInputLeftThumbstick()
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputLeftThumbstickButton, NSString *)
#define GCInputLeftThumbstickButton CyberCore::get_GameController_GCInputLeftThumbstickButton()
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputRightShoulder, NSString *)
#define GCInputRightShoulder CyberCore::get_GameController_GCInputRightShoulder()
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputRightTrigger, NSString *)
#define GCInputRightTrigger CyberCore::get_GameController_GCInputRightTrigger()
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputRightThumbstick, NSString *)
#define GCInputRightThumbstick CyberCore::get_GameController_GCInputRightThumbstick()
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCInputRightThumbstickButton, NSString *)
#define GCInputRightThumbstickButton CyberCore::get_GameController_GCInputRightThumbstickButton()

SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCControllerDidConnectNotification, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCControllerDidDisconnectNotification, NSString *)

SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCHapticsLocalityLeftHandle, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCHapticsLocalityRightHandle, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCHapticsLocalityLeftTrigger, NSString *)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(CyberCore, GameController, GCHapticsLocalityRightTrigger, NSString *)

#if HAVE(MULTIGAMEPADPROVIDER_SUPPORT)
SOFT_LINK_FUNCTION_FOR_HEADER(CyberCore, GameController, ControllerClassForService, Class, (IOHIDServiceClientRef service), (service))
#endif

#endif // ENABLE(GAMEPAD) && PLATFORM(COCOA)
