/*
 * Copyright (C) 2022 Apple Inc. All rights reserved.
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

#if ENABLE(WK_WEB_EXTENSIONS)

#include "MessageReceiver.h"
#include "WebExtensionContextParameters.h"
#include "WebExtensionEventListenerType.h"
#include "WebPageProxyIdentifier.h"
#include <CyberCore/DOMWrapperWorld.h>
#include <CyberCore/FrameIdentifier.h>
#include <CyberCore/PageIdentifier.h>
#include <wtf/Forward.h>
#include <wtf/WeakHashSet.h>

namespace CyberKit {

class WebExtensionAPINamespace;
class WebExtensionMatchPattern;
class WebFrame;

class WebExtensionContextProxy final : public RefCounted<WebExtensionContextProxy>, public IPC::MessageReceiver {
    WTF_MAKE_FAST_ALLOCATED;
    WTF_MAKE_NONCOPYABLE(WebExtensionContextProxy);

public:
    static RefPtr<WebExtensionContextProxy> get(WebExtensionContextIdentifier);
    static Ref<WebExtensionContextProxy> getOrCreate(WebExtensionContextParameters);

    ~WebExtensionContextProxy();

    using WeakFrameSet = WeakHashSet<WebFrame>;

    WebExtensionContextIdentifier identifier() { return m_identifier; }

    bool operator==(const WebExtensionContextProxy& other) const { return (this == &other); }
    bool operator!=(const WebExtensionContextProxy& other) const { return !(this == &other); }

    const URL& baseURL() { return m_baseURL; }
    const String& uniqueIdentifier() const { return m_uniqueIdentifier; }

    NSDictionary *manifest() { return m_manifest.get(); }

    double manifestVersion() { return m_manifestVersion; }
    bool supportsManifestVersion(double version) { return manifestVersion() >= version; }

    bool inTestingMode() { return m_testingMode; }

    CyberCore::DOMWrapperWorld* contentScriptWorld() { return m_contentScriptWorld.get(); }
    void setContentScriptWorld(CyberCore::DOMWrapperWorld* world) { m_contentScriptWorld = world; }

    void addFrameWithExtensionContent(WebFrame&);

    void enumerateNamespaceObjects(const Function<void(WebExtensionAPINamespace&)>&, CyberCore::DOMWrapperWorld& = CyberCore::mainThreadNormalWorld());
    void enumerateContentScriptNamespaceObjects(const Function<void(WebExtensionAPINamespace&)>& function) { ASSERT(contentScriptWorld()); enumerateNamespaceObjects(function, *contentScriptWorld()); };

private:
    explicit WebExtensionContextProxy(WebExtensionContextParameters);

    // webNavigation support
    void dispatchWebNavigationOnBeforeNavigateEvent(WebPageProxyIdentifier, CyberCore::FrameIdentifier, URL);
    void dispatchWebNavigationOnCommittedEvent(WebPageProxyIdentifier, CyberCore::FrameIdentifier, URL);
    void dispatchWebNavigationOnDOMContentLoadedEvent(WebPageProxyIdentifier, CyberCore::FrameIdentifier, URL);
    void dispatchWebNavigationOnCompletedEvent(WebPageProxyIdentifier, CyberCore::FrameIdentifier, URL);
    void dispatchWebNavigationOnErrorOccurredEvent(WebPageProxyIdentifier, CyberCore::FrameIdentifier, URL);

    // Permissions support
    void dispatchPermissionsEvent(const CyberKit::WebExtensionEventListenerType&, HashSet<String> permissions, HashSet<String> origins);

    // IPC::MessageReceiver.
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;

    WebExtensionContextIdentifier m_identifier;
    URL m_baseURL;
    String m_uniqueIdentifier;
    RetainPtr<NSDictionary> m_manifest;
    double m_manifestVersion { 0 };
    bool m_testingMode { false };
    RefPtr<CyberCore::DOMWrapperWorld> m_contentScriptWorld;
    WeakFrameSet m_extensionContentFrames;
};

} // namespace CyberKit

#endif // ENABLE(WK_WEB_EXTENSIONS)
