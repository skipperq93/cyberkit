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

#ifndef CyberKitDOMHTMLTableSectionElement_h
#define CyberKitDOMHTMLTableSectionElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_TABLE_SECTION_ELEMENT            (webkit_dom_html_table_section_element_get_type())
#define WEBKIT_DOM_HTML_TABLE_SECTION_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_TABLE_SECTION_ELEMENT, CyberKitDOMHTMLTableSectionElement))
#define WEBKIT_DOM_HTML_TABLE_SECTION_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_TABLE_SECTION_ELEMENT, CyberKitDOMHTMLTableSectionElementClass)
#define WEBKIT_DOM_IS_HTML_TABLE_SECTION_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_TABLE_SECTION_ELEMENT))
#define WEBKIT_DOM_IS_HTML_TABLE_SECTION_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_TABLE_SECTION_ELEMENT))
#define WEBKIT_DOM_HTML_TABLE_SECTION_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_TABLE_SECTION_ELEMENT, CyberKitDOMHTMLTableSectionElementClass))

struct _CyberKitDOMHTMLTableSectionElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLTableSectionElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_table_section_element_get_type(void);

/**
 * webkit_dom_html_table_section_element_insert_row:
 * @self: A #CyberKitDOMHTMLTableSectionElement
 * @index: A #glong
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLElement
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLElement*
webkit_dom_html_table_section_element_insert_row(CyberKitDOMHTMLTableSectionElement* self, glong index, GError** error);

/**
 * webkit_dom_html_table_section_element_delete_row:
 * @self: A #CyberKitDOMHTMLTableSectionElement
 * @index: A #glong
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_section_element_delete_row(CyberKitDOMHTMLTableSectionElement* self, glong index, GError** error);

/**
 * webkit_dom_html_table_section_element_get_align:
 * @self: A #CyberKitDOMHTMLTableSectionElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_section_element_get_align(CyberKitDOMHTMLTableSectionElement* self);

/**
 * webkit_dom_html_table_section_element_set_align:
 * @self: A #CyberKitDOMHTMLTableSectionElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_section_element_set_align(CyberKitDOMHTMLTableSectionElement* self, const gchar* value);

/**
 * webkit_dom_html_table_section_element_get_ch:
 * @self: A #CyberKitDOMHTMLTableSectionElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_section_element_get_ch(CyberKitDOMHTMLTableSectionElement* self);

/**
 * webkit_dom_html_table_section_element_set_ch:
 * @self: A #CyberKitDOMHTMLTableSectionElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_section_element_set_ch(CyberKitDOMHTMLTableSectionElement* self, const gchar* value);

/**
 * webkit_dom_html_table_section_element_get_ch_off:
 * @self: A #CyberKitDOMHTMLTableSectionElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_section_element_get_ch_off(CyberKitDOMHTMLTableSectionElement* self);

/**
 * webkit_dom_html_table_section_element_set_ch_off:
 * @self: A #CyberKitDOMHTMLTableSectionElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_section_element_set_ch_off(CyberKitDOMHTMLTableSectionElement* self, const gchar* value);

/**
 * webkit_dom_html_table_section_element_get_v_align:
 * @self: A #CyberKitDOMHTMLTableSectionElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_section_element_get_v_align(CyberKitDOMHTMLTableSectionElement* self);

/**
 * webkit_dom_html_table_section_element_set_v_align:
 * @self: A #CyberKitDOMHTMLTableSectionElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_section_element_set_v_align(CyberKitDOMHTMLTableSectionElement* self, const gchar* value);

/**
 * webkit_dom_html_table_section_element_get_rows:
 * @self: A #CyberKitDOMHTMLTableSectionElement
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_html_table_section_element_get_rows(CyberKitDOMHTMLTableSectionElement* self);

G_END_DECLS

#endif /* CyberKitDOMHTMLTableSectionElement_h */
