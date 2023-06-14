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

#ifndef CyberKitPrintOperation_h
#define CyberKitPrintOperation_h

#include <glib-object.h>
#include <webkit2/CyberKitDefines.h>
#include <webkit2/CyberKitForwardDeclarations.h>
#include <webkit2/CyberKitWebView.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_PRINT_OPERATION            (webkit_print_operation_get_type())
#define WEBKIT_PRINT_OPERATION(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_PRINT_OPERATION, CyberKitPrintOperation))
#define WEBKIT_IS_PRINT_OPERATION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_PRINT_OPERATION))
#define WEBKIT_PRINT_OPERATION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_PRINT_OPERATION, CyberKitPrintOperationClass))
#define WEBKIT_IS_PRINT_OPERATION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_PRINT_OPERATION))
#define WEBKIT_PRINT_OPERATION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_PRINT_OPERATION, CyberKitPrintOperationClass))

typedef struct _CyberKitPrintOperationClass   CyberKitPrintOperationClass;
typedef struct _CyberKitPrintOperationPrivate CyberKitPrintOperationPrivate;

/**
 * CyberKitPrintOperationResponse:
 * @WEBKIT_PRINT_OPERATION_RESPONSE_PRINT: Print button was clicked in print dialog
 * @WEBKIT_PRINT_OPERATION_RESPONSE_CANCEL: Print dialog was cancelled
 *
 * Enum values representing the response of the print dialog shown with
 * webkit_print_operation_run_dialog().
 */
typedef enum {
    WEBKIT_PRINT_OPERATION_RESPONSE_PRINT,
    WEBKIT_PRINT_OPERATION_RESPONSE_CANCEL
} CyberKitPrintOperationResponse;

struct _CyberKitPrintOperation {
    GObject parent;

    CyberKitPrintOperationPrivate *priv;
};

struct _CyberKitPrintOperationClass {
    GObjectClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_print_operation_get_type           (void);

WEBKIT_API CyberKitPrintOperation *
webkit_print_operation_new                (CyberKitWebView        *web_view);

WEBKIT_API GtkPrintSettings *
webkit_print_operation_get_print_settings (CyberKitPrintOperation *print_operation);

WEBKIT_API void
webkit_print_operation_set_print_settings (CyberKitPrintOperation *print_operation,
                                           GtkPrintSettings     *print_settings);

WEBKIT_API GtkPageSetup *
webkit_print_operation_get_page_setup     (CyberKitPrintOperation *print_operation);

WEBKIT_API void
webkit_print_operation_set_page_setup     (CyberKitPrintOperation *print_operation,
                                           GtkPageSetup         *page_setup);

WEBKIT_API CyberKitPrintOperationResponse
webkit_print_operation_run_dialog         (CyberKitPrintOperation *print_operation,
                                           GtkWindow            *parent);

WEBKIT_API void
webkit_print_operation_print              (CyberKitPrintOperation *print_operation);

G_END_DECLS

#endif
