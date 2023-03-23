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

#ifndef CyberKitDOMStyleSheet_h
#define CyberKitDOMStyleSheet_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMObject.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_STYLE_SHEET            (webkit_dom_style_sheet_get_type())
#define WEBKIT_DOM_STYLE_SHEET(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_STYLE_SHEET, CyberKitDOMStyleSheet))
#define WEBKIT_DOM_STYLE_SHEET_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_STYLE_SHEET, CyberKitDOMStyleSheetClass)
#define WEBKIT_DOM_IS_STYLE_SHEET(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_STYLE_SHEET))
#define WEBKIT_DOM_IS_STYLE_SHEET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_STYLE_SHEET))
#define WEBKIT_DOM_STYLE_SHEET_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_STYLE_SHEET, CyberKitDOMStyleSheetClass))

struct _CyberKitDOMStyleSheet {
    CyberKitDOMObject parent_instance;
};

struct _CyberKitDOMStyleSheetClass {
    CyberKitDOMObjectClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_style_sheet_get_type(void);

/**
 * webkit_dom_style_sheet_get_content_type:
 * @self: A #CyberKitDOMStyleSheet
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_style_sheet_get_content_type(CyberKitDOMStyleSheet* self);

/**
 * webkit_dom_style_sheet_get_disabled:
 * @self: A #CyberKitDOMStyleSheet
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_style_sheet_get_disabled(CyberKitDOMStyleSheet* self);

/**
 * webkit_dom_style_sheet_set_disabled:
 * @self: A #CyberKitDOMStyleSheet
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_style_sheet_set_disabled(CyberKitDOMStyleSheet* self, gboolean value);

/**
 * webkit_dom_style_sheet_get_owner_node:
 * @self: A #CyberKitDOMStyleSheet
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_style_sheet_get_owner_node(CyberKitDOMStyleSheet* self);

/**
 * webkit_dom_style_sheet_get_parent_style_sheet:
 * @self: A #CyberKitDOMStyleSheet
 *
 * Returns: (transfer full): A #CyberKitDOMStyleSheet
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMStyleSheet*
webkit_dom_style_sheet_get_parent_style_sheet(CyberKitDOMStyleSheet* self);

/**
 * webkit_dom_style_sheet_get_href:
 * @self: A #CyberKitDOMStyleSheet
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_style_sheet_get_href(CyberKitDOMStyleSheet* self);

/**
 * webkit_dom_style_sheet_get_title:
 * @self: A #CyberKitDOMStyleSheet
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_style_sheet_get_title(CyberKitDOMStyleSheet* self);

/**
 * webkit_dom_style_sheet_get_media:
 * @self: A #CyberKitDOMStyleSheet
 *
 * Returns: (transfer full): A #CyberKitDOMMediaList
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMMediaList*
webkit_dom_style_sheet_get_media(CyberKitDOMStyleSheet* self);

G_END_DECLS

#endif /* CyberKitDOMStyleSheet_h */
