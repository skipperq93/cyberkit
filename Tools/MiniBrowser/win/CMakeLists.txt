set(MiniBrowser_INCLUDE_DIRECTORIES
    ${WTF_FRAMEWORK_HEADERS_DIR}
    ${CyberScriptCore_FRAMEWORK_HEADERS_DIR}
    ${CyberScriptCore_PRIVATE_FRAMEWORK_HEADERS_DIR}
    ${PAL_FRAMEWORK_HEADERS_DIR}
    ${CyberCore_PRIVATE_FRAMEWORK_HEADERS_DIR}
    ${CyberKitLegacy_FRAMEWORK_HEADERS_DIR}
    ${CyberKit_FRAMEWORK_HEADERS_DIR}
    ${CyberKit_PRIVATE_FRAMEWORK_HEADERS_DIR}
)

set(MiniBrowser_SOURCES
    AccessibilityDelegate.cpp
    BrowserWindow.cpp
    Common.cpp
    DOMDefaultImpl.cpp
    MainWindow.cpp
    PageLoadTestClient.cpp
    PrintWebUIDelegate.cpp
    ResourceLoadDelegate.cpp
    WebDownloadDelegate.cpp
    MiniBrowserLib.rc
    MiniBrowserWebHost.cpp
    CyberKitLegacyBrowserWindow.cpp
    WinMain.cpp
    stdafx.cpp
)

set(MiniBrowser_LIBRARIES
    DbgHelp
    WTF
    CyberKitLegacy
    CyberKitLegacyGUID
    comctl32
    comsupp
    comsuppw
    shlwapi
)

if (USE_CF)
    list(APPEND MiniBrowser_LIBRARIES
        ${COREFOUNDATION_LIBRARY}
    )
endif ()

if (ENABLE_WEBKIT)
    add_definitions(-DENABLE_WEBKIT)
    list(APPEND MiniBrowser_SOURCES
        CyberKitBrowserWindow.cpp
    )
    list(APPEND MiniBrowser_LIBRARIES CyberKit)
endif ()

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${MSVC_RUNTIME_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:wWinMainCRTStartup")

if (${WTF_PLATFORM_WIN_CAIRO})
    add_definitions(-DWIN_CAIRO)
endif ()
add_definitions(-D_UNICODE)
include_directories(${MiniBrowser_INCLUDE_DIRECTORIES})
add_library(MiniBrowserLib SHARED ${MiniBrowser_SOURCES})
target_link_libraries(MiniBrowserLib ${MiniBrowser_LIBRARIES})

add_executable(MiniBrowser WIN32 ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp MiniBrowser.rc)
target_link_libraries(MiniBrowser shlwapi)
set_target_properties(MiniBrowser PROPERTIES OUTPUT_NAME "MiniBrowser")

add_dependencies(MiniBrowser MiniBrowserLib)
