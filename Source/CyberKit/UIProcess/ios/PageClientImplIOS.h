/*
 * Copyright (C) 2012-2016 Apple Inc. All rights reserved.
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

#if PLATFORM(IOS_FAMILY)

#import "PageClientImplCocoa.h"
#import "WebFullScreenManagerProxy.h"
#import <wtf/RetainPtr.h>
#import <wtf/WeakObjCPtr.h>

OBJC_CLASS NSTextAlternatives;
OBJC_CLASS WKContentView;
OBJC_CLASS WKEditorUndoTarget;

namespace CyberCore {
enum class DOMPasteAccessResponse : uint8_t;
struct PromisedAttachmentInfo;
}

namespace CyberKit {

enum class UndoOrRedo : bool;

class PageClientImpl final : public PageClientImplCocoa
#if ENABLE(FULLSCREEN_API)
    , public WebFullScreenManagerProxyClient
#endif
    {
public:
    PageClientImpl(WKContentView *, WKWebView *);
    virtual ~PageClientImpl();
    
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
    bool canTakeForegroundAssertions() override;
    bool isViewInWindow() override;
    bool isViewVisibleOrOccluded() override;
    bool isVisuallyIdle() override;
    void processDidExit() override;
    void processWillSwap() override;
    void didRelaunchProcess() override;
#if HAVE(VISIBILITY_PROPAGATION_VIEW)
    void didCreateContextForVisibilityPropagation(LayerHostingContextID) override;
    void didCreateContextInGPUProcessForVisibilityPropagation(LayerHostingContextID) override;
#endif
#if ENABLE(GPU_PROCESS)
    void gpuProcessCrashed() override;
#endif
    void preferencesDidChange() override;
    void toolTipChanged(const String&, const String&) override;
    void decidePolicyForGeolocationPermissionRequest(WebFrameProxy&, const FrameInfoData&, Function<void(bool)>&) override;
    void didStartProvisionalLoadForMainFrame() override;
    void didFailProvisionalLoadForMainFrame() override;
    void didCommitLoadForMainFrame(const String& mimeType, bool useCustomContentProvider) override;
    void handleDownloadRequest(DownloadProxy&) override;
    void didChangeContentSize(const CyberCore::IntSize&) override;
    void setCursor(const CyberCore::Cursor&) override;
    void setCursorHiddenUntilMouseMoves(bool) override;
    void didChangeViewportProperties(const CyberCore::ViewportAttributes&) override;
    void registerEditCommand(Ref<WebEditCommandProxy>&&, UndoOrRedo) override;
    void clearAllEditCommands() override;
    bool canUndoRedo(UndoOrRedo) override;
    void executeUndoRedo(UndoOrRedo) override;
    void accessibilityWebProcessTokenReceived(const IPC::DataReference&) override;
    bool executeSavedCommandBySelector(const String& selector) override;
    void updateSecureInputState() override;
    void resetSecureInputState() override;
    void notifyInputContextAboutDiscardedComposition() override;
    void makeFirstResponder() override;
    void assistiveTechnologyMakeFirstResponder() override;
    CyberCore::FloatRect convertToDeviceSpace(const CyberCore::FloatRect&) override;
    CyberCore::FloatRect convertToUserSpace(const CyberCore::FloatRect&) override;
    CyberCore::IntPoint screenToRootView(const CyberCore::IntPoint&) override;
    CyberCore::IntRect rootViewToScreen(const CyberCore::IntRect&) override;
    CyberCore::IntPoint accessibilityScreenToRootView(const CyberCore::IntPoint&) override;
    CyberCore::IntRect rootViewToAccessibilityScreen(const CyberCore::IntRect&) override;
    void doneWithKeyEvent(const NativeWebKeyboardEvent&, bool wasEventHandled) override;
#if ENABLE(TOUCH_EVENTS)
    void doneWithTouchEvent(const NativeWebTouchEvent&, bool wasEventHandled) override;
#endif
#if ENABLE(IOS_TOUCH_EVENTS)
    void doneDeferringNativeGestures(bool preventNativeGestures) override;
#endif
    RefPtr<WebPopupMenuProxy> createPopupMenuProxy(WebPageProxy&) override;
    Ref<CyberCore::ValidationBubble> createValidationBubble(const String& message, const CyberCore::ValidationBubble::Settings&) final;

#if ENABLE(INPUT_TYPE_COLOR)
    RefPtr<WebColorPicker> createColorPicker(WebPageProxy*, const CyberCore::Color& initialColor, const CyberCore::IntRect&, Vector<CyberCore::Color>&&) final;
#endif

#if ENABLE(DATALIST_ELEMENT)
    RefPtr<WebDataListSuggestionsDropdown> createDataListSuggestionsDropdown(WebPageProxy&) final;
#endif

    void setTextIndicator(Ref<CyberCore::TextIndicator>, CyberCore::TextIndicatorWindowLifetime) override;
    void clearTextIndicator(CyberCore::TextIndicatorWindowDismissalAnimation) override;
    void setTextIndicatorAnimationProgress(float) override;

    void showSafeBrowsingWarning(const SafeBrowsingWarning&, CompletionHandler<void(Variant<CyberKit::ContinueUnsafeLoad, URL>&&)>&&) override;
    void clearSafeBrowsingWarning() override;
    void clearSafeBrowsingWarningIfForMainFrameNavigation() override;

    void enterAcceleratedCompositingMode(const LayerTreeContext&) override;
    void exitAcceleratedCompositingMode() override;
    void updateAcceleratedCompositingMode(const LayerTreeContext&) override;
    void setRemoteLayerTreeRootNode(RemoteLayerTreeNode*) override;
    CALayer* acceleratedCompositingRootLayer() const override;
    LayerHostingMode viewLayerHostingMode() override { return LayerHostingMode::OutOfProcess; }

    RefPtr<ViewSnapshot> takeViewSnapshot(Optional<CyberCore::IntRect>&&) override;
    void wheelEventWasNotHandledByCyberCore(const NativeWebWheelEvent&) override;

    void commitPotentialTapFailed() override;
    void didGetTapHighlightGeometries(uint64_t requestID, const CyberCore::Color&, const Vector<CyberCore::FloatQuad>& highlightedQuads, const CyberCore::IntSize& topLeftRadius, const CyberCore::IntSize& topRightRadius, const CyberCore::IntSize& bottomLeftRadius, const CyberCore::IntSize& bottomRightRadius, bool nodeHasBuiltInClickHandling) override;

    void didCommitLayerTree(const RemoteLayerTreeTransaction&) override;
    void layerTreeCommitComplete() override;
        
    void didPerformDictionaryLookup(const CyberCore::DictionaryPopupInfo&) override;

    bool effectiveAppearanceIsDark() const override;
    bool effectiveUserInterfaceLevelIsElevated() const override;

    void couldNotRestorePageState() override;
    void restorePageState(Optional<CyberCore::FloatPoint>, const CyberCore::FloatPoint&, const CyberCore::FloatBoxExtent&, double) override;
    void restorePageCenterAndScale(Optional<CyberCore::FloatPoint>, double) override;

    void elementDidFocus(const FocusedElementInformation&, bool userIsInteracting, bool blurPreviousNode, OptionSet<CyberCore::ActivityState::Flag> activityStateChanges, API::Object* userData) override;
    void updateInputContextAfterBlurringAndRefocusingElement() final;
    void elementDidBlur() override;
    void focusedElementDidChangeInputMode(CyberCore::InputMode) override;
    void didUpdateEditorState() override;
    bool isFocusingElement() override;
    void selectionDidChange() override;
    bool interpretKeyEvent(const NativeWebKeyboardEvent&, bool isCharEvent) override;
    void positionInformationDidChange(const InteractionInformationAtPosition&) override;
    void saveImageToLibrary(Ref<CyberCore::SharedBuffer>&&) override;
    void showPlaybackTargetPicker(bool hasVideo, const CyberCore::IntRect& elementRect, CyberCore::RouteSharingPolicy, const String&) override;
    void showDataDetectorsUIForPositionInformation(const InteractionInformationAtPosition&) override;

    bool handleRunOpenPanel(WebPageProxy*, WebFrameProxy*, const FrameInfoData&, API::OpenPanelParameters*, WebOpenPanelResultListenerProxy*) override;
    bool showShareSheet(const CyberCore::ShareDataWithParsedURL&, WTF::CompletionHandler<void(bool)>&&) override;
    
    void disableDoubleTapGesturesDuringTapIfNecessary(uint64_t requestID) override;
    void handleSmartMagnificationInformationForPotentialTap(uint64_t requestID, const CyberCore::FloatRect& renderRect, bool fitEntireRect, double viewportMinimumScale, double viewportMaximumScale, bool nodeIsRootLevel) override;

    double minimumZoomScale() const override;
    CyberCore::FloatRect documentRect() const override;

    void showInspectorHighlight(const CyberCore::Highlight&) override;
    void hideInspectorHighlight() override;

    void showInspectorIndication() override;
    void hideInspectorIndication() override;

    void enableInspectorNodeSearch() override;
    void disableInspectorNodeSearch() override;

    void scrollingNodeScrollViewWillStartPanGesture() override;
    void scrollingNodeScrollViewDidScroll() override;
    void scrollingNodeScrollWillStartScroll() override;
    void scrollingNodeScrollDidEndScroll() override;

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

    void didFinishLoadingDataForCustomContentProvider(const String& suggestedFilename, const IPC::DataReference&) override;

    Vector<String> mimeTypesWithCustomContentProviders() override;

    void navigationGestureDidBegin() override;
    void navigationGestureWillEnd(bool willNavigate, WebBackForwardListItem&) override;
    void navigationGestureDidEnd(bool willNavigate, WebBackForwardListItem&) override;
    void navigationGestureDidEnd() override;
    void willRecordNavigationSnapshot(WebBackForwardListItem&) override;
    void didRemoveNavigationGestureSnapshot() override;

    void didFirstVisuallyNonEmptyLayoutForMainFrame() override;
    void didFinishNavigation(API::Navigation*) override;
    void didFailNavigation(API::Navigation*) override;
    void didSameDocumentNavigationForMainFrame(SameDocumentNavigationType) override;
    void didNotHandleTapAsClick(const CyberCore::IntPoint&) override;
    void didCompleteSyntheticClick() override;

    void didChangeBackgroundColor() override;
    void videoControlsManagerDidChange() override;

    void refView() override;
    void derefView() override;

    void didRestoreScrollPosition() override;

    CyberCore::UserInterfaceLayoutDirection userInterfaceLayoutDirection() override;

#if USE(QUICK_LOOK)
    void requestPasswordForQuickLookDocument(const String& fileName, WTF::Function<void(const String&)>&&) override;
#endif

    void requestDOMPasteAccess(const CyberCore::IntRect& elementRect, const String&, CompletionHandler<void(CyberCore::DOMPasteAccessResponse)>&&) final;

#if ENABLE(DRAG_SUPPORT)
    void didPerformDragOperation(bool handled) override;
    void didHandleDragStartRequest(bool started) override;
    void didHandleAdditionalDragItemsRequest(bool added) override;
    void startDrag(const CyberCore::DragItem&, const ShareableBitmap::Handle& image) override;
    void willReceiveEditDragSnapshot() override;
    void didReceiveEditDragSnapshot(Optional<CyberCore::TextIndicatorData>) override;
    void didChangeDragCaretRect(const CyberCore::IntRect& previousCaretRect, const CyberCore::IntRect& caretRect) override;
#endif

    void handleAutocorrectionContext(const WebAutocorrectionContext&) final;

    void setMouseEventPolicy(CyberCore::MouseEventPolicy) final;

#if HAVE(PENCILKIT)
    RetainPtr<WKDrawingView> createDrawingView(CyberCore::GraphicsLayer::EmbeddedViewID) override;
#endif

#if ENABLE(ATTACHMENT_ELEMENT)
    void writePromisedAttachmentToPasteboard(CyberCore::PromisedAttachmentInfo&&) final;
#endif

    void cancelPointersForGestureRecognizer(UIGestureRecognizer*) override;
    WTF::Optional<unsigned> activeTouchIdentifierForGestureRecognizer(UIGestureRecognizer*) override;

    void showDictationAlternativeUI(const CyberCore::FloatRect&, CyberCore::DictationContext) final;

    WeakObjCPtr<WKContentView> m_contentView;
    RetainPtr<WKEditorUndoTarget> m_undoTarget;
};
} // namespace CyberKit

#endif // PLATFORM(IOS_FAMILY)
