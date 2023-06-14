/*
 * Copyright (C) 2012 Igalia S.L.
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
#include "CyberKitDownloadClient.h"

#include "APIDownloadClient.h"
#include "CyberKitDownloadPrivate.h"
#include "CyberKitURIResponsePrivate.h"
#include "CyberKitWebContextPrivate.h"
#include "CyberKitWebViewPrivate.h"
#include "WebProcessPool.h"
#include <wtf/glib/GRefPtr.h>
#include <wtf/text/CString.h>

using namespace CyberCore;
using namespace CyberKit;

class DownloadClient final : public API::DownloadClient {
public:
    explicit DownloadClient(CyberKitWebContext* webContext)
        : m_webContext(webContext)
    {
    }

private:
    void didStart(WebProcessPool&, DownloadProxy& downloadProxy) override
    {
        GRefPtr<CyberKitDownload> download = webkitWebContextGetOrCreateDownload(&downloadProxy);
        webkitDownloadStarted(download.get());
        webkitWebContextDownloadStarted(m_webContext, download.get());
    }

    void didReceiveAuthenticationChallenge(WebProcessPool&, DownloadProxy& downloadProxy, AuthenticationChallengeProxy& authenticationChallenge) override
    {
        GRefPtr<CyberKitDownload> download = webkitWebContextGetOrCreateDownload(&downloadProxy);
        if (webkitDownloadIsCancelled(download.get()))
            return;

        // FIXME: Add API to handle authentication of downloads without a web view associted.
        if (auto* webView = webkit_download_get_web_view(download.get()))
            webkitWebViewHandleAuthenticationChallenge(webView, &authenticationChallenge);
    }

    void didReceiveResponse(WebProcessPool&, DownloadProxy& downloadProxy, const ResourceResponse& resourceResponse) override
    {
        GRefPtr<CyberKitDownload> download = webkitWebContextGetOrCreateDownload(&downloadProxy);
        if (webkitDownloadIsCancelled(download.get()))
            return;

        GRefPtr<CyberKitURIResponse> response = adoptGRef(webkitURIResponseCreateForResourceResponse(resourceResponse));
        webkitDownloadSetResponse(download.get(), response.get());
    }

    void didReceiveData(WebProcessPool&, DownloadProxy& downloadProxy, uint64_t length) override
    {
        GRefPtr<CyberKitDownload> download = webkitWebContextGetOrCreateDownload(&downloadProxy);
        webkitDownloadNotifyProgress(download.get(), length);
    }

    void decideDestinationWithSuggestedFilename(WebProcessPool&, DownloadProxy& downloadProxy, const String& filename, Function<void(AllowOverwrite, String)>&& completionHandler) override
    {
        GRefPtr<CyberKitDownload> download = webkitWebContextGetOrCreateDownload(&downloadProxy);
        bool allowOverwrite = false;
        String destination = webkitDownloadDecideDestinationWithSuggestedFilename(download.get(), filename.utf8(), allowOverwrite);
        completionHandler(allowOverwrite ? AllowOverwrite::Yes : AllowOverwrite::No, destination);
    }

    void didCreateDestination(WebProcessPool&, DownloadProxy& downloadProxy, const String& path) override
    {
        GRefPtr<CyberKitDownload> download = webkitWebContextGetOrCreateDownload(&downloadProxy);
        webkitDownloadDestinationCreated(download.get(), path);
    }

    void didFail(WebProcessPool&, DownloadProxy& downloadProxy, const ResourceError& error) override
    {
        GRefPtr<CyberKitDownload> download = webkitWebContextGetOrCreateDownload(&downloadProxy);
        if (webkitDownloadIsCancelled(download.get())) {
            // Cancellation takes precedence over other errors.
            webkitDownloadCancelled(download.get());
        } else
            webkitDownloadFailed(download.get(), error);
        webkitWebContextRemoveDownload(&downloadProxy);
    }

    void didCancel(WebProcessPool&, DownloadProxy& downloadProxy) override
    {
        GRefPtr<CyberKitDownload> download = webkitWebContextGetOrCreateDownload(&downloadProxy);
        webkitDownloadCancelled(download.get());
        webkitWebContextRemoveDownload(&downloadProxy);
    }

    void didFinish(WebProcessPool&, DownloadProxy& downloadProxy) override
    {
        GRefPtr<CyberKitDownload> download = webkitWebContextGetOrCreateDownload(&downloadProxy);
        webkitDownloadFinished(download.get());
        webkitWebContextRemoveDownload(&downloadProxy);
    }

    CyberKitWebContext* m_webContext;
};

void attachDownloadClientToContext(CyberKitWebContext* webContext)
{
    webkitWebContextGetProcessPool(webContext).setDownloadClient(std::make_unique<DownloadClient>(webContext));
}
