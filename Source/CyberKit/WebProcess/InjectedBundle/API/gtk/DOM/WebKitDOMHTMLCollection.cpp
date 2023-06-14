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
#include "CyberKitDOMHTMLCollection.h"

#include <CyberCore/CSSImportRule.h>
#include "DOMObjectCache.h"
#include <CyberCore/Document.h>
#include <CyberCore/ExceptionCode.h>
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMHTMLCollectionPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitDOMPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_HTML_COLLECTION_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_DOM_TYPE_HTML_COLLECTION, CyberKitDOMHTMLCollectionPrivate)

typedef struct _CyberKitDOMHTMLCollectionPrivate {
    RefPtr<CyberCore::HTMLCollection> coreObject;
} CyberKitDOMHTMLCollectionPrivate;

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMHTMLCollection* kit(CyberCore::HTMLCollection* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_HTML_COLLECTION(ret);

    return wrap(obj);
}

CyberCore::HTMLCollection* core(CyberKitDOMHTMLCollection* request)
{
    return request ? static_cast<CyberCore::HTMLCollection*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

CyberKitDOMHTMLCollection* wrapHTMLCollection(CyberCore::HTMLCollection* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_HTML_COLLECTION(g_object_new(WEBKIT_DOM_TYPE_HTML_COLLECTION, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

G_DEFINE_TYPE(CyberKitDOMHTMLCollection, webkit_dom_html_collection, WEBKIT_DOM_TYPE_OBJECT)

enum {
    DOM_HTML_COLLECTION_PROP_0,
    DOM_HTML_COLLECTION_PROP_LENGTH,
};

static void webkit_dom_html_collection_finalize(GObject* object)
{
    CyberKitDOMHTMLCollectionPrivate* priv = WEBKIT_DOM_HTML_COLLECTION_GET_PRIVATE(object);

    CyberKit::DOMObjectCache::forget(priv->coreObject.get());

    priv->~CyberKitDOMHTMLCollectionPrivate();
    G_OBJECT_CLASS(webkit_dom_html_collection_parent_class)->finalize(object);
}

static void webkit_dom_html_collection_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMHTMLCollection* self = WEBKIT_DOM_HTML_COLLECTION(object);

    switch (propertyId) {
    case DOM_HTML_COLLECTION_PROP_LENGTH:
        g_value_set_ulong(value, webkit_dom_html_collection_get_length(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static GObject* webkit_dom_html_collection_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_html_collection_parent_class)->constructor(type, constructPropertiesCount, constructProperties);

    CyberKitDOMHTMLCollectionPrivate* priv = WEBKIT_DOM_HTML_COLLECTION_GET_PRIVATE(object);
    priv->coreObject = static_cast<CyberCore::HTMLCollection*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    CyberKit::DOMObjectCache::put(priv->coreObject.get(), object);

    return object;
}

static void webkit_dom_html_collection_class_init(CyberKitDOMHTMLCollectionClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(CyberKitDOMHTMLCollectionPrivate));
    gobjectClass->constructor = webkit_dom_html_collection_constructor;
    gobjectClass->finalize = webkit_dom_html_collection_finalize;
    gobjectClass->get_property = webkit_dom_html_collection_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_HTML_COLLECTION_PROP_LENGTH,
        g_param_spec_ulong(
            "length",
            "HTMLCollection:length",
            "read-only gulong HTMLCollection:length",
            0, G_MAXULONG, 0,
            WEBKIT_PARAM_READABLE));

}

static void webkit_dom_html_collection_init(CyberKitDOMHTMLCollection* request)
{
    CyberKitDOMHTMLCollectionPrivate* priv = WEBKIT_DOM_HTML_COLLECTION_GET_PRIVATE(request);
    new (priv) CyberKitDOMHTMLCollectionPrivate();
}

CyberKitDOMNode* webkit_dom_html_collection_item(CyberKitDOMHTMLCollection* self, gulong index)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_COLLECTION(self), 0);
    CyberCore::HTMLCollection* item = CyberKit::core(self);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->item(index));
    return CyberKit::kit(gobjectResult.get());
}

CyberKitDOMNode* webkit_dom_html_collection_named_item(CyberKitDOMHTMLCollection* self, const gchar* name)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_COLLECTION(self), 0);
    g_return_val_if_fail(name, 0);
    CyberCore::HTMLCollection* item = CyberKit::core(self);
    WTF::String convertedName = WTF::String::fromUTF8(name);
    RefPtr<CyberCore::Node> gobjectResult = WTF::getPtr(item->namedItem(convertedName));
    return CyberKit::kit(gobjectResult.get());
}

gulong webkit_dom_html_collection_get_length(CyberKitDOMHTMLCollection* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_HTML_COLLECTION(self), 0);
    CyberCore::HTMLCollection* item = CyberKit::core(self);
    gulong result = item->length();
    return result;
}

G_GNUC_END_IGNORE_DEPRECATIONS;
