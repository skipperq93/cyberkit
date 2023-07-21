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

#include "config.h"
#include "CyberKitDOMDeprecated.h"

#include "ConvertToUTF8String.h"
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include <CyberCore/Element.h>
#include <CyberCore/JSExecState.h>
#include <CyberCore/HTMLCollection.h>
#include "CyberKitDOMDocumentPrivate.h"
#include "CyberKitDOMElementPrivate.h"
#include "CyberKitDOMHTMLDocumentPrivate.h"
#include "CyberKitDOMHTMLInputElementPrivate.h"
#include "CyberKitDOMHTMLTitleElement.h"
#include "CyberKitDOMNodeListPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "CyberKitDOMTextPrivate.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>
#include <wtf/text/WTFString.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

gchar* webkit_dom_html_element_get_inner_html(CyberKitDOMHTMLElement* self)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_ELEMENT(self), nullptr);
    return webkit_dom_element_get_inner_html(WEBKIT_DOM_ELEMENT(self));
}

void webkit_dom_html_element_set_inner_html(CyberKitDOMHTMLElement* self, const gchar* contents, GError** error)
{
    g_return_if_fail(WEBKIT_DOM_IS_HTML_ELEMENT(self));
    webkit_dom_element_set_inner_html(WEBKIT_DOM_ELEMENT(self), contents, error);
}

gchar* webkit_dom_html_element_get_outer_html(CyberKitDOMHTMLElement* self)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_ELEMENT(self), nullptr);
    return webkit_dom_element_get_outer_html(WEBKIT_DOM_ELEMENT(self));
}

void webkit_dom_html_element_set_outer_html(CyberKitDOMHTMLElement* self, const gchar* contents, GError** error)
{
    g_return_if_fail(WEBKIT_DOM_IS_HTML_ELEMENT(self));
    webkit_dom_element_set_outer_html(WEBKIT_DOM_ELEMENT(self), contents, error);
}

CyberKitDOMHTMLCollection* webkit_dom_html_element_get_children(CyberKitDOMHTMLElement* self)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_ELEMENT(self), nullptr);
    return webkit_dom_element_get_children(WEBKIT_DOM_ELEMENT(self));
}

CyberKitDOMNodeList* webkit_dom_document_get_elements_by_tag_name(CyberKitDOMDocument* self, const gchar* tagName)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), nullptr);
    g_return_val_if_fail(tagName, nullptr);

    CyberCore::JSMainThreadNullState state;
    CyberCore::Document* document = CyberKit::core(self);
    RefPtr<CyberCore::NodeList> nodeList = WTF::getPtr(document->getElementsByTagName(String::fromUTF8(tagName)));
    return CyberKit::kit(nodeList.get());
}

CyberKitDOMNodeList* webkit_dom_document_get_elements_by_tag_name_ns(CyberKitDOMDocument* self, const gchar* namespaceURI, const gchar* tagName)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), nullptr);
    g_return_val_if_fail(namespaceURI, nullptr);
    g_return_val_if_fail(tagName, nullptr);

    CyberCore::JSMainThreadNullState state;
    CyberCore::Document* document = CyberKit::core(self);
    RefPtr<CyberCore::NodeList> nodeList = WTF::getPtr(document->getElementsByTagNameNS(String::fromUTF8(namespaceURI), String::fromUTF8(tagName)));
    return CyberKit::kit(nodeList.get());
}

CyberKitDOMNodeList* webkit_dom_document_get_elements_by_class_name(CyberKitDOMDocument* self, const gchar* className)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), nullptr);
    g_return_val_if_fail(className, nullptr);

    CyberCore::JSMainThreadNullState state;
    CyberCore::Document* document = CyberKit::core(self);
    RefPtr<CyberCore::NodeList> nodeList = WTF::getPtr(document->getElementsByClassName(String::fromUTF8(className)));
    return CyberKit::kit(nodeList.get());
}

CyberKitDOMNodeList* webkit_dom_element_get_elements_by_tag_name(CyberKitDOMElement* self, const gchar* tagName)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_ELEMENT(self), nullptr);
    g_return_val_if_fail(tagName, nullptr);

    CyberCore::JSMainThreadNullState state;
    CyberCore::Element* element = CyberKit::core(self);
    RefPtr<CyberCore::NodeList> nodeList = WTF::getPtr(element->getElementsByTagName(String::fromUTF8(tagName)));
    return CyberKit::kit(nodeList.get());
}

CyberKitDOMNodeList* webkit_dom_element_get_elements_by_tag_name_ns(CyberKitDOMElement* self, const gchar* namespaceURI, const gchar* tagName)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_ELEMENT(self), nullptr);
    g_return_val_if_fail(namespaceURI, nullptr);
    g_return_val_if_fail(tagName, nullptr);

    CyberCore::JSMainThreadNullState state;
    CyberCore::Element* element = CyberKit::core(self);
    RefPtr<CyberCore::NodeList> nodeList = WTF::getPtr(element->getElementsByTagNameNS(String::fromUTF8(namespaceURI), String::fromUTF8(tagName)));
    return CyberKit::kit(nodeList.get());
}

CyberKitDOMNodeList* webkit_dom_element_get_elements_by_class_name(CyberKitDOMElement* self, const gchar* className)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_ELEMENT(self), nullptr);
    g_return_val_if_fail(className, nullptr);

    CyberCore::JSMainThreadNullState state;
    CyberCore::Element* element = CyberKit::core(self);
    RefPtr<CyberCore::NodeList> nodeList = WTF::getPtr(element->getElementsByClassName(String::fromUTF8(className)));
    return CyberKit::kit(nodeList.get());
}

CyberKitDOMNode* webkit_dom_node_clone_node(CyberKitDOMNode* self, gboolean deep)
{
    return webkit_dom_node_clone_node_with_error(self, deep, nullptr);
}

gchar* webkit_dom_document_get_default_charset(CyberKitDOMDocument* self)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), nullptr);
    return convertToUTF8String(CyberKit::core(self)->defaultCharsetForLegacyBindings());
}

CyberKitDOMText* webkit_dom_text_replace_whole_text(CyberKitDOMText* self, const gchar* content, GError** error)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_TEXT(self), nullptr);
    g_return_val_if_fail(content, nullptr);
    g_return_val_if_fail(!error || !*error, nullptr);

    CyberCore::JSMainThreadNullState state;
    return CyberKit::kit(CyberKit::core(self)->replaceWholeText(WTF::String::fromUTF8(content)).get());
}

gboolean webkit_dom_html_input_element_get_capture(CyberKitDOMHTMLInputElement* self)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(self), FALSE);

#if ENABLE(MEDIA_CAPTURE)
    CyberCore::JSMainThreadNullState state;
    CyberCore::HTMLInputElement* item = CyberKit::core(self);
    return item->mediaCaptureType() != CyberCore::MediaCaptureTypeNone;
#else
    UNUSED_PARAM(self);
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Media Capture")
    return FALSE;
#endif /* ENABLE(MEDIA_CAPTURE) */
}

gchar* webkit_dom_html_document_get_design_mode(CyberKitDOMHTMLDocument* self)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_DOCUMENT(self), nullptr);
    return webkit_dom_document_get_design_mode(WEBKIT_DOM_DOCUMENT(self));
}


void webkit_dom_html_document_set_design_mode(CyberKitDOMHTMLDocument* self, const gchar* value)
{
    g_return_if_fail(WEBKIT_DOM_IS_HTML_DOCUMENT(self));
    webkit_dom_document_set_design_mode(WEBKIT_DOM_DOCUMENT(self), value);
}

gchar* webkit_dom_html_document_get_compat_mode(CyberKitDOMHTMLDocument* self)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_DOCUMENT(self), nullptr);
    return webkit_dom_document_get_compat_mode(WEBKIT_DOM_DOCUMENT(self));
}

CyberKitDOMHTMLCollection* webkit_dom_html_document_get_embeds(CyberKitDOMHTMLDocument* self)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_DOCUMENT(self), nullptr);
    return webkit_dom_document_get_embeds(WEBKIT_DOM_DOCUMENT(self));
}

CyberKitDOMHTMLCollection* webkit_dom_html_document_get_plugins(CyberKitDOMHTMLDocument* self)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_DOCUMENT(self), nullptr);
    return webkit_dom_document_get_plugins(WEBKIT_DOM_DOCUMENT(self));
}

CyberKitDOMHTMLCollection* webkit_dom_html_document_get_scripts(CyberKitDOMHTMLDocument* self)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_DOCUMENT(self), nullptr);
    return webkit_dom_document_get_scripts(WEBKIT_DOM_DOCUMENT(self));
}

gchar* webkit_dom_node_get_namespace_uri(CyberKitDOMNode* self)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), nullptr);

    CyberCore::JSMainThreadNullState state;
    return convertToUTF8String(CyberKit::core(self)->namespaceURI());
}

gchar* webkit_dom_node_get_prefix(CyberKitDOMNode* self)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), nullptr);
    CyberCore::JSMainThreadNullState state;
    return convertToUTF8String(CyberKit::core(self)->prefix());
}

void webkit_dom_node_set_prefix(CyberKitDOMNode* self, const gchar* value, GError** error)
{
    g_return_if_fail(WEBKIT_DOM_IS_NODE(self));
    g_return_if_fail(value);
    g_return_if_fail(!error || !*error);

    g_warning("%s: prefix is now a readonly property according to the DOM spec.", __func__);

    CyberCore::JSMainThreadNullState state;
    CyberCore::Node* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    auto result = item->setPrefix(convertedValue);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

gchar* webkit_dom_node_get_local_name(CyberKitDOMNode* self)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(self), nullptr);
    CyberCore::JSMainThreadNullState state;
    return convertToUTF8String(CyberKit::core(self)->localName());
}

G_DEFINE_TYPE(CyberKitDOMEntityReference, webkit_dom_entity_reference, WEBKIT_DOM_TYPE_NODE)

static void webkit_dom_entity_reference_init(CyberKitDOMEntityReference*)
{
}

static void webkit_dom_entity_reference_class_init(CyberKitDOMEntityReferenceClass*)
{
}

gboolean webkit_dom_node_iterator_get_expand_entity_references(CyberKitDOMNodeIterator*)
{
    g_warning("%s: EntityReference has been removed from DOM spec, this function does nothing.", __func__);
    return FALSE;
}

gboolean webkit_dom_tree_walker_get_expand_entity_references(CyberKitDOMTreeWalker*)
{
    g_warning("%s: EntityReference has been removed from DOM spec, this function does nothing.", __func__);
    return FALSE;
}

CyberKitDOMEntityReference* webkit_dom_document_create_entity_reference(CyberKitDOMDocument*, const gchar*, GError**)
{
    g_warning("%s: EntityReference has been removed from DOM spec, this function does nothing.", __func__);
    return nullptr;
}

G_DEFINE_TYPE(CyberKitDOMHTMLBaseFontElement, webkit_dom_html_base_font_element, WEBKIT_DOM_TYPE_HTML_ELEMENT)

static void webkit_dom_html_base_font_element_init(CyberKitDOMHTMLBaseFontElement*)
{
}

static void webkit_dom_html_base_font_element_class_init(CyberKitDOMHTMLBaseFontElementClass*)
{
}

gchar* webkit_dom_html_base_font_element_get_color(CyberKitDOMHTMLBaseFontElement*)
{
    g_warning("%s: HTMLBaseFont has been removed from DOM spec, this function does nothing.", __func__);
    return nullptr;
}

void webkit_dom_html_base_font_element_set_color(CyberKitDOMHTMLBaseFontElement*, const gchar*)
{
    g_warning("%s: HTMLBaseFont has been removed from DOM spec, this function does nothing.", __func__);
}

gchar* webkit_dom_html_base_font_element_get_face(CyberKitDOMHTMLBaseFontElement*)
{
    g_warning("%s: HTMLBaseFont has been removed from DOM spec, this function does nothing.", __func__);
    return nullptr;
}

void webkit_dom_html_base_font_element_set_face(CyberKitDOMHTMLBaseFontElement*, const gchar*)
{
    g_warning("%s: HTMLBaseFont has been removed from DOM spec, this function does nothing.", __func__);
}

glong webkit_dom_html_base_font_element_get_size(CyberKitDOMHTMLBaseFontElement*)
{
    g_warning("%s: HTMLBaseFont has been removed from DOM spec, this function does nothing.", __func__);
    return 0;
}

void webkit_dom_html_base_font_element_set_size(CyberKitDOMHTMLBaseFontElement*, glong)
{
    g_warning("%s: HTMLBaseFont has been removed from DOM spec, this function does nothing.", __func__);
}

gchar* webkit_dom_element_get_webkit_region_overset(CyberKitDOMElement*)
{
    g_warning("%s: CSS Regions support has been removed, this function does nothing.", __func__);
    return nullptr;
}
G_GNUC_END_IGNORE_DEPRECATIONS;
