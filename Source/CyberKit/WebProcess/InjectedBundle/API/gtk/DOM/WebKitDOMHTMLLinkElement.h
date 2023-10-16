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

#ifndef CyberKitDOMHTMLLinkElement_h
#define CyberKitDOMHTMLLinkElement_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMHTMLElement.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_HTML_LINK_ELEMENT            (webkit_dom_html_link_element_get_type())
#define WEBKIT_DOM_HTML_LINK_ELEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_HTML_LINK_ELEMENT, CyberKitDOMHTMLLinkElement))
#define WEBKIT_DOM_HTML_LINK_ELEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_HTML_LINK_ELEMENT, CyberKitDOMHTMLLinkElementClass)
#define WEBKIT_DOM_IS_HTML_LINK_ELEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_HTML_LINK_ELEMENT))
#define WEBKIT_DOM_IS_HTML_LINK_ELEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_HTML_LINK_ELEMENT))
#define WEBKIT_DOM_HTML_LINK_ELEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_HTML_LINK_ELEMENT, CyberKitDOMHTMLLinkElementClass))

struct _CyberKitDOMHTMLLinkElement {
    CyberKitDOMHTMLElement parent_instance;
};

struct _CyberKitDOMHTMLLinkElementClass {
    CyberKitDOMHTMLElementClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_html_link_element_get_type(void);

/**
 * webkit_dom_html_link_element_get_disabled:
 * @self: A #CyberKitDOMHTMLLinkElement
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_html_link_element_get_disabled(CyberKitDOMHTMLLinkElement* self);

/**
 * webkit_dom_html_link_element_set_disabled:
 * @self: A #CyberKitDOMHTMLLinkElement
 * @value: A #gboolean
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_link_element_set_disabled(CyberKitDOMHTMLLinkElement* self, gboolean value);

/**
 * webkit_dom_html_link_element_get_charset:
 * @self: A #CyberKitDOMHTMLLinkElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_link_element_get_charset(CyberKitDOMHTMLLinkElement* self);

/**
 * webkit_dom_html_link_element_set_charset:
 * @self: A #CyberKitDOMHTMLLinkElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_link_element_set_charset(CyberKitDOMHTMLLinkElement* self, const gchar* value);

/**
 * webkit_dom_html_link_element_get_href:
 * @self: A #CyberKitDOMHTMLLinkElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_link_element_get_href(CyberKitDOMHTMLLinkElement* self);

/**
 * webkit_dom_html_link_element_set_href:
 * @self: A #CyberKitDOMHTMLLinkElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_link_element_set_href(CyberKitDOMHTMLLinkElement* self, const gchar* value);

/**
 * webkit_dom_html_link_element_get_hreflang:
 * @self: A #CyberKitDOMHTMLLinkElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_link_element_get_hreflang(CyberKitDOMHTMLLinkElement* self);

/**
 * webkit_dom_html_link_element_set_hreflang:
 * @self: A #CyberKitDOMHTMLLinkElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_link_element_set_hreflang(CyberKitDOMHTMLLinkElement* self, const gchar* value);

/**
 * webkit_dom_html_link_element_get_media:
 * @self: A #CyberKitDOMHTMLLinkElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_link_element_get_media(CyberKitDOMHTMLLinkElement* self);

/**
 * webkit_dom_html_link_element_set_media:
 * @self: A #CyberKitDOMHTMLLinkElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_link_element_set_media(CyberKitDOMHTMLLinkElement* self, const gchar* value);

/**
 * webkit_dom_html_link_element_get_rel:
 * @self: A #CyberKitDOMHTMLLinkElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_link_element_get_rel(CyberKitDOMHTMLLinkElement* self);

/**
 * webkit_dom_html_link_element_set_rel:
 * @self: A #CyberKitDOMHTMLLinkElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_link_element_set_rel(CyberKitDOMHTMLLinkElement* self, const gchar* value);

/**
 * webkit_dom_html_link_element_get_rev:
 * @self: A #CyberKitDOMHTMLLinkElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_link_element_get_rev(CyberKitDOMHTMLLinkElement* self);

/**
 * webkit_dom_html_link_element_set_rev:
 * @self: A #CyberKitDOMHTMLLinkElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_link_element_set_rev(CyberKitDOMHTMLLinkElement* self, const gchar* value);

/**
 * webkit_dom_html_link_element_get_target:
 * @self: A #CyberKitDOMHTMLLinkElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_link_element_get_target(CyberKitDOMHTMLLinkElement* self);

/**
 * webkit_dom_html_link_element_set_target:
 * @self: A #CyberKitDOMHTMLLinkElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_link_element_set_target(CyberKitDOMHTMLLinkElement* self, const gchar* value);

/**
 * webkit_dom_html_link_element_get_type_attr:
 * @self: A #CyberKitDOMHTMLLinkElement
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_html_link_element_get_type_attr(CyberKitDOMHTMLLinkElement* self);

/**
 * webkit_dom_html_link_element_set_type_attr:
 * @self: A #CyberKitDOMHTMLLinkElement
 * @value: A #gchar
 *
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_link_element_set_type_attr(CyberKitDOMHTMLLinkElement* self, const gchar* value);

/**
 * webkit_dom_html_link_element_get_sheet:
 * @self: A #CyberKitDOMHTMLLinkElement
 *
 * Returns: (transfer full): A #CyberKitDOMStyleSheet
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMStyleSheet*
webkit_dom_html_link_element_get_sheet(CyberKitDOMHTMLLinkElement* self);

/**
 * webkit_dom_html_link_element_get_sizes:
 * @self: A #CyberKitDOMHTMLLinkElement
 *
 * Returns: (transfer full): A #CyberKitDOMDOMTokenList
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMDOMTokenList*
webkit_dom_html_link_element_get_sizes(CyberKitDOMHTMLLinkElement* self);

/**
 * webkit_dom_html_link_element_set_sizes:
 * @self: A #CyberKitDOMHTMLLinkElement
 * @value: a #gchar
 *
 * Since: 2.16
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_html_link_element_set_sizes(CyberKitDOMHTMLLinkElement* self, const gchar* value);

G_END_DECLS

#endif /* CyberKitDOMHTMLLinkElement_h */
