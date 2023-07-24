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

#import <CyberCore/FrameLoaderClient.h>
#import <CyberCore/Timer.h>
#import <wtf/Forward.h>
#import <wtf/HashMap.h>
#import <wtf/RetainPtr.h>
#import <wtf/WeakObjCPtr.h>

@class WebDownload;
@class WebFrame;
@class WebFramePolicyListener;
@class WebHistoryItem;
@class WebResource;

namespace PAL {
class SessionID;
}

namespace CyberCore {
class AuthenticationChallenge;
class CachedFrame;
class HistoryItem;
class ProtectionSpace;
class ResourceLoader;
class ResourceRequest;
}

class WebFrameLoaderClient : public CyberCore::FrameLoaderClient {
public:
    explicit WebFrameLoaderClient(WebFrame* = nullptr);
    ~WebFrameLoaderClient();

    void setWebFrame(WebFrame& webFrame) { m_webFrame = &webFrame; }
    WebFrame* webFrame() const { return m_webFrame.get(); }

private:
    bool hasWebView() const final; // mainly for assertions

    Optional<CyberCore::PageIdentifier> pageID() const final;
    Optional<CyberCore::FrameIdentifier> frameID() const final;

    void makeRepresentation(CyberCore::DocumentLoader*) final;
    bool hasHTMLView() const final;
#if PLATFORM(IOS_FAMILY)
    bool forceLayoutOnRestoreFromBackForwardCache() final;
#endif
    void forceLayoutForNonHTML() final;

    void setCopiesOnScroll() final;

    void detachedFromParent2() final;
    void detachedFromParent3() final;

    void convertMainResourceLoadToDownload(CyberCore::DocumentLoader*, const CyberCore::ResourceRequest&, const CyberCore::ResourceResponse&) final;

    void assignIdentifierToInitialRequest(unsigned long identifier, CyberCore::DocumentLoader*, const CyberCore::ResourceRequest&) final;

    void dispatchWillSendRequest(CyberCore::DocumentLoader*, unsigned long identifier, CyberCore::ResourceRequest&, const CyberCore::ResourceResponse& redirectResponse) final;
    bool shouldUseCredentialStorage(CyberCore::DocumentLoader*, unsigned long identifier) final;
    void dispatchDidReceiveAuthenticationChallenge(CyberCore::DocumentLoader*, unsigned long identifier, const CyberCore::AuthenticationChallenge&) final;
#if USE(PROTECTION_SPACE_AUTH_CALLBACK)
    bool canAuthenticateAgainstProtectionSpace(CyberCore::DocumentLoader*, unsigned long identifier, const CyberCore::ProtectionSpace&) final;
#endif

#if PLATFORM(IOS_FAMILY)
    RetainPtr<CFDictionaryRef> connectionProperties(CyberCore::DocumentLoader*, unsigned long identifier) final;
#endif

    void dispatchDidReceiveResponse(CyberCore::DocumentLoader*, unsigned long identifier, const CyberCore::ResourceResponse&) final;
    void dispatchDidReceiveContentLength(CyberCore::DocumentLoader*, unsigned long identifier, int dataLength) final;
    void dispatchDidFinishLoading(CyberCore::DocumentLoader*, unsigned long identifier) final;
#if ENABLE(DATA_DETECTION)
    void dispatchDidFinishDataDetection(NSArray *detectionResults) final;
#endif
    void dispatchDidFailLoading(CyberCore::DocumentLoader*, unsigned long identifier, const CyberCore::ResourceError&) final;

    void willCacheResponse(CyberCore::DocumentLoader*, unsigned long identifier, NSCachedURLResponse*, CompletionHandler<void(NSCachedURLResponse *)>&&) const final;

    void dispatchDidDispatchOnloadEvents() final;
    void dispatchDidReceiveServerRedirectForProvisionalLoad() final;
    void dispatchDidCancelClientRedirect() final;
    void dispatchWillPerformClientRedirect(const URL&, double interval, WallTime fireDate, CyberCore::LockBackForwardList) final;
    void dispatchDidChangeLocationWithinPage() final;
    void dispatchDidPushStateWithinPage() final;
    void dispatchDidReplaceStateWithinPage() final;
    void dispatchDidPopStateWithinPage() final;
    
    void dispatchWillClose() final;
    void dispatchDidStartProvisionalLoad() final;
    void dispatchDidReceiveTitle(const CyberCore::StringWithDirection&) final;
    void dispatchDidCommitLoad(Optional<CyberCore::HasInsecureContent>, Optional<CyberCore::UsedLegacyTLS>) final;
    void dispatchDidFailProvisionalLoad(const CyberCore::ResourceError&, CyberCore::WillContinueLoading) final;
    void dispatchDidFailLoad(const CyberCore::ResourceError&) final;
    void dispatchDidFinishDocumentLoad() final;
    void dispatchDidFinishLoad() final;
    void dispatchDidReachLayoutMilestone(OptionSet<CyberCore::LayoutMilestone>) final;

    CyberCore::Frame* dispatchCreatePage(const CyberCore::NavigationAction&) final;
    void dispatchShow() final;

    void dispatchDecidePolicyForResponse(const CyberCore::ResourceResponse&, const CyberCore::ResourceRequest&, CyberCore::PolicyCheckIdentifier, const String&,  CyberCore::FramePolicyFunction&&) final;
    void dispatchDecidePolicyForNewWindowAction(const CyberCore::NavigationAction&, const CyberCore::ResourceRequest&, CyberCore::FormState*, const WTF::String& frameName, CyberCore::PolicyCheckIdentifier, CyberCore::FramePolicyFunction&&) final;
    void dispatchDecidePolicyForNavigationAction(const CyberCore::NavigationAction&, const CyberCore::ResourceRequest&, const CyberCore::ResourceResponse& redirectResponse, CyberCore::FormState*, CyberCore::PolicyDecisionMode, CyberCore::PolicyCheckIdentifier, CyberCore::FramePolicyFunction&&) final;
    void cancelPolicyCheck() final;

    void dispatchUnableToImplementPolicy(const CyberCore::ResourceError&) final;

    void dispatchWillSendSubmitEvent(Ref<CyberCore::FormState>&&) final;
    void dispatchWillSubmitForm(CyberCore::FormState&, CompletionHandler<void()>&&) final;

    void revertToProvisionalState(CyberCore::DocumentLoader*) final;
    void setMainDocumentError(CyberCore::DocumentLoader*, const CyberCore::ResourceError&) final;
    bool dispatchDidLoadResourceFromMemoryCache(CyberCore::DocumentLoader*, const CyberCore::ResourceRequest&, const CyberCore::ResourceResponse&, int length) final;

    void setMainFrameDocumentReady(bool) final;

    void startDownload(const CyberCore::ResourceRequest&, const String& suggestedName = String()) final;

    void willChangeTitle(CyberCore::DocumentLoader*) final;
    void didChangeTitle(CyberCore::DocumentLoader*) final;

    void willReplaceMultipartContent() final { }
    void didReplaceMultipartContent() final;

    void committedLoad(CyberCore::DocumentLoader*, const char*, int) final;
    void finishedLoading(CyberCore::DocumentLoader*) final;
    void updateGlobalHistory() final;
    void updateGlobalHistoryRedirectLinks() final;

    bool shouldGoToHistoryItem(CyberCore::HistoryItem&) const final;

    void didDisplayInsecureContent() final;
    void didRunInsecureContent(CyberCore::SecurityOrigin&, const URL&) final;
    void didDetectXSS(const URL&, bool didBlockEntirePage) final;

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
    CyberCore::ResourceError pluginWillHandleLoadError(const CyberCore::ResourceResponse&) const final;

    bool shouldFallBack(const CyberCore::ResourceError&) const final;

    WTF::String userAgent(const URL&) const final;
    
    void savePlatformDataToCachedFrame(CyberCore::CachedFrame*) final;
    void transitionToCommittedFromCachedFrame(CyberCore::CachedFrame*) final;
#if PLATFORM(IOS_FAMILY)
    void didRestoreFrameHierarchyForCachedFrame() final;
#endif
    void transitionToCommittedForNewPage() final;

    void didRestoreFromBackForwardCache() final;

    bool canHandleRequest(const CyberCore::ResourceRequest&) const final;
    bool canShowMIMEType(const WTF::String& MIMEType) const final;
    bool canShowMIMETypeAsHTML(const WTF::String& MIMEType) const final;
    bool representationExistsForURLScheme(const WTF::String& URLScheme) const final;
    WTF::String generatedMIMETypeForURLScheme(const WTF::String& URLScheme) const final;

    void frameLoadCompleted() final;
    void saveViewStateToItem(CyberCore::HistoryItem&) final;
    void restoreViewState() final;
    void provisionalLoadStarted() final;
    void didFinishLoad() final;
    void prepareForDataSourceReplacement() final;
    Ref<CyberCore::DocumentLoader> createDocumentLoader(const CyberCore::ResourceRequest&, const CyberCore::SubstituteData&) final;
    void updateCachedDocumentLoader(CyberCore::DocumentLoader&) final { }

    void setTitle(const CyberCore::StringWithDirection&, const URL&) final;

    RefPtr<CyberCore::Frame> createFrame(const WTF::String& name, CyberCore::HTMLFrameOwnerElement&) final;
    RefPtr<CyberCore::Widget> createPlugin(const CyberCore::IntSize&, CyberCore::HTMLPlugInElement&, const URL&,
    const Vector<WTF::String>&, const Vector<WTF::String>&, const WTF::String&, bool) final;
    void redirectDataToPlugin(CyberCore::Widget&) final;

#if ENABLE(WEBGL)
    CyberCore::WebGLLoadPolicy webGLPolicyForURL(const URL&) const final;
    CyberCore::WebGLLoadPolicy resolveWebGLPolicyForURL(const URL&) const final;
#endif

    RefPtr<CyberCore::Widget> createJavaAppletWidget(const CyberCore::IntSize&, CyberCore::HTMLAppletElement&, const URL& baseURL,
        const Vector<WTF::String>& paramNames, const Vector<WTF::String>& paramValues) final;
    
    CyberCore::ObjectContentType objectContentType(const URL&, const WTF::String& mimeType) final;
    WTF::String overrideMediaType() const final;
    
    void dispatchDidClearWindowObjectInWorld(CyberCore::DOMWrapperWorld&) final;

#if PLATFORM(IOS_FAMILY)
    bool shouldLoadMediaElementURL(const URL&) const final;
#endif

    RemoteAXObjectRef accessibilityRemoteObject() final { return 0; }
    
    RetainPtr<WebFramePolicyListener> setUpPolicyListener(CyberCore::PolicyCheckIdentifier, CyberCore::FramePolicyFunction&&, CyberCore::PolicyAction defaultPolicy, NSURL *appLinkURL = nil);

    NSDictionary *actionDictionary(const CyberCore::NavigationAction&, CyberCore::FormState*) const;
    
    bool canCachePage() const final;

    Ref<CyberCore::FrameNetworkingContext> createNetworkingContext() final;

    bool shouldPaintBrokenImage(const URL&) const final;

#if USE(QUICK_LOOK)
    RefPtr<CyberCore::LegacyPreviewLoaderClient> createPreviewLoaderClient(const String& fileName, const String& uti) final;
#endif

#if ENABLE(CONTENT_FILTERING)
    void contentFilterDidBlockLoad(CyberCore::ContentFilterUnblockHandler) final;
#endif

    void prefetchDNS(const String&) final;
    void sendH2Ping(const URL&, CompletionHandler<void(Expected<Seconds, CyberCore::ResourceError>&&)>&&) final;

    void getLoadDecisionForIcons(const Vector<std::pair<CyberCore::LinkIcon&, uint64_t>>&) final;
    void finishedLoadingIcon(uint64_t, CyberCore::SharedBuffer*) final;

    uint64_t m_activeIconLoadCallbackID { 0 };

    RetainPtr<WebFrame> m_webFrame;

    WeakObjCPtr<WebFramePolicyListener> m_policyListener;
};

WebDataSource *dataSource(CyberCore::DocumentLoader*);
void addTypesFromClass(NSMutableDictionary *, Class, NSArray *);
