list(APPEND CyberScriptCore_UNIFIED_SOURCE_LIST_FILES
    "inspector/remote/SourcesCocoa.txt"
)

list(APPEND CyberScriptCore_PRIVATE_INCLUDE_DIRECTORIES
    "${JAVASCRIPTCORE_DIR}/inspector/remote/cocoa"
)

list(APPEND CyberScriptCore_PRIVATE_FRAMEWORK_HEADERS
    inspector/remote/cocoa/RemoteInspectorXPCConnection.h
)
