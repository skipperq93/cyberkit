/*
 * Copyright (C) 2012 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef CyberKitSoupRequestGeneric_h
#define CyberKitSoupRequestGeneric_h

#include "CyberKitSoupRequestGenericClient.h"
#include <glib-object.h>
#include <libsoup/soup.h>

G_BEGIN_DECLS

namespace CyberCore {
class ResourceRequest;
}

#define WEBKIT_TYPE_SOUP_REQUEST_GENERIC            (webkit_soup_request_generic_get_type())
#define WEBKIT_SOUP_REQUEST_GENERIC(object)         (G_TYPE_CHECK_INSTANCE_CAST((object), WEBKIT_TYPE_SOUP_REQUEST_GENERIC, CyberKitSoupRequestGeneric))
#define WEBKIT_IS_SOUP_REQUEST_GENERIC(object)      (G_TYPE_CHECK_INSTANCE_TYPE((object), WEBKIT_TYPE_SOUP_REQUEST_GENERIC))
#define WEBKIT_SOUP_REQUEST_GENERIC_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), WEBKIT_TYPE_SOUP_REQUEST_GENERIC, CyberKitSoupRequestGenericClass))
#define WEBKIT_IS_SOUP_REQUEST_GENERIC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), WEBKIT_TYPE_SOUP_REQUEST_GENERIC))
#define WEBKIT_SOUP_REQUEST_GENERIC_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), WEBKIT_TYPE_SOUP_REQUEST_GENERIC, CyberKitSoupRequestGenericClass))

typedef struct _CyberKitSoupRequestGeneric CyberKitSoupRequestGeneric;
typedef struct _CyberKitSoupRequestGenericClass CyberKitSoupRequestGenericClass;
typedef struct _CyberKitSoupRequestGenericPrivate CyberKitSoupRequestGenericPrivate;

struct _CyberKitSoupRequestGeneric {
    SoupRequest parent;

    CyberKitSoupRequestGenericPrivate *priv;
};

struct _CyberKitSoupRequestGenericClass {
    SoupRequestClass parent;

    CyberCore::CyberKitSoupRequestGenericClient* client;
};

GType webkit_soup_request_generic_get_type();

G_END_DECLS

void webkitSoupRequestGenericSetContentLength(CyberKitSoupRequestGeneric*, goffset contentLength);
void webkitSoupRequestGenericSetContentType(CyberKitSoupRequestGeneric*, const char* mimeType);
void webkitSoupRequestGenericSetRequest(CyberKitSoupRequestGeneric*, const CyberCore::ResourceRequest&);
const CyberCore::ResourceRequest& webkitSoupRequestGenericGetRequest(CyberKitSoupRequestGeneric*);

#endif // CyberKitSoupRequestGeneric_h
