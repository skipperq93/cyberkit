list(APPEND CyberScriptCore_UNIFIED_SOURCE_LIST_FILES
    "inspector/remote/SourcesGLib.txt"
)

list(APPEND CyberScriptCore_PRIVATE_INCLUDE_DIRECTORIES
    "${JAVASCRIPTCORE_DIR}/inspector/remote/glib"
)

list(APPEND CyberScriptCore_PRIVATE_FRAMEWORK_HEADERS
    inspector/remote/glib/RemoteInspectorServer.h
    inspector/remote/glib/RemoteInspectorUtils.h
)

list(APPEND CyberScriptCore_LIBRARIES
    ${GLIB_LIBRARIES}
)
list(APPEND CyberScriptCore_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
)
