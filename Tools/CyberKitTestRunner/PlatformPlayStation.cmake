list(APPEND CyberKitTestRunner_SOURCES
    cairo/TestInvocationCairo.cpp

    libwpe/EventSenderProxyLibWPE.cpp
    libwpe/PlatformWebViewLibWPE.cpp

    playstation/TestControllerPlayStation.cpp
    playstation/UIScriptControllerPlayStation.cpp
    playstation/main.cpp
)

list(APPEND CyberKitTestRunner_INCLUDE_DIRECTORIES
    ${CyberKitTestRunner_DIR}/playstation
)

list(APPEND CyberKitTestRunner_PRIVATE_LIBRARIES
    Cairo::Cairo
    CyberKit::WPEToolingBackends
)

list(APPEND TestRunnerInjectedBundle_SOURCES
    InjectedBundle/playstation/ActivateFontsPlayStation.cpp
    InjectedBundle/playstation/InjectedBundlePlayStation.cpp
    InjectedBundle/playstation/TestRunnerPlayStation.cpp
)

list(APPEND TestRunnerInjectedBundle_INCLUDE_DIRECTORIES
    ${CyberKitTestRunner_DIR}/InjectedBundle/playstation
)
