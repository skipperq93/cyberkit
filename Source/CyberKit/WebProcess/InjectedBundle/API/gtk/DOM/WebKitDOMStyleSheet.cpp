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
#include "CyberKitDOMStyleSheet.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/Document.h>
#include <CyberCore/ExceptionCode.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMMediaListPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "CyberKitDOMStyleSheetPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_STYLE_SHEET_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_DOM_TYPE_STYLE_SHEET, CyberKitDOMStyleSheetPrivate)

typedef struct _CyberKitDOMStyleSheetPrivate {
    RefPtr<CyberCore::StyleSheet> coreObject;
} CyberKitDOMStyleSheetPrivate;

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMStyleSheet* kit(CyberCore::StyleSheet* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_STYLE_SHEET(ret);

    return wrap(obj);
}

CyberCore::StyleSheet* core(CyberKitDOMStyleSheet* request)
{
    return request ? static_cast<CyberCore::StyleSheet*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMStyleSheet* wrapStyleSheet(CyberCore::StyleSheet* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_STYLE_SHEET(g_object_new(WEBKIT_DOM_TYPE_STYLE_SHEET, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

G_DEFINE_TYPE(CyberKitDOMStyleSheet, webkit_dom_style_sheet, WEBKIT_DOM_TYPE_OBJECT)

enum {
    DOM_STYLE_SHEET_PROP_0,
    DOM_STYLE_SHEET_PROP_TYPE,
    DOM_STYLE_SHEET_PROP_DISABLED,
    DOM_STYLE_SHEET_PROP_OWNER_NODE,
    DOM_STYLE_SHEET_PROP_PARENT_STYLE_SHEET,
    DOM_STYLE_SHEET_PROP_HREF,
    DOM_STYLE_SHEET_PROP_TITLE,
    DOM_STYLE_SHEET_PROP_MEDIA,
};

static void webkit_dom_style_sheet_finalize(GObject* object)
{
    CyberKitDOMStyleSheetPrivate* priv = WEBKIT_DOM_STYLE_SHEET_GET_PRIVATE(object);

    CyberKit::DOMObjectCache::forget(priv->coreObject.get());

    priv->~CyberKitDOMStyleSheetPrivate();
    G_OBJECT_CLASS(webkit_dom_style_sheet_parent_class)->finalize(object);
}

static void webkit_dom_style_sheet_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMStyleSheet* self = WEBKIT_DOM_STYLE_SHEET(object);

    switch (propertyId) {
    case DOM_STYLE_SHEET_PROP_DISABLED:
        webkit_dom_style_sheet_set_disabled(self, g_value_get_boolean(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_style_sheet_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMStyleSheet* self = WEBKIT_DOM_STYLE_SHEET(object);

    switch (propertyId) {
    case DOM_STYLE_SHEET_PROP_TYPE:
        g_value_take_string(value, webkit_dom_style_sheet_get_content_type(self));
        break;
    case DOM_STYLE_SHEET_PROP_DISABLED:
        g_value_set_boolean(value, webkit_dom_style_sheet_get_disabled(self));
        break;
    case DOM_STYLE_SHEET_PROP_OWNER_NODE:
        g_value_set_object(value, webkit_dom_style_sheet_get_owner_node(self));
        break;
    case DOM_STYLE_SHEET_PROP_PARENT_STYLE_SHEET:
        g_value_set_object(value, webkit_dom_style_sheet_get_parent_style_sheet(self));
        break;
    case DOM_STYLE_SHEET_PROP_HREF:
        g_value_take_string(value, webkit_dom_style_sheet_get_href(self));
        break;
    case DOM_STYLE_SHEET_PROP_TITLE:
        g_value_take_string(value, webkit_dom_style_sheet_get_title(self));
        break;
    case DOM_STYLE_SHEET_PROP_MEDIA:
        g_value_set_object(value, webkit_dom_style_sheet_get_media(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static GObject* webkit_dom_style_sheet_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_style_sheet_parent_class)->constructor(type, constructPropertiesCount, constructProperties);

    CyberKitDOMStyleSheetPrivate* priv = WEBKIT_DOM_STYLE_SHEET_GET_PRIVATE(object);
    priv->coreObject = static_cast<CyberCore::StyleSheet*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    CyberKit::DOMObjectCache::put(priv->coreObject.get(), object);

    return object;
}

static void webkit_dom_style_sheet_class_init(CyberKitDOMStyleSheetClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(CyberKitDOMStyleSheetPrivate));
    gobjectClass->constructor = webkit_dom_style_sheet_constructor;
    gobjectClass->finalize = webkit_dom_style_sheet_finalize;
    gobjectClass->set_property = webkit_dom_style_sheet_set_property;
    gobjectClass->get_property = webkit_dom_style_sheet_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_STYLE_SHEET_PROP_TYPE,
        g_param_spec_string(
            "type",
            "StyleSheet:type",
            "read-only gchar* StyleSheet:type",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_STYLE_SHEET_PROP_DISABLED,
        g_param_spec_boolean(
            "disabled",
            "StyleSheet:disabled",
            "read-write gboolean StyleSheet:disabled",
            FALSE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_STYLE_SHEET_PROP_OWNER_NODE,
        g_param_spec_object(
            "owner-node",
            "StyleSheet:owner-node",
            "read-only CyberKitDOMNode* StyleSheet:owner-node",
            WEBKIT_DOM_TYPE_NODE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_STYLE_SHEET_PROP_PARENT_STYLE_SHEET,
        g_param_spec_object(
            "parent-style-sheet",
            "StyleSheet:parent-style-sheet",
            "read-only CyberKitDOMStyleSheet* StyleSheet:parent-style-sheet",
            WEBKIT_DOM_TYPE_STYLE_SHEET,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_STYLE_SHEET_PROP_HREF,
        g_param_spec_string(
            "href",
            "StyleSheet:href",
            "read-only gchar* StyleSheet:href",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_STYLE_SHEET_PROP_TITLE,
        g_param_spec_string(
            "title",
            "StyleSheet:title",
            "read-only gchar* StyleSheet:title",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_STYLE_SHEET_PROP_MEDIA,
        g_param_spec_object(
            "media",
            "StyleSheet:media",
            "read-only CyberKitDOMMediaList* StyleSheet:media",
            WEBKIT_DOM_TYPE_MEDIA_LIST,
            WEBKIT_PARAM_READABLE));

}

static void webkit_dom_style_sheet_init(CyberKitDOMStyleSheet* request)
{
    CyberKitDOMStyleSheetPrivate* priv = WEBKIT_DOM_STYLE_SHEET_GET_PRIVATE(request);
    new (priv) CyberKitDOMStyleSheetPrivate();
}

gchar* webkit_dom_style_sheet_get_content_type(CyberKitDOMStyleSheet* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_STYLE_SHEET(self), 0);
    CyberCore::StyleSheet* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->type());
    return result;
}

gboolean webkit_dom_style_sheet_get_disabled(CyberKitDOMStyleSheet* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_STYLE_SHEET(self), FALSE);
    CyberCore::StyleSheet* item = CyberKit::core(self);
    gboolean result = item->disabled();
    return result;
}

void webkit_dom_style_sheet_set_disabled(CyberKitDOMStyleSheet* self, gboolean value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_STYLE_SHEET(self));
    CyberCore::StyleSheet* item = CyberKit::core(self);
    item->setDisabled(value);
}

CyberKitDOMNode* webkit_dom_style_sheet_get_owner_node(CyberKitDOMStyleSheet* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_STYLE_SHEET(self), 0);
    CyberCore::StyleSheet* item = CyberKit::core(self);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->ownerNode());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMStyleSheet* webkit_dom_style_sheet_get_parent_style_sheet(CyberKitDOMStyleSheet* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_STYLE_SHEET(self), 0);
    CyberCore::StyleSheet* item = CyberKit::core(self);
    RefPtr<CyberCore::StyleSheet> gobjectResult = WTF::getPtr(item->parentStyleSheet());
    return CyberKit::kit(gobjectResult.get());
}

gchar* webkit_dom_style_sheet_get_href(CyberKitDOMStyleSheet* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_STYLE_SHEET(self), 0);
    CyberCore::StyleSheet* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->href());
    return result;
}

gchar* webkit_dom_style_sheet_get_title(CyberKitDOMStyleSheet* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_STYLE_SHEET(self), 0);
    CyberCore::StyleSheet* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->title());
    return result;
}

CyberKitDOMMediaList* webkit_dom_style_sheet_get_media(CyberKitDOMStyleSheet* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_STYLE_SHEET(self), 0);
    CyberCore::StyleSheet* item = CyberKit::core(self);
    RefPtr<CyberCore::MediaList> gobjectResult = WTF::getPtr(item->media());
    return CyberKit::kit(gobjectResult.get());
}

G_GNUC_END_IGNORE_DEPRECATIONS;
