list(APPEND CyberKitTestRunner_SOURCES
    cairo/TestInvocationCairo.cpp

    win/EventSenderProxyWin.cpp
    win/PlatformWebViewWin.cpp
    win/TestControllerWin.cpp
    win/UIScriptControllerWin.cpp
    win/main.cpp
)

set(wrapper_DEFINITIONS USE_CONSOLE_ENTRY_POINT)

list(APPEND CyberKitTestRunner_INCLUDE_DIRECTORIES
    ${CyberKitTestRunner_DIR}/InjectedBundle/win
)

list(APPEND CyberKitTestRunner_LIBRARIES
    Comsuppw
    Oleacc
)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${MSVC_RUNTIME_LINKER_FLAGS}")

WEBKIT_WRAP_EXECUTABLE(CyberKitTestRunner
    SOURCES ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
    LIBRARIES shlwapi
)
target_compile_definitions(CyberKitTestRunner PRIVATE ${wrapper_DEFINITIONS})

# Add precompiled headers to wrapper library
target_precompile_headers(CyberKitTestRunnerLib PRIVATE CyberKitTestRunnerPrefix.h)

list(APPEND TestRunnerInjectedBundle_SOURCES
    InjectedBundle/win/AccessibilityControllerWin.cpp
    InjectedBundle/win/AccessibilityUIElementWin.cpp
    InjectedBundle/win/ActivateFontsWin.cpp
    InjectedBundle/win/InjectedBundleWin.cpp
    InjectedBundle/win/TestRunnerWin.cpp
)
