/*
 * Copyright (C) 2011 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "CyberKitBackForwardListItem.h"

#include "CyberKitBackForwardListPrivate.h"
#include <wtf/HashMap.h>
#include <wtf/NeverDestroyed.h>
#include <wtf/glib/GRefPtr.h>
#include <wtf/glib/WTFGType.h>
#include <wtf/text/CString.h>

using namespace CyberKit;

/**
 * CyberKitBackForwardListItem:
 * @See_also: #CyberKitBackForwardList
 *
 * One item of the #CyberKitBackForwardList.
 *
 * A history item is part of the #CyberKitBackForwardList and consists
 * out of a title and a URI.
 */

struct _CyberKitBackForwardListItemPrivate {
    RefPtr<WebBackForwardListItem> webListItem;
    CString uri;
    CString title;
    CString originalURI;
};

WEBKIT_DEFINE_FINAL_TYPE(CyberKitBackForwardListItem, webkit_back_forward_list_item, G_TYPE_INITIALLY_UNOWNED, GInitiallyUnowned)

static void webkit_back_forward_list_item_class_init(CyberKitBackForwardListItemClass*)
{
}

typedef HashMap<WebBackForwardListItem*, CyberKitBackForwardListItem*> HistoryItemsMap;

static HistoryItemsMap& historyItemsMap()
{
    static NeverDestroyed<HistoryItemsMap> itemsMap;
    return itemsMap;
}

static void webkitBackForwardListItemFinalized(gpointer webListItem, GObject* finalizedListItem)
{
    ASSERT_UNUSED(finalizedListItem, G_OBJECT(historyItemsMap().get(static_cast<WebBackForwardListItem*>(webListItem))) == finalizedListItem);
    historyItemsMap().remove(static_cast<WebBackForwardListItem*>(webListItem));
}

CyberKitBackForwardListItem* webkitBackForwardListItemGetOrCreate(WebBackForwardListItem* webListItem)
{
    if (!webListItem)
        return 0;

    CyberKitBackForwardListItem* listItem = historyItemsMap().get(webListItem);
    if (listItem)
        return listItem;

    listItem = WEBKIT_BACK_FORWARD_LIST_ITEM(g_object_new(WEBKIT_TYPE_BACK_FORWARD_LIST_ITEM, NULL));
    listItem->priv->webListItem = webListItem;

    g_object_weak_ref(G_OBJECT(listItem), webkitBackForwardListItemFinalized, webListItem);
    historyItemsMap().set(webListItem, listItem);

    return listItem;
}

WebBackForwardListItem* webkitBackForwardListItemGetItem(CyberKitBackForwardListItem* listItem)
{
    return listItem->priv->webListItem.get();
}

/**
 * webkit_back_forward_list_item_get_uri:
 * @list_item: a #CyberKitBackForwardListItem
 *
 * Obtain the URI of the item.
 *
 * This URI may differ from the original URI if the page was,
 * for example, redirected to a new location.
 * See also webkit_back_forward_list_item_get_original_uri().
 *
 * Returns: the URI of @list_item or %NULL
 *    when the URI is empty.
 */
const gchar* webkit_back_forward_list_item_get_uri(CyberKitBackForwardListItem* listItem)
{
    g_return_val_if_fail(WEBKIT_IS_BACK_FORWARD_LIST_ITEM(listItem), 0);

    CyberKitBackForwardListItemPrivate* priv = listItem->priv;
    String url = priv->webListItem->url();
    if (url.isEmpty())
        return 0;

    priv->uri = url.utf8();
    return priv->uri.data();
}

/**
 * webkit_back_forward_list_item_get_title:
 * @list_item: a #CyberKitBackForwardListItem
 *
 * Obtain the title of the item.
 *
 * Returns: the page title of @list_item or %NULL
 *    when the title is empty.
 */
const gchar* webkit_back_forward_list_item_get_title(CyberKitBackForwardListItem* listItem)
{
    g_return_val_if_fail(WEBKIT_IS_BACK_FORWARD_LIST_ITEM(listItem), 0);

    CyberKitBackForwardListItemPrivate* priv = listItem->priv;
    String title = priv->webListItem->title();
    if (title.isEmpty())
        return 0;

    priv->title = title.utf8();
    return priv->title.data();
}

/**
 * webkit_back_forward_list_item_get_original_uri:
 * @list_item: a #CyberKitBackForwardListItem
 *
 * Obtain the original URI of the item.
 *
 * See also webkit_back_forward_list_item_get_uri().
 *
 * Returns: the original URI of @list_item or %NULL
 *    when the original URI is empty.
 */
const gchar* webkit_back_forward_list_item_get_original_uri(CyberKitBackForwardListItem* listItem)
{
    g_return_val_if_fail(WEBKIT_IS_BACK_FORWARD_LIST_ITEM(listItem), 0);

    CyberKitBackForwardListItemPrivate* priv = listItem->priv;
    String originalURL = priv->webListItem->originalURL();
    if (originalURL.isEmpty())
        return 0;

    priv->originalURI = originalURL.utf8();
    return priv->originalURI.data();
}
