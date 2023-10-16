/*
 * Copyright (C) 2013 Samsung Electronics Inc. All rights reserved.
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
#include "CyberKitAuthenticationRequest.h"

#include "AuthenticationChallengeDisposition.h"
#include "AuthenticationDecisionListener.h"
#include "WebCredential.h"
#include "CyberKitAuthenticationRequestPrivate.h"
#include "CyberKitCredentialPrivate.h"
#include "CyberKitSecurityOriginPrivate.h"
#include "WebProtectionSpace.h"
#include <CyberCore/AuthenticationChallenge.h>
#include <CyberCore/ProtectionSpace.h>
#include <glib/gi18n-lib.h>
#include <wtf/glib/WTFGType.h>
#include <wtf/text/CString.h>

using namespace CyberKit;
using namespace CyberCore;

/**
 * CyberKitAuthenticationRequest:
 * @See_also: #CyberKitWebView
 *
 * Represents an authentication request.
 *
 * Whenever a client attempts to load a page protected by HTTP
 * authentication, credentials will need to be provided to authorize access.
 * To allow the client to decide how it wishes to handle authentication,
 * CyberKit will fire a #CyberKitWebView::authenticate signal with a
 * CyberKitAuthenticationRequest object to provide client side
 * authentication support. Credentials are exposed through the
 * #CyberKitCredential object.
 *
 * In case the client application does not wish
 * to handle this signal CyberKit will provide a default handler. To handle
 * authentication asynchronously, simply increase the reference count of the
 * CyberKitAuthenticationRequest object.
 */

enum {
    AUTHENTICATED,
    CANCELLED,

    LAST_SIGNAL
};

struct _CyberKitAuthenticationRequestPrivate {
    RefPtr<AuthenticationChallengeProxy> authenticationChallenge;
    bool privateBrowsingEnabled;
    bool persistentCredentialStorageEnabled;
    bool handledRequest;
    CString host;
    CString realm;
    std::optional<CyberCore::Credential> proposedCredential;
    std::optional<CyberCore::Credential> acceptedCredential;
    std::optional<bool> canSaveCredentials;
};

static guint signals[LAST_SIGNAL] = { 0, };

WEBKIT_DEFINE_FINAL_TYPE(CyberKitAuthenticationRequest, webkit_authentication_request, G_TYPE_OBJECT, GObject)

static inline CyberKitAuthenticationScheme toCyberKitAuthenticationScheme(CyberCore::ProtectionSpace::AuthenticationScheme coreScheme)
{
    switch (coreScheme) {
    case CyberCore::ProtectionSpace::AuthenticationScheme::Default:
        return WEBKIT_AUTHENTICATION_SCHEME_DEFAULT;
    case CyberCore::ProtectionSpace::AuthenticationScheme::HTTPBasic:
        return WEBKIT_AUTHENTICATION_SCHEME_HTTP_BASIC;
    case CyberCore::ProtectionSpace::AuthenticationScheme::HTTPDigest:
        return WEBKIT_AUTHENTICATION_SCHEME_HTTP_DIGEST;
    case CyberCore::ProtectionSpace::AuthenticationScheme::HTMLForm:
        return WEBKIT_AUTHENTICATION_SCHEME_HTML_FORM;
    case CyberCore::ProtectionSpace::AuthenticationScheme::NTLM:
        return WEBKIT_AUTHENTICATION_SCHEME_NTLM;
    case CyberCore::ProtectionSpace::AuthenticationScheme::Negotiate:
        return WEBKIT_AUTHENTICATION_SCHEME_NEGOTIATE;
    case CyberCore::ProtectionSpace::AuthenticationScheme::ClientCertificateRequested:
        return WEBKIT_AUTHENTICATION_SCHEME_CLIENT_CERTIFICATE_REQUESTED;
    case CyberCore::ProtectionSpace::AuthenticationScheme::ServerTrustEvaluationRequested:
        return WEBKIT_AUTHENTICATION_SCHEME_SERVER_TRUST_EVALUATION_REQUESTED;
    case CyberCore::ProtectionSpace::AuthenticationScheme::ClientCertificatePINRequested:
        return WEBKIT_AUTHENTICATION_SCHEME_CLIENT_CERTIFICATE_PIN_REQUESTED;
    case CyberCore::ProtectionSpace::AuthenticationScheme::Unknown:
        return WEBKIT_AUTHENTICATION_SCHEME_UNKNOWN;
    default:
        ASSERT_NOT_REACHED();
        return WEBKIT_AUTHENTICATION_SCHEME_DEFAULT;
    }
}

static void webkitAuthenticationRequestDispose(GObject* object)
{
    CyberKitAuthenticationRequest* request = WEBKIT_AUTHENTICATION_REQUEST(object);

    // Make sure the request is always handled before finalizing.
    webkit_authentication_request_cancel(request);

    G_OBJECT_CLASS(webkit_authentication_request_parent_class)->dispose(object);
}

static void webkit_authentication_request_class_init(CyberKitAuthenticationRequestClass* requestClass)
{
    GObjectClass* objectClass = G_OBJECT_CLASS(requestClass);
    objectClass->dispose = webkitAuthenticationRequestDispose;

    /**
     * CyberKitAuthenticationRequest::authenticated:
     * @request: the #CyberKitAuthenticationRequest
     * @credential: the #CyberKitCredential accepted
     *
     * This signal is emitted when the user authentication request succeeded.
     * Applications handling their own credential storage should connect to
     * this signal to save the credentials.
     *
     * Since: 2.30
     */
    signals[AUTHENTICATED] =
        g_signal_new(
            "authenticated",
            G_TYPE_FROM_CLASS(objectClass),
            G_SIGNAL_RUN_LAST,
            0, 0, nullptr,
            g_cclosure_marshal_generic,
            G_TYPE_NONE, 1,
            WEBKIT_TYPE_CREDENTIAL | G_SIGNAL_TYPE_STATIC_SCOPE);

    /**
     * CyberKitAuthenticationRequest::cancelled:
     * @request: the #CyberKitAuthenticationRequest
     *
     * This signal is emitted when the user authentication request is
     * cancelled. It allows the application to dismiss its authentication
     * dialog in case of page load failure for example.
     *
     * Since: 2.2
     */
    signals[CANCELLED] =
        g_signal_new("cancelled",
            G_TYPE_FROM_CLASS(objectClass),
            G_SIGNAL_RUN_LAST,
            0, 0, 0,
            g_cclosure_marshal_VOID__VOID,
            G_TYPE_NONE, 0);
}

CyberKitAuthenticationRequest* webkitAuthenticationRequestCreate(AuthenticationChallengeProxy* authenticationChallenge, bool privateBrowsingEnabled, bool persistentCredentialStorageEnabled)
{
    CyberKitAuthenticationRequest* request = WEBKIT_AUTHENTICATION_REQUEST(g_object_new(WEBKIT_TYPE_AUTHENTICATION_REQUEST, NULL));
    request->priv->authenticationChallenge = authenticationChallenge;
    request->priv->privateBrowsingEnabled = privateBrowsingEnabled;
    request->priv->persistentCredentialStorageEnabled = persistentCredentialStorageEnabled;
    return request;
}

AuthenticationChallengeProxy* webkitAuthenticationRequestGetAuthenticationChallenge(CyberKitAuthenticationRequest* request)
{
    return request->priv->authenticationChallenge.get();
}

void webkitAuthenticationRequestDidAuthenticate(CyberKitAuthenticationRequest* request)
{
    auto* credential = webkitCredentialCreate(request->priv->acceptedCredential.value_or(CyberCore::Credential()));
    g_signal_emit(request, signals[AUTHENTICATED], 0, credential);
    webkit_credential_free(credential);
}

const CyberCore::Credential& webkitAuthenticationRequestGetProposedCredential(CyberKitAuthenticationRequest* request)
{
    if (request->priv->proposedCredential)
        return request->priv->proposedCredential.value();
    return request->priv->authenticationChallenge->core().proposedCredential();
}

/**
 * webkit_authentication_request_can_save_credentials:
 * @request: a #CyberKitAuthenticationRequest
 *
 * Determine whether this #CyberKitAuthenticationRequest should allow the storage of credentials.
 *
 * Determine whether the authentication method associated with this
 * #CyberKitAuthenticationRequest should allow the storage of credentials.
 * This will return %FALSE if CyberKit doesn't support credential storing,
 * if private browsing is enabled, or if persistent credential storage has been
 * disabled in #CyberKitWebsiteDataManager, unless credentials saving has been
 * explicitly enabled with webkit_authentication_request_set_can_save_credentials().
 *
 * Returns: %TRUE if CyberKit can store credentials or %FALSE otherwise.
 *
 * Since: 2.2
 */
gboolean webkit_authentication_request_can_save_credentials(CyberKitAuthenticationRequest* request)
{
    g_return_val_if_fail(WEBKIT_IS_AUTHENTICATION_REQUEST(request), FALSE);

    if (request->priv->privateBrowsingEnabled)
        return FALSE;

    if (request->priv->canSaveCredentials)
        return request->priv->canSaveCredentials.value();

#if USE(LIBSECRET)
    return request->priv->persistentCredentialStorageEnabled;
#else
    return FALSE;
#endif
}

/**
 * webkit_authentication_request_set_can_save_credentials:
 * @request: a #CyberKitAuthenticationRequest
 * @enabled: value to set
 *
 * Set whether the authentication method associated with @request
 * should allow the storage of credentials.
 *
 * Set whether the authentication method associated with @request
 * should allow the storage of credentials.
 * This should be used by applications handling their own credentials
 * storage to indicate that it should be supported even when internal
 * credential storage is disabled or unsupported.
 * Note that storing of credentials will not be allowed on ephemeral
 * sessions in any case.
 *
 * Since: 2.30
 */
void webkit_authentication_request_set_can_save_credentials(CyberKitAuthenticationRequest* request, gboolean enabled)
{
    g_return_if_fail(WEBKIT_IS_AUTHENTICATION_REQUEST(request));

    request->priv->canSaveCredentials = enabled;
}

/**
 * webkit_authentication_request_get_proposed_credential:
 * @request: a #CyberKitAuthenticationRequest
 *
 * Get the #CyberKitCredential of the proposed authentication challenge.
 *
 * Get the #CyberKitCredential of the proposed authentication challenge that was
 * stored from a previous session. The client can use this directly for
 * authentication or construct their own #CyberKitCredential.
 *
 * Returns: (transfer full): A #CyberKitCredential encapsulating credential details
 * or %NULL if there is no stored credential.
 *
 * Since: 2.2
 */
CyberKitCredential* webkit_authentication_request_get_proposed_credential(CyberKitAuthenticationRequest* request)
{
    g_return_val_if_fail(WEBKIT_IS_AUTHENTICATION_REQUEST(request), nullptr);

    const auto& credential = webkitAuthenticationRequestGetProposedCredential(request);
    if (credential.isEmpty())
        return nullptr;

    return webkitCredentialCreate(credential);
}

/**
 * webkit_authentication_request_set_proposed_credential:
 * @request: a #CyberKitAuthenticationRequest
 * @credential: a #CyberKitCredential, or %NULL
 *
 * Set the #CyberKitCredential of the proposed authentication challenge.
 *
 * Set the #CyberKitCredential of the proposed authentication challenge that was
 * stored from a previous session. This should only be used by applications handling
 * their own credential storage. (When using the default CyberKit credential storage,
 * webkit_authentication_request_get_proposed_credential() already contains previously-stored
 * credentials.)
 * Passing a %NULL @credential will clear the proposed credential.
 *
 * Since: 2.30
 */
void webkit_authentication_request_set_proposed_credential(CyberKitAuthenticationRequest* request, CyberKitCredential* credential)
{
    g_return_if_fail(WEBKIT_IS_AUTHENTICATION_REQUEST(request));

    if (!credential) {
        request->priv->proposedCredential = std::nullopt;
        return;
    }

    request->priv->proposedCredential = webkitCredentialGetCredential(credential);
}

/**
 * webkit_authentication_request_get_host:
 * @request: a #CyberKitAuthenticationRequest
 *
 * Get the host that this authentication challenge is applicable to.
 *
 * Returns: The host of @request.
 *
 * Since: 2.2
 */
const gchar* webkit_authentication_request_get_host(CyberKitAuthenticationRequest* request)
{
    g_return_val_if_fail(WEBKIT_IS_AUTHENTICATION_REQUEST(request), 0);

    if (request->priv->host.isNull())
        request->priv->host = request->priv->authenticationChallenge->core().protectionSpace().host().utf8();
    return request->priv->host.data();
}

/**
 * webkit_authentication_request_get_port:
 * @request: a #CyberKitAuthenticationRequest
 *
 * Get the port that this authentication challenge is applicable to.
 *
 * Returns: The port of @request.
 *
 * Since: 2.2
 */
guint webkit_authentication_request_get_port(CyberKitAuthenticationRequest* request)
{
    g_return_val_if_fail(WEBKIT_IS_AUTHENTICATION_REQUEST(request), 0);

    return request->priv->authenticationChallenge->core().protectionSpace().port();
}

/**
 * webkit_authentication_request_get_security_origin:
 * @request: a #CyberKitAuthenticationRequest
 *
 * Get the #CyberKitSecurityOrigin that this authentication challenge is applicable to.
 *
 * Returns: (transfer full): a newly created #CyberKitSecurityOrigin.
 *
 * Since: 2.30
 */
CyberKitSecurityOrigin* webkit_authentication_request_get_security_origin(CyberKitAuthenticationRequest* request)
{
    g_return_val_if_fail(WEBKIT_IS_AUTHENTICATION_REQUEST(request), nullptr);

    const auto& protectionSpace = request->priv->authenticationChallenge->core().protectionSpace();
    String protocol;
    switch (protectionSpace.serverType()) {
    case ProtectionSpace::ServerType::HTTP:
    case ProtectionSpace::ServerType::ProxyHTTP:
        protocol = "http"_s;
        break;
    case ProtectionSpace::ServerType::HTTPS:
    case ProtectionSpace::ServerType::ProxyHTTPS:
        protocol = "https"_s;
        break;
    case ProtectionSpace::ServerType::FTP:
    case ProtectionSpace::ServerType::ProxyFTP:
        protocol = "ftp"_s;
        break;
    case ProtectionSpace::ServerType::FTPS:
        protocol = "ftps"_s;
        break;
    case ProtectionSpace::ServerType::ProxySOCKS:
        protocol = "socks"_s;
        break;
    }
    return webkitSecurityOriginCreate(SecurityOriginData(protocol, protectionSpace.host(), protectionSpace.port()));
}

/**
 * webkit_authentication_request_get_realm:
 * @request: a #CyberKitAuthenticationRequest
 *
 * Get the realm that this authentication challenge is applicable to.
 *
 * Returns: The realm of @request.
 *
 * Since: 2.2
 */
const gchar* webkit_authentication_request_get_realm(CyberKitAuthenticationRequest* request)
{
    g_return_val_if_fail(WEBKIT_IS_AUTHENTICATION_REQUEST(request), 0);

    if (request->priv->realm.isNull())
        request->priv->realm = request->priv->authenticationChallenge->core().protectionSpace().realm().utf8();
    return request->priv->realm.data();
}

/**
 * webkit_authentication_request_get_scheme:
 * @request: a #CyberKitAuthenticationRequest
 *
 * Get the authentication scheme of the authentication challenge.
 *
 * Returns: The #CyberKitAuthenticationScheme of @request.
 *
 * Since: 2.2
 */
CyberKitAuthenticationScheme webkit_authentication_request_get_scheme(CyberKitAuthenticationRequest* request)
{
    g_return_val_if_fail(WEBKIT_IS_AUTHENTICATION_REQUEST(request), WEBKIT_AUTHENTICATION_SCHEME_UNKNOWN);

    return toCyberKitAuthenticationScheme(request->priv->authenticationChallenge->core().protectionSpace().authenticationScheme());
}

/**
 * webkit_authentication_request_is_for_proxy:
 * @request: a #CyberKitAuthenticationRequest
 *
 * Determine whether the authentication challenge is associated with a proxy server.
 *
 * Determine whether the authentication challenge is associated with a proxy server rather than an "origin" server.
 *
 * Returns: %TRUE if authentication is for a proxy or %FALSE otherwise.
 *
 * Since: 2.2
 */
gboolean webkit_authentication_request_is_for_proxy(CyberKitAuthenticationRequest* request)
{
    g_return_val_if_fail(WEBKIT_IS_AUTHENTICATION_REQUEST(request), FALSE);

    return request->priv->authenticationChallenge->core().protectionSpace().isProxy();
}

/**
 * webkit_authentication_request_is_retry:
 * @request: a #CyberKitAuthenticationRequest
 *
 * Determine whether this this is a first attempt or a retry for this authentication challenge.
 *
 * Returns: %TRUE if authentication attempt is a retry or %FALSE otherwise.
 *
 * Since: 2.2
 */
gboolean webkit_authentication_request_is_retry(CyberKitAuthenticationRequest* request)
{
    g_return_val_if_fail(WEBKIT_IS_AUTHENTICATION_REQUEST(request), 0);

    return request->priv->authenticationChallenge->core().previousFailureCount() ? TRUE : FALSE;
}

/**
 * webkit_authentication_request_authenticate:
 * @request: a #CyberKitAuthenticationRequest
 * @credential: (transfer none) (allow-none): A #CyberKitCredential, or %NULL
 *
 * Authenticate the #CyberKitAuthenticationRequest.
 *
 * Authenticate the #CyberKitAuthenticationRequest using the #CyberKitCredential
 * supplied. To continue without credentials, pass %NULL as @credential.
 *
 * Since: 2.2
 */
void webkit_authentication_request_authenticate(CyberKitAuthenticationRequest* request, CyberKitCredential* credential)
{
    g_return_if_fail(WEBKIT_IS_AUTHENTICATION_REQUEST(request));

    if (credential)
        request->priv->acceptedCredential = webkitCredentialGetCredential(credential);
    else
        request->priv->acceptedCredential = std::nullopt;
    request->priv->authenticationChallenge->listener().completeChallenge(CyberKit::AuthenticationChallengeDisposition::UseCredential, request->priv->acceptedCredential.value_or(CyberCore::Credential()));
    request->priv->handledRequest = true;
}

/**
 * webkit_authentication_request_cancel:
 * @request: a #CyberKitAuthenticationRequest
 *
 * Cancel the authentication challenge.
 *
 * This will also cancel the page loading and result in a
 * #CyberKitWebView::load-failed signal with a #CyberKitNetworkError of type %WEBKIT_NETWORK_ERROR_CANCELLED being emitted.
 *
 * Since: 2.2
 */
void webkit_authentication_request_cancel(CyberKitAuthenticationRequest* request)
{
    g_return_if_fail(WEBKIT_IS_AUTHENTICATION_REQUEST(request));

    if (request->priv->handledRequest)
        return;

    request->priv->authenticationChallenge->listener().completeChallenge(CyberKit::AuthenticationChallengeDisposition::Cancel);
    request->priv->acceptedCredential = std::nullopt;
    request->priv->handledRequest = true;

    g_signal_emit(request, signals[CANCELLED], 0);
}

/**
 * webkit_authentication_request_get_certificate_pin_flags:
 * @request: a #CyberKitAuthenticationRequest
 *
 * Get the #GTlsPasswordFlags of the %WEBKIT_AUTHENTICATION_SCHEME_CLIENT_CERTIFICATE_PIN_REQUESTED authentication challenge.
 *
 * Returns: a #GTlsPasswordFlags
 *
 * Since: 2.34
 */
GTlsPasswordFlags webkit_authentication_request_get_certificate_pin_flags(CyberKitAuthenticationRequest* request)
{
    g_return_val_if_fail(WEBKIT_IS_AUTHENTICATION_REQUEST(request), G_TLS_PASSWORD_NONE);

    return static_cast<GTlsPasswordFlags>(request->priv->authenticationChallenge->core().tlsPasswordFlags());
}
