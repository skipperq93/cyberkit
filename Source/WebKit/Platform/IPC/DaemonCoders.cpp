/*
 * Copyright (C) 2022 Apple Inc. All rights reserved.
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
#include "DaemonCoders.h"

#include "DaemonDecoder.h"
#include "DaemonEncoder.h"
#include "PushMessageForTesting.h"
#include "WebPushDaemonConnectionConfiguration.h"
#include "WebPushMessage.h"
#include <CyberCore/CertificateInfo.h>
#include <CyberCore/ExceptionData.h>
#include <CyberCore/PrivateClickMeasurement.h>
#include <CyberCore/PushSubscriptionData.h>
#include <CyberCore/RegistrableDomain.h>
#include <CyberCore/SecurityOriginData.h>

#if PLATFORM(COCOA)
#include <CoreFoundation/CoreFoundation.h>
#include <wtf/spi/cocoa/SecuritySPI.h>
#endif

namespace WebKit::Daemon {

#if ENABLE(SERVICE_WORKER)
void Coder<CyberCore::PushSubscriptionData>::encode(Encoder& encoder, const CyberCore::PushSubscriptionData& instance)
{
    encoder << instance.identifier;
    encoder << instance.endpoint;
    encoder << instance.expirationTime;
    encoder << instance.serverVAPIDPublicKey;
    encoder << instance.clientECDHPublicKey;
    encoder << instance.sharedAuthenticationSecret;
}

std::optional<CyberCore::PushSubscriptionData> Coder<CyberCore::PushSubscriptionData>::decode(Decoder& decoder)
{
    std::optional<CyberCore::PushSubscriptionIdentifier> identifier;
    decoder >> identifier;
    if (!identifier)
        return std::nullopt;

    std::optional<String> endpoint;
    decoder >> endpoint;
    if (!endpoint)
        return std::nullopt;

    std::optional<std::optional<CyberCore::EpochTimeStamp>> expirationTime;
    decoder >> expirationTime;
    if (!expirationTime)
        return std::nullopt;

    std::optional<Vector<uint8_t>> serverVAPIDPublicKey;
    decoder >> serverVAPIDPublicKey;
    if (!serverVAPIDPublicKey)
        return std::nullopt;

    std::optional<Vector<uint8_t>> clientECDHPublicKey;
    decoder >> clientECDHPublicKey;
    if (!clientECDHPublicKey)
        return std::nullopt;

    std::optional<Vector<uint8_t>> sharedAuthenticationSecret;
    decoder >> sharedAuthenticationSecret;
    if (!sharedAuthenticationSecret)
        return std::nullopt;

    return { {
        WTFMove(*identifier),
        WTFMove(*endpoint),
        WTFMove(*expirationTime),
        WTFMove(*serverVAPIDPublicKey),
        WTFMove(*clientECDHPublicKey),
        WTFMove(*sharedAuthenticationSecret),
    } };
}
#endif

void Coder<WTF::WallTime>::encode(Encoder& encoder, const WTF::WallTime& instance)
{
    encoder << instance.secondsSinceEpoch().value();
}

std::optional<WTF::WallTime> Coder<WTF::WallTime>::decode(Decoder& decoder)
{
    std::optional<double> value;
    decoder >> value;
    if (!value)
        return std::nullopt;
    return WallTime::fromRawSeconds(*value);
}

void Coder<CyberCore::CertificateInfo>::encode(Encoder& encoder, const CyberCore::CertificateInfo& instance)
{
#if PLATFORM(COCOA)
    auto data = adoptCF(SecTrustSerialize(instance.trust().get(), nullptr));
    if (!data) {
        encoder << false;
        return;
    }
    encoder << true;
    uint64_t length = CFDataGetLength(data.get());
    encoder << length;
    encoder.encodeFixedLengthData({ CFDataGetBytePtr(data.get()), static_cast<size_t>(length) });
#endif
}

std::optional<CyberCore::CertificateInfo> Coder<CyberCore::CertificateInfo>::decode(Decoder& decoder)
{
#if PLATFORM(COCOA)
    std::optional<bool> hasTrust;
    decoder >> hasTrust;
    if (!hasTrust)
        return std::nullopt;
    if (!*hasTrust)
        return CyberCore::CertificateInfo();
    std::optional<uint64_t> length;
    decoder >> length;
    if (!length)
        return std::nullopt;
    Span<const uint8_t> bytes = decoder.decodeFixedLengthReference(*length);
    if (bytes.size() != *length)
        return std::nullopt;
    auto trust = adoptCF(SecTrustDeserialize(adoptCF(CFDataCreate(nullptr, bytes.data(), bytes.size())).get(), nullptr));
    if (!trust)
        return std::nullopt;
    return CyberCore::CertificateInfo(WTFMove(trust));
#else
    return CyberCore::CertificateInfo();
#endif
}

template<> struct Coder<CyberCore::PCM::SourceSite> {
    static void encode(Encoder& encoder, const CyberCore::PCM::SourceSite& instance)
    {
        encoder << instance.registrableDomain;
    }
    static std::optional<CyberCore::PCM::SourceSite> decode(Decoder& decoder)
    {
        std::optional<CyberCore::RegistrableDomain> registrableDomain;
        decoder >> registrableDomain;
        if (!registrableDomain)
            return std::nullopt;
        return { CyberCore::PCM::SourceSite { WTFMove(*registrableDomain) } };
    }
};

template<> struct Coder<CyberCore::PCM::AttributionDestinationSite> {
    static void encode(Encoder& encoder, const CyberCore::PCM::AttributionDestinationSite& instance)
    {
        encoder << instance.registrableDomain;
    }
    static std::optional<CyberCore::PCM::AttributionDestinationSite> decode(Decoder& decoder)
    {
        std::optional<CyberCore::RegistrableDomain> registrableDomain;
        decoder >> registrableDomain;
        if (!registrableDomain)
            return std::nullopt;
        return { CyberCore::PCM::AttributionDestinationSite { WTFMove(*registrableDomain) } };
    }
};

template<> struct Coder<CyberCore::PCM::EphemeralNonce> {
    static void encode(Encoder& encoder, const CyberCore::PCM::EphemeralNonce& instance)
    {
        encoder << instance.nonce;
    }
    static std::optional<CyberCore::PCM::EphemeralNonce> decode(Decoder& decoder)
    {
        std::optional<String> nonce;
        decoder >> nonce;
        if (!nonce)
            return std::nullopt;
        return { CyberCore::PCM::EphemeralNonce { WTFMove(*nonce) } };
    }
};

template<> struct Coder<CyberCore::PCM::AttributionTimeToSendData> {
    static void encode(Encoder& encoder, const CyberCore::PCM::AttributionTimeToSendData& instance)
    {
        encoder << instance.sourceEarliestTimeToSend;
        encoder << instance.destinationEarliestTimeToSend;
    }
    static std::optional<CyberCore::PCM::AttributionTimeToSendData> decode(Decoder& decoder)
    {
        std::optional<std::optional<WallTime>> sourceEarliestTimeToSend;
        decoder >> sourceEarliestTimeToSend;
        if (!sourceEarliestTimeToSend)
            return std::nullopt;

        std::optional<std::optional<WallTime>> destinationEarliestTimeToSend;
        decoder >> destinationEarliestTimeToSend;
        if (!destinationEarliestTimeToSend)
            return std::nullopt;

        return { { WTFMove(*sourceEarliestTimeToSend), WTFMove(*destinationEarliestTimeToSend) } };
    }
};

void Coder<CyberCore::PrivateClickMeasurement, void>::encode(Encoder& encoder, const CyberCore::PrivateClickMeasurement& instance)
{
    encoder << instance.sourceID();
    encoder << instance.sourceSite();
    encoder << instance.destinationSite();
    encoder << instance.timeOfAdClick();
    encoder << instance.isEphemeral();
    encoder << instance.adamID();
    encoder << instance.attributionTriggerData();
    encoder << instance.timesToSend();
    encoder << instance.ephemeralSourceNonce();
    encoder << instance.sourceApplicationBundleID();
}

std::optional<CyberCore::PrivateClickMeasurement> Coder<CyberCore::PrivateClickMeasurement, void>::decode(Decoder& decoder)
{
    std::optional<uint8_t> sourceID;
    decoder >> sourceID;
    if (!sourceID)
        return std::nullopt;

    std::optional<CyberCore::PCM::SourceSite> sourceSite;
    decoder >> sourceSite;
    if (!sourceSite)
        return std::nullopt;

    std::optional<CyberCore::PCM::AttributionDestinationSite> destinationSite;
    decoder >> destinationSite;
    if (!destinationSite)
        return std::nullopt;

    std::optional<WallTime> timeOfAdClick;
    decoder >> timeOfAdClick;
    if (!timeOfAdClick)
        return std::nullopt;

    std::optional<CyberCore::PCM::AttributionEphemeral> isEphemeral;
    decoder >> isEphemeral;
    if (!isEphemeral)
        return std::nullopt;

    std::optional<std::optional<uint64_t>> adamID;
    decoder >> adamID;
    if (!adamID)
        return std::nullopt;

    std::optional<std::optional<CyberCore::PCM::AttributionTriggerData>> attributionTriggerData;
    decoder >> attributionTriggerData;
    if (!attributionTriggerData)
        return std::nullopt;

    std::optional<CyberCore::PCM::AttributionTimeToSendData> timesToSend;
    decoder >> timesToSend;
    if (!timesToSend)
        return std::nullopt;

    std::optional<std::optional<CyberCore::PCM::EphemeralNonce>> ephemeralSourceNonce;
    decoder >> ephemeralSourceNonce;
    if (!ephemeralSourceNonce)
        return std::nullopt;

    std::optional<String> sourceApplicationBundleID;
    decoder >> sourceApplicationBundleID;
    if (!sourceApplicationBundleID)
        return std::nullopt;

    return { {
        WTFMove(*sourceID),
        WTFMove(*sourceSite),
        WTFMove(*destinationSite),
        WTFMove(*timeOfAdClick),
        WTFMove(*isEphemeral),
        WTFMove(*adamID),
        WTFMove(*attributionTriggerData),
        WTFMove(*timesToSend),
        WTFMove(*ephemeralSourceNonce),
        WTFMove(*sourceApplicationBundleID),
    } };
}

void Coder<CyberCore::PCM::AttributionTriggerData, void>::encode(Encoder& encoder, const CyberCore::PCM::AttributionTriggerData& instance)
{
    encoder << instance.data;
    encoder << instance.priority;
    encoder << instance.wasSent;
    encoder << instance.sourceRegistrableDomain;
    encoder << instance.ephemeralDestinationNonce;
    encoder << instance.destinationSite;
}

std::optional<CyberCore::PCM::AttributionTriggerData> Coder<CyberCore::PCM::AttributionTriggerData, void>::decode(Decoder& decoder)
{
    std::optional<uint8_t> data;
    decoder >> data;
    if (!data)
        return std::nullopt;

    std::optional<CyberCore::PCM::AttributionTriggerData::Priority::PriorityValue> priority;
    decoder >> priority;
    if (!priority)
        return std::nullopt;

    std::optional<CyberCore::PCM::WasSent> wasSent;
    decoder >> wasSent;
    if (!wasSent)
        return std::nullopt;

    std::optional<std::optional<CyberCore::RegistrableDomain>> sourceRegistrableDomain;
    decoder >> sourceRegistrableDomain;
    if (!sourceRegistrableDomain)
        return std::nullopt;

    std::optional<std::optional<CyberCore::PCM::EphemeralNonce>> ephemeralDestinationNonce;
    decoder >> ephemeralDestinationNonce;
    if (!ephemeralDestinationNonce)
        return std::nullopt;

    std::optional<std::optional<CyberCore::RegistrableDomain>> destinationSite;
    decoder >> destinationSite;
    if (!destinationSite)
        return std::nullopt;

    return { {
        WTFMove(*data),
        WTFMove(*priority),
        WTFMove(*wasSent),
        WTFMove(*sourceRegistrableDomain),
        WTFMove(*ephemeralDestinationNonce),
        WTFMove(*destinationSite),
        // destinationUnlinkableToken and destinationSecretToken are not serialized.
    } };
}

void Coder<WebPushD::WebPushDaemonConnectionConfiguration, void>::encode(Encoder& encoder, const WebPushD::WebPushDaemonConnectionConfiguration& instance)
{
    instance.encode(encoder);
}

std::optional<WebPushD::WebPushDaemonConnectionConfiguration> Coder<WebPushD::WebPushDaemonConnectionConfiguration, void>::decode(Decoder& decoder)
{
    return WebPushD::WebPushDaemonConnectionConfiguration::decode(decoder);
}

void Coder<WebPushMessage, void>::encode(Encoder& encoder, const WebPushMessage& instance)
{
    encoder << instance.pushData << instance.registrationURL << instance.pushPartitionString;
}

std::optional<WebPushMessage> Coder<WebPushMessage, void>::decode(Decoder& decoder)
{
    std::optional<std::optional<Vector<uint8_t>>> pushData;
    decoder >> pushData;
    if (!pushData)
        return std::nullopt;

    std::optional<URL> registrationURL;
    decoder >> registrationURL;
    if (!registrationURL)
        return std::nullopt;

    std::optional<String> pushPartitionString;
    decoder >> pushPartitionString;
    if (!pushPartitionString)
        return std::nullopt;

    return { {
        WTFMove(*pushData),
        WTFMove(*pushPartitionString),
        WTFMove(*registrationURL)
    } };
}

void Coder<CyberCore::ExceptionData, void>::encode(Encoder& encoder, const CyberCore::ExceptionData& instance)
{
    encoder << instance.code;
    encoder << instance.message;
}

std::optional<CyberCore::ExceptionData> Coder<CyberCore::ExceptionData, void>::decode(Decoder& decoder)
{
    std::optional<CyberCore::ExceptionCode> code;
    decoder >> code;
    if (!code)
        return std::nullopt;

    std::optional<String> message;
    decoder >> message;
    if (!message)
        return std::nullopt;

    return CyberCore::ExceptionData { WTFMove(*code), WTFMove(*message) };
}

void Coder<CyberCore::SecurityOriginData, void>::encode(Encoder& encoder, const CyberCore::SecurityOriginData& instance)
{
    encoder << instance.protocol();
    encoder << instance.host();
    encoder << instance.port();
}

std::optional<CyberCore::SecurityOriginData> Coder<CyberCore::SecurityOriginData, void>::decode(Decoder& decoder)
{
    std::optional<String> protocol;
    decoder >> protocol;
    if (!protocol)
        return std::nullopt;
    
    std::optional<String> host;
    decoder >> host;
    if (!host)
        return std::nullopt;
    
    std::optional<std::optional<uint16_t>> port;
    decoder >> port;
    if (!port)
        return std::nullopt;
    
    CyberCore::SecurityOriginData data { WTFMove(*protocol), WTFMove(*host), WTFMove(*port) };
    if (data.isHashTableDeletedValue())
        return std::nullopt;

    return data;
}

void Coder<CyberCore::RegistrableDomain, void>::encode(Encoder& encoder, const CyberCore::RegistrableDomain& instance)
{
    encoder << instance.string();
}

std::optional<CyberCore::RegistrableDomain> Coder<CyberCore::RegistrableDomain, void>::decode(Decoder& decoder)
{
    std::optional<String> host;
    decoder >> host;
    if (!host)
        return std::nullopt;

    return { CyberCore::RegistrableDomain::fromRawString(WTFMove(*host)) };
}

void Coder<CyberCore::PushSubscriptionIdentifier>::encode(Encoder& encoder, const CyberCore::PushSubscriptionIdentifier& instance)
{
    instance.encode(encoder);
}

std::optional<CyberCore::PushSubscriptionIdentifier> Coder<CyberCore::PushSubscriptionIdentifier>::decode(Decoder& decoder)
{
    return CyberCore::PushSubscriptionIdentifier::decode(decoder);
}

}
