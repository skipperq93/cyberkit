/*
 * Copyright (C) 2019 Apple Inc. All rights reserved.
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

#include "MessageReceiver.h"
#include "SandboxExtension.h"
#include "WebFramePolicyListenerProxy.h"
#include "WebPageProxyMessages.h"
#include "WebsitePoliciesData.h"
#include <wtf/WeakPtr.h>

namespace API {
class Navigation;
}

namespace IPC {
class FormDataReference;
}

namespace CyberCore {
class ResourceRequest;
struct BackForwardItemIdentifier;
}

namespace CyberKit {

class DrawingAreaProxy;
class SuspendedPageProxy;
class UserData;
class WebFrameProxy;
class WebPageProxy;
class WebProcessProxy;
struct FrameInfoData;
struct NavigationActionData;
struct URLSchemeTaskParameters;
struct WebNavigationDataStore;

class ProvisionalPageProxy : public IPC::MessageReceiver, public CanMakeWeakPtr<ProvisionalPageProxy> {
    WTF_MAKE_FAST_ALLOCATED;
public:
    ProvisionalPageProxy(WebPageProxy&, Ref<WebProcessProxy>&&, std::unique_ptr<SuspendedPageProxy>, uint64_t navigationID, bool isServerRedirect, const CyberCore::ResourceRequest&, ProcessSwapRequestedByClient);
    ~ProvisionalPageProxy();

    WebPageProxy& page() { return m_page; }
    WebFrameProxy* mainFrame() const { return m_mainFrame.get(); }
    WebProcessProxy& process() { return m_process.get(); }
    ProcessSwapRequestedByClient processSwapRequestedByClient() const { return m_processSwapRequestedByClient; }
    uint64_t navigationID() const { return m_navigationID; }
    const URL& provisionalURL() const { return m_provisionalLoadURL; }

    DrawingAreaProxy* drawingArea() const { return m_drawingArea.get(); }
    std::unique_ptr<DrawingAreaProxy> takeDrawingArea();

    void setNavigationID(uint64_t navigationID) { m_navigationID = navigationID; }

#if PLATFORM(COCOA)
    Vector<uint8_t> takeAccessibilityToken() { return WTFMove(m_accessibilityToken); }
#endif

    void loadData(API::Navigation&, const IPC::DataReference&, const String& MIMEType, const String& encoding, const String& baseURL, API::Object* userData, Optional<WebsitePoliciesData>&& = WTF::nullopt);
    void loadRequest(API::Navigation&, CyberCore::ResourceRequest&&, CyberCore::ShouldOpenExternalURLsPolicy, API::Object* userData, Optional<WebsitePoliciesData>&& = WTF::nullopt);
    void goToBackForwardItem(API::Navigation&, WebBackForwardListItem&, Optional<WebsitePoliciesData>&&);
    void cancel();

    void processDidTerminate();

private:
    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;
    void didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, std::unique_ptr<IPC::Encoder>&) final;

    void decidePolicyForNavigationActionAsync(uint64_t frameID, CyberCore::SecurityOriginData&&, CyberCore::PolicyCheckIdentifier, uint64_t navigationID, NavigationActionData&&, FrameInfoData&&,
        Optional<CyberCore::PageIdentifier> originatingPageID, const CyberCore::ResourceRequest& originalRequest, CyberCore::ResourceRequest&&, IPC::FormDataReference&& requestBody,
        CyberCore::ResourceResponse&& redirectResponse, const UserData&, uint64_t listenerID);
    void decidePolicyForResponse(uint64_t frameID, const CyberCore::SecurityOriginData&, CyberCore::PolicyCheckIdentifier, uint64_t navigationID, const CyberCore::ResourceResponse&,
        const CyberCore::ResourceRequest&, bool canShowMIMEType, const String& downloadAttribute, uint64_t listenerID, const UserData&);
    void didChangeProvisionalURLForFrame(uint64_t frameID, uint64_t navigationID, URL&&);
    void didPerformServerRedirect(const String& sourceURLString, const String& destinationURLString, uint64_t frameID);
    void didReceiveServerRedirectForProvisionalLoadForFrame(uint64_t frameID, uint64_t navigationID, CyberCore::ResourceRequest&&, const UserData&);
    void didNavigateWithNavigationData(const WebNavigationDataStore&, uint64_t frameID);
    void didPerformClientRedirect(const String& sourceURLString, const String& destinationURLString, uint64_t frameID);
    void didCreateMainFrame(uint64_t frameID);
    void didStartProvisionalLoadForFrame(uint64_t frameID, uint64_t navigationID, URL&&, URL&& unreachableURL, const UserData&);
    void didCommitLoadForFrame(uint64_t frameID, uint64_t navigationID, const String& mimeType, bool frameHasCustomContentProvider, uint32_t frameLoadType, const CyberCore::CertificateInfo&, bool containsPluginDocument, Optional<CyberCore::HasInsecureContent> forcedHasInsecureContent, const UserData&);
    void didFailProvisionalLoadForFrame(uint64_t frameID, const CyberCore::SecurityOriginData& frameSecurityOrigin, uint64_t navigationID, const String& provisionalURL, const CyberCore::ResourceError&, CyberCore::WillContinueLoading, const UserData&);
    void startURLSchemeTask(URLSchemeTaskParameters&&);
    void backForwardGoToItem(const CyberCore::BackForwardItemIdentifier&, CompletionHandler<void(SandboxExtension::Handle&&)>&&);
    void decidePolicyForNavigationActionSync(uint64_t frameID, bool isMainFrame, CyberCore::SecurityOriginData&&, CyberCore::PolicyCheckIdentifier, uint64_t navigationID, NavigationActionData&&,
        FrameInfoData&&, Optional<CyberCore::PageIdentifier> originatingPageID, const CyberCore::ResourceRequest& originalRequest, CyberCore::ResourceRequest&&, IPC::FormDataReference&& requestBody,
        CyberCore::ResourceResponse&& redirectResponse, const UserData&, Messages::WebPageProxy::DecidePolicyForNavigationActionSync::DelayedReply&&);
#if USE(QUICK_LOOK)
    void didRequestPasswordForQuickLookDocumentInMainFrame(const String& fileName);
#endif
#if PLATFORM(COCOA)
    void registerWebProcessAccessibilityToken(const IPC::DataReference&);
#endif
#if ENABLE(CONTENT_FILTERING)
    void contentFilterDidBlockLoadForFrame(const CyberCore::ContentFilterUnblockHandler&, uint64_t frameID);
#endif

    void initializeWebPage();
    bool validateInput(uint64_t frameID, const Optional<uint64_t>& navigationID = WTF::nullopt);

    WebPageProxy& m_page;
    Ref<WebProcessProxy> m_process;
    std::unique_ptr<DrawingAreaProxy> m_drawingArea;
    RefPtr<WebFrameProxy> m_mainFrame;
    uint64_t m_navigationID;
    bool m_isServerRedirect;
    CyberCore::ResourceRequest m_request;
    ProcessSwapRequestedByClient m_processSwapRequestedByClient;
    bool m_wasCommitted { false };
    URL m_provisionalLoadURL;

#if PLATFORM(COCOA)
    Vector<uint8_t> m_accessibilityToken;
#endif
#if PLATFORM(IOS_FAMILY)
    ProcessThrottler::ForegroundActivityToken m_suspensionToken;
#endif
};

} // namespace CyberKit
