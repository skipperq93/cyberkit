set(CyberKitGLibAPITests_SOURCES
    ${TOOLS_DIR}/TestCyberKitAPI/glib/CyberKitGLib/LoadTrackingTest.cpp
    ${TOOLS_DIR}/TestCyberKitAPI/glib/CyberKitGLib/CyberKitTestServer.cpp
    ${TOOLS_DIR}/TestCyberKitAPI/glib/CyberKitGLib/TestMain.cpp
    ${TOOLS_DIR}/TestCyberKitAPI/glib/CyberKitGLib/WebViewTest.cpp
)

set(CyberKitGLibAPITests_INCLUDE_DIRECTORIES
    ${CMAKE_BINARY_DIR}
    ${FORWARDING_HEADERS_DIR}
    ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib
    ${TOOLS_DIR}/TestCyberKitAPI/glib/CyberKitGLib
)

set(CyberKitGLibAPITests_SYSTEM_INCLUDE_DIRECTORIES
    ${GIO_UNIX_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${GSTREAMER_INCLUDE_DIRS}
    ${LIBSOUP_INCLUDE_DIRS}
)

set(CyberKitAPITest_LIBRARIES
    CyberKit::CyberScriptCore
    CyberKit
)

set(CyberKitGLibAPITest_LIBRARIES
    ${CyberKitAPITest_LIBRARIES}
    CyberKitGLibAPITestsCore
    ${GLIB_LIBRARIES}
    ${LIBSOUP_LIBRARIES}
)

set(CyberKitGLibAPITestExtension_LIBRARIES
    CyberKit::CyberScriptCore
    CyberKit
    ${GLIB_LIBRARIES}
)

set(CyberKitGLibAPIWebProcessTests
    ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/DOMElementTest.cpp
    ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/EditorTest.cpp
    ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/FrameTest.cpp
    ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/WebProcessTest.cpp
)

macro(ADD_WK2_TEST_WEB_EXTENSION extension_name)
    add_library(${extension_name} MODULE ${ARGN})
    add_dependencies(${extension_name} CyberKit)
    set_property(
        TARGET ${extension_name}
        APPEND
        PROPERTY COMPILE_DEFINITIONS WEBKIT2_COMPILATION
    )
    set_target_properties(${extension_name} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${TEST_LIBRARY_DIR}
    )
    target_link_libraries(${extension_name} ${CyberKitGLibAPITestExtension_LIBRARIES})

    if (COMPILER_IS_GCC_OR_CLANG)
        WEBKIT_ADD_TARGET_CXX_FLAGS(${extension_name} -Wno-unused-parameter)
    endif ()
endmacro()

macro(ADD_WK2_TEST test_name)
    add_executable(${test_name} ${ARGN})
    add_dependencies(${test_name}
        test-gresource-bundle
        WebExtensionTest
    )
    set_target_properties(${test_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TEST_BINARY_DIR}
    )
    target_link_libraries(${test_name} ${CyberKitGLibAPITest_LIBRARIES})

    if (COMPILER_IS_GCC_OR_CLANG)
        WEBKIT_ADD_TARGET_CXX_FLAGS(${test_name} -Wno-unused-parameter)
    endif ()
endmacro()

WEBKIT_INCLUDE_CONFIG_FILES_IF_EXISTS()

set(TEST_RESOURCES_DIR ${TEST_BINARY_DIR}/resources)
file(MAKE_DIRECTORY ${TEST_RESOURCES_DIR})

add_definitions(
    -DWEBKIT_TEST_PLUGIN_DIR="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/plugins"
    -DWEBKIT_EXEC_PATH="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    -DWEBKIT_SRC_DIR="${CMAKE_SOURCE_DIR}"
    -DWEBKIT_TEST_WEB_EXTENSIONS_DIR="${TEST_LIBRARY_DIR}"
    -DWEBKIT_INJECTED_BUNDLE_PATH="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    -DWEBKIT_TEST_RESOURCES_DIR="${TEST_RESOURCES_DIR}"
)

include_directories(${CyberKitGLibAPITests_INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${CyberKitGLibAPITests_SYSTEM_INCLUDE_DIRECTORIES})

add_library(CyberKitGLibAPITestsCore STATIC ${CyberKitGLibAPITests_SOURCES})
target_link_libraries(CyberKitGLibAPITestsCore CyberKit)

if (COMPILER_IS_GCC_OR_CLANG)
    WEBKIT_ADD_TARGET_CXX_FLAGS(CyberKitGLibAPITestsCore -Wno-unused-parameter)
endif ()

add_custom_command(
    OUTPUT ${TEST_RESOURCES_DIR}/webkitglib-tests-resources.gresource
    DEPENDS ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/resources/webkitglib-tests.gresource.xml
            ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/resources/link-title.js
    COMMAND glib-compile-resources
            --target=${TEST_RESOURCES_DIR}/webkitglib-tests-resources.gresource
            --sourcedir=${CMAKE_SOURCE_DIR}
            ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/resources/webkitglib-tests.gresource.xml
)

add_custom_target(test-gresource-bundle
    DEPENDS ${TEST_RESOURCES_DIR}/webkitglib-tests-resources.gresource
)

ADD_WK2_TEST_WEB_EXTENSION(WebExtensionTest ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/WebExtensionTest.cpp)
ADD_WK2_TEST_WEB_EXTENSION(WebProcessTest ${CyberKitGLibAPIWebProcessTests})

ADD_WK2_TEST(TestAuthentication ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestAuthentication.cpp)
ADD_WK2_TEST(TestAutomationSession ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestAutomationSession.cpp)
ADD_WK2_TEST(TestBackForwardList ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestBackForwardList.cpp)
ADD_WK2_TEST(TestDownloads ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestDownloads.cpp)
ADD_WK2_TEST(TestCyberKitFindController ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestCyberKitFindController.cpp)
ADD_WK2_TEST(TestEditor ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestEditor.cpp)
ADD_WK2_TEST(TestFrame ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestFrame.cpp)
ADD_WK2_TEST(TestLoaderClient ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestLoaderClient.cpp)
ADD_WK2_TEST(TestMultiprocess ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestMultiprocess.cpp)
ADD_WK2_TEST(TestOptionMenu ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestOptionMenu.cpp)
ADD_WK2_TEST(TestResources ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestResources.cpp)
ADD_WK2_TEST(TestSSL ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestSSL.cpp)
ADD_WK2_TEST(TestUIClient ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestUIClient.cpp)
ADD_WK2_TEST(TestWebExtensions ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestWebExtensions.cpp)
ADD_WK2_TEST(TestCyberKitPolicyClient ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestCyberKitPolicyClient.cpp)
ADD_WK2_TEST(TestCyberKitSecurityOrigin ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestCyberKitSecurityOrigin.cpp)
ADD_WK2_TEST(TestCyberKitSettings ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestCyberKitSettings.cpp)
ADD_WK2_TEST(TestCyberKitURIUtilities ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestCyberKitURIUtilities.cpp)
ADD_WK2_TEST(TestCyberKitWebContext ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestCyberKitWebContext.cpp)
ADD_WK2_TEST(TestCyberKitWebView ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestCyberKitWebView.cpp)
ADD_WK2_TEST(TestCyberKitUserContentFilterStore ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestCyberKitUserContentFilterStore.cpp)
ADD_WK2_TEST(TestCyberKitUserContentManager ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestCyberKitUserContentManager.cpp)
ADD_WK2_TEST(TestWebsiteData ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestWebsiteData.cpp)
ADD_WK2_TEST(TestConsoleMessage ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestConsoleMessage.cpp)
ADD_WK2_TEST(TestDOMElement ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestDOMElement.cpp)
ADD_WK2_TEST(TestGeolocationManager ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestGeolocationManager.cpp)
ADD_WK2_TEST(TestInputMethodContext ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestInputMethodContext.cpp)

# FIXME: Enable for WPE
if (PORT STREQUAL "GTK")
    ADD_WK2_TEST(TestCookieManager ${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKitGLib/TestCookieManager.cpp)
endif ()

macro(ADD_WPE_QT_TEST test_name)
    add_executable(${test_name} ${ARGN} ${TOOLS_DIR}/TestCyberKitAPI/Tests/WPEQt/WPEQtTest.cpp)
    set_target_properties(${test_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestCyberKitAPI/WPEQt
        AUTOMOC ON
        CXX_STANDARD 17
    )
    target_compile_definitions(${test_name} PUBLIC QT_NO_KEYWORDS=1)
    target_link_libraries(${test_name} ${WPEQtAPITest_LIBRARIES})
    target_include_directories(${test_name} SYSTEM PRIVATE ${WPEQtAPITests_INCLUDE_DIRECTORIES})
    target_include_directories(${test_name} PRIVATE
      ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Source/CyberKit/UIProcess/API/wpe/qt)
endmacro()

if (PORT STREQUAL "WPE" AND ENABLE_WPE_QT_API)
    set(WPEQtAPITests_INCLUDE_DIRECTORIES
        ${Qt5_INCLUDE_DIRS}
        ${Qt5Test_INCLUDE_DIRS}
    )
    set(WPEQtAPITest_LIBRARIES
         ${CyberKitAPITest_LIBRARIES}
         Qt5::Test qtwpe
    )
    ADD_WPE_QT_TEST(TestLoad ${TOOLS_DIR}/TestCyberKitAPI/Tests/WPEQt/TestLoad.cpp)
    ADD_WPE_QT_TEST(TestLoadHtml ${TOOLS_DIR}/TestCyberKitAPI/Tests/WPEQt/TestLoadHtml.cpp)
    ADD_WPE_QT_TEST(TestLoadRequest ${TOOLS_DIR}/TestCyberKitAPI/Tests/WPEQt/TestLoadRequest.cpp)
    ADD_WPE_QT_TEST(TestRunJavaScript ${TOOLS_DIR}/TestCyberKitAPI/Tests/WPEQt/TestRunJavaScript.cpp)
endif ()
