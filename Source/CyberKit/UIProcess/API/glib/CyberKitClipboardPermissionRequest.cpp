/*
 * Copyright (C) 2023 Igalia S.L.
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
#include "CyberKitClipboardPermissionRequest.h"

#include "CyberKitClipboardPermissionRequestPrivate.h"
#include "CyberKitPermissionRequest.h"
#include <wtf/glib/WTFGType.h>

#if !ENABLE(2022_GLIB_API)
typedef CyberKitPermissionRequestIface CyberKitPermissionRequestInterface;
#endif

/**
 * CyberKitClipboardPermissionRequest:
 * @See_also: #CyberKitPermissionRequest, #CyberKitWebView
 *
 * A permission request for reading clipboard contents.
 *
 * CyberKitClipboardPermissionRequest represents a request for
 * permission to decide whether CyberKit can access the clipboard to read
 * its contents through the Async Clipboard API.
 *
 * When a CyberKitClipboardPermissionRequest is not handled by the user,
 * it is denied by default.
 *
 * Since: 2.42
 */

static void webkit_permission_request_interface_init(CyberKitPermissionRequestInterface*);

struct _CyberKitClipboardPermissionRequestPrivate {
    CompletionHandler<void(CyberCore::DOMPasteAccessResponse)> completionHandler;
};

WEBKIT_DEFINE_FINAL_TYPE_WITH_CODE(
    CyberKitClipboardPermissionRequest, webkit_clipboard_permission_request, G_TYPE_OBJECT, GObject,
    G_IMPLEMENT_INTERFACE(WEBKIT_TYPE_PERMISSION_REQUEST, webkit_permission_request_interface_init))

static void webkitClipboardPermissionRequestAllow(CyberKitPermissionRequest* request)
{
    ASSERT(WEBKIT_IS_CLIPBOARD_PERMISSION_REQUEST(request));

    CyberKitClipboardPermissionRequestPrivate* priv = WEBKIT_CLIPBOARD_PERMISSION_REQUEST(request)->priv;

    if (priv->completionHandler)
        priv->completionHandler(CyberCore::DOMPasteAccessResponse::GrantedForGesture);
}

static void webkitClipboardPermissionRequestDeny(CyberKitPermissionRequest* request)
{
    ASSERT(WEBKIT_IS_CLIPBOARD_PERMISSION_REQUEST(request));

    CyberKitClipboardPermissionRequestPrivate* priv = WEBKIT_CLIPBOARD_PERMISSION_REQUEST(request)->priv;

    if (priv->completionHandler)
        priv->completionHandler(CyberCore::DOMPasteAccessResponse::DeniedForGesture);
}

static void webkit_permission_request_interface_init(CyberKitPermissionRequestInterface* iface)
{
    iface->allow = webkitClipboardPermissionRequestAllow;
    iface->deny = webkitClipboardPermissionRequestDeny;
}

static void webkitClipboardPermissionRequestDispose(GObject* object)
{
    // Default behaviour when no decision has been made is denying the request.
    webkitClipboardPermissionRequestDeny(WEBKIT_PERMISSION_REQUEST(object));
    G_OBJECT_CLASS(webkit_clipboard_permission_request_parent_class)->dispose(object);
}

static void webkit_clipboard_permission_request_class_init(CyberKitClipboardPermissionRequestClass* klass)
{
    GObjectClass* objectClass = G_OBJECT_CLASS(klass);
    objectClass->dispose = webkitClipboardPermissionRequestDispose;
}

CyberKitClipboardPermissionRequest* webkitClipboardPermissionRequestCreate(CompletionHandler<void(CyberCore::DOMPasteAccessResponse)>&& completionHandler)
{
    CyberKitClipboardPermissionRequest* clipboardPermissionRequest = WEBKIT_CLIPBOARD_PERMISSION_REQUEST(g_object_new(WEBKIT_TYPE_CLIPBOARD_PERMISSION_REQUEST, nullptr));
    clipboardPermissionRequest->priv->completionHandler = WTFMove(completionHandler);
    return clipboardPermissionRequest;
}
