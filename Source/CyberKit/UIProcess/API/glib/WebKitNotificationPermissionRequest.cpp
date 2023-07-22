/*
 * Copyright (C) 2013 Igalia S.L.
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
#include "CyberKitNotificationPermissionRequest.h"

#include "NotificationPermissionRequest.h"
#include "CyberKitNotificationPermissionRequestPrivate.h"
#include "CyberKitPermissionRequest.h"
#include <wtf/glib/WTFGType.h>

using namespace CyberKit;

/**
 * SECTION: CyberKitNotificationPermissionRequest
 * @Short_description: A permission request for displaying web notifications
 * @Title: CyberKitNotificationPermissionRequest
 * @See_also: #CyberKitPermissionRequest, #CyberKitWebView
 *
 * CyberKitNotificationPermissionRequest represents a request for
 * permission to decide whether CyberKit should provide the user with
 * notifications through the Web Notification API.
 *
 * When a CyberKitNotificationPermissionRequest is not handled by the user,
 * it is denied by default.
 *
 * Since: 2.8
 */

static void webkit_permission_request_interface_init(CyberKitPermissionRequestIface*);

struct _CyberKitNotificationPermissionRequestPrivate {
    RefPtr<NotificationPermissionRequest> request;
    bool madeDecision;
};

WEBKIT_DEFINE_TYPE_WITH_CODE(
    CyberKitNotificationPermissionRequest, webkit_notification_permission_request, G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(WEBKIT_TYPE_PERMISSION_REQUEST, webkit_permission_request_interface_init))

static void webkitNotificationPermissionRequestAllow(CyberKitPermissionRequest* request)
{
    ASSERT(WEBKIT_IS_NOTIFICATION_PERMISSION_REQUEST(request));

    CyberKitNotificationPermissionRequestPrivate* priv = WEBKIT_NOTIFICATION_PERMISSION_REQUEST(request)->priv;

    // Only one decision at a time.
    if (priv->madeDecision)
        return;

    priv->request->allow();
    priv->madeDecision = true;
}

static void webkitNotificationPermissionRequestDeny(CyberKitPermissionRequest* request)
{
    ASSERT(WEBKIT_IS_NOTIFICATION_PERMISSION_REQUEST(request));

    CyberKitNotificationPermissionRequestPrivate* priv = WEBKIT_NOTIFICATION_PERMISSION_REQUEST(request)->priv;

    // Only one decision at a time.
    if (priv->madeDecision)
        return;

    priv->request->deny();
    priv->madeDecision = true;
}

static void webkit_permission_request_interface_init(CyberKitPermissionRequestIface* iface)
{
    iface->allow = webkitNotificationPermissionRequestAllow;
    iface->deny = webkitNotificationPermissionRequestDeny;
}

static void webkitNotificationPermissionRequestDispose(GObject* object)
{
    // Default behaviour when no decision has been made is denying the request.
    webkitNotificationPermissionRequestDeny(WEBKIT_PERMISSION_REQUEST(object));
    G_OBJECT_CLASS(webkit_notification_permission_request_parent_class)->dispose(object);
}

static void webkit_notification_permission_request_class_init(CyberKitNotificationPermissionRequestClass* klass)
{
    GObjectClass* objectClass = G_OBJECT_CLASS(klass);
    objectClass->dispose = webkitNotificationPermissionRequestDispose;
}

CyberKitNotificationPermissionRequest* webkitNotificationPermissionRequestCreate(NotificationPermissionRequest* request)
{
    CyberKitNotificationPermissionRequest* notificationPermissionRequest = WEBKIT_NOTIFICATION_PERMISSION_REQUEST(g_object_new(WEBKIT_TYPE_NOTIFICATION_PERMISSION_REQUEST, nullptr));
    notificationPermissionRequest->priv->request = request;
    return notificationPermissionRequest;
}
