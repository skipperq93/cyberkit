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

#include "config.h"
#include "CyberKitDOMDocument.h"

#include "ConvertToUTF8String.h"
#include "GObjectEventListener.h"
#include "CyberKitDOMAttrPrivate.h"
#include "CyberKitDOMCDATASectionPrivate.h"
#include "CyberKitDOMCSSStyleDeclarationPrivate.h"
#include "CyberKitDOMCommentPrivate.h"
#include "CyberKitDOMDOMImplementationPrivate.h"
#include "CyberKitDOMDOMWindowPrivate.h"
#include "CyberKitDOMDocumentFragmentPrivate.h"
#include "CyberKitDOMDocumentPrivate.h"
#include "CyberKitDOMDocumentTypePrivate.h"
#include "CyberKitDOMDocumentUnstable.h"
#include "CyberKitDOMElementPrivate.h"
#include "CyberKitDOMEventPrivate.h"
#include "CyberKitDOMEventTarget.h"
#include "CyberKitDOMHTMLCollectionPrivate.h"
#include "CyberKitDOMHTMLElementPrivate.h"
#include "CyberKitDOMHTMLHeadElementPrivate.h"
#include "CyberKitDOMHTMLScriptElementPrivate.h"
#include "CyberKitDOMNodeFilterPrivate.h"
#include "CyberKitDOMNodeIteratorPrivate.h"
#include "CyberKitDOMNodeListPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "CyberKitDOMProcessingInstructionPrivate.h"
#include "CyberKitDOMRangePrivate.h"
#include "CyberKitDOMStyleSheetListPrivate.h"
#include "CyberKitDOMTextPrivate.h"
#include "CyberKitDOMTreeWalkerPrivate.h"
#include "CyberKitDOMXPathExpressionPrivate.h"
#include "CyberKitDOMXPathNSResolverPrivate.h"
#include "CyberKitDOMXPathResultPrivate.h"
#include <CyberCore/CSSImportRule.h>
#include <CyberCore/DOMException.h>
#include <CyberCore/Document.h>
#include <CyberCore/FullscreenManager.h>
#include <CyberCore/JSExecState.h>
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

static gboolean webkit_dom_document_dispatch_event(CyberKitDOMEventTarget* target, CyberKitDOMEvent* event, GError** error)
{
    CyberCore::Event* coreEvent = CyberKit::core(event);
    if (!coreEvent)
        return false;
    CyberCore::Document* coreTarget = static_cast<CyberCore::Document*>(WEBKIT_DOM_OBJECT(target)->coreObject);

    auto result = coreTarget->dispatchEventForBindings(*coreEvent);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return false;
    }
    return result.releaseReturnValue();
}

static gboolean webkit_dom_document_add_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::Document* coreTarget = static_cast<CyberCore::Document*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::addEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

static gboolean webkit_dom_document_remove_event_listener(CyberKitDOMEventTarget* target, const char* eventName, GClosure* handler, gboolean useCapture)
{
    CyberCore::Document* coreTarget = static_cast<CyberCore::Document*>(WEBKIT_DOM_OBJECT(target)->coreObject);
    return CyberKit::GObjectEventListener::removeEventListener(G_OBJECT(target), coreTarget, eventName, handler, useCapture);
}

void webkitDOMDocumentDOMEventTargetInit(CyberKitDOMEventTargetIface* iface)
{
    iface->dispatch_event = webkit_dom_document_dispatch_event;
    iface->add_event_listener = webkit_dom_document_add_event_listener;
    iface->remove_event_listener = webkit_dom_document_remove_event_listener;
}

enum {
    DOM_DOCUMENT_PROP_0,
    DOM_DOCUMENT_PROP_DOCTYPE,
    DOM_DOCUMENT_PROP_IMPLEMENTATION,
    DOM_DOCUMENT_PROP_DOCUMENT_ELEMENT,
    DOM_DOCUMENT_PROP_INPUT_ENCODING,
    DOM_DOCUMENT_PROP_XML_ENCODING,
    DOM_DOCUMENT_PROP_XML_VERSION,
    DOM_DOCUMENT_PROP_XML_STANDALONE,
    DOM_DOCUMENT_PROP_DOCUMENT_URI,
    DOM_DOCUMENT_PROP_DEFAULT_VIEW,
    DOM_DOCUMENT_PROP_STYLE_SHEETS,
    DOM_DOCUMENT_PROP_CONTENT_TYPE,
    DOM_DOCUMENT_PROP_TITLE,
    DOM_DOCUMENT_PROP_DIR,
    DOM_DOCUMENT_PROP_DESIGN_MODE,
    DOM_DOCUMENT_PROP_REFERRER,
    DOM_DOCUMENT_PROP_DOMAIN,
    DOM_DOCUMENT_PROP_URL,
    DOM_DOCUMENT_PROP_COOKIE,
    DOM_DOCUMENT_PROP_BODY,
    DOM_DOCUMENT_PROP_HEAD,
    DOM_DOCUMENT_PROP_IMAGES,
    DOM_DOCUMENT_PROP_APPLETS,
    DOM_DOCUMENT_PROP_LINKS,
    DOM_DOCUMENT_PROP_FORMS,
    DOM_DOCUMENT_PROP_ANCHORS,
    DOM_DOCUMENT_PROP_EMBEDS,
    DOM_DOCUMENT_PROP_PLUGINS,
    DOM_DOCUMENT_PROP_SCRIPTS,
    DOM_DOCUMENT_PROP_LAST_MODIFIED,
    DOM_DOCUMENT_PROP_CHARSET,
    DOM_DOCUMENT_PROP_READY_STATE,
    DOM_DOCUMENT_PROP_CHARACTER_SET,
    DOM_DOCUMENT_PROP_PREFERRED_STYLESHEET_SET,
    DOM_DOCUMENT_PROP_SELECTED_STYLESHEET_SET,
    DOM_DOCUMENT_PROP_ACTIVE_ELEMENT,
    DOM_DOCUMENT_PROP_COMPAT_MODE,
    DOM_DOCUMENT_PROP_WEBKIT_IS_FULL_SCREEN,
    DOM_DOCUMENT_PROP_WEBKIT_FULL_SCREEN_KEYBOARD_INPUT_ALLOWED,
    DOM_DOCUMENT_PROP_WEBKIT_CURRENT_FULL_SCREEN_ELEMENT,
    DOM_DOCUMENT_PROP_WEBKIT_FULLSCREEN_ENABLED,
    DOM_DOCUMENT_PROP_WEBKIT_FULLSCREEN_ELEMENT,
    DOM_DOCUMENT_PROP_POINTER_LOCK_ELEMENT,
    DOM_DOCUMENT_PROP_VISIBILITY_STATE,
    DOM_DOCUMENT_PROP_HIDDEN,
    DOM_DOCUMENT_PROP_CURRENT_SCRIPT,
    DOM_DOCUMENT_PROP_ORIGIN,
    DOM_DOCUMENT_PROP_SCROLLING_ELEMENT,
    DOM_DOCUMENT_PROP_CHILDREN,
    DOM_DOCUMENT_PROP_FIRST_ELEMENT_CHILD,
    DOM_DOCUMENT_PROP_LAST_ELEMENT_CHILD,
    DOM_DOCUMENT_PROP_CHILD_ELEMENT_COUNT,
};

static void webkit_dom_document_set_property(GObject* object, guint propertyId, const GValue* value, GParamSpec* pspec)
{
    CyberKitDOMDocument* self = WEBKIT_DOM_DOCUMENT(object);

    switch (propertyId) {
    case DOM_DOCUMENT_PROP_XML_VERSION:
        webkit_dom_document_set_xml_version(self, g_value_get_string(value), nullptr);
        break;
    case DOM_DOCUMENT_PROP_XML_STANDALONE:
        webkit_dom_document_set_xml_standalone(self, g_value_get_boolean(value), nullptr);
        break;
    case DOM_DOCUMENT_PROP_DOCUMENT_URI:
        webkit_dom_document_set_document_uri(self, g_value_get_string(value));
        break;
    case DOM_DOCUMENT_PROP_TITLE:
        webkit_dom_document_set_title(self, g_value_get_string(value));
        break;
    case DOM_DOCUMENT_PROP_DIR:
        webkit_dom_document_set_dir(self, g_value_get_string(value));
        break;
    case DOM_DOCUMENT_PROP_DESIGN_MODE:
        webkit_dom_document_set_design_mode(self, g_value_get_string(value));
        break;
    case DOM_DOCUMENT_PROP_COOKIE:
        webkit_dom_document_set_cookie(self, g_value_get_string(value), nullptr);
        break;
    case DOM_DOCUMENT_PROP_CHARSET:
        webkit_dom_document_set_charset(self, g_value_get_string(value));
        break;
    case DOM_DOCUMENT_PROP_SELECTED_STYLESHEET_SET:
        g_warning("%s: The selected-stylesheet-set property has been removed and no longer works.", __func__);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_document_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMDocument* self = WEBKIT_DOM_DOCUMENT(object);

    switch (propertyId) {
    case DOM_DOCUMENT_PROP_DOCTYPE:
        g_value_set_object(value, webkit_dom_document_get_doctype(self));
        break;
    case DOM_DOCUMENT_PROP_IMPLEMENTATION:
        g_value_set_object(value, webkit_dom_document_get_implementation(self));
        break;
    case DOM_DOCUMENT_PROP_DOCUMENT_ELEMENT:
        g_value_set_object(value, webkit_dom_document_get_document_element(self));
        break;
    case DOM_DOCUMENT_PROP_INPUT_ENCODING:
        g_value_take_string(value, webkit_dom_document_get_input_encoding(self));
        break;
    case DOM_DOCUMENT_PROP_XML_ENCODING:
        g_value_take_string(value, webkit_dom_document_get_xml_encoding(self));
        break;
    case DOM_DOCUMENT_PROP_XML_VERSION:
        g_value_take_string(value, webkit_dom_document_get_xml_version(self));
        break;
    case DOM_DOCUMENT_PROP_XML_STANDALONE:
        g_value_set_boolean(value, webkit_dom_document_get_xml_standalone(self));
        break;
    case DOM_DOCUMENT_PROP_DOCUMENT_URI:
        g_value_take_string(value, webkit_dom_document_get_document_uri(self));
        break;
    case DOM_DOCUMENT_PROP_DEFAULT_VIEW:
        g_value_set_object(value, webkit_dom_document_get_default_view(self));
        break;
    case DOM_DOCUMENT_PROP_STYLE_SHEETS:
        g_value_set_object(value, webkit_dom_document_get_style_sheets(self));
        break;
    case DOM_DOCUMENT_PROP_CONTENT_TYPE:
        g_value_take_string(value, webkit_dom_document_get_content_type(self));
        break;
    case DOM_DOCUMENT_PROP_TITLE:
        g_value_take_string(value, webkit_dom_document_get_title(self));
        break;
    case DOM_DOCUMENT_PROP_DIR:
        g_value_take_string(value, webkit_dom_document_get_dir(self));
        break;
    case DOM_DOCUMENT_PROP_DESIGN_MODE:
        g_value_take_string(value, webkit_dom_document_get_design_mode(self));
        break;
    case DOM_DOCUMENT_PROP_REFERRER:
        g_value_take_string(value, webkit_dom_document_get_referrer(self));
        break;
    case DOM_DOCUMENT_PROP_DOMAIN:
        g_value_take_string(value, webkit_dom_document_get_domain(self));
        break;
    case DOM_DOCUMENT_PROP_URL:
        g_value_take_string(value, webkit_dom_document_get_url(self));
        break;
    case DOM_DOCUMENT_PROP_COOKIE:
        g_value_take_string(value, webkit_dom_document_get_cookie(self, nullptr));
        break;
    case DOM_DOCUMENT_PROP_BODY:
        g_value_set_object(value, webkit_dom_document_get_body(self));
        break;
    case DOM_DOCUMENT_PROP_HEAD:
        g_value_set_object(value, webkit_dom_document_get_head(self));
        break;
    case DOM_DOCUMENT_PROP_IMAGES:
        g_value_set_object(value, webkit_dom_document_get_images(self));
        break;
    case DOM_DOCUMENT_PROP_APPLETS:
        g_value_set_object(value, webkit_dom_document_get_applets(self));
        break;
    case DOM_DOCUMENT_PROP_LINKS:
        g_value_set_object(value, webkit_dom_document_get_links(self));
        break;
    case DOM_DOCUMENT_PROP_FORMS:
        g_value_set_object(value, webkit_dom_document_get_forms(self));
        break;
    case DOM_DOCUMENT_PROP_ANCHORS:
        g_value_set_object(value, webkit_dom_document_get_anchors(self));
        break;
    case DOM_DOCUMENT_PROP_EMBEDS:
        g_value_set_object(value, webkit_dom_document_get_embeds(self));
        break;
    case DOM_DOCUMENT_PROP_PLUGINS:
        g_value_set_object(value, webkit_dom_document_get_plugins(self));
        break;
    case DOM_DOCUMENT_PROP_SCRIPTS:
        g_value_set_object(value, webkit_dom_document_get_scripts(self));
        break;
    case DOM_DOCUMENT_PROP_LAST_MODIFIED:
        g_value_take_string(value, webkit_dom_document_get_last_modified(self));
        break;
    case DOM_DOCUMENT_PROP_CHARSET:
        g_value_take_string(value, webkit_dom_document_get_charset(self));
        break;
    case DOM_DOCUMENT_PROP_READY_STATE:
        g_value_take_string(value, webkit_dom_document_get_ready_state(self));
        break;
    case DOM_DOCUMENT_PROP_CHARACTER_SET:
        g_value_take_string(value, webkit_dom_document_get_character_set(self));
        break;
    case DOM_DOCUMENT_PROP_PREFERRED_STYLESHEET_SET:
        g_warning("%s: The preferred-stylesheet-set property has been removed and no longer works.", __func__);
        break;
    case DOM_DOCUMENT_PROP_SELECTED_STYLESHEET_SET:
        g_warning("%s: The selected-stylesheet-set property has been removed and no longer works.", __func__);
        break;
    case DOM_DOCUMENT_PROP_ACTIVE_ELEMENT:
        g_value_set_object(value, webkit_dom_document_get_active_element(self));
        break;
    case DOM_DOCUMENT_PROP_COMPAT_MODE:
        g_value_take_string(value, webkit_dom_document_get_compat_mode(self));
        break;
    case DOM_DOCUMENT_PROP_WEBKIT_IS_FULL_SCREEN:
        g_value_set_boolean(value, webkit_dom_document_get_webkit_is_fullscreen(self));
        break;
    case DOM_DOCUMENT_PROP_WEBKIT_FULL_SCREEN_KEYBOARD_INPUT_ALLOWED:
        g_value_set_boolean(value, webkit_dom_document_get_webkit_fullscreen_keyboard_input_allowed(self));
        break;
    case DOM_DOCUMENT_PROP_WEBKIT_CURRENT_FULL_SCREEN_ELEMENT:
        g_value_set_object(value, webkit_dom_document_get_webkit_current_fullscreen_element(self));
        break;
    case DOM_DOCUMENT_PROP_WEBKIT_FULLSCREEN_ENABLED:
        g_value_set_boolean(value, webkit_dom_document_get_webkit_fullscreen_enabled(self));
        break;
    case DOM_DOCUMENT_PROP_WEBKIT_FULLSCREEN_ELEMENT:
        g_value_set_object(value, webkit_dom_document_get_webkit_fullscreen_element(self));
        break;
    case DOM_DOCUMENT_PROP_POINTER_LOCK_ELEMENT:
        g_value_set_object(value, webkit_dom_document_get_pointer_lock_element(self));
        break;
    case DOM_DOCUMENT_PROP_VISIBILITY_STATE:
        g_value_take_string(value, webkit_dom_document_get_visibility_state(self));
        break;
    case DOM_DOCUMENT_PROP_HIDDEN:
        g_value_set_boolean(value, webkit_dom_document_get_hidden(self));
        break;
    case DOM_DOCUMENT_PROP_CURRENT_SCRIPT:
        g_value_set_object(value, webkit_dom_document_get_current_script(self));
        break;
    case DOM_DOCUMENT_PROP_ORIGIN:
        g_value_take_string(value, webkit_dom_document_get_origin(self));
        break;
    case DOM_DOCUMENT_PROP_SCROLLING_ELEMENT:
        g_value_set_object(value, webkit_dom_document_get_scrolling_element(self));
        break;
    case DOM_DOCUMENT_PROP_CHILDREN:
        g_value_set_object(value, webkit_dom_document_get_children(self));
        break;
    case DOM_DOCUMENT_PROP_FIRST_ELEMENT_CHILD:
        g_value_set_object(value, webkit_dom_document_get_first_element_child(self));
        break;
    case DOM_DOCUMENT_PROP_LAST_ELEMENT_CHILD:
        g_value_set_object(value, webkit_dom_document_get_last_element_child(self));
        break;
    case DOM_DOCUMENT_PROP_CHILD_ELEMENT_COUNT:
        g_value_set_ulong(value, webkit_dom_document_get_child_element_count(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

void webkitDOMDocumentInstallProperties(GObjectClass* gobjectClass)
{
    gobjectClass->set_property = webkit_dom_document_set_property;
    gobjectClass->get_property = webkit_dom_document_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_DOCTYPE,
        g_param_spec_object(
            "doctype",
            "Document:doctype",
            "read-only CyberKitDOMDocumentType* Document:doctype",
            WEBKIT_DOM_TYPE_DOCUMENT_TYPE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_IMPLEMENTATION,
        g_param_spec_object(
            "implementation",
            "Document:implementation",
            "read-only CyberKitDOMDOMImplementation* Document:implementation",
            WEBKIT_DOM_TYPE_DOM_IMPLEMENTATION,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_DOCUMENT_ELEMENT,
        g_param_spec_object(
            "document-element",
            "Document:document-element",
            "read-only CyberKitDOMElement* Document:document-element",
            WEBKIT_DOM_TYPE_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_INPUT_ENCODING,
        g_param_spec_string(
            "input-encoding",
            "Document:input-encoding",
            "read-only gchar* Document:input-encoding",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_XML_ENCODING,
        g_param_spec_string(
            "xml-encoding",
            "Document:xml-encoding",
            "read-only gchar* Document:xml-encoding",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_XML_VERSION,
        g_param_spec_string(
            "xml-version",
            "Document:xml-version",
            "read-write gchar* Document:xml-version",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_XML_STANDALONE,
        g_param_spec_boolean(
            "xml-standalone",
            "Document:xml-standalone",
            "read-write gboolean Document:xml-standalone",
            FALSE,
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_DOCUMENT_URI,
        g_param_spec_string(
            "document-uri",
            "Document:document-uri",
            "read-write gchar* Document:document-uri",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_DEFAULT_VIEW,
        g_param_spec_object(
            "default-view",
            "Document:default-view",
            "read-only CyberKitDOMDOMWindow* Document:default-view",
            WEBKIT_DOM_TYPE_DOM_WINDOW,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_STYLE_SHEETS,
        g_param_spec_object(
            "style-sheets",
            "Document:style-sheets",
            "read-only CyberKitDOMStyleSheetList* Document:style-sheets",
            WEBKIT_DOM_TYPE_STYLE_SHEET_LIST,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_CONTENT_TYPE,
        g_param_spec_string(
            "content-type",
            "Document:content-type",
            "read-only gchar* Document:content-type",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_TITLE,
        g_param_spec_string(
            "title",
            "Document:title",
            "read-write gchar* Document:title",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_DIR,
        g_param_spec_string(
            "dir",
            "Document:dir",
            "read-write gchar* Document:dir",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_DESIGN_MODE,
        g_param_spec_string(
            "design-mode",
            "Document:design-mode",
            "read-write gchar* Document:design-mode",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_REFERRER,
        g_param_spec_string(
            "referrer",
            "Document:referrer",
            "read-only gchar* Document:referrer",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_DOMAIN,
        g_param_spec_string(
            "domain",
            "Document:domain",
            "read-only gchar* Document:domain",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_URL,
        g_param_spec_string(
            "url",
            "Document:url",
            "read-only gchar* Document:url",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_COOKIE,
        g_param_spec_string(
            "cookie",
            "Document:cookie",
            "read-write gchar* Document:cookie",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_BODY,
        g_param_spec_object(
            "body",
            "Document:body",
            "read-only CyberKitDOMHTMLElement* Document:body",
            WEBKIT_DOM_TYPE_HTML_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_HEAD,
        g_param_spec_object(
            "head",
            "Document:head",
            "read-only CyberKitDOMHTMLHeadElement* Document:head",
            WEBKIT_DOM_TYPE_HTML_HEAD_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_IMAGES,
        g_param_spec_object(
            "images",
            "Document:images",
            "read-only CyberKitDOMHTMLCollection* Document:images",
            WEBKIT_DOM_TYPE_HTML_COLLECTION,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_APPLETS,
        g_param_spec_object(
            "applets",
            "Document:applets",
            "read-only CyberKitDOMHTMLCollection* Document:applets",
            WEBKIT_DOM_TYPE_HTML_COLLECTION,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_LINKS,
        g_param_spec_object(
            "links",
            "Document:links",
            "read-only CyberKitDOMHTMLCollection* Document:links",
            WEBKIT_DOM_TYPE_HTML_COLLECTION,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_FORMS,
        g_param_spec_object(
            "forms",
            "Document:forms",
            "read-only CyberKitDOMHTMLCollection* Document:forms",
            WEBKIT_DOM_TYPE_HTML_COLLECTION,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_ANCHORS,
        g_param_spec_object(
            "anchors",
            "Document:anchors",
            "read-only CyberKitDOMHTMLCollection* Document:anchors",
            WEBKIT_DOM_TYPE_HTML_COLLECTION,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_EMBEDS,
        g_param_spec_object(
            "embeds",
            "Document:embeds",
            "read-only CyberKitDOMHTMLCollection* Document:embeds",
            WEBKIT_DOM_TYPE_HTML_COLLECTION,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_PLUGINS,
        g_param_spec_object(
            "plugins",
            "Document:plugins",
            "read-only CyberKitDOMHTMLCollection* Document:plugins",
            WEBKIT_DOM_TYPE_HTML_COLLECTION,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_SCRIPTS,
        g_param_spec_object(
            "scripts",
            "Document:scripts",
            "read-only CyberKitDOMHTMLCollection* Document:scripts",
            WEBKIT_DOM_TYPE_HTML_COLLECTION,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_LAST_MODIFIED,
        g_param_spec_string(
            "last-modified",
            "Document:last-modified",
            "read-only gchar* Document:last-modified",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_CHARSET,
        g_param_spec_string(
            "charset",
            "Document:charset",
            "read-write gchar* Document:charset",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_READY_STATE,
        g_param_spec_string(
            "ready-state",
            "Document:ready-state",
            "read-only gchar* Document:ready-state",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_CHARACTER_SET,
        g_param_spec_string(
            "character-set",
            "Document:character-set",
            "read-only gchar* Document:character-set",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_PREFERRED_STYLESHEET_SET,
        g_param_spec_string(
            "preferred-stylesheet-set",
            "Document:preferred-stylesheet-set",
            "read-only gchar* Document:preferred-stylesheet-set",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_SELECTED_STYLESHEET_SET,
        g_param_spec_string(
            "selected-stylesheet-set",
            "Document:selected-stylesheet-set",
            "read-write gchar* Document:selected-stylesheet-set",
            "",
            WEBKIT_PARAM_READWRITE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_ACTIVE_ELEMENT,
        g_param_spec_object(
            "active-element",
            "Document:active-element",
            "read-only CyberKitDOMElement* Document:active-element",
            WEBKIT_DOM_TYPE_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_COMPAT_MODE,
        g_param_spec_string(
            "compat-mode",
            "Document:compat-mode",
            "read-only gchar* Document:compat-mode",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_WEBKIT_IS_FULL_SCREEN,
        g_param_spec_boolean(
            "webkit-is-full-screen",
            "Document:webkit-is-full-screen",
            "read-only gboolean Document:webkit-is-full-screen",
            FALSE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_WEBKIT_FULL_SCREEN_KEYBOARD_INPUT_ALLOWED,
        g_param_spec_boolean(
            "webkit-full-screen-keyboard-input-allowed",
            "Document:webkit-full-screen-keyboard-input-allowed",
            "read-only gboolean Document:webkit-full-screen-keyboard-input-allowed",
            FALSE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_WEBKIT_CURRENT_FULL_SCREEN_ELEMENT,
        g_param_spec_object(
            "webkit-current-full-screen-element",
            "Document:webkit-current-full-screen-element",
            "read-only CyberKitDOMElement* Document:webkit-current-full-screen-element",
            WEBKIT_DOM_TYPE_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_WEBKIT_FULLSCREEN_ENABLED,
        g_param_spec_boolean(
            "webkit-fullscreen-enabled",
            "Document:webkit-fullscreen-enabled",
            "read-only gboolean Document:webkit-fullscreen-enabled",
            FALSE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_WEBKIT_FULLSCREEN_ELEMENT,
        g_param_spec_object(
            "webkit-fullscreen-element",
            "Document:webkit-fullscreen-element",
            "read-only CyberKitDOMElement* Document:webkit-fullscreen-element",
            WEBKIT_DOM_TYPE_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_POINTER_LOCK_ELEMENT,
        g_param_spec_object(
            "pointer-lock-element",
            "Document:pointer-lock-element",
            "read-only CyberKitDOMElement* Document:pointer-lock-element",
            WEBKIT_DOM_TYPE_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_VISIBILITY_STATE,
        g_param_spec_string(
            "visibility-state",
            "Document:visibility-state",
            "read-only gchar* Document:visibility-state",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_HIDDEN,
        g_param_spec_boolean(
            "hidden",
            "Document:hidden",
            "read-only gboolean Document:hidden",
            FALSE,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_CURRENT_SCRIPT,
        g_param_spec_object(
            "current-script",
            "Document:current-script",
            "read-only CyberKitDOMHTMLScriptElement* Document:current-script",
            WEBKIT_DOM_TYPE_HTML_SCRIPT_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_ORIGIN,
        g_param_spec_string(
            "origin",
            "Document:origin",
            "read-only gchar* Document:origin",
            "",
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_SCROLLING_ELEMENT,
        g_param_spec_object(
            "scrolling-element",
            "Document:scrolling-element",
            "read-only CyberKitDOMElement* Document:scrolling-element",
            WEBKIT_DOM_TYPE_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_CHILDREN,
        g_param_spec_object(
            "children",
            "Document:children",
            "read-only CyberKitDOMHTMLCollection* Document:children",
            WEBKIT_DOM_TYPE_HTML_COLLECTION,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_FIRST_ELEMENT_CHILD,
        g_param_spec_object(
            "first-element-child",
            "Document:first-element-child",
            "read-only CyberKitDOMElement* Document:first-element-child",
            WEBKIT_DOM_TYPE_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_LAST_ELEMENT_CHILD,
        g_param_spec_object(
            "last-element-child",
            "Document:last-element-child",
            "read-only CyberKitDOMElement* Document:last-element-child",
            WEBKIT_DOM_TYPE_ELEMENT,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_DOCUMENT_PROP_CHILD_ELEMENT_COUNT,
        g_param_spec_ulong(
            "child-element-count",
            "Document:child-element-count",
            "read-only gulong Document:child-element-count",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READABLE));

}

CyberKitDOMElement* webkit_dom_document_create_element(CyberKitDOMDocument* self, const gchar* tagName, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(tagName, 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedTagName = WTF::String::fromUTF8(tagName);
    auto result = item->createElementForBindings(convertedTagName);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

CyberKitDOMDocumentFragment* webkit_dom_document_create_document_fragment(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::DocumentFragment> gobjectResult = WTF::getPtr(item->createDocumentFragment());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMText* webkit_dom_document_create_text_node(CyberKitDOMDocument* self, const gchar* data)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(data, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedData = WTF::String::fromUTF8(data);
    RefPtr<CyberCore::Text> gobjectResult = WTF::getPtr(item->createTextNode(convertedData));
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMComment* webkit_dom_document_create_comment(CyberKitDOMDocument* self, const gchar* data)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(data, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedData = WTF::String::fromUTF8(data);
    RefPtr<CyberCore::Comment> gobjectResult = WTF::getPtr(item->createComment(convertedData));
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMCDATASection* webkit_dom_document_create_cdata_section(CyberKitDOMDocument* self, const gchar* data, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(data, 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedData = WTF::String::fromUTF8(data);
    auto result = item->createCDATASection(convertedData);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

CyberKitDOMProcessingInstruction* webkit_dom_document_create_processing_instruction(CyberKitDOMDocument* self, const gchar* target, const gchar* data, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(target, 0);
    g_return_val_if_fail(data, 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedTarget = WTF::String::fromUTF8(target);
    WTF::String convertedData = WTF::String::fromUTF8(data);
    auto result = item->createProcessingInstruction(convertedTarget, convertedData);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

CyberKitDOMAttr* webkit_dom_document_create_attribute(CyberKitDOMDocument* self, const gchar* name, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(name, 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedName = WTF::String::fromUTF8(name);
    auto result = item->createAttribute(convertedName);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

CyberKitDOMHTMLCollection* webkit_dom_document_get_elements_by_tag_name_as_html_collection(CyberKitDOMDocument* self, const gchar* tagname)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(tagname, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedTagname = WTF::String::fromUTF8(tagname);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->getElementsByTagName(convertedTagname));
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMNode* webkit_dom_document_import_node(CyberKitDOMDocument* self, CyberKitDOMNode* importedNode, gboolean deep, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(importedNode), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Document* item = CyberKit::core(self);
    CyberCore::Node* convertedImportedNode = CyberKit::core(importedNode);
    auto result = item->importNode(*convertedImportedNode, deep);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

CyberKitDOMElement* webkit_dom_document_create_element_ns(CyberKitDOMDocument* self, const gchar* namespaceURI, const gchar* qualifiedName, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(qualifiedName, 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedNamespaceURI = WTF::String::fromUTF8(namespaceURI);
    WTF::String convertedQualifiedName = WTF::String::fromUTF8(qualifiedName);
    auto result = item->createElementNS(convertedNamespaceURI, convertedQualifiedName);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

CyberKitDOMAttr* webkit_dom_document_create_attribute_ns(CyberKitDOMDocument* self, const gchar* namespaceURI, const gchar* qualifiedName, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(qualifiedName, 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedNamespaceURI = WTF::String::fromUTF8(namespaceURI);
    WTF::String convertedQualifiedName = WTF::String::fromUTF8(qualifiedName);
    auto result = item->createAttributeNS(convertedNamespaceURI, convertedQualifiedName);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

CyberKitDOMHTMLCollection* webkit_dom_document_get_elements_by_tag_name_ns_as_html_collection(CyberKitDOMDocument* self, const gchar* namespaceURI, const gchar* localName)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(namespaceURI, 0);
    g_return_val_if_fail(localName, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedNamespaceURI = WTF::String::fromUTF8(namespaceURI);
    WTF::String convertedLocalName = WTF::String::fromUTF8(localName);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->getElementsByTagNameNS(convertedNamespaceURI, convertedLocalName));
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMNode* webkit_dom_document_adopt_node(CyberKitDOMDocument* self, CyberKitDOMNode* source, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(source), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Document* item = CyberKit::core(self);
    CyberCore::Node* convertedSource = CyberKit::core(source);
    auto result = item->adoptNode(*convertedSource);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

CyberKitDOMEvent* webkit_dom_document_create_event(CyberKitDOMDocument* self, const gchar* eventType, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(eventType, 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedEventType = WTF::String::fromUTF8(eventType);
    auto result = item->createEvent(convertedEventType);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

CyberKitDOMRange* webkit_dom_document_create_range(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::Range> gobjectResult = WTF::getPtr(item->createRange());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMNodeIterator* webkit_dom_document_create_node_iterator(CyberKitDOMDocument* self, CyberKitDOMNode* root, gulong whatToShow, CyberKitDOMNodeFilter* filter, gboolean expandEntityReferences, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(root), 0);
    g_return_val_if_fail(!filter || WEBKIT_DOM_IS_NODE_FILTER(filter), 0);
    UNUSED_PARAM(error);
    CyberCore::Document* item = CyberKit::core(self);
    CyberCore::Node* convertedRoot = CyberKit::core(root);
    RefPtr<CyberCore::NodeFilter> convertedFilter = CyberKit::core(item, filter);
    RefPtr<CyberCore::NodeIterator> gobjectResult = WTF::getPtr(item->createNodeIterator(*convertedRoot, whatToShow, WTF::getPtr(convertedFilter), expandEntityReferences));
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMTreeWalker* webkit_dom_document_create_tree_walker(CyberKitDOMDocument* self, CyberKitDOMNode* root, gulong whatToShow, CyberKitDOMNodeFilter* filter, gboolean expandEntityReferences, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(root), 0);
    g_return_val_if_fail(!filter || WEBKIT_DOM_IS_NODE_FILTER(filter), 0);
    UNUSED_PARAM(error);
    CyberCore::Document* item = CyberKit::core(self);
    CyberCore::Node* convertedRoot = CyberKit::core(root);
    RefPtr<CyberCore::NodeFilter> convertedFilter = CyberKit::core(item, filter);
    RefPtr<CyberCore::TreeWalker> gobjectResult = WTF::getPtr(item->createTreeWalker(*convertedRoot, whatToShow, WTF::getPtr(convertedFilter), expandEntityReferences));
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMCSSStyleDeclaration* webkit_dom_document_get_override_style(CyberKitDOMDocument*, CyberKitDOMElement*, const gchar*)
{
    return nullptr;
}

CyberKitDOMXPathExpression* webkit_dom_document_create_expression(CyberKitDOMDocument* self, const gchar* expression, CyberKitDOMXPathNSResolver* resolver, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(expression, 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_XPATH_NS_RESOLVER(resolver), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedExpression = WTF::String::fromUTF8(expression);
    RefPtr<CyberCore::XPathNSResolver> convertedResolver = CyberKit::core(resolver);
    auto result = item->createExpression(convertedExpression, WTFMove(convertedResolver));
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

CyberKitDOMXPathNSResolver* webkit_dom_document_create_ns_resolver(CyberKitDOMDocument* self, CyberKitDOMNode* nodeResolver)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(WEBKIT_DOM_IS_NODE(nodeResolver), 0);
    CyberCore::Document* item = CyberKit::core(self);
    CyberCore::Node* convertedNodeResolver = CyberKit::core(nodeResolver);
    if (!convertedNodeResolver)
        return nullptr;
    RefPtr<CyberCore::XPathNSResolver> gobjectResult = WTF::getPtr(item->createNSResolver(*convertedNodeResolver));
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMXPathResult* webkit_dom_document_evaluate(CyberKitDOMDocument* self, const gchar* expression, CyberKitDOMNode* contextNode, CyberKitDOMXPathNSResolver* resolver, gushort type, CyberKitDOMXPathResult* inResult, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(expression, 0);
    g_return_val_if_fail(contextNode && WEBKIT_DOM_IS_NODE(contextNode), 0);
    g_return_val_if_fail(!resolver || WEBKIT_DOM_IS_XPATH_NS_RESOLVER(resolver), 0);
    g_return_val_if_fail(!inResult || WEBKIT_DOM_IS_XPATH_RESULT(inResult), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedExpression = WTF::String::fromUTF8(expression);
    CyberCore::Node* convertedContextNode = CyberKit::core(contextNode);
    RefPtr<CyberCore::XPathNSResolver> convertedResolver = CyberKit::core(resolver);
    CyberCore::XPathResult* convertedInResult = CyberKit::core(inResult);
    auto result = item->evaluate(convertedExpression, *convertedContextNode, WTFMove(convertedResolver), type, convertedInResult);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

gboolean webkit_dom_document_exec_command(CyberKitDOMDocument* self, const gchar* command, gboolean userInterface, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), FALSE);
    g_return_val_if_fail(command, FALSE);
    g_return_val_if_fail(value, FALSE);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedCommand = WTF::String::fromUTF8(command);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    gboolean result = item->execCommand(convertedCommand, userInterface, convertedValue);
    return result;
}

gboolean webkit_dom_document_query_command_enabled(CyberKitDOMDocument* self, const gchar* command)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), FALSE);
    g_return_val_if_fail(command, FALSE);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedCommand = WTF::String::fromUTF8(command);
    gboolean result = item->queryCommandEnabled(convertedCommand);
    return result;
}

gboolean webkit_dom_document_query_command_indeterm(CyberKitDOMDocument* self, const gchar* command)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), FALSE);
    g_return_val_if_fail(command, FALSE);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedCommand = WTF::String::fromUTF8(command);
    gboolean result = item->queryCommandIndeterm(convertedCommand);
    return result;
}

gboolean webkit_dom_document_query_command_state(CyberKitDOMDocument* self, const gchar* command)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), FALSE);
    g_return_val_if_fail(command, FALSE);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedCommand = WTF::String::fromUTF8(command);
    gboolean result = item->queryCommandState(convertedCommand);
    return result;
}

gboolean webkit_dom_document_query_command_supported(CyberKitDOMDocument* self, const gchar* command)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), FALSE);
    g_return_val_if_fail(command, FALSE);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedCommand = WTF::String::fromUTF8(command);
    gboolean result = item->queryCommandSupported(convertedCommand);
    return result;
}

gchar* webkit_dom_document_query_command_value(CyberKitDOMDocument* self, const gchar* command)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(command, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedCommand = WTF::String::fromUTF8(command);
    gchar* result = convertToUTF8String(item->queryCommandValue(convertedCommand));
    return result;
}

CyberKitDOMNodeList* webkit_dom_document_get_elements_by_name(CyberKitDOMDocument* self, const gchar* elementName)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(elementName, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedElementName = WTF::String::fromUTF8(elementName);
    RefPtr<CyberCore::NodeList> gobjectResult = WTF::getPtr(item->getElementsByName(convertedElementName));
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMElement* webkit_dom_document_element_from_point(CyberKitDOMDocument* self, glong x, glong y)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::Element> gobjectResult = WTF::getPtr(item->elementFromPoint(x, y));
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMRange* webkit_dom_document_caret_range_from_point(CyberKitDOMDocument* self, glong x, glong y)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::Range> gobjectResult = WTF::getPtr(item->caretRangeFromPoint(x, y));
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMCSSStyleDeclaration* webkit_dom_document_create_css_style_declaration(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::CSSStyleDeclaration> gobjectResult = WTF::getPtr(item->createCSSStyleDeclaration());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMHTMLCollection* webkit_dom_document_get_elements_by_class_name_as_html_collection(CyberKitDOMDocument* self, const gchar* classNames)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(classNames, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedClassNames = WTF::String::fromUTF8(classNames);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->getElementsByClassName(convertedClassNames));
    return CyberKit::kit(gobjectResult.get());
}

gboolean webkit_dom_document_has_focus(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), FALSE);
    CyberCore::Document* item = CyberKit::core(self);
    gboolean result = item->hasFocus();
    return result;
}

void webkit_dom_document_webkit_cancel_fullscreen(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOCUMENT(self));
    CyberCore::Document* item = CyberKit::core(self);
    item->fullscreenManager().cancelFullscreen();
}

void webkit_dom_document_webkit_exit_fullscreen(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOCUMENT(self));
    CyberCore::Document* item = CyberKit::core(self);
    item->fullscreenManager().exitFullscreen();
}

void webkit_dom_document_exit_pointer_lock(CyberKitDOMDocument* self)
{
#if ENABLE(POINTER_LOCK)
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOCUMENT(self));
    CyberCore::Document* item = CyberKit::core(self);
    item->exitPointerLock();
#else
    UNUSED_PARAM(self);
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Pointer Lock")
#endif /* ENABLE(POINTER_LOCK) */
}

CyberKitDOMElement* webkit_dom_document_get_element_by_id(CyberKitDOMDocument* self, const gchar* elementId)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(elementId, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedElementId = WTF::String::fromUTF8(elementId);
    RefPtr<CyberCore::Element> gobjectResult = WTF::getPtr(item->getElementById(convertedElementId));
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMElement* webkit_dom_document_query_selector(CyberKitDOMDocument* self, const gchar* selectors, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(selectors, 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedSelectors = WTF::String::fromUTF8(selectors);
    auto result = item->querySelector(convertedSelectors);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue());
}

CyberKitDOMNodeList* webkit_dom_document_query_selector_all(CyberKitDOMDocument* self, const gchar* selectors, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(selectors, 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedSelectors = WTF::String::fromUTF8(selectors);
    auto result = item->querySelectorAll(convertedSelectors);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
        return nullptr;
    }
    return CyberKit::kit(result.releaseReturnValue().ptr());
}

CyberKitDOMDocumentType* webkit_dom_document_get_doctype(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::DocumentType> gobjectResult = WTF::getPtr(item->doctype());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMDOMImplementation* webkit_dom_document_get_implementation(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::DOMImplementation> gobjectResult = WTF::getPtr(item->implementation());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMElement* webkit_dom_document_get_document_element(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::Element> gobjectResult = WTF::getPtr(item->documentElement());
    return CyberKit::kit(gobjectResult.get());
}

gchar* webkit_dom_document_get_input_encoding(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->characterSetWithUTF8Fallback());
    return result;
}

gchar* webkit_dom_document_get_xml_encoding(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->xmlEncoding());
    return result;
}

gchar* webkit_dom_document_get_xml_version(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->xmlVersion());
    return result;
}

void webkit_dom_document_set_xml_version(CyberKitDOMDocument* self, const gchar* value, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOCUMENT(self));
    g_return_if_fail(value);
    g_return_if_fail(!error || !*error);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    auto result = item->setXMLVersion(convertedValue);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

gboolean webkit_dom_document_get_xml_standalone(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), FALSE);
    CyberCore::Document* item = CyberKit::core(self);
    gboolean result = item->xmlStandalone();
    return result;
}

void webkit_dom_document_set_xml_standalone(CyberKitDOMDocument* self, gboolean value, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOCUMENT(self));
    g_return_if_fail(!error || !*error);
    CyberKit::core(self)->setXMLStandalone(value);
}

gchar* webkit_dom_document_get_document_uri(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->documentURI());
    return result;
}

void webkit_dom_document_set_document_uri(CyberKitDOMDocument* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOCUMENT(self));
    g_return_if_fail(value);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setDocumentURI(convertedValue);
}

CyberKitDOMDOMWindow* webkit_dom_document_get_default_view(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    return CyberKit::kit(item->windowProxy());
}

CyberKitDOMStyleSheetList* webkit_dom_document_get_style_sheets(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::StyleSheetList> gobjectResult = WTF::getPtr(item->styleSheets());
    return CyberKit::kit(gobjectResult.get());
}

gchar* webkit_dom_document_get_content_type(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->contentType());
    return result;
}

gchar* webkit_dom_document_get_title(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->title());
    return result;
}

void webkit_dom_document_set_title(CyberKitDOMDocument* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOCUMENT(self));
    g_return_if_fail(value);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setTitle(convertedValue);
}

gchar* webkit_dom_document_get_dir(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->dir());
    return result;
}

void webkit_dom_document_set_dir(CyberKitDOMDocument* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOCUMENT(self));
    g_return_if_fail(value);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setDir(convertedValue);
}

gchar* webkit_dom_document_get_design_mode(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->designMode());
    return result;
}

void webkit_dom_document_set_design_mode(CyberKitDOMDocument* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOCUMENT(self));
    g_return_if_fail(value);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setDesignMode(convertedValue);
}

gchar* webkit_dom_document_get_referrer(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->referrer());
    return result;
}

gchar* webkit_dom_document_get_domain(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->domain());
    return result;
}

gchar* webkit_dom_document_get_url(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->urlForBindings());
    return result;
}

gchar* webkit_dom_document_get_cookie(CyberKitDOMDocument* self, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    g_return_val_if_fail(!error || !*error, 0);
    CyberCore::Document* item = CyberKit::core(self);
    auto result = item->cookie();
    if (result.hasException())
        return nullptr;
    return convertToUTF8String(result.releaseReturnValue());
}

void webkit_dom_document_set_cookie(CyberKitDOMDocument* self, const gchar* value, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOCUMENT(self));
    g_return_if_fail(value);
    g_return_if_fail(!error || !*error);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    auto result = item->setCookie(convertedValue);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

CyberKitDOMHTMLElement* webkit_dom_document_get_body(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLElement> gobjectResult = WTF::getPtr(item->bodyOrFrameset());
    return CyberKit::kit(gobjectResult.get());
}

void webkit_dom_document_set_body(CyberKitDOMDocument* self, CyberKitDOMHTMLElement* value, GError** error)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOCUMENT(self));
    g_return_if_fail(WEBKIT_DOM_IS_HTML_ELEMENT(value));
    g_return_if_fail(!error || !*error);
    CyberCore::Document* item = CyberKit::core(self);
    CyberCore::HTMLElement* convertedValue = CyberKit::core(value);
    auto result = item->setBodyOrFrameset(convertedValue);
    if (result.hasException()) {
        auto description = CyberCore::DOMException::description(result.releaseException().code());
        g_set_error_literal(error, g_quark_from_string("WEBKIT_DOM"), description.legacyCode, description.name);
    }
}

CyberKitDOMHTMLHeadElement* webkit_dom_document_get_head(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLHeadElement> gobjectResult = WTF::getPtr(item->head());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMHTMLCollection* webkit_dom_document_get_images(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->images());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMHTMLCollection* webkit_dom_document_get_applets(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->applets());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMHTMLCollection* webkit_dom_document_get_links(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->links());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMHTMLCollection* webkit_dom_document_get_forms(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->forms());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMHTMLCollection* webkit_dom_document_get_anchors(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->anchors());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMHTMLCollection* webkit_dom_document_get_embeds(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->embeds());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMHTMLCollection* webkit_dom_document_get_plugins(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->plugins());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMHTMLCollection* webkit_dom_document_get_scripts(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->scripts());
    return CyberKit::kit(gobjectResult.get());
}

gchar* webkit_dom_document_get_last_modified(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->lastModified());
    return result;
}

gchar* webkit_dom_document_get_charset(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->charset());
    return result;
}

void webkit_dom_document_set_charset(CyberKitDOMDocument* self, const gchar* value)
{
    CyberCore::JSMainThreadNullState state;
    g_return_if_fail(WEBKIT_DOM_IS_DOCUMENT(self));
    g_return_if_fail(value);
    CyberCore::Document* item = CyberKit::core(self);
    WTF::String convertedValue = WTF::String::fromUTF8(value);
    item->setCharset(convertedValue);
}

gchar* webkit_dom_document_get_ready_state(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);

    auto readyState = CyberKit::core(self)->readyState();
    switch (readyState) {
    case CyberCore::Document::Loading:
        return convertToUTF8String("loading"_s);
    case CyberCore::Document::Interactive:
        return convertToUTF8String("interactive"_s);
    case CyberCore::Document::Complete:
        return convertToUTF8String("complete"_s);
    }
    return 0;
}

gchar* webkit_dom_document_get_character_set(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->characterSetWithUTF8Fallback());
    return result;
}

gchar* webkit_dom_document_get_preferred_stylesheet_set(CyberKitDOMDocument* self)
{
    g_warning("%s: this function has been removed and does nothing", __func__);
    return nullptr;
}

gchar* webkit_dom_document_get_selected_stylesheet_set(CyberKitDOMDocument* self)
{
    g_warning("%s: this function has been removed and does nothing", __func__);
    return nullptr;
}

void webkit_dom_document_set_selected_stylesheet_set(CyberKitDOMDocument* self, const gchar* value)
{
    g_warning("%s: this function has been removed and does nothing", __func__);
}

CyberKitDOMElement* webkit_dom_document_get_active_element(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::Element> gobjectResult = WTF::getPtr(item->activeElement());
    return CyberKit::kit(gobjectResult.get());
}

gchar* webkit_dom_document_get_compat_mode(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->compatMode());
    return result;
}

gboolean webkit_dom_document_get_webkit_is_fullscreen(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), FALSE);
    CyberCore::Document* item = CyberKit::core(self);
    gboolean result = item->fullscreenManager().isFullscreen();
    return result;
}

gboolean webkit_dom_document_get_webkit_fullscreen_keyboard_input_allowed(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), FALSE);
    CyberCore::Document* item = CyberKit::core(self);
    gboolean result = item->fullscreenManager().isFullscreenKeyboardInputAllowed();
    return result;
}

CyberKitDOMElement* webkit_dom_document_get_webkit_current_fullscreen_element(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::Element> gobjectResult = WTF::getPtr(item->fullscreenManager().currentFullscreenElement());
    return CyberKit::kit(gobjectResult.get());
}

gboolean webkit_dom_document_get_webkit_fullscreen_enabled(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), FALSE);
    CyberCore::Document* item = CyberKit::core(self);
    gboolean result = item->fullscreenManager().isFullscreenEnabled();
    return result;
}

CyberKitDOMElement* webkit_dom_document_get_webkit_fullscreen_element(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::Element> gobjectResult = WTF::getPtr(item->fullscreenManager().fullscreenElement());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMElement* webkit_dom_document_get_pointer_lock_element(CyberKitDOMDocument* self)
{
#if ENABLE(POINTER_LOCK)
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::Element> gobjectResult = WTF::getPtr(item->pointerLockElement());
    return CyberKit::kit(gobjectResult.get());
#else
    UNUSED_PARAM(self);
    WEBKIT_WARN_FEATURE_NOT_PRESENT("Pointer Lock")
    return 0;
#endif /* ENABLE(POINTER_LOCK) */
}

gchar* webkit_dom_document_get_visibility_state(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    switch (item->visibilityState()) {
    case CyberCore::VisibilityState::Hidden:
        return convertToUTF8String("hidden");
    case CyberCore::VisibilityState::Visible:
        return convertToUTF8String("visible");
    case CyberCore::VisibilityState::Prerender:
        return convertToUTF8String("prerender");
    }
    ASSERT_NOT_REACHED();
    return nullptr;
}

gboolean webkit_dom_document_get_hidden(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), FALSE);
    CyberCore::Document* item = CyberKit::core(self);
    gboolean result = item->hidden();
    return result;
}

CyberKitDOMHTMLScriptElement* webkit_dom_document_get_current_script(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLScriptElement> gobjectResult = WTF::getPtr(item->currentScript());
    return CyberKit::kit(gobjectResult.get());
}

gchar* webkit_dom_document_get_origin(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gchar* result = convertToUTF8String(item->origin());
    return result;
}

CyberKitDOMElement* webkit_dom_document_get_scrolling_element(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::Element> gobjectResult = WTF::getPtr(item->scrollingElementForAPI());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMHTMLCollection* webkit_dom_document_get_children(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::HTMLCollection> gobjectResult = WTF::getPtr(item->children());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMElement* webkit_dom_document_get_first_element_child(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::Element> gobjectResult = WTF::getPtr(item->firstElementChild());
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMElement* webkit_dom_document_get_last_element_child(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    RefPtr<CyberCore::Element> gobjectResult = WTF::getPtr(item->lastElementChild());
    return CyberKit::kit(gobjectResult.get());
}

gulong webkit_dom_document_get_child_element_count(CyberKitDOMDocument* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_DOCUMENT(self), 0);
    CyberCore::Document* item = CyberKit::core(self);
    gulong result = item->childElementCount();
    return result;
}

G_GNUC_END_IGNORE_DEPRECATIONS;
