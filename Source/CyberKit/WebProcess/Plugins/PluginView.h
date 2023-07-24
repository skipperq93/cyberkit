/*
 * Copyright (C) 2010-2018 Apple Inc. All rights reserved.
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

#include "LayerTreeContext.h"
#include "NPRuntimeObjectMap.h"
#include "Plugin.h"
#include "PluginController.h"
#include "WebFrame.h"
#include <CyberCore/ActivityState.h>
#include <CyberCore/FindOptions.h>
#include <CyberCore/Image.h>
#include <CyberCore/MediaCanStartListener.h>
#include <CyberCore/MediaProducer.h>
#include <CyberCore/PluginViewBase.h>
#include <CyberCore/ResourceError.h>
#include <CyberCore/ResourceResponse.h>
#include <CyberCore/Timer.h>
#include <memory>
#include <wtf/Deque.h>
#include <wtf/RunLoop.h>

// FIXME: Eventually this should move to CyberCore.

#if PLATFORM(COCOA)
OBJC_CLASS NSDictionary;
OBJC_CLASS PDFSelection;
#endif

namespace WTF {
class MachSendRight;
}

namespace CyberCore {
class Frame;
class HTMLPlugInElement;
class MouseEvent;
}

namespace CyberKit {

class WebEvent;

class PluginView : public CyberCore::PluginViewBase, public PluginController, private CyberCore::MediaCanStartListener, private WebFrame::LoadListener, private CyberCore::MediaProducer {
public:
    static Ref<PluginView> create(CyberCore::HTMLPlugInElement&, Ref<Plugin>&&, const Plugin::Parameters&);

    void recreateAndInitialize(Ref<Plugin>&&);

    CyberCore::Frame* frame() const;

    bool isBeingDestroyed() const { return !m_plugin || m_plugin->isBeingDestroyed(); }

    void manualLoadDidReceiveResponse(const CyberCore::ResourceResponse&);
    void manualLoadDidReceiveData(const char* bytes, int length);
    void manualLoadDidFinishLoading();
    void manualLoadDidFail(const CyberCore::ResourceError&);

    void activityStateDidChange(OptionSet<CyberCore::ActivityState::Flag> changed);
    void setLayerHostingMode(LayerHostingMode);

#if PLATFORM(COCOA)
    void setDeviceScaleFactor(float);
    void windowAndViewFramesChanged(const CyberCore::FloatRect& windowFrameInScreenCoordinates, const CyberCore::FloatRect& viewFrameInWindowCoordinates);
    bool sendComplexTextInput(uint64_t pluginComplexTextInputIdentifier, const String& textInput);
    RetainPtr<PDFDocument> pdfDocumentForPrinting() const { return m_plugin->pdfDocumentForPrinting(); }
    NSObject *accessibilityObject() const;
    id accessibilityAssociatedPluginParentForElement(CyberCore::Element*) const override;
#endif

    CyberCore::HTMLPlugInElement* pluginElement() const { return m_pluginElement.get(); }
    const Plugin::Parameters& initialParameters() const { return m_parameters; }
    Plugin* plugin() const { return m_plugin.get(); }

    void setPageScaleFactor(double scaleFactor, CyberCore::IntPoint origin);
    double pageScaleFactor() const;
    bool handlesPageScaleFactor() const;
    bool requiresUnifiedScaleFactor() const;

    void pageScaleFactorDidChange();
    void topContentInsetDidChange();

    void webPageDestroyed();

    bool handleEditingCommand(const String& commandName, const String& argument);
    bool isEditingCommandEnabled(const String& commandName);
    
    unsigned countFindMatches(const String& target, CyberCore::FindOptions, unsigned maxMatchCount);
    bool findString(const String& target, CyberCore::FindOptions, unsigned maxMatchCount);

    String getSelectionString() const;

    bool shouldAllowScripting();

    RefPtr<CyberCore::SharedBuffer> liveResourceData() const;
    bool performDictionaryLookupAtLocation(const CyberCore::FloatPoint&);
    String getSelectionForWordAtPoint(const CyberCore::FloatPoint&) const;
    bool existingSelectionContainsPoint(const CyberCore::FloatPoint&) const;

private:
    PluginView(CyberCore::HTMLPlugInElement&, Ref<Plugin>&&, const Plugin::Parameters&);
    virtual ~PluginView();

    void initializePlugin();

    void viewGeometryDidChange();
    void viewVisibilityDidChange();
    CyberCore::IntRect clipRectInWindowCoordinates() const;
    void focusPluginElement();
    
    void pendingURLRequestsTimerFired();
    class URLRequest;
    void performURLRequest(URLRequest*);

    // Perform a URL request where the frame target is not null.
    void performFrameLoadURLRequest(URLRequest*);

    // Perform a URL request where the URL protocol is "javascript:".
    void performJavaScriptURLRequest(URLRequest*);

    class Stream;
    void addStream(Stream*);
    void removeStream(Stream*);
    void cancelAllStreams();

    void redeliverManualStream();

    void pluginSnapshotTimerFired();
    void pluginDidReceiveUserInteraction();

    bool shouldCreateTransientPaintingSnapshot() const;

    // CyberCore::PluginViewBase
#if PLATFORM(COCOA)
    PlatformLayer* platformLayer() const override;
#endif
    JSC::JSObject* scriptObject(JSC::JSGlobalObject*) override;
    void storageBlockingStateChanged() override;
    void privateBrowsingStateChanged(bool) override;
    bool getFormValue(String&) override;
    bool scroll(CyberCore::ScrollDirection, CyberCore::ScrollGranularity) override;
    CyberCore::Scrollbar* horizontalScrollbar() override;
    CyberCore::Scrollbar* verticalScrollbar() override;
    bool wantsWheelEvents() override;
    bool shouldAlwaysAutoStart() const override;
    void beginSnapshottingRunningPlugin() override;
    bool shouldAllowNavigationFromDrags() const override;
    bool shouldNotAddLayer() const override;
    void willDetachRenderer() override;

    // CyberCore::Widget
    void setFrameRect(const CyberCore::IntRect&) override;
    void paint(CyberCore::GraphicsContext&, const CyberCore::IntRect&, CyberCore::Widget::SecurityOriginPaintPolicy, CyberCore::EventRegionContext*) override;
    void invalidateRect(const CyberCore::IntRect&) override;
    void setFocus(bool) override;
    void frameRectsChanged() override;
    void setParent(CyberCore::ScrollView*) override;
    void handleEvent(CyberCore::Event&) override;
    void notifyWidget(CyberCore::WidgetNotification) override;
    void show() override;
    void hide() override;
    void setParentVisible(bool) override;
    bool transformsAffectFrameRect() override;
    void clipRectChanged() override;

    // CyberCore::MediaCanStartListener
    void mediaCanStart(CyberCore::Document&) override;

    // CyberCore::MediaProducer
    MediaProducer::MediaStateFlags mediaState() const override { return m_pluginIsPlayingAudio ? MediaProducer::IsPlayingAudio : MediaProducer::IsNotPlaying; }
    void pageMutedStateDidChange() override;

    // PluginController
    void invalidate(const CyberCore::IntRect&) override;
    String userAgent() override;
    void loadURL(uint64_t requestID, const String& method, const String& urlString, const String& target, const CyberCore::HTTPHeaderMap& headerFields, const Vector<uint8_t>& httpBody, bool allowPopups) override;
    void cancelStreamLoad(uint64_t streamID) override;
    void continueStreamLoad(uint64_t streamID) override;
    void cancelManualStreamLoad() override;
#if ENABLE(NETSCAPE_PLUGIN_API)
    NPObject* windowScriptNPObject() override;
    NPObject* pluginElementNPObject() override;
    bool evaluate(NPObject*, const String& scriptString, NPVariant* result, bool allowPopups) override;
    void setPluginIsPlayingAudio(bool) override;
    bool isMuted() const override;
#endif
    void setStatusbarText(const String&) override;
    bool isAcceleratedCompositingEnabled() override;
    void pluginProcessCrashed() override;
#if PLATFORM(COCOA)
    void pluginFocusOrWindowFocusChanged(bool pluginHasFocusAndWindowHasFocus) override;
    void setComplexTextInputState(PluginComplexTextInputState) override;
    const WTF::MachSendRight& compositingRenderServerPort() override;
#endif
    float contentsScaleFactor() override;
    String proxiesForURL(const String&) override;
    String cookiesForURL(const String&) override;
    void setCookiesForURL(const String& urlString, const String& cookieString) override;
    bool getAuthenticationInfo(const CyberCore::ProtectionSpace&, String& username, String& password) override;
    bool isPrivateBrowsingEnabled() override;
    bool asynchronousPluginInitializationEnabled() const override;
    bool asynchronousPluginInitializationEnabledForAllPlugins() const override;
    bool artificialPluginInitializationDelayEnabled() const override;
    void protectPluginFromDestruction() override;
    void unprotectPluginFromDestruction() override;

    void didInitializePlugin() override;
    void didFailToInitializePlugin() override;
    void destroyPluginAndReset();

    // WebFrame::LoadListener
    void didFinishLoad(WebFrame*) override;
    void didFailLoad(WebFrame*, bool wasCancelled) override;

    std::unique_ptr<WebEvent> createWebEvent(CyberCore::MouseEvent&) const;

    RefPtr<CyberCore::HTMLPlugInElement> m_pluginElement;
    RefPtr<Plugin> m_plugin;
    WebPage* m_webPage;
    Plugin::Parameters m_parameters;

    bool m_isInitialized { false };
    bool m_isWaitingForSynchronousInitialization { false };
    bool m_isWaitingUntilMediaCanStart { false };
    bool m_pluginProcessHasCrashed { false };

#if ENABLE(PRIMARY_SNAPSHOTTED_PLUGIN_HEURISTIC)
    bool m_didPlugInStartOffScreen { false };
#endif

    // Pending URLRequests that the plug-in has made.
    Deque<RefPtr<URLRequest>> m_pendingURLRequests;
    RunLoop::Timer<PluginView> m_pendingURLRequestsTimer;

    // Pending frame loads that the plug-in has made.
    typedef HashMap<RefPtr<WebFrame>, RefPtr<URLRequest>> FrameLoadMap;
    FrameLoadMap m_pendingFrameLoads;

    // Streams that the plug-in has requested to load. 
    HashMap<uint64_t, RefPtr<Stream>> m_streams;

#if ENABLE(NETSCAPE_PLUGIN_API)
    // A map of all related NPObjects for this plug-in view.
    NPRuntimeObjectMap m_npRuntimeObjectMap { this };
#endif

    // The manual stream state. This is used so we can deliver a manual stream to a plug-in
    // when it is initialized.
    enum class ManualStreamState { Initial, HasReceivedResponse, Finished, Failed };
    ManualStreamState m_manualStreamState { ManualStreamState::Initial };

    CyberCore::ResourceResponse m_manualStreamResponse;
    CyberCore::ResourceError m_manualStreamError;
    RefPtr<CyberCore::SharedBuffer> m_manualStreamData;

    // This snapshot is used to avoid side effects should the plugin run JS during painting.
    RefPtr<ShareableBitmap> m_transientPaintingSnapshot;
    // This timer is used when plugin snapshotting is enabled, to capture a plugin placeholder.
    CyberCore::DeferrableOneShotTimer m_pluginSnapshotTimer;
#if ENABLE(PRIMARY_SNAPSHOTTED_PLUGIN_HEURISTIC) || PLATFORM(COCOA)
    unsigned m_countSnapshotRetries { 0 };
#endif
    bool m_didReceiveUserInteraction { false };

    double m_pageScaleFactor { 1 };

    bool m_pluginIsPlayingAudio { false };
};

} // namespace CyberKit
