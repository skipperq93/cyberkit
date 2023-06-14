/*
 * Copyright (C) 2011 Igalia S.L.
 * Copyright (C) 2008 Luca Bruno <lethalman88@gmail.com>
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
#include "CyberKitError.h"

#include "APIError.h"
#include "CyberKitPrivate.h"

using namespace CyberCore;

/**
 * SECTION: CyberKitError
 * @Short_description: Categorized CyberKit errors
 * @Title: CyberKitError
 *
 * Categorized CyberKit errors.
 *
 */

GQuark webkit_network_error_quark()
{
    return g_quark_from_static_string(reinterpret_cast<const char*>(API::Error::webKitNetworkErrorDomain().characters8()));
}

GQuark webkit_policy_error_quark()
{
    return g_quark_from_static_string(reinterpret_cast<const char*>(API::Error::webKitPolicyErrorDomain().characters8()));
}

GQuark webkit_plugin_error_quark()
{
    return g_quark_from_static_string(reinterpret_cast<const char*>(API::Error::webKitPluginErrorDomain().characters8()));
}

GQuark webkit_download_error_quark()
{
    return g_quark_from_static_string(reinterpret_cast<const char*>(API::Error::webKitDownloadErrorDomain().characters8()));
}

#if PLATFORM(GTK)
GQuark webkit_print_error_quark()
{
    return g_quark_from_static_string(reinterpret_cast<const char*>(API::Error::webKitPrintErrorDomain().characters8()));
}
#endif

GQuark webkit_javascript_error_quark()
{
    return g_quark_from_static_string("CyberKitJavascriptError");
}

GQuark webkit_snapshot_error_quark()
{
    return g_quark_from_static_string("CyberKitSnapshotError");
}
