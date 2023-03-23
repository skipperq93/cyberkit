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

#pragma once

#include "WebHitTestResultData.h"
#include "WebMouseEvent.h"
#include <CyberCore/BackForwardItemIdentifier.h>
#include <CyberCore/FloatPoint.h>
#include <CyberCore/FrameLoaderTypes.h>
#include <CyberCore/PrivateClickMeasurement.h>
#include <CyberCore/SecurityOriginData.h>

namespace IPC {
class Decoder;
class Encoder;
}

namespace CyberCore {
typedef int SandboxFlags;
}

namespace CyberKit {

struct NavigationActionData {
    CyberCore::NavigationType navigationType { CyberCore::NavigationType::Other };
    OptionSet<WebEventModifier> modifiers;
    WebMouseEventButton mouseButton { WebMouseEventButton::NoButton };
    WebMouseEventSyntheticClickType syntheticClickType { WebMouseEventSyntheticClickType::NoTap };
    uint64_t userGestureTokenIdentifier { 0 };
    bool canHandleRequest { false };
    CyberCore::ShouldOpenExternalURLsPolicy shouldOpenExternalURLsPolicy { CyberCore::ShouldOpenExternalURLsPolicy::ShouldNotAllow };
    WTF::String downloadAttribute;
    CyberCore::FloatPoint clickLocationInRootViewCoordinates;
    bool isRedirect { false };
    bool treatAsSameOriginNavigation { false };
    bool hasOpenedFrames { false };
    bool openedByDOMWithOpener { false };
    bool hasOpener { false };
    CyberCore::SecurityOriginData requesterOrigin;
    std::optional<CyberCore::BackForwardItemIdentifier> targetBackForwardItemIdentifier;
    std::optional<CyberCore::BackForwardItemIdentifier> sourceBackForwardItemIdentifier;
    CyberCore::LockHistory lockHistory { CyberCore::LockHistory::No };
    CyberCore::LockBackForwardList lockBackForwardList { CyberCore::LockBackForwardList::No };
    WTF::String clientRedirectSourceForHistory;
    CyberCore::SandboxFlags effectiveSandboxFlags { 0 };
    std::optional<CyberCore::PrivateClickMeasurement> privateClickMeasurement;
#if PLATFORM(MAC) || HAVE(UIKIT_WITH_MOUSE_SUPPORT)
    std::optional<CyberKit::WebHitTestResultData> webHitTestResultData;
#endif
};

}
