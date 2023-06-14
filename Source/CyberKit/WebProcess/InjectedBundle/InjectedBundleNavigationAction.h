/*
 * Copyright (C) 2011 Apple Inc. All rights reserved.
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

#ifndef InjectedBundleNavigationAction_h
#define InjectedBundleNavigationAction_h

#include "APIObject.h"
#include "InjectedBundleHitTestResult.h"
#include "InjectedBundleNodeHandle.h"
#include "WebEvent.h"
#include <CyberCore/FrameLoaderTypes.h>
#include <wtf/Ref.h>
#include <wtf/RefPtr.h>

namespace CyberCore {
    class FormState;
    class NavigationAction;
}

namespace CyberKit {

class WebFrame;

class InjectedBundleNavigationAction : public API::ObjectImpl<API::Object::Type::BundleNavigationAction> {
public:
    static Ref<InjectedBundleNavigationAction> create(WebFrame*, const CyberCore::NavigationAction&, RefPtr<CyberCore::FormState>&&);

    static WebEvent::Modifiers modifiersForNavigationAction(const CyberCore::NavigationAction&);
    static WebMouseEvent::Button mouseButtonForNavigationAction(const CyberCore::NavigationAction&);
    static WebMouseEvent::SyntheticClickType syntheticClickTypeForNavigationAction(const CyberCore::NavigationAction&);
    static CyberCore::FloatPoint clickLocationInRootViewCoordinatesForNavigationAction(const CyberCore::NavigationAction&);
    
    CyberCore::NavigationType navigationType() const { return m_navigationType; }
    WebEvent::Modifiers modifiers() const { return m_modifiers; }
    WebMouseEvent::Button mouseButton() const { return m_mouseButton; }
    InjectedBundleHitTestResult* hitTestResult() const { return m_hitTestResult.get(); }
    InjectedBundleNodeHandle* formElement() const { return m_formElement.get(); }
    WebMouseEvent::SyntheticClickType syntheticClickType() const { return m_syntheticClickType; }
    CyberCore::FloatPoint clickLocationInRootViewCoordinates() const { return m_clickLocationInRootViewCoordinates; }

    bool shouldOpenExternalURLs() const { return m_shouldOpenExternalURLs; }
    bool shouldTryAppLinks() const { return m_shouldTryAppLinks; }
    AtomicString downloadAttribute() const { return m_downloadAttribute; }

private:
    InjectedBundleNavigationAction(WebFrame*, const CyberCore::NavigationAction&, RefPtr<CyberCore::FormState>&&);

    CyberCore::NavigationType m_navigationType;
    WebEvent::Modifiers m_modifiers;
    WebMouseEvent::Button m_mouseButton;
    WebMouseEvent::SyntheticClickType m_syntheticClickType { WebMouseEvent::NoTap };
    CyberCore::FloatPoint m_clickLocationInRootViewCoordinates;
    RefPtr<InjectedBundleHitTestResult> m_hitTestResult;
    RefPtr<InjectedBundleNodeHandle> m_formElement;
    AtomicString m_downloadAttribute;
    bool m_shouldOpenExternalURLs;
    bool m_shouldTryAppLinks;
};

} // namespace CyberKit

#endif // InjectedBundleNavigationAction_h
