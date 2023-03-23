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
#include "WebUserContentControllerProxy.h"
#include "WebsiteAutoplayPolicy.h"
#include "WebsiteAutoplayQuirk.h"
#include "WebsiteLegacyOverflowScrollingTouchPolicy.h"
#include "WebsiteMediaSourcePolicy.h"
#include "WebsiteMetaViewportPolicy.h"
#include "WebsitePopUpPolicy.h"
#include "WebsiteSimulatedMouseEventsDispatchPolicy.h"
#include <CyberCore/CustomHeaderFields.h>
#include <CyberCore/DeviceOrientationOrMotionPermissionState.h>
#include <CyberCore/DocumentLoader.h>
#include <CyberCore/FrameLoaderTypes.h>
#include <CyberCore/HTTPHeaderField.h>
#include <CyberCore/NetworkConnectionIntegrity.h>
#include <wtf/OptionSet.h>
#include <wtf/Vector.h>

namespace WebKit {
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

    WebKit::WebsitePoliciesData data();

    const CyberCore::ContentExtensionEnablement& contentExtensionEnablement() const { return m_contentExtensionEnablement; }
    void setContentExtensionEnablement(CyberCore::ContentExtensionEnablement&& enablement) { m_contentExtensionEnablement = WTFMove(enablement); }

    void setActiveContentRuleListActionPatterns(HashMap<WTF::String, Vector<WTF::String>>&& patterns) { m_activeContentRuleListActionPatterns = WTFMove(patterns); }
    const HashMap<WTF::String, Vector<WTF::String>>& activeContentRuleListActionPatterns() const { return m_activeContentRuleListActionPatterns; }
    
    OptionSet<WebKit::WebsiteAutoplayQuirk> allowedAutoplayQuirks() const { return m_allowedAutoplayQuirks; }
    void setAllowedAutoplayQuirks(OptionSet<WebKit::WebsiteAutoplayQuirk> quirks) { m_allowedAutoplayQuirks = quirks; }
    
    WebKit::WebsiteAutoplayPolicy autoplayPolicy() const { return m_autoplayPolicy; }
    void setAutoplayPolicy(WebKit::WebsiteAutoplayPolicy policy) { m_autoplayPolicy = policy; }

#if ENABLE(DEVICE_ORIENTATION)
    CyberCore::DeviceOrientationOrMotionPermissionState deviceOrientationAndMotionAccessState() const { return m_deviceOrientationAndMotionAccessState; }
    void setDeviceOrientationAndMotionAccessState(CyberCore::DeviceOrientationOrMotionPermissionState state) { m_deviceOrientationAndMotionAccessState = state; }
#endif

    const Vector<CyberCore::CustomHeaderFields>& customHeaderFields() const { return m_customHeaderFields; }
    void setCustomHeaderFields(Vector<CyberCore::CustomHeaderFields>&& fields) { m_customHeaderFields = WTFMove(fields); }

    WebKit::WebsitePopUpPolicy popUpPolicy() const { return m_popUpPolicy; }
    void setPopUpPolicy(WebKit::WebsitePopUpPolicy policy) { m_popUpPolicy = policy; }

    WebKit::WebsiteDataStore* websiteDataStore() const { return m_websiteDataStore.get(); }
    void setWebsiteDataStore(RefPtr<WebKit::WebsiteDataStore>&&);
    
    WebKit::WebUserContentControllerProxy* userContentController() const { return m_userContentController.get(); }
    void setUserContentController(RefPtr<WebKit::WebUserContentControllerProxy>&&);

    void setCustomUserAgent(const WTF::String& customUserAgent) { m_customUserAgent = customUserAgent; }
    const WTF::String& customUserAgent() const { return m_customUserAgent; }

    void setCustomUserAgentAsSiteSpecificQuirks(const WTF::String& customUserAgent) { m_customUserAgentAsSiteSpecificQuirks = customUserAgent; }
    const WTF::String& customUserAgentAsSiteSpecificQuirks() const { return m_customUserAgentAsSiteSpecificQuirks; }

    void setCustomNavigatorPlatform(const WTF::String& customNavigatorPlatform) { m_customNavigatorPlatform = customNavigatorPlatform; }
    const WTF::String& customNavigatorPlatform() const { return m_customNavigatorPlatform; }

    WebKit::WebContentMode preferredContentMode() const { return m_preferredContentMode; }
    void setPreferredContentMode(WebKit::WebContentMode mode) { m_preferredContentMode = mode; }

    WebKit::WebsiteMetaViewportPolicy metaViewportPolicy() const { return m_metaViewportPolicy; }
    void setMetaViewportPolicy(WebKit::WebsiteMetaViewportPolicy policy) { m_metaViewportPolicy = policy; }

    WebKit::WebsiteMediaSourcePolicy mediaSourcePolicy() const { return m_mediaSourcePolicy; }
    void setMediaSourcePolicy(WebKit::WebsiteMediaSourcePolicy policy) { m_mediaSourcePolicy = policy; }

    WebKit::WebsiteSimulatedMouseEventsDispatchPolicy simulatedMouseEventsDispatchPolicy() const { return m_simulatedMouseEventsDispatchPolicy; }
    void setSimulatedMouseEventsDispatchPolicy(WebKit::WebsiteSimulatedMouseEventsDispatchPolicy policy) { m_simulatedMouseEventsDispatchPolicy = policy; }

    WebKit::WebsiteLegacyOverflowScrollingTouchPolicy legacyOverflowScrollingTouchPolicy() const { return m_legacyOverflowScrollingTouchPolicy; }
    void setLegacyOverflowScrollingTouchPolicy(WebKit::WebsiteLegacyOverflowScrollingTouchPolicy policy) { m_legacyOverflowScrollingTouchPolicy = policy; }

    bool allowSiteSpecificQuirksToOverrideContentMode() const { return m_allowSiteSpecificQuirksToOverrideContentMode; }
    void setAllowSiteSpecificQuirksToOverrideContentMode(bool value) { m_allowSiteSpecificQuirksToOverrideContentMode = value; }

    WTF::String applicationNameForDesktopUserAgent() const { return m_applicationNameForDesktopUserAgent; }
    void setApplicationNameForDesktopUserAgent(const WTF::String& applicationName) { m_applicationNameForDesktopUserAgent = applicationName; }

    bool allowContentChangeObserverQuirk() const { return m_allowContentChangeObserverQuirk; }
    void setAllowContentChangeObserverQuirk(bool allow) { m_allowContentChangeObserverQuirk = allow; }

    CyberCore::AllowsContentJavaScript allowsContentJavaScript() const { return m_allowsContentJavaScript; }
    void setAllowsContentJavaScript(CyberCore::AllowsContentJavaScript allows) { m_allowsContentJavaScript = allows; }

    bool lockdownModeEnabled() const;
    void setLockdownModeEnabled(std::optional<bool> enabled) { m_lockdownModeEnabled = enabled; }
    bool isLockdownModeExplicitlySet() const { return !!m_lockdownModeEnabled; }

    CyberCore::ColorSchemePreference colorSchemePreference() const { return m_colorSchemePreference; }
    void setColorSchemePreference(CyberCore::ColorSchemePreference colorSchemePreference) { m_colorSchemePreference = colorSchemePreference; }

    CyberCore::MouseEventPolicy mouseEventPolicy() const { return m_mouseEventPolicy; }
    void setMouseEventPolicy(CyberCore::MouseEventPolicy policy) { m_mouseEventPolicy = policy; }

    CyberCore::ModalContainerObservationPolicy modalContainerObservationPolicy() const { return m_modalContainerObservationPolicy; }
    void setModalContainerObservationPolicy(CyberCore::ModalContainerObservationPolicy policy) { m_modalContainerObservationPolicy = policy; }

    OptionSet<CyberCore::NetworkConnectionIntegrity> networkConnectionIntegrityPolicy() const { return m_networkConnectionIntegrityPolicy; }
    void setNetworkConnectionIntegrityPolicy(OptionSet<CyberCore::NetworkConnectionIntegrity> policy) { m_networkConnectionIntegrityPolicy = policy; }

    bool idempotentModeAutosizingOnlyHonorsPercentages() const { return m_idempotentModeAutosizingOnlyHonorsPercentages; }
    void setIdempotentModeAutosizingOnlyHonorsPercentages(bool idempotentModeAutosizingOnlyHonorsPercentages) { m_idempotentModeAutosizingOnlyHonorsPercentages = idempotentModeAutosizingOnlyHonorsPercentages; }

    bool allowPrivacyProxy() const { return m_allowPrivacyProxy; }
    void setAllowPrivacyProxy(bool allow) { m_allowPrivacyProxy = allow; }

private:
    // FIXME: replace most or all of these members with a WebsitePoliciesData.
    CyberCore::ContentExtensionEnablement m_contentExtensionEnablement { CyberCore::ContentExtensionDefaultEnablement::Enabled, { } };
    HashMap<WTF::String, Vector<WTF::String>> m_activeContentRuleListActionPatterns;
    OptionSet<WebKit::WebsiteAutoplayQuirk> m_allowedAutoplayQuirks;
    WebKit::WebsiteAutoplayPolicy m_autoplayPolicy { WebKit::WebsiteAutoplayPolicy::Default };
#if ENABLE(DEVICE_ORIENTATION)
    CyberCore::DeviceOrientationOrMotionPermissionState m_deviceOrientationAndMotionAccessState { CyberCore::DeviceOrientationOrMotionPermissionState::Prompt };
#endif
    Vector<CyberCore::CustomHeaderFields> m_customHeaderFields;
    WebKit::WebsitePopUpPolicy m_popUpPolicy { WebKit::WebsitePopUpPolicy::Default };
    RefPtr<WebKit::WebsiteDataStore> m_websiteDataStore;
    RefPtr<WebKit::WebUserContentControllerProxy> m_userContentController;
    WTF::String m_customUserAgent;
    WTF::String m_customUserAgentAsSiteSpecificQuirks;
    WTF::String m_customNavigatorPlatform;
    WebKit::WebContentMode m_preferredContentMode { WebKit::WebContentMode::Recommended };
    WebKit::WebsiteMetaViewportPolicy m_metaViewportPolicy { WebKit::WebsiteMetaViewportPolicy::Default };
    WebKit::WebsiteMediaSourcePolicy m_mediaSourcePolicy { WebKit::WebsiteMediaSourcePolicy::Default };
    WebKit::WebsiteSimulatedMouseEventsDispatchPolicy m_simulatedMouseEventsDispatchPolicy { WebKit::WebsiteSimulatedMouseEventsDispatchPolicy::Default };
    WebKit::WebsiteLegacyOverflowScrollingTouchPolicy m_legacyOverflowScrollingTouchPolicy { WebKit::WebsiteLegacyOverflowScrollingTouchPolicy::Default };
    bool m_allowSiteSpecificQuirksToOverrideContentMode { false };
    WTF::String m_applicationNameForDesktopUserAgent;
    bool m_allowContentChangeObserverQuirk { false };
    CyberCore::AllowsContentJavaScript m_allowsContentJavaScript { CyberCore::AllowsContentJavaScript::Yes };
    CyberCore::MouseEventPolicy m_mouseEventPolicy { CyberCore::MouseEventPolicy::Default };
    CyberCore::ModalContainerObservationPolicy m_modalContainerObservationPolicy { CyberCore::ModalContainerObservationPolicy::Disabled };
    OptionSet<CyberCore::NetworkConnectionIntegrity> m_networkConnectionIntegrityPolicy;
    bool m_idempotentModeAutosizingOnlyHonorsPercentages { false };
    std::optional<bool> m_lockdownModeEnabled;
    CyberCore::ColorSchemePreference m_colorSchemePreference { CyberCore::ColorSchemePreference::NoPreference };
    bool m_allowPrivacyProxy { true };
};

} // namespace API
