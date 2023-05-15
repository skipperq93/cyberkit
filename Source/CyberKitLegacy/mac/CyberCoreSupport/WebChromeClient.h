/*
 * Copyright (C) 2006-2023 Apple Inc. All rights reserved.
 * Copyright (C) 2008, 2010 Nokia Corporation and/or its subsidiary(-ies)
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
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
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

#import <CyberCore/ChromeClient.h>
#import <CyberCore/FocusDirection.h>
#import <wtf/Forward.h>

namespace CyberCore {
class HTMLImageElement;
}

@class WebView;

// FIXME: This class is used as a concrete class on Mac, but on iOS this is an abstract
// base class of the concrete class, WebChromeClientIOS. Because of that, this class and
// many of its functions are not marked final. That is messy way to organize things.
class WebChromeClient : public CyberCore::ChromeClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    WebChromeClient(WebView*);

    WebView* webView() const { return m_webView; }

private:
    void chromeDestroyed() final;

    void setWindowRect(const CyberCore::FloatRect&) override;
    CyberCore::FloatRect windowRect() const override;

    CyberCore::FloatRect pageRect() const final;

    void focus() override;
    void unfocus() final;

    bool canTakeFocus(CyberCore::FocusDirection) const final;
    void takeFocus(CyberCore::FocusDirection) override;

    void focusedElementChanged(CyberCore::Element*) override;
    void focusedFrameChanged(CyberCore::LocalFrame*) final;

    CyberCore::Page* createWindow(CyberCore::LocalFrame&, const CyberCore::WindowFeatures&, const CyberCore::NavigationAction&) final;
    void show() final;

    bool canRunModal() const final;
    void runModal() final;

    void setToolbarsVisible(bool) final;
    bool toolbarsVisible() const final;

    void setStatusbarVisible(bool) final;
    bool statusbarVisible() const final;

    void setScrollbarsVisible(bool) final;
    bool scrollbarsVisible() const final;

    void setMenubarVisible(bool) final;
    bool menubarVisible() const final;

    void setResizable(bool) final;

    void addMessageToConsole(JSC::MessageSource, JSC::MessageLevel, const String& message, unsigned lineNumber, unsigned columnNumber, const String& sourceURL) final;

    bool canRunBeforeUnloadConfirmPanel() final;
    bool runBeforeUnloadConfirmPanel(const String& message, CyberCore::LocalFrame&) final;

    void closeWindow() final;

    void runJavaScriptAlert(CyberCore::LocalFrame&, const String&) override;
    bool runJavaScriptConfirm(CyberCore::LocalFrame&, const String&) override;
    bool runJavaScriptPrompt(CyberCore::LocalFrame&, const String& message, const String& defaultValue, String& result) override;

    void invalidateRootView(const CyberCore::IntRect&) final;
    void invalidateContentsAndRootView(const CyberCore::IntRect&) final;
    void invalidateContentsForSlowScroll(const CyberCore::IntRect&) final;
    void scroll(const CyberCore::IntSize& scrollDelta, const CyberCore::IntRect& rectToScroll, const CyberCore::IntRect& clipRect) final;

    CyberCore::IntPoint screenToRootView(const CyberCore::IntPoint&) const final;
    CyberCore::IntRect rootViewToScreen(const CyberCore::IntRect&) const final;

    CyberCore::IntPoint accessibilityScreenToRootView(const CyberCore::IntPoint&) const final;
    CyberCore::IntRect rootViewToAccessibilityScreen(const CyberCore::IntRect&) const final;

    void didFinishLoadingImageForElement(CyberCore::HTMLImageElement&) final;

    PlatformPageClient platformPageClient() const final;
    void contentsSizeChanged(CyberCore::LocalFrame&, const CyberCore::IntSize&) const final;
    void intrinsicContentsSizeChanged(const CyberCore::IntSize&) const final { }

    void scrollContainingScrollViewsToRevealRect(const CyberCore::IntRect&) const final;
    void setStatusbarText(const String&) override;

    bool shouldUnavailablePluginMessageBeButton(CyberCore::RenderEmbeddedObject::PluginUnavailabilityReason) const final;
    void unavailablePluginButtonClicked(CyberCore::Element&, CyberCore::RenderEmbeddedObject::PluginUnavailabilityReason) const final;
    void mouseDidMoveOverElement(const CyberCore::HitTestResult&, unsigned modifierFlags, const String&, CyberCore::TextDirection) final;

    void setToolTip(const String&);

    void print(CyberCore::LocalFrame&, const CyberCore::StringWithDirection&) final;
    void exceededDatabaseQuota(CyberCore::LocalFrame&, const String& databaseName, CyberCore::DatabaseDetails) final;
    void reachedMaxAppCacheSize(int64_t spaceNeeded) final;
    void reachedApplicationCacheOriginQuota(CyberCore::SecurityOrigin&, int64_t totalSpaceNeeded) final;

    void runOpenPanel(CyberCore::LocalFrame&, CyberCore::FileChooser&) override;
    void showShareSheet(CyberCore::ShareDataWithParsedURL&, CompletionHandler<void(bool)>&&) override;

    void loadIconForFiles(const Vector<String>&, CyberCore::FileIconLoader&) final;
    RefPtr<CyberCore::Icon> createIconForFiles(const Vector<String>& filenames) override;

#if !PLATFORM(IOS_FAMILY)
    void setCursor(const CyberCore::Cursor&) final;
    void setCursorHiddenUntilMouseMoves(bool) final;
#endif

#if ENABLE(INPUT_TYPE_COLOR)
    std::unique_ptr<CyberCore::ColorChooser> createColorChooser(CyberCore::ColorChooserClient&, const CyberCore::Color&) final;
#endif

#if ENABLE(DATALIST_ELEMENT)
    std::unique_ptr<CyberCore::DataListSuggestionPicker> createDataListSuggestionPicker(CyberCore::DataListSuggestionsClient&) final;
    bool canShowDataListSuggestionLabels() const final { return false; }
#endif

#if ENABLE(DATE_AND_TIME_INPUT_TYPES)
    std::unique_ptr<CyberCore::DateTimeChooser> createDateTimeChooser(CyberCore::DateTimeChooserClient&) final;
#endif

#if ENABLE(APP_HIGHLIGHTS)
    void storeAppHighlight(CyberCore::AppHighlight&&) const final;
#endif

    void setTextIndicator(const CyberCore::TextIndicatorData&) const final;

#if ENABLE(POINTER_LOCK)
    bool requestPointerLock() final;
    void requestPointerUnlock() final;
#endif

    CyberCore::KeyboardUIMode keyboardUIMode() final;

    bool hoverSupportedByPrimaryPointingDevice() const override { return true; }
    bool hoverSupportedByAnyAvailablePointingDevice() const override { return true; }
    std::optional<CyberCore::PointerCharacteristics> pointerCharacteristicsOfPrimaryPointingDevice() const override { return CyberCore::PointerCharacteristics::Fine; }
    OptionSet<CyberCore::PointerCharacteristics> pointerCharacteristicsOfAllAvailablePointingDevices() const override { return CyberCore::PointerCharacteristics::Fine; }

    NSResponder *firstResponder() final;
    void makeFirstResponder(NSResponder *) final;

    void enableSuddenTermination() final;
    void disableSuddenTermination() final;

#if !PLATFORM(IOS_FAMILY)
    void elementDidFocus(CyberCore::Element&, const CyberCore::FocusOptions&) override;
    void elementDidBlur(CyberCore::Element&) override;
#endif

    bool shouldPaintEntireContents() const final;

    void attachRootGraphicsLayer(CyberCore::LocalFrame&, CyberCore::GraphicsLayer*) override;
    void attachViewOverlayGraphicsLayer(CyberCore::GraphicsLayer*) final;
    void setNeedsOneShotDrawingSynchronization() final;
    void triggerRenderingUpdate() final;

    CompositingTriggerFlags allowedCompositingTriggers() const final
    {
        return static_cast<CompositingTriggerFlags>(
            ThreeDTransformTrigger |
            VideoTrigger |
            PluginTrigger| 
            CanvasTrigger |
#if PLATFORM(IOS_FAMILY)
            AnimatedOpacityTrigger | // Allow opacity animations to trigger compositing mode for iOS: <rdar://problem/7830677>
#endif
            AnimationTrigger);
    }

#if ENABLE(VIDEO) && PLATFORM(MAC) && ENABLE(VIDEO_PRESENTATION_MODE)
    void setUpPlaybackControlsManager(CyberCore::HTMLMediaElement&) final;
    void clearPlaybackControlsManager() final;
    void playbackControlsMediaEngineChanged() final;
#endif

#if ENABLE(VIDEO)
    bool canEnterVideoFullscreen(CyberCore::HTMLMediaElementEnums::VideoFullscreenMode) const final;
    bool supportsVideoFullscreen(CyberCore::HTMLMediaElementEnums::VideoFullscreenMode) final;
#if ENABLE(VIDEO_PRESENTATION_MODE)
    void setMockVideoPresentationModeEnabled(bool) final;
    void enterVideoFullscreenForVideoElement(CyberCore::HTMLVideoElement&, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode, bool standby) final;
    void exitVideoFullscreenForVideoElement(CyberCore::HTMLVideoElement&, WTF::CompletionHandler<void(bool)>&& = [](bool) { }) final;
    void exitVideoFullscreenToModeWithoutAnimation(CyberCore::HTMLVideoElement&, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode) final;
#endif
#endif

#if ENABLE(FULLSCREEN_API)
    bool supportsFullScreenForElement(const CyberCore::Element&, bool withKeyboard) final;
    void enterFullScreenForElement(CyberCore::Element&) final;
    void exitFullScreenForElement(CyberCore::Element*) final;
#endif

    bool selectItemWritingDirectionIsNatural() override;
    bool selectItemAlignmentFollowsMenuWritingDirection() override;
    RefPtr<CyberCore::PopupMenu> createPopupMenu(CyberCore::PopupMenuClient&) const override;
    RefPtr<CyberCore::SearchPopupMenu> createSearchPopupMenu(CyberCore::PopupMenuClient&) const override;

    void wheelEventHandlersChanged(bool) final { }

#if ENABLE(WEB_CRYPTO)
    bool wrapCryptoKey(const Vector<uint8_t>&, Vector<uint8_t>&) const final;
    bool unwrapCryptoKey(const Vector<uint8_t>&, Vector<uint8_t>&) const final;
#endif

#if ENABLE(SERVICE_CONTROLS)
    void handleSelectionServiceClick(CyberCore::FrameSelection&, const Vector<String>& telephoneNumbers, const CyberCore::IntPoint&) final;
    bool hasRelevantSelectionServices(bool isTextOnly) const final;
#endif

#if ENABLE(WIRELESS_PLAYBACK_TARGET) && !PLATFORM(IOS_FAMILY)
    void addPlaybackTargetPickerClient(CyberCore::PlaybackTargetClientContextIdentifier) final;
    void removePlaybackTargetPickerClient(CyberCore::PlaybackTargetClientContextIdentifier) final;
    void showPlaybackTargetPicker(CyberCore::PlaybackTargetClientContextIdentifier, const CyberCore::IntPoint&, bool /* hasVideo */) final;
    void playbackTargetPickerClientStateDidChange(CyberCore::PlaybackTargetClientContextIdentifier, CyberCore::MediaProducerMediaStateFlags) final;
    void setMockMediaPlaybackTargetPickerEnabled(bool) final;
    void setMockMediaPlaybackTargetPickerState(const String&, CyberCore::MediaPlaybackTargetContext::MockState) final;
    void mockMediaPlaybackTargetPickerDismissPopup() override;
#endif

#if PLATFORM(MAC)
    void changeUniversalAccessZoomFocus(const CyberCore::IntRect&, const CyberCore::IntRect&) final;
#endif

    RefPtr<PAL::WebGPU::GPU> createGPUForWebGPU() const final;

    RefPtr<CyberCore::ShapeDetection::BarcodeDetector> createBarcodeDetector(const CyberCore::ShapeDetection::BarcodeDetectorOptions&) const final;
    void getBarcodeDetectorSupportedFormats(CompletionHandler<void(Vector<CyberCore::ShapeDetection::BarcodeFormat>&&)>&&) const final;
    RefPtr<CyberCore::ShapeDetection::FaceDetector> createFaceDetector(const CyberCore::ShapeDetection::FaceDetectorOptions&) const final;
    RefPtr<CyberCore::ShapeDetection::TextDetector> createTextDetector() const final;

    void requestCookieConsent(CompletionHandler<void(CyberCore::CookieConsentDecisionResult)>&&) final;

    void classifyModalContainerControls(Vector<String>&&, CompletionHandler<void(Vector<CyberCore::ModalContainerControlType>&&)>&&) final;

    void decidePolicyForModalContainer(OptionSet<CyberCore::ModalContainerControlType>, CompletionHandler<void(CyberCore::ModalContainerDecision)>&&) final;

#if ENABLE(VIDEO_PRESENTATION_MODE)
    bool m_mockVideoPresentationModeEnabled { false };
#endif

    __weak WebView *m_webView;
};
