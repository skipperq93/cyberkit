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

#if !defined(__WEBKIT_H_INSIDE__) && !defined(WEBKIT2_COMPILATION)
#error "Only <wpe/webkit.h> can be included directly."
#endif

#ifndef CyberKitResponsePolicyDecision_h
#define CyberKitResponsePolicyDecision_h

#include <glib-object.h>
#include <wpe/CyberKitDefines.h>
#include <wpe/CyberKitPolicyDecision.h>
#include <wpe/CyberKitURIResponse.h>
#include <wpe/CyberKitURIRequest.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_RESPONSE_POLICY_DECISION            (webkit_response_policy_decision_get_type())
#define WEBKIT_RESPONSE_POLICY_DECISION(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_RESPONSE_POLICY_DECISION, CyberKitResponsePolicyDecision))
#define WEBKIT_RESPONSE_POLICY_DECISION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_RESPONSE_POLICY_DECISION, CyberKitResponsePolicyDecisionClass))
#define WEBKIT_IS_RESPONSE_POLICY_DECISION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_RESPONSE_POLICY_DECISION))
#define WEBKIT_IS_RESPONSE_POLICY_DECISION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_RESPONSE_POLICY_DECISION))
#define WEBKIT_RESPONSE_POLICY_DECISION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_RESPONSE_POLICY_DECISION, CyberKitResponsePolicyDecisionClass))

typedef struct _CyberKitResponsePolicyDecision        CyberKitResponsePolicyDecision;
typedef struct _CyberKitResponsePolicyDecisionClass   CyberKitResponsePolicyDecisionClass;
typedef struct _CyberKitResponsePolicyDecisionPrivate CyberKitResponsePolicyDecisionPrivate;

struct _CyberKitResponsePolicyDecision {
    CyberKitPolicyDecision parent;

    /*< private >*/
    CyberKitResponsePolicyDecisionPrivate *priv;
};

struct _CyberKitResponsePolicyDecisionClass {
    CyberKitPolicyDecisionClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_response_policy_decision_get_type               (void);

WEBKIT_API CyberKitURIRequest *
webkit_response_policy_decision_get_request            (CyberKitResponsePolicyDecision *decision);

WEBKIT_API CyberKitURIResponse *
webkit_response_policy_decision_get_response           (CyberKitResponsePolicyDecision *decision);

WEBKIT_API gboolean
webkit_response_policy_decision_is_mime_type_supported (CyberKitResponsePolicyDecision *decision);

G_END_DECLS

#endif
