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
#include "CyberKitURISchemeRequest.h"

#include "APIData.h"
#include "CyberKitPrivate.h"
#include "CyberKitURISchemeRequestPrivate.h"
#include "CyberKitWebContextPrivate.h"
#include "CyberKitWebView.h"
#include "WebPageProxy.h"
#include <CyberCore/GUniquePtrSoup.h>
#include <CyberCore/ResourceError.h>
#include <CyberCore/URLSoup.h>
#include <libsoup/soup.h>
#include <wtf/glib/GRefPtr.h>
#include <wtf/glib/RunLoopSourcePriority.h>
#include <wtf/glib/WTFGType.h>
#include <wtf/text/CString.h>

using namespace CyberKit;
using namespace CyberCore;

/**
 * SECTION: CyberKitURISchemeRequest
 * @Short_description: Represents a URI scheme request
 * @Title: CyberKitURISchemeRequest
 *
 * If you register a particular URI scheme in a #CyberKitWebContext,
 * using webkit_web_context_register_uri_scheme(), you have to provide
 * a #CyberKitURISchemeRequestCallback. After that, when a URI request
 * is made with that particular scheme, your callback will be
 * called. There you will be able to access properties such as the
 * scheme, the URI and path, and the #CyberKitWebView that initiated the
 * request, and also finish the request with
 * webkit_uri_scheme_request_finish().
 *
 */

static const unsigned int gReadBufferSize = 8192;

struct _CyberKitURISchemeRequestPrivate {
    CyberKitWebContext* webContext;
    LegacyCustomProtocolManagerProxy* manager;
    RefPtr<WebPageProxy> initiatingPage;
    uint64_t requestID;
    CString uri;
    GUniquePtr<SoupURI> soupURI;

    GRefPtr<GInputStream> stream;
    uint64_t streamLength;
    GRefPtr<GCancellable> cancellable;
    char readBuffer[gReadBufferSize];
    uint64_t bytesRead;
    CString mimeType;
};

WEBKIT_DEFINE_TYPE(CyberKitURISchemeRequest, webkit_uri_scheme_request, G_TYPE_OBJECT)

static void webkit_uri_scheme_request_class_init(CyberKitURISchemeRequestClass*)
{
}

CyberKitURISchemeRequest* webkitURISchemeRequestCreate(uint64_t requestID, CyberKitWebContext* webContext, const ResourceRequest& resourceRequest, LegacyCustomProtocolManagerProxy& manager)
{
    CyberKitURISchemeRequest* request = WEBKIT_URI_SCHEME_REQUEST(g_object_new(WEBKIT_TYPE_URI_SCHEME_REQUEST, nullptr));
    request->priv->webContext = webContext;
    request->priv->manager = &manager;
    request->priv->uri = resourceRequest.url().string().utf8();
    request->priv->requestID = requestID;

    ASSERT(resourceRequest.initiatingPageID());
    request->priv->initiatingPage = WebProcessProxy::webPage(*resourceRequest.initiatingPageID());
    ASSERT(request->priv->initiatingPage);

    return request;
}

void webkitURISchemeRequestCancel(CyberKitURISchemeRequest* request)
{
    g_cancellable_cancel(request->priv->cancellable.get());
}

LegacyCustomProtocolManagerProxy* webkitURISchemeRequestGetManager(CyberKitURISchemeRequest* request)
{
    return request->priv->manager;
}

void webkitURISchemeRequestInvalidate(CyberKitURISchemeRequest* request)
{
    request->priv->manager = nullptr;
    webkitURISchemeRequestCancel(request);
}

/**
 * webkit_uri_scheme_request_get_scheme:
 * @request: a #CyberKitURISchemeRequest
 *
 * Get the URI scheme of @request
 *
 * Returns: the URI scheme of @request
 */
const char* webkit_uri_scheme_request_get_scheme(CyberKitURISchemeRequest* request)
{
    g_return_val_if_fail(WEBKIT_IS_URI_SCHEME_REQUEST(request), 0);

    if (!request->priv->soupURI)
        request->priv->soupURI.reset(soup_uri_new(request->priv->uri.data()));
    return request->priv->soupURI->scheme;
}

/**
 * webkit_uri_scheme_request_get_uri:
 * @request: a #CyberKitURISchemeRequest
 *
 * Get the URI of @request
 *
 * Returns: the full URI of @request
 */
const char* webkit_uri_scheme_request_get_uri(CyberKitURISchemeRequest* request)
{
    g_return_val_if_fail(WEBKIT_IS_URI_SCHEME_REQUEST(request), 0);

    return request->priv->uri.data();
}

/**
 * webkit_uri_scheme_request_get_path:
 * @request: a #CyberKitURISchemeRequest
 *
 * Get the URI path of @request
 *
 * Returns: the URI path of @request
 */
const char* webkit_uri_scheme_request_get_path(CyberKitURISchemeRequest* request)
{
    g_return_val_if_fail(WEBKIT_IS_URI_SCHEME_REQUEST(request), 0);

    if (!request->priv->soupURI)
        request->priv->soupURI.reset(soup_uri_new(request->priv->uri.data()));
    return request->priv->soupURI->path;
}

/**
 * webkit_uri_scheme_request_get_web_view:
 * @request: a #CyberKitURISchemeRequest
 *
 * Get the #CyberKitWebView that initiated the request.
 *
 * Returns: (transfer none): the #CyberKitWebView that initiated @request.
 */
CyberKitWebView* webkit_uri_scheme_request_get_web_view(CyberKitURISchemeRequest* request)
{
    g_return_val_if_fail(WEBKIT_IS_URI_SCHEME_REQUEST(request), 0);

    return webkitWebContextGetWebViewForPage(request->priv->webContext, request->priv->initiatingPage.get());
}

static void webkitURISchemeRequestReadCallback(GInputStream* inputStream, GAsyncResult* result, CyberKitURISchemeRequest* schemeRequest)
{
    GRefPtr<CyberKitURISchemeRequest> request = adoptGRef(schemeRequest);
    CyberKitURISchemeRequestPrivate* priv = request->priv;
    GUniqueOutPtr<GError> error;
    gssize bytesRead = g_input_stream_read_finish(inputStream, result, &error.outPtr());
    if (!priv->manager) {
        webkitWebContextDidFinishLoadingCustomProtocol(priv->webContext, priv->requestID);
        return;
    }

    if (bytesRead == -1) {
        webkit_uri_scheme_request_finish_error(request.get(), error.get());
        return;
    }

    // Need to check the stream before proceeding as it can be cancelled if finish_error
    // was previously call, which won't be detected by g_input_stream_read_finish().
    if (!request->priv->stream)
        return;

    auto webData = IPC::DataReference(reinterpret_cast<const uint8_t*>(priv->readBuffer), bytesRead);
    if (!priv->bytesRead) {
        // First chunk read. In case of empty reply an empty API::Data is sent to the networking process.
        ResourceResponse response(URL(URL(), String::fromUTF8(priv->uri)), String::fromUTF8(priv->mimeType.data()),
            priv->streamLength, emptyString());
        priv->manager->didReceiveResponse(priv->requestID, response, 0);
        priv->manager->didLoadData(priv->requestID, webData);
    } else if (bytesRead || (!bytesRead && !priv->streamLength)) {
        // Subsequent chunk read. We only send an empty API::Data to the networking process when stream length is unknown.
        priv->manager->didLoadData(priv->requestID, webData);
    }

    if (!bytesRead) {
        priv->manager->didFinishLoading(priv->requestID);
        webkitWebContextDidFinishLoadingCustomProtocol(priv->webContext, priv->requestID);
        return;
    }

    priv->bytesRead += bytesRead;
    g_input_stream_read_async(inputStream, priv->readBuffer, gReadBufferSize, RunLoopSourcePriority::AsyncIONetwork, priv->cancellable.get(),
        reinterpret_cast<GAsyncReadyCallback>(webkitURISchemeRequestReadCallback), g_object_ref(request.get()));
}

/**
 * webkit_uri_scheme_request_finish:
 * @request: a #CyberKitURISchemeRequest
 * @stream: a #GInputStream to read the contents of the request
 * @stream_length: the length of the stream or -1 if not known
 * @mime_type: (allow-none): the content type of the stream or %NULL if not known
 *
 * Finish a #CyberKitURISchemeRequest by setting the contents of the request and its mime type.
 */
void webkit_uri_scheme_request_finish(CyberKitURISchemeRequest* request, GInputStream* inputStream, gint64 streamLength, const gchar* mimeType)
{
    g_return_if_fail(WEBKIT_IS_URI_SCHEME_REQUEST(request));
    g_return_if_fail(G_IS_INPUT_STREAM(inputStream));
    g_return_if_fail(streamLength == -1 || streamLength >= 0);

    request->priv->stream = inputStream;
    // We use -1 in the API for consistency with soup when the content length is not known, but 0 internally.
    request->priv->streamLength = streamLength == -1 ? 0 : streamLength;
    request->priv->cancellable = adoptGRef(g_cancellable_new());
    request->priv->bytesRead = 0;
    request->priv->mimeType = mimeType;
    g_input_stream_read_async(inputStream, request->priv->readBuffer, gReadBufferSize, RunLoopSourcePriority::AsyncIONetwork, request->priv->cancellable.get(),
        reinterpret_cast<GAsyncReadyCallback>(webkitURISchemeRequestReadCallback), g_object_ref(request));
}

/**
 * webkit_uri_scheme_request_finish_error:
 * @request: a #CyberKitURISchemeRequest
 * @error: a #GError that will be passed to the #CyberKitWebView
 *
 * Finish a #CyberKitURISchemeRequest with a #GError.
 *
 * Since: 2.2
 */
void webkit_uri_scheme_request_finish_error(CyberKitURISchemeRequest* request, GError* error)
{
    g_return_if_fail(WEBKIT_IS_URI_SCHEME_REQUEST(request));
    g_return_if_fail(error);

    CyberKitURISchemeRequestPrivate* priv = request->priv;
    if (!webkitWebContextIsLoadingCustomProtocol(priv->webContext, priv->requestID))
        return;

    priv->stream = nullptr;
    ResourceError resourceError(g_quark_to_string(error->domain), toCyberCoreError(error->code), soupURIToURL(priv->soupURI.get()), String::fromUTF8(error->message));
    priv->manager->didFailWithError(priv->requestID, resourceError);
    webkitWebContextDidFinishLoadingCustomProtocol(priv->webContext, priv->requestID);
}
