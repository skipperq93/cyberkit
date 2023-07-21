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
#include "CyberKitDOMNode.h"

#include "ConvertToUTF8String.h"
#include "GObjectEventListener.h"
#include "CyberKitDOMDocumentPrivate.h"
#include "CyberKitDOMElementPrivate.h"
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMNodeListPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include <CyberCore/CSSImportRule.h>
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include <CyberCore/JSExecState.h>
#include <CyberCore/JSNode.h>
#include <CyberCore/SVGTests.h>
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

static gboolean webkit_dom_node_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::Node* coreTarget = static_cast<CyberCore::Node*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_node_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::Node* coreTarget = static_cast<CyberCore::Node*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_node_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::Node* coreTarget = static_cast<CyberCore::Node*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

void webkitDOMNodeDOMEventTargetInit(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_node_dispatch_event;
    iface->add_event_listener = webkit_dom_node_add_event_listener;
    iface->remove_event_listener = webkit_dom_node_remove_event_listener;
}

enum {
    DOM_NODE_PROP_0,
    DOM_NODE_PROP_NODE_NAME,
    DOM_NODE_PROP_NODE_VALUE,
    DOM_NODE_PROP_NODE_TYPE,
    DOM_NODE_PROP_PARENT_NODE,
    DOM_NODE_PROP_CHILD_NODES,
    DOM_NODE_PROP_FIRST_CHILD,
    DOM_NODE_PROP_LAST_CHILD,
    DOM_NODE_PROP_PREVIOUS_SIBLING,
    DOM_NODE_PROP_NEXT_SIBLING,
    DOM_NODE_PROP_OWNER_DOCUMENT,
    DOM_NODE_PROP_BASE_URI,
    DOM_NODE_PROP_TEXT_CONTENT,
    DOM_NODE_PROP_PARENT_ELEMENT,
};

static void webkit_dom_node_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMNode* self = WEBKIT_DOM_NODE(object);

    switch (propertyId) {
    case DOM_NODE_PROP_NODE_VALUE:
        webkit_dom_node_set_node_value(self, g_value_get_string(value), nullptr);
        break;
    case DOM_NODE_PROP_TEXT_CONTENT:
        webkit_dom_node_set_text_content(self, g_value_get_string(value), nullptr);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_node_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMNode* self = WEBKIT_DOM_NODE(object);

    switch (propertyId) {
    case DOM_NODE_PROP_NODE_NAME:
        g_value_take_string(value, webkit_dom_node_get_node_name(self));
        break;
    case DOM_NODE_PROP_NODE_VALUE:
        g_value_take_string(value, webkit_dom_node_get_node_value(self));
        break;
    case DOM_NODE_PROP_NODE_TYPE:
        g_value_set_uint(value, webkit_dom_node_get_node_type(self));
        break;
    case DOM_NODE_PROP_PARENT_NODE:
        g_value_set_object(value, webkit_dom_node_get_parent_node(self));
        break;
    case DOM_NODE_PROP_CHILD_NODES:
        g_value_set_object(value, webkit_dom_node_get_child_nodes(self));
        break;
    case DOM_NODE_PROP_FIRST_CHILD:
        g_value_set_object(value, webkit_dom_node_get_first_child(self));
        break;
    case DOM_NODE_PROP_LAST_CHILD:
        g_value_set_object(value, webkit_dom_node_get_last_child(self));
        break;
    case DOM_NODE_PROP_PREVIOUS_SIBLING:
        g_value_set_object(value, webkit_dom_node_get_previous_sibling(self));
        break;
    case DOM_NODE_PROP_NEXT_SIBLING:
        g_value_set_object(value, webkit_dom_node_get_next_sibling(self));
        break;
    case DOM_NODE_PROP_OWNER_DOCUMENT:
        g_value_set_object(value, webkit_dom_node_get_owner_document(self));
        break;
    case DOM_NODE_PROP_BASE_URI:
        g_value_take_string(value, webkit_dom_node_get_base_uri(self));
        break;
    case DOM_NODE_PROP_TEXT_CONTENT:
        g_value_take_string(value, webkit_dom_node_get_text_content(self));
        break;
    case DOM_NODE_PROP_PARENT_ELEMENT:
        g_value_set_object(value, webkit_dom_node_get_parent_element(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

void webkitDOMNodeInstallProperties(GObjectClass* gobjectClass)
{
    gobjectClass->set_property = webkit_dom_node_set_property;
    gobjectClass->get_property = webkit_dom_node_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_PROP_NODE_NAME,
        g_param_spec_string(
            "node-name",
            "Node:node-name",
            "read-only gchar* Node:node-name",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_PROP_NODE_VALUE,
        g_param_spec_string(
            "node-value",
            "Node:node-value",
            "read-write gchar* Node:node-value",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_PROP_NODE_TYPE,
        g_param_spec_uint(
            "node-type",
            "Node:node-type",
            "read-only gushort Node:node-type",
            0, G_MAXUINT, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_PROP_PARENT_NODE,
        g_param_spec_object(
            "parent-node",
            "Node:parent-node",
            "read-only CyberKitDOMNode* Node:parent-node",
            WEBKIT_DOM_TYPE_NODE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_PROP_CHILD_NODES,
        g_param_spec_object(
            "child-nodes",
            "Node:child-nodes",
            "read-only CyberKitDOMNodeList* Node:child-nodes",
            WEBKIT_DOM_TYPE_NODE_LIST,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_PROP_FIRST_CHILD,
        g_param_spec_object(
            "first-child",
            "Node:first-child",
            "read-only CyberKitDOMNode* Node:first-child",
            WEBKIT_DOM_TYPE_NODE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_PROP_LAST_CHILD,
        g_param_spec_object(
            "last-child",
            "Node:last-child",
            "read-only CyberKitDOMNode* Node:last-child",
            WEBKIT_DOM_TYPE_NODE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_PROP_PREVIOUS_SIBLING,
        g_param_spec_object(
            "previous-sibling",
            "Node:previous-sibling",
            "read-only CyberKitDOMNode* Node:previous-sibling",
            WEBKIT_DOM_TYPE_NODE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_PROP_NEXT_SIBLING,
        g_param_spec_object(
            "next-sibling",
            "Node:next-sibling",
            "read-only CyberKitDOMNode* Node:next-sibling",
            WEBKIT_DOM_TYPE_NODE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_PROP_OWNER_DOCUMENT,
        g_param_spec_object(
            "owner-document",
            "Node:owner-document",
            "read-only CyberKitDOMDocument* Node:owner-document",
            WEBKIT_DOM_TYPE_DOCUMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_PROP_BASE_URI,
        g_param_spec_string(
            "base-uri",
            "Node:base-uri",
            "read-only gchar* Node:base-uri",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_PROP_TEXT_CONTENT,
        g_param_spec_string(
            "text-content",
            "Node:text-content",
            "read-write gchar* Node:text-content",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_NODE_PROP_PARENT_ELEMENT,
        g_param_spec_object(
            "parent-element",
            "Node:parent-element",
            "read-only CyberKitDOMElement* Node:parent-element",
            WEBKIT_DOM_TYPE_ELEMENT,
            WEBKIT_PARAM_READABLE));

}

CyberKitDOMNode* webkit_dom_node_insert_before(CyberKitDOMNode* self, CyberKitDOMNode* newChild, CyberKitDOMNode* refChild, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(newChild), 0);
    g_return_val_if_fail(!refChild || WEBKIT_DOM_IS_NODE(refChild), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Node* item = CyberKit::core(self);
    CyberCore::Node* convertedNewChild = CyberKit::core(newChild);
    CyberCore::Node* convertedRefChild = CyberKit::core(refChild);
    auto result = item->insertBefore(*convertedNewChild, convertedRefChild);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return newChild;
}

CyberKitDOMNode* webkit_dom_node_replace_child(CyberKitDOMNode* self, CyberKitDOMNode* newChild, CyberKitDOMNode* oldChild, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(newChild), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(oldChild), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Node* item = CyberKit::core(self);
    CyberCore::Node* convertedNewChild = CyberKit::core(newChild);
    CyberCore::Node* convertedOldChild = CyberKit::core(oldChild);
    auto result = item->replaceChild(*convertedNewChild, *convertedOldChild);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return oldChild;
}

CyberKitDOMNode* webkit_dom_node_remove_child(CyberKitDOMNode* self, CyberKitDOMNode* oldChild, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(oldChild), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Node* item = CyberKit::core(self);
    CyberCore::Node* convertedOldChild = CyberKit::core(oldChild);
    auto result = item->removeChild(*convertedOldChild);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return oldChild;
}

CyberKitDOMNode* webkit_dom_node_append_child(CyberKitDOMNode* self, CyberKitDOMNode* newChild, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(newChild), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Node* item = CyberKit::core(self);
    CyberCore::Node* convertedNewChild = CyberKit::core(newChild);
    auto result = item->appendChild(*convertedNewChild);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return newChild;
}

gboolean webkit_dom_node_has_child_nodes(CyberKitDOMNode* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), FALSE);
    CyberCore::Node* item = CyberKit::core(self);
    gboolean result = item->hasChildNodes();
    return result;
}

CyberKitDOMNode* webkit_dom_node_clone_node_with_error(CyberKitDOMNode* self, gboolean deep, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Node* item = CyberKit::core(self);
    auto result = item->cloneNodeForBindings(deep);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

void webkit_dom_node_normalize(CyberKitDOMNode* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_NODE(self));
    CyberCore::Node* item = CyberKit::core(self);
    item->normalize();
}

gboolean webkit_dom_node_is_supported(CyberKitDOMNode* self, const gchar* feature, const gchar* version)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), FALSE);
    g_return_val_if_fail(feature, FALSE);
    g_return_val_if_fail(version, FALSE);
    WTF::String convertedFeature = WTF::String::fromUTF8(feature);
    WTF::String convertedVersion = WTF::String::fromUTF8(version);
    return CyberCore::SVGTests::hasFeatureForLegacyBindings(convertedFeature, convertedVersion);
}

gboolean webkit_dom_node_is_same_node(CyberKitDOMNode* self, CyberKitDOMNode* other)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), FALSE);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(other), FALSE);
    CyberCore::Node* item = CyberKit::core(self);
    CyberCore::Node* convertedOther = CyberKit::core(other);
    gboolean result = item->isSameNode(convertedOther);
    return result;
}

gboolean webkit_dom_node_is_equal_node(CyberKitDOMNode* self, CyberKitDOMNode* other)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), FALSE);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(other), FALSE);
    CyberCore::Node* item = CyberKit::core(self);
    CyberCore::Node* convertedOther = CyberKit::core(other);
    gboolean result = item->isEqualNode(convertedOther);
    return result;
}

gchar* webkit_dom_node_lookup_prefix(CyberKitDOMNode* self, const gchar* namespaceURI)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    g_return_val_if_fail(namespaceURI, 0);
    CyberCore::Node* item = CyberKit::core(self);
    WTF::String convertedNamespaceURI = WTF::String::fromUTF8(namespaceURI);
    gchar* result = convertToUTF8String(item->lookupPrefix(convertedNamespaceURI));
    return result;
}

gchar* webkit_dom_node_lookup_namespace_uri(CyberKitDOMNode* self, const gchar* prefix)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    g_return_val_if_fail(prefix, 0);
    CyberCore::Node* item = CyberKit::core(self);
    WTF::String convertedPrefix = WTF::String::fromUTF8(prefix);
    gchar* result = convertToUTF8String(item->lookupNamespaceURI(convertedPrefix));
    return result;
}

gboolean webkit_dom_node_is_default_namespace(CyberKitDOMNode* self, const gchar* namespaceURI)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), FALSE);
    g_return_val_if_fail(namespaceURI, FALSE);
    CyberCore::Node* item = CyberKit::core(self);
    WTF::String convertedNamespaceURI = WTF::String::fromUTF8(namespaceURI);
    gboolean result = item->isDefaultNamespace(convertedNamespaceURI);
    return result;
}

gushort webkit_dom_node_compare_document_position(CyberKitDOMNode* self, CyberKitDOMNode* other)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(other), 0);
    CyberCore::Node* item = CyberKit::core(self);
    CyberCore::Node* convertedOther = CyberKit::core(other);
    gushort result = item->compareDocumentPosition(*convertedOther);
    return result;
}

gboolean webkit_dom_node_contains(CyberKitDOMNode* self, CyberKitDOMNode* other)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), FALSE);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(other), FALSE);
    CyberCore::Node* item = CyberKit::core(self);
    CyberCore::Node* convertedOther = CyberKit::core(other);
    gboolean result = item->contains(convertedOther);
    return result;
}

gchar* webkit_dom_node_get_node_name(CyberKitDOMNode* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    CyberCore::Node* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->nodeName());
    return result;
}

gchar* webkit_dom_node_get_node_value(CyberKitDOMNode* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    CyberCore::Node* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->nodeValue());
    return result;
}

void webkit_dom_node_set_node_value(CyberKitDOMNode* self, const gchar* value, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_NODE(self));
    g_return_if_fail(value);
    g_return_if_fail(!error || !*error);
    CyberCore::Node* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    auto result = item->setNodeValue(convertedValue);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

gushort webkit_dom_node_get_node_type(CyberKitDOMNode* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    CyberCore::Node* item = CyberKit::core(self);
    gushort result = item->nodeType();
    return result;
}

CyberKitDOMNode* webkit_dom_node_get_parent_node(CyberKitDOMNode* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    CyberCore::Node* item = CyberKit::core(self);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->parentNode());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMNodeList* webkit_dom_node_get_child_nodes(CyberKitDOMNode* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    CyberCore::Node* item = CyberKit::core(self);
    RefPtr<CyberCore::NodeList> gobjectResult = WTF::getPtr(item->childNodes());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMNode* webkit_dom_node_get_first_child(CyberKitDOMNode* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    CyberCore::Node* item = CyberKit::core(self);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->firstChild());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMNode* webkit_dom_node_get_last_child(CyberKitDOMNode* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    CyberCore::Node* item = CyberKit::core(self);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->lastChild());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMNode* webkit_dom_node_get_previous_sibling(CyberKitDOMNode* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    CyberCore::Node* item = CyberKit::core(self);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->previousSibling());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMNode* webkit_dom_node_get_next_sibling(CyberKitDOMNode* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    CyberCore::Node* item = CyberKit::core(self);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->nextSibling());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMDocument* webkit_dom_node_get_owner_document(CyberKitDOMNode* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    CyberCore::Node* item = CyberKit::core(self);
    RefPtr<CyberCore::Document> gobjectResult = WTF::getPtr(item->ownerDocument());
    return CyberKit::kit(gobjectResult.get());
}

gchar* webkit_dom_node_get_base_uri(CyberKitDOMNode* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    CyberCore::Node* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->baseURI());
    return result;
}

gchar* webkit_dom_node_get_text_content(CyberKitDOMNode* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    CyberCore::Node* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->textContent());
    return result;
}

void webkit_dom_node_set_text_content(CyberKitDOMNode* self, const gchar* value, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_NODE(self));
    g_return_if_fail(value);
    g_return_if_fail(!error || !*error);
    CyberCore::Node* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    auto result = item->setTextContent(convertedValue);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

CyberKitDOMElement* webkit_dom_node_get_parent_element(CyberKitDOMNode* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), 0);
    CyberCore::Node* item = CyberKit::core(self);
    RefPtr<CyberCore::Element> gobjectResult = WTF::getPtr(item->parentElement());
    return CyberKit::kit(gobjectResult.get());
}

G_GNUC_END_IGNORE_DEPRECATIONS;
