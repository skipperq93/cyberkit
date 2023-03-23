list(APPEND CyberScriptCore_SOURCES
    API/JSStringRefBSTR.cpp
)

list(APPEND CyberScriptCore_PUBLIC_FRAMEWORK_HEADERS
    API/JSStringRefBSTR.h
    API/CyberScriptCore.h
)

if (USE_CF)
    list(APPEND CyberScriptCore_SOURCES
        API/JSStringRefCF.cpp
    )

    list(APPEND CyberScriptCore_PUBLIC_FRAMEWORK_HEADERS
        API/JSStringRefCF.h
    )

    list(APPEND CyberScriptCore_LIBRARIES
        Apple::CoreFoundation
    )
endif ()

if (ENABLE_REMOTE_INSPECTOR)
    include(inspector/remote/Socket.cmake)
else ()
    list(REMOVE_ITEM CyberScriptCore_SOURCES
        inspector/JSGlobalObjectInspectorController.cpp
    )
endif ()
