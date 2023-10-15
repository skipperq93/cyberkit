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

#include "config.h"
#include "CyberKitPlugin.h"

#include <wtf/glib/WTFGType.h>

/**
 * CyberKitPlugin:
 *
 * Represents a plugin, enabling fine-grained control.
 *
 * This object represents a single plugin, found while scanning the
 * various platform plugin directories. This object can be used to get
 * more information about a plugin, and enable/disable it, allowing
 * fine-grained control of plugins. The list of available plugins can
 * be obtained from the #CyberKitWebContext, with
 * webkit_web_context_get_plugins().
 *
 * Deprecated: 2.32
 */

struct _CyberKitPluginPrivate {
};

ALLOW_DEPRECATED_DECLARATIONS_BEGIN
WEBKIT_DEFINE_TYPE(CyberKitPlugin, webkit_plugin, G_TYPE_OBJECT)
ALLOW_DEPRECATED_DECLARATIONS_END

static void webkit_plugin_class_init(CyberKitPluginClass*)
{
}

/**
 * webkit_plugin_get_name:
 * @plugin: a #CyberKitPlugin
 *
 * Obtain the plugin name.
 *
 * Returns: (nullable): name, as a string.
 *
 * Deprecated: 2.32
 */
const char* webkit_plugin_get_name(CyberKitPlugin*)
{
    return nullptr;
}

/**
 * webkit_plugin_get_description:
 * @plugin: a #CyberKitPlugin
 *
 * Obtain the plugin description.
 *
 * Returns: (nullable): description, as a string.
 *
 * Deprecated: 2.32
 */
const char* webkit_plugin_get_description(CyberKitPlugin*)
{
    return nullptr;
}

/**
 * webkit_plugin_get_path:
 * @plugin: a #CyberKitPlugin
 *
 * Obtain the absolute path where the plugin is installed.
 *
 * Returns: (nullable): path, as a string.
 *
 * Deprecated: 2.32
 */
const char* webkit_plugin_get_path(CyberKitPlugin*)
{
    return nullptr;
}

/**
 * webkit_plugin_get_mime_info_list:
 * @plugin: a #CyberKitPlugin
 *
 * Get information about MIME types handled by the plugin.
 *
 * Get information about MIME types handled by the plugin,
 * as a list of #CyberKitMimeInfo.
 *
 * Returns: (element-type CyberKitMimeInfo) (transfer none): a #GList of #CyberKitMimeInfo.
 *
 * Deprecated: 2.32
 */
GList* webkit_plugin_get_mime_info_list(CyberKitPlugin*)
{
    return nullptr;
}
