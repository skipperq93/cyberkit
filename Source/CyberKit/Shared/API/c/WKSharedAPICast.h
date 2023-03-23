/*
 * Copyright (C) 2010-2023 Apple Inc. All rights reserved.
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

#include "APIError.h"
#include "APINumber.h"
#include "APISecurityOrigin.h"
#include "APIString.h"
#include "APIURL.h"
#include "APIURLRequest.h"
#include "APIURLResponse.h"
#include "ImageOptions.h"
#include "SameDocumentNavigationType.h"
#include "WKBase.h"
#include "WKContextMenuItemTypes.h"
#include "WKDiagnosticLoggingResultType.h"
#include "WKEvent.h"
#include "WKFindOptions.h"
#include "WKGeometry.h"
#include "WKImage.h"
#include "WKPageLoadTypes.h"
#include "WKPageLoadTypesPrivate.h"
#include "WKPageVisibilityTypes.h"
#include "WKUserContentInjectedFrames.h"
#include "WKUserScriptInjectionTime.h"
#include "WebFindOptions.h"
#include "WebMouseEvent.h"
#include <CyberCore/ContextMenuItem.h>
#include <CyberCore/DiagnosticLoggingResultType.h>
#include <CyberCore/FloatRect.h>
#include <CyberCore/FrameLoaderTypes.h>
#include <CyberCore/IntRect.h>
#include <CyberCore/LayoutMilestone.h>
#include <CyberCore/PlatformMouseEvent.h>
#include <CyberCore/SecurityOrigin.h>
#include <CyberCore/UserContentTypes.h>
#include <CyberCore/UserScriptTypes.h>
#include <CyberCore/VisibilityState.h>

namespace API {
class Array;
class Dictionary;
class Data;
class Point;
class Rect;
class SecurityOrigin;
class SerializedScriptValue;
class Size;
class UserContentURLPattern;
class WebArchive;
class WebArchiveResource;
}

namespace CyberKit {

class ObjCObjectGraph;
class WebConnection;
class WebContextMenuItem;
class WebImage;

template<typename APIType> struct APITypeInfo;
template<typename ImplType> struct ImplTypeInfo;

#define WK_ADD_API_MAPPING(TheAPIType, TheImplType) \
    template<> struct APITypeInfo<TheAPIType> { typedef TheImplType ImplType; }; \
    template<> struct ImplTypeInfo<TheImplType> { typedef TheAPIType APIType; };

WK_ADD_API_MAPPING(WKArrayRef, API::Array)
WK_ADD_API_MAPPING(WKBooleanRef, API::Boolean)
WK_ADD_API_MAPPING(WKConnectionRef, WebConnection)
WK_ADD_API_MAPPING(WKContextMenuItemRef, WebContextMenuItem)
WK_ADD_API_MAPPING(WKDataRef, API::Data)
WK_ADD_API_MAPPING(WKDictionaryRef, API::Dictionary)
WK_ADD_API_MAPPING(WKDoubleRef, API::Double)
WK_ADD_API_MAPPING(WKErrorRef, API::Error)
WK_ADD_API_MAPPING(WKImageRef, WebImage)
WK_ADD_API_MAPPING(WKPointRef, API::Point)
WK_ADD_API_MAPPING(WKRectRef, API::Rect)
WK_ADD_API_MAPPING(WKSecurityOriginRef, API::SecurityOrigin)
WK_ADD_API_MAPPING(WKSerializedScriptValueRef, API::SerializedScriptValue)
WK_ADD_API_MAPPING(WKSizeRef, API::Size)
WK_ADD_API_MAPPING(WKStringRef, API::String)
WK_ADD_API_MAPPING(WKTypeRef, API::Object)
WK_ADD_API_MAPPING(WKUInt64Ref, API::UInt64)
WK_ADD_API_MAPPING(WKURLRef, API::URL)
WK_ADD_API_MAPPING(WKURLRequestRef, API::URLRequest)
WK_ADD_API_MAPPING(WKURLResponseRef, API::URLResponse)
WK_ADD_API_MAPPING(WKUserContentURLPatternRef, API::UserContentURLPattern)

template<> struct APITypeInfo<WKMutableArrayRef> { typedef API::Array ImplType; };
template<> struct APITypeInfo<WKMutableDictionaryRef> { typedef API::Dictionary ImplType; };

#if PLATFORM(COCOA)
WK_ADD_API_MAPPING(WKWebArchiveRef, API::WebArchive)
WK_ADD_API_MAPPING(WKWebArchiveResourceRef, API::WebArchiveResource)
WK_ADD_API_MAPPING(WKObjCTypeWrapperRef, ObjCObjectGraph)
#endif

template<typename T, typename APIType = typename ImplTypeInfo<T>::APIType>
auto toAPI(T* t) -> APIType
{
    return reinterpret_cast<APIType>(API::Object::wrap(t));
}

template<typename T, typename APIType = typename ImplTypeInfo<T>::APIType>
auto toAPI(T& t) -> APIType
{
    return reinterpret_cast<APIType>(API::Object::wrap(&t));
}

template<typename T, typename ImplType = typename APITypeInfo<T>::ImplType>
auto toImpl(T t) -> ImplType*
{
    return static_cast<ImplType*>(API::Object::unwrap(static_cast<void*>(const_cast<typename std::remove_const<typename std::remove_pointer<T>::type>::type*>(t))));
}

template<typename ImplType, typename APIType = typename ImplTypeInfo<ImplType>::APIType>
class ProxyingRefPtr {
public:
    ProxyingRefPtr(RefPtr<ImplType>&& impl)
        : m_impl(impl)
    {
    }

    ProxyingRefPtr(Ref<ImplType>&& impl)
        : m_impl(WTFMove(impl))
    {
    }

    operator APIType() { return toAPI(m_impl.get()); }

private:
    RefPtr<ImplType> m_impl;
};

/* Special cases. */

inline ProxyingRefPtr<API::String> toAPI(StringImpl* string)
{
    return ProxyingRefPtr<API::String>(API::String::create(string));
}

inline WKStringRef toCopiedAPI(const String& string)
{
    return toAPI(&API::String::create(string).leakRef());
}

inline ProxyingRefPtr<API::URL> toURLRef(StringImpl* string)
{
    if (!string)
        return ProxyingRefPtr<API::URL>(nullptr);
    return ProxyingRefPtr<API::URL>(API::URL::create(String(string)));
}

inline WKURLRef toCopiedURLAPI(const String& string)
{
    if (!string)
        return nullptr;
    return toAPI(&API::URL::create(string).leakRef());
}

inline WKURLRef toCopiedURLAPI(const URL& url)
{
    return toCopiedURLAPI(url.string());
}

inline String toWTFString(WKStringRef stringRef)
{
    if (!stringRef)
        return String();
    return toImpl(stringRef)->string();
}

inline String toWTFString(WKURLRef urlRef)
{
    if (!urlRef)
        return String();
    return toImpl(urlRef)->string();
}

inline ProxyingRefPtr<API::Error> toAPI(const CyberCore::ResourceError& error)
{
    return ProxyingRefPtr<API::Error>(API::Error::create(error));
}

inline ProxyingRefPtr<API::URLRequest> toAPI(const CyberCore::ResourceRequest& request)
{
    return ProxyingRefPtr<API::URLRequest>(API::URLRequest::create(request));
}

inline ProxyingRefPtr<API::URLResponse> toAPI(const CyberCore::ResourceResponse& response)
{
    return ProxyingRefPtr<API::URLResponse>(API::URLResponse::create(response));
}

inline WKSecurityOriginRef toCopiedAPI(CyberCore::SecurityOrigin* origin)
{
    if (!origin)
        return nullptr;
    return toAPI(&API::SecurityOrigin::create(*origin).leakRef());
}

/* Geometry conversions */

inline CyberCore::FloatRect toFloatRect(const WKRect& wkRect)
{
    return CyberCore::FloatRect(static_cast<float>(wkRect.origin.x), static_cast<float>(wkRect.origin.y),
                              static_cast<float>(wkRect.size.width), static_cast<float>(wkRect.size.height));
}

inline CyberCore::IntSize toIntSize(const WKSize& wkSize)
{
    return CyberCore::IntSize(static_cast<int>(wkSize.width), static_cast<int>(wkSize.height));
}

inline CyberCore::IntPoint toIntPoint(const WKPoint& wkPoint)
{
    return CyberCore::IntPoint(static_cast<int>(wkPoint.x), static_cast<int>(wkPoint.y));
}

inline CyberCore::IntRect toIntRect(const WKRect& wkRect)
{
    return CyberCore::IntRect(static_cast<int>(wkRect.origin.x), static_cast<int>(wkRect.origin.y),
                            static_cast<int>(wkRect.size.width), static_cast<int>(wkRect.size.height));
}

inline WKRect toAPI(const CyberCore::FloatRect& rect)
{
    WKRect wkRect;
    wkRect.origin.x = rect.x();
    wkRect.origin.y = rect.y();
    wkRect.size.width = rect.width();
    wkRect.size.height = rect.height();
    return wkRect;
}

inline WKRect toAPI(const CyberCore::IntRect& rect)
{
    WKRect wkRect;
    wkRect.origin.x = rect.x();
    wkRect.origin.y = rect.y();
    wkRect.size.width = rect.width();
    wkRect.size.height = rect.height();
    return wkRect;
}

inline WKSize toAPI(const CyberCore::IntSize& size)
{
    WKSize wkSize;
    wkSize.width = size.width();
    wkSize.height = size.height();
    return wkSize;
}

inline WKPoint toAPI(const CyberCore::IntPoint& point)
{
    WKPoint wkPoint;
    wkPoint.x = point.x();
    wkPoint.y = point.y();
    return wkPoint;
}

/* Enum conversions */

inline WKTypeID toAPI(API::Object::Type type)
{
    return static_cast<WKTypeID>(type);
}

inline OptionSet<WebEventModifier> fromAPI(WKEventModifiers wkModifiers)
{
    OptionSet<WebEventModifier> modifiers;
    if (wkModifiers & kWKEventModifiersShiftKey)
        modifiers.add(WebEventModifier::ShiftKey);
    if (wkModifiers & kWKEventModifiersControlKey)
        modifiers.add(WebEventModifier::ControlKey);
    if (wkModifiers & kWKEventModifiersAltKey)
        modifiers.add(WebEventModifier::AltKey);
    if (wkModifiers & kWKEventModifiersMetaKey)
        modifiers.add(WebEventModifier::MetaKey);
    if (wkModifiers & kWKEventModifiersCapsLockKey)
        modifiers.add(WebEventModifier::CapsLockKey);
    return modifiers;
}

inline WKEventModifiers toAPI(OptionSet<WebEventModifier> modifiers)
{
    WKEventModifiers wkModifiers = 0;
    if (modifiers.contains(WebEventModifier::ShiftKey))
        wkModifiers |= kWKEventModifiersShiftKey;
    if (modifiers.contains(WebEventModifier::ControlKey))
        wkModifiers |= kWKEventModifiersControlKey;
    if (modifiers.contains(WebEventModifier::AltKey))
        wkModifiers |= kWKEventModifiersAltKey;
    if (modifiers.contains(WebEventModifier::MetaKey))
        wkModifiers |= kWKEventModifiersMetaKey;
    if (modifiers.contains(WebEventModifier::CapsLockKey))
        wkModifiers |= kWKEventModifiersCapsLockKey;
    return wkModifiers;
}

inline WKEventMouseButton toAPI(WebMouseEventButton mouseButton)
{
    WKEventMouseButton wkMouseButton = kWKEventMouseButtonNoButton;

    switch (mouseButton) {
    case WebMouseEventButton::NoButton:
        wkMouseButton = kWKEventMouseButtonNoButton;
        break;
    case WebMouseEventButton::LeftButton:
        wkMouseButton = kWKEventMouseButtonLeftButton;
        break;
    case WebMouseEventButton::MiddleButton:
        wkMouseButton = kWKEventMouseButtonMiddleButton;
        break;
    case WebMouseEventButton::RightButton:
        wkMouseButton = kWKEventMouseButtonRightButton;
        break;
    }

    return wkMouseButton;
}

inline WKEventMouseButton toAPI(CyberCore::MouseButton mouseButton)
{
    WKEventMouseButton wkMouseButton = kWKEventMouseButtonNoButton;

    switch (mouseButton) {
    case CyberCore::MouseButton::NoButton:
        wkMouseButton = kWKEventMouseButtonNoButton;
        break;
    case CyberCore::MouseButton::LeftButton:
        wkMouseButton = kWKEventMouseButtonLeftButton;
        break;
    case CyberCore::MouseButton::MiddleButton:
        wkMouseButton = kWKEventMouseButtonMiddleButton;
        break;
    case CyberCore::MouseButton::RightButton:
        wkMouseButton = kWKEventMouseButtonRightButton;
        break;
    }

    return wkMouseButton;
}

inline WKContextMenuItemTag toAPI(CyberCore::ContextMenuAction action)
{
    switch (action) {
    case CyberCore::ContextMenuItemTagNoAction:
        return kWKContextMenuItemTagNoAction;
    case CyberCore::ContextMenuItemTagOpenLinkInNewWindow:
        return kWKContextMenuItemTagOpenLinkInNewWindow;
    case CyberCore::ContextMenuItemTagDownloadLinkToDisk:
        return kWKContextMenuItemTagDownloadLinkToDisk;
    case CyberCore::ContextMenuItemTagCopyLinkToClipboard:
        return kWKContextMenuItemTagCopyLinkToClipboard;
    case CyberCore::ContextMenuItemTagOpenImageInNewWindow:
        return kWKContextMenuItemTagOpenImageInNewWindow;
    case CyberCore::ContextMenuItemTagDownloadImageToDisk:
        return kWKContextMenuItemTagDownloadImageToDisk;
    case CyberCore::ContextMenuItemTagCopyImageToClipboard:
        return kWKContextMenuItemTagCopyImageToClipboard;
#if ENABLE(ACCESSIBILITY_ANIMATION_CONTROL)
    case CyberCore::ContextMenuItemTagPlayAllAnimations:
        return kWKContextMenuItemTagPlayAllAnimations;
    case CyberCore::ContextMenuItemTagPauseAllAnimations:
        return kWKContextMenuItemTagPauseAllAnimations;
    case CyberCore::ContextMenuItemTagPlayAnimation:
        return kWKContextMenuItemTagPlayAnimation;
    case CyberCore::ContextMenuItemTagPauseAnimation:
        return kWKContextMenuItemTagPauseAnimation;
#endif // ENABLE(ACCESSIBILITY_ANIMATION_CONTROL)
#if PLATFORM(GTK)
    case CyberCore::ContextMenuItemTagCopyImageUrlToClipboard:
        return kWKContextMenuItemTagCopyImageUrlToClipboard;
#endif
    case CyberCore::ContextMenuItemTagOpenFrameInNewWindow:
        return kWKContextMenuItemTagOpenFrameInNewWindow;
    case CyberCore::ContextMenuItemTagCopy:
        return kWKContextMenuItemTagCopy;
    case CyberCore::ContextMenuItemTagGoBack:
        return kWKContextMenuItemTagGoBack;
    case CyberCore::ContextMenuItemTagGoForward:
        return kWKContextMenuItemTagGoForward;
    case CyberCore::ContextMenuItemTagStop:
        return kWKContextMenuItemTagStop;
    case CyberCore::ContextMenuItemTagReload:
        return kWKContextMenuItemTagReload;
    case CyberCore::ContextMenuItemTagCut:
        return kWKContextMenuItemTagCut;
    case CyberCore::ContextMenuItemTagPaste:
        return kWKContextMenuItemTagPaste;
#if PLATFORM(GTK)
    case CyberCore::ContextMenuItemTagSelectAll:
        return kWKContextMenuItemTagSelectAll;
#endif
    case CyberCore::ContextMenuItemTagSpellingGuess:
        return kWKContextMenuItemTagSpellingGuess;
    case CyberCore::ContextMenuItemTagNoGuessesFound:
        return kWKContextMenuItemTagNoGuessesFound;
    case CyberCore::ContextMenuItemTagIgnoreSpelling:
        return kWKContextMenuItemTagIgnoreSpelling;
    case CyberCore::ContextMenuItemTagLearnSpelling:
        return kWKContextMenuItemTagLearnSpelling;
    case CyberCore::ContextMenuItemTagOther:
        return kWKContextMenuItemTagOther;
    case CyberCore::ContextMenuItemTagSearchInSpotlight:
        return kWKContextMenuItemTagSearchInSpotlight;
    case CyberCore::ContextMenuItemTagSearchWeb:
        return kWKContextMenuItemTagSearchWeb;
    case CyberCore::ContextMenuItemTagLookUpInDictionary:
        return kWKContextMenuItemTagLookUpInDictionary;
    case CyberCore::ContextMenuItemTagOpenWithDefaultApplication:
        return kWKContextMenuItemTagOpenWithDefaultApplication;
    case CyberCore::ContextMenuItemPDFActualSize:
        return kWKContextMenuItemTagPDFActualSize;
    case CyberCore::ContextMenuItemPDFZoomIn:
        return kWKContextMenuItemTagPDFZoomIn;
    case CyberCore::ContextMenuItemPDFZoomOut:
        return kWKContextMenuItemTagPDFZoomOut;
    case CyberCore::ContextMenuItemPDFAutoSize:
        return kWKContextMenuItemTagPDFAutoSize;
    case CyberCore::ContextMenuItemPDFSinglePage:
        return kWKContextMenuItemTagPDFSinglePage;
    case CyberCore::ContextMenuItemPDFFacingPages:
        return kWKContextMenuItemTagPDFFacingPages;
    case CyberCore::ContextMenuItemPDFContinuous:
        return kWKContextMenuItemTagPDFContinuous;
    case CyberCore::ContextMenuItemPDFNextPage:
        return kWKContextMenuItemTagPDFNextPage;
    case CyberCore::ContextMenuItemPDFPreviousPage:
        return kWKContextMenuItemTagPDFPreviousPage;
    case CyberCore::ContextMenuItemTagOpenLink:
        return kWKContextMenuItemTagOpenLink;
    case CyberCore::ContextMenuItemTagIgnoreGrammar:
        return kWKContextMenuItemTagIgnoreGrammar;
    case CyberCore::ContextMenuItemTagSpellingMenu:
        return kWKContextMenuItemTagSpellingMenu;
    case CyberCore::ContextMenuItemTagShowSpellingPanel:
        return kWKContextMenuItemTagShowSpellingPanel;
    case CyberCore::ContextMenuItemTagCheckSpelling:
        return kWKContextMenuItemTagCheckSpelling;
    case CyberCore::ContextMenuItemTagCheckSpellingWhileTyping:
        return kWKContextMenuItemTagCheckSpellingWhileTyping;
    case CyberCore::ContextMenuItemTagCheckGrammarWithSpelling:
        return kWKContextMenuItemTagCheckGrammarWithSpelling;
    case CyberCore::ContextMenuItemTagFontMenu:
        return kWKContextMenuItemTagFontMenu;
    case CyberCore::ContextMenuItemTagShowFonts:
        return kWKContextMenuItemTagShowFonts;
    case CyberCore::ContextMenuItemTagBold:
        return kWKContextMenuItemTagBold;
    case CyberCore::ContextMenuItemTagItalic:
        return kWKContextMenuItemTagItalic;
    case CyberCore::ContextMenuItemTagUnderline:
        return kWKContextMenuItemTagUnderline;
    case CyberCore::ContextMenuItemTagOutline:
        return kWKContextMenuItemTagOutline;
    case CyberCore::ContextMenuItemTagStyles:
        return kWKContextMenuItemTagStyles;
    case CyberCore::ContextMenuItemTagShowColors:
        return kWKContextMenuItemTagShowColors;
    case CyberCore::ContextMenuItemTagSpeechMenu:
        return kWKContextMenuItemTagSpeechMenu;
    case CyberCore::ContextMenuItemTagStartSpeaking:
        return kWKContextMenuItemTagStartSpeaking;
    case CyberCore::ContextMenuItemTagStopSpeaking:
        return kWKContextMenuItemTagStopSpeaking;
    case CyberCore::ContextMenuItemTagWritingDirectionMenu:
        return kWKContextMenuItemTagWritingDirectionMenu;
    case CyberCore::ContextMenuItemTagDefaultDirection:
        return kWKContextMenuItemTagDefaultDirection;
    case CyberCore::ContextMenuItemTagLeftToRight:
        return kWKContextMenuItemTagLeftToRight;
    case CyberCore::ContextMenuItemTagRightToLeft:
        return kWKContextMenuItemTagRightToLeft;
    case CyberCore::ContextMenuItemTagPDFSinglePageScrolling:
        return kWKContextMenuItemTagPDFSinglePageScrolling;
    case CyberCore::ContextMenuItemTagPDFFacingPagesScrolling:
        return kWKContextMenuItemTagPDFFacingPagesScrolling;
    case CyberCore::ContextMenuItemTagDictationAlternative:
        return kWKContextMenuItemTagDictationAlternative;
    case CyberCore::ContextMenuItemTagInspectElement:
        return kWKContextMenuItemTagInspectElement;
    case CyberCore::ContextMenuItemTagTextDirectionMenu:
        return kWKContextMenuItemTagTextDirectionMenu;
    case CyberCore::ContextMenuItemTagTextDirectionDefault:
        return kWKContextMenuItemTagTextDirectionDefault;
    case CyberCore::ContextMenuItemTagTextDirectionLeftToRight:
        return kWKContextMenuItemTagTextDirectionLeftToRight;
    case CyberCore::ContextMenuItemTagTextDirectionRightToLeft:
        return kWKContextMenuItemTagTextDirectionRightToLeft;
    case CyberCore::ContextMenuItemTagOpenMediaInNewWindow:
        return kWKContextMenuItemTagOpenMediaInNewWindow;
    case CyberCore::ContextMenuItemTagDownloadMediaToDisk:
        return kWKContextMenuItemTagDownloadMediaToDisk;
    case CyberCore::ContextMenuItemTagCopyMediaLinkToClipboard:
        return kWKContextMenuItemTagCopyMediaLinkToClipboard;
    case CyberCore::ContextMenuItemTagToggleMediaControls:
        return kWKContextMenuItemTagToggleMediaControls;
    case CyberCore::ContextMenuItemTagToggleMediaLoop:
        return kWKContextMenuItemTagToggleMediaLoop;
    case CyberCore::ContextMenuItemTagToggleVideoFullscreen:
        return kWKContextMenuItemTagToggleVideoFullscreen;
    case CyberCore::ContextMenuItemTagEnterVideoFullscreen:
        return kWKContextMenuItemTagEnterVideoFullscreen;
    case CyberCore::ContextMenuItemTagToggleVideoEnhancedFullscreen:
        return kWKContextMenuItemTagToggleVideoEnhancedFullscreen;
    case CyberCore::ContextMenuItemTagMediaPlayPause:
        return kWKContextMenuItemTagMediaPlayPause;
    case CyberCore::ContextMenuItemTagMediaMute:
        return kWKContextMenuItemTagMediaMute;
    case CyberCore::ContextMenuItemTagAddHighlightToCurrentQuickNote:
        return kWKContextMenuItemTagAddHighlightToCurrentQuickNote;
    case CyberCore::ContextMenuItemTagAddHighlightToNewQuickNote:
        return kWKContextMenuItemTagAddHighlightToNewQuickNote;
#if PLATFORM(COCOA)
    case CyberCore::ContextMenuItemTagCorrectSpellingAutomatically:
        return kWKContextMenuItemTagCorrectSpellingAutomatically;
    case CyberCore::ContextMenuItemTagSubstitutionsMenu:
        return kWKContextMenuItemTagSubstitutionsMenu;
    case CyberCore::ContextMenuItemTagShowSubstitutions:
        return kWKContextMenuItemTagShowSubstitutions;
    case CyberCore::ContextMenuItemTagSmartCopyPaste:
        return kWKContextMenuItemTagSmartCopyPaste;
    case CyberCore::ContextMenuItemTagSmartQuotes:
        return kWKContextMenuItemTagSmartQuotes;
    case CyberCore::ContextMenuItemTagSmartDashes:
        return kWKContextMenuItemTagSmartDashes;
    case CyberCore::ContextMenuItemTagSmartLinks:
        return kWKContextMenuItemTagSmartLinks;
    case CyberCore::ContextMenuItemTagTextReplacement:
        return kWKContextMenuItemTagTextReplacement;
    case CyberCore::ContextMenuItemTagTransformationsMenu:
        return kWKContextMenuItemTagTransformationsMenu;
    case CyberCore::ContextMenuItemTagMakeUpperCase:
        return kWKContextMenuItemTagMakeUpperCase;
    case CyberCore::ContextMenuItemTagMakeLowerCase:
        return kWKContextMenuItemTagMakeLowerCase;
    case CyberCore::ContextMenuItemTagCapitalize:
        return kWKContextMenuItemTagCapitalize;
    case CyberCore::ContextMenuItemTagChangeBack:
        return kWKContextMenuItemTagChangeBack;
#endif
    case CyberCore::ContextMenuItemTagShareMenu:
        return kWKContextMenuItemTagShareMenu;
    case CyberCore::ContextMenuItemTagLookUpImage:
        return kWKContextMenuItemTagRevealImage;
    case CyberCore::ContextMenuItemTagTranslate:
        return kWKContextMenuItemTagTranslate;
    case CyberCore::ContextMenuItemTagCopySubject:
        return kWKContextMenuItemTagCopyCroppedImage;
    default:
        if (action < CyberCore::ContextMenuItemBaseApplicationTag && !(action >= CyberCore::ContextMenuItemBaseCustomTag && action <= CyberCore::ContextMenuItemLastCustomTag))
            LOG_ERROR("ContextMenuAction %i is an unknown tag but is below the allowable custom tag value of %i", action, CyberCore::ContextMenuItemBaseApplicationTag);
        return static_cast<WKContextMenuItemTag>(action);
    }
}

inline CyberCore::ContextMenuAction toImpl(WKContextMenuItemTag tag)
{
    switch (tag) {
    case kWKContextMenuItemTagNoAction:
        return CyberCore::ContextMenuItemTagNoAction;
    case kWKContextMenuItemTagOpenLinkInNewWindow:
        return CyberCore::ContextMenuItemTagOpenLinkInNewWindow;
    case kWKContextMenuItemTagDownloadLinkToDisk:
        return CyberCore::ContextMenuItemTagDownloadLinkToDisk;
    case kWKContextMenuItemTagCopyLinkToClipboard:
        return CyberCore::ContextMenuItemTagCopyLinkToClipboard;
    case kWKContextMenuItemTagOpenImageInNewWindow:
        return CyberCore::ContextMenuItemTagOpenImageInNewWindow;
    case kWKContextMenuItemTagDownloadImageToDisk:
        return CyberCore::ContextMenuItemTagDownloadImageToDisk;
    case kWKContextMenuItemTagCopyImageToClipboard:
        return CyberCore::ContextMenuItemTagCopyImageToClipboard;
#if ENABLE(ACCESSIBILITY_ANIMATION_CONTROL)
    case kWKContextMenuItemTagPlayAllAnimations:
        return CyberCore::ContextMenuItemTagPlayAllAnimations;
    case kWKContextMenuItemTagPauseAllAnimations:
        return CyberCore::ContextMenuItemTagPauseAllAnimations;
    case kWKContextMenuItemTagPlayAnimation:
        return CyberCore::ContextMenuItemTagPlayAnimation;
    case kWKContextMenuItemTagPauseAnimation:
        return CyberCore::ContextMenuItemTagPauseAnimation;
#endif // ENABLE(ACCESSIBILITY_ANIMATION_CONTROL)
    case kWKContextMenuItemTagOpenFrameInNewWindow:
#if PLATFORM(GTK)
    case kWKContextMenuItemTagCopyImageUrlToClipboard:
        return CyberCore::ContextMenuItemTagCopyImageUrlToClipboard;
#endif
        return CyberCore::ContextMenuItemTagOpenFrameInNewWindow;
    case kWKContextMenuItemTagCopy:
        return CyberCore::ContextMenuItemTagCopy;
    case kWKContextMenuItemTagGoBack:
        return CyberCore::ContextMenuItemTagGoBack;
    case kWKContextMenuItemTagGoForward:
        return CyberCore::ContextMenuItemTagGoForward;
    case kWKContextMenuItemTagStop:
        return CyberCore::ContextMenuItemTagStop;
    case kWKContextMenuItemTagReload:
        return CyberCore::ContextMenuItemTagReload;
    case kWKContextMenuItemTagCut:
        return CyberCore::ContextMenuItemTagCut;
    case kWKContextMenuItemTagPaste:
        return CyberCore::ContextMenuItemTagPaste;
#if PLATFORM(GTK)
    case kWKContextMenuItemTagSelectAll:
        return CyberCore::ContextMenuItemTagSelectAll;
#endif
    case kWKContextMenuItemTagSpellingGuess:
        return CyberCore::ContextMenuItemTagSpellingGuess;
    case kWKContextMenuItemTagNoGuessesFound:
        return CyberCore::ContextMenuItemTagNoGuessesFound;
    case kWKContextMenuItemTagIgnoreSpelling:
        return CyberCore::ContextMenuItemTagIgnoreSpelling;
    case kWKContextMenuItemTagLearnSpelling:
        return CyberCore::ContextMenuItemTagLearnSpelling;
    case kWKContextMenuItemTagOther:
        return CyberCore::ContextMenuItemTagOther;
    case kWKContextMenuItemTagSearchInSpotlight:
        return CyberCore::ContextMenuItemTagSearchInSpotlight;
    case kWKContextMenuItemTagSearchWeb:
        return CyberCore::ContextMenuItemTagSearchWeb;
    case kWKContextMenuItemTagLookUpInDictionary:
        return CyberCore::ContextMenuItemTagLookUpInDictionary;
    case kWKContextMenuItemTagOpenWithDefaultApplication:
        return CyberCore::ContextMenuItemTagOpenWithDefaultApplication;
    case kWKContextMenuItemTagPDFActualSize:
        return CyberCore::ContextMenuItemPDFActualSize;
    case kWKContextMenuItemTagPDFZoomIn:
        return CyberCore::ContextMenuItemPDFZoomIn;
    case kWKContextMenuItemTagPDFZoomOut:
        return CyberCore::ContextMenuItemPDFZoomOut;
    case kWKContextMenuItemTagPDFAutoSize:
        return CyberCore::ContextMenuItemPDFAutoSize;
    case kWKContextMenuItemTagPDFSinglePage:
        return CyberCore::ContextMenuItemPDFSinglePage;
    case kWKContextMenuItemTagPDFFacingPages:
        return CyberCore::ContextMenuItemPDFFacingPages;
    case kWKContextMenuItemTagPDFContinuous:
        return CyberCore::ContextMenuItemPDFContinuous;
    case kWKContextMenuItemTagPDFNextPage:
        return CyberCore::ContextMenuItemPDFNextPage;
    case kWKContextMenuItemTagPDFPreviousPage:
        return CyberCore::ContextMenuItemPDFPreviousPage;
    case kWKContextMenuItemTagOpenLink:
        return CyberCore::ContextMenuItemTagOpenLink;
    case kWKContextMenuItemTagIgnoreGrammar:
        return CyberCore::ContextMenuItemTagIgnoreGrammar;
    case kWKContextMenuItemTagSpellingMenu:
        return CyberCore::ContextMenuItemTagSpellingMenu;
    case kWKContextMenuItemTagShowSpellingPanel:
        return CyberCore::ContextMenuItemTagShowSpellingPanel;
    case kWKContextMenuItemTagCheckSpelling:
        return CyberCore::ContextMenuItemTagCheckSpelling;
    case kWKContextMenuItemTagCheckSpellingWhileTyping:
        return CyberCore::ContextMenuItemTagCheckSpellingWhileTyping;
    case kWKContextMenuItemTagCheckGrammarWithSpelling:
        return CyberCore::ContextMenuItemTagCheckGrammarWithSpelling;
    case kWKContextMenuItemTagFontMenu:
        return CyberCore::ContextMenuItemTagFontMenu;
    case kWKContextMenuItemTagShowFonts:
        return CyberCore::ContextMenuItemTagShowFonts;
    case kWKContextMenuItemTagBold:
        return CyberCore::ContextMenuItemTagBold;
    case kWKContextMenuItemTagItalic:
        return CyberCore::ContextMenuItemTagItalic;
    case kWKContextMenuItemTagUnderline:
        return CyberCore::ContextMenuItemTagUnderline;
    case kWKContextMenuItemTagOutline:
        return CyberCore::ContextMenuItemTagOutline;
    case kWKContextMenuItemTagStyles:
        return CyberCore::ContextMenuItemTagStyles;
    case kWKContextMenuItemTagShowColors:
        return CyberCore::ContextMenuItemTagShowColors;
    case kWKContextMenuItemTagSpeechMenu:
        return CyberCore::ContextMenuItemTagSpeechMenu;
    case kWKContextMenuItemTagStartSpeaking:
        return CyberCore::ContextMenuItemTagStartSpeaking;
    case kWKContextMenuItemTagStopSpeaking:
        return CyberCore::ContextMenuItemTagStopSpeaking;
    case kWKContextMenuItemTagWritingDirectionMenu:
        return CyberCore::ContextMenuItemTagWritingDirectionMenu;
    case kWKContextMenuItemTagDefaultDirection:
        return CyberCore::ContextMenuItemTagDefaultDirection;
    case kWKContextMenuItemTagLeftToRight:
        return CyberCore::ContextMenuItemTagLeftToRight;
    case kWKContextMenuItemTagRightToLeft:
        return CyberCore::ContextMenuItemTagRightToLeft;
    case kWKContextMenuItemTagPDFSinglePageScrolling:
        return CyberCore::ContextMenuItemTagPDFSinglePageScrolling;
    case kWKContextMenuItemTagPDFFacingPagesScrolling:
        return CyberCore::ContextMenuItemTagPDFFacingPagesScrolling;
    case kWKContextMenuItemTagDictationAlternative:
        return CyberCore::ContextMenuItemTagDictationAlternative;
    case kWKContextMenuItemTagInspectElement:
        return CyberCore::ContextMenuItemTagInspectElement;
    case kWKContextMenuItemTagTextDirectionMenu:
        return CyberCore::ContextMenuItemTagTextDirectionMenu;
    case kWKContextMenuItemTagTextDirectionDefault:
        return CyberCore::ContextMenuItemTagTextDirectionDefault;
    case kWKContextMenuItemTagTextDirectionLeftToRight:
        return CyberCore::ContextMenuItemTagTextDirectionLeftToRight;
    case kWKContextMenuItemTagTextDirectionRightToLeft:
        return CyberCore::ContextMenuItemTagTextDirectionRightToLeft;
    case kWKContextMenuItemTagOpenMediaInNewWindow:
        return CyberCore::ContextMenuItemTagOpenMediaInNewWindow;
    case kWKContextMenuItemTagDownloadMediaToDisk:
        return CyberCore::ContextMenuItemTagDownloadMediaToDisk;
    case kWKContextMenuItemTagCopyMediaLinkToClipboard:
        return CyberCore::ContextMenuItemTagCopyMediaLinkToClipboard;
    case kWKContextMenuItemTagToggleMediaControls:
        return CyberCore::ContextMenuItemTagToggleMediaControls;
    case kWKContextMenuItemTagToggleMediaLoop:
        return CyberCore::ContextMenuItemTagToggleMediaLoop;
    case kWKContextMenuItemTagToggleVideoFullscreen:
        return CyberCore::ContextMenuItemTagToggleVideoFullscreen;
    case kWKContextMenuItemTagEnterVideoFullscreen:
        return CyberCore::ContextMenuItemTagEnterVideoFullscreen;
    case kWKContextMenuItemTagToggleVideoEnhancedFullscreen:
        return CyberCore::ContextMenuItemTagToggleVideoEnhancedFullscreen;
    case kWKContextMenuItemTagMediaPlayPause:
        return CyberCore::ContextMenuItemTagMediaPlayPause;
    case kWKContextMenuItemTagMediaMute:
        return CyberCore::ContextMenuItemTagMediaMute;
    case kWKContextMenuItemTagAddHighlightToCurrentQuickNote:
        return CyberCore::ContextMenuItemTagAddHighlightToCurrentQuickNote;
    case kWKContextMenuItemTagAddHighlightToNewQuickNote:
        return CyberCore::ContextMenuItemTagAddHighlightToNewQuickNote;
#if PLATFORM(COCOA)
    case kWKContextMenuItemTagCorrectSpellingAutomatically:
        return CyberCore::ContextMenuItemTagCorrectSpellingAutomatically;
    case kWKContextMenuItemTagSubstitutionsMenu:
        return CyberCore::ContextMenuItemTagSubstitutionsMenu;
    case kWKContextMenuItemTagShowSubstitutions:
        return CyberCore::ContextMenuItemTagShowSubstitutions;
    case kWKContextMenuItemTagSmartCopyPaste:
        return CyberCore::ContextMenuItemTagSmartCopyPaste;
    case kWKContextMenuItemTagSmartQuotes:
        return CyberCore::ContextMenuItemTagSmartQuotes;
    case kWKContextMenuItemTagSmartDashes:
        return CyberCore::ContextMenuItemTagSmartDashes;
    case kWKContextMenuItemTagSmartLinks:
        return CyberCore::ContextMenuItemTagSmartLinks;
    case kWKContextMenuItemTagTextReplacement:
        return CyberCore::ContextMenuItemTagTextReplacement;
    case kWKContextMenuItemTagTransformationsMenu:
        return CyberCore::ContextMenuItemTagTransformationsMenu;
    case kWKContextMenuItemTagMakeUpperCase:
        return CyberCore::ContextMenuItemTagMakeUpperCase;
    case kWKContextMenuItemTagMakeLowerCase:
        return CyberCore::ContextMenuItemTagMakeLowerCase;
    case kWKContextMenuItemTagCapitalize:
        return CyberCore::ContextMenuItemTagCapitalize;
    case kWKContextMenuItemTagChangeBack:
        return CyberCore::ContextMenuItemTagChangeBack;
    case kWKContextMenuItemTagShareMenu:
        return CyberCore::ContextMenuItemTagShareMenu;
#endif
    case kWKContextMenuItemTagRevealImage:
        return CyberCore::ContextMenuItemTagLookUpImage;
    case kWKContextMenuItemTagTranslate:
        return CyberCore::ContextMenuItemTagTranslate;
    case kWKContextMenuItemTagCopyCroppedImage:
        return CyberCore::ContextMenuItemTagCopySubject;
    case kWKContextMenuItemTagOpenLinkInThisWindow:
    default:
        if (tag < kWKContextMenuItemBaseApplicationTag && !(tag >= CyberCore::ContextMenuItemBaseCustomTag && tag <= CyberCore::ContextMenuItemLastCustomTag))
            LOG_ERROR("WKContextMenuItemTag %i is an unknown tag but is below the allowable custom tag value of %i", tag, kWKContextMenuItemBaseApplicationTag);
        return static_cast<CyberCore::ContextMenuAction>(tag);
    }
}

inline WKContextMenuItemType toAPI(CyberCore::ContextMenuItemType type)
{
    switch(type) {
    case CyberCore::ActionType:
        return kWKContextMenuItemTypeAction;
    case CyberCore::CheckableActionType:
        return kWKContextMenuItemTypeCheckableAction;
    case CyberCore::SeparatorType:
        return kWKContextMenuItemTypeSeparator;
    case CyberCore::SubmenuType:
        return kWKContextMenuItemTypeSubmenu;
    default:
        ASSERT_NOT_REACHED();
        return kWKContextMenuItemTypeAction;
    }
}

inline OptionSet<FindOptions> toFindOptions(WKFindOptions wkFindOptions)
{
    OptionSet<FindOptions> findOptions;

    if (wkFindOptions & kWKFindOptionsCaseInsensitive)
        findOptions.add(FindOptions::CaseInsensitive);
    if (wkFindOptions & kWKFindOptionsAtWordStarts)
        findOptions.add(FindOptions::AtWordStarts);
    if (wkFindOptions & kWKFindOptionsTreatMedialCapitalAsWordStart)
        findOptions.add(FindOptions::TreatMedialCapitalAsWordStart);
    if (wkFindOptions & kWKFindOptionsBackwards)
        findOptions.add(FindOptions::Backwards);
    if (wkFindOptions & kWKFindOptionsWrapAround)
        findOptions.add(FindOptions::WrapAround);
    if (wkFindOptions & kWKFindOptionsShowOverlay)
        findOptions.add(FindOptions::ShowOverlay);
    if (wkFindOptions & kWKFindOptionsShowFindIndicator)
        findOptions.add(FindOptions::ShowFindIndicator);
    if (wkFindOptions & kWKFindOptionsShowHighlight)
        findOptions.add(FindOptions::ShowHighlight);

    return findOptions;
}

inline WKFrameNavigationType toAPI(CyberCore::NavigationType type)
{
    switch (type) {
    case CyberCore::NavigationType::LinkClicked:
        return kWKFrameNavigationTypeLinkClicked;
    case CyberCore::NavigationType::FormSubmitted:
        return kWKFrameNavigationTypeFormSubmitted;
    case CyberCore::NavigationType::BackForward:
        return kWKFrameNavigationTypeBackForward;
    case CyberCore::NavigationType::Reload:
        return kWKFrameNavigationTypeReload;
    case CyberCore::NavigationType::FormResubmitted:
        return kWKFrameNavigationTypeFormResubmitted;
    case CyberCore::NavigationType::Other:
        return kWKFrameNavigationTypeOther;
    }

    ASSERT_NOT_REACHED();
    return kWKFrameNavigationTypeOther;

}

inline WKSameDocumentNavigationType toAPI(SameDocumentNavigationType type)
{
    switch (type) {
    case SameDocumentNavigationType::AnchorNavigation:
        return kWKSameDocumentNavigationAnchorNavigation;
    case SameDocumentNavigationType::SessionStatePush:
        return kWKSameDocumentNavigationSessionStatePush;
    case SameDocumentNavigationType::SessionStateReplace:
        return kWKSameDocumentNavigationSessionStateReplace;
    case SameDocumentNavigationType::SessionStatePop:
        return kWKSameDocumentNavigationSessionStatePop;
    }

    ASSERT_NOT_REACHED();
    return kWKSameDocumentNavigationAnchorNavigation;
}

inline SameDocumentNavigationType toSameDocumentNavigationType(WKSameDocumentNavigationType wkType)
{
    switch (wkType) {
    case kWKSameDocumentNavigationAnchorNavigation:
        return SameDocumentNavigationType::AnchorNavigation;
    case kWKSameDocumentNavigationSessionStatePush:
        return SameDocumentNavigationType::SessionStatePush;
    case kWKSameDocumentNavigationSessionStateReplace:
        return SameDocumentNavigationType::SessionStateReplace;
    case kWKSameDocumentNavigationSessionStatePop:
        return SameDocumentNavigationType::SessionStatePop;
    }
    
    ASSERT_NOT_REACHED();
    return SameDocumentNavigationType::AnchorNavigation;
}

inline WKDiagnosticLoggingResultType toAPI(CyberCore::DiagnosticLoggingResultType type)
{
    switch (type) {
    case CyberCore::DiagnosticLoggingResultPass:
        return kWKDiagnosticLoggingResultPass;
    case CyberCore::DiagnosticLoggingResultFail:
        return kWKDiagnosticLoggingResultFail;
    case CyberCore::DiagnosticLoggingResultNoop:
        return kWKDiagnosticLoggingResultNoop;
    }

    ASSERT_NOT_REACHED();
    return { };
}

inline CyberCore::DiagnosticLoggingResultType toDiagnosticLoggingResultType(WKDiagnosticLoggingResultType wkType)
{
    switch (wkType) {
    case kWKDiagnosticLoggingResultPass:
        return CyberCore::DiagnosticLoggingResultPass;
    case kWKDiagnosticLoggingResultFail:
        return CyberCore::DiagnosticLoggingResultFail;
    case kWKDiagnosticLoggingResultNoop:
        return CyberCore::DiagnosticLoggingResultNoop;
    }

    ASSERT_NOT_REACHED();
    return { };
}

inline WKLayoutMilestones toWKLayoutMilestones(OptionSet<CyberCore::LayoutMilestone> milestones)
{
    unsigned wkMilestones = 0;

    if (milestones & CyberCore::DidFirstLayout)
        wkMilestones |= kWKDidFirstLayout;
    if (milestones & CyberCore::DidFirstVisuallyNonEmptyLayout)
        wkMilestones |= kWKDidFirstVisuallyNonEmptyLayout;
    if (milestones & CyberCore::DidHitRelevantRepaintedObjectsAreaThreshold)
        wkMilestones |= kWKDidHitRelevantRepaintedObjectsAreaThreshold;
    if (milestones & CyberCore::DidFirstFlushForHeaderLayer)
        wkMilestones |= kWKDidFirstFlushForHeaderLayer;
    if (milestones & CyberCore::DidFirstLayoutAfterSuppressedIncrementalRendering)
        wkMilestones |= kWKDidFirstLayoutAfterSuppressedIncrementalRendering;
    if (milestones & CyberCore::DidFirstPaintAfterSuppressedIncrementalRendering)
        wkMilestones |= kWKDidFirstPaintAfterSuppressedIncrementalRendering;
    if (milestones & CyberCore::DidRenderSignificantAmountOfText)
        wkMilestones |= kWKDidRenderSignificantAmountOfText;
    if (milestones & CyberCore::DidFirstMeaningfulPaint)
        wkMilestones |= kWKDidFirstMeaningfulPaint;

    return wkMilestones;
}

inline OptionSet<CyberCore::LayoutMilestone> toLayoutMilestones(WKLayoutMilestones wkMilestones)
{
    OptionSet<CyberCore::LayoutMilestone> milestones;

    if (wkMilestones & kWKDidFirstLayout)
        milestones.add(CyberCore::DidFirstLayout);
    if (wkMilestones & kWKDidFirstVisuallyNonEmptyLayout)
        milestones.add(CyberCore::DidFirstVisuallyNonEmptyLayout);
    if (wkMilestones & kWKDidHitRelevantRepaintedObjectsAreaThreshold)
        milestones.add(CyberCore::DidHitRelevantRepaintedObjectsAreaThreshold);
    if (wkMilestones & kWKDidFirstFlushForHeaderLayer)
        milestones.add(CyberCore::DidFirstFlushForHeaderLayer);
    if (wkMilestones & kWKDidFirstLayoutAfterSuppressedIncrementalRendering)
        milestones.add(CyberCore::DidFirstLayoutAfterSuppressedIncrementalRendering);
    if (wkMilestones & kWKDidFirstPaintAfterSuppressedIncrementalRendering)
        milestones.add(CyberCore::DidFirstPaintAfterSuppressedIncrementalRendering);
    if (wkMilestones & kWKDidRenderSignificantAmountOfText)
        milestones.add(CyberCore::DidRenderSignificantAmountOfText);
    if (wkMilestones & kWKDidFirstMeaningfulPaint)
        milestones.add(CyberCore::DidFirstMeaningfulPaint);
    
    return milestones;
}

inline CyberCore::VisibilityState toVisibilityState(WKPageVisibilityState wkPageVisibilityState)
{
    switch (wkPageVisibilityState) {
    case kWKPageVisibilityStateVisible:
        return CyberCore::VisibilityState::Visible;
    case kWKPageVisibilityStateHidden:
        return CyberCore::VisibilityState::Hidden;
    case kWKPageVisibilityStatePrerender:
        return CyberCore::VisibilityState::Hidden;
    }

    ASSERT_NOT_REACHED();
    return CyberCore::VisibilityState::Visible;
}

inline ImageOptions toImageOptions(WKImageOptions wkImageOptions)
{
    unsigned imageOptions = 0;

    if (wkImageOptions & kWKImageOptionsShareable)
        imageOptions |= ImageOptionsShareable;

    return static_cast<ImageOptions>(imageOptions);
}

inline SnapshotOptions snapshotOptionsFromImageOptions(WKImageOptions wkImageOptions)
{
    unsigned snapshotOptions = 0;

    if (wkImageOptions & kWKImageOptionsShareable)
        snapshotOptions |= SnapshotOptionsShareable;
    
    return snapshotOptions;
}

inline SnapshotOptions toSnapshotOptions(WKSnapshotOptions wkSnapshotOptions)
{
    unsigned snapshotOptions = 0;

    if (wkSnapshotOptions & kWKSnapshotOptionsShareable)
        snapshotOptions |= SnapshotOptionsShareable;
    if (wkSnapshotOptions & kWKSnapshotOptionsExcludeSelectionHighlighting)
        snapshotOptions |= SnapshotOptionsExcludeSelectionHighlighting;
    if (wkSnapshotOptions & kWKSnapshotOptionsInViewCoordinates)
        snapshotOptions |= SnapshotOptionsInViewCoordinates;
    if (wkSnapshotOptions & kWKSnapshotOptionsPaintSelectionRectangle)
        snapshotOptions |= SnapshotOptionsPaintSelectionRectangle;
    if (wkSnapshotOptions & kWKSnapshotOptionsForceBlackText)
        snapshotOptions |= SnapshotOptionsForceBlackText;
    if (wkSnapshotOptions & kWKSnapshotOptionsForceWhiteText)
        snapshotOptions |= SnapshotOptionsForceWhiteText;
    if (wkSnapshotOptions & kWKSnapshotOptionsPrinting)
        snapshotOptions |= SnapshotOptionsPrinting;
    if (wkSnapshotOptions & kWKSnapshotOptionsExtendedColor)
        snapshotOptions |= SnapshotOptionsUseScreenColorSpace;

    return snapshotOptions;
}

inline CyberCore::UserScriptInjectionTime toUserScriptInjectionTime(_WKUserScriptInjectionTime wkInjectedTime)
{
    switch (wkInjectedTime) {
    case kWKInjectAtDocumentStart:
        return CyberCore::UserScriptInjectionTime::DocumentStart;
    case kWKInjectAtDocumentEnd:
        return CyberCore::UserScriptInjectionTime::DocumentEnd;
    }

    ASSERT_NOT_REACHED();
    return CyberCore::UserScriptInjectionTime::DocumentStart;
}

inline _WKUserScriptInjectionTime toWKUserScriptInjectionTime(CyberCore::UserScriptInjectionTime injectedTime)
{
    switch (injectedTime) {
    case CyberCore::UserScriptInjectionTime::DocumentStart:
        return kWKInjectAtDocumentStart;
    case CyberCore::UserScriptInjectionTime::DocumentEnd:
        return kWKInjectAtDocumentEnd;
    }

    ASSERT_NOT_REACHED();
    return kWKInjectAtDocumentStart;
}

inline CyberCore::UserContentInjectedFrames toUserContentInjectedFrames(WKUserContentInjectedFrames wkInjectedFrames)
{
    switch (wkInjectedFrames) {
    case kWKInjectInAllFrames:
        return CyberCore::UserContentInjectedFrames::InjectInAllFrames;
    case kWKInjectInTopFrameOnly:
        return CyberCore::UserContentInjectedFrames::InjectInTopFrameOnly;
    }

    ASSERT_NOT_REACHED();
    return CyberCore::UserContentInjectedFrames::InjectInAllFrames;
}

} // namespace CyberKit
