/*
 * Copyright (C) 2014, 2015 Apple Inc. All rights reserved.
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

#include "APIData.h"
#include "APIString.h"
#include "AuthenticationChallengeDisposition.h"
#include "AuthenticationChallengeProxy.h"
#include "AuthenticationDecisionListener.h"
#include "ProcessTerminationReason.h"
#include "SameDocumentNavigationType.h"
#include "WebFramePolicyListenerProxy.h"
#include "WebsitePoliciesData.h"
#include <CyberCore/FrameLoaderTypes.h>
#include <CyberCore/LayoutMilestone.h>
#include <wtf/Forward.h>

#if HAVE(APP_SSO)
#include "SOAuthorizationLoadPolicy.h"
#endif

namespace CyberCore {
struct ContentRuleListResults;
class ResourceError;
class ResourceRequest;
class ResourceResponse;
class FragmentedSharedBuffer;
class SecurityOriginData;
}

namespace CyberKit {
class AuthenticationChallengeProxy;
class DownloadProxy;
class WebBackForwardListItem;
class WebFramePolicyListenerProxy;
class WebFrameProxy;
class WebPageProxy;
class WebProtectionSpace;
struct FrameInfoData;
struct NavigationActionData;
struct WebNavigationDataStore;
}

namespace API {

class Dictionary;
class Navigation;
class NavigationAction;
class NavigationResponse;
class Object;

class NavigationClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    virtual ~NavigationClient() { }

    virtual void didStartProvisionalNavigation(CyberKit::WebPageProxy&, const CyberCore::ResourceRequest&, Navigation*, Object*) { }
    virtual void didStartProvisionalLoadForFrame(CyberKit::WebPageProxy&, CyberCore::ResourceRequest&&, CyberKit::FrameInfoData&&) { }
    virtual void didReceiveServerRedirectForProvisionalNavigation(CyberKit::WebPageProxy&, Navigation*, Object*) { }
    virtual void willPerformClientRedirect(CyberKit::WebPageProxy&, const WTF::String& destinationURL, double) { }
    virtual void didPerformClientRedirect(CyberKit::WebPageProxy&, const WTF::String& sourceURL, const WTF::String& destinationURL) { }
    virtual void didCancelClientRedirect(CyberKit::WebPageProxy&) { }
    virtual void didFailProvisionalNavigationWithError(CyberKit::WebPageProxy&, CyberKit::FrameInfoData&&, Navigation*, const CyberCore::ResourceError&, Object*) { }
    virtual void didFailProvisionalLoadWithErrorForFrame(CyberKit::WebPageProxy&, CyberCore::ResourceRequest&&, const CyberCore::ResourceError&, CyberKit::FrameInfoData&&) { }
    virtual void didCommitNavigation(CyberKit::WebPageProxy&, Navigation*, Object*) { }
    virtual void didCommitLoadForFrame(CyberKit::WebPageProxy&, CyberCore::ResourceRequest&&, CyberKit::FrameInfoData&&) { }
    virtual void didFinishDocumentLoad(CyberKit::WebPageProxy&, Navigation*, Object*) { }
    virtual void didFinishNavigation(CyberKit::WebPageProxy&, Navigation*, Object*) { }
    virtual void didFinishLoadForFrame(CyberKit::WebPageProxy&, CyberCore::ResourceRequest&&, CyberKit::FrameInfoData&&) { }
    virtual void didFailLoadDueToNetworkConnectionIntegrity(CyberKit::WebPageProxy&, const WTF::URL&) { }
    virtual void didFailNavigationWithError(CyberKit::WebPageProxy&, const CyberKit::FrameInfoData&, Navigation*, const CyberCore::ResourceError&, Object*) { }
    virtual void didFailLoadWithErrorForFrame(CyberKit::WebPageProxy&, CyberCore::ResourceRequest&&, const CyberCore::ResourceError&, CyberKit::FrameInfoData&&) { }
    virtual void didSameDocumentNavigation(CyberKit::WebPageProxy&, Navigation*, CyberKit::SameDocumentNavigationType, Object*) { }
    virtual void didChangeLookalikeCharacters(CyberKit::WebPageProxy&, const WTF::URL&, const WTF::URL&) { }

    virtual void didDisplayInsecureContent(CyberKit::WebPageProxy&, API::Object*) { }
    virtual void didRunInsecureContent(CyberKit::WebPageProxy&, API::Object*) { }

    virtual void renderingProgressDidChange(CyberKit::WebPageProxy&, OptionSet<CyberCore::LayoutMilestone>) { }

    virtual void navigationResponseDidBecomeDownload(CyberKit::WebPageProxy&, NavigationResponse&, CyberKit::DownloadProxy&) { }
    virtual void navigationActionDidBecomeDownload(CyberKit::WebPageProxy&, NavigationAction&, CyberKit::DownloadProxy&) { }
    virtual void contextMenuDidCreateDownload(CyberKit::WebPageProxy&, CyberKit::DownloadProxy&) { }

    virtual void didReceiveAuthenticationChallenge(CyberKit::WebPageProxy&, CyberKit::AuthenticationChallengeProxy& challenge) { challenge.listener().completeChallenge(CyberKit::AuthenticationChallengeDisposition::PerformDefaultHandling); }
    virtual void shouldAllowLegacyTLS(CyberKit::WebPageProxy&, CyberKit::AuthenticationChallengeProxy&, CompletionHandler<void(bool)>&& completionHandler) { completionHandler(true); }
    virtual void didNegotiateModernTLS(const WTF::URL&) { }
    virtual bool shouldBypassContentModeSafeguards() const { return false; }

    // FIXME: These function should not be part of this client.
    virtual bool processDidTerminate(CyberKit::WebPageProxy&, CyberKit::ProcessTerminationReason) { return false; }
    virtual void processDidBecomeResponsive(CyberKit::WebPageProxy&) { }
    virtual void processDidBecomeUnresponsive(CyberKit::WebPageProxy&) { }

    virtual RefPtr<Data> webCryptoMasterKey(CyberKit::WebPageProxy&) { return nullptr; }

#if USE(QUICK_LOOK)
    virtual void didStartLoadForQuickLookDocumentInMainFrame(const WTF::String& fileName, const WTF::String& uti) { }
    virtual void didFinishLoadForQuickLookDocumentInMainFrame(const CyberCore::FragmentedSharedBuffer&) { }
#endif

    virtual void decidePolicyForNavigationAction(CyberKit::WebPageProxy&, Ref<NavigationAction>&&, Ref<CyberKit::WebFramePolicyListenerProxy>&& listener)
    {
        listener->use();
    }

    virtual void decidePolicyForNavigationResponse(CyberKit::WebPageProxy&, Ref<NavigationResponse>&&, Ref<CyberKit::WebFramePolicyListenerProxy>&& listener)
    {
        listener->use();
    }
    
    virtual void contentRuleListNotification(CyberKit::WebPageProxy&, WTF::URL&&, CyberCore::ContentRuleListResults&&) { };

    virtual bool willGoToBackForwardListItem(CyberKit::WebPageProxy&, CyberKit::WebBackForwardListItem&, bool inBackForwardCache) { return false; }

    virtual void didBeginNavigationGesture(CyberKit::WebPageProxy&) { }
    virtual void willEndNavigationGesture(CyberKit::WebPageProxy&, bool willNavigate, CyberKit::WebBackForwardListItem&) { }
    virtual void didEndNavigationGesture(CyberKit::WebPageProxy&, bool willNavigate, CyberKit::WebBackForwardListItem&) { }
    virtual void didRemoveNavigationGestureSnapshot(CyberKit::WebPageProxy&) { }
    virtual bool didChangeBackForwardList(CyberKit::WebPageProxy&, CyberKit::WebBackForwardListItem*, const Vector<Ref<CyberKit::WebBackForwardListItem>>&) { return false; }

#if HAVE(APP_SSO)
    virtual void decidePolicyForSOAuthorizationLoad(CyberKit::WebPageProxy&, CyberKit::SOAuthorizationLoadPolicy currentSOAuthorizationLoadPolicy, const WTF::String&, CompletionHandler<void(CyberKit::SOAuthorizationLoadPolicy)>&& completionHandler)
    {
        completionHandler(currentSOAuthorizationLoadPolicy);
    }
#endif
};

} // namespace API
