/*
 * Copyright (C) 2019 Apple Inc. All rights reserved.
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

#include "NetworkResourceLoadParameters.h"
#include <CyberCore/AdClickAttribution.h>
#include <CyberCore/RegistrableDomain.h>
#include <CyberCore/ResourceError.h>
#include <CyberCore/ResourceResponse.h>
#include <CyberCore/Timer.h>
#include <wtf/CompletionHandler.h>
#include <wtf/HashMap.h>
#include <wtf/WeakPtr.h>
#include <wtf/text/WTFString.h>

namespace CyberKit {

class AdClickAttributionManager : public CanMakeWeakPtr<AdClickAttributionManager> {
public:

    using RegistrableDomain = CyberCore::RegistrableDomain;
    using AdClickAttribution = CyberCore::AdClickAttribution;
    using Source = CyberCore::AdClickAttribution::Source;
    using Destination = CyberCore::AdClickAttribution::Destination;
    using Conversion = CyberCore::AdClickAttribution::Conversion;

    explicit AdClickAttributionManager(PAL::SessionID sessionID)
        : m_firePendingConversionRequestsTimer(*this, &AdClickAttributionManager::firePendingConversionRequests)
        , m_pingLoadFunction([](NetworkResourceLoadParameters&& params, CompletionHandler<void(const CyberCore::ResourceError&, const CyberCore::ResourceResponse&)>&& completionHandler) {
            UNUSED_PARAM(params);
            completionHandler(CyberCore::ResourceError(), CyberCore::ResourceResponse());
        })
        , m_sessionID(sessionID)
    {
    }

    void storeUnconverted(AdClickAttribution&&);
    void handleConversion(Conversion&&, const URL& requestURL, const CyberCore::ResourceRequest& redirectRequest);
    void clear();
    void clearForRegistrableDomain(const RegistrableDomain&);
    void toString(CompletionHandler<void(String)>&&) const;
    void setPingLoadFunction(Function<void(NetworkResourceLoadParameters&&, CompletionHandler<void(const CyberCore::ResourceError&, const CyberCore::ResourceResponse&)>&&)>&& pingLoadFunction) { m_pingLoadFunction = WTFMove(pingLoadFunction); }
    void setOverrideTimerForTesting(bool value) { m_isRunningTest = value; }
    void setConversionURLForTesting(URL&&);
    void markAllUnconvertedAsExpiredForTesting();

private:
    void startTimer(Seconds);
    void convert(const Source&, const Destination&, Conversion&&);
    void fireConversionRequest(const AdClickAttribution&);
    void firePendingConversionRequests();
    void clearExpired();
    bool debugModeEnabled() const;

    HashMap<std::pair<Source, Destination>, AdClickAttribution> m_unconvertedAdClickAttributionMap;
    HashMap<std::pair<Source, Destination>, AdClickAttribution> m_convertedAdClickAttributionMap;
    CyberCore::Timer m_firePendingConversionRequestsTimer;
    Function<void(NetworkResourceLoadParameters&&, CompletionHandler<void(const CyberCore::ResourceError&, const CyberCore::ResourceResponse&)>&&)> m_pingLoadFunction;
    bool m_isRunningTest { false };
    Optional<URL> m_conversionBaseURLForTesting;
    PAL::SessionID m_sessionID;
};
    
} // namespace CyberKit
