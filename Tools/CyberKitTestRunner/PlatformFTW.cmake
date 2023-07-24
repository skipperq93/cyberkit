list(APPEND CyberKitTestRunner_SOURCES
    win/EventSenderProxyWin.cpp
    win/PlatformWebViewWin.cpp
    win/TestControllerWin.cpp
    win/TestInvocationDirect2D.cpp
    win/UIScriptControllerWin.cpp
    win/main.cpp
)

set(wrapper_DEFINITIONS
    USE_CONSOLE_ENTRY_POINT
    WIN_CAIRO
)

list(APPEND CyberKitTestRunnerInjectedBundle_SOURCES
    InjectedBundle/win/AccessibilityControllerWin.cpp
    InjectedBundle/win/AccessibilityUIElementWin.cpp
    InjectedBundle/win/ActivateFontsWin.cpp
    InjectedBundle/win/InjectedBundleWin.cpp
    InjectedBundle/win/TestRunnerWin.cpp
)

list(APPEND CyberKitTestRunner_INCLUDE_DIRECTORIES
    ${CyberKitTestRunner_DIR}/InjectedBundle/win
)

list(APPEND CyberKitTestRunner_LIBRARIES
    Comsuppw
    Oleacc
)

# Add precompiled header
# JSWrapper.cpp is shared between the test runner and injected bundle so it can't be
# present in the list of sources when the macro is invoked. Remove it, create the precompiled
# header, and add it back to work around this.
list(REMOVE_ITEM CyberKitTestRunner_SOURCES ${CyberKitTestRunner_BINDINGS_DIR}/JSWrapper.cpp)
WEBKIT_ADD_PRECOMPILED_HEADER("CyberKitTestRunnerPrefix.h" "win/CyberKitTestRunnerPrefix.cpp" CyberKitTestRunner_SOURCES)
list(APPEND CyberKitTestRunner_SOURCES ${CyberKitTestRunner_BINDINGS_DIR}/JSWrapper.cpp)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${MSVC_RUNTIME_LINKER_FLAGS}")

WEBKIT_WRAP_EXECUTABLE(CyberKitTestRunner
    SOURCES ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
    LIBRARIES shlwapi
)
target_compile_definitions(CyberKitTestRunner PRIVATE ${wrapper_DEFINITIONS})
