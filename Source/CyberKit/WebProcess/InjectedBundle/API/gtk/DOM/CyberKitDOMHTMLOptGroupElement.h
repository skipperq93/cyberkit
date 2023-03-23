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

#if !defined(__WEBKITDOM_H_INSIDE__) && !defined(BUILDING_WEBKIT)
#error "Only <webkitdom/webkitdom.h> can be included directly."
#endif

#ifndef CyberKitDOMHTMLOptGroupElement_h
#define CyberKitDOMHTMLOptGroupElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_OPT_GROUP_ELEMENT            (webkit_dom_html_opt_group_element_get_type())
#define WEBKIT_DOM_HTML_OPT_GROUP_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_OPT_GROUP_ELEMENT, CyberKitDOMHTMLOptGroupElement))
#define WEBKIT_DOM_HTML_OPT_GROUP_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_OPT_GROUP_ELEMENT, CyberKitDOMHTMLOptGroupElementClass)
#define WEBKIT_DOM_IS_HTML_OPT_GROUP_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_OPT_GROUP_ELEMENT))
#define WEBKIT_DOM_IS_HTML_OPT_GROUP_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_OPT_GROUP_ELEMENT))
#define WEBKIT_DOM_HTML_OPT_GROUP_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_OPT_GROUP_ELEMENT, CyberKitDOMHTMLOptGroupElementClass))

struct _CyberKitDOMHTMLOptGroupElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLOptGroupElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_opt_group_element_get_type(void);

/**
 * webkit_dom_html_opt_group_element_get_disabled:
 * @self: A #CyberKitDOMHTMLOptGroupElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_opt_group_element_get_disabled(CyberKitDOMHTMLOptGroupElement* self);

/**
 * webkit_dom_html_opt_group_element_set_disabled:
 * @self: A #CyberKitDOMHTMLOptGroupElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_opt_group_element_set_disabled(CyberKitDOMHTMLOptGroupElement* self, gboolean value);

/**
 * webkit_dom_html_opt_group_element_get_label:
 * @self: A #CyberKitDOMHTMLOptGroupElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_opt_group_element_get_label(CyberKitDOMHTMLOptGroupElement* self);

/**
 * webkit_dom_html_opt_group_element_set_label:
 * @self: A #CyberKitDOMHTMLOptGroupElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_opt_group_element_set_label(CyberKitDOMHTMLOptGroupElement* self, const gchar* value);

G_END_DECLS

#endif /* CyberKitDOMHTMLOptGroupElement_h */
