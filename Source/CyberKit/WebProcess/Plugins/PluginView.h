/*
 * Copyright (C) 2010-2022 Apple Inc. All rights reserved.
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

#if ENABLE(PDFKIT_PLUGIN)

#include <CyberCore/FindOptions.h>
#include <CyberCore/PluginViewBase.h>
#include <CyberCore/ResourceResponse.h>
#include <CyberCore/SharedBuffer.h>
#include <CyberCore/Timer.h>
#include <memory>
#include <wtf/RunLoop.h>

OBJC_CLASS NSDictionary;
OBJC_CLASS PDFDocument;
OBJC_CLASS PDFSelection;

namespace CyberCore {
class HTMLPlugInElement;
class LocalFrame;
}

namespace CyberKit {

class PDFPlugin;
class ShareableBitmap;
class WebPage;

struct WebHitTestResultData;

class PluginView final : public CyberCore::PluginViewBase {
public:
    static RefPtr<PluginView> create(CyberCore::HTMLPlugInElement&, const URL&, const String& contentType, bool shouldUseManualLoader);

    CyberCore::LocalFrame* frame() const;

    bool isBeingDestroyed() const;

    void manualLoadDidReceiveResponse(const CyberCore::ResourceResponse&);
    void manualLoadDidReceiveData(const CyberCore::SharedBuffer&);
    void manualLoadDidFinishLoading();
    void manualLoadDidFail();

    void setDeviceScaleFactor(float);
    RetainPtr<PDFDocument> pdfDocumentForPrinting() const;
    CyberCore::FloatSize pdfDocumentSizeForPrinting() const;
    id accessibilityHitTest(const CyberCore::IntPoint&) const final;
    id accessibilityObject() const final;
    id accessibilityAssociatedPluginParentForElement(CyberCore::Element*) const final;

    CyberCore::HTMLPlugInElement& pluginElement() const { return m_pluginElement; }
    const URL& mainResourceURL() const { return m_mainResourceURL; }

    void setPageScaleFactor(double);
    double pageScaleFactor() const;

    void pageScaleFactorDidChange();
    void topContentInsetDidChange();

    void webPageDestroyed();

    bool handleEditingCommand(const String& commandName, const String& argument);
    bool isEditingCommandEnabled(const String& commandName);
    
    unsigned countFindMatches(const String& target, CyberCore::FindOptions, unsigned maxMatchCount);
    bool findString(const String& target, CyberCore::FindOptions, unsigned maxMatchCount);

    String getSelectionString() const;

    RefPtr<CyberCore::FragmentedSharedBuffer> liveResourceData() const;
    bool performDictionaryLookupAtLocation(const CyberCore::FloatPoint&);
    bool existingSelectionContainsPoint(const CyberCore::FloatPoint&) const;

    std::tuple<String, PDFSelection *, NSDictionary *> lookupTextAtLocation(const CyberCore::FloatPoint&, WebHitTestResultData&) const;
    CyberCore::FloatRect rectForSelectionInRootView(PDFSelection *) const;
    CGFloat contentScaleFactor() const;
    
    bool isUsingUISideCompositing() const;

private:
    PluginView(CyberCore::HTMLPlugInElement&, const URL&, const String& contentType, bool shouldUseManualLoader, WebPage&);
    virtual ~PluginView();

    void initializePlugin();

    void viewGeometryDidChange();
    void viewVisibilityDidChange();
    CyberCore::IntRect clipRectInWindowCoordinates() const;
    void focusPluginElement();
    
    void pendingResourceRequestTimerFired();

    void loadMainResource();
    void redeliverManualStream();

    bool shouldCreateTransientPaintingSnapshot() const;

    // CyberCore::PluginViewBase
    PlatformLayer* platformLayer() const final;
    bool scroll(CyberCore::ScrollDirection, CyberCore::ScrollGranularity) final;
    CyberCore::ScrollPosition scrollPositionForTesting() const final;
    CyberCore::Scrollbar* horizontalScrollbar() final;
    CyberCore::Scrollbar* verticalScrollbar() final;
    bool wantsWheelEvents() final;
    bool shouldAllowNavigationFromDrags() const final;
    void willDetachRenderer() final;

    // CyberCore::Widget
    void setFrameRect(const CyberCore::IntRect&) final;
    void paint(CyberCore::GraphicsContext&, const CyberCore::IntRect&, CyberCore::Widget::SecurityOriginPaintPolicy, CyberCore::EventRegionContext*) final;
    void invalidateRect(const CyberCore::IntRect&) final;
    void frameRectsChanged() final;
    void setParent(CyberCore::ScrollView*) final;
    void handleEvent(CyberCore::Event&) final;
    void notifyWidget(CyberCore::WidgetNotification) final;
    void show() final;
    void hide() final;
    void setParentVisible(bool) final;
    bool transformsAffectFrameRect() final;
    void clipRectChanged() final;

    Ref<CyberCore::HTMLPlugInElement> m_pluginElement;
    Ref<PDFPlugin> m_plugin;
    WeakPtr<WebPage> m_webPage;
    URL m_mainResourceURL;
    String m_mainResourceContentType;
    bool m_shouldUseManualLoader { false };

    bool m_isInitialized { false };

    // Pending request that the plug-in has made.
    std::unique_ptr<const CyberCore::ResourceRequest> m_pendingResourceRequest;
    RunLoop::Timer m_pendingResourceRequestTimer;

    // Stream that the plug-in has requested to load.
    class Stream;
    RefPtr<Stream> m_stream;

    // The manual stream state. We deliver a manual stream to a plug-in when it is initialized.
    enum class ManualStreamState { Initial, HasReceivedResponse, Finished, Failed };
    ManualStreamState m_manualStreamState { ManualStreamState::Initial };
    CyberCore::ResourceResponse m_manualStreamResponse;
    CyberCore::SharedBufferBuilder m_manualStreamData;

    // This snapshot is used to avoid side effects should the plugin run JS during painting.
    RefPtr<ShareableBitmap> m_transientPaintingSnapshot;

    double m_pageScaleFactor { 1 };
};

} // namespace CyberKit

#endif
