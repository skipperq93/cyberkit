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
#include "CyberKitDOMCSSValue.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMCSSValuePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_CSS_VALUE_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_DOM_TYPE_CSS_VALUE, CyberKitDOMCSSValuePrivate)

typedef struct _CyberKitDOMCSSValuePrivate {
    RefPtr<CyberCore::DeprecatedCSSOMValue> coreObject;
} CyberKitDOMCSSValuePrivate;

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMCSSValue* kit(CyberCore::DeprecatedCSSOMValue* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_CSS_VALUE(ret);

    return wrapCSSValue(obj);
}

CyberCore::DeprecatedCSSOMValue* core(CyberKitDOMCSSValue* request)
{
    return request ? static_cast<CyberCore::DeprecatedCSSOMValue*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMCSSValue* wrapCSSValue(CyberCore::DeprecatedCSSOMValue* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_CSS_VALUE(g_object_new(WEBKIT_DOM_TYPE_CSS_VALUE, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

G_DEFINE_TYPE(CyberKitDOMCSSValue, webkit_dom_css_value, WEBKIT_DOM_TYPE_OBJECT)

enum {
    DOM_CSS_VALUE_PROP_0,
    DOM_CSS_VALUE_PROP_CSS_TEXT,
    DOM_CSS_VALUE_PROP_CSS_VALUE_TYPE,
};

static void webkit_dom_css_value_finalize(GObject* object)
{
    CyberKitDOMCSSValuePrivate* priv = WEBKIT_DOM_CSS_VALUE_GET_PRIVATE(object);

    CyberKit::DOMObjectCache::forget(priv->coreObject.get());

    priv->~CyberKitDOMCSSValuePrivate();
    G_OBJECT_CLASS(webkit_dom_css_value_parent_class)->finalize(object);
}

static void webkit_dom_css_value_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMCSSValue* self = WEBKIT_DOM_CSS_VALUE(object);

    switch (propertyId) {
    case DOM_CSS_VALUE_PROP_CSS_TEXT:
        webkit_dom_css_value_set_css_text(self, g_value_get_string(value), nullptr);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_css_value_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMCSSValue* self = WEBKIT_DOM_CSS_VALUE(object);

    switch (propertyId) {
    case DOM_CSS_VALUE_PROP_CSS_TEXT:
        g_value_take_string(value, webkit_dom_css_value_get_css_text(self));
        break;
    case DOM_CSS_VALUE_PROP_CSS_VALUE_TYPE:
        g_value_set_uint(value, webkit_dom_css_value_get_css_value_type(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static GObject* webkit_dom_css_value_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_css_value_parent_class)->constructor(type, constructPropertiesCount, constructProperties);

    CyberKitDOMCSSValuePrivate* priv = WEBKIT_DOM_CSS_VALUE_GET_PRIVATE(object);
    priv->coreObject = static_cast<CyberCore::DeprecatedCSSOMValue*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    CyberKit::DOMObjectCache::put(priv->coreObject.get(), object);

    return object;
}

static void webkit_dom_css_value_class_init(CyberKitDOMCSSValueClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(CyberKitDOMCSSValuePrivate));
    gobjectClass->constructor = webkit_dom_css_value_constructor;
    gobjectClass->finalize = webkit_dom_css_value_finalize;
    gobjectClass->set_property = webkit_dom_css_value_set_property;
    gobjectClass->get_property = webkit_dom_css_value_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_CSS_VALUE_PROP_CSS_TEXT,
        g_param_spec_string(
            "css-text",
            "CSSValue:css-text",
            "read-write gchar* CSSValue:css-text",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_CSS_VALUE_PROP_CSS_VALUE_TYPE,
        g_param_spec_uint(
            "css-value-type",
            "CSSValue:css-value-type",
            "read-only gushort CSSValue:css-value-type",
            0, G_MAXUINT, 0,
            WEBKIT_PARAM_READABLE));

}

static void webkit_dom_css_value_init(CyberKitDOMCSSValue* request)
{
    CyberKitDOMCSSValuePrivate* priv = WEBKIT_DOM_CSS_VALUE_GET_PRIVATE(request);
    new (priv) CyberKitDOMCSSValuePrivate();
}

gchar* webkit_dom_css_value_get_css_text(CyberKitDOMCSSValue* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CSS_VALUE(self), 0);
    CyberCore::DeprecatedCSSOMValue* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->cssText());
    return result;
}

void webkit_dom_css_value_set_css_text(CyberKitDOMCSSValue* self, const gchar* value, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_CSS_VALUE(self));
    g_return_if_fail(value);
    g_return_if_fail(!error || !*error);
    CyberCore::DeprecatedCSSOMValue* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    auto result = item->setCssText(convertedValue);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

gushort webkit_dom_css_value_get_css_value_type(CyberKitDOMCSSValue* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CSS_VALUE(self), 0);
    CyberCore::DeprecatedCSSOMValue* item = CyberKit::core(self);
    gushort result = item->cssValueType();
    return result;
}

G_GNUC_END_IGNORE_DEPRECATIONS;
