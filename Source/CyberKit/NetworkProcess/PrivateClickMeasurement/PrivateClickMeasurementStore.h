/*
 * Copyright (C) 2021 Apple Inc. All rights reserved.
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

#include <CyberCore/PrivateClickMeasurement.h>
#include <wtf/Ref.h>
#include <wtf/ThreadSafeRefCounted.h>

namespace CyberKit {

enum class PrivateClickMeasurementAttributionType : bool;

namespace PCM {

class Database;
struct DebugInfo;

class Store : public ThreadSafeRefCounted<Store> {
public:
    static Ref<Store> create(const String& databaseDirectory)
    {
        return adoptRef(*new Store(databaseDirectory));
    }

    ~Store();

    using ApplicationBundleIdentifier = String;

    static void prepareForProcessToSuspend(CompletionHandler<void()>&&);
    static void processDidResume();

    void insertPrivateClickMeasurement(CyberCore::PrivateClickMeasurement&&, CyberKit::PrivateClickMeasurementAttributionType, CompletionHandler<void()>&&);
    void attributePrivateClickMeasurement(CyberCore::PCM::SourceSite&&, CyberCore::PCM::AttributionDestinationSite&&, const ApplicationBundleIdentifier&, CyberCore::PCM::AttributionTriggerData&&, CyberCore::PrivateClickMeasurement::IsRunningLayoutTest, CompletionHandler<void(std::optional<CyberCore::PCM::AttributionSecondsUntilSendData>&&, DebugInfo&&)>&&);

    void privateClickMeasurementToStringForTesting(CompletionHandler<void(String)>&&) const;
    void markAllUnattributedPrivateClickMeasurementAsExpiredForTesting();
    void markAttributedPrivateClickMeasurementsAsExpiredForTesting(CompletionHandler<void()>&&);

    void allAttributedPrivateClickMeasurement(CompletionHandler<void(Vector<CyberCore::PrivateClickMeasurement>&&)>&&);
    void clearExpiredPrivateClickMeasurement();
    void clearPrivateClickMeasurement(CompletionHandler<void()>&&);
    void clearPrivateClickMeasurementForRegistrableDomain(CyberCore::RegistrableDomain&&, CompletionHandler<void()>&&);
    void clearSentAttribution(CyberCore::PrivateClickMeasurement&& attributionToClear, CyberCore::PCM::AttributionReportEndpoint);

    void close(CompletionHandler<void()>&&);

private:
    Store(const String& databaseDirectory);

    void postTask(Function<void()>&&) const;
    void postTaskReply(Function<void()>&&) const;

    std::unique_ptr<Database> m_database;
    Ref<SuspendableWorkQueue> m_queue;
};

} // namespace PCM

} // namespace CyberKit
