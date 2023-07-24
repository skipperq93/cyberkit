add_definitions(-DSTATICALLY_LINKED_WITH_WTF)

if (ENABLE_REMOTE_INSPECTOR)
    list(APPEND CyberScriptCore_SOURCES
        inspector/remote/RemoteConnectionToTarget.cpp
        inspector/remote/RemoteControllableTarget.cpp
        inspector/remote/RemoteInspectionTarget.cpp
        inspector/remote/RemoteInspector.cpp
    )
endif ()

if (USE_GLIB)
    if (ENABLE_REMOTE_INSPECTOR)
        list(APPEND CyberScriptCore_SOURCES
            inspector/remote/glib/RemoteInspectorGlib.cpp
        )
    endif ()

    list(APPEND CyberScriptCore_SYSTEM_INCLUDE_DIRECTORIES
        ${GLIB_INCLUDE_DIRS}
    )
    list(APPEND CyberScriptCore_LIBRARIES
        ${GLIB_LIBRARIES}
    )
endif ()
