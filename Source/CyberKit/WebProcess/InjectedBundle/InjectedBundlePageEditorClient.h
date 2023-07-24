/*
 * Copyright (C) 2010, 2016 Apple Inc. All rights reserved.
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

#include "APIClient.h"
#include "APIInjectedBundleEditorClient.h"
#include "WKBundlePageEditorClient.h"

namespace API {
template<> struct ClientTraits<WKBundlePageEditorClientBase> {
    typedef std::tuple<WKBundlePageEditorClientV0, WKBundlePageEditorClientV1> Versions;
};
}

namespace CyberCore {
class CSSStyleDeclaration;
class DocumentFragment;
class Node;
struct SimpleRange;
}

namespace CyberKit {

class WebFrame;
class WebPage;

class InjectedBundlePageEditorClient final : public API::Client<WKBundlePageEditorClientBase>, public API::InjectedBundle::EditorClient {
public:
    explicit InjectedBundlePageEditorClient(const WKBundlePageEditorClientBase&);

private:
    bool shouldBeginEditing(WebPage&, const CyberCore::SimpleRange&) final;
    bool shouldEndEditing(WebPage&, const CyberCore::SimpleRange&) final;
    bool shouldInsertNode(WebPage&, CyberCore::Node&, const Optional<CyberCore::SimpleRange>& rangeToReplace, CyberCore::EditorInsertAction) final;
    bool shouldInsertText(WebPage&, const String&, const Optional<CyberCore::SimpleRange>& rangeToReplace, CyberCore::EditorInsertAction) final;
    bool shouldDeleteRange(WebPage&, const Optional<CyberCore::SimpleRange>&) final;
    bool shouldChangeSelectedRange(WebPage&, const Optional<CyberCore::SimpleRange>& fromRange, const Optional<CyberCore::SimpleRange>& toRange, CyberCore::EAffinity, bool stillSelecting) final;
    bool shouldApplyStyle(WebPage&, const CyberCore::StyleProperties&, const Optional<CyberCore::SimpleRange>&) final;
    void didBeginEditing(WebPage&, const String& notificationName) final;
    void didEndEditing(WebPage&, const String& notificationName) final;
    void didChange(WebPage&, const String& notificationName) final;
    void didChangeSelection(WebPage&, const String& notificationName) final;
    void willWriteToPasteboard(WebPage&, const Optional<CyberCore::SimpleRange>&) final;
    void getPasteboardDataForRange(WebPage&, const Optional<CyberCore::SimpleRange>&, Vector<String>& pasteboardTypes, Vector<RefPtr<CyberCore::SharedBuffer>>& pasteboardData) final;
    void didWriteToPasteboard(WebPage&) final;
    bool performTwoStepDrop(WebPage&, CyberCore::DocumentFragment&, const CyberCore::SimpleRange& destination, bool isMove) final;
};

} // namespace CyberKit
