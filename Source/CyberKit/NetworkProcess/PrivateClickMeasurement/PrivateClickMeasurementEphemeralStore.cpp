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

#include "config.h"
#include "PrivateClickMeasurementEphemeralStore.h"

#include "PrivateClickMeasurementDebugInfo.h"
#include "PrivateClickMeasurementManagerInterface.h"
#include <CyberCore/PrivateClickMeasurement.h>

namespace CyberKit::PCM {

EphemeralStore::EphemeralStore() = default;
EphemeralStore::~EphemeralStore() = default;

void EphemeralStore::insertPrivateClickMeasurement(CyberCore::PrivateClickMeasurement&& attribution, PrivateClickMeasurementAttributionType type, CompletionHandler<void()>&& completionHandler)
{
    ASSERT(attribution.isEphemeral() == CyberCore::PCM::AttributionEphemeral::Yes);
    ASSERT_UNUSED(type, type == PrivateClickMeasurementAttributionType::Unattributed);
    m_clickMeasurement = WTFMove(attribution);
    completionHandler();
}

void EphemeralStore::markAllUnattributedPrivateClickMeasurementAsExpiredForTesting()
{
    if (m_clickMeasurement && !m_clickMeasurement->attributionTriggerData())
        reset();
}

void EphemeralStore::attributePrivateClickMeasurement(CyberCore::PCM::SourceSite&& sourceSite, CyberCore::PCM::AttributionDestinationSite&& destinationSite, const ApplicationBundleIdentifier& applicationBundleIdentifier, CyberCore::PCM::AttributionTriggerData&& attributionTriggerData, CyberCore::PrivateClickMeasurement::IsRunningLayoutTest isLayoutTest, CompletionHandler<void(std::optional<CyberCore::PCM::AttributionSecondsUntilSendData>&&, DebugInfo&&)>&& completionHandler)
{
    DebugInfo debugInfo;
    if (!m_clickMeasurement)
        return completionHandler(std::nullopt, WTFMove(debugInfo));

    if (m_clickMeasurement->sourceSite() != sourceSite || m_clickMeasurement->destinationSite() != destinationSite)
        return completionHandler(std::nullopt, WTFMove(debugInfo));

    if (!applicationBundleIdentifier.isEmpty() && m_clickMeasurement->sourceApplicationBundleID() != applicationBundleIdentifier)
        return completionHandler(std::nullopt, WTFMove(debugInfo));

    completionHandler(m_clickMeasurement->attributeAndGetEarliestTimeToSend(WTFMove(attributionTriggerData), isLayoutTest), WTFMove(debugInfo));
}

void EphemeralStore::privateClickMeasurementToStringForTesting(CompletionHandler<void(String)>&& completionHandler) const
{
    if (!m_clickMeasurement)
        return completionHandler("\nNo ephemeral Private Click Measurement data.\n"_s);

    StringBuilder builder;
    builder.append("\nEphemeral Private Click Measurement:\n"_s);
    builder.append("SourceSite: "_s, m_clickMeasurement->sourceSite().registrableDomain.string(), "\n"_s);
    builder.append("DestinationSite: "_s, m_clickMeasurement->destinationSite().registrableDomain.string(), "\n"_s);
    builder.append("SourceID: "_s, m_clickMeasurement->sourceID(), "\n"_s);
    if (auto trigger = m_clickMeasurement->attributionTriggerData()) {
        builder.append("Trigger data: "_s, trigger->data, "\n"_s);
        builder.append("Trigger priority: "_s, trigger->priority, "\n"_s);
    }
    return completionHandler(builder.toString());
}

void EphemeralStore::allAttributedPrivateClickMeasurement(CompletionHandler<void(Vector<CyberCore::PrivateClickMeasurement>&&)>&& completionHandler)
{
    if (m_clickMeasurement && m_clickMeasurement->attributionTriggerData())
        completionHandler({ *m_clickMeasurement });
    else
        completionHandler({ });
}

void EphemeralStore::markAttributedPrivateClickMeasurementsAsExpiredForTesting(CompletionHandler<void()>&& completionHandler)
{
    if (m_clickMeasurement && m_clickMeasurement->attributionTriggerData())
        reset();
    completionHandler();
}

void EphemeralStore::clearExpiredPrivateClickMeasurement()
{
    if (!m_clickMeasurement)
        return;

    if (WallTime::now() - m_clickMeasurement->timeOfAdClick() <= CyberCore::PrivateClickMeasurement::maxAge())
        return;

    reset();
}

void EphemeralStore::clearPrivateClickMeasurement(CompletionHandler<void()>&& completionHandler)
{
    reset();
    completionHandler();
}

void EphemeralStore::reset()
{
    m_clickMeasurement = std::nullopt;
}

void EphemeralStore::clearPrivateClickMeasurementForRegistrableDomain(CyberCore::RegistrableDomain&& domain, CompletionHandler<void()>&& completionHandler)
{
    if (!m_clickMeasurement)
        return completionHandler();

    if (m_clickMeasurement->sourceSite().registrableDomain == domain || m_clickMeasurement->destinationSite().registrableDomain == domain)
        m_clickMeasurement = std::nullopt;

    completionHandler();
}

void EphemeralStore::clearSentAttribution(CyberCore::PrivateClickMeasurement&& attributionToClear, CyberCore::PCM::AttributionReportEndpoint endpoint)
{
    auto timesToSend = attributionToClear.timesToSend();
    switch (endpoint) {
    case CyberCore::PCM::AttributionReportEndpoint::Source:
        timesToSend.sourceEarliestTimeToSend = std::nullopt;
        break;
    case CyberCore::PCM::AttributionReportEndpoint::Destination:
        timesToSend.destinationEarliestTimeToSend = std::nullopt;
        break;
    }

    if (!timesToSend.attributionReportEndpoint()) {
        m_clickMeasurement = std::nullopt;
        return;
    }

    attributionToClear.setTimesToSend(WTFMove(timesToSend));
    m_clickMeasurement = WTFMove(attributionToClear);
}

void EphemeralStore::close(CompletionHandler<void()>&& completionHandler)
{
    reset();
    completionHandler();
}

} // namespace CyberKit::PCM
