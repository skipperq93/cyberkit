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

#ifndef CyberKitFormSubmissionRequest_h
#define CyberKitFormSubmissionRequest_h

#include <glib-object.h>
#include <wpe/CyberKitDefines.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_FORM_SUBMISSION_REQUEST            (webkit_form_submission_request_get_type())
#define WEBKIT_FORM_SUBMISSION_REQUEST(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_FORM_SUBMISSION_REQUEST, CyberKitFormSubmissionRequest))
#define WEBKIT_IS_FORM_SUBMISSION_REQUEST(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_FORM_SUBMISSION_REQUEST))
#define WEBKIT_FORM_SUBMISSION_REQUEST_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_FORM_SUBMISSION_REQUEST, CyberKitFormSubmissionRequestClass))
#define WEBKIT_IS_FORM_SUBMISSION_REQUEST_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_FORM_SUBMISSION_REQUEST))
#define WEBKIT_FORM_SUBMISSION_REQUEST_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_FORM_SUBMISSION_REQUEST, CyberKitFormSubmissionRequestClass))

typedef struct _CyberKitFormSubmissionRequest        CyberKitFormSubmissionRequest;
typedef struct _CyberKitFormSubmissionRequestClass   CyberKitFormSubmissionRequestClass;
typedef struct _CyberKitFormSubmissionRequestPrivate CyberKitFormSubmissionRequestPrivate;

struct _CyberKitFormSubmissionRequest {
    GObject parent;

    /*< private >*/
    CyberKitFormSubmissionRequestPrivate *priv;
};

struct _CyberKitFormSubmissionRequestClass {
    GObjectClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_form_submission_request_get_type         (void);

WEBKIT_API gboolean
webkit_form_submission_request_list_text_fields (CyberKitFormSubmissionRequest  *request,
                                                 GPtrArray                   **field_names,
                                                 GPtrArray                   **field_values);

WEBKIT_API void
webkit_form_submission_request_submit           (CyberKitFormSubmissionRequest  *request);

G_END_DECLS

#endif
