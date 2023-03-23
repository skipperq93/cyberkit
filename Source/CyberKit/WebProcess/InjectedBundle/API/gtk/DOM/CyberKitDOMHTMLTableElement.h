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

#ifndef CyberKitDOMHTMLTableElement_h
#define CyberKitDOMHTMLTableElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_TABLE_ELEMENT            (webkit_dom_html_table_element_get_type())
#define WEBKIT_DOM_HTML_TABLE_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_TABLE_ELEMENT, CyberKitDOMHTMLTableElement))
#define WEBKIT_DOM_HTML_TABLE_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_TABLE_ELEMENT, CyberKitDOMHTMLTableElementClass)
#define WEBKIT_DOM_IS_HTML_TABLE_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_TABLE_ELEMENT))
#define WEBKIT_DOM_IS_HTML_TABLE_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_TABLE_ELEMENT))
#define WEBKIT_DOM_HTML_TABLE_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_TABLE_ELEMENT, CyberKitDOMHTMLTableElementClass))

struct _CyberKitDOMHTMLTableElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLTableElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_table_element_get_type(void);

/**
 * webkit_dom_html_table_element_create_t_head:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLElement*
webkit_dom_html_table_element_create_t_head(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_delete_t_head:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_element_delete_t_head(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_create_t_foot:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLElement*
webkit_dom_html_table_element_create_t_foot(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_delete_t_foot:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_element_delete_t_foot(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_create_caption:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLElement*
webkit_dom_html_table_element_create_caption(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_delete_caption:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_element_delete_caption(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_insert_row:
 * @self: A #CyberKitDOMHTMLTableElement
 * @index: A #glong
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLElement*
webkit_dom_html_table_element_insert_row(CyberKitDOMHTMLTableElement* self, glong index, GError** error);

/**
 * webkit_dom_html_table_element_delete_row:
 * @self: A #CyberKitDOMHTMLTableElement
 * @index: A #glong
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_element_delete_row(CyberKitDOMHTMLTableElement* self, glong index, GError** error);

/**
 * webkit_dom_html_table_element_get_caption:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLTableCaptionElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLTableCaptionElement*
webkit_dom_html_table_element_get_caption(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_set_caption:
 * @self: A #CyberKitDOMHTMLTableElement
 * @value: A #CyberKitDOMHTMLTableCaptionElement
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_element_set_caption(CyberKitDOMHTMLTableElement* self, CyberKitDOMHTMLTableCaptionElement* value, GError** error);

/**
 * webkit_dom_html_table_element_get_t_head:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLTableSectionElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLTableSectionElement*
webkit_dom_html_table_element_get_t_head(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_set_t_head:
 * @self: A #CyberKitDOMHTMLTableElement
 * @value: A #CyberKitDOMHTMLTableSectionElement
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_element_set_t_head(CyberKitDOMHTMLTableElement* self, CyberKitDOMHTMLTableSectionElement* value, GError** error);

/**
 * webkit_dom_html_table_element_get_t_foot:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLTableSectionElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLTableSectionElement*
webkit_dom_html_table_element_get_t_foot(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_set_t_foot:
 * @self: A #CyberKitDOMHTMLTableElement
 * @value: A #CyberKitDOMHTMLTableSectionElement
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_element_set_t_foot(CyberKitDOMHTMLTableElement* self, CyberKitDOMHTMLTableSectionElement* value, GError** error);

/**
 * webkit_dom_html_table_element_get_rows:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_html_table_element_get_rows(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_get_t_bodies:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_html_table_element_get_t_bodies(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_get_align:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_element_get_align(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_set_align:
 * @self: A #CyberKitDOMHTMLTableElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_element_set_align(CyberKitDOMHTMLTableElement* self, const gchar* value);

/**
 * webkit_dom_html_table_element_get_bg_color:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_element_get_bg_color(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_set_bg_color:
 * @self: A #CyberKitDOMHTMLTableElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_element_set_bg_color(CyberKitDOMHTMLTableElement* self, const gchar* value);

/**
 * webkit_dom_html_table_element_get_border:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_element_get_border(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_set_border:
 * @self: A #CyberKitDOMHTMLTableElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_element_set_border(CyberKitDOMHTMLTableElement* self, const gchar* value);

/**
 * webkit_dom_html_table_element_get_cell_padding:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_element_get_cell_padding(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_set_cell_padding:
 * @self: A #CyberKitDOMHTMLTableElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_element_set_cell_padding(CyberKitDOMHTMLTableElement* self, const gchar* value);

/**
 * webkit_dom_html_table_element_get_cell_spacing:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_element_get_cell_spacing(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_set_cell_spacing:
 * @self: A #CyberKitDOMHTMLTableElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_element_set_cell_spacing(CyberKitDOMHTMLTableElement* self, const gchar* value);

/**
 * webkit_dom_html_table_element_get_rules:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_element_get_rules(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_set_rules:
 * @self: A #CyberKitDOMHTMLTableElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_element_set_rules(CyberKitDOMHTMLTableElement* self, const gchar* value);

/**
 * webkit_dom_html_table_element_get_summary:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_element_get_summary(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_set_summary:
 * @self: A #CyberKitDOMHTMLTableElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_element_set_summary(CyberKitDOMHTMLTableElement* self, const gchar* value);

/**
 * webkit_dom_html_table_element_get_width:
 * @self: A #CyberKitDOMHTMLTableElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_table_element_get_width(CyberKitDOMHTMLTableElement* self);

/**
 * webkit_dom_html_table_element_set_width:
 * @self: A #CyberKitDOMHTMLTableElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_table_element_set_width(CyberKitDOMHTMLTableElement* self, const gchar* value);

G_END_DECLS

#endif /* CyberKitDOMHTMLTableElement_h */
