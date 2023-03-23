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

#ifndef CyberKitDOMHTMLHRElement_h
#define CyberKitDOMHTMLHRElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_HR_ELEMENT            (webkit_dom_html_hr_element_get_type())
#define WEBKIT_DOM_HTML_HR_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_HR_ELEMENT, CyberKitDOMHTMLHRElement))
#define WEBKIT_DOM_HTML_HR_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_HR_ELEMENT, CyberKitDOMHTMLHRElementClass)
#define WEBKIT_DOM_IS_HTML_HR_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_HR_ELEMENT))
#define WEBKIT_DOM_IS_HTML_HR_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_HR_ELEMENT))
#define WEBKIT_DOM_HTML_HR_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_HR_ELEMENT, CyberKitDOMHTMLHRElementClass))

struct _CyberKitDOMHTMLHRElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLHRElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_hr_element_get_type(void);

/**
 * webkit_dom_html_hr_element_get_align:
 * @self: A #CyberKitDOMHTMLHRElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_hr_element_get_align(CyberKitDOMHTMLHRElement* self);

/**
 * webkit_dom_html_hr_element_set_align:
 * @self: A #CyberKitDOMHTMLHRElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_hr_element_set_align(CyberKitDOMHTMLHRElement* self, const gchar* value);

/**
 * webkit_dom_html_hr_element_get_no_shade:
 * @self: A #CyberKitDOMHTMLHRElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_hr_element_get_no_shade(CyberKitDOMHTMLHRElement* self);

/**
 * webkit_dom_html_hr_element_set_no_shade:
 * @self: A #CyberKitDOMHTMLHRElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_hr_element_set_no_shade(CyberKitDOMHTMLHRElement* self, gboolean value);

/**
 * webkit_dom_html_hr_element_get_size:
 * @self: A #CyberKitDOMHTMLHRElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_hr_element_get_size(CyberKitDOMHTMLHRElement* self);

/**
 * webkit_dom_html_hr_element_set_size:
 * @self: A #CyberKitDOMHTMLHRElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_hr_element_set_size(CyberKitDOMHTMLHRElement* self, const gchar* value);

/**
 * webkit_dom_html_hr_element_get_width:
 * @self: A #CyberKitDOMHTMLHRElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_hr_element_get_width(CyberKitDOMHTMLHRElement* self);

/**
 * webkit_dom_html_hr_element_set_width:
 * @self: A #CyberKitDOMHTMLHRElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_hr_element_set_width(CyberKitDOMHTMLHRElement* self, const gchar* value);

G_END_DECLS

#endif /* CyberKitDOMHTMLHRElement_h */
