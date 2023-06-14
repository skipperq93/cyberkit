list(APPEND CyberCore_INCLUDE_DIRECTORIES
    "${WEBCORE_DIR}/platform/graphics/cairo"
)

list(APPEND CyberCore_UNIFIED_SOURCE_LIST_FILES
    "platform/SourcesCairo.txt"
)

list(APPEND CyberCore_SYSTEM_INCLUDE_DIRECTORIES
    ${CAIRO_INCLUDE_DIRS}
)

list(APPEND CyberCore_LIBRARIES
    ${CAIRO_LIBRARIES}
)
