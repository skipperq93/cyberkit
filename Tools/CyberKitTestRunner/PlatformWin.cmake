set(CyberKitTestRunnerLib_SOURCES
    ${CyberKitTestRunner_SOURCES}
    ${WEBKIT_TESTRUNNER_DIR}/cairo/TestInvocationCairo.cpp
    win/EventSenderProxyWin.cpp
    win/PlatformWebViewWin.cpp
    win/TestControllerWin.cpp
    win/UIScriptControllerWin.cpp
    win/main.cpp
)

set(CyberKitTestRunner_SOURCES
    ${TOOLS_DIR}/win/DLLLauncher/DLLLauncherMain.cpp
)

list(APPEND CyberKitTestRunnerInjectedBundle_SOURCES
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/win/ActivateFontsWin.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/win/InjectedBundleWin.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/win/TestRunnerWin.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/win/AccessibilityControllerWin.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/win/AccessibilityUIElementWin.cpp
)


list(APPEND CyberKitTestRunner_INCLUDE_DIRECTORIES
    cairo
    win
    ${CAIRO_INCLUDE_DIRS}
    ${FORWARDING_HEADERS_DIR}
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/win
)


set(CyberKitTestRunnerLib_LIBRARIES
    ${CyberKitTestRunner_LIBRARIES}
    Comsuppw
    Oleacc
)

set(CyberKitTestRunner_LIBRARIES
    shlwapi
)

set(CyberKitTestRunnerInjectedBundle_LIBRARIES
    CyberCoreTestSupport
    CyberKit
)

list(REMOVE_ITEM
    CyberKitTestRunnerLib_SOURCES
    ${WEBKIT_TESTRUNNER_BINDINGS_DIR}/JSWrapper.cpp
)
list(REMOVE_ITEM
    CyberKitTestRunnerInjectedBundle_SOURCES
    ${WEBKIT_TESTRUNNER_BINDINGS_DIR}/JSWrapper.cpp
)

WEBKIT_ADD_PRECOMPILED_HEADER(CyberKitTestRunnerPrefix.h
    ${WEBKIT_TESTRUNNER_DIR}/win/CyberKitTestRunnerPrefix.cpp
    CyberKitTestRunnerLib_SOURCES
)
WEBKIT_ADD_PRECOMPILED_HEADER(TestRunnerInjectedBundlePrefix.h
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/win/TestRunnerInjectedBundlePrefix.cpp
    CyberKitTestRunnerInjectedBundle_SOURCES
)

list(APPEND
    CyberKitTestRunnerLib_SOURCES
    ${WEBKIT_TESTRUNNER_BINDINGS_DIR}/JSWrapper.cpp
)
list(APPEND
    CyberKitTestRunnerInjectedBundle_SOURCES
    ${WEBKIT_TESTRUNNER_BINDINGS_DIR}/JSWrapper.cpp
)


set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${MSVC_RUNTIME_LINKER_FLAGS}")
add_library(CyberKitTestRunnerLib SHARED ${CyberKitTestRunnerLib_SOURCES})
target_link_libraries(CyberKitTestRunnerLib ${CyberKitTestRunnerLib_LIBRARIES})

add_definitions(
    -DWIN_CAIRO
    -DUSE_CONSOLE_ENTRY_POINT
    -D_UNICODE
)
