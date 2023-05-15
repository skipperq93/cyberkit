/*
 * Copyright (C) 2017 Igalia S.L.
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
#include <CyberCore/LayoutMilestone.h>
#include <wtf/Forward.h>
#include <wtf/WallTime.h>
#include <wtf/text/WTFString.h>

namespace CyberCore {
class DOMWindowExtension;
class DOMWrapperWorld;
class ResourceError;
class ResourceRequest;
class FragmentedSharedBuffer;
}

namespace CyberKit {
class InjectedBundleBackForwardListItem;
class WebFrame;
class WebPage;
}

namespace API {
class Object;

namespace InjectedBundle {

class PageLoaderClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    virtual ~PageLoaderClient() = default;

    virtual void willLoadURLRequest(CyberKit::WebPage&, const CyberCore::ResourceRequest&, API::Object*) { }
    virtual void willLoadDataRequest(CyberKit::WebPage&, const CyberCore::ResourceRequest&, RefPtr<CyberCore::FragmentedSharedBuffer>, const WTF::String&, const WTF::String&, const WTF::URL&, API::Object*) { }

    virtual void didStartProvisionalLoadForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, RefPtr<API::Object>&) { }
    virtual void didReceiveServerRedirectForProvisionalLoadForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, RefPtr<API::Object>&) { }
    virtual void didFailProvisionalLoadWithErrorForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, const CyberCore::ResourceError&, RefPtr<API::Object>&) { }
    virtual void didCommitLoadForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, RefPtr<API::Object>&) { }
    virtual void didFinishDocumentLoadForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, RefPtr<API::Object>&) { }
    virtual void didFinishLoadForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, RefPtr<API::Object>&) { }
    virtual void didFinishProgress(CyberKit::WebPage&) { }
    virtual void didFailLoadWithErrorForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, const CyberCore::ResourceError&, RefPtr<API::Object>&) { }
    virtual void didSameDocumentNavigationForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, CyberKit::SameDocumentNavigationType, RefPtr<API::Object>&) { }
    virtual void didReceiveTitleForFrame(CyberKit::WebPage&, const WTF::String&, CyberKit::WebFrame&, RefPtr<API::Object>&) { }
    virtual void didRemoveFrameFromHierarchy(CyberKit::WebPage&, CyberKit::WebFrame&, RefPtr<API::Object>&) { }
    virtual void didDisplayInsecureContentForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, RefPtr<API::Object>&) { }
    virtual void didRunInsecureContentForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, RefPtr<API::Object>&) { }

    virtual void didFirstLayoutForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, RefPtr<API::Object>&) { }
    virtual void didFirstVisuallyNonEmptyLayoutForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, RefPtr<API::Object>&) { }
    virtual void didLayoutForFrame(CyberKit::WebPage&, CyberKit::WebFrame&) { }
    virtual void didReachLayoutMilestone(CyberKit::WebPage&, OptionSet<CyberCore::LayoutMilestone>, RefPtr<API::Object>&) { }

    virtual void didClearWindowObjectForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, CyberCore::DOMWrapperWorld&) { }
    virtual void didCancelClientRedirectForFrame(CyberKit::WebPage&, CyberKit::WebFrame&) { }
    virtual void willPerformClientRedirectForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, const WTF::String&, double /*delay*/, WallTime /*date*/) { }
    virtual void didHandleOnloadEventsForFrame(CyberKit::WebPage&, CyberKit::WebFrame&) { }

    virtual void globalObjectIsAvailableForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, CyberCore::DOMWrapperWorld&) { }
    virtual void serviceWorkerGlobalObjectIsAvailableForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, CyberCore::DOMWrapperWorld&) { }
    virtual void willDisconnectDOMWindowExtensionFromGlobalObject(CyberKit::WebPage&, CyberCore::DOMWindowExtension*) { }
    virtual void didReconnectDOMWindowExtensionToGlobalObject(CyberKit::WebPage&, CyberCore::DOMWindowExtension*) { }
    virtual void willDestroyGlobalObjectForDOMWindowExtension(CyberKit::WebPage&, CyberCore::DOMWindowExtension*) { }

    virtual void willInjectUserScriptForFrame(CyberKit::WebPage&, CyberKit::WebFrame&, CyberCore::DOMWrapperWorld&) { }

    virtual bool shouldForceUniversalAccessFromLocalURL(CyberKit::WebPage&, const WTF::String&) { return false; }

    virtual void featuresUsedInPage(CyberKit::WebPage&, const Vector<WTF::String>&) { }

    virtual void willDestroyFrame(CyberKit::WebPage&, CyberKit::WebFrame&) { }

    virtual OptionSet<CyberCore::LayoutMilestone> layoutMilestones() const { return { }; }
};

} // namespace InjectedBundle

} // namespace API
