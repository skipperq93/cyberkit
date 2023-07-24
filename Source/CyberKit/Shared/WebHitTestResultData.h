/*
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#pragma once

#include "APIObject.h"
#include "SharedMemory.h"
#include <CyberCore/DictionaryPopupInfo.h>
#include <CyberCore/FloatPoint.h>
#include <CyberCore/IntRect.h>
#include <CyberCore/PageOverlay.h>
#include <wtf/EnumTraits.h>
#include <wtf/Forward.h>
#include <wtf/RefPtr.h>
#include <wtf/text/WTFString.h>

OBJC_CLASS DDActionContext;

namespace IPC {
class Decoder;
class Encoder;
}

namespace CyberCore {
class HitTestResult;
}

namespace CyberKit {

struct WebHitTestResultData {
    String absoluteImageURL;
    String absolutePDFURL;
    String absoluteLinkURL;
    String absoluteMediaURL;
    String linkLabel;
    String linkTitle;
    String linkSuggestedFilename;
    bool isContentEditable;
    CyberCore::IntRect elementBoundingBox;
    enum class IsScrollbar { No, Vertical, Horizontal };
    IsScrollbar isScrollbar;
    bool isSelected;
    bool isTextNode;
    bool isOverTextInsideFormControlElement;
    bool isDownloadableMedia;

    String lookupText;
    String toolTipText;
    RefPtr<CyberKit::SharedMemory> imageSharedMemory;
    uint64_t imageSize;

#if PLATFORM(MAC)
    RetainPtr<DDActionContext> detectedDataActionContext;
#endif
    CyberCore::FloatRect detectedDataBoundingBox;
    RefPtr<CyberCore::TextIndicator> detectedDataTextIndicator;
    CyberCore::PageOverlay::PageOverlayID detectedDataOriginatingPageOverlay;

    CyberCore::DictionaryPopupInfo dictionaryPopupInfo;

    RefPtr<CyberCore::TextIndicator> linkTextIndicator;

    WebHitTestResultData();
    WebHitTestResultData(const CyberCore::HitTestResult&, const String& toolTipText);
    WebHitTestResultData(const CyberCore::HitTestResult&, bool includeImage);
    ~WebHitTestResultData();

    void encode(IPC::Encoder&) const;
    void platformEncode(IPC::Encoder&) const;
    static WARN_UNUSED_RETURN bool decode(IPC::Decoder&, WebHitTestResultData&);
    static WARN_UNUSED_RETURN bool platformDecode(IPC::Decoder&, WebHitTestResultData&);

    CyberCore::IntRect elementBoundingBoxInWindowCoordinates(const CyberCore::HitTestResult&);
};

} // namespace CyberKit

namespace WTF {

template<> struct EnumTraits<CyberKit::WebHitTestResultData::IsScrollbar> {
    using values = EnumValues<
    CyberKit::WebHitTestResultData::IsScrollbar,
    CyberKit::WebHitTestResultData::IsScrollbar::No,
    CyberKit::WebHitTestResultData::IsScrollbar::Vertical,
    CyberKit::WebHitTestResultData::IsScrollbar::Horizontal
    >;
};

} // namespace WTF
