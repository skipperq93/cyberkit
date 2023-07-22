add_definitions(-DSTATICALLY_LINKED_WITH_WTF)

if (USE_GLIB)
    list(APPEND CyberScriptCore_SYSTEM_INCLUDE_DIRECTORIES
        ${GLIB_INCLUDE_DIRS}
    )
    list(APPEND CyberScriptCore_LIBRARIES
        ${GLIB_LIBRARIES}
    )
endif ()
