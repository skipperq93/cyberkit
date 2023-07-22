
/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 * Portions Copyright (c) 2010 Motorola Mobility, Inc.  All rights reserved.
 * Copyright (C) 2011 Igalia S.L.
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

#include "DefaultUndoController.h"
#include "PageClient.h"
#include "WebFullScreenManagerProxy.h"
#include "WebPageProxy.h"
#include <CyberCore/IntSize.h>
#include <gtk/gtk.h>
#include <memory>

namespace CyberCore {
enum class DOMPasteAccessPolicy : uint8_t;
}

namespace CyberKit {

class DrawingAreaProxy;
class WebPageNamespace;

enum class UndoOrRedo : bool;

class PageClientImpl : public PageClient
#if ENABLE(FULLSCREEN_API)
    , public WebFullScreenManagerProxyClient
#endif
{
    WTF_MAKE_FAST_ALLOCATED;
public:
    explicit PageClientImpl(GtkWidget*);

    GtkWidget* viewWidget() { return m_viewWidget; }

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
    bool isViewInWindow() override;
    void processWillSwap() override;
    void processDidExit() override;
    void didRelaunchProcess() override;
    void pageClosed() override;
    void preferencesDidChange() override;
    void toolTipChanged(const WTF::String&, const WTF::String&) override;
    void setCursor(const CyberCore::Cursor&) override;
    void setCursorHiddenUntilMouseMoves(bool) override;
    void didChangeViewportProperties(const CyberCore::ViewportAttributes&) override;
    void registerEditCommand(Ref<WebEditCommandProxy>&&, UndoOrRedo) override;
    void clearAllEditCommands() override;
    bool canUndoRedo(UndoOrRedo) override;
    void executeUndoRedo(UndoOrRedo) override;
    CyberCore::FloatRect convertToDeviceSpace(const CyberCore::FloatRect&) override;
    CyberCore::FloatRect convertToUserSpace(const CyberCore::FloatRect&) override;
    CyberCore::IntPoint screenToRootView(const CyberCore::IntPoint&) override;
    CyberCore::IntRect rootViewToScreen(const CyberCore::IntRect&) override;
    CyberCore::IntPoint accessibilityScreenToRootView(const CyberCore::IntPoint&) override;
    CyberCore::IntRect rootViewToAccessibilityScreen(const CyberCore::IntRect&) override;
    void doneWithKeyEvent(const NativeWebKeyboardEvent&, bool wasEventHandled) override;
    RefPtr<WebPopupMenuProxy> createPopupMenuProxy(WebPageProxy&) override;
    Ref<WebContextMenuProxy> createContextMenuProxy(WebPageProxy&, ContextMenuContextData&&, const UserData&) override;
#if ENABLE(INPUT_TYPE_COLOR)
    RefPtr<WebColorPicker> createColorPicker(WebPageProxy*, const CyberCore::Color& initialColor, const CyberCore::IntRect&, Vector<CyberCore::Color>&&) override;
#endif
#if ENABLE(DATALIST_ELEMENT)
    RefPtr<WebDataListSuggestionsDropdown> createDataListSuggestionsDropdown(WebPageProxy&) override;
#endif
    void selectionDidChange() override;
    RefPtr<ViewSnapshot> takeViewSnapshot() override;
#if ENABLE(DRAG_SUPPORT)
    void startDrag(Ref<CyberCore::SelectionData>&&, CyberCore::DragOperation, RefPtr<ShareableBitmap>&& dragImage) override;
#endif

    void enterAcceleratedCompositingMode(const LayerTreeContext&) override;
    void exitAcceleratedCompositingMode() override;
    void updateAcceleratedCompositingMode(const LayerTreeContext&) override;

    void handleDownloadRequest(DownloadProxy&) override;
    void didChangeContentSize(const CyberCore::IntSize&) override;
    void didCommitLoadForMainFrame(const String& mimeType, bool useCustomContentProvider) override;
    void didFailLoadForMainFrame() override;

    // Auxiliary Client Creation
#if ENABLE(FULLSCREEN_API)
    WebFullScreenManagerProxyClient& fullScreenManagerProxyClient() final;
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

    void navigationGestureDidBegin() override;
    void navigationGestureWillEnd(bool, WebBackForwardListItem&) override;
    void navigationGestureDidEnd(bool, WebBackForwardListItem&) override;
    void navigationGestureDidEnd() override;
    void willRecordNavigationSnapshot(WebBackForwardListItem&) override;
    void didRemoveNavigationGestureSnapshot() override;

    void didStartProvisionalLoadForMainFrame() override;
    void didFirstVisuallyNonEmptyLayoutForMainFrame() override;
    void didFinishLoadForMainFrame() override;
    void didSameDocumentNavigationForMainFrame(SameDocumentNavigationType) override;

#if ENABLE(TOUCH_EVENTS)
    void doneWithTouchEvent(const NativeWebTouchEvent&, bool wasEventHandled) override;
#endif

    void wheelEventWasNotHandledByCyberCore(const NativeWebWheelEvent&) override;

    void didChangeBackgroundColor() override;

    void refView() override;
    void derefView() override;

    void didRestoreScrollPosition() override;
    void isPlayingAudioWillChange() final { }
    void isPlayingAudioDidChange() final { }

    void requestDOMPasteAccess(const CyberCore::IntRect&, const String&, CompletionHandler<void(CyberCore::DOMPasteAccessResponse)>&&) final;

#if ENABLE(VIDEO) && USE(GSTREAMER)
    bool decidePolicyForInstallMissingMediaPluginsPermissionRequest(InstallMissingMediaPluginsPermissionRequest&) override;
#endif

    CyberCore::UserInterfaceLayoutDirection userInterfaceLayoutDirection() override;

    bool effectiveAppearanceIsDark() const override;

#if USE(WPE_RENDERER)
    IPC::Attachment hostFileDescriptor() override;
#endif

    void didChangeWebPageID() const override;

    // Members of PageClientImpl class
    GtkWidget* m_viewWidget;
    DefaultUndoController m_undoController;
};

} // namespace CyberKit
