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

#include "DataReference.h"
#include "MessageReceiver.h"
#include "MessageSender.h"
#include "NetworkResourceLoadIdentifier.h"
#include "PolicyDecision.h"
#include "ProcessThrottler.h"
#include "SandboxExtension.h"
#include "WebFramePolicyListenerProxy.h"
#include "WebPageProxyIdentifier.h"
#include "WebsitePoliciesData.h"
#include <CyberCore/DiagnosticLoggingClient.h>
#include <CyberCore/FrameIdentifier.h>
#include <CyberCore/ResourceRequest.h>
#include <wtf/WeakPtr.h>

namespace API {
class Navigation;
}

namespace IPC {
class FormDataReference;
}

namespace CyberCore {
class ResourceRequest;
enum class ShouldTreatAsContinuingLoad : uint8_t;
}

namespace CyberKit {

class DrawingAreaProxy;
class SuspendedPageProxy;
class UserData;
class WebBackForwardListItem;
class WebFrameProxy;
class WebPageProxy;
class WebProcessProxy;
class WebsiteDataStore;
struct FrameInfoData;
struct NavigationActionData;
struct URLSchemeTaskParameters;
struct WebBackForwardListCounts;
struct WebNavigationDataStore;

#if HAVE(VISIBILITY_PROPAGATION_VIEW)
using LayerHostingContextID = uint32_t;
#endif

class ProvisionalPageProxy : public IPC::MessageReceiver, public IPC::MessageSender {
    WTF_MAKE_FAST_ALLOCATED;
public:
    ProvisionalPageProxy(WebPageProxy&, Ref<WebProcessProxy>&&, std::unique_ptr<SuspendedPageProxy>, uint64_t navigationID, bool isServerRedirect, const CyberCore::ResourceRequest&, ProcessSwapRequestedByClient, bool isProcessSwappingOnNavigationResponse, API::WebsitePolicies*);
    ~ProvisionalPageProxy();

    WebPageProxy& page() const { return m_page; }
    CyberCore::PageIdentifier webPageID() const { return m_webPageID; }
    WebFrameProxy* mainFrame() const { return m_mainFrame.get(); }
    WebProcessProxy& process() { return m_process.get(); }
    ProcessSwapRequestedByClient processSwapRequestedByClient() const { return m_processSwapRequestedByClient; }
    uint64_t navigationID() const { return m_navigationID; }
    const URL& provisionalURL() const { return m_provisionalLoadURL; }

    bool isProcessSwappingOnNavigationResponse() const { return m_isProcessSwappingOnNavigationResponse; }

    DrawingAreaProxy* drawingArea() const { return m_drawingArea.get(); }
    std::unique_ptr<DrawingAreaProxy> takeDrawingArea();

    void setNavigationID(uint64_t navigationID) { m_navigationID = navigationID; }

#if PLATFORM(COCOA)
    Vector<uint8_t> takeAccessibilityToken() { return WTFMove(m_accessibilityToken); }
#endif
#if PLATFORM(GTK) || PLATFORM(WPE)
    const String& accessibilityPlugID() { return m_accessibilityPlugID; }
#endif
#if HAVE(VISIBILITY_PROPAGATION_VIEW)
    LayerHostingContextID contextIDForVisibilityPropagationInWebProcess() const { return m_contextIDForVisibilityPropagationInWebProcess; }
#if ENABLE(GPU_PROCESS)
    void didCreateContextInGPUProcessForVisibilityPropagation(LayerHostingContextID);
    LayerHostingContextID contextIDForVisibilityPropagationInGPUProcess() const { return m_contextIDForVisibilityPropagationInGPUProcess; }
#endif
#endif

    void loadData(API::Navigation&, const IPC::DataReference&, const String& mimeType, const String& encoding, const String& baseURL, API::Object* userData, CyberCore::ShouldTreatAsContinuingLoad, std::optional<NavigatingToAppBoundDomain>, std::optional<WebsitePoliciesData>&&, CyberCore::SubstituteData::SessionHistoryVisibility);
    void loadRequest(API::Navigation&, CyberCore::ResourceRequest&&, API::Object* userData, CyberCore::ShouldTreatAsContinuingLoad, std::optional<NavigatingToAppBoundDomain>, std::optional<WebsitePoliciesData>&& = std::nullopt, std::optional<NetworkResourceLoadIdentifier> existingNetworkResourceLoadIdentifierToResume = std::nullopt);
    void goToBackForwardItem(API::Navigation&, WebBackForwardListItem&, RefPtr<API::WebsitePolicies>&&, CyberCore::ShouldTreatAsContinuingLoad, std::optional<NetworkResourceLoadIdentifier> existingNetworkResourceLoadIdentifierToResume = std::nullopt);
    void cancel();

    void unfreezeLayerTreeDueToSwipeAnimation();

    void processDidTerminate();

private:
    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;
    bool didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, UniqueRef<IPC::Encoder>&) final;

    // IPC::MessageSender
    IPC::Connection* messageSenderConnection() const final;
    uint64_t messageSenderDestinationID() const final;
    bool sendMessage(UniqueRef<IPC::Encoder>&&, OptionSet<IPC::SendOption>) final;
    bool sendMessageWithAsyncReply(UniqueRef<IPC::Encoder>&&, AsyncReplyHandler, OptionSet<IPC::SendOption>) final;

    void decidePolicyForNavigationActionAsync(CyberCore::FrameIdentifier, FrameInfoData&&, CyberCore::PolicyCheckIdentifier, uint64_t navigationID, NavigationActionData&&, FrameInfoData&& originatingFrameInfo, std::optional<WebPageProxyIdentifier> originatingPageID, const CyberCore::ResourceRequest& originalRequest, CyberCore::ResourceRequest&&, IPC::FormDataReference&& requestBody, CyberCore::ResourceResponse&& redirectResponse, uint64_t listenerID);
    void decidePolicyForResponse(CyberCore::FrameIdentifier, FrameInfoData&&, CyberCore::PolicyCheckIdentifier, uint64_t navigationID, const CyberCore::ResourceResponse&, const CyberCore::ResourceRequest&, bool canShowMIMEType, const String& downloadAttribute, uint64_t listenerID);
    void didChangeProvisionalURLForFrame(CyberCore::FrameIdentifier, uint64_t navigationID, URL&&);
    void didPerformServerRedirect(const String& sourceURLString, const String& destinationURLString, CyberCore::FrameIdentifier);
    void didReceiveServerRedirectForProvisionalLoadForFrame(CyberCore::FrameIdentifier, uint64_t navigationID, CyberCore::ResourceRequest&&, const UserData&);
    void didNavigateWithNavigationData(const WebNavigationDataStore&, CyberCore::FrameIdentifier);
    void didPerformClientRedirect(const String& sourceURLString, const String& destinationURLString, CyberCore::FrameIdentifier);
    void didCreateMainFrame(CyberCore::FrameIdentifier);
    void didStartProvisionalLoadForFrame(CyberCore::FrameIdentifier, FrameInfoData&&, CyberCore::ResourceRequest&&, uint64_t navigationID, URL&&, URL&& unreachableURL, const UserData&);
    void didCommitLoadForFrame(CyberCore::FrameIdentifier, FrameInfoData&&, CyberCore::ResourceRequest&&, uint64_t navigationID, const String& mimeType, bool frameHasCustomContentProvider, CyberCore::FrameLoadType, const CyberCore::CertificateInfo&, bool usedLegacyTLS, bool privateRelayed, bool containsPluginDocument, CyberCore::HasInsecureContent, CyberCore::MouseEventPolicy, const UserData&);
    void didFailProvisionalLoadForFrame(CyberCore::FrameIdentifier, FrameInfoData&&, CyberCore::ResourceRequest&&, uint64_t navigationID, const String& provisionalURL, const CyberCore::ResourceError&, CyberCore::WillContinueLoading, const UserData&, CyberCore::WillInternallyHandleFailure);
    void logDiagnosticMessageFromWebProcess(const String& message, const String& description, CyberCore::ShouldSample);
    void logDiagnosticMessageWithEnhancedPrivacyFromWebProcess(const String& message, const String& description, CyberCore::ShouldSample);
    void logDiagnosticMessageWithValueDictionaryFromWebProcess(const String& message, const String& description, const CyberCore::DiagnosticLoggingClient::ValueDictionary&, CyberCore::ShouldSample);
    void startURLSchemeTask(URLSchemeTaskParameters&&);
    void backForwardGoToItem(const CyberCore::BackForwardItemIdentifier&, CompletionHandler<void(const WebBackForwardListCounts&)>&&);
    void decidePolicyForNavigationActionSync(CyberCore::FrameIdentifier, bool isMainFrame, FrameInfoData&&, CyberCore::PolicyCheckIdentifier, uint64_t navigationID, NavigationActionData&&, FrameInfoData&& originatingFrameInfo, std::optional<WebPageProxyIdentifier> originatingPageID, const CyberCore::ResourceRequest& originalRequest, CyberCore::ResourceRequest&&, IPC::FormDataReference&& requestBody, CyberCore::ResourceResponse&& redirectResponse, CompletionHandler<void(PolicyDecision&&)>&&);
    void backForwardAddItem(BackForwardListItemState&&);
#if USE(QUICK_LOOK)
    void requestPasswordForQuickLookDocumentInMainFrame(const String& fileName, CompletionHandler<void(const String&)>&&);
#endif
#if PLATFORM(COCOA)
    void registerWebProcessAccessibilityToken(const IPC::DataReference&);
#endif
#if PLATFORM(GTK) || PLATFORM(WPE)
    void bindAccessibilityTree(const String&);
#endif
#if ENABLE(CONTENT_FILTERING)
    void contentFilterDidBlockLoadForFrame(const CyberCore::ContentFilterUnblockHandler&, CyberCore::FrameIdentifier);
#endif
#if HAVE(VISIBILITY_PROPAGATION_VIEW)
    void didCreateContextInWebProcessForVisibilityPropagation(LayerHostingContextID);
#endif

    void initializeWebPage(RefPtr<API::WebsitePolicies>&&);
    bool validateInput(CyberCore::FrameIdentifier, const std::optional<uint64_t>& navigationID = std::nullopt);

    WebPageProxy& m_page;
    CyberCore::PageIdentifier m_webPageID;
    Ref<WebProcessProxy> m_process;
    // Keep WebsiteDataStore alive for provisional page load.
    RefPtr<WebsiteDataStore> m_websiteDataStore;
    std::unique_ptr<DrawingAreaProxy> m_drawingArea;
    RefPtr<WebFrameProxy> m_mainFrame;
    uint64_t m_navigationID;
    bool m_isServerRedirect;
    CyberCore::ResourceRequest m_request;
    ProcessSwapRequestedByClient m_processSwapRequestedByClient;
    bool m_wasCommitted { false };
    bool m_isProcessSwappingOnNavigationResponse { false };
    URL m_provisionalLoadURL;

#if PLATFORM(COCOA)
    Vector<uint8_t> m_accessibilityToken;
#endif
#if PLATFORM(GTK) || PLATFORM(WPE)
    String m_accessibilityPlugID;
    CompletionHandler<void(String&&)> m_accessibilityBindCompletionHandler;
#endif
#if USE(RUNNINGBOARD)
    UniqueRef<ProcessThrottler::ForegroundActivity> m_provisionalLoadActivity;
#endif
#if HAVE(VISIBILITY_PROPAGATION_VIEW)
    LayerHostingContextID m_contextIDForVisibilityPropagationInWebProcess { 0 };
#if ENABLE(GPU_PROCESS)
    LayerHostingContextID m_contextIDForVisibilityPropagationInGPUProcess { 0 };
#endif
#endif
};

} // namespace CyberKit
