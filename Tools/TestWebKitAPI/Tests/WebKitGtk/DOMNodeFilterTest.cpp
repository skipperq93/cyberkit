/*
 * Copyright (C) 2013 Igalia S.L.
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

#include "config.h"

#include "WebProcessTest.h"
#include <gio/gio.h>
#include <wtf/glib/GUniquePtr.h>

#if USE(GTK4)
#include <webkit/webkit-web-process-extension.h>
#else
#include <webkit2/webkit-web-extension.h>
#endif

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

typedef struct _CyberKitNodeFilter {
    GObject parent;
} CyberKitNodeFilter;

typedef struct _CyberKitNodeFilterClass {
    GObjectClass parentClass;
} CyberKitNodeFilterClass;

static short webkitNodeFilterAcceptNode(CyberKitDOMNodeFilter*, CyberKitDOMNode* node)
{
    // Filter out input elements.
    return WEBKIT_DOM_IS_HTML_INPUT_ELEMENT(node) ? WEBKIT_DOM_NODE_FILTER_REJECT : WEBKIT_DOM_NODE_FILTER_ACCEPT;
}

static void webkitNodeFilterDOMNodeFilterIfaceInit(CyberKitDOMNodeFilterIface* iface)
{
    iface->accept_node = webkitNodeFilterAcceptNode;
}

G_DEFINE_TYPE_WITH_CODE(CyberKitNodeFilter, webkit_node_filter, G_TYPE_OBJECT, G_IMPLEMENT_INTERFACE(WEBKIT_DOM_TYPE_NODE_FILTER, webkitNodeFilterDOMNodeFilterIfaceInit))

static void webkit_node_filter_init(CyberKitNodeFilter*)
{
}

static void webkit_node_filter_class_init(CyberKitNodeFilterClass*)
{
}

static const char* expectedNodesAll[] = { "HTML", "HEAD", "TITLE", "#text", "BODY", "INPUT", "INPUT", "BR" };
static const char* expectedNodesNoInput[] = { "HTML", "HEAD", "TITLE", "#text", "BODY", "BR" };
static const char* expectedElementsNoInput[] = { "HTML", "HEAD", "TITLE", "BODY", "BR" };

class CyberKitDOMNodeFilterTest : public WebProcessTest {
public:
    static std::unique_ptr<WebProcessTest> create() { return std::unique_ptr<WebProcessTest>(new CyberKitDOMNodeFilterTest()); }

private:
    bool testTreeWalker(CyberKitWebPage* page)
    {
        CyberKitDOMDocument* document = webkit_web_page_get_dom_document(page);
        g_assert_true(WEBKIT_DOM_IS_DOCUMENT(document));
        assertObjectIsDeletedWhenTestFinishes(G_OBJECT(document));

        CyberKitDOMElement* root = webkit_dom_document_get_element_by_id(document, "root");
        g_assert_true(WEBKIT_DOM_IS_NODE(root));
        assertObjectIsDeletedWhenTestFinishes(G_OBJECT(root));

        // No filter.
        GRefPtr<CyberKitDOMTreeWalker> walker = adoptGRef(webkit_dom_document_create_tree_walker(document, WEBKIT_DOM_NODE(root), WEBKIT_DOM_NODE_FILTER_SHOW_ALL, nullptr, FALSE, nullptr));
        g_assert_true(WEBKIT_DOM_IS_TREE_WALKER(walker.get()));
        assertObjectIsDeletedWhenTestFinishes(G_OBJECT(walker.get()));
        g_assert_null(webkit_dom_tree_walker_get_filter(walker.get()));

        unsigned i = 0;
        for (CyberKitDOMNode* node = WEBKIT_DOM_NODE(root); node; node = webkit_dom_tree_walker_next_node(walker.get()), ++i) {
            assertObjectIsDeletedWhenTestFinishes(G_OBJECT(node));
            g_assert_cmpuint(i, <, G_N_ELEMENTS(expectedNodesAll));
            GUniquePtr<char> nodeName(webkit_dom_node_get_node_name(node));
            g_assert_cmpstr(nodeName.get(), ==, expectedNodesAll[i]);
        }
        g_assert_cmpuint(i, ==, G_N_ELEMENTS(expectedNodesAll));

        // Input elements filter.
        GRefPtr<CyberKitDOMNodeFilter> filter = adoptGRef(static_cast<CyberKitDOMNodeFilter*>(g_object_new(webkit_node_filter_get_type(), nullptr)));
        walker = adoptGRef(webkit_dom_document_create_tree_walker(document, WEBKIT_DOM_NODE(root), WEBKIT_DOM_NODE_FILTER_SHOW_ALL, filter.get(), FALSE, nullptr));
        g_assert_true(WEBKIT_DOM_IS_TREE_WALKER(walker.get()));
        assertObjectIsDeletedWhenTestFinishes(G_OBJECT(filter.get()));
        g_assert_true(webkit_dom_tree_walker_get_filter(walker.get()) == filter.get());

        i = 0;
        for (CyberKitDOMNode* node = WEBKIT_DOM_NODE(root); node; node = webkit_dom_tree_walker_next_node(walker.get()), ++i) {
            assertObjectIsDeletedWhenTestFinishes(G_OBJECT(node));
            g_assert_cmpuint(i, <, G_N_ELEMENTS(expectedNodesNoInput));
            GUniquePtr<char> nodeName(webkit_dom_node_get_node_name(node));
            g_assert_cmpstr(nodeName.get(), ==, expectedNodesNoInput[i]);
        }
        g_assert_cmpuint(i, ==, G_N_ELEMENTS(expectedNodesNoInput));

        // Show only elements, reusing the input filter.
        walker = adoptGRef(webkit_dom_document_create_tree_walker(document, WEBKIT_DOM_NODE(root), WEBKIT_DOM_NODE_FILTER_SHOW_ELEMENT, filter.get(), FALSE, nullptr));
        g_assert_true(WEBKIT_DOM_IS_TREE_WALKER(walker.get()));
        assertObjectIsDeletedWhenTestFinishes(G_OBJECT(walker.get()));
        g_assert_true(webkit_dom_tree_walker_get_filter(walker.get()) == filter.get());

        i = 0;
        for (CyberKitDOMNode* node = WEBKIT_DOM_NODE(root); node; node = webkit_dom_tree_walker_next_node(walker.get()), ++i) {
            assertObjectIsDeletedWhenTestFinishes(G_OBJECT(node));
            g_assert_cmpuint(i, <, G_N_ELEMENTS(expectedElementsNoInput));
            GUniquePtr<char> nodeName(webkit_dom_node_get_node_name(node));
            g_assert_cmpstr(nodeName.get(), ==, expectedElementsNoInput[i]);
        }
        g_assert_cmpuint(i, ==, G_N_ELEMENTS(expectedElementsNoInput));

        return true;
    }

    bool testNodeIterator(CyberKitWebPage* page)
    {
        CyberKitDOMDocument* document = webkit_web_page_get_dom_document(page);
        g_assert_true(WEBKIT_DOM_IS_DOCUMENT(document));
        assertObjectIsDeletedWhenTestFinishes(G_OBJECT(document));

        CyberKitDOMElement* root = webkit_dom_document_get_element_by_id(document, "root");
        g_assert_true(WEBKIT_DOM_IS_NODE(root));
        assertObjectIsDeletedWhenTestFinishes(G_OBJECT(root));

        // No filter.
        GRefPtr<CyberKitDOMNodeIterator> iter = adoptGRef(webkit_dom_document_create_node_iterator(document, WEBKIT_DOM_NODE(root), WEBKIT_DOM_NODE_FILTER_SHOW_ALL, nullptr, FALSE, nullptr));
        g_assert_true(WEBKIT_DOM_IS_NODE_ITERATOR(iter.get()));
        assertObjectIsDeletedWhenTestFinishes(G_OBJECT(iter.get()));
        g_assert_null(webkit_dom_node_iterator_get_filter(iter.get()));

        unsigned i = 0;
        while (CyberKitDOMNode* node = webkit_dom_node_iterator_next_node(iter.get(), nullptr)) {
            assertObjectIsDeletedWhenTestFinishes(G_OBJECT(node));
            g_assert_cmpuint(i, <, G_N_ELEMENTS(expectedNodesAll));
            GUniquePtr<char> nodeName(webkit_dom_node_get_node_name(node));
            g_assert_cmpstr(nodeName.get(), ==, expectedNodesAll[i]);
            i++;
        }
        g_assert_cmpuint(i, ==, G_N_ELEMENTS(expectedNodesAll));

        // Input elements filter.
        GRefPtr<CyberKitDOMNodeFilter> filter = adoptGRef(static_cast<CyberKitDOMNodeFilter*>(g_object_new(webkit_node_filter_get_type(), nullptr)));
        iter = adoptGRef(webkit_dom_document_create_node_iterator(document, WEBKIT_DOM_NODE(root), WEBKIT_DOM_NODE_FILTER_SHOW_ALL, filter.get(), FALSE, nullptr));
        g_assert_true(WEBKIT_DOM_IS_NODE_ITERATOR(iter.get()));
        assertObjectIsDeletedWhenTestFinishes(G_OBJECT(iter.get()));
        g_assert_true(webkit_dom_node_iterator_get_filter(iter.get()) == filter.get());

        i = 0;
        while (CyberKitDOMNode* node = webkit_dom_node_iterator_next_node(iter.get(), nullptr)) {
            assertObjectIsDeletedWhenTestFinishes(G_OBJECT(node));
            g_assert_cmpuint(i, <, G_N_ELEMENTS(expectedNodesNoInput));
            GUniquePtr<char> nodeName(webkit_dom_node_get_node_name(node));
            g_assert_cmpstr(nodeName.get(), ==, expectedNodesNoInput[i]);
            i++;
        }
        g_assert_cmpuint(i, ==, G_N_ELEMENTS(expectedNodesNoInput));

        // Show only elements, reusing the input filter.
        iter = adoptGRef(webkit_dom_document_create_node_iterator(document, WEBKIT_DOM_NODE(root), WEBKIT_DOM_NODE_FILTER_SHOW_ELEMENT, filter.get(), FALSE, nullptr));
        g_assert_true(WEBKIT_DOM_IS_NODE_ITERATOR(iter.get()));
        assertObjectIsDeletedWhenTestFinishes(G_OBJECT(iter.get()));
        g_assert_true(webkit_dom_node_iterator_get_filter(iter.get()) == filter.get());

        i = 0;
        while (CyberKitDOMNode* node = webkit_dom_node_iterator_next_node(iter.get(), nullptr)) {
            assertObjectIsDeletedWhenTestFinishes(G_OBJECT(node));
            g_assert_cmpuint(i, <, G_N_ELEMENTS(expectedElementsNoInput));
            GUniquePtr<char> nodeName(webkit_dom_node_get_node_name(node));
            g_assert_cmpstr(nodeName.get(), ==, expectedElementsNoInput[i]);
            i++;
        }
        g_assert_cmpuint(i, ==, G_N_ELEMENTS(expectedElementsNoInput));

        return true;
    }

    bool runTest(const char* testName, CyberKitWebPage* page) override
    {
        if (!strcmp(testName, "tree-walker"))
            return testTreeWalker(page);
        if (!strcmp(testName, "node-iterator"))
            return testNodeIterator(page);

        g_assert_not_reached();
        return false;
    }
};

static void __attribute__((constructor)) registerTests()
{
    REGISTER_TEST(CyberKitDOMNodeFilterTest, "CyberKitDOMNodeFilter/tree-walker");
    REGISTER_TEST(CyberKitDOMNodeFilterTest, "CyberKitDOMNodeFilter/node-iterator");
}

G_GNUC_END_IGNORE_DEPRECATIONS;
