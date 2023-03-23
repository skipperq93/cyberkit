/*
 * Copyright (C) 2010-2022 Apple Inc. All rights reserved.
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

#include "ArgumentCoders.h"
#include "Decoder.h"
#include "Encoder.h"
#include <CyberCore/AutoplayEvent.h>
#include <CyberCore/ColorSpace.h>
#include <CyberCore/DiagnosticLoggingClient.h>
#include <CyberCore/DisplayListItems.h>
#include <CyberCore/FloatRoundedRect.h>
#include <CyberCore/FloatSize.h>
#include <CyberCore/FrameLoaderTypes.h>
#include <CyberCore/IndexedDB.h>
#include <CyberCore/InputMode.h>
#include <CyberCore/LayoutPoint.h>
#include <CyberCore/LayoutSize.h>
#include <CyberCore/LengthBox.h>
#include <CyberCore/MediaSelectionOption.h>
#include <CyberCore/NetworkLoadMetrics.h>
#include <CyberCore/NotificationDirection.h>
#include <CyberCore/RealtimeMediaSource.h>
#include <CyberCore/RenderingMode.h>
#include <CyberCore/ScrollSnapOffsetsInfo.h>
#include <CyberCore/ScrollTypes.h>
#include <CyberCore/SerializedPlatformDataCueValue.h>
#include <CyberCore/ServiceWorkerTypes.h>
#include <CyberCore/StoredCredentialsPolicy.h>
#include <CyberCore/WorkerType.h>
#include <wtf/ArgumentCoder.h>
#include <wtf/EnumTraits.h>

#if ENABLE(APPLE_PAY)
#include <CyberCore/PaymentHeaders.h>
#endif

#if USE(CURL)
#include <CyberCore/CurlProxySettings.h>
#endif

#if ENABLE(WIRELESS_PLAYBACK_TARGET)
#include <CyberCore/MediaPlaybackTargetContext.h>
#endif

#if ENABLE(ENCRYPTED_MEDIA)
#include <CyberCore/CDMInstance.h>
#include <CyberCore/CDMInstanceSession.h>
#endif

#if PLATFORM(IOS_FAMILY)
#include <CyberCore/InspectorOverlay.h>
#endif

#if PLATFORM(GTK)
#include "ArgumentCodersGtk.h"
#endif

#if ENABLE(GPU_PROCESS) && ENABLE(WEBGL)
#include <CyberCore/GraphicsContextGLEnums.h>
#endif

#if ENABLE(WEBXR)
#include <CyberCore/PlatformXR.h>
#endif

#if ENABLE(CONTENT_FILTERING_IN_NETWORKING_PROCESS)
#include <CyberCore/MockContentFilterSettings.h>
#endif

#if PLATFORM(COCOA)
#include "ArgumentCodersCF.h"
#endif

OBJC_CLASS VKCImageAnalysis;

#if USE(AVFOUNDATION)
typedef struct __CVBuffer* CVPixelBufferRef;
#endif

namespace CyberCore {

class AbsolutePositionConstraints;
class AuthenticationChallenge;
class BlobPart;
class CertificateInfo;
class Color;
class ControlPart;
class SharedBuffer;
class CSPViolationReportBody;
class CSSFilter;
class Credential;
class Cursor;
class DatabaseDetails;
class DragData;
class DecomposedGlyphs;
class File;
class Filter;
class FilterEffect;
class FilterFunction;
class FilterOperation;
class FilterOperations;
class FixedPositionViewportConstraints;
class Font;
class FontPlatformData;
class HTTPHeaderMap;
class KeyframeValueList;
class LightSource;
class Notification;
class NotificationResources;
class PasteboardCustomData;
class PaymentInstallmentConfiguration;
class PixelBuffer;
class ProtectionSpace;
class Region;
class Report;
class ReportBody;
class ResourceError;
class ResourceRequest;
class ResourceResponse;
class ScriptBuffer;
class SerializedScriptValue;
class FragmentedSharedBuffer;
class StickyPositionViewportConstraints;
class SVGFilter;
class SystemImage;
class TextCheckingRequestData;
class TransformOperation;
class UserStyleSheet;

struct AttributedString;
struct CompositionUnderline;
struct DataDetectorElementInfo;
struct DictationAlternative;
struct DictionaryPopupInfo;
struct EventTrackingRegions;
struct ExceptionDetails;
struct FileChooserSettings;
struct TextRecognitionDataDetector;
struct Length;
struct GrammarDetail;
struct MimeClassInfo;
struct PasteboardImage;
struct PromisedAttachmentInfo;
struct RecentSearch;
struct ScrollableAreaParameters;
struct TextCheckingResult;
struct TextIndicatorData;
struct TouchActionData;
struct VelocityData;
struct ViewportAttributes;
struct WindowFeatures;

#if PLATFORM(COCOA)
struct KeypressCommand;
#endif

#if PLATFORM(IOS_FAMILY)
class FloatQuad;
class SelectionGeometry;
struct PasteboardImage;
struct PasteboardWebContent;
#endif

#if USE(APPKIT)
class AppKitControlSystemImage;
#endif

#if ENABLE(META_VIEWPORT)
struct ViewportArguments;
#endif

#if USE(SOUP)
struct SoupNetworkProxySettings;
#endif

#if USE(LIBWPE)
struct PasteboardWebContent;
#endif

#if ENABLE(CONTENT_FILTERING)
class ContentFilterUnblockHandler;
#endif

#if ENABLE(MEDIA_STREAM)
struct MediaConstraints;
#endif

#if ENABLE(ATTACHMENT_ELEMENT)
struct SerializedAttachmentData;
#endif

namespace DOMCacheEngine {
struct Record;
}

} // namespace CyberCore

namespace IPC {

template<> struct ArgumentCoder<CyberCore::DOMCacheEngine::Record> {
    static void encode(Encoder&, const CyberCore::DOMCacheEngine::Record&);
    static std::optional<CyberCore::DOMCacheEngine::Record> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::RectEdges<bool>> {
    static void encode(Encoder&, const CyberCore::RectEdges<bool>&);
    static std::optional<CyberCore::RectEdges<bool>> decode(Decoder&);
};

#if ENABLE(META_VIEWPORT)
template<> struct ArgumentCoder<CyberCore::ViewportArguments> {
    static void encode(Encoder&, const CyberCore::ViewportArguments&);
    static std::optional<CyberCore::ViewportArguments> decode(Decoder&);
};
#endif

template<> struct ArgumentCoder<CyberCore::Length> {
    static void encode(Encoder&, const CyberCore::Length&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::Length&);
};

template<> struct ArgumentCoder<CyberCore::ProtectionSpace> {
    static void encode(Encoder&, const CyberCore::ProtectionSpace&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ProtectionSpace&);
    static void encodePlatformData(Encoder&, const CyberCore::ProtectionSpace&);
    static WARN_UNUSED_RETURN bool decodePlatformData(Decoder&, CyberCore::ProtectionSpace&);
};

template<> struct ArgumentCoder<CyberCore::Credential> {
    static void encode(Encoder&, const CyberCore::Credential&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::Credential&);
    static void encodePlatformData(Encoder&, const CyberCore::Credential&);
    static WARN_UNUSED_RETURN bool decodePlatformData(Decoder&, CyberCore::Credential&);
};

template<> struct ArgumentCoder<CyberCore::Cursor> {
    static void encode(Encoder&, const CyberCore::Cursor&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::Cursor&);
};

template<> struct ArgumentCoder<RefPtr<CyberCore::Image>> {
    static void encode(Encoder&, const RefPtr<CyberCore::Image>&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, RefPtr<CyberCore::Image>&);
};

template<> struct ArgumentCoder<RefPtr<CyberCore::SerializedScriptValue>> {
    static void encode(Encoder&, const RefPtr<CyberCore::SerializedScriptValue>&);
    static std::optional<RefPtr<CyberCore::SerializedScriptValue>> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::Font> {
    static void encode(Encoder&, const CyberCore::Font&);
    static std::optional<Ref<CyberCore::Font>> decode(Decoder&);
    static void encodePlatformData(Encoder&, const CyberCore::Font&);
    static std::optional<CyberCore::FontPlatformData> decodePlatformData(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::ResourceError> {
    static void encode(Encoder&, const CyberCore::ResourceError&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ResourceError&);
    static void encodePlatformData(Encoder&, const CyberCore::ResourceError&);
    static WARN_UNUSED_RETURN bool decodePlatformData(Decoder&, CyberCore::ResourceError&);
};

#if PLATFORM(COCOA)

template<> struct ArgumentCoder<CyberCore::KeypressCommand> {
    static void encode(Encoder&, const CyberCore::KeypressCommand&);
    static std::optional<CyberCore::KeypressCommand> decode(Decoder&);
};

#endif // PLATFORM(COCOA)

#if PLATFORM(IOS_FAMILY)
template<> struct ArgumentCoder<CyberCore::InspectorOverlay::Highlight> {
    static void encode(Encoder&, const CyberCore::InspectorOverlay::Highlight&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::InspectorOverlay::Highlight&);
};

#endif

#if USE(APPKIT)

template<> struct ArgumentCoder<CyberCore::AppKitControlSystemImage> {
    template<typename Encoder>
    static void encode(Encoder&, const CyberCore::AppKitControlSystemImage&);
    static std::optional<Ref<CyberCore::AppKitControlSystemImage>> decode(Decoder&);
};

#endif

#if USE(SOUP)
template<> struct ArgumentCoder<CyberCore::SoupNetworkProxySettings> {
    static void encode(Encoder&, const CyberCore::SoupNetworkProxySettings&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::SoupNetworkProxySettings&);
};
#endif

#if USE(CURL)
template<> struct ArgumentCoder<CyberCore::CurlProxySettings> {
    static void encode(Encoder&, const CyberCore::CurlProxySettings&);
    static std::optional<CyberCore::CurlProxySettings> decode(Decoder&);
};
#endif


template<> struct ArgumentCoder<CyberCore::FixedPositionViewportConstraints> {
    static void encode(Encoder&, const CyberCore::FixedPositionViewportConstraints&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::FixedPositionViewportConstraints&);
};

template<> struct ArgumentCoder<CyberCore::StickyPositionViewportConstraints> {
    static void encode(Encoder&, const CyberCore::StickyPositionViewportConstraints&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::StickyPositionViewportConstraints&);
};

#if !USE(COORDINATED_GRAPHICS)
template<> struct ArgumentCoder<CyberCore::FilterOperations> {
    static void encode(Encoder&, const CyberCore::FilterOperations&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::FilterOperations&);
};

template<> struct ArgumentCoder<CyberCore::FilterOperation> {
    static void encode(Encoder&, const CyberCore::FilterOperation&);
};
WARN_UNUSED_RETURN bool decodeFilterOperation(Decoder&, RefPtr<CyberCore::FilterOperation>&);

template<> struct ArgumentCoder<RefPtr<CyberCore::FilterOperation>> {
    static void encode(Encoder&, const RefPtr<CyberCore::FilterOperation>&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, RefPtr<CyberCore::FilterOperation>&);
};
#endif

template<> struct ArgumentCoder<CyberCore::BlobPart> {
    static void encode(Encoder&, const CyberCore::BlobPart&);
    static std::optional<CyberCore::BlobPart> decode(Decoder&);
};

#if ENABLE(CONTENT_FILTERING)
template<> struct ArgumentCoder<CyberCore::ContentFilterUnblockHandler> {
    static void encode(Encoder&, const CyberCore::ContentFilterUnblockHandler&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ContentFilterUnblockHandler&);
};
#endif

#if ENABLE(WIRELESS_PLAYBACK_TARGET)
template<> struct ArgumentCoder<CyberCore::MediaPlaybackTargetContext> {
    static void encode(Encoder&, const CyberCore::MediaPlaybackTargetContext&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::MediaPlaybackTargetContext&);
    static void encodePlatformData(Encoder&, const CyberCore::MediaPlaybackTargetContext&);
    static WARN_UNUSED_RETURN bool decodePlatformData(Decoder&, CyberCore::MediaPlaybackTargetContext::Type, CyberCore::MediaPlaybackTargetContext&);
};
#endif

#if ENABLE(APPLE_PAY)

template<> struct ArgumentCoder<CyberCore::Payment> {
    static void encode(Encoder&, const CyberCore::Payment&);
    static std::optional<CyberCore::Payment> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::PaymentContact> {
    static void encode(Encoder&, const CyberCore::PaymentContact&);
    static std::optional<CyberCore::PaymentContact> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::PaymentMerchantSession> {
    static void encode(Encoder&, const CyberCore::PaymentMerchantSession&);
    static std::optional<CyberCore::PaymentMerchantSession> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::PaymentMethod> {
    static void encode(Encoder&, const CyberCore::PaymentMethod&);
    static std::optional<CyberCore::PaymentMethod> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::ApplePaySessionPaymentRequest> {
    static void encode(Encoder&, const CyberCore::ApplePaySessionPaymentRequest&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ApplePaySessionPaymentRequest&);
};
template<> struct ArgumentCoder<CyberCore::ApplePaySessionPaymentRequest::ContactFields> {
    static void encode(Encoder&, const CyberCore::ApplePaySessionPaymentRequest::ContactFields&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ApplePaySessionPaymentRequest::ContactFields&);
};

template<> struct ArgumentCoder<CyberCore::ApplePaySessionPaymentRequest::MerchantCapabilities> {
    static void encode(Encoder&, const CyberCore::ApplePaySessionPaymentRequest::MerchantCapabilities&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ApplePaySessionPaymentRequest::MerchantCapabilities&);
};

template<> struct ArgumentCoder<RefPtr<CyberCore::ApplePayError>> {
    static void encode(Encoder&, const RefPtr<CyberCore::ApplePayError>&);
    static std::optional<RefPtr<CyberCore::ApplePayError>> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::PaymentSessionError> {
    static void encode(Encoder&, const CyberCore::PaymentSessionError&);
    static std::optional<CyberCore::PaymentSessionError> decode(Decoder&);
};

#endif

#if ENABLE(SERVICE_WORKER)

template<> struct ArgumentCoder<CyberCore::ServiceWorkerOrClientData> {
    static void encode(Encoder&, const CyberCore::ServiceWorkerOrClientData&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ServiceWorkerOrClientData&);
};

template<> struct ArgumentCoder<CyberCore::ServiceWorkerOrClientIdentifier> {
    static void encode(Encoder&, const CyberCore::ServiceWorkerOrClientIdentifier&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ServiceWorkerOrClientIdentifier&);
};

#endif

#if ENABLE(ATTACHMENT_ELEMENT)

template<> struct ArgumentCoder<CyberCore::SerializedAttachmentData> {
    static void encode(Encoder&, const CyberCore::SerializedAttachmentData&);
    static std::optional<CyberCore::SerializedAttachmentData> decode(Decoder&);
};

#endif // ENABLE(ATTACHMENT_ELEMENT)

#if ENABLE(VIDEO)
template<> struct ArgumentCoder<CyberCore::SerializedPlatformDataCueValue> {
    static void encode(Encoder&, const CyberCore::SerializedPlatformDataCueValue&);
    static std::optional<CyberCore::SerializedPlatformDataCueValue> decode(Decoder&);
    static void encodePlatformData(Encoder&, const CyberCore::SerializedPlatformDataCueValue&);
    static std::optional<CyberCore::SerializedPlatformDataCueValue> decodePlatformData(Decoder&, CyberCore::SerializedPlatformDataCueValue::PlatformType);
};
#endif

template<> struct ArgumentCoder<CyberCore::FragmentedSharedBuffer> {
    static void encode(Encoder&, const CyberCore::FragmentedSharedBuffer&);
    static std::optional<Ref<CyberCore::FragmentedSharedBuffer>> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::SharedBuffer> {
    static void encode(Encoder&, const CyberCore::SharedBuffer&);
    static std::optional<Ref<CyberCore::SharedBuffer>> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::ScriptBuffer> {
    static void encode(Encoder&, const CyberCore::ScriptBuffer&);
    static std::optional<CyberCore::ScriptBuffer> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::SystemImage> {
    template<typename Encoder>
    static void encode(Encoder&, const CyberCore::SystemImage&);
    static std::optional<Ref<CyberCore::SystemImage>> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::ControlPart> {
    template<typename Encoder>
    static void encode(Encoder&, const CyberCore::ControlPart&);
    static std::optional<Ref<CyberCore::ControlPart>> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::LightSource> {
    template<typename Encoder>
    static void encode(Encoder&, const CyberCore::LightSource&);
    static std::optional<Ref<CyberCore::LightSource>> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::FilterFunction> {
    template<typename Encoder>
    static void encode(Encoder&, const CyberCore::FilterFunction&);
    static std::optional<Ref<CyberCore::FilterFunction>> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::FilterEffect> {
    template<typename Encoder>
    static void encode(Encoder&, const CyberCore::FilterEffect&);
    static std::optional<Ref<CyberCore::FilterEffect>> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::CSSFilter> {
    template<typename Encoder>
    static void encode(Encoder&, const CyberCore::CSSFilter&);
    static std::optional<Ref<CyberCore::CSSFilter>> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::SVGFilter> {
    template<typename Encoder>
    static void encode(Encoder&, const CyberCore::SVGFilter&);
    static std::optional<Ref<CyberCore::SVGFilter>> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::Filter> {
    template<typename Encoder>
    static void encode(Encoder&, const CyberCore::Filter&);
    static std::optional<Ref<CyberCore::Filter>> decode(Decoder&);
};

#if ENABLE(DATA_DETECTION)

template<> struct ArgumentCoder<CyberCore::DataDetectorElementInfo> {
    static void encode(Encoder&, const CyberCore::DataDetectorElementInfo&);
    static std::optional<CyberCore::DataDetectorElementInfo> decode(Decoder&);
};

#endif

#if ENABLE(ENCRYPTED_MEDIA)
template<> struct ArgumentCoder<CyberCore::CDMInstanceSession::Message> {
    static void encode(Encoder&, const CyberCore::CDMInstanceSession::Message&);
    static std::optional<CyberCore::CDMInstanceSession::Message> decode(Decoder&);
};
#endif

#if HAVE(PASSKIT_INSTALLMENTS)
template<> struct ArgumentCoder<CyberCore::PaymentInstallmentConfiguration> {
    static void encode(Encoder&, const CyberCore::PaymentInstallmentConfiguration&);
    static std::optional<CyberCore::PaymentInstallmentConfiguration> decode(Decoder&);
};
#endif

#if ENABLE(IMAGE_ANALYSIS) && ENABLE(DATA_DETECTION)

template<> struct ArgumentCoder<CyberCore::TextRecognitionDataDetector> {
    static void encode(Encoder&, const CyberCore::TextRecognitionDataDetector&);
    static WARN_UNUSED_RETURN std::optional<CyberCore::TextRecognitionDataDetector> decode(Decoder&);
    static void encodePlatformData(Encoder&, const CyberCore::TextRecognitionDataDetector&);
    static WARN_UNUSED_RETURN bool decodePlatformData(Decoder&, CyberCore::TextRecognitionDataDetector&);
};

#endif // ENABLE(IMAGE_ANALYSIS) && ENABLE(DATA_DETECTION)

#if ENABLE(IMAGE_ANALYSIS_ENHANCEMENTS)

template<> struct ArgumentCoder<RetainPtr<VKCImageAnalysis>> {
    static void encode(Encoder&, const RetainPtr<VKCImageAnalysis>&);
    static WARN_UNUSED_RETURN std::optional<RetainPtr<VKCImageAnalysis>> decode(Decoder&);
};

#endif // ENABLE(IMAGE_ANALYSIS_ENHANCEMENTS)

#if USE(AVFOUNDATION)

template<> struct ArgumentCoder<RetainPtr<CVPixelBufferRef>> {
    static void encode(Encoder&, const RetainPtr<CVPixelBufferRef>&);
    static std::optional<RetainPtr<CVPixelBufferRef>> decode(Decoder&);
};

#endif

template<> struct ArgumentCoder<CyberCore::PixelBuffer> {
    template<class Encoder> static void encode(Encoder&, const CyberCore::PixelBuffer&);
    static std::optional<Ref<CyberCore::PixelBuffer>> decode(Decoder&);
};

} // namespace IPC

namespace WTF {

#if ENABLE(MEDIA_STREAM)
template<> struct EnumTraits<CyberCore::RealtimeMediaSource::Type> {
    using values = EnumValues<
        CyberCore::RealtimeMediaSource::Type,
        CyberCore::RealtimeMediaSource::Type::Audio,
        CyberCore::RealtimeMediaSource::Type::Video
    >;
};
#endif

#if USE(CURL)
template <> struct EnumTraits<CyberCore::CurlProxySettings::Mode> {
    using values = EnumValues<
        CyberCore::CurlProxySettings::Mode,
        CyberCore::CurlProxySettings::Mode::Default,
        CyberCore::CurlProxySettings::Mode::NoProxy,
        CyberCore::CurlProxySettings::Mode::Custom
    >;
};
#endif

#undef Always
template<> struct EnumTraits<WTFLogLevel> {
    using values = EnumValues<
    WTFLogLevel,
    WTFLogLevel::Always,
    WTFLogLevel::Error,
    WTFLogLevel::Warning,
    WTFLogLevel::Info,
    WTFLogLevel::Debug
    >;
};

#if ENABLE(ENCRYPTED_MEDIA)
template <> struct EnumTraits<CyberCore::CDMInstanceSession::SessionLoadFailure> {
    using values = EnumValues <
    CyberCore::CDMInstanceSession::SessionLoadFailure,
    CyberCore::CDMInstanceSession::SessionLoadFailure::None,
    CyberCore::CDMInstanceSession::SessionLoadFailure::NoSessionData,
    CyberCore::CDMInstanceSession::SessionLoadFailure::MismatchedSessionType,
    CyberCore::CDMInstanceSession::SessionLoadFailure::QuotaExceeded,
    CyberCore::CDMInstanceSession::SessionLoadFailure::Other
    >;
};

template <> struct EnumTraits<CyberCore::CDMInstance::HDCPStatus> {
    using values = EnumValues <
    CyberCore::CDMInstance::HDCPStatus,
    CyberCore::CDMInstance::HDCPStatus::Unknown,
    CyberCore::CDMInstance::HDCPStatus::Valid,
    CyberCore::CDMInstance::HDCPStatus::OutputRestricted,
    CyberCore::CDMInstance::HDCPStatus::OutputDownscaled
    >;
};
#endif

#if ENABLE(GPU_PROCESS) && ENABLE(WEBGL)
template <> struct EnumTraits<CyberCore::GraphicsContextGLSimulatedEventForTesting> {
    using values = EnumValues<
    CyberCore::GraphicsContextGLSimulatedEventForTesting,
    CyberCore::GraphicsContextGLSimulatedEventForTesting::ContextChange,
    CyberCore::GraphicsContextGLSimulatedEventForTesting::GPUStatusFailure,
    CyberCore::GraphicsContextGLSimulatedEventForTesting::Timeout
    >;
};
#endif

} // namespace WTF
