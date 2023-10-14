/*
 * Copyright (C) 2015-2020 Apple Inc. All rights reserved.
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
#include <CyberCore/ContentSecurityPolicy.h>
#include <CyberCore/ShouldRelaxThirdPartyCookieBlocking.h>
#include <wtf/Forward.h>
#include <wtf/GetPtr.h>
#include <wtf/HashMap.h>
#include <wtf/RobinHoodHashSet.h>
#include <wtf/text/WTFString.h>

#if PLATFORM(IOS_FAMILY)
OBJC_PROTOCOL(_UIClickInteractionDriving);
#include <wtf/RetainPtr.h>
#endif

namespace CyberKit {
class VisitedLinkStore;
class WebPageGroup;
class WebPageProxy;
class WebPreferences;
class WebProcessPool;
class WebURLSchemeHandler;
class WebUserContentControllerProxy;
class WebsiteDataStore;

#if ENABLE(WK_WEB_EXTENSIONS)
class WebExtensionController;
#endif

#if PLATFORM(IOS_FAMILY)
enum class AttributionOverrideTesting : uint8_t {
    NoOverride,
    UserInitiated,
    AppInitiated
};
#endif

}

namespace API {

class ApplicationManifest;
class WebsitePolicies;

class PageConfiguration : public ObjectImpl<Object::Type::PageConfiguration> {
public:
    static Ref<PageConfiguration> create();

    explicit PageConfiguration();
    virtual ~PageConfiguration();

    Ref<PageConfiguration> copy() const;

    // FIXME: The configuration properties should return their default values
    // rather than nullptr.
    
    CyberKit::WebProcessPool* processPool();
    void setProcessPool(CyberKit::WebProcessPool*);

    CyberKit::WebUserContentControllerProxy* userContentController();
    void setUserContentController(CyberKit::WebUserContentControllerProxy*);

#if ENABLE(WK_WEB_EXTENSIONS)
    CyberKit::WebExtensionController* webExtensionController();
    void setWebExtensionController(CyberKit::WebExtensionController*);

    CyberKit::WebExtensionController* weakWebExtensionController();
    void setWeakWebExtensionController(CyberKit::WebExtensionController*);
#endif

    CyberKit::WebPageGroup* pageGroup();
    void setPageGroup(CyberKit::WebPageGroup*);

    CyberKit::WebPreferences* preferences();
    void setPreferences(CyberKit::WebPreferences*);

    CyberKit::WebPageProxy* relatedPage() const;
    void setRelatedPage(CyberKit::WebPageProxy*);

    CyberKit::WebPageProxy* pageToCloneSessionStorageFrom() const;
    void setPageToCloneSessionStorageFrom(CyberKit::WebPageProxy*);

    CyberKit::VisitedLinkStore* visitedLinkStore();
    void setVisitedLinkStore(CyberKit::VisitedLinkStore*);

    CyberKit::WebsiteDataStore* websiteDataStore();
    void setWebsiteDataStore(CyberKit::WebsiteDataStore*);

    WebsitePolicies* defaultWebsitePolicies() const;
    void setDefaultWebsitePolicies(WebsitePolicies*);

#if PLATFORM(IOS_FAMILY)
    bool clientNavigationsRunAtForegroundPriority() const { return m_data.clientNavigationsRunAtForegroundPriority; }
    void setClientNavigationsRunAtForegroundPriority(bool value) { m_data.clientNavigationsRunAtForegroundPriority = value; }

    bool canShowWhileLocked() const { return m_data.canShowWhileLocked; }
    void setCanShowWhileLocked(bool canShowWhileLocked) { m_data.canShowWhileLocked = canShowWhileLocked; }

    const RetainPtr<_UIClickInteractionDriving>& clickInteractionDriverForTesting() const { return m_data.clickInteractionDriverForTesting; }
    void setClickInteractionDriverForTesting(RetainPtr<_UIClickInteractionDriving>&& driver) { m_data.clickInteractionDriverForTesting = WTFMove(driver); }
#endif
    bool initialCapitalizationEnabled() { return m_data.initialCapitalizationEnabled; }
    void setInitialCapitalizationEnabled(bool initialCapitalizationEnabled) { m_data.initialCapitalizationEnabled = initialCapitalizationEnabled; }

    std::optional<double> cpuLimit() const { return m_data.cpuLimit; }
    void setCPULimit(double cpuLimit) { m_data.cpuLimit = cpuLimit; }

    bool waitsForPaintAfterViewDidMoveToWindow() const { return m_data.waitsForPaintAfterViewDidMoveToWindow; }
    void setWaitsForPaintAfterViewDidMoveToWindow(bool shouldSynchronize) { m_data.waitsForPaintAfterViewDidMoveToWindow = shouldSynchronize; }

    bool drawsBackground() const { return m_data.drawsBackground; }
    void setDrawsBackground(bool drawsBackground) { m_data.drawsBackground = drawsBackground; }

    bool isControlledByAutomation() const { return m_data.controlledByAutomation; }
    void setControlledByAutomation(bool controlledByAutomation) { m_data.controlledByAutomation = controlledByAutomation; }

    const WTF::String& overrideContentSecurityPolicy() const { return m_data.overrideContentSecurityPolicy; }
    void setOverrideContentSecurityPolicy(const WTF::String& overrideContentSecurityPolicy) { m_data.overrideContentSecurityPolicy = overrideContentSecurityPolicy; }

#if PLATFORM(COCOA)
    const WTF::Vector<WTF::String>& additionalSupportedImageTypes() const { return m_data.additionalSupportedImageTypes; }
    void setAdditionalSupportedImageTypes(WTF::Vector<WTF::String>&& additionalSupportedImageTypes) { m_data.additionalSupportedImageTypes = WTFMove(additionalSupportedImageTypes); }
#endif

#if ENABLE(APPLICATION_MANIFEST)
    ApplicationManifest* applicationManifest() const;
    void setApplicationManifest(ApplicationManifest*);
#endif

    RefPtr<CyberKit::WebURLSchemeHandler> urlSchemeHandlerForURLScheme(const WTF::String&);
    void setURLSchemeHandlerForURLScheme(Ref<CyberKit::WebURLSchemeHandler>&&, const WTF::String&);
    const HashMap<WTF::String, Ref<CyberKit::WebURLSchemeHandler>>& urlSchemeHandlers() { return m_data.urlSchemeHandlers; }

    const Vector<WTF::String>& corsDisablingPatterns() const { return m_data.corsDisablingPatterns; }
    void setCORSDisablingPatterns(Vector<WTF::String>&& patterns) { m_data.corsDisablingPatterns = WTFMove(patterns); }

    const HashSet<WTF::String>& maskedURLSchemes() const { return m_data.maskedURLSchemes; }
    void setMaskedURLSchemes(HashSet<WTF::String>&& schemes) { m_data.maskedURLSchemes = WTFMove(schemes); }

    bool userScriptsShouldWaitUntilNotification() const { return m_data.userScriptsShouldWaitUntilNotification; }
    void setUserScriptsShouldWaitUntilNotification(bool value) { m_data.userScriptsShouldWaitUntilNotification = value; }

    bool crossOriginAccessControlCheckEnabled() const { return m_data.crossOriginAccessControlCheckEnabled; }
    void setCrossOriginAccessControlCheckEnabled(bool enabled) { m_data.crossOriginAccessControlCheckEnabled = enabled; }

    const WTF::String& processDisplayName() const { return m_data.processDisplayName; }
    void setProcessDisplayName(const WTF::String& name) { m_data.processDisplayName = name; }

    bool loadsSubresources() const { return m_data.loadsSubresources; }
    void setLoadsSubresources(bool loads) { m_data.loadsSubresources = loads; }

    const std::optional<MemoryCompactLookupOnlyRobinHoodHashSet<WTF::String>>& allowedNetworkHosts() const { return m_data.allowedNetworkHosts; }
    void setAllowedNetworkHosts(std::optional<MemoryCompactLookupOnlyRobinHoodHashSet<WTF::String>>&& hosts) { m_data.allowedNetworkHosts = WTFMove(hosts); }

#if ENABLE(APP_BOUND_DOMAINS)
    bool ignoresAppBoundDomains() const { return m_data.ignoresAppBoundDomains; }
    void setIgnoresAppBoundDomains(bool shouldIgnore) { m_data.ignoresAppBoundDomains = shouldIgnore; }
    
    bool limitsNavigationsToAppBoundDomains() const { return m_data.limitsNavigationsToAppBoundDomains; }
    void setLimitsNavigationsToAppBoundDomains(bool limits) { m_data.limitsNavigationsToAppBoundDomains = limits; }
#endif

    void setMediaCaptureEnabled(bool value) { m_data.mediaCaptureEnabled = value; }
    bool mediaCaptureEnabled() const { return m_data.mediaCaptureEnabled; }

    void setHTTPSUpgradeEnabled(bool enabled) { m_data.httpsUpgradeEnabled = enabled; }
    bool httpsUpgradeEnabled() const { return m_data.httpsUpgradeEnabled; }

    void setShouldRelaxThirdPartyCookieBlocking(CyberCore::ShouldRelaxThirdPartyCookieBlocking value) { m_data.shouldRelaxThirdPartyCookieBlocking = value; }
    CyberCore::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking() const { return m_data.shouldRelaxThirdPartyCookieBlocking; }

    void setAttributedBundleIdentifier(WTF::String&& identifier) { m_data.attributedBundleIdentifier = WTFMove(identifier); }
    const WTF::String& attributedBundleIdentifier() const { return m_data.attributedBundleIdentifier; }

#if PLATFORM(IOS_FAMILY)
    CyberKit::AttributionOverrideTesting appInitiatedOverrideValueForTesting() const { return m_data.appInitiatedOverrideValueForTesting; }
    void setAppInitiatedOverrideValueForTesting(CyberKit::AttributionOverrideTesting appInitiatedOverrideValueForTesting) { m_data.appInitiatedOverrideValueForTesting = appInitiatedOverrideValueForTesting; }
#endif

#if HAVE(TOUCH_BAR)
    bool requiresUserActionForEditingControlsManager() const { return m_data.requiresUserActionForEditingControlsManager; }
    void setRequiresUserActionForEditingControlsManager(bool value) { m_data.requiresUserActionForEditingControlsManager = value; }
#endif

    bool isLockdownModeExplicitlySet() const;
    bool lockdownModeEnabled() const;
    
    void setAllowTestOnlyIPC(bool enabled) { m_data.allowTestOnlyIPC = enabled; }
    bool allowTestOnlyIPC() const { return m_data.allowTestOnlyIPC; }

    void setDelaysWebProcessLaunchUntilFirstLoad(bool);
    bool delaysWebProcessLaunchUntilFirstLoad() const;

    void setContentSecurityPolicyModeForExtension(CyberCore::ContentSecurityPolicyModeForExtension mode) { m_data.contentSecurityPolicyModeForExtension = mode; }
    CyberCore::ContentSecurityPolicyModeForExtension contentSecurityPolicyModeForExtension() const { return m_data.contentSecurityPolicyModeForExtension; }

private:
    struct Data {
        RefPtr<CyberKit::WebProcessPool> processPool;
        RefPtr<CyberKit::WebUserContentControllerProxy> userContentController;
#if ENABLE(WK_WEB_EXTENSIONS)
        RefPtr<CyberKit::WebExtensionController> webExtensionController;
        WeakPtr<CyberKit::WebExtensionController> weakWebExtensionController;
#endif
        RefPtr<CyberKit::WebPageGroup> pageGroup;
        RefPtr<CyberKit::WebPreferences> preferences;
        RefPtr<CyberKit::WebPageProxy> relatedPage;
        WeakPtr<CyberKit::WebPageProxy> pageToCloneSessionStorageFrom;
        RefPtr<CyberKit::VisitedLinkStore> visitedLinkStore;

        RefPtr<CyberKit::WebsiteDataStore> websiteDataStore;
        RefPtr<WebsitePolicies> defaultWebsitePolicies;

#if PLATFORM(IOS_FAMILY)
        bool clientNavigationsRunAtForegroundPriority { true };
        bool canShowWhileLocked { false };
        CyberKit::AttributionOverrideTesting appInitiatedOverrideValueForTesting { CyberKit::AttributionOverrideTesting::NoOverride };
        RetainPtr<_UIClickInteractionDriving> clickInteractionDriverForTesting;
#endif
        bool initialCapitalizationEnabled { true };
        bool waitsForPaintAfterViewDidMoveToWindow { true };
        bool drawsBackground { true };
        bool controlledByAutomation { false };
        bool allowTestOnlyIPC { false };
        std::optional<bool> delaysWebProcessLaunchUntilFirstLoad;
        std::optional<double> cpuLimit;

        WTF::String overrideContentSecurityPolicy;

#if PLATFORM(COCOA)
        WTF::Vector<WTF::String> additionalSupportedImageTypes;
#endif

#if ENABLE(APPLICATION_MANIFEST)
        RefPtr<ApplicationManifest> applicationManifest;
#endif

        HashMap<WTF::String, Ref<CyberKit::WebURLSchemeHandler>> urlSchemeHandlers;
        Vector<WTF::String> corsDisablingPatterns;
        HashSet<WTF::String> maskedURLSchemes;
        bool userScriptsShouldWaitUntilNotification { true };
        bool crossOriginAccessControlCheckEnabled { true };
        WTF::String processDisplayName;
        bool loadsSubresources { true };
        std::optional<MemoryCompactLookupOnlyRobinHoodHashSet<WTF::String>> allowedNetworkHosts;

#if ENABLE(APP_BOUND_DOMAINS)
        bool ignoresAppBoundDomains { false };
        bool limitsNavigationsToAppBoundDomains { false };
#endif

        bool mediaCaptureEnabled { false };
        bool httpsUpgradeEnabled { true };

        CyberCore::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking { CyberCore::ShouldRelaxThirdPartyCookieBlocking::No };
        WTF::String attributedBundleIdentifier;

#if HAVE(TOUCH_BAR)
        bool requiresUserActionForEditingControlsManager { false };
#endif

        CyberCore::ContentSecurityPolicyModeForExtension contentSecurityPolicyModeForExtension { CyberCore::ContentSecurityPolicyModeForExtension::None };
    };

    // All data members should be added to the Data structure to avoid breaking PageConfiguration::copy().
    Data m_data;
};

} // namespace API
