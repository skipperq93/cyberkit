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

#ifndef CyberKitWebsiteData_h
#define CyberKitWebsiteData_h

#include <glib-object.h>
#include <webkit2/CyberKitDefines.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_WEBSITE_DATA (webkit_website_data_get_type())

typedef struct _CyberKitWebsiteData CyberKitWebsiteData;

/**
 * CyberKitWebsiteDataTypes:
 * @WEBKIT_WEBSITE_DATA_MEMORY_CACHE: Memory cache.
 * @WEBKIT_WEBSITE_DATA_DISK_CACHE: HTTP disk cache.
 * @WEBKIT_WEBSITE_DATA_OFFLINE_APPLICATION_CACHE: Offline web application cache.
 * @WEBKIT_WEBSITE_DATA_SESSION_STORAGE: Session storage data.
 * @WEBKIT_WEBSITE_DATA_LOCAL_STORAGE: Local storage data.
 * @WEBKIT_WEBSITE_DATA_WEBSQL_DATABASES: WebSQL databases. Deprecated 2.24
 * @WEBKIT_WEBSITE_DATA_INDEXEDDB_DATABASES: IndexedDB databases.
 * @WEBKIT_WEBSITE_DATA_PLUGIN_DATA: Plugins data. Deprecated 2.32
 * @WEBKIT_WEBSITE_DATA_COOKIES: Cookies.
 * @WEBKIT_WEBSITE_DATA_DEVICE_ID_HASH_SALT: Hash salt used to generate the device ids used by webpages. Since 2.24
 * @WEBKIT_WEBSITE_DATA_HSTS_CACHE: HSTS cache. Since 2.26
 * @WEBKIT_WEBSITE_DATA_ITP: Intelligent Tracking Prevention data. Since 2.30.
 * @WEBKIT_WEBSITE_DATA_SERVICE_WORKER_REGISTRATIONS: Service worker registrations. Since 2.30
 * @WEBKIT_WEBSITE_DATA_DOM_CACHE: DOM (CacheStorage) cache. Since 2.30
 * @WEBKIT_WEBSITE_DATA_ALL: All types.
 *
 * Enum values with flags representing types of Website data.
 *
 * Since: 2.16
 */
typedef enum {
    WEBKIT_WEBSITE_DATA_MEMORY_CACHE                 = 1 << 0,
    WEBKIT_WEBSITE_DATA_DISK_CACHE                   = 1 << 1,
    WEBKIT_WEBSITE_DATA_OFFLINE_APPLICATION_CACHE    = 1 << 2,
    WEBKIT_WEBSITE_DATA_SESSION_STORAGE              = 1 << 3,
    WEBKIT_WEBSITE_DATA_LOCAL_STORAGE                = 1 << 4,
    WEBKIT_WEBSITE_DATA_WEBSQL_DATABASES             = 1 << 5,
    WEBKIT_WEBSITE_DATA_INDEXEDDB_DATABASES          = 1 << 6,
    WEBKIT_WEBSITE_DATA_PLUGIN_DATA                  = 1 << 7,
    WEBKIT_WEBSITE_DATA_COOKIES                      = 1 << 8,
    WEBKIT_WEBSITE_DATA_DEVICE_ID_HASH_SALT          = 1 << 9,
    WEBKIT_WEBSITE_DATA_HSTS_CACHE                   = 1 << 10,
    WEBKIT_WEBSITE_DATA_ITP                          = 1 << 11,
    WEBKIT_WEBSITE_DATA_SERVICE_WORKER_REGISTRATIONS = 1 << 12,
    WEBKIT_WEBSITE_DATA_DOM_CACHE                    = 1 << 13,
    WEBKIT_WEBSITE_DATA_ALL                          = (1 << 14) - 1
} CyberKitWebsiteDataTypes;

WEBKIT_API GType
webkit_website_data_get_type      (void);

WEBKIT_API CyberKitWebsiteData *
webkit_website_data_ref           (CyberKitWebsiteData     *website_data);

WEBKIT_API void
webkit_website_data_unref         (CyberKitWebsiteData     *website_data);

WEBKIT_API const char *
webkit_website_data_get_name      (CyberKitWebsiteData     *website_data);

WEBKIT_API CyberKitWebsiteDataTypes
webkit_website_data_get_types     (CyberKitWebsiteData     *website_data);

WEBKIT_API guint64
webkit_website_data_get_size      (CyberKitWebsiteData     *website_data,
                                   CyberKitWebsiteDataTypes types);

G_END_DECLS

#endif /* CyberKitWebsiteData_h */
