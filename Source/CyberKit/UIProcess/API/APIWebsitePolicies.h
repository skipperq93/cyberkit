/*
 * Copyright (C) 2016 Apple Inc. All rights reserved.
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

#include "APIObject.h"
#include "WebContentMode.h"
#include "WebsiteAutoplayPolicy.h"
#include "WebsiteAutoplayQuirk.h"
#include "WebsiteLegacyOverflowScrollingTouchPolicy.h"
#include "WebsiteMediaSourcePolicy.h"
#include "WebsiteMetaViewportPolicy.h"
#include "WebsitePopUpPolicy.h"
#include "WebsiteSimulatedMouseEventsDispatchPolicy.h"
#include <CyberCore/CustomHeaderFields.h>
#include <CyberCore/DeviceOrientationOrMotionPermissionState.h>
#include <CyberCore/HTTPHeaderField.h>
#include <wtf/OptionSet.h>
#include <wtf/Vector.h>

namespace CyberKit {
struct WebsitePoliciesData;
class WebsiteDataStore;
}

namespace API {

class WebsitePolicies final : public API::ObjectImpl<API::Object::Type::WebsitePolicies> {
public:
    static Ref<WebsitePolicies> create() { return adoptRef(*new WebsitePolicies); }
    WebsitePolicies();
    ~WebsitePolicies();

    Ref<WebsitePolicies> copy() const;

    bool contentBlockersEnabled() const { return m_contentBlockersEnabled; }
    void setContentBlockersEnabled(bool enabled) { m_contentBlockersEnabled = enabled; }
    
    OptionSet<CyberKit::WebsiteAutoplayQuirk> allowedAutoplayQuirks() const { return m_allowedAutoplayQuirks; }
    void setAllowedAutoplayQuirks(OptionSet<CyberKit::WebsiteAutoplayQuirk> quirks) { m_allowedAutoplayQuirks = quirks; }
    
    CyberKit::WebsiteAutoplayPolicy autoplayPolicy() const { return m_autoplayPolicy; }
    void setAutoplayPolicy(CyberKit::WebsiteAutoplayPolicy policy) { m_autoplayPolicy = policy; }

#if ENABLE(DEVICE_ORIENTATION)
    CyberCore::DeviceOrientationOrMotionPermissionState deviceOrientationAndMotionAccessState() const { return m_deviceOrientationAndMotionAccessState; }
    void setDeviceOrientationAndMotionAccessState(CyberCore::DeviceOrientationOrMotionPermissionState state) { m_deviceOrientationAndMotionAccessState = state; }
#endif

    const Vector<CyberCore::HTTPHeaderField>& legacyCustomHeaderFields() const { return m_legacyCustomHeaderFields; }
    void setLegacyCustomHeaderFields(Vector<CyberCore::HTTPHeaderField>&& fields) { m_legacyCustomHeaderFields = WTFMove(fields); }

    const Vector<CyberCore::CustomHeaderFields>& customHeaderFields() const { return m_customHeaderFields; }
    void setCustomHeaderFields(Vector<CyberCore::CustomHeaderFields>&& fields) { m_customHeaderFields = WTFMove(fields); }

    CyberKit::WebsitePopUpPolicy popUpPolicy() const { return m_popUpPolicy; }
    void setPopUpPolicy(CyberKit::WebsitePopUpPolicy policy) { m_popUpPolicy = policy; }

    CyberKit::WebsiteDataStore* websiteDataStore() const { return m_websiteDataStore.get(); }
    void setWebsiteDataStore(RefPtr<CyberKit::WebsiteDataStore>&&);

    CyberKit::WebsitePoliciesData data();

    void setCustomUserAgent(const WTF::String& customUserAgent) { m_customUserAgent = customUserAgent; }
    const WTF::String& customUserAgent() const { return m_customUserAgent; }

    void setCustomUserAgentAsSiteSpecificQuirks(const WTF::String& customUserAgent) { m_customUserAgentAsSiteSpecificQuirks = customUserAgent; }
    const WTF::String& customUserAgentAsSiteSpecificQuirks() const { return m_customUserAgentAsSiteSpecificQuirks; }

    void setCustomNavigatorPlatform(const WTF::String& customNavigatorPlatform) { m_customNavigatorPlatform = customNavigatorPlatform; }
    const WTF::String& customNavigatorPlatform() const { return m_customNavigatorPlatform; }

    CyberKit::WebContentMode preferredContentMode() const { return m_preferredContentMode; }
    void setPreferredContentMode(CyberKit::WebContentMode mode) { m_preferredContentMode = mode; }

    CyberKit::WebsiteMetaViewportPolicy metaViewportPolicy() const { return m_metaViewportPolicy; }
    void setMetaViewportPolicy(CyberKit::WebsiteMetaViewportPolicy policy) { m_metaViewportPolicy = policy; }

    CyberKit::WebsiteMediaSourcePolicy mediaSourcePolicy() const { return m_mediaSourcePolicy; }
    void setMediaSourcePolicy(CyberKit::WebsiteMediaSourcePolicy policy) { m_mediaSourcePolicy = policy; }

    CyberKit::WebsiteSimulatedMouseEventsDispatchPolicy simulatedMouseEventsDispatchPolicy() const { return m_simulatedMouseEventsDispatchPolicy; }
    void setSimulatedMouseEventsDispatchPolicy(CyberKit::WebsiteSimulatedMouseEventsDispatchPolicy policy) { m_simulatedMouseEventsDispatchPolicy = policy; }

    CyberKit::WebsiteLegacyOverflowScrollingTouchPolicy legacyOverflowScrollingTouchPolicy() const { return m_legacyOverflowScrollingTouchPolicy; }
    void setLegacyOverflowScrollingTouchPolicy(CyberKit::WebsiteLegacyOverflowScrollingTouchPolicy policy) { m_legacyOverflowScrollingTouchPolicy = policy; }

    bool allowSiteSpecificQuirksToOverrideContentMode() const { return m_allowSiteSpecificQuirksToOverrideContentMode; }
    void setAllowSiteSpecificQuirksToOverrideContentMode(bool value) { m_allowSiteSpecificQuirksToOverrideContentMode = value; }

    WTF::String applicationNameForDesktopUserAgent() const { return m_applicationNameForDesktopUserAgent; }
    void setApplicationNameForDesktopUserAgent(const WTF::String& applicationName) { m_applicationNameForDesktopUserAgent = applicationName; }

    bool allowContentChangeObserverQuirk() const { return m_allowContentChangeObserverQuirk; }
    void setAllowContentChangeObserverQuirk(bool allow) { m_allowContentChangeObserverQuirk = allow; }

private:
    WebsitePolicies(bool contentBlockersEnabled, OptionSet<CyberKit::WebsiteAutoplayQuirk>, CyberKit::WebsiteAutoplayPolicy, Vector<CyberCore::HTTPHeaderField>&&, Vector<CyberCore::CustomHeaderFields>&&, CyberKit::WebsitePopUpPolicy, RefPtr<CyberKit::WebsiteDataStore>&&);

    bool m_contentBlockersEnabled { true };
    OptionSet<CyberKit::WebsiteAutoplayQuirk> m_allowedAutoplayQuirks;
    CyberKit::WebsiteAutoplayPolicy m_autoplayPolicy { CyberKit::WebsiteAutoplayPolicy::Default };
#if ENABLE(DEVICE_ORIENTATION)
    CyberCore::DeviceOrientationOrMotionPermissionState m_deviceOrientationAndMotionAccessState { CyberCore::DeviceOrientationOrMotionPermissionState::Prompt };
#endif
    Vector<CyberCore::HTTPHeaderField> m_legacyCustomHeaderFields;
    Vector<CyberCore::CustomHeaderFields> m_customHeaderFields;
    CyberKit::WebsitePopUpPolicy m_popUpPolicy { CyberKit::WebsitePopUpPolicy::Default };
    RefPtr<CyberKit::WebsiteDataStore> m_websiteDataStore;
    WTF::String m_customUserAgent;
    WTF::String m_customUserAgentAsSiteSpecificQuirks;
    WTF::String m_customNavigatorPlatform;
    CyberKit::WebContentMode m_preferredContentMode { CyberKit::WebContentMode::Recommended };
    CyberKit::WebsiteMetaViewportPolicy m_metaViewportPolicy { CyberKit::WebsiteMetaViewportPolicy::Default };
    CyberKit::WebsiteMediaSourcePolicy m_mediaSourcePolicy { CyberKit::WebsiteMediaSourcePolicy::Default };
    CyberKit::WebsiteSimulatedMouseEventsDispatchPolicy m_simulatedMouseEventsDispatchPolicy { CyberKit::WebsiteSimulatedMouseEventsDispatchPolicy::Default };
    CyberKit::WebsiteLegacyOverflowScrollingTouchPolicy m_legacyOverflowScrollingTouchPolicy { CyberKit::WebsiteLegacyOverflowScrollingTouchPolicy::Default };
    bool m_allowSiteSpecificQuirksToOverrideContentMode { false };
    WTF::String m_applicationNameForDesktopUserAgent;
    bool m_allowContentChangeObserverQuirk { false };
};

} // namespace API
