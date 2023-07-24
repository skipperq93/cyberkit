add_custom_target(CyberKitTestRunner-forwarding-headers
    COMMAND ${PERL_EXECUTABLE} ${WEBKIT_DIR}/Scripts/generate-forwarding-headers.pl --include-path ${CyberKitTestRunner_DIR} --output ${FORWARDING_HEADERS_DIR} --platform wpe
)
list(APPEND CyberKitTestRunner_DEPENDENCIES CyberKitTestRunner-forwarding-headers)

list(APPEND CyberKitTestRunner_SOURCES
    cairo/TestInvocationCairo.cpp

    wpe/EventSenderProxyWPE.cpp
    wpe/PlatformWebViewWPE.cpp
    wpe/TestControllerWPE.cpp
    wpe/UIScriptControllerWPE.cpp
    wpe/main.cpp
)

list(APPEND CyberKitTestRunner_INCLUDE_DIRECTORIES
    ${FORWARDING_HEADERS_DIR}
    ${TOOLS_DIR}/wpe/backends
)

list(APPEND CyberKitTestRunner_SYSTEM_INCLUDE_DIRECTORIES
    ${ATK_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${LIBXKBCOMMON_INCLUDE_DIRS}
    ${WPEBACKEND_FDO_INCLUDE_DIRS}
)

list(APPEND CyberKitTestRunner_LIBRARIES
    ${GLIB_LIBRARIES}
    ${LIBXKBCOMMON_LIBRARIES}
    ${WPEBACKEND_FDO_LIBRARIES}
    Cairo::Cairo
    WPEToolingBackends
)

list(APPEND CyberKitTestRunnerInjectedBundle_LIBRARIES
    ${ATK_LIBRARIES}
    ${GLIB_LIBRARIES}
    Cairo::Cairo
)

list(APPEND CyberKitTestRunnerInjectedBundle_SOURCES
    InjectedBundle/atk/AccessibilityControllerAtk.cpp
    InjectedBundle/atk/AccessibilityNotificationHandlerAtk.cpp
    InjectedBundle/atk/AccessibilityUIElementAtk.cpp

    InjectedBundle/wpe/ActivateFontsWPE.cpp
    InjectedBundle/wpe/InjectedBundleWPE.cpp
    InjectedBundle/wpe/TestRunnerWPE.cpp
)

list(APPEND CyberKitTestRunnerInjectedBundle_INCLUDE_DIRECTORIES
    ${ATK_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${CyberKitTestRunner_DIR}/InjectedBundle/atk
    ${CyberKitTestRunner_DIR}/InjectedBundle/wpe
)

add_definitions(
    -DFONTS_CONF_DIR="${TOOLS_DIR}/CyberKitTestRunner/gtk/fonts"
    -DTOP_LEVEL_DIR="${CMAKE_SOURCE_DIR}"
)
