set(TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestCyberKitAPI")

add_custom_target(TestCyberKitAPI-forwarding-headers
    COMMAND ${PERL_EXECUTABLE} ${WEBKIT_DIR}/Scripts/generate-forwarding-headers.pl --include-path ${TESTWEBKITAPI_DIR} --output ${FORWARDING_HEADERS_DIR} --platform wpe --platform soup
    DEPENDS webkitwpe-forwarding-headers
)

list(APPEND TestCyberKit_DEPENDENCIES TestCyberKitAPI-forwarding-headers)
add_dependencies(TestCyberKitAPIInjectedBundle TestCyberKitAPI-forwarding-headers)

set(test_main_SOURCES generic/main.cpp)

# TestWTF
list(APPEND TestWTF_SOURCES
    ${test_main_SOURCES}

    Tests/WTF/glib/GRefPtr.cpp
    Tests/WTF/glib/GUniquePtr.cpp
    Tests/WTF/glib/GWeakPtr.cpp
    Tests/WTF/glib/WorkQueueGLib.cpp
)

list(APPEND TestWTF_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
)

# TestCyberScriptCore
list(APPEND TestCyberScriptCore_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
)

# TestCyberCore
list(APPEND TestCyberCore_SOURCES
    ${test_main_SOURCES}

    Tests/CyberCore/UserAgentQuirks.cpp
    Tests/CyberCore/gstreamer/GStreamerTest.cpp
    Tests/CyberCore/gstreamer/GstElementHarness.cpp
    Tests/CyberCore/gstreamer/GstMappedBuffer.cpp
)

list(APPEND TestCyberCore_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
    ${GSTREAMER_INCLUDE_DIRS}
    ${GSTREAMER_AUDIO_INCLUDE_DIRS}
    ${GSTREAMER_PBUTILS_INCLUDE_DIRS}
    ${GSTREAMER_VIDEO_INCLUDE_DIRS}
    ${LIBSOUP_INCLUDE_DIRS}
)

# TestCyberKit
list(APPEND TestCyberKit_SOURCES
    ${test_main_SOURCES}

    wpe/PlatformUtilitiesWPE.cpp
    wpe/PlatformWebViewWPE.cpp
)

list(APPEND TestCyberKit_PRIVATE_INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/Source
    ${FORWARDING_HEADERS_DIR}
    ${WPEBACKEND_FDO_INCLUDE_DIRS}
)

list(APPEND TestCyberKit_SYSTEM_INCLUDE_DIRECTORIES
    ${GIO_UNIX_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
)

list(APPEND TestCyberKit_LIBRARIES
    ${WPEBACKEND_FDO_LIBRARIES}
    CyberKit::WPEToolingBackends
)

# TestCyberKitAPIBase
target_include_directories(TestCyberKitAPIBase PRIVATE
    ${CMAKE_SOURCE_DIR}/Source
    ${FORWARDING_HEADERS_DIR}
)

# TestCyberKitAPIInjectedBundle
target_sources(TestCyberKitAPIInjectedBundle PRIVATE
    wpe/PlatformUtilitiesWPE.cpp
)
target_include_directories(TestCyberKitAPIInjectedBundle PRIVATE
    ${CMAKE_SOURCE_DIR}/Source
    ${FORWARDING_HEADERS_DIR}
    ${GLIB_INCLUDE_DIRS}
)

# TestJSC
set(TestJSC_SOURCES
    Tests/CyberScriptCore/glib/TestJSC.cpp
)

set(TestJSC_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
)

set(TestJSC_PRIVATE_INCLUDE_DIRECTORIES
    ${CMAKE_BINARY_DIR}
    ${TESTWEBKITAPI_DIR}
    "${CyberScriptCoreGLib_DERIVED_SOURCES_DIR}/jsc"
)

set(TestJSC_LIBRARIES
    ${GLIB_LIBRARIES}
    ${GLIB_GMODULE_LIBRARIES}
)

set(TestJSC_FRAMEWORKS
    CyberScriptCore
    WTF
)

if (NOT USE_SYSTEM_MALLOC)
    list(APPEND TestJSC_FRAMEWORKS bmalloc)
endif ()

set(TestJSC_DEFINITIONS
    WEBKIT_SRC_DIR="${CMAKE_SOURCE_DIR}"
)

WEBKIT_EXECUTABLE_DECLARE(TestJSC)
WEBKIT_TEST(TestJSC)
