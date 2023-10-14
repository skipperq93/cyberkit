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

#ifndef CyberKitDOMHTMLTableCellElement_h
#define CyberKitDOMHTMLTableCellElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_TABLE_CELL_ELEMENT            (webkit_dom_html_table_cell_element_get_type())
#define WEBKIT_DOM_HTML_TABLE_CELL_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_TABLE_CELL_ELEMENT, CyberKitDOMHTMLTableCellElement))
#define WEBKIT_DOM_HTML_TABLE_CELL_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_TABLE_CELL_ELEMENT, CyberKitDOMHTMLTableCellElementClass)
#define WEBKIT_DOM_IS_HTML_TABLE_CELL_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_TABLE_CELL_ELEMENT))
#define WEBKIT_DOM_IS_HTML_TABLE_CELL_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_TABLE_CELL_ELEMENT))
#define WEBKIT_DOM_HTML_TABLE_CELL_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_TABLE_CELL_ELEMENT, CyberKitDOMHTMLTableCellElementClass))

struct _CyberKitDOMHTMLTableCellElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLTableCellElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_table_cell_element_get_type(void);

/**
 * webkit_dom_html_table_cell_element_get_cell_index:
 * @self: A #CyberKitDOMHTMLTableCellElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_table_cell_element_get_cell_index(CyberKitDOMHTMLTableCellElement* self);

/**
 * webkit_dom_html_table_cell_element_get_align:
 * @self: A #CyberKitDOMHTMLTableCellElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_cell_element_get_align(CyberKitDOMHTMLTableCellElement* self);

/**
 * webkit_dom_html_table_cell_element_set_align:
 * @self: A #CyberKitDOMHTMLTableCellElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_cell_element_set_align(CyberKitDOMHTMLTableCellElement* self, const gchar* value);

/**
 * webkit_dom_html_table_cell_element_get_axis:
 * @self: A #CyberKitDOMHTMLTableCellElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_cell_element_get_axis(CyberKitDOMHTMLTableCellElement* self);

/**
 * webkit_dom_html_table_cell_element_set_axis:
 * @self: A #CyberKitDOMHTMLTableCellElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_cell_element_set_axis(CyberKitDOMHTMLTableCellElement* self, const gchar* value);

/**
 * webkit_dom_html_table_cell_element_get_bg_color:
 * @self: A #CyberKitDOMHTMLTableCellElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_cell_element_get_bg_color(CyberKitDOMHTMLTableCellElement* self);

/**
 * webkit_dom_html_table_cell_element_set_bg_color:
 * @self: A #CyberKitDOMHTMLTableCellElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_cell_element_set_bg_color(CyberKitDOMHTMLTableCellElement* self, const gchar* value);

/**
 * webkit_dom_html_table_cell_element_get_ch:
 * @self: A #CyberKitDOMHTMLTableCellElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_cell_element_get_ch(CyberKitDOMHTMLTableCellElement* self);

/**
 * webkit_dom_html_table_cell_element_set_ch:
 * @self: A #CyberKitDOMHTMLTableCellElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_cell_element_set_ch(CyberKitDOMHTMLTableCellElement* self, const gchar* value);

/**
 * webkit_dom_html_table_cell_element_get_ch_off:
 * @self: A #CyberKitDOMHTMLTableCellElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_cell_element_get_ch_off(CyberKitDOMHTMLTableCellElement* self);

/**
 * webkit_dom_html_table_cell_element_set_ch_off:
 * @self: A #CyberKitDOMHTMLTableCellElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_cell_element_set_ch_off(CyberKitDOMHTMLTableCellElement* self, const gchar* value);

/**
 * webkit_dom_html_table_cell_element_get_col_span:
 * @self: A #CyberKitDOMHTMLTableCellElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_table_cell_element_get_col_span(CyberKitDOMHTMLTableCellElement* self);

/**
 * webkit_dom_html_table_cell_element_set_col_span:
 * @self: A #CyberKitDOMHTMLTableCellElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_cell_element_set_col_span(CyberKitDOMHTMLTableCellElement* self, glong value);

/**
 * webkit_dom_html_table_cell_element_get_row_span:
 * @self: A #CyberKitDOMHTMLTableCellElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_table_cell_element_get_row_span(CyberKitDOMHTMLTableCellElement* self);

/**
 * webkit_dom_html_table_cell_element_set_row_span:
 * @self: A #CyberKitDOMHTMLTableCellElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_cell_element_set_row_span(CyberKitDOMHTMLTableCellElement* self, glong value);

/**
 * webkit_dom_html_table_cell_element_get_headers:
 * @self: A #CyberKitDOMHTMLTableCellElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_cell_element_get_headers(CyberKitDOMHTMLTableCellElement* self);

/**
 * webkit_dom_html_table_cell_element_set_headers:
 * @self: A #CyberKitDOMHTMLTableCellElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_cell_element_set_headers(CyberKitDOMHTMLTableCellElement* self, const gchar* value);

/**
 * webkit_dom_html_table_cell_element_get_height:
 * @self: A #CyberKitDOMHTMLTableCellElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_cell_element_get_height(CyberKitDOMHTMLTableCellElement* self);

/**
 * webkit_dom_html_table_cell_element_set_height:
 * @self: A #CyberKitDOMHTMLTableCellElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_cell_element_set_height(CyberKitDOMHTMLTableCellElement* self, const gchar* value);

/**
 * webkit_dom_html_table_cell_element_get_no_wrap:
 * @self: A #CyberKitDOMHTMLTableCellElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_table_cell_element_get_no_wrap(CyberKitDOMHTMLTableCellElement* self);

/**
 * webkit_dom_html_table_cell_element_set_no_wrap:
 * @self: A #CyberKitDOMHTMLTableCellElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_cell_element_set_no_wrap(CyberKitDOMHTMLTableCellElement* self, gboolean value);

/**
 * webkit_dom_html_table_cell_element_get_v_align:
 * @self: A #CyberKitDOMHTMLTableCellElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_cell_element_get_v_align(CyberKitDOMHTMLTableCellElement* self);

/**
 * webkit_dom_html_table_cell_element_set_v_align:
 * @self: A #CyberKitDOMHTMLTableCellElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_cell_element_set_v_align(CyberKitDOMHTMLTableCellElement* self, const gchar* value);

/**
 * webkit_dom_html_table_cell_element_get_width:
 * @self: A #CyberKitDOMHTMLTableCellElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_cell_element_get_width(CyberKitDOMHTMLTableCellElement* self);

/**
 * webkit_dom_html_table_cell_element_set_width:
 * @self: A #CyberKitDOMHTMLTableCellElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_cell_element_set_width(CyberKitDOMHTMLTableCellElement* self, const gchar* value);

/**
 * webkit_dom_html_table_cell_element_get_abbr:
 * @self: A #CyberKitDOMHTMLTableCellElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_cell_element_get_abbr(CyberKitDOMHTMLTableCellElement* self);

/**
 * webkit_dom_html_table_cell_element_set_abbr:
 * @self: A #CyberKitDOMHTMLTableCellElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_cell_element_set_abbr(CyberKitDOMHTMLTableCellElement* self, const gchar* value);

/**
 * webkit_dom_html_table_cell_element_get_scope:
 * @self: A #CyberKitDOMHTMLTableCellElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_cell_element_get_scope(CyberKitDOMHTMLTableCellElement* self);

/**
 * webkit_dom_html_table_cell_element_set_scope:
 * @self: A #CyberKitDOMHTMLTableCellElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_cell_element_set_scope(CyberKitDOMHTMLTableCellElement* self, const gchar* value);

G_END_DECLS

#endif /* CyberKitDOMHTMLTableCellElement_h */
