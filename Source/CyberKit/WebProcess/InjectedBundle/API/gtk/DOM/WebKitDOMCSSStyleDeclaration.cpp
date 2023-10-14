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
#include "CyberKitDOMCSSStyleDeclaration.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMCSSRulePrivate.h"
#include "CyberKitDOMCSSStyleDeclarationPrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_CSS_STYLE_DECLARATION_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_DOM_TYPE_CSS_STYLE_DECLARATION, CyberKitDOMCSSStyleDeclarationPrivate)

typedef struct _CyberKitDOMCSSStyleDeclarationPrivate {
    RefPtr<CyberCore::CSSStyleDeclaration> coreObject;
} CyberKitDOMCSSStyleDeclarationPrivate;

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMCSSStyleDeclaration* kit(CyberCore::CSSStyleDeclaration* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_CSS_STYLE_DECLARATION(ret);

    return wrapCSSStyleDeclaration(obj);
}

CyberCore::CSSStyleDeclaration* core(CyberKitDOMCSSStyleDeclaration* request)
{
    return request ? static_cast<CyberCore::CSSStyleDeclaration*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMCSSStyleDeclaration* wrapCSSStyleDeclaration(CyberCore::CSSStyleDeclaration* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_CSS_STYLE_DECLARATION(g_object_new(WEBKIT_DOM_TYPE_CSS_STYLE_DECLARATION, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

G_DEFINE_TYPE(CyberKitDOMCSSStyleDeclaration, webkit_dom_css_style_declaration, WEBKIT_DOM_TYPE_OBJECT)

enum {
    DOM_CSS_STYLE_DECLARATION_PROP_0,
    DOM_CSS_STYLE_DECLARATION_PROP_CSS_TEXT,
    DOM_CSS_STYLE_DECLARATION_PROP_LENGTH,
    DOM_CSS_STYLE_DECLARATION_PROP_PARENT_RULE,
};

static void webkit_dom_css_style_declaration_finalize(GObject* object)
{
    CyberKitDOMCSSStyleDeclarationPrivate* priv = WEBKIT_DOM_CSS_STYLE_DECLARATION_GET_PRIVATE(object);

    CyberKit::DOMObjectCache::forget(priv->coreObject.get());

    priv->~CyberKitDOMCSSStyleDeclarationPrivate();
    G_OBJECT_CLASS(webkit_dom_css_style_declaration_parent_class)->finalize(object);
}

static void webkit_dom_css_style_declaration_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMCSSStyleDeclaration* self = WEBKIT_DOM_CSS_STYLE_DECLARATION(object);

    switch (propertyId) {
    case DOM_CSS_STYLE_DECLARATION_PROP_CSS_TEXT:
        webkit_dom_css_style_declaration_set_css_text(self, g_value_get_string(value), nullptr);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_css_style_declaration_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMCSSStyleDeclaration* self = WEBKIT_DOM_CSS_STYLE_DECLARATION(object);

    switch (propertyId) {
    case DOM_CSS_STYLE_DECLARATION_PROP_CSS_TEXT:
        g_value_take_string(value, webkit_dom_css_style_declaration_get_css_text(self));
        break;
    case DOM_CSS_STYLE_DECLARATION_PROP_LENGTH:
        g_value_set_ulong(value, webkit_dom_css_style_declaration_get_length(self));
        break;
    case DOM_CSS_STYLE_DECLARATION_PROP_PARENT_RULE:
        g_value_set_object(value, webkit_dom_css_style_declaration_get_parent_rule(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static GObject* webkit_dom_css_style_declaration_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_css_style_declaration_parent_class)->constructor(type, constructPropertiesCount, constructProperties);

    CyberKitDOMCSSStyleDeclarationPrivate* priv = WEBKIT_DOM_CSS_STYLE_DECLARATION_GET_PRIVATE(object);
    priv->coreObject = static_cast<CyberCore::CSSStyleDeclaration*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    CyberKit::DOMObjectCache::put(priv->coreObject.get(), object);

    return object;
}

static void webkit_dom_css_style_declaration_class_init(CyberKitDOMCSSStyleDeclarationClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(CyberKitDOMCSSStyleDeclarationPrivate));
    gobjectClass->constructor = webkit_dom_css_style_declaration_constructor;
    gobjectClass->finalize = webkit_dom_css_style_declaration_finalize;
    gobjectClass->set_property = webkit_dom_css_style_declaration_set_property;
    gobjectClass->get_property = webkit_dom_css_style_declaration_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_CSS_STYLE_DECLARATION_PROP_CSS_TEXT,
        g_param_spec_string(
            "css-text",
            "CSSStyleDeclaration:css-text",
            "read-write gchar* CSSStyleDeclaration:css-text",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_CSS_STYLE_DECLARATION_PROP_LENGTH,
        g_param_spec_ulong(
            "length",
            "CSSStyleDeclaration:length",
            "read-only gulong CSSStyleDeclaration:length",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_CSS_STYLE_DECLARATION_PROP_PARENT_RULE,
        g_param_spec_object(
            "parent-rule",
            "CSSStyleDeclaration:parent-rule",
            "read-only CyberKitDOMCSSRule* CSSStyleDeclaration:parent-rule",
            WEBKIT_DOM_TYPE_CSS_RULE,
            WEBKIT_PARAM_READABLE));

}

static void webkit_dom_css_style_declaration_init(CyberKitDOMCSSStyleDeclaration* request)
{
    CyberKitDOMCSSStyleDeclarationPrivate* priv = WEBKIT_DOM_CSS_STYLE_DECLARATION_GET_PRIVATE(request);
    new (priv) CyberKitDOMCSSStyleDeclarationPrivate();
}

gchar* webkit_dom_css_style_declaration_get_property_value(CyberKitDOMCSSStyleDeclaration* self, const gchar* propertyName)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CSS_STYLE_DECLARATION(self), 0);
    g_return_val_if_fail(propertyName, 0);
    CyberCore::CSSStyleDeclaration* item = CyberKit::core(self);
    WTF::String convertedPropertyName = WTF::String::fromUTF8(propertyName);
    gchar* result = convertToUTF8String(item->getPropertyValue(convertedPropertyName));
    return result;
}

gchar* webkit_dom_css_style_declaration_remove_property(CyberKitDOMCSSStyleDeclaration* self, const gchar* propertyName, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CSS_STYLE_DECLARATION(self), 0);
    g_return_val_if_fail(propertyName, 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::CSSStyleDeclaration* item = CyberKit::core(self);
    WTF::String convertedPropertyName = WTF::String::fromUTF8(propertyName);
    auto result = item->removeProperty(convertedPropertyName);
    if (result.hasException())
        return nullptr;
    return convertToUTF8String(result.releaseReturnValue());
}

gchar* webkit_dom_css_style_declaration_get_property_priority(CyberKitDOMCSSStyleDeclaration* self, const gchar* propertyName)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CSS_STYLE_DECLARATION(self), 0);
    g_return_val_if_fail(propertyName, 0);
    CyberCore::CSSStyleDeclaration* item = CyberKit::core(self);
    WTF::String convertedPropertyName = WTF::String::fromUTF8(propertyName);
    gchar* result = convertToUTF8String(item->getPropertyPriority(convertedPropertyName));
    return result;
}

void webkit_dom_css_style_declaration_set_property(CyberKitDOMCSSStyleDeclaration* self, const gchar* propertyName, const gchar* value, const gchar* priority, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_CSS_STYLE_DECLARATION(self));
    g_return_if_fail(propertyName);
    g_return_if_fail(value);
    g_return_if_fail(priority);
    g_return_if_fail(!error || !*error);
    CyberCore::CSSStyleDeclaration* item = CyberKit::core(self);
    WTF::String convertedPropertyName = WTF::String::fromUTF8(propertyName);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    WTF::String convertedPriority = WTF::String::fromUTF8(priority);
    auto result = item->setProperty(convertedPropertyName, convertedValue, convertedPriority);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

gchar* webkit_dom_css_style_declaration_item(CyberKitDOMCSSStyleDeclaration* self, gulong index)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CSS_STYLE_DECLARATION(self), 0);
    CyberCore::CSSStyleDeclaration* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->item(index));
    return result;
}

gchar* webkit_dom_css_style_declaration_get_property_shorthand(CyberKitDOMCSSStyleDeclaration* self, const gchar* propertyName)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CSS_STYLE_DECLARATION(self), 0);
    g_return_val_if_fail(propertyName, 0);
    CyberCore::CSSStyleDeclaration* item = CyberKit::core(self);
    WTF::String convertedPropertyName = WTF::String::fromUTF8(propertyName);
    gchar* result = convertToUTF8String(item->getPropertyShorthand(convertedPropertyName));
    return result;
}

gboolean webkit_dom_css_style_declaration_is_property_implicit(CyberKitDOMCSSStyleDeclaration* self, const gchar* propertyName)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CSS_STYLE_DECLARATION(self), FALSE);
    g_return_val_if_fail(propertyName, FALSE);
    CyberCore::CSSStyleDeclaration* item = CyberKit::core(self);
    WTF::String convertedPropertyName = WTF::String::fromUTF8(propertyName);
    gboolean result = item->isPropertyImplicit(convertedPropertyName);
    return result;
}

gchar* webkit_dom_css_style_declaration_get_css_text(CyberKitDOMCSSStyleDeclaration* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CSS_STYLE_DECLARATION(self), 0);
    CyberCore::CSSStyleDeclaration* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->cssText());
    return result;
}

void webkit_dom_css_style_declaration_set_css_text(CyberKitDOMCSSStyleDeclaration* self, const gchar* value, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_CSS_STYLE_DECLARATION(self));
    g_return_if_fail(value);
    g_return_if_fail(!error || !*error);
    CyberCore::CSSStyleDeclaration* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    auto result = item->setCssText(convertedValue);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

gulong webkit_dom_css_style_declaration_get_length(CyberKitDOMCSSStyleDeclaration* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CSS_STYLE_DECLARATION(self), 0);
    CyberCore::CSSStyleDeclaration* item = CyberKit::core(self);
    gulong result = item->length();
    return result;
}

CyberKitDOMCSSRule* webkit_dom_css_style_declaration_get_parent_rule(CyberKitDOMCSSStyleDeclaration* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CSS_STYLE_DECLARATION(self), 0);
    CyberCore::CSSStyleDeclaration* item = CyberKit::core(self);
    RefPtr<CyberCore::CSSRule> gobjectResult = WTF::getPtr(item->parentRule());
    return CyberKit::kit(gobjectResult.get());
}

G_GNUC_END_IGNORE_DEPRECATIONS;
