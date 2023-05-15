/*
 * Copyright (C) 2023 Igalia S.L.
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

#if ENABLE(2022_GLIB_API)

#include "config.h"
#include "CyberKitNetworkSession.h"

#include "APIDownloadClient.h"
#include "FrameInfoData.h"
#include "NetworkProcessMessages.h"
#include "CyberKitCookieManagerPrivate.h"
#include "CyberKitDownloadPrivate.h"
#include "CyberKitInitialize.h"
#include "CyberKitMemoryPressureSettings.h"
#include "CyberKitMemoryPressureSettingsPrivate.h"
#include "CyberKitNetworkProxySettingsPrivate.h"
#include "CyberKitPrivate.h"
#include "CyberKitWebsiteDataManagerPrivate.h"
#include "WebProcessPool.h"
#include <glib/gi18n-lib.h>
#include <pal/HysteresisActivity.h>
#include <wtf/HashSet.h>
#include <wtf/glib/GRefPtr.h>
#include <wtf/glib/GUniquePtr.h>
#include <wtf/glib/WTFGType.h>

using namespace CyberKit;

/**
 * CyberKitNetworkSession:
 * @see_also: #CyberKitWebContext, #CyberKitWebsiteData
 *
 * Manages network configuration.
 *
 *
 * Since: 2.40
 */

using namespace CyberKit;

enum {
    PROP_0,

    PROP_DATA_DIRECTORY,
    PROP_CACHE_DIRECTORY,
    PROP_IS_EPHEMERAL,

    N_PROPERTIES
};

static GParamSpec* sObjProperties[N_PROPERTIES] = { nullptr, };

enum {
    DOWNLOAD_STARTED,

    LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0, };

struct _CyberKitNetworkSessionPrivate {
    _CyberKitNetworkSessionPrivate()
        : dnsPrefetchHystereris([this](PAL::HysteresisState state) {
            if (state == PAL::HysteresisState::Stopped)
                dnsPrefetchedHosts.clear();
        })
    {
    }

    GRefPtr<CyberKitWebsiteDataManager> websiteDataManager;
    GRefPtr<CyberKitCookieManager> cookieManager;
    CyberKitTLSErrorsPolicy tlsErrorsPolicy;

    CString dataDirectory;
    CString cacheDirectory;

    HashSet<String> dnsPrefetchedHosts;
    PAL::HysteresisActivity dnsPrefetchHystereris;
};

WEBKIT_DEFINE_FINAL_TYPE(CyberKitNetworkSession, webkit_network_session, G_TYPE_OBJECT, GObject)

static void webkitNetworkSessionGetProperty(GObject* object, guint propID, GValue* value, GParamSpec* paramSpec)
{
    CyberKitNetworkSession* session = WEBKIT_NETWORK_SESSION(object);

    switch (propID) {
    case PROP_IS_EPHEMERAL:
        g_value_set_boolean(value, webkit_network_session_is_ephemeral(session));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propID, paramSpec);
    }
}

static void webkitNetworkSessionSetProperty(GObject* object, guint propID, const GValue* value, GParamSpec* paramSpec)
{
    CyberKitNetworkSession* session = WEBKIT_NETWORK_SESSION(object);

    switch (propID) {
    case PROP_DATA_DIRECTORY:
        session->priv->dataDirectory = g_value_get_string(value);
        break;
    case PROP_CACHE_DIRECTORY:
        session->priv->cacheDirectory = g_value_get_string(value);
        break;
    case PROP_IS_EPHEMERAL:
        if (g_value_get_boolean(value))
            session->priv->websiteDataManager = adoptGRef(WEBKIT_WEBSITE_DATA_MANAGER(g_object_new(WEBKIT_TYPE_WEBSITE_DATA_MANAGER, "is-ephemeral", TRUE, nullptr)));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propID, paramSpec);
    }
}

static void webkitNetworkSessionConstructed(GObject* object)
{
    G_OBJECT_CLASS(webkit_network_session_parent_class)->constructed(object);

    CyberKitNetworkSessionPrivate* priv = WEBKIT_NETWORK_SESSION(object)->priv;
    if (!priv->websiteDataManager) {
        priv->websiteDataManager = adoptGRef(webkitWebsiteDataManagerCreate(
            !priv->dataDirectory.isNull() ? WTFMove(priv->dataDirectory) : WebsiteDataStore::defaultBaseDataDirectory().utf8(),
            !priv->cacheDirectory.isNull() ? WTFMove(priv->cacheDirectory) : WebsiteDataStore::defaultBaseCacheDirectory().utf8())
        );
    }

    priv->tlsErrorsPolicy = WEBKIT_TLS_ERRORS_POLICY_FAIL;
    webkitWebsiteDataManagerGetDataStore(priv->websiteDataManager.get()).setIgnoreTLSErrors(false);
}

static void webkit_network_session_class_init(CyberKitNetworkSessionClass* sessionClass)
{
    webkitInitialize();

    GObjectClass* gObjectClass = G_OBJECT_CLASS(sessionClass);
    gObjectClass->get_property = webkitNetworkSessionGetProperty;
    gObjectClass->set_property = webkitNetworkSessionSetProperty;
    gObjectClass->constructed = webkitNetworkSessionConstructed;

    /**
     * CyberKitNetworkSession:data-directory:
     *
     * The base data directory used to create the #CyberKitWebsiteDataManager. If %NULL, a default location will be used.
     *
     * Since: 2.40
     */
    sObjProperties[PROP_DATA_DIRECTORY] =
        g_param_spec_string(
            "data-directory",
            nullptr, nullptr,
            nullptr,
            static_cast<GParamFlags>(WEBKIT_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

    /**
     * CyberKitNetworkSession:cache-directory:
     *
     * The base caches directory used to create the #CyberKitWebsiteDataManager. If %NULL, a default location will be used.
     *
     * Since: 2.40
     */
    sObjProperties[PROP_CACHE_DIRECTORY] =
        g_param_spec_string(
            "cache-directory",
            nullptr, nullptr,
            nullptr,
            static_cast<GParamFlags>(WEBKIT_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

    /**
     * CyberKitNetworkSession:is-ephemeral:
     *
     * Whether to create an ephermeral #CyberKitWebsiteDataManager for the session.
     *
     * Since: 2.40
     */
    sObjProperties[PROP_IS_EPHEMERAL] =
        g_param_spec_boolean(
            "is-ephemeral",
            nullptr, nullptr,
            FALSE,
            static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

    g_object_class_install_properties(gObjectClass, N_PROPERTIES, sObjProperties);

    /**
     * CyberKitNetworkSession::download-started:
     * @session: the #CyberKitNetworkSession
     * @download: the #CyberKitDownload associated with this event
     *
     * This signal is emitted when a new download request is made.
     *
     * Since: 2.40
     */
    signals[DOWNLOAD_STARTED] =
        g_signal_new("download-started",
            G_TYPE_FROM_CLASS(gObjectClass),
            G_SIGNAL_RUN_LAST,
            0, nullptr, nullptr,
            nullptr,
            G_TYPE_NONE, 1,
            WEBKIT_TYPE_DOWNLOAD);
}

void webkitNetworkSessionDownloadStarted(CyberKitNetworkSession* session, CyberKitDownload* download)
{
    g_signal_emit(session, signals[DOWNLOAD_STARTED], 0, download);
}

static gpointer createDefaultNetworkSession(gpointer)
{
    static GRefPtr<CyberKitNetworkSession> session = adoptGRef(webkit_network_session_new(nullptr, nullptr));
    return session.get();
}

/**
 * webkit_network_session_get_default:
 *
 * Get the default network session.
 * The default network session is created using webkit_network_session_new() and passing
 * %NULL as data and cache directories.
 *
 * Returns: (transfer none): a #CyberKitNetworkSession
 *
 * Since: 2.40
 */
CyberKitNetworkSession* webkit_network_session_get_default()
{
    static GOnce onceInit = G_ONCE_INIT;
    return WEBKIT_NETWORK_SESSION(g_once(&onceInit, createDefaultNetworkSession, nullptr));
}

/**
 * webkit_network_session_new:
 * @data_directory: (nullable): a base directory for data, or %NULL
 * @cache_directory: (nullable): a base directory for caches, or %NULL
 *
 * Creates a new #CyberKitNetworkSession with a persistent #CyberKitWebsiteDataManager.
 * The parameters @data_directory and @cache_directory will be used as construct
 * properties of the #CyberKitWebsiteDataManager of the network session. Note that if
 * %NULL is passed, the default directory will be passed to #CyberKitWebsiteDataManager
 * so that webkit_website_data_manager_get_base_data_directory() and
 * webkit_website_data_manager_get_base_cache_directory() always return a value for
 * non ephemeral sessions.
 *
 * It must be passed as construct parameter of a #CyberKitWebView.
 *
 * Returns: (transfer full): the newly created #CyberKitNetworkSession
 *
 * Since: 2.40
 */
CyberKitNetworkSession* webkit_network_session_new(const char* dataDirectory, const char* cacheDirectory)
{
    return WEBKIT_NETWORK_SESSION(g_object_new(WEBKIT_TYPE_NETWORK_SESSION, "data-directory", dataDirectory, "cache-directory", cacheDirectory, nullptr));
}

/**
 * webkit_network_session_new_ephemeral:
 *
 * Creates a new #CyberKitNetworkSession with an ephemeral #CyberKitWebsiteDataManager.
 *
 *
 * Returns: (transfer full): a new ephemeral #CyberKitNetworkSession.
 *
 * Since: 2.40
 */
CyberKitNetworkSession* webkit_network_session_new_ephemeral()
{
    return WEBKIT_NETWORK_SESSION(g_object_new(WEBKIT_TYPE_NETWORK_SESSION, "is-ephemeral", TRUE, nullptr));
}

/**
 * webkit_network_session_is_ephemeral:
 * @session: a #CyberKitNetworkSession
 *
 * Get whether @session is ephemeral.
 * A #CyberKitNetworkSession is ephemeral when its #CyberKitWebsiteDataManager is ephemeral.
 * See #CyberKitWebsiteDataManager:is-ephemeral for more details.
 *
 * Returns: %TRUE if @session is pehmeral, or %FALSE otherwise
 *
 * Since: 2.40
 */
gboolean webkit_network_session_is_ephemeral(CyberKitNetworkSession* session)
{
    g_return_val_if_fail(WEBKIT_IS_NETWORK_SESSION(session), FALSE);

    return webkit_website_data_manager_is_ephemeral(session->priv->websiteDataManager.get());
}

/**
 * webkit_network_session_get_website_data_manager:
 * @session: a #CyberKitNetworkSession
 *
 * Get the #CyberKitWebsiteDataManager of @session.
 *
 * Returns: (transfer none): a #CyberKitWebsiteDataManager
 *
 * Since: 2.40
 */
CyberKitWebsiteDataManager* webkit_network_session_get_website_data_manager(CyberKitNetworkSession* session)
{
    g_return_val_if_fail(WEBKIT_IS_NETWORK_SESSION(session), nullptr);

    return session->priv->websiteDataManager.get();
}

/**
 * webkit_network_session_get_cookie_manager:
 * @session: a #CyberKitNetworkSession
 *
 * Get the #CyberKitCookieManager of @session.
 *
 * Returns: (transfer none): a #CyberKitCookieManager
 *
 * Since: 2.40
 */
CyberKitCookieManager* webkit_network_session_get_cookie_manager(CyberKitNetworkSession* session)
{
    g_return_val_if_fail(WEBKIT_IS_NETWORK_SESSION(session), nullptr);

    if (!session->priv->cookieManager)
        session->priv->cookieManager = adoptGRef(webkitCookieManagerCreate(session->priv->websiteDataManager.get()));

    return session->priv->cookieManager.get();
}

/**
 * webkit_network_session_set_itp_enabled:
 * @session: a #CyberKitNetworkSession
 * @enabled: value to set
 *
 * Enable or disable Intelligent Tracking Prevention (ITP).
 *
 * When ITP is enabled resource load statistics
 * are collected and used to decide whether to allow or block third-party cookies and prevent user tracking.
 * Note that while ITP is enabled the accept policy %WEBKIT_COOKIE_POLICY_ACCEPT_NO_THIRD_PARTY is ignored and
 * %WEBKIT_COOKIE_POLICY_ACCEPT_ALWAYS is used instead. See also webkit_cookie_session_set_accept_policy().
 *
 * Since: 2.40
 */
void webkit_network_session_set_itp_enabled(CyberKitNetworkSession* session, gboolean enabled)
{
    g_return_if_fail(WEBKIT_IS_NETWORK_SESSION(session));

    auto& websiteDataStore = webkitWebsiteDataManagerGetDataStore(session->priv->websiteDataManager.get());
    websiteDataStore.setTrackingPreventionEnabled(enabled);
}

/**
 * webkit_network_session_get_itp_enabled:
 * @session: a #CyberKitNetworkSession
 *
 * Get whether Intelligent Tracking Prevention (ITP) is enabled or not.
 *
 * Returns: %TRUE if ITP is enabled, or %FALSE otherwise.
 *
 * Since: 2.40
 */
gboolean webkit_network_session_get_itp_enabled(CyberKitNetworkSession* session)
{
    g_return_val_if_fail(WEBKIT_IS_NETWORK_SESSION(session), FALSE);

    auto& websiteDataStore = webkitWebsiteDataManagerGetDataStore(session->priv->websiteDataManager.get());
    return websiteDataStore.trackingPreventionEnabled();
}

/**
 * webkit_network_session_set_persistent_credential_storage_enabled:
 * @session: a #CyberKitNetworkSession
 * @enabled: value to set
 *
 * Enable or disable persistent credential storage.
 *
 * When enabled, which is the default for
 * non-ephemeral sessions, the network process will try to read and write HTTP authentiacation
 * credentials from persistent storage.
 *
 * Since: 2.40
 */
void webkit_network_session_set_persistent_credential_storage_enabled(CyberKitNetworkSession* session, gboolean enabled)
{
    g_return_if_fail(WEBKIT_IS_NETWORK_SESSION(session));

    auto& websiteDataStore = webkitWebsiteDataManagerGetDataStore(session->priv->websiteDataManager.get());
    websiteDataStore.setPersistentCredentialStorageEnabled(enabled);
}

/**
 * webkit_network_session_get_persistent_credential_storage_enabled:
 * @session: a #CyberKitNetworkSession
 *
 * Get whether persistent credential storage is enabled or not.
 *
 * See also webkit_network_session_set_persistent_credential_storage_enabled().
 *
 * Returns: %TRUE if persistent credential storage is enabled, or %FALSE otherwise.
 *
 * Since: 2.40
 */
gboolean webkit_network_session_get_persistent_credential_storage_enabled(CyberKitNetworkSession* session)
{
    g_return_val_if_fail(WEBKIT_IS_NETWORK_SESSION(session), FALSE);

    auto& websiteDataStore = webkitWebsiteDataManagerGetDataStore(session->priv->websiteDataManager.get());
    return websiteDataStore.persistentCredentialStorageEnabled();
}

/**
 * webkit_network_session_set_tls_errors_policy:
 * @session: a #CyberKitNetworkSession
 * @policy: a #CyberKitTLSErrorsPolicy
 *
 * Set the TLS errors policy of @session as @policy.
 *
 * Since: 2.40
 */
void webkit_network_session_set_tls_errors_policy(CyberKitNetworkSession* session, CyberKitTLSErrorsPolicy policy)
{
    g_return_if_fail(WEBKIT_IS_NETWORK_SESSION(session));

    if (session->priv->tlsErrorsPolicy == policy)
        return;

    session->priv->tlsErrorsPolicy = policy;
    auto& websiteDataStore = webkitWebsiteDataManagerGetDataStore(session->priv->websiteDataManager.get());
    websiteDataStore.setIgnoreTLSErrors(policy == WEBKIT_TLS_ERRORS_POLICY_IGNORE);
}

/**
 * webkit_network_session_get_tls_errors_policy:
 * @session: a #CyberKitNetworkSession
 *
 * Get the TLS errors policy of @session.
 *
 * Returns: a #CyberKitTLSErrorsPolicy
 *
 * Since: 2.40
 */
CyberKitTLSErrorsPolicy webkit_network_session_get_tls_errors_policy(CyberKitNetworkSession* session)
{
    g_return_val_if_fail(WEBKIT_IS_NETWORK_SESSION(session), WEBKIT_TLS_ERRORS_POLICY_FAIL);

    return session->priv->tlsErrorsPolicy;
}

/**
 * webkit_network_session_allow_tls_certificate_for_host:
 * @session: a #CyberKitNetworkSession
 * @certificate: a #GTlsCertificate
 * @host: the host for which a certificate is to be allowed
 *
 * Ignore further TLS errors on the @host for the certificate present in @info.
 *
 * Since: 2.40
 */
void webkit_network_session_allow_tls_certificate_for_host(CyberKitNetworkSession* session, GTlsCertificate* certificate, const char* host)
{
    g_return_if_fail(WEBKIT_IS_NETWORK_SESSION(session));
    g_return_if_fail(G_IS_TLS_CERTIFICATE(certificate));
    g_return_if_fail(host);

    auto certificateInfo = CyberCore::CertificateInfo(certificate, static_cast<GTlsCertificateFlags>(0));
    auto& websiteDataStore = webkitWebsiteDataManagerGetDataStore(session->priv->websiteDataManager.get());
    websiteDataStore.allowSpecificHTTPSCertificateForHost(certificateInfo, String::fromUTF8(host));
}

/**
 * webkit_network_session_set_proxy_settings:
 * @session: a #CyberKitNetworkSession
 * @proxy_mode: a #CyberKitNetworkProxyMode
 * @proxy_settings: (allow-none): a #CyberKitNetworkProxySettings, or %NULL
 *
 * Set the network proxy settings to be used by connections started in @session session.
 *
 * By default %WEBKIT_NETWORK_PROXY_MODE_DEFAULT is used, which means that the
 * system settings will be used (g_proxy_resolver_get_default()).
 * If you want to override the system default settings, you can either use
 * %WEBKIT_NETWORK_PROXY_MODE_NO_PROXY to make sure no proxies are used at all,
 * or %WEBKIT_NETWORK_PROXY_MODE_CUSTOM to provide your own proxy settings.
 * When @proxy_mode is %WEBKIT_NETWORK_PROXY_MODE_CUSTOM @proxy_settings must be
 * a valid #CyberKitNetworkProxySettings; otherwise, @proxy_settings must be %NULL.
 *
 * Since: 2.40
 */
void webkit_network_session_set_proxy_settings(CyberKitNetworkSession* session, CyberKitNetworkProxyMode proxyMode, CyberKitNetworkProxySettings* proxySettings)
{
    g_return_if_fail(WEBKIT_IS_NETWORK_SESSION(session));
    g_return_if_fail((proxyMode != WEBKIT_NETWORK_PROXY_MODE_CUSTOM && !proxySettings) || (proxyMode == WEBKIT_NETWORK_PROXY_MODE_CUSTOM && proxySettings));

    auto& websiteDataStore = webkitWebsiteDataManagerGetDataStore(session->priv->websiteDataManager.get());
    switch (proxyMode) {
    case WEBKIT_NETWORK_PROXY_MODE_DEFAULT:
        websiteDataStore.setNetworkProxySettings({ });
        break;
    case WEBKIT_NETWORK_PROXY_MODE_NO_PROXY:
        websiteDataStore.setNetworkProxySettings(CyberCore::SoupNetworkProxySettings(CyberCore::SoupNetworkProxySettings::Mode::NoProxy));
        break;
    case WEBKIT_NETWORK_PROXY_MODE_CUSTOM:
        auto settings = webkitNetworkProxySettingsGetNetworkProxySettings(proxySettings);
        if (settings.isEmpty()) {
            g_warning("Invalid attempt to set custom network proxy settings with an empty CyberKitNetworkProxySettings. Use "
                "WEBKIT_NETWORK_PROXY_MODE_NO_PROXY to not use any proxy or WEBKIT_NETWORK_PROXY_MODE_DEFAULT to use the default system settings");
        } else
            websiteDataStore.setNetworkProxySettings(WTFMove(settings));
        break;
    }
}

/**
 * webkit_network_session_set_memory_pressure_settings:
 * @settings: a CyberKitMemoryPressureSettings.
 *
 * Sets @settings as the #CyberKitMemoryPressureSettings.
 *
 * Sets @settings as the #CyberKitMemoryPressureSettings to be used by the network
 * process created by any instance of #CyberKitNetworkSession after this function
 * is called.
 *
 * Be sure to call this function before creating any #CyberKitNetworkSession.
 *
 * The periodic check for used memory is disabled by default on network processes. This will
 * be enabled only if custom settings have been set using this function. After that, in order
 * to remove the custom settings and disable the periodic check, this function must be called
 * passing %NULL as the value of @settings.
 *
 * Since: 2.40
 */
void webkit_network_session_set_memory_pressure_settings(CyberKitMemoryPressureSettings* settings)
{
    std::optional<MemoryPressureHandler::Configuration> config = settings ? std::make_optional(webkitMemoryPressureSettingsGetMemoryPressureHandlerConfiguration(settings)) : std::nullopt;
    WebProcessPool::setNetworkProcessMemoryPressureHandlerConfiguration(config);
}

/**
 * webkit_network_session_get_itp_summary:
 * @session: a #CyberKitNetworkSession
 * @cancellable: (allow-none): a #GCancellable or %NULL to ignore
 * @callback: (scope async): a #GAsyncReadyCallback to call when the request is satisfied
 * @user_data: (closure): the data to pass to callback function
 *
 * Asynchronously get the list of #CyberKitITPThirdParty seen for @session.
 *
 * Every #CyberKitITPThirdParty
 * contains the list of #CyberKitITPFirstParty under which it has been seen.
 *
 * When the operation is finished, @callback will be called. You can then call
 * webkit_network_session_get_itp_summary_finish() to get the result of the operation.
 *
 * Since: 2.40
 */
void webkit_network_session_get_itp_summary(CyberKitNetworkSession* session, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer userData)
{
    g_return_if_fail(WEBKIT_IS_NETWORK_SESSION(session));

    auto& websiteDataStore = webkitWebsiteDataManagerGetDataStore(session->priv->websiteDataManager.get());
    GRefPtr<GTask> task = adoptGRef(g_task_new(session, cancellable, callback, userData));
    websiteDataStore.getResourceLoadStatisticsDataSummary([task = WTFMove(task)](Vector<WebResourceLoadStatisticsStore::ThirdPartyData>&& thirdPartyList) {
        GList* result = nullptr;
        while (!thirdPartyList.isEmpty())
            result = g_list_prepend(result, webkitITPThirdPartyCreate(thirdPartyList.takeLast()));
        g_task_return_pointer(task.get(), result, [](gpointer data) {
            g_list_free_full(static_cast<GList*>(data), reinterpret_cast<GDestroyNotify>(webkit_itp_third_party_unref));
        });
    });
}

/**
 * webkit_network_session_get_itp_summary_finish:
 * @session: a #CyberKitNetworkSession
 * @result: a #GAsyncResult
 * @error: return location for error or %NULL to ignore
 *
 * Finish an asynchronous operation started with webkit_network_session_get_itp_summary().
 *
 * Returns: (transfer full) (element-type CyberKitITPThirdParty): a #GList of #CyberKitITPThirdParty.
 *    You must free the #GList with g_list_free() and unref the #CyberKitITPThirdParty<!-- -->s with
 *    webkit_itp_third_party_unref() when you're done with them.
 *
 * Since: 2.40
 */
GList* webkit_network_session_get_itp_summary_finish(CyberKitNetworkSession* session, GAsyncResult* result, GError** error)
{
    g_return_val_if_fail(WEBKIT_IS_NETWORK_SESSION(session), nullptr);
    g_return_val_if_fail(g_task_is_valid(result, session), nullptr);

    return static_cast<GList*>(g_task_propagate_pointer(G_TASK(result), error));
}

/**
 * webkit_network_session_prefetch_dns:
 * @session: a #CyberKitNetworkSession
 * @hostname: a hostname to be resolved
 *
 * Resolve the domain name of the given @hostname in advance, so that if a URI
 * of @hostname is requested the load will be performed more quickly.
 *
 * Since: 2.40
 */
void webkit_network_session_prefetch_dns(CyberKitNetworkSession* session, const char* hostname)
{
    g_return_if_fail(WEBKIT_IS_NETWORK_SESSION(session));
    g_return_if_fail(hostname);

    if (session->priv->dnsPrefetchedHosts.add(String::fromUTF8(hostname)).isNewEntry) {
        auto& websiteDataStore = webkitWebsiteDataManagerGetDataStore(session->priv->websiteDataManager.get());
        websiteDataStore.networkProcess().send(Messages::NetworkProcess::PrefetchDNS(String::fromUTF8(hostname)), 0);
    }
    session->priv->dnsPrefetchHystereris.impulse();
}

/**
 * webkit_network_session_download_uri:
 * @session: a #CyberKitNetworkSession
 * @uri: the URI to download
 *
 * Requests downloading of the specified URI string.
 *
 * The download operation will not be associated to any #CyberKitWebView,
 * if you are interested in starting a download from a particular #CyberKitWebView use
 * webkit_web_view_download_uri() instead.
 *
 * Returns: (transfer full): a new #CyberKitDownload representing
 *    the download operation.
 *
 * Since: 2.40
 */
CyberKitDownload* webkit_network_session_download_uri(CyberKitNetworkSession* session, const char* uri)
{
    g_return_val_if_fail(WEBKIT_IS_NETWORK_SESSION(session), nullptr);
    g_return_val_if_fail(uri, nullptr);

    CyberCore::ResourceRequest request(String::fromUTF8(uri));
    auto& websiteDataStore = webkitWebsiteDataManagerGetDataStore(session->priv->websiteDataManager.get());
    auto downloadProxy = websiteDataStore.createDownloadProxy(adoptRef(*new API::DownloadClient), request, nullptr, { });
    auto download = webkitDownloadCreate(downloadProxy);
    downloadProxy->setDidStartCallback([session = GRefPtr<CyberKitNetworkSession> { session }, download = download.get()](auto* downloadProxy) {
        if (!downloadProxy)
            return;

        webkitDownloadStarted(download);
        webkitNetworkSessionDownloadStarted(session.get(), download);
    });
    websiteDataStore.download(downloadProxy, { });
    return download.leakRef();
}

#endif // ENABLE(2022_GLIB_API)
