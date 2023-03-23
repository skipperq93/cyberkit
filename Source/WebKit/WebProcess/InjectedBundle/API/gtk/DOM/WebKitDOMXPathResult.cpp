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
#include "CyberKitDOMXPathResult.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "CyberKitDOMXPathResultPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_XPATH_RESULT_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_DOM_TYPE_XPATH_RESULT, CyberKitDOMXPathResultPrivate)

typedef struct _CyberKitDOMXPathResultPrivate {
    RefPtr<CyberCore::XPathResult> coreObject;
} CyberKitDOMXPathResultPrivate;

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMXPathResult* kit(CyberCore::XPathResult* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_XPATH_RESULT(ret);

    return wrapXPathResult(obj);
}

CyberCore::XPathResult* core(CyberKitDOMXPathResult* request)
{
    return request ? static_cast<CyberCore::XPathResult*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMXPathResult* wrapXPathResult(CyberCore::XPathResult* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_XPATH_RESULT(g_object_new(WEBKIT_DOM_TYPE_XPATH_RESULT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

G_DEFINE_TYPE(CyberKitDOMXPathResult, webkit_dom_xpath_result, WEBKIT_DOM_TYPE_OBJECT)

enum {
    DOM_XPATH_RESULT_PROP_0,
    DOM_XPATH_RESULT_PROP_RESULT_TYPE,
    DOM_XPATH_RESULT_PROP_NUMBER_VALUE,
    DOM_XPATH_RESULT_PROP_STRING_VALUE,
    DOM_XPATH_RESULT_PROP_BOOLEAN_VALUE,
    DOM_XPATH_RESULT_PROP_SINGLE_NODE_VALUE,
    DOM_XPATH_RESULT_PROP_INVALID_ITERATOR_STATE,
    DOM_XPATH_RESULT_PROP_SNAPSHOT_LENGTH,
};

static void webkit_dom_xpath_result_finalize(GObject* object)
{
    CyberKitDOMXPathResultPrivate* priv = WEBKIT_DOM_XPATH_RESULT_GET_PRIVATE(object);

    CyberKit::DOMObjectCache::forget(priv->coreObject.get());

    priv->~CyberKitDOMXPathResultPrivate();
    G_OBJECT_CLASS(webkit_dom_xpath_result_parent_class)->finalize(object);
}

static void webkit_dom_xpath_result_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMXPathResult* self = WEBKIT_DOM_XPATH_RESULT(object);

    switch (propertyId) {
    case DOM_XPATH_RESULT_PROP_RESULT_TYPE:
        g_value_set_uint(value, webkit_dom_xpath_result_get_result_type(self));
        break;
    case DOM_XPATH_RESULT_PROP_NUMBER_VALUE:
        g_value_set_double(value, webkit_dom_xpath_result_get_number_value(self, nullptr));
        break;
    case DOM_XPATH_RESULT_PROP_STRING_VALUE:
        g_value_take_string(value, webkit_dom_xpath_result_get_string_value(self, nullptr));
        break;
    case DOM_XPATH_RESULT_PROP_BOOLEAN_VALUE:
        g_value_set_boolean(value, webkit_dom_xpath_result_get_boolean_value(self, nullptr));
        break;
    case DOM_XPATH_RESULT_PROP_SINGLE_NODE_VALUE:
        g_value_set_object(value, webkit_dom_xpath_result_get_single_node_value(self, nullptr));
        break;
    case DOM_XPATH_RESULT_PROP_INVALID_ITERATOR_STATE:
        g_value_set_boolean(value, webkit_dom_xpath_result_get_invalid_iterator_state(self));
        break;
    case DOM_XPATH_RESULT_PROP_SNAPSHOT_LENGTH:
        g_value_set_ulong(value, webkit_dom_xpath_result_get_snapshot_length(self, nullptr));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static GObject* webkit_dom_xpath_result_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_xpath_result_parent_class)->constructor(type, constructPropertiesCount, constructProperties);

    CyberKitDOMXPathResultPrivate* priv = WEBKIT_DOM_XPATH_RESULT_GET_PRIVATE(object);
    priv->coreObject = static_cast<CyberCore::XPathResult*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    CyberKit::DOMObjectCache::put(priv->coreObject.get(), object);

    return object;
}

static void webkit_dom_xpath_result_class_init(CyberKitDOMXPathResultClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(CyberKitDOMXPathResultPrivate));
    gobjectClass->constructor = webkit_dom_xpath_result_constructor;
    gobjectClass->finalize = webkit_dom_xpath_result_finalize;
    gobjectClass->get_property = webkit_dom_xpath_result_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_XPATH_RESULT_PROP_RESULT_TYPE,
        g_param_spec_uint(
            "result-type",
            "XPathResult:result-type",
            "read-only gushort XPathResult:result-type",
            0, G_MAXUINT, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_XPATH_RESULT_PROP_NUMBER_VALUE,
        g_param_spec_double(
            "number-value",
            "XPathResult:number-value",
            "read-only gdouble XPathResult:number-value",
            -G_MAXDOUBLE, G_MAXDOUBLE, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_XPATH_RESULT_PROP_STRING_VALUE,
        g_param_spec_string(
            "string-value",
            "XPathResult:string-value",
            "read-only gchar* XPathResult:string-value",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_XPATH_RESULT_PROP_BOOLEAN_VALUE,
        g_param_spec_boolean(
            "boolean-value",
            "XPathResult:boolean-value",
            "read-only gboolean XPathResult:boolean-value",
            FALSE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_XPATH_RESULT_PROP_SINGLE_NODE_VALUE,
        g_param_spec_object(
            "single-node-value",
            "XPathResult:single-node-value",
            "read-only CyberKitDOMNode* XPathResult:single-node-value",
            WEBKIT_DOM_TYPE_NODE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_XPATH_RESULT_PROP_INVALID_ITERATOR_STATE,
        g_param_spec_boolean(
            "invalid-iterator-state",
            "XPathResult:invalid-iterator-state",
            "read-only gboolean XPathResult:invalid-iterator-state",
            FALSE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_XPATH_RESULT_PROP_SNAPSHOT_LENGTH,
        g_param_spec_ulong(
            "snapshot-length",
            "XPathResult:snapshot-length",
            "read-only gulong XPathResult:snapshot-length",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READABLE));

}

static void webkit_dom_xpath_result_init(CyberKitDOMXPathResult* request)
{
    CyberKitDOMXPathResultPrivate* priv = WEBKIT_DOM_XPATH_RESULT_GET_PRIVATE(request);
    new (priv) CyberKitDOMXPathResultPrivate();
}

CyberKitDOMNode* webkit_dom_xpath_result_iterate_next(CyberKitDOMXPathResult* self, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_XPATH_RESULT(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::XPathResult* item = CyberKit::core(self);
    auto result = item->iterateNext();
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue());
}

CyberKitDOMNode* webkit_dom_xpath_result_snapshot_item(CyberKitDOMXPathResult* self, gulong index, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_XPATH_RESULT(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::XPathResult* item = CyberKit::core(self);
    auto result = item->snapshotItem(index);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue());
}

gushort webkit_dom_xpath_result_get_result_type(CyberKitDOMXPathResult* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_XPATH_RESULT(self), 0);
    CyberCore::XPathResult* item = CyberKit::core(self);
    gushort result = item->resultType();
    return result;
}

gdouble webkit_dom_xpath_result_get_number_value(CyberKitDOMXPathResult* self, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_XPATH_RESULT(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::XPathResult* item = CyberKit::core(self);
    auto result = item->numberValue();
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return 0;
    }
    return result.releaseReturnValue();
}

gchar* webkit_dom_xpath_result_get_string_value(CyberKitDOMXPathResult* self, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_XPATH_RESULT(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::XPathResult* item = CyberKit::core(self);
    auto result = item->stringValue();
    if (result.hasException())
        return nullptr;
    return convertToUTF8String(result.releaseReturnValue());
}

gboolean webkit_dom_xpath_result_get_boolean_value(CyberKitDOMXPathResult* self, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_XPATH_RESULT(self), FALSE);
    g_return_val_if_fail(!error || !*error, FALSE);
    CyberCore::XPathResult* item = CyberKit::core(self);
    auto result = item->booleanValue();
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

CyberKitDOMNode* webkit_dom_xpath_result_get_single_node_value(CyberKitDOMXPathResult* self, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_XPATH_RESULT(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::XPathResult* item = CyberKit::core(self);
    auto result = item->singleNodeValue();
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue());
}

gboolean webkit_dom_xpath_result_get_invalid_iterator_state(CyberKitDOMXPathResult* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_XPATH_RESULT(self), FALSE);
    CyberCore::XPathResult* item = CyberKit::core(self);
    gboolean result = item->invalidIteratorState();
    return result;
}

gulong webkit_dom_xpath_result_get_snapshot_length(CyberKitDOMXPathResult* self, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_XPATH_RESULT(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::XPathResult* item = CyberKit::core(self);
    auto result = item->snapshotLength();
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
    return result.releaseReturnValue();
}

G_GNUC_END_IGNORE_DEPRECATIONS;
