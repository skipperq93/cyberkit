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
        ${COREFOUNDATION_LIBRARY}
    )
endif ()

if (NOT WTF_PLATFORM_WIN_CAIRO)
    list(APPEND CyberScriptCore_LIBRARIES
        winmm
    )
endif ()

if (ENABLE_REMOTE_INSPECTOR)
    list(APPEND CyberScriptCore_PRIVATE_INCLUDE_DIRECTORIES
        "${JAVASCRIPTCORE_DIR}/inspector/remote/socket"
    )

    list(APPEND CyberScriptCore_PRIVATE_FRAMEWORK_HEADERS
        inspector/remote/RemoteAutomationTarget.h
        inspector/remote/RemoteConnectionToTarget.h
        inspector/remote/RemoteControllableTarget.h
        inspector/remote/RemoteInspectionTarget.h
        inspector/remote/RemoteInspector.h

        inspector/remote/socket/RemoteInspectorConnectionClient.h
        inspector/remote/socket/RemoteInspectorMessageParser.h
        inspector/remote/socket/RemoteInspectorServer.h
        inspector/remote/socket/RemoteInspectorSocket.h
        inspector/remote/socket/RemoteInspectorSocketEndpoint.h
    )

    list(APPEND CyberScriptCore_SOURCES
        API/JSRemoteInspector.cpp

        inspector/remote/RemoteAutomationTarget.cpp
        inspector/remote/RemoteConnectionToTarget.cpp
        inspector/remote/RemoteControllableTarget.cpp
        inspector/remote/RemoteInspectionTarget.cpp
        inspector/remote/RemoteInspector.cpp

        inspector/remote/socket/RemoteInspectorConnectionClient.cpp
        inspector/remote/socket/RemoteInspectorMessageParser.cpp
        inspector/remote/socket/RemoteInspectorServer.cpp
        inspector/remote/socket/RemoteInspectorSocket.cpp
        inspector/remote/socket/RemoteInspectorSocketEndpoint.cpp

        inspector/remote/socket/win/RemoteInspectorSocketWin.cpp
    )

    set(CyberScriptCore_LIBRARIES PUBLIC ${CyberScriptCore_LIBRARIES})
    list(APPEND CyberScriptCore_LIBRARIES
        PRIVATE ws2_32
    )
else ()
    list(REMOVE_ITEM CyberScriptCore_SOURCES
        inspector/JSGlobalObjectInspectorController.cpp
    )
endif ()

file(COPY
    "${JAVASCRIPTCORE_DIR}/CyberScriptCore.vcxproj/CyberScriptCore.resources"
    DESTINATION
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

set(CyberScriptCore_OUTPUT_NAME CyberScriptCore${DEBUG_SUFFIX})
