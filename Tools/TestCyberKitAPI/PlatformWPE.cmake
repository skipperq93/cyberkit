set(TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestCyberKitAPI")
set(TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY_WTF "${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}/WTF")

# This is necessary because it is possible to build TestCyberKitAPI with CyberKit2
# disabled and this triggers the inclusion of the CyberKit2 headers.
add_definitions(-DBUILDING_WEBKIT2__)

add_custom_target(TestCyberKitAPI-forwarding-headers
    COMMAND ${PERL_EXECUTABLE} ${WEBKIT_DIR}/Scripts/generate-forwarding-headers.pl --include-path ${TESTWEBKITAPI_DIR} --output ${FORWARDING_HEADERS_DIR} --platform wpe --platform soup
    DEPENDS webkitwpe-forwarding-headers
)

list(APPEND TestCyberKitAPI_DEPENDENCIES TestCyberKitAPI-forwarding-headers)

include_directories(
    ${FORWARDING_HEADERS_DIR}
    ${FORWARDING_HEADERS_DIR}/CyberScriptCore
    ${FORWARDING_HEADERS_DIR}/CyberScriptCore/glib
    ${DERIVED_SOURCES_JAVASCRIPCOREWPE_DIR}
    ${TOOLS_DIR}/wpe/backends
)

include_directories(SYSTEM
    ${CAIRO_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${LIBSOUP_INCLUDE_DIRS}
    ${WPE_INCLUDE_DIRS}
    ${WPEBACKEND_FDO_INCLUDE_DIRS}
)

set(test_main_SOURCES
    ${TESTWEBKITAPI_DIR}/generic/main.cpp
)

set(bundle_harness_SOURCES
    ${TESTWEBKITAPI_DIR}/glib/UtilitiesGLib.cpp
    ${TESTWEBKITAPI_DIR}/wpe/InjectedBundleControllerWPE.cpp
    ${TESTWEBKITAPI_DIR}/wpe/PlatformUtilitiesWPE.cpp
)

set(webkit_api_harness_SOURCES
    ${TESTWEBKITAPI_DIR}/glib/UtilitiesGLib.cpp
    ${TESTWEBKITAPI_DIR}/wpe/PlatformUtilitiesWPE.cpp
    ${TESTWEBKITAPI_DIR}/wpe/PlatformWebViewWPE.cpp
)

# TestWTF

list(APPEND TestWTF_SOURCES
    ${TESTWEBKITAPI_DIR}/glib/UtilitiesGLib.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/glib/GUniquePtr.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/glib/WorkQueueGLib.cpp
)

# TestCyberCore

add_executable(TestCyberCore
    ${test_main_SOURCES}
    ${TESTWEBKITAPI_DIR}/glib/UtilitiesGLib.cpp
    ${TESTWEBKITAPI_DIR}/TestsController.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/FileMonitor.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/FileSystem.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/HTMLParserIdioms.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/LayoutUnit.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/MIMETypeRegistry.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/PublicSuffix.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/SharedBuffer.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/SharedBufferTest.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/URLParserTextEncoding.cpp
)

target_link_libraries(TestCyberCore ${test_webcore_LIBRARIES})
add_dependencies(TestCyberCore ${TestCyberKitAPI_DEPENDENCIES})

add_test(TestCyberCore ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}/CyberCore/TestCyberCore)
set_tests_properties(TestCyberCore PROPERTIES TIMEOUT 60)
set_target_properties(TestCyberCore PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}/CyberCore)

# TestCyberKit

list(APPEND test_webkit_api_LIBRARIES
    WPEToolingBackends
)

add_executable(TestCyberKit ${test_webkit_api_SOURCES})

target_link_libraries(TestCyberKit ${test_webkit_api_LIBRARIES})
add_test(TestCyberKit ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}/CyberKit/TestCyberKit)
set_tests_properties(TestCyberKit PROPERTIES TIMEOUT 60)
set_target_properties(TestCyberKit PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}/CyberKit)

# TestCyberKitAPIBase
list(APPEND TestCyberKitAPIBase_LIBRARIES
    WPEBackend-fdo-0.1
)
find_package(WPEBackend-fdo REQUIRED)
list(APPEND TestCyberKitAPI_LIBRARIES ${WPEBACKEND_FDO_LIBRARIES})
list(APPEND TestCyberKitAPIBase_SOURCES
    ${TOOLS_DIR}/wpe/backends/ViewBackend.cpp
    ${TOOLS_DIR}/wpe/backends/HeadlessViewBackend.cpp
)

# TestJSC

add_definitions(-DWEBKIT_SRC_DIR="${CMAKE_SOURCE_DIR}")
add_executable(TestJSC ${TESTWEBKITAPI_DIR}/Tests/CyberScriptCore/glib/TestJSC.cpp)
target_link_libraries(TestJSC
    ${GLIB_LIBRARIES}
    ${GLIB_GMODULE_LIBRARIES}
    CyberScriptCore
)
add_test(TestJSC ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}/CyberScriptCore/TestJSC)
add_dependencies(TestJSC ${TestCyberKitAPI_DEPENDENCIES})
set_tests_properties(TestJSC PROPERTIES TIMEOUT 60)
set_target_properties(TestJSC PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}/CyberScriptCore)

# Add an intermediate target between TestJSC and CyberScriptCore to ensure derived headers are copied into the forwarding header directory.
add_custom_target(pre-TestJSC DEPENDS CyberScriptCore)
add_dependencies(TestJSC pre-TestJSC)

if (COMPILER_IS_GCC_OR_CLANG)
    WEBKIT_ADD_TARGET_CXX_FLAGS(TestCyberCore -Wno-sign-compare
                                            -Wno-undef
                                            -Wno-unused-parameter)
    WEBKIT_ADD_TARGET_CXX_FLAGS(TestCyberKit -Wno-sign-compare
                                           -Wno-undef
                                           -Wno-unused-parameter)
    WEBKIT_ADD_TARGET_CXX_FLAGS(TestJSC -Wno-sign-compare
                                        -Wno-undef
                                        -Wno-unused-parameter)
endif ()
