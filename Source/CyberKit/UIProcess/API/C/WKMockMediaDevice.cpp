/*
 * Copyright (C) 2018 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "config.h"
#include "WKMockMediaDevice.h"

#include "WKAPICast.h"
#include "WKDictionary.h"
#include "WKRetainPtr.h"
#include "WKString.h"
#include "WebProcessPool.h"
#include <CyberCore/MockMediaDevice.h>

using namespace CyberKit;

void WKAddMockMediaDevice(WKContextRef context, WKStringRef persistentId, WKStringRef label, WKStringRef type, WKDictionaryRef properties)
{
#if ENABLE(MEDIA_STREAM)
    String typeString = CyberKit::toImpl(type)->string();
    std::variant<CyberCore::MockMicrophoneProperties, CyberCore::MockSpeakerProperties, CyberCore::MockCameraProperties, CyberCore::MockDisplayProperties> deviceProperties;
    if (typeString == "camera"_s) {
        CyberCore::MockCameraProperties cameraProperties;
        if (properties) {
            auto facingModeKey = adoptWK(WKStringCreateWithUTF8CString("facingMode"));
            if (auto facingMode = WKDictionaryGetItemForKey(properties, facingModeKey.get())) {
                if (WKStringIsEqualToUTF8CString(static_cast<WKStringRef>(facingMode), "unknown"))
                    cameraProperties.facingMode = CyberCore::VideoFacingMode::Unknown;
            }
            auto fillColorKey = adoptWK(WKStringCreateWithUTF8CString("fillColor"));
            if (auto fillColor = WKDictionaryGetItemForKey(properties, fillColorKey.get())) {
                if (WKStringIsEqualToUTF8CString(static_cast<WKStringRef>(fillColor), "green"))
                    cameraProperties.fillColor = CyberCore::Color::green;
            }
        }
        deviceProperties = WTFMove(cameraProperties);
    } else if (typeString == "screen"_s)
        deviceProperties = CyberCore::MockDisplayProperties { };
    else if (typeString == "speaker"_s)
        deviceProperties = CyberCore::MockSpeakerProperties { };
    else if (typeString != "microphone"_s)
        return;

    toImpl(context)->addMockMediaDevice({ CyberKit::toImpl(persistentId)->string(), CyberKit::toImpl(label)->string(), false, WTFMove(deviceProperties) });
#endif
}

void WKClearMockMediaDevices(WKContextRef context)
{
    toImpl(context)->clearMockMediaDevices();
}

void WKRemoveMockMediaDevice(WKContextRef context, WKStringRef persistentId)
{
    toImpl(context)->removeMockMediaDevice(CyberKit::toImpl(persistentId)->string());
}

void WKResetMockMediaDevices(WKContextRef context)
{
    toImpl(context)->resetMockMediaDevices();
}

void WKSetMockMediaDeviceIsEphemeral(WKContextRef context, WKStringRef persistentId, bool isEphemeral)
{
    toImpl(context)->setMockMediaDeviceIsEphemeral(CyberKit::toImpl(persistentId)->string(), isEphemeral);
}
