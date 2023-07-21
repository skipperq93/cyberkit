/*
 * Copyright (C) 2006-2017 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <CyberCore/COMPtr.h>
#include <CyberCore/FrameLoaderClient.h>
#include <CyberCore/ProgressTrackerClient.h>

namespace CyberCore {
    class PluginManualLoader;
    class PluginView;
}

class WebFrame;
class WebFramePolicyListener;
class WebHistory;

class WebFrameLoaderClient : public CyberCore::FrameLoaderClient, public CyberCore::ProgressTrackerClient {
public:
    WebFrameLoaderClient(WebFrame* = 0);
    ~WebFrameLoaderClient();

    void setWebFrame(WebFrame* webFrame) { m_webFrame = webFrame; }
    WebFrame* webFrame() const { return m_webFrame; }

    void dispatchDidFailToStartPlugin(const CyberCore::PluginView&) const;

    Optional<CyberCore::PageIdentifier> pageID() const final;
    Optional<uint64_t> frameID() const final;
    PAL::SessionID sessionID() const final;

    bool hasWebView() const override;

    Ref<CyberCore::FrameNetworkingContext> createNetworkingContext() override;

    void frameLoaderDestroyed() override;
    void makeRepresentation(CyberCore::DocumentLoader*) override;
    void forceLayoutForNonHTML() override;

    void setCopiesOnScroll() override;

    void detachedFromParent2() override;
    void detachedFromParent3() override;

    void convertMainResourceLoadToDownload(CyberCore::DocumentLoader*, PAL::SessionID, const CyberCore::ResourceRequest&, const CyberCore::ResourceResponse&) override;
    void assignIdentifierToInitialRequest(unsigned long identifier, CyberCore::DocumentLoader*, const CyberCore::ResourceRequest&) override;

    void dispatchWillSendRequest(CyberCore::DocumentLoader*, unsigned long identifier, CyberCore::ResourceRequest&, const CyberCore::ResourceResponse& redirectResponse) override;
    bool shouldUseCredentialStorage(CyberCore::DocumentLoader*, unsigned long identifier) override;
    void dispatchDidReceiveAuthenticationChallenge(CyberCore::DocumentLoader*, unsigned long identifier, const CyberCore::AuthenticationChallenge&) override;
    void dispatchDidReceiveResponse(CyberCore::DocumentLoader*, unsigned long identifier, const CyberCore::ResourceResponse&) override;
    void dispatchDidReceiveContentLength(CyberCore::DocumentLoader*, unsigned long identifier, int dataLength) override;
    void dispatchDidFinishLoading(CyberCore::DocumentLoader*, unsigned long identifier) override;
    void dispatchDidFailLoading(CyberCore::DocumentLoader*, unsigned long identifier, const CyberCore::ResourceError&) override;
#if USE(CFURLCONNECTION)
    bool shouldCacheResponse(CyberCore::DocumentLoader*, unsigned long identifier, const CyberCore::ResourceResponse&, const unsigned char* data, unsigned long long length) override;
#endif

    void dispatchDidDispatchOnloadEvents() override;
    void dispatchDidReceiveServerRedirectForProvisionalLoad() override;
    void dispatchDidCancelClientRedirect() override;
    void dispatchWillPerformClientRedirect(const URL&, double interval, WallTime fireDate, CyberCore::LockBackForwardList) override;
    void dispatchDidChangeLocationWithinPage() override;
    void dispatchDidPushStateWithinPage() override;
    void dispatchDidReplaceStateWithinPage() override;
    void dispatchDidPopStateWithinPage() override;
    void dispatchWillClose() override;
    void dispatchDidStartProvisionalLoad() override;
    void dispatchDidReceiveTitle(const CyberCore::StringWithDirection&) override;
    void dispatchDidCommitLoad(Optional<CyberCore::HasInsecureContent>) override;
    void dispatchDidFailProvisionalLoad(const CyberCore::ResourceError&, CyberCore::WillContinueLoading) override;
    void dispatchDidFailLoad(const CyberCore::ResourceError&) override;
    void dispatchDidFinishDocumentLoad() override;
    void dispatchDidFinishLoad() override;
    void dispatchDidReachLayoutMilestone(OptionSet<CyberCore::LayoutMilestone>) override;

    void dispatchDecidePolicyForResponse(const CyberCore::ResourceResponse&, const CyberCore::ResourceRequest&, CyberCore::PolicyCheckIdentifier, const String&, CyberCore::FramePolicyFunction&&) override;
    void dispatchDecidePolicyForNewWindowAction(const CyberCore::NavigationAction&, const CyberCore::ResourceRequest&, CyberCore::FormState*, const WTF::String& frameName, CyberCore::PolicyCheckIdentifier, CyberCore::FramePolicyFunction&&) override;
    void dispatchDecidePolicyForNavigationAction(const CyberCore::NavigationAction&, const CyberCore::ResourceRequest&, const CyberCore::ResourceResponse& redirectResponse, CyberCore::FormState*, CyberCore::PolicyDecisionMode, CyberCore::PolicyCheckIdentifier, CyberCore::FramePolicyFunction&&) override;
    void cancelPolicyCheck() override;

    void dispatchUnableToImplementPolicy(const CyberCore::ResourceError&) override;

    void dispatchWillSendSubmitEvent(Ref<CyberCore::FormState>&&) override;
    void dispatchWillSubmitForm(CyberCore::FormState&, CompletionHandler<void()>&&) override;

    void revertToProvisionalState(CyberCore::DocumentLoader*) override;
    bool dispatchDidLoadResourceFromMemoryCache(CyberCore::DocumentLoader*, const CyberCore::ResourceRequest&, const CyberCore::ResourceResponse&, int length) override;

    CyberCore::Frame* dispatchCreatePage(const CyberCore::NavigationAction&) override;
    void dispatchShow() override;

    void setMainDocumentError(CyberCore::DocumentLoader*, const CyberCore::ResourceError&) override;
    void setMainFrameDocumentReady(bool) override;

    void startDownload(const CyberCore::ResourceRequest&, const String& suggestedName = String()) override;

    void progressStarted(CyberCore::Frame&) override;
    void progressEstimateChanged(CyberCore::Frame&) override;
    void progressFinished(CyberCore::Frame&) override;

    void committedLoad(CyberCore::DocumentLoader*, const char*, int) override;
    void finishedLoading(CyberCore::DocumentLoader*) override;

    void willChangeTitle(CyberCore::DocumentLoader*) override;
    void didChangeTitle(CyberCore::DocumentLoader*) override;

    void willReplaceMultipartContent() override { }
    void didReplaceMultipartContent() override { }

    void updateGlobalHistory() override;
    void updateGlobalHistoryRedirectLinks() override;
    bool shouldGoToHistoryItem(CyberCore::HistoryItem&) const override;

    void didDisplayInsecureContent() override;
    void didRunInsecureContent(CyberCore::SecurityOrigin&, const URL&) override;
    void didDetectXSS(const URL&, bool didBlockEntirePage) override;

    CyberCore::ResourceError cancelledError(const CyberCore::ResourceRequest&) override;
    CyberCore::ResourceError blockedError(const CyberCore::ResourceRequest&) override;
    CyberCore::ResourceError blockedByContentBlockerError(const CyberCore::ResourceRequest&) override;
    CyberCore::ResourceError cannotShowURLError(const CyberCore::ResourceRequest&) override;
    CyberCore::ResourceError interruptedForPolicyChangeError(const CyberCore::ResourceRequest&) override;
    CyberCore::ResourceError cannotShowMIMETypeError(const CyberCore::ResourceResponse&) override;
    CyberCore::ResourceError fileDoesNotExistError(const CyberCore::ResourceResponse&) override;
    CyberCore::ResourceError pluginWillHandleLoadError(const CyberCore::ResourceResponse&) override;

    bool shouldFallBack(const CyberCore::ResourceError&) override;

    WTF::String userAgent(const URL&) override;

    Ref<CyberCore::DocumentLoader> createDocumentLoader(const CyberCore::ResourceRequest&, const CyberCore::SubstituteData&) override;
    void updateCachedDocumentLoader(CyberCore::DocumentLoader&) override { }

    void setTitle(const CyberCore::StringWithDirection&, const URL&) override;

    void savePlatformDataToCachedFrame(CyberCore::CachedFrame*) override;
    void transitionToCommittedFromCachedFrame(CyberCore::CachedFrame*) override;
    void transitionToCommittedForNewPage() override;

    bool canHandleRequest(const CyberCore::ResourceRequest&) const override;
    bool canShowMIMEType(const WTF::String& MIMEType) const override;
    bool canShowMIMETypeAsHTML(const WTF::String& MIMEType) const override;
    bool representationExistsForURLScheme(const WTF::String& URLScheme) const override;
    WTF::String generatedMIMETypeForURLScheme(const WTF::String& URLScheme) const override;

    void frameLoadCompleted() override;
    void saveViewStateToItem(CyberCore::HistoryItem&) override;
    void restoreViewState() override;
    void provisionalLoadStarted() override;
    void didFinishLoad() override;
    void prepareForDataSourceReplacement() override;

    void didSaveToPageCache() override;
    void didRestoreFromPageCache() override;

    void dispatchDidBecomeFrameset(bool) override;

    bool canCachePage() const override;

    RefPtr<CyberCore::Frame> createFrame(const URL&, const WTF::String& name, CyberCore::HTMLFrameOwnerElement&,
        const WTF::String& referrer) override;
    RefPtr<CyberCore::Widget> createPlugin(const CyberCore::IntSize&, CyberCore::HTMLPlugInElement&, const URL&, const Vector<WTF::String>&, const Vector<WTF::String>&, const WTF::String&, bool loadManually) override;
    void redirectDataToPlugin(CyberCore::Widget&) override;

    RefPtr<CyberCore::Widget> createJavaAppletWidget(const CyberCore::IntSize&, CyberCore::HTMLAppletElement&, const URL& baseURL, const Vector<WTF::String>& paramNames, const Vector<WTF::String>& paramValues) override;

    CyberCore::ObjectContentType objectContentType(const URL&, const WTF::String& mimeType) override;
    WTF::String overrideMediaType() const override;

    void dispatchDidClearWindowObjectInWorld(CyberCore::DOMWrapperWorld&) override;

    COMPtr<WebFramePolicyListener> setUpPolicyListener(CyberCore::PolicyCheckIdentifier, CyberCore::FramePolicyFunction&&);
    void receivedPolicyDecision(CyberCore::PolicyAction);

    bool shouldAlwaysUsePluginDocument(const WTF::String& mimeType) const override;

    void prefetchDNS(const String&) override;

private:
    WebHistory* webHistory() const;

    class WebFramePolicyListenerPrivate;
    std::unique_ptr<WebFramePolicyListenerPrivate> m_policyListenerPrivate;

    WebFrame* m_webFrame;

    // Points to the manual loader that data should be redirected to.
    CyberCore::PluginManualLoader* m_manualLoader;

    bool m_hasSentResponseToPlugin;
};
