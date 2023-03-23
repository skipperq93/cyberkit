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
#include "CyberKitDOMCSSRuleList.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/Document.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMCSSRuleListPrivate.h"
#include "CyberKitDOMCSSRulePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_CSS_RULE_LIST_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_DOM_TYPE_CSS_RULE_LIST, CyberKitDOMCSSRuleListPrivate)

typedef struct _CyberKitDOMCSSRuleListPrivate {
    RefPtr<CyberCore::CSSRuleList> coreObject;
} CyberKitDOMCSSRuleListPrivate;

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMCSSRuleList* kit(CyberCore::CSSRuleList* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_CSS_RULE_LIST(ret);

    return wrapCSSRuleList(obj);
}

CyberCore::CSSRuleList* core(CyberKitDOMCSSRuleList* request)
{
    return request ? static_cast<CyberCore::CSSRuleList*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMCSSRuleList* wrapCSSRuleList(CyberCore::CSSRuleList* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_CSS_RULE_LIST(g_object_new(WEBKIT_DOM_TYPE_CSS_RULE_LIST, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

G_DEFINE_TYPE(CyberKitDOMCSSRuleList, webkit_dom_css_rule_list, WEBKIT_DOM_TYPE_OBJECT)

enum {
    DOM_CSS_RULE_LIST_PROP_0,
    DOM_CSS_RULE_LIST_PROP_LENGTH,
};

static void webkit_dom_css_rule_list_finalize(GObject* object)
{
    CyberKitDOMCSSRuleListPrivate* priv = WEBKIT_DOM_CSS_RULE_LIST_GET_PRIVATE(object);

    CyberKit::DOMObjectCache::forget(priv->coreObject.get());

    priv->~CyberKitDOMCSSRuleListPrivate();
    G_OBJECT_CLASS(webkit_dom_css_rule_list_parent_class)->finalize(object);
}

static void webkit_dom_css_rule_list_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMCSSRuleList* self = WEBKIT_DOM_CSS_RULE_LIST(object);

    switch (propertyId) {
    case DOM_CSS_RULE_LIST_PROP_LENGTH:
        g_value_set_ulong(value, webkit_dom_css_rule_list_get_length(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static GObject* webkit_dom_css_rule_list_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_css_rule_list_parent_class)->constructor(type, constructPropertiesCount, constructProperties);

    CyberKitDOMCSSRuleListPrivate* priv = WEBKIT_DOM_CSS_RULE_LIST_GET_PRIVATE(object);
    priv->coreObject = static_cast<CyberCore::CSSRuleList*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    CyberKit::DOMObjectCache::put(priv->coreObject.get(), object);

    return object;
}

static void webkit_dom_css_rule_list_class_init(CyberKitDOMCSSRuleListClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(CyberKitDOMCSSRuleListPrivate));
    gobjectClass->constructor = webkit_dom_css_rule_list_constructor;
    gobjectClass->finalize = webkit_dom_css_rule_list_finalize;
    gobjectClass->get_property = webkit_dom_css_rule_list_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_CSS_RULE_LIST_PROP_LENGTH,
        g_param_spec_ulong(
            "length",
            "CSSRuleList:length",
            "read-only gulong CSSRuleList:length",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READABLE));

}

static void webkit_dom_css_rule_list_init(CyberKitDOMCSSRuleList* request)
{
    CyberKitDOMCSSRuleListPrivate* priv = WEBKIT_DOM_CSS_RULE_LIST_GET_PRIVATE(request);
    new (priv) CyberKitDOMCSSRuleListPrivate();
}

CyberKitDOMCSSRule* webkit_dom_css_rule_list_item(CyberKitDOMCSSRuleList* self, gulong index)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CSS_RULE_LIST(self), 0);
    CyberCore::CSSRuleList* item = CyberKit::core(self);
    RefPtr<CyberCore::CSSRule> gobjectResult = WTF::getPtr(item->item(index));
    return CyberKit::kit(gobjectResult.get());
}

gulong webkit_dom_css_rule_list_get_length(CyberKitDOMCSSRuleList* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CSS_RULE_LIST(self), 0);
    CyberCore::CSSRuleList* item = CyberKit::core(self);
    gulong result = item->length();
    return result;
}

G_GNUC_END_IGNORE_DEPRECATIONS;
