/*
 * Copyright (C) 2010-2023 Apple Inc. All rights reserved.
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <CyberCore/ChromeClient.h>

namespace CyberCore {
class HTMLImageElement;
class RegistrableDomain;
enum class CookieConsentDecisionResult : uint8_t;
enum class StorageAccessPromptWasShown : bool;
enum class StorageAccessWasGranted : bool;
struct TextRecognitionOptions;
}

namespace CyberKit {

class WebFrame;
class WebPage;

class WebChromeClient final : public CyberCore::ChromeClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    WebChromeClient(WebPage&);
    ~WebChromeClient();

    WebPage& page() const { return m_page; }

private:
    void didInsertMenuElement(CyberCore::HTMLMenuElement&) final;
    void didRemoveMenuElement(CyberCore::HTMLMenuElement&) final;
    void didInsertMenuItemElement(CyberCore::HTMLMenuItemElement&) final;
    void didRemoveMenuItemElement(CyberCore::HTMLMenuItemElement&) final;

    void chromeDestroyed() final;
    
    void setWindowRect(const CyberCore::FloatRect&) final;
    CyberCore::FloatRect windowRect() const final;
    
    CyberCore::FloatRect pageRect() const final;
    
    void focus() final;
    void unfocus() final;
    
    bool canTakeFocus(CyberCore::FocusDirection) const final;
    void takeFocus(CyberCore::FocusDirection) final;

    void focusedElementChanged(CyberCore::Element*) final;
    void focusedFrameChanged(CyberCore::LocalFrame*) final;

    // The Frame pointer provides the ChromeClient with context about which
    // Frame wants to create the new Page.  Also, the newly created window
    // should not be shown to the user until the ChromeClient of the newly
    // created Page has its show method called.
    CyberCore::Page* createWindow(CyberCore::LocalFrame&, const CyberCore::WindowFeatures&, const CyberCore::NavigationAction&) final;
    void show() final;
    
    bool canRunModal() const final;
    void runModal() final;

    void reportProcessCPUTime(Seconds, CyberCore::ActivityStateForCPUSampling) final;
    
    void setToolbarsVisible(bool) final;
    bool toolbarsVisible() const final;
    
    void setStatusbarVisible(bool) final;
    bool statusbarVisible() const final;
    
    void setScrollbarsVisible(bool) final;
    bool scrollbarsVisible() const final;
    
    void setMenubarVisible(bool) final;
    bool menubarVisible() const final;
    
    void setResizable(bool) final;
    
    void addMessageToConsole(JSC::MessageSource, JSC::MessageLevel, const String& message, unsigned lineNumber, unsigned columnNumber, const String& sourceID) final;
    void addMessageWithArgumentsToConsole(JSC::MessageSource, JSC::MessageLevel, const String& message, Span<const String> messageArguments, unsigned lineNumber, unsigned columnNumber, const String& sourceID) final;
    
    bool canRunBeforeUnloadConfirmPanel() final;
    bool runBeforeUnloadConfirmPanel(const String& message, CyberCore::LocalFrame&) final;
    
    void closeWindow() final;
    
    void runJavaScriptAlert(CyberCore::LocalFrame&, const String&) final;
    bool runJavaScriptConfirm(CyberCore::LocalFrame&, const String&) final;
    bool runJavaScriptPrompt(CyberCore::LocalFrame&, const String& message, const String& defaultValue, String& result) final;
    void setStatusbarText(const String&) final;

    CyberCore::KeyboardUIMode keyboardUIMode() final;

    bool hoverSupportedByPrimaryPointingDevice() const final;
    bool hoverSupportedByAnyAvailablePointingDevice() const final;
    std::optional<CyberCore::PointerCharacteristics> pointerCharacteristicsOfPrimaryPointingDevice() const final;
    OptionSet<CyberCore::PointerCharacteristics> pointerCharacteristicsOfAllAvailablePointingDevices() const final;

    // HostWindow member function finals.
    void invalidateRootView(const CyberCore::IntRect&) final;
    void invalidateContentsAndRootView(const CyberCore::IntRect&) final;
    void invalidateContentsForSlowScroll(const CyberCore::IntRect&) final;
    void scroll(const CyberCore::IntSize& scrollDelta, const CyberCore::IntRect& scrollRect, const CyberCore::IntRect& clipRect) final;

    CyberCore::IntPoint screenToRootView(const CyberCore::IntPoint&) const final;
    CyberCore::IntRect rootViewToScreen(const CyberCore::IntRect&) const final;

    CyberCore::IntPoint accessibilityScreenToRootView(const CyberCore::IntPoint&) const final;
    CyberCore::IntRect rootViewToAccessibilityScreen(const CyberCore::IntRect&) const final;

    void didFinishLoadingImageForElement(CyberCore::HTMLImageElement&) final;

    PlatformPageClient platformPageClient() const final;
    void contentsSizeChanged(CyberCore::LocalFrame&, const CyberCore::IntSize&) const final;
    void intrinsicContentsSizeChanged(const CyberCore::IntSize&) const final;

    void scrollContainingScrollViewsToRevealRect(const CyberCore::IntRect&) const final; // Currently only Mac has a non empty implementation.
    void scrollMainFrameToRevealRect(const CyberCore::IntRect&) const final;

    bool shouldUnavailablePluginMessageBeButton(CyberCore::RenderEmbeddedObject::PluginUnavailabilityReason) const final;
    void unavailablePluginButtonClicked(CyberCore::Element&, CyberCore::RenderEmbeddedObject::PluginUnavailabilityReason) const final;

    void mouseDidMoveOverElement(const CyberCore::HitTestResult&, unsigned modifierFlags, const String& toolTip, CyberCore::TextDirection) final;

    void print(CyberCore::LocalFrame&, const CyberCore::StringWithDirection&) final;

    void exceededDatabaseQuota(CyberCore::LocalFrame&, const String& databaseName, CyberCore::DatabaseDetails) final { }

    void reachedMaxAppCacheSize(int64_t spaceNeeded) final;
    void reachedApplicationCacheOriginQuota(CyberCore::SecurityOrigin&, int64_t spaceNeeded) final;
    
#if ENABLE(INPUT_TYPE_COLOR)
    std::unique_ptr<CyberCore::ColorChooser> createColorChooser(CyberCore::ColorChooserClient&, const CyberCore::Color&) final;
#endif

#if ENABLE(DATALIST_ELEMENT)
    std::unique_ptr<CyberCore::DataListSuggestionPicker> createDataListSuggestionPicker(CyberCore::DataListSuggestionsClient&) final;
    bool canShowDataListSuggestionLabels() const final;
#endif

#if ENABLE(DATE_AND_TIME_INPUT_TYPES)
    std::unique_ptr<CyberCore::DateTimeChooser> createDateTimeChooser(CyberCore::DateTimeChooserClient&) final;
#endif

#if ENABLE(IOS_TOUCH_EVENTS)
    void didPreventDefaultForEvent() final;
#endif

#if PLATFORM(IOS_FAMILY)
    void didReceiveMobileDocType(bool) final;
    void setNeedsScrollNotifications(CyberCore::LocalFrame&, bool) final;
    void didFinishContentChangeObserving(CyberCore::LocalFrame&, WKContentChange) final;
    void notifyRevealedSelectionByScrollingFrame(CyberCore::LocalFrame&) final;
    bool isStopping() final;

    void didLayout(LayoutType = NormalLayout) final;
    void didStartOverflowScroll() final;
    void didEndOverflowScroll() final;
    bool hasStablePageScaleFactor() const final;

    // FIXME: See <rdar://problem/5975559>
    void suppressFormNotifications() final;
    void restoreFormNotifications() final;

    void addOrUpdateScrollingLayer(CyberCore::Node*, PlatformLayer* scrollingLayer, PlatformLayer* contentsLayer, const CyberCore::IntSize& scrollSize, bool allowHorizontalScrollbar, bool allowVerticalScrollbar) final;
    void removeScrollingLayer(CyberCore::Node*, PlatformLayer* scrollingLayer, PlatformLayer* contentsLayer) final;

    void webAppOrientationsUpdated() final;
    void showPlaybackTargetPicker(bool hasVideo, CyberCore::RouteSharingPolicy, const String&) final;

    Seconds eventThrottlingDelay() final;

    bool shouldUseMouseEventForSelection(const CyberCore::PlatformMouseEvent&) final;

    bool showDataDetectorsUIForElement(const CyberCore::Element&, const CyberCore::Event&) final;
#endif

#if ENABLE(ORIENTATION_EVENTS)
    CyberCore::IntDegrees deviceOrientation() const final;
#endif

    void runOpenPanel(CyberCore::LocalFrame&, CyberCore::FileChooser&) final;
    void showShareSheet(CyberCore::ShareDataWithParsedURL&, WTF::CompletionHandler<void(bool)>&&) final;
    void showContactPicker(const CyberCore::ContactsRequestData&, WTF::CompletionHandler<void(std::optional<Vector<CyberCore::ContactInfo>>&&)>&&) final;
    void loadIconForFiles(const Vector<String>&, CyberCore::FileIconLoader&) final;

    void setCursor(const CyberCore::Cursor&) final;
    void setCursorHiddenUntilMouseMoves(bool) final;
#if !HAVE(NSCURSOR) && !PLATFORM(GTK)
    bool supportsSettingCursor() final { return false; }
#endif

#if ENABLE(POINTER_LOCK)
    bool requestPointerLock() final;
    void requestPointerUnlock() final;
#endif

    void didAssociateFormControls(const Vector<RefPtr<CyberCore::Element>>&, CyberCore::LocalFrame&) final;
    bool shouldNotifyOnFormChanges() final;

    bool selectItemWritingDirectionIsNatural() final;
    bool selectItemAlignmentFollowsMenuWritingDirection() final;
    RefPtr<CyberCore::PopupMenu> createPopupMenu(CyberCore::PopupMenuClient&) const final;
    RefPtr<CyberCore::SearchPopupMenu> createSearchPopupMenu(CyberCore::PopupMenuClient&) const final;

    CyberCore::GraphicsLayerFactory* graphicsLayerFactory() const final;
    void attachRootGraphicsLayer(CyberCore::LocalFrame&, CyberCore::GraphicsLayer*) final;
    void attachViewOverlayGraphicsLayer(CyberCore::GraphicsLayer*) final;
    void setNeedsOneShotDrawingSynchronization() final;
    bool shouldTriggerRenderingUpdate(unsigned rescheduledRenderingUpdateCount) const final;
    void triggerRenderingUpdate() final;
    unsigned remoteImagesCountForTesting() const final; 

    void contentRuleListNotification(const URL&, const CyberCore::ContentRuleListResults&) final;

    bool testProcessIncomingSyncMessagesWhenWaitingForSyncReply() final;

#if PLATFORM(WIN)
    void setLastSetCursorToCurrentCursor() final { }
    void AXStartFrameLoad() final { }
    void AXFinishFrameLoad() final { }
#endif

    void animationDidFinishForElement(const CyberCore::Element&) final;

    CyberCore::DisplayRefreshMonitorFactory* displayRefreshMonitorFactory() const final;

#if ENABLE(GPU_PROCESS)
    RefPtr<CyberCore::ImageBuffer> createImageBuffer(const CyberCore::FloatSize&, CyberCore::RenderingMode, CyberCore::RenderingPurpose, float resolutionScale, const CyberCore::DestinationColorSpace&, CyberCore::PixelFormat, bool avoidBackendSizeCheck = false) const final;
    RefPtr<CyberCore::ImageBuffer> sinkIntoImageBuffer(std::unique_ptr<CyberCore::SerializedImageBuffer>) final;
#endif
    std::unique_ptr<CyberCore::WorkerClient> createWorkerClient(SerialFunctionDispatcher&) final;

#if ENABLE(WEBGL)
    RefPtr<CyberCore::GraphicsContextGL> createGraphicsContextGL(const CyberCore::GraphicsContextGLAttributes&) const final;
#endif

    RefPtr<PAL::WebGPU::GPU> createGPUForWebGPU() const final;

    RefPtr<CyberCore::ShapeDetection::BarcodeDetector> createBarcodeDetector(const CyberCore::ShapeDetection::BarcodeDetectorOptions&) const final;
    void getBarcodeDetectorSupportedFormats(CompletionHandler<void(Vector<CyberCore::ShapeDetection::BarcodeFormat>&&)>&&) const final;
    RefPtr<CyberCore::ShapeDetection::FaceDetector> createFaceDetector(const CyberCore::ShapeDetection::FaceDetectorOptions&) const final;
    RefPtr<CyberCore::ShapeDetection::TextDetector> createTextDetector() const final;

    CompositingTriggerFlags allowedCompositingTriggers() const final
    {
        return static_cast<CompositingTriggerFlags>(
            ThreeDTransformTrigger |
            VideoTrigger |
            PluginTrigger|
            CanvasTrigger |
#if PLATFORM(COCOA) || USE(NICOSIA)
            ScrollableNonMainFrameTrigger |
#endif
#if PLATFORM(IOS_FAMILY)
            AnimatedOpacityTrigger | // Allow opacity animations to trigger compositing mode for iPhone: <rdar://problem/7830677>
#endif
            AnimationTrigger);
    }

    bool layerTreeStateIsFrozen() const final;

#if ENABLE(ASYNC_SCROLLING)
    RefPtr<CyberCore::ScrollingCoordinator> createScrollingCoordinator(CyberCore::Page&) const final;
#endif

#if PLATFORM(MAC)
    std::unique_ptr<CyberCore::ScrollbarsController> createScrollbarsController(CyberCore::Page&, CyberCore::ScrollableArea&) const final;
#endif

#if ENABLE(VIDEO_PRESENTATION_MODE)
    void prepareForVideoFullscreen() final;
    bool canEnterVideoFullscreen(CyberCore::HTMLMediaElementEnums::VideoFullscreenMode) const final;
    bool supportsVideoFullscreen(CyberCore::HTMLMediaElementEnums::VideoFullscreenMode) final;
    bool supportsVideoFullscreenStandby() final;
    void setMockVideoPresentationModeEnabled(bool) final;
    void enterVideoFullscreenForVideoElement(CyberCore::HTMLVideoElement&, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode, bool standby) final;
    void exitVideoFullscreenForVideoElement(CyberCore::HTMLVideoElement&, WTF::CompletionHandler<void(bool)>&& = [](bool) { }) final;
    void setUpPlaybackControlsManager(CyberCore::HTMLMediaElement&) final;
    void clearPlaybackControlsManager() final;
    void playbackControlsMediaEngineChanged() final;
#endif

#if ENABLE(MEDIA_USAGE)
    void addMediaUsageManagerSession(CyberCore::MediaSessionIdentifier, const String&, const URL&) final;
    void updateMediaUsageManagerSessionState(CyberCore::MediaSessionIdentifier, const CyberCore::MediaUsageInfo&) final;
    void removeMediaUsageManagerSession(CyberCore::MediaSessionIdentifier) final;
#endif

#if ENABLE(VIDEO_PRESENTATION_MODE)
    void exitVideoFullscreenToModeWithoutAnimation(CyberCore::HTMLVideoElement&, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode) final;
#endif

#if ENABLE(FULLSCREEN_API)
    bool supportsFullScreenForElement(const CyberCore::Element&, bool withKeyboard) final;
    void enterFullScreenForElement(CyberCore::Element&) final;
    void exitFullScreenForElement(CyberCore::Element*) final;
#endif

#if PLATFORM(COCOA)
    void elementDidFocus(CyberCore::Element&, const CyberCore::FocusOptions&) final;
    void elementDidBlur(CyberCore::Element&) final;
    void elementDidRefocus(CyberCore::Element&, const CyberCore::FocusOptions&) final;
    void focusedElementDidChangeInputMode(CyberCore::Element&, CyberCore::InputMode) final;

    void makeFirstResponder() final;
    void assistiveTechnologyMakeFirstResponder() final;
#endif

    void enableSuddenTermination() final;
    void disableSuddenTermination() final;

#if PLATFORM(IOS_FAMILY)
    CyberCore::FloatSize screenSize() const final;
    CyberCore::FloatSize availableScreenSize() const final;
    CyberCore::FloatSize overrideScreenSize() const final;
#endif

    void dispatchDisabledAdaptationsDidChange(const OptionSet<CyberCore::DisabledAdaptations>&) const final;
    void dispatchViewportPropertiesDidChange(const CyberCore::ViewportArguments&) const final;

    void notifyScrollerThumbIsVisibleInRect(const CyberCore::IntRect&) final;
    void recommendedScrollbarStyleDidChange(CyberCore::ScrollbarStyle newStyle) final;

    std::optional<CyberCore::ScrollbarOverlayStyle> preferredScrollbarOverlayStyle() final;

    CyberCore::Color underlayColor() const final;

    void themeColorChanged() const final;
    void pageExtendedBackgroundColorDidChange() const final;
    void sampledPageTopColorChanged() const final;
    
#if ENABLE(APP_HIGHLIGHTS)
    CyberCore::HighlightVisibility appHighlightsVisiblility() const final;
#endif
    
    void wheelEventHandlersChanged(bool) final;

    String plugInStartLabelTitle(const String& mimeType) const final;
    String plugInStartLabelSubtitle(const String& mimeType) const final;
    String plugInExtraStyleSheet() const final;
    String plugInExtraScript() const final;

    void didAddHeaderLayer(CyberCore::GraphicsLayer&) final;
    void didAddFooterLayer(CyberCore::GraphicsLayer&) final;

    bool shouldUseTiledBackingForFrameView(const CyberCore::LocalFrameView&) const final;

#if ENABLE(ACCESSIBILITY_ANIMATION_CONTROL)
    void isAnyAnimationAllowedToPlayDidChange(bool /* anyAnimationCanPlay */) final;
#endif
    void isPlayingMediaDidChange(CyberCore::MediaProducerMediaStateFlags) final;
    void handleAutoplayEvent(CyberCore::AutoplayEvent, OptionSet<CyberCore::AutoplayEventFlags>) final;

#if ENABLE(APP_HIGHLIGHTS)
    void storeAppHighlight(CyberCore::AppHighlight&&) const final;
#endif

    void setTextIndicator(const CyberCore::TextIndicatorData&) const final;

#if ENABLE(WEB_CRYPTO)
    bool wrapCryptoKey(const Vector<uint8_t>&, Vector<uint8_t>&) const final;
    bool unwrapCryptoKey(const Vector<uint8_t>&, Vector<uint8_t>&) const final;
#endif

#if ENABLE(TELEPHONE_NUMBER_DETECTION) && PLATFORM(MAC)
    void handleTelephoneNumberClick(const String& number, const CyberCore::IntPoint&, const CyberCore::IntRect&) final;
#endif

#if ENABLE(DATA_DETECTION)
    void handleClickForDataDetectionResult(const CyberCore::DataDetectorElementInfo&, const CyberCore::IntPoint&) final;
#endif

#if ENABLE(SERVICE_CONTROLS)
    void handleSelectionServiceClick(CyberCore::FrameSelection&, const Vector<String>& telephoneNumbers, const CyberCore::IntPoint&) final;
    bool hasRelevantSelectionServices(bool isTextOnly) const final;
    void handleImageServiceClick(const CyberCore::IntPoint&, CyberCore::Image&, CyberCore::HTMLImageElement&) final;
    void handlePDFServiceClick(const CyberCore::IntPoint&, CyberCore::HTMLAttachmentElement&);
#endif

    bool shouldDispatchFakeMouseMoveEvents() const final;

    void handleAutoFillButtonClick(CyberCore::HTMLInputElement&) final;

    void inputElementDidResignStrongPasswordAppearance(CyberCore::HTMLInputElement&) final;

#if ENABLE(WIRELESS_PLAYBACK_TARGET) && !PLATFORM(IOS_FAMILY)
    void addPlaybackTargetPickerClient(CyberCore::PlaybackTargetClientContextIdentifier) final;
    void removePlaybackTargetPickerClient(CyberCore::PlaybackTargetClientContextIdentifier) final;
    void showPlaybackTargetPicker(CyberCore::PlaybackTargetClientContextIdentifier, const CyberCore::IntPoint&, bool) final;
    void playbackTargetPickerClientStateDidChange(CyberCore::PlaybackTargetClientContextIdentifier, CyberCore::MediaProducerMediaStateFlags) final;
    void setMockMediaPlaybackTargetPickerEnabled(bool) final;
    void setMockMediaPlaybackTargetPickerState(const String&, CyberCore::MediaPlaybackTargetContext::MockState) final;
    void mockMediaPlaybackTargetPickerDismissPopup() final;
#endif

    void imageOrMediaDocumentSizeChanged(const CyberCore::IntSize&) final;

    RefPtr<CyberCore::Icon> createIconForFiles(const Vector<String>& filenames) final;

    void didInvalidateDocumentMarkerRects() final;

#if ENABLE(TRACKING_PREVENTION)
    void hasStorageAccess(CyberCore::RegistrableDomain&& subFrameDomain, CyberCore::RegistrableDomain&& topFrameDomain, CyberCore::LocalFrame&, WTF::CompletionHandler<void(bool)>&&) final;
    void requestStorageAccess(CyberCore::RegistrableDomain&& subFrameDomain, CyberCore::RegistrableDomain&& topFrameDomain, CyberCore::LocalFrame&, CyberCore::StorageAccessScope, WTF::CompletionHandler<void(CyberCore::RequestStorageAccessResult)>&&) final;
    bool hasPageLevelStorageAccess(const CyberCore::RegistrableDomain& topLevelDomain, const CyberCore::RegistrableDomain& resourceDomain) const final;
#endif

#if ENABLE(DEVICE_ORIENTATION)
    void shouldAllowDeviceOrientationAndMotionAccess(CyberCore::LocalFrame&, bool mayPrompt, CompletionHandler<void(CyberCore::DeviceOrientationOrMotionPermissionState)>&&) final;
#endif

    void configureLoggingChannel(const String&, WTFLogChannelState, WTFLogLevel) final;

    bool userIsInteracting() const final;
    void setUserIsInteracting(bool) final;

#if ENABLE(WEB_AUTHN)
    void setMockWebAuthenticationConfiguration(const CyberCore::MockWebAuthenticationConfiguration&) final;
#endif

#if PLATFORM(MAC)
    void changeUniversalAccessZoomFocus(const CyberCore::IntRect&, const CyberCore::IntRect&) final;
#endif

#if ENABLE(IMAGE_ANALYSIS)
    void requestTextRecognition(CyberCore::Element&, CyberCore::TextRecognitionOptions&&, CompletionHandler<void(RefPtr<CyberCore::Element>&&)>&& = { }) final;
#endif

    bool needsImageOverlayControllerForSelectionPainting() const final
    {
#if USE(UIKIT_EDITING)
        return false;
#else
        return true;
#endif
    }

#if ENABLE(TEXT_AUTOSIZING)
    void textAutosizingUsesIdempotentModeChanged() final;
#endif

    URL sanitizeLookalikeCharacters(const URL&, CyberCore::LookalikeCharacterSanitizationTrigger) const final;
    URL allowedLookalikeCharacters(const URL&) const final;

#if ENABLE(MEDIA_CONTROLS_CONTEXT_MENUS) && USE(UICONTEXTMENU)
    void showMediaControlsContextMenu(CyberCore::FloatRect&&, Vector<CyberCore::MediaControlsContextMenuItem>&&, CompletionHandler<void(CyberCore::MediaControlsContextMenuItem::ID)>&&) final;
#endif // ENABLE(MEDIA_CONTROLS_CONTEXT_MENUS) && USE(UICONTEXTMENU)

#if ENABLE(WEBXR) && !USE(OPENXR)
    void enumerateImmersiveXRDevices(CompletionHandler<void(const PlatformXR::Instance::DeviceList&)>&&) final;
    void requestPermissionOnXRSessionFeatures(const CyberCore::SecurityOriginData&, PlatformXR::SessionMode, const PlatformXR::Device::FeatureList& /* granted */, const PlatformXR::Device::FeatureList& /* consentRequired */, const PlatformXR::Device::FeatureList& /* consentOptional */, CompletionHandler<void(std::optional<PlatformXR::Device::FeatureList>&&)>&&) final;
#endif

#if ENABLE(APPLE_PAY_AMS_UI)
    void startApplePayAMSUISession(const URL&, const CyberCore::ApplePayAMSUIRequest&, CompletionHandler<void(std::optional<bool>&&)>&&) final;
    void abortApplePayAMSUISession() final;
#endif

    void requestCookieConsent(CompletionHandler<void(CyberCore::CookieConsentDecisionResult)>&&) final;

    void classifyModalContainerControls(Vector<String>&&, CompletionHandler<void(Vector<CyberCore::ModalContainerControlType>&&)>&&) final;

    void decidePolicyForModalContainer(OptionSet<CyberCore::ModalContainerControlType>, CompletionHandler<void(CyberCore::ModalContainerDecision)>&&) final;

    const AtomString& searchStringForModalContainerObserver() const final;
    
    bool isUsingUISideCompositing() const;

    mutable bool m_cachedMainFrameHasHorizontalScrollbar { false };
    mutable bool m_cachedMainFrameHasVerticalScrollbar { false };

    WebPage& m_page;
};

} // namespace CyberKit
