/*
 * Copyright (C) 2012 Igalia S.L.
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
#include "CyberKitContextMenuItem.h"

#include "APIArray.h"
#include "WebContextMenuItem.h"
#include "WebContextMenuItemGlib.h"
#include "CyberKitContextMenuActionsPrivate.h"
#include "CyberKitContextMenuItemPrivate.h"
#include "CyberKitContextMenuPrivate.h"
#include <CyberCore/ContextMenu.h>
#include <CyberCore/ContextMenuItem.h>
#include <memory>
#include <wtf/glib/GRefPtr.h>
#include <wtf/glib/GUniquePtr.h>
#include <wtf/glib/WTFGType.h>

using namespace CyberKit;
using namespace CyberCore;

/**
 * CyberKitContextMenuItem:
 *
 * One item of a #CyberKitContextMenu.
 *
 * The #CyberKitContextMenu is composed of #CyberKitContextMenuItem<!--
 * -->s. These items can be created from a #GtkAction, from a
 * #CyberKitContextMenuAction or from a #CyberKitContextMenuAction and a
 * label. These #CyberKitContextMenuAction<!-- -->s denote stock actions
 * for the items. You can also create separators and submenus.
 *
 */

struct _CyberKitContextMenuItemPrivate {
    ~_CyberKitContextMenuItemPrivate()
    {
        if (subMenu)
            webkitContextMenuSetParentItem(subMenu.get(), 0);
    }

    std::unique_ptr<WebContextMenuItemGlib> menuItem;
    GRefPtr<CyberKitContextMenu> subMenu;
};

WEBKIT_DEFINE_FINAL_TYPE(CyberKitContextMenuItem, webkit_context_menu_item, G_TYPE_INITIALLY_UNOWNED, GInitiallyUnowned)

static void webkit_context_menu_item_class_init(CyberKitContextMenuItemClass*)
{
}

static bool checkAndWarnIfMenuHasParentItem(CyberKitContextMenu* menu)
{
    if (menu && webkitContextMenuGetParentItem(menu)) {
        g_warning("Attempting to set a CyberKitContextMenu as submenu of "
                  "a CyberKitContextMenuItem, but the menu is already "
                  "a submenu of a CyberKitContextMenuItem");
        return true;
    }

    return false;
}

static void webkitContextMenuItemSetSubMenu(CyberKitContextMenuItem* item, GRefPtr<CyberKitContextMenu> subMenu)
{
    if (checkAndWarnIfMenuHasParentItem(subMenu.get()))
        return;

    if (item->priv->subMenu)
        webkitContextMenuSetParentItem(item->priv->subMenu.get(), nullptr);
    item->priv->subMenu = subMenu;
    if (subMenu)
        webkitContextMenuSetParentItem(subMenu.get(), item);
}

CyberKitContextMenuItem* webkitContextMenuItemCreate(const WebContextMenuItemData& itemData)
{
    CyberKitContextMenuItem* item = WEBKIT_CONTEXT_MENU_ITEM(g_object_new(WEBKIT_TYPE_CONTEXT_MENU_ITEM, NULL));

    item->priv->menuItem = makeUnique<WebContextMenuItemGlib>(itemData);
    const Vector<WebContextMenuItemData>& subMenu = itemData.submenu();
    if (!subMenu.isEmpty())
        webkitContextMenuItemSetSubMenu(item, adoptGRef(webkitContextMenuCreate(subMenu)));

    return item;
}

WebContextMenuItemGlib webkitContextMenuItemToWebContextMenuItemGlib(CyberKitContextMenuItem* item)
{
    if (item->priv->subMenu) {
        Vector<WebContextMenuItemGlib> subMenuItems;
        webkitContextMenuPopulate(item->priv->subMenu.get(), subMenuItems);
        return WebContextMenuItemGlib(*item->priv->menuItem, WTFMove(subMenuItems));
    }

    return *item->priv->menuItem;
}

WebContextMenuItemData webkitContextMenuItemToWebContextMenuItemData(CyberKitContextMenuItem* item)
{
    if (item->priv->subMenu) {
        Vector<WebContextMenuItemData> subMenuItems;
        webkitContextMenuPopulate(item->priv->subMenu.get(), subMenuItems);
        return WebContextMenuItemData(item->priv->menuItem->action(), item->priv->menuItem->title(), item->priv->menuItem->enabled(), subMenuItems);
    }

    return WebContextMenuItemData(item->priv->menuItem->type(), item->priv->menuItem->action(), item->priv->menuItem->title(), item->priv->menuItem->enabled(), item->priv->menuItem->checked());
}

#if PLATFORM(GTK) && !USE(GTK4)
/**
 * webkit_context_menu_item_new:
 * @action: a #GtkAction
 *
 * Creates a new #CyberKitContextMenuItem for the given @action.
 *
 * Returns: the newly created #CyberKitContextMenuItem object.
 *
 * Deprecated: 2.18: Use webkit_context_menu_item_new_from_gaction() instead.
 */
CyberKitContextMenuItem* webkit_context_menu_item_new(GtkAction* action)
{
    g_return_val_if_fail(GTK_IS_ACTION(action), nullptr);

    CyberKitContextMenuItem* item = WEBKIT_CONTEXT_MENU_ITEM(g_object_new(WEBKIT_TYPE_CONTEXT_MENU_ITEM, nullptr));
    item->priv->menuItem = makeUnique<WebContextMenuItemGlib>(action);

    return item;
}
#endif

/**
 * webkit_context_menu_item_new_from_gaction:
 * @action: a #GAction
 * @label: the menu item label text
 * @target: (allow-none): a #GVariant to use as the action target
 *
 * Creates a new #CyberKitContextMenuItem for the given @action and @label.
 *
 * On activation
 * @target will be passed as parameter to the callback.
 *
 * Returns: the newly created #CyberKitContextMenuItem object.
 *
 * Since: 2.18
 */
CyberKitContextMenuItem* webkit_context_menu_item_new_from_gaction(GAction* action, const gchar* label, GVariant* target)
{
    g_return_val_if_fail(G_IS_ACTION(action), nullptr);
    g_return_val_if_fail(!g_action_get_state_type(action) || g_variant_type_equal(g_action_get_state_type(action), G_VARIANT_TYPE_BOOLEAN), nullptr);
    g_return_val_if_fail(label, nullptr);
    g_return_val_if_fail(!target || g_variant_is_of_type(target, g_action_get_parameter_type(action)), nullptr);

    CyberKitContextMenuItem* item = WEBKIT_CONTEXT_MENU_ITEM(g_object_new(WEBKIT_TYPE_CONTEXT_MENU_ITEM, nullptr));
    item->priv->menuItem = makeUnique<WebContextMenuItemGlib>(action, String::fromUTF8(label), target);

    return item;
}

/**
 * webkit_context_menu_item_new_from_stock_action:
 * @action: a #CyberKitContextMenuAction stock action
 *
 * Creates a new #CyberKitContextMenuItem for the given stock action.
 *
 * Stock actions are handled automatically by CyberKit so that, for example,
 * when a menu item created with a %WEBKIT_CONTEXT_MENU_ACTION_STOP is
 * activated the action associated will be handled by CyberKit and the current
 * load operation will be stopped. You can get the #GAction of a
 * #CyberKitContextMenuItem created with a #CyberKitContextMenuAction with
 * webkit_context_menu_item_get_gaction() and connect to the #GSimpleAction::activate signal
 * to be notified when the item is activated, but you can't prevent the associated
 * action from being performed.
 *
 * Returns: the newly created #CyberKitContextMenuItem object.
 */
CyberKitContextMenuItem* webkit_context_menu_item_new_from_stock_action(CyberKitContextMenuAction action)
{
    g_return_val_if_fail(action > WEBKIT_CONTEXT_MENU_ACTION_NO_ACTION && action < WEBKIT_CONTEXT_MENU_ACTION_CUSTOM, nullptr);

    CyberKitContextMenuItem* item = WEBKIT_CONTEXT_MENU_ITEM(g_object_new(WEBKIT_TYPE_CONTEXT_MENU_ITEM, nullptr));
    ContextMenuItemType type = webkitContextMenuActionIsCheckable(action) ? CheckableActionType : ActionType;
    item->priv->menuItem = makeUnique<WebContextMenuItemGlib>(type, webkitContextMenuActionGetActionTag(action), webkitContextMenuActionGetLabel(action));

    return item;
}

/**
 * webkit_context_menu_item_new_from_stock_action_with_label:
 * @action: a #CyberKitContextMenuAction stock action
 * @label: a custom label text to use instead of the predefined one
 *
 * Creates a new #CyberKitContextMenuItem for the given stock action using the given @label.
 *
 * Stock actions have a predefined label, this method can be used to create a
 * #CyberKitContextMenuItem for a #CyberKitContextMenuAction but using a custom label.
 *
 * Returns: the newly created #CyberKitContextMenuItem object.
 */
CyberKitContextMenuItem* webkit_context_menu_item_new_from_stock_action_with_label(CyberKitContextMenuAction action, const gchar* label)
{
    g_return_val_if_fail(action > WEBKIT_CONTEXT_MENU_ACTION_NO_ACTION && action < WEBKIT_CONTEXT_MENU_ACTION_CUSTOM, nullptr);

    CyberKitContextMenuItem* item = WEBKIT_CONTEXT_MENU_ITEM(g_object_new(WEBKIT_TYPE_CONTEXT_MENU_ITEM, nullptr));
    ContextMenuItemType type = webkitContextMenuActionIsCheckable(action) ? CheckableActionType : ActionType;
    item->priv->menuItem = makeUnique<WebContextMenuItemGlib>(type, webkitContextMenuActionGetActionTag(action), String::fromUTF8(label));

    return item;
}

/**
 * webkit_context_menu_item_new_with_submenu:
 * @label: the menu item label text
 * @submenu: a #CyberKitContextMenu to set
 *
 * Creates a new #CyberKitContextMenuItem using the given @label with a submenu.
 *
 * Returns: the newly created #CyberKitContextMenuItem object.
 */
CyberKitContextMenuItem* webkit_context_menu_item_new_with_submenu(const gchar* label, CyberKitContextMenu* submenu)
{
    g_return_val_if_fail(label, nullptr);
    g_return_val_if_fail(WEBKIT_IS_CONTEXT_MENU(submenu), nullptr);

    if (checkAndWarnIfMenuHasParentItem(submenu))
        return nullptr;

    CyberKitContextMenuItem* item = WEBKIT_CONTEXT_MENU_ITEM(g_object_new(WEBKIT_TYPE_CONTEXT_MENU_ITEM, nullptr));
    item->priv->menuItem = makeUnique<WebContextMenuItemGlib>(ActionType, ContextMenuItemBaseApplicationTag, String::fromUTF8(label));
    item->priv->subMenu = submenu;
    webkitContextMenuSetParentItem(submenu, item);

    return item;
}

/**
 * webkit_context_menu_item_new_separator:
 *
 * Creates a new #CyberKitContextMenuItem representing a separator.
 *
 * Returns: the newly created #CyberKitContextMenuItem object.
 */
CyberKitContextMenuItem* webkit_context_menu_item_new_separator(void)
{
    CyberKitContextMenuItem* item = WEBKIT_CONTEXT_MENU_ITEM(g_object_new(WEBKIT_TYPE_CONTEXT_MENU_ITEM, nullptr));
    item->priv->menuItem = makeUnique<WebContextMenuItemGlib>(SeparatorType, ContextMenuItemTagNoAction, String());

    return item;
}

#if PLATFORM(GTK) && !USE(GTK4)
/**
 * webkit_context_menu_item_get_action:
 * @item: a #CyberKitContextMenuItem
 *
 * Gets the action associated to @item as a #GtkAction.
 *
 * Returns: (transfer none): the #GtkAction associated to the #CyberKitContextMenuItem,
 *    or %NULL if @item is a separator.
 *
 * Deprecated: 2.18: Use webkit_context_menu_item_get_gaction() instead.
 */
GtkAction* webkit_context_menu_item_get_action(CyberKitContextMenuItem* item)
{
    g_return_val_if_fail(WEBKIT_IS_CONTEXT_MENU_ITEM(item), nullptr);

    return item->priv->menuItem->gtkAction();
}
#endif

/**
 * webkit_context_menu_item_get_gaction:
 * @item: a #CyberKitContextMenuItem
 *
 * Gets the action associated to @item as a #GAction.
 *
 * Returns: (transfer none): the #GAction associated to the #CyberKitContextMenuItem,
 *    or %NULL if @item is a separator.
 *
 * Since: 2.18
 */
GAction* webkit_context_menu_item_get_gaction(CyberKitContextMenuItem* item)
{
    g_return_val_if_fail(WEBKIT_IS_CONTEXT_MENU_ITEM(item), nullptr);

    return item->priv->menuItem->gAction();
}

/**
 * webkit_context_menu_item_get_stock_action:
 * @item: a #CyberKitContextMenuItem
 *
 * Gets the #CyberKitContextMenuAction of @item.
 *
 * If the #CyberKitContextMenuItem was not
 * created for a stock action %WEBKIT_CONTEXT_MENU_ACTION_CUSTOM will be
 * returned. If the #CyberKitContextMenuItem is a separator %WEBKIT_CONTEXT_MENU_ACTION_NO_ACTION
 * will be returned.
 *
 * Returns: the #CyberKitContextMenuAction of @item
 */
CyberKitContextMenuAction webkit_context_menu_item_get_stock_action(CyberKitContextMenuItem* item)
{
    g_return_val_if_fail(WEBKIT_IS_CONTEXT_MENU_ITEM(item), WEBKIT_CONTEXT_MENU_ACTION_NO_ACTION);

    return webkitContextMenuActionGetForContextMenuItem(*item->priv->menuItem);
}

/**
 * webkit_context_menu_item_is_separator:
 * @item: a #CyberKitContextMenuItem
 *
 * Checks whether @item is a separator.
 *
 * Returns: %TRUE is @item is a separator or %FALSE otherwise
 */
gboolean webkit_context_menu_item_is_separator(CyberKitContextMenuItem* item)
{
    g_return_val_if_fail(WEBKIT_IS_CONTEXT_MENU_ITEM(item), FALSE);

    return item->priv->menuItem->type() == SeparatorType;
}

/**
 * webkit_context_menu_item_set_submenu:
 * @item: a #CyberKitContextMenuItem
 * @submenu: (allow-none): a #CyberKitContextMenu
 *
 * Sets or replaces the @item submenu.
 *
 * If @submenu is %NULL the current
 * submenu of @item is removed.
 */
void webkit_context_menu_item_set_submenu(CyberKitContextMenuItem* item, CyberKitContextMenu* submenu)
{
    g_return_if_fail(WEBKIT_IS_CONTEXT_MENU_ITEM(item));

    if (item->priv->subMenu == submenu)
        return;

    webkitContextMenuItemSetSubMenu(item, submenu);
}

/**
 * webkit_context_menu_item_get_submenu:
 * @item: a #CyberKitContextMenuItem
 *
 * Gets the submenu of @item.
 *
 * Returns: (transfer none): the #CyberKitContextMenu representing the submenu of
 *    @item or %NULL if @item doesn't have a submenu.
 */
CyberKitContextMenu* webkit_context_menu_item_get_submenu(CyberKitContextMenuItem* item)
{
    g_return_val_if_fail(WEBKIT_IS_CONTEXT_MENU_ITEM(item), 0);

    return item->priv->subMenu.get();
}

