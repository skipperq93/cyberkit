set(TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY_WTF "${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}")
add_definitions(-DUSE_CONSOLE_ENTRY_POINT)

if (${WTF_PLATFORM_WIN_CAIRO})
    add_definitions(-DWIN_CAIRO)
endif ()

set(test_main_SOURCES
    ${TESTWEBKITAPI_DIR}/win/main.cpp
)

include_directories(
    ${DERIVED_SOURCES_DIR}
    ${FORWARDING_HEADERS_DIR}
    ${FORWARDING_HEADERS_DIR}/CyberScriptCore
    ${TESTWEBKITAPI_DIR}/win
    ${DERIVED_SOURCES_DIR}/CyberKit/Interfaces
)

add_definitions(-DWEBCORE_EXPORT=)

set(test_webcore_LIBRARIES
    Crypt32
    D2d1
    Dwrite
    dxguid
    Iphlpapi
    Psapi
    Shlwapi
    Usp10
    CyberCore${DEBUG_SUFFIX}
    WindowsCodecs
    gtest
)

set(TestCyberCoreLib_SOURCES
    ${test_main_SOURCES}
    win/TestCyberCoreStubs.cpp
    ${TESTWEBKITAPI_DIR}/TestsController.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/AffineTransform.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/CalculationValue.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/ComplexTextController.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/CSSParser.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/FloatRect.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/FloatPoint.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/FloatSize.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/GridPosition.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/HTMLParserIdioms.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/IntRect.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/IntPoint.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/IntSize.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/LayoutUnit.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/MIMETypeRegistry.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/ParsedContentRange.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/SecurityOrigin.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/SharedBuffer.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/SharedBufferTest.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/TimeRanges.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/TransformationMatrix.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/URLParserTextEncoding.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/win/DIBPixelData.cpp
    ${TESTWEBKITAPI_DIR}/Tests/CyberCore/win/LinkedFonts.cpp
)

if (${WTF_PLATFORM_WIN_CAIRO})
    list(APPEND test_webcore_LIBRARIES
        ${CAIRO_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        libANGLE
        mfuuid
        strmiids
        vcruntime
    )
    list(APPEND TestCyberCoreLib_SOURCES
        ${TESTWEBKITAPI_DIR}/Tests/CyberCore/curl/Cookies.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberCore/win/BitmapImage.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberCore/CryptoDigest.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberCore/PublicSuffix.cpp
    )
else ()
    list(APPEND test_webcore_LIBRARIES
        ASL${DEBUG_SUFFIX}
        CFNetwork${DEBUG_SUFFIX}
        CoreGraphics${DEBUG_SUFFIX}
        CoreText${DEBUG_SUFFIX}
        QuartzCore${DEBUG_SUFFIX}
        CyberKitQuartzCoreAdditions${DEBUG_SUFFIX}
        libdispatch${DEBUG_SUFFIX}
        libexslt${DEBUG_SUFFIX}
        libicuin${DEBUG_SUFFIX}
        libicuuc${DEBUG_SUFFIX}
    )
endif ()

if (USE_CF)
    list(APPEND test_webcore_LIBRARIES
        ${COREFOUNDATION_LIBRARY}
    )
endif ()

list(APPEND TestCyberKitAPI_DEPENDENCIES CyberCoreForwardingHeaders)
if (ENABLE_WEBKIT)
    list(APPEND TestCyberKitAPI_DEPENDENCIES CyberKitForwardingHeaders)
endif ()

add_library(TestWTFLib SHARED
    ${test_main_SOURCES}
    ${TestWTF_SOURCES}
)
set_target_properties(TestWTFLib PROPERTIES OUTPUT_NAME "TestWTFLib")
target_link_libraries(TestWTFLib ${test_wtf_LIBRARIES})
add_dependencies(TestWTFLib ${TestCyberKitAPI_DEPENDENCIES})

set(test_wtf_LIBRARIES
    shlwapi
)
set(TestWTF_SOURCES
)

add_library(TestCyberCoreLib SHARED
    ${TestCyberCoreLib_SOURCES}
)

target_link_libraries(TestCyberCoreLib ${test_webcore_LIBRARIES})
set_target_properties(TestCyberCoreLib PROPERTIES OUTPUT_NAME "TestCyberCoreLib")
add_dependencies(TestCyberCoreLib ${TestCyberKitAPI_DEPENDENCIES})

if (PAL_LIBRARY_TYPE MATCHES STATIC)
    target_compile_definitions(TestCyberCoreLib PRIVATE -DSTATICALLY_LINKED_WITH_PAL=1)
endif ()

add_executable(TestCyberCore
    ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
)
target_link_libraries(TestCyberCore shlwapi)


add_test(TestCyberCore ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}/TestCyberCore)
set_tests_properties(TestCyberCore PROPERTIES TIMEOUT 60)

if (${WTF_PLATFORM_WIN_CAIRO})
    include_directories(
        ${CAIRO_INCLUDE_DIRS}
    )
endif ()

set(test_webkitlegacy_LIBRARIES
    CyberCoreTestSupport
    CyberKitLegacy${DEBUG_SUFFIX}
    gtest
)

if (ENABLE_WEBKIT_LEGACY)
    add_library(TestCyberKitLegacyLib SHARED
        ${test_main_SOURCES}
        ${TESTWEBKITAPI_DIR}/TestsController.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKitLegacy/win/ScaleWebView.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKitLegacy/win/WebViewDestruction.cpp
        ${TESTWEBKITAPI_DIR}/win/HostWindow.cpp
    )

    target_link_libraries(TestCyberKitLegacyLib ${test_webkitlegacy_LIBRARIES})
    add_dependencies(TestCyberKitLegacyLib ${TestCyberKitAPI_DEPENDENCIES})

    add_executable(TestCyberKitLegacy
        ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
    )
    target_link_libraries(TestCyberKitLegacy shlwapi)

    add_test(TestCyberKitLegacy ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}/TestCyberKitLegacy)
    set_tests_properties(TestCyberKitLegacy PROPERTIES TIMEOUT 60)

    add_dependencies(TestCyberKitLegacy TestCyberKitLegacyLib)
endif ()

if (ENABLE_WEBKIT)
    set(bundle_harness_SOURCES
        ${TESTWEBKITAPI_DIR}/win/UtilitiesWin.cpp
        ${TESTWEBKITAPI_DIR}/win/InjectedBundleControllerWin.cpp
        ${TESTWEBKITAPI_DIR}/win/PlatformUtilitiesWin.cpp
    )

    set(webkit_api_harness_SOURCES
        ${TESTWEBKITAPI_DIR}/win/PlatformUtilitiesWin.cpp
        ${TESTWEBKITAPI_DIR}/win/PlatformWebViewWin.cpp
        ${TESTWEBKITAPI_DIR}/win/UtilitiesWin.cpp
    )

    if (${WTF_PLATFORM_WIN_CAIRO})
        list(APPEND test_webkit_api_SOURCES
            ${TESTWEBKITAPI_DIR}/Tests/CyberKit/curl/Certificates.cpp
        )
    endif ()

    add_library(TestCyberKitLib SHARED
        ${TESTWEBKITAPI_DIR}/win/main.cpp
        ${test_webkit_api_SOURCES}
    )

    target_link_libraries(TestCyberKitLib ${test_webkit_api_LIBRARIES})

    add_executable(TestCyberKit
        ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
    )
    target_link_libraries(TestCyberKit shlwapi)

    add_test(TestCyberKit ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}/CyberKit/TestCyberKit)
    set_tests_properties(TestCyberKit PROPERTIES TIMEOUT 60)
    set_target_properties(TestCyberKit PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}/CyberKit)

    add_dependencies(TestCyberKit TestCyberKitAPIBase)
endif ()

set(test_main_SOURCES
    ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
)

add_dependencies(TestCyberCore TestCyberCoreLib)
