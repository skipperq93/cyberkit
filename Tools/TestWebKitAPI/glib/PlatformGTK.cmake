set(TEST_LIBRARY_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/CyberKitGTKAPITests)
set(TEST_BINARY_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestCyberKitAPI/CyberKitGTK)

list(APPEND CyberKitGLibAPITests_SOURCES
    ${TOOLS_DIR}/TestCyberKitAPI/glib/CyberKitGLib/gtk/WebViewTestGtk.cpp
)

list(APPEND CyberKitGLibAPITests_INCLUDE_DIRECTORIES
    ${CyberKitGTK_DERIVED_SOURCES_DIR}
    ${CyberKitGTK_FRAMEWORK_HEADERS_DIR}
    ${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/webkitgtk-${WEBKITGTK_API_VERSION}
    ${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/webkitgtk-web-process-extension
)

list(APPEND CyberKitGLibAPITests_SYSTEM_INCLUDE_DIRECTORIES
    ${ATSPI_INCLUDE_DIRS}
)

list(APPEND CyberKitGLibAPITest_LIBRARIES
    ${ATSPI_LIBRARIES}
    GTK::GTK
)

if (GTK_UNIX_PRINT_FOUND)
    list(APPEND CyberKitGLibAPITest_LIBRARIES GTK::UnixPrint)
endif ()

list(APPEND CyberKitGLibAPIWebProcessTests
    ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/AutocleanupsTest.cpp
)

if (NOT USE_GTK4)
    list(APPEND CyberKitGLibAPIWebProcessTests
        ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/DOMClientRectTest.cpp
        ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/DOMNodeTest.cpp
        ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/DOMNodeFilterTest.cpp
        ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/DOMXPathNSResolverTest.cpp
    )
endif ()

ADD_WK2_TEST(InspectorTestServer ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/InspectorTestServer.cpp)
ADD_WK2_TEST(TestAutocleanups ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestAutocleanups.cpp)
ADD_WK2_TEST(TestContextMenu ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestContextMenu.cpp)
ADD_WK2_TEST(TestInspector ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestInspector.cpp)
ADD_WK2_TEST(TestInspectorServer ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestInspectorServer.cpp)
ADD_WK2_TEST(TestPrinting ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestPrinting.cpp)
ADD_WK2_TEST(TestCyberKitFaviconDatabase ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestCyberKitFaviconDatabase.cpp)
ADD_WK2_TEST(TestCyberKitVersion ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestCyberKitVersion.cpp)
ADD_WK2_TEST(TestWebViewEditor ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestWebViewEditor.cpp)

if (ENABLE_ACCESSIBILITY AND ATSPI_FOUND)
    ADD_WK2_TEST(TestCyberKitAccessibility ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestCyberKitAccessibility.cpp)
endif ()

if (NOT USE_GTK4)
    ADD_WK2_TEST(TestDOMClientRect ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestDOMClientRect.cpp)
    ADD_WK2_TEST(TestDOMNode ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestDOMNode.cpp)
    ADD_WK2_TEST(TestDOMNodeFilter ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestDOMNodeFilter.cpp)
    ADD_WK2_TEST(TestDOMXPathNSResolver ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGtk/TestDOMXPathNSResolver.cpp)
endif ()
