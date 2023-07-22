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
class CSSStyleDeclaration;
class DocumentFragment;
class Node;
class Range;
class SharedBuffer;
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

    virtual bool shouldBeginEditing(CyberKit::WebPage&, CyberCore::Range*) { return true; }
    virtual bool shouldEndEditing(CyberKit::WebPage&, CyberCore::Range*) { return true; }
    virtual bool shouldInsertNode(CyberKit::WebPage&, CyberCore::Node*, CyberCore::Range* rangeToReplace, CyberCore::EditorInsertAction) { return true; }
    virtual bool shouldInsertText(CyberKit::WebPage&, StringImpl*, CyberCore::Range* rangeToReplace, CyberCore::EditorInsertAction) { return true; }
    virtual bool shouldDeleteRange(CyberKit::WebPage&, CyberCore::Range*) { return true; }
    virtual bool shouldChangeSelectedRange(CyberKit::WebPage&, CyberCore::Range* fromRange, CyberCore::Range* toRange, CyberCore::EAffinity affinity, bool stillSelecting) { return true; }
    virtual bool shouldApplyStyle(CyberKit::WebPage&, CyberCore::CSSStyleDeclaration*, CyberCore::Range*) { return true; }
    virtual void didBeginEditing(CyberKit::WebPage&, StringImpl* notificationName) { }
    virtual void didEndEditing(CyberKit::WebPage&, StringImpl* notificationName) { }
    virtual void didChange(CyberKit::WebPage&, StringImpl* notificationName) { }
    virtual void didChangeSelection(CyberKit::WebPage&, StringImpl* notificationName) { }
    virtual void willWriteToPasteboard(CyberKit::WebPage&, CyberCore::Range*) { }
    virtual void getPasteboardDataForRange(CyberKit::WebPage&, CyberCore::Range*, Vector<WTF::String>& pasteboardTypes, Vector<RefPtr<CyberCore::SharedBuffer>>& pasteboardData) { }
    virtual void didWriteToPasteboard(CyberKit::WebPage&) { }
    virtual bool performTwoStepDrop(CyberKit::WebPage&, CyberCore::DocumentFragment&, CyberCore::Range&, bool) { return false; }
};

} // namespace InjectedBundle

}
