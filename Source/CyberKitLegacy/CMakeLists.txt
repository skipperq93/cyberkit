cmake_minimum_required(VERSION 3.10)
include(CyberKitCommon)
include(target/TargetWTF)
include(target/TargetCyberScriptCore)
include(target/PAL)
include(target/CyberCore)
set_property(DIRECTORY . PROPERTY FOLDER "CyberKitLegacy")

set(CyberKitLegacy_SOURCES
    Storage/InprocessIDBServer.cpp
    Storage/StorageAreaImpl.cpp
    Storage/StorageAreaSync.cpp
    Storage/StorageNamespaceImpl.cpp
    Storage/StorageSyncManager.cpp
    Storage/StorageThread.cpp
    Storage/StorageTracker.cpp
    Storage/WebDatabaseProvider.cpp
    Storage/WebStorageNamespaceProvider.cpp

    CyberCoreSupport/NetworkStorageSessionMap.cpp
    CyberCoreSupport/WebResourceLoadScheduler.cpp
    CyberCoreSupport/WebViewGroup.cpp
)

set(CyberKitLegacy_PRIVATE_INCLUDE_DIRECTORIES
    "${CMAKE_BINARY_DIR}"
    "${WEBKITLEGACY_DIR}/Storage"
    "${WEBKITLEGACY_DIR}/CyberCoreSupport"
    "${CyberKitLegacy_DERIVED_SOURCES_DIR}"
)

set(CyberKitLegacy_PRIVATE_LIBRARIES
    CyberKit::CyberScriptCore
    CyberKit::PAL
    CyberKit::CyberCore
)

WEBKIT_FRAMEWORK_DECLARE(CyberKitLegacy)
WEBKIT_INCLUDE_CONFIG_FILES_IF_EXISTS()

if (MSVC)
    WEBKIT_ADD_PRECOMPILED_HEADER("CyberKitPrefix.h" "win/CyberKitPrefix.cpp" CyberKitLegacy_SOURCES)
    list(APPEND CyberKitLegacy_SOURCES win/CyberKit.resources/CyberKit.rc)
    add_definitions(/DWEBKIT_EXPORTS /DFRAMEWORK_NAME=CyberKitLegacy)
endif ()

WEBKIT_FRAMEWORK(CyberKitLegacy)

if (TARGET CyberKitLegacyGUID)
    add_dependencies(CyberKitLegacy CyberKitLegacyGUID)
endif ()

if (APPLE)
    set_target_properties(CyberKitLegacy PROPERTIES LINK_FLAGS "-umbrella CyberKit")
endif ()

if (MSVC)
    add_custom_command(
        TARGET CyberKitLegacy
        POST_BUILD
        COMMAND ${PERL_EXECUTABLE} ${WEBKIT_LIBRARIES_DIR}/tools/scripts/version-stamp.pl ${CyberKitLegacy_DERIVED_SOURCES_DIR} $<TARGET_FILE:CyberKitLegacy>
        VERBATIM)

    if (USE_CA)
        add_subdirectory(win/CyberKitQuartzCoreAdditions)
    endif ()
endif ()
