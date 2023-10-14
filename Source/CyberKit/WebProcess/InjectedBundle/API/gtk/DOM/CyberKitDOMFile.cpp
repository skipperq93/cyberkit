/*
 *  This file is part of the CyberKit open source project.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "CyberKitDOMFile.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/Document.h>
#include <CyberCore/ExceptionCode.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMBlobPrivate.h"
#include "CyberKitDOMFilePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMFile* kit(CyberCore::File* obj)
{
    return WEBKIT_DOM_FILE(kit(static_cast<CyberCore::Blob*>(obj)));
}

CyberCore::File* core(CyberKitDOMFile* request)
{
    return request ? static_cast<CyberCore::File*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMFile* wrapFile(CyberCore::File* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_FILE(g_object_new(WEBKIT_DOM_TYPE_FILE, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

G_DEFINE_TYPE(CyberKitDOMFile, webkit_dom_file, WEBKIT_DOM_TYPE_BLOB)

enum {
    DOM_FILE_PROP_0,
    DOM_FILE_PROP_NAME,
};

static void webkit_dom_file_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMFile* self = WEBKIT_DOM_FILE(object);

    switch (propertyId) {
    case DOM_FILE_PROP_NAME:
        g_value_take_string(value, webkit_dom_file_get_name(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_file_class_init(CyberKitDOMFileClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    gobjectClass->get_property = webkit_dom_file_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_FILE_PROP_NAME,
        g_param_spec_string(
            "name",
            "File:name",
            "read-only gchar* File:name",
            "",
            WEBKIT_PARAM_READABLE));
}

static void webkit_dom_file_init(CyberKitDOMFile* request)
{
    UNUSED_PARAM(request);
}

gchar* webkit_dom_file_get_name(CyberKitDOMFile* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_FILE(self), 0);
    CyberCore::File* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->name());
    return result;
}
G_GNUC_END_IGNORE_DEPRECATIONS;
