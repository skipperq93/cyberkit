set(TEST_LIBRARY_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/CyberKit2GtkAPITests)
set(TEST_BINARY_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestCyberKitAPI/CyberKit2Gtk)

list(APPEND CyberKitGLibAPITests_SOURCES
    ${TOOLS_DIR}/TestCyberKitAPI/glib/CyberKitGLib/gtk/WebViewTestGtk.cpp
)

list(APPEND CyberKitGLibAPITests_INCLUDE_DIRECTORIES
    ${DERIVED_SOURCES_JAVASCRIPCOREGTK_DIR}
    ${DERIVED_SOURCES_WEBKIT2GTK_DIR}
    ${FORWARDING_HEADERS_WEBKIT2GTK_DIR}
    ${FORWARDING_HEADERS_WEBKIT2GTK_EXTENSION_DIR}
)

list(APPEND CyberKitGLibAPITests_SYSTEM_INCLUDE_DIRECTORIES
    ${ATSPI_INCLUDE_DIRS}
    ${GTK3_INCLUDE_DIRS}
    ${GTK_UNIX_PRINT_INCLUDE_DIRS}
)

list(APPEND CyberKitGLibAPITest_LIBRARIES
    ${ATSPI_LIBRARIES}
    ${GTK3_LIBRARIES}
    ${GTK_UNIX_PRINT_LIBRARIES}
)

list(APPEND CyberKitGLibAPIWebProcessTests
    ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/AutocleanupsTest.cpp
    ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/DOMClientRectTest.cpp
    ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/DOMNodeTest.cpp
    ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/DOMNodeFilterTest.cpp
    ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/DOMXPathNSResolverTest.cpp
)

ADD_WK2_TEST(InspectorTestServer ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/InspectorTestServer.cpp)
ADD_WK2_TEST(TestAutocleanups ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestAutocleanups.cpp)
ADD_WK2_TEST(TestContextMenu ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestContextMenu.cpp)
ADD_WK2_TEST(TestDOMClientRect ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestDOMClientRect.cpp)
ADD_WK2_TEST(TestDOMNode ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestDOMNode.cpp)
ADD_WK2_TEST(TestDOMNodeFilter ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestDOMNodeFilter.cpp)
ADD_WK2_TEST(TestDOMXPathNSResolver ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestDOMXPathNSResolver.cpp)
ADD_WK2_TEST(TestInspector ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestInspector.cpp)
ADD_WK2_TEST(TestInspectorServer ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestInspectorServer.cpp)
ADD_WK2_TEST(TestOptionMenu ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestOptionMenu.cpp)
ADD_WK2_TEST(TestPrinting ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestPrinting.cpp)
ADD_WK2_TEST(TestCyberKitVersion ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestCyberKitVersion.cpp)
ADD_WK2_TEST(TestWebViewEditor ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestWebViewEditor.cpp)

if (ATSPI_FOUND)
    ADD_WK2_TEST(AccessibilityTestServer ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/AccessibilityTestServer.cpp)
    ADD_WK2_TEST(TestCyberKitAccessibility ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestCyberKitAccessibility.cpp)
endif ()
