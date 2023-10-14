/*
 * Copyright (C) 2021 Sony Interactive Entertainment Inc.
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

#include "config.h"
#include "WKView.h"

#include "APIClient.h"
#include "APIPageConfiguration.h"
#include "APIViewClient.h"
#include "PlayStationWebView.h"
#include "WKAPICast.h"
#include "WKSharedAPICast.h"
#include <CyberCore/Cursor.h>
#include <CyberCore/Region.h>

namespace API {
template<> struct ClientTraits<WKViewClientBase> {
    typedef std::tuple<WKViewClientV0> Versions;
};
}

WKCursorType toWKCursorType(const CyberCore::Cursor& cursor)
{
    switch (cursor.type()) {
    case CyberCore::Cursor::Hand:
        return kWKCursorTypeHand;
    case CyberCore::Cursor::None:
        return kWKCursorTypeNone;
    case CyberCore::Cursor::Pointer:
    default:
        return kWKCursorTypePointer;
    }
}

WKViewRef WKViewCreate(WKPageConfigurationRef configuration)
{
    return CyberKit::toAPI(CyberKit::PlayStationWebView::create(*CyberKit::toImpl(configuration)).leakRef());
}

WKPageRef WKViewGetPage(WKViewRef view)
{
    return CyberKit::toAPI(CyberKit::toImpl(view)->page());
}

void WKViewSetSize(WKViewRef view, WKSize viewSize)
{
    CyberKit::toImpl(view)->setViewSize(CyberKit::toIntSize(viewSize));
}

static void setViewActivityStateFlag(WKViewRef view, CyberCore::ActivityState flag, bool set)
{
    auto viewState = CyberKit::toImpl(view)->viewState();
    if (set)
        viewState.add(flag);
    else
        viewState.remove(flag);
    CyberKit::toImpl(view)->setViewState(viewState);
}

void WKViewSetFocus(WKViewRef view, bool focused)
{
    setViewActivityStateFlag(view, CyberCore::ActivityState::IsFocused, focused);
}

void WKViewSetActive(WKViewRef view, bool active)
{
    setViewActivityStateFlag(view, CyberCore::ActivityState::WindowIsActive, active);
}

void WKViewSetVisible(WKViewRef view, bool visible)
{
    setViewActivityStateFlag(view, CyberCore::ActivityState::IsVisible, visible);
}

void WKViewWillEnterFullScreen(WKViewRef view)
{
#if ENABLE(FULLSCREEN_API)
    CyberKit::toImpl(view)->willEnterFullScreen();
#endif
}

void WKViewDidEnterFullScreen(WKViewRef view)
{
#if ENABLE(FULLSCREEN_API)
    CyberKit::toImpl(view)->didEnterFullScreen();
#endif
}

void WKViewWillExitFullScreen(WKViewRef view)
{
#if ENABLE(FULLSCREEN_API)
    CyberKit::toImpl(view)->willExitFullScreen();
#endif
}

void WKViewDidExitFullScreen(WKViewRef view)
{
#if ENABLE(FULLSCREEN_API)
    CyberKit::toImpl(view)->didExitFullScreen();
#endif
}

void WKViewRequestExitFullScreen(WKViewRef view)
{
#if ENABLE(FULLSCREEN_API)
    CyberKit::toImpl(view)->requestExitFullScreen();
#endif
}

bool WKViewIsFullScreen(WKViewRef view)
{
#if ENABLE(FULLSCREEN_API)
    return CyberKit::toImpl(view)->isFullScreen();
#else
    return false;
#endif
}

void WKViewSetViewClient(WKViewRef view, const WKViewClientBase* client)
{
    class ViewClient final : public API::Client<WKViewClientBase>, public API::ViewClient {
    public:
        explicit ViewClient(const WKViewClientBase* client)
        {
            initialize(client);
        }

    private:
        void setViewNeedsDisplay(CyberKit::PlayStationWebView& view, const CyberCore::Region& region) final
        {
            if (!m_client.setViewNeedsDisplay)
                return;
            m_client.setViewNeedsDisplay(CyberKit::toAPI(&view), CyberKit::toAPI(region.bounds()), m_client.base.clientInfo);
        }

        void enterFullScreen(CyberKit::PlayStationWebView& view)
        {
            if (!m_client.enterFullScreen)
                return;
            m_client.enterFullScreen(CyberKit::toAPI(&view), m_client.base.clientInfo);
        }
        
        void exitFullScreen(CyberKit::PlayStationWebView& view)
        {
            if (!m_client.exitFullScreen)
                return;
            m_client.exitFullScreen(CyberKit::toAPI(&view), m_client.base.clientInfo);
        }
        
        void closeFullScreen(CyberKit::PlayStationWebView& view)
        {
            if (!m_client.closeFullScreen)
                return;
            m_client.closeFullScreen(CyberKit::toAPI(&view), m_client.base.clientInfo);
        }
        
        void beganEnterFullScreen(CyberKit::PlayStationWebView& view, const CyberCore::IntRect& initialFrame, const CyberCore::IntRect& finalFrame)
        {
            if (!m_client.beganEnterFullScreen)
                return;
            m_client.beganEnterFullScreen(CyberKit::toAPI(&view), CyberKit::toAPI(initialFrame), CyberKit::toAPI(finalFrame), m_client.base.clientInfo);
        }
        
        void beganExitFullScreen(CyberKit::PlayStationWebView& view, const CyberCore::IntRect& initialFrame, const CyberCore::IntRect& finalFrame)
        {
            if (!m_client.beganExitFullScreen)
                return;
            m_client.beganExitFullScreen(CyberKit::toAPI(&view), CyberKit::toAPI(initialFrame), CyberKit::toAPI(finalFrame), m_client.base.clientInfo);
        }

        void setCursor(CyberKit::PlayStationWebView& view, const CyberCore::Cursor& cursor) final
        {
            if (!m_client.setCursor)
                return;
            m_client.setCursor(CyberKit::toAPI(&view), toWKCursorType(cursor), m_client.base.clientInfo);
        }
    };

    CyberKit::toImpl(view)->setClient(makeUnique<ViewClient>(client));
}
