list(APPEND CyberScriptCore_SOURCES
    API/JSStringRefBSTR.cpp
)

list(APPEND CyberScriptCore_PUBLIC_FRAMEWORK_HEADERS
    API/JSStringRefBSTR.h
    API/CyberScriptCore.h
)

list(APPEND CyberScriptCore_PRIVATE_INCLUDE_DIRECTORIES
    ${CMAKE_BINARY_DIR}/../include/private
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
        ${ICU_LIBRARIES}
        winmm
    )
endif ()

list(REMOVE_ITEM CyberScriptCore_SOURCES
    inspector/JSGlobalObjectInspectorController.cpp
)

file(COPY
    "${JAVASCRIPTCORE_DIR}/CyberScriptCore.vcxproj/CyberScriptCore.resources"
    DESTINATION
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

set(CyberScriptCore_OUTPUT_NAME CyberScriptCore${DEBUG_SUFFIX})
