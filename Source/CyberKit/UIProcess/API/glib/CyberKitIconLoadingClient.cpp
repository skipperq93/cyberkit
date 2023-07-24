/*
 * Copyright (C) 2017 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "CyberKitIconLoadingClient.h"

#include "APIIconLoadingClient.h"
#include "CyberKitWebViewPrivate.h"

using namespace CyberKit;

class IconLoadingClient : public API::IconLoadingClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    explicit IconLoadingClient(CyberKitWebView* webView)
        : m_webView(webView)
    {
    }

private:
    void getLoadDecisionForIcon(const CyberCore::LinkIcon& icon, CompletionHandler<void(Function<void(API::Data*, CallbackBase::Error)>&&)>&& completionHandler) override
    {
        // CyberCore can send non HTTP icons.
        if (!icon.url.protocolIsInHTTPFamily()) {
            completionHandler(nullptr);
            return;
        }

        CyberCore::LinkIcon copiedIcon = icon;
        webkitWebViewGetLoadDecisionForIcon(m_webView, icon, [protectedWebView = GRefPtr<CyberKitWebView>(m_webView), icon = WTFMove(copiedIcon), completionHandler = WTFMove(completionHandler)] (bool loadIcon) mutable {
            if (!loadIcon) {
                completionHandler(nullptr);
                return;
            }

            completionHandler([protectedWebView = WTFMove(protectedWebView), icon = WTFMove(icon)] (API::Data* iconData, CallbackBase::Error error) {
                if (error != CallbackBase::Error::None || !iconData)
                    return;
                webkitWebViewSetIcon(protectedWebView.get(), icon, *iconData);
            });
        });
    }

    CyberKitWebView* m_webView;
};

void attachIconLoadingClientToView(CyberKitWebView* webView)
{
    webkitWebViewGetPage(webView).setIconLoadingClient(makeUnique<IconLoadingClient>(webView));
}
