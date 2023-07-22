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

#if !defined(__WEBKIT2_H_INSIDE__) && !defined(WEBKIT2_COMPILATION)
#error "Only <webkit2/webkit2.h> can be included directly."
#endif

#ifndef CyberKitNetworkProxySettings_h
#define CyberKitNetworkProxySettings_h

#include <glib-object.h>
#include <webkit2/CyberKitDefines.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_NETWORK_NETWORK_PROXY_SETTINGS (webkit_network_proxy_settings_get_type())

typedef struct _CyberKitNetworkProxySettings CyberKitNetworkProxySettings;

WEBKIT_API GType
webkit_network_proxy_settings_get_type             (void);

WEBKIT_API CyberKitNetworkProxySettings *
webkit_network_proxy_settings_new                  (const gchar                *default_proxy_uri,
                                                    const gchar* const         *ignore_hosts);

WEBKIT_API CyberKitNetworkProxySettings *
webkit_network_proxy_settings_copy                 (CyberKitNetworkProxySettings *proxy_settings);

WEBKIT_API void
webkit_network_proxy_settings_free                 (CyberKitNetworkProxySettings *proxy_settings);

WEBKIT_API void
webkit_network_proxy_settings_add_proxy_for_scheme (CyberKitNetworkProxySettings *proxy_settings,
                                                    const gchar                *scheme,
                                                    const gchar                *proxy_uri);

G_END_DECLS

#endif /* CyberKitNetworkProxySettings_h */
