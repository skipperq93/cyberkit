/*
 * Copyright (C) 2014 Collabora Ltd.
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
#include "CyberKitNotification.h"

#include "CyberKitNotificationPrivate.h"
#include "WebNotification.h"
#include <glib/gi18n-lib.h>
#include <wtf/glib/WTFGType.h>
#include <wtf/text/CString.h>

/**
 * SECTION: CyberKitNotification
 * @Short_description: Object used to hold information about a notification that should be shown to the user.
 * @Title: CyberKitNotification
 *
 * Since: 2.8
 */

enum {
    PROP_0,

    PROP_ID,
    PROP_TITLE,
    PROP_BODY,
    PROP_TAG
};

enum {
    CLOSED,
    CLICKED,

    LAST_SIGNAL
};

struct _CyberKitNotificationPrivate {
    CString title;
    CString body;
    CString tag;
    guint64 id;

    CyberKitWebView* webView;
};

static guint signals[LAST_SIGNAL] = { 0, };

WEBKIT_DEFINE_TYPE(CyberKitNotification, webkit_notification, G_TYPE_OBJECT)

static void webkitNotificationGetProperty(GObject* object, guint propId, GValue* value, GParamSpec* paramSpec)
{
    CyberKitNotification* notification = WEBKIT_NOTIFICATION(object);

    switch (propId) {
    case PROP_ID:
        g_value_set_uint64(value, webkit_notification_get_id(notification));
        break;
    case PROP_TITLE:
        g_value_set_string(value, webkit_notification_get_title(notification));
        break;
    case PROP_BODY:
        g_value_set_string(value, webkit_notification_get_body(notification));
        break;
    case PROP_TAG:
        g_value_set_string(value, webkit_notification_get_tag(notification));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propId, paramSpec);
    }
}

static void webkit_notification_class_init(CyberKitNotificationClass* notificationClass)
{
    GObjectClass* objectClass = G_OBJECT_CLASS(notificationClass);
    objectClass->get_property = webkitNotificationGetProperty;

    /**
     * CyberKitNotification:id:
     *
     * The unique id for the notification.
     *
     * Since: 2.8
     */
    g_object_class_install_property(objectClass,
        PROP_ID,
        g_param_spec_uint64("id",
            _("ID"),
            _("The unique id for the notification"),
            0, G_MAXUINT64, 0,
            WEBKIT_PARAM_READABLE));

    /**
     * CyberKitNotification:title:
     *
     * The title for the notification.
     *
     * Since: 2.8
     */
    g_object_class_install_property(objectClass,
        PROP_TITLE,
        g_param_spec_string("title",
            _("Title"),
            _("The title for the notification"),
            nullptr,
            WEBKIT_PARAM_READABLE));

    /**
     * CyberKitNotification:body:
     *
     * The body for the notification.
     *
     * Since: 2.8
     */
    g_object_class_install_property(objectClass,
        PROP_BODY,
        g_param_spec_string("body",
            _("Body"),
            _("The body for the notification"),
            nullptr,
            WEBKIT_PARAM_READABLE));

    /**
     * CyberKitNotification:tag:
     *
     * The tag identifier for the notification.
     *
     * Since: 2.16
     */
    g_object_class_install_property(objectClass,
        PROP_TAG,
        g_param_spec_string("tag",
            _("Tag"),
            _("The tag identifier for the notification"),
            nullptr,
            WEBKIT_PARAM_READABLE));

    /**
     * CyberKitNotification::closed:
     * @notification: the #CyberKitNotification on which the signal is emitted
     *
     * Emitted when a notification has been withdrawn.
     *
     * The default handler will close the notification using libnotify, if built with
     * support for it.
     *
     * Since: 2.8
     */
    signals[CLOSED] =
        g_signal_new(
            "closed",
            G_TYPE_FROM_CLASS(notificationClass),
            G_SIGNAL_RUN_LAST,
            0, 0,
            nullptr,
            g_cclosure_marshal_VOID__VOID,
            G_TYPE_NONE, 0);

    /**
     * CyberKitNotification::clicked:
     * @notification: the #CyberKitNotification on which the signal is emitted
     *
     * Emitted when a notification has been clicked. See webkit_notification_clicked().
     *
     * Since: 2.12
     */
    signals[CLICKED] =
        g_signal_new(
            "clicked",
            G_TYPE_FROM_CLASS(notificationClass),
            G_SIGNAL_RUN_LAST,
            0, 0,
            nullptr,
            g_cclosure_marshal_VOID__VOID,
            G_TYPE_NONE, 0);
}

CyberKitNotification* webkitNotificationCreate(CyberKitWebView* webView, const CyberKit::WebNotification& webNotification)
{
    CyberKitNotification* notification = WEBKIT_NOTIFICATION(g_object_new(WEBKIT_TYPE_NOTIFICATION, nullptr));
    notification->priv->id = webNotification.notificationID();
    notification->priv->title = webNotification.title().utf8();
    notification->priv->body = webNotification.body().utf8();
    notification->priv->tag = webNotification.tag().utf8();
    notification->priv->webView = webView;
    return notification;
}

CyberKitWebView* webkitNotificationGetWebView(CyberKitNotification* notification)
{
    return notification->priv->webView;
}

/**
 * webkit_notification_get_id:
 * @notification: a #CyberKitNotification
 *
 * Obtains the unique id for the notification.
 *
 * Returns: the unique id for the notification
 *
 * Since: 2.8
 */
guint64 webkit_notification_get_id(CyberKitNotification* notification)
{
    g_return_val_if_fail(WEBKIT_IS_NOTIFICATION(notification), 0);

    return notification->priv->id;
}

/**
 * webkit_notification_get_title:
 * @notification: a #CyberKitNotification
 *
 * Obtains the title for the notification.
 *
 * Returns: the title for the notification
 *
 * Since: 2.8
 */
const gchar* webkit_notification_get_title(CyberKitNotification* notification)
{
    g_return_val_if_fail(WEBKIT_IS_NOTIFICATION(notification), nullptr);

    return notification->priv->title.data();
}

/**
 * webkit_notification_get_body:
 * @notification: a #CyberKitNotification
 *
 * Obtains the body for the notification.
 *
 * Returns: the body for the notification
 *
 * Since: 2.8
 */
const gchar* webkit_notification_get_body(CyberKitNotification* notification)
{
    g_return_val_if_fail(WEBKIT_IS_NOTIFICATION(notification), nullptr);

    return notification->priv->body.data();
}

/**
 * webkit_notification_get_tag:
 * @notification: a #CyberKitNotification
 *
 * Obtains the tag identifier for the notification.
 *
 * Returns: (allow-none): the tag for the notification
 *
 * Since: 2.16
 */
const gchar* webkit_notification_get_tag(CyberKitNotification* notification)
{
    g_return_val_if_fail(WEBKIT_IS_NOTIFICATION(notification), nullptr);

    const gchar* tag = notification->priv->tag.data();
    return notification->priv->tag.length() ? tag : nullptr;
}

/**
 * webkit_notification_close:
 * @notification: a #CyberKitNotification
 *
 * Closes the notification.
 *
 * Since: 2.8
 */
void webkit_notification_close(CyberKitNotification* notification)
{
    g_return_if_fail(WEBKIT_IS_NOTIFICATION(notification));

    g_signal_emit(notification, signals[CLOSED], 0);
}

/**
 * webkit_notification_clicked:
 * @notification: a #CyberKitNotification
 *
 * Tells CyberKit the notification has been clicked. This will emit the
 * #CyberKitNotification::clicked signal.
 *
 * Since: 2.12
 */
void webkit_notification_clicked(CyberKitNotification* notification)
{
    g_return_if_fail(WEBKIT_IS_NOTIFICATION(notification));

    g_signal_emit(notification, signals[CLICKED], 0);
}
