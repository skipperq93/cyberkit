/*
 * Copyright (C) 2020 Sony Interactive Entertainment Inc.
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

#include "PageClient.h"

namespace CyberKit {

class DrawingAreaProxy;
class PlayStationWebView;

class PageClientImpl : public PageClient
#if ENABLE(FULLSCREEN_API)
    , public WebFullScreenManagerProxyClient
#endif
{
    WTF_MAKE_FAST_ALLOCATED;
public:
    PageClientImpl(PlayStationWebView&);

private:
    // Create a new drawing area proxy for the given page.
    std::unique_ptr<DrawingAreaProxy> createDrawingAreaProxy(WebProcessProxy&) final;

    // Tell the view to invalidate the given region. The region is in view coordinates.
    void setViewNeedsDisplay(const CyberCore::Region&) final;

    // Tell the view to scroll to the given position, and whether this was a programmatic scroll.
    void requestScroll(const CyberCore::FloatPoint& scrollPosition, const CyberCore::IntPoint& scrollOrigin) final;

    // Return the current scroll position (not necessarily the same as the CyberCore scroll position, because of scaling, insets etc.)
    CyberCore::FloatPoint viewScrollPosition() final;

    // Return the size of the view the page is associated with.
    CyberCore::IntSize viewSize() final;

    // Return whether the view's containing window is active.
    bool isViewWindowActive() final;

    // Return whether the view is focused.
    bool isViewFocused() final;

    // Return whether the view is visible.
    bool isViewVisible() final;

    // Return whether the view is in a window.
    bool isViewInWindow() final;

    void processDidExit() final;
    void didRelaunchProcess() final;
    void pageClosed() final;

    void preferencesDidChange() final;

    void toolTipChanged(const String&, const String&) final;

    void didCommitLoadForMainFrame(const String& mimeType, bool useCustomContentProvider) final;

    void handleDownloadRequest(DownloadProxy&) final;

    void didChangeContentSize(const CyberCore::IntSize&) final;

    void setCursor(const CyberCore::Cursor&) final;
    void setCursorHiddenUntilMouseMoves(bool) final;
    void didChangeViewportProperties(const CyberCore::ViewportAttributes&) final;

    void registerEditCommand(Ref<WebEditCommandProxy>&&, UndoOrRedo) final;
    void clearAllEditCommands() final;
    bool canUndoRedo(UndoOrRedo) final;
    void executeUndoRedo(UndoOrRedo) final;
    void wheelEventWasNotHandledByCyberCore(const NativeWebWheelEvent&) final;

    CyberCore::FloatRect convertToDeviceSpace(const CyberCore::FloatRect&) final;
    CyberCore::FloatRect convertToUserSpace(const CyberCore::FloatRect&) final;
    CyberCore::IntPoint screenToRootView(const CyberCore::IntPoint&) final;
    CyberCore::IntRect rootViewToScreen(const CyberCore::IntRect&) final;
    CyberCore::IntPoint accessibilityScreenToRootView(const CyberCore::IntPoint&) final;
    CyberCore::IntRect rootViewToAccessibilityScreen(const CyberCore::IntRect&) final;

    void doneWithKeyEvent(const NativeWebKeyboardEvent&, bool wasEventHandled) final;

    RefPtr<WebPopupMenuProxy> createPopupMenuProxy(WebPageProxy&) final;

    void enterAcceleratedCompositingMode(const LayerTreeContext&) final;
    void exitAcceleratedCompositingMode() final;
    void updateAcceleratedCompositingMode(const LayerTreeContext&) final;

    // Auxiliary Client Creation
#if ENABLE(FULLSCREEN_API)
    WebFullScreenManagerProxyClient& fullScreenManagerProxyClient() final;
#endif

    // Custom representations.
    void didFinishLoadingDataForCustomContentProvider(const String& suggestedFilename, const IPC::DataReference&) final;

    void navigationGestureDidBegin() final;
    void navigationGestureWillEnd(bool willNavigate, WebBackForwardListItem&) final;
    void navigationGestureDidEnd(bool willNavigate, WebBackForwardListItem&) final;
    void navigationGestureDidEnd() final;
    void willRecordNavigationSnapshot(WebBackForwardListItem&) final;
    void didRemoveNavigationGestureSnapshot() final;

    void didFirstVisuallyNonEmptyLayoutForMainFrame() final;
    void didFinishNavigation(API::Navigation*) final;
    void didFailNavigation(API::Navigation*) final;
    void didSameDocumentNavigationForMainFrame(SameDocumentNavigationType) final;

    void didChangeBackgroundColor() final;
    void isPlayingAudioWillChange() final;
    void isPlayingAudioDidChange() final;

    void refView() final;
    void derefView() final;

    void didRestoreScrollPosition() final;

    CyberCore::UserInterfaceLayoutDirection userInterfaceLayoutDirection() final;

    void requestDOMPasteAccess(const CyberCore::IntRect&, const String&, CompletionHandler<void(CyberCore::DOMPasteAccessResponse)>&&) final;

    PlayStationWebView& m_view;
};

} // namespace CyberKit
