/*
 * Copyright (C) 2010-2020 Apple Inc. All rights reserved.
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

#include "SameDocumentNavigationType.h"
#include "WebPageProxyIdentifier.h"
#include <CyberCore/FrameIdentifier.h>
#include <CyberCore/FrameLoaderClient.h>
#include <pal/SessionID.h>
#include <wtf/Scope.h>

namespace CyberKit {

class PluginView;
class WebFrame;
struct WebsitePoliciesData;
    
class WebFrameLoaderClient final : public CyberCore::FrameLoaderClient {
public:
    explicit WebFrameLoaderClient(Ref<WebFrame>&&, std::optional<ScopeExit<Function<void()>>>&& = std::nullopt);
    ~WebFrameLoaderClient();

    WebFrame& webFrame() const { return m_frame.get(); }

    bool frameHasCustomContentProvider() const { return m_frameHasCustomContentProvider; }

    void setUseIconLoadingClient(bool useIconLoadingClient) { m_useIconLoadingClient = useIconLoadingClient; }

    void applyToDocumentLoader(WebsitePoliciesData&&);

    std::optional<WebPageProxyIdentifier> webPageProxyID() const;
    std::optional<CyberCore::PageIdentifier> pageID() const final;

#if ENABLE(TRACKING_PREVENTION)
    bool hasFrameSpecificStorageAccess() final { return !!m_frameSpecificStorageAccessIdentifier; }
    
    struct FrameSpecificStorageAccessIdentifier {
        CyberCore::FrameIdentifier frameID;
        CyberCore::PageIdentifier pageID;
    };
    void setHasFrameSpecificStorageAccess(FrameSpecificStorageAccessIdentifier&&);
    void didLoadFromRegistrableDomain(CyberCore::RegistrableDomain&&) final;
    Vector<CyberCore::RegistrableDomain> loadedSubresourceDomains() const final;
#endif

    CyberCore::AllowsContentJavaScript allowsContentJavaScriptFromMostRecentNavigation() const final;

    ScopeExit<Function<void()>> takeFrameInvalidator() { return WTFMove(m_frameInvalidator); }

private:
    bool hasHTMLView() const final;
    bool hasWebView() const final;
    
    void makeRepresentation(CyberCore::DocumentLoader*) final;
#if PLATFORM(IOS_FAMILY)
    bool forceLayoutOnRestoreFromBackForwardCache() final;
#endif
    void forceLayoutForNonHTML() final;
    
    void setCopiesOnScroll() final;
    
    void detachedFromParent2() final;
    void detachedFromParent3() final;
    
    void assignIdentifierToInitialRequest(CyberCore::ResourceLoaderIdentifier, CyberCore::DocumentLoader*, const CyberCore::ResourceRequest&) final;
    
    void dispatchWillSendRequest(CyberCore::DocumentLoader*, CyberCore::ResourceLoaderIdentifier, CyberCore::ResourceRequest&, const CyberCore::ResourceResponse& redirectResponse) final;
    bool shouldUseCredentialStorage(CyberCore::DocumentLoader*, CyberCore::ResourceLoaderIdentifier) final;
    void dispatchDidReceiveAuthenticationChallenge(CyberCore::DocumentLoader*, CyberCore::ResourceLoaderIdentifier, const CyberCore::AuthenticationChallenge&) final;
#if USE(PROTECTION_SPACE_AUTH_CALLBACK)
    bool canAuthenticateAgainstProtectionSpace(CyberCore::DocumentLoader*, CyberCore::ResourceLoaderIdentifier, const CyberCore::ProtectionSpace&) final;
#endif
#if PLATFORM(IOS_FAMILY)
    RetainPtr<CFDictionaryRef> connectionProperties(CyberCore::DocumentLoader*, CyberCore::ResourceLoaderIdentifier) final;
#endif
    void dispatchDidReceiveResponse(CyberCore::DocumentLoader*, CyberCore::ResourceLoaderIdentifier, const CyberCore::ResourceResponse&) final;
    void dispatchDidReceiveContentLength(CyberCore::DocumentLoader*, CyberCore::ResourceLoaderIdentifier, int dataLength) final;
    void dispatchDidFinishLoading(CyberCore::DocumentLoader*, CyberCore::ResourceLoaderIdentifier) final;
    void dispatchDidFailLoading(CyberCore::DocumentLoader*, CyberCore::ResourceLoaderIdentifier, const CyberCore::ResourceError&) final;
    bool dispatchDidLoadResourceFromMemoryCache(CyberCore::DocumentLoader*, const CyberCore::ResourceRequest&, const CyberCore::ResourceResponse&, int length) final;
#if ENABLE(DATA_DETECTION)
    void dispatchDidFinishDataDetection(NSArray *detectionResults) final;
#endif
    void dispatchDidChangeMainDocument() final;
    void dispatchWillChangeDocument(const URL& currentUrl, const URL& newUrl) final;

    void dispatchDidDispatchOnloadEvents() final;
    void dispatchDidReceiveServerRedirectForProvisionalLoad() final;
    void dispatchDidChangeProvisionalURL() final;
    void dispatchDidCancelClientRedirect() final;
    void dispatchWillPerformClientRedirect(const URL&, double interval, WallTime fireDate, CyberCore::LockBackForwardList) final;
    void dispatchDidChangeLocationWithinPage() final;
    void dispatchDidPushStateWithinPage() final;
    void dispatchDidReplaceStateWithinPage() final;
    void dispatchDidPopStateWithinPage() final;
    void didSameDocumentNavigationForFrameViaJSHistoryAPI(SameDocumentNavigationType);
    void dispatchWillClose() final;
    void dispatchDidStartProvisionalLoad() final;
    void dispatchDidReceiveTitle(const CyberCore::StringWithDirection&) final;
    void dispatchDidCommitLoad(std::optional<CyberCore::HasInsecureContent>, std::optional<CyberCore::UsedLegacyTLS>, std::optional<CyberCore::WasPrivateRelayed>) final;
    void dispatchDidFailProvisionalLoad(const CyberCore::ResourceError&, CyberCore::WillContinueLoading, CyberCore::WillInternallyHandleFailure) final;
    void dispatchDidFailLoad(const CyberCore::ResourceError&) final;
    void dispatchDidFinishDocumentLoad() final;
    void dispatchDidFinishLoad() final;
    void dispatchDidExplicitOpen(const URL&, const String& mimeType) final;

    void dispatchDidReachLayoutMilestone(OptionSet<CyberCore::LayoutMilestone>) final;
    void dispatchDidReachVisuallyNonEmptyState() final;
    void dispatchDidLayout() final;

    CyberCore::LocalFrame* dispatchCreatePage(const CyberCore::NavigationAction&, CyberCore::NewFrameOpenerPolicy) final;
    void dispatchShow() final;
    
    void dispatchDecidePolicyForResponse(const CyberCore::ResourceResponse&, const CyberCore::ResourceRequest&, CyberCore::PolicyCheckIdentifier, const String&, CyberCore::FramePolicyFunction&&) final;
    void dispatchDecidePolicyForNewWindowAction(const CyberCore::NavigationAction&, const CyberCore::ResourceRequest&, CyberCore::FormState*, const String& frameName, CyberCore::PolicyCheckIdentifier, CyberCore::FramePolicyFunction&&) final;
    void dispatchDecidePolicyForNavigationAction(const CyberCore::NavigationAction&, const CyberCore::ResourceRequest&, const CyberCore::ResourceResponse& redirectResponse, CyberCore::FormState*, CyberCore::PolicyDecisionMode, CyberCore::PolicyCheckIdentifier, CyberCore::FramePolicyFunction&&) final;
    void cancelPolicyCheck() final;
    
    void dispatchUnableToImplementPolicy(const CyberCore::ResourceError&) final;
    
    void dispatchWillSendSubmitEvent(Ref<CyberCore::FormState>&&) final;
    void dispatchWillSubmitForm(CyberCore::FormState&, CompletionHandler<void()>&&) final;
    
    void revertToProvisionalState(CyberCore::DocumentLoader*) final;
    void setMainDocumentError(CyberCore::DocumentLoader*, const CyberCore::ResourceError&) final;
    
    void setMainFrameDocumentReady(bool) final;
    
    void startDownload(const CyberCore::ResourceRequest&, const String& suggestedName = String()) final;
    
    void willChangeTitle(CyberCore::DocumentLoader*) final;
    void didChangeTitle(CyberCore::DocumentLoader*) final;

    void willReplaceMultipartContent() final;
    void didReplaceMultipartContent() final;

    void committedLoad(CyberCore::DocumentLoader*, const CyberCore::SharedBuffer&) final;
    void finishedLoading(CyberCore::DocumentLoader*) final;
    
    void updateGlobalHistory() final;
    void updateGlobalHistoryRedirectLinks() final;
    
    bool shouldGoToHistoryItem(CyberCore::HistoryItem&) const final;

    void didDisplayInsecureContent() final;
    void didRunInsecureContent(CyberCore::SecurityOrigin&, const URL&) final;

#if ENABLE(SERVICE_WORKER)
    void didFinishServiceWorkerPageRegistration(bool success) final;
#endif

    CyberCore::ResourceError cancelledError(const CyberCore::ResourceRequest&) const final;
    CyberCore::ResourceError blockedError(const CyberCore::ResourceRequest&) const final;
    CyberCore::ResourceError blockedByContentBlockerError(const CyberCore::ResourceRequest&) const final;
    CyberCore::ResourceError cannotShowURLError(const CyberCore::ResourceRequest&) const final;
    CyberCore::ResourceError interruptedForPolicyChangeError(const CyberCore::ResourceRequest&) const final;
#if ENABLE(CONTENT_FILTERING)
    CyberCore::ResourceError blockedByContentFilterError(const CyberCore::ResourceRequest&) const final;
#endif
    
    CyberCore::ResourceError cannotShowMIMETypeError(const CyberCore::ResourceResponse&) const final;
    CyberCore::ResourceError fileDoesNotExistError(const CyberCore::ResourceResponse&) const final;
    CyberCore::ResourceError httpsUpgradeRedirectLoopError(const CyberCore::ResourceRequest&) const final;
    CyberCore::ResourceError pluginWillHandleLoadError(const CyberCore::ResourceResponse&) const final;
    
    bool shouldFallBack(const CyberCore::ResourceError&) const final;
    
    bool canHandleRequest(const CyberCore::ResourceRequest&) const final;
    bool canShowMIMEType(const String& MIMEType) const final;
    bool canShowMIMETypeAsHTML(const String& MIMEType) const final;
    bool representationExistsForURLScheme(StringView URLScheme) const final;
    String generatedMIMETypeForURLScheme(StringView URLScheme) const final;
    
    void frameLoadCompleted() final;
    void saveViewStateToItem(CyberCore::HistoryItem&) final;
    void restoreViewState() final;
    void provisionalLoadStarted() final;
    void didFinishLoad() final;
    void prepareForDataSourceReplacement() final;
    
    Ref<CyberCore::DocumentLoader> createDocumentLoader(const CyberCore::ResourceRequest&, const CyberCore::SubstituteData&) final;
    void updateCachedDocumentLoader(CyberCore::DocumentLoader&) final;

    void setTitle(const CyberCore::StringWithDirection&, const URL&) final;
    
    String userAgent(const URL&) const final;

    String overrideContentSecurityPolicy() const final;

    void savePlatformDataToCachedFrame(CyberCore::CachedFrame*) final;
    void transitionToCommittedFromCachedFrame(CyberCore::CachedFrame*) final;
#if PLATFORM(IOS_FAMILY)
    void didRestoreFrameHierarchyForCachedFrame() final;
#endif
    void transitionToCommittedForNewPage() final;

    void didRestoreFromBackForwardCache() final;

    bool canCachePage() const final;
    void convertMainResourceLoadToDownload(CyberCore::DocumentLoader*, const CyberCore::ResourceRequest&, const CyberCore::ResourceResponse&) final;

    RefPtr<CyberCore::LocalFrame> createFrame(const AtomString& name, CyberCore::HTMLFrameOwnerElement&) final;

    RefPtr<CyberCore::Widget> createPlugin(const CyberCore::IntSize&, CyberCore::HTMLPlugInElement&, const URL&, const Vector<AtomString>&, const Vector<AtomString>&, const String&, bool loadManually) final;
    void redirectDataToPlugin(CyberCore::Widget&) final;
    
    CyberCore::ObjectContentType objectContentType(const URL&, const String& mimeType) final;
    AtomString overrideMediaType() const final;

    void dispatchDidClearWindowObjectInWorld(CyberCore::DOMWrapperWorld&) final;
    
    void dispatchGlobalObjectAvailable(CyberCore::DOMWrapperWorld&) final;
    void dispatchServiceWorkerGlobalObjectAvailable(CyberCore::DOMWrapperWorld&) final;
    void dispatchWillDisconnectDOMWindowExtensionFromGlobalObject(CyberCore::DOMWindowExtension*) final;
    void dispatchDidReconnectDOMWindowExtensionToGlobalObject(CyberCore::DOMWindowExtension*) final;
    void dispatchWillDestroyGlobalObjectForDOMWindowExtension(CyberCore::DOMWindowExtension*) final;

    void willInjectUserScript(CyberCore::DOMWrapperWorld&) final;

#if PLATFORM(COCOA)
    RemoteAXObjectRef accessibilityRemoteObject() final;
    
    void willCacheResponse(CyberCore::DocumentLoader*, CyberCore::ResourceLoaderIdentifier, NSCachedURLResponse*, CompletionHandler<void(NSCachedURLResponse *)>&&) const final;

    std::optional<double> dataDetectionReferenceDate() final;
#endif

    void didChangeScrollOffset() final;

    bool allowScript(bool enabledPerSettings) final;

    bool shouldForceUniversalAccessFromLocalURL(const URL&) final;

    Ref<CyberCore::FrameNetworkingContext> createNetworkingContext() final;

    void completePageTransitionIfNeeded() final;

#if USE(QUICK_LOOK)
    RefPtr<CyberCore::LegacyPreviewLoaderClient> createPreviewLoaderClient(const String& fileName, const String& uti) final;
#endif

#if ENABLE(CONTENT_FILTERING)
    void contentFilterDidBlockLoad(CyberCore::ContentFilterUnblockHandler) final;
#endif

    void prefetchDNS(const String&) final;
    void sendH2Ping(const URL&, CompletionHandler<void(Expected<WTF::Seconds, CyberCore::ResourceError>&&)>&&) final;

    void didRestoreScrollPosition() final;

    void getLoadDecisionForIcons(const Vector<std::pair<CyberCore::LinkIcon&, uint64_t>>&) final;

    inline bool hasPlugInView() const;

    Ref<WebFrame> m_frame;
    ScopeExit<Function<void()>> m_frameInvalidator;

#if ENABLE(PDFKIT_PLUGIN)
    RefPtr<PluginView> m_pluginView;
    bool m_hasSentResponseToPluginView { false };
#endif

    bool m_didCompletePageTransition { false };
    bool m_frameHasCustomContentProvider { false };
    bool m_frameCameFromBackForwardCache { false };
    bool m_useIconLoadingClient { false };
#if ENABLE(TRACKING_PREVENTION)
    std::optional<FrameSpecificStorageAccessIdentifier> m_frameSpecificStorageAccessIdentifier;
#endif

#if ENABLE(APP_BOUND_DOMAINS)
    bool shouldEnableInAppBrowserPrivacyProtections() const final;
    void notifyPageOfAppBoundBehavior() final;
#endif

#if ENABLE(PDFKIT_PLUGIN)
    bool shouldUsePDFPlugin(const String& contentType, StringView path) const final;
#endif

    bool isParentProcessAFullWebBrowser() const final;

#if ENABLE(ARKIT_INLINE_PREVIEW_MAC)
    void modelInlinePreviewUUIDs(CompletionHandler<void(Vector<String>)>&&) const final;
#endif
};

// As long as EmptyFrameLoaderClient exists in CyberCore, this can return nullptr.
inline WebFrameLoaderClient* toWebFrameLoaderClient(CyberCore::FrameLoaderClient& client)
{
    return client.isEmptyFrameLoaderClient() ? nullptr : static_cast<WebFrameLoaderClient*>(&client);
}

inline const WebFrameLoaderClient* toWebFrameLoaderClient(const CyberCore::FrameLoaderClient& client)
{
    return client.isEmptyFrameLoaderClient() ? nullptr : static_cast<const WebFrameLoaderClient*>(&client);
}

} // namespace CyberKit
