include(CyberKitCommon)

set_property(DIRECTORY . PROPERTY FOLDER "CyberKit")

if (PAL_LIBRARY_TYPE MATCHES STATIC)
    add_definitions(-DSTATICALLY_LINKED_WITH_PAL=1)
endif ()

set(CyberKit_PRIVATE_INCLUDE_DIRECTORIES
    "${CMAKE_BINARY_DIR}"
)

set(CyberKit_INCLUDE_DIRECTORIES
    "${CyberCore_PRIVATE_FRAMEWORK_HEADERS_DIR}"
    "${CyberKit_FRAMEWORK_HEADERS_DIR}"
    "${CyberKit_DERIVED_SOURCES_DIR}"
    "${CyberKit_DERIVED_SOURCES_DIR}/include"
    "${WEBKIT_DIR}"
    "${WEBKIT_DIR}/GPUProcess"
    "${WEBKIT_DIR}/GPUProcess/graphics"
    "${WEBKIT_DIR}/GPUProcess/media"
    "${WEBKIT_DIR}/GPUProcess/webrtc"
    "${WEBKIT_DIR}/NetworkProcess"
    "${WEBKIT_DIR}/NetworkProcess/Classifier"
    "${WEBKIT_DIR}/NetworkProcess/Cookies"
    "${WEBKIT_DIR}/NetworkProcess/CustomProtocols"
    "${WEBKIT_DIR}/NetworkProcess/Downloads"
    "${WEBKIT_DIR}/NetworkProcess/FileAPI"
    "${WEBKIT_DIR}/NetworkProcess/IndexedDB"
    "${WEBKIT_DIR}/NetworkProcess/ServiceWorker"
    "${WEBKIT_DIR}/NetworkProcess/WebStorage"
    "${WEBKIT_DIR}/NetworkProcess/cache"
    "${WEBKIT_DIR}/NetworkProcess/watchos"
    "${WEBKIT_DIR}/NetworkProcess/webrtc"
    "${WEBKIT_DIR}/Platform"
    "${WEBKIT_DIR}/Platform/IPC"
    "${WEBKIT_DIR}/PluginProcess"
    "${WEBKIT_DIR}/Shared"
    "${WEBKIT_DIR}/Shared/API"
    "${WEBKIT_DIR}/Shared/API/c"
    "${WEBKIT_DIR}/Shared/ApplePay"
    "${WEBKIT_DIR}/Shared/Authentication"
    "${WEBKIT_DIR}/Shared/CoreIPCSupport"
    "${WEBKIT_DIR}/Shared/Databases"
    "${WEBKIT_DIR}/Shared/Databases/IndexedDB"
    "${WEBKIT_DIR}/Shared/FileAPI"
    "${WEBKIT_DIR}/Shared/Gamepad"
    "${WEBKIT_DIR}/Shared/Plugins"
    "${WEBKIT_DIR}/Shared/Plugins/Netscape"
    "${WEBKIT_DIR}/Shared/Plugins/Netscape/x11"
    "${WEBKIT_DIR}/Shared/RemoteLayerTree"
    "${WEBKIT_DIR}/Shared/WebsiteData"
    "${WEBKIT_DIR}/UIProcess"
    "${WEBKIT_DIR}/UIProcess/API"
    "${WEBKIT_DIR}/UIProcess/API/C"
    "${WEBKIT_DIR}/UIProcess/API/cpp"
    "${WEBKIT_DIR}/UIProcess/Authentication"
    "${WEBKIT_DIR}/UIProcess/Automation"
    "${WEBKIT_DIR}/UIProcess/Downloads"
    "${WEBKIT_DIR}/UIProcess/Gamepad"
    "${WEBKIT_DIR}/UIProcess/GPU"
    "${WEBKIT_DIR}/UIProcess/Inspector"
    "${WEBKIT_DIR}/UIProcess/Inspector/Agents"
    "${WEBKIT_DIR}/UIProcess/Launcher"
    "${WEBKIT_DIR}/UIProcess/Network"
    "${WEBKIT_DIR}/UIProcess/Network/CustomProtocols"
    "${WEBKIT_DIR}/UIProcess/Notifications"
    "${WEBKIT_DIR}/UIProcess/Plugins"
    "${WEBKIT_DIR}/UIProcess/RemoteLayerTree"
    "${WEBKIT_DIR}/UIProcess/UserContent"
    "${WEBKIT_DIR}/UIProcess/WebAuthentication"
    "${WEBKIT_DIR}/UIProcess/WebAuthentication/Mock"
    "${WEBKIT_DIR}/UIProcess/WebsiteData"
    "${WEBKIT_DIR}/WebProcess"
    "${WEBKIT_DIR}/WebProcess/ApplePay"
    "${WEBKIT_DIR}/WebProcess/ApplicationCache"
    "${WEBKIT_DIR}/WebProcess/Automation"
    "${WEBKIT_DIR}/WebProcess/Cache"
    "${WEBKIT_DIR}/WebProcess/Databases"
    "${WEBKIT_DIR}/WebProcess/Databases/IndexedDB"
    "${WEBKIT_DIR}/WebProcess/FileAPI"
    "${WEBKIT_DIR}/WebProcess/FullScreen"
    "${WEBKIT_DIR}/WebProcess/Gamepad"
    "${WEBKIT_DIR}/WebProcess/Geolocation"
    "${WEBKIT_DIR}/WebProcess/GPU"
    "${WEBKIT_DIR}/WebProcess/GPU/graphics"
    "${WEBKIT_DIR}/WebProcess/GPU/media"
    "${WEBKIT_DIR}/WebProcess/GPU/webrtc"
    "${WEBKIT_DIR}/WebProcess/IconDatabase"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/c"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/DOM"
    "${WEBKIT_DIR}/WebProcess/Inspector"
    "${WEBKIT_DIR}/WebProcess/Launching"
    "${WEBKIT_DIR}/WebProcess/MediaCache"
    "${WEBKIT_DIR}/WebProcess/MediaStream"
    "${WEBKIT_DIR}/WebProcess/Network"
    "${WEBKIT_DIR}/WebProcess/Network/webrtc"
    "${WEBKIT_DIR}/WebProcess/Notifications"
    "${WEBKIT_DIR}/WebProcess/OriginData"
    "${WEBKIT_DIR}/WebProcess/Plugins"
    "${WEBKIT_DIR}/WebProcess/Plugins/Netscape"
    "${WEBKIT_DIR}/WebProcess/ResourceCache"
    "${WEBKIT_DIR}/WebProcess/Storage"
    "${WEBKIT_DIR}/WebProcess/UserContent"
    "${WEBKIT_DIR}/WebProcess/CyberCoreSupport"
    "${WEBKIT_DIR}/WebProcess/WebPage"
    "${WEBKIT_DIR}/WebProcess/WebPage/RemoteLayerTree"
    "${WEBKIT_DIR}/WebProcess/WebStorage"
    "${CMAKE_SOURCE_DIR}/Source"
    "${CMAKE_SOURCE_DIR}/CyberKitLibraries"
)

set(PROTOCOL_GENERATOR_SCRIPTS_DIR "${JAVASCRIPTCORE_DIR}/inspector/scripts")

list(APPEND CyberKit_UNIFIED_SOURCE_LIST_FILES
    "Sources.txt"
)

set(CyberKit_MESSAGES_IN_FILES
    GPUProcess/GPUConnectionToWebProcess
    GPUProcess/GPUProcess

    GPUProcess/graphics/RemoteRenderingBackendProxy

    GPUProcess/media/RemoteAudioDestinationManager
    GPUProcess/media/RemoteAudioSessionProxy
    GPUProcess/media/RemoteCDMFactoryProxy
    GPUProcess/media/RemoteCDMInstanceProxy
    GPUProcess/media/RemoteCDMInstanceSessionProxy
    GPUProcess/media/RemoteCDMProxy
    GPUProcess/media/RemoteLegacyCDMFactoryProxy
    GPUProcess/media/RemoteLegacyCDMProxy
    GPUProcess/media/RemoteLegacyCDMSessionProxy
    GPUProcess/media/RemoteMediaPlayerManagerProxy
    GPUProcess/media/RemoteMediaPlayerProxy
    GPUProcess/media/RemoteMediaResourceManager

    GPUProcess/webrtc/LibWebRTCCodecsProxy
    GPUProcess/webrtc/RemoteAudioMediaStreamTrackRenderer
    GPUProcess/webrtc/RemoteAudioMediaStreamTrackRendererManager
    GPUProcess/webrtc/RemoteMediaRecorder
    GPUProcess/webrtc/RemoteMediaRecorderManager
    GPUProcess/webrtc/RemoteSampleBufferDisplayLayer
    GPUProcess/webrtc/RemoteSampleBufferDisplayLayerManager

    NetworkProcess/Cookies/WebCookieManager

    NetworkProcess/IndexedDB/WebIDBServer

    NetworkProcess/NetworkConnectionToWebProcess
    NetworkProcess/NetworkContentRuleListManager
    NetworkProcess/NetworkProcess
    NetworkProcess/NetworkResourceLoader
    NetworkProcess/NetworkSocketChannel
    NetworkProcess/NetworkSocketStream

    NetworkProcess/ServiceWorker/ServiceWorkerFetchTask
    NetworkProcess/ServiceWorker/WebSWServerConnection
    NetworkProcess/ServiceWorker/WebSWServerToContextConnection

    NetworkProcess/WebStorage/StorageManagerSet

    NetworkProcess/cache/CacheStorageEngineConnection

    NetworkProcess/webrtc/NetworkMDNSRegister
    NetworkProcess/webrtc/NetworkRTCMonitor
    NetworkProcess/webrtc/NetworkRTCProvider

    PluginProcess/PluginControllerProxy
    PluginProcess/PluginProcess
    PluginProcess/WebProcessConnection

    Shared/AuxiliaryProcess
    Shared/WebConnection

    Shared/Authentication/AuthenticationManager

    Shared/Plugins/NPObjectMessageReceiver

    UIProcess/DrawingAreaProxy
    UIProcess/VisitedLinkStore
    UIProcess/WebCookieManagerProxy
    UIProcess/WebFullScreenManagerProxy
    UIProcess/WebGeolocationManagerProxy
    UIProcess/WebPageProxy
    UIProcess/WebPasteboardProxy
    UIProcess/WebProcessPool
    UIProcess/WebProcessProxy

    UIProcess/Automation/WebAutomationSession

    UIProcess/Downloads/DownloadProxy

    UIProcess/GPU/GPUProcessProxy

    UIProcess/Inspector/RemoteWebInspectorProxy
    UIProcess/Inspector/WebInspectorProxy

    UIProcess/Network/NetworkProcessProxy

    UIProcess/Plugins/PluginProcessProxy

    UIProcess/UserContent/WebUserContentControllerProxy

    WebProcess/WebProcess

    WebProcess/Automation/WebAutomationSessionProxy

    WebProcess/Databases/IndexedDB/WebIDBConnectionToServer

    WebProcess/FullScreen/WebFullScreenManager

    WebProcess/GPU/GPUProcessConnection

    WebProcess/GPU/graphics/RemoteRenderingBackend

    WebProcess/GPU/media/MediaPlayerPrivateRemote
    WebProcess/GPU/media/RemoteAudioDestinationProxy
    WebProcess/GPU/media/RemoteAudioSession
    WebProcess/GPU/media/RemoteCDMInstanceSession
    WebProcess/GPU/media/RemoteLegacyCDMSession
    WebProcess/GPU/media/RemoteMediaPlayerManager

    WebProcess/GPU/webrtc/LibWebRTCCodecs
    WebProcess/GPU/webrtc/SampleBufferDisplayLayer

    WebProcess/Geolocation/WebGeolocationManager

    WebProcess/Inspector/RemoteWebInspectorUI
    WebProcess/Inspector/WebInspector
    WebProcess/Inspector/WebInspectorInterruptDispatcher
    WebProcess/Inspector/WebInspectorUI

    WebProcess/Network/webrtc/LibWebRTCNetwork
    WebProcess/Network/webrtc/WebMDNSRegister
    WebProcess/Network/webrtc/WebRTCMonitor
    WebProcess/Network/webrtc/WebRTCResolver

    WebProcess/Network/NetworkProcessConnection
    WebProcess/Network/WebResourceLoader
    WebProcess/Network/WebSocketChannel
    WebProcess/Network/WebSocketStream

    WebProcess/Notifications/WebNotificationManager

    WebProcess/Plugins/PluginProcessConnection
    WebProcess/Plugins/PluginProcessConnectionManager
    WebProcess/Plugins/PluginProxy

    WebProcess/Storage/WebSWClientConnection
    WebProcess/Storage/WebSWContextManagerConnection

    WebProcess/UserContent/WebUserContentController

    WebProcess/WebPage/DrawingArea
    WebProcess/WebPage/EventDispatcher
    WebProcess/WebPage/VisitedLinkTableController
    WebProcess/WebPage/WebPage

    WebProcess/WebStorage/StorageAreaMap
)

# FIXME: https://bugs.webkit.org/show_bug.cgi?id=210366
set(CyberKit_LIBRARIES CyberKit::CyberCore)

set(WebProcess_LIBRARIES
    CyberKit
)

set(NetworkProcess_LIBRARIES
    CyberKit
)

set(GPUProcess_LIBRARIES
    CyberKit
)

set(PluginProcess_LIBRARIES
    CyberKit
)

if (ENABLE_ENCRYPTED_MEDIA AND ENABLE_OPENCDM)
  list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
      ${OPENCDM_INCLUDE_DIRS}
  )
endif ()

if (ENABLE_GRAPHICS_CONTEXT_GL)
    # For platforms that want to use system-provided OpenGL (ES) / EGL headers,
    # these include directories, libraries or definitions need to be
    # added before the ANGLE directories.
    if (USE_LIBEPOXY)
        list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
            ${LIBEPOXY_INCLUDE_DIRS}
        )
        list(APPEND CyberKit_PRIVATE_LIBRARIES
            ${LIBEPOXY_LIBRARIES}
        )
        add_definitions(${LIBEPOXY_DEFINITIONS})
    else ()
        if (USE_OPENGL)
            list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
                ${OPENGL_INCLUDE_DIRS}
            )
            list(APPEND CyberKit_PRIVATE_LIBRARIES
                ${OPENGL_LIBRARIES}
            )
            add_definitions(${OPENGL_DEFINITIONS})
        elseif (USE_OPENGL_ES)
            list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
                ${OPENGLES2_INCLUDE_DIRS}
            )
            list(APPEND CyberKit_PRIVATE_LIBRARIES
                ${OPENGLES2_LIBRARIES}
            )
            add_definitions(${OPENGLES2_DEFINITIONS})
        endif ()

        if (USE_EGL)
            list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
                ${EGL_INCLUDE_DIRS}
            )
            list(APPEND CyberKit_PRIVATE_LIBRARIES
                ${EGL_LIBRARIES}
            )
            add_definitions(${EGL_DEFINITIONS})
        endif ()
    endif ()
endif ()

if (USE_LIBWEBRTC)
    list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES "${THIRDPARTY_DIR}/libwebrtc/Source/"
        "${THIRDPARTY_DIR}/libwebrtc/Source/webrtc"
        "${THIRDPARTY_DIR}/libwebrtc/Source/third_party/abseil-cpp")
    list(APPEND CyberKit_LIBRARIES webrtc)
endif ()

if (ATOMIC_INT64_REQUIRES_LIBATOMIC)
    list(APPEND CyberKit_PRIVATE_LIBRARIES atomic)
endif ()

if (UNIX)
    check_function_exists(shm_open SHM_OPEN_EXISTS)
    if (NOT SHM_OPEN_EXISTS)
        set(CMAKE_REQUIRED_LIBRARIES rt)
        check_function_exists(shm_open SHM_OPEN_REQUIRES_LIBRT)
        if (SHM_OPEN_REQUIRES_LIBRT)
            list(APPEND CyberKit_PRIVATE_LIBRARIES rt)
        endif ()
        unset(CMAKE_REQUIRED_LIBRARIES)
    endif ()
endif ()

macro(ADD_WEBKIT_PREFIX_HEADER _target)
    if (CyberKit_USE_PREFIX_HEADER)
        get_target_property(OLD_COMPILE_FLAGS ${_target} COMPILE_FLAGS)
        if (${OLD_COMPILE_FLAGS} STREQUAL "OLD_COMPILE_FLAGS-NOTFOUND")
            set(OLD_COMPILE_FLAGS "")
        endif ()
        set_target_properties(${_target} PROPERTIES COMPILE_FLAGS "${OLD_COMPILE_FLAGS} -include ${WEBKIT_DIR}/CyberKit2Prefix.h")
    endif ()
endmacro()

set(CyberKit_AUTOMATION_PROTOCOL_GENERATOR_EXTRA_FLAGS)

set(CyberKit_INTERFACE_LIBRARIES CyberKit)
set(CyberKit_INTERFACE_INCLUDE_DIRECTORIES
    ${CyberKit_FRAMEWORK_HEADERS_DIR}
    ${CyberKit_PRIVATE_FRAMEWORK_HEADERS_DIR}
)
set(CyberKit_INTERFACE_DEPENDENCIES
    NetworkProcess
    WebProcess
)

WEBKIT_EXECUTABLE_DECLARE(WebProcess)
WEBKIT_EXECUTABLE_DECLARE(NetworkProcess)

if (ENABLE_GPU_PROCESS)
    WEBKIT_EXECUTABLE_DECLARE(GPUProcess)
    list(APPEND CyberKit_INTERFACE_DEPENDENCIES GPUProcess)
endif ()

if (ENABLE_PLUGIN_PROCESS)
    WEBKIT_EXECUTABLE_DECLARE(PluginProcess)
    list(APPEND CyberKit_INTERFACE_DEPENDENCIES PluginProcess)
endif ()

WEBKIT_FRAMEWORK_DECLARE(CyberKit)
WEBKIT_INCLUDE_CONFIG_FILES_IF_EXISTS()

if (CyberKit_PUBLIC_FRAMEWORK_HEADERS)
    WEBKIT_COPY_FILES(CyberKit_CopyHeaders
        DESTINATION ${CyberKit_FRAMEWORK_HEADERS_DIR}/CyberKit
        FILES ${CyberKit_PUBLIC_FRAMEWORK_HEADERS}
        FLATTENED
    )
    list(APPEND CyberKit_DEPENDENCIES CyberKit_CopyHeaders)
endif ()

if (CyberKit_PRIVATE_FRAMEWORK_HEADERS)
    WEBKIT_COPY_FILES(CyberKit_CopyPrivateHeaders
        DESTINATION ${CyberKit_PRIVATE_FRAMEWORK_HEADERS_DIR}/CyberKit
        FILES ${CyberKit_PRIVATE_FRAMEWORK_HEADERS}
        FLATTENED
    )
    list(APPEND CyberKit_INTERFACE_DEPENDENCIES CyberKit_CopyPrivateHeaders)
endif ()

# Helper macro which wraps the generate-message-receiver.py script
#   _output_source is a list name which will contain generated sources.(eg. CyberKit_SOURCES)
#   _inputs are messages.in files to generate.
macro(GENERATE_MESSAGE_SOURCES _output_source _inputs)
    unset(_input_files)
    unset(_outputs)
    foreach (_file IN ITEMS ${_inputs})
        get_filename_component(_name ${_file} NAME_WE)
        list(APPEND _input_files ${WEBKIT_DIR}/${_file}.messages.in)
        list(APPEND _outputs
            ${CyberKit_DERIVED_SOURCES_DIR}/${_name}MessageReceiver.cpp
            ${CyberKit_DERIVED_SOURCES_DIR}/${_name}Messages.h
            ${CyberKit_DERIVED_SOURCES_DIR}/${_name}MessagesReplies.h
        )
        list(APPEND ${_output_source} ${CyberKit_DERIVED_SOURCES_DIR}/${_name}MessageReceiver.cpp)
    endforeach ()
    list(APPEND ${_output_source} ${CyberKit_DERIVED_SOURCES_DIR}/MessageNames.cpp)

    add_custom_command(
        OUTPUT
            ${CyberKit_DERIVED_SOURCES_DIR}/MessageNames.cpp
            ${CyberKit_DERIVED_SOURCES_DIR}/MessageNames.h
            ${_outputs}
        MAIN_DEPENDENCY ${WEBKIT_DIR}/Scripts/generate-message-receiver.py
        DEPENDS
            ${WEBKIT_DIR}/Scripts/webkit/__init__.py
            ${WEBKIT_DIR}/Scripts/webkit/messages.py
            ${WEBKIT_DIR}/Scripts/webkit/model.py
            ${WEBKIT_DIR}/Scripts/webkit/parser.py
            ${_input_files}
        COMMAND ${PYTHON_EXECUTABLE} ${WEBKIT_DIR}/Scripts/generate-message-receiver.py ${WEBKIT_DIR} ${_inputs}
        WORKING_DIRECTORY ${CyberKit_DERIVED_SOURCES_DIR}
        VERBATIM
    )
endmacro()
GENERATE_MESSAGE_SOURCES(CyberKit_DERIVED_SOURCES "${CyberKit_MESSAGES_IN_FILES}")

set(CyberKit_AUTOMATION_PROTOCOL_GENERATOR_SCRIPTS
    ${PROTOCOL_GENERATOR_SCRIPTS_DIR}/generate-inspector-protocol-bindings.py
    ${PROTOCOL_GENERATOR_SCRIPTS_DIR}/codegen/cpp_generator.py
    ${PROTOCOL_GENERATOR_SCRIPTS_DIR}/codegen/cpp_generator_templates.py
    ${PROTOCOL_GENERATOR_SCRIPTS_DIR}/codegen/generate_cpp_backend_dispatcher_header.py
    ${PROTOCOL_GENERATOR_SCRIPTS_DIR}/codegen/generate_cpp_backend_dispatcher_implementation.py
    ${PROTOCOL_GENERATOR_SCRIPTS_DIR}/codegen/generate_cpp_frontend_dispatcher_header.py
    ${PROTOCOL_GENERATOR_SCRIPTS_DIR}/codegen/generate_cpp_frontend_dispatcher_implementation.py
    ${PROTOCOL_GENERATOR_SCRIPTS_DIR}/codegen/generate_cpp_protocol_types_header.py
    ${PROTOCOL_GENERATOR_SCRIPTS_DIR}/codegen/generate_cpp_protocol_types_implementation.py
    ${PROTOCOL_GENERATOR_SCRIPTS_DIR}/codegen/generator.py
    ${PROTOCOL_GENERATOR_SCRIPTS_DIR}/codegen/generator_templates.py
    ${PROTOCOL_GENERATOR_SCRIPTS_DIR}/codegen/models.py
)

set(CyberKit_AUTOMATION_PROTOCOL_GENERATOR_INPUTS
    ${WEBKIT_DIR}/UIProcess/Automation/Automation.json
)

add_custom_command(
    OUTPUT ${CyberKit_DERIVED_SOURCES_DIR}/AutomationBackendDispatchers.h ${CyberKit_DERIVED_SOURCES_DIR}/AutomationBackendDispatchers.cpp ${CyberKit_DERIVED_SOURCES_DIR}/AutomationProtocolObjects.h ${CyberKit_DERIVED_SOURCES_DIR}/AutomationProtocolObjects.cpp ${CyberKit_DERIVED_SOURCES_DIR}/AutomationFrontendDispatchers.h ${CyberKit_DERIVED_SOURCES_DIR}/AutomationFrontendDispatchers.cpp
    MAIN_DEPENDENCY ${CyberKit_AUTOMATION_PROTOCOL_GENERATOR_INPUTS}
    DEPENDS ${CyberKit_AUTOMATION_PROTOCOL_GENERATOR_SCRIPTS}
    COMMAND ${PYTHON_EXECUTABLE} ${PROTOCOL_GENERATOR_SCRIPTS_DIR}/generate-inspector-protocol-bindings.py --outputDir "${CyberKit_DERIVED_SOURCES_DIR}" --framework CyberKit ${CyberKit_AUTOMATION_PROTOCOL_GENERATOR_EXTRA_FLAGS} --backend ${CyberKit_AUTOMATION_PROTOCOL_GENERATOR_INPUTS}
    VERBATIM)

list(APPEND CyberKit_HEADERS
    ${CyberKit_DERIVED_SOURCES_DIR}/AutomationBackendDispatchers.h
    ${CyberKit_DERIVED_SOURCES_DIR}/AutomationFrontendDispatchers.h
    ${CyberKit_DERIVED_SOURCES_DIR}/AutomationProtocolObjects.h
)

list(APPEND CyberKit_SOURCES
    ${CyberKit_DERIVED_SOURCES_DIR}/AutomationBackendDispatchers.cpp
    ${CyberKit_DERIVED_SOURCES_DIR}/AutomationFrontendDispatchers.cpp
    ${CyberKit_DERIVED_SOURCES_DIR}/AutomationProtocolObjects.cpp
)

add_custom_command(
    OUTPUT ${CyberKit_DERIVED_SOURCES_DIR}/WebAutomationSessionProxyScriptSource.h ${CyberKit_DERIVED_SOURCES_DIR}/WebAutomationSessionProxy.min.js
    MAIN_DEPENDENCY ${WEBKIT_DIR}/WebProcess/Automation/WebAutomationSessionProxy.js
    DEPENDS ${CyberScriptCore_SCRIPTS_DIR}/xxd.pl ${CyberScriptCore_SCRIPTS_DIR}/jsmin.py
    COMMAND ${CMAKE_COMMAND} -E echo "//# sourceURL=__InjectedScript_WebAutomationSessionProxy.js" > ${CyberKit_DERIVED_SOURCES_DIR}/WebAutomationSessionProxy.min.js
    COMMAND ${PYTHON_EXECUTABLE} ${CyberScriptCore_SCRIPTS_DIR}/jsmin.py < ${WEBKIT_DIR}/WebProcess/Automation/WebAutomationSessionProxy.js >> ${CyberKit_DERIVED_SOURCES_DIR}/WebAutomationSessionProxy.min.js
    COMMAND ${PERL_EXECUTABLE} ${CyberScriptCore_SCRIPTS_DIR}/xxd.pl WebAutomationSessionProxyScriptSource ${CyberKit_DERIVED_SOURCES_DIR}/WebAutomationSessionProxy.min.js ${CyberKit_DERIVED_SOURCES_DIR}/WebAutomationSessionProxyScriptSource.h
    VERBATIM)

list(APPEND CyberKit_HEADERS
    ${CyberKit_DERIVED_SOURCES_DIR}/WebAutomationSessionProxyScriptSource.h
)

set(CyberKit_WEB_PREFERENCES_TEMPLATES
    ${WEBKIT_DIR}/Scripts/PreferencesTemplates/WebPageUpdatePreferences.cpp.erb
    ${WEBKIT_DIR}/Scripts/PreferencesTemplates/WebPreferencesDefinitions.h.erb
    ${WEBKIT_DIR}/Scripts/PreferencesTemplates/WebPreferencesExperimentalFeatures.cpp.erb
    ${WEBKIT_DIR}/Scripts/PreferencesTemplates/WebPreferencesInternalDebugFeatures.cpp.erb
    ${WEBKIT_DIR}/Scripts/PreferencesTemplates/WebPreferencesKeys.cpp.erb
    ${WEBKIT_DIR}/Scripts/PreferencesTemplates/WebPreferencesKeys.h.erb
    ${WEBKIT_DIR}/Scripts/PreferencesTemplates/WebPreferencesStoreDefaultsMap.cpp.erb
)

add_custom_command(
    OUTPUT ${CyberKit_DERIVED_SOURCES_DIR}/WebPageUpdatePreferences.cpp ${CyberKit_DERIVED_SOURCES_DIR}/WebPreferencesDefinitions.h ${CyberKit_DERIVED_SOURCES_DIR}/WebPreferencesExperimentalFeatures.cpp ${CyberKit_DERIVED_SOURCES_DIR}/WebPreferencesInternalDebugFeatures.cpp ${CyberKit_DERIVED_SOURCES_DIR}/WebPreferencesKeys.cpp ${CyberKit_DERIVED_SOURCES_DIR}/WebPreferencesKeys.h ${CyberKit_DERIVED_SOURCES_DIR}/WebPreferencesStoreDefaultsMap.cpp
    MAIN_DEPENDENCY ${WEBKIT_DIR}/Shared/WebPreferences.yaml
    DEPENDS ${CyberKit_WEB_PREFERENCES_TEMPLATES}
    COMMAND ${RUBY_EXECUTABLE} ${WEBKIT_DIR}/Scripts/GeneratePreferences.rb --input ${WEBKIT_DIR}/Shared/WebPreferences.yaml --outputDir "${CyberKit_DERIVED_SOURCES_DIR}"
    VERBATIM)

list(APPEND CyberKit_HEADERS
    ${CyberKit_DERIVED_SOURCES_DIR}/WebPreferencesDefinitions.h
    ${CyberKit_DERIVED_SOURCES_DIR}/WebPreferencesKeys.h
)

list(APPEND CyberKit_SOURCES
    ${CyberKit_DERIVED_SOURCES_DIR}/WebPageUpdatePreferences.cpp
    ${CyberKit_DERIVED_SOURCES_DIR}/WebPreferencesExperimentalFeatures.cpp
    ${CyberKit_DERIVED_SOURCES_DIR}/WebPreferencesInternalDebugFeatures.cpp
    ${CyberKit_DERIVED_SOURCES_DIR}/WebPreferencesKeys.cpp
    ${CyberKit_DERIVED_SOURCES_DIR}/WebPreferencesStoreDefaultsMap.cpp
)

list(APPEND CyberKit_SOURCES ${CyberKit_DERIVED_SOURCES})

WEBKIT_COMPUTE_SOURCES(CyberKit)
WEBKIT_FRAMEWORK(CyberKit)
WEBKIT_FRAMEWORK_TARGET(CyberKit)
ADD_WEBKIT_PREFIX_HEADER(CyberKit)

list(APPEND WebProcess_PRIVATE_INCLUDE_DIRECTORIES ${CyberKit_PRIVATE_INCLUDE_DIRECTORIES})
WEBKIT_EXECUTABLE(WebProcess)
ADD_WEBKIT_PREFIX_HEADER(WebProcess)

list(APPEND NetworkProcess_PRIVATE_INCLUDE_DIRECTORIES ${CyberKit_PRIVATE_INCLUDE_DIRECTORIES})
WEBKIT_EXECUTABLE(NetworkProcess)
ADD_WEBKIT_PREFIX_HEADER(NetworkProcess)

if (ENABLE_GPU_PROCESS)
    list(APPEND GPUProcess_PRIVATE_INCLUDE_DIRECTORIES ${CyberKit_PRIVATE_INCLUDE_DIRECTORIES})
    WEBKIT_EXECUTABLE(GPUProcess)
    ADD_WEBKIT_PREFIX_HEADER(GPUProcess)
endif ()

if (ENABLE_PLUGIN_PROCESS)
    list(APPEND PluginProcess_PRIVATE_INCLUDE_DIRECTORIES ${CyberKit_PRIVATE_INCLUDE_DIRECTORIES})
    WEBKIT_EXECUTABLE(PluginProcess)
    ADD_WEBKIT_PREFIX_HEADER(PluginProcess)
endif ()

if (COMPILER_IS_GCC_OR_CLANG)
    WEBKIT_ADD_TARGET_CXX_FLAGS(CyberKit -Wno-unused-parameter)
    WEBKIT_ADD_TARGET_CXX_FLAGS(WebProcess -Wno-unused-parameter)
    WEBKIT_ADD_TARGET_CXX_FLAGS(NetworkProcess -Wno-unused-parameter)
    if (ENABLE_GPU_PROCESS)
        WEBKIT_ADD_TARGET_CXX_FLAGS(GPUProcess -Wno-unused-parameter)
    endif ()
    if (ENABLE_PLUGIN_PROCESS)
        WEBKIT_ADD_TARGET_CXX_FLAGS(PluginProcess -Wno-unused-parameter)
    endif ()
endif ()

if ("${PORT}" STREQUAL "Mac")
    WEBKIT_DEFINE_XPC_SERVICES()
else ()
    WEBKIT_POPULATE_LIBRARY_VERSION(WEBKIT)

    set_target_properties(CyberKit PROPERTIES VERSION ${WEBKIT_VERSION} SOVERSION ${WEBKIT_VERSION_MAJOR})

    install(TARGETS CyberKit WebProcess NetworkProcess
        LIBRARY DESTINATION "${LIB_INSTALL_DIR}"
        RUNTIME DESTINATION "${LIBEXEC_INSTALL_DIR}"
    )
endif ()
