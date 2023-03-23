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
#include "CyberKitDOMNodeIterator.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/Document.h>
#include <CyberCore/ExceptionCode.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMNodeFilterPrivate.h"
#include "CyberKitDOMNodeIteratorPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_NODE_ITERATOR_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_DOM_TYPE_NODE_ITERATOR, CyberKitDOMNodeIteratorPrivate)

typedef struct _CyberKitDOMNodeIteratorPrivate {
    RefPtr<CyberCore::NodeIterator> coreObject;
} CyberKitDOMNodeIteratorPrivate;

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMNodeIterator* kit(CyberCore::NodeIterator* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_NODE_ITERATOR(ret);

    return wrapNodeIterator(obj);
}

CyberCore::NodeIterator* core(CyberKitDOMNodeIterator* request)
{
    return request ? static_cast<CyberCore::NodeIterator*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMNodeIterator* wrapNodeIterator(CyberCore::NodeIterator* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_NODE_ITERATOR(g_object_new(WEBKIT_DOM_TYPE_NODE_ITERATOR, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

G_DEFINE_TYPE(CyberKitDOMNodeIterator, webkit_dom_node_iterator, WEBKIT_DOM_TYPE_OBJECT)

enum {
    DOM_NODE_ITERATOR_PROP_0,
    DOM_NODE_ITERATOR_PROP_ROOT,
    DOM_NODE_ITERATOR_PROP_WHAT_TO_SHOW,
    DOM_NODE_ITERATOR_PROP_FILTER,
    DOM_NODE_ITERATOR_PROP_REFERENCE_NODE,
    DOM_NODE_ITERATOR_PROP_POINTER_BEFORE_REFERENCE_NODE,
};

static void webkit_dom_node_iterator_finalize(GObject* object)
{
    CyberKitDOMNodeIteratorPrivate* priv = WEBKIT_DOM_NODE_ITERATOR_GET_PRIVATE(object);

    CyberKit::DOMObjectCache::forget(priv->coreObject.get());

    priv->~CyberKitDOMNodeIteratorPrivate();
    G_OBJECT_CLASS(webkit_dom_node_iterator_parent_class)->finalize(object);
}

static void webkit_dom_node_iterator_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMNodeIterator* self = WEBKIT_DOM_NODE_ITERATOR(object);

    switch (propertyId) {
    case DOM_NODE_ITERATOR_PROP_ROOT:
        g_value_set_object(value, webkit_dom_node_iterator_get_root(self));
        break;
    case DOM_NODE_ITERATOR_PROP_WHAT_TO_SHOW:
        g_value_set_ulong(value, webkit_dom_node_iterator_get_what_to_show(self));
        break;
    case DOM_NODE_ITERATOR_PROP_FILTER:
        g_value_set_object(value, webkit_dom_node_iterator_get_filter(self));
        break;
    case DOM_NODE_ITERATOR_PROP_REFERENCE_NODE:
        g_value_set_object(value, webkit_dom_node_iterator_get_reference_node(self));
        break;
    case DOM_NODE_ITERATOR_PROP_POINTER_BEFORE_REFERENCE_NODE:
        g_value_set_boolean(value, webkit_dom_node_iterator_get_pointer_before_reference_node(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static GObject* webkit_dom_node_iterator_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_node_iterator_parent_class)->constructor(type, constructPropertiesCount, constructProperties);

    CyberKitDOMNodeIteratorPrivate* priv = WEBKIT_DOM_NODE_ITERATOR_GET_PRIVATE(object);
    priv->coreObject = static_cast<CyberCore::NodeIterator*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    CyberKit::DOMObjectCache::put(priv->coreObject.get(), object);

    return object;
}

static void webkit_dom_node_iterator_class_init(CyberKitDOMNodeIteratorClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(CyberKitDOMNodeIteratorPrivate));
    gobjectClass->constructor = webkit_dom_node_iterator_constructor;
    gobjectClass->finalize = webkit_dom_node_iterator_finalize;
    gobjectClass->get_property = webkit_dom_node_iterator_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_ITERATOR_PROP_ROOT,
        g_param_spec_object(
            "root",
            "NodeIterator:root",
            "read-only CyberKitDOMNode* NodeIterator:root",
            WEBKIT_DOM_TYPE_NODE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_ITERATOR_PROP_WHAT_TO_SHOW,
        g_param_spec_ulong(
            "what-to-show",
            "NodeIterator:what-to-show",
            "read-only gulong NodeIterator:what-to-show",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_ITERATOR_PROP_FILTER,
        g_param_spec_object(
            "filter",
            "NodeIterator:filter",
            "read-only CyberKitDOMNodeFilter* NodeIterator:filter",
            WEBKIT_DOM_TYPE_NODE_FILTER,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_ITERATOR_PROP_REFERENCE_NODE,
        g_param_spec_object(
            "reference-node",
            "NodeIterator:reference-node",
            "read-only CyberKitDOMNode* NodeIterator:reference-node",
            WEBKIT_DOM_TYPE_NODE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_ITERATOR_PROP_POINTER_BEFORE_REFERENCE_NODE,
        g_param_spec_boolean(
            "pointer-before-reference-node",
            "NodeIterator:pointer-before-reference-node",
            "read-only gboolean NodeIterator:pointer-before-reference-node",
            FALSE,
            WEBKIT_PARAM_READABLE));

}

static void webkit_dom_node_iterator_init(CyberKitDOMNodeIterator* request)
{
    CyberKitDOMNodeIteratorPrivate* priv = WEBKIT_DOM_NODE_ITERATOR_GET_PRIVATE(request);
    new (priv) CyberKitDOMNodeIteratorPrivate();
}

CyberKitDOMNode* webkit_dom_node_iterator_next_node(CyberKitDOMNodeIterator* self, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE_ITERATOR(self), 0);
    UNUSED_PARAM(error);
    CyberCore::NodeIterator* item = CyberKit::core(self);

    auto result = item->nextNode();
    if (result.hasException())
        return nullptr;

    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(result.releaseReturnValue());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMNode* webkit_dom_node_iterator_previous_node(CyberKitDOMNodeIterator* self, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE_ITERATOR(self), 0);
    UNUSED_PARAM(error);
    CyberCore::NodeIterator* item = CyberKit::core(self);

    auto result = item->previousNode();
    if (result.hasException())
        return nullptr;

    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(result.releaseReturnValue());
    return CyberKit::kit(gobjectResult.get());
}

void webkit_dom_node_iterator_detach(CyberKitDOMNodeIterator* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_NODE_ITERATOR(self));
    CyberCore::NodeIterator* item = CyberKit::core(self);
    item->detach();
}

CyberKitDOMNode* webkit_dom_node_iterator_get_root(CyberKitDOMNodeIterator* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE_ITERATOR(self), 0);
    CyberCore::NodeIterator* item = CyberKit::core(self);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->root());
    return CyberKit::kit(gobjectResult.get());
}

gulong webkit_dom_node_iterator_get_what_to_show(CyberKitDOMNodeIterator* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE_ITERATOR(self), 0);
    CyberCore::NodeIterator* item = CyberKit::core(self);
    gulong result = item->whatToShow();
    return result;
}

CyberKitDOMNodeFilter* webkit_dom_node_iterator_get_filter(CyberKitDOMNodeIterator* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE_ITERATOR(self), 0);
    CyberCore::NodeIterator* item = CyberKit::core(self);
    RefPtr<CyberCore::NodeFilter> gobjectResult = WTF::getPtr(item->filter());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMNode* webkit_dom_node_iterator_get_reference_node(CyberKitDOMNodeIterator* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE_ITERATOR(self), 0);
    CyberCore::NodeIterator* item = CyberKit::core(self);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->referenceNode());
    return CyberKit::kit(gobjectResult.get());
}

gboolean webkit_dom_node_iterator_get_pointer_before_reference_node(CyberKitDOMNodeIterator* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE_ITERATOR(self), FALSE);
    CyberCore::NodeIterator* item = CyberKit::core(self);
    gboolean result = item->pointerBeforeReferenceNode();
    return result;
}

G_GNUC_END_IGNORE_DEPRECATIONS;
