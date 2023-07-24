set(TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestCyberKitAPI")

set(test_main_SOURCES
    playstation/main.cpp
)

list(APPEND TestWTF_SOURCES
    ${test_main_SOURCES}

    generic/UtilitiesGeneric.cpp
)

# Both bmalloc and WTF are built as object libraries. The CyberKit:: interface
# targets are used. A limitation of that is the object files are not propagated
# so they are added here.
list(APPEND TestWTF_PRIVATE_LIBRARIES
    $<TARGET_OBJECTS:WTF>
    $<TARGET_OBJECTS:bmalloc>
)

list(APPEND TestCyberCore_SOURCES
    ${test_main_SOURCES}
)

# Both PAL and CyberCore are built as object libraries. The CyberKit:: interface
# targets are used. A limitation of that is the object files are not propagated
# so they are added here.
list(APPEND TestCyberCore_PRIVATE_LIBRARIES
    $<TARGET_OBJECTS:PAL>
    $<TARGET_OBJECTS:CyberCore>
)

# TestCyberKit
if (ENABLE_WEBKIT)
    target_sources(TestCyberKitAPIInjectedBundle PRIVATE
        generic/UtilitiesGeneric.cpp

        playstation/PlatformUtilitiesPlayStation.cpp
    )

    list(APPEND TestCyberKit_SOURCES
        ${test_main_SOURCES}

        Tests/CyberKit/curl/Certificates.cpp

        generic/UtilitiesGeneric.cpp

        playstation/PlatformUtilitiesPlayStation.cpp
        playstation/PlatformWebViewPlayStation.cpp
    )

    # Exclude tests which don't finish.
    list(REMOVE_ITEM TestCyberKit_SOURCES
        Tests/CyberKit/ForceRepaint.cpp
        Tests/CyberKit/Geolocation.cpp
    )
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
