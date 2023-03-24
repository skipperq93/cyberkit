/*
 * Copyright (C) 2017 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2,1 of the License, or (at your option) any later version.
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

#include "config.h"

#include "TestMain.h"
#include <wtf/glib/GUniquePtr.h>

static void testSecurityOriginBasicConstructor(Test*, gconstpointer)
{
    CyberKitSecurityOrigin* origin = webkit_security_origin_new("http", "127.0.0.1", 1234);
    g_assert_nonnull(origin);
    GUniquePtr<char> asString(webkit_security_origin_to_string(origin));
    g_assert_cmpstr(asString.get(), ==, "http://127.0.0.1:1234");
    g_assert_cmpstr(webkit_security_origin_get_protocol(origin), ==, "http");
    g_assert_cmpstr(webkit_security_origin_get_host(origin), ==, "127.0.0.1");
    g_assert_cmpint(webkit_security_origin_get_port(origin), ==, 1234);
    webkit_security_origin_unref(origin);
}

static void testSecurityOriginURIConstructor(Test*, gconstpointer)
{
    CyberKitSecurityOrigin* origin = webkit_security_origin_new_for_uri("http://127.0.0.1:1234");
    g_assert_nonnull(origin);
    GUniquePtr<char> asString(webkit_security_origin_to_string(origin));
    g_assert_cmpstr(asString.get(), ==, "http://127.0.0.1:1234");
    g_assert_cmpstr(webkit_security_origin_get_protocol(origin), ==, "http");
    g_assert_cmpstr(webkit_security_origin_get_host(origin), ==, "127.0.0.1");
    g_assert_cmpint(webkit_security_origin_get_port(origin), ==, 1234);
    webkit_security_origin_unref(origin);

    origin = webkit_security_origin_new_for_uri("http://127.0.0.1:1234/this/path/?should=be#ignored");
    g_assert_nonnull(origin);
    asString.reset(webkit_security_origin_to_string(origin));
    g_assert_cmpstr(asString.get(), ==, "http://127.0.0.1:1234");
    g_assert_cmpstr(webkit_security_origin_get_protocol(origin), ==, "http");
    g_assert_cmpstr(webkit_security_origin_get_host(origin), ==, "127.0.0.1");
    g_assert_cmpint(webkit_security_origin_get_port(origin), ==, 1234);
    webkit_security_origin_unref(origin);
}

static void testSecurityOriginDefaultPort(Test*, gconstpointer)
{
    CyberKitSecurityOrigin* origin = webkit_security_origin_new("http", "127.0.0.1", 0);
    g_assert_nonnull(origin);
    GUniquePtr<char> asString(webkit_security_origin_to_string(origin));
    g_assert_cmpstr(asString.get(), ==, "http://127.0.0.1");
    g_assert_cmpstr(webkit_security_origin_get_protocol(origin), ==, "http");
    g_assert_cmpstr(webkit_security_origin_get_host(origin), ==, "127.0.0.1");
    g_assert_cmpint(webkit_security_origin_get_port(origin), ==, 0);
    webkit_security_origin_unref(origin);

    origin = webkit_security_origin_new("http", "127.0.0.1", 80);
    g_assert_nonnull(origin);
    asString.reset(webkit_security_origin_to_string(origin));
    g_assert_cmpstr(asString.get(), ==, "http://127.0.0.1");
    g_assert_cmpstr(webkit_security_origin_get_protocol(origin), ==, "http");
    g_assert_cmpstr(webkit_security_origin_get_host(origin), ==, "127.0.0.1");
    g_assert_cmpint(webkit_security_origin_get_port(origin), ==, 0);
    webkit_security_origin_unref(origin);

    origin = webkit_security_origin_new_for_uri("http://127.0.0.1");
    g_assert_nonnull(origin);
    asString.reset(webkit_security_origin_to_string(origin));
    g_assert_cmpstr(asString.get(), ==, "http://127.0.0.1");
    g_assert_cmpstr(webkit_security_origin_get_protocol(origin), ==, "http");
    g_assert_cmpstr(webkit_security_origin_get_host(origin), ==, "127.0.0.1");
    g_assert_cmpint(webkit_security_origin_get_port(origin), ==, 0);
    webkit_security_origin_unref(origin);

    origin = webkit_security_origin_new_for_uri("http://127.0.0.1:80");
    g_assert_nonnull(origin);
    asString.reset(webkit_security_origin_to_string(origin));
    g_assert_cmpstr(asString.get(), ==, "http://127.0.0.1");
    g_assert_cmpstr(webkit_security_origin_get_protocol(origin), ==, "http");
    g_assert_cmpstr(webkit_security_origin_get_host(origin), ==, "127.0.0.1");
    g_assert_cmpint(webkit_security_origin_get_port(origin), ==, 0);
    webkit_security_origin_unref(origin);
}

static void testSecurityOriginFileURI(Test*, gconstpointer)
{
    CyberKitSecurityOrigin* origin = webkit_security_origin_new_for_uri("file:///abcdefg");
    g_assert_nonnull(origin);
    GUniquePtr<char> asString(webkit_security_origin_to_string(origin));
    g_assert_cmpstr(asString.get(), ==, "file://");
    g_assert_cmpstr(webkit_security_origin_get_protocol(origin), ==, "file");
    g_assert_null(webkit_security_origin_get_host(origin));
    g_assert_cmpint(webkit_security_origin_get_port(origin), ==, 0);
    webkit_security_origin_unref(origin);
}

static void testSecurityOriginDataURI(Test*, gconstpointer)
{
    CyberKitSecurityOrigin* origin = webkit_security_origin_new_for_uri("data:Lali ho!");
    g_assert_nonnull(origin);
    GUniquePtr<char> asString(webkit_security_origin_to_string(origin));
    g_assert_cmpstr(asString.get(), ==, "data://");
    g_assert_cmpstr(webkit_security_origin_get_protocol(origin), ==, "data");
    g_assert_null(webkit_security_origin_get_host(origin));
    g_assert_cmpint(webkit_security_origin_get_port(origin), ==, 0);
    webkit_security_origin_unref(origin);
}

static void testCustomProtocolOrigin(Test*, gconstpointer)
{
    CyberKitSecurityOrigin* origin = webkit_security_origin_new_for_uri("squirrel://fish");
    g_assert_nonnull(origin);
    GUniquePtr<char> asString(webkit_security_origin_to_string(origin));
    g_assert_cmpstr(asString.get(), ==, "squirrel://fish");
    g_assert_cmpstr(webkit_security_origin_get_protocol(origin), ==, "squirrel");
    g_assert_cmpstr(webkit_security_origin_get_host(origin), ==, "fish");
    g_assert_cmpint(webkit_security_origin_get_port(origin), ==, 0);
    webkit_security_origin_unref(origin);
}

static void testBogusURI(Test*, gconstpointer)
{
    CyberKitSecurityOrigin* origin = webkit_security_origin_new_for_uri("http://localhost:2984375932");
    g_assert_nonnull(origin);
    GUniquePtr<char> asString(webkit_security_origin_to_string(origin));
    g_assert_null(asString.get());
    g_assert_null(webkit_security_origin_get_protocol(origin));
    g_assert_null(webkit_security_origin_get_host(origin));
    g_assert_cmpint(webkit_security_origin_get_port(origin), ==, 0);
    webkit_security_origin_unref(origin);
}

void beforeAll()
{
    Test::add("CyberKitSecurityOrigin", "basic-constructor", testSecurityOriginBasicConstructor);
    Test::add("CyberKitSecurityOrigin", "uri-constructor", testSecurityOriginURIConstructor);
    Test::add("CyberKitSecruityOrigin", "default-port", testSecurityOriginDefaultPort);
    Test::add("CyberKitSecurityOrigin", "file-uri", testSecurityOriginFileURI);
    Test::add("CyberKitSecurityOrigin", "blob-uri", testSecurityOriginDataURI);
    Test::add("CyberKitSecurityOrigin", "custom-protocol-origin", testCustomProtocolOrigin);
    Test::add("CyberKitSecurityOrigin", "bogus-uri", testBogusURI);
}

void afterAll()
{
}
