add_custom_target(CyberKitTestRunner-forwarding-headers
    COMMAND ${PERL_EXECUTABLE} ${WEBKIT_DIR}/Scripts/generate-forwarding-headers.pl --include-path ${WEBKIT_TESTRUNNER_DIR} --output ${FORWARDING_HEADERS_DIR} --platform wpe
)

set(ForwardingHeadersForCyberKitTestRunner_NAME CyberKitTestRunner-forwarding-headers)

list(APPEND CyberKitTestRunner_SOURCES
    ${WEBKIT_TESTRUNNER_DIR}/cairo/TestInvocationCairo.cpp

    ${WEBKIT_TESTRUNNER_DIR}/wpe/EventSenderProxyWPE.cpp
    ${WEBKIT_TESTRUNNER_DIR}/wpe/PlatformWebViewWPE.cpp
    ${WEBKIT_TESTRUNNER_DIR}/wpe/TestControllerWPE.cpp
    ${WEBKIT_TESTRUNNER_DIR}/wpe/UIScriptControllerWPE.cpp
    ${WEBKIT_TESTRUNNER_DIR}/wpe/main.cpp
)

list(APPEND CyberKitTestRunner_INCLUDE_DIRECTORIES
    ${WEBKIT_TESTRUNNER_DIR}/InjectedBundle/wpe
    ${FORWARDING_HEADERS_DIR}
    ${TOOLS_DIR}/wpe/backends
)

list(APPEND CyberKitTestRunner_SYSTEM_INCLUDE_DIRECTORIES
    ${CAIRO_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${LIBXKBCOMMON_INCLUDE_DIRS}
    ${WPEBACKEND_FDO_INCLUDE_DIRS}
)

list(APPEND CyberKitTestRunner_LIBRARIES
    ${CAIRO_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${LIBXKBCOMMON_LIBRARIES}
    ${WPEBACKEND_FDO_LIBRARIES}
    WPEToolingBackends
)

set(CyberKitTestRunnerInjectedBundle_LIBRARIES
    ${CAIRO_LIBRARIES}
    ${GLIB_LIBRARIES}
    CyberCoreTestSupport
    CyberKit
)

list(APPEND CyberKitTestRunnerInjectedBundle_SOURCES
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/atk/AccessibilityControllerAtk.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/atk/AccessibilityNotificationHandlerAtk.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/atk/AccessibilityUIElementAtk.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/wpe/ActivateFontsWPE.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/wpe/InjectedBundleWPE.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/wpe/TestRunnerWPE.cpp
)

add_definitions(
    -DFONTS_CONF_DIR="${TOOLS_DIR}/CyberKitTestRunner/gtk/fonts"
    -DTOP_LEVEL_DIR="${CMAKE_SOURCE_DIR}"
)
