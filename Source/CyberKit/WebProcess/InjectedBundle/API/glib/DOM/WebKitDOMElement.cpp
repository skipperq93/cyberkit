/*
 *  Copyright (C) 2018 Igalia S.L.
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
#include "CyberKitDOMElement.h"

#include "DOMObjectCache.h"
#include "CyberKitDOMElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include <CyberCore/HTMLInputElement.h>
#include <CyberCore/HTMLTextAreaElement.h>

#if PLATFORM(GTK)
#include "CyberKitDOMEventTarget.h"
#endif

namespace CyberKit {

CyberKitDOMElement* kit(CyberCore::Element* obj)
{
    return WEBKIT_DOM_ELEMENT(kit(static_cast<CyberCore::Node*>(obj)));
}

CyberCore::Element* core(CyberKitDOMElement* element)
{
    return element ? static_cast<CyberCore::Element*>(webkitDOMNodeGetCoreObject(WEBKIT_DOM_NODE(element))) : nullptr;
}

CyberKitDOMElement* wrapElement(CyberCore::Element* coreObject)
{
    ASSERT(coreObject);
#if PLATFORM(GTK)
    return WEBKIT_DOM_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_ELEMENT, "core-object", coreObject, nullptr));
#else
    auto* element = WEBKIT_DOM_ELEMENT(g_object_new(WEBKIT_DOM_TYPE_ELEMENT, nullptr));
    webkitDOMNodeSetCoreObject(WEBKIT_DOM_NODE(element), static_cast<CyberCore::Node*>(coreObject));
    return element;
#endif
}

} // namespace CyberKit

#if PLATFORM(GTK)
G_GNUC_BEGIN_IGNORE_DEPRECATIONS;
G_DEFINE_TYPE_WITH_CODE(CyberKitDOMElement, webkit_dom_element, WEBKIT_DOM_TYPE_NODE, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_EVENT_TARGET, webkitDOMElementDOMEventTargetInit))
G_GNUC_END_IGNORE_DEPRECATIONS;
#else
G_DEFINE_TYPE(CyberKitDOMElement, webkit_dom_element, WEBKIT_DOM_TYPE_NODE)
#endif

static void webkit_dom_element_class_init(CyberKitDOMElementClass* elementClass)
{
#if PLATFORM(GTK)
    GObjectClass* gobjectClass = G_OBJECT_CLASS(elementClass);
    webkitDOMElementInstallProperties(gobjectClass);
#endif
}

static void webkit_dom_element_init(CyberKitDOMElement*)
{
}

/**
 * webkit_dom_element_html_input_element_is_user_edited:
 * @element: a #CyberKitDOMElement
 *
 * Get whether @element is an HTML text input element that has been edited by a user action.
 *
 * Returns: whether @element has been edited by a user action.
 *
 * Since: 2.22
 */
gboolean webkit_dom_element_html_input_element_is_user_edited(CyberKitDOMElement* element)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_ELEMENT(element), FALSE);

    auto* node = webkitDOMNodeGetCoreObject(WEBKIT_DOM_NODE(element));
    if (is<CyberCore::HTMLInputElement>(node))
        return downcast<CyberCore::HTMLInputElement>(*node).lastChangeWasUserEdit();

    if (is<CyberCore::HTMLTextAreaElement>(node))
        return downcast<CyberCore::HTMLTextAreaElement>(*node).lastChangeWasUserEdit();

    return FALSE;
}

/**
 * webkit_dom_element_is_html_input_element_auto_filled:
 * @element: a #CyberKitDOMElement
 *
 * Get whether the element is an HTML input element that has been filled automatically.
 *
 * Returns: whether @element has been filled automatically.
 *
 * Since: 2.22
 */
gboolean webkit_dom_element_html_input_element_get_auto_filled(CyberKitDOMElement* element)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_ELEMENT(element), FALSE);

    auto* node = webkitDOMNodeGetCoreObject(WEBKIT_DOM_NODE(element));
    if (!is<CyberCore::HTMLInputElement>(node))
        return false;

    return downcast<CyberCore::HTMLInputElement>(*node).isAutoFilled();
}

/**
 * webkit_dom_element_html_input_element_set_auto_filled:
 * @element: a #CyberKitDOMElement
 * @auto_filled: value to set
 *
 * Set whether the element is an HTML input element that has been filled automatically.
 * If @element is not an HTML input element this function does nothing.
 *
 * Since: 2.22
 */
void webkit_dom_element_html_input_element_set_auto_filled(CyberKitDOMElement* element, gboolean autoFilled)
{
    g_return_if_fail(WEBKIT_DOM_IS_ELEMENT(element));

    auto* node = webkitDOMNodeGetCoreObject(WEBKIT_DOM_NODE(element));
    if (!is<CyberCore::HTMLInputElement>(node))
        return;

    downcast<CyberCore::HTMLInputElement>(*node).setAutoFilled(autoFilled);
}

/**
 * webkit_dom_element_html_input_element_set_editing_value:
 * @element: a #CyberKitDOMElement
 * @value: the text to set
 *
 * Set editing value of an HTML input element. If @element is not an HTML input element this function does nothing.
 *
 * Since: 2.22
 */
void webkit_dom_element_html_input_element_set_editing_value(CyberKitDOMElement* element, const char* value)
{
    g_return_if_fail(WEBKIT_DOM_IS_ELEMENT(element));

    auto* node = webkitDOMNodeGetCoreObject(WEBKIT_DOM_NODE(element));
    if (!is<CyberCore::HTMLInputElement>(node))
        return;

    downcast<CyberCore::HTMLInputElement>(*node).setEditingValue(String::fromUTF8(value));
}
