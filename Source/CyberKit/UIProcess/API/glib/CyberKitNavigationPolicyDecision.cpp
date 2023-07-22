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
#include "CyberKitNavigationPolicyDecision.h"

#include "CyberKitEnumTypes.h"
#include "CyberKitNavigationActionPrivate.h"
#include "CyberKitNavigationPolicyDecisionPrivate.h"
#include "CyberKitPolicyDecisionPrivate.h"
#include "CyberKitURIRequestPrivate.h"
#include <glib/gi18n-lib.h>
#include <wtf/glib/GRefPtr.h>
#include <wtf/glib/WTFGType.h>
#include <wtf/text/CString.h>

using namespace CyberKit;
using namespace CyberCore;

/**
 * SECTION: CyberKitNavigationPolicyDecision
 * @Short_description: A policy decision for navigation actions
 * @Title: CyberKitNavigationPolicyDecision
 * @See_also: #CyberKitPolicyDecision, #CyberKitWebView
 *
 * CyberKitNavigationPolicyDecision represents a policy decision for events associated with
 * navigations. If the value of #CyberKitNavigationPolicyDecision:mouse-button is not 0, then
 * the navigation was triggered by a mouse event.
 */

struct _CyberKitNavigationPolicyDecisionPrivate {
    ~_CyberKitNavigationPolicyDecisionPrivate()
    {
        webkit_navigation_action_free(navigationAction);
    }

    CyberKitNavigationAction* navigationAction;
    CString frameName;
};

WEBKIT_DEFINE_TYPE(CyberKitNavigationPolicyDecision, webkit_navigation_policy_decision, WEBKIT_TYPE_POLICY_DECISION)

enum {
    PROP_0,
    PROP_NAVIGATION_ACTION,
#if PLATFORM(GTK)
    PROP_NAVIGATION_TYPE,
    PROP_MOUSE_BUTTON,
    PROP_MODIFIERS,
    PROP_REQUEST,
#endif
    PROP_FRAME_NAME,
};

static void webkitNavigationPolicyDecisionGetProperty(GObject* object, guint propId, GValue* value, GParamSpec* paramSpec)
{
    CyberKitNavigationPolicyDecision* decision = WEBKIT_NAVIGATION_POLICY_DECISION(object);
    switch (propId) {
    case PROP_NAVIGATION_ACTION:
        g_value_set_boxed(value, webkit_navigation_policy_decision_get_navigation_action(decision));
        break;
#if PLATFORM(GTK)
    case PROP_NAVIGATION_TYPE:
        g_value_set_enum(value, webkit_navigation_action_get_navigation_type(decision->priv->navigationAction));
        break;
    case PROP_MOUSE_BUTTON:
        g_value_set_enum(value, webkit_navigation_action_get_mouse_button(decision->priv->navigationAction));
        break;
    case PROP_MODIFIERS:
        g_value_set_uint(value, webkit_navigation_action_get_modifiers(decision->priv->navigationAction));
        break;
    case PROP_REQUEST:
        g_value_set_object(value, webkit_navigation_action_get_request(decision->priv->navigationAction));
        break;
#endif
    case PROP_FRAME_NAME:
        g_value_set_string(value, webkit_navigation_policy_decision_get_frame_name(decision));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propId, paramSpec);
        break;
    }
}

static void webkit_navigation_policy_decision_class_init(CyberKitNavigationPolicyDecisionClass* decisionClass)
{
    GObjectClass* objectClass = G_OBJECT_CLASS(decisionClass);
    objectClass->get_property = webkitNavigationPolicyDecisionGetProperty;

    /**
     * CyberKitNavigationPolicyDecision:navigation-action:
     *
     * The #CyberKitNavigationAction that triggered this policy decision.
     *
     * Since: 2.6
     */
    g_object_class_install_property(
        objectClass,
        PROP_NAVIGATION_ACTION,
        g_param_spec_boxed(
            "navigation-action",
            _("Navigation action"),
            _("The CyberKitNavigationAction triggering this decision"),
            WEBKIT_TYPE_NAVIGATION_ACTION,
            WEBKIT_PARAM_READABLE));

#if PLATFORM(GTK)
    /**
     * CyberKitNavigationPolicyDecision:navigation-type:
     *
     * The type of navigation that triggered this policy decision. This is
     * useful for enacting different policies depending on what type of user
     * action caused the navigation.
     *
     * Deprecated: 2.6: Use #CyberKitNavigationPolicyDecision:navigation-action instead
     */
    g_object_class_install_property(objectClass,
                                    PROP_NAVIGATION_TYPE,
                                    g_param_spec_enum("navigation-type",
                                                      _("Navigation type"),
                                                      _("The type of navigation triggering this decision"),
                                                      WEBKIT_TYPE_NAVIGATION_TYPE,
                                                      WEBKIT_NAVIGATION_TYPE_LINK_CLICKED,
                                                      WEBKIT_PARAM_READABLE));

    /**
     * CyberKitNavigationPolicyDecision:mouse-button:
     *
     * If the navigation associated with this policy decision was originally
     * triggered by a mouse event, this property contains non-zero button number
     * of the button triggering that event. The button numbers match those from GDK.
     * If the navigation was not triggered by a mouse event, the value of this
     * property will be 0.
     *
     * Deprecated: 2.6: Use #CyberKitNavigationPolicyDecision:navigation-action instead
     */
    g_object_class_install_property(objectClass,
                                    PROP_MOUSE_BUTTON,
                                    g_param_spec_uint("mouse-button",
                                                      _("Mouse button"),
                                                      _("The mouse button used if this decision was triggered by a mouse event"),
                                                      0, G_MAXUINT, 0,
                                                      WEBKIT_PARAM_READABLE));

    /**
     * CyberKitNavigationPolicyDecision:modifiers:
     *
     * If the navigation associated with this policy decision was originally
     * triggered by a mouse event, this property contains a bitmask of various
     * #GdkModifierType values describing the modifiers used for that click.
     * If the navigation was not triggered by a mouse event or no modifiers
     * were active, the value of this property will be zero.
     *
     * Deprecated: 2.6: Use #CyberKitNavigationPolicyDecision:navigation-action instead
     */
    g_object_class_install_property(objectClass,
                                    PROP_MODIFIERS,
                                    g_param_spec_uint("modifiers",
                                                      _("Mouse event modifiers"),
                                                      _("The modifiers active if this decision was triggered by a mouse event"),
                                                      0, G_MAXUINT, 0,
                                                      WEBKIT_PARAM_READABLE));

    /**
     * CyberKitNavigationPolicyDecision:request:
     *
     * This property contains the #CyberKitURIRequest associated with this
     * navigation.
     *
     * Deprecated: 2.6: Use #CyberKitNavigationPolicyDecision:navigation-action instead
     */
    g_object_class_install_property(objectClass,
                                    PROP_REQUEST,
                                    g_param_spec_object("request",
                                                      _("Navigation URI request"),
                                                      _("The URI request that is associated with this navigation"),
                                                      WEBKIT_TYPE_URI_REQUEST,
                                                      WEBKIT_PARAM_READABLE));
#endif

    /**
     * CyberKitNavigationPolicyDecision:frame-name:
     *
     * If this navigation request targets a new frame, this property contains
     * the name of that frame. For example if the decision was triggered by clicking a
     * link with a target attribute equal to "_blank", this property will contain the
     * value of that attribute. In all other cases, this value will be %NULL.
     */
    g_object_class_install_property(objectClass,
                                    PROP_FRAME_NAME,
                                    g_param_spec_string("frame-name",
                                                      _("Frame name"),
                                                      _("The name of the new frame this navigation action targets"),
                                                      0,
                                                      WEBKIT_PARAM_READABLE));
}

/**
 * webkit_navigation_policy_decision_get_navigation_action:
 * @decision: a #CyberKitNavigationPolicyDecision
 *
 * Gets the value of the #CyberKitNavigationPolicyDecision:navigation-action property.
 *
 * Returns: (transfer none): The #CyberKitNavigationAction triggering this policy decision.
 *
 * Since: 2.6
 */
CyberKitNavigationAction* webkit_navigation_policy_decision_get_navigation_action(CyberKitNavigationPolicyDecision* decision)
{
    g_return_val_if_fail(WEBKIT_IS_NAVIGATION_POLICY_DECISION(decision), nullptr);
    return decision->priv->navigationAction;
}

#if PLATFORM(GTK)
/**
 * webkit_navigation_policy_decision_get_navigation_type:
 * @decision: a #CyberKitNavigationPolicyDecision
 *
 * Gets the value of the #CyberKitNavigationPolicyDecision:navigation-type property.
 *
 * Returns: The type of navigation triggering this policy decision.
 *
 * Deprecated: 2.6: Use webkit_navigation_policy_decision_get_navigation_action() instead.
 */
CyberKitNavigationType webkit_navigation_policy_decision_get_navigation_type(CyberKitNavigationPolicyDecision* decision)
{
    g_return_val_if_fail(WEBKIT_IS_NAVIGATION_POLICY_DECISION(decision), WEBKIT_NAVIGATION_TYPE_OTHER);
    return webkit_navigation_action_get_navigation_type(decision->priv->navigationAction);
}

/**
 * webkit_navigation_policy_decision_get_mouse_button:
 * @decision: a #CyberKitNavigationPolicyDecision
 *
 * Gets the value of the #CyberKitNavigationPolicyDecision:mouse-button property.
 *
 * Returns: The mouse button used if this decision was triggered by a mouse event or 0 otherwise
 *
 * Deprecated: 2.6: Use webkit_navigation_policy_decision_get_navigation_action() instead.
 */
guint webkit_navigation_policy_decision_get_mouse_button(CyberKitNavigationPolicyDecision* decision)
{
    g_return_val_if_fail(WEBKIT_IS_NAVIGATION_POLICY_DECISION(decision), 0);
    return webkit_navigation_action_get_mouse_button(decision->priv->navigationAction);
}

/**
 * webkit_navigation_policy_decision_get_modifiers:
 * @decision: a #CyberKitNavigationPolicyDecision
 *
 * Gets the value of the #CyberKitNavigationPolicyDecision:modifiers property.
 *
 * Returns: The modifiers active if this decision was triggered by a mouse event
 *
 * Deprecated: 2.6: Use webkit_navigation_policy_decision_get_navigation_action() instead.
 */
unsigned webkit_navigation_policy_decision_get_modifiers(CyberKitNavigationPolicyDecision* decision)
{
    g_return_val_if_fail(WEBKIT_IS_NAVIGATION_POLICY_DECISION(decision), 0);
    return webkit_navigation_action_get_modifiers(decision->priv->navigationAction);
}

/**
 * webkit_navigation_policy_decision_get_request:
 * @decision: a #CyberKitNavigationPolicyDecision
 *
 * Gets the value of the #CyberKitNavigationPolicyDecision:request property.
 *
 * Returns: (transfer none): The URI request that is associated with this navigation
 *
 * Deprecated: 2.6: Use webkit_navigation_policy_decision_get_navigation_action() instead.
 */
CyberKitURIRequest* webkit_navigation_policy_decision_get_request(CyberKitNavigationPolicyDecision* decision)
{
    g_return_val_if_fail(WEBKIT_IS_NAVIGATION_POLICY_DECISION(decision), nullptr);
    return webkit_navigation_action_get_request(decision->priv->navigationAction);
}
#endif

/**
 * webkit_navigation_policy_decision_get_frame_name:
 * @decision: a #CyberKitNavigationPolicyDecision
 *
 * Gets the value of the #CyberKitNavigationPolicyDecision:frame-name property.
 *
 * Returns: The name of the new frame this navigation action targets or %NULL
 */
const char* webkit_navigation_policy_decision_get_frame_name(CyberKitNavigationPolicyDecision* decision)
{
    g_return_val_if_fail(WEBKIT_IS_NAVIGATION_POLICY_DECISION(decision), nullptr);
    // FIXME: frame name should also be moved to CyberKitNavigationAction and this method deprecated.
    return decision->priv->frameName.data();
}

CyberKitPolicyDecision* webkitNavigationPolicyDecisionCreate(Ref<API::NavigationAction>&& navigationAction, Ref<WebFramePolicyListenerProxy>&& listener)
{
    CyberKitNavigationPolicyDecision* navigationDecision = WEBKIT_NAVIGATION_POLICY_DECISION(g_object_new(WEBKIT_TYPE_NAVIGATION_POLICY_DECISION, nullptr));
    // FIXME: frame name should also be moved to CyberKitNavigationAction.
    auto targetFrameName = navigationAction->targetFrameName();
    navigationDecision->priv->navigationAction = webkitNavigationActionCreate(WTFMove(navigationAction));
    if (targetFrameName)
        navigationDecision->priv->frameName = targetFrameName->utf8();
    CyberKitPolicyDecision* decision = WEBKIT_POLICY_DECISION(navigationDecision);
    webkitPolicyDecisionSetListener(decision, WTFMove(listener));
    return decision;
}
