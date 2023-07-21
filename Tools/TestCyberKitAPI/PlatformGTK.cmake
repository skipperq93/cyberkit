set(TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestCyberKitAPI")

add_custom_target(TestCyberKitAPI-forwarding-headers
    COMMAND ${PERL_EXECUTABLE} ${WEBKIT_DIR}/Scripts/generate-forwarding-headers.pl --include-path ${TESTWEBKITAPI_DIR} --output ${FORWARDING_HEADERS_DIR} --platform gtk --platform soup
    DEPENDS CyberKit-forwarding-headers
)

list(APPEND TestCyberKit_DEPENDENCIES TestCyberKitAPI-forwarding-headers)

set(test_main_SOURCES gtk/main.cpp)

# TestWTF
list(APPEND TestWTF_SOURCES
    ${test_main_SOURCES}

    Tests/WTF/glib/GUniquePtr.cpp
    Tests/WTF/glib/WorkQueueGLib.cpp

    glib/UtilitiesGLib.cpp
)

list(APPEND TestWTF_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
    ${GTK3_INCLUDE_DIRS}
)

list(APPEND TestWTF_LIBRARIES
    ${GDK3_LIBRARIES}
    ${GTK3_LIBRARIES}
)

# TestCyberCore
list(APPEND TestCyberCore_SOURCES
    ${test_main_SOURCES}

    Tests/CyberCore/gstreamer/GStreamerTest.cpp
    Tests/CyberCore/gstreamer/GstMappedBuffer.cpp

    glib/UtilitiesGLib.cpp
)

list(APPEND TestCyberCore_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
    ${GTK3_INCLUDE_DIRS}
)

list(APPEND TestCyberCore_LIBRARIES
    ${GDK3_LIBRARIES}
    ${GTK3_LIBRARIES}
)
ADD_WHOLE_ARCHIVE_TO_LIBRARIES(TestCyberCore_LIBRARIES)

# TestCyberKit
list(APPEND TestCyberKit_SOURCES
    ${test_main_SOURCES}

    Tests/CyberKit/gtk/InputMethodFilter.cpp

    glib/UtilitiesGLib.cpp

    gtk/PlatformUtilitiesGtk.cpp
    gtk/PlatformWebViewGtk.cpp
)

list(APPEND TestCyberKit_PRIVATE_INCLUDE_DIRECTORIES
    "${CMAKE_SOURCE_DIR}/Source"
)

list(APPEND TestCyberKit_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
    ${GTK3_INCLUDE_DIRS}
)

list(APPEND TestCyberKit_LIBRARIES
    ${GDK3_LIBRARIES}
    ${GTK3_LIBRARIES}
)

# TestCyberKitAPIBase
target_include_directories(TestCyberKitAPIBase PRIVATE "${CMAKE_SOURCE_DIR}/Source")

# TestCyberKitAPIInjectedBundle
target_sources(TestCyberKitAPIInjectedBundle PRIVATE
    glib/UtilitiesGLib.cpp

    gtk/InjectedBundleControllerGtk.cpp
    gtk/PlatformUtilitiesGtk.cpp
)
target_include_directories(TestCyberKitAPIInjectedBundle PRIVATE
    "${CMAKE_SOURCE_DIR}/Source"
)
target_include_directories(TestCyberKitAPIInjectedBundle SYSTEM PRIVATE
    ${GLIB_INCLUDE_DIRS}
    ${GTK3_INCLUDE_DIRS}
)

# TestJSC
set(TestJSC_SOURCES
    Tests/CyberScriptCore/glib/TestJSC.cpp
)

set(TestJSC_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
    ${GTK3_INCLUDE_DIRS}
)

set(TestJSC_PRIVATE_INCLUDE_DIRECTORIES
    ${CMAKE_BINARY_DIR}
    ${TESTWEBKITAPI_DIR}
    ${THIRDPARTY_DIR}/gtest/include
    ${WTF_FRAMEWORK_HEADERS_DIR}
    ${CyberScriptCore_PRIVATE_FRAMEWORK_HEADERS_DIR}
    ${FORWARDING_HEADERS_DIR}
    ${FORWARDING_HEADERS_DIR}/CyberScriptCore
    ${FORWARDING_HEADERS_DIR}/CyberScriptCore/glib
    ${DERIVED_SOURCES_JAVASCRIPCOREGTK_DIR}
)

set(TestJSC_LIBRARIES
    ${GLIB_LIBRARIES}
    ${GLIB_GMODULE_LIBRARIES}
    CyberScriptCore
)

set(TestJSC_DEFINITIONS
    WEBKIT_SRC_DIR="${CMAKE_SOURCE_DIR}"
)

WEBKIT_EXECUTABLE_DECLARE(TestJSC)
WEBKIT_TEST(TestJSC)
