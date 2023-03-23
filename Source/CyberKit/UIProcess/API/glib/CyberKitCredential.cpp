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
#include "CyberKitCredential.h"

#include "WebCredential.h"
#include "CyberKitCredentialPrivate.h"
#include <wtf/text/CString.h>

using namespace CyberKit;

/**
 * CyberKitCredential:
 *
 * Groups information used for user authentication.
 *
 * Since: 2.2
 */

struct _CyberKitCredential {
    _CyberKitCredential(const CyberCore::Credential& coreCredential)
        : credential(coreCredential)
    {
    }

    CyberCore::Credential credential;
    CString username;
    CString password;
};

G_DEFINE_BOXED_TYPE(CyberKitCredential, webkit_credential, webkit_credential_copy, webkit_credential_free)

static inline CyberKitCredentialPersistence toCyberKitCredentialPersistence(CyberCore::CredentialPersistence corePersistence)
{
    switch (corePersistence) {
    case CyberCore::CredentialPersistenceNone:
        return WEBKIT_CREDENTIAL_PERSISTENCE_NONE;
    case CyberCore::CredentialPersistenceForSession:
        return WEBKIT_CREDENTIAL_PERSISTENCE_FOR_SESSION;
    case CyberCore::CredentialPersistencePermanent:
        return WEBKIT_CREDENTIAL_PERSISTENCE_PERMANENT;
    default:
        ASSERT_NOT_REACHED();
        return WEBKIT_CREDENTIAL_PERSISTENCE_NONE;
    }
}

static inline CyberCore::CredentialPersistence toCyberCoreCredentialPersistence(CyberKitCredentialPersistence kitPersistence)
{
    switch (kitPersistence) {
    case WEBKIT_CREDENTIAL_PERSISTENCE_NONE:
        return CyberCore::CredentialPersistenceNone;
    case WEBKIT_CREDENTIAL_PERSISTENCE_FOR_SESSION:
        return CyberCore::CredentialPersistenceForSession;
    case WEBKIT_CREDENTIAL_PERSISTENCE_PERMANENT:
        return CyberCore::CredentialPersistencePermanent;
    default:
        ASSERT_NOT_REACHED();
        return CyberCore::CredentialPersistenceNone;
    }
}

CyberKitCredential* webkitCredentialCreate(const CyberCore::Credential& coreCredential)
{
    CyberKitCredential* credential = static_cast<CyberKitCredential*>(fastMalloc(sizeof(CyberKitCredential)));
    new (credential) CyberKitCredential(coreCredential);
    return credential;
}

const CyberCore::Credential& webkitCredentialGetCredential(CyberKitCredential* credential)
{
    ASSERT(credential);
    return credential->credential;
}

/**
 * webkit_credential_new:
 * @username: The username for the new credential
 * @password: The password for the new credential
 * @persistence: The #CyberKitCredentialPersistence of the new credential
 *
 * Create a new credential from the provided username, password and persistence mode.
 *
 * Returns: (transfer full): A #CyberKitCredential.
 *
 * Since: 2.2
 */
CyberKitCredential* webkit_credential_new(const gchar* username, const gchar* password, CyberKitCredentialPersistence persistence)
{
    g_return_val_if_fail(username, 0);
    g_return_val_if_fail(password, 0);

    return webkitCredentialCreate(CyberCore::Credential(String::fromUTF8(username), String::fromUTF8(password), toCyberCoreCredentialPersistence(persistence)));
}

/**
 * webkit_credential_new_for_certificate_pin:
 * @pin: The PIN for the new credential
 * @persistence: The #CyberKitCredentialPersistence of the new credential
 *
 * Create a new credential from the provided PIN and persistence mode.
 *
 * Note that %WEBKIT_CREDENTIAL_PERSISTENCE_PERMANENT is not supported for certificate pin credentials.
 *
 * Returns: (transfer full): A #CyberKitCredential.
 *
 * Since: 2.34
 */
CyberKitCredential* webkit_credential_new_for_certificate_pin(const gchar* pin, CyberKitCredentialPersistence persistence)
{
    g_return_val_if_fail(pin, 0);

    if (persistence == WEBKIT_CREDENTIAL_PERSISTENCE_PERMANENT) {
        g_warning("Permanent persistence is not supported for certificate pin credentials. Session persistence will be used instead.");
        persistence = WEBKIT_CREDENTIAL_PERSISTENCE_FOR_SESSION;
    }

    return webkitCredentialCreate(CyberCore::Credential(emptyString(), String::fromUTF8(pin), toCyberCoreCredentialPersistence(persistence)));
}

/**
 * webkit_credential_new_for_certificate:
 * @certificate: (nullable): The #GTlsCertificate, or %NULL
 * @persistence: The #CyberKitCredentialPersistence of the new credential
 *
 * Create a new credential from the @certificate and persistence mode.
 *
 * Note that %WEBKIT_CREDENTIAL_PERSISTENCE_PERMANENT is not supported for certificate credentials.
 *
 * Returns: (transfer full): A #CyberKitCredential.
 *
 * Since: 2.34
 */
CyberKitCredential* webkit_credential_new_for_certificate(GTlsCertificate* certificate, CyberKitCredentialPersistence persistence)
{
    g_return_val_if_fail(!certificate || G_IS_TLS_CERTIFICATE(certificate), nullptr);

    if (persistence == WEBKIT_CREDENTIAL_PERSISTENCE_PERMANENT) {
        g_warning("Permanent persistence is not supported for certificate credentials. Session persistence will be used instead.");
        persistence = WEBKIT_CREDENTIAL_PERSISTENCE_FOR_SESSION;
    }

    return webkitCredentialCreate(CyberCore::Credential(certificate, toCyberCoreCredentialPersistence(persistence)));
}

/**
 * webkit_credential_copy:
 * @credential: a #CyberKitCredential
 *
 * Make a copy of the #CyberKitCredential.
 *
 * Returns: (transfer full): A copy of passed in #CyberKitCredential
 *
 * Since: 2.2
 */
CyberKitCredential* webkit_credential_copy(CyberKitCredential* credential)
{
    g_return_val_if_fail(credential, 0);

    return webkitCredentialCreate(credential->credential);
}

/**
 * webkit_credential_free:
 * @credential: A #CyberKitCredential
 *
 * Free the #CyberKitCredential.
 *
 * Since: 2.2
 */
void webkit_credential_free(CyberKitCredential* credential)
{
    g_return_if_fail(credential);

    credential->~CyberKitCredential();
    fastFree(credential);
}

/**
 * webkit_credential_get_username:
 * @credential: a #CyberKitCredential
 *
 * Get the username currently held by this #CyberKitCredential.
 *
 * Returns: The username stored in the #CyberKitCredential.
 *
 * Since: 2.2
 */
const gchar* webkit_credential_get_username(CyberKitCredential* credential)
{
    g_return_val_if_fail(credential, 0);

    if (credential->username.isNull())
        credential->username = credential->credential.user().utf8();
    return credential->username.data();
}

/**
 * webkit_credential_get_password:
 * @credential: a #CyberKitCredential
 *
 * Get the password currently held by this #CyberKitCredential.
 *
 * Returns: The password stored in the #CyberKitCredential.
 *
 * Since: 2.2
 */
const gchar* webkit_credential_get_password(CyberKitCredential* credential)
{
    g_return_val_if_fail(credential, 0);

    if (credential->password.isNull())
        credential->password = credential->credential.password().utf8();
    return credential->password.data();
}

/**
 * webkit_credential_has_password:
 * @credential: a #CyberKitCredential
 *
 * Determine whether this credential has a password stored.
 *
 * Returns: %TRUE if the credential has a password or %FALSE otherwise.
 *
 * Since: 2.2
 */
gboolean webkit_credential_has_password(CyberKitCredential* credential)
{
    g_return_val_if_fail(credential, FALSE);

    return credential->credential.hasPassword();
}

/**
 * webkit_credential_get_certificate:
 * @credential: a #CyberKitCredential
 *
 * Get the certificate currently held by this #CyberKitCredential.
 *
 * Returns: (transfer none): a #GTlsCertificate, or %NULL
 *
 * Since: 2.34
 */
GTlsCertificate* webkit_credential_get_certificate(CyberKitCredential* credential)
{
    g_return_val_if_fail(credential, NULL);

    return credential->credential.certificate();
}

/**
 * webkit_credential_get_persistence:
 * @credential: a #CyberKitCredential
 *
 * Get the persistence mode currently held by this #CyberKitCredential.
 *
 * Returns: The #CyberKitCredentialPersistence stored in the #CyberKitCredential.
 *
 * Since: 2.2
 */
CyberKitCredentialPersistence webkit_credential_get_persistence(CyberKitCredential* credential)
{
    g_return_val_if_fail(credential, WEBKIT_CREDENTIAL_PERSISTENCE_NONE);

    return toCyberKitCredentialPersistence(credential->credential.persistence());
}
