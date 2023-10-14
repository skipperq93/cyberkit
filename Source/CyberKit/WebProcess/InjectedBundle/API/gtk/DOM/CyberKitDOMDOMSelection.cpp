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
#include "CyberKitDOMDOMSelection.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMDOMSelectionPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "CyberKitDOMRangePrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_DOM_SELECTION_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_DOM_TYPE_DOM_SELECTION, CyberKitDOMDOMSelectionPrivate)

typedef struct _CyberKitDOMDOMSelectionPrivate {
    RefPtr<CyberCore::DOMSelection> coreObject;
} CyberKitDOMDOMSelectionPrivate;

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMDOMSelection* kit(CyberCore::DOMSelection* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_DOM_SELECTION(ret);

    return wrapDOMSelection(obj);
}

CyberCore::DOMSelection* core(CyberKitDOMDOMSelection* request)
{
    return request ? static_cast<CyberCore::DOMSelection*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMDOMSelection* wrapDOMSelection(CyberCore::DOMSelection* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_DOM_SELECTION(g_object_new(WEBKIT_DOM_TYPE_DOM_SELECTION, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

G_DEFINE_TYPE(CyberKitDOMDOMSelection, webkit_dom_dom_selection, WEBKIT_DOM_TYPE_OBJECT)

enum {
    DOM_DOM_SELECTION_PROP_0,
    DOM_DOM_SELECTION_PROP_ANCHOR_NODE,
    DOM_DOM_SELECTION_PROP_ANCHOR_OFFSET,
    DOM_DOM_SELECTION_PROP_FOCUS_NODE,
    DOM_DOM_SELECTION_PROP_FOCUS_OFFSET,
    DOM_DOM_SELECTION_PROP_IS_COLLAPSED,
    DOM_DOM_SELECTION_PROP_RANGE_COUNT,
    DOM_DOM_SELECTION_PROP_TYPE,
    DOM_DOM_SELECTION_PROP_BASE_NODE,
    DOM_DOM_SELECTION_PROP_BASE_OFFSET,
    DOM_DOM_SELECTION_PROP_EXTENT_NODE,
    DOM_DOM_SELECTION_PROP_EXTENT_OFFSET,
};

static void webkit_dom_dom_selection_finalize(GObject* object)
{
    CyberKitDOMDOMSelectionPrivate* priv = WEBKIT_DOM_DOM_SELECTION_GET_PRIVATE(object);

    CyberKit::DOMObjectCache::forget(priv->coreObject.get());

    priv->~CyberKitDOMDOMSelectionPrivate();
    G_OBJECT_CLASS(webkit_dom_dom_selection_parent_class)->finalize(object);
}

static void webkit_dom_dom_selection_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMDOMSelection* self = WEBKIT_DOM_DOM_SELECTION(object);

    switch (propertyId) {
    case DOM_DOM_SELECTION_PROP_ANCHOR_NODE:
        g_value_set_object(value, webkit_dom_dom_selection_get_anchor_node(self));
        break;
    case DOM_DOM_SELECTION_PROP_ANCHOR_OFFSET:
        g_value_set_ulong(value, webkit_dom_dom_selection_get_anchor_offset(self));
        break;
    case DOM_DOM_SELECTION_PROP_FOCUS_NODE:
        g_value_set_object(value, webkit_dom_dom_selection_get_focus_node(self));
        break;
    case DOM_DOM_SELECTION_PROP_FOCUS_OFFSET:
        g_value_set_ulong(value, webkit_dom_dom_selection_get_focus_offset(self));
        break;
    case DOM_DOM_SELECTION_PROP_IS_COLLAPSED:
        g_value_set_boolean(value, webkit_dom_dom_selection_get_is_collapsed(self));
        break;
    case DOM_DOM_SELECTION_PROP_RANGE_COUNT:
        g_value_set_ulong(value, webkit_dom_dom_selection_get_range_count(self));
        break;
    case DOM_DOM_SELECTION_PROP_TYPE:
        g_value_take_string(value, webkit_dom_dom_selection_get_selection_type(self));
        break;
    case DOM_DOM_SELECTION_PROP_BASE_NODE:
        g_value_set_object(value, webkit_dom_dom_selection_get_base_node(self));
        break;
    case DOM_DOM_SELECTION_PROP_BASE_OFFSET:
        g_value_set_ulong(value, webkit_dom_dom_selection_get_base_offset(self));
        break;
    case DOM_DOM_SELECTION_PROP_EXTENT_NODE:
        g_value_set_object(value, webkit_dom_dom_selection_get_extent_node(self));
        break;
    case DOM_DOM_SELECTION_PROP_EXTENT_OFFSET:
        g_value_set_ulong(value, webkit_dom_dom_selection_get_extent_offset(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static GObject* webkit_dom_dom_selection_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_dom_selection_parent_class)->constructor(type, constructPropertiesCount, constructProperties);

    CyberKitDOMDOMSelectionPrivate* priv = WEBKIT_DOM_DOM_SELECTION_GET_PRIVATE(object);
    priv->coreObject = static_cast<CyberCore::DOMSelection*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    CyberKit::DOMObjectCache::put(priv->coreObject.get(), object);

    return object;
}

static void webkit_dom_dom_selection_class_init(CyberKitDOMDOMSelectionClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(CyberKitDOMDOMSelectionPrivate));
    gobjectClass->constructor = webkit_dom_dom_selection_constructor;
    gobjectClass->finalize = webkit_dom_dom_selection_finalize;
    gobjectClass->get_property = webkit_dom_dom_selection_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_DOM_SELECTION_PROP_ANCHOR_NODE,
        g_param_spec_object(
            "anchor-node",
            "DOMSelection:anchor-node",
            "read-only CyberKitDOMNode* DOMSelection:anchor-node",
            WEBKIT_DOM_TYPE_NODE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOM_SELECTION_PROP_ANCHOR_OFFSET,
        g_param_spec_ulong(
            "anchor-offset",
            "DOMSelection:anchor-offset",
            "read-only gulong DOMSelection:anchor-offset",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOM_SELECTION_PROP_FOCUS_NODE,
        g_param_spec_object(
            "focus-node",
            "DOMSelection:focus-node",
            "read-only CyberKitDOMNode* DOMSelection:focus-node",
            WEBKIT_DOM_TYPE_NODE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOM_SELECTION_PROP_FOCUS_OFFSET,
        g_param_spec_ulong(
            "focus-offset",
            "DOMSelection:focus-offset",
            "read-only gulong DOMSelection:focus-offset",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOM_SELECTION_PROP_IS_COLLAPSED,
        g_param_spec_boolean(
            "is-collapsed",
            "DOMSelection:is-collapsed",
            "read-only gboolean DOMSelection:is-collapsed",
            FALSE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOM_SELECTION_PROP_RANGE_COUNT,
        g_param_spec_ulong(
            "range-count",
            "DOMSelection:range-count",
            "read-only gulong DOMSelection:range-count",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOM_SELECTION_PROP_TYPE,
        g_param_spec_string(
            "type",
            "DOMSelection:type",
            "read-only gchar* DOMSelection:type",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOM_SELECTION_PROP_BASE_NODE,
        g_param_spec_object(
            "base-node",
            "DOMSelection:base-node",
            "read-only CyberKitDOMNode* DOMSelection:base-node",
            WEBKIT_DOM_TYPE_NODE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOM_SELECTION_PROP_BASE_OFFSET,
        g_param_spec_ulong(
            "base-offset",
            "DOMSelection:base-offset",
            "read-only gulong DOMSelection:base-offset",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOM_SELECTION_PROP_EXTENT_NODE,
        g_param_spec_object(
            "extent-node",
            "DOMSelection:extent-node",
            "read-only CyberKitDOMNode* DOMSelection:extent-node",
            WEBKIT_DOM_TYPE_NODE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOM_SELECTION_PROP_EXTENT_OFFSET,
        g_param_spec_ulong(
            "extent-offset",
            "DOMSelection:extent-offset",
            "read-only gulong DOMSelection:extent-offset",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READABLE));

}

static void webkit_dom_dom_selection_init(CyberKitDOMDOMSelection* request)
{
    CyberKitDOMDOMSelectionPrivate* priv = WEBKIT_DOM_DOM_SELECTION_GET_PRIVATE(request);
    new (priv) CyberKitDOMDOMSelectionPrivate();
}

void webkit_dom_dom_selection_collapse(CyberKitDOMDOMSelection* self, CyberKitDOMNode* node, gulong offset)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self));
    g_return_if_fail(WEBKIT_DOM_IS_NODE(node));
    CyberCore::DOMSelection* item = CyberKit::core(self);
    CyberCore::Node* convertedNode = CyberKit::core(node);
    item->collapse(convertedNode, offset);
}

void webkit_dom_dom_selection_collapse_to_end(CyberKitDOMDOMSelection* self, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self));
    g_return_if_fail(!error || !*error);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    auto result = item->collapseToEnd();
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

void webkit_dom_dom_selection_collapse_to_start(CyberKitDOMDOMSelection* self, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self));
    g_return_if_fail(!error || !*error);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    auto result = item->collapseToStart();
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

void webkit_dom_dom_selection_delete_from_document(CyberKitDOMDOMSelection* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self));
    CyberCore::DOMSelection* item = CyberKit::core(self);
    item->deleteFromDocument();
}

gboolean webkit_dom_dom_selection_contains_node(CyberKitDOMDOMSelection* self, CyberKitDOMNode* node, gboolean allowPartial)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self), FALSE);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(node), FALSE);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    CyberCore::Node* convertedNode = CyberKit::core(node);
    gboolean result = item->containsNode(*convertedNode, allowPartial);
    return result;
}

void webkit_dom_dom_selection_select_all_children(CyberKitDOMDOMSelection* self, CyberKitDOMNode* node)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self));
    g_return_if_fail(WEBKIT_DOM_IS_NODE(node));
    CyberCore::DOMSelection* item = CyberKit::core(self);
    CyberCore::Node* convertedNode = CyberKit::core(node);
    item->selectAllChildren(*convertedNode);
}

void webkit_dom_dom_selection_extend(CyberKitDOMDOMSelection* self, CyberKitDOMNode* node, gulong offset, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self));
    g_return_if_fail(WEBKIT_DOM_IS_NODE(node));
    g_return_if_fail(!error || !*error);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    CyberCore::Node* convertedNode = CyberKit::core(node);
    auto result = item->extend(*convertedNode, offset);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

CyberKitDOMRange* webkit_dom_dom_selection_get_range_at(CyberKitDOMDOMSelection* self, gulong index, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    auto result = item->getRangeAt(index);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

void webkit_dom_dom_selection_remove_all_ranges(CyberKitDOMDOMSelection* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self));
    CyberCore::DOMSelection* item = CyberKit::core(self);
    item->removeAllRanges();
}

void webkit_dom_dom_selection_add_range(CyberKitDOMDOMSelection* self, CyberKitDOMRange* range)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self));
    g_return_if_fail(WEBKIT_DOM_IS_RANGE(range));
    CyberCore::DOMSelection* item = CyberKit::core(self);
    CyberCore::Range* convertedRange = CyberKit::core(range);
    item->addRange(*convertedRange);
}

void webkit_dom_dom_selection_set_base_and_extent(CyberKitDOMDOMSelection* self, CyberKitDOMNode* baseNode, gulong baseOffset, CyberKitDOMNode* extentNode, gulong extentOffset)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self));
    g_return_if_fail(WEBKIT_DOM_IS_NODE(baseNode));
    g_return_if_fail(WEBKIT_DOM_IS_NODE(extentNode));
    CyberCore::DOMSelection* item = CyberKit::core(self);
    CyberCore::Node* convertedBaseNode = CyberKit::core(baseNode);
    CyberCore::Node* convertedExtentNode = CyberKit::core(extentNode);
    item->setBaseAndExtent(convertedBaseNode, baseOffset, convertedExtentNode, extentOffset);
}

void webkit_dom_dom_selection_set_position(CyberKitDOMDOMSelection* self, CyberKitDOMNode* node, gulong offset)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self));
    g_return_if_fail(WEBKIT_DOM_IS_NODE(node));
    CyberCore::DOMSelection* item = CyberKit::core(self);
    CyberCore::Node* convertedNode = CyberKit::core(node);
    item->setPosition(convertedNode, offset);
}

void webkit_dom_dom_selection_empty(CyberKitDOMDOMSelection* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self));
    CyberCore::DOMSelection* item = CyberKit::core(self);
    item->empty();
}

void webkit_dom_dom_selection_modify(CyberKitDOMDOMSelection* self, const gchar* alter, const gchar* direction, const gchar* granularity)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self));
    g_return_if_fail(alter);
    g_return_if_fail(direction);
    g_return_if_fail(granularity);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    WTF::String convertedAlter = WTF::String::fromUTF8(alter);
    WTF::String convertedDirection = WTF::String::fromUTF8(direction);
    WTF::String convertedGranularity = WTF::String::fromUTF8(granularity);
    item->modify(convertedAlter, convertedDirection, convertedGranularity);
}

CyberKitDOMNode* webkit_dom_dom_selection_get_anchor_node(CyberKitDOMDOMSelection* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self), 0);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->anchorNode());
    return CyberKit::kit(gobjectResult.get());
}

gulong webkit_dom_dom_selection_get_anchor_offset(CyberKitDOMDOMSelection* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self), 0);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    gulong result = item->anchorOffset();
    return result;
}

CyberKitDOMNode* webkit_dom_dom_selection_get_focus_node(CyberKitDOMDOMSelection* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self), 0);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->focusNode());
    return CyberKit::kit(gobjectResult.get());
}

gulong webkit_dom_dom_selection_get_focus_offset(CyberKitDOMDOMSelection* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self), 0);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    gulong result = item->focusOffset();
    return result;
}

gboolean webkit_dom_dom_selection_get_is_collapsed(CyberKitDOMDOMSelection* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self), FALSE);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    gboolean result = item->isCollapsed();
    return result;
}

gulong webkit_dom_dom_selection_get_range_count(CyberKitDOMDOMSelection* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self), 0);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    gulong result = item->rangeCount();
    return result;
}

gchar* webkit_dom_dom_selection_get_selection_type(CyberKitDOMDOMSelection* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self), 0);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->type());
    return result;
}

CyberKitDOMNode* webkit_dom_dom_selection_get_base_node(CyberKitDOMDOMSelection* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self), 0);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->baseNode());
    return CyberKit::kit(gobjectResult.get());
}

gulong webkit_dom_dom_selection_get_base_offset(CyberKitDOMDOMSelection* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self), 0);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    gulong result = item->baseOffset();
    return result;
}

CyberKitDOMNode* webkit_dom_dom_selection_get_extent_node(CyberKitDOMDOMSelection* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self), 0);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->extentNode());
    return CyberKit::kit(gobjectResult.get());
}

gulong webkit_dom_dom_selection_get_extent_offset(CyberKitDOMDOMSelection* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOM_SELECTION(self), 0);
    CyberCore::DOMSelection* item = CyberKit::core(self);
    gulong result = item->extentOffset();
    return result;
}

G_GNUC_END_IGNORE_DEPRECATIONS;
