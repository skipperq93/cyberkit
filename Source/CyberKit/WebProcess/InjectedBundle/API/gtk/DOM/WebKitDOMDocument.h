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

#ifndef CyberKitDOMDocument_h
#define CyberKitDOMDocument_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMNode.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_DOCUMENT            (webkit_dom_document_get_type())
#define WEBKIT_DOM_DOCUMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_DOCUMENT, CyberKitDOMDocument))
#define WEBKIT_DOM_DOCUMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_DOCUMENT, CyberKitDOMDocumentClass)
#define WEBKIT_DOM_IS_DOCUMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_DOCUMENT))
#define WEBKIT_DOM_IS_DOCUMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_DOCUMENT))
#define WEBKIT_DOM_DOCUMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_DOCUMENT, CyberKitDOMDocumentClass))

struct _CyberKitDOMDocument {
    CyberKitDOMNode parent_instance;
};

struct _CyberKitDOMDocumentClass {
    CyberKitDOMNodeClass parent_class;
};

WEBKIT_API GType
webkit_dom_document_get_type(void);

/**
 * webkit_dom_document_create_element:
 * @self: A #CyberKitDOMDocument
 * @tagName: A #gchar
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_document_create_element(CyberKitDOMDocument* self, const gchar* tagName, GError** error);

/**
 * webkit_dom_document_create_document_fragment:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer none): A #CyberKitDOMDocumentFragment
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMDocumentFragment*
webkit_dom_document_create_document_fragment(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_create_text_node:
 * @self: A #CyberKitDOMDocument
 * @data: A #gchar
 *
 * Returns: (transfer none): A #CyberKitDOMText
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMText*
webkit_dom_document_create_text_node(CyberKitDOMDocument* self, const gchar* data);

/**
 * webkit_dom_document_create_comment:
 * @self: A #CyberKitDOMDocument
 * @data: A #gchar
 *
 * Returns: (transfer none): A #CyberKitDOMComment
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMComment*
webkit_dom_document_create_comment(CyberKitDOMDocument* self, const gchar* data);

/**
 * webkit_dom_document_create_cdata_section:
 * @self: A #CyberKitDOMDocument
 * @data: A #gchar
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMCDATASection
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMCDATASection*
webkit_dom_document_create_cdata_section(CyberKitDOMDocument* self, const gchar* data, GError** error);

/**
 * webkit_dom_document_create_processing_instruction:
 * @self: A #CyberKitDOMDocument
 * @target: A #gchar
 * @data: A #gchar
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMProcessingInstruction
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMProcessingInstruction*
webkit_dom_document_create_processing_instruction(CyberKitDOMDocument* self, const gchar* target, const gchar* data, GError** error);

/**
 * webkit_dom_document_create_attribute:
 * @self: A #CyberKitDOMDocument
 * @name: A #gchar
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMAttr
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMAttr*
webkit_dom_document_create_attribute(CyberKitDOMDocument* self, const gchar* name, GError** error);

/**
 * webkit_dom_document_get_elements_by_tag_name_as_html_collection:
 * @self: A #CyberKitDOMDocument
 * @tagname: A #gchar
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Since: 2.12
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_document_get_elements_by_tag_name_as_html_collection(CyberKitDOMDocument* self, const gchar* tagname);

/**
 * webkit_dom_document_import_node:
 * @self: A #CyberKitDOMDocument
 * @importedNode: A #CyberKitDOMNode
 * @deep: A #gboolean
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_document_import_node(CyberKitDOMDocument* self, CyberKitDOMNode* importedNode, gboolean deep, GError** error);

/**
 * webkit_dom_document_create_element_ns:
 * @self: A #CyberKitDOMDocument
 * @namespaceURI: (allow-none): A #gchar
 * @qualifiedName: A #gchar
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_document_create_element_ns(CyberKitDOMDocument* self, const gchar* namespaceURI, const gchar* qualifiedName, GError** error);

/**
 * webkit_dom_document_create_attribute_ns:
 * @self: A #CyberKitDOMDocument
 * @namespaceURI: (allow-none): A #gchar
 * @qualifiedName: A #gchar
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMAttr
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMAttr*
webkit_dom_document_create_attribute_ns(CyberKitDOMDocument* self, const gchar* namespaceURI, const gchar* qualifiedName, GError** error);

/**
 * webkit_dom_document_get_elements_by_tag_name_ns_as_html_collection:
 * @self: A #CyberKitDOMDocument
 * @namespaceURI: A #gchar
 * @localName: A #gchar
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Since: 2.12
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_document_get_elements_by_tag_name_ns_as_html_collection(CyberKitDOMDocument* self, const gchar* namespaceURI, const gchar* localName);

/**
 * webkit_dom_document_adopt_node:
 * @self: A #CyberKitDOMDocument
 * @source: A #CyberKitDOMNode
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMNode
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNode*
webkit_dom_document_adopt_node(CyberKitDOMDocument* self, CyberKitDOMNode* source, GError** error);

/**
 * webkit_dom_document_create_event:
 * @self: A #CyberKitDOMDocument
 * @eventType: A #gchar
 * @error: #GError
 *
 * Returns: (transfer full): A #CyberKitDOMEvent
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMEvent*
webkit_dom_document_create_event(CyberKitDOMDocument* self, const gchar* eventType, GError** error);

/**
 * webkit_dom_document_create_range:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer full): A #CyberKitDOMRange
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMRange*
webkit_dom_document_create_range(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_create_node_iterator:
 * @self: A #CyberKitDOMDocument
 * @root: A #CyberKitDOMNode
 * @whatToShow: A #gulong
 * @filter: (allow-none): A #CyberKitDOMNodeFilter
 * @expandEntityReferences: A #gboolean
 * @error: #GError
 *
 * Returns: (transfer full): A #CyberKitDOMNodeIterator
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNodeIterator*
webkit_dom_document_create_node_iterator(CyberKitDOMDocument* self, CyberKitDOMNode* root, gulong whatToShow, CyberKitDOMNodeFilter* filter, gboolean expandEntityReferences, GError** error);

/**
 * webkit_dom_document_create_tree_walker:
 * @self: A #CyberKitDOMDocument
 * @root: A #CyberKitDOMNode
 * @whatToShow: A #gulong
 * @filter: (allow-none): A #CyberKitDOMNodeFilter
 * @expandEntityReferences: A #gboolean
 * @error: #GError
 *
 * Returns: (transfer full): A #CyberKitDOMTreeWalker
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMTreeWalker*
webkit_dom_document_create_tree_walker(CyberKitDOMDocument* self, CyberKitDOMNode* root, gulong whatToShow, CyberKitDOMNodeFilter* filter, gboolean expandEntityReferences, GError** error);

/**
 * webkit_dom_document_get_override_style:
 * @self: A #CyberKitDOMDocument
 * @element: A #CyberKitDOMElement
 * @pseudoElement: (allow-none): A #gchar
 *
 * Returns: (transfer full): A #CyberKitDOMCSSStyleDeclaration
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMCSSStyleDeclaration*
webkit_dom_document_get_override_style(CyberKitDOMDocument* self, CyberKitDOMElement* element, const gchar* pseudoElement);

/**
 * webkit_dom_document_create_expression:
 * @self: A #CyberKitDOMDocument
 * @expression: A #gchar
 * @resolver: A #CyberKitDOMXPathNSResolver
 * @error: #GError
 *
 * Returns: (transfer full): A #CyberKitDOMXPathExpression
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMXPathExpression*
webkit_dom_document_create_expression(CyberKitDOMDocument* self, const gchar* expression, CyberKitDOMXPathNSResolver* resolver, GError** error);

/**
 * webkit_dom_document_create_ns_resolver:
 * @self: A #CyberKitDOMDocument
 * @nodeResolver: A #CyberKitDOMNode
 *
 * Returns: (transfer full): A #CyberKitDOMXPathNSResolver
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMXPathNSResolver*
webkit_dom_document_create_ns_resolver(CyberKitDOMDocument* self, CyberKitDOMNode* nodeResolver);

/**
 * webkit_dom_document_evaluate:
 * @self: A #CyberKitDOMDocument
 * @expression: A #gchar
 * @contextNode: A #CyberKitDOMNode
 * @resolver: (allow-none): A #CyberKitDOMXPathNSResolver
 * @type: A #gushort
 * @inResult: (allow-none): A #CyberKitDOMXPathResult
 * @error: #GError
 *
 * Returns: (transfer full): A #CyberKitDOMXPathResult
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMXPathResult*
webkit_dom_document_evaluate(CyberKitDOMDocument* self, const gchar* expression, CyberKitDOMNode* contextNode, CyberKitDOMXPathNSResolver* resolver, gushort type, CyberKitDOMXPathResult* inResult, GError** error);

/**
 * webkit_dom_document_exec_command:
 * @self: A #CyberKitDOMDocument
 * @command: A #gchar
 * @userInterface: A #gboolean
 * @value: A #gchar
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_document_exec_command(CyberKitDOMDocument* self, const gchar* command, gboolean userInterface, const gchar* value);

/**
 * webkit_dom_document_query_command_enabled:
 * @self: A #CyberKitDOMDocument
 * @command: A #gchar
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_document_query_command_enabled(CyberKitDOMDocument* self, const gchar* command);

/**
 * webkit_dom_document_query_command_indeterm:
 * @self: A #CyberKitDOMDocument
 * @command: A #gchar
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_document_query_command_indeterm(CyberKitDOMDocument* self, const gchar* command);

/**
 * webkit_dom_document_query_command_state:
 * @self: A #CyberKitDOMDocument
 * @command: A #gchar
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_document_query_command_state(CyberKitDOMDocument* self, const gchar* command);

/**
 * webkit_dom_document_query_command_supported:
 * @self: A #CyberKitDOMDocument
 * @command: A #gchar
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_document_query_command_supported(CyberKitDOMDocument* self, const gchar* command);

/**
 * webkit_dom_document_query_command_value:
 * @self: A #CyberKitDOMDocument
 * @command: A #gchar
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_query_command_value(CyberKitDOMDocument* self, const gchar* command);

/**
 * webkit_dom_document_get_elements_by_name:
 * @self: A #CyberKitDOMDocument
 * @elementName: A #gchar
 *
 * Returns: (transfer full): A #CyberKitDOMNodeList
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNodeList*
webkit_dom_document_get_elements_by_name(CyberKitDOMDocument* self, const gchar* elementName);

/**
 * webkit_dom_document_element_from_point:
 * @self: A #CyberKitDOMDocument
 * @x: A #glong
 * @y: A #glong
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_document_element_from_point(CyberKitDOMDocument* self, glong x, glong y);

/**
 * webkit_dom_document_create_css_style_declaration:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer full): A #CyberKitDOMCSSStyleDeclaration
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMCSSStyleDeclaration*
webkit_dom_document_create_css_style_declaration(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_elements_by_class_name_as_html_collection:
 * @self: A #CyberKitDOMDocument
 * @classNames: A #gchar
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Since: 2.12
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_document_get_elements_by_class_name_as_html_collection(CyberKitDOMDocument* self, const gchar* classNames);

/**
 * webkit_dom_document_has_focus:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_document_has_focus(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_element_by_id:
 * @self: A #CyberKitDOMDocument
 * @elementId: A #gchar
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_document_get_element_by_id(CyberKitDOMDocument* self, const gchar* elementId);

/**
 * webkit_dom_document_query_selector:
 * @self: A #CyberKitDOMDocument
 * @selectors: A #gchar
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_document_query_selector(CyberKitDOMDocument* self, const gchar* selectors, GError** error);

/**
 * webkit_dom_document_query_selector_all:
 * @self: A #CyberKitDOMDocument
 * @selectors: A #gchar
 * @error: #GError
 *
 * Returns: (transfer full): A #CyberKitDOMNodeList
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMNodeList*
webkit_dom_document_query_selector_all(CyberKitDOMDocument* self, const gchar* selectors, GError** error);

/**
 * webkit_dom_document_get_doctype:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer none): A #CyberKitDOMDocumentType
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMDocumentType*
webkit_dom_document_get_doctype(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_implementation:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer full): A #CyberKitDOMDOMImplementation
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMDOMImplementation*
webkit_dom_document_get_implementation(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_document_element:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_document_get_document_element(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_input_encoding:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_input_encoding(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_xml_encoding:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_xml_encoding(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_xml_version:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_xml_version(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_set_xml_version:
 * @self: A #CyberKitDOMDocument
 * @value: A #gchar
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_document_set_xml_version(CyberKitDOMDocument* self, const gchar* value, GError** error);

/**
 * webkit_dom_document_get_xml_standalone:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_document_get_xml_standalone(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_set_xml_standalone:
 * @self: A #CyberKitDOMDocument
 * @value: A #gboolean
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_document_set_xml_standalone(CyberKitDOMDocument* self, gboolean value, GError** error);

/**
 * webkit_dom_document_get_document_uri:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_document_uri(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_set_document_uri:
 * @self: A #CyberKitDOMDocument
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_document_set_document_uri(CyberKitDOMDocument* self, const gchar* value);

/**
 * webkit_dom_document_get_default_view:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer full): A #CyberKitDOMDOMWindow
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMDOMWindow*
webkit_dom_document_get_default_view(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_style_sheets:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer none): A #CyberKitDOMStyleSheetList
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMStyleSheetList*
webkit_dom_document_get_style_sheets(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_title:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_title(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_set_title:
 * @self: A #CyberKitDOMDocument
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_document_set_title(CyberKitDOMDocument* self, const gchar* value);

/**
 * webkit_dom_document_get_design_mode:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Since: 2.14
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_design_mode(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_set_design_mode:
 * @self: A #CyberKitDOMDocument
 * @value: A #gchar
 *
 * Since: 2.14
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_document_set_design_mode(CyberKitDOMDocument* self, const gchar* value);

/**
 * webkit_dom_document_get_referrer:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_referrer(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_domain:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_domain(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_url:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_url(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_cookie:
 * @self: A #CyberKitDOMDocument
 * @error: #GError
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_cookie(CyberKitDOMDocument* self, GError** error);

/**
 * webkit_dom_document_set_cookie:
 * @self: A #CyberKitDOMDocument
 * @value: A #gchar
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_document_set_cookie(CyberKitDOMDocument* self, const gchar* value, GError** error);

/**
 * webkit_dom_document_get_body:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLElement
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLElement*
webkit_dom_document_get_body(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_set_body:
 * @self: A #CyberKitDOMDocument
 * @value: A #CyberKitDOMHTMLElement
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_document_set_body(CyberKitDOMDocument* self, CyberKitDOMHTMLElement* value, GError** error);

/**
 * webkit_dom_document_get_head:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLHeadElement
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLHeadElement*
webkit_dom_document_get_head(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_images:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_document_get_images(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_applets:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_document_get_applets(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_links:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_document_get_links(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_forms:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_document_get_forms(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_anchors:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_document_get_anchors(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_embeds:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Since: 2.14
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_document_get_embeds(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_plugins:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Since: 2.14
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_document_get_plugins(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_scripts:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Since: 2.14
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_document_get_scripts(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_last_modified:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_last_modified(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_charset:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_charset(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_set_charset:
 * @self: A #CyberKitDOMDocument
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_document_set_charset(CyberKitDOMDocument* self, const gchar* value);

/**
 * webkit_dom_document_get_ready_state:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_ready_state(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_character_set:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_character_set(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_preferred_stylesheet_set:
 * @self: A #CyberKitDOMDocument
 *
 * This function has been removed and does nothing.
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_preferred_stylesheet_set(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_selected_stylesheet_set:
 * @self: A #CyberKitDOMDocument
 *
 * This function has been removed and does nothing.
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_selected_stylesheet_set(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_set_selected_stylesheet_set:
 * @self: A #CyberKitDOMDocument
 * @value: A #gchar
 *
 * This function has been removed and does nothing.
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_document_set_selected_stylesheet_set(CyberKitDOMDocument* self, const gchar* value);

/**
 * webkit_dom_document_get_active_element:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_document_get_active_element(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_compat_mode:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Since: 2.14
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_compat_mode(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_caret_range_from_point:
 * @self: A #CyberKitDOMDocument
 * @x: A #glong
 * @y: A #glong
 *
 * Returns: (transfer full): A #CyberKitDOMRange
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMRange*
webkit_dom_document_caret_range_from_point(CyberKitDOMDocument* self, glong x, glong y);

/**
 * webkit_dom_document_webkit_cancel_fullscreen:
 * @self: A #CyberKitDOMDocument
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_document_webkit_cancel_fullscreen(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_webkit_exit_fullscreen:
 * @self: A #CyberKitDOMDocument
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_document_webkit_exit_fullscreen(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_exit_pointer_lock:
 * @self: A #CyberKitDOMDocument
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_document_exit_pointer_lock(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_content_type:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_content_type(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_dir:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_dir(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_set_dir:
 * @self: A #CyberKitDOMDocument
 * @value: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_document_set_dir(CyberKitDOMDocument* self, const gchar* value);

/**
 * webkit_dom_document_get_webkit_is_fullscreen:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_document_get_webkit_is_fullscreen(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_webkit_fullscreen_keyboard_input_allowed:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_document_get_webkit_fullscreen_keyboard_input_allowed(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_webkit_current_fullscreen_element:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_document_get_webkit_current_fullscreen_element(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_webkit_fullscreen_enabled:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_document_get_webkit_fullscreen_enabled(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_webkit_fullscreen_element:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_document_get_webkit_fullscreen_element(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_pointer_lock_element:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_document_get_pointer_lock_element(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_visibility_state:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_visibility_state(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_hidden:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gboolean
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_document_get_hidden(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_current_script:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer none): A #CyberKitDOMHTMLScriptElement
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLScriptElement*
webkit_dom_document_get_current_script(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_origin:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_document_get_origin(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_scrolling_element:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_document_get_scrolling_element(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_children:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer full): A #CyberKitDOMHTMLCollection
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMHTMLCollection*
webkit_dom_document_get_children(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_first_element_child:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_document_get_first_element_child(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_last_element_child:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: (transfer none): A #CyberKitDOMElement
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMElement*
webkit_dom_document_get_last_element_child(CyberKitDOMDocument* self);

/**
 * webkit_dom_document_get_child_element_count:
 * @self: A #CyberKitDOMDocument
 *
 * Returns: A #gulong
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gulong
webkit_dom_document_get_child_element_count(CyberKitDOMDocument* self);

G_END_DECLS

#endif /* CyberKitDOMDocument_h */
