if (${WTF_PLATFORM_WIN_CAIRO})
    add_definitions(-DUSE_CAIRO=1 -DUSE_CURL=1 -DWEBKIT_EXPORTS=1)
    list(APPEND CyberKitLegacy_PRIVATE_INCLUDE_DIRECTORIES
        ${CAIRO_INCLUDE_DIRS}
        "${WEBKIT_LIBRARIES_DIR}/include"
    )
    list(APPEND CyberKitLegacy_SOURCES_Classes
        win/WebDownloadCURL.cpp
        win/WebURLAuthenticationChallengeSenderCURL.cpp
    )
    list(APPEND CyberKitLegacy_LIBRARIES
        ${OPENSSL_LIBRARIES}
        PRIVATE mfuuid.lib
        PRIVATE strmiids.lib
    )
else ()
    list(APPEND CyberKitLegacy_SOURCES_Classes
        win/WebDownloadCFNet.cpp
        win/WebURLAuthenticationChallengeSenderCFNet.cpp
    )
    list(APPEND CyberKitLegacy_LIBRARIES
        PRIVATE CFNetwork${DEBUG_SUFFIX}
        PRIVATE CoreGraphics${DEBUG_SUFFIX}
        PRIVATE CoreText${DEBUG_SUFFIX}
        PRIVATE QuartzCore${DEBUG_SUFFIX}
        PRIVATE libdispatch${DEBUG_SUFFIX}
        PRIVATE ${LIBXML2_LIBRARIES}
        PRIVATE ${LIBXSLT_LIBRARIES}
        PRIVATE ${SQLITE_LIBRARIES}
        PRIVATE ${ZLIB_LIBRARIES}
    )
endif ()

add_custom_command(
    OUTPUT ${CyberKitLegacy_DERIVED_SOURCES_DIR}/CyberKitVersion.h
    MAIN_DEPENDENCY ${WEBKITLEGACY_DIR}/scripts/generate-webkitversion.pl
    DEPENDS ${WEBKITLEGACY_DIR}/mac/Configurations/Version.xcconfig
    COMMAND ${PERL_EXECUTABLE} ${WEBKITLEGACY_DIR}/scripts/generate-webkitversion.pl --config ${WEBKITLEGACY_DIR}/mac/Configurations/Version.xcconfig --outputDir ${CyberKitLegacy_DERIVED_SOURCES_DIR}
    VERBATIM)
list(APPEND CyberKitLegacy_SOURCES ${CyberKitLegacy_DERIVED_SOURCES_DIR}/CyberKitVersion.h)

list(APPEND CyberKitLegacy_PRIVATE_INCLUDE_DIRECTORIES
    "${CMAKE_BINARY_DIR}/../include/private"
    "${CMAKE_BINARY_DIR}/../include/private/CyberScriptCore"
    "${CMAKE_BINARY_DIR}/../include/private/CyberCore"
    "${WEBKITLEGACY_DIR}/win"
    "${WEBKITLEGACY_DIR}/win/plugins"
    "${WEBKITLEGACY_DIR}/win/CyberCoreSupport"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/include"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces"
)

list(APPEND CyberKitLegacy_INCLUDES
    win/CFDictionaryPropertyBag.h
    win/COMEnumVariant.h
    win/COMPropertyBag.h
    win/COMVariantSetter.h
    win/CodeAnalysisConfig.h
    win/DOMCSSClasses.h
    win/DOMCoreClasses.h
    win/DOMEventsClasses.h
    win/DOMHTMLClasses.h
    win/DefaultDownloadDelegate.h
    win/DefaultPolicyDelegate.h
    win/ForEachCoClass.h
    win/FullscreenVideoController.h
    win/MarshallingHelpers.h
    win/MemoryStream.h
    win/ProgIDMacros.h
    win/WebActionPropertyBag.h
    win/WebApplicationCache.h
    win/WebArchive.h
    win/WebBackForwardList.h
    win/WebCache.h
    win/WebCachedFramePlatformData.h
    win/CyberCoreStatistics.h
    win/WebDataSource.h
    win/WebDatabaseManager.h
    win/WebDocumentLoader.h
    win/WebDownload.h
    win/WebDropSource.h
    win/WebElementPropertyBag.h
    win/WebError.h
    win/WebFrame.h
    win/WebFramePolicyListener.h
    win/WebGeolocationPolicyListener.h
    win/WebGeolocationPosition.h
    win/WebHTMLRepresentation.h
    win/WebHistory.h
    win/WebHistoryItem.h
    win/WebJavaScriptCollector.h
    win/CyberKitCOMAPI.h
    win/CyberKitClassFactory.h
    win/CyberKitDLL.h
    win/CyberKitGraphics.h
    win/CyberKitLogging.h
    win/CyberKitStatistics.h
    win/CyberKitStatisticsPrivate.h
    win/CyberKitSystemBits.h
    win/WebLocalizableStrings.h
    win/WebMutableURLRequest.h
    win/WebNavigationData.h
    win/WebNotification.h
    win/WebNotificationCenter.h
    win/WebPreferenceKeysPrivate.h
    win/WebPreferences.h
    win/WebResource.h
    win/WebScriptObject.h
    win/WebScriptWorld.h
    win/WebSecurityOrigin.h
    win/WebSerializedJSValue.h
    win/WebTextRenderer.h
    win/WebURLAuthenticationChallenge.h
    win/WebURLAuthenticationChallengeSender.h
    win/WebURLCredential.h
    win/WebURLProtectionSpace.h
    win/WebURLResponse.h
    win/WebUserContentURLPattern.h
    win/WebView.h
    win/WebWorkersPrivate.h
)

list(APPEND CyberKitLegacy_SOURCES_Classes
    win/AccessibleBase.cpp
    win/AccessibleDocument.cpp
    win/AccessibleImage.cpp
    win/AccessibleTextImpl.cpp
    win/BackForwardList.cpp
    win/CFDictionaryPropertyBag.cpp
    win/DOMCSSClasses.cpp
    win/DOMCoreClasses.cpp
    win/DOMEventsClasses.cpp
    win/DOMHTMLClasses.cpp
    win/DefaultDownloadDelegate.cpp
    win/DefaultPolicyDelegate.cpp
    win/ForEachCoClass.cpp
    win/FullscreenVideoController.cpp
    win/MarshallingHelpers.cpp
    win/MemoryStream.cpp
    win/WebActionPropertyBag.cpp
    win/WebApplicationCache.cpp
    win/WebArchive.cpp
    win/WebBackForwardList.cpp
    win/WebCache.cpp
    win/CyberCoreStatistics.cpp
    win/WebDataSource.cpp
    win/WebDatabaseManager.cpp
    win/WebDocumentLoader.cpp
    win/WebDownload.cpp
    win/WebDropSource.cpp
    win/WebElementPropertyBag.cpp
    win/WebError.cpp
    win/WebFrame.cpp
    win/WebFramePolicyListener.cpp
    win/WebGeolocationPolicyListener.cpp
    win/WebGeolocationPosition.cpp
    win/WebHTMLRepresentation.cpp
    win/WebHistory.cpp
    win/WebHistoryItem.cpp
    win/WebInspector.cpp
    win/WebJavaScriptCollector.cpp
    win/CyberKitCOMAPI.cpp
    win/CyberKitClassFactory.cpp
    win/CyberKitDLL.cpp
    win/CyberKitLogging.cpp
    win/CyberKitMessageLoop.cpp
    win/CyberKitStatistics.cpp
    win/CyberKitSystemBits.cpp
    win/WebLocalizableStrings.cpp
    win/WebMutableURLRequest.cpp
    win/WebNavigationData.cpp
    win/WebNodeHighlight.cpp
    win/WebNotification.cpp
    win/WebNotificationCenter.cpp
    win/WebPreferences.cpp
    win/WebResource.cpp
    win/WebScriptObject.cpp
    win/WebScriptWorld.cpp
    win/WebSecurityOrigin.cpp
    win/WebSerializedJSValue.cpp
    win/WebTextRenderer.cpp
    win/WebURLAuthenticationChallenge.cpp
    win/WebURLAuthenticationChallengeSender.cpp
    win/WebURLCredential.cpp
    win/WebURLProtectionSpace.cpp
    win/WebURLResponse.cpp
    win/WebUserContentURLPattern.cpp
    win/WebView.cpp
    win/WebWorkersPrivate.cpp

    win/plugins/PluginDatabase.cpp
    win/plugins/PluginDatabaseWin.cpp
    win/plugins/PluginDebug.cpp
    win/plugins/PluginMainThreadScheduler.cpp
    win/plugins/PluginMessageThrottlerWin.cpp
    win/plugins/PluginPackage.cpp
    win/plugins/PluginPackageWin.cpp
    win/plugins/PluginStream.cpp
    win/plugins/PluginView.cpp
    win/plugins/PluginViewWin.cpp
    win/plugins/npapi.cpp

    win/storage/WebDatabaseProvider.cpp
)

list(APPEND CyberKitLegacy_SOURCES_CyberCoreSupport
    win/CyberCoreSupport/AcceleratedCompositingContext.cpp
    win/CyberCoreSupport/EmbeddedWidget.cpp
    win/CyberCoreSupport/EmbeddedWidget.h
    win/CyberCoreSupport/WebChromeClient.cpp
    win/CyberCoreSupport/WebChromeClient.h
    win/CyberCoreSupport/WebContextMenuClient.cpp
    win/CyberCoreSupport/WebContextMenuClient.h
    win/CyberCoreSupport/WebDesktopNotificationsDelegate.cpp
    win/CyberCoreSupport/WebDesktopNotificationsDelegate.h
    win/CyberCoreSupport/WebDragClient.cpp
    win/CyberCoreSupport/WebDragClient.h
    win/CyberCoreSupport/WebEditorClient.cpp
    win/CyberCoreSupport/WebEditorClient.h
    win/CyberCoreSupport/WebFrameLoaderClient.cpp
    win/CyberCoreSupport/WebFrameLoaderClient.h
    win/CyberCoreSupport/WebFrameNetworkingContext.cpp
    win/CyberCoreSupport/WebFrameNetworkingContext.h
    win/CyberCoreSupport/WebGeolocationClient.cpp
    win/CyberCoreSupport/WebGeolocationClient.h
    win/CyberCoreSupport/WebInspectorClient.cpp
    win/CyberCoreSupport/WebInspectorClient.h
    win/CyberCoreSupport/WebInspectorDelegate.cpp
    win/CyberCoreSupport/WebInspectorDelegate.h
    win/CyberCoreSupport/WebPlatformStrategies.cpp
    win/CyberCoreSupport/WebPlatformStrategies.h
    win/CyberCoreSupport/WebPluginInfoProvider.cpp
    win/CyberCoreSupport/WebPluginInfoProvider.h
    win/CyberCoreSupport/WebProgressTrackerClient.cpp
    win/CyberCoreSupport/WebProgressTrackerClient.h
    win/CyberCoreSupport/WebVisitedLinkStore.cpp
    win/CyberCoreSupport/WebVisitedLinkStore.h
)

if (USE_CF)
    list(APPEND CyberKitLegacy_SOURCES_Classes
        cf/CyberCoreSupport/WebInspectorClientCF.cpp
    )

    list(APPEND CyberKitLegacy_LIBRARIES
        ${COREFOUNDATION_LIBRARY}
    )
endif ()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    enable_language(ASM_MASM)
    if (MSVC)
        set(MASM_EXECUTABLE ml64)
        set(MASM_FLAGS /c /Fo)
        add_custom_command(
            OUTPUT ${CyberKitLegacy_DERIVED_SOURCES_DIR}/PaintHooks.obj
            MAIN_DEPENDENCY win/plugins/PaintHooks.asm
            COMMAND ${MASM_EXECUTABLE} ${MASM_FLAGS}
                ${CyberKitLegacy_DERIVED_SOURCES_DIR}/PaintHooks.obj
                ${CMAKE_CURRENT_SOURCE_DIR}/win/plugins/PaintHooks.asm
            VERBATIM)
        list(APPEND CyberKitLegacy_SOURCES
            ${CyberKitLegacy_DERIVED_SOURCES_DIR}/PaintHooks.obj
        )
    else ()
        list(APPEND CyberKitLegacy_SOURCES
            win/plugins/PaintHooks.asm
        )
    endif ()
endif ()

list(APPEND CyberKitLegacy_SOURCES ${CyberKitLegacy_INCLUDES} ${CyberKitLegacy_SOURCES_Classes} ${CyberKitLegacy_SOURCES_CyberCoreSupport})

source_group(Includes FILES ${CyberKitLegacy_INCLUDES})
source_group(Classes FILES ${CyberKitLegacy_SOURCES_Classes})
source_group(CyberCoreSupport FILES ${CyberKitLegacy_SOURCES_CyberCoreSupport})

# Build the COM interface:
macro(GENERATE_INTERFACE _infile _defines _depends)
    get_filename_component(_filewe ${_infile} NAME_WE)
    add_custom_command(
        OUTPUT  ${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/${_filewe}.h
        MAIN_DEPENDENCY ${_infile}
        DEPENDS ${_depends}
        COMMAND midl.exe /I "${CMAKE_CURRENT_SOURCE_DIR}/win/Interfaces" /I "${CMAKE_CURRENT_SOURCE_DIR}/win/Interfaces/Accessible2" /I "${CyberKitLegacy_DERIVED_SOURCES_DIR}/include" /I "${CMAKE_CURRENT_SOURCE_DIR}/win" /WX /char signed /env win32 /tlb "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_filewe}.tlb" /out "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces" /h "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/${_filewe}.h" /iid "${_filewe}_i.c" ${_defines} "${CMAKE_CURRENT_SOURCE_DIR}/${_infile}"
        USES_TERMINAL VERBATIM)
    set_source_files_properties(${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/${_filewe}.h PROPERTIES GENERATED TRUE)
    set_source_files_properties(${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/${_filewe}_i.c PROPERTIES GENERATED TRUE)
endmacro()

set(MIDL_DEFINES /D\ \"__PRODUCTION__=01\")

set(WEBKITLEGACY_IDL_DEPENDENCIES
    win/Interfaces/AccessibleComparable.idl
    win/Interfaces/DOMCSS.idl
    win/Interfaces/DOMCore.idl
    win/Interfaces/DOMEvents.idl
    win/Interfaces/DOMExtensions.idl
    win/Interfaces/DOMHTML.idl
    win/Interfaces/DOMPrivate.idl
    win/Interfaces/DOMRange.idl
    win/Interfaces/DOMWindow.idl
    win/Interfaces/IGEN_DOMObject.idl
    win/Interfaces/IWebArchive.idl
    win/Interfaces/IWebBackForwardList.idl
    win/Interfaces/IWebBackForwardListPrivate.idl
    win/Interfaces/IWebCache.idl
    win/Interfaces/ICyberCoreStatistics.idl
    win/Interfaces/IWebDataSource.idl
    win/Interfaces/IWebDatabaseManager.idl
    win/Interfaces/IWebDesktopNotificationsDelegate.idl
    win/Interfaces/IWebDocument.idl
    win/Interfaces/IWebDownload.idl
    win/Interfaces/IWebEditingDelegate.idl
    win/Interfaces/IWebEmbeddedView.idl
    win/Interfaces/IWebError.idl
    win/Interfaces/IWebErrorPrivate.idl
    win/Interfaces/IWebFormDelegate.idl
    win/Interfaces/IWebFrame.idl
    win/Interfaces/IWebFrameLoadDelegate.idl
    win/Interfaces/IWebFrameLoadDelegatePrivate.idl
    win/Interfaces/IWebFrameLoadDelegatePrivate2.idl
    win/Interfaces/IWebFramePrivate.idl
    win/Interfaces/IWebFrameView.idl
    win/Interfaces/IWebGeolocationPolicyListener.idl
    win/Interfaces/IWebGeolocationPosition.idl
    win/Interfaces/IWebGeolocationProvider.idl
    win/Interfaces/IWebHTMLRepresentation.idl
    win/Interfaces/IWebHTTPURLResponse.idl
    win/Interfaces/IWebHistory.idl
    win/Interfaces/IWebHistoryDelegate.idl
    win/Interfaces/IWebHistoryItem.idl
    win/Interfaces/IWebHistoryItemPrivate.idl
    win/Interfaces/IWebHistoryPrivate.idl
    win/Interfaces/IWebInspector.idl
    win/Interfaces/IWebInspectorPrivate.idl
    win/Interfaces/IWebJavaScriptCollector.idl
    win/Interfaces/ICyberKitStatistics.idl
    win/Interfaces/IWebMutableURLRequest.idl
    win/Interfaces/IWebMutableURLRequestPrivate.idl
    win/Interfaces/IWebNavigationData.idl
    win/Interfaces/IWebNotification.idl
    win/Interfaces/IWebNotificationCenter.idl
    win/Interfaces/IWebNotificationObserver.idl
    win/Interfaces/IWebPolicyDelegate.idl
    win/Interfaces/IWebPolicyDelegatePrivate.idl
    win/Interfaces/IWebPreferences.idl
    win/Interfaces/IWebPreferencesPrivate.idl
    win/Interfaces/IWebResource.idl
    win/Interfaces/IWebResourceLoadDelegate.idl
    win/Interfaces/IWebResourceLoadDelegatePrivate.idl
    win/Interfaces/IWebResourceLoadDelegatePrivate2.idl
    win/Interfaces/IWebScriptObject.idl
    win/Interfaces/IWebScriptWorld.idl
    win/Interfaces/IWebSecurityOrigin.idl
    win/Interfaces/IWebSerializedJSValue.idl
    win/Interfaces/IWebSerializedJSValuePrivate.idl
    win/Interfaces/IWebTextRenderer.idl
    win/Interfaces/IWebUIDelegate.idl
    win/Interfaces/IWebUIDelegate2.idl
    win/Interfaces/IWebUIDelegatePrivate.idl
    win/Interfaces/IWebURLAuthenticationChallenge.idl
    win/Interfaces/IWebURLRequest.idl
    win/Interfaces/IWebURLResponse.idl
    win/Interfaces/IWebURLResponsePrivate.idl
    win/Interfaces/IWebUndoManager.idl
    win/Interfaces/IWebUndoTarget.idl
    win/Interfaces/IWebUserContentURLPattern.idl
    win/Interfaces/IWebView.idl
    win/Interfaces/IWebViewPrivate.idl
    win/Interfaces/IWebWorkersPrivate.idl
    win/Interfaces/CyberScriptCoreAPITypes.idl
    win/Interfaces/CyberKit.idl
    win/Interfaces/WebScrollbarTypes.idl

    win/Interfaces/Accessible2/Accessible2.idl
    win/Interfaces/Accessible2/Accessible2_2.idl
    win/Interfaces/Accessible2/AccessibleApplication.idl
    win/Interfaces/Accessible2/AccessibleEditableText.idl
    win/Interfaces/Accessible2/AccessibleRelation.idl
    win/Interfaces/Accessible2/AccessibleStates.idl
    win/Interfaces/Accessible2/AccessibleText.idl
    win/Interfaces/Accessible2/AccessibleText2.idl
    win/Interfaces/Accessible2/IA2CommonTypes.idl
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/include/autoversion.h"
)

add_custom_command(
    OUTPUT ${CyberKitLegacy_DERIVED_SOURCES_DIR}/include/autoversion.h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${PERL_EXECUTABLE} ${WEBKIT_LIBRARIES_DIR}/tools/scripts/auto-version.pl ${CyberKitLegacy_DERIVED_SOURCES_DIR}
    VERBATIM)

GENERATE_INTERFACE(win/Interfaces/CyberKit.idl ${MIDL_DEFINES} "${WEBKITLEGACY_IDL_DEPENDENCIES}")
GENERATE_INTERFACE(win/Interfaces/Accessible2/AccessibleApplication.idl ${MIDL_DEFINES} "${WEBKITLEGACY_IDL_DEPENDENCIES}")
GENERATE_INTERFACE(win/Interfaces/Accessible2/Accessible2.idl ${MIDL_DEFINES} "${WEBKITLEGACY_IDL_DEPENDENCIES}")
GENERATE_INTERFACE(win/Interfaces/Accessible2/Accessible2_2.idl ${MIDL_DEFINES} "${WEBKITLEGACY_IDL_DEPENDENCIES}")
GENERATE_INTERFACE(win/Interfaces/Accessible2/AccessibleRelation.idl ${MIDL_DEFINES} "${WEBKITLEGACY_IDL_DEPENDENCIES}")
GENERATE_INTERFACE(win/Interfaces/Accessible2/AccessibleStates.idl ${MIDL_DEFINES} "${WEBKITLEGACY_IDL_DEPENDENCIES}")
GENERATE_INTERFACE(win/Interfaces/Accessible2/IA2CommonTypes.idl ${MIDL_DEFINES} "${WEBKITLEGACY_IDL_DEPENDENCIES}")
GENERATE_INTERFACE(win/Interfaces/Accessible2/AccessibleEditableText.idl ${MIDL_DEFINES} "${WEBKITLEGACY_IDL_DEPENDENCIES}")
GENERATE_INTERFACE(win/Interfaces/Accessible2/AccessibleText.idl ${MIDL_DEFINES} "${WEBKITLEGACY_IDL_DEPENDENCIES}")
GENERATE_INTERFACE(win/Interfaces/Accessible2/AccessibleText2.idl ${MIDL_DEFINES} "${WEBKITLEGACY_IDL_DEPENDENCIES}")

add_library(CyberKitLegacyGUID STATIC
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/CyberKit.h"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleApplication.h"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/Accessible2.h"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/Accessible2_2.h"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleRelation.h"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleStates.h"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/IA2CommonTypes.h"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleEditableText.h"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleText.h"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleText2.h"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/CyberKit_i.c"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleApplication_i.c"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/Accessible2_i.c"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/Accessible2_2_i.c"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleRelation_i.c"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleEditableText_i.c"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleText_i.c"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleText2_i.c"
)
set_target_properties(CyberKitLegacyGUID PROPERTIES OUTPUT_NAME CyberKitGUID${DEBUG_SUFFIX})

list(APPEND CyberKitLegacy_LIBRARIES
    PRIVATE Comctl32
    PRIVATE Comsupp
    PRIVATE Crypt32
    PRIVATE D2d1
    PRIVATE Dwrite
    PRIVATE dxguid
    PRIVATE Iphlpapi
    PRIVATE Psapi
    PRIVATE Rpcrt4
    PRIVATE Shlwapi
    PRIVATE Usp10
    PRIVATE Version
    PRIVATE Winmm
    PRIVATE CyberKitGUID${DEBUG_SUFFIX}
    PRIVATE WindowsCodecs
)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

# We need the webkit libraries to come before the system default libraries to prevent symbol conflicts with uuid.lib.
# To do this we add system default libs as webkit libs and zero out system default libs.
string(REPLACE " " "\;" CXX_LIBS ${CMAKE_CXX_STANDARD_LIBRARIES})
list(APPEND CyberKitLegacy_LIBRARIES ${CXX_LIBS})
set(CMAKE_CXX_STANDARD_LIBRARIES "")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${MSVC_RUNTIME_LINKER_FLAGS}")

# If this directory isn't created before midl runs and attempts to output CyberKit.tlb,
# It fails with an unusual error - midl failed - failed to save all changes
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(MAKE_DIRECTORY ${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces)

set(CyberKitLegacy_PUBLIC_FRAMEWORK_HEADERS
    ${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/Accessible2.h
    ${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/Accessible2_2.h
    ${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleApplication.h
    ${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleEditableText.h
    ${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleRelation.h
    ${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleStates.h
    ${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleText.h
    ${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/AccessibleText2.h
    ${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/IA2CommonTypes.h
    ${CyberKitLegacy_DERIVED_SOURCES_DIR}/Interfaces/CyberKit.h

    win/AccessibleBase.h
    win/AccessibleDocument.h
    win/CFDictionaryPropertyBag.h
    win/WebDataSource.h
    win/WebFrame.h
    win/CyberKitCOMAPI.h
)

WEBKIT_MAKE_FORWARDING_HEADERS(CyberKitLegacyGUID
    TARGET_NAME CyberKitLegacyFrameworkHeaders
    DESTINATION ${CyberKitLegacy_FRAMEWORK_HEADERS_DIR}/CyberKitLegacy
    FILES ${CyberKitLegacy_PUBLIC_FRAMEWORK_HEADERS}
    FLATTENED
)
if (NOT INTERNAL_BUILD)
    add_dependencies(CyberKitLegacyFrameworkHeaders CyberCorePrivateFrameworkHeaders)
endif ()

set(CyberKitLegacy_OUTPUT_NAME
    CyberKit${DEBUG_SUFFIX}
)
