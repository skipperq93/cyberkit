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
#include "CyberKitDOMXPathExpression.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "CyberKitDOMXPathExpressionPrivate.h"
#include "CyberKitDOMXPathResultPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_XPATH_EXPRESSION_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_DOM_TYPE_XPATH_EXPRESSION, CyberKitDOMXPathExpressionPrivate)

typedef struct _CyberKitDOMXPathExpressionPrivate {
    RefPtr<CyberCore::XPathExpression> coreObject;
} CyberKitDOMXPathExpressionPrivate;

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMXPathExpression* kit(CyberCore::XPathExpression* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_XPATH_EXPRESSION(ret);

    return wrapXPathExpression(obj);
}

CyberCore::XPathExpression* core(CyberKitDOMXPathExpression* request)
{
    return request ? static_cast<CyberCore::XPathExpression*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMXPathExpression* wrapXPathExpression(CyberCore::XPathExpression* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_XPATH_EXPRESSION(g_object_new(WEBKIT_DOM_TYPE_XPATH_EXPRESSION, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

G_DEFINE_TYPE(CyberKitDOMXPathExpression, webkit_dom_xpath_expression, WEBKIT_DOM_TYPE_OBJECT)

static void webkit_dom_xpath_expression_finalize(GObject* object)
{
    CyberKitDOMXPathExpressionPrivate* priv = WEBKIT_DOM_XPATH_EXPRESSION_GET_PRIVATE(object);

    CyberKit::DOMObjectCache::forget(priv->coreObject.get());

    priv->~CyberKitDOMXPathExpressionPrivate();
    G_OBJECT_CLASS(webkit_dom_xpath_expression_parent_class)->finalize(object);
}

static GObject* webkit_dom_xpath_expression_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_xpath_expression_parent_class)->constructor(type, constructPropertiesCount, constructProperties);

    CyberKitDOMXPathExpressionPrivate* priv = WEBKIT_DOM_XPATH_EXPRESSION_GET_PRIVATE(object);
    priv->coreObject = static_cast<CyberCore::XPathExpression*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    CyberKit::DOMObjectCache::put(priv->coreObject.get(), object);

    return object;
}

static void webkit_dom_xpath_expression_class_init(CyberKitDOMXPathExpressionClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(CyberKitDOMXPathExpressionPrivate));
    gobjectClass->constructor = webkit_dom_xpath_expression_constructor;
    gobjectClass->finalize = webkit_dom_xpath_expression_finalize;
}

static void webkit_dom_xpath_expression_init(CyberKitDOMXPathExpression* request)
{
    CyberKitDOMXPathExpressionPrivate* priv = WEBKIT_DOM_XPATH_EXPRESSION_GET_PRIVATE(request);
    new (priv) CyberKitDOMXPathExpressionPrivate();
}

CyberKitDOMXPathResult* webkit_dom_xpath_expression_evaluate(CyberKitDOMXPathExpression* self, CyberKitDOMNode* contextNode, gushort type, CyberKitDOMXPathResult* inResult, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_XPATH_EXPRESSION(self), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(contextNode), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_XPATH_RESULT(inResult), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::XPathExpression* item = CyberKit::core(self);
    CyberCore::Node* convertedContextNode = CyberKit::core(contextNode);
    CyberCore::XPathResult* convertedInResult = CyberKit::core(inResult);
    auto result = item->evaluate(convertedContextNode, type, convertedInResult);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

G_GNUC_END_IGNORE_DEPRECATIONS;
