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

#ifndef CyberKitDOMHTMLInputElement_h
#define CyberKitDOMHTMLInputElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_INPUT_ELEMENT            (webkit_dom_html_input_element_get_type())
#define WEBKIT_DOM_HTML_INPUT_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_INPUT_ELEMENT, CyberKitDOMHTMLInputElement))
#define WEBKIT_DOM_HTML_INPUT_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_INPUT_ELEMENT, CyberKitDOMHTMLInputElementClass)
#define WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_INPUT_ELEMENT))
#define WEBKIT_DOM_IS_HTML_INPUT_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_INPUT_ELEMENT))
#define WEBKIT_DOM_HTML_INPUT_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_INPUT_ELEMENT, CyberKitDOMHTMLInputElementClass))

struct _CyberKitDOMHTMLInputElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLInputElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_input_element_get_type(void);

/**
 * webkit_dom_html_input_element_select:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_select(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_get_accept:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_input_element_get_accept(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_accept:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_accept(CyberKitDOMHTMLInputElement* self, const gchar* value);

/**
 * webkit_dom_html_input_element_get_alt:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_input_element_get_alt(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_alt:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_alt(CyberKitDOMHTMLInputElement* self, const gchar* value);

/**
 * webkit_dom_html_input_element_get_autofocus:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_input_element_get_autofocus(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_autofocus:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_autofocus(CyberKitDOMHTMLInputElement* self, gboolean value);

/**
 * webkit_dom_html_input_element_get_default_checked:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_input_element_get_default_checked(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_default_checked:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_default_checked(CyberKitDOMHTMLInputElement* self, gboolean value);

/**
 * webkit_dom_html_input_element_get_checked:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_input_element_get_checked(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_checked:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_checked(CyberKitDOMHTMLInputElement* self, gboolean value);

/**
 * webkit_dom_html_input_element_get_disabled:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_input_element_get_disabled(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_disabled:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_disabled(CyberKitDOMHTMLInputElement* self, gboolean value);

/**
 * webkit_dom_html_input_element_get_form:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLFormElement
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLFormElement*
webkit_dom_html_input_element_get_form(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_get_files:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: (transfer full): A #CyberKitDOMFileList
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMFileList*
webkit_dom_html_input_element_get_files(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_files:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #CyberKitDOMFileList
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_files(CyberKitDOMHTMLInputElement* self, CyberKitDOMFileList* value);

/**
 * webkit_dom_html_input_element_get_height:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gulong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gulong
webkit_dom_html_input_element_get_height(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_height:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gulong
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_height(CyberKitDOMHTMLInputElement* self, gulong value);

/**
 * webkit_dom_html_input_element_get_indeterminate:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_input_element_get_indeterminate(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_indeterminate:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_indeterminate(CyberKitDOMHTMLInputElement* self, gboolean value);

/**
 * webkit_dom_html_input_element_get_max_length:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_input_element_get_max_length(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_max_length:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #glong
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_max_length(CyberKitDOMHTMLInputElement* self, glong value, GError** error);

/**
 * webkit_dom_html_input_element_get_multiple:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_input_element_get_multiple(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_multiple:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_multiple(CyberKitDOMHTMLInputElement* self, gboolean value);

/**
 * webkit_dom_html_input_element_get_name:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_input_element_get_name(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_name:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_name(CyberKitDOMHTMLInputElement* self, const gchar* value);

/**
 * webkit_dom_html_input_element_get_read_only:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_input_element_get_read_only(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_read_only:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_read_only(CyberKitDOMHTMLInputElement* self, gboolean value);

/**
 * webkit_dom_html_input_element_get_size:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gulong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gulong
webkit_dom_html_input_element_get_size(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_size:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gulong
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_size(CyberKitDOMHTMLInputElement* self, gulong value, GError** error);

/**
 * webkit_dom_html_input_element_get_src:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_input_element_get_src(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_src:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_src(CyberKitDOMHTMLInputElement* self, const gchar* value);

/**
 * webkit_dom_html_input_element_get_input_type:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_input_element_get_input_type(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_input_type:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_input_type(CyberKitDOMHTMLInputElement* self, const gchar* value);

/**
 * webkit_dom_html_input_element_get_default_value:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_input_element_get_default_value(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_default_value:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_default_value(CyberKitDOMHTMLInputElement* self, const gchar* value);

/**
 * webkit_dom_html_input_element_get_value:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_input_element_get_value(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_value:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_value(CyberKitDOMHTMLInputElement* self, const gchar* value);

/**
 * webkit_dom_html_input_element_get_width:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gulong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gulong
webkit_dom_html_input_element_get_width(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_width:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gulong
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_width(CyberKitDOMHTMLInputElement* self, gulong value);

/**
 * webkit_dom_html_input_element_get_will_validate:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_input_element_get_will_validate(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_get_align:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_input_element_get_align(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_align:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_align(CyberKitDOMHTMLInputElement* self, const gchar* value);

/**
 * webkit_dom_html_input_element_get_use_map:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_input_element_get_use_map(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_use_map:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_use_map(CyberKitDOMHTMLInputElement* self, const gchar* value);

/**
 * webkit_dom_html_input_element_get_capture_type:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gchar
 *
 * Since: 2.14
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_input_element_get_capture_type(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_capture_type:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_input_element_set_capture_type(CyberKitDOMHTMLInputElement* self, const gchar* value);

/**
 * webkit_dom_html_input_element_is_edited:
 * @input: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use webkit_dom_element_html_input_element_is_user_edited() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_element_html_input_element_is_user_edited) gboolean
webkit_dom_html_input_element_is_edited(CyberKitDOMHTMLInputElement* input);

/**
 * webkit_dom_html_input_element_get_auto_filled:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use webkit_dom_element_html_input_element_get_auto_filled() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_element_html_input_element_get_auto_filled) gboolean
webkit_dom_html_input_element_get_auto_filled(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_input_element_set_auto_filled:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use webkit_dom_element_html_input_element_set_auto_filled() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_element_html_input_element_set_auto_filled) void
webkit_dom_html_input_element_set_auto_filled(CyberKitDOMHTMLInputElement* self, gboolean value);

/**
 * webkit_dom_html_input_element_set_editing_value:
 * @self: A #CyberKitDOMHTMLInputElement
 * @value: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use webkit_dom_element_html_input_element_set_editing_value() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_element_html_input_element_set_editing_value) void
webkit_dom_html_input_element_set_editing_value(CyberKitDOMHTMLInputElement* self, const gchar* value);

G_END_DECLS

#endif /* CyberKitDOMHTMLInputElement_h */
