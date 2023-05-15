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
endif ()

# TestCyberKit
if (ENABLE_WEBKIT)
    target_sources(TestCyberKitAPIInjectedBundle PRIVATE
        win/PlatformUtilitiesWin.cpp
    )

    list(APPEND TestCyberKit_SOURCES
        ${test_main_SOURCES}

        Tests/CyberKit/CookieStorageFile.cpp

        Tests/CyberKit/curl/Certificates.cpp

        win/PlatformUtilitiesWin.cpp
        win/PlatformWebViewWin.cpp
    )

    WEBKIT_WRAP_EXECUTABLE(TestCyberKit
        SOURCES ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
        LIBRARIES shlwapi
    )
    target_compile_definitions(TestCyberKit PRIVATE ${wrapper_DEFINITIONS})
endif ()
