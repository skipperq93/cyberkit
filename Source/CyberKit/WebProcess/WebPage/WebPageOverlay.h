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

#pragma once

#include "APIObject.h"
#include <CyberCore/FloatPoint.h>
#include <CyberCore/PageOverlay.h>
#include <CyberCore/SimpleRange.h>
#include <wtf/RetainPtr.h>

OBJC_CLASS DDActionContext;

namespace CyberCore {
class IntRect;
}

namespace CyberKit {

class WebFrame;
class WebPage;

class WebPageOverlay : public API::ObjectImpl<API::Object::Type::BundlePageOverlay>, private CyberCore::PageOverlay::Client {
public:
    struct ActionContext;

    class Client {
    public:
        virtual ~Client() { }

        virtual void willMoveToPage(WebPageOverlay&, WebPage*) = 0;
        virtual void didMoveToPage(WebPageOverlay&, WebPage*) = 0;
        virtual void drawRect(WebPageOverlay&, CyberCore::GraphicsContext&, const CyberCore::IntRect& dirtyRect) = 0;
        virtual bool mouseEvent(WebPageOverlay&, const CyberCore::PlatformMouseEvent&) = 0;
        virtual void didScrollFrame(WebPageOverlay&, WebFrame*) { }

#if PLATFORM(MAC)
        virtual std::optional<ActionContext> actionContextForResultAtPoint(WebPageOverlay&, CyberCore::FloatPoint) { return std::nullopt; }
        virtual void dataDetectorsDidPresentUI(WebPageOverlay&) { }
        virtual void dataDetectorsDidChangeUI(WebPageOverlay&) { }
        virtual void dataDetectorsDidHideUI(WebPageOverlay&) { }
#endif

        virtual bool copyAccessibilityAttributeStringValueForPoint(WebPageOverlay&, String /* attribute */, CyberCore::FloatPoint /* parameter */, String& /* value */) { return false; }
        virtual bool copyAccessibilityAttributeBoolValueForPoint(WebPageOverlay&, String /* attribute */, CyberCore::FloatPoint /* parameter */, bool& /* value */) { return false; }
        virtual Vector<String> copyAccessibilityAttributeNames(WebPageOverlay&, bool /* parameterizedNames */) { return Vector<String>(); }
    };

    static Ref<WebPageOverlay> create(std::unique_ptr<Client>, CyberCore::PageOverlay::OverlayType = CyberCore::PageOverlay::OverlayType::View);
    static WebPageOverlay* fromCoreOverlay(CyberCore::PageOverlay&);
    virtual ~WebPageOverlay();

    void setNeedsDisplay(const CyberCore::IntRect& dirtyRect);
    void setNeedsDisplay();

    void clear();

    CyberCore::PageOverlay* coreOverlay() const { return m_overlay.get(); }
    Client& client() const { return *m_client; }

#if PLATFORM(MAC)
    struct ActionContext {
        RetainPtr<DDActionContext> context;
        CyberCore::SimpleRange range;
    };
    std::optional<ActionContext> actionContextForResultAtPoint(CyberCore::FloatPoint);
    void dataDetectorsDidPresentUI();
    void dataDetectorsDidChangeUI();
    void dataDetectorsDidHideUI();
#endif

private:
    WebPageOverlay(std::unique_ptr<Client>, CyberCore::PageOverlay::OverlayType);

    // CyberCore::PageOverlay::Client
    void willMoveToPage(CyberCore::PageOverlay&, CyberCore::Page*) override;
    void didMoveToPage(CyberCore::PageOverlay&, CyberCore::Page*) override;
    void drawRect(CyberCore::PageOverlay&, CyberCore::GraphicsContext&, const CyberCore::IntRect& dirtyRect) override;
    bool mouseEvent(CyberCore::PageOverlay&, const CyberCore::PlatformMouseEvent&) override;
    void didScrollFrame(CyberCore::PageOverlay&, CyberCore::Frame&) override;

    bool copyAccessibilityAttributeStringValueForPoint(CyberCore::PageOverlay&, String /* attribute */, CyberCore::FloatPoint /* parameter */, String& value) override;
    bool copyAccessibilityAttributeBoolValueForPoint(CyberCore::PageOverlay&, String /* attribute */, CyberCore::FloatPoint /* parameter */, bool& value) override;
    Vector<String> copyAccessibilityAttributeNames(CyberCore::PageOverlay&, bool /* parameterizedNames */) override;

    RefPtr<CyberCore::PageOverlay> m_overlay;
    std::unique_ptr<Client> m_client;
};

} // namespace CyberKit
