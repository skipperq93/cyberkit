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

#include "DrawingAreaInfo.h"
#include "FrameTreeCreationParameters.h"
#include "LayerTreeContext.h"
#include "SandboxExtension.h"
#include "SessionState.h"
#include "UserContentControllerParameters.h"
#include "CyberCoreArgumentCoders.h"
#include "WebPageGroupData.h"
#include "WebPageProxyIdentifier.h"
#include "WebPreferencesStore.h"
#include "WebURLSchemeHandlerIdentifier.h"
#include <CyberCore/ActivityState.h>
#include <CyberCore/Color.h>
#include <CyberCore/ContentSecurityPolicy.h>
#include <CyberCore/DestinationColorSpace.h>
#include <CyberCore/FloatSize.h>
#include <CyberCore/FrameIdentifier.h>
#include <CyberCore/HighlightVisibility.h>
#include <CyberCore/IntDegrees.h>
#include <CyberCore/IntSize.h>
#include <CyberCore/LayerHostingContextIdentifier.h>
#include <CyberCore/LayoutMilestone.h>
#include <CyberCore/MediaProducer.h>
#include <CyberCore/PageIdentifier.h>
#include <CyberCore/Pagination.h>
#include <CyberCore/ScrollTypes.h>
#include <CyberCore/ShouldRelaxThirdPartyCookieBlocking.h>
#include <CyberCore/UserInterfaceLayoutDirection.h>
#include <CyberCore/ViewportArguments.h>
#include <wtf/RobinHoodHashSet.h>
#include <wtf/text/WTFString.h>

#if ENABLE(APPLICATION_MANIFEST)
#include <CyberCore/ApplicationManifest.h>
#endif

#if ENABLE(NETWORK_CONNECTION_INTEGRITY)
#include <CyberCore/LookalikeCharactersSanitizationData.h>
#endif

#if ENABLE(WK_WEB_EXTENSIONS)
#include "WebExtensionControllerParameters.h"
#endif

namespace IPC {
class Decoder;
class Encoder;
}

namespace CyberKit {

struct WebPageCreationParameters {
    void encode(IPC::Encoder&) const;
    static std::optional<WebPageCreationParameters> decode(IPC::Decoder&);

    CyberCore::IntSize viewSize;

    OptionSet<CyberCore::ActivityState> activityState;
    
    WebPreferencesStore store;
    DrawingAreaType drawingAreaType;
    DrawingAreaIdentifier drawingAreaIdentifier;
    WebPageProxyIdentifier webPageProxyIdentifier;
    WebPageGroupData pageGroupData;

    bool isEditable;

    CyberCore::Color underlayColor;

    bool useFixedLayout;
    CyberCore::IntSize fixedLayoutSize;

    CyberCore::FloatSize defaultUnobscuredSize;
    CyberCore::FloatSize minimumUnobscuredSize;
    CyberCore::FloatSize maximumUnobscuredSize;

    std::optional<CyberCore::FloatRect> viewExposedRect;

    bool alwaysShowsHorizontalScroller;
    bool alwaysShowsVerticalScroller;

    bool suppressScrollbarAnimations;

    CyberCore::Pagination::Mode paginationMode;
    bool paginationBehavesLikeColumns;
    double pageLength;
    double gapBetweenPages;
    
    String userAgent;

    bool itemStatesWereRestoredByAPIRequest { false };
    Vector<BackForwardListItemState> itemStates;

    uint64_t visitedLinkTableID;
    bool canRunBeforeUnloadConfirmPanel;
    bool canRunModal;

    float deviceScaleFactor;
    float viewScaleFactor;

    double textZoomFactor { 1 };
    double pageZoomFactor { 1 };

    float topContentInset;
    
    float mediaVolume;
    CyberCore::MediaProducerMutedStateFlags muted;
    bool openedByDOM { false };
    bool mayStartMediaWhenInWindow;
    bool mediaPlaybackIsSuspended { false };

    CyberCore::IntSize minimumSizeForAutoLayout;
    CyberCore::IntSize sizeToContentAutoSizeMaximumSize;
    bool autoSizingShouldExpandToViewHeight;
    std::optional<CyberCore::FloatSize> viewportSizeForCSSViewportUnits;
    
    CyberCore::ScrollPinningBehavior scrollPinningBehavior;

    // FIXME: This should be std::optional<CyberCore::ScrollbarOverlayStyle>, but we would need to
    // correctly handle enums inside Optionals when encoding and decoding. 
    std::optional<uint32_t> scrollbarOverlayStyle;

    bool backgroundExtendsBeyondPage;

    LayerHostingMode layerHostingMode;

    bool hasResourceLoadClient { false };

    Vector<String> mimeTypesWithCustomContentProviders;

    bool controlledByAutomation;
    bool isProcessSwap { false };

    bool useDarkAppearance { false };
    bool useElevatedUserInterfaceLevel { false };

#if PLATFORM(MAC)
    std::optional<CyberCore::DestinationColorSpace> colorSpace;
    bool useSystemAppearance { false };
    bool useFormSemanticContext { false };
    int headerBannerHeight { 0 };
    int footerBannerHeight { 0 };
#endif
#if ENABLE(META_VIEWPORT)
    bool ignoresViewportScaleLimits;
    CyberCore::FloatSize viewportConfigurationViewLayoutSize;
    double viewportConfigurationLayoutSizeScaleFactor;
    double viewportConfigurationMinimumEffectiveDeviceWidth;
    CyberCore::FloatSize viewportConfigurationViewSize;
    std::optional<CyberCore::ViewportArguments> overrideViewportArguments;
#endif
#if PLATFORM(IOS_FAMILY)
    CyberCore::FloatSize screenSize;
    CyberCore::FloatSize availableScreenSize;
    CyberCore::FloatSize overrideScreenSize;
    float textAutosizingWidth;
    CyberCore::IntDegrees deviceOrientation { 0 };
    bool keyboardIsAttached { false };
    bool canShowWhileLocked { false };
    bool isCapturingScreen { false };
#endif
#if PLATFORM(COCOA)
    bool smartInsertDeleteEnabled;
    Vector<String> additionalSupportedImageTypes;
    Vector<SandboxExtension::Handle> gpuIOKitExtensionHandles;
    Vector<SandboxExtension::Handle> gpuMachExtensionHandles;
#endif
#if HAVE(STATIC_FONT_REGISTRY)
    Vector<SandboxExtension::Handle> fontMachExtensionHandles;
#endif
#if HAVE(APP_ACCENT_COLORS)
    CyberCore::Color accentColor;
#endif
#if USE(WPE_RENDERER)
    UnixFileDescriptor hostFileDescriptor;
#endif
#if USE(GRAPHICS_LAYER_TEXTURE_MAPPER) || USE(GRAPHICS_LAYER_WC)
    uint64_t nativeWindowHandle;
#endif
#if USE(GRAPHICS_LAYER_WC)
    bool usesOffscreenRendering { false };
#endif
    bool shouldScaleViewToFitDocument;

    CyberCore::UserInterfaceLayoutDirection userInterfaceLayoutDirection;
    OptionSet<CyberCore::LayoutMilestone> observedLayoutMilestones;

    String overrideContentSecurityPolicy;
    std::optional<double> cpuLimit;

    HashMap<String, WebURLSchemeHandlerIdentifier> urlSchemeHandlers;
    Vector<String> urlSchemesWithLegacyCustomProtocolHandlers;

#if ENABLE(APPLICATION_MANIFEST)
    std::optional<CyberCore::ApplicationManifest> applicationManifest;
#endif

    bool needsFontAttributes { false };

    // WebRTC members.
    bool iceCandidateFilteringEnabled { true };
    bool enumeratingAllNetworkInterfacesEnabled { false };

    UserContentControllerParameters userContentControllerParameters;

#if ENABLE(WK_WEB_EXTENSIONS)
    std::optional<WebExtensionControllerParameters> webExtensionControllerParameters;
#endif

    std::optional<CyberCore::Color> backgroundColor;

    std::optional<CyberCore::PageIdentifier> oldPageID;

    String overriddenMediaType;
    Vector<String> corsDisablingPatterns;
    HashSet<String> maskedURLSchemes;
    bool userScriptsShouldWaitUntilNotification { true };
    bool loadsSubresources { true };
    std::optional<MemoryCompactLookupOnlyRobinHoodHashSet<String>> allowedNetworkHosts;

    bool crossOriginAccessControlCheckEnabled { true };
    String processDisplayName;

    bool shouldCaptureAudioInUIProcess { false };
    bool shouldCaptureAudioInGPUProcess { false };
    bool shouldCaptureVideoInUIProcess { false };
    bool shouldCaptureVideoInGPUProcess { false };
    bool shouldCaptureDisplayInUIProcess { false };
    bool shouldCaptureDisplayInGPUProcess { false };
    bool shouldRenderCanvasInGPUProcess { false };
    bool shouldRenderDOMInGPUProcess { false };
    bool shouldPlayMediaInGPUProcess { false };
#if ENABLE(WEBGL)
    bool shouldRenderWebGLInGPUProcess { false };
#endif
    bool shouldEnableVP8Decoder { false };
    bool shouldEnableVP9Decoder { false };
    bool shouldEnableVP9SWDecoder { false };
#if ENABLE(APP_BOUND_DOMAINS)
    bool limitsNavigationsToAppBoundDomains { false };
#endif
    bool lastNavigationWasAppInitiated { true };
    bool canUseCredentialStorage { true };

    CyberCore::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking { CyberCore::ShouldRelaxThirdPartyCookieBlocking::No };
    
    bool httpsUpgradeEnabled { true };

#if PLATFORM(IOS)
    bool allowsDeprecatedSynchronousXMLHttpRequestDuringUnload { false };
#endif
    
#if ENABLE(APP_HIGHLIGHTS)
    CyberCore::HighlightVisibility appHighlightsVisible { CyberCore::HighlightVisibility::Hidden };
#endif

#if HAVE(TOUCH_BAR)
    bool requiresUserActionForEditingControlsManager { false };
#endif

    bool hasResizableWindows { false };

    CyberCore::ContentSecurityPolicyModeForExtension contentSecurityPolicyModeForExtension { CyberCore::ContentSecurityPolicyModeForExtension::None };

    struct SubframeProcessFrameTreeInitializationParameters {
        CyberCore::FrameIdentifier localFrameIdentifier;
        FrameTreeCreationParameters treeCreationParameters;
        CyberCore::LayerHostingContextIdentifier layerHostingContextIdentifier;

        void encode(IPC::Encoder&) const;
        static std::optional<SubframeProcessFrameTreeInitializationParameters> decode(IPC::Decoder&);
    };
    std::optional<SubframeProcessFrameTreeInitializationParameters> subframeProcessFrameTreeInitializationParameters;

#if ENABLE(NETWORK_CONNECTION_INTEGRITY)
    Vector<CyberCore::LookalikeCharactersSanitizationData> lookalikeCharacterStrings;
    Vector<CyberCore::LookalikeCharactersSanitizationData> allowedLookalikeCharacterStrings;
#endif

#if HAVE(MACH_BOOTSTRAP_EXTENSION)
    SandboxExtension::Handle machBootstrapHandle;
#endif
};

} // namespace CyberKit
