/*
 * Copyright (C) 2006-2017 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include <CyberCore/ChromeClient.h>
#include <CyberCore/COMPtr.h>
#include <CyberCore/GraphicsContext.h>
#include <CyberCore/FocusDirection.h>
#include <CyberCore/ScrollTypes.h>
#include <wtf/Forward.h>
#include <wtf/RefPtr.h>

class WebView;
class WebDesktopNotificationsDelegate;

interface IWebUIDelegate;

class WebChromeClient final : public CyberCore::ChromeClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    WebChromeClient(WebView*);

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

    void addMessageToConsole(JSC::MessageSource, JSC::MessageLevel, const WTF::String& message, unsigned lineNumber, unsigned columnNumber, const WTF::String& url) final;

    bool canRunBeforeUnloadConfirmPanel() final;
    bool runBeforeUnloadConfirmPanel(const WTF::String& message, CyberCore::Frame&) final;

    void closeWindowSoon() final;

    void runJavaScriptAlert(CyberCore::Frame&, const WTF::String&) final;
    bool runJavaScriptConfirm(CyberCore::Frame&, const WTF::String&) final;
    bool runJavaScriptPrompt(CyberCore::Frame&, const WTF::String& message, const WTF::String& defaultValue, WTF::String& result) final;
    void setStatusbarText(const WTF::String&) final;

    CyberCore::KeyboardUIMode keyboardUIMode() final;

    void invalidateRootView(const CyberCore::IntRect&) final;
    void invalidateContentsAndRootView(const CyberCore::IntRect&) final;
    void invalidateContentsForSlowScroll(const CyberCore::IntRect&) final;
    void scroll(const CyberCore::IntSize& scrollDelta, const CyberCore::IntRect& rectToScroll, const CyberCore::IntRect& clipRect) final;

    CyberCore::IntPoint screenToRootView(const CyberCore::IntPoint&) const final;
    CyberCore::IntRect rootViewToScreen(const CyberCore::IntRect&) const final;
    CyberCore::IntPoint accessibilityScreenToRootView(const CyberCore::IntPoint&) const final;
    CyberCore::IntRect rootViewToAccessibilityScreen(const CyberCore::IntRect&) const final;
    PlatformPageClient platformPageClient() const final;
    void contentsSizeChanged(CyberCore::Frame&, const CyberCore::IntSize&) const final;
    void intrinsicContentsSizeChanged(const CyberCore::IntSize&) const final;

    void mouseDidMoveOverElement(const CyberCore::HitTestResult&, unsigned modifierFlags, const WTF::String&, CyberCore::TextDirection) final;
    bool shouldUnavailablePluginMessageBeButton(CyberCore::RenderEmbeddedObject::PluginUnavailabilityReason) const final;
    void unavailablePluginButtonClicked(CyberCore::Element&, CyberCore::RenderEmbeddedObject::PluginUnavailabilityReason) const final;

    void print(CyberCore::Frame&) final;

    void exceededDatabaseQuota(CyberCore::Frame&, const WTF::String&, CyberCore::DatabaseDetails) final;

    void reachedMaxAppCacheSize(int64_t spaceNeeded) final;
    void reachedApplicationCacheOriginQuota(CyberCore::SecurityOrigin&, int64_t totalSpaceNeeded) final;

    void runOpenPanel(CyberCore::Frame&, CyberCore::FileChooser&) final;
    void loadIconForFiles(const Vector<WTF::String>&, CyberCore::FileIconLoader&) final;

    void setCursor(const CyberCore::Cursor&) final;
    void setCursorHiddenUntilMouseMoves(bool) final;
    void setLastSetCursorToCurrentCursor() final;

    // Pass 0 as the GraphicsLayer to detatch the root layer.
    void attachRootGraphicsLayer(CyberCore::Frame&, CyberCore::GraphicsLayer*) final;
    void attachViewOverlayGraphicsLayer(CyberCore::GraphicsLayer*) final;
    // Sets a flag to specify that the next time content is drawn to the window,
    // the changes appear on the screen in synchrony with updates to GraphicsLayers.
    void setNeedsOneShotDrawingSynchronization() final { }
    // Sets a flag to specify that the view needs to be updated, so we need
    // to do an eager layout before the drawing.
    void scheduleCompositingLayerFlush() final;

#if PLATFORM(WIN) && USE(AVFOUNDATION)
    CyberCore::GraphicsDeviceAdapter* graphicsDeviceAdapter() const final;
#endif

    void scrollRectIntoView(const CyberCore::IntRect&) const final { }

#if ENABLE(VIDEO)
    bool supportsVideoFullscreen(CyberCore::HTMLMediaElementEnums::VideoFullscreenMode) final;
    void enterVideoFullscreenForVideoElement(CyberCore::HTMLVideoElement&, CyberCore::HTMLMediaElementEnums::VideoFullscreenMode, bool) final;
    void exitVideoFullscreenForVideoElement(CyberCore::HTMLVideoElement&) final;
#endif

#if ENABLE(NOTIFICATIONS)
    CyberCore::NotificationClient* notificationPresenter() const final { return reinterpret_cast<CyberCore::NotificationClient*>(m_notificationsDelegate.get()); }
#endif

    bool selectItemWritingDirectionIsNatural() final;
    bool selectItemAlignmentFollowsMenuWritingDirection() final;
    RefPtr<CyberCore::PopupMenu> createPopupMenu(CyberCore::PopupMenuClient&) const final;
    RefPtr<CyberCore::SearchPopupMenu> createSearchPopupMenu(CyberCore::PopupMenuClient&) const final;

#if ENABLE(FULLSCREEN_API)
    bool supportsFullScreenForElement(const CyberCore::Element&, bool withKeyboard) final;
    void enterFullScreenForElement(CyberCore::Element&) final;
    void exitFullScreenForElement(CyberCore::Element*) final;
#endif

    void wheelEventHandlersChanged(bool) final { }

    WebView* webView() { return m_webView; }

    void AXStartFrameLoad() final;
    void AXFinishFrameLoad() final;

    bool shouldUseTiledBackingForFrameView(const CyberCore::FrameView&) const final;

    RefPtr<CyberCore::Icon> createIconForFiles(const Vector<String>&) final;

    void didFinishLoadingImageForElement(CyberCore::HTMLImageElement&) final;

private:
    COMPtr<IWebUIDelegate> uiDelegate();

    WebView* m_webView;

#if ENABLE(NOTIFICATIONS)
    std::unique_ptr<WebDesktopNotificationsDelegate> m_notificationsDelegate;
#endif
};
