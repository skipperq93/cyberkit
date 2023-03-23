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

#ifndef CyberKitDOMHTMLLIElement_h
#define CyberKitDOMHTMLLIElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_LI_ELEMENT            (webkit_dom_html_li_element_get_type())
#define WEBKIT_DOM_HTML_LI_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_LI_ELEMENT, CyberKitDOMHTMLLIElement))
#define WEBKIT_DOM_HTML_LI_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_LI_ELEMENT, CyberKitDOMHTMLLIElementClass)
#define WEBKIT_DOM_IS_HTML_LI_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_LI_ELEMENT))
#define WEBKIT_DOM_IS_HTML_LI_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_LI_ELEMENT))
#define WEBKIT_DOM_HTML_LI_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_LI_ELEMENT, CyberKitDOMHTMLLIElementClass))

struct _CyberKitDOMHTMLLIElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLLIElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_li_element_get_type(void);

/**
 * webkit_dom_html_li_element_get_type_attr:
 * @self: A #CyberKitDOMHTMLLIElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_li_element_get_type_attr(CyberKitDOMHTMLLIElement* self);

/**
 * webkit_dom_html_li_element_set_type_attr:
 * @self: A #CyberKitDOMHTMLLIElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_li_element_set_type_attr(CyberKitDOMHTMLLIElement* self, const gchar* value);

/**
 * webkit_dom_html_li_element_get_value:
 * @self: A #CyberKitDOMHTMLLIElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_li_element_get_value(CyberKitDOMHTMLLIElement* self);

/**
 * webkit_dom_html_li_element_set_value:
 * @self: A #CyberKitDOMHTMLLIElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_li_element_set_value(CyberKitDOMHTMLLIElement* self, glong value);

G_END_DECLS

#endif /* CyberKitDOMHTMLLIElement_h */
