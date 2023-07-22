set(TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

set(wrapper_DEFINITIONS USE_CONSOLE_ENTRY_POINT)

if (${WTF_PLATFORM_WIN_CAIRO})
    list(APPEND wrapper_DEFINITIONS WIN_CAIRO)
endif ()

set(webcore_DEFINITIONS
    STATICALLY_LINKED_WITH_PAL=1
    WEBCORE_EXPORT=
    WEBCORE_TESTSUPPORT_EXPORT=
)

set(test_main_SOURCES
    win/main.cpp
)

# TestWTF
list(APPEND TestWTF_SOURCES
    ${test_main_SOURCES}
    win/UtilitiesWin.cpp
)

WEBKIT_WRAP_EXECUTABLE(TestWTF
    SOURCES ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
    LIBRARIES shlwapi
)
target_compile_definitions(TestWTF PRIVATE ${wrapper_DEFINITIONS})
set(TestWTF_OUTPUT_NAME TestWTF${DEBUG_SUFFIX})

# TestCyberCore
list(APPEND TestCyberCore_SOURCES
    ${test_main_SOURCES}

    Tests/CyberCore/win/DIBPixelData.cpp
    Tests/CyberCore/win/LinkedFonts.cpp

    win/TestCyberCoreStubs.cpp
)
list(APPEND TestCyberCore_DEFINITIONS ${webcore_DEFINITIONS})

list(APPEND TestCyberCore_LIBRARIES
    Crypt32
    D2d1
    Dwrite
    dxguid
    Iphlpapi
    Psapi
    Shlwapi
    Usp10
    WindowsCodecs
)

if (${WTF_PLATFORM_WIN_CAIRO})
    list(APPEND TestCyberCore_LIBRARIES
        ${CAIRO_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        mfuuid
        strmiids
        vcruntime
    )
    list(APPEND TestCyberCore_SOURCES
        Tests/CyberCore/CryptoDigest.cpp

        Tests/CyberCore/curl/Cookies.cpp

        Tests/CyberCore/win/BitmapImage.cpp
    )
else ()
    list(APPEND TestCyberCore_LIBRARIES
        ASL${DEBUG_SUFFIX}
        CFNetwork${DEBUG_SUFFIX}
        CoreGraphics${DEBUG_SUFFIX}
        CoreText${DEBUG_SUFFIX}
        QuartzCore${DEBUG_SUFFIX}
        CyberKitQuartzCoreAdditions${DEBUG_SUFFIX}
        libdispatch${DEBUG_SUFFIX}
        libexslt${DEBUG_SUFFIX}
    )
endif ()

if (USE_CF)
    list(APPEND TestCyberCore_LIBRARIES
        ${COREFOUNDATION_LIBRARY}
    )
endif ()

WEBKIT_WRAP_EXECUTABLE(TestCyberCore
    SOURCES ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
    LIBRARIES shlwapi
)
target_compile_definitions(TestCyberCore PRIVATE ${wrapper_DEFINITIONS})
set(TestCyberCore_OUTPUT_NAME TestCyberCore${DEBUG_SUFFIX})

# TestCyberKitLegacy
if (ENABLE_WEBKIT_LEGACY)
    list(APPEND TestCyberKitLegacy_SOURCES
        ${test_main_SOURCES}

        Tests/CyberKitLegacy/win/ScaleWebView.cpp
        Tests/CyberKitLegacy/win/WebViewDestruction.cpp

        win/HostWindow.cpp
    )

    list(APPEND TestCyberKitLegacy_DEFINITIONS ${webcore_DEFINITIONS})

    list(APPEND TestCyberKitLegacy_LIBRARIES
        CyberKit::WTF
    )

    list(APPEND TestCyberKitLegacy_PRIVATE_INCLUDE_DIRECTORIES
        ${TESTWEBKITAPI_DIR}/win
    )

    WEBKIT_WRAP_EXECUTABLE(TestCyberKitLegacy
        SOURCES ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
        LIBRARIES shlwapi
    )
    target_compile_definitions(TestCyberKitLegacy PRIVATE ${wrapper_DEFINITIONS})
    set(TestCyberKitLegacy_OUTPUT_NAME TestCyberKitLegacy${DEBUG_SUFFIX})
endif ()

# TestCyberKit
if (ENABLE_WEBKIT)
    add_dependencies(TestCyberKitAPIBase CyberKitFrameworkHeaders)
    add_dependencies(TestCyberKitAPIInjectedBundle CyberKitFrameworkHeaders)

    target_sources(TestCyberKitAPIInjectedBundle PRIVATE
        win/InjectedBundleControllerWin.cpp
        win/PlatformUtilitiesWin.cpp
        win/UtilitiesWin.cpp
    )

    list(APPEND TestCyberKit_SOURCES
        ${test_main_SOURCES}

        win/PlatformUtilitiesWin.cpp
        win/PlatformWebViewWin.cpp
        win/UtilitiesWin.cpp
    )

    if (${WTF_PLATFORM_WIN_CAIRO})
        list(APPEND TestCyberKit_SOURCES
            Tests/CyberKit/curl/Certificates.cpp
        )
    endif ()

    list(APPEND TestCyberKit_DEFINITIONS ${webcore_DEFINITIONS})

    list(APPEND TestCyberKit_DEPENDENCIES
        CyberKitFrameworkHeaders
    )

    WEBKIT_WRAP_EXECUTABLE(TestCyberKit
        SOURCES ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
        LIBRARIES shlwapi
    )
    target_compile_definitions(TestCyberKit PRIVATE ${wrapper_DEFINITIONS})
    set(TestCyberKit_OUTPUT_NAME TestCyberKit${DEBUG_SUFFIX})
endif ()
