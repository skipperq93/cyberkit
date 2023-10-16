/*
 * Copyright (C) 2014-2017 Apple Inc. All rights reserved.
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

#import "config.h"
#import "UserAgent.h"

#if PLATFORM(IOS_FAMILY)

#import "Device.h"
#import "SystemVersion.h"
#import <CyberCore/RuntimeApplicationChecks.h>
#import <pal/spi/ios/MobileGestaltSPI.h>
#import <pal/spi/ios/UIKitSPI.h>
#import <wtf/RetainPtr.h>
#import <wtf/cf/TypeCastsCF.h>
#import <wtf/cocoa/RuntimeApplicationChecksCocoa.h>

#import <pal/ios/UIKitSoftLink.h>

namespace CyberCore {

static inline bool isClassic()
{
    return [[PAL::getUIApplicationClass() sharedApplication] _isClassic];
}

static inline bool isClassicPad()
{
    return [PAL::getUIApplicationClass() _classicMode] == UIApplicationSceneClassicModeOriginalPad;
}

static inline bool isClassicPhone()
{
    return isClassic() && [PAL::getUIApplicationClass() _classicMode] != UIApplicationSceneClassicModeOriginalPad;
}

ASCIILiteral osNameForUserAgent()
{
    if (deviceHasIPadCapability() && !isClassicPhone())
        return "OS"_s;
    return "iPhone OS"_s;
}

#ifndef CYBERKIT_DEPLOYMENT_TARGET
static StringView deviceNameForUserAgent()
{
    if (isClassic()) {
        if (isClassicPad())
            return "iPad"_s;
        return "iPhone"_s;
    }

    static NeverDestroyed<String> name = [] {
        auto name = deviceName();
#if PLATFORM(IOS_FAMILY_SIMULATOR)
        size_t location = name.find(" Simulator"_s);
        if (location != notFound)
            return name.left(location);
#endif
        return name;
    }();
    return name.get();
}
#endif

String standardUserAgentWithApplicationName(const String& applicationName, const String& userAgentOSVersion, UserAgentType type)
{
    auto separator = applicationName.isEmpty() ? "" : " ";

    if (type == UserAgentType::Desktop)
        return makeString("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleCyberKit/605.1.15 (KHTML, like Gecko) Version/17.0 Safari/605.1.15");

#ifdef CYBERKIT_DEPLOYMENT_TARGET
    UNUSED_PARAM(userAgentOSVersion);
    UNUSED_PARAM(separator);
    return makeString("Mozilla/5.0 (iPad; CPU OS 17_0_1 like Mac OS X) AppleCyberKit/605.1.15 (KHTML, like Gecko) Version/17.0 Mobile/15E148 Safari/604.1");
#else
    if (!linkedOnOrAfterSDKWithBehavior(SDKAlignedBehavior::DoesNotOverrideUAFromNSUserDefault)) {
        if (auto override = dynamic_cf_cast<CFStringRef>(adoptCF(CFPreferencesCopyAppValue(CFSTR("UserAgent"), CFSTR("com.apple.WebFoundation"))))) {
            static BOOL hasLoggedDeprecationWarning = NO;
            if (!hasLoggedDeprecationWarning) {
                NSLog(@"Reading an override UA from the NSUserDefault [com.apple.WebFoundation UserAgent]. This is incompatible with the modern need to compose the UA and clients should use the API to set the application name or UA instead.");
                hasLoggedDeprecationWarning = YES;
            }
            return override.get();
        }
    }

    auto osVersion = userAgentOSVersion.isEmpty() ? systemMarketingVersionForUserAgentString() : userAgentOSVersion;
    return makeString("Mozilla/5.0 (", deviceNameForUserAgent(), "; CPU ", osNameForUserAgent(), " ", osVersion, " like Mac OS X) AppleCyberKit/605.1.15 (KHTML, like Gecko)", separator, applicationName);
#endif
}

} // namespace CyberCore.

#endif // PLATFORM(IOS_FAMILY)
