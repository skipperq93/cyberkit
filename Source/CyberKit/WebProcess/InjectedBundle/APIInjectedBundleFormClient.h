/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
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

#ifndef APIInjectedBundleFormClient_h
#define APIInjectedBundleFormClient_h

#include <wtf/Forward.h>

namespace CyberCore {
class Element;
class HTMLFormElement;
class HTMLInputElement;
class HTMLTextAreaElement;
}

namespace CyberKit {
class WebFrame;
class WebPage;
}

namespace API {

class Object;

namespace InjectedBundle {

class FormClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    virtual ~FormClient() { }

    virtual void didFocusTextField(CyberKit::WebPage*, CyberCore::HTMLInputElement*, CyberKit::WebFrame*) { }
    virtual void textFieldDidBeginEditing(CyberKit::WebPage*, CyberCore::HTMLInputElement*, CyberKit::WebFrame*) { }
    virtual void textFieldDidEndEditing(CyberKit::WebPage*, CyberCore::HTMLInputElement*, CyberKit::WebFrame*) { }
    virtual void textDidChangeInTextField(CyberKit::WebPage*, CyberCore::HTMLInputElement*, CyberKit::WebFrame*, bool) { }
    virtual void textDidChangeInTextArea(CyberKit::WebPage*, CyberCore::HTMLTextAreaElement*, CyberKit::WebFrame*) { }

    enum class InputFieldAction {
        MoveUp,
        MoveDown,
        Cancel,
        InsertTab,
        InsertBacktab,
        InsertNewline,
        InsertDelete,
    };

    virtual bool shouldPerformActionInTextField(CyberKit::WebPage*, CyberCore::HTMLInputElement*, InputFieldAction, CyberKit::WebFrame*) { return false; }
    virtual void willSubmitForm(CyberKit::WebPage*, CyberCore::HTMLFormElement*, CyberKit::WebFrame*, CyberKit::WebFrame*, const Vector<std::pair<WTF::String, WTF::String>>&, RefPtr<API::Object>& userData) { UNUSED_PARAM(userData); }
    virtual void willSendSubmitEvent(CyberKit::WebPage*, CyberCore::HTMLFormElement*, CyberKit::WebFrame*, CyberKit::WebFrame*, const Vector<std::pair<WTF::String, WTF::String>>&) { }
    virtual void didAssociateFormControls(CyberKit::WebPage*, const Vector<RefPtr<CyberCore::Element>>&, CyberKit::WebFrame*) { }
    virtual bool shouldNotifyOnFormChanges(CyberKit::WebPage*) { return false; }
    virtual void willBeginInputSession(CyberKit::WebPage*, CyberCore::Element*, CyberKit::WebFrame*, bool userIsInteracting, RefPtr<API::Object>& userData) { UNUSED_PARAM(userData); }
};

} // namespace InjectedBundle

} // namespace API

#endif // APIInjectedBundleFormClient_h
