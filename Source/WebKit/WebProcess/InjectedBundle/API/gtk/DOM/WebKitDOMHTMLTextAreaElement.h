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

#ifndef CyberKitDOMHTMLTextAreaElement_h
#define CyberKitDOMHTMLTextAreaElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_TEXT_AREA_ELEMENT            (webkit_dom_html_text_area_element_get_type())
#define WEBKIT_DOM_HTML_TEXT_AREA_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_TEXT_AREA_ELEMENT, CyberKitDOMHTMLTextAreaElement))
#define WEBKIT_DOM_HTML_TEXT_AREA_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_TEXT_AREA_ELEMENT, CyberKitDOMHTMLTextAreaElementClass)
#define WEBKIT_DOM_IS_HTML_TEXT_AREA_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_TEXT_AREA_ELEMENT))
#define WEBKIT_DOM_IS_HTML_TEXT_AREA_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_TEXT_AREA_ELEMENT))
#define WEBKIT_DOM_HTML_TEXT_AREA_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_TEXT_AREA_ELEMENT, CyberKitDOMHTMLTextAreaElementClass))

struct _CyberKitDOMHTMLTextAreaElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLTextAreaElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_text_area_element_get_type(void);

/**
 * webkit_dom_html_text_area_element_select:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_text_area_element_select(CyberKitDOMHTMLTextAreaElement* self);

/**
 * webkit_dom_html_text_area_element_set_selection_range:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 * @start: A #glong
 * @end: A #glong
 * @direction: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_text_area_element_set_selection_range(CyberKitDOMHTMLTextAreaElement* self, glong start, glong end, const gchar* direction);

/**
 * webkit_dom_html_text_area_element_get_autofocus:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_text_area_element_get_autofocus(CyberKitDOMHTMLTextAreaElement* self);

/**
 * webkit_dom_html_text_area_element_set_autofocus:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_text_area_element_set_autofocus(CyberKitDOMHTMLTextAreaElement* self, gboolean value);

/**
 * webkit_dom_html_text_area_element_get_disabled:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_text_area_element_get_disabled(CyberKitDOMHTMLTextAreaElement* self);

/**
 * webkit_dom_html_text_area_element_set_disabled:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_text_area_element_set_disabled(CyberKitDOMHTMLTextAreaElement* self, gboolean value);

/**
 * webkit_dom_html_text_area_element_get_form:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLFormElement
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLFormElement*
webkit_dom_html_text_area_element_get_form(CyberKitDOMHTMLTextAreaElement* self);

/**
 * webkit_dom_html_text_area_element_get_name:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_text_area_element_get_name(CyberKitDOMHTMLTextAreaElement* self);

/**
 * webkit_dom_html_text_area_element_set_name:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_text_area_element_set_name(CyberKitDOMHTMLTextAreaElement* self, const gchar* value);

/**
 * webkit_dom_html_text_area_element_get_read_only:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_text_area_element_get_read_only(CyberKitDOMHTMLTextAreaElement* self);

/**
 * webkit_dom_html_text_area_element_set_read_only:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_text_area_element_set_read_only(CyberKitDOMHTMLTextAreaElement* self, gboolean value);

/**
 * webkit_dom_html_text_area_element_get_rows:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_text_area_element_get_rows(CyberKitDOMHTMLTextAreaElement* self);

/**
 * webkit_dom_html_text_area_element_set_rows:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_text_area_element_set_rows(CyberKitDOMHTMLTextAreaElement* self, glong value);

/**
 * webkit_dom_html_text_area_element_get_cols:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_text_area_element_get_cols(CyberKitDOMHTMLTextAreaElement* self);

/**
 * webkit_dom_html_text_area_element_set_cols:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_text_area_element_set_cols(CyberKitDOMHTMLTextAreaElement* self, glong value);

/**
 * webkit_dom_html_text_area_element_get_area_type:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_text_area_element_get_area_type(CyberKitDOMHTMLTextAreaElement* self);

/**
 * webkit_dom_html_text_area_element_get_default_value:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_text_area_element_get_default_value(CyberKitDOMHTMLTextAreaElement* self);

/**
 * webkit_dom_html_text_area_element_set_default_value:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_text_area_element_set_default_value(CyberKitDOMHTMLTextAreaElement* self, const gchar* value);

/**
 * webkit_dom_html_text_area_element_get_value:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_text_area_element_get_value(CyberKitDOMHTMLTextAreaElement* self);

/**
 * webkit_dom_html_text_area_element_set_value:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_text_area_element_set_value(CyberKitDOMHTMLTextAreaElement* self, const gchar* value);

/**
 * webkit_dom_html_text_area_element_get_will_validate:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_text_area_element_get_will_validate(CyberKitDOMHTMLTextAreaElement* self);

/**
 * webkit_dom_html_text_area_element_get_selection_start:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_text_area_element_get_selection_start(CyberKitDOMHTMLTextAreaElement* self);

/**
 * webkit_dom_html_text_area_element_set_selection_start:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_text_area_element_set_selection_start(CyberKitDOMHTMLTextAreaElement* self, glong value);

/**
 * webkit_dom_html_text_area_element_get_selection_end:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_text_area_element_get_selection_end(CyberKitDOMHTMLTextAreaElement* self);

/**
 * webkit_dom_html_text_area_element_set_selection_end:
 * @self: A #CyberKitDOMHTMLTextAreaElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_text_area_element_set_selection_end(CyberKitDOMHTMLTextAreaElement* self, glong value);

/**
 * webkit_dom_html_text_area_element_is_edited:
 * @input: A #CyberKitDOMHTMLTextAreaElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
 */
WEBKIT_DEPRECATED gboolean webkit_dom_html_text_area_element_is_edited(CyberKitDOMHTMLTextAreaElement* input);

G_END_DECLS

#endif /* CyberKitDOMHTMLTextAreaElement_h */
