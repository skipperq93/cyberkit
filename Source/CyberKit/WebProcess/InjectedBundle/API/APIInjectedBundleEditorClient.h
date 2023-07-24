/*
 * Copyright (C) 2016 Apple Inc. All rights reserved.
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

#include <CyberCore/EditorInsertAction.h>
#include <CyberCore/TextAffinity.h>
#include <wtf/Forward.h>
#include <wtf/text/WTFString.h>

namespace CyberCore {
class DocumentFragment;
class Node;
class SharedBuffer;
class StyleProperties;
struct SimpleRange;
}

namespace CyberKit {
class WebPage;
}

namespace API {

namespace InjectedBundle {

class EditorClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    virtual ~EditorClient() { }

    virtual bool shouldBeginEditing(CyberKit::WebPage&, const CyberCore::SimpleRange&) { return true; }
    virtual bool shouldEndEditing(CyberKit::WebPage&, const CyberCore::SimpleRange&) { return true; }
    virtual bool shouldInsertNode(CyberKit::WebPage&, CyberCore::Node&, const Optional<CyberCore::SimpleRange>&, CyberCore::EditorInsertAction) { return true; }
    virtual bool shouldInsertText(CyberKit::WebPage&, const WTF::String&, const Optional<CyberCore::SimpleRange>&, CyberCore::EditorInsertAction) { return true; }
    virtual bool shouldDeleteRange(CyberKit::WebPage&, const Optional<CyberCore::SimpleRange>&) { return true; }
    virtual bool shouldChangeSelectedRange(CyberKit::WebPage&, const Optional<CyberCore::SimpleRange>&, const Optional<CyberCore::SimpleRange>&, CyberCore::EAffinity, bool) { return true; }
    virtual bool shouldApplyStyle(CyberKit::WebPage&, const CyberCore::StyleProperties&, const Optional<CyberCore::SimpleRange>&) { return true; }
    virtual void didBeginEditing(CyberKit::WebPage&, const WTF::String&) { }
    virtual void didEndEditing(CyberKit::WebPage&, const WTF::String&) { }
    virtual void didChange(CyberKit::WebPage&, const WTF::String&) { }
    virtual void didChangeSelection(CyberKit::WebPage&, const WTF::String&) { }
    virtual void willWriteToPasteboard(CyberKit::WebPage&, const Optional<CyberCore::SimpleRange>&) { }
    virtual void getPasteboardDataForRange(CyberKit::WebPage&, const Optional<CyberCore::SimpleRange>&, Vector<WTF::String>&, Vector<RefPtr<CyberCore::SharedBuffer>>&) { }
    virtual void didWriteToPasteboard(CyberKit::WebPage&) { }
    virtual bool performTwoStepDrop(CyberKit::WebPage&, CyberCore::DocumentFragment&, const CyberCore::SimpleRange&, bool) { return false; }
};

} // namespace InjectedBundle

}
