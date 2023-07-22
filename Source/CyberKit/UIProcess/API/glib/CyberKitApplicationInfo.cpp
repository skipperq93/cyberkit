/*
 * Copyright (C) 2017 Igalia S.L.
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
#include "CyberKitApplicationInfo.h"

#include <wtf/text/CString.h>

struct _CyberKitApplicationInfo {
    CString name;
    uint64_t majorVersion;
    uint64_t minorVersion;
    uint64_t microVersion;

    int referenceCount { 1 };
};

G_DEFINE_BOXED_TYPE(CyberKitApplicationInfo, webkit_application_info, webkit_application_info_ref, webkit_application_info_unref)

/**
 * webkit_application_info_new:
 *
 * Creates a new #CyberKitApplicationInfo
 *
 * Returns: (transfer full): the newly created #CyberKitApplicationInfo.
 *
 * since: 2.18
 */
CyberKitApplicationInfo* webkit_application_info_new()
{
    CyberKitApplicationInfo* info = static_cast<CyberKitApplicationInfo*>(fastMalloc(sizeof(CyberKitApplicationInfo)));
    new (info) CyberKitApplicationInfo();
    return info;
}

/**
 * webkit_application_info_ref:
 * @info: a #CyberKitApplicationInfo
 *
 * Atomically increments the reference count of @info by one. This
 * function is MT-safe and may be called from any thread.
 *
 * Returns: The passed in #CyberKitApplicationInfo
 *
 * Since: 2.18
 */
CyberKitApplicationInfo* webkit_application_info_ref(CyberKitApplicationInfo* info)
{
    g_atomic_int_inc(&info->referenceCount);
    return info;
}

/**
 * webkit_application_info_unref:
 * @info: a #CyberKitApplicationInfo
 *
 * Atomically decrements the reference count of @info by one. If the
 * reference count drops to 0, all memory allocated by the #CyberKitApplicationInfo is
 * released. This function is MT-safe and may be called from any
 * thread.
 *
 * Since: 2.18
 */
void webkit_application_info_unref(CyberKitApplicationInfo* info)
{
    if (g_atomic_int_dec_and_test(&info->referenceCount)) {
        info->~CyberKitApplicationInfo();
        fastFree(info);
    }
}

/**
 * webkit_application_info_set_name:
 * @info: a #CyberKitApplicationInfo
 * @name: the application name
 *
 * Set the name of the application. If not provided, or %NULL is passed,
 * g_get_prgname() will be used.
 *
 * Since: 2.18
 */
void webkit_application_info_set_name(CyberKitApplicationInfo* info, const char* name)
{
    g_return_if_fail(info);

    info->name = name;
}

/**
 * webkit_application_info_get_name:
 * @info: a #CyberKitApplicationInfo
 *
 * Get the name of the application. If webkit_application_info_set_name() hasn't been
 * called with a valid name, this returns g_get_prgname().
 *
 * Returns: the application name
 *
 * Since: 2.18
 */
const char* webkit_application_info_get_name(CyberKitApplicationInfo* info)
{
    g_return_val_if_fail(info, nullptr);

    if (!info->name.isNull())
        return info->name.data();

    return g_get_prgname();
}

/**
 * webkit_application_info_set_version:
 * @info: a #CyberKitApplicationInfo
 * @major: the major version number
 * @minor: the minor version number
 * @micro: the micro version number
 *
 * Set the application version. If the application doesn't use the format
 * major.minor.micro you can pass 0 as the micro to use major.minor, or pass
 * 0 as both micro and minor to use only major number. Any other format must
 * be converted to major.minor.micro so that it can be used in version comparisons.
 *
 * Since: 2.18
 */
void webkit_application_info_set_version(CyberKitApplicationInfo* info, guint64 major, guint64 minor, guint64 micro)
{
    g_return_if_fail(info);

    info->majorVersion = major;
    info->minorVersion = minor;
    info->microVersion = micro;
}

/**
 * webkit_application_info_get_version:
 * @info: a #CyberKitApplicationInfo
 * @major: (out): return location for the major version number
 * @minor: (out) (allow-none): return location for the minor version number
 * @micro: (out) (allow-none): return location for the micro version number
 *
 * Get the application version previously set with webkit_application_info_set_version().
 *
 * Since: 2.18
 */
void webkit_application_info_get_version(CyberKitApplicationInfo* info, guint64* major, guint64* minor, guint64* micro)
{
    g_return_if_fail(info && major);

    *major = info->majorVersion;
    if (minor)
        *minor = info->minorVersion;
    if (micro)
        *micro = info->microVersion;
}
