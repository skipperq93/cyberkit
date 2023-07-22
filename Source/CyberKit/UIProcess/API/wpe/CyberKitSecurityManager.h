/*
 * Copyright (C) 2012 Igalia S.L.
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

#ifndef CyberKitSecurityManager_h
#define CyberKitSecurityManager_h

#include <glib-object.h>
#include <wpe/CyberKitDefines.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_SECURITY_MANAGER            (webkit_security_manager_get_type())
#define WEBKIT_SECURITY_MANAGER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_SECURITY_MANAGER, CyberKitSecurityManager))
#define WEBKIT_IS_SECURITY_MANAGER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_SECURITY_MANAGER))
#define WEBKIT_SECURITY_MANAGER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_SECURITY_MANAGER, CyberKitSecurityManagerClass))
#define WEBKIT_IS_SECURITY_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_SECURITY_MANAGER))
#define WEBKIT_SECURITY_MANAGER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_SECURITY_MANAGER, CyberKitSecurityManagerClass))

typedef struct _CyberKitSecurityManager        CyberKitSecurityManager;
typedef struct _CyberKitSecurityManagerClass   CyberKitSecurityManagerClass;
typedef struct _CyberKitSecurityManagerPrivate CyberKitSecurityManagerPrivate;

struct _CyberKitSecurityManager {
    GObject parent;

    CyberKitSecurityManagerPrivate *priv;
};

struct _CyberKitSecurityManagerClass {
    GObjectClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_security_manager_get_type                                (void);

WEBKIT_API void
webkit_security_manager_register_uri_scheme_as_local            (CyberKitSecurityManager *security_manager,
                                                                 const gchar           *scheme);

WEBKIT_API gboolean
webkit_security_manager_uri_scheme_is_local                     (CyberKitSecurityManager *security_manager,
                                                                 const gchar           *scheme);

WEBKIT_API void
webkit_security_manager_register_uri_scheme_as_no_access        (CyberKitSecurityManager *security_manager,
                                                                 const gchar           *scheme);

WEBKIT_API gboolean
webkit_security_manager_uri_scheme_is_no_access                 (CyberKitSecurityManager *security_manager,
                                                                 const gchar           *scheme);

WEBKIT_API void
webkit_security_manager_register_uri_scheme_as_display_isolated (CyberKitSecurityManager *security_manager,
                                                                 const gchar           *scheme);

WEBKIT_API gboolean
webkit_security_manager_uri_scheme_is_display_isolated          (CyberKitSecurityManager *security_manager,
                                                                 const gchar           *scheme);

WEBKIT_API void
webkit_security_manager_register_uri_scheme_as_secure           (CyberKitSecurityManager *security_manager,
                                                                 const gchar           *scheme);

WEBKIT_API gboolean
webkit_security_manager_uri_scheme_is_secure                    (CyberKitSecurityManager *security_manager,
                                                                 const gchar           *scheme);

WEBKIT_API void
webkit_security_manager_register_uri_scheme_as_cors_enabled     (CyberKitSecurityManager *security_manager,
                                                                 const gchar           *scheme);

WEBKIT_API gboolean
webkit_security_manager_uri_scheme_is_cors_enabled              (CyberKitSecurityManager *security_manager,
                                                                 const gchar           *scheme);

WEBKIT_API void
webkit_security_manager_register_uri_scheme_as_empty_document   (CyberKitSecurityManager *security_manager,
                                                                 const gchar           *scheme);

WEBKIT_API gboolean
webkit_security_manager_uri_scheme_is_empty_document            (CyberKitSecurityManager *security_manager,
                                                                 const gchar           *scheme);

G_END_DECLS

#endif
