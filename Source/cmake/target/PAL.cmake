if (NOT TARGET CyberKit::PAL)
    if (NOT INTERNAL_BUILD)
        message(FATAL_ERROR "CyberKit::PAL target not found")
    endif ()

    # This should be moved to an if block if the Apple Mac/iOS build moves completely to CMake
    # Just assuming Windows for the moment
    add_library(CyberKit::PAL STATIC IMPORTED)
    set_target_properties(CyberKit::PAL PROPERTIES
        IMPORTED_LOCATION ${WEBKIT_LIBRARIES_LINK_DIR}/PAL${DEBUG_SUFFIX}.lib
        # Should add Apple libraries here when https://bugs.webkit.org/show_bug.cgi?id=205085 lands
        INTERFACE_LINK_LIBRARIES "CyberKit::WTF"
    )
    set(PAL_FRAMEWORK_HEADERS_DIR "${CMAKE_BINARY_DIR}/../include/private")
    target_include_directories(CyberKit::PAL INTERFACE
        ${PAL_FRAMEWORK_HEADERS_DIR}
    )
endif ()
