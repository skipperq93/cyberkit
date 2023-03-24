include(GNUInstallDirs)
include(GLibMacros)
include(InspectorGResources.cmake)

if (ENABLE_PDFJS)
    include(PdfJSGResources.cmake)
endif ()

if (ENABLE_MODERN_MEDIA_CONTROLS)
    include(ModernMediaControlsGResources.cmake)
endif ()

set(CyberKit_OUTPUT_NAME WPECyberKit-${WPE_API_VERSION})
set(WebProcess_OUTPUT_NAME WPEWebProcess)
set(NetworkProcess_OUTPUT_NAME WPENetworkProcess)
set(GPUProcess_OUTPUT_NAME WPEGPUProcess)

file(MAKE_DIRECTORY ${DERIVED_SOURCES_WPE_API_DIR})
file(MAKE_DIRECTORY ${FORWARDING_HEADERS_WPE_DIR})
file(MAKE_DIRECTORY ${FORWARDING_HEADERS_WPE_EXTENSION_DIR})
file(MAKE_DIRECTORY ${FORWARDING_HEADERS_WPE_JSC_DIR})

configure_file(Shared/glib/BuildRevision.h.in ${FORWARDING_HEADERS_WPE_DIR}/BuildRevision.h)
configure_file(UIProcess/API/wpe/CyberKitVersion.h.in ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitVersion.h)
configure_file(wpe/wpe-webkit.pc.in ${WPE_PKGCONFIG_FILE} @ONLY)
configure_file(wpe/wpe-web-process-extension.pc.in ${WPEWebProcessExtension_PKGCONFIG_FILE} @ONLY)
configure_file(wpe/wpe-webkit-uninstalled.pc.in ${WPE_Uninstalled_PKGCONFIG_FILE} @ONLY)
configure_file(wpe/wpe-web-process-extension-uninstalled.pc.in ${WPEWebProcessExtension_Uninstalled_PKGCONFIG_FILE} @ONLY)

if (EXISTS "${TOOLS_DIR}/glib/apply-build-revision-to-files.py")
    add_custom_target(CyberKit-build-revision
        ${PYTHON_EXECUTABLE} "${TOOLS_DIR}/glib/apply-build-revision-to-files.py" ${FORWARDING_HEADERS_WPE_DIR}/BuildRevision.h
        DEPENDS ${FORWARDING_HEADERS_WPE_DIR}/BuildRevision.h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} VERBATIM)
    list(APPEND CyberKit_DEPENDENCIES
        CyberKit-build-revision
    )
endif ()

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
    OUTPUT ${FORWARDING_HEADERS_WPE_JSC_DIR}/jsc
    DEPENDS ${JAVASCRIPTCORE_DIR}/API/glib/
    COMMAND ln -n -s -f ${JAVASCRIPTCORE_DIR}/API/glib ${FORWARDING_HEADERS_WPE_JSC_DIR}/jsc
    VERBATIM
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
    ${CyberKit_DERIVED_SOURCES_DIR}/CyberKitDirectoryInputStreamData.cpp

    ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitEnumTypes.cpp
    ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitWebProcessEnumTypes.cpp
)

if (ENABLE_PDFJS)
    list(APPEND CyberKit_DERIVED_SOURCES
        ${CyberKit_DERIVED_SOURCES_DIR}/PdfJSGResourceBundle.c
        ${CyberKit_DERIVED_SOURCES_DIR}/PdfJSGResourceBundleExtras.c
    )

    WEBKIT_BUILD_PDFJS_GRESOURCES(${CyberKit_DERIVED_SOURCES_DIR})
endif ()

if (ENABLE_MODERN_MEDIA_CONTROLS)
  list(APPEND CyberKit_DERIVED_SOURCES
      ${CyberKit_DERIVED_SOURCES_DIR}/ModernMediaControlsGResourceBundle.c
  )

  WEBKIT_BUILD_MODERN_MEDIA_CONTROLS_GRESOURCES(${CyberKit_DERIVED_SOURCES_DIR})
endif ()

set(CyberKit_DirectoryInputStream_DATA
    ${WEBKIT_DIR}/NetworkProcess/soup/Resources/directory.css
    ${WEBKIT_DIR}/NetworkProcess/soup/Resources/directory.js
)

add_custom_command(
    OUTPUT ${CyberKit_DERIVED_SOURCES_DIR}/CyberKitDirectoryInputStreamData.cpp ${CyberKit_DERIVED_SOURCES_DIR}/CyberKitDirectoryInputStreamData.h
    MAIN_DEPENDENCY ${WEBCORE_DIR}/css/make-css-file-arrays.pl
    DEPENDS ${CyberKit_DirectoryInputStream_DATA}
    COMMAND ${PERL_EXECUTABLE} ${WEBCORE_DIR}/css/make-css-file-arrays.pl --defines "${FEATURE_DEFINES_WITH_SPACE_SEPARATOR}" --preprocessor "${CODE_GENERATOR_PREPROCESSOR}" ${CyberKit_DERIVED_SOURCES_DIR}/CyberKitDirectoryInputStreamData.h ${CyberKit_DERIVED_SOURCES_DIR}/CyberKitDirectoryInputStreamData.cpp ${CyberKit_DirectoryInputStream_DATA}
    VERBATIM
)

set(WPE_API_HEADER_TEMPLATES
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitApplicationInfo.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitAuthenticationRequest.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitAutocleanups.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitAutomationSession.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitBackForwardList.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitBackForwardListItem.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitCredential.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitContextMenu.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitContextMenuActions.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitContextMenuItem.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitCookieManager.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitDefines.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitDeviceInfoPermissionRequest.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitDownload.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitEditingCommands.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitEditorState.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitError.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitFileChooserRequest.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitFindController.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitFormSubmissionRequest.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitGeolocationManager.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitGeolocationPermissionRequest.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitHitTestResult.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitInputMethodContext.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitInstallMissingMediaPluginsPermissionRequest.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitMediaKeySystemPermissionRequest.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitMemoryPressureSettings.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitNavigationAction.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitNavigationPolicyDecision.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitNetworkProxySettings.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitNotificationPermissionRequest.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitNotification.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitOptionMenu.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitOptionMenuItem.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitPermissionRequest.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitPermissionStateQuery.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitPolicyDecision.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitResponsePolicyDecision.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitScriptDialog.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitSecurityManager.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitSecurityOrigin.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitSettings.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitURIRequest.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitURIResponse.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitURISchemeRequest.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitURISchemeResponse.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitURIUtilities.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitUserContent.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitUserContentFilterStore.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitUserContentManager.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitUserMediaPermissionRequest.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitUserMessage.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitWebContext.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitWebResource.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitWebView.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitWebViewSessionState.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitWebsiteData.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitWebsiteDataAccessPermissionRequest.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitWebsiteDataManager.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitWindowProperties.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitWebsitePolicies.h.in
    ${WEBKIT_DIR}/UIProcess/API/glib/webkit.h.in
)

if (ENABLE_2022_GLIB_API)
    list(APPEND WPE_API_HEADER_TEMPLATES
        ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitNetworkSession.h.in
    )
endif ()

set(WPE_API_INSTALLED_HEADERS
    ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitEnumTypes.h
    ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitVersion.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitColor.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitRectangle.h
    ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitWebViewBackend.h
)

set(WPE_WEB_PROCESS_EXTENSION_API_INSTALLED_HEADERS
    ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitWebProcessEnumTypes.h
)

set(WPE_WEB_PROCESS_EXTENSION_API_HEADER_TEMPLATES
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitFrame.h.in
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitScriptWorld.h.in
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebEditor.h.in
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebFormManager.h.in
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebHitTestResult.h.in
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebPage.h.in
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/webkit-web-process-extension.h.in
)

if (ENABLE_2022_GLIB_API)
    list(APPEND WPE_WEB_PROCESS_EXTENSION_API_HEADER_TEMPLATES
        ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebProcessExtension.h.in
    )
    list(APPEND CyberKit_SOURCES
        ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebProcessExtension.cpp
    )
else ()
    list(APPEND WPE_WEB_PROCESS_EXTENSION_API_HEADER_TEMPLATES
        ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebExtension.h.in
        ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebExtensionAutocleanups.h.in
    )
    list(APPEND CyberKit_SOURCES
        ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebExtension.cpp
    )
endif ()

set(WPE_FAKE_API_HEADERS
    ${FORWARDING_HEADERS_WPE_DIR}/wpe
    ${FORWARDING_HEADERS_WPE_EXTENSION_DIR}/wpe
    ${FORWARDING_HEADERS_WPE_JSC_DIR}/jsc
)

if (NOT ENABLE_2022_GLIB_API)
    include(PlatformWPEDeprecated.cmake)
endif ()

add_custom_target(webkitwpe-fake-api-headers
    DEPENDS ${WPE_FAKE_API_HEADERS}
)

list(APPEND CyberKit_DEPENDENCIES
    webkitwpe-fake-api-headers
    webkitwpe-forwarding-headers
)

GENERATE_GLIB_API_HEADERS(CyberKit WPE_API_HEADER_TEMPLATES
    ${DERIVED_SOURCES_WPE_API_DIR}
    WPE_API_INSTALLED_HEADERS
    "-DWTF_PLATFORM_GTK=0"
    "-DWTF_PLATFORM_WPE=1"
    "-DUSE_GTK4=0"
    "-DENABLE_2022_GLIB_API=$<BOOL:${ENABLE_2022_GLIB_API}>"
)

GENERATE_GLIB_API_HEADERS(CyberKit WPE_WEB_PROCESS_EXTENSION_API_HEADER_TEMPLATES
    ${DERIVED_SOURCES_WPE_API_DIR}
    WPE_WEB_PROCESS_EXTENSION_API_INSTALLED_HEADERS
    "-DWTF_PLATFORM_GTK=0"
    "-DWTF_PLATFORM_WPE=1"
    "-DUSE_GTK4=0"
    "-DENABLE_2022_GLIB_API=$<BOOL:${ENABLE_2022_GLIB_API}>"
)

if (NOT ENABLE_2022_GLIB_API)
    list(REMOVE_ITEM WPE_WEB_PROCESS_EXTENSION_API_INSTALLED_HEADERS ${DERIVED_SOURCES_WPE_API_DIR}/webkit-web-process-extension.h)
endif ()

# To generate CyberKitEnumTypes.h we want to use all installed headers, except CyberKitEnumTypes.h itself.
set(WPE_ENUM_GENERATION_HEADERS ${WPE_API_INSTALLED_HEADERS})
list(REMOVE_ITEM WPE_ENUM_GENERATION_HEADERS ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitEnumTypes.h)
add_custom_command(
    OUTPUT ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitEnumTypes.h
           ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitEnumTypes.cpp
    DEPENDS ${WPE_ENUM_GENERATION_HEADERS}

    COMMAND glib-mkenums --template ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitEnumTypes.h.in ${WPE_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ | sed s/WEBKIT_TYPE_KIT/WEBKIT_TYPE/ > ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitEnumTypes.h

    COMMAND glib-mkenums --template ${WEBKIT_DIR}/UIProcess/API/wpe/CyberKitEnumTypes.cpp.in ${WPE_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ > ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitEnumTypes.cpp
    VERBATIM
)

if (ENABLE_2022_GLIB_API)
    set(WPE_WEB_PROCESS_ENUM_HEADER_TEMPLATE "CyberKitWebProcessEnumTypesWPE2.h.in")
else ()
    set(WPE_WEB_PROCESS_ENUM_HEADER_TEMPLATE "CyberKitWebProcessEnumTypesWPE1.h.in")
endif ()

set(WPE_WEB_PROCESS_ENUM_GENERATION_HEADERS ${WPE_WEB_PROCESS_EXTENSION_API_INSTALLED_HEADERS})
list(REMOVE_ITEM WPE_WEB_PROCESS_ENUM_GENERATION_HEADERS ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitWebProcessEnumTypes.h)
add_custom_command(
    OUTPUT ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitWebProcessEnumTypes.h
           ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitWebProcessEnumTypes.cpp
    DEPENDS ${WPE_WEB_PROCESS_ENUM_GENERATION_HEADERS}

    COMMAND glib-mkenums --template ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/${WPE_WEB_PROCESS_ENUM_HEADER_TEMPLATE} ${WPE_WEB_PROCESS_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ | sed s/WEBKIT_TYPE_KIT/WEBKIT_TYPE/ > ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitWebProcessEnumTypes.h

    COMMAND glib-mkenums --template ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe/CyberKitWebProcessEnumTypes.cpp.in ${WPE_WEB_PROCESS_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ > ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitWebProcessEnumTypes.cpp
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
    OUTPUT ${CyberKit_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.c ${CyberKit_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.deps
    DEPENDS ${CyberKit_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.xml
    DEPFILE ${CyberKit_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.deps
    COMMAND glib-compile-resources --generate --sourcedir=${CMAKE_SOURCE_DIR}/Source/CyberCore/Resources --sourcedir=${CMAKE_SOURCE_DIR}/Source/CyberCore/platform/audio/resources --target=${CyberKit_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.c --dependency-file=${CyberKit_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.deps ${CyberKit_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.xml
    VERBATIM
)

list(APPEND CyberKit_INCLUDE_DIRECTORIES
    "${DERIVED_SOURCES_WPE_API_DIR}"
    "${FORWARDING_HEADERS_WPE_DIR}"
    "${FORWARDING_HEADERS_WPE_EXTENSION_DIR}"
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
    "${WEBKIT_DIR}/UIProcess/API/C/glib"
    "${WEBKIT_DIR}/UIProcess/API/C/wpe"
    "${WEBKIT_DIR}/UIProcess/API/glib"
    "${WEBKIT_DIR}/UIProcess/API/libwpe"
    "${WEBKIT_DIR}/UIProcess/API/wpe"
    "${WEBKIT_DIR}/UIProcess/CoordinatedGraphics"
    "${WEBKIT_DIR}/UIProcess/Inspector/glib"
    "${WEBKIT_DIR}/UIProcess/Launcher/glib"
    "${WEBKIT_DIR}/UIProcess/Launcher/libwpe"
    "${WEBKIT_DIR}/UIProcess/Notifications/glib/"
    "${WEBKIT_DIR}/UIProcess/geoclue"
    "${WEBKIT_DIR}/UIProcess/gstreamer"
    "${WEBKIT_DIR}/UIProcess/linux"
    "${WEBKIT_DIR}/UIProcess/soup"
    "${WEBKIT_DIR}/UIProcess/wpe"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/wpe"
    "${WEBKIT_DIR}/WebProcess/CyberCoreSupport/soup"
    "${WEBKIT_DIR}/WebProcess/WebPage/CoordinatedGraphics"
    "${WEBKIT_DIR}/WebProcess/WebPage/libwpe"
    "${WEBKIT_DIR}/WebProcess/WebPage/wpe"
    "${WEBKIT_DIR}/WebProcess/glib"
    "${WEBKIT_DIR}/WebProcess/soup"
)

list(APPEND CyberKit_PRIVATE_INCLUDE_DIRECTORIES
    "${CyberScriptCoreGLib_DERIVED_SOURCES_DIR}/jsc"
)

list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
    ${ATK_INCLUDE_DIRS}
    ${GIO_UNIX_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${LIBSOUP_INCLUDE_DIRS}
)

list(APPEND CyberKit_LIBRARIES
    Cairo::Cairo
    Freetype::Freetype
    HarfBuzz::HarfBuzz
    HarfBuzz::ICU
    WPE::libwpe
    ${ATK_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${GLIB_GMODULE_LIBRARIES}
    ${LIBSOUP_LIBRARIES}
)

if (ENABLE_ACCESSIBILITY)
    list(APPEND CyberKit_LIBRARIES ATK::Bridge)
endif ()

if (ENABLE_BUBBLEWRAP_SANDBOX)
    list(APPEND CyberKit_LIBRARIES Libseccomp::Libseccomp)
endif ()

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

if (ENABLE_MEDIA_STREAM)
    list(APPEND CyberKit_SOURCES
        UIProcess/glib/UserMediaPermissionRequestManagerProxyGLib.cpp

        WebProcess/glib/UserMediaCaptureManager.cpp
    )
    list(APPEND CyberKit_MESSAGES_IN_FILES
        WebProcess/glib/UserMediaCaptureManager
    )
endif ()

if (ENABLE_BREAKPAD)
    list(APPEND CyberKit_SOURCES
        Shared/unix/BreakpadExceptionHandler.cpp
    )
    list(APPEND CyberKit_LIBRARIES
        Breakpad::Breakpad
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

target_include_directories(WPEInjectedBundle PRIVATE $<TARGET_PROPERTY:CyberKit,INCLUDE_DIRECTORIES>)

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
        ${GLIB_GOBJECT_LIBRARIES}
        ${GLIB_LIBRARIES}
        ${LIBEPOXY_LIBRARIES}
        ${WPEBACKEND_FDO_LIBRARIES}
    )

    set(qtwpe_INCLUDE_DIRECTORIES
        $<TARGET_PROPERTY:CyberKit,INCLUDE_DIRECTORIES>
        ${CyberScriptCoreGLib_FRAMEWORK_HEADERS_DIR}
        ${CMAKE_BINARY_DIR}
        ${GLIB_INCLUDE_DIRS}
        ${Qt5_INCLUDE_DIRS}
        ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
        ${LIBEPOXY_INCLUDE_DIRS}
        ${LIBSOUP_INCLUDE_DIRS}
        ${WPE_INCLUDE_DIRS}
        ${WPEBACKEND_FDO_INCLUDE_DIRS}
    )

    list(APPEND WPE_QT_API_INSTALLED_HEADERS
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
    target_include_directories(qtwpe PRIVATE ${qtwpe_INCLUDE_DIRECTORIES})
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

if (ENABLE_2022_GLIB_API)
    install(FILES "${CMAKE_BINARY_DIR}/wpe-webkit-${WPE_API_VERSION}.pc"
                  "${CMAKE_BINARY_DIR}/wpe-web-process-extension-${WPE_API_VERSION}.pc"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
            COMPONENT "Development"
    )
else ()
    install(FILES "${CMAKE_BINARY_DIR}/wpe-webkit-${WPE_API_VERSION}.pc"
                  "${CMAKE_BINARY_DIR}/wpe-web-extension-${WPE_API_VERSION}.pc"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
            COMPONENT "Development"
    )
endif ()

install(FILES ${WPE_API_INSTALLED_HEADERS}
              ${WPE_QT_API_INSTALLED_HEADERS}
              ${WPE_WEB_PROCESS_EXTENSION_API_INSTALLED_HEADERS}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/wpe-webkit-${WPE_API_VERSION}/wpe"
        COMPONENT "Development"
)

# XXX: Using ${CyberScriptCore_INSTALLED_HEADERS} here expands to nothing.
GI_INTROSPECT(WPECyberScriptCore ${WPE_API_VERSION} jsc/jsc.h
    TARGET CyberKit
    PACKAGE wpe-javascriptcore
    SYMBOL_PREFIX jsc
    DEPENDENCIES GObject-2.0
    OPTIONS
        -I${CyberScriptCoreGLib_FRAMEWORK_HEADERS_DIR}
        -I${CyberScriptCoreGLib_DERIVED_SOURCES_DIR}
    SOURCES
        ${JAVASCRIPTCORE_DIR}/API/glib/JSCOptions.h
        ${CyberScriptCoreGLib_DERIVED_SOURCES_DIR}/jsc/JSCClass.h
        ${CyberScriptCoreGLib_DERIVED_SOURCES_DIR}/jsc/JSCContext.h
        ${CyberScriptCoreGLib_DERIVED_SOURCES_DIR}/jsc/JSCDefines.h
        ${CyberScriptCoreGLib_DERIVED_SOURCES_DIR}/jsc/JSCException.h
        ${CyberScriptCoreGLib_DERIVED_SOURCES_DIR}/jsc/JSCValue.h
        ${CyberScriptCoreGLib_DERIVED_SOURCES_DIR}/jsc/JSCVersion.h
        ${CyberScriptCoreGLib_DERIVED_SOURCES_DIR}/jsc/JSCVirtualMachine.h
        ${CyberScriptCoreGLib_DERIVED_SOURCES_DIR}/jsc/JSCWeakValue.h
        ${CyberScriptCoreGLib_DERIVED_SOURCES_DIR}/jsc/jsc.h
        ${JAVASCRIPTCORE_DIR}/API/glib
    NO_IMPLICIT_SOURCES
)
GI_DOCGEN(WPECyberScriptCore "${JAVASCRIPTCORE_DIR}/API/glib/docs/jsc.toml.in")

set(WPE_SOURCES_FOR_INTROSPECTION
    UIProcess/API/wpe/CyberKitColor.cpp
    UIProcess/API/wpe/CyberKitInputMethodContextWPE.cpp
    UIProcess/API/wpe/CyberKitRectangle.cpp
    UIProcess/API/wpe/CyberKitWebViewBackend.cpp
    UIProcess/API/wpe/CyberKitWebViewWPE.cpp
 )

 if (ENABLE_2022_GLIB_API)
     list(APPEND WPE_SOURCES_FOR_INTROSPECTION UIProcess/API/wpe/CyberKitWebViewWPE2.cpp)
 else ()
     list(APPEND WPE_SOURCES_FOR_INTROSPECTION UIProcess/API/wpe/CyberKitWebViewWPE1.cpp)
 endif ()

GI_INTROSPECT(WPECyberKit ${WPE_API_VERSION} wpe/webkit.h
    TARGET CyberKit
    PACKAGE wpe-webkit
    IDENTIFIER_PREFIX CyberKit
    SYMBOL_PREFIX webkit
    DEPENDENCIES
        WPECyberScriptCore
        Soup-${SOUP_API_VERSION}:libsoup-${SOUP_API_VERSION}
    OPTIONS
        -I${CyberScriptCoreGLib_FRAMEWORK_HEADERS_DIR}
        -I${CyberScriptCoreGLib_DERIVED_SOURCES_DIR}
    SOURCES
        ${WPE_API_INSTALLED_HEADERS}
        Shared/API/glib
        UIProcess/API/glib
    NO_IMPLICIT_SOURCES
)
GI_DOCGEN(WPECyberKit wpe/wpewebkit.toml.in)

if (ENABLE_2022_GLIB_API)
    set(WPE_WEB_PROCESS_EXTENSION_API_NAME "WPEWebProcessExtension")
    set(WPE_WEB_PROCESS_EXTENSION_PACKAGE_NAME "wpe-web-process-extension")
else ()
    set(WPE_WEB_PROCESS_EXTENSION_API_NAME "WPEWebExtension")
    set(WPE_WEB_PROCESS_EXTENSION_PACKAGE_NAME "wpe-web-extension")
endif ()

GI_INTROSPECT(${WPE_WEB_PROCESS_EXTENSION_API_NAME} ${WPE_API_VERSION} wpe/${WPE_WEB_PROCESS_EXTENSION_PACKAGE_NAME}.h
    TARGET CyberKit
    PACKAGE ${WPE_WEB_PROCESS_EXTENSION_PACKAGE_NAME}
    IDENTIFIER_PREFIX CyberKit
    SYMBOL_PREFIX webkit
    DEPENDENCIES
        WPECyberScriptCore
        Soup-${SOUP_API_VERSION}:libsoup-${SOUP_API_VERSION}
    OPTIONS
        -I${CyberScriptCoreGLib_FRAMEWORK_HEADERS_DIR}
        -I${CyberScriptCoreGLib_DERIVED_SOURCES_DIR}
    SOURCES
        ${WPE_WEB_PROCESS_EXTENSION_API_INSTALLED_HEADERS}
        ${WPE_DOM_SOURCES_FOR_INTROSPECTION}
        ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitContextMenu.h
        ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitContextMenuActions.h
        ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitContextMenuItem.h
        ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitHitTestResult.h
        ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitUserMessage.h
        ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitURIRequest.h
        ${DERIVED_SOURCES_WPE_API_DIR}/CyberKitURIResponse.h
        Shared/API/glib/CyberKitContextMenu.cpp
        Shared/API/glib/CyberKitContextMenuItem.cpp
        Shared/API/glib/CyberKitHitTestResult.cpp
        Shared/API/glib/CyberKitUserMessage.cpp
        Shared/API/glib/CyberKitURIRequest.cpp
        Shared/API/glib/CyberKitURIResponse.cpp
        WebProcess/InjectedBundle/API/glib
    NO_IMPLICIT_SOURCES
)
GI_DOCGEN(${WPE_WEB_PROCESS_EXTENSION_API_NAME} wpe/wpe-web-process-extension.toml.in)
