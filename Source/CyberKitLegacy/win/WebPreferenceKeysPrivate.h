/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009, 2011 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Inc. ("Apple" nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// These are private because callers should be using the cover methods. They are in
// a Private (as opposed to Internal) header file because Safari uses some of them
// for managed preferences.
#define CyberKitLogLevelPreferenceKey "CyberKitLogLevel"
#define CyberKitStandardFontPreferenceKey "CyberKitStandardFont"
#define CyberKitFixedFontPreferenceKey "CyberKitFixedFont"
#define CyberKitSerifFontPreferenceKey "CyberKitSerifFont"
#define CyberKitSansSerifFontPreferenceKey "CyberKitSansSerifFont"
#define CyberKitCursiveFontPreferenceKey "CyberKitCursiveFont"
#define CyberKitFantasyFontPreferenceKey "CyberKitFantasyFont"
#define CyberKitPictographFontPreferenceKey "CyberKitPictographFont"
#define CyberKitMinimumFontSizePreferenceKey "CyberKitMinimumFontSize"
#define CyberKitMinimumLogicalFontSizePreferenceKey "CyberKitMinimumLogicalFontSize"
#define CyberKitDefaultFontSizePreferenceKey "CyberKitDefaultFontSize"
#define CyberKitDefaultFixedFontSizePreferenceKey "CyberKitDefaultFixedFontSize"
#define CyberKitDefaultTextEncodingNamePreferenceKey "CyberKitDefaultTextEncodingName"
#define CyberKitUserStyleSheetEnabledPreferenceKey "CyberKitUserStyleSheetEnabledPreferenceKey"
#define CyberKitUserStyleSheetLocationPreferenceKey "CyberKitUserStyleSheetLocationPreferenceKey"
#define CyberKitShouldPrintBackgroundsPreferenceKey "CyberKitShouldPrintBackgroundsPreferenceKey"
#define CyberKitTextAreasAreResizablePreferenceKey "CyberKitTextAreasAreResizable"
#define CyberKitJavaEnabledPreferenceKey "CyberKitJavaEnabled"
#define CyberKitJavaScriptEnabledPreferenceKey "CyberKitJavaScriptEnabled"
#define CyberKitJavaScriptRuntimeFlagsPreferenceKey "CyberKitJavaScriptRuntimeFlags"
#define CyberKitWebSecurityEnabledPreferenceKey "CyberKitWebSecurityEnabled"
#define CyberKitAllowUniversalAccessFromFileURLsPreferenceKey "CyberKitAllowUniversalAccessFromFileURLs"
#define CyberKitAllowFileAccessFromFileURLsPreferenceKey "CyberKitAllowFileAccessFromFileURLs"
#define CyberKitJavaScriptCanOpenWindowsAutomaticallyPreferenceKey "CyberKitJavaScriptCanOpenWindowsAutomatically"
#define CyberKitPluginsEnabledPreferenceKey "CyberKitPluginsEnabled"
#define CyberKitCSSRegionsEnabledPreferenceKey "CyberKitCSSRegionsEnabled"
#define CyberKitDatabasesEnabledPreferenceKey "CyberKitDatabasesEnabled"
#define CyberKitLocalStorageEnabledPreferenceKey "CyberKitLocalStorageEnabled"
#define CyberKitExperimentalNotificationsEnabledPreferenceKey "CyberKitExperimentalNotificationsEnabled"
#define CyberKitAllowAnimatedImagesPreferenceKey "CyberKitAllowAnimatedImagesPreferenceKey"
#define CyberKitAllowAnimatedImageLoopingPreferenceKey "CyberKitAllowAnimatedImageLoopingPreferenceKey"
#define CyberKitDisplayImagesKey "CyberKitDisplayImagesKey"
#define CyberKitLoadSiteIconsKey "CyberKitLoadSiteIconsKey"
#define CyberKitBackForwardCacheExpirationIntervalKey "CyberKitBackForwardCacheExpirationIntervalKey"
#define CyberKitTabToLinksPreferenceKey "CyberKitTabToLinksPreferenceKey"
#define CyberKitPrivateBrowsingEnabledPreferenceKey "CyberKitPrivateBrowsingEnabled"
#define CyberKitIconDatabaseLocationKey "CyberKitIconDatabaseLocation"
#define CyberKitIconDatabaseEnabledPreferenceKey "CyberKitIconDatabaseEnabled"
#define CyberKitUsesPageCachePreferenceKey "CyberKitUsesPageCachePreferenceKey"
#define CyberKitCacheModelPreferenceKey "CyberKitCacheModelPreferenceKey"
#define CyberKitLocalStorageDatabasePathPreferenceKey "CyberKitLocalStorageDatabasePath"
#define CyberKitHyperlinkAuditingEnabledPreferenceKey "CyberKitHyperlinkAuditingEnabled"
#define CyberKitWebAudioEnabledPreferenceKey "CyberKitWebAudioEnabled"
#define CyberKitShouldDisplaySubtitlesPreferenceKey "CyberKitShouldDisplaySubtitles"
#define CyberKitShouldDisplayCaptionsPreferenceKey "CyberKitShouldDisplayCaptions"
#define CyberKitShouldDisplayTextDescriptionsPreferenceKey "CyberKitShouldDisplayTextDescriptions"

// These are private both because callers should be using the cover methods and because the
// cover methods themselves are private.
#define CyberKitRespectStandardStyleKeyEquivalentsPreferenceKey "CyberKitRespectStandardStyleKeyEquivalents"
#define CyberKitShowsURLsInToolTipsPreferenceKey "CyberKitShowsURLsInToolTips"
#define CyberKitShowsToolTipOverTruncatedTextPreferenceKey "CyberKitShowsToolTipOverTruncatedText"
#define CyberKitPDFDisplayModePreferenceKey "CyberKitPDFDisplayMode"
#define CyberKitPDFScaleFactorPreferenceKey "CyberKitPDFScaleFactor"
#define CyberKitEditableLinkBehaviorPreferenceKey "CyberKitEditableLinkBehavior"
#define CyberKitShouldInvertColorsPreferenceKey "CyberKitShouldInvertColors"

// Window display is throttled to 60 frames per second if CyberKitThrottleWindowDisplayPreferenceKey
// is set to YES.  The window display throttle is OFF by default for compatibility with Mac OS X
// 10.4.6.
#define CyberKitThrottleWindowDisplayPreferenceKey "CyberKitThrottleWindowDisplay"

// CoreGraphics deferred updates are disabled if CyberKitEnableCoalescedUpdatesPreferenceKey is set
// to NO, or has no value.  For compatibility with Mac OS X 10.4.6, deferred updates are OFF by
// default.
#define CyberKitEnableDeferredUpdatesPreferenceKey "CyberKitEnableDeferredUpdates"

// From WebHistory.h
#define CyberKitHistoryItemLimitKey "CyberKitHistoryItemLimit" // default: "1000"
#define CyberKitHistoryAgeInDaysLimitKey "CyberKitHistoryAgeInDaysLimit" // default: "7"

// Windows-specific keys
#define CyberKitFontSmoothingTypePreferenceKey "CyberKitFontSmoothingType" // default: FontSmoothingTypeMedium (2)
#define CyberKitFontSmoothingContrastPreferenceKey "CyberKitFontSmoothingContrast" // default: "2"
#define CyberKitCookieStorageAcceptPolicyPreferenceKey "CyberKitCookieStorageAcceptPolicy" // default: CyberKitCookieStorageAcceptPolicyOnlyFromMainDocumentDomain

#define WebContinuousSpellCheckingEnabledPreferenceKey "WebContinuousSpellCheckingEnabled" // default: false
#define WebGrammarCheckingEnabledPreferenceKey "WebGrammarCheckingEnabled" // default: false

#define AllowContinuousSpellCheckingPreferenceKey "AllowContinuousSpellCheckingPreferenceKey" // default: true

#define CyberKitDOMPasteAllowedPreferenceKey "CyberKitDOMPasteAllowedPreferenceKey" // default: false

#define CyberKitApplicationChromeModePreferenceKey "CyberKitApplicationChromeMode" // default: false

#define CyberKitOfflineWebApplicationCacheEnabledPreferenceKey "CyberKitOfflineWebApplicationCacheEnabled" // default: false

// If this key is present and has a value of true, we have already removed the default values from the user's preferences <rdar://problem/5214504>
#define CyberKitDidMigrateDefaultSettingsFromSafari3BetaPreferenceKey "CyberKitDidMigrateDefaultSettingsFromSafari3BetaPreferenceKey"

#define CyberKitDidMigrateCyberKitPreferencesToCFPreferencesPreferenceKey "CyberKitDidMigrateCyberKitPreferencesToCFPreferences"

#define CyberKitDeveloperExtrasEnabledPreferenceKey "CyberKitDeveloperExtras"
#define DisableCyberKitDeveloperExtrasPreferenceKey "DisableCyberKitDeveloperExtras"

#define CyberKitAuthorAndUserStylesEnabledPreferenceKey "CyberKitAuthorAndUserStylesEnabled"

#define CyberKitPaintNativeControlsPreferenceKey "CyberKitPaintNativeControls"

#define CyberKitZoomsTextOnlyPreferenceKey "CyberKitZoomsTextOnly"

#define CyberKitJavaScriptCanAccessClipboardPreferenceKey "CyberKitJavaScriptCanAccessClipboard"

#define CyberKitXSSAuditorEnabledPreferenceKey "CyberKitXSSAuditorEnabled"

#define CyberKitUseHighResolutionTimersPreferenceKey "CyberKitUseHighResolutionTimers"

#define CyberKitFrameFlatteningEnabledPreferenceKey "CyberKitFrameFlatteningEnabled"

#define CyberKitAcceleratedCompositingEnabledPreferenceKey "CyberKitAcceleratedCompositingEnabled"

#define CyberKitShowDebugBordersPreferenceKey "CyberKitShowDebugBorders"

#define CyberKitShowRepaintCounterPreferenceKey "CyberKitShowRepaintCounter"

#define CyberKitCustomDragCursorsEnabledPreferenceKey "CyberKitCustomDragCursorsEnabled"

#define CyberKitSpatialNavigationEnabledPreferenceKey "CyberKitSpatialNavigationEnabled"

#define CyberKitDNSPrefetchingEnabledPreferenceKey "CyberKitDNSPrefetchingEnabled"

#define CyberKitFullScreenEnabledPreferenceKey "CyberKitFullScreenEnabled"

#define CyberKitHixie76WebSocketProtocolEnabledPreferenceKey "CyberKitHixie76WebSocketProtocolEnabled"

#define CyberKitMediaPlaybackRequiresUserGesturePreferenceKey "CyberKitMediaPlaybackRequiresUserGesture"

#define CyberKitMediaPlaybackAllowsInlinePreferenceKey "CyberKitMediaPlaybackAllowsInline"

#define CyberKitAVFoundationEnabledPreferenceKey "CyberKitAVFoundationEnabled"

#define CyberKitRequestAnimationFrameEnabledPreferenceKey "CyberKitRequestAnimationFrameEnabled"

#define CyberKitMockScrollbarsEnabledPreferenceKey "CyberKitMockScrollbarsEnabled"

#define CyberKitEnableInheritURIQueryComponentPreferenceKey "CyberKitEnableInheritURIQueryComponent"

#define CyberKitAllowDisplayAndRunningOfInsecureContentPreferenceKey "CyberKitAllowDisplayAndRunningOfInsecureContent"

#define CyberKitShowTiledScrollingIndicatorPreferenceKey "CyberKitShowTiledScrollingIndicator"

#define CyberKitFetchAPIEnabledPreferenceKey "CyberKitFetchAPIEnabled"

#define CyberKitFetchAPIKeepAliveEnabledPreferenceKey "CyberKitFetchAPIKeepAlivEnabled"

#define CyberKitShadowDOMEnabledPreferenceKey "CyberKitShadowDOMEnabled"

#define CyberKitCustomElementsEnabledPreferenceKey "CyberKitCustomElementsEnabled"

#define CyberKitMenuItemElementEnabledPreferenceKey "CyberKitMenuItemElementEnabled"

#define CyberKitKeygenElementEnabledPreferenceKey "CyberKitKeygenElementEnabled"

#define CyberKitModernMediaControlsEnabledPreferenceKey "CyberKitModernMediaControlsEnabled"

#define CyberKitWebAnimationsEnabledPreferenceKey "CyberKitWebAnimationsEnabled"

#define CyberKitWebAnimationsCompositeOperationsEnabledPreferenceKey "CyberKitWebAnimationsCompositeOperationsEnabled"

#define CyberKitWebAnimationsMutableTimelinesEnabledPreferenceKey "CyberKitWebAnimationsMutableTimelinesEnabled"

#define CyberKitWebAnimationsCSSIntegrationEnabledPreferenceKey "CyberKitWebAnimationsCSSIntegrationEnabled"

#define CyberKitUserTimingEnabledPreferenceKey "CyberKitUserTimingEnabled"

#define CyberKitResourceTimingEnabledPreferenceKey "CyberKitResourceTimingEnabled"

#define CyberKitLinkPreloadEnabledPreferenceKey "CyberKitLinkPreloadEnabled"

#define CyberKitMediaPreloadingEnabledPreferenceKey "CyberKitMediaPreloadingEnabled"

#define CyberKitIsSecureContextAttributeEnabledPreferenceKey "CyberKitIsSecureContextAttributeEnabled"

#define CyberKitDataTransferItemsEnabledPreferenceKey "CyberKitDataTransferItemsEnabled"

#define CyberKitInspectorAdditionsEnabledPreferenceKey "CyberKitInspectorAdditionsEnabled"

#define CyberKitVisualViewportAPIEnabledPreferenceKey "CyberKitVisualViewportAPIEnabled"

#define CyberKitServerTimingEnabledPreferenceKey "CyberKitServerTimingEnabled"

#define CyberKitCSSOMViewScrollingAPIEnabledPreferenceKey "CyberKitCSSOMViewScrollingAPIEnabled"

#define CyberKitResizeObserverEnabledPreferenceKey "CyberKitResizeObserverEnabled"

#define CyberKitCoreMathMLEnabledPreferenceKey "CyberKitCoreMathMLEnabled"

#define CyberKitRequestIdleCallbackEnabledPreferenceKey "CyberKitRequestIdleCallbackEnabled"

#define CyberKitAsyncClipboardAPIEnabledPreferenceKey "CyberKitAsyncClipboardAPIEnabled"

#define CyberKitRenderingUpdateThrottlingEnabledPreferenceKey "CyberKitRenderingUpdateThrottlingEnabled"
