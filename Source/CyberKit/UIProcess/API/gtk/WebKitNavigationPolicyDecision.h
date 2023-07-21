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

#if !defined(__WEBKIT2_H_INSIDE__) && !defined(WEBKIT2_COMPILATION)
#error "Only <webkit2/webkit2.h> can be included directly."
#endif

#ifndef CyberKitNavigationPolicyDecision_h
#define CyberKitNavigationPolicyDecision_h

#include <glib-object.h>
#include <webkit2/CyberKitDefines.h>
#include <webkit2/CyberKitNavigationAction.h>
#include <webkit2/CyberKitPolicyDecision.h>
#include <webkit2/CyberKitURIRequest.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_NAVIGATION_POLICY_DECISION            (webkit_navigation_policy_decision_get_type())
#define WEBKIT_NAVIGATION_POLICY_DECISION(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_NAVIGATION_POLICY_DECISION, CyberKitNavigationPolicyDecision))
#define WEBKIT_NAVIGATION_POLICY_DECISION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_NAVIGATION_POLICY_DECISION, CyberKitNavigationPolicyDecisionClass))
#define WEBKIT_IS_NAVIGATION_POLICY_DECISION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_NAVIGATION_POLICY_DECISION))
#define WEBKIT_IS_NAVIGATION_POLICY_DECISION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_NAVIGATION_POLICY_DECISION))
#define WEBKIT_NAVIGATION_POLICY_DECISION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_NAVIGATION_POLICY_DECISION, CyberKitNavigationPolicyDecisionClass))

typedef struct _CyberKitNavigationPolicyDecision        CyberKitNavigationPolicyDecision;
typedef struct _CyberKitNavigationPolicyDecisionClass   CyberKitNavigationPolicyDecisionClass;
typedef struct _CyberKitNavigationPolicyDecisionPrivate CyberKitNavigationPolicyDecisionPrivate;

struct _CyberKitNavigationPolicyDecision {
    CyberKitPolicyDecision parent;

    /*< private >*/
    CyberKitNavigationPolicyDecisionPrivate *priv;
};

struct _CyberKitNavigationPolicyDecisionClass {
    CyberKitPolicyDecisionClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_navigation_policy_decision_get_type            (void);

WEBKIT_API CyberKitNavigationAction *
webkit_navigation_policy_decision_get_navigation_action (CyberKitNavigationPolicyDecision *decision);

WEBKIT_DEPRECATED_FOR(webkit_navigation_policy_decision_get_navigation_action) CyberKitNavigationType
webkit_navigation_policy_decision_get_navigation_type   (CyberKitNavigationPolicyDecision *decision);

WEBKIT_DEPRECATED_FOR(webkit_navigation_policy_decision_get_navigation_action) guint
webkit_navigation_policy_decision_get_mouse_button      (CyberKitNavigationPolicyDecision *decision);

WEBKIT_DEPRECATED_FOR(webkit_navigation_policy_decision_get_navigation_action) guint
webkit_navigation_policy_decision_get_modifiers         (CyberKitNavigationPolicyDecision *decision);

WEBKIT_DEPRECATED_FOR(webkit_navigation_policy_decision_get_navigation_action) CyberKitURIRequest *
webkit_navigation_policy_decision_get_request           (CyberKitNavigationPolicyDecision *decision);

WEBKIT_API const gchar *
webkit_navigation_policy_decision_get_frame_name        (CyberKitNavigationPolicyDecision *decision);

G_END_DECLS

#endif
