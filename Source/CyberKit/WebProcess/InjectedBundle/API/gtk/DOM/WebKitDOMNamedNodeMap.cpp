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
#include "CyberKitDOMNamedNodeMap.h"

#include "ConvertToUTF8String.h"
#include "DOMObjectCache.h"
#include "CyberKitDOMNamedNodeMapPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include <CyberCore/Attr.h>
#include <CyberCore/CSSImportRule.h>
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include <CyberCore/Element.h>
#include <CyberCore/JSExecState.h>
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_NAMED_NODE_MAP_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_DOM_TYPE_NAMED_NODE_MAP, CyberKitDOMNamedNodeMapPrivate)

typedef struct _CyberKitDOMNamedNodeMapPrivate {
    RefPtr<CyberCore::NamedNodeMap> coreObject;
} CyberKitDOMNamedNodeMapPrivate;

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMNamedNodeMap* kit(CyberCore::NamedNodeMap* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_NAMED_NODE_MAP(ret);

    return wrapNamedNodeMap(obj);
}

CyberCore::NamedNodeMap* core(CyberKitDOMNamedNodeMap* request)
{
    return request ? static_cast<CyberCore::NamedNodeMap*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMNamedNodeMap* wrapNamedNodeMap(CyberCore::NamedNodeMap* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_NAMED_NODE_MAP(g_object_new(WEBKIT_DOM_TYPE_NAMED_NODE_MAP, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

G_DEFINE_TYPE(CyberKitDOMNamedNodeMap, webkit_dom_named_node_map, WEBKIT_DOM_TYPE_OBJECT)

enum {
    DOM_NAMED_NODE_MAP_PROP_0,
    DOM_NAMED_NODE_MAP_PROP_LENGTH,
};

static void webkit_dom_named_node_map_finalize(GObject* object)
{
    CyberKitDOMNamedNodeMapPrivate* priv = WEBKIT_DOM_NAMED_NODE_MAP_GET_PRIVATE(object);

    CyberKit::DOMObjectCache::forget(priv->coreObject.get());

    priv->~CyberKitDOMNamedNodeMapPrivate();
    G_OBJECT_CLASS(webkit_dom_named_node_map_parent_class)->finalize(object);
}

static void webkit_dom_named_node_map_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMNamedNodeMap* self = WEBKIT_DOM_NAMED_NODE_MAP(object);

    switch (propertyId) {
    case DOM_NAMED_NODE_MAP_PROP_LENGTH:
        g_value_set_ulong(value, webkit_dom_named_node_map_get_length(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static GObject* webkit_dom_named_node_map_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_named_node_map_parent_class)->constructor(type, constructPropertiesCount, constructProperties);

    CyberKitDOMNamedNodeMapPrivate* priv = WEBKIT_DOM_NAMED_NODE_MAP_GET_PRIVATE(object);
    priv->coreObject = static_cast<CyberCore::NamedNodeMap*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    CyberKit::DOMObjectCache::put(priv->coreObject.get(), object);

    return object;
}

static void webkit_dom_named_node_map_class_init(CyberKitDOMNamedNodeMapClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(CyberKitDOMNamedNodeMapPrivate));
    gobjectClass->constructor = webkit_dom_named_node_map_constructor;
    gobjectClass->finalize = webkit_dom_named_node_map_finalize;
    gobjectClass->get_property = webkit_dom_named_node_map_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_NAMED_NODE_MAP_PROP_LENGTH,
        g_param_spec_ulong(
            "length",
            "NamedNodeMap:length",
            "read-only gulong NamedNodeMap:length",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READABLE));

}

static void webkit_dom_named_node_map_init(CyberKitDOMNamedNodeMap* request)
{
    CyberKitDOMNamedNodeMapPrivate* priv = WEBKIT_DOM_NAMED_NODE_MAP_GET_PRIVATE(request);
    new (priv) CyberKitDOMNamedNodeMapPrivate();
}

CyberKitDOMNode* webkit_dom_named_node_map_get_named_item(CyberKitDOMNamedNodeMap* self, const gchar* name)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NAMED_NODE_MAP(self), 0);
    g_return_val_if_fail(name, 0);
    CyberCore::NamedNodeMap* item = CyberKit::core(self);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->getNamedItem(WTF::AtomString::fromUTF8(name)));
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMNode* webkit_dom_named_node_map_set_named_item(CyberKitDOMNamedNodeMap* self, CyberKitDOMNode* node, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NAMED_NODE_MAP(self), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(node), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::NamedNodeMap* item = CyberKit::core(self);
    CyberCore::Node* convertedNode = CyberKit::core(node);
    if (!is<CyberCore::Attr>(*convertedNode)) {
        auto description = CyberCore::DOMException::description(CyberCore::TypeError);
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    auto result = item->setNamedItem(downcast<CyberCore::Attr>(*convertedNode));
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().get());
}

CyberKitDOMNode* webkit_dom_named_node_map_remove_named_item(CyberKitDOMNamedNodeMap* self, const gchar* name, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NAMED_NODE_MAP(self), 0);
    g_return_val_if_fail(name, 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::NamedNodeMap* item = CyberKit::core(self);
    auto result = item->removeNamedItem(WTF::AtomString::fromUTF8(name));
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

CyberKitDOMNode* webkit_dom_named_node_map_item(CyberKitDOMNamedNodeMap* self, gulong index)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NAMED_NODE_MAP(self), 0);
    CyberCore::NamedNodeMap* item = CyberKit::core(self);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->item(index));
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMNode* webkit_dom_named_node_map_get_named_item_ns(CyberKitDOMNamedNodeMap* self, const gchar* namespaceURI, const gchar* localName)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NAMED_NODE_MAP(self), 0);
    g_return_val_if_fail(namespaceURI, 0);
    g_return_val_if_fail(localName, 0);
    CyberCore::NamedNodeMap* item = CyberKit::core(self);
    auto convertedNamespaceURI = WTF::AtomString::fromUTF8(namespaceURI);
    auto convertedLocalName = WTF::AtomString::fromUTF8(localName);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->getNamedItemNS(convertedNamespaceURI, convertedLocalName));
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMNode* webkit_dom_named_node_map_set_named_item_ns(CyberKitDOMNamedNodeMap* self, CyberKitDOMNode* node, GError** error)
{
    return webkit_dom_named_node_map_set_named_item(self, node, error);
}

CyberKitDOMNode* webkit_dom_named_node_map_remove_named_item_ns(CyberKitDOMNamedNodeMap* self, const gchar* namespaceURI, const gchar* localName, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NAMED_NODE_MAP(self), 0);
    g_return_val_if_fail(namespaceURI, 0);
    g_return_val_if_fail(localName, 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::NamedNodeMap* item = CyberKit::core(self);
    auto convertedNamespaceURI = WTF::AtomString::fromUTF8(namespaceURI);
    auto convertedLocalName = WTF::AtomString::fromUTF8(localName);
    auto result = item->removeNamedItemNS(convertedNamespaceURI, convertedLocalName);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

gulong webkit_dom_named_node_map_get_length(CyberKitDOMNamedNodeMap* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NAMED_NODE_MAP(self), 0);
    CyberCore::NamedNodeMap* item = CyberKit::core(self);
    gulong result = item->length();
    return result;
}

G_GNUC_END_IGNORE_DEPRECATIONS;
