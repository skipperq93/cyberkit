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

#include "config.h"
#include "CyberKitWebsiteDataManager.h"

#include "CyberKitCookieManagerPrivate.h"
#include "CyberKitInitialize.h"
#include "CyberKitMemoryPressureSettings.h"
#include "CyberKitMemoryPressureSettingsPrivate.h"
#include "CyberKitNetworkProxySettingsPrivate.h"
#include "CyberKitPrivate.h"
#include "CyberKitWebsiteDataManagerPrivate.h"
#include "CyberKitWebsiteDataPrivate.h"
#include "WebProcessPool.h"
#include "WebsiteDataFetchOption.h"
#include "WebsiteDataStore.h"
#include <glib/gi18n-lib.h>
#include <pal/SessionID.h>
#include <wtf/FileSystem.h>
#include <wtf/glib/GUniquePtr.h>
#include <wtf/glib/WTFGType.h>

#if PLATFORM(GTK) && ENABLE(2022_GLIB_API)
#include "CyberKitFaviconDatabasePrivate.h"
#endif

using namespace CyberKit;

/**
 * CyberKitWebsiteDataManager:
 * @See_also: #CyberKitWebContext, #CyberKitWebsiteData
 *
 * Manages data stored locally by web sites.
 *
 * You can use CyberKitWebsiteDataManager to configure the local directories
 * where website data will be stored. Use #CyberKitWebsiteDataManager:base-data-directory
 * and #CyberKitWebsiteDataManager:base-cache-directory set a common base directory for all
 * website data and caches. The newly created CyberKitWebsiteDataManager must be passed as
 * a construct property to a #CyberKitWebContext; you can use webkit_web_context_new_with_website_data_manager()
 * to create a new #CyberKitWebContext with a CyberKitWebsiteDataManager.
 * If you don't want to set any specific configuration, you don't need to create
 * a CyberKitWebsiteDataManager: the #CyberKitWebContext will create a CyberKitWebsiteDataManager
 * with the default configuration. To get the CyberKitWebsiteDataManager of a #CyberKitWebContext,
 * you can use webkit_web_context_get_website_data_manager().
 *
 * A CyberKitWebsiteDataManager can also be ephemeral, in which case all the directory configuration
 * is not needed because website data will never persist. You can create an ephemeral CyberKitWebsiteDataManager
 * with webkit_website_data_manager_new_ephemeral() and pass the ephemeral CyberKitWebsiteDataManager to
 * a #CyberKitWebContext, or simply use webkit_web_context_new_ephemeral().
 *
 * CyberKitWebsiteDataManager can also be used to fetch website data, remove data
 * stored by particular websites, or clear data for all websites modified since a given
 * period of time.
 *
 * Since: 2.10
 */

using namespace CyberKit;

enum {
    PROP_0,

    PROP_BASE_DATA_DIRECTORY,
    PROP_BASE_CACHE_DIRECTORY,
#if !ENABLE(2022_GLIB_API)
    PROP_LOCAL_STORAGE_DIRECTORY,
    PROP_DISK_CACHE_DIRECTORY,
    PROP_APPLICATION_CACHE_DIRECTORY,
    PROP_INDEXEDDB_DIRECTORY,
    PROP_WEBSQL_DIRECTORY,
    PROP_HSTS_CACHE_DIRECTORY,
    PROP_ITP_DIRECTORY,
    PROP_SERVICE_WORKER_REGISTRATIONS_DIRECTORY,
    PROP_DOM_CACHE_DIRECTORY,
#endif
    PROP_IS_EPHEMERAL
};

struct _CyberKitWebsiteDataManagerPrivate {
    RefPtr<CyberKit::WebsiteDataStore> websiteDataStore;
    CString baseDataDirectory;
    CString baseCacheDirectory;

#if PLATFORM(GTK) && ENABLE(2022_GLIB_API)
    GRefPtr<CyberKitFaviconDatabase> faviconDatabase;
#endif

#if !ENABLE(2022_GLIB_API)
    CyberKitTLSErrorsPolicy tlsErrorsPolicy;

    GRefPtr<CyberKitCookieManager> cookieManager;

    // These manual directory overrides are deprecated. Please don't add more.
    GUniquePtr<char> localStorageDirectory;
    GUniquePtr<char> diskCacheDirectory;
    GUniquePtr<char> applicationCacheDirectory;
    GUniquePtr<char> indexedDBDirectory;
    GUniquePtr<char> webSQLDirectory;
    GUniquePtr<char> hstsCacheDirectory;
    GUniquePtr<char> itpDirectory;
    GUniquePtr<char> swRegistrationsDirectory;
    GUniquePtr<char> domCacheDirectory;
#endif
};

WEBKIT_DEFINE_FINAL_TYPE(CyberKitWebsiteDataManager, webkit_website_data_manager, G_TYPE_OBJECT, GObject)

static void webkitWebsiteDataManagerGetProperty(GObject* object, guint propID, GValue* value, GParamSpec* paramSpec)
{
    CyberKitWebsiteDataManager* manager = WEBKIT_WEBSITE_DATA_MANAGER(object);

#if !ENABLE(2022_GLIB_API)
    ALLOW_DEPRECATED_DECLARATIONS_BEGIN
#endif
    switch (propID) {
    case PROP_BASE_DATA_DIRECTORY:
        g_value_set_string(value, webkit_website_data_manager_get_base_data_directory(manager));
        break;
    case PROP_BASE_CACHE_DIRECTORY:
        g_value_set_string(value, webkit_website_data_manager_get_base_cache_directory(manager));
        break;
#if !ENABLE(2022_GLIB_API)
    case PROP_LOCAL_STORAGE_DIRECTORY:
        g_value_set_string(value, webkit_website_data_manager_get_local_storage_directory(manager));
        break;
    case PROP_DISK_CACHE_DIRECTORY:
        g_value_set_string(value, webkit_website_data_manager_get_disk_cache_directory(manager));
        break;
    case PROP_APPLICATION_CACHE_DIRECTORY:
        g_value_set_string(value, webkit_website_data_manager_get_offline_application_cache_directory(manager));
        break;
    case PROP_INDEXEDDB_DIRECTORY:
        g_value_set_string(value, webkit_website_data_manager_get_indexeddb_directory(manager));
        break;
    case PROP_WEBSQL_DIRECTORY:
        g_value_set_string(value, webkit_website_data_manager_get_websql_directory(manager));
        break;
    case PROP_HSTS_CACHE_DIRECTORY:
        g_value_set_string(value, webkit_website_data_manager_get_hsts_cache_directory(manager));
        break;
    case PROP_ITP_DIRECTORY:
        g_value_set_string(value, webkit_website_data_manager_get_itp_directory(manager));
        break;
    case PROP_SERVICE_WORKER_REGISTRATIONS_DIRECTORY:
        g_value_set_string(value, webkit_website_data_manager_get_service_worker_registrations_directory(manager));
        break;
    case PROP_DOM_CACHE_DIRECTORY:
        g_value_set_string(value, webkit_website_data_manager_get_dom_cache_directory(manager));
        break;
#endif
    case PROP_IS_EPHEMERAL:
        g_value_set_boolean(value, webkit_website_data_manager_is_ephemeral(manager));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propID, paramSpec);
    }
#if !ENABLE(2022_GLIB_API)
    ALLOW_DEPRECATED_DECLARATIONS_END
#endif
}

static void webkitWebsiteDataManagerSetProperty(GObject* object, guint propID, const GValue* value, GParamSpec* paramSpec)
{
    CyberKitWebsiteDataManager* manager = WEBKIT_WEBSITE_DATA_MANAGER(object);

    switch (propID) {
    case PROP_BASE_DATA_DIRECTORY:
        manager->priv->baseDataDirectory = g_value_get_string(value);
        break;
    case PROP_BASE_CACHE_DIRECTORY:
        manager->priv->baseCacheDirectory = g_value_get_string(value);
        break;
#if !ENABLE(2022_GLIB_API)
    case PROP_LOCAL_STORAGE_DIRECTORY:
        manager->priv->localStorageDirectory.reset(g_value_dup_string(value));
        break;
    case PROP_DISK_CACHE_DIRECTORY:
        manager->priv->diskCacheDirectory.reset(g_value_dup_string(value));
        break;
    case PROP_APPLICATION_CACHE_DIRECTORY:
        manager->priv->applicationCacheDirectory.reset(g_value_dup_string(value));
        break;
    case PROP_INDEXEDDB_DIRECTORY:
        manager->priv->indexedDBDirectory.reset(g_value_dup_string(value));
        break;
    case PROP_WEBSQL_DIRECTORY:
        manager->priv->webSQLDirectory.reset(g_value_dup_string(value));
        break;
    case PROP_HSTS_CACHE_DIRECTORY:
        manager->priv->hstsCacheDirectory.reset(g_value_dup_string(value));
        break;
    case PROP_ITP_DIRECTORY:
        manager->priv->itpDirectory.reset(g_value_dup_string(value));
        break;
    case PROP_SERVICE_WORKER_REGISTRATIONS_DIRECTORY:
        manager->priv->swRegistrationsDirectory.reset(g_value_dup_string(value));
        break;
    case PROP_DOM_CACHE_DIRECTORY:
        manager->priv->domCacheDirectory.reset(g_value_dup_string(value));
        break;
#endif
    case PROP_IS_EPHEMERAL:
        if (g_value_get_boolean(value))
            manager->priv->websiteDataStore = CyberKit::WebsiteDataStore::createNonPersistent();
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propID, paramSpec);
    }
}

#if !ENABLE(2022_GLIB_API)
static void webkitWebsiteDataManagerConstructed(GObject* object)
{
    G_OBJECT_CLASS(webkit_website_data_manager_parent_class)->constructed(object);

    CyberKitWebsiteDataManagerPrivate* priv = WEBKIT_WEBSITE_DATA_MANAGER(object)->priv;
    if (!priv->baseDataDirectory.isNull()) {
        if (!priv->localStorageDirectory)
            priv->localStorageDirectory.reset(g_build_filename(priv->baseDataDirectory.data(), "localstorage", nullptr));
        if (!priv->indexedDBDirectory)
            priv->indexedDBDirectory.reset(g_build_filename(priv->baseDataDirectory.data(), "databases", "indexeddb", nullptr));
        if (!priv->webSQLDirectory)
            priv->webSQLDirectory.reset(g_build_filename(priv->baseDataDirectory.data(), "databases", nullptr));
        if (!priv->itpDirectory)
            priv->itpDirectory.reset(g_build_filename(priv->baseDataDirectory.data(), "itp", nullptr));
        if (!priv->swRegistrationsDirectory)
            priv->swRegistrationsDirectory.reset(g_build_filename(priv->baseDataDirectory.data(), "serviceworkers", nullptr));
    }

    if (!priv->baseCacheDirectory.isNull()) {
        if (!priv->diskCacheDirectory)
            priv->diskCacheDirectory.reset(g_strdup(priv->baseCacheDirectory.data()));
        if (!priv->applicationCacheDirectory)
            priv->applicationCacheDirectory.reset(g_build_filename(priv->baseCacheDirectory.data(), "applications", nullptr));
        if (!priv->hstsCacheDirectory)
            priv->hstsCacheDirectory.reset(g_strdup(priv->baseCacheDirectory.data()));
        if (!priv->domCacheDirectory)
            priv->domCacheDirectory.reset(g_build_filename(priv->baseCacheDirectory.data(), "CacheStorage", nullptr));
    }

    priv->tlsErrorsPolicy = WEBKIT_TLS_ERRORS_POLICY_FAIL;
    if (priv->websiteDataStore)
        priv->websiteDataStore->setIgnoreTLSErrors(false);
}
#endif

static void webkit_website_data_manager_class_init(CyberKitWebsiteDataManagerClass* findClass)
{
    webkitInitialize();

    GObjectClass* gObjectClass = G_OBJECT_CLASS(findClass);
    gObjectClass->get_property = webkitWebsiteDataManagerGetProperty;
    gObjectClass->set_property = webkitWebsiteDataManagerSetProperty;
#if !ENABLE(2022_GLIB_API)
    gObjectClass->constructed = webkitWebsiteDataManagerConstructed;
#endif

    /**
     * CyberKitWebsiteDataManager:base-data-directory:
     *
     * The base directory for website data. If %NULL, a default location will be used.
     *
     * Since: 2.10
     */
    g_object_class_install_property(
        gObjectClass,
        PROP_BASE_DATA_DIRECTORY,
        g_param_spec_string(
            "base-data-directory",
            nullptr, nullptr,
            nullptr,
            static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY)));

    /**
     * CyberKitWebsiteDataManager:base-cache-directory:
     *
     * The base directory for caches. If %NULL, a default location will be used.
     *
     * Since: 2.10
     */
    g_object_class_install_property(
        gObjectClass,
        PROP_BASE_CACHE_DIRECTORY,
        g_param_spec_string(
            "base-cache-directory",
            nullptr, nullptr,
            nullptr,
            static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY)));

#if !ENABLE(2022_GLIB_API)
    /**
     * CyberKitWebsiteDataManager:local-storage-directory:
     *
     * The directory where local storage data will be stored.
     *
     * Since: 2.10
     *
     * Deprecated: 2.40. Use CyberKitWebsiteDataManager:base-data-directory instead.
     */
    g_object_class_install_property(
        gObjectClass,
        PROP_LOCAL_STORAGE_DIRECTORY,
        g_param_spec_string(
            "local-storage-directory",
            nullptr, nullptr,
            nullptr,
            static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_DEPRECATED)));

    /**
     * CyberKitWebsiteDataManager:disk-cache-directory:
     *
     * The directory where HTTP disk cache will be stored.
     *
     * Since: 2.10
     *
     * Deprecated: 2.40. Use CyberKitWebsiteDataManager:base-cache-directory instead.
     */
    g_object_class_install_property(
        gObjectClass,
        PROP_DISK_CACHE_DIRECTORY,
        g_param_spec_string(
            "disk-cache-directory",
            nullptr, nullptr,
            nullptr,
            static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_DEPRECATED)));

    /**
     * CyberKitWebsiteDataManager:offline-application-cache-directory:
     *
     * The directory where offline web application cache will be stored.
     *
     * Since: 2.10
     *
     * Deprecated: 2.40. Use CyberKitWebsiteDataManager:base-cache-directory instead.
     */
    g_object_class_install_property(
        gObjectClass,
        PROP_APPLICATION_CACHE_DIRECTORY,
        g_param_spec_string(
            "offline-application-cache-directory",
            nullptr, nullptr,
            nullptr,
            static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_DEPRECATED)));

    /**
     * CyberKitWebsiteDataManager:indexeddb-directory:
     *
     * The directory where IndexedDB databases will be stored.
     *
     * Since: 2.10
     *
     * Deprecated: 2.40. Use CyberKitWebsiteDataManager:base-data-directory instead.
     */
    g_object_class_install_property(
        gObjectClass,
        PROP_INDEXEDDB_DIRECTORY,
        g_param_spec_string(
            "indexeddb-directory",
            nullptr, nullptr,
            nullptr,
            static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_DEPRECATED)));

    /**
     * CyberKitWebsiteDataManager:websql-directory:
     *
     * The directory where WebSQL databases will be stored.
     *
     * Since: 2.10
     *
     * Deprecated: 2.24. WebSQL is no longer supported. Use IndexedDB instead.
     */
    g_object_class_install_property(
        gObjectClass,
        PROP_WEBSQL_DIRECTORY,
        g_param_spec_string(
            "websql-directory",
            nullptr, nullptr,
            nullptr,
            static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_DEPRECATED)));

    /**
     * CyberKitWebsiteDataManager:hsts-cache-directory:
     *
     * The directory where the HTTP Strict-Transport-Security (HSTS) cache will be stored.
     *
     * Since: 2.26
     *
     * Deprecated: 2.40. Use CyberKitWebsiteDataManager:base-cache-directory instead.
     */
    g_object_class_install_property(
        gObjectClass,
        PROP_HSTS_CACHE_DIRECTORY,
        g_param_spec_string(
            "hsts-cache-directory",
            nullptr, nullptr,
            nullptr,
            static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_DEPRECATED)));

    /**
     * CyberKitWebsiteDataManager:itp-directory:
     *
     * The directory where Intelligent Tracking Prevention (ITP) data will be stored.
     *
     * Since: 2.30
     *
     * Deprecated: 2.40. Use CyberKitWebsiteDataManager:base-data-directory instead.
     */
    g_object_class_install_property(
        gObjectClass,
        PROP_ITP_DIRECTORY,
        g_param_spec_string(
            "itp-directory",
            nullptr, nullptr,
            nullptr,
            static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_DEPRECATED)));

    /**
     * CyberKitWebsiteDataManager:service-worker-registrations-directory:
     *
     * The directory where service workers registrations will be stored.
     *
     * Since: 2.30
     *
     * Deprecated: 2.40. Use CyberKitWebsiteDataManager:base-data-directory instead.
     */
    g_object_class_install_property(
        gObjectClass,
        PROP_SERVICE_WORKER_REGISTRATIONS_DIRECTORY,
        g_param_spec_string(
            "service-worker-registrations-directory",
            nullptr, nullptr,
            nullptr,
            static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_DEPRECATED)));

    /**
     * CyberKitWebsiteDataManager:dom-cache-directory:
     *
     * The directory where DOM cache will be stored.
     *
     * Since: 2.30
     *
     * Deprecated: 2.40. Use CyberKitWebsiteDataManager:base-cache-directory instead.
     */
    g_object_class_install_property(
        gObjectClass,
        PROP_DOM_CACHE_DIRECTORY,
        g_param_spec_string(
            "dom-cache-directory",
            nullptr, nullptr,
            nullptr,
            static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_DEPRECATED)));
#endif

    /**
     * CyberKitWebsiteDataManager:is-ephemeral:
     *
     * Whether the #CyberKitWebsiteDataManager is ephemeral. An ephemeral #CyberKitWebsiteDataManager
     * handles all websites data as non-persistent, and nothing will be written to the client
     * storage. Note that if you create an ephemeral #CyberKitWebsiteDataManager all other construction
     * parameters to configure data directories will be ignored.
     *
     * Since: 2.16
     */
    g_object_class_install_property(
        gObjectClass,
        PROP_IS_EPHEMERAL,
        g_param_spec_boolean(
            "is-ephemeral",
            nullptr, nullptr,
            FALSE,
            static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY)));
}

CyberKit::WebsiteDataStore& webkitWebsiteDataManagerGetDataStore(CyberKitWebsiteDataManager* manager)
{
    CyberKitWebsiteDataManagerPrivate* priv = manager->priv;
    if (!priv->websiteDataStore) {
        auto configuration = WebsiteDataStoreConfiguration::createWithBaseDirectories(String::fromUTF8(priv->baseCacheDirectory), String::fromUTF8(priv->baseDataDirectory));
#if !ENABLE(2022_GLIB_API)
        if (priv->localStorageDirectory)
            configuration->setLocalStorageDirectory(FileSystem::stringFromFileSystemRepresentation(priv->localStorageDirectory.get()));
        if (priv->diskCacheDirectory)
            configuration->setNetworkCacheDirectory(FileSystem::pathByAppendingComponent(FileSystem::stringFromFileSystemRepresentation(priv->diskCacheDirectory.get()), networkCacheSubdirectory));
        if (priv->applicationCacheDirectory)
            configuration->setApplicationCacheDirectory(FileSystem::stringFromFileSystemRepresentation(priv->applicationCacheDirectory.get()));
        if (priv->indexedDBDirectory)
            configuration->setIndexedDBDatabaseDirectory(FileSystem::stringFromFileSystemRepresentation(priv->indexedDBDirectory.get()));
        if (priv->webSQLDirectory)
            configuration->setWebSQLDatabaseDirectory(FileSystem::stringFromFileSystemRepresentation(priv->webSQLDirectory.get()));
        if (priv->hstsCacheDirectory)
            configuration->setHSTSStorageDirectory(FileSystem::stringFromFileSystemRepresentation(priv->hstsCacheDirectory.get()));
        if (priv->itpDirectory)
            configuration->setResourceLoadStatisticsDirectory(FileSystem::stringFromFileSystemRepresentation(priv->itpDirectory.get()));
        if (priv->swRegistrationsDirectory)
            configuration->setServiceWorkerRegistrationDirectory(FileSystem::stringFromFileSystemRepresentation(priv->swRegistrationsDirectory.get()));
        if (priv->domCacheDirectory)
            configuration->setCacheStorageDirectory(FileSystem::stringFromFileSystemRepresentation(priv->domCacheDirectory.get()));
#endif
        priv->websiteDataStore = CyberKit::WebsiteDataStore::create(WTFMove(configuration), PAL::SessionID::generatePersistentSessionID());
#if !ENABLE(2022_GLIB_API)
        priv->websiteDataStore->setIgnoreTLSErrors(priv->tlsErrorsPolicy == WEBKIT_TLS_ERRORS_POLICY_IGNORE);
#endif
    }

    return *priv->websiteDataStore;
}

#if ENABLE(2022_GLIB_API)
CyberKitWebsiteDataManager* webkitWebsiteDataManagerCreate(CString&& baseDataDirectory, CString&& baseCacheDirectory)
{
    auto* manager = WEBKIT_WEBSITE_DATA_MANAGER(g_object_new(WEBKIT_TYPE_WEBSITE_DATA_MANAGER, nullptr));
    manager->priv->baseDataDirectory = WTFMove(baseDataDirectory);
    manager->priv->baseCacheDirectory = WTFMove(baseCacheDirectory);
    return manager;
}
#else
/**
 * webkit_website_data_manager_new:
 * @first_option_name: name of the first option to set
 * @...: value of first option, followed by more options, %NULL-terminated
 *
 * Creates a new #CyberKitWebsiteDataManager with the given options.
 *
 * It must be passed as construction parameter of a #CyberKitWebContext.
 *
 * Returns: (transfer full): the newly created #CyberKitWebsiteDataManager
 *
 * Since: 2.10
 */
CyberKitWebsiteDataManager* webkit_website_data_manager_new(const gchar* firstOptionName, ...)
{
    va_list args;
    va_start(args, firstOptionName);
    CyberKitWebsiteDataManager* manager = WEBKIT_WEBSITE_DATA_MANAGER(g_object_new_valist(WEBKIT_TYPE_WEBSITE_DATA_MANAGER, firstOptionName, args));
    va_end(args);

    return manager;
}

/**
 * webkit_website_data_manager_new_ephemeral:
 *
 * Creates an ephemeral #CyberKitWebsiteDataManager.
 *
 * See #CyberKitWebsiteDataManager:is-ephemeral for more details.
 *
 * Returns: (transfer full): a new ephemeral #CyberKitWebsiteDataManager.
 *
 * Since: 2.16
 */
CyberKitWebsiteDataManager* webkit_website_data_manager_new_ephemeral()
{
    return WEBKIT_WEBSITE_DATA_MANAGER(g_object_new(WEBKIT_TYPE_WEBSITE_DATA_MANAGER, "is-ephemeral", TRUE, nullptr));
}
#endif

/**
 * webkit_website_data_manager_is_ephemeral:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get whether a #CyberKitWebsiteDataManager is ephemeral.
 *
 * See #CyberKitWebsiteDataManager:is-ephemeral for more details.
 *
 * Returns: %TRUE if @manager is ephemeral or %FALSE otherwise.
 *
 * Since: 2.16
 */
gboolean webkit_website_data_manager_is_ephemeral(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), FALSE);

    return manager->priv->websiteDataStore && !manager->priv->websiteDataStore->isPersistent();
}

/**
 * webkit_website_data_manager_get_base_data_directory:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get the #CyberKitWebsiteDataManager:base-data-directory property.
 *
 * Returns: (nullable): the base directory for website data, or %NULL if
 *    #CyberKitWebsiteDataManager:base-data-directory was not provided or @manager is ephemeral.
 *
 * Since: 2.10
 */
const gchar* webkit_website_data_manager_get_base_data_directory(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), nullptr);

    if (manager->priv->websiteDataStore && !manager->priv->websiteDataStore->isPersistent())
        return nullptr;

    return manager->priv->baseDataDirectory.data();
}

/**
 * webkit_website_data_manager_get_base_cache_directory:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get the #CyberKitWebsiteDataManager:base-cache-directory property.
 *
 * Returns: (nullable): the base directory for caches, or %NULL if
 *    #CyberKitWebsiteDataManager:base-cache-directory was not provided or @manager is ephemeral.
 *
 * Since: 2.10
 */
const gchar* webkit_website_data_manager_get_base_cache_directory(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), nullptr);

    if (manager->priv->websiteDataStore && !manager->priv->websiteDataStore->isPersistent())
        return nullptr;

    return manager->priv->baseCacheDirectory.data();
}

#if !ENABLE(2022_GLIB_API)
/**
 * webkit_website_data_manager_get_local_storage_directory:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get the #CyberKitWebsiteDataManager:local-storage-directory property.
 *
 * Returns: (allow-none): the directory where local storage data is stored or %NULL if @manager is ephemeral.
 *
 * Since: 2.10
 *
 * Deprecated: 2.40, use webkit_website_data_manager_get_base_data_directory() instead.
 */
const gchar* webkit_website_data_manager_get_local_storage_directory(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), nullptr);

    CyberKitWebsiteDataManagerPrivate* priv = manager->priv;
    if (priv->websiteDataStore && !priv->websiteDataStore->isPersistent())
        return nullptr;

    if (!priv->localStorageDirectory)
        priv->localStorageDirectory.reset(g_strdup(CyberKit::WebsiteDataStore::defaultLocalStorageDirectory().utf8().data()));
    return priv->localStorageDirectory.get();
}

/**
 * webkit_website_data_manager_get_disk_cache_directory:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get the #CyberKitWebsiteDataManager:disk-cache-directory property.
 *
 * Returns: (allow-none): the directory where HTTP disk cache is stored or %NULL if @manager is ephemeral.
 *
 * Since: 2.10
 *
 * Deprecated: 2.40, use webkit_website_data_manager_get_base_cache_directory() instead.
 */
const gchar* webkit_website_data_manager_get_disk_cache_directory(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), nullptr);

    CyberKitWebsiteDataManagerPrivate* priv = manager->priv;
    if (priv->websiteDataStore && !priv->websiteDataStore->isPersistent())
        return nullptr;

    if (!priv->diskCacheDirectory) {
        // The default directory already has the subdirectory.
        priv->diskCacheDirectory.reset(g_strdup(FileSystem::parentPath(CyberKit::WebsiteDataStore::defaultNetworkCacheDirectory()).utf8().data()));
    }
    return priv->diskCacheDirectory.get();
}

/**
 * webkit_website_data_manager_get_offline_application_cache_directory:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get the #CyberKitWebsiteDataManager:offline-application-cache-directory property.
 *
 * Returns: (allow-none): the directory where offline web application cache is stored or %NULL if @manager is ephemeral.
 *
 * Since: 2.10
 *
 * Deprecated: 2.40, use webkit_website_data_manager_get_base_cache_directory() instead.
 */
const gchar* webkit_website_data_manager_get_offline_application_cache_directory(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), nullptr);

    CyberKitWebsiteDataManagerPrivate* priv = manager->priv;
    if (priv->websiteDataStore && !priv->websiteDataStore->isPersistent())
        return nullptr;

    if (!priv->applicationCacheDirectory)
        priv->applicationCacheDirectory.reset(g_strdup(CyberKit::WebsiteDataStore::defaultApplicationCacheDirectory().utf8().data()));
    return priv->applicationCacheDirectory.get();
}

/**
 * webkit_website_data_manager_get_indexeddb_directory:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get the #CyberKitWebsiteDataManager:indexeddb-directory property.
 *
 * Returns: (allow-none): the directory where IndexedDB databases are stored or %NULL if @manager is ephemeral.
 *
 * Since: 2.10
 *
 * Deprecated: 2.40, use webkit_website_data_manager_get_base_data_directory() instead.
 */
const gchar* webkit_website_data_manager_get_indexeddb_directory(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), nullptr);

    CyberKitWebsiteDataManagerPrivate* priv = manager->priv;
    if (priv->websiteDataStore && !priv->websiteDataStore->isPersistent())
        return nullptr;

    if (!priv->indexedDBDirectory)
        priv->indexedDBDirectory.reset(g_strdup(CyberKit::WebsiteDataStore::defaultIndexedDBDatabaseDirectory().utf8().data()));
    return priv->indexedDBDirectory.get();
}

/**
 * webkit_website_data_manager_get_websql_directory:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get the #CyberKitWebsiteDataManager:websql-directory property.
 *
 * Returns: (allow-none): the directory where WebSQL databases are stored or %NULL if @manager is ephemeral.
 *
 * Since: 2.10
 *
 * Deprecated: 2.24. WebSQL is no longer supported. Use IndexedDB instead.
 */
const gchar* webkit_website_data_manager_get_websql_directory(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), nullptr);

    CyberKitWebsiteDataManagerPrivate* priv = manager->priv;
    if (priv->websiteDataStore && !priv->websiteDataStore->isPersistent())
        return nullptr;

    if (!priv->webSQLDirectory)
        priv->webSQLDirectory.reset(g_strdup(CyberKit::WebsiteDataStore::defaultWebSQLDatabaseDirectory().utf8().data()));
    return priv->webSQLDirectory.get();
}

/**
 * webkit_website_data_manager_get_hsts_cache_directory:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get the #CyberKitWebsiteDataManager:hsts-cache-directory property.
 *
 * Returns: (allow-none): the directory where the HSTS cache is stored or %NULL if @manager is ephemeral.
 *
 * Since: 2.26
 *
 * Deprecated: 2.40, use webkit_website_data_manager_get_base_cache_directory() instead.
 */
const gchar* webkit_website_data_manager_get_hsts_cache_directory(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), nullptr);

    CyberKitWebsiteDataManagerPrivate* priv = manager->priv;
    if (priv->websiteDataStore && !priv->websiteDataStore->isPersistent())
        return nullptr;

    if (!priv->hstsCacheDirectory)
        priv->hstsCacheDirectory.reset(g_strdup(CyberKit::WebsiteDataStore::defaultHSTSStorageDirectory().utf8().data()));
    return priv->hstsCacheDirectory.get();
}

/**
 * webkit_website_data_manager_get_itp_directory:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get the #CyberKitWebsiteDataManager:itp-directory property.
 *
 * Returns: (allow-none): the directory where Intelligent Tracking Prevention data is stored or %NULL if @manager is ephemeral.
 *
 * Since: 2.30
 *
 * Deprecated: 2.40, use webkit_website_data_manager_get_base_data_directory() instead.
 */
const gchar* webkit_website_data_manager_get_itp_directory(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), nullptr);

    CyberKitWebsiteDataManagerPrivate* priv = manager->priv;
    if (priv->websiteDataStore && !priv->websiteDataStore->isPersistent())
        return nullptr;

    if (!priv->itpDirectory)
        priv->itpDirectory.reset(g_strdup(CyberKit::WebsiteDataStore::defaultResourceLoadStatisticsDirectory().utf8().data()));
    return priv->itpDirectory.get();
}

/**
 * webkit_website_data_manager_get_service_worker_registrations_directory:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get the #CyberKitWebsiteDataManager:service-worker-registrations-directory property.
 *
 * Returns: (allow-none): the directory where service worker registrations are stored or %NULL if @manager is ephemeral.
 *
 * Since: 2.30
 *
 * Deprecated: 2.40, use webkit_website_data_manager_get_base_data_directory() instead.
 */
const gchar* webkit_website_data_manager_get_service_worker_registrations_directory(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), nullptr);

    CyberKitWebsiteDataManagerPrivate* priv = manager->priv;
    if (priv->websiteDataStore && !priv->websiteDataStore->isPersistent())
        return nullptr;

    if (!priv->swRegistrationsDirectory)
        priv->swRegistrationsDirectory.reset(g_strdup(CyberKit::WebsiteDataStore::defaultServiceWorkerRegistrationDirectory().utf8().data()));
    return priv->swRegistrationsDirectory.get();
}

/**
 * webkit_website_data_manager_get_dom_cache_directory:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get the #CyberKitWebsiteDataManager:dom-cache-directory property.
 *
 * Returns: (allow-none): the directory where DOM cache is stored or %NULL if @manager is ephemeral.
 *
 * Since: 2.30
 *
 * Deprecated: 2.40, use webkit_website_data_manager_get_base_cache_directory() instead.
 */
const gchar* webkit_website_data_manager_get_dom_cache_directory(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), nullptr);

    CyberKitWebsiteDataManagerPrivate* priv = manager->priv;
    if (priv->websiteDataStore && !priv->websiteDataStore->isPersistent())
        return nullptr;

    if (!priv->domCacheDirectory)
        priv->domCacheDirectory.reset(g_strdup(CyberKit::WebsiteDataStore::defaultCacheStorageDirectory().utf8().data()));
    return priv->domCacheDirectory.get();
}

/**
 * webkit_website_data_manager_get_cookie_manager:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get the #CyberKitCookieManager of @manager.
 *
 * Returns: (transfer none): a #CyberKitCookieManager
 *
 * Since: 2.16
 */
CyberKitCookieManager* webkit_website_data_manager_get_cookie_manager(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), nullptr);

    if (!manager->priv->cookieManager)
        manager->priv->cookieManager = adoptGRef(webkitCookieManagerCreate(manager));

    return manager->priv->cookieManager.get();
}

/**
 * webkit_website_data_manager_set_itp_enabled:
 * @manager: a #CyberKitWebsiteDataManager
 * @enabled: value to set
 *
 * Enable or disable Intelligent Tracking Prevention (ITP).
 *
 * When ITP is enabled resource load statistics
 * are collected and used to decide whether to allow or block third-party cookies and prevent user tracking.
 * Note that while ITP is enabled the accept policy %WEBKIT_COOKIE_POLICY_ACCEPT_NO_THIRD_PARTY is ignored and
 * %WEBKIT_COOKIE_POLICY_ACCEPT_ALWAYS is used instead. See also webkit_cookie_manager_set_accept_policy().
 *
 * Since: 2.30
 */
void webkit_website_data_manager_set_itp_enabled(CyberKitWebsiteDataManager* manager, gboolean enabled)
{
    g_return_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager));

    webkitWebsiteDataManagerGetDataStore(manager).setTrackingPreventionEnabled(enabled);
}

/**
 * webkit_website_data_manager_get_itp_enabled:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get whether Intelligent Tracking Prevention (ITP) is enabled or not.
 *
 * Returns: %TRUE if ITP is enabled, or %FALSE otherwise.
 *
 * Since: 2.30
 */
gboolean webkit_website_data_manager_get_itp_enabled(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), FALSE);

    return webkitWebsiteDataManagerGetDataStore(manager).trackingPreventionEnabled();
}

/**
 * webkit_website_data_manager_set_persistent_credential_storage_enabled:
 * @manager: a #CyberKitWebsiteDataManager
 * @enabled: value to set
 *
 * Enable or disable persistent credential storage.
 *
 * When enabled, which is the default for
 * non-ephemeral sessions, the network process will try to read and write HTTP authentiacation
 * credentials from persistent storage.
 *
 * Since: 2.30
 */
void webkit_website_data_manager_set_persistent_credential_storage_enabled(CyberKitWebsiteDataManager* manager, gboolean enabled)
{
    g_return_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager));

    webkitWebsiteDataManagerGetDataStore(manager).setPersistentCredentialStorageEnabled(enabled);
}

/**
 * webkit_website_data_manager_get_persistent_credential_storage_enabled:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get whether persistent credential storage is enabled or not.
 *
 * See also webkit_website_data_manager_set_persistent_credential_storage_enabled().
 *
 * Returns: %TRUE if persistent credential storage is enabled, or %FALSE otherwise.
 *
 * Since: 2.30
 */
gboolean webkit_website_data_manager_get_persistent_credential_storage_enabled(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), FALSE);

    return webkitWebsiteDataManagerGetDataStore(manager).persistentCredentialStorageEnabled();
}

/**
 * webkit_website_data_manager_set_tls_errors_policy:
 * @manager: a #CyberKitWebsiteDataManager
 * @policy: a #CyberKitTLSErrorsPolicy
 *
 * Set the TLS errors policy of @manager as @policy.
 *
 * Since: 2.32
 */
void webkit_website_data_manager_set_tls_errors_policy(CyberKitWebsiteDataManager* manager, CyberKitTLSErrorsPolicy policy)
{
    g_return_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager));

    if (manager->priv->tlsErrorsPolicy == policy)
        return;

    manager->priv->tlsErrorsPolicy = policy;
    webkitWebsiteDataManagerGetDataStore(manager).setIgnoreTLSErrors(policy == WEBKIT_TLS_ERRORS_POLICY_IGNORE);
}

/**
 * webkit_website_data_manager_get_tls_errors_policy:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get the TLS errors policy of @manager.
 *
 * Returns: a #CyberKitTLSErrorsPolicy
 *
 * Since: 2.32
 */
CyberKitTLSErrorsPolicy webkit_website_data_manager_get_tls_errors_policy(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), WEBKIT_TLS_ERRORS_POLICY_FAIL);

    return manager->priv->tlsErrorsPolicy;
}

/**
 * webkit_website_data_manager_set_network_proxy_settings:
 * @manager: a #CyberKitWebsiteDataManager
 * @proxy_mode: a #CyberKitNetworkProxyMode
 * @proxy_settings: (allow-none): a #CyberKitNetworkProxySettings, or %NULL
 *
 * Set the network proxy settings to be used by connections started in @manager session.
 *
 * By default %WEBKIT_NETWORK_PROXY_MODE_DEFAULT is used, which means that the
 * system settings will be used (g_proxy_resolver_get_default()).
 * If you want to override the system default settings, you can either use
 * %WEBKIT_NETWORK_PROXY_MODE_NO_PROXY to make sure no proxies are used at all,
 * or %WEBKIT_NETWORK_PROXY_MODE_CUSTOM to provide your own proxy settings.
 * When @proxy_mode is %WEBKIT_NETWORK_PROXY_MODE_CUSTOM @proxy_settings must be
 * a valid #CyberKitNetworkProxySettings; otherwise, @proxy_settings must be %NULL.
 *
 * Since: 2.32
 */
void webkit_website_data_manager_set_network_proxy_settings(CyberKitWebsiteDataManager* manager, CyberKitNetworkProxyMode proxyMode, CyberKitNetworkProxySettings* proxySettings)
{
    g_return_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager));
    g_return_if_fail((proxyMode != WEBKIT_NETWORK_PROXY_MODE_CUSTOM && !proxySettings) || (proxyMode == WEBKIT_NETWORK_PROXY_MODE_CUSTOM && proxySettings));

    auto& dataStore = webkitWebsiteDataManagerGetDataStore(manager);
    switch (proxyMode) {
    case WEBKIT_NETWORK_PROXY_MODE_DEFAULT:
        dataStore.setNetworkProxySettings({ });
        break;
    case WEBKIT_NETWORK_PROXY_MODE_NO_PROXY:
        dataStore.setNetworkProxySettings(CyberCore::SoupNetworkProxySettings(CyberCore::SoupNetworkProxySettings::Mode::NoProxy));
        break;
    case WEBKIT_NETWORK_PROXY_MODE_CUSTOM:
        auto settings = webkitNetworkProxySettingsGetNetworkProxySettings(proxySettings);
        if (settings.isEmpty()) {
            g_warning("Invalid attempt to set custom network proxy settings with an empty CyberKitNetworkProxySettings. Use "
                "WEBKIT_NETWORK_PROXY_MODE_NO_PROXY to not use any proxy or WEBKIT_NETWORK_PROXY_MODE_DEFAULT to use the default system settings");
        } else
            dataStore.setNetworkProxySettings(WTFMove(settings));
        break;
    }
}
#endif

#if PLATFORM(GTK) && ENABLE(2022_GLIB_API)
static String webkitWebsiteDataManagerGetFaviconDatabasePath(CyberKitWebsiteDataManager* manager)
{
    if (webkit_website_data_manager_is_ephemeral(manager))
        return { };

    if (!manager->priv->baseCacheDirectory.isNull())
        return FileSystem::pathByAppendingComponents(FileSystem::stringFromFileSystemRepresentation(manager->priv->baseCacheDirectory.data()), { "icondatabase"_s, "WebpageIcons.db"_s });

    return FileSystem::pathByAppendingComponents(WebsiteDataStore::defaultBaseCacheDirectory(), { "icondatabase"_s, "WebpageIcons.db"_s });
}

/**
 * webkit_website_data_manager_set_favicons_enabled:
 * @manager: a #CyberKitWebsiteDataManager
 * @enabled: value to set
 *
 * Set whether website icons are enabled. Website icons are disabled by default.
 * When website icons are disabled, the #CyberKitFaviconDatabase of @manager is closed and
 * its reference removed, so webkit_website_data_manager_get_favicon_database() will
 * return %NULL. If website icons are enabled again, a new #CyberKitFaviconDatabase will
 * be created.
 *
 * Since: 2.40
 */
void webkit_website_data_manager_set_favicons_enabled(CyberKitWebsiteDataManager* manager, gboolean enabled)
{
    g_return_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager));

    if (enabled) {
        if (!manager->priv->faviconDatabase)
            manager->priv->faviconDatabase = adoptGRef(webkitFaviconDatabaseCreate());
        auto path = webkitWebsiteDataManagerGetFaviconDatabasePath(manager);
        webkitFaviconDatabaseOpen(manager->priv->faviconDatabase.get(), path, path.isNull());
    } else if (manager->priv->faviconDatabase) {
        webkitFaviconDatabaseClose(manager->priv->faviconDatabase.get());
        manager->priv->faviconDatabase = nullptr;
    }
}

/**
 * webkit_website_data_manager_get_favicons_enabled:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get whether website icons are enabled.
 *
 * Returns: %TRUE if website icons are enabled, or %FALSE otherwise.
 *
 * Since: 2.40
 */
gboolean webkit_website_data_manager_get_favicons_enabled(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), FALSE);

    return !!manager->priv->faviconDatabase;
}

/**
 * webkit_website_data_manager_get_favicon_database:
 * @manager: a #CyberKitWebsiteDataManager
 *
 * Get the #CyberKitFaviconDatabase of @manager.
 *
 * Returns: (transfer none) (nullable): a #CyberKitFaviconDatabase, or %NULL if website icons are disabled
 *
 * Since: 2.40
 */
CyberKitFaviconDatabase* webkit_website_data_manager_get_favicon_database(CyberKitWebsiteDataManager* manager)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), nullptr);

    return manager->priv->faviconDatabase.get();
}
#endif

static OptionSet<WebsiteDataType> toWebsiteDataTypes(CyberKitWebsiteDataTypes types)
{
    OptionSet<WebsiteDataType> returnValue;
    if (types & WEBKIT_WEBSITE_DATA_MEMORY_CACHE)
        returnValue.add(WebsiteDataType::MemoryCache);
    if (types & WEBKIT_WEBSITE_DATA_DISK_CACHE)
        returnValue.add(WebsiteDataType::DiskCache);
    if (types & WEBKIT_WEBSITE_DATA_OFFLINE_APPLICATION_CACHE)
        returnValue.add(WebsiteDataType::OfflineWebApplicationCache);
    if (types & WEBKIT_WEBSITE_DATA_SESSION_STORAGE)
        returnValue.add(WebsiteDataType::SessionStorage);
    if (types & WEBKIT_WEBSITE_DATA_LOCAL_STORAGE)
        returnValue.add(WebsiteDataType::LocalStorage);
#if !ENABLE(2022_GLIB_API)
    if (types & WEBKIT_WEBSITE_DATA_WEBSQL_DATABASES)
        returnValue.add(WebsiteDataType::WebSQLDatabases);
#endif
    if (types & WEBKIT_WEBSITE_DATA_INDEXEDDB_DATABASES)
        returnValue.add(WebsiteDataType::IndexedDBDatabases);
    if (types & WEBKIT_WEBSITE_DATA_HSTS_CACHE)
        returnValue.add(WebsiteDataType::HSTSCache);
    if (types & WEBKIT_WEBSITE_DATA_COOKIES)
        returnValue.add(WebsiteDataType::Cookies);
    if (types & WEBKIT_WEBSITE_DATA_DEVICE_ID_HASH_SALT)
        returnValue.add(WebsiteDataType::DeviceIdHashSalt);
    if (types & WEBKIT_WEBSITE_DATA_ITP)
        returnValue.add(WebsiteDataType::ResourceLoadStatistics);
#if ENABLE(SERVICE_WORKER)
    if (types & WEBKIT_WEBSITE_DATA_SERVICE_WORKER_REGISTRATIONS)
        returnValue.add(WebsiteDataType::ServiceWorkerRegistrations);
#endif
    if (types & WEBKIT_WEBSITE_DATA_DOM_CACHE)
        returnValue.add(WebsiteDataType::DOMCache);
    return returnValue;
}

/**
 * webkit_website_data_manager_fetch:
 * @manager: a #CyberKitWebsiteDataManager
 * @types: #CyberKitWebsiteDataTypes
 * @cancellable: (allow-none): a #GCancellable or %NULL to ignore
 * @callback: (scope async): a #GAsyncReadyCallback to call when the request is satisfied
 * @user_data: (closure): the data to pass to callback function
 *
 * Asynchronously get the list of #CyberKitWebsiteData for the given @types.
 *
 * When the operation is finished, @callback will be called. You can then call
 * webkit_website_data_manager_fetch_finish() to get the result of the operation.
 *
 * Since: 2.16
 */
void webkit_website_data_manager_fetch(CyberKitWebsiteDataManager* manager, CyberKitWebsiteDataTypes types, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer userData)
{
    g_return_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager));

    GRefPtr<GTask> task = adoptGRef(g_task_new(manager, cancellable, callback, userData));
    manager->priv->websiteDataStore->fetchData(toWebsiteDataTypes(types), WebsiteDataFetchOption::ComputeSizes, [task = WTFMove(task)] (Vector<WebsiteDataRecord> records) {
        GList* dataList = nullptr;
        while (!records.isEmpty()) {
            if (auto* data = webkitWebsiteDataCreate(records.takeLast()))
                dataList = g_list_prepend(dataList, data);
        }

        g_task_return_pointer(task.get(), dataList, [](gpointer data) {
            g_list_free_full(static_cast<GList*>(data), reinterpret_cast<GDestroyNotify>(webkit_website_data_unref));
        });
    });
}

/**
 * webkit_website_data_manager_fetch_finish:
 * @manager: a #CyberKitWebsiteDataManager
 * @result: a #GAsyncResult
 * @error: return location for error or %NULL to ignore
 *
 * Finish an asynchronous operation started with webkit_website_data_manager_fetch().
 *
 * Returns: (element-type CyberKitWebsiteData) (transfer full): a #GList of #CyberKitWebsiteData. You must free the #GList with
 *    g_list_free() and unref the #CyberKitWebsiteData<!-- -->s with webkit_website_data_unref() when you're done with them.
 *
 * Since: 2.16
 */
GList* webkit_website_data_manager_fetch_finish(CyberKitWebsiteDataManager* manager, GAsyncResult* result, GError** error)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), nullptr);
    g_return_val_if_fail(g_task_is_valid(result, manager), nullptr);

    return static_cast<GList*>(g_task_propagate_pointer(G_TASK(result), error));
}

/**
 * webkit_website_data_manager_remove:
 * @manager: a #CyberKitWebsiteDataManager
 * @types: #CyberKitWebsiteDataTypes
 * @website_data: (element-type CyberKitWebsiteData): a #GList of #CyberKitWebsiteData
 * @cancellable: (allow-none): a #GCancellable or %NULL to ignore
 * @callback: (scope async): a #GAsyncReadyCallback to call when the request is satisfied
 * @user_data: (closure): the data to pass to callback function
 *
 * Asynchronously removes the website data in the given @website_data list.
 *
 * Asynchronously removes the website data of the given @types for websites in the given @website_data list.
 * Use webkit_website_data_manager_clear() if you want to remove the website data for all sites.
 *
 * When the operation is finished, @callback will be called. You can then call
 * webkit_website_data_manager_remove_finish() to get the result of the operation.
 *
 * Since: 2.16
 */
void webkit_website_data_manager_remove(CyberKitWebsiteDataManager* manager, CyberKitWebsiteDataTypes types, GList* websiteData, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer userData)
{
    g_return_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager));
    g_return_if_fail(websiteData);

    // We have to remove the hash salts when cookies are removed.
    if (types & WEBKIT_WEBSITE_DATA_COOKIES)
        types = static_cast<CyberKitWebsiteDataTypes>(types | WEBKIT_WEBSITE_DATA_DEVICE_ID_HASH_SALT);

    Vector<WebsiteDataRecord> records;
    for (GList* item = websiteData; item; item = g_list_next(item)) {
        CyberKitWebsiteData* data = static_cast<CyberKitWebsiteData*>(item->data);

        if (webkit_website_data_get_types(data) & types)
            records.append(webkitWebsiteDataGetRecord(data));
    }

    GRefPtr<GTask> task = adoptGRef(g_task_new(manager, cancellable, callback, userData));
    if (records.isEmpty()) {
        g_task_return_boolean(task.get(), TRUE);
        return;
    }

    manager->priv->websiteDataStore->removeData(toWebsiteDataTypes(types), records, [task = WTFMove(task)] {
        g_task_return_boolean(task.get(), TRUE);
    });
}

/**
 * webkit_website_data_manager_remove_finish:
 * @manager: a #CyberKitWebsiteDataManager
 * @result: a #GAsyncResult
 * @error: return location for error or %NULL to ignore
 *
 * Finish an asynchronous operation started with webkit_website_data_manager_remove().
 *
 * Returns: %TRUE if website data resources were successfully removed, or %FALSE otherwise.
 *
 * Since: 2.16
 */
gboolean webkit_website_data_manager_remove_finish(CyberKitWebsiteDataManager* manager, GAsyncResult* result, GError** error)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), FALSE);
    g_return_val_if_fail(g_task_is_valid(result, manager), FALSE);

    return g_task_propagate_boolean(G_TASK(result), error);
}

/**
 * webkit_website_data_manager_clear:
 * @manager: a #CyberKitWebsiteDataManager
 * @types: #CyberKitWebsiteDataTypes
 * @timespan: a #GTimeSpan
 * @cancellable: (allow-none): a #GCancellable or %NULL to ignore
 * @callback: (scope async): a #GAsyncReadyCallback to call when the request is satisfied
 * @user_data: (closure): the data to pass to callback function
 *
 * Asynchronously clear the website data of the given @types modified in the past @timespan.
 *
 * If @timespan is 0, all website data will be removed.
 *
 * When the operation is finished, @callback will be called. You can then call
 * webkit_website_data_manager_clear_finish() to get the result of the operation.
 *
 * Due to implementation limitations, this function does not currently delete
 * any stored cookies if @timespan is nonzero. This behavior may change in the
 * future.
 *
 * Since: 2.16
 */
void webkit_website_data_manager_clear(CyberKitWebsiteDataManager* manager, CyberKitWebsiteDataTypes types, GTimeSpan timeSpan, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer userData)
{
    g_return_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager));

    WallTime timePoint = timeSpan ? WallTime::now() - Seconds::fromMicroseconds(timeSpan) : WallTime::fromRawSeconds(0);
    GRefPtr<GTask> task = adoptGRef(g_task_new(manager, cancellable, callback, userData));
    manager->priv->websiteDataStore->removeData(toWebsiteDataTypes(types), timePoint, [task = WTFMove(task)] {
        g_task_return_boolean(task.get(), TRUE);
    });
}

/**
 * webkit_website_data_manager_clear_finish:
 * @manager: a #CyberKitWebsiteDataManager
 * @result: a #GAsyncResult
 * @error: return location for error or %NULL to ignore
 *
 * Finish an asynchronous operation started with webkit_website_data_manager_clear()
 *
 * Returns: %TRUE if website data was successfully cleared, or %FALSE otherwise.
 *
 * Since: 2.16
 */
gboolean webkit_website_data_manager_clear_finish(CyberKitWebsiteDataManager* manager, GAsyncResult* result, GError** error)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), FALSE);
    g_return_val_if_fail(g_task_is_valid(result, manager), FALSE);

    return g_task_propagate_boolean(G_TASK(result), error);
}

/**
 * CyberKitITPFirstParty: (ref-func webkit_itp_first_party_ref) (unref-func webkit_itp_first_party_unref)
 *
 * Describes a first party origin.
 *
 * Since: 2.30
 */
struct _CyberKitITPFirstParty {
    explicit _CyberKitITPFirstParty(WebResourceLoadStatisticsStore::ThirdPartyDataForSpecificFirstParty&& data)
        : domain(data.firstPartyDomain.string().utf8())
        , storageAccessGranted(data.storageAccessGranted)
        , lastUpdated(adoptGRef(g_date_time_new_from_unix_utc(data.timeLastUpdated.secondsAs<gint64>())))
    {
    }

    CString domain;
    bool storageAccessGranted { false };
    GRefPtr<GDateTime> lastUpdated;
    int referenceCount { 1 };
};

#if !ENABLE(2022_GLIB_API)
/**
 * webkit_website_data_manager_set_memory_pressure_settings:
 * @settings: a CyberKitMemoryPressureSettings.
 *
 * Sets @settings as the #CyberKitMemoryPressureSettings.
 *
 * Sets @settings as the #CyberKitMemoryPressureSettings to be used by all the network
 * processes created by any instance of #CyberKitWebsiteDataManager after this function
 * is called.
 *
 * Be sure to call this function before creating any #CyberKitWebsiteDataManager, as network
 * processes of existing instances are not guaranteed to receive the passed settings.
 *
 * The periodic check for used memory is disabled by default on network processes. This will
 * be enabled only if custom settings have been set using this function. After that, in order
 * to remove the custom settings and disable the periodic check, this function must be called
 * passing %NULL as the value of @settings.
 *
 * Since: 2.34
 */
void webkit_website_data_manager_set_memory_pressure_settings(CyberKitMemoryPressureSettings* settings)
{
    std::optional<MemoryPressureHandler::Configuration> config = settings ? std::make_optional(webkitMemoryPressureSettingsGetMemoryPressureHandlerConfiguration(settings)) : std::nullopt;
    WebProcessPool::setNetworkProcessMemoryPressureHandlerConfiguration(config);
}
#endif

G_DEFINE_BOXED_TYPE(CyberKitITPFirstParty, webkit_itp_first_party, webkit_itp_first_party_ref, webkit_itp_first_party_unref)

static CyberKitITPFirstParty* webkitITPFirstPartyCreate(WebResourceLoadStatisticsStore::ThirdPartyDataForSpecificFirstParty&& data)
{
    auto* firstParty = static_cast<CyberKitITPFirstParty*>(fastMalloc(sizeof(CyberKitITPFirstParty)));
    new (firstParty) CyberKitITPFirstParty(WTFMove(data));
    return firstParty;
}


/**
 * webkit_itp_first_party_ref:
 * @itp_first_party: a #CyberKitITPFirstParty
 *
 * Atomically increments the reference count of @itp_first_party by one.
 *
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: The passed #CyberKitITPFirstParty
 *
 * Since: 2.30
 */
CyberKitITPFirstParty* webkit_itp_first_party_ref(CyberKitITPFirstParty* firstParty)
{
    g_return_val_if_fail(firstParty, nullptr);

    g_atomic_int_inc(&firstParty->referenceCount);
    return firstParty;
}

/**
 * webkit_itp_first_party_unref:
 * @itp_first_party: a #CyberKitITPFirstParty
 *
 * Atomically decrements the reference count of @itp_first_party by one.
 *
 * If the reference count drops to 0, all memory allocated by
 * #CyberKitITPFirstParty is released. This function is MT-safe and may be
 * called from any thread.
 *
 * Since: 2.30
 */
void webkit_itp_first_party_unref(CyberKitITPFirstParty* firstParty)
{
    g_return_if_fail(firstParty);

    if (g_atomic_int_dec_and_test(&firstParty->referenceCount)) {
        firstParty->~CyberKitITPFirstParty();
        fastFree(firstParty);
    }
}

/**
 * webkit_itp_first_party_get_domain:
 * @itp_first_party: a #CyberKitITPFirstParty
 *
 * Get the domain name of @itp_first_party.
 *
 * Returns: the domain name
 *
 * Since: 2.30
 */
const char* webkit_itp_first_party_get_domain(CyberKitITPFirstParty* firstParty)
{
    g_return_val_if_fail(firstParty, nullptr);

    return firstParty->domain.data();
}

/**
 * webkit_itp_first_party_get_website_data_access_allowed:
 * @itp_first_party: a #CyberKitITPFirstParty
 *
 * Get whether @itp_first_party has granted website data access to its #CyberKitITPThirdParty.
 *
 * Each @CyberKitITPFirstParty is created by webkit_itp_third_party_get_first_parties() and
 * therefore corresponds to exactly one #CyberKitITPThirdParty.
 *
 * Returns: %TRUE if website data access has been granted, or %FALSE otherwise
 *
 * Since: 2.30
 */
gboolean webkit_itp_first_party_get_website_data_access_allowed(CyberKitITPFirstParty* firstParty)
{
    g_return_val_if_fail(firstParty, FALSE);

    return firstParty->storageAccessGranted;
}

/**
 * webkit_itp_first_party_get_last_update_time:
 * @itp_first_party: a #CyberKitITPFirstParty
 *
 * Get the last time a #CyberKitITPThirdParty has been seen under @itp_first_party.
 *
 * Each @CyberKitITPFirstParty is created by webkit_itp_third_party_get_first_parties() and
 * therefore corresponds to exactly one #CyberKitITPThirdParty.
 *
 * Returns: (transfer none): the last update time as a #GDateTime
 *
 * Since: 2.30
 */
GDateTime* webkit_itp_first_party_get_last_update_time(CyberKitITPFirstParty* firstParty)
{
    g_return_val_if_fail(firstParty, nullptr);

    return firstParty->lastUpdated.get();
}

/**
 * CyberKitITPThirdParty: (ref-func webkit_itp_first_party_ref) (unref-func webkit_itp_first_party_unref)
 *
 * Describes a third party origin.
 *
 * Since: 2.30
 */

struct _CyberKitITPThirdParty {
    explicit _CyberKitITPThirdParty(WebResourceLoadStatisticsStore::ThirdPartyData&& data)
        : domain(data.thirdPartyDomain.string().utf8())
    {
        while (!data.underFirstParties.isEmpty())
            firstParties = g_list_prepend(firstParties, webkitITPFirstPartyCreate(data.underFirstParties.takeLast()));
    }

    ~_CyberKitITPThirdParty()
    {
        g_list_free_full(firstParties, reinterpret_cast<GDestroyNotify>(webkit_itp_first_party_unref));
    }

    CString domain;
    GList* firstParties { nullptr };
    int referenceCount { 1 };
};

G_DEFINE_BOXED_TYPE(CyberKitITPThirdParty, webkit_itp_third_party, webkit_itp_third_party_ref, webkit_itp_third_party_unref)

CyberKitITPThirdParty* webkitITPThirdPartyCreate(WebResourceLoadStatisticsStore::ThirdPartyData&& data)
{
    auto* thirdParty = static_cast<CyberKitITPThirdParty*>(fastMalloc(sizeof(CyberKitITPThirdParty)));
    new (thirdParty) CyberKitITPThirdParty(WTFMove(data));
    return thirdParty;
}

/**
 * webkit_itp_third_party_ref:
 * @itp_third_party: a #CyberKitITPThirdParty
 *
 * Atomically increments the reference count of @itp_third_party by one.
 *
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: The passed #CyberKitITPThirdParty
 *
 * Since: 2.30
 */
CyberKitITPThirdParty* webkit_itp_third_party_ref(CyberKitITPThirdParty* thirdParty)
{
    g_return_val_if_fail(thirdParty, nullptr);

    g_atomic_int_inc(&thirdParty->referenceCount);
    return thirdParty;
}

/**
 * webkit_itp_third_party_unref:
 * @itp_third_party: a #CyberKitITPThirdParty
 *
 * Atomically decrements the reference count of @itp_third_party by one.
 *
 * If the reference count drops to 0, all memory allocated by
 * #CyberKitITPThirdParty is released. This function is MT-safe and may be
 * called from any thread.
 *
 * Since: 2.30
 */
void webkit_itp_third_party_unref(CyberKitITPThirdParty* thirdParty)
{
    g_return_if_fail(thirdParty);

    if (g_atomic_int_dec_and_test(&thirdParty->referenceCount)) {
        thirdParty->~CyberKitITPThirdParty();
        fastFree(thirdParty);
    }
}

/**
 * webkit_itp_third_party_get_domain:
 * @itp_third_party: a #CyberKitITPThirdParty
 *
 * Get the domain name of @itp_third_party.
 *
 * Returns: the domain name
 *
 * Since: 2.30
 */
const char* webkit_itp_third_party_get_domain(CyberKitITPThirdParty* thirdParty)
{
    g_return_val_if_fail(thirdParty, nullptr);

    return thirdParty->domain.data();
}

/**
 * webkit_itp_third_party_get_first_parties:
 * @itp_third_party: a #CyberKitITPThirdParty
 *
 * Get the list of #CyberKitITPFirstParty under which @itp_third_party has been seen.
 *
 * Returns: (transfer none) (element-type CyberKitITPFirstParty): a #GList of #CyberKitITPFirstParty
 *
 * Since: 2.30
 */
GList* webkit_itp_third_party_get_first_parties(CyberKitITPThirdParty* thirdParty)
{
    g_return_val_if_fail(thirdParty, nullptr);

    return thirdParty->firstParties;
}

/**
 * webkit_website_data_manager_get_itp_summary:
 * @manager: a #CyberKitWebsiteDataManager
 * @cancellable: (allow-none): a #GCancellable or %NULL to ignore
 * @callback: (scope async): a #GAsyncReadyCallback to call when the request is satisfied
 * @user_data: (closure): the data to pass to callback function
 *
 * Asynchronously get the list of #CyberKitITPThirdParty seen for @manager.
 *
 * Every #CyberKitITPThirdParty
 * contains the list of #CyberKitITPFirstParty under which it has been seen.
 *
 * When the operation is finished, @callback will be called. You can then call
 * webkit_website_data_manager_get_itp_summary_finish() to get the result of the operation.
 *
 * Since: 2.30
 */
void webkit_website_data_manager_get_itp_summary(CyberKitWebsiteDataManager* manager, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer userData)
{
    g_return_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager));

    GRefPtr<GTask> task = adoptGRef(g_task_new(manager, cancellable, callback, userData));
    manager->priv->websiteDataStore->getResourceLoadStatisticsDataSummary([task = WTFMove(task)](Vector<WebResourceLoadStatisticsStore::ThirdPartyData>&& thirdPartyList) {
        GList* result = nullptr;
        while (!thirdPartyList.isEmpty())
            result = g_list_prepend(result, webkitITPThirdPartyCreate(thirdPartyList.takeLast()));
        g_task_return_pointer(task.get(), result, [](gpointer data) {
            g_list_free_full(static_cast<GList*>(data), reinterpret_cast<GDestroyNotify>(webkit_itp_third_party_unref));
        });
    });
}

/**
 * webkit_website_data_manager_get_itp_summary_finish:
 * @manager: a #CyberKitWebsiteDataManager
 * @result: a #GAsyncResult
 * @error: return location for error or %NULL to ignore
 *
 * Finish an asynchronous operation started with webkit_website_data_manager_get_itp_summary().
 *
 * Returns: (transfer full) (element-type CyberKitITPThirdParty): a #GList of #CyberKitITPThirdParty.
 *    You must free the #GList with g_list_free() and unref the #CyberKitITPThirdParty<!-- -->s with
 *    webkit_itp_third_party_unref() when you're done with them.
 *
 * Since: 2.30
 */
GList* webkit_website_data_manager_get_itp_summary_finish(CyberKitWebsiteDataManager* manager, GAsyncResult* result, GError** error)
{
    g_return_val_if_fail(WEBKIT_IS_WEBSITE_DATA_MANAGER(manager), nullptr);
    g_return_val_if_fail(g_task_is_valid(result, manager), nullptr);

    return static_cast<GList*>(g_task_propagate_pointer(G_TASK(result), error));
}
