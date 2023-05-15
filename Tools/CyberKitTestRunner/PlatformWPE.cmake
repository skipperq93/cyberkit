add_custom_target(CyberKitTestRunner-forwarding-headers
    COMMAND ${PERL_EXECUTABLE} ${WEBKIT_DIR}/Scripts/generate-forwarding-headers.pl --include-path ${CyberKitTestRunner_DIR} --output ${FORWARDING_HEADERS_DIR} --platform wpe
)
list(APPEND CyberKitTestRunner_DEPENDENCIES CyberKitTestRunner-forwarding-headers)

list(APPEND CyberKitTestRunner_SOURCES
    cairo/TestInvocationCairo.cpp

    libwpe/EventSenderProxyLibWPE.cpp
    libwpe/PlatformWebViewLibWPE.cpp

    wpe/TestControllerWPE.cpp
    wpe/UIScriptControllerWPE.cpp
    wpe/main.cpp
)

list(APPEND CyberKitTestRunner_PRIVATE_INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/Source
    $<TARGET_PROPERTY:CyberKit,INCLUDE_DIRECTORIES>
)

list(APPEND CyberKitTestRunner_INCLUDE_DIRECTORIES
    ${FORWARDING_HEADERS_DIR}
)

list(APPEND CyberKitTestRunner_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
    ${LIBXKBCOMMON_INCLUDE_DIRS}
    ${WPEBACKEND_FDO_INCLUDE_DIRS}
)

list(APPEND CyberKitTestRunner_LIBRARIES
    ${GLIB_LIBRARIES}
    ${LIBXKBCOMMON_LIBRARIES}
    ${WPEBACKEND_FDO_LIBRARIES}
    Cairo::Cairo
    CyberKit::WPEToolingBackends
)

list(APPEND TestRunnerInjectedBundle_LIBRARIES
    ${GLIB_LIBRARIES}
    Cairo::Cairo
)

list(APPEND TestRunnerInjectedBundle_SOURCES
    InjectedBundle/atspi/AccessibilityControllerAtspi.cpp
    InjectedBundle/atspi/AccessibilityNotificationHandler.cpp
    InjectedBundle/atspi/AccessibilityUIElementAtspi.cpp

    InjectedBundle/wpe/ActivateFontsWPE.cpp
    InjectedBundle/wpe/InjectedBundleWPE.cpp
    InjectedBundle/wpe/TestRunnerWPE.cpp
)

list(APPEND TestRunnerInjectedBundle_INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/Source
    ${GLIB_INCLUDE_DIRS}
    ${CyberKitTestRunner_DIR}/InjectedBundle/atspi
    ${CyberKitTestRunner_DIR}/InjectedBundle/wpe
)

add_definitions(
    -DFONTS_CONF_DIR="${TOOLS_DIR}/CyberKitTestRunner/gtk/fonts"
    -DTOP_LEVEL_DIR="${CMAKE_SOURCE_DIR}"
)
