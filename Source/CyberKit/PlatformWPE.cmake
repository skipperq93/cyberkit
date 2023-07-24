include(InspectorGResources.cmake)
include(GNUInstallDirs)

set(CyberKit_OUTPUT_NAME WPECyberKit-${WPE_API_VERSION})
set(WebProcess_OUTPUT_NAME WPEWebProcess)
set(NetworkProcess_OUTPUT_NAME WPENetworkProcess)
set(GPUProcess_OUTPUT_NAME WPEGPUProcess)

file(MAKE_DIRECTORY ${DERIVED_SOURCES_WPE_API_DIR})
file(MAKE_DIRECTORY ${FORWARDING_HEADERS_WPE_DIR})
file(MAKE_DIRECTORY ${FORWARDING_HEADERS_WPE_EXTENSION_DIR})
file(MAKE_DIRECTORY ${FORWARDING_HEADERS_WPE_DOM_DIR})

configure_file(UIProcess/API/wpe/CyberKitVersion.h.in ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitVersion.h)
configure_file(wpe/wpe-webkit.pc.in ${WPE_PKGCONFIG_FILE} @ONLY)
configure_file(wpe/wpe-web-extension.pc.in ${WPEWebExtension_PKGCONFIG_FILE} @ONLY)

add_definitions(-DWEBKIT2_COMPILATION)

add_definitions(-DLIBDIR="${LIB_INSTALL_DIR}")
add_definitions(-DPKGLIBDIR="${LIB_INSTALL_DIR}/wpe-webkit-${WPE_API_VERSION}")
add_definitions(-DPKGLIBEXECDIR="${LIBEXEC_INSTALL_DIR}")
add_definitions(-DDATADIR="${CMAKE_INSTALL_FULL_DATADIR}")
add_definitions(-DLOCALEDIR="${CMAKE_INSTALL_FULL_LOCALEDIR}")

if (NOT DEVELOPER_MODE AND NOT CMAKE_SYSTEM_NAME MATCHES "Darwin")
    WEBKIT_ADD_TARGET_PROPERTIES(CyberKit LINK_FLAGS "-Wl,--version-script,${CMAKE_CURRENT_SOURCE_DIR}/webkitglib-symbols.map")
endif ()

set(CyberKit_USE_PREFIX_HEADER ON)

add_custom_target(webkitwpe-forwarding-headers
    COMMAND ${PERL_EXECUTABLE} ${WEBKIT_DIR}/Scripts/generate-forwarding-headers.pl --include-path ${WEBKIT_DIR} --output ${FORWARDING_HEADERS_DIR} --platform wpe --platform soup
)

 # These symbolic link allows includes like #include <wpe/WebkitWebView.h> which simulates installed headers.
add_custom_command(
    OUTPUT ${FORWARDING_HEADERS_WPE_DIR}/wpe
    DEPENDS ${WEBKIT_DIR}/UIProcess/API/wpe
    COMMAND ln -n -s -f ${WEBKIT_DIR}/UIProcess/API/wpe ${FORWARDING_HEADERS_WPE_DIR}/wpe
)

add_custom_command(
    OUTPUT ${FORWARDING_HEADERS_WPE_EXTENSION_DIR}/wpe
    DEPENDS ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe
    COMMAND ln -n -s -f ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe ${FORWARDING_HEADERS_WPE_EXTENSION_DIR}/wpe
)

add_custom_command(
    OUTPUT ${FORWARDING_HEADERS_WPE_DOM_DIR}/wpe
    DEPENDS ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM
    COMMAND ln -n -s -f ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM ${FORWARDING_HEADERS_WPE_DOM_DIR}/wpe
    VERBATIM
)

add_custom_target(webkitwpe-fake-api-headers
    DEPENDS ${FORWARDING_HEADERS_WPE_DIR}/wpe
            ${FORWARDING_HEADERS_WPE_EXTENSION_DIR}/wpe
            ${FORWARDING_HEADERS_WPE_DOM_DIR}/wpe
)

list(APPEND CyberKit_DEPENDENCIES
    webkitwpe-fake-api-headers
    webkitwpe-forwarding-headers
)

list(APPEND WebProcess_SOURCES
    WebProcess/EntryPoint/unix/WebProcessMain.cpp
)

list(APPEND NetworkProcess_SOURCES
    NetworkProcess/EntryPoint/unix/NetworkProcessMain.cpp
)

list(APPEND GPUProcess_SOURCES
    GPUProcess/EntryPoint/unix/GPUProcessMain.cpp
)

list(APPEND CyberKit_UNIFIED_SOURCE_LIST_FILES
    "SourcesWPE.txt"
)

list(APPEND CyberKit_DERIVED_SOURCES
    ${CyberKit_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.c

    ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitEnumTypes.cpp
    ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitWebProcessEnumTypes.cpp
)

set(WPE_API_INSTALLED_HEADERS
    ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitEnumTypes.h
    ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitVersion.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitApplicationInfo.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitAuthenticationRequest.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitAutocleanups.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitAutomationSession.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitBackForwardList.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitBackForwardListItem.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitCredential.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitColor.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitContextMenu.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitContextMenuActions.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitContextMenuItem.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitCookieManager.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitDefines.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitDeviceInfoPermissionRequest.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitDownload.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitEditingCommands.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitEditorState.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitError.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitFaviconDatabase.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitFileChooserRequest.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitFindController.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitFormSubmissionRequest.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitGeolocationManager.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitGeolocationPermissionRequest.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitHitTestResult.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitInputMethodContext.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitInstallMissingMediaPluginsPermissionRequest.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitJavascriptResult.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitMimeInfo.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitNavigationAction.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitNavigationPolicyDecision.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitNetworkProxySettings.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitNotificationPermissionRequest.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitNotification.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitOptionMenu.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitOptionMenuItem.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitPermissionRequest.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitPlugin.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitPolicyDecision.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitRectangle.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitResponsePolicyDecision.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitScriptDialog.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitSecurityManager.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitSecurityOrigin.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitSettings.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitURIRequest.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitURIResponse.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitURISchemeRequest.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitURIUtilities.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitUserContent.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitUserContentFilterStore.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitUserContentManager.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitUserMediaPermissionRequest.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitUserMessage.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitWebContext.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitWebResource.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitWebView.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitWebViewBackend.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitWebViewSessionState.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitWebsiteData.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitWebsiteDataAccessPermissionRequest.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitWebsiteDataManager.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitWindowProperties.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitWebsitePolicies.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/webkit.h
)

set(WPE_WEB_EXTENSION_API_INSTALLED_HEADERS
    ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitWebProcessEnumTypes.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/CyberKitConsoleMessage.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/CyberKitFrame.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/CyberKitScriptWorld.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/CyberKitWebEditor.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/CyberKitWebExtension.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/CyberKitWebExtensionAutocleanups.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/CyberKitWebHitTestResult.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/CyberKitWebPage.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/webkit-web-extension.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM/webkitdom.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM/CyberKitDOMDefines.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM/CyberKitDOMDocument.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM/CyberKitDOMElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM/CyberKitDOMNode.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM/CyberKitDOMObject.h
)

# To generate CyberKitEnumTypes.h we want to use all installed headers, except CyberKitEnumTypes.h itself.
set(WPE_ENUM_GENERATION_HEADERS ${WPE_API_INSTALLED_HEADERS})
list(REMOVE_ITEM WPE_ENUM_GENERATION_HEADERS ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitEnumTypes.h)
add_custom_command(
    OUTPUT ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitEnumTypes.h
           ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitEnumTypes.cpp
    DEPENDS ${WPE_ENUM_GENERATION_HEADERS}

    COMMAND glib-mkenums --template ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitEnumTypes.h.template ${WPE_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ | sed s/WEBKIT_TYPE_KIT/WEBKIT_TYPE/ > ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitEnumTypes.h

    COMMAND glib-mkenums --template ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitEnumTypes.cpp.template ${WPE_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ > ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitEnumTypes.cpp
    VERBATIM
)

set(WPE_WEB_PROCESS_ENUM_GENERATION_HEADERS ${WPE_WEB_EXTENSION_API_INSTALLED_HEADERS})
list(REMOVE_ITEM WPE_WEB_PROCESS_ENUM_GENERATION_HEADERS ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitWebProcessEnumTypes.h)
add_custom_command(
    OUTPUT ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitWebProcessEnumTypes.h
           ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitWebProcessEnumTypes.cpp
    DEPENDS ${WPE_WEB_PROCESS_ENUM_GENERATION_HEADERS}

    COMMAND glib-mkenums --template ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/CyberKitWebProcessEnumTypes.h.template ${WPE_WEB_PROCESS_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ | sed s/WEBKIT_TYPE_KIT/WEBKIT_TYPE/ > ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitWebProcessEnumTypes.h

    COMMAND glib-mkenums --template ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/CyberKitWebProcessEnumTypes.cpp.template ${WPE_WEB_PROCESS_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ > ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitWebProcessEnumTypes.cpp
    VERBATIM
)

set(CyberKitResources
)

if (ENABLE_WEB_AUDIO)
    list(APPEND CyberKitResources
        "        <file alias=\"audio/Composite\">Composite.wav</file>\n"
    )
endif ()

file(WRITE ${CyberKit_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.xml
    "<?xml version=1.0 encoding=UTF-8?>\n"
    "<gresources>\n"
    "    <gresource prefix=\"/org/webkitwpe/resources\">\n"
    ${CyberKitResources}
    "    </gresource>\n"
    "</gresources>\n"
)

add_custom_command(
    OUTPUT ${CyberKit_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.c
    DEPENDS ${CyberKit_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.xml
    COMMAND glib-compile-resources --generate --sourcedir=${CMAKE_SOURCE_DIR}/Source/CyberCore/Resources --sourcedir=${CMAKE_SOURCE_DIR}/Source/CyberCore/platform/audio/resources --target=${CyberKit_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.c ${CyberKit_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.xml
    VERBATIM
)

list(APPEND CyberKit_INCLUDE_DIRECTORIES
    "${DERIVED_SOURCES_JAVASCRIPCOREWPE_DIR}"
    "${FORWARDING_HEADERS_DIR}"
    "${FORWARDING_HEADERS_DIR}/CyberScriptCore/"
    "${FORWARDING_HEADERS_DIR}/CyberScriptCore/glib"
    "${FORWARDING_HEADERS_WPE_DIR}"
    "${FORWARDING_HEADERS_WPE_EXTENSION_DIR}"
    "${FORWARDING_HEADERS_WPE_DOM_DIR}"
    "${DERIVED_SOURCES_DIR}"
    "${DERIVED_SOURCES_WPE_API_DIR}"
    "${WEBKIT_DIR}/NetworkProcess/glib"
    "${WEBKIT_DIR}/NetworkProcess/soup"
    "${WEBKIT_DIR}/Platform/IPC/glib"
    "${WEBKIT_DIR}/Platform/IPC/unix"
    "${WEBKIT_DIR}/Platform/classifier"
    "${WEBKIT_DIR}/Platform/generic"
    "${WEBKIT_DIR}/Shared/API/c/wpe"
    "${WEBKIT_DIR}/Shared/API/glib"
    "${WEBKIT_DIR}/Shared/CoordinatedGraphics"
    "${WEBKIT_DIR}/Shared/CoordinatedGraphics/threadedcompositor"
    "${WEBKIT_DIR}/Shared/glib"
    "${WEBKIT_DIR}/Shared/libwpe"
    "${WEBKIT_DIR}/Shared/soup"
    "${WEBKIT_DIR}/UIProcess/API/C/cairo"
    "${WEBKIT_DIR}/UIProcess/API/C/wpe"
    "${WEBKIT_DIR}/UIProcess/API/glib"
    "${WEBKIT_DIR}/UIProcess/API/wpe"
    "${WEBKIT_DIR}/UIProcess/CoordinatedGraphics"
    "${WEBKIT_DIR}/UIProcess/geoclue"
    "${WEBKIT_DIR}/UIProcess/gstreamer"
    "${WEBKIT_DIR}/UIProcess/linux"
    "${WEBKIT_DIR}/UIProcess/soup"
    "${WEBKIT_DIR}/UIProcess/wpe"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/DOM"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM"
    "${WEBKIT_DIR}/WebProcess/soup"
    "${WEBKIT_DIR}/WebProcess/CyberCoreSupport/soup"
    "${WEBKIT_DIR}/WebProcess/WebPage/CoordinatedGraphics"
    "${WEBKIT_DIR}/WebProcess/WebPage/atk"
    "${WEBKIT_DIR}/WebProcess/WebPage/libwpe"
    "${WEBKIT_DIR}/WebProcess/WebPage/wpe"
    "${WTF_DIR}/wtf/gtk/"
    "${WTF_DIR}/wtf/gobject"
    "${WTF_DIR}"
)

list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
    ${ATK_INCLUDE_DIRS}
    ${ATK_BRIDGE_INCLUDE_DIRS}
    ${GIO_UNIX_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${LIBSECCOMP_INCLUDE_DIRS}
    ${LIBSOUP_INCLUDE_DIRS}
)

list(APPEND CyberKit_LIBRARIES
    Cairo::Cairo
    Freetype::Freetype
    HarfBuzz::HarfBuzz
    HarfBuzz::ICU
    WPE::libwpe
    ${ATK_LIBRARIES}
    ${ATK_BRIDGE_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${GLIB_GMODULE_LIBRARIES}
    ${LIBSECCOMP_LIBRARIES}
    ${LIBSOUP_LIBRARIES}
)

if (USE_GSTREAMER_FULL)
    list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
        ${GSTREAMER_FULL_INCLUDE_DIRS}
    )
    list(APPEND CyberKit_LIBRARIES
        ${GSTREAMER_FULL_LIBRARIES}
    )
else ()
    list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
        ${GSTREAMER_INCLUDE_DIRS}
        ${GSTREAMER_AUDIO_INCLUDE_DIRS}
        ${GSTREAMER_PBUTILS_INCLUDE_DIRS}
        ${GSTREAMER_VIDEO_INCLUDE_DIRS}
    )
    list(APPEND CyberKit_LIBRARIES
        ${GSTREAMER_LIBRARIES}
    )
endif ()

WEBKIT_BUILD_INSPECTOR_GRESOURCES(${WebInspectorUI_DERIVED_SOURCES_DIR})
list(APPEND WPEWebInspectorResources_DERIVED_SOURCES
    ${WebInspectorUI_DERIVED_SOURCES_DIR}/InspectorGResourceBundle.c
)

list(APPEND WPEWebInspectorResources_LIBRARIES
    ${GLIB_GIO_LIBRARIES}
)

list(APPEND WPEWebInspectorResources_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
)

add_library(WPEWebInspectorResources SHARED ${WPEWebInspectorResources_DERIVED_SOURCES})
add_dependencies(WPEWebInspectorResources CyberKit)
target_link_libraries(WPEWebInspectorResources ${WPEWebInspectorResources_LIBRARIES})
target_include_directories(WPEWebInspectorResources SYSTEM PUBLIC ${WPEWebInspectorResources_SYSTEM_INCLUDE_DIRECTORIES})
install(TARGETS WPEWebInspectorResources DESTINATION "${LIB_INSTALL_DIR}/wpe-webkit-${WPE_API_VERSION}")

add_library(WPEInjectedBundle MODULE "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitInjectedBundleMain.cpp")
ADD_WEBKIT_PREFIX_HEADER(WPEInjectedBundle)
target_link_libraries(WPEInjectedBundle CyberKit)

target_include_directories(WPEInjectedBundle PRIVATE ${CyberKit_INCLUDE_DIRECTORIES} ${CyberKit_PRIVATE_INCLUDE_DIRECTORIES})
target_include_directories(WPEInjectedBundle SYSTEM PRIVATE ${CyberKit_SYSTEM_INCLUDE_DIRECTORIES})

file(WRITE ${CMAKE_BINARY_DIR}/gtkdoc-wpe.cfg
    "[wpe-${WPE_API_VERSION}]\n"
    "pkgconfig_file=${WPE_PKGCONFIG_FILE}\n"
    "decorator=WEBKIT_API|WEBKIT_DEPRECATED|WEBKIT_DEPRECATED_FOR\\(.+\\)\n"
    "deprecation_guard=WEBKIT_DISABLE_DEPRECATED\n"
    "namespace=webkit\n"
    "cflags=-I${CMAKE_SOURCE_DIR}/Source\n"
    "       -I${WEBKIT_DIR}/Shared/API/glib\n"
    "       -I${WEBKIT_DIR}/UIProcess/API/glib\n"
    "       -I${WEBKIT_DIR}/UIProcess/API/wpe\n"
    "       -I${FORWARDING_HEADERS_WPE_DIR}\n"
    "doc_dir=${WEBKIT_DIR}/UIProcess/API/wpe/docs\n"
    "source_dirs=${WEBKIT_DIR}/Shared/API/glib\n"
    "            ${WEBKIT_DIR}/UIProcess/API/glib\n"
    "            ${WEBKIT_DIR}/UIProcess/API/wpe\n"
    "            ${DERIVED_SOURCES_WPE_API_DIR}\n"
    "headers=${WPE_ENUM_GENERATION_HEADERS}\n"
    "main_sgml_file=wpe-docs.sgml\n"
)

file(WRITE ${CMAKE_BINARY_DIR}/gtkdoc-webextensions.cfg
    "[wpe-webextensions-${WPE_API_VERSION}]\n"
    "pkgconfig_file=${WPEWebExtension_PKGCONFIG_FILE}\n"
    "decorator=WEBKIT_API|WEBKIT_DEPRECATED|WEBKIT_DEPRECATED_FOR\\(.+\\)\n"
    "deprecation_guard=WEBKIT_DISABLE_DEPRECATED\n"
    "namespace=webkit_webextensions\n"
    "cflags=-I${CMAKE_SOURCE_DIR}/Source\n"
    "       -I${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe\n"
    "       -I${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM\n"
    "       -I${FORWARDING_HEADERS_WPE_DIR}\n"
    "doc_dir=${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/docs\n"
    "source_dirs=${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib\n"
    "            ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/DOM\n"
    "            ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe\n"
    "            ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/DOM\n"
    "headers=${WPE_WEB_EXTENSION_API_INSTALLED_HEADERS}\n"
    "main_sgml_file=wpe-webextensions-docs.sgml\n"
)

if (ENABLE_WPE_QT_API)
    set(qtwpe_SOURCES
        ${WEBKIT_DIR}/UIProcess/API/wpe/qt/WPEQtViewBackend.cpp
        ${WEBKIT_DIR}/UIProcess/API/wpe/qt/WPEQmlExtensionPlugin.cpp
        ${WEBKIT_DIR}/UIProcess/API/wpe/qt/WPEQtView.cpp
        ${WEBKIT_DIR}/UIProcess/API/wpe/qt/WPEQtViewLoadRequest.cpp
    )

    set(qtwpe_LIBRARIES
        Qt5::Core Qt5::Quick
        CyberKit
        ${LIBEPOXY_LIBRARIES}
        ${WPEBACKEND_FDO_LIBRARIES}
    )

    set(qtwpe_INCLUDE_DIRECTORIES
        ${CMAKE_BINARY_DIR}
        ${GLIB_INCLUDE_DIRS}
        ${Qt5_INCLUDE_DIRS}
        ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
        ${LIBEPOXY_INCLUDE_DIRS}
        ${LIBSOUP_INCLUDE_DIRS}
        ${WPEBACKEND_FDO_INCLUDE_DIRS}
    )

    list(APPEND WPE_API_INSTALLED_HEADERS
        ${WEBKIT_DIR}/UIProcess/API/wpe/qt/WPEQtView.h
        ${WEBKIT_DIR}/UIProcess/API/wpe/qt/WPEQtViewLoadRequest.h
    )

    add_library(qtwpe SHARED ${qtwpe_SOURCES})
    set_target_properties(qtwpe PROPERTIES
        OUTPUT_NAME qtwpe
        AUTOMOC ON
    )
    target_compile_definitions(qtwpe PUBLIC QT_NO_KEYWORDS=1)
    target_link_libraries(qtwpe ${qtwpe_LIBRARIES})
    target_include_directories(qtwpe SYSTEM PRIVATE ${qtwpe_INCLUDE_DIRECTORIES})
    install(TARGETS qtwpe DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/qt5/qml/org/wpewebkit/qtwpe/")
    install(FILES ${WEBKIT_DIR}/UIProcess/API/wpe/qt/qmldir DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/qt5/qml/org/wpewebkit/qtwpe/")

    file(MAKE_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/qt5/qml/org/wpewebkit/qtwpe)
    add_custom_command(TARGET qtwpe POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libqtwpe.so
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/qt5/qml/org/wpewebkit/qtwpe)
    add_custom_command(TARGET qtwpe POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${WEBKIT_DIR}/UIProcess/API/wpe/qt/qmldir
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/qt5/qml/org/wpewebkit/qtwpe)
endif ()

install(TARGETS WPEInjectedBundle
        DESTINATION "${LIB_INSTALL_DIR}/wpe-webkit-${WPE_API_VERSION}/injected-bundle"
)

install(FILES "${CMAKE_BINARY_DIR}/wpe-webkit-${WPE_API_VERSION}.pc"
              "${CMAKE_BINARY_DIR}/wpe-web-extension-${WPE_API_VERSION}.pc"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
        COMPONENT "Development"
)

install(FILES ${WPE_API_INSTALLED_HEADERS}
              ${WPE_WEB_EXTENSION_API_INSTALLED_HEADERS}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/wpe-webkit-${WPE_API_VERSION}/wpe"
        COMPONENT "Development"
)
