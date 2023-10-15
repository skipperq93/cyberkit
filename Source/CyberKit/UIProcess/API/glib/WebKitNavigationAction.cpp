/*
 * Copyright (C) 2014 Igalia S.L.
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
#include "CyberKitNavigationAction.h"

#include "CyberKitNavigationActionPrivate.h"
#include "CyberKitPrivate.h"
#include "CyberKitURIRequestPrivate.h"

using namespace CyberKit;

/**
 * CyberKitNavigationAction:
 *
 * Provides details about interaction resulting in a resource load.
 */

G_DEFINE_BOXED_TYPE(CyberKitNavigationAction, webkit_navigation_action, webkit_navigation_action_copy, webkit_navigation_action_free)

CyberKitNavigationAction* webkitNavigationActionCreate(Ref<API::NavigationAction>&& action)
{
    CyberKitNavigationAction* navigation = static_cast<CyberKitNavigationAction*>(fastZeroedMalloc(sizeof(CyberKitNavigationAction)));
    new (navigation) CyberKitNavigationAction(WTFMove(action));
    return navigation;
}

/**
 * webkit_navigation_action_copy:
 * @navigation: a #CyberKitNavigationAction
 *
 * Make a copy of @navigation.
 *
 * Returns: (transfer full): A copy of passed in #CyberKitNavigationAction
 *
 * Since: 2.6
 */
CyberKitNavigationAction* webkit_navigation_action_copy(CyberKitNavigationAction* navigation)
{
    g_return_val_if_fail(navigation, nullptr);

    CyberKitNavigationAction* copy = static_cast<CyberKitNavigationAction*>(fastZeroedMalloc(sizeof(CyberKitNavigationAction)));
    new (copy) CyberKitNavigationAction(navigation);
    return copy;
}

/**
 * webkit_navigation_action_free:
 * @navigation: a #CyberKitNavigationAction
 *
 * Free the #CyberKitNavigationAction
 *
 * Since: 2.6
 */
void webkit_navigation_action_free(CyberKitNavigationAction* navigation)
{
    g_return_if_fail(navigation);

    navigation->~CyberKitNavigationAction();
    fastFree(navigation);
}

/**
 * webkit_navigation_action_get_navigation_type:
 * @navigation: a #CyberKitNavigationAction
 *
 * Return the type of action that triggered the navigation.
 *
 * Returns: a #CyberKitNavigationType
 *
 * Since: 2.6
 */
CyberKitNavigationType webkit_navigation_action_get_navigation_type(CyberKitNavigationAction* navigation)
{
    g_return_val_if_fail(navigation, WEBKIT_NAVIGATION_TYPE_OTHER);
    return toCyberKitNavigationType(navigation->action->navigationType());
}

/**
 * webkit_navigation_action_get_mouse_button:
 * @navigation: a #CyberKitNavigationAction
 *
 * Return the number of the mouse button that triggered the navigation.
 *
 * Return the number of the mouse button that triggered the navigation, or 0 if
 * the navigation was not started by a mouse event.
 *
 * Returns: the mouse button number or 0
 *
 * Since: 2.6
 */
unsigned webkit_navigation_action_get_mouse_button(CyberKitNavigationAction* navigation)
{
    g_return_val_if_fail(navigation, 0);
    return toCyberKitMouseButton(navigation->action->mouseButton());
}

/**
 * webkit_navigation_action_get_modifiers:
 * @navigation: a #CyberKitNavigationAction
 *
 * Return the modifier keys.
 *
 * Return a bitmask of #GdkModifierType values describing the modifier keys that were in effect
 * when the navigation was requested
 *
 * Returns: the modifier keys
 *
 * Since: 2.6
 */
unsigned webkit_navigation_action_get_modifiers(CyberKitNavigationAction* navigation)
{
    g_return_val_if_fail(navigation, 0);
    return toPlatformModifiers(navigation->action->modifiers());
}

/**
 * webkit_navigation_action_get_request:
 * @navigation: a #CyberKitNavigationAction
 *
 * Return the #CyberKitURIRequest associated with the navigation action.
 *
 * Modifications to the returned object are <emphasis>not</emphasis> taken
 * into account when the request is sent over the network, and is intended
 * only to aid in evaluating whether a navigation action should be taken or
 * not. To modify requests before they are sent over the network the
 * #CyberKitPage::send-request signal can be used instead.
 *
 * Returns: (transfer none): a #CyberKitURIRequest
 *
 * Since: 2.6
 */
CyberKitURIRequest* webkit_navigation_action_get_request(CyberKitNavigationAction* navigation)
{
    g_return_val_if_fail(navigation, nullptr);
    if (!navigation->request)
        navigation->request = adoptGRef(webkitURIRequestCreateForResourceRequest(navigation->action->request()));
    return navigation->request.get();
}

/**
 * webkit_navigation_action_is_user_gesture:
 * @navigation: a #CyberKitNavigationAction
 *
 * Return whether the navigation was triggered by a user gesture like a mouse click.
 *
 * Returns: whether navigation action is a user gesture
 *
 * Since: 2.6
 */
gboolean webkit_navigation_action_is_user_gesture(CyberKitNavigationAction* navigation)
{
    g_return_val_if_fail(navigation, FALSE);
    return navigation->action->isProcessingUserGesture();
}

/**
 * webkit_navigation_action_is_redirect:
 * @navigation: a #CyberKitNavigationAction
 *
 * Returns whether the @navigation was redirected.
 *
 * Returns: %TRUE if the original navigation was redirected, %FALSE otherwise.
 *
 * Since: 2.20
 */
gboolean webkit_navigation_action_is_redirect(CyberKitNavigationAction* navigation)
{
    g_return_val_if_fail(navigation, FALSE);
    return navigation->action->isRedirect();
}

/**
 * webkit_navigation_action_get_frame_name:
 * @navigation: a #CyberKitNavigationAction
 *
 * Gets the @navigation target frame name. For example if navigation was triggered by clicking a
 * link with a target attribute equal to "_blank", this will return the value of that attribute.
 * In all other cases this function will return %NULL.
 *
 * Returns: (nullable): The name of the new frame this navigation action targets or %NULL
 *
 * Since: 2.40
 */
const char* webkit_navigation_action_get_frame_name(CyberKitNavigationAction* navigation)
{
    g_return_val_if_fail(navigation, nullptr);
    if (!navigation->frameName) {
        if (auto targetFrameName = navigation->action->targetFrameName())
            navigation->frameName = targetFrameName->utf8();
        else
            navigation->frameName = CString();
    }
    return navigation->frameName->data();
}
