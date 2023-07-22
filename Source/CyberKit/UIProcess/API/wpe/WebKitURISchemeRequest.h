/*
 * Copyright (C) 2012 Igalia S.L.
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

#if !defined(__WEBKIT_H_INSIDE__) && !defined(WEBKIT2_COMPILATION)
#error "Only <wpe/webkit.h> can be included directly."
#endif

#ifndef CyberKitURISchemeRequest_h
#define CyberKitURISchemeRequest_h

#include <glib-object.h>
#include <wpe/CyberKitDefines.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_URI_SCHEME_REQUEST            (webkit_uri_scheme_request_get_type())
#define WEBKIT_URI_SCHEME_REQUEST(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_URI_SCHEME_REQUEST, CyberKitURISchemeRequest))
#define WEBKIT_IS_URI_SCHEME_REQUEST(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_URI_SCHEME_REQUEST))
#define WEBKIT_URI_SCHEME_REQUEST_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_URI_SCHEME_REQUEST, CyberKitURISchemeRequestClass))
#define WEBKIT_IS_URI_SCHEME_REQUEST_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_URI_SCHEME_REQUEST))
#define WEBKIT_URI_SCHEME_REQUEST_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_URI_SCHEME_REQUEST, CyberKitURISchemeRequestClass))

typedef struct _CyberKitURISchemeRequest        CyberKitURISchemeRequest;
typedef struct _CyberKitURISchemeRequestClass   CyberKitURISchemeRequestClass;
typedef struct _CyberKitURISchemeRequestPrivate CyberKitURISchemeRequestPrivate;

typedef struct _CyberKitWebView CyberKitWebView;

struct _CyberKitURISchemeRequest {
    GObject parent;

    CyberKitURISchemeRequestPrivate *priv;
};

struct _CyberKitURISchemeRequestClass {
    GObjectClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_uri_scheme_request_get_type     (void);

WEBKIT_API const gchar *
webkit_uri_scheme_request_get_scheme   (CyberKitURISchemeRequest *request);

WEBKIT_API const gchar *
webkit_uri_scheme_request_get_uri      (CyberKitURISchemeRequest *request);

WEBKIT_API const gchar *
webkit_uri_scheme_request_get_path     (CyberKitURISchemeRequest *request);

WEBKIT_API CyberKitWebView *
webkit_uri_scheme_request_get_web_view (CyberKitURISchemeRequest *request);

WEBKIT_API void
webkit_uri_scheme_request_finish       (CyberKitURISchemeRequest *request,
                                        GInputStream           *stream,
                                        gint64                  stream_length,
                                        const gchar            *content_type);

WEBKIT_API void
webkit_uri_scheme_request_finish_error (CyberKitURISchemeRequest *request,
                                        GError                 *error);

G_END_DECLS

#endif
