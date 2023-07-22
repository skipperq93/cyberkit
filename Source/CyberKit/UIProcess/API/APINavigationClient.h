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
#include "PluginModuleInfo.h"
#include "ProcessTerminationReason.h"
#include "SameDocumentNavigationType.h"
#include "WebEvent.h"
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
class SharedBuffer;
struct SecurityOriginData;
}

namespace CyberKit {
class AuthenticationChallengeProxy;
class WebBackForwardListItem;
class WebFramePolicyListenerProxy;
class WebFrameProxy;
class WebPageProxy;
class WebProtectionSpace;
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

    virtual void didStartProvisionalNavigation(CyberKit::WebPageProxy&, Navigation*, Object*) { }
    virtual void didReceiveServerRedirectForProvisionalNavigation(CyberKit::WebPageProxy&, Navigation*, Object*) { }
    virtual void willPerformClientRedirect(CyberKit::WebPageProxy&, const WTF::String& destinationURL, double) { }
    virtual void didPerformClientRedirect(CyberKit::WebPageProxy&, const WTF::String& sourceURL, const WTF::String& destinationURL) { }
    virtual void didCancelClientRedirect(CyberKit::WebPageProxy&) { }
    virtual void didFailProvisionalNavigationWithError(CyberKit::WebPageProxy&, CyberKit::WebFrameProxy&, Navigation*, const CyberCore::ResourceError&, Object*) { }
    virtual void didFailProvisionalLoadInSubframeWithError(CyberKit::WebPageProxy&, CyberKit::WebFrameProxy&, CyberCore::SecurityOriginData&&, Navigation*, const CyberCore::ResourceError&, Object*) { }
    virtual void didCommitNavigation(CyberKit::WebPageProxy&, Navigation*, Object*) { }
    virtual void didFinishDocumentLoad(CyberKit::WebPageProxy&, Navigation*, Object*) { }
    virtual void didFinishNavigation(CyberKit::WebPageProxy&, Navigation*, Object*) { }
    virtual void didFailNavigationWithError(CyberKit::WebPageProxy&, CyberKit::WebFrameProxy&, Navigation*, const CyberCore::ResourceError&, Object*) { }
    virtual void didSameDocumentNavigation(CyberKit::WebPageProxy&, Navigation*, CyberKit::SameDocumentNavigationType, Object*) { }

    virtual void didDisplayInsecureContent(CyberKit::WebPageProxy&, API::Object*) { }
    virtual void didRunInsecureContent(CyberKit::WebPageProxy&, API::Object*) { }

    virtual void renderingProgressDidChange(CyberKit::WebPageProxy&, OptionSet<CyberCore::LayoutMilestone>) { }

    virtual void didReceiveAuthenticationChallenge(CyberKit::WebPageProxy&, CyberKit::AuthenticationChallengeProxy& challenge) { challenge.listener().completeChallenge(CyberKit::AuthenticationChallengeDisposition::PerformDefaultHandling); }
    virtual void shouldAllowLegacyTLS(CyberKit::WebPageProxy&, CyberKit::AuthenticationChallengeProxy&, CompletionHandler<void(bool)>&& completionHandler) { completionHandler(true); }
    virtual bool shouldBypassContentModeSafeguards() const { return false; }

    // FIXME: These function should not be part of this client.
    virtual bool processDidTerminate(CyberKit::WebPageProxy&, CyberKit::ProcessTerminationReason) { return false; }
    virtual void processDidBecomeResponsive(CyberKit::WebPageProxy&) { }
    virtual void processDidBecomeUnresponsive(CyberKit::WebPageProxy&) { }

    virtual RefPtr<Data> webCryptoMasterKey(CyberKit::WebPageProxy&) { return nullptr; }

    virtual RefPtr<String> signedPublicKeyAndChallengeString(CyberKit::WebPageProxy&, unsigned keySizeIndex, const RefPtr<String>& challengeString, const WTF::URL&) { return nullptr; }

#if USE(QUICK_LOOK)
    virtual void didStartLoadForQuickLookDocumentInMainFrame(const WTF::String& fileName, const WTF::String& uti) { }
    virtual void didFinishLoadForQuickLookDocumentInMainFrame(const CyberCore::SharedBuffer&) { }
#endif

    virtual void decidePolicyForNavigationAction(CyberKit::WebPageProxy&, Ref<NavigationAction>&&, Ref<CyberKit::WebFramePolicyListenerProxy>&& listener, Object*)
    {
        listener->use();
    }

    virtual void decidePolicyForNavigationResponse(CyberKit::WebPageProxy&, Ref<NavigationResponse>&&, Ref<CyberKit::WebFramePolicyListenerProxy>&& listener, Object*)
    {
        listener->use();
    }
    
    virtual void contentRuleListNotification(CyberKit::WebPageProxy&, WTF::URL&&, CyberCore::ContentRuleListResults&&) { };
    
#if ENABLE(NETSCAPE_PLUGIN_API)
    virtual bool didFailToInitializePlugIn(CyberKit::WebPageProxy&, API::Dictionary&) { return false; }
    virtual bool didBlockInsecurePluginVersion(CyberKit::WebPageProxy&, API::Dictionary&) { return false; }
    virtual void decidePolicyForPluginLoad(CyberKit::WebPageProxy&, CyberKit::PluginModuleLoadPolicy currentPluginLoadPolicy, Dictionary&, CompletionHandler<void(CyberKit::PluginModuleLoadPolicy, const WTF::String&)>&& completionHandler)
    {
        completionHandler(currentPluginLoadPolicy, { });
    }
#endif

#if ENABLE(WEBGL)
    virtual void webGLLoadPolicy(CyberKit::WebPageProxy&, const WTF::URL&, CompletionHandler<void(CyberCore::WebGLLoadPolicy)>&& completionHandler) const { completionHandler(CyberCore::WebGLLoadPolicy::WebGLAllowCreation); }
    virtual void resolveWebGLLoadPolicy(CyberKit::WebPageProxy&, const WTF::URL&, CompletionHandler<void(CyberCore::WebGLLoadPolicy)>&& completionHandler) const { completionHandler(CyberCore::WebGLLoadPolicy::WebGLAllowCreation); }
#endif
    
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
