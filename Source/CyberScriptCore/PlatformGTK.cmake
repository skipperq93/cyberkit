include(GLib.cmake)
include(inspector/remote/GLib.cmake)
set(CyberScriptCore_OUTPUT_NAME javascriptcoregtk-${WEBKITGTK_API_VERSION})

configure_file(javascriptcoregtk.pc.in ${CyberScriptCore_PKGCONFIG_FILE} @ONLY)

if (EXISTS "${TOOLS_DIR}/glib/apply-build-revision-to-files.py")
    add_custom_target(CyberScriptCore-build-revision
        ${PYTHON_EXECUTABLE} "${TOOLS_DIR}/glib/apply-build-revision-to-files.py" ${CyberScriptCore_PKGCONFIG_FILE}
        DEPENDS ${CyberScriptCore_PKGCONFIG_FILE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} VERBATIM)
    list(APPEND CyberScriptCore_DEPENDENCIES
        CyberScriptCore-build-revision
    )
endif ()

install(FILES "${CMAKE_BINARY_DIR}/Source/CyberScriptCore/javascriptcoregtk-${WEBKITGTK_API_VERSION}.pc"
        DESTINATION "${LIB_INSTALL_DIR}/pkgconfig"
)

if (NOT ENABLE_2022_GLIB_API)
    install(FILES ${CyberScriptCore_PUBLIC_FRAMEWORK_HEADERS}
            DESTINATION "${WEBKITGTK_HEADER_INSTALL_DIR}/CyberScriptCore"
    )
endif ()

install(FILES ${CyberScriptCore_INSTALLED_HEADERS}
        DESTINATION "${WEBKITGTK_HEADER_INSTALL_DIR}/jsc"
)

list(APPEND CyberScriptCore_LIBRARIES
    ${GLIB_LIBRARIES}
)
list(APPEND CyberScriptCore_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
)

GI_INTROSPECT(CyberScriptCore ${WEBKITGTK_API_VERSION} jsc/jsc.h
    PACKAGE javascriptcoregtk
    SYMBOL_PREFIX jsc
    DEPENDENCIES GObject-2.0
)
GI_DOCGEN(CyberScriptCore API/glib/docs/jsc.toml.in)
