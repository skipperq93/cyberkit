add_custom_target(CyberKitTestRunner-forwarding-headers
    COMMAND ${PERL_EXECUTABLE} ${WEBKIT_DIR}/Scripts/generate-forwarding-headers.pl --include-path ${WEBKIT_TESTRUNNER_DIR} --include-path ${WEBKIT_TESTRUNNER_SHARED_DIR} --output ${FORWARDING_HEADERS_DIR} --platform gtk --platform soup
)

set(ForwardingHeadersForCyberKitTestRunner_NAME CyberKitTestRunner-forwarding-headers)

list(APPEND CyberKitTestRunner_SOURCES
    ${WEBKIT_TESTRUNNER_DIR}/cairo/TestInvocationCairo.cpp

    ${WEBKIT_TESTRUNNER_DIR}/gtk/EventSenderProxyGtk.cpp
    ${WEBKIT_TESTRUNNER_DIR}/gtk/PlatformWebViewGtk.cpp
    ${WEBKIT_TESTRUNNER_DIR}/gtk/TestControllerGtk.cpp
    ${WEBKIT_TESTRUNNER_DIR}/gtk/UIScriptControllerGtk.cpp
    ${WEBKIT_TESTRUNNER_DIR}/gtk/main.cpp
)

list(APPEND CyberKitTestRunner_INCLUDE_DIRECTORIES
    ${FORWARDING_HEADERS_DIR}
)

list(APPEND CyberKitTestRunner_SYSTEM_INCLUDE_DIRECTORIES
    ${ATK_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
    ${GTK3_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
)

list(APPEND CyberKitTestRunner_LIBRARIES
    ${ATK_LIBRARIES}
    ${CAIRO_LIBRARIES}
    ${GTK3_LIBRARIES}
    ${GLIB_LIBRARIES}
    WTF
    CyberCore
)

set(CyberKitTestRunnerInjectedBundle_LIBRARIES
    ${ATK_LIBRARIES}
    ${FONTCONFIG_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${GTK3_LIBRARIES}
    CyberCoreTestSupport
    CyberKit
)

list(APPEND CyberKitTestRunnerInjectedBundle_SOURCES
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/atk/AccessibilityControllerAtk.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/atk/AccessibilityNotificationHandlerAtk.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/atk/AccessibilityUIElementAtk.cpp

    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/gtk/ActivateFontsGtk.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/gtk/InjectedBundleGtk.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/gtk/InjectedBundleUtilities.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/gtk/TestRunnerGtk.cpp
)

add_definitions(
    -DFONTS_CONF_DIR="${TOOLS_DIR}/CyberKitTestRunner/gtk/fonts"
    -DTOP_LEVEL_DIR="${CMAKE_SOURCE_DIR}"
)
