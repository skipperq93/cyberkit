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

#include "PrivateClickMeasurementStore.h"

namespace CyberKit {

enum class PrivateClickMeasurementAttributionType : bool;

namespace PCM {

class Database;
struct DebugInfo;

class EphemeralStore : public Store {
public:
    static Ref<Store> create()
    {
        return adoptRef(*new EphemeralStore);
    }

    ~EphemeralStore();

    using ApplicationBundleIdentifier = String;

    void insertPrivateClickMeasurement(CyberCore::PrivateClickMeasurement&&, CyberKit::PrivateClickMeasurementAttributionType, CompletionHandler<void()>&&) final;
    void attributePrivateClickMeasurement(CyberCore::PCM::SourceSite&&, CyberCore::PCM::AttributionDestinationSite&&, const ApplicationBundleIdentifier&, CyberCore::PCM::AttributionTriggerData&&, CyberCore::PrivateClickMeasurement::IsRunningLayoutTest, CompletionHandler<void(std::optional<CyberCore::PCM::AttributionSecondsUntilSendData>&&, DebugInfo&&)>&&) final;

    void privateClickMeasurementToStringForTesting(CompletionHandler<void(String)>&&) const final;
    void markAllUnattributedPrivateClickMeasurementAsExpiredForTesting() final;
    void markAttributedPrivateClickMeasurementsAsExpiredForTesting(CompletionHandler<void()>&&) final;

    void allAttributedPrivateClickMeasurement(CompletionHandler<void(Vector<CyberCore::PrivateClickMeasurement>&&)>&&) final;
    void clearExpiredPrivateClickMeasurement() final;
    void clearPrivateClickMeasurement(CompletionHandler<void()>&&) final;
    void clearPrivateClickMeasurementForRegistrableDomain(CyberCore::RegistrableDomain&&, CompletionHandler<void()>&&) final;
    void clearSentAttribution(CyberCore::PrivateClickMeasurement&& attributionToClear, CyberCore::PCM::AttributionReportEndpoint) final;

    void close(CompletionHandler<void()>&&) final;

private:
    EphemeralStore();

    void reset();

    std::optional<CyberCore::PrivateClickMeasurement> m_clickMeasurement;
};

} // namespace PCM

} // namespace CyberKit
