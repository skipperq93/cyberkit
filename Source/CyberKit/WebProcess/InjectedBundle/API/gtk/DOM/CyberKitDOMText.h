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

#ifndef CyberKitDOMText_h
#define CyberKitDOMText_h

#include <glib-object.h>
#include <webkitdom/CyberKitDOMCharacterData.h>
#include <webkitdom/webkitdomdefines.h>

G_BEGIN_DECLS

#define WEBKIT_DOM_TYPE_TEXT            (webkit_dom_text_get_type())
#define WEBKIT_DOM_TEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_DOM_TYPE_TEXT, CyberKitDOMText))
#define WEBKIT_DOM_TEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_DOM_TYPE_TEXT, CyberKitDOMTextClass)
#define WEBKIT_DOM_IS_TEXT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_DOM_TYPE_TEXT))
#define WEBKIT_DOM_IS_TEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_DOM_TYPE_TEXT))
#define WEBKIT_DOM_TEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_DOM_TYPE_TEXT, CyberKitDOMTextClass))

struct _CyberKitDOMText {
    CyberKitDOMCharacterData parent_instance;
};

struct _CyberKitDOMTextClass {
    CyberKitDOMCharacterDataClass parent_class;
};

WEBKIT_DEPRECATED GType
webkit_dom_text_get_type(void);

/**
 * webkit_dom_text_split_text:
 * @self: A #CyberKitDOMText
 * @offset: A #gulong
 * @error: #GError
 *
 * Returns: (transfer none): A #CyberKitDOMText
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED CyberKitDOMText*
webkit_dom_text_split_text(CyberKitDOMText* self, gulong offset, GError** error);

/**
 * webkit_dom_text_get_whole_text:
 * @self: A #CyberKitDOMText
 *
 * Returns: A #gchar
 *
 * Deprecated: 2.22: Use CyberScriptCore API instead
**/
WEBKIT_DEPRECATED gchar*
webkit_dom_text_get_whole_text(CyberKitDOMText* self);

G_END_DECLS

#endif /* CyberKitDOMText_h */
