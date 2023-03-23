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

#ifndef CyberKitDOMCSSStyleDeclaration_h
#define CyberKitDOMCSSStyleDeclaration_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMObject.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_CSS_STYLE_DECLARATION            (webkit_dom_css_style_declaration_get_type())
#define WEBKIT_DOM_CSS_STYLE_DECLARATION(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_CSS_STYLE_DECLARATION, CyberKitDOMCSSStyleDeclaration))
#define WEBKIT_DOM_CSS_STYLE_DECLARATION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_CSS_STYLE_DECLARATION, CyberKitDOMCSSStyleDeclarationClass)
#define WEBKIT_DOM_IS_CSS_STYLE_DECLARATION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_CSS_STYLE_DECLARATION))
#define WEBKIT_DOM_IS_CSS_STYLE_DECLARATION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_CSS_STYLE_DECLARATION))
#define WEBKIT_DOM_CSS_STYLE_DECLARATION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_CSS_STYLE_DECLARATION, CyberKitDOMCSSStyleDeclarationClass))

struct _CyberKitDOMCSSStyleDeclaration {
    CyberKitDOMObject parent_instance;
};

struct _CyberKitDOMCSSStyleDeclarationClass {
    CyberKitDOMObjectClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_css_style_declaration_get_type(void);

/**
 * webkit_dom_css_style_declaration_get_property_value:
 * @self: A #CyberKitDOMCSSStyleDeclaration
 * @propertyName: A #gchar
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_css_style_declaration_get_property_value(CyberKitDOMCSSStyleDeclaration* self, const gchar* propertyName);

/**
 * webkit_dom_css_style_declaration_remove_property:
 * @self: A #CyberKitDOMCSSStyleDeclaration
 * @propertyName: A #gchar
 * @error: #GError
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_css_style_declaration_remove_property(CyberKitDOMCSSStyleDeclaration* self, const gchar* propertyName, GError** error);

/**
 * webkit_dom_css_style_declaration_get_property_priority:
 * @self: A #CyberKitDOMCSSStyleDeclaration
 * @propertyName: A #gchar
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_css_style_declaration_get_property_priority(CyberKitDOMCSSStyleDeclaration* self, const gchar* propertyName);

/**
 * webkit_dom_css_style_declaration_set_property:
 * @self: A #CyberKitDOMCSSStyleDeclaration
 * @propertyName: A #gchar
 * @value: A #gchar
 * @priority: A #gchar
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_css_style_declaration_set_property(CyberKitDOMCSSStyleDeclaration* self, const gchar* propertyName, const gchar* value, const gchar* priority, GError** error);

/**
 * webkit_dom_css_style_declaration_item:
 * @self: A #CyberKitDOMCSSStyleDeclaration
 * @index: A #gulong
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_css_style_declaration_item(CyberKitDOMCSSStyleDeclaration* self, gulong index);

/**
 * webkit_dom_css_style_declaration_get_property_shorthand:
 * @self: A #CyberKitDOMCSSStyleDeclaration
 * @propertyName: A #gchar
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_css_style_declaration_get_property_shorthand(CyberKitDOMCSSStyleDeclaration* self, const gchar* propertyName);

/**
 * webkit_dom_css_style_declaration_is_property_implicit:
 * @self: A #CyberKitDOMCSSStyleDeclaration
 * @propertyName: A #gchar
 *
 * Returns: A #gboolean
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gboolean
webkit_dom_css_style_declaration_is_property_implicit(CyberKitDOMCSSStyleDeclaration* self, const gchar* propertyName);

/**
 * webkit_dom_css_style_declaration_get_css_text:
 * @self: A #CyberKitDOMCSSStyleDeclaration
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_css_style_declaration_get_css_text(CyberKitDOMCSSStyleDeclaration* self);

/**
 * webkit_dom_css_style_declaration_set_css_text:
 * @self: A #CyberKitDOMCSSStyleDeclaration
 * @value: A #gchar
 * @error: #GError
 *
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED void
webkit_dom_css_style_declaration_set_css_text(CyberKitDOMCSSStyleDeclaration* self, const gchar* value, GError** error);

/**
 * webkit_dom_css_style_declaration_get_length:
 * @self: A #CyberKitDOMCSSStyleDeclaration
 *
 * Returns: A #gulong
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED gulong
webkit_dom_css_style_declaration_get_length(CyberKitDOMCSSStyleDeclaration* self);

/**
 * webkit_dom_css_style_declaration_get_parent_rule:
 * @self: A #CyberKitDOMCSSStyleDeclaration
 *
 * Returns: (transfer full): A #CyberKitDOMCSSRule
 *
 * Deprecated: 2.22: Use JavaScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMCSSRule*
webkit_dom_css_style_declaration_get_parent_rule(CyberKitDOMCSSStyleDeclaration* self);

G_END_DECLS

#endif /* CyberKitDOMCSSStyleDeclaration_h */
