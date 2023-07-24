/*
 * Copyright (C) 2010, 2011, 2016 Apple Inc. All rights reserved.
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

#if PLATFORM(MAC)

#include "CorrectionPanel.h"
#include "PageClientImplCocoa.h"
#include "WebFullScreenManagerProxy.h"
#include <CyberCore/DOMPasteAccess.h>
#include <wtf/CompletionHandler.h>
#include <wtf/Forward.h>

@class WKEditorUndoTarget;
@class WKView;

namespace CyberCore {
class AlternativeTextUIController;
struct DragItem;
struct PromisedAttachmentInfo;
}

namespace CyberKit {

class WebViewImpl;

class PageClientImpl final : public PageClientImplCocoa
#if ENABLE(FULLSCREEN_API)
    , public WebFullScreenManagerProxyClient
#endif
    {
public:
    PageClientImpl(NSView *, WKWebView *);
    virtual ~PageClientImpl();

    // FIXME: Eventually WebViewImpl should become the PageClient.
    void setImpl(WebViewImpl&);

    void viewWillMoveToAnotherWindow();

private:
    // PageClient
    std::unique_ptr<DrawingAreaProxy> createDrawingAreaProxy(WebProcessProxy&) override;
    void setViewNeedsDisplay(const CyberCore::Region&) override;
    void requestScroll(const CyberCore::FloatPoint& scrollPosition, const CyberCore::IntPoint& scrollOrigin) override;
    CyberCore::FloatPoint viewScrollPosition() override;

    CyberCore::IntSize viewSize() override;
    bool isViewWindowActive() override;
    bool isViewFocused() override;
    bool isViewVisible() override;
    bool isViewVisibleOrOccluded() override;
    bool isViewInWindow() override;
    bool isVisuallyIdle() override;
    LayerHostingMode viewLayerHostingMode() override;
    ColorSpaceData colorSpace() override;
    void setRemoteLayerTreeRootNode(RemoteLayerTreeNode*) override;
    CALayer *acceleratedCompositingRootLayer() const override;

    void processDidExit() override;
    void processWillSwap() override;
    void pageClosed() override;
    void didRelaunchProcess() override;
    void preferencesDidChange() override;
    void toolTipChanged(const String& oldToolTip, const String& newToolTip) override;
    void didCommitLoadForMainFrame(const String& mimeType, bool useCustomContentProvider) override;
    void didFinishLoadingDataForCustomContentProvider(const String& suggestedFilename, const IPC::DataReference&) override;
    void handleDownloadRequest(DownloadProxy&) override;
    void didChangeContentSize(const CyberCore::IntSize&) override;
    void setCursor(const CyberCore::Cursor&) override;
    void setCursorHiddenUntilMouseMoves(bool) override;
    void didChangeViewportProperties(const CyberCore::ViewportAttributes&) override;

    void registerEditCommand(Ref<WebEditCommandProxy>&&, UndoOrRedo) override;
    void clearAllEditCommands() override;
    bool canUndoRedo(UndoOrRedo) override;
    void executeUndoRedo(UndoOrRedo) override;
    bool executeSavedCommandBySelector(const String& selector) override;
    void startDrag(const CyberCore::DragItem&, const ShareableBitmap::Handle& image) override;
    void setPromisedDataForImage(const String& pasteboardName, Ref<CyberCore::SharedBuffer>&& imageBuffer, const String& filename, const String& extension, const String& title,
        const String& url, const String& visibleUrl, RefPtr<CyberCore::SharedBuffer>&& archiveBuffer) override;
    void updateSecureInputState() override;
    void resetSecureInputState() override;
    void notifyInputContextAboutDiscardedComposition() override;
    void selectionDidChange() override;
    void showSafeBrowsingWarning(const SafeBrowsingWarning&, CompletionHandler<void(Variant<CyberKit::ContinueUnsafeLoad, URL>&&)>&&) override;
    void clearSafeBrowsingWarning() override;
    void clearSafeBrowsingWarningIfForMainFrameNavigation() override;
    bool hasSafeBrowsingWarning() const override;
    
    bool showShareSheet(const CyberCore::ShareDataWithParsedURL&, WTF::CompletionHandler<void(bool)>&&) override;
        
    CyberCore::FloatRect convertToDeviceSpace(const CyberCore::FloatRect&) override;
    CyberCore::FloatRect convertToUserSpace(const CyberCore::FloatRect&) override;
    CyberCore::IntPoint screenToRootView(const CyberCore::IntPoint&) override;
    CyberCore::IntRect rootViewToScreen(const CyberCore::IntRect&) override;
#if PLATFORM(MAC)
    CyberCore::IntRect rootViewToWindow(const CyberCore::IntRect&) override;
#endif
    CyberCore::IntPoint accessibilityScreenToRootView(const CyberCore::IntPoint&) override;
    CyberCore::IntRect rootViewToAccessibilityScreen(const CyberCore::IntRect&) override;

    void pinnedStateWillChange() final;
    void pinnedStateDidChange() final;

    CGRect boundsOfLayerInLayerBackedWindowCoordinates(CALayer *) const override;

    void doneWithKeyEvent(const NativeWebKeyboardEvent&, bool wasEventHandled) override;

    RefPtr<WebPopupMenuProxy> createPopupMenuProxy(WebPageProxy&) override;
#if ENABLE(CONTEXT_MENUS)
    Ref<WebContextMenuProxy> createContextMenuProxy(WebPageProxy&, ContextMenuContextData&&, const UserData&) override;
#endif

#if ENABLE(INPUT_TYPE_COLOR)
    RefPtr<WebColorPicker> createColorPicker(WebPageProxy*, const CyberCore::Color& initialColor, const CyberCore::IntRect&, Vector<CyberCore::Color>&&) override;
#endif

#if ENABLE(DATALIST_ELEMENT)
    RefPtr<WebDataListSuggestionsDropdown> createDataListSuggestionsDropdown(WebPageProxy&) override;
#endif

    Ref<CyberCore::ValidationBubble> createValidationBubble(const String& message, const CyberCore::ValidationBubble::Settings&) final;

    void setTextIndicator(Ref<CyberCore::TextIndicator>, CyberCore::TextIndicatorWindowLifetime) override;
    void clearTextIndicator(CyberCore::TextIndicatorWindowDismissalAnimation) override;
    void setTextIndicatorAnimationProgress(float) override;

    void enterAcceleratedCompositingMode(const LayerTreeContext&) override;
    void exitAcceleratedCompositingMode() override;
    void updateAcceleratedCompositingMode(const LayerTreeContext&) override;
    void didFirstLayerFlush(const LayerTreeContext&) override;

    RefPtr<ViewSnapshot> takeViewSnapshot(Optional<CyberCore::IntRect>&&) override;
    void wheelEventWasNotHandledByCyberCore(const NativeWebWheelEvent&) override;
#if ENABLE(MAC_GESTURE_EVENTS)
    void gestureEventWasNotHandledByCyberCore(const NativeWebGestureEvent&) override;
#endif

    void accessibilityWebProcessTokenReceived(const IPC::DataReference&) override;

    void pluginFocusOrWindowFocusChanged(uint64_t pluginComplexTextInputIdentifier, bool pluginHasFocusAndWindowHasFocus) override;
    void setPluginComplexTextInputState(uint64_t pluginComplexTextInputIdentifier, PluginComplexTextInputState) override;

    void makeFirstResponder() override;
    void assistiveTechnologyMakeFirstResponder() override;
    void setShouldSuppressFirstResponderChanges(bool shouldSuppress) override { m_shouldSuppressFirstResponderChanges = shouldSuppress; }

    void didPerformDictionaryLookup(const CyberCore::DictionaryPopupInfo&) override;

    void showCorrectionPanel(CyberCore::AlternativeTextType, const CyberCore::FloatRect& boundingBoxOfReplacedString, const String& replacedString, const String& replacementString, const Vector<String>& alternativeReplacementStrings) override;
    void dismissCorrectionPanel(CyberCore::ReasonForDismissingAlternativeText) override;
    String dismissCorrectionPanelSoon(CyberCore::ReasonForDismissingAlternativeText) override;
    void recordAutocorrectionResponse(CyberCore::AutocorrectionResponse, const String& replacedString, const String& replacementString) override;

    void recommendedScrollbarStyleDidChange(CyberCore::ScrollbarStyle) override;

    void intrinsicContentSizeDidChange(const CyberCore::IntSize& intrinsicContentSize) override;

    void showDictationAlternativeUI(const CyberCore::FloatRect& boundingBoxOfDictatedText, CyberCore::DictationContext) final;

    void setEditableElementIsFocused(bool) override;

    void registerInsertionUndoGrouping() override;

    // Auxiliary Client Creation
#if ENABLE(FULLSCREEN_API)
    WebFullScreenManagerProxyClient& fullScreenManagerProxyClient() override;
#endif

#if ENABLE(FULLSCREEN_API)
    // WebFullScreenManagerProxyClient
    void closeFullScreenManager() override;
    bool isFullScreen() override;
    void enterFullScreen() override;
    void exitFullScreen() override;
    void beganEnterFullScreen(const CyberCore::IntRect& initialFrame, const CyberCore::IntRect& finalFrame) override;
    void beganExitFullScreen(const CyberCore::IntRect& initialFrame, const CyberCore::IntRect& finalFrame) override;
#endif

    void navigationGestureDidBegin() override;
    void navigationGestureWillEnd(bool willNavigate, WebBackForwardListItem&) override;
    void navigationGestureDidEnd(bool willNavigate, WebBackForwardListItem&) override;
    void navigationGestureDidEnd() override;
    void willRecordNavigationSnapshot(WebBackForwardListItem&) override;
    void didRemoveNavigationGestureSnapshot() override;

    void requestDOMPasteAccess(const CyberCore::IntRect&, const String&, CompletionHandler<void(CyberCore::DOMPasteAccessResponse)>&&) final;

    NSView *activeView() const;
    NSWindow *activeWindow() const;

    void didStartProvisionalLoadForMainFrame() override;
    void didFirstVisuallyNonEmptyLayoutForMainFrame() override;
    void didFinishNavigation(API::Navigation*) override;
    void didFailNavigation(API::Navigation*) override;
    void didSameDocumentNavigationForMainFrame(SameDocumentNavigationType) override;
    void handleControlledElementIDResponse(const String&) override;

    void didPerformImmediateActionHitTest(const WebHitTestResultData&, bool contentPreventsDefault, API::Object*) override;
    NSObject *immediateActionAnimationControllerForHitTestResult(RefPtr<API::HitTestResult>, uint64_t, RefPtr<API::Object>) override;

    void didHandleAcceptedCandidate() override;

    void videoControlsManagerDidChange() override;

    void showPlatformContextMenu(NSMenu *, CyberCore::IntPoint) override;

    void didChangeBackgroundColor() override;

    void startWindowDrag() override;
    NSWindow *platformWindow() override;

    CyberCore::UserInterfaceLayoutDirection userInterfaceLayoutDirection() override;
    bool effectiveAppearanceIsDark() const override;
    bool effectiveUserInterfaceLevelIsElevated() const override;

#if ENABLE(DRAG_SUPPORT)
    void didPerformDragOperation(bool handled) final;
#endif

    NSView *inspectorAttachmentView() override;
    _WKRemoteObjectRegistry *remoteObjectRegistry() override;

#if ENABLE(WIRELESS_PLAYBACK_TARGET)
    CyberCore::WebMediaSessionManager& mediaSessionManager() override;
#endif

    void refView() override;
    void derefView() override;

    void didRestoreScrollPosition() override;
    bool windowIsFrontWindowUnderMouse(const NativeWebMouseEvent&) override;

    void takeFocus(CyberCore::FocusDirection) override;

#if HAVE(APP_ACCENT_COLORS)
    CyberCore::Color accentColor() override;
#endif

    NSView *m_view;
    WeakPtr<WebViewImpl> m_impl;
#if USE(AUTOCORRECTION_PANEL)
    CorrectionPanel m_correctionPanel;
#endif

    bool m_shouldSuppressFirstResponderChanges { false };
};

} // namespace CyberKit

#endif // PLATFORM(MAC)
