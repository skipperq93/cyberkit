/*
 * Copyright (C) 2010-2020 Apple Inc. All rights reserved.
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
enum class StorageAccessPromptWasShown : bool;
enum class StorageAccessWasGranted : bool;
}

namespace CyberKit {

class RemoteRenderingBackend;
class WebFrame;
class WebPage;

class WebChromeClient final : public CyberCore::ChromeClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    WebChromeClient(WebPage&);

    WebPage& page() const { return m_page; }

private:
    ~WebChromeClient();

    void didInsertMenuElement(CyberCore::HTMLMenuElement&) final;
    void didRemoveMenuElement(CyberCore::HTMLMenuElement&) final;
    void didInsertMenuItemElement(CyberCore::HTMLMenuItemElement&) final;
    void didRemoveMenuItemElement(CyberCore::HTMLMenuItemElement&) final;

    void chromeDestroyed() final;
    
    void setWindowRect(const CyberCore::FloatRect&) final;
    CyberCore::FloatRect windowRect() final;
    
    CyberCore::FloatRect pageRect() final;
    
    void focus() final;
    void unfocus() final;
    
    bool canTakeFocus(CyberCore::FocusDirection) final;
    void takeFocus(CyberCore::FocusDirection) final;

    void focusedElementChanged(CyberCore::Element*) final;
    void focusedFrameChanged(CyberCore::Frame*) final;

    // The Frame pointer provides the ChromeClient with context about which
    // Frame wants to create the new Page.  Also, the newly created window
    // should not be shown to the user until the ChromeClient of the newly
    // created Page has its show method called.
    CyberCore::Page* createWindow(CyberCore::Frame&, const CyberCore::WindowFeatures&, const CyberCore::NavigationAction&) final;
    void show() final;
    
    bool canRunModal() final;
    void runModal() final;

    void reportProcessCPUTime(Seconds, CyberCore::ActivityStateForCPUSampling) final;
    
    void setToolbarsVisible(bool) final;
    bool toolbarsVisible() final;
    
    void setStatusbarVisible(bool) final;
    bool statusbarVisible() final;
    
    void setScrollbarsVisible(bool) final;
    bool scrollbarsVisible() final;
    
    void setMenubarVisible(bool) final;
    bool menubarVisible() final;
    
    void setResizable(bool) final;
    
    void addMessageToConsole(JSC::MessageSource, JSC::MessageLevel, const String& message, unsigned lineNumber, unsigned columnNumber, const String& sourceID) final;
    
    bool canRunBeforeUnloadConfirmPanel() final;
    bool runBeforeUnloadConfirmPanel(const String& message, CyberCore::Frame&) final;
    
    void closeWindowSoon() final;
    
    void runJavaScriptAlert(CyberCore::Frame&, const String&) final;
    bool runJavaScriptConfirm(CyberCore::Frame&, const String&) final;
    bool runJavaScriptPrompt(CyberCore::Frame&, const String& message, const String& defaultValue, String& result) final;
    void setStatusbarText(const String&) final;

    CyberCore::KeyboardUIMode keyboardUIMode() final;

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
    void contentsSizeChanged(CyberCore::Frame&, const CyberCore::IntSize&) const final;
    void intrinsicContentsSizeChanged(const CyberCore::IntSize&) const final;
    void scrollRectIntoView(const CyberCore::IntRect&) const final; // Currently only Mac has a non empty implementation.

    bool shouldUnavailablePluginMessageBeButton(CyberCore::RenderEmbeddedObject::PluginUnavailabilityReason) const final;
    void unavailablePluginButtonClicked(CyberCore::Element&, CyberCore::RenderEmbeddedObject::PluginUnavailabilityReason) const final;

    void mouseDidMoveOverElement(const CyberCore::HitTestResult&, unsigned modifierFlags, const String& toolTip, CyberCore::TextDirection) final;

    void print(CyberCore::Frame&) final;

    void exceededDatabaseQuota(CyberCore::Frame&, const String& databaseName, CyberCore::DatabaseDetails) final;

    void reachedMaxAppCacheSize(int64_t spaceNeeded) final;
    void reachedApplicationCacheOriginQuota(CyberCore::SecurityOrigin&, int64_t spaceNeeded) final;
    
#if ENABLE(INPUT_TYPE_COLOR)
    std::unique_ptr<CyberCore::ColorChooser> createColorChooser(CyberCore::ColorChooserClient&, const CyberCore::Color&) final;
#endif

#if ENABLE(DATALIST_ELEMENT)
    std::unique_ptr<CyberCore::DataListSuggestionPicker> createDataListSuggestionPicker(CyberCore::DataListSuggestionsClient&) final;
    bool canShowDataListSuggestionLabels() const final;
#endif

#if ENABLE(IOS_TOUCH_EVENTS)
    void didPreventDefaultForEvent() final;
#endif

#if PLATFORM(IOS_FAMILY)
    void didReceiveMobileDocType(bool) final;
    void setNeedsScrollNotifications(CyberCore::Frame&, bool) final;
    void didFinishContentChangeObserving(CyberCore::Frame&, WKContentChange) final;
    void notifyRevealedSelectionByScrollingFrame(CyberCore::Frame&) final;
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

    void associateEditableImageWithAttachment(CyberCore::GraphicsLayer::EmbeddedViewID, const String& attachmentID) final;
    void didCreateEditableImage(CyberCore::GraphicsLayer::EmbeddedViewID) final;
    void didDestroyEditableImage(CyberCore::GraphicsLayer::EmbeddedViewID) final;

    bool shouldUseMouseEventForSelection(const CyberCore::PlatformMouseEvent&) final;

    bool showDataDetectorsUIForElement(const CyberCore::Element&, const CyberCore::Event&) final;
#endif

#if ENABLE(ORIENTATION_EVENTS)
    int deviceOrientation() const final;
#endif

    void runOpenPanel(CyberCore::Frame&, CyberCore::FileChooser&) final;
    void showShareSheet(CyberCore::ShareDataWithParsedURL&, WTF::CompletionHandler<void(bool)>&&) final;
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

    void didAssociateFormControls(const Vector<RefPtr<CyberCore::Element>>&, CyberCore::Frame&) final;
    bool shouldNotifyOnFormChanges() final;

    bool selectItemWritingDirectionIsNatural() final;
    bool selectItemAlignmentFollowsMenuWritingDirection() final;
    RefPtr<CyberCore::PopupMenu> createPopupMenu(CyberCore::PopupMenuClient&) const final;
    RefPtr<CyberCore::SearchPopupMenu> createSearchPopupMenu(CyberCore::PopupMenuClient&) const final;

    CyberCore::GraphicsLayerFactory* graphicsLayerFactory() const final;
    void attachRootGraphicsLayer(CyberCore::Frame&, CyberCore::GraphicsLayer*) final;
    void attachViewOverlayGraphicsLayer(CyberCore::GraphicsLayer*) final;
    void setNeedsOneShotDrawingSynchronization() final;
    void scheduleRenderingUpdate() final;

    void contentRuleListNotification(const URL&, const CyberCore::ContentRuleListResults&) final;

    bool testProcessIncomingSyncMessagesWhenWaitingForSyncReply() final;

#if PLATFORM(WIN)
    void setLastSetCursorToCurrentCursor() final { }
    void AXStartFrameLoad() final { }
    void AXFinishFrameLoad() final { }
#endif

    void animationDidFinishForElement(const CyberCore::Element&) final;

    RefPtr<CyberCore::DisplayRefreshMonitor> createDisplayRefreshMonitor(CyberCore::PlatformDisplayID) const final;

#if ENABLE(GPU_PROCESS)
    RemoteRenderingBackend& ensureRemoteRenderingBackend() const;
    std::unique_ptr<CyberCore::ImageBuffer> createImageBuffer(const CyberCore::FloatSize&, CyberCore::ShouldAccelerate, CyberCore::ShouldUseDisplayList, CyberCore::RenderingPurpose, float resolutionScale, CyberCore::ColorSpace) const final;
#endif

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

#if ENABLE(VIDEO_PRESENTATION_MODE)
    void prepareForVideoFullscreen() final;
    bool supportsVideoFullscreen(CyberCore::HTMLMediaElementEnums::VideoFullscreenMode) final;
    bool supportsVideoFullscreenStandby() final;
    void setMockVideoPresentationModeEnabled(bool) final;
    void enterVideoFullscreenForVideoElement(CyberCore::HTMLVideoElement&, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode, bool standby) final;
    void exitVideoFullscreenForVideoElement(CyberCore::HTMLVideoElement&) final;
    void setUpPlaybackControlsManager(CyberCore::HTMLMediaElement&) final;
    void clearPlaybackControlsManager() final;
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
    void elementDidFocus(CyberCore::Element&) final;
    void elementDidBlur(CyberCore::Element&) final;
    void elementDidRefocus(CyberCore::Element&) final;
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

    Optional<CyberCore::ScrollbarOverlayStyle> preferredScrollbarOverlayStyle() final;

    CyberCore::Color underlayColor() const final;

    void pageExtendedBackgroundColorDidChange(CyberCore::Color) const final;
    
    void wheelEventHandlersChanged(bool) final;

    String plugInStartLabelTitle(const String& mimeType) const final;
    String plugInStartLabelSubtitle(const String& mimeType) const final;
    String plugInExtraStyleSheet() const final;
    String plugInExtraScript() const final;

    void didAddHeaderLayer(CyberCore::GraphicsLayer&) final;
    void didAddFooterLayer(CyberCore::GraphicsLayer&) final;

    bool shouldUseTiledBackingForFrameView(const CyberCore::FrameView&) const final;

    MonotonicTime timestampForPaintFrequencyTracking() const final;

    void isPlayingMediaDidChange(CyberCore::MediaProducer::MediaStateFlags, uint64_t) final;
    void handleAutoplayEvent(CyberCore::AutoplayEvent, OptionSet<CyberCore::AutoplayEventFlags>) final;

#if ENABLE(MEDIA_SESSION)
    void hasMediaSessionWithActiveMediaElementsDidChange(bool) final;
    void mediaSessionMetadataDidChange(const CyberCore::MediaSessionMetadata&) final;
    void focusedContentMediaElementDidChange(uint64_t) final;
#endif

#if ENABLE(WEB_CRYPTO)
    bool wrapCryptoKey(const Vector<uint8_t>&, Vector<uint8_t>&) const final;
    bool unwrapCryptoKey(const Vector<uint8_t>&, Vector<uint8_t>&) const final;
#endif

    String signedPublicKeyAndChallengeString(unsigned keySizeIndex, const String& challengeString, const URL&) const final;

#if ENABLE(TELEPHONE_NUMBER_DETECTION) && PLATFORM(MAC)
    void handleTelephoneNumberClick(const String& number, const CyberCore::IntPoint&) final;
#endif

#if ENABLE(SERVICE_CONTROLS)
    void handleSelectionServiceClick(CyberCore::FrameSelection&, const Vector<String>& telephoneNumbers, const CyberCore::IntPoint&) final;
    bool hasRelevantSelectionServices(bool isTextOnly) const final;
#endif

    bool shouldDispatchFakeMouseMoveEvents() const final;

    void handleAutoFillButtonClick(CyberCore::HTMLInputElement&) final;

    void inputElementDidResignStrongPasswordAppearance(CyberCore::HTMLInputElement&) final;

#if ENABLE(WIRELESS_PLAYBACK_TARGET) && !PLATFORM(IOS_FAMILY)
    void addPlaybackTargetPickerClient(CyberCore::PlaybackTargetClientContextIdentifier) final;
    void removePlaybackTargetPickerClient(CyberCore::PlaybackTargetClientContextIdentifier) final;
    void showPlaybackTargetPicker(CyberCore::PlaybackTargetClientContextIdentifier, const CyberCore::IntPoint&, bool) final;
    void playbackTargetPickerClientStateDidChange(CyberCore::PlaybackTargetClientContextIdentifier, CyberCore::MediaProducer::MediaStateFlags) final;
    void setMockMediaPlaybackTargetPickerEnabled(bool) final;
    void setMockMediaPlaybackTargetPickerState(const String&, CyberCore::MediaPlaybackTargetContext::State) final;
    void mockMediaPlaybackTargetPickerDismissPopup() final;
#endif

    void imageOrMediaDocumentSizeChanged(const CyberCore::IntSize&) final;

    RefPtr<CyberCore::Icon> createIconForFiles(const Vector<String>& filenames) final;

#if ENABLE(VIDEO) && USE(GSTREAMER)
    void requestInstallMissingMediaPlugins(const String& /*details*/, const String& /*description*/, CyberCore::MediaPlayerRequestInstallMissingPluginsCallback&) final;
#endif

    void didInvalidateDocumentMarkerRects() final;

#if ENABLE(RESOURCE_LOAD_STATISTICS)
    void hasStorageAccess(CyberCore::RegistrableDomain&& subFrameDomain, CyberCore::RegistrableDomain&& topFrameDomain, CyberCore::Frame&, WTF::CompletionHandler<void(bool)>&&) final;
    void requestStorageAccess(CyberCore::RegistrableDomain&& subFrameDomain, CyberCore::RegistrableDomain&& topFrameDomain, CyberCore::Frame&, CyberCore::StorageAccessScope, WTF::CompletionHandler<void(CyberCore::RequestStorageAccessResult)>&&) final;
#endif

#if ENABLE(DEVICE_ORIENTATION)
    void shouldAllowDeviceOrientationAndMotionAccess(CyberCore::Frame&, bool mayPrompt, CompletionHandler<void(CyberCore::DeviceOrientationOrMotionPermissionState)>&&) final;
#endif

    void configureLoggingChannel(const String&, WTFLogChannelState, WTFLogLevel) final;

    bool userIsInteracting() const final;
    void setUserIsInteracting(bool) final;

#if ENABLE(WEB_AUTHN)
    void setMockWebAuthenticationConfiguration(const CyberCore::MockWebAuthenticationConfiguration&) final;
#endif

    mutable bool m_cachedMainFrameHasHorizontalScrollbar { false };
    mutable bool m_cachedMainFrameHasVerticalScrollbar { false };
#if ENABLE(GPU_PROCESS)
    mutable std::unique_ptr<RemoteRenderingBackend> m_remoteRenderingBackend;
#endif
    WebPage& m_page;
};

} // namespace CyberKit
