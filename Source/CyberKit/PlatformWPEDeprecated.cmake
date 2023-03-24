file(MAKE_DIRECTORY ${FORWARDING_HEADERS_WPE_DOM_DIR})

list(APPEND CyberKit_UNIFIED_SOURCE_LIST_FILES
    "SourcesWPEDeprecated.txt"
)

list(APPEND WPE_API_HEADER_TEMPLATES
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitFaviconDatabase.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitJavascriptResult.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitMimeInfo.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitPlugin.h.in
)

list(APPEND WPE_WEB_PROCESS_EXTENSION_API_HEADER_TEMPLATES
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitConsoleMessage.h.in
)

list(APPEND CyberKit_INCLUDE_DIRECTORIES
    "${FORWARDING_HEADERS_WPE_DOM_DIR}"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/DOM"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM"
)

list(APPEND WPE_WEB_PROCESS_EXTENSION_API_INSTALLED_HEADERS
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM/webkitdom.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM/CyberKitDOMDefines.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM/CyberKitDOMDocument.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM/CyberKitDOMElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM/CyberKitDOMNode.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM/CyberKitDOMObject.h
)

set(WPE_DOM_SOURCES_FOR_INTROSPECTION
    WebProcess/InjectedBundle/API/glib/DOM/CyberKitDOMNode.cpp
)

list(APPEND WPE_FAKE_API_HEADERS
    ${FORWARDING_HEADERS_WPE_DOM_DIR}/wpe
    ${DERIVED_SOURCES_WPE_API_DIR}/webkit-web-extension.h
)

add_custom_command(
    OUTPUT ${FORWARDING_HEADERS_WPE_DOM_DIR}/wpe
    DEPENDS ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM
    COMMAND ln -n -s -f ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM ${FORWARDING_HEADERS_WPE_DOM_DIR}/wpe
    VERBATIM
)

add_custom_command(
    OUTPUT ${DERIVED_SOURCES_WPE_API_DIR}/webkit-web-extension.h
    DEPENDS ${DERIVED_SOURCES_WPE_API_DIR}/webkit-web-process-extension.h
    COMMAND ${CMAKE_COMMAND} -E copy ${DERIVED_SOURCES_WPE_API_DIR}/webkit-web-process-extension.h ${DERIVED_SOURCES_WPE_API_DIR}/webkit-web-extension.h
    VERBATIM
)

install(FILES ${DERIVED_SOURCES_WPE_API_DIR}/webkit-web-extension.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wpe-webkit-${WPE_API_VERSION}/wpe
)
