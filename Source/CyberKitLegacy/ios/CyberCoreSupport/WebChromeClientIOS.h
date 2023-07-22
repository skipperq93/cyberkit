/*
 * Copyright (C) 2008-2017 Apple Inc. All rights reserved.
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

#if PLATFORM(IOS_FAMILY)

#import "WebChromeClient.h"

class WebChromeClientIOS final : public WebChromeClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    WebChromeClientIOS(WebView* webView)
        : WebChromeClient(webView)
    {
    }

private:
    void setWindowRect(const CyberCore::FloatRect&) final;
    CyberCore::FloatRect windowRect() final;
    void setStatusbarText(const WTF::String&) final { }

    void focus() final;
    void takeFocus(CyberCore::FocusDirection) final { }

    void runJavaScriptAlert(CyberCore::Frame&, const WTF::String&) final;
    bool runJavaScriptConfirm(CyberCore::Frame&, const WTF::String&) final;
    bool runJavaScriptPrompt(CyberCore::Frame&, const WTF::String& message, const WTF::String& defaultValue, WTF::String& result) final;

    void runOpenPanel(CyberCore::Frame&, CyberCore::FileChooser&) final;
    void showShareSheet(CyberCore::ShareDataWithParsedURL&, CompletionHandler<void(bool)>&&) final;

    void setCursor(const CyberCore::Cursor&) final { }
    void setCursorHiddenUntilMouseMoves(bool) final { }

#if ENABLE(TOUCH_EVENTS)
    void didPreventDefaultForEvent() final;
#endif

    void didReceiveMobileDocType(bool) final;
    void setNeedsScrollNotifications(CyberCore::Frame&, bool) final;
    void didFinishContentChangeObserving(CyberCore::Frame&, WKContentChange) final;
    CyberCore::FloatSize screenSize() const final;
    CyberCore::FloatSize availableScreenSize() const final;
    CyberCore::FloatSize overrideScreenSize() const final;
    void dispatchDisabledAdaptationsDidChange(const OptionSet<CyberCore::DisabledAdaptations>&) const final;
    void dispatchViewportPropertiesDidChange(const CyberCore::ViewportArguments&) const final;
    void notifyRevealedSelectionByScrollingFrame(CyberCore::Frame&) final;
    bool isStopping() final;
    void didLayout(LayoutType) final;
    void didStartOverflowScroll() final;
    void didEndOverflowScroll() final;

    void suppressFormNotifications() final;
    void restoreFormNotifications() final;

    void elementDidFocus(CyberCore::Element&) final;
    void elementDidBlur(CyberCore::Element&) final;

    void attachRootGraphicsLayer(CyberCore::Frame&, CyberCore::GraphicsLayer*) final;

    void didFlushCompositingLayers() final;

    void updateViewportConstrainedLayers(HashMap<PlatformLayer*, std::unique_ptr<CyberCore::ViewportConstraints>>&, const HashMap<PlatformLayer*, PlatformLayer*>&) final;

    bool fetchCustomFixedPositionLayoutRect(CyberCore::IntRect&) final;
    void addOrUpdateScrollingLayer(CyberCore::Node*, PlatformLayer*, PlatformLayer*, const CyberCore::IntSize&, bool allowHorizontalScrollbar, bool allowVerticalScrollbar) final;
    void removeScrollingLayer(CyberCore::Node*, PlatformLayer*, PlatformLayer*) final;

    bool selectItemWritingDirectionIsNatural() final;
    bool selectItemAlignmentFollowsMenuWritingDirection() final;
    RefPtr<CyberCore::PopupMenu> createPopupMenu(CyberCore::PopupMenuClient&) const final;
    RefPtr<CyberCore::SearchPopupMenu> createSearchPopupMenu(CyberCore::PopupMenuClient&) const final;

    void webAppOrientationsUpdated() final;
    void focusedElementChanged(CyberCore::Element*) final;
    void showPlaybackTargetPicker(bool hasVideo, CyberCore::RouteSharingPolicy, const String&) final;
    RefPtr<CyberCore::Icon> createIconForFiles(const Vector<String>& filenames) final;

#if ENABLE(ORIENTATION_EVENTS)
    int deviceOrientation() const final;
#endif

    int m_formNotificationSuppressions { 0 };
};

#endif
