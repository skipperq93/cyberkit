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

#if !defined(__WEBKIT_H_INSIDE__) && !defined(WEBKIT2_COMPILATION) && !defined(__WEBKIT_WEB_EXTENSION_H_INSIDE__)
#error "Only <wpe/webkit.h> can be included directly."
#endif

#ifndef CyberKitHitTestResult_h
#define CyberKitHitTestResult_h

#include <glib-object.h>
#include <wpe/CyberKitDefines.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_HIT_TEST_RESULT            (webkit_hit_test_result_get_type())
#define WEBKIT_HIT_TEST_RESULT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_HIT_TEST_RESULT, CyberKitHitTestResult))
#define WEBKIT_IS_HIT_TEST_RESULT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_HIT_TEST_RESULT))
#define WEBKIT_HIT_TEST_RESULT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_HIT_TEST_RESULT, CyberKitHitTestResultClass))
#define WEBKIT_IS_HIT_TEST_RESULT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_HIT_TEST_RESULT))
#define WEBKIT_HIT_TEST_RESULT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_HIT_TEST_RESULT, CyberKitHitTestResultClass))

typedef struct _CyberKitHitTestResult        CyberKitHitTestResult;
typedef struct _CyberKitHitTestResultClass   CyberKitHitTestResultClass;
typedef struct _CyberKitHitTestResultPrivate CyberKitHitTestResultPrivate;

/**
 * CyberKitHitTestResultContext:
 * @WEBKIT_HIT_TEST_RESULT_CONTEXT_DOCUMENT: anywhere in the document.
 * @WEBKIT_HIT_TEST_RESULT_CONTEXT_LINK: a hyperlink element.
 * @WEBKIT_HIT_TEST_RESULT_CONTEXT_IMAGE: an image element.
 * @WEBKIT_HIT_TEST_RESULT_CONTEXT_MEDIA: a video or audio element.
 * @WEBKIT_HIT_TEST_RESULT_CONTEXT_EDITABLE: an editable element
 * @WEBKIT_HIT_TEST_RESULT_CONTEXT_SCROLLBAR: a scrollbar element.
 * @WEBKIT_HIT_TEST_RESULT_CONTEXT_SELECTION: a selected element. Since 2.8
 *
 * Enum values with flags representing the context of a #CyberKitHitTestResult.
 */
typedef enum
{
    WEBKIT_HIT_TEST_RESULT_CONTEXT_DOCUMENT  = 1 << 1,
    WEBKIT_HIT_TEST_RESULT_CONTEXT_LINK      = 1 << 2,
    WEBKIT_HIT_TEST_RESULT_CONTEXT_IMAGE     = 1 << 3,
    WEBKIT_HIT_TEST_RESULT_CONTEXT_MEDIA     = 1 << 4,
    WEBKIT_HIT_TEST_RESULT_CONTEXT_EDITABLE  = 1 << 5,
    WEBKIT_HIT_TEST_RESULT_CONTEXT_SCROLLBAR = 1 << 6,
    WEBKIT_HIT_TEST_RESULT_CONTEXT_SELECTION = 1 << 7
} CyberKitHitTestResultContext;

struct _CyberKitHitTestResult {
    GObject parent;

    CyberKitHitTestResultPrivate *priv;
};

struct _CyberKitHitTestResultClass {
    GObjectClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_hit_test_result_get_type             (void);

WEBKIT_API guint
webkit_hit_test_result_get_context          (CyberKitHitTestResult *hit_test_result);

WEBKIT_API gboolean
webkit_hit_test_result_context_is_link      (CyberKitHitTestResult *hit_test_result);

WEBKIT_API gboolean
webkit_hit_test_result_context_is_image     (CyberKitHitTestResult *hit_test_result);

WEBKIT_API gboolean
webkit_hit_test_result_context_is_media     (CyberKitHitTestResult *hit_test_result);

WEBKIT_API gboolean
webkit_hit_test_result_context_is_editable  (CyberKitHitTestResult *hit_test_result);

WEBKIT_API gboolean
webkit_hit_test_result_context_is_selection (CyberKitHitTestResult *hit_test_result);

WEBKIT_API const gchar *
webkit_hit_test_result_get_link_uri         (CyberKitHitTestResult *hit_test_result);

WEBKIT_API const gchar *
webkit_hit_test_result_get_link_title       (CyberKitHitTestResult *hit_test_result);

WEBKIT_API const gchar *
webkit_hit_test_result_get_link_label       (CyberKitHitTestResult *hit_test_result);

WEBKIT_API const gchar *
webkit_hit_test_result_get_image_uri        (CyberKitHitTestResult *hit_test_result);

WEBKIT_API const gchar *
webkit_hit_test_result_get_media_uri        (CyberKitHitTestResult *hit_test_result);

WEBKIT_API gboolean
webkit_hit_test_result_context_is_scrollbar (CyberKitHitTestResult *hit_test_result);

G_END_DECLS

#endif
