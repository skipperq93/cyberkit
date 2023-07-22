/*
 * Copyright (C) 2014-2016 Apple Inc. All rights reserved.
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
#include "NavigationActionData.h"

#include "ArgumentCoders.h"
#include "Decoder.h"
#include "Encoder.h"
#include "CyberCoreArgumentCoders.h"

namespace CyberKit {

void NavigationActionData::encode(IPC::Encoder& encoder) const
{
    encoder.encodeEnum(navigationType);
    encoder << modifiers;
    encoder.encodeEnum(mouseButton);
    encoder.encodeEnum(syntheticClickType);
    encoder << userGestureTokenIdentifier;
    encoder << canHandleRequest;
    encoder.encodeEnum(shouldOpenExternalURLsPolicy);
    encoder << downloadAttribute;
    encoder << clickLocationInRootViewCoordinates;
    encoder << isRedirect;
    encoder << treatAsSameOriginNavigation;
    encoder << hasOpenedFrames;
    encoder << openedByDOMWithOpener;
    encoder << requesterOrigin;
    encoder << targetBackForwardItemIdentifier;
    encoder << sourceBackForwardItemIdentifier;
    encoder.encodeEnum(lockHistory);
    encoder.encodeEnum(lockBackForwardList);
    encoder << clientRedirectSourceForHistory;
    encoder << adClickAttribution;
}

Optional<NavigationActionData> NavigationActionData::decode(IPC::Decoder& decoder)
{
    CyberCore::NavigationType navigationType;
    if (!decoder.decodeEnum(navigationType))
        return WTF::nullopt;
    
    OptionSet<WebEvent::Modifier> modifiers;
    if (!decoder.decode(modifiers))
        return WTF::nullopt;

    WebMouseEvent::Button mouseButton;
    if (!decoder.decodeEnum(mouseButton))
        return WTF::nullopt;
    
    WebMouseEvent::SyntheticClickType syntheticClickType;
    if (!decoder.decodeEnum(syntheticClickType))
        return WTF::nullopt;
    
    Optional<uint64_t> userGestureTokenIdentifier;
    decoder >> userGestureTokenIdentifier;
    if (!userGestureTokenIdentifier)
        return WTF::nullopt;
    
    Optional<bool> canHandleRequest;
    decoder >> canHandleRequest;
    if (!canHandleRequest)
        return WTF::nullopt;
    
    CyberCore::ShouldOpenExternalURLsPolicy shouldOpenExternalURLsPolicy;
    if (!decoder.decodeEnum(shouldOpenExternalURLsPolicy))
        return WTF::nullopt;
    
    Optional<String> downloadAttribute;
    decoder >> downloadAttribute;
    if (!downloadAttribute)
        return WTF::nullopt;
    
    CyberCore::FloatPoint clickLocationInRootViewCoordinates;
    if (!decoder.decode(clickLocationInRootViewCoordinates))
        return WTF::nullopt;
    
    Optional<bool> isRedirect;
    decoder >> isRedirect;
    if (!isRedirect)
        return WTF::nullopt;

    Optional<bool> treatAsSameOriginNavigation;
    decoder >> treatAsSameOriginNavigation;
    if (!treatAsSameOriginNavigation)
        return WTF::nullopt;

    Optional<bool> hasOpenedFrames;
    decoder >> hasOpenedFrames;
    if (!hasOpenedFrames)
        return WTF::nullopt;

    Optional<bool> openedByDOMWithOpener;
    decoder >> openedByDOMWithOpener;
    if (!openedByDOMWithOpener)
        return WTF::nullopt;

    Optional<CyberCore::SecurityOriginData> requesterOrigin;
    decoder >> requesterOrigin;
    if (!requesterOrigin)
        return WTF::nullopt;

    Optional<Optional<CyberCore::BackForwardItemIdentifier>> targetBackForwardItemIdentifier;
    decoder >> targetBackForwardItemIdentifier;
    if (!targetBackForwardItemIdentifier)
        return WTF::nullopt;

    Optional<Optional<CyberCore::BackForwardItemIdentifier>> sourceBackForwardItemIdentifier;
    decoder >> sourceBackForwardItemIdentifier;
    if (!sourceBackForwardItemIdentifier)
        return WTF::nullopt;

    CyberCore::LockHistory lockHistory;
    if (!decoder.decodeEnum(lockHistory))
        return WTF::nullopt;

    CyberCore::LockBackForwardList lockBackForwardList;
    if (!decoder.decodeEnum(lockBackForwardList))
        return WTF::nullopt;

    Optional<String> clientRedirectSourceForHistory;
    decoder >> clientRedirectSourceForHistory;
    if (!clientRedirectSourceForHistory)
        return WTF::nullopt;

    Optional<Optional<CyberCore::AdClickAttribution>> adClickAttribution;
    decoder >> adClickAttribution;
    if (!adClickAttribution)
        return WTF::nullopt;

    return {{ WTFMove(navigationType), modifiers, WTFMove(mouseButton), WTFMove(syntheticClickType), WTFMove(*userGestureTokenIdentifier),
        WTFMove(*canHandleRequest), WTFMove(shouldOpenExternalURLsPolicy), WTFMove(*downloadAttribute), WTFMove(clickLocationInRootViewCoordinates),
        WTFMove(*isRedirect), *treatAsSameOriginNavigation, *hasOpenedFrames, *openedByDOMWithOpener, WTFMove(*requesterOrigin),
        WTFMove(*targetBackForwardItemIdentifier), WTFMove(*sourceBackForwardItemIdentifier), lockHistory, lockBackForwardList, WTFMove(*clientRedirectSourceForHistory), WTFMove(*adClickAttribution) }};
}

} // namespace CyberKit
