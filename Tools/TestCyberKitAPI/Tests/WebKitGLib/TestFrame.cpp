/*
 * Copyright (C) 2013 Igalia S.L.
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

#include "WebViewTest.h"

static void testCyberKitFrameMainFrame(WebViewTest* test, gconstpointer)
{
    g_assert_true(test->runWebProcessTest("CyberKitFrame", "main-frame"));
}

static void testCyberKitFrameURI(WebViewTest* test, gconstpointer)
{
    g_assert_true(test->runWebProcessTest("CyberKitFrame", "uri"));
}

static void testCyberKitFrameJavaScriptContext(WebViewTest* test, gconstpointer)
{
    g_assert_true(test->runWebProcessTest("CyberKitFrame", "javascript-context"));
}

static void testCyberKitFrameJavaScriptValues(WebViewTest* test, gconstpointer)
{
    static const char* testHTML = "<html><body><p id='paragraph'>This is a test</p><img id='image' src='foo.png'></body></html>";
    g_assert_true(test->runWebProcessTest("CyberKitFrame", "javascript-values", testHTML));
}

static void testCyberKitFrameSubframe(WebViewTest* test, gconstpointer)
{
    static const char* testHTML = "<html><body><iframe src='resource:///org/webkit/glib/tests/form-in-frame.html' id='frame'></iframe></body></html>";
    g_assert_true(test->runWebProcessTest("CyberKitFrame", "subframe", testHTML));
}

void beforeAll()
{
    WebViewTest::add("CyberKitFrame", "main-frame", testCyberKitFrameMainFrame);
    WebViewTest::add("CyberKitFrame", "uri", testCyberKitFrameURI);
    WebViewTest::add("CyberKitFrame", "javascript-context", testCyberKitFrameJavaScriptContext);
    WebViewTest::add("CyberKitFrame", "javascript-values", testCyberKitFrameJavaScriptValues);
    WebViewTest::add("CyberKitFrame", "subframe", testCyberKitFrameSubframe);
}

void afterAll()
{
}
