set(TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

set(wrapper_DEFINITIONS USE_CONSOLE_ENTRY_POINT)

set(test_main_SOURCES
    win/main.cpp
)

# TestWTF
list(APPEND TestWTF_SOURCES
    ${test_main_SOURCES}
)

WEBKIT_WRAP_EXECUTABLE(TestWTF
    SOURCES ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
    LIBRARIES shlwapi
)
target_compile_definitions(TestWTF PRIVATE ${wrapper_DEFINITIONS})

# TestCyberCore
list(APPEND TestCyberCore_SOURCES
    ${test_main_SOURCES}

    Tests/CyberCore/CryptoDigest.cpp

    Tests/CyberCore/curl/Cookies.cpp
    Tests/CyberCore/curl/OpenSSLHelperTests.cpp

    Tests/CyberCore/win/BitmapImage.cpp
    Tests/CyberCore/win/DIBPixelData.cpp
    Tests/CyberCore/win/LinkedFonts.cpp
    Tests/CyberCore/win/CyberCoreBundle.cpp

    win/TestCyberCoreStubs.cpp
)

list(APPEND TestCyberCore_LIBRARIES
    Crypt32
    D2d1
    Dwrite
    Iphlpapi
    Psapi
    Shlwapi
    Usp10
    WindowsCodecs
    dxguid
)

if (USE_CF)
    list(APPEND TestCyberCore_LIBRARIES
        Apple::CoreFoundation
    )
endif ()

WEBKIT_WRAP_EXECUTABLE(TestCyberCore
    SOURCES ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
    LIBRARIES shlwapi
)
target_compile_definitions(TestCyberCore PRIVATE ${wrapper_DEFINITIONS})

# TestCyberKitLegacy
if (ENABLE_WEBKIT_LEGACY)
    list(APPEND TestCyberKitLegacy_SOURCES
        ${test_main_SOURCES}

        Tests/CyberKitLegacy/win/ScaleWebView.cpp
        Tests/CyberKitLegacy/win/WebViewDestruction.cpp

        win/HostWindow.cpp
    )

    list(APPEND TestCyberKitLegacy_LIBRARIES
        WebKit::WTF
    )

    list(APPEND TestCyberKitLegacy_PRIVATE_INCLUDE_DIRECTORIES
        ${TESTWEBKITAPI_DIR}/win
    )

    WEBKIT_WRAP_EXECUTABLE(TestCyberKitLegacy
        SOURCES ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
        LIBRARIES shlwapi
    )
    target_compile_definitions(TestCyberKitLegacy PRIVATE ${wrapper_DEFINITIONS})
endif ()

# TestWebKit
if (ENABLE_WEBKIT)
    target_sources(TestWebKitAPIInjectedBundle PRIVATE
        win/PlatformUtilitiesWin.cpp
    )

    list(APPEND TestWebKit_SOURCES
        ${test_main_SOURCES}

        Tests/WebKit/CookieStorageFile.cpp

        Tests/WebKit/curl/Certificates.cpp

        win/PlatformUtilitiesWin.cpp
        win/PlatformWebViewWin.cpp
    )

    WEBKIT_WRAP_EXECUTABLE(TestWebKit
        SOURCES ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
        LIBRARIES shlwapi
    )
    target_compile_definitions(TestWebKit PRIVATE ${wrapper_DEFINITIONS})
endif ()
