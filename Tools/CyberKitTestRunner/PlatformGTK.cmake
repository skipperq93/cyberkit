add_custom_target(CyberKitTestRunner-forwarding-headers
    COMMAND ${PERL_EXECUTABLE} ${WEBKIT_DIR}/Scripts/generate-forwarding-headers.pl --include-path ${CyberKitTestRunner_DIR} --include-path ${CyberKitTestRunner_SHARED_DIR} --output ${FORWARDING_HEADERS_DIR} --platform gtk --platform soup
)
list(APPEND CyberKitTestRunner_DEPENDENCIES CyberKitTestRunner-forwarding-headers)

list(APPEND CyberKitTestRunner_SOURCES
    cairo/TestInvocationCairo.cpp

    gtk/EventSenderProxyGtk.cpp
    gtk/PlatformWebViewGtk.cpp
    gtk/TestControllerGtk.cpp
    gtk/UIScriptControllerGtk.cpp
    gtk/main.cpp
)

list(APPEND CyberKitTestRunner_INCLUDE_DIRECTORIES
    ${FORWARDING_HEADERS_DIR}
)

list(APPEND CyberKitTestRunner_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
)

list(APPEND CyberKitTestRunner_LIBRARIES
    ${ATK_LIBRARIES}
    ${GLIB_LIBRARIES}
    Cairo::Cairo
    GTK::GTK
)

list(APPEND CyberKitTestRunnerInjectedBundle_LIBRARIES
    ${ATK_LIBRARIES}
    ${GLIB_LIBRARIES}
    Fontconfig::Fontconfig
    GTK::GTK
)

list(APPEND CyberKitTestRunnerInjectedBundle_SOURCES
    InjectedBundle/atk/AccessibilityControllerAtk.cpp
    InjectedBundle/atk/AccessibilityNotificationHandlerAtk.cpp
    InjectedBundle/atk/AccessibilityUIElementAtk.cpp

    InjectedBundle/gtk/ActivateFontsGtk.cpp
    InjectedBundle/gtk/InjectedBundleGtk.cpp
    InjectedBundle/gtk/InjectedBundleUtilities.cpp
    InjectedBundle/gtk/TestRunnerGtk.cpp
)

list(APPEND CyberKitTestRunnerInjectedBundle_INCLUDE_DIRECTORIES
    ${ATK_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${CyberKitTestRunner_DIR}/InjectedBundle/atk
    ${CyberKitTestRunner_DIR}/InjectedBundle/gtk
)

add_definitions(
    -DFONTS_CONF_DIR="${TOOLS_DIR}/CyberKitTestRunner/gtk/fonts"
    -DTOP_LEVEL_DIR="${CMAKE_SOURCE_DIR}"
)
