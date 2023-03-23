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

#ifndef CyberKitDOMHTMLTableRowElement_h
#define CyberKitDOMHTMLTableRowElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_TABLE_ROW_ELEMENT            (webkit_dom_html_table_row_element_get_type())
#define WEBKIT_DOM_HTML_TABLE_ROW_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_TABLE_ROW_ELEMENT, CyberKitDOMHTMLTableRowElement))
#define WEBKIT_DOM_HTML_TABLE_ROW_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_TABLE_ROW_ELEMENT, CyberKitDOMHTMLTableRowElementClass)
#define WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_TABLE_ROW_ELEMENT))
#define WEBKIT_DOM_IS_HTML_TABLE_ROW_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_TABLE_ROW_ELEMENT))
#define WEBKIT_DOM_HTML_TABLE_ROW_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_TABLE_ROW_ELEMENT, CyberKitDOMHTMLTableRowElementClass))

struct _CyberKitDOMHTMLTableRowElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLTableRowElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_table_row_element_get_type(void);

/**
 * webkit_dom_html_table_row_element_insert_cell:
 * @self: A #CyberKitDOMHTMLTableRowElement
 * @index: A #glong
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLElement
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLElement*
webkit_dom_html_table_row_element_insert_cell(CyberKitDOMHTMLTableRowElement* self, glong index, GError** error);

/**
 * webkit_dom_html_table_row_element_delete_cell:
 * @self: A #CyberKitDOMHTMLTableRowElement
 * @index: A #glong
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_row_element_delete_cell(CyberKitDOMHTMLTableRowElement* self, glong index, GError** error);

/**
 * webkit_dom_html_table_row_element_get_row_index:
 * @self: A #CyberKitDOMHTMLTableRowElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_table_row_element_get_row_index(CyberKitDOMHTMLTableRowElement* self);

/**
 * webkit_dom_html_table_row_element_get_section_row_index:
 * @self: A #CyberKitDOMHTMLTableRowElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_table_row_element_get_section_row_index(CyberKitDOMHTMLTableRowElement* self);

/**
 * webkit_dom_html_table_row_element_get_cells:
 * @self: A #CyberKitDOMHTMLTableRowElement
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_html_table_row_element_get_cells(CyberKitDOMHTMLTableRowElement* self);

/**
 * webkit_dom_html_table_row_element_get_align:
 * @self: A #CyberKitDOMHTMLTableRowElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_row_element_get_align(CyberKitDOMHTMLTableRowElement* self);

/**
 * webkit_dom_html_table_row_element_set_align:
 * @self: A #CyberKitDOMHTMLTableRowElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_row_element_set_align(CyberKitDOMHTMLTableRowElement* self, const gchar* value);

/**
 * webkit_dom_html_table_row_element_get_bg_color:
 * @self: A #CyberKitDOMHTMLTableRowElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_row_element_get_bg_color(CyberKitDOMHTMLTableRowElement* self);

/**
 * webkit_dom_html_table_row_element_set_bg_color:
 * @self: A #CyberKitDOMHTMLTableRowElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_row_element_set_bg_color(CyberKitDOMHTMLTableRowElement* self, const gchar* value);

/**
 * webkit_dom_html_table_row_element_get_ch:
 * @self: A #CyberKitDOMHTMLTableRowElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_row_element_get_ch(CyberKitDOMHTMLTableRowElement* self);

/**
 * webkit_dom_html_table_row_element_set_ch:
 * @self: A #CyberKitDOMHTMLTableRowElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_row_element_set_ch(CyberKitDOMHTMLTableRowElement* self, const gchar* value);

/**
 * webkit_dom_html_table_row_element_get_ch_off:
 * @self: A #CyberKitDOMHTMLTableRowElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_row_element_get_ch_off(CyberKitDOMHTMLTableRowElement* self);

/**
 * webkit_dom_html_table_row_element_set_ch_off:
 * @self: A #CyberKitDOMHTMLTableRowElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_row_element_set_ch_off(CyberKitDOMHTMLTableRowElement* self, const gchar* value);

/**
 * webkit_dom_html_table_row_element_get_v_align:
 * @self: A #CyberKitDOMHTMLTableRowElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_row_element_get_v_align(CyberKitDOMHTMLTableRowElement* self);

/**
 * webkit_dom_html_table_row_element_set_v_align:
 * @self: A #CyberKitDOMHTMLTableRowElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_row_element_set_v_align(CyberKitDOMHTMLTableRowElement* self, const gchar* value);

G_END_DECLS

#endif /* CyberKitDOMHTMLTableRowElement_h */
