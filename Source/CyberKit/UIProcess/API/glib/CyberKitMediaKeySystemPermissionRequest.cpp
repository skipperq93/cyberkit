/*
 * Copyright (C) 2021 Igalia S.L.
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
#include "CyberKitMediaKeySystemPermissionRequest.h"

#include "MediaKeySystemPermissionRequest.h"
#include "CyberKitMediaKeySystemPermissionRequestPrivate.h"
#include "CyberKitPermissionRequest.h"
#include <wtf/glib/WTFGType.h>

#if !ENABLE(2022_GLIB_API)
typedef CyberKitPermissionRequestIface CyberKitPermissionRequestInterface;
#endif

using namespace CyberKit;

/**
 * CyberKitMediaKeySystemPermissionRequest:
 * @See_also: #CyberKitPermissionRequest, #CyberKitWebView
 *
 * A permission request for using an EME Content Decryption Module.
 *
 * CyberKitMediaKeySystemPermissionRequest represents a request for permission to decide whether
 * CyberKit should use the given CDM to access protected media when requested through the
 * MediaKeySystem API.
 *
 * When a CyberKitMediaKeySystemPermissionRequest is not handled by the user,
 * it is denied by default.
 *
 * When handling this permission request the application may perform additional installation of the
 * requested CDM, unless it is already present on the host system.
 */

static void webkit_permission_request_interface_init(CyberKitPermissionRequestInterface*);

struct _CyberKitMediaKeySystemPermissionRequestPrivate {
    RefPtr<MediaKeySystemPermissionRequest> request;
    bool madeDecision;
    CString keySystem;
};

WEBKIT_DEFINE_FINAL_TYPE_WITH_CODE(
    CyberKitMediaKeySystemPermissionRequest, webkit_media_key_system_permission_request, G_TYPE_OBJECT, GObject,
    G_IMPLEMENT_INTERFACE(WEBKIT_TYPE_PERMISSION_REQUEST, webkit_permission_request_interface_init))

static void webkitMediaKeySystemPermissionRequestAllow(CyberKitPermissionRequest* request)
{
    ASSERT(WEBKIT_IS_MEDIA_KEY_SYSTEM_PERMISSION_REQUEST(request));

    CyberKitMediaKeySystemPermissionRequestPrivate* priv = WEBKIT_MEDIA_KEY_SYSTEM_PERMISSION_REQUEST(request)->priv;

    // Only one decision at a time.
    if (priv->madeDecision)
        return;

    priv->request->complete(true);
    priv->madeDecision = true;
}

static void webkitMediaKeySystemPermissionRequestDeny(CyberKitPermissionRequest* request)
{
    ASSERT(WEBKIT_IS_MEDIA_KEY_SYSTEM_PERMISSION_REQUEST(request));

    CyberKitMediaKeySystemPermissionRequestPrivate* priv = WEBKIT_MEDIA_KEY_SYSTEM_PERMISSION_REQUEST(request)->priv;

    // Only one decision at a time.
    if (priv->madeDecision)
        return;

    priv->request->complete(false);
    priv->madeDecision = true;
}

static void webkit_permission_request_interface_init(CyberKitPermissionRequestInterface* iface)
{
    iface->allow = webkitMediaKeySystemPermissionRequestAllow;
    iface->deny = webkitMediaKeySystemPermissionRequestDeny;
}

static void webkitMediaKeySystemPermissionRequestDispose(GObject* object)
{
    // Default behaviour when no decision has been made is denying the request.
    webkitMediaKeySystemPermissionRequestDeny(WEBKIT_PERMISSION_REQUEST(object));
    G_OBJECT_CLASS(webkit_media_key_system_permission_request_parent_class)->dispose(object);
}

static void webkit_media_key_system_permission_request_class_init(CyberKitMediaKeySystemPermissionRequestClass* klass)
{
    GObjectClass* objectClass = G_OBJECT_CLASS(klass);
    objectClass->dispose = webkitMediaKeySystemPermissionRequestDispose;
}

CyberKitMediaKeySystemPermissionRequest* webkitMediaKeySystemPermissionRequestCreate(Ref<MediaKeySystemPermissionRequest>&& request)
{
    CyberKitMediaKeySystemPermissionRequest* permissionRequest = WEBKIT_MEDIA_KEY_SYSTEM_PERMISSION_REQUEST(g_object_new(WEBKIT_TYPE_MEDIA_KEY_SYSTEM_PERMISSION_REQUEST, NULL));
    permissionRequest->priv->request = WTFMove(request);
    return permissionRequest;
}

/**
 * webkit_media_key_system_permission_get_name:
 * @request: a #CyberKitMediaKeySystemPermissionRequest
 *
 * Get the key system for which access permission is being requested.
 *
 * Returns: the key system name for @request
 *
 * Since: 2.32
 */
const gchar*
webkit_media_key_system_permission_get_name(CyberKitMediaKeySystemPermissionRequest* request)
{
    auto* priv = request->priv;
    if (priv->keySystem.isNull())
        priv->keySystem = priv->request->keySystem().utf8().data();
    return priv->keySystem.data();
}
