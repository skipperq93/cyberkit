list(APPEND CyberKitTestRunner_SOURCES
    cairo/TestInvocationCairo.cpp

    win/EventSenderProxyWin.cpp
    win/PlatformWebViewWin.cpp
    win/TestControllerWin.cpp
    win/UIScriptControllerWin.cpp
    win/main.cpp
)

list(APPEND CyberKitTestRunner_INCLUDE_DIRECTORIES
    ${CyberKitTestRunner_DIR}/InjectedBundle/win
)

list(APPEND CyberKitTestRunner_LIBRARIES
    Comsuppw
    Oleacc
)

target_precompile_headers(CyberKitTestRunner PRIVATE CyberKitTestRunnerPrefix.h)

list(APPEND TestRunnerInjectedBundle_SOURCES
    InjectedBundle/win/AccessibilityControllerWin.cpp
    InjectedBundle/win/AccessibilityUIElementWin.cpp
    InjectedBundle/win/ActivateFontsWin.cpp
    InjectedBundle/win/InjectedBundleWin.cpp
    InjectedBundle/win/TestRunnerWin.cpp
)
