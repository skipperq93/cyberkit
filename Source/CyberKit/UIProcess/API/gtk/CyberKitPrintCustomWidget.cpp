/*
 * Copyright (C) 2017 Red Hat Inc.
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
#include "CyberKitPrintCustomWidget.h"

#include "CyberKitPrintCustomWidgetPrivate.h"
#include <glib/gi18n-lib.h>
#include <gtk/gtk.h>
#include <wtf/glib/GRefPtr.h>
#include <wtf/glib/WTFGType.h>
#include <wtf/text/CString.h>

/**
 * CyberKitPrintCustomWidget:
 * @See_also: #CyberKitPrintOperation
 *
 * Allows to embed a custom widget in print dialog.
 *
 * A CyberKitPrintCustomWidget allows to embed a custom widget in the print
 * dialog by connecting to the #CyberKitPrintOperation::create-custom-widget
 * signal, creating a new CyberKitPrintCustomWidget with
 * webkit_print_custom_widget_new() and returning it from there. You can later
 * use webkit_print_operation_run_dialog() to display the dialog.
 *
 * Unfortunately, use of custom widgets is incompatible with modern
 * containerized application frameworks like Flatpak. A print dialog
 * constructed in the application process will not have access to host
 * printers, so instead it must be constructed by a desktop portal service
 * running on the host system. Because this print dialog runs in a separate
 * process, it's not possible to attach a custom widget.
 *
 * Since: 2.16
 *
 * Deprecated: 2.40
 */

ALLOW_DEPRECATED_DECLARATIONS_BEGIN

enum {
    APPLY,
    UPDATE,

    LAST_SIGNAL
};

enum {
    PROP_0,

    PROP_WIDGET,
    PROP_TITLE
};

struct _CyberKitPrintCustomWidgetPrivate {
    CString title;
    GRefPtr<GtkWidget> widget;
};

static guint signals[LAST_SIGNAL] = { 0, };

WEBKIT_DEFINE_TYPE(CyberKitPrintCustomWidget, webkit_print_custom_widget, G_TYPE_OBJECT)

static void webkitPrintCustomWidgetGetProperty(GObject* object, guint propId, GValue* value, GParamSpec* paramSpec)
{
    CyberKitPrintCustomWidget* printCustomWidget = WEBKIT_PRINT_CUSTOM_WIDGET(object);

    switch (propId) {
    case PROP_WIDGET:
        g_value_set_object(value, webkit_print_custom_widget_get_widget(printCustomWidget));
        break;
    case PROP_TITLE:
        g_value_set_string(value, webkit_print_custom_widget_get_title(printCustomWidget));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propId, paramSpec);
    }
}

static void webkitPrintCustomWidgetSetProperty(GObject* object, guint propId, const GValue* value, GParamSpec* paramSpec)
{
    CyberKitPrintCustomWidget* printCustomWidget = WEBKIT_PRINT_CUSTOM_WIDGET(object);

    switch (propId) {
    case PROP_WIDGET:
        printCustomWidget->priv->widget = GTK_WIDGET(g_value_get_object(value));
        break;
    case PROP_TITLE:
        printCustomWidget->priv->title = g_value_get_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propId, paramSpec);
    }
}

static void webkit_print_custom_widget_class_init(CyberKitPrintCustomWidgetClass* printCustomWidgetClass)
{
    GObjectClass* objectClass = G_OBJECT_CLASS(printCustomWidgetClass);
    objectClass->get_property = webkitPrintCustomWidgetGetProperty;
    objectClass->set_property = webkitPrintCustomWidgetSetProperty;

    /**
     * CyberKitPrintCustomWidget:widget:
     *
     * The custom #GtkWidget that will be embedded in the dialog.
     *
     * Since: 2.16
     *
     * Deprecated: 2.40
     */
    g_object_class_install_property(
        objectClass,
        PROP_WIDGET,
        g_param_spec_object(
            "widget",
            nullptr, nullptr,
            GTK_TYPE_WIDGET,
            static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY)));

    /**
     * CyberKitPrintCustomWidget:title:
     *
     * The title of the custom widget.
     *
     * Since: 2.16
     *
     * Deprecated: 2.40
     */
    g_object_class_install_property(
        objectClass,
        PROP_TITLE,
        g_param_spec_string(
            "title",
            nullptr, nullptr,
            nullptr,
            static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY)));

    /**
     * CyberKitPrintCustomWidget::update:
     * @print_custom_widget: the #CyberKitPrintCustomWidget on which the signal was emitted
     * @page_setup: actual page setup
     * @print_settings: actual print settings
     *
     * Emitted after change of selected printer in the dialog. The actual page setup
     * and print settings are available and the custom widget can actualize itself
     * according to their values.
     *
     * Since: 2.16
     *
     * Deprecated: 2.40
     */
    signals[UPDATE] =
        g_signal_new(
            "update",
            G_TYPE_FROM_CLASS(printCustomWidgetClass),
            G_SIGNAL_RUN_LAST,
            G_STRUCT_OFFSET(CyberKitPrintCustomWidgetClass, update),
            0, 0,
            g_cclosure_marshal_generic,
            G_TYPE_NONE, 2,
            GTK_TYPE_PAGE_SETUP, GTK_TYPE_PRINT_SETTINGS);

    /**
     * CyberKitPrintCustomWidget::apply:
     * @print_custom_widget: the #CyberKitPrintCustomWidget on which the signal was emitted
     *
     * Emitted right before the printing will start. You should read the information
     * from the widget and update the content based on it if necessary. The widget
     * is not guaranteed to be valid at a later time.
     *
     * Since: 2.16
     *
     * Deprecated: 2.40
     */
    signals[APPLY] =
        g_signal_new(
            "apply",
            G_TYPE_FROM_CLASS(printCustomWidgetClass),
            G_SIGNAL_RUN_LAST,
            G_STRUCT_OFFSET(CyberKitPrintCustomWidgetClass, apply),
            0, 0,
            g_cclosure_marshal_VOID__VOID,
            G_TYPE_NONE, 0);
}

/**
 * webkit_print_custom_widget_new:
 * @widget: a #GtkWidget
 * @title: a @widget's title
 *
 * Create a new #CyberKitPrintCustomWidget with given @widget and @title.
 *
 * The @widget
 * ownership is taken and it is destroyed together with the dialog even if this
 * object could still be alive at that point. You typically want to pass a container
 * widget with multiple widgets in it.
 *
 * Returns: (transfer full): a new #CyberKitPrintOperation.
 *
 * Since: 2.16
 *
 * Deprecated: 2.40
 */
CyberKitPrintCustomWidget* webkit_print_custom_widget_new(GtkWidget* widget, const char* title)
{
    g_return_val_if_fail(GTK_IS_WIDGET(widget), nullptr);
    g_return_val_if_fail(title, nullptr);

    return WEBKIT_PRINT_CUSTOM_WIDGET(g_object_new(WEBKIT_TYPE_PRINT_CUSTOM_WIDGET, "widget", widget, "title", title, nullptr));
}

/**
 * webkit_print_custom_widget_get_widget:
 * @print_custom_widget: a #CyberKitPrintCustomWidget
 *
 * Return the value of #CyberKitPrintCustomWidget:widget property.
 *
 * Return the value of #CyberKitPrintCustomWidget:widget property for the given
 * @print_custom_widget object. The returned value will always be valid if called
 * from #CyberKitPrintCustomWidget::apply or #CyberKitPrintCustomWidget::update
 * callbacks, but it will be %NULL if called after the
 * #CyberKitPrintCustomWidget::apply signal is emitted.
 *
 * Returns: (transfer none): a #GtkWidget.
 *
 * Since: 2.16
 *
 * Deprecated: 2.40
 */
GtkWidget* webkit_print_custom_widget_get_widget(CyberKitPrintCustomWidget* printCustomWidget)
{
    g_return_val_if_fail(WEBKIT_IS_PRINT_CUSTOM_WIDGET(printCustomWidget), nullptr);

    return printCustomWidget->priv->widget.get();
}

/**
 * webkit_print_custom_widget_get_title:
 * @print_custom_widget: a #CyberKitPrintCustomWidget
 *
 * Return the value of #CyberKitPrintCustomWidget:title property.
 *
 * Return the value of #CyberKitPrintCustomWidget:title property for the given
 * @print_custom_widget object.
 *
 * Returns: Title of the @print_custom_widget.
 *
 * Since: 2.16
 *
 * Deprecated: 2.40
 */
const gchar* webkit_print_custom_widget_get_title(CyberKitPrintCustomWidget* printCustomWidget)
{
    g_return_val_if_fail(WEBKIT_IS_PRINT_CUSTOM_WIDGET(printCustomWidget), nullptr);

    return printCustomWidget->priv->title.data();
}

void webkitPrintCustomWidgetEmitCustomWidgetApplySignal(CyberKitPrintCustomWidget* printCustomWidget)
{
    g_signal_emit(printCustomWidget, signals[APPLY], 0);
    printCustomWidget->priv->widget = nullptr;
}

void webkitPrintCustomWidgetEmitUpdateCustomWidgetSignal(CyberKitPrintCustomWidget *printCustomWidget, GtkPageSetup *pageSetup, GtkPrintSettings *printSettings)
{
    g_signal_emit(printCustomWidget, signals[UPDATE], 0, pageSetup, printSettings);
}

ALLOW_DEPRECATED_DECLARATIONS_END
