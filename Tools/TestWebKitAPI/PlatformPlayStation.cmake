set(TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestCyberKitAPI")

set(test_main_SOURCES
    playstation/main.cpp
)

list(APPEND TestWTF_SOURCES
    ${test_main_SOURCES}
)
list(APPEND TestWTF_PRIVATE_INCLUDE_DIRECTORIES
    ${WEBKIT_LIBRARIES_DIR}/include
)

WEBKIT_ADD_TARGET_CXX_FLAGS(TestWTF -Wno-unused-function)

list(APPEND TestCyberScriptCore_SOURCES
    ${test_main_SOURCES}
)
list(APPEND TestCyberScriptCore_PRIVATE_INCLUDE_DIRECTORIES
    ${WEBKIT_LIBRARIES_DIR}/include
)

WEBKIT_ADD_TARGET_CXX_FLAGS(TestCyberScriptCore -Wno-unused-function)

list(APPEND TestCyberCore_SOURCES
    ${test_main_SOURCES}

    Tests/CyberCore/curl/OpenSSLHelperTests.cpp
)
list(APPEND TestCyberCore_PRIVATE_INCLUDE_DIRECTORIES
    ${WEBKIT_LIBRARIES_DIR}/include
)

# TestCyberKit
if (ENABLE_WEBKIT)
    target_sources(TestCyberKitAPIInjectedBundle PRIVATE
        playstation/PlatformUtilitiesPlayStation.cpp
    )

    list(APPEND TestCyberKit_SOURCES
        ${test_main_SOURCES}

        Tests/CyberKit/curl/Certificates.cpp

        playstation/PlatformUtilitiesPlayStation.cpp
        playstation/PlatformWebViewPlayStation.cpp
    )
    list(APPEND TestCyberKit_PRIVATE_INCLUDE_DIRECTORIES
        ${WEBKIT_LIBRARIES_DIR}/include
    )

    # Exclude tests which don't finish.
    list(REMOVE_ITEM TestCyberKit_SOURCES
        Tests/CyberKit/ForceRepaint.cpp
        Tests/CyberKit/Geolocation.cpp
    )

    list(APPEND TestCyberKit_PRIVATE_LIBRARIES
        ${ProcessLauncher_LIBRARY}
    )

    WEBKIT_ADD_TARGET_CXX_FLAGS(TestCyberKit -Wno-deprecated-declarations)
endif ()

# Set the debugger working directory for Visual Studio
if (${CMAKE_GENERATOR} MATCHES "Visual Studio")
    set_target_properties(TestWTF PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    if (ENABLE_WEBCORE)
        set_target_properties(TestCyberCore PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    endif ()
    if (ENABLE_WEBCORE)
        set_target_properties(TestCyberKit PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    endif ()
endif ()

add_definitions(
    -DTEST_WEBKIT_RESOURCES_DIR=\"${TOOLS_DIR}/TestCyberKitAPI/Tests/CyberKit\"
)
