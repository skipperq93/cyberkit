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
#include "CyberCorePersistentCoders.h"

#include "AppHighlightRangeData.h"
#include "CertificateInfo.h"
#include "ClientOrigin.h"
#include "ContentSecurityPolicyResponseHeaders.h"
#include "CrossOriginEmbedderPolicy.h"
#include "FetchOptions.h"
#include "HTTPHeaderMap.h"
#include "ImageResource.h"
#include "NavigationPreloadState.h"
#include "RegistrationDatabase.h"
#include "ResourceRequest.h"
#include "ResourceResponse.h"
#include <wtf/persistence/PersistentCoders.h>

#if PLATFORM(COCOA)
#include <wtf/spi/cocoa/SecuritySPI.h>
#endif

namespace WTF::Persistence {

#if ENABLE(APP_HIGHLIGHTS)
template<> struct Coder<CyberCore::AppHighlightRangeData::NodePathComponent> {
    static void encode(Encoder& encoder, const CyberCore::AppHighlightRangeData::NodePathComponent& instance)
    {
        encoder << instance.identifier;
        encoder << instance.nodeName;
        encoder << instance.textData;
        encoder << instance.pathIndex;
    }

    static std::optional<CyberCore::AppHighlightRangeData::NodePathComponent> decode(Decoder& decoder)
    {
        std::optional<String> identifier;
        decoder >> identifier;
        if (!identifier)
            return std::nullopt;

        std::optional<String> nodeName;
        decoder >> nodeName;
        if (!nodeName)
            return std::nullopt;

        std::optional<String> textData;
        decoder >> textData;
        if (!textData)
            return std::nullopt;

        std::optional<uint32_t> pathIndex;
        decoder >> pathIndex;
        if (!pathIndex)
            return std::nullopt;

        return { { WTFMove(*identifier), WTFMove(*nodeName), WTFMove(*textData), *pathIndex } };
    }
};

constexpr uint64_t highlightFileSignature = 0x4141504832303231; // File Signature  (A)pple(AP)plication(H)ighlights(2021)

void Coder<CyberCore::AppHighlightRangeData>::encode(Encoder& encoder, const CyberCore::AppHighlightRangeData& instance)
{
    constexpr uint64_t currentAppHighlightVersion = 1;
    
    encoder << highlightFileSignature;
    encoder << currentAppHighlightVersion;
    encoder << instance.identifier();
    encoder << instance.text();
    encoder << instance.startContainer();
    encoder << instance.startOffset();
    encoder << instance.endContainer();
    encoder << instance.endOffset();
}

std::optional<CyberCore::AppHighlightRangeData> Coder<CyberCore::AppHighlightRangeData>::decode(Decoder& decoder)
{
    std::optional<uint64_t> version;
    
    std::optional<uint64_t> decodedHighlightFileSignature;
    decoder >> decodedHighlightFileSignature;
    if (!decodedHighlightFileSignature)
        return std::nullopt;
    if (decodedHighlightFileSignature != highlightFileSignature) {
        if (!decoder.rewind(sizeof(highlightFileSignature)))
            return std::nullopt;
        version = 0;
    }
    
    std::optional<String> identifier;
    if (version)
        identifier = nullString();
    else {
        decoder >> version;
        if (!version)
            return std::nullopt;
        
        decoder >> identifier;
        if (!identifier)
            return std::nullopt;
    }

    std::optional<String> text;
    decoder >> text;
    if (!text)
        return std::nullopt;

    std::optional<CyberCore::AppHighlightRangeData::NodePath> startContainer;
    decoder >> startContainer;
    if (!startContainer)
        return std::nullopt;

    std::optional<uint32_t> startOffset;
    decoder >> startOffset;
    if (!startOffset)
        return std::nullopt;

    std::optional<CyberCore::AppHighlightRangeData::NodePath> endContainer;
    decoder >> endContainer;
    if (!endContainer)
        return std::nullopt;

    std::optional<uint32_t> endOffset;
    decoder >> endOffset;
    if (!endOffset)
        return std::nullopt;

    return { { WTFMove(*identifier), WTFMove(*text), WTFMove(*startContainer), *startOffset, WTFMove(*endContainer), *endOffset } };
}
#endif // ENABLE(APP_HIGHLIGHTS)

#if ENABLE(SERVICE_WORKER)
void Coder<CyberCore::ImportedScriptAttributes>::encode(Encoder& encoder, const CyberCore::ImportedScriptAttributes& instance)
{
    encoder << instance.responseURL << instance.mimeType;
}

std::optional<CyberCore::ImportedScriptAttributes> Coder<CyberCore::ImportedScriptAttributes>::decode(Decoder& decoder)
{
    std::optional<URL> responseURL;
    decoder >> responseURL;
    if (!responseURL)
        return std::nullopt;

    std::optional<String> mimeType;
    decoder >> mimeType;
    if (!mimeType)
        return std::nullopt;

    return { {
        WTFMove(*responseURL),
        WTFMove(*mimeType)
    } };
}

void Coder<CyberCore::ImageResource>::encode(Encoder& encoder, const CyberCore::ImageResource& instance)
{
    encoder << instance.src << instance.sizes << instance.type << instance.label;
}

std::optional<CyberCore::ImageResource> Coder<CyberCore::ImageResource>::decode(Decoder& decoder)
{
    std::optional<String> src;
    decoder >> src;
    if (!src)
        return std::nullopt;

    std::optional<String> sizes;
    decoder >> sizes;
    if (!sizes)
        return std::nullopt;

    std::optional<String> type;
    decoder >> type;
    if (!type)
        return std::nullopt;

    std::optional<String> label;
    decoder >> label;
    if (!label)
        return std::nullopt;

    return { {
        WTFMove(*src),
        WTFMove(*sizes),
        WTFMove(*type),
        WTFMove(*label)
    } };
}
#endif

void Coder<CyberCore::ResourceRequest>::encode(Encoder& encoder, const CyberCore::ResourceRequest& instance)
{
    ASSERT(!instance.httpBody());
    ASSERT(!instance.platformRequestUpdated());
    encoder << instance.url();
    encoder << instance.timeoutInterval();
    encoder << instance.firstPartyForCookies().string();
    encoder << instance.httpMethod();
    encoder << instance.httpHeaderFields();
    encoder << instance.responseContentDispositionEncodingFallbackArray();
    encoder << instance.cachePolicy();
    encoder << instance.allowCookies();
    encoder << instance.sameSiteDisposition();
    encoder << instance.isTopSite();
    encoder << instance.priority();
    encoder << instance.requester();
    encoder << instance.isAppInitiated();
}

std::optional<CyberCore::ResourceRequest> Coder<CyberCore::ResourceRequest>::decode(Decoder& decoder)
{
    std::optional<URL> url;
    decoder >> url;
    if (!url)
        return std::nullopt;

    std::optional<double> timeoutInterval;
    decoder >> timeoutInterval;
    if (!timeoutInterval)
        return std::nullopt;

    std::optional<String> firstPartyForCookies;
    decoder >> firstPartyForCookies;
    if (!firstPartyForCookies)
        return std::nullopt;

    std::optional<String> httpMethod;
    decoder >> httpMethod;
    if (!httpMethod)
        return std::nullopt;

    std::optional<CyberCore::HTTPHeaderMap> fields;
    decoder >> fields;
    if (!fields)
        return std::nullopt;

    std::optional<Vector<String>> array;
    decoder >> array;
    if (!array)
        return std::nullopt;

    std::optional<CyberCore::ResourceRequestCachePolicy> cachePolicy;
    decoder >> cachePolicy;
    if (!cachePolicy)
        return std::nullopt;

    std::optional<bool> allowCookies;
    decoder >> allowCookies;
    if (!allowCookies)
        return std::nullopt;

    std::optional<CyberCore::ResourceRequestBase::SameSiteDisposition> sameSiteDisposition;
    decoder >> sameSiteDisposition;
    if (!sameSiteDisposition)
        return std::nullopt;

    std::optional<bool> isTopSite;
    decoder >> isTopSite;
    if (!isTopSite)
        return std::nullopt;

    std::optional<CyberCore::ResourceLoadPriority> priority;
    decoder >> priority;
    if (!priority)
        return std::nullopt;

    std::optional<CyberCore::ResourceRequestRequester> requester;
    decoder >> requester;
    if (!requester)
        return std::nullopt;

    std::optional<bool> isAppInitiated;
    decoder >> isAppInitiated;
    if (!isAppInitiated)
        return std::nullopt;

    CyberCore::ResourceRequest request;
    request.setURL(WTFMove(*url));
    request.setTimeoutInterval(WTFMove(*timeoutInterval));
    request.setFirstPartyForCookies(URL({ }, *firstPartyForCookies));
    request.setHTTPMethod(WTFMove(*httpMethod));
    request.setHTTPHeaderFields(WTFMove(*fields));
    request.setResponseContentDispositionEncodingFallbackArray(WTFMove(*array));
    request.setCachePolicy(*cachePolicy);
    request.setAllowCookies(*allowCookies);
    request.setSameSiteDisposition(*sameSiteDisposition);
    request.setIsTopSite(*isTopSite);
    request.setPriority(*priority);
    request.setRequester(*requester);
    request.setIsAppInitiated(*isAppInitiated);
    return { request };
}

#if PLATFORM(COCOA)

} // namespace WTF::Persistence

namespace WTF {

// FIXME: Remove this when CyberKit::NetworkCache::Storage::version is incremented.
enum class LegacyCertificateInfoType {
    None,
    CertificateChain,
    Trust,
};

template<> struct EnumTraitsForPersistence<LegacyCertificateInfoType> {
    using values = EnumValues<
        LegacyCertificateInfoType,
        LegacyCertificateInfoType::None,
        LegacyCertificateInfoType::CertificateChain,
        LegacyCertificateInfoType::Trust
    >;
};

} // namespace WTF

namespace WTF::Persistence {

static void encodeCFData(Encoder& encoder, CFDataRef data)
{
    uint64_t length = CFDataGetLength(data);
    const uint8_t* bytePtr = CFDataGetBytePtr(data);

    encoder << length;
    encoder.encodeFixedLengthData({ bytePtr, static_cast<size_t>(length) });
}

static std::optional<RetainPtr<CFDataRef>> decodeCFData(Decoder& decoder)
{
    std::optional<uint64_t> size;
    decoder >> size;

    if (UNLIKELY(!isInBounds<size_t>(*size)))
        return std::nullopt;

    auto pointer = decoder.bufferPointerForDirectRead(static_cast<size_t>(*size));
    if (!pointer)
        return std::nullopt;

    return adoptCF(CFDataCreate(nullptr, pointer, *size));
}

static void encodeSecTrustRef(Encoder& encoder, SecTrustRef trust)
{
    auto data = adoptCF(SecTrustSerialize(trust, nullptr));
    if (!data) {
        encoder << false;
        return;
    }

    encoder << true;
    encodeCFData(encoder, data.get());
}

static std::optional<RetainPtr<SecTrustRef>> decodeSecTrustRef(Decoder& decoder)
{
    std::optional<bool> hasTrust;
    decoder >> hasTrust;
    if (!hasTrust)
        return std::nullopt;

    if (!*hasTrust)
        return { nullptr };

    auto trustData = decodeCFData(decoder);
    if (!trustData)
        return std::nullopt;

    auto trust = adoptCF(SecTrustDeserialize(trustData->get(), nullptr));
    if (!trust)
        return std::nullopt;

    return trust;
}

static std::optional<RetainPtr<CFArrayRef>> decodeCertificateChain(Decoder& decoder)
{
    std::optional<uint64_t> size;
    decoder >> size;
    if (!size)
        return std::nullopt;

    auto array = adoptCF(CFArrayCreateMutable(0, 0, &kCFTypeArrayCallBacks));

    for (size_t i = 0; i < *size; ++i) {
        auto data = decodeCFData(decoder);
        if (!data)
            return std::nullopt;

        auto certificate = adoptCF(SecCertificateCreateWithData(0, data->get()));
        CFArrayAppendValue(array.get(), certificate.get());
    }

    return { WTFMove(array) };
}

void Coder<CyberCore::CertificateInfo>::encode(Encoder& encoder, const CyberCore::CertificateInfo& certificateInfo)
{
    encoder << LegacyCertificateInfoType::Trust;
    encodeSecTrustRef(encoder, certificateInfo.trust().get());
}

std::optional<CyberCore::CertificateInfo> Coder<CyberCore::CertificateInfo>::decode(Decoder& decoder)
{
    std::optional<LegacyCertificateInfoType> certificateInfoType;
    decoder >> certificateInfoType;
    if (!certificateInfoType)
        return std::nullopt;

    switch (*certificateInfoType) {
    case LegacyCertificateInfoType::Trust: {
        auto trust = decodeSecTrustRef(decoder);
        if (!trust)
            return std::nullopt;

        return CyberCore::CertificateInfo(WTFMove(*trust));
    }
    case LegacyCertificateInfoType::CertificateChain: {
        auto certificateChain = decodeCertificateChain(decoder);
        if (!certificateChain)
            return std::nullopt;
        return CyberCore::CertificateInfo(CyberCore::CertificateInfo::secTrustFromCertificateChain(certificateChain->get()));
    }
    case LegacyCertificateInfoType::None:
        // Do nothing.
        return CyberCore::CertificateInfo();
    }
}

#elif USE(CURL)

void Coder<CyberCore::CertificateInfo>::encode(Encoder& encoder, const CyberCore::CertificateInfo& certificateInfo)
{
    auto& certificateChain = certificateInfo.certificateChain();

    encoder << certificateInfo.verificationError();
    encoder << certificateChain.size();
    for (auto& certificate : certificateChain)
        encoder << certificate;
}

std::optional<CyberCore::CertificateInfo> Coder<CyberCore::CertificateInfo>::decode(Decoder& decoder)
{
    std::optional<int> verificationError;
    decoder >> verificationError;
    if (!verificationError)
        return std::nullopt;

    std::optional<size_t> numOfCerts;
    decoder >> numOfCerts;
    if (!numOfCerts)
        return std::nullopt;

    CyberCore::CertificateInfo::CertificateChain certificateChain;
    for (size_t i = 0; i < numOfCerts.value(); i++) {
        std::optional<CyberCore::CertificateInfo::Certificate> certificate;
        decoder >> certificate;
        if (!certificate)
            return std::nullopt;

        certificateChain.append(WTFMove(certificate.value()));
    }

    return CyberCore::CertificateInfo(verificationError.value(), WTFMove(certificateChain));
}

#elif USE(SOUP)

template<> struct Coder<GRefPtr<GByteArray>> {
    static void encode(Encoder &encoder, const GRefPtr<GByteArray>& byteArray)
    {
        encoder << static_cast<uint32_t>(byteArray->len);
        encoder.encodeFixedLengthData({ byteArray->data, byteArray->len });
    }

    static std::optional<GRefPtr<GByteArray>> decode(Decoder& decoder)
    {
        std::optional<uint32_t> size;
        decoder >> size;
        if (!size)
            return std::nullopt;

        GRefPtr<GByteArray> byteArray = adoptGRef(g_byte_array_sized_new(*size));
        g_byte_array_set_size(byteArray.get(), *size);
        if (!decoder.decodeFixedLengthData({ byteArray->data, *size }))
            return std::nullopt;
        return byteArray;
    }
};

static Vector<GRefPtr<GByteArray>> certificatesDataListFromCertificateInfo(const CyberCore::CertificateInfo &certificateInfo)
{
    auto* certificate = certificateInfo.certificate().get();
    if (!certificate)
        return { };

    Vector<GRefPtr<GByteArray>> certificatesDataList;
    for (; certificate; certificate = g_tls_certificate_get_issuer(certificate)) {
        GByteArray* certificateData = nullptr;
        g_object_get(G_OBJECT(certificate), "certificate", &certificateData, nullptr);

        if (!certificateData) {
            certificatesDataList.clear();
            break;
        }
        certificatesDataList.append(adoptGRef(certificateData));
    }

    // Reverse so that the list starts from the rootmost certificate.
    certificatesDataList.reverse();

    return certificatesDataList;
}

static GRefPtr<GTlsCertificate> certificateFromCertificatesDataList(const Vector<GRefPtr<GByteArray>> &certificatesDataList)
{
    GType certificateType = g_tls_backend_get_certificate_type(g_tls_backend_get_default());
    GRefPtr<GTlsCertificate> certificate;
    for (auto& certificateData : certificatesDataList) {
        certificate = adoptGRef(G_TLS_CERTIFICATE(g_initable_new(
            certificateType, nullptr, nullptr, "certificate", certificateData.get(), "issuer", certificate.get(), nullptr)));
        if (!certificate)
            break;
    }

    return certificate;
}

void Coder<CyberCore::CertificateInfo>::encode(Encoder& encoder, const CyberCore::CertificateInfo& certificateInfo)
{
    auto certificatesDataList = certificatesDataListFromCertificateInfo(certificateInfo);

    encoder << certificatesDataList;

    if (certificatesDataList.isEmpty())
        return;

    encoder << static_cast<uint32_t>(certificateInfo.tlsErrors());
}

std::optional<CyberCore::CertificateInfo> Coder<CyberCore::CertificateInfo>::decode(Decoder& decoder)
{
    std::optional<Vector<GRefPtr<GByteArray>>> certificatesDataList;
    decoder >> certificatesDataList;
    if (!certificatesDataList)
        return std::nullopt;

    CyberCore::CertificateInfo certificateInfo;
    if (certificatesDataList->isEmpty())
        return certificateInfo;

    auto certificate = certificateFromCertificatesDataList(certificatesDataList.value());
    if (!certificate)
        return std::nullopt;
    certificateInfo.setCertificate(certificate.get());

    std::optional<uint32_t> tlsErrors;
    decoder >> tlsErrors;
    if (!tlsErrors)
        return std::nullopt;
    certificateInfo.setTLSErrors(static_cast<GTlsCertificateFlags>(*tlsErrors));

    return certificateInfo;
}

#elif PLATFORM(WIN)

void Coder<CyberCore::CertificateInfo>::encode(Encoder&, const CyberCore::CertificateInfo&)
{
}

std::optional<CyberCore::CertificateInfo> Coder<CyberCore::CertificateInfo>::decode(Decoder&)
{
    return CyberCore::CertificateInfo();
}

#endif

// FIXME: Move persistent coder implementations here and generate IPC coders for these structures.
#if ENABLE(SERVICE_WORKER)
void Coder<CyberCore::NavigationPreloadState>::encode(Encoder& encoder, const CyberCore::NavigationPreloadState& instance)
{
    encoder << instance.enabled;
    encoder << instance.headerValue;
}

std::optional<CyberCore::NavigationPreloadState> Coder<CyberCore::NavigationPreloadState>::decode(Decoder& decoder)
{
    std::optional<bool> enabled;
    decoder >> enabled;
    if (!enabled)
        return { };

    std::optional<String> headerValue;
    decoder >> headerValue;
    if (!headerValue)
        return { };
    return { { *enabled, WTFMove(*headerValue) } };
}
#endif

void Coder<CyberCore::CrossOriginEmbedderPolicy>::encode(Encoder& encoder, const CyberCore::CrossOriginEmbedderPolicy& instance)
{
    instance.encode(encoder);
}

std::optional<CyberCore::CrossOriginEmbedderPolicy> Coder<CyberCore::CrossOriginEmbedderPolicy>::decode(Decoder& decoder)
{
    return CyberCore::CrossOriginEmbedderPolicy::decode(decoder);
}

void Coder<CyberCore::ContentSecurityPolicyResponseHeaders>::encode(Encoder& encoder, const CyberCore::ContentSecurityPolicyResponseHeaders& instance)
{
    instance.encode(encoder);
}

std::optional<CyberCore::ContentSecurityPolicyResponseHeaders> Coder<CyberCore::ContentSecurityPolicyResponseHeaders>::decode(Decoder& decoder)
{
    return CyberCore::ContentSecurityPolicyResponseHeaders::decode(decoder);
}

void Coder<CyberCore::ClientOrigin>::encode(Encoder& encoder, const CyberCore::ClientOrigin& instance)
{
    encoder << instance.topOrigin;
    encoder << instance.clientOrigin;
}

std::optional<CyberCore::ClientOrigin> Coder<CyberCore::ClientOrigin>::decode(Decoder& decoder)
{
    std::optional<CyberCore::SecurityOriginData> topOrigin;
    std::optional<CyberCore::SecurityOriginData> clientOrigin;
    decoder >> topOrigin;
    if (!topOrigin || topOrigin->isNull())
        return std::nullopt;
    decoder >> clientOrigin;
    if (!clientOrigin || clientOrigin->isNull())
        return std::nullopt;

    return CyberCore::ClientOrigin { WTFMove(*topOrigin), WTFMove(*clientOrigin) };
}

void Coder<CyberCore::SecurityOriginData>::encode(Encoder& encoder, const CyberCore::SecurityOriginData& instance)
{
    encoder << instance.protocol();
    encoder << instance.host();
    encoder << instance.port();
}

std::optional<CyberCore::SecurityOriginData> Coder<CyberCore::SecurityOriginData>::decode(Decoder& decoder)
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

void Coder<CyberCore::ResourceResponse>::encode(Encoder& encoder, const CyberCore::ResourceResponse& instance)
{
    instance.encode(encoder);
}

std::optional<CyberCore::ResourceResponse> Coder<CyberCore::ResourceResponse>::decode(Decoder& decoder)
{
    CyberCore::ResourceResponse response;
    if (!CyberCore::ResourceResponseBase::decode(decoder, response))
        return std::nullopt;
    return response;
}

void Coder<CyberCore::FetchOptions>::encode(Encoder& encoder, const CyberCore::FetchOptions& instance)
{
    instance.encodePersistent(encoder);
}

std::optional<CyberCore::FetchOptions> Coder<CyberCore::FetchOptions>::decode(Decoder& decoder)
{
    CyberCore::FetchOptions options;
    if (!CyberCore::FetchOptions::decodePersistent(decoder, options))
        return std::nullopt;
    return options;
}

// Store common HTTP headers as strings instead of using their value in the HTTPHeaderName enumeration
// so that the headers stored in the cache stays valid even after HTTPHeaderName.in gets updated.
void Coder<CyberCore::HTTPHeaderMap>::encode(Encoder& encoder, const CyberCore::HTTPHeaderMap& headers)
{
    encoder << static_cast<uint64_t>(headers.size());
    for (auto& keyValue : headers) {
        encoder << keyValue.key;
        encoder << keyValue.value;
    }
}

std::optional<CyberCore::HTTPHeaderMap> Coder<CyberCore::HTTPHeaderMap>::decode(Decoder& decoder)
{
    std::optional<uint64_t> headersSize;
    decoder >> headersSize;
    if (!headersSize)
        return std::nullopt;

    CyberCore::HTTPHeaderMap headers;
    for (uint64_t i = 0; i < *headersSize; ++i) {
        std::optional<String> name;
        decoder >> name;
        if (!name)
            return std::nullopt;
        std::optional<String> value;
        decoder >> value;
        if (!value)
            return std::nullopt;
        headers.append(WTFMove(*name), WTFMove(*value));
    }
    return headers;
}

} // namespace WTF::Persistence
