/*
 * Copyright (C) 2010-2020 Apple Inc. All rights reserved.
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
#include <CyberCore/AutoplayEvent.h>
#include <CyberCore/ColorSpace.h>
#include <CyberCore/DiagnosticLoggingClient.h>
#include <CyberCore/FrameLoaderTypes.h>
#include <CyberCore/ImageData.h>
#include <CyberCore/IndexedDB.h>
#include <CyberCore/InputMode.h>
#include <CyberCore/MediaSelectionOption.h>
#include <CyberCore/NetworkLoadMetrics.h>
#include <CyberCore/NotificationDirection.h>
#include <CyberCore/RealtimeMediaSource.h>
#include <CyberCore/RenderingMode.h>
#include <CyberCore/ScrollSnapOffsetsInfo.h>
#include <CyberCore/SerializedPlatformDataCueValue.h>
#include <CyberCore/ServiceWorkerTypes.h>
#include <CyberCore/StoredCredentialsPolicy.h>
#include <CyberCore/WorkerType.h>
#include <wtf/EnumTraits.h>

#if ENABLE(APPLE_PAY)
#include <CyberCore/PaymentHeaders.h>
#endif

#if USE(CURL)
#include <CyberCore/CurlProxySettings.h>
#endif

#if USE(APPLE_INTERNAL_SDK)
#include <CyberKitAdditions/CyberCoreArgumentCodersAdditions.h>
#endif

#if ENABLE(ENCRYPTED_MEDIA)
#include <CyberCore/CDMInstance.h>
#include <CyberCore/CDMInstanceSession.h>
#endif

#if PLATFORM(GTK)
#include "ArgumentCodersGtk.h"
#endif

#if PLATFORM(COCOA)
namespace WTF {
class MachSendRight;
}
#endif

namespace CyberCore {

class AbsolutePositionConstraints;
class AffineTransform;
class AuthenticationChallenge;
class BlobPart;
class CertificateInfo;
class Color;
class Credential;
class CubicBezierTimingFunction;
class Cursor;
class DatabaseDetails;
class DragData;
class File;
class FilterOperation;
class FilterOperations;
class FloatPoint;
class FloatPoint3D;
class FloatRect;
class FloatRoundedRect;
class FloatSize;
class FixedPositionViewportConstraints;
class FontHandle;
class HTTPHeaderMap;
class ImageHandle;
class IntPoint;
class IntRect;
class IntSize;
class KeyframeValueList;
class LayoutSize;
class LayoutPoint;
class LinearTimingFunction;
class NativeImageHandle;
class Notification;
class PasteboardCustomData;
class PaymentInstallmentConfiguration;
class ProtectionSpace;
class Region;
class ResourceError;
class ResourceRequest;
class ResourceResponse;
class SecurityOrigin;
class SharedBuffer;
class SpringTimingFunction;
class StepsTimingFunction;
class StickyPositionViewportConstraints;
class TextCheckingRequestData;
class TransformationMatrix;
class UserStyleSheet;

struct AttributedString;
struct CacheQueryOptions;
struct CharacterRange;
struct CompositionUnderline;
struct DictationAlternative;
struct DictionaryPopupInfo;
struct EventTrackingRegions;
struct ExceptionDetails;
struct FontAttributes;
struct FileChooserSettings;
struct RawFile;
struct ShareData;
struct ShareDataWithParsedURL;
struct Length;
struct GrammarDetail;
struct MimeClassInfo;
struct PasteboardImage;
struct PasteboardURL;
struct PluginInfo;
struct PromisedAttachmentInfo;
struct RecentSearch;
struct ResourceLoadStatistics;
struct ScrollableAreaParameters;
struct TextCheckingResult;
struct TextIndicatorData;
struct TouchActionData;
struct VelocityData;
struct ViewportAttributes;
struct WindowFeatures;
    
template<typename> class RectEdges;
using FloatBoxExtent = RectEdges<float>;

#if PLATFORM(COCOA)
struct KeypressCommand;
#endif

#if PLATFORM(IOS_FAMILY)
class FloatQuad;
class SelectionRect;
struct Highlight;
struct PasteboardImage;
struct PasteboardWebContent;
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

#if ENABLE(WIRELESS_PLAYBACK_TARGET)
class MediaPlaybackTargetContext;
#endif

#if ENABLE(MEDIA_SESSION)
class MediaSessionMetadata;
#endif

#if ENABLE(MEDIA_STREAM)
struct MediaConstraints;
#endif

#if ENABLE(ATTACHMENT_ELEMENT)
struct SerializedAttachmentData;
#endif

#if ENABLE(INDEXED_DATABASE)
using IDBKeyPath = Variant<String, Vector<String>>;
#endif

namespace DOMCacheEngine {
struct CacheInfo;
struct Record;
}

} // namespace CyberCore

namespace IPC {

template<> struct ArgumentCoder<CyberCore::AffineTransform> {
    static void encode(Encoder&, const CyberCore::AffineTransform&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::AffineTransform&);
};

template<> struct ArgumentCoder<CyberCore::AttributedString> {
    static void encode(Encoder&, const CyberCore::AttributedString&);
    static Optional<CyberCore::AttributedString> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::CacheQueryOptions> {
    static void encode(Encoder&, const CyberCore::CacheQueryOptions&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::CacheQueryOptions&);
};

template<> struct ArgumentCoder<CyberCore::CharacterRange> {
    static void encode(Encoder&, const CyberCore::CharacterRange&);
    static Optional<CyberCore::CharacterRange> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::DOMCacheEngine::CacheInfo> {
    static void encode(Encoder&, const CyberCore::DOMCacheEngine::CacheInfo&);
    static Optional<CyberCore::DOMCacheEngine::CacheInfo> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::DOMCacheEngine::Record> {
    static void encode(Encoder&, const CyberCore::DOMCacheEngine::Record&);
    static Optional<CyberCore::DOMCacheEngine::Record> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::TouchActionData> {
    static void encode(Encoder&, const CyberCore::TouchActionData&);
    static Optional<CyberCore::TouchActionData> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::EventTrackingRegions> {
    static void encode(Encoder&, const CyberCore::EventTrackingRegions&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::EventTrackingRegions&);
};

template<> struct ArgumentCoder<CyberCore::TransformationMatrix> {
    static void encode(Encoder&, const CyberCore::TransformationMatrix&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::TransformationMatrix&);
};

template<> struct ArgumentCoder<CyberCore::LinearTimingFunction> {
    static void encode(Encoder&, const CyberCore::LinearTimingFunction&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::LinearTimingFunction&);
};

template<> struct ArgumentCoder<CyberCore::CubicBezierTimingFunction> {
    static void encode(Encoder&, const CyberCore::CubicBezierTimingFunction&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::CubicBezierTimingFunction&);
};

template<> struct ArgumentCoder<CyberCore::StepsTimingFunction> {
    static void encode(Encoder&, const CyberCore::StepsTimingFunction&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::StepsTimingFunction&);
};

template<> struct ArgumentCoder<CyberCore::SpringTimingFunction> {
    static void encode(Encoder&, const CyberCore::SpringTimingFunction&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::SpringTimingFunction&);
};

template<> struct ArgumentCoder<CyberCore::CertificateInfo> {
    static void encode(Encoder&, const CyberCore::CertificateInfo&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::CertificateInfo&);
};

template<> struct ArgumentCoder<CyberCore::FloatPoint> {
    static void encode(Encoder&, const CyberCore::FloatPoint&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::FloatPoint&);
    static Optional<CyberCore::FloatPoint> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::FloatPoint3D> {
    static void encode(Encoder&, const CyberCore::FloatPoint3D&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::FloatPoint3D&);
};

template<> struct ArgumentCoder<CyberCore::FloatRect> {
    static void encode(Encoder&, const CyberCore::FloatRect&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::FloatRect&);
    static Optional<CyberCore::FloatRect> decode(Decoder&);
};
    
template<> struct ArgumentCoder<CyberCore::FloatBoxExtent> {
    static void encode(Encoder&, const CyberCore::FloatBoxExtent&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::FloatBoxExtent&);
};

template<> struct ArgumentCoder<CyberCore::FloatSize> {
    static void encode(Encoder&, const CyberCore::FloatSize&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::FloatSize&);
};

template<> struct ArgumentCoder<CyberCore::FloatRoundedRect> {
    static void encode(Encoder&, const CyberCore::FloatRoundedRect&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::FloatRoundedRect&);
};

#if PLATFORM(IOS_FAMILY)
template<> struct ArgumentCoder<CyberCore::FloatQuad> {
    static void encode(Encoder&, const CyberCore::FloatQuad&);
    static Optional<CyberCore::FloatQuad> decode(Decoder&);
};
#endif // PLATFORM(IOS_FAMILY)

#if ENABLE(META_VIEWPORT)
template<> struct ArgumentCoder<CyberCore::ViewportArguments> {
    static void encode(Encoder&, const CyberCore::ViewportArguments&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ViewportArguments&);
    static Optional<CyberCore::ViewportArguments> decode(Decoder&);
};

#endif

template<> struct ArgumentCoder<CyberCore::ViewportAttributes> {
    static void encode(Encoder&, const CyberCore::ViewportAttributes&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ViewportAttributes&);
};

template<> struct ArgumentCoder<CyberCore::IntPoint> {
    static void encode(Encoder&, const CyberCore::IntPoint&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::IntPoint&);
    static Optional<CyberCore::IntPoint> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::IntRect> {
    static void encode(Encoder&, const CyberCore::IntRect&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::IntRect&);
    static Optional<CyberCore::IntRect> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::IntSize> {
    static void encode(Encoder&, const CyberCore::IntSize&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::IntSize&);
    static Optional<CyberCore::IntSize> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::LayoutSize> {
    static void encode(Encoder&, const CyberCore::LayoutSize&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::LayoutSize&);
};

template<> struct ArgumentCoder<CyberCore::LayoutPoint> {
    static void encode(Encoder&, const CyberCore::LayoutPoint&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::LayoutPoint&);
};

template<> struct ArgumentCoder<CyberCore::Length> {
    static void encode(Encoder&, const CyberCore::Length&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::Length&);
};

template<> struct ArgumentCoder<CyberCore::VelocityData> {
    static void encode(Encoder&, const CyberCore::VelocityData&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::VelocityData&);
};

template<> struct ArgumentCoder<CyberCore::MimeClassInfo> {
    static void encode(Encoder&, const CyberCore::MimeClassInfo&);
    static Optional<CyberCore::MimeClassInfo> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::PluginInfo> {
    static void encode(Encoder&, const CyberCore::PluginInfo&);
    static Optional<CyberCore::PluginInfo> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::AuthenticationChallenge> {
    static void encode(Encoder&, const CyberCore::AuthenticationChallenge&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::AuthenticationChallenge&);
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

template<> struct ArgumentCoder<CyberCore::FontHandle> {
    static void encode(Encoder&, const CyberCore::FontHandle&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::FontHandle&);
    static void encodePlatformData(Encoder&, const CyberCore::FontHandle&);
    static WARN_UNUSED_RETURN bool decodePlatformData(Decoder&, CyberCore::FontHandle&);
};

template<> struct ArgumentCoder<CyberCore::ImageHandle> {
    static void encode(Encoder&, const CyberCore::ImageHandle&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ImageHandle&);
};

template<> struct ArgumentCoder<CyberCore::NativeImageHandle> {
    static void encode(Encoder&, const CyberCore::NativeImageHandle&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::NativeImageHandle&);
};

template<> struct ArgumentCoder<CyberCore::ResourceRequest> {
    static void encode(Encoder&, const CyberCore::ResourceRequest&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ResourceRequest&);
    static void encodePlatformData(Encoder&, const CyberCore::ResourceRequest&);
    static WARN_UNUSED_RETURN bool decodePlatformData(Decoder&, CyberCore::ResourceRequest&);
};

template<> struct ArgumentCoder<CyberCore::ResourceError> {
    static void encode(Encoder&, const CyberCore::ResourceError&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ResourceError&);
    static void encodePlatformData(Encoder&, const CyberCore::ResourceError&);
    static WARN_UNUSED_RETURN bool decodePlatformData(Decoder&, CyberCore::ResourceError&);
};

template<> struct ArgumentCoder<CyberCore::WindowFeatures> {
    static void encode(Encoder&, const CyberCore::WindowFeatures&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::WindowFeatures&);
};

#if ENABLE(DRAG_SUPPORT)
template<> struct ArgumentCoder<CyberCore::DragData> {
    static void encode(Encoder&, const CyberCore::DragData&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::DragData&);
};
#endif

#if PLATFORM(COCOA)
template<> struct ArgumentCoder<WTF::MachSendRight> {
    static void encode(Encoder&, const WTF::MachSendRight&);
    static void encode(Encoder&, WTF::MachSendRight&&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, WTF::MachSendRight&);
};

template<> struct ArgumentCoder<CyberCore::KeypressCommand> {
    static void encode(Encoder&, const CyberCore::KeypressCommand&);
    static Optional<CyberCore::KeypressCommand> decode(Decoder&);
};

template<> struct ArgumentCoder<CGPoint> {
    static void encode(Encoder&, CGPoint);
    static Optional<CGPoint> decode(Decoder&);
};

template<> struct ArgumentCoder<CGSize> {
    static void encode(Encoder&, CGSize);
    static Optional<CGSize> decode(Decoder&);
};

template<> struct ArgumentCoder<CGRect> {
    static void encode(Encoder&, CGRect);
    static Optional<CGRect> decode(Decoder&);
};

template<> struct ArgumentCoder<CGAffineTransform> {
    static void encode(Encoder&, CGAffineTransform);
    static Optional<CGAffineTransform> decode(Decoder&);
};
#endif

#if PLATFORM(IOS_FAMILY)
template<> struct ArgumentCoder<CyberCore::SelectionRect> {
    static void encode(Encoder&, const CyberCore::SelectionRect&);
    static Optional<CyberCore::SelectionRect> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::Highlight> {
    static void encode(Encoder&, const CyberCore::Highlight&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::Highlight&);
};

template<> struct ArgumentCoder<CyberCore::PasteboardWebContent> {
    static void encode(Encoder&, const CyberCore::PasteboardWebContent&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::PasteboardWebContent&);
};

template<> struct ArgumentCoder<CyberCore::PasteboardImage> {
    static void encode(Encoder&, const CyberCore::PasteboardImage&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::PasteboardImage&);
};
#endif

template<> struct ArgumentCoder<CyberCore::PasteboardCustomData> {
    static void encode(Encoder&, const CyberCore::PasteboardCustomData&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::PasteboardCustomData&);
};

template<> struct ArgumentCoder<CyberCore::PasteboardURL> {
    static void encode(Encoder&, const CyberCore::PasteboardURL&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::PasteboardURL&);
};

#if USE(SOUP)
template<> struct ArgumentCoder<CyberCore::SoupNetworkProxySettings> {
    static void encode(Encoder&, const CyberCore::SoupNetworkProxySettings&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::SoupNetworkProxySettings&);
};
#endif

#if USE(LIBWPE)
template<> struct ArgumentCoder<CyberCore::PasteboardWebContent> {
    static void encode(Encoder&, const CyberCore::PasteboardWebContent&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::PasteboardWebContent&);
};
#endif

#if USE(CURL)
template<> struct ArgumentCoder<CyberCore::CurlProxySettings> {
    static void encode(Encoder&, const CyberCore::CurlProxySettings&);
    static Optional<CyberCore::CurlProxySettings> decode(Decoder&);
};
#endif

template<> struct ArgumentCoder<CyberCore::CompositionUnderline> {
    static void encode(Encoder&, const CyberCore::CompositionUnderline&);
    static Optional<CyberCore::CompositionUnderline> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::DatabaseDetails> {
    static void encode(Encoder&, const CyberCore::DatabaseDetails&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::DatabaseDetails&);
};

template<> struct ArgumentCoder<CyberCore::DictationAlternative> {
    static void encode(Encoder&, const CyberCore::DictationAlternative&);
    static Optional<CyberCore::DictationAlternative> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::FileChooserSettings> {
    static void encode(Encoder&, const CyberCore::FileChooserSettings&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::FileChooserSettings&);
};
    
template<> struct ArgumentCoder<CyberCore::RawFile> {
    static void encode(Encoder&, const CyberCore::RawFile&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::RawFile&);
};

template<> struct ArgumentCoder<CyberCore::ShareData> {
    static void encode(Encoder&, const CyberCore::ShareData&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ShareData&);
};
    
template<> struct ArgumentCoder<CyberCore::ShareDataWithParsedURL> {
    static void encode(Encoder&, const CyberCore::ShareDataWithParsedURL&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ShareDataWithParsedURL&);
};

template<> struct ArgumentCoder<CyberCore::GrammarDetail> {
    static void encode(Encoder&, const CyberCore::GrammarDetail&);
    static Optional<CyberCore::GrammarDetail> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::TextCheckingRequestData> {
    static void encode(Encoder&, const CyberCore::TextCheckingRequestData&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::TextCheckingRequestData&);
};

template<> struct ArgumentCoder<CyberCore::TextCheckingResult> {
    static void encode(Encoder&, const CyberCore::TextCheckingResult&);
    static Optional<CyberCore::TextCheckingResult> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::UserStyleSheet> {
    static void encode(Encoder&, const CyberCore::UserStyleSheet&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::UserStyleSheet&);
};

template<> struct ArgumentCoder<CyberCore::ScrollableAreaParameters> {
    static void encode(Encoder&, const CyberCore::ScrollableAreaParameters&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ScrollableAreaParameters&);
};

template<> struct ArgumentCoder<CyberCore::FixedPositionViewportConstraints> {
    static void encode(Encoder&, const CyberCore::FixedPositionViewportConstraints&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::FixedPositionViewportConstraints&);
};

template<> struct ArgumentCoder<CyberCore::StickyPositionViewportConstraints> {
    static void encode(Encoder&, const CyberCore::StickyPositionViewportConstraints&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::StickyPositionViewportConstraints&);
};

template<> struct ArgumentCoder<CyberCore::AbsolutePositionConstraints> {
    static void encode(Encoder&, const CyberCore::AbsolutePositionConstraints&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::AbsolutePositionConstraints&);
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
#endif

template<> struct ArgumentCoder<CyberCore::BlobPart> {
    static void encode(Encoder&, const CyberCore::BlobPart&);
    static Optional<CyberCore::BlobPart> decode(Decoder&);
};

#if ENABLE(CONTENT_FILTERING)
template<> struct ArgumentCoder<CyberCore::ContentFilterUnblockHandler> {
    static void encode(Encoder&, const CyberCore::ContentFilterUnblockHandler&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ContentFilterUnblockHandler&);
};
#endif

#if ENABLE(MEDIA_SESSION)
template<> struct ArgumentCoder<CyberCore::MediaSessionMetadata> {
    static void encode(Encoder&, const CyberCore::MediaSessionMetadata&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::MediaSessionMetadata&);
};
#endif

template<> struct ArgumentCoder<CyberCore::TextIndicatorData> {
    static void encode(Encoder&, const CyberCore::TextIndicatorData&);
    static Optional<CyberCore::TextIndicatorData> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::DictionaryPopupInfo> {
    static void encode(Encoder&, const CyberCore::DictionaryPopupInfo&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::DictionaryPopupInfo&);
    static void encodePlatformData(Encoder&, const CyberCore::DictionaryPopupInfo&);
    static WARN_UNUSED_RETURN bool decodePlatformData(Decoder&, CyberCore::DictionaryPopupInfo&);
};

#if ENABLE(WIRELESS_PLAYBACK_TARGET)
template<> struct ArgumentCoder<CyberCore::MediaPlaybackTargetContext> {
    static void encode(Encoder&, const CyberCore::MediaPlaybackTargetContext&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::MediaPlaybackTargetContext&);
    static void encodePlatformData(Encoder&, const CyberCore::MediaPlaybackTargetContext&);
    static WARN_UNUSED_RETURN bool decodePlatformData(Decoder&, CyberCore::MediaPlaybackTargetContext&);
};
#endif

template<> struct ArgumentCoder<CyberCore::RecentSearch> {
    static void encode(Encoder&, const CyberCore::RecentSearch&);
    static Optional<CyberCore::RecentSearch> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::ExceptionDetails> {
    static void encode(Encoder&, const CyberCore::ExceptionDetails&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ExceptionDetails&);
};

template<> struct ArgumentCoder<CyberCore::ResourceLoadStatistics> {
    static void encode(Encoder&, const CyberCore::ResourceLoadStatistics&);
    static Optional<CyberCore::ResourceLoadStatistics> decode(Decoder&);
};

#if ENABLE(APPLE_PAY)

template<> struct ArgumentCoder<CyberCore::Payment> {
    static void encode(Encoder&, const CyberCore::Payment&);
    static Optional<CyberCore::Payment> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::PaymentAuthorizationResult> {
    static void encode(Encoder&, const CyberCore::PaymentAuthorizationResult&);
    static Optional<CyberCore::PaymentAuthorizationResult> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::PaymentContact> {
    static void encode(Encoder&, const CyberCore::PaymentContact&);
    static Optional<CyberCore::PaymentContact> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::PaymentError> {
    static void encode(Encoder&, const CyberCore::PaymentError&);
    static Optional<CyberCore::PaymentError> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::PaymentMerchantSession> {
    static void encode(Encoder&, const CyberCore::PaymentMerchantSession&);
    static Optional<CyberCore::PaymentMerchantSession> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::PaymentMethod> {
    static void encode(Encoder&, const CyberCore::PaymentMethod&);
    static Optional<CyberCore::PaymentMethod> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::PaymentMethodUpdate> {
    static void encode(Encoder&, const CyberCore::PaymentMethodUpdate&);
    static Optional<CyberCore::PaymentMethodUpdate> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::ApplePaySessionPaymentRequest> {
    static void encode(Encoder&, const CyberCore::ApplePaySessionPaymentRequest&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ApplePaySessionPaymentRequest&);
};

template<> struct ArgumentCoder<CyberCore::ApplePaySessionPaymentRequest::ContactFields> {
    static void encode(Encoder&, const CyberCore::ApplePaySessionPaymentRequest::ContactFields&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ApplePaySessionPaymentRequest::ContactFields&);
};

template<> struct ArgumentCoder<CyberCore::ApplePaySessionPaymentRequest::LineItem> {
    static void encode(Encoder&, const CyberCore::ApplePaySessionPaymentRequest::LineItem&);
    static Optional<CyberCore::ApplePaySessionPaymentRequest::LineItem> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::ApplePaySessionPaymentRequest::MerchantCapabilities> {
    static void encode(Encoder&, const CyberCore::ApplePaySessionPaymentRequest::MerchantCapabilities&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::ApplePaySessionPaymentRequest::MerchantCapabilities&);
};

template<> struct ArgumentCoder<CyberCore::ApplePaySessionPaymentRequest::ShippingMethod> {
    static void encode(Encoder&, const CyberCore::ApplePaySessionPaymentRequest::ShippingMethod&);
    static Optional<CyberCore::ApplePaySessionPaymentRequest::ShippingMethod> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::ApplePaySessionPaymentRequest::TotalAndLineItems> {
    static void encode(Encoder&, const CyberCore::ApplePaySessionPaymentRequest::TotalAndLineItems&);
    static Optional<CyberCore::ApplePaySessionPaymentRequest::TotalAndLineItems> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::ShippingContactUpdate> {
    static void encode(Encoder&, const CyberCore::ShippingContactUpdate&);
    static Optional<CyberCore::ShippingContactUpdate> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::ShippingMethodUpdate> {
    static void encode(Encoder&, const CyberCore::ShippingMethodUpdate&);
    static Optional<CyberCore::ShippingMethodUpdate> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::PaymentSessionError> {
    static void encode(Encoder&, const CyberCore::PaymentSessionError&);
    static Optional<CyberCore::PaymentSessionError> decode(Decoder&);
};

#endif

#if ENABLE(MEDIA_STREAM)
template<> struct ArgumentCoder<CyberCore::MediaConstraints> {
    static void encode(Encoder&, const CyberCore::MediaConstraints&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::MediaConstraints&);
};
#endif

#if ENABLE(INDEXED_DATABASE)

template<> struct ArgumentCoder<CyberCore::IDBKeyPath> {
    static void encode(Encoder&, const CyberCore::IDBKeyPath&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::IDBKeyPath&);
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

#if ENABLE(CSS_SCROLL_SNAP)

template<> struct ArgumentCoder<CyberCore::ScrollOffsetRange<float>> {
    static void encode(Encoder&, const CyberCore::ScrollOffsetRange<float>&);
    static Optional<CyberCore::ScrollOffsetRange<float>> decode(Decoder&);
};

#endif

template<> struct ArgumentCoder<CyberCore::MediaSelectionOption> {
    static void encode(Encoder&, const CyberCore::MediaSelectionOption&);
    static Optional<CyberCore::MediaSelectionOption> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::PromisedAttachmentInfo> {
    static void encode(Encoder&, const CyberCore::PromisedAttachmentInfo&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, CyberCore::PromisedAttachmentInfo&);
};

template<> struct ArgumentCoder<Vector<RefPtr<CyberCore::SecurityOrigin>>> {
    static void encode(Encoder&, const Vector<RefPtr<CyberCore::SecurityOrigin>>&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, Vector<RefPtr<CyberCore::SecurityOrigin>>&);
};

template<> struct ArgumentCoder<CyberCore::FontAttributes> {
    static void encode(Encoder&, const CyberCore::FontAttributes&);
    static Optional<CyberCore::FontAttributes> decode(Decoder&);
    static void encodePlatformData(Encoder&, const CyberCore::FontAttributes&);
    static Optional<CyberCore::FontAttributes> decodePlatformData(Decoder&, CyberCore::FontAttributes&);
};

#if ENABLE(ATTACHMENT_ELEMENT)

template<> struct ArgumentCoder<CyberCore::SerializedAttachmentData> {
    static void encode(Encoder&, const CyberCore::SerializedAttachmentData&);
    static Optional<CyberCore::SerializedAttachmentData> decode(Decoder&);
};

#endif // ENABLE(ATTACHMENT_ELEMENT)

#if ENABLE(VIDEO)
template<> struct ArgumentCoder<CyberCore::SerializedPlatformDataCueValue> {
    static void encode(Encoder&, const CyberCore::SerializedPlatformDataCueValue&);
    static Optional<CyberCore::SerializedPlatformDataCueValue> decode(Decoder&);
    static void encodePlatformData(Encoder&, const CyberCore::SerializedPlatformDataCueValue&);
    static Optional<CyberCore::SerializedPlatformDataCueValue> decodePlatformData(Decoder&, CyberCore::SerializedPlatformDataCueValue::PlatformType);
};
#endif

template<> struct ArgumentCoder<RefPtr<CyberCore::SharedBuffer>> {
    static void encode(Encoder&, const RefPtr<CyberCore::SharedBuffer>&);
    static Optional<RefPtr<CyberCore::SharedBuffer>> decode(Decoder&);
};

template<> struct ArgumentCoder<Ref<CyberCore::SharedBuffer>> {
    static void encode(Encoder&, const Ref<CyberCore::SharedBuffer>&);
    static Optional<Ref<CyberCore::SharedBuffer>> decode(Decoder&);
};

#if ENABLE(ENCRYPTED_MEDIA)
template<> struct ArgumentCoder<CyberCore::CDMInstanceSession::Message> {
    static void encode(Encoder&, const CyberCore::CDMInstanceSession::Message&);
    static Optional<CyberCore::CDMInstanceSession::Message> decode(Decoder&);
};

template<> struct ArgumentCoder<CyberCore::CDMInstanceSession::KeyStatusVector> {
    static void encode(Encoder&, const CyberCore::CDMInstanceSession::KeyStatusVector&);
    static Optional<CyberCore::CDMInstanceSession::KeyStatusVector> decode(Decoder&);
};
#endif

template<> struct ArgumentCoder<RefPtr<CyberCore::ImageData>> {
    static void encode(Encoder&, const RefPtr<CyberCore::ImageData>&);
    static Optional<RefPtr<CyberCore::ImageData>> decode(Decoder&);
};

template<> struct ArgumentCoder<Ref<CyberCore::ImageData>> {
    static void encode(Encoder&, const Ref<CyberCore::ImageData>&);
    static Optional<Ref<CyberCore::ImageData>> decode(Decoder&);
};

#if HAVE(PASSKIT_INSTALLMENTS)
template<> struct ArgumentCoder<CyberCore::PaymentInstallmentConfiguration> {
    static void encode(Encoder&, const CyberCore::PaymentInstallmentConfiguration&);
    static Optional<CyberCore::PaymentInstallmentConfiguration> decode(Decoder&);
};
#endif

} // namespace IPC

namespace WTF {

template<> struct EnumTraits<CyberCore::ColorSpace> {
    using values = EnumValues<
    CyberCore::ColorSpace,
    CyberCore::ColorSpace::SRGB,
    CyberCore::ColorSpace::LinearRGB,
    CyberCore::ColorSpace::DisplayP3
    >;
};

template<> struct EnumTraits<CyberCore::RenderingMode> {
    using values = EnumValues<
    CyberCore::RenderingMode,
    CyberCore::RenderingMode::Accelerated,
    CyberCore::RenderingMode::Unaccelerated,
    CyberCore::RenderingMode::DisplayListAccelerated,
    CyberCore::RenderingMode::DisplayListUnaccelerated,
    CyberCore::RenderingMode::RemoteAccelerated,
    CyberCore::RenderingMode::RemoteUnaccelerated
    >;
};

template<> struct EnumTraits<CyberCore::AutoplayEvent> {
    using values = EnumValues<
        CyberCore::AutoplayEvent,
        CyberCore::AutoplayEvent::DidPreventMediaFromPlaying,
        CyberCore::AutoplayEvent::DidPlayMediaWithUserGesture,
        CyberCore::AutoplayEvent::DidAutoplayMediaPastThresholdWithoutUserInterference,
        CyberCore::AutoplayEvent::UserDidInterfereWithPlayback
    >;
};

template<> struct EnumTraits<CyberCore::InputMode> {
    using values = EnumValues<
        CyberCore::InputMode,
        CyberCore::InputMode::Unspecified,
        CyberCore::InputMode::None,
        CyberCore::InputMode::Text,
        CyberCore::InputMode::Telephone,
        CyberCore::InputMode::Url,
        CyberCore::InputMode::Email,
        CyberCore::InputMode::Numeric,
        CyberCore::InputMode::Decimal,
        CyberCore::InputMode::Search
    >;
};

template<> struct EnumTraits<CyberCore::NetworkLoadPriority> {
    using values = EnumValues<
        CyberCore::NetworkLoadPriority,
        CyberCore::NetworkLoadPriority::Low,
        CyberCore::NetworkLoadPriority::Medium,
        CyberCore::NetworkLoadPriority::High,
        CyberCore::NetworkLoadPriority::Unknown
    >;
};

template<> struct EnumTraits<CyberCore::NotificationDirection> {
    using values = EnumValues<
        CyberCore::NotificationDirection,
        CyberCore::NotificationDirection::Auto,
        CyberCore::NotificationDirection::Ltr,
        CyberCore::NotificationDirection::Rtl
    >;
};

#if ENABLE(INDEXED_DATABASE)
template<> struct EnumTraits<CyberCore::IndexedDB::GetAllType> {
    using values = EnumValues<
        CyberCore::IndexedDB::GetAllType,
        CyberCore::IndexedDB::GetAllType::Keys,
        CyberCore::IndexedDB::GetAllType::Values
    >;
};
#endif

#if ENABLE(MEDIA_STREAM)
template<> struct EnumTraits<CyberCore::RealtimeMediaSource::Type> {
    using values = EnumValues<
        CyberCore::RealtimeMediaSource::Type,
        CyberCore::RealtimeMediaSource::Type::None,
        CyberCore::RealtimeMediaSource::Type::Audio,
        CyberCore::RealtimeMediaSource::Type::Video
    >;
};
#endif

template<> struct EnumTraits<CyberCore::MediaSelectionOption::Type> {
    using values = EnumValues<
        CyberCore::MediaSelectionOption::Type,
        CyberCore::MediaSelectionOption::Type::Regular,
        CyberCore::MediaSelectionOption::Type::LegibleOff,
        CyberCore::MediaSelectionOption::Type::LegibleAuto
    >;
};

template <> struct EnumTraits<CyberCore::WorkerType> {
    using values = EnumValues<
        CyberCore::WorkerType,
        CyberCore::WorkerType::Classic,
        CyberCore::WorkerType::Module
    >;
};

template<> struct EnumTraits<CyberCore::StoredCredentialsPolicy> {
    using values = EnumValues<
        CyberCore::StoredCredentialsPolicy,
        CyberCore::StoredCredentialsPolicy::DoNotUse,
        CyberCore::StoredCredentialsPolicy::Use,
        CyberCore::StoredCredentialsPolicy::EphemeralStateless
    >;
};

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

template<> struct EnumTraits<WTFLogChannelState> {
    using values = EnumValues<
    WTFLogChannelState,
    WTFLogChannelState::Off,
    WTFLogChannelState::On,
    WTFLogChannelState::OnWithAccumulation
    >;
};

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

} // namespace WTF
