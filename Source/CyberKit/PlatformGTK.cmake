include(GLibMacros)
include(InspectorGResources.cmake)

if (ENABLE_PDFJS)
    include(PdfJSGResources.cmake)
endif ()

if (ENABLE_MODERN_MEDIA_CONTROLS)
    include(ModernMediaControlsGResources.cmake)
endif ()

set(CyberKit_OUTPUT_NAME webkit${WEBKITGTK_API_INFIX}gtk-${WEBKITGTK_API_VERSION})
set(WebProcess_OUTPUT_NAME CyberKitWebProcess)
set(NetworkProcess_OUTPUT_NAME CyberKitNetworkProcess)
set(GPUProcess_OUTPUT_NAME CyberKitGPUProcess)

file(MAKE_DIRECTORY ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit)
file(MAKE_DIRECTORY ${CyberKitGTK_FRAMEWORK_HEADERS_DIR})
file(MAKE_DIRECTORY ${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/webkitgtk-${WEBKITGTK_API_VERSION})
file(MAKE_DIRECTORY ${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/webkitgtk-web-process-extension)

configure_file(Shared/glib/BuildRevision.h.in ${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/BuildRevision.h)
configure_file(UIProcess/API/gtk/CyberKitVersion.h.in ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitVersion.h)
configure_file(gtk/webkitgtk.pc.in ${CyberKitGTK_PKGCONFIG_FILE} @ONLY)
configure_file(gtk/webkitgtk-web-process-extension.pc.in ${CyberKitGTKWebProcessExtension_PKGCONFIG_FILE} @ONLY)

if (EXISTS "${TOOLS_DIR}/glib/apply-build-revision-to-files.py")
    add_custom_target(CyberKit-build-revision
        ${PYTHON_EXECUTABLE} "${TOOLS_DIR}/glib/apply-build-revision-to-files.py" ${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/BuildRevision.h ${CyberKitGTK_PKGCONFIG_FILE} ${CyberKitGTKWebProcessExtension_PKGCONFIG_FILE}
        DEPENDS ${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/BuildRevision.h ${CyberKitGTK_PKGCONFIG_FILE} ${CyberKitGTKWebProcessExtension_PKGCONFIG_FILE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} VERBATIM)
    list(APPEND CyberKit_DEPENDENCIES
        CyberKit-build-revision
    )
endif ()

add_definitions(-DPKGLIBEXECDIR="${LIBEXEC_INSTALL_DIR}")
add_definitions(-DLOCALEDIR="${CMAKE_INSTALL_FULL_LOCALEDIR}")
add_definitions(-DDATADIR="${CMAKE_INSTALL_FULL_DATADIR}")
add_definitions(-DLIBDIR="${LIB_INSTALL_DIR}")
add_definitions(-DPKGLIBDIR="${LIB_INSTALL_DIR}/webkit${WEBKITGTK_API_INFIX}gtk-${WEBKITGTK_API_VERSION}")

if (NOT DEVELOPER_MODE AND NOT CMAKE_SYSTEM_NAME MATCHES "Darwin")
    WEBKIT_ADD_TARGET_PROPERTIES(CyberKit LINK_FLAGS "-Wl,--version-script,${CMAKE_CURRENT_SOURCE_DIR}/webkitglib-symbols.map")
endif ()

set(CyberKit_USE_PREFIX_HEADER ON)

list(APPEND CyberKit_UNIFIED_SOURCE_LIST_FILES
    "SourcesGTK.txt"
)

list(APPEND CyberKit_MESSAGES_IN_FILES
    UIProcess/ViewGestureController

    WebProcess/gtk/GtkSettingsManagerProxy
    WebProcess/WebPage/ViewGestureGeometryCollector
)

list(APPEND CyberKit_DERIVED_SOURCES
    ${CyberKitGTK_DERIVED_SOURCES_DIR}/InspectorGResourceBundle.c
    ${CyberKitGTK_DERIVED_SOURCES_DIR}/CyberKitDirectoryInputStreamData.cpp
    ${CyberKitGTK_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.c

    ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitEnumTypes.cpp
    ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitWebProcessEnumTypes.cpp
)

if (ENABLE_WAYLAND_TARGET)
    list(APPEND CyberKit_DERIVED_SOURCES
        ${CyberKitGTK_DERIVED_SOURCES_DIR}/pointer-constraints-unstable-v1-protocol.c
        ${CyberKitGTK_DERIVED_SOURCES_DIR}/relative-pointer-unstable-v1-protocol.c
    )
endif ()

if (ENABLE_PDFJS)
    list(APPEND CyberKit_DERIVED_SOURCES
        ${CyberKitGTK_DERIVED_SOURCES_DIR}/PdfJSGResourceBundle.c
        ${CyberKitGTK_DERIVED_SOURCES_DIR}/PdfJSGResourceBundleExtras.c
    )

    WEBKIT_BUILD_PDFJS_GRESOURCES(${CyberKitGTK_DERIVED_SOURCES_DIR})
endif ()

if (ENABLE_MODERN_MEDIA_CONTROLS)
    list(APPEND CyberKit_DERIVED_SOURCES
        ${CyberKitGTK_DERIVED_SOURCES_DIR}/ModernMediaControlsGResourceBundle.c
    )

    WEBKIT_BUILD_MODERN_MEDIA_CONTROLS_GRESOURCES(${CyberKitGTK_DERIVED_SOURCES_DIR})
endif ()

set(CyberKit_DirectoryInputStream_DATA
    ${WEBKIT_DIR}/NetworkProcess/soup/Resources/directory.css
    ${WEBKIT_DIR}/NetworkProcess/soup/Resources/directory.js
)

add_custom_command(
    OUTPUT ${CyberKitGTK_DERIVED_SOURCES_DIR}/CyberKitDirectoryInputStreamData.cpp ${CyberKitGTK_DERIVED_SOURCES_DIR}/CyberKitDirectoryInputStreamData.h
    MAIN_DEPENDENCY ${WEBCORE_DIR}/css/make-css-file-arrays.pl
    DEPENDS ${CyberKit_DirectoryInputStream_DATA}
    COMMAND ${PERL_EXECUTABLE} ${WEBCORE_DIR}/css/make-css-file-arrays.pl --defines "${FEATURE_DEFINES_WITH_SPACE_SEPARATOR}" --preprocessor "${CODE_GENERATOR_PREPROCESSOR}" ${CyberKitGTK_DERIVED_SOURCES_DIR}/CyberKitDirectoryInputStreamData.h ${CyberKitGTK_DERIVED_SOURCES_DIR}/CyberKitDirectoryInputStreamData.cpp ${CyberKit_DirectoryInputStream_DATA}
    VERBATIM
)

if (USE_GTK4)
    set(GTK_API_VERSION 4)
    set(GTK_PKGCONFIG_PACKAGE gtk4)
else ()
    set(GTK_API_VERSION 3)
    set(GTK_PKGCONFIG_PACKAGE gtk+-3.0)
endif ()

set(CyberKitGTK_HEADER_TEMPLATES
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
    ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitFaviconDatabase.h.in
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
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitColorChooserRequest.h.in
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitPointerLockPermissionRequest.h.in
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitPrintOperation.h.in
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitWebInspector.h.in
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitWebViewBase.h.in
)

if (ENABLE_2022_GLIB_API)
    list(APPEND CyberKitGTK_HEADER_TEMPLATES
        ${WEBKIT_DIR}/UIProcess/API/glib/CyberKitNetworkSession.h.in
    )
endif ()

set(CyberKitGTK_INSTALLED_HEADERS
    ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitEnumTypes.h
    ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitVersion.h
)

set(CyberKitWebProcessExtension_INSTALLED_HEADERS
    ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitWebProcessEnumTypes.h
)

set(CyberKitWebProcessExtension_HEADER_TEMPLATES
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitFrame.h.in
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitScriptWorld.h.in
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebEditor.h.in
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebFormManager.h.in
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebHitTestResult.h.in
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebPage.h.in
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/webkit-web-process-extension.h.in
)

if (ENABLE_2022_GLIB_API)
    list(APPEND CyberKitWebProcessExtension_HEADER_TEMPLATES
        ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebProcessExtension.h.in
    )
    list(APPEND CyberKit_SOURCES
        ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebProcessExtension.cpp
    )
else ()
    list(APPEND CyberKitWebProcessExtension_HEADER_TEMPLATES
        ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebExtension.h.in
        ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebExtensionAutocleanups.h.in
    )
    list(APPEND CyberKit_SOURCES
        ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitWebExtension.cpp
    )
endif ()

set(CyberKitGTK_FAKE_API_HEADERS
    ${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/webkit
    ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit
    ${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/webkitgtk-web-process-extension/webkit
)

if (NOT ENABLE_2022_GLIB_API)
    include(PlatformGTKDeprecated.cmake)
endif ()

# This is necessary because of a conflict between the GTK+ API CyberKitVersion.h and one generated by CyberCore.
list(INSERT CyberKit_INCLUDE_DIRECTORIES 0
    "${CyberKitGTK_FRAMEWORK_HEADERS_DIR}"
    "${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/webkitgtk-${WEBKITGTK_API_VERSION}"
    "${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/webkitgtk-web-process-extension"
    "${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit"
    "${CyberKitGTK_DERIVED_SOURCES_DIR}"
)

list(APPEND CyberKit_PRIVATE_INCLUDE_DIRECTORIES
    "${CyberScriptCoreGLib_DERIVED_SOURCES_DIR}/jsc"
)

list(APPEND CyberKit_INCLUDE_DIRECTORIES
    "${WEBKIT_DIR}/NetworkProcess/glib"
    "${WEBKIT_DIR}/NetworkProcess/gtk"
    "${WEBKIT_DIR}/NetworkProcess/soup"
    "${WEBKIT_DIR}/Platform/IPC/glib"
    "${WEBKIT_DIR}/Platform/IPC/unix"
    "${WEBKIT_DIR}/Platform/classifier"
    "${WEBKIT_DIR}/Platform/generic"
    "${WEBKIT_DIR}/Shared/API/c/gtk"
    "${WEBKIT_DIR}/Shared/API/glib"
    "${WEBKIT_DIR}/Shared/CoordinatedGraphics"
    "${WEBKIT_DIR}/Shared/CoordinatedGraphics/threadedcompositor"
    "${WEBKIT_DIR}/Shared/glib"
    "${WEBKIT_DIR}/Shared/gtk"
    "${WEBKIT_DIR}/Shared/linux"
    "${WEBKIT_DIR}/Shared/soup"
    "${WEBKIT_DIR}/UIProcess/API/C/cairo"
    "${WEBKIT_DIR}/UIProcess/API/C/glib"
    "${WEBKIT_DIR}/UIProcess/API/C/gtk"
    "${WEBKIT_DIR}/UIProcess/API/glib"
    "${WEBKIT_DIR}/UIProcess/API/gtk"
    "${WEBKIT_DIR}/UIProcess/CoordinatedGraphics"
    "${WEBKIT_DIR}/UIProcess/Inspector/glib"
    "${WEBKIT_DIR}/UIProcess/Inspector/gtk"
    "${WEBKIT_DIR}/UIProcess/Notifications/glib/"
    "${WEBKIT_DIR}/UIProcess/geoclue"
    "${WEBKIT_DIR}/UIProcess/glib"
    "${WEBKIT_DIR}/UIProcess/gstreamer"
    "${WEBKIT_DIR}/UIProcess/gtk"
    "${WEBKIT_DIR}/UIProcess/linux"
    "${WEBKIT_DIR}/UIProcess/soup"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk"
    "${WEBKIT_DIR}/WebProcess/Inspector/gtk"
    "${WEBKIT_DIR}/WebProcess/glib"
    "${WEBKIT_DIR}/WebProcess/gtk"
    "${WEBKIT_DIR}/WebProcess/soup"
    "${WEBKIT_DIR}/WebProcess/CyberCoreSupport/gtk"
    "${WEBKIT_DIR}/WebProcess/CyberCoreSupport/soup"
    "${WEBKIT_DIR}/WebProcess/WebPage/CoordinatedGraphics"
    "${WEBKIT_DIR}/WebProcess/WebPage/gtk"
)

list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
    ${ENCHANT_INCLUDE_DIRS}
    ${GIO_UNIX_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${GSTREAMER_INCLUDE_DIRS}
    ${GSTREAMER_PBUTILS_INCLUDE_DIRS}
    ${GTK_INCLUDE_DIRS}
    ${LIBSOUP_INCLUDE_DIRS}
)

if (USE_WPE_RENDERER)
    list(APPEND CyberKit_INCLUDE_DIRECTORIES
        "${WEBKIT_DIR}/WebProcess/WebPage/libwpe"
    )
    list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
        ${WPEBACKEND_FDO_INCLUDE_DIRS}
    )
endif ()

set(CyberKitCommonIncludeDirectories ${CyberKit_INCLUDE_DIRECTORIES})
set(CyberKitCommonSystemIncludeDirectories ${CyberKit_SYSTEM_INCLUDE_DIRECTORIES})

list(APPEND WebProcess_SOURCES
    WebProcess/EntryPoint/unix/WebProcessMain.cpp
)

list(APPEND NetworkProcess_SOURCES
    NetworkProcess/EntryPoint/unix/NetworkProcessMain.cpp
)

list(APPEND GPUProcess_SOURCES
    GPUProcess/EntryPoint/unix/GPUProcessMain.cpp
)

if (USE_WPE_RENDERER)
    list(APPEND CyberKit_LIBRARIES
      WPE::libwpe
      ${WPEBACKEND_FDO_LIBRARIES}
    )
endif ()

if (GTK_UNIX_PRINT_FOUND)
    list(APPEND CyberKit_LIBRARIES GTK::UnixPrint)
endif ()

if (USE_LIBWEBRTC)
    list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
        "${THIRDPARTY_DIR}/libwebrtc/Source/"
        "${THIRDPARTY_DIR}/libwebrtc/Source/webrtc"
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

GENERATE_GLIB_API_HEADERS(CyberKit CyberKitGTK_HEADER_TEMPLATES
    ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit
    CyberKitGTK_INSTALLED_HEADERS
    "-DWTF_PLATFORM_GTK=1"
    "-DWTF_PLATFORM_WPE=0"
    "-DUSE_GTK4=$<BOOL:${USE_GTK4}>"
    "-DENABLE_2022_GLIB_API=$<BOOL:${ENABLE_2022_GLIB_API}>"
)

GENERATE_GLIB_API_HEADERS(CyberKit CyberKitWebProcessExtension_HEADER_TEMPLATES
    ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit
    CyberKitWebProcessExtension_INSTALLED_HEADERS
    "-DWTF_PLATFORM_GTK=1"
    "-DWTF_PLATFORM_WPE=0"
    "-DUSE_GTK4=$<BOOL:${USE_GTK4}>"
    "-DENABLE_2022_GLIB_API=$<BOOL:${ENABLE_2022_GLIB_API}>"
)

if (NOT USE_GTK4)
    list(REMOVE_ITEM CyberKitGTK_INSTALLED_HEADERS ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/webkit.h)
    list(REMOVE_ITEM CyberKitWebProcessExtension_INSTALLED_HEADERS ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/webkit-web-process-extension.h)
endif ()

if (USE_GTK4)
    set(CyberKitGTK_ENUM_HEADER_TEMPLATE ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitEnumTypesGtk4.h.in)
else ()
    set(CyberKitGTK_ENUM_HEADER_TEMPLATE ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitEnumTypesGtk3.h.in)
endif ()

# To generate CyberKitEnumTypes.h we want to use all installed headers, except CyberKitEnumTypes.h itself.
set(CyberKitGTK_ENUM_GENERATION_HEADERS ${CyberKitGTK_INSTALLED_HEADERS})
list(REMOVE_ITEM CyberKitGTK_ENUM_GENERATION_HEADERS ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitEnumTypes.h)
add_custom_command(
    OUTPUT ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitEnumTypes.h
           ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitEnumTypes.cpp
    DEPENDS ${CyberKitGTK_ENUM_GENERATION_HEADERS}

    COMMAND glib-mkenums --template ${CyberKitGTK_ENUM_HEADER_TEMPLATE} ${CyberKitGTK_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ | sed s/WEBKIT_TYPE_KIT/WEBKIT_TYPE/ > ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitEnumTypes.h

    COMMAND glib-mkenums --template ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitEnumTypes.cpp.in ${CyberKitGTK_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ > ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitEnumTypes.cpp
    VERBATIM
)

if (USE_GTK4)
    set(CyberKitGTK_WEB_PROCESS_ENUM_HEADER_TEMPLATE ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/CyberKitWebProcessEnumTypesGtk4.h.in)
else ()
    set(CyberKitGTK_WEB_PROCESS_ENUM_HEADER_TEMPLATE ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/CyberKitWebProcessEnumTypesGtk3.h.in)
endif ()

set(CyberKitGTK_WEB_PROCESS_ENUM_GENERATION_HEADERS ${CyberKitWebProcessExtension_INSTALLED_HEADERS})
list(REMOVE_ITEM CyberKitGTK_WEB_PROCESS_ENUM_GENERATION_HEADERS ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitWebProcessEnumTypes.h)
add_custom_command(
    OUTPUT ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitWebProcessEnumTypes.h
           ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitWebProcessEnumTypes.cpp
    DEPENDS ${CyberKitGTK_WEB_PROCESS_ENUM_GENERATION_HEADERS}

    COMMAND glib-mkenums --template ${CyberKitGTK_WEB_PROCESS_ENUM_HEADER_TEMPLATE} ${CyberKitGTK_WEB_PROCESS_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ | sed s/WEBKIT_TYPE_KIT/WEBKIT_TYPE/ > ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitWebProcessEnumTypes.h

    COMMAND glib-mkenums --template ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/CyberKitWebProcessEnumTypes.cpp.in ${CyberKitGTK_WEB_PROCESS_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ > ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitWebProcessEnumTypes.cpp
    VERBATIM
)

WEBKIT_BUILD_INSPECTOR_GRESOURCES(${CyberKitGTK_DERIVED_SOURCES_DIR})

set(CyberKitResources "")
list(APPEND CyberKitResources "<file alias=\"css/gtk-theme.css\">gtk-theme.css</file>\n")
list(APPEND CyberKitResources "<file alias=\"images/missingImage\">missingImage.png</file>\n")
list(APPEND CyberKitResources "<file alias=\"images/missingImage@2x\">missingImage@2x.png</file>\n")
list(APPEND CyberKitResources "<file alias=\"images/missingImage@3x\">missingImage@3x.png</file>\n")
list(APPEND CyberKitResources "<file alias=\"images/panIcon\">panIcon.png</file>\n")
list(APPEND CyberKitResources "<file alias=\"images/textAreaResizeCorner\">textAreaResizeCorner.png</file>\n")
list(APPEND CyberKitResources "<file alias=\"images/textAreaResizeCorner@2x\">textAreaResizeCorner@2x.png</file>\n")

if (ENABLE_WEB_AUDIO)
    list(APPEND CyberKitResources
        "        <file alias=\"audio/Composite\">Composite.wav</file>\n"
    )
endif ()

file(WRITE ${CyberKitGTK_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.xml
    "<?xml version=1.0 encoding=UTF-8?>\n"
    "<gresources>\n"
    "    <gresource prefix=\"/org/webkitgtk/resources\">\n"
    ${CyberKitResources}
    "    </gresource>\n"
    "</gresources>\n"
)

add_custom_command(
    OUTPUT ${CyberKitGTK_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.c ${CyberKitGTK_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.deps
    DEPENDS ${CyberKitGTK_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.xml
    DEPFILE ${CyberKitGTK_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.deps
    COMMAND glib-compile-resources --generate --sourcedir=${CMAKE_SOURCE_DIR}/Source/CyberCore/Resources --sourcedir=${CMAKE_SOURCE_DIR}/Source/CyberCore/platform/audio/resources --sourcedir=${CMAKE_SOURCE_DIR}/Source/CyberKit/Resources/gtk --target=${CyberKitGTK_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.c --dependency-file=${CyberKitGTK_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.deps ${CyberKitGTK_DERIVED_SOURCES_DIR}/CyberKitResourcesGResourceBundle.xml
    VERBATIM
)

if (ENABLE_WAYLAND_TARGET)
    add_custom_command(
        OUTPUT ${CyberKitGTK_DERIVED_SOURCES_DIR}/pointer-constraints-unstable-v1-protocol.c
        DEPENDS ${WAYLAND_PROTOCOLS_DATADIR}/unstable/pointer-constraints/pointer-constraints-unstable-v1.xml
        COMMAND ${WAYLAND_SCANNER} private-code ${WAYLAND_PROTOCOLS_DATADIR}/unstable/pointer-constraints/pointer-constraints-unstable-v1.xml ${CyberKitGTK_DERIVED_SOURCES_DIR}/pointer-constraints-unstable-v1-protocol.c
        COMMAND ${WAYLAND_SCANNER} client-header ${WAYLAND_PROTOCOLS_DATADIR}/unstable/pointer-constraints/pointer-constraints-unstable-v1.xml ${CyberKitGTK_DERIVED_SOURCES_DIR}/pointer-constraints-unstable-v1-client-protocol.h
        VERBATIM
    )

    add_custom_command(
        OUTPUT ${CyberKitGTK_DERIVED_SOURCES_DIR}/relative-pointer-unstable-v1-protocol.c
        DEPENDS ${WAYLAND_PROTOCOLS_DATADIR}/unstable/relative-pointer/relative-pointer-unstable-v1.xml
        COMMAND ${WAYLAND_SCANNER} private-code ${WAYLAND_PROTOCOLS_DATADIR}/unstable/relative-pointer/relative-pointer-unstable-v1.xml ${CyberKitGTK_DERIVED_SOURCES_DIR}/relative-pointer-unstable-v1-protocol.c
        COMMAND ${WAYLAND_SCANNER} client-header ${WAYLAND_PROTOCOLS_DATADIR}/unstable/relative-pointer/relative-pointer-unstable-v1.xml ${CyberKitGTK_DERIVED_SOURCES_DIR}/relative-pointer-unstable-v1-client-protocol.h
        VERBATIM
    )
endif ()

# Commands for building the built-in injected bundle.
add_library(webkit${WEBKITGTK_API_INFIX}gtkinjectedbundle MODULE "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitInjectedBundleMain.cpp")
ADD_WEBKIT_PREFIX_HEADER(webkit${WEBKITGTK_API_INFIX}gtkinjectedbundle)
target_link_libraries(webkit${WEBKITGTK_API_INFIX}gtkinjectedbundle CyberKit)

target_include_directories(webkit${WEBKITGTK_API_INFIX}gtkinjectedbundle PRIVATE
    $<TARGET_PROPERTY:CyberKit,INCLUDE_DIRECTORIES>
    "${DERIVED_SOURCES_DIR}/InjectedBundle"
    "${CyberKitGTK_FRAMEWORK_HEADERS_DIR}"
    "${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/webkit${WEBKITGTK_API_INFIX}gtk-${WEBKITGTK_API_VERSION}"
    "${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit"
)

target_include_directories(webkit${WEBKITGTK_API_INFIX}gtkinjectedbundle SYSTEM PRIVATE
    ${CyberKit_SYSTEM_INCLUDE_DIRECTORIES}
)

if (COMPILER_IS_GCC_OR_CLANG)
    WEBKIT_ADD_TARGET_CXX_FLAGS(webkit${WEBKITGTK_API_INFIX}gtkinjectedbundle -Wno-unused-parameter)
endif ()

install(TARGETS webkit${WEBKITGTK_API_INFIX}gtkinjectedbundle
        DESTINATION "${LIB_INSTALL_DIR}/webkit${WEBKITGTK_API_INFIX}gtk-${WEBKITGTK_API_VERSION}/injected-bundle"
)
if (ENABLE_2022_GLIB_API)
    install(FILES "${CMAKE_BINARY_DIR}/Source/CyberKit/webkitgtk-${WEBKITGTK_API_VERSION}.pc"
                  "${CMAKE_BINARY_DIR}/Source/CyberKit/webkitgtk-web-process-extension-${WEBKITGTK_API_VERSION}.pc"
            DESTINATION "${LIB_INSTALL_DIR}/pkgconfig"
    )
else ()
    install(FILES "${CMAKE_BINARY_DIR}/Source/CyberKit/webkit2gtk-${WEBKITGTK_API_VERSION}.pc"
                  "${CMAKE_BINARY_DIR}/Source/CyberKit/webkit2gtk-web-extension-${WEBKITGTK_API_VERSION}.pc"
            DESTINATION "${LIB_INSTALL_DIR}/pkgconfig"
    )
endif ()
install(FILES ${CyberKitGTK_INSTALLED_HEADERS}
              ${CyberKitWebProcessExtension_INSTALLED_HEADERS}
        DESTINATION "${WEBKITGTK_HEADER_INSTALL_DIR}/webkit"
)

add_custom_target(CyberKit-forwarding-headers
    COMMAND ${PERL_EXECUTABLE} ${WEBKIT_DIR}/Scripts/generate-forwarding-headers.pl --include-path ${WEBKIT_DIR} --output ${FORWARDING_HEADERS_DIR} --platform gtk --platform soup
)

# These symbolic link allows includes like #include <webkit/WebkitWebView.h>, which simulates installed headers.
add_custom_command(
    OUTPUT ${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/webkit
    DEPENDS ${WEBKIT_DIR}/UIProcess/API/gtk
    COMMAND ln -n -s -f ${WEBKIT_DIR}/UIProcess/API/gtk ${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/webkit
)
add_custom_command(
    OUTPUT ${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/webkitgtk-web-process-extension/webkit
    DEPENDS ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk
    COMMAND ln -n -s -f ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk ${CyberKitGTK_FRAMEWORK_HEADERS_DIR}/webkitgtk-web-process-extension/webkit
)

add_custom_target(CyberKit-fake-api-headers
    DEPENDS ${CyberKitGTK_FAKE_API_HEADERS}
)

list(APPEND CyberKit_DEPENDENCIES
     CyberKit-fake-api-headers
     CyberKit-forwarding-headers
)

set(WEBKITGTK_SOURCES_FOR_INTROSPECTION
    UIProcess/API/gtk/CyberKitColorChooserRequest.cpp
    UIProcess/API/gtk/CyberKitInputMethodContextGtk.cpp
    UIProcess/API/gtk/CyberKitPrintCustomWidget.cpp
    UIProcess/API/gtk/CyberKitPrintOperation.cpp
    UIProcess/API/gtk/CyberKitWebInspector.cpp
    UIProcess/API/gtk/CyberKitWebViewGtk.cpp
)

if (USE_GTK4)
    list(APPEND WEBKITGTK_SOURCES_FOR_INTROSPECTION UIProcess/API/gtk/CyberKitWebViewGtk4.cpp)
else ()
    list(APPEND WEBKITGTK_SOURCES_FOR_INTROSPECTION UIProcess/API/gtk/CyberKitWebViewGtk3.cpp)
endif ()

GI_INTROSPECT(CyberKit${WEBKITGTK_API_INFIX} ${WEBKITGTK_API_VERSION} webkit${WEBKITGTK_API_INFIX}/webkit${WEBKITGTK_API_INFIX}.h
    TARGET CyberKit
    PACKAGE webkit${WEBKITGTK_API_INFIX}gtk
    IDENTIFIER_PREFIX CyberKit
    SYMBOL_PREFIX webkit
    DEPENDENCIES
        CyberScriptCore
        Gtk-${GTK_API_VERSION}.0:${GTK_PKGCONFIG_PACKAGE}
        Soup-${SOUP_API_VERSION}:libsoup-${SOUP_API_VERSION}
    SOURCES
        ${CyberKitGTK_INSTALLED_HEADERS}
        ${WEBKITGTK_SOURCES_FOR_INTROSPECTION}
        Shared/API/glib
        UIProcess/API/glib
    NO_IMPLICIT_SOURCES
)

GI_DOCGEN(CyberKit${WEBKITGTK_API_INFIX} gtk/gtk${GTK_API_VERSION}-webkitgtk.toml.in
    CONTENT_TEMPLATES gtk/gtk${GTK_API_VERSION}-urlmap.js
)

if (ENABLE_2022_GLIB_API)
    set(WEBKITGTK_WEB_PROCESS_EXTENSION_API_NAME "CyberKitWebProcessExtension")
    set(WEBKITGTK_WEB_PROCESS_EXTENSION_PACKAGE_NAME "webkitgtk-web-process-extension")
    set(WEBKITGTK_WEB_PROCESS_EXTENSION_HEADER_NAME "webkit-web-process-extension.h")
else ()
    set(WEBKITGTK_WEB_PROCESS_EXTENSION_API_NAME "CyberKit2WebExtension")
    set(WEBKITGTK_WEB_PROCESS_EXTENSION_PACKAGE_NAME "webkit2gtk-web-extension")
    set(WEBKITGTK_WEB_PROCESS_EXTENSION_HEADER_NAME "webkit-web-extension.h")
endif ()

GI_INTROSPECT(${WEBKITGTK_WEB_PROCESS_EXTENSION_API_NAME} ${WEBKITGTK_API_VERSION} webkit${WEBKITGTK_API_INFIX}/${WEBKITGTK_WEB_PROCESS_EXTENSION_HEADER_NAME}
    TARGET CyberKit
    PACKAGE ${WEBKITGTK_WEB_PROCESS_EXTENSION_PACKAGE_NAME}
    IDENTIFIER_PREFIX CyberKit
    SYMBOL_PREFIX webkit
    DEPENDENCIES
        CyberScriptCore
        Gtk-${GTK_API_VERSION}.0:${GTK_PKGCONFIG_PACKAGE}
        Soup-${SOUP_API_VERSION}:libsoup-${SOUP_API_VERSION}
    SOURCES
        ${CyberKitDOM_SOURCES_FOR_INTROSPECTION}
        ${CyberKitWebProcessExtension_INSTALLED_HEADERS}
        ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitContextMenu.h
        ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitContextMenuActions.h
        ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitContextMenuItem.h
        ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitHitTestResult.h
        ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitUserMessage.h
        ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitURIRequest.h
        ${CyberKitGTK_DERIVED_SOURCES_DIR}/webkit/CyberKitURIResponse.h
        Shared/API/glib/CyberKitContextMenu.cpp
        Shared/API/glib/CyberKitContextMenuItem.cpp
        Shared/API/glib/CyberKitHitTestResult.cpp
        Shared/API/glib/CyberKitUserMessage.cpp
        Shared/API/glib/CyberKitURIRequest.cpp
        Shared/API/glib/CyberKitURIResponse.cpp
        WebProcess/InjectedBundle/API/glib
    NO_IMPLICIT_SOURCES
)

GI_DOCGEN(${WEBKITGTK_WEB_PROCESS_EXTENSION_API_NAME} gtk/webkitgtk-web-process-extension.toml.in
    CONTENT_TEMPLATES gtk/gtk${GTK_API_VERSION}-urlmap.js
)
