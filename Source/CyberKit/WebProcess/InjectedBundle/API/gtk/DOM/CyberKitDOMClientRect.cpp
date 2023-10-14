/*
 *  Copyright (C) 2017 Aidan Holm <aidanholm@gmail.com>
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
#include "CyberKitDOMClientRect.h"

#include "ConvertToUTF8String.h"
#include "DOMObjectCache.h"
#include "CyberKitDOMClientRectPrivate.h"
#include "CyberKitDOMPrivate.h"
#include <CyberCore/CSSImportRule.h>
#include <CyberCore/Document.h>
#include <CyberCore/ExceptionCode.h>
#include <CyberCore/JSExecState.h>
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_CLIENT_RECT_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_DOM_TYPE_CLIENT_RECT, CyberKitDOMClientRectPrivate)

typedef struct _CyberKitDOMClientRectPrivate {
    RefPtr<CyberCore::DOMRect> coreObject;
} CyberKitDOMClientRectPrivate;

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

CyberKitDOMClientRect* kit(CyberCore::DOMRect* obj)
{
    if (!obj)
        return nullptr;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_CLIENT_RECT(ret);

    return wrapClientRect(obj);
}

CyberCore::DOMRect* core(CyberKitDOMClientRect* request)
{
    return request ? static_cast<CyberCore::DOMRect*>(WEBKIT_DOM_OBJECT(request)->coreObject) : nullptr;
}

CyberKitDOMClientRect* wrapClientRect(CyberCore::DOMRect* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_CLIENT_RECT(g_object_new(WEBKIT_DOM_TYPE_CLIENT_RECT, "core-object", coreObject, nullptr));
}

} // namespace CyberKit

G_DEFINE_TYPE(CyberKitDOMClientRect, webkit_dom_client_rect, WEBKIT_DOM_TYPE_OBJECT)

enum {
    DOM_CLIENT_RECT_PROP_0,
    DOM_CLIENT_RECT_PROP_TOP,
    DOM_CLIENT_RECT_PROP_RIGHT,
    DOM_CLIENT_RECT_PROP_BOTTOM,
    DOM_CLIENT_RECT_PROP_LEFT,
    DOM_CLIENT_RECT_PROP_WIDTH,
    DOM_CLIENT_RECT_PROP_HEIGHT,
};

static void webkit_dom_client_rect_finalize(GObject* object)
{
    CyberKitDOMClientRectPrivate* priv = WEBKIT_DOM_CLIENT_RECT_GET_PRIVATE(object);

    CyberKit::DOMObjectCache::forget(priv->coreObject.get());

    priv->~CyberKitDOMClientRectPrivate();
    G_OBJECT_CLASS(webkit_dom_client_rect_parent_class)->finalize(object);
}

static void webkit_dom_client_rect_get_property(GObject* object, guint propertyId, GValue* value, GParamSpec* pspec)
{
    CyberKitDOMClientRect* self = WEBKIT_DOM_CLIENT_RECT(object);

    switch (propertyId) {
    case DOM_CLIENT_RECT_PROP_TOP:
        g_value_set_float(value, webkit_dom_client_rect_get_top(self));
        break;
    case DOM_CLIENT_RECT_PROP_RIGHT:
        g_value_set_float(value, webkit_dom_client_rect_get_right(self));
        break;
    case DOM_CLIENT_RECT_PROP_BOTTOM:
        g_value_set_float(value, webkit_dom_client_rect_get_bottom(self));
        break;
    case DOM_CLIENT_RECT_PROP_LEFT:
        g_value_set_float(value, webkit_dom_client_rect_get_left(self));
        break;
    case DOM_CLIENT_RECT_PROP_WIDTH:
        g_value_set_float(value, webkit_dom_client_rect_get_width(self));
        break;
    case DOM_CLIENT_RECT_PROP_HEIGHT:
        g_value_set_float(value, webkit_dom_client_rect_get_height(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, pspec);
        break;
    }
}

static void webkit_dom_client_rect_constructed(GObject* object)
{
    G_OBJECT_CLASS(webkit_dom_client_rect_parent_class)->constructed(object);

    CyberKitDOMClientRectPrivate* priv = WEBKIT_DOM_CLIENT_RECT_GET_PRIVATE(object);
    priv->coreObject = static_cast<CyberCore::DOMRect*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    CyberKit::DOMObjectCache::put(priv->coreObject.get(), object);
}

static void webkit_dom_client_rect_class_init(CyberKitDOMClientRectClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(CyberKitDOMClientRectPrivate));
    gobjectClass->constructed = webkit_dom_client_rect_constructed;
    gobjectClass->finalize = webkit_dom_client_rect_finalize;
    gobjectClass->get_property = webkit_dom_client_rect_get_property;

    g_object_class_install_property(
        gobjectClass,
        DOM_CLIENT_RECT_PROP_TOP,
        g_param_spec_float(
            "top",
            "ClientRect:top",
            "read-only gfloat ClientRect:top",
            -G_MAXFLOAT, G_MAXFLOAT, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_CLIENT_RECT_PROP_RIGHT,
        g_param_spec_float(
            "right",
            "ClientRect:right",
            "read-only gfloat ClientRect:right",
            -G_MAXFLOAT, G_MAXFLOAT, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_CLIENT_RECT_PROP_BOTTOM,
        g_param_spec_float(
            "bottom",
            "ClientRect:bottom",
            "read-only gfloat ClientRect:bottom",
            -G_MAXFLOAT, G_MAXFLOAT, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_CLIENT_RECT_PROP_LEFT,
        g_param_spec_float(
            "left",
            "ClientRect:left",
            "read-only gfloat ClientRect:left",
            -G_MAXFLOAT, G_MAXFLOAT, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_CLIENT_RECT_PROP_WIDTH,
        g_param_spec_float(
            "width",
            "ClientRect:width",
            "read-only gfloat ClientRect:width",
            0, G_MAXFLOAT, 0,
            WEBKIT_PARAM_READABLE));

    g_object_class_install_property(
        gobjectClass,
        DOM_CLIENT_RECT_PROP_HEIGHT,
        g_param_spec_float(
            "height",
            "ClientRect:height",
            "read-only gfloat ClientRect:height",
            0, G_MAXFLOAT, 0,
            WEBKIT_PARAM_READABLE));

}

static void webkit_dom_client_rect_init(CyberKitDOMClientRect* request)
{
    CyberKitDOMClientRectPrivate* priv = WEBKIT_DOM_CLIENT_RECT_GET_PRIVATE(request);
    new (priv) CyberKitDOMClientRectPrivate();
}

gfloat webkit_dom_client_rect_get_top(CyberKitDOMClientRect* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CLIENT_RECT(self), 0);
    return CyberKit::core(self)->top();
}

gfloat webkit_dom_client_rect_get_right(CyberKitDOMClientRect* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CLIENT_RECT(self), 0);
    return CyberKit::core(self)->right();
}

gfloat webkit_dom_client_rect_get_bottom(CyberKitDOMClientRect* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CLIENT_RECT(self), 0);
    return CyberKit::core(self)->bottom();
}

gfloat webkit_dom_client_rect_get_left(CyberKitDOMClientRect* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CLIENT_RECT(self), 0);
    return CyberKit::core(self)->left();
}

gfloat webkit_dom_client_rect_get_width(CyberKitDOMClientRect* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CLIENT_RECT(self), 0);
    return CyberKit::core(self)->width();
}

gfloat webkit_dom_client_rect_get_height(CyberKitDOMClientRect* self)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_CLIENT_RECT(self), 0);
    return CyberKit::core(self)->height();
}
G_GNUC_END_IGNORE_DEPRECATIONS;
