/*
 * Copyright (C) 2006-2017 Apple Inc. All rights reserved.
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
    CyberCore::FloatRect windowRect() override;

    CyberCore::FloatRect pageRect() final;

    void focus() override;
    void unfocus() final;

    bool canTakeFocus(CyberCore::FocusDirection) final;
    void takeFocus(CyberCore::FocusDirection) override;

    void focusedElementChanged(CyberCore::Element*) override;
    void focusedFrameChanged(CyberCore::Frame*) final;

    CyberCore::Page* createWindow(CyberCore::Frame&, const CyberCore::FrameLoadRequest&, const CyberCore::WindowFeatures&, const CyberCore::NavigationAction&) final;
    void show() final;

    bool canRunModal() final;
    void runModal() final;

    void setToolbarsVisible(bool) final;
    bool toolbarsVisible() final;

    void setStatusbarVisible(bool) final;
    bool statusbarVisible() final;

    void setScrollbarsVisible(bool) final;
    bool scrollbarsVisible() final;

    void setMenubarVisible(bool) final;
    bool menubarVisible() final;

    void setResizable(bool) final;

    void addMessageToConsole(JSC::MessageSource, JSC::MessageLevel, const String& message, unsigned lineNumber, unsigned columnNumber, const String& sourceURL) final;

    bool canRunBeforeUnloadConfirmPanel() final;
    bool runBeforeUnloadConfirmPanel(const String& message, CyberCore::Frame&) final;

    void closeWindowSoon() final;

    void runJavaScriptAlert(CyberCore::Frame&, const String&) override;
    bool runJavaScriptConfirm(CyberCore::Frame&, const String&) override;
    bool runJavaScriptPrompt(CyberCore::Frame&, const String& message, const String& defaultValue, String& result) override;

    bool supportsImmediateInvalidation() final;
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
    void contentsSizeChanged(CyberCore::Frame&, const CyberCore::IntSize&) const final;
    void intrinsicContentsSizeChanged(const CyberCore::IntSize&) const final { }
    void scrollRectIntoView(const CyberCore::IntRect&) const final;

    void setStatusbarText(const String&) override;

    bool shouldUnavailablePluginMessageBeButton(CyberCore::RenderEmbeddedObject::PluginUnavailabilityReason) const final;
    void unavailablePluginButtonClicked(CyberCore::Element&, CyberCore::RenderEmbeddedObject::PluginUnavailabilityReason) const final;
    void mouseDidMoveOverElement(const CyberCore::HitTestResult&, unsigned modifierFlags, const String&, CyberCore::TextDirection) final;

    void setToolTip(const String&);

    void print(CyberCore::Frame&) final;
    void exceededDatabaseQuota(CyberCore::Frame&, const String& databaseName, CyberCore::DatabaseDetails) final;
    void reachedMaxAppCacheSize(int64_t spaceNeeded) final;
    void reachedApplicationCacheOriginQuota(CyberCore::SecurityOrigin&, int64_t totalSpaceNeeded) final;

    void runOpenPanel(CyberCore::Frame&, CyberCore::FileChooser&) override;
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
#endif

#if ENABLE(POINTER_LOCK)
    bool requestPointerLock() final;
    void requestPointerUnlock() final;
#endif

    CyberCore::KeyboardUIMode keyboardUIMode() final;

    NSResponder *firstResponder() final;
    void makeFirstResponder(NSResponder *) final;

    void enableSuddenTermination() final;
    void disableSuddenTermination() final;

#if !PLATFORM(IOS_FAMILY)
    void elementDidFocus(CyberCore::Element&) override;
    void elementDidBlur(CyberCore::Element&) override;
#endif

    bool shouldPaintEntireContents() const final;

    void attachRootGraphicsLayer(CyberCore::Frame&, CyberCore::GraphicsLayer*) override;
    void attachViewOverlayGraphicsLayer(CyberCore::GraphicsLayer*) final;
    void setNeedsOneShotDrawingSynchronization() final;
    void scheduleCompositingLayerFlush() final;
    bool needsImmediateRenderingUpdate() const final { return true; }

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
#endif

#if ENABLE(VIDEO)
    bool supportsVideoFullscreen(CyberCore::HTMLMediaElementEnums::VideoFullscreenMode) final;
    void enterVideoFullscreenForVideoElement(CyberCore::HTMLVideoElement&, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode, bool standby) final;
    void exitVideoFullscreenForVideoElement(CyberCore::HTMLVideoElement&) final;
    void exitVideoFullscreenToModeWithoutAnimation(CyberCore::HTMLVideoElement&, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode) final;
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
    void addPlaybackTargetPickerClient(uint64_t /*contextId*/) final;
    void removePlaybackTargetPickerClient(uint64_t /*contextId*/) final;
    void showPlaybackTargetPicker(uint64_t /*contextId*/, const CyberCore::IntPoint&, bool /* hasVideo */) final;
    void playbackTargetPickerClientStateDidChange(uint64_t /*contextId*/, CyberCore::MediaProducer::MediaStateFlags) final;
    void setMockMediaPlaybackTargetPickerEnabled(bool) final;
    void setMockMediaPlaybackTargetPickerState(const String&, CyberCore::MediaPlaybackTargetContext::State) final;
    void mockMediaPlaybackTargetPickerDismissPopup() override;
#endif

    String signedPublicKeyAndChallengeString(unsigned keySizeIndex, const String& challengeString, const URL&) const final;

    WebView *m_webView;
};
