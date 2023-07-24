/*
 * Copyright (C) 2015 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2,1 of the License, or (at your option) any later version.
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

#if !defined(__WEBKIT_H_INSIDE__) && !defined(WEBKIT2_COMPILATION)
#error "Only <wpe/webkit.h> can be included directly."
#endif

#ifndef CyberKitWebsiteDataManager_h
#define CyberKitWebsiteDataManager_h

#include <gio/gio.h>
#include <wpe/CyberKitCookieManager.h>
#include <wpe/CyberKitDefines.h>
#include <wpe/CyberKitWebsiteData.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_WEBSITE_DATA_MANAGER            (webkit_website_data_manager_get_type())
#define WEBKIT_WEBSITE_DATA_MANAGER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_WEBSITE_DATA_MANAGER, CyberKitWebsiteDataManager))
#define WEBKIT_IS_WEBSITE_DATA_MANAGER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_WEBSITE_DATA_MANAGER))
#define WEBKIT_WEBSITE_DATA_MANAGER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_WEBSITE_DATA_MANAGER, CyberKitWebsiteDataManagerClass))
#define WEBKIT_IS_WEBSITE_DATA_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_WEBSITE_DATA_MANAGER))
#define WEBKIT_WEBSITE_DATA_MANAGER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_WEBSITE_DATA_MANAGER, CyberKitWebsiteDataManagerClass))

typedef struct _CyberKitWebsiteDataManager        CyberKitWebsiteDataManager;
typedef struct _CyberKitWebsiteDataManagerClass   CyberKitWebsiteDataManagerClass;
typedef struct _CyberKitWebsiteDataManagerPrivate CyberKitWebsiteDataManagerPrivate;

struct _CyberKitWebsiteDataManager {
    GObject parent;

    CyberKitWebsiteDataManagerPrivate *priv;
};

struct _CyberKitWebsiteDataManagerClass {
    GObjectClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_website_data_manager_get_type                                  (void);

WEBKIT_API CyberKitWebsiteDataManager *
webkit_website_data_manager_new                                       (const gchar              *first_option_name,
                                                                       ...);
WEBKIT_API CyberKitWebsiteDataManager *
webkit_website_data_manager_new_ephemeral                             (void);

WEBKIT_API gboolean
webkit_website_data_manager_is_ephemeral                              (CyberKitWebsiteDataManager* manager);

WEBKIT_API const gchar *
webkit_website_data_manager_get_base_data_directory                   (CyberKitWebsiteDataManager *manager);

WEBKIT_API const gchar *
webkit_website_data_manager_get_base_cache_directory                  (CyberKitWebsiteDataManager *manager);

WEBKIT_API const gchar *
webkit_website_data_manager_get_local_storage_directory               (CyberKitWebsiteDataManager *manager);

WEBKIT_API const gchar *
webkit_website_data_manager_get_disk_cache_directory                  (CyberKitWebsiteDataManager *manager);

WEBKIT_API const gchar *
webkit_website_data_manager_get_offline_application_cache_directory   (CyberKitWebsiteDataManager *manager);

WEBKIT_API const gchar *
webkit_website_data_manager_get_indexeddb_directory                   (CyberKitWebsiteDataManager *manager);

WEBKIT_DEPRECATED const gchar *
webkit_website_data_manager_get_websql_directory                      (CyberKitWebsiteDataManager *manager);

WEBKIT_API const gchar *
webkit_website_data_manager_get_hsts_cache_directory                  (CyberKitWebsiteDataManager *manager);

WEBKIT_API const gchar *
webkit_website_data_manager_get_itp_directory                         (CyberKitWebsiteDataManager *manager);

WEBKIT_API const gchar *
webkit_website_data_manager_get_service_worker_registrations_directory(CyberKitWebsiteDataManager *manager);

WEBKIT_API const gchar *
webkit_website_data_manager_get_dom_cache_directory                   (CyberKitWebsiteDataManager *manager);

WEBKIT_API CyberKitCookieManager *
webkit_website_data_manager_get_cookie_manager                        (CyberKitWebsiteDataManager *manager);

WEBKIT_API void
webkit_website_data_manager_set_itp_enabled                           (CyberKitWebsiteDataManager *manager,
                                                                       gboolean                  enabled);

WEBKIT_API gboolean
webkit_website_data_manager_get_itp_enabled                           (CyberKitWebsiteDataManager *manager);

WEBKIT_API void
webkit_website_data_manager_set_persistent_credential_storage_enabled (CyberKitWebsiteDataManager *manager,
                                                                       gboolean                  enabled);

WEBKIT_API gboolean
webkit_website_data_manager_get_persistent_credential_storage_enabled (CyberKitWebsiteDataManager *manager);

WEBKIT_API void
webkit_website_data_manager_fetch                                     (CyberKitWebsiteDataManager *manager,
                                                                       CyberKitWebsiteDataTypes    types,
                                                                       GCancellable             *cancellable,
                                                                       GAsyncReadyCallback       callback,
                                                                       gpointer                  user_data);

WEBKIT_API GList *
webkit_website_data_manager_fetch_finish                              (CyberKitWebsiteDataManager *manager,
                                                                       GAsyncResult             *result,
                                                                       GError                  **error);
WEBKIT_API void
webkit_website_data_manager_remove                                    (CyberKitWebsiteDataManager *manager,
                                                                       CyberKitWebsiteDataTypes    types,
                                                                       GList                    *website_data,
                                                                       GCancellable             *cancellable,
                                                                       GAsyncReadyCallback       callback,
                                                                       gpointer                  user_data);
WEBKIT_API gboolean
webkit_website_data_manager_remove_finish                             (CyberKitWebsiteDataManager *manager,
                                                                       GAsyncResult             *result,
                                                                       GError                  **error);

WEBKIT_API void
webkit_website_data_manager_clear                                      (CyberKitWebsiteDataManager *manager,
                                                                        CyberKitWebsiteDataTypes    types,
                                                                        GTimeSpan                 timespan,
                                                                        GCancellable             *cancellable,
                                                                        GAsyncReadyCallback       callback,
                                                                        gpointer                  user_data);

WEBKIT_API gboolean
webkit_website_data_manager_clear_finish                               (CyberKitWebsiteDataManager *manager,
                                                                        GAsyncResult             *result,
                                                                        GError                  **error);


#define WEBKIT_TYPE_ITP_FIRST_PARTY   (webkit_itp_first_party_get_type())

typedef struct _CyberKitITPFirstParty CyberKitITPFirstParty;

WEBKIT_API GType
webkit_itp_first_party_get_type                        (void);

WEBKIT_API CyberKitITPFirstParty *
webkit_itp_first_party_ref                             (CyberKitITPFirstParty      *itp_first_party);

WEBKIT_API void
webkit_itp_first_party_unref                           (CyberKitITPFirstParty      *itp_first_party);

WEBKIT_API const char *
webkit_itp_first_party_get_domain                      (CyberKitITPFirstParty      *itp_first_party);

WEBKIT_API gboolean
webkit_itp_first_party_get_website_data_access_allowed (CyberKitITPFirstParty      *itp_first_party);

WEBKIT_API GDateTime *
webkit_itp_first_party_get_last_update_time            (CyberKitITPFirstParty      *itp_first_party);


#define WEBKIT_TYPE_ITP_THIRD_PARTY   (webkit_itp_third_party_get_type())

typedef struct _CyberKitITPThirdParty CyberKitITPThirdParty;

WEBKIT_API GType
webkit_itp_third_party_get_type                        (void);

WEBKIT_API CyberKitITPThirdParty *
webkit_itp_third_party_ref                             (CyberKitITPThirdParty      *itp_third_party);

WEBKIT_API void
webkit_itp_third_party_unref                           (CyberKitITPThirdParty      *itp_third_party);

WEBKIT_API const char *
webkit_itp_third_party_get_domain                      (CyberKitITPThirdParty      *itp_third_party);

WEBKIT_API GList *
webkit_itp_third_party_get_first_parties               (CyberKitITPThirdParty      *itp_third_party);


WEBKIT_API void
webkit_website_data_manager_get_itp_summary            (CyberKitWebsiteDataManager *manager,
                                                        GCancellable             *cancellable,
                                                        GAsyncReadyCallback       callback,
                                                        gpointer                  user_data);
WEBKIT_API GList *
webkit_website_data_manager_get_itp_summary_finish     (CyberKitWebsiteDataManager *manager,
                                                        GAsyncResult             *result,
                                                        GError                  **error);

G_END_DECLS

#endif
