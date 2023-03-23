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

#ifndef CyberKitDOMHTMLElement_h
#define CyberKitDOMHTMLElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_ELEMENT            (webkit_dom_html_element_get_type())
#define WEBKIT_DOM_HTML_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_ELEMENT, CyberKitDOMHTMLElement))
#define WEBKIT_DOM_HTML_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_ELEMENT, CyberKitDOMHTMLElementClass)
#define WEBKIT_DOM_IS_HTML_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_ELEMENT))
#define WEBKIT_DOM_IS_HTML_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_ELEMENT))
#define WEBKIT_DOM_HTML_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_ELEMENT, CyberKitDOMHTMLElementClass))

struct _CyberKitDOMHTMLElement {
    CyberKitDOMElement parent_instance;
};

struct _CyberKitDOMHTMLElementClass {
    CyberKitDOMElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_element_get_type(void);

/**
 * webkit_dom_html_element_click:
 * @self: A #CyberKitDOMHTMLElement
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_element_click(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_get_title:
 * @self: A #CyberKitDOMHTMLElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_element_get_title(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_set_title:
 * @self: A #CyberKitDOMHTMLElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_element_set_title(CyberKitDOMHTMLElement* self, const gchar* value);

/**
 * webkit_dom_html_element_get_lang:
 * @self: A #CyberKitDOMHTMLElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_element_get_lang(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_set_lang:
 * @self: A #CyberKitDOMHTMLElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_element_set_lang(CyberKitDOMHTMLElement* self, const gchar* value);

/**
 * webkit_dom_html_element_get_dir:
 * @self: A #CyberKitDOMHTMLElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_element_get_dir(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_set_dir:
 * @self: A #CyberKitDOMHTMLElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_element_set_dir(CyberKitDOMHTMLElement* self, const gchar* value);

/**
 * webkit_dom_html_element_get_tab_index:
 * @self: A #CyberKitDOMHTMLElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_element_get_tab_index(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_set_tab_index:
 * @self: A #CyberKitDOMHTMLElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_element_set_tab_index(CyberKitDOMHTMLElement* self, glong value);

/**
 * webkit_dom_html_element_get_access_key:
 * @self: A #CyberKitDOMHTMLElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_element_get_access_key(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_set_access_key:
 * @self: A #CyberKitDOMHTMLElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_element_set_access_key(CyberKitDOMHTMLElement* self, const gchar* value);

/**
 * webkit_dom_html_element_get_inner_text:
 * @self: A #CyberKitDOMHTMLElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_element_get_inner_text(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_set_inner_text:
 * @self: A #CyberKitDOMHTMLElement
 * @value: A #gchar
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_element_set_inner_text(CyberKitDOMHTMLElement* self, const gchar* value, GError** error);

/**
 * webkit_dom_html_element_get_outer_text:
 * @self: A #CyberKitDOMHTMLElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_element_get_outer_text(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_set_outer_text:
 * @self: A #CyberKitDOMHTMLElement
 * @value: A #gchar
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_element_set_outer_text(CyberKitDOMHTMLElement* self, const gchar* value, GError** error);

/**
 * webkit_dom_html_element_get_content_editable:
 * @self: A #CyberKitDOMHTMLElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_element_get_content_editable(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_set_content_editable:
 * @self: A #CyberKitDOMHTMLElement
 * @value: A #gchar
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_element_set_content_editable(CyberKitDOMHTMLElement* self, const gchar* value, GError** error);

/**
 * webkit_dom_html_element_get_is_content_editable:
 * @self: A #CyberKitDOMHTMLElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_element_get_is_content_editable(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_get_translate:
 * @self: A #CyberKitDOMHTMLElement
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_element_get_translate(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_set_translate:
 * @self: A #CyberKitDOMHTMLElement
 * @value: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_element_set_translate(CyberKitDOMHTMLElement* self, gboolean value);

/**
 * webkit_dom_html_element_get_draggable:
 * @self: A #CyberKitDOMHTMLElement
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_element_get_draggable(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_set_draggable:
 * @self: A #CyberKitDOMHTMLElement
 * @value: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_element_set_draggable(CyberKitDOMHTMLElement* self, gboolean value);

/**
 * webkit_dom_html_element_get_webkitdropzone:
 * @self: A #CyberKitDOMHTMLElement
 *
 * Returns: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_element_get_webkitdropzone(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_set_webkitdropzone:
 * @self: A #CyberKitDOMHTMLElement
 * @value: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_element_set_webkitdropzone(CyberKitDOMHTMLElement* self, const gchar* value);

/**
 * webkit_dom_html_element_get_hidden:
 * @self: A #CyberKitDOMHTMLElement
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_element_get_hidden(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_set_hidden:
 * @self: A #CyberKitDOMHTMLElement
 * @value: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_element_set_hidden(CyberKitDOMHTMLElement* self, gboolean value);

/**
 * webkit_dom_html_element_get_spellcheck:
 * @self: A #CyberKitDOMHTMLElement
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_element_get_spellcheck(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_set_spellcheck:
 * @self: A #CyberKitDOMHTMLElement
 * @value: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_element_set_spellcheck(CyberKitDOMHTMLElement* self, gboolean value);

G_END_DECLS

#endif /* CyberKitDOMHTMLElement_h */
