/*
 * Copyright (C) 2014 Igalia S.L.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "CyberKitDOMNodeFilter.h"

#include "GObjectNodeFilterCondition.h"
#include <CyberCore/Document.h>
#include <CyberCore/NativeNodeFilter.h>
#include "CyberKitDOMNode.h"
#include "CyberKitDOMNodeFilterPrivate.h"
#include <wtf/HashMap.h>
#include <wtf/NeverDestroyed.h>

typedef CyberKitDOMNodeFilterIface CyberKitDOMNodeFilterInterface;

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

G_DEFINE_INTERFACE(CyberKitDOMNodeFilter, webkit_dom_node_filter, G_TYPE_OBJECT)

static void webkit_dom_node_filter_default_init(CyberKitDOMNodeFilterIface*)
{
}

gshort webkit_dom_node_filter_accept_node(CyberKitDOMNodeFilter* filter, CyberKitDOMNode* node)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE_FILTER(filter), WEBKIT_DOM_NODE_FILTER_REJECT);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(node), WEBKIT_DOM_NODE_FILTER_REJECT);

    return WEBKIT_DOM_NODE_FILTER_GET_IFACE(filter)->accept_node(filter, node);
}

namespace CyberKit {

static HashMap<CyberCore::NodeFilter*, CyberKitDOMNodeFilter*>& nodeFilterMap()
{
    static NeverDestroyed<HashMap<CyberCore::NodeFilter*, CyberKitDOMNodeFilter*>> nodeFilterMap;
    return nodeFilterMap;
}

static void nodeFilterObjectDestroyedCallback(gpointer coreNodeFilter, GObject* nodeFilter)
{
    CyberKitDOMNodeFilter* filter = nodeFilterMap().take(static_cast<CyberCore::NodeFilter*>(coreNodeFilter));
    UNUSED_PARAM(nodeFilter);
    ASSERT_UNUSED(filter, reinterpret_cast<GObject*>(filter) == nodeFilter);
}

CyberKitDOMNodeFilter* kit(CyberCore::NodeFilter* coreNodeFilter)
{
    if (!coreNodeFilter)
        return nullptr;

    return nodeFilterMap().get(coreNodeFilter);
}

RefPtr<CyberCore::NodeFilter> core(CyberCore::Document* document, CyberKitDOMNodeFilter* nodeFilter)
{
    if (!nodeFilter)
        return nullptr;

    RefPtr<CyberCore::NodeFilter> coreNodeFilter = static_cast<CyberCore::NodeFilter*>(g_object_get_data(G_OBJECT(nodeFilter), "webkit-core-node-filter"));
    if (!coreNodeFilter) {
        coreNodeFilter = CyberCore::NativeNodeFilter::create(document, CyberKit::GObjectNodeFilterCondition::create(nodeFilter));
        nodeFilterMap().add(coreNodeFilter.get(), nodeFilter);
        g_object_weak_ref(G_OBJECT(nodeFilter), nodeFilterObjectDestroyedCallback, coreNodeFilter.get());
        g_object_set_data(G_OBJECT(nodeFilter), "webkit-core-node-filter", coreNodeFilter.get());
    }
    return coreNodeFilter;
}

} // namespace CyberKit
G_GNUC_END_IGNORE_DEPRECATIONS;
