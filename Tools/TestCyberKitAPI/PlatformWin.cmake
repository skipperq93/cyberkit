set(TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

set(test_main_SOURCES
    win/main.cpp
)

# TestWTF
list(APPEND TestWTF_SOURCES
    ${test_main_SOURCES}
)

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
endif ()

# TestCyberKit
if (ENABLE_WEBKIT)
    target_sources(TestCyberKitAPIInjectedBundle PRIVATE
        win/PlatformUtilitiesWin.cpp
    )

    list(APPEND TestCyberKit_SOURCES
        ${test_main_SOURCES}

        Tests/CyberKit/CookieStorageFile.cpp

        win/PlatformUtilitiesWin.cpp
        win/PlatformWebViewWin.cpp
    )
endif ()
