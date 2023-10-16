/*
 *  Copyright (C) 2014 Igalia S.L.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef CyberKitDOMDeprecated_h
#define CyberKitDOMDeprecated_h

#if !defined(WEBKIT_DISABLE_DEPRECATED)

#include <glib.h>
#include <webkitdom/CyberKitDOMNode.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

/**
 * webkit_dom_html_element_get_inner_html:
 * @self: a #CyberKitDOMHTMLElement
 *
 * Returns: a #gchar
 *
 * Deprecated: 2.8: Use webkit_dom_element_get_inner_html() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_element_get_inner_html) gchar*
webkit_dom_html_element_get_inner_html(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_set_inner_html:
 * @self: a #CyberKitDOMHTMLElement
 * @contents: a #gchar with contents to set
 * @error: a #GError or %NULL
 *
 * Deprecated: 2.8: Use webkit_dom_element_set_inner_html() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_element_set_inner_html) void
webkit_dom_html_element_set_inner_html(CyberKitDOMHTMLElement* self, const gchar* contents, GError** error);

/**
 * webkit_dom_html_element_get_outer_html:
 * @self: a #CyberKitDOMHTMLElement
 *
 * Returns: a #gchar
 *
 * Deprecated: 2.8: Use webkit_dom_element_get_outer_html() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_element_get_outer_html) gchar*
webkit_dom_html_element_get_outer_html(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_html_element_set_outer_html:
 * @self: a #CyberKitDOMHTMLElement
 * @contents: a #gchar with contents to set
 * @error: a #GError or %NULL
 *
 * Deprecated: 2.8: Use webkit_dom_element_set_outer_html() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_element_set_outer_html) void
webkit_dom_html_element_set_outer_html(CyberKitDOMHTMLElement* self, const gchar* contents, GError** error);

/**
 * webkit_dom_html_element_get_children:
 * @self: A #CyberKitDOMHTMLElement
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Deprecated: 2.10: Use webkit_dom_element_get_children() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_element_get_children) CyberKitDOMHTMLCollection*
webkit_dom_html_element_get_children(CyberKitDOMHTMLElement* self);

/**
 * webkit_dom_document_get_elements_by_tag_name:
 * @self: A #CyberKitDOMDocument
 * @tag_name: a #gchar with the tag name
 *
 * Returns: (transfer full): a #CyberKitDOMNodeList
 *
 * Deprecated: 2.12: Use webkit_dom_document_get_elements_by_tag_name_as_html_collection() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_document_get_elements_by_tag_name_as_html_collection) CyberKitDOMNodeList*
webkit_dom_document_get_elements_by_tag_name(CyberKitDOMDocument* self, const gchar* tag_name);

/**
 * webkit_dom_document_get_elements_by_tag_name_ns:
 * @self: A #CyberKitDOMDocument
 * @namespace_uri: a #gchar with the namespace URI
 * @tag_name: a #gchar with the tag name
 *
 * Returns: (transfer full): a #CyberKitDOMNodeList
 *
 * Deprecated: 2.12: Use webkit_dom_document_get_elements_by_tag_name_ns_as_html_collection() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_document_get_elements_by_tag_name_as_html_collection) CyberKitDOMNodeList*
webkit_dom_document_get_elements_by_tag_name_ns(CyberKitDOMDocument* self, const gchar* namespace_uri, const gchar* tag_name);


/**
 * webkit_dom_document_get_elements_by_class_name:
 * @self: A #CyberKitDOMDocument
 * @class_name: a #gchar with the tag name
 *
 * Returns: (transfer full): a #CyberKitDOMNodeList
 *
 * Deprecated: 2.12: Use webkit_dom_document_get_elements_by_class_name_as_html_collection() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_document_get_elements_by_class_name_as_html_collection) CyberKitDOMNodeList*
webkit_dom_document_get_elements_by_class_name(CyberKitDOMDocument* self, const gchar* class_name);

/**
 * webkit_dom_element_get_elements_by_tag_name:
 * @self: A #CyberKitDOMElement
 * @tag_name: a #gchar with the tag name
 *
 * Returns: (transfer full): a #CyberKitDOMNodeList
 *
 * Deprecated: 2.12: Use webkit_dom_element_get_elements_by_tag_name_as_html_collection() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_element_get_elements_by_tag_name_as_html_collection) CyberKitDOMNodeList*
webkit_dom_element_get_elements_by_tag_name(CyberKitDOMElement* self, const gchar* tag_name);

/**
 * webkit_dom_element_get_elements_by_tag_name_ns:
 * @self: A #CyberKitDOMElement
 * @namespace_uri: a #gchar with the namespace URI
 * @tag_name: a #gchar with the tag name
 *
 * Returns: (transfer full): a #CyberKitDOMNodeList
 *
 * Deprecated: 2.12: Use webkit_dom_element_get_elements_by_tag_name_ns_as_html_collection() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_element_get_elements_by_tag_name_as_html_collection) CyberKitDOMNodeList*
webkit_dom_element_get_elements_by_tag_name_ns(CyberKitDOMElement* self, const gchar* namespace_uri, const gchar* tag_name);


/**
 * webkit_dom_element_get_elements_by_class_name:
 * @self: A #CyberKitDOMElement
 * @class_name: a #gchar with the tag name
 *
 * Returns: (transfer full): a #CyberKitDOMNodeList
 *
 * Deprecated: 2.12: Use webkit_dom_element_get_elements_by_class_name_as_html_collection() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_element_get_elements_by_class_name_as_html_collection) CyberKitDOMNodeList*
webkit_dom_element_get_elements_by_class_name(CyberKitDOMElement* self, const gchar* class_name);

/**
 * webkit_dom_node_clone_node:
 * @self: A #CyberKitDOMNode
 * @deep: A #gboolean
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.14: Use webkit_dom_node_clone_node_with_error() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_node_clone_node_with_error) CyberKitDOMNode*
webkit_dom_node_clone_node(CyberKitDOMNode* self, gboolean deep, GError** error);


/**
 * webkit_dom_document_get_default_charset:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.14
 */
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_default_charset(CyberKitDOMDocument* self);

/**
 * webkit_dom_text_replace_whole_text:
 * @self: A #CyberKitDOMText
 * @content: A #gchar
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMText
 *
 * Deprecated: 2.14
 */
WEBKIT_DEPRECATED CyberKitDOMText*
webkit_dom_text_replace_whole_text(CyberKitDOMText* self, const gchar* content, GError** error);

/**
 * webkit_dom_html_input_element_get_capture:
 * @self: A #CyberKitDOMHTMLInputElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.14: Use webkit_dom_html_input_element_get_capture_type() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_html_input_element_get_capture_type) gboolean
webkit_dom_html_input_element_get_capture(CyberKitDOMHTMLInputElement* self);

/**
 * webkit_dom_html_document_get_design_mode:
 * @self: A #CyberKitDOMHTMLDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.14: Use webkit_dom_document_get_design_mode() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_document_get_design_mode) gchar*
webkit_dom_html_document_get_design_mode(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_set_design_mode:
 * @self: A #CyberKitDOMHTMLDocument
 * @value: A #gchar
 *
 * Deprecated: 2.14: Use webkit_dom_document_set_design_mode() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_document_set_design_mode) void
webkit_dom_html_document_set_design_mode(CyberKitDOMHTMLDocument* self, const gchar* value);

/**
 * webkit_dom_html_document_get_compat_mode:
 * @self: A #CyberKitDOMHTMLDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.14: Use webkit_dom_document_get_compat_mode() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_document_get_compat_mode) gchar*
webkit_dom_html_document_get_compat_mode(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_get_embeds:
 * @self: A #CyberKitDOMHTMLDocument
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Deprecated: 2.14: Use webkit_dom_document_get_embeds() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_document_get_embeds) CyberKitDOMHTMLCollection*
webkit_dom_html_document_get_embeds(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_get_plugins:
 * @self: A #CyberKitDOMHTMLDocument
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Deprecated: 2.14: Use webkit_dom_document_get_plugins() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_document_get_plugins) CyberKitDOMHTMLCollection*
webkit_dom_html_document_get_plugins(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_html_document_get_scripts:
 * @self: A #CyberKitDOMHTMLDocument
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Deprecated: 2.14: Use webkit_dom_document_get_scripts() instead.
 */
WEBKIT_DEPRECATED_FOR(webkit_dom_document_get_scripts) CyberKitDOMHTMLCollection*
webkit_dom_html_document_get_scripts(CyberKitDOMHTMLDocument* self);

/**
 * webkit_dom_node_get_namespace_uri:
 * @self: A #CyberKitDOMNode
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.14: Use webkit_dom_attr_get_namespace_uri() or webkit_dom_element_get_namespace_uri() instead.
 */
WEBKIT_DEPRECATED gchar*
webkit_dom_node_get_namespace_uri(CyberKitDOMNode* self);

/**
 * webkit_dom_node_get_prefix:
 * @self: A #CyberKitDOMNode
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.14: Use webkit_dom_attr_get_prefix() or webkit_dom_element_get_prefix() instead.
 */
WEBKIT_DEPRECATED gchar*
webkit_dom_node_get_prefix(CyberKitDOMNode* self);

/**
 * webkit_dom_node_set_prefix:
 * @self: A #CyberKitDOMNode
 * @value: A #gchar
 * @error: #GError
 *
 * Deprecated: 2.14
 */
WEBKIT_DEPRECATED void
webkit_dom_node_set_prefix(CyberKitDOMNode* self, const gchar* value, GError** error);

/**
 * webkit_dom_node_get_local_name:
 * @self: A #CyberKitDOMNode
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.14: Use webkit_dom_attr_get_local_name() or webkit_dom_element_get_local_name() instead.
 */
WEBKIT_DEPRECATED gchar*
webkit_dom_node_get_local_name(CyberKitDOMNode* self);

#define WEBKIT_DOM_TYPE_ENTITY_REFERENCE            (webkit_dom_entity_reference_get_type())
#define WEBKIT_DOM_ENTITY_REFERENCE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_ENTITY_REFERENCE, CyberKitDOMEntityReference))
#define WEBKIT_DOM_ENTITY_REFERENCE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_ENTITY_REFERENCE, CyberKitDOMEntityReferenceClass)
#define WEBKIT_DOM_IS_ENTITY_REFERENCE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_ENTITY_REFERENCE))
#define WEBKIT_DOM_IS_ENTITY_REFERENCE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_ENTITY_REFERENCE))
#define WEBKIT_DOM_ENTITY_REFERENCE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_ENTITY_REFERENCE, CyberKitDOMEntityReferenceClass))

typedef struct _CyberKitDOMEntityReference CyberKitDOMEntityReference;
typedef struct _CyberKitDOMEntityReferenceClass CyberKitDOMEntityReferenceClass;

struct _CyberKitDOMEntityReference {
    CyberKitDOMNode parent_instance;
};

struct _CyberKitDOMEntityReferenceClass {
    CyberKitDOMNodeClass parent_class;
};

WEBKIT_DEPRECATED GType webkit_dom_entity_reference_get_type(void);

/**
 * webkit_dom_node_iterator_get_expand_entity_references:
 * @self: A #CyberKitDOMNodeIterator
 *
 * This function has been removed from the DOM spec and it just returns %FALSE.
 *
 * Returns: A #gboolean                                                                                                                                                                       *
 * Deprecated: 2.12
 */
WEBKIT_DEPRECATED gboolean webkit_dom_node_iterator_get_expand_entity_references(CyberKitDOMNodeIterator* self);

/**
 * webkit_dom_tree_walker_get_expand_entity_references:
 * @self: A #CyberKitDOMTreeWalker
 *
 * This function has been removed from the DOM spec and it just returns %FALSE.
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.12
 */
WEBKIT_DEPRECATED gboolean webkit_dom_tree_walker_get_expand_entity_references(CyberKitDOMTreeWalker* self);

/**
 * webkit_dom_document_create_entity_reference:
 * @self: A #CyberKitDOMDocument
 * @name: (allow-none): A #gchar
 * @error: #GError
 *
 * This function has been removed from the DOM spec and it just returns %NULL.
 *
 * Returns: (transfer none): A #CyberKitDOMEntityReference
 *
 * Deprecated: 2.12
 */
WEBKIT_DEPRECATED CyberKitDOMEntityReference* webkit_dom_document_create_entity_reference(CyberKitDOMDocument* self, const gchar* name, GError** error);

#define WEBKIT_DOM_TYPE_HTML_BASE_FONT_ELEMENT            (webkit_dom_html_base_font_element_get_type())
#define WEBKIT_DOM_HTML_BASE_FONT_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_BASE_FONT_ELEMENT, CyberKitDOMHTMLBaseFontElement))
#define WEBKIT_DOM_HTML_BASE_FONT_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_BASE_FONT_ELEMENT, CyberKitDOMHTMLBaseFontElementClass)
#define WEBKIT_DOM_IS_HTML_BASE_FONT_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_BASE_FONT_ELEMENT))
#define WEBKIT_DOM_IS_HTML_BASE_FONT_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_BASE_FONT_ELEMENT))
#define WEBKIT_DOM_HTML_BASE_FONT_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_BASE_FONT_ELEMENT, CyberKitDOMHTMLBaseFontElementClass))

typedef struct _CyberKitDOMHTMLBaseFontElement CyberKitDOMHTMLBaseFontElement;
typedef struct _CyberKitDOMHTMLBaseFontElementClass CyberKitDOMHTMLBaseFontElementClass;

struct _CyberKitDOMHTMLBaseFontElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLBaseFontElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_base_font_element_get_type(void);

/**
 * webkit_dom_html_base_font_element_get_color:
 * @self: A #CyberKitDOMHTMLBaseFontElement
 *
 * This function has been removed from the DOM spec and it just returns %NULL.
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.12
 */
WEBKIT_DEPRECATED gchar*
webkit_dom_html_base_font_element_get_color(CyberKitDOMHTMLBaseFontElement* self);

/**
 * webkit_dom_html_base_font_element_set_color:
 * @self: A #CyberKitDOMHTMLBaseFontElement
 * @value: A #gchar
 *
 * This function has been removed from the DOM spec and it does nothing.
 *
 * Deprecated: 2.12
 */
WEBKIT_DEPRECATED void
webkit_dom_html_base_font_element_set_color(CyberKitDOMHTMLBaseFontElement* self, const gchar* value);

/**
 * webkit_dom_html_base_font_element_get_face:
 * @self: A #CyberKitDOMHTMLBaseFontElement
 *
 * This function has been removed from the DOM spec and it just returns %NULL.
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.12
 */
WEBKIT_DEPRECATED gchar*
webkit_dom_html_base_font_element_get_face(CyberKitDOMHTMLBaseFontElement* self);

/**
 * webkit_dom_html_base_font_element_set_face:
 * @self: A #CyberKitDOMHTMLBaseFontElement
 * @value: A #gchar
 *
 * This function has been removed from the DOM spec and it does nothing.
 *
 * Deprecated: 2.12
 */
WEBKIT_DEPRECATED void
webkit_dom_html_base_font_element_set_face(CyberKitDOMHTMLBaseFontElement* self, const gchar* value);

/**
 * webkit_dom_html_base_font_element_get_size:
 * @self: A #CyberKitDOMHTMLBaseFontElement
 *
 * This function has been removed from the DOM spec and it just returns 0.
 *
 * Returns: A #glong
 *
 * Deprecated: 2.12
 */
WEBKIT_DEPRECATED glong
webkit_dom_html_base_font_element_get_size(CyberKitDOMHTMLBaseFontElement* self);

/**
 * webkit_dom_html_base_font_element_set_size:
 * @self: A #CyberKitDOMHTMLBaseFontElement
 * @value: A #glong
 *
 * This function has been removed from the DOM spec and it does nothing.
 *
 * Deprecated: 2.12
 */
WEBKIT_DEPRECATED void
webkit_dom_html_base_font_element_set_size(CyberKitDOMHTMLBaseFontElement* self, glong value);

/**
 * webkit_dom_element_get_webkit_region_overset:
 * @self: A #CyberKitDOMElement
 *
 * CSS Regions support has been removed. This function does nothing.
 *
 * Returns: %NULL
 *
 * Deprecated: 2.20
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_element_get_webkit_region_overset(CyberKitDOMElement* self);

G_END_DECLS

#endif /* WEBKIT_DISABLE_DEPRECATED */

#endif
