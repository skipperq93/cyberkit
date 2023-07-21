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

#if !defined(__WEBKIT2_H_INSIDE__) && !defined(WEBKIT2_COMPILATION)
#error "Only <webkit2/webkit2.h> can be included directly."
#endif

#ifndef CyberKitDownload_h
#define CyberKitDownload_h

#include <glib-object.h>
#include <webkit2/CyberKitDefines.h>
#include <webkit2/CyberKitForwardDeclarations.h>
#include <webkit2/CyberKitURIRequest.h>
#include <webkit2/CyberKitURIResponse.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_DOWNLOAD            (webkit_download_get_type())
#define WEBKIT_DOWNLOAD(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_DOWNLOAD, CyberKitDownload))
#define WEBKIT_IS_DOWNLOAD(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_DOWNLOAD))
#define WEBKIT_DOWNLOAD_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_DOWNLOAD, CyberKitDownloadClass))
#define WEBKIT_IS_DOWNLOAD_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_DOWNLOAD))
#define WEBKIT_DOWNLOAD_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_DOWNLOAD, CyberKitDownloadClass))

typedef struct _CyberKitDownload        CyberKitDownload;
typedef struct _CyberKitDownloadClass   CyberKitDownloadClass;
typedef struct _CyberKitDownloadPrivate CyberKitDownloadPrivate;

struct _CyberKitDownload {
    GObject parent;

    CyberKitDownloadPrivate *priv;
};

struct _CyberKitDownloadClass {
    GObjectClass parent_class;

    gboolean (* decide_destination)  (CyberKitDownload *download,
                                      const gchar    *suggested_filename);

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_download_get_type                 (void);

WEBKIT_API CyberKitURIRequest *
webkit_download_get_request              (CyberKitDownload *download);

WEBKIT_API const gchar *
webkit_download_get_destination          (CyberKitDownload *download);

WEBKIT_API void
webkit_download_set_destination          (CyberKitDownload *download,
                                          const gchar    *uri);

WEBKIT_API CyberKitURIResponse*
webkit_download_get_response             (CyberKitDownload *download);

WEBKIT_API void
webkit_download_cancel                   (CyberKitDownload *download);

WEBKIT_API gdouble
webkit_download_get_estimated_progress   (CyberKitDownload *download);

WEBKIT_API gdouble
webkit_download_get_elapsed_time         (CyberKitDownload *download);

WEBKIT_API guint64
webkit_download_get_received_data_length (CyberKitDownload *download);

WEBKIT_API CyberKitWebView *
webkit_download_get_web_view             (CyberKitDownload *download);

WEBKIT_API gboolean
webkit_download_get_allow_overwrite      (CyberKitDownload *download);

WEBKIT_API void
webkit_download_set_allow_overwrite      (CyberKitDownload *download,
                                          gboolean        allowed);

G_END_DECLS

#endif
