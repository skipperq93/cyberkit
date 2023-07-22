/*
 * Copyright (C) 2017 Igalia S.L.
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
#include "CyberKitOptionMenuItem.h"

#include "CyberKitOptionMenuItemPrivate.h"

using namespace CyberKit;

/**
 * SECTION: CyberKitOptionMenuItem
 * @Short_description: One item of the #CyberKitOptionMenu
 * @Title: CyberKitOptionMenuItem
 *
 * The #CyberKitOptionMenu is composed of CyberKitOptionMenuItem<!-- -->s.
 * A CyberKitOptionMenuItem always has a label and can contain a tooltip text.
 * You can use the CyberKitOptionMenuItem of a #CyberKitOptionMenu to build your
 * own menus.
 *
 * Since: 2.18
 */

G_DEFINE_BOXED_TYPE(CyberKitOptionMenuItem, webkit_option_menu_item, webkit_option_menu_item_copy, webkit_option_menu_item_free)

/**
 * webkit_option_menu_item_copy:
 * @item: a #CyberKitOptionMenuItem
 *
 * Make a copy of the #CyberKitOptionMenuItem.
 *
 * Returns: (transfer full): A copy of passed in #CyberKitOptionMenuItem
 *
 * Since: 2.18
 */
CyberKitOptionMenuItem* webkit_option_menu_item_copy(CyberKitOptionMenuItem* item)
{
    g_return_val_if_fail(item, nullptr);

    auto* copyItem = static_cast<CyberKitOptionMenuItem*>(fastMalloc(sizeof(CyberKitOptionMenuItem)));
    new (copyItem) CyberKitOptionMenuItem(item);
    return copyItem;
}

/**
 * webkit_option_menu_item_free:
 * @item: A #CyberKitOptionMenuItem
 *
 * Free the #CyberKitOptionMenuItem.
 *
 * Since: 2.18
 */
void webkit_option_menu_item_free(CyberKitOptionMenuItem* item)
{
    g_return_if_fail(item);

    item->~CyberKitOptionMenuItem();
    fastFree(item);
}

/**
 * webkit_option_menu_item_get_label:
 * @item: a #CyberKitOptionMenuItem
 *
 * Get the label of a #CyberKitOptionMenuItem.
 *
 * Returns: The label of @item.
 *
 * Since: 2.18
 */
const gchar* webkit_option_menu_item_get_label(CyberKitOptionMenuItem* item)
{
    g_return_val_if_fail(item, nullptr);

    return item->label.data();
}

/**
 * webkit_option_menu_item_get_tooltip:
 * @item: a #CyberKitOptionMenuItem
 *
 * Get the tooltip of a #CyberKitOptionMenuItem.
 *
 * Returns: The tooltip of @item, or %NULL.
 *
 * Since: 2.18
 */
const gchar* webkit_option_menu_item_get_tooltip(CyberKitOptionMenuItem* item)
{
    g_return_val_if_fail(item, nullptr);

    return item->tooltip.isNull() ? nullptr : item->tooltip.data();
}

/**
 * webkit_option_menu_item_is_group_label:
 * @item: a #CyberKitOptionMenuItem
 *
 * Whether a #CyberKitOptionMenuItem is a group label.
 *
 * Returns: %TRUE if the @item is a group label or %FALSE otherwise.
 *
 * Since: 2.18
 */
gboolean webkit_option_menu_item_is_group_label(CyberKitOptionMenuItem* item)
{
    g_return_val_if_fail(item, FALSE);

    return item->isGroupLabel;
}

/**
 * webkit_option_menu_item_is_group_child:
 * @item: a #CyberKitOptionMenuItem
 *
 * Whether a #CyberKitOptionMenuItem is a group child.
 *
 * Returns: %TRUE if the @item is a group child or %FALSE otherwise.
 *
 * Since: 2.18
 */
gboolean webkit_option_menu_item_is_group_child(CyberKitOptionMenuItem* item)
{
    g_return_val_if_fail(item, FALSE);

    return item->isGroupChild;
}

/**
 * webkit_option_menu_item_is_enabled:
 * @item: a #CyberKitOptionMenuItem
 *
 * Whether a #CyberKitOptionMenuItem is enabled.
 *
 * Returns: %TRUE if the @item is enabled or %FALSE otherwise.
 *
 * Since: 2.18
 */
gboolean webkit_option_menu_item_is_enabled(CyberKitOptionMenuItem* item)
{
    g_return_val_if_fail(item, FALSE);

    return item->isEnabled;
}

/**
 * webkit_option_menu_item_is_selected:
 * @item: a #CyberKitOptionMenuItem
 *
 * Whether a #CyberKitOptionMenuItem is the currently selected one.
 *
 * Returns: %TRUE if the @item is selected or %FALSE otherwise.
 *
 * Since: 2.18
 */
gboolean webkit_option_menu_item_is_selected(CyberKitOptionMenuItem* item)
{
    g_return_val_if_fail(item, FALSE);

    return item->isSelected;
}
