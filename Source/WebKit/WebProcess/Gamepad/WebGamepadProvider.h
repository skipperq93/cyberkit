/*
 * Copyright (C) 2016 Apple Inc. All rights reserved.
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

#if ENABLE(GAMEPAD)

#include <CyberCore/GamepadProvider.h>
#include <wtf/Forward.h>
#include <wtf/HashSet.h>
#include <wtf/Vector.h>

namespace CyberCore {
enum class EventMakesGamepadsVisible : bool;
}

namespace CyberKit {

class SharedMemory;
class WebGamepad;

class GamepadData;

class WebGamepadProvider final : public CyberCore::GamepadProvider {
public:
    static WebGamepadProvider& singleton();

    void gamepadConnected(const GamepadData&, CyberCore::EventMakesGamepadsVisible);
    void gamepadDisconnected(unsigned index);
    void gamepadActivity(const Vector<GamepadData>&, CyberCore::EventMakesGamepadsVisible);

    void setInitialGamepads(const Vector<GamepadData>&);

private:
    friend NeverDestroyed<WebGamepadProvider>;
    WebGamepadProvider();
    ~WebGamepadProvider() final;
    
    void startMonitoringGamepads(CyberCore::GamepadProviderClient&) final;
    void stopMonitoringGamepads(CyberCore::GamepadProviderClient&) final;
    const Vector<CyberCore::PlatformGamepad*>& platformGamepads() final;
    void playEffect(unsigned gamepadIndex, const String& gamepadID, CyberCore::GamepadHapticEffectType, const CyberCore::GamepadEffectParameters&, CompletionHandler<void(bool)>&&) final;
    void stopEffects(unsigned gamepadIndex, const String& gamepadID, CompletionHandler<void()>&&) final;

    HashSet<CyberCore::GamepadProviderClient*> m_clients;

    Vector<std::unique_ptr<WebGamepad>> m_gamepads;
    Vector<CyberCore::PlatformGamepad*> m_rawGamepads;
};

} // namespace CyberKit

#endif // ENABLE(GAMEPAD)
