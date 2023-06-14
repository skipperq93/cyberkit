/*
 * Copyright (C) 2014 Igalia S.L.
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
#include "CyberKitDOMXPathNSResolver.h"

#include "DOMObjectCache.h"
#include "GObjectXPathNSResolver.h"
#include <CyberCore/JSExecState.h>
#include "CyberKitDOMObject.h"
#include "CyberKitDOMXPathNSResolverPrivate.h"
#include "ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

typedef CyberKitDOMXPathNSResolverIface CyberKitDOMXPathNSResolverInterface;

G_DEFINE_INTERFACE(CyberKitDOMXPathNSResolver, webkit_dom_xpath_ns_resolver, G_TYPE_OBJECT)

static void webkit_dom_xpath_ns_resolver_default_init(CyberKitDOMXPathNSResolverIface*)
{
}

char* webkit_dom_xpath_ns_resolver_lookup_namespace_uri(CyberKitDOMXPathNSResolver* resolver, const char* prefix)
{
    g_return_val_if_fail(WEBKIT_DOM_IS_XPATH_NS_RESOLVER(resolver), nullptr);
    g_return_val_if_fail(prefix, nullptr);

    return WEBKIT_DOM_XPATH_NS_RESOLVER_GET_IFACE(resolver)->lookup_namespace_uri(resolver, prefix);
}

// CyberKitDOMNativeXPathNSResolver.
struct _CyberKitDOMNativeXPathNSResolver {
    CyberKitDOMObject parent;
};

struct _CyberKitDOMNativeXPathNSResolverClass {
    CyberKitDOMObjectClass parentClass;
};

typedef struct _CyberKitDOMNativeXPathNSResolverPrivate {
    RefPtr<CyberCore::XPathNSResolver> coreObject;
} CyberKitDOMNativeXPathNSResolverPrivate;

#define WEBKIT_DOM_NATIVE_XPATH_NS_RESOLVER_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_DOM_TYPE_NATIVE_XPATH_NS_RESOLVER, CyberKitDOMNativeXPathNSResolverPrivate)

static void webkitDOMXPathNSResolverIfaceInit(CyberKitDOMXPathNSResolverIface*);

G_DEFINE_TYPE_WITH_CODE(CyberKitDOMNativeXPathNSResolver, webkit_dom_native_xpath_ns_resolver, WEBKIT_DOM_TYPE_OBJECT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_XPATH_NS_RESOLVER, webkitDOMXPathNSResolverIfaceInit))

static void webkitDOMNativeXPathNSResolverFinalize(GObject* object)
{
    CyberKitDOMNativeXPathNSResolverPrivate* priv = WEBKIT_DOM_NATIVE_XPATH_NS_RESOLVER_GET_PRIVATE(object);
    priv->~CyberKitDOMNativeXPathNSResolverPrivate();
    G_OBJECT_CLASS(webkit_dom_native_xpath_ns_resolver_parent_class)->finalize(object);
}

static GObject* webkitDOMNativeXPathNSResolverConstructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_native_xpath_ns_resolver_parent_class)->constructor(type, constructPropertiesCount, constructProperties);
    CyberKitDOMNativeXPathNSResolverPrivate* priv = WEBKIT_DOM_NATIVE_XPATH_NS_RESOLVER_GET_PRIVATE(object);
    priv->coreObject = static_cast<CyberCore::XPathNSResolver*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    CyberKit::DOMObjectCache::put(priv->coreObject.get(), object);
    return object;
}

static void webkit_dom_native_xpath_ns_resolver_init(CyberKitDOMNativeXPathNSResolver* resolver)
{
    CyberKitDOMNativeXPathNSResolverPrivate* priv = WEBKIT_DOM_NATIVE_XPATH_NS_RESOLVER_GET_PRIVATE(resolver);
    new (priv) CyberKitDOMNativeXPathNSResolverPrivate();
}

static void webkit_dom_native_xpath_ns_resolver_class_init(CyberKitDOMNativeXPathNSResolverClass* klass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(klass);
    g_type_class_add_private(gobjectClass, sizeof(CyberKitDOMNativeXPathNSResolverPrivate));
    gobjectClass->constructor = webkitDOMNativeXPathNSResolverConstructor;
    gobjectClass->finalize = webkitDOMNativeXPathNSResolverFinalize;
}

static char* webkitDOMNativeXPathNSResolverLookupNamespaceURI(CyberKitDOMXPathNSResolver* resolver, const char* prefix)
{
    CyberCore::JSMainThreadNullState state;
    g_return_val_if_fail(WEBKIT_DOM_IS_NATIVE_XPATH_NS_RESOLVER(resolver), nullptr);

    return convertToUTF8String(CyberKit::core(resolver)->lookupNamespaceURI(WTF::String::fromUTF8(prefix)));
}

static void webkitDOMXPathNSResolverIfaceInit(CyberKitDOMXPathNSResolverIface* iface)
{
    iface->lookup_namespace_uri = webkitDOMNativeXPathNSResolverLookupNamespaceURI;
}

namespace CyberKit {

RefPtr<CyberCore::XPathNSResolver> core(CyberKitDOMXPathNSResolver* xPathNSResolver)
{
    if (!xPathNSResolver)
        return nullptr;

    RefPtr<CyberCore::XPathNSResolver> coreResolver;
    if (WEBKIT_DOM_IS_NATIVE_XPATH_NS_RESOLVER(xPathNSResolver))
        coreResolver = core(WEBKIT_DOM_NATIVE_XPATH_NS_RESOLVER(xPathNSResolver));
    else
        coreResolver = CyberKit::GObjectXPathNSResolver::create(xPathNSResolver);
    return coreResolver;
}

CyberKitDOMXPathNSResolver* kit(CyberCore::XPathNSResolver* coreXPathNSResolver)
{
    if (!coreXPathNSResolver)
        return nullptr;

    if (gpointer ret = DOMObjectCache::get(coreXPathNSResolver))
        return WEBKIT_DOM_XPATH_NS_RESOLVER(ret);

    return WEBKIT_DOM_XPATH_NS_RESOLVER(g_object_new(WEBKIT_DOM_TYPE_NATIVE_XPATH_NS_RESOLVER, "core-object", coreXPathNSResolver, nullptr));
}

CyberCore::XPathNSResolver* core(CyberKitDOMNativeXPathNSResolver* xPathNSResolver)
{
    return xPathNSResolver ? static_cast<CyberCore::XPathNSResolver*>(WEBKIT_DOM_OBJECT(xPathNSResolver)->coreObject) : nullptr;
}

} // namespace CyberKit
G_GNUC_END_IGNORE_DEPRECATIONS;
