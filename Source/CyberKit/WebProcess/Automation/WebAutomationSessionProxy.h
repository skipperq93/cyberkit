/*
 * Copyright (C) 2016-2019 Apple Inc. All rights reserved.
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

#include "Connection.h"
#include "CoordinateSystem.h"
#include <CyberScriptCore/JSBase.h>
#include <CyberScriptCore/PrivateName.h>
#include <CyberCore/FrameIdentifier.h>
#include <CyberCore/IntRect.h>
#include <CyberCore/PageIdentifier.h>
#include <wtf/text/WTFString.h>

namespace CyberCore {
struct Cookie;
class Element;
}

namespace CyberKit {

class WebFrame;
class WebPage;
class WebAutomationDOMWindowObserver;

class WebAutomationSessionProxy : public IPC::MessageReceiver {
    WTF_MAKE_FAST_ALLOCATED;
public:
    WebAutomationSessionProxy(const String& sessionIdentifier);
    ~WebAutomationSessionProxy();

    String sessionIdentifier() const { return m_sessionIdentifier; }

    void didClearWindowObjectForFrame(WebFrame&);
    void willDestroyGlobalObjectForFrame(CyberCore::FrameIdentifier);

    void didEvaluateJavaScriptFunction(CyberCore::FrameIdentifier, uint64_t callbackID, const String& result, const String& errorType);

private:
    JSObjectRef scriptObject(JSGlobalContextRef);
    void setScriptObject(JSGlobalContextRef, JSObjectRef);
    JSObjectRef scriptObjectForFrame(WebFrame&);
    CyberCore::Element* elementForNodeHandle(WebFrame&, const String&);

    void ensureObserverForFrame(WebFrame&);

    // Implemented in generated WebAutomationSessionProxyMessageReceiver.cpp
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;

    // Called by WebAutomationSessionProxy messages
    void evaluateJavaScriptFunction(CyberCore::PageIdentifier, Optional<CyberCore::FrameIdentifier>, const String& function, Vector<String> arguments, bool expectsImplicitCallbackArgument, Optional<double> callbackTimeout, uint64_t callbackID);
    void resolveChildFrameWithOrdinal(CyberCore::PageIdentifier, Optional<CyberCore::FrameIdentifier>, uint32_t ordinal, CompletionHandler<void(Optional<String>, Optional<CyberCore::FrameIdentifier>)>&&);
    void resolveChildFrameWithNodeHandle(CyberCore::PageIdentifier, Optional<CyberCore::FrameIdentifier>, const String& nodeHandle, CompletionHandler<void(Optional<String>, Optional<CyberCore::FrameIdentifier>)>&&);
    void resolveChildFrameWithName(CyberCore::PageIdentifier, Optional<CyberCore::FrameIdentifier>, const String& name, CompletionHandler<void(Optional<String>, Optional<CyberCore::FrameIdentifier>)>&&);
    void resolveParentFrame(CyberCore::PageIdentifier, Optional<CyberCore::FrameIdentifier>, CompletionHandler<void(Optional<String>, Optional<CyberCore::FrameIdentifier>)>&&);
    void focusFrame(CyberCore::PageIdentifier, Optional<CyberCore::FrameIdentifier>);
    void computeElementLayout(CyberCore::PageIdentifier, Optional<CyberCore::FrameIdentifier>, String nodeHandle, bool scrollIntoViewIfNeeded, CoordinateSystem, CompletionHandler<void(Optional<String>, CyberCore::IntRect, Optional<CyberCore::IntPoint>, bool)>&&);
    void selectOptionElement(CyberCore::PageIdentifier, Optional<CyberCore::FrameIdentifier>, String nodeHandle, CompletionHandler<void(Optional<String>)>&&);
    void setFilesForInputFileUpload(CyberCore::PageIdentifier, Optional<CyberCore::FrameIdentifier>, String nodeHandle, Vector<String>&& filenames, CompletionHandler<void(Optional<String>)>&&);
    void takeScreenshot(CyberCore::PageIdentifier, Optional<CyberCore::FrameIdentifier>, String nodeHandle, bool scrollIntoViewIfNeeded, bool clipToViewport, uint64_t callbackID);
    void snapshotRectForScreenshot(CyberCore::PageIdentifier, Optional<CyberCore::FrameIdentifier>, String nodeHandle, bool scrollIntoViewIfNeeded, bool clipToViewport, CompletionHandler<void(Optional<String>, CyberCore::IntRect&&)>&&);
    void getCookiesForFrame(CyberCore::PageIdentifier, Optional<CyberCore::FrameIdentifier>, CompletionHandler<void(Optional<String>, Vector<CyberCore::Cookie>)>&&);
    void deleteCookie(CyberCore::PageIdentifier, Optional<CyberCore::FrameIdentifier>, String cookieName, CompletionHandler<void(Optional<String>)>&&);

    String m_sessionIdentifier;
    JSC::PrivateName m_scriptObjectIdentifier;

    HashMap<CyberCore::FrameIdentifier, Vector<uint64_t>> m_webFramePendingEvaluateJavaScriptCallbacksMap;
    HashMap<CyberCore::FrameIdentifier, RefPtr<WebAutomationDOMWindowObserver>> m_frameObservers;
};

} // namespace CyberKit
