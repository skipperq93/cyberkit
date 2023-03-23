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

#ifndef CyberKitDOMHTMLObjectElement_h
#define CyberKitDOMHTMLObjectElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_OBJECT_ELEMENT            (webkit_dom_html_object_element_get_type())
#define WEBKIT_DOM_HTML_OBJECT_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_OBJECT_ELEMENT, CyberKitDOMHTMLObjectElement))
#define WEBKIT_DOM_HTML_OBJECT_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_OBJECT_ELEMENT, CyberKitDOMHTMLObjectElementClass)
#define WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_OBJECT_ELEMENT))
#define WEBKIT_DOM_IS_HTML_OBJECT_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_OBJECT_ELEMENT))
#define WEBKIT_DOM_HTML_OBJECT_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_OBJECT_ELEMENT, CyberKitDOMHTMLObjectElementClass))

struct _CyberKitDOMHTMLObjectElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLObjectElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_object_element_get_type(void);

/**
 * webkit_dom_html_object_element_get_form:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLFormElement
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLFormElement*
webkit_dom_html_object_element_get_form(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_get_code:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_object_element_get_code(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_code:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_code(CyberKitDOMHTMLObjectElement* self, const gchar* value);

/**
 * webkit_dom_html_object_element_get_align:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_object_element_get_align(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_align:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_align(CyberKitDOMHTMLObjectElement* self, const gchar* value);

/**
 * webkit_dom_html_object_element_get_archive:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_object_element_get_archive(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_archive:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_archive(CyberKitDOMHTMLObjectElement* self, const gchar* value);

/**
 * webkit_dom_html_object_element_get_border:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_object_element_get_border(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_border:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_border(CyberKitDOMHTMLObjectElement* self, const gchar* value);

/**
 * webkit_dom_html_object_element_get_code_base:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_object_element_get_code_base(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_code_base:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_code_base(CyberKitDOMHTMLObjectElement* self, const gchar* value);

/**
 * webkit_dom_html_object_element_get_code_type:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_object_element_get_code_type(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_code_type:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_code_type(CyberKitDOMHTMLObjectElement* self, const gchar* value);

/**
 * webkit_dom_html_object_element_get_data:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_object_element_get_data(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_data:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_data(CyberKitDOMHTMLObjectElement* self, const gchar* value);

/**
 * webkit_dom_html_object_element_get_declare:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_object_element_get_declare(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_declare:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_declare(CyberKitDOMHTMLObjectElement* self, gboolean value);

/**
 * webkit_dom_html_object_element_get_height:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_object_element_get_height(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_height:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_height(CyberKitDOMHTMLObjectElement* self, const gchar* value);

/**
 * webkit_dom_html_object_element_get_hspace:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_object_element_get_hspace(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_hspace:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_hspace(CyberKitDOMHTMLObjectElement* self, glong value);

/**
 * webkit_dom_html_object_element_get_name:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_object_element_get_name(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_name:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_name(CyberKitDOMHTMLObjectElement* self, const gchar* value);

/**
 * webkit_dom_html_object_element_get_standby:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_object_element_get_standby(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_standby:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_standby(CyberKitDOMHTMLObjectElement* self, const gchar* value);

/**
 * webkit_dom_html_object_element_get_type_attr:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_object_element_get_type_attr(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_type_attr:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_type_attr(CyberKitDOMHTMLObjectElement* self, const gchar* value);

/**
 * webkit_dom_html_object_element_get_use_map:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_object_element_get_use_map(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_use_map:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_use_map(CyberKitDOMHTMLObjectElement* self, const gchar* value);

/**
 * webkit_dom_html_object_element_get_vspace:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #glong
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED glong
webkit_dom_html_object_element_get_vspace(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_vspace:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #glong
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_vspace(CyberKitDOMHTMLObjectElement* self, glong value);

/**
 * webkit_dom_html_object_element_get_width:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_object_element_get_width(CyberKitDOMHTMLObjectElement* self);

/**
 * webkit_dom_html_object_element_set_width:
 * @self: A #CyberKitDOMHTMLObjectElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_object_element_set_width(CyberKitDOMHTMLObjectElement* self, const gchar* value);

/**
 * webkit_dom_html_object_element_get_content_document:
 * @self: A #CyberKitDOMHTMLObjectElement
 *
 * Returns: (transfer none): A #CyberKitDOMDocument
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMDocument*
webkit_dom_html_object_element_get_content_document(CyberKitDOMHTMLObjectElement* self);

G_END_DECLS

#endif /* CyberKitDOMHTMLObjectElement_h */
