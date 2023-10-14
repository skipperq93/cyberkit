/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
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
#include "LibWebRTCProviderCocoa.h"

#if USE(LIBWEBRTC)

#include "DeprecatedGlobalSettings.h"
#include "MediaCapabilitiesInfo.h"
#include "VP9UtilitiesCocoa.h"

ALLOW_UNUSED_PARAMETERS_BEGIN
ALLOW_COMMA_BEGIN
#include <webrtc/sdk/CyberKit/CyberKitDecoder.h>
#include <webrtc/sdk/CyberKit/CyberKitEncoder.h>
ALLOW_UNUSED_PARAMETERS_END
ALLOW_COMMA_END
#include <webrtc/sdk/CyberKit/CyberKitVP8Decoder.h>
#include <webrtc/sdk/CyberKit/CyberKitVP9Decoder.h>
#include <wtf/MainThread.h>
#include <wtf/darwin/WeakLinking.h>

WTF_WEAK_LINK_FORCE_IMPORT(webrtc::setApplicationStatus);

namespace CyberCore {

UniqueRef<WebRTCProvider> WebRTCProvider::create()
{
    return makeUniqueRef<LibWebRTCProviderCocoa>();
}

void WebRTCProvider::setH264HardwareEncoderAllowed(bool allowed)
{
    if (webRTCAvailable())
        webrtc::setH264HardwareEncoderAllowed(allowed);
}

LibWebRTCProviderCocoa::~LibWebRTCProviderCocoa()
{
}

std::unique_ptr<webrtc::VideoDecoderFactory> LibWebRTCProviderCocoa::createDecoderFactory()
{
    ASSERT(isMainThread());

    if (!webRTCAvailable())
        return nullptr;

    auto vp9Support = isSupportingVP9Profile2() ? webrtc::CyberKitVP9::Profile0And2 : isSupportingVP9Profile0() ? webrtc::CyberKitVP9::Profile0 : webrtc::CyberKitVP9::Off;
    return webrtc::createCyberKitDecoderFactory(isSupportingH265() ? webrtc::CyberKitH265::On : webrtc::CyberKitH265::Off, vp9Support, isSupportingVP9VTB() ? webrtc::CyberKitVP9VTB::On : webrtc::CyberKitVP9VTB::Off, isSupportingAV1() ? webrtc::CyberKitAv1::On : webrtc::CyberKitAv1::Off);
}

std::unique_ptr<webrtc::VideoEncoderFactory> LibWebRTCProviderCocoa::createEncoderFactory()
{
    ASSERT(isMainThread());

    if (!webRTCAvailable())
        return nullptr;

    auto vp9Support = isSupportingVP9Profile2() ? webrtc::CyberKitVP9::Profile0And2 : isSupportingVP9Profile0() ? webrtc::CyberKitVP9::Profile0 : webrtc::CyberKitVP9::Off;
    return webrtc::createCyberKitEncoderFactory(isSupportingH265() ? webrtc::CyberKitH265::On : webrtc::CyberKitH265::Off, vp9Support, DeprecatedGlobalSettings::webRTCH264LowLatencyEncoderEnabled() ? webrtc::CyberKitH264LowLatency::On : webrtc::CyberKitH264LowLatency::Off, isSupportingAV1() ? webrtc::CyberKitAv1::On : webrtc::CyberKitAv1::Off);
}

std::optional<MediaCapabilitiesInfo> LibWebRTCProviderCocoa::computeVPParameters(const VideoConfiguration& configuration)
{
    return CyberCore::computeVPParameters(configuration);
}

bool LibWebRTCProviderCocoa::isVPSoftwareDecoderSmooth(const VideoConfiguration& configuration)
{
    return CyberCore::isVPSoftwareDecoderSmooth(configuration);
}

void LibWebRTCProviderCocoa::setActive(bool value)
{
    if (webRTCAvailable())
        webrtc::setApplicationStatus(value);
}

bool WebRTCProvider::webRTCAvailable()
{
#if PLATFORM(IOS)
    return true;
#else
    return !!webrtc::setApplicationStatus;
#endif
}

void LibWebRTCProvider::registerCyberKitVP9Decoder()
{
    if (webRTCAvailable())
        webrtc::registerCyberKitVP9Decoder();
}

void LibWebRTCProvider::registerCyberKitVP8Decoder()
{
    if (webRTCAvailable())
        webrtc::registerCyberKitVP8Decoder();
}

} // namespace CyberCore

#endif // USE(LIBWEBRTC)
