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

#ifndef CyberKitSoupRequestInputStream_h
#define CyberKitSoupRequestInputStream_h

#include <CyberCore/ResourceError.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_SOUP_REQUEST_INPUT_STREAM            (webkit_soup_request_input_stream_get_type())
#define WEBKIT_SOUP_REQUEST_INPUT_STREAM(object)         (G_TYPE_CHECK_INSTANCE_CAST((object), WEBKIT_TYPE_SOUP_REQUEST_INPUT_STREAM, CyberKitSoupRequestInputStream))
#define WEBKIT_IS_SOUP_REQUEST_INPUT_STREAM(object)      (G_TYPE_CHECK_INSTANCE_TYPE((object), WEBKIT_TYPE_SOUP_REQUEST_INPUT_STREAM))
#define WEBKIT_SOUP_REQUEST_INPUT_STREAM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), WEBKIT_TYPE_SOUP_REQUEST_INPUT_STREAM, CyberKitSoupRequestInputStreamClass))
#define WEBKIT_IS_SOUP_REQUEST_INPUT_STREAM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), WEBKIT_TYPE_SOUP_REQUEST_INPUT_STREAM))
#define WEBKIT_SOUP_REQUEST_INPUT_STREAM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), WEBKIT_TYPE_SOUP_REQUEST_INPUT_STREAM, CyberKitSoupRequestInputStreamClass))

typedef struct _CyberKitSoupRequestInputStream CyberKitSoupRequestInputStream;
typedef struct _CyberKitSoupRequestInputStreamClass CyberKitSoupRequestInputStreamClass;
typedef struct _CyberKitSoupRequestInputStreamPrivate CyberKitSoupRequestInputStreamPrivate;

struct _CyberKitSoupRequestInputStream {
    GMemoryInputStream parent;

    CyberKitSoupRequestInputStreamPrivate* priv;
};

struct _CyberKitSoupRequestInputStreamClass {
    GMemoryInputStreamClass parent;
};

GType webkit_soup_request_input_stream_get_type();
GInputStream* webkitSoupRequestInputStreamNew(uint64_t contentLength);
void webkitSoupRequestInputStreamAddData(CyberKitSoupRequestInputStream*, const void* data, size_t dataLength);
void webkitSoupRequestInputStreamDidFailWithError(CyberKitSoupRequestInputStream*, const CyberCore::ResourceError&);
bool webkitSoupRequestInputStreamFinished(CyberKitSoupRequestInputStream*);

G_END_DECLS

#endif // CyberKitSoupRequestInputStream_h
