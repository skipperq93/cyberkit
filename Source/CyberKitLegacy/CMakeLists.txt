set_property(DIRECTORY . PROPERTY FOLDER "CyberKitLegacy")

set(CyberKitLegacy_SOURCES
    Storage/InProcessIDBServer.cpp
    Storage/StorageAreaImpl.cpp
    Storage/StorageAreaSync.cpp
    Storage/StorageNamespaceImpl.cpp
    Storage/StorageSyncManager.cpp
    Storage/StorageThread.cpp
    Storage/StorageTracker.cpp
    Storage/WebDatabaseProvider.cpp
    Storage/WebStorageNamespaceProvider.cpp

    CyberCoreSupport/NetworkStorageSessionMap.cpp
    CyberCoreSupport/WebBroadcastChannelRegistry.cpp
    CyberCoreSupport/WebResourceLoadScheduler.cpp
    CyberCoreSupport/WebViewGroup.cpp
)

set(CyberKitLegacy_PRIVATE_INCLUDE_DIRECTORIES
    "${CMAKE_BINARY_DIR}"
    "${WEBKITLEGACY_DIR}/Storage"
    "${WEBKITLEGACY_DIR}/CyberCoreSupport"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}"
)

set(CyberKitLegacy_FRAMEWORKS
    JavaScriptCore
    PAL
    WTF
    CyberCore
)
if (NOT USE_SYSTEM_MALLOC)
    list(APPEND CyberKit_FRAMEWORKS bmalloc)
endif ()

set(CyberKitLegacy_INTERFACE_LIBRARIES CyberKitLegacy)
set(CyberKitLegacy_INTERFACE_INCLUDE_DIRECTORIES ${CyberKitLegacy_FRAMEWORK_HEADERS_DIR})
set(CyberKitLegacy_INTERFACE_DEPENDENCIES CyberKitLegacy_CopyHeaders)

WEBKIT_FRAMEWORK_DECLARE(CyberKitLegacy)
WEBKIT_INCLUDE_CONFIG_FILES_IF_EXISTS()
WEBKIT_FRAMEWORK(CyberKitLegacy)

WEBKIT_COPY_FILES(CyberKitLegacy_CopyHeaders
    DESTINATION ${CyberKitLegacy_FRAMEWORK_HEADERS_DIR}/CyberKitLegacy
    FILES ${CyberKitLegacy_PUBLIC_FRAMEWORK_HEADERS}
    FLATTENED
)
