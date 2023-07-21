/*
 * Copyright (C) 2010-2018 Apple Inc. All rights reserved.
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
#include "LayerTreeContext.h"
#include "SessionState.h"
#include "WebCompiledContentRuleListData.h"
#include "CyberCoreArgumentCoders.h"
#include "WebPageGroupData.h"
#include "WebPreferencesStore.h"
#include "WebUserContentControllerDataTypes.h"
#include <CyberCore/ActivityState.h>
#include <CyberCore/Color.h>
#include <CyberCore/FloatSize.h>
#include <CyberCore/IntSize.h>
#include <CyberCore/LayoutMilestone.h>
#include <CyberCore/MediaProducer.h>
#include <CyberCore/PageIdentifier.h>
#include <CyberCore/Pagination.h>
#include <CyberCore/ScrollTypes.h>
#include <CyberCore/UserInterfaceLayoutDirection.h>
#include <CyberCore/ViewportArguments.h>
#include <pal/SessionID.h>
#include <wtf/HashMap.h>
#include <wtf/text/WTFString.h>

#if PLATFORM(MAC)
#include "ColorSpaceData.h"
#endif

#if ENABLE(APPLICATION_MANIFEST)
#include <CyberCore/ApplicationManifest.h>
#endif

namespace IPC {
class Decoder;
class Encoder;
}

namespace CyberKit {

struct WebPageCreationParameters {
    void encode(IPC::Encoder&) const;
    static Optional<WebPageCreationParameters> decode(IPC::Decoder&);

    CyberCore::IntSize viewSize;

    OptionSet<CyberCore::ActivityState::Flag> activityState;
    
    WebPreferencesStore store;
    DrawingAreaType drawingAreaType;
    DrawingAreaIdentifier drawingAreaIdentifier;
    WebPageGroupData pageGroupData;

    bool isEditable;

    CyberCore::Color underlayColor;

    bool useFixedLayout;
    CyberCore::IntSize fixedLayoutSize;

    bool alwaysShowsHorizontalScroller;
    bool alwaysShowsVerticalScroller;

    bool suppressScrollbarAnimations;

    CyberCore::Pagination::Mode paginationMode;
    bool paginationBehavesLikeColumns;
    double pageLength;
    double gapBetweenPages;
    bool paginationLineGridEnabled;
    
    String userAgent;

    Vector<BackForwardListItemState> itemStates;
    PAL::SessionID sessionID;

    UserContentControllerIdentifier userContentControllerID;
    uint64_t visitedLinkTableID;
    uint64_t websiteDataStoreID;
    bool canRunBeforeUnloadConfirmPanel;
    bool canRunModal;

    float deviceScaleFactor;
    float viewScaleFactor;

    double textZoomFactor { 1 };
    double pageZoomFactor { 1 };

    float topContentInset;
    
    float mediaVolume;
    CyberCore::MediaProducer::MutedStateFlags muted;
    bool mayStartMediaWhenInWindow;
    bool mediaPlaybackIsSuspended { false };

    CyberCore::IntSize viewLayoutSize;
    bool autoSizingShouldExpandToViewHeight;
    Optional<CyberCore::IntSize> viewportSizeForCSSViewportUnits;
    
    CyberCore::ScrollPinningBehavior scrollPinningBehavior;

    // FIXME: This should be Optional<CyberCore::ScrollbarOverlayStyle>, but we would need to
    // correctly handle enums inside Optionals when encoding and decoding. 
    Optional<uint32_t> scrollbarOverlayStyle;

    bool backgroundExtendsBeyondPage;

    LayerHostingMode layerHostingMode;

    Vector<String> mimeTypesWithCustomContentProviders;

    bool controlledByAutomation;
    bool isProcessSwap { false };

    bool useDarkAppearance { false };
    bool useElevatedUserInterfaceLevel { false };

#if PLATFORM(MAC)
    ColorSpaceData colorSpace;
    bool useSystemAppearance;
#endif
#if PLATFORM(IOS_FAMILY)
    CyberCore::FloatSize screenSize;
    CyberCore::FloatSize availableScreenSize;
    CyberCore::FloatSize overrideScreenSize;
    float textAutosizingWidth;
    bool ignoresViewportScaleLimits;
    CyberCore::FloatSize viewportConfigurationViewLayoutSize;
    double viewportConfigurationLayoutSizeScaleFactor;
    double viewportConfigurationMinimumEffectiveDeviceWidth;
    CyberCore::FloatSize viewportConfigurationViewSize;
    CyberCore::FloatSize maximumUnobscuredSize;
    int32_t deviceOrientation { 0 };
    bool keyboardIsAttached { false };
    bool canShowWhileLocked { false };
    Optional<CyberCore::ViewportArguments> overrideViewportArguments;
#endif
#if PLATFORM(COCOA)
    bool smartInsertDeleteEnabled;
    Vector<String> additionalSupportedImageTypes;
#endif
#if USE(WPE_RENDERER)
    IPC::Attachment hostFileDescriptor;
#endif
    bool appleMailPaginationQuirkEnabled;
    bool appleMailLinesClampEnabled;
    bool shouldScaleViewToFitDocument;

    CyberCore::UserInterfaceLayoutDirection userInterfaceLayoutDirection;
    OptionSet<CyberCore::LayoutMilestone> observedLayoutMilestones;

    String overrideContentSecurityPolicy;
    Optional<double> cpuLimit;

    HashMap<String, uint64_t> urlSchemeHandlers;

#if ENABLE(APPLICATION_MANIFEST)
    Optional<CyberCore::ApplicationManifest> applicationManifest;
#endif

#if ENABLE(SERVICE_WORKER)
    bool hasRegisteredServiceWorkers { true };
#endif

    bool needsFontAttributes { false };

    // WebRTC members.
    bool iceCandidateFilteringEnabled { true };
    bool enumeratingAllNetworkInterfacesEnabled { false };

    // UserContentController members
    Vector<std::pair<uint64_t, String>> userContentWorlds;
    Vector<WebUserScriptData> userScripts;
    Vector<WebUserStyleSheetData> userStyleSheets;
    Vector<WebScriptMessageHandlerData> messageHandlers;
#if ENABLE(CONTENT_EXTENSIONS)
    Vector<std::pair<String, WebCompiledContentRuleListData>> contentRuleLists;
#endif

    Optional<CyberCore::Color> backgroundColor;

    Optional<CyberCore::PageIdentifier> oldPageID;
};

} // namespace CyberKit
