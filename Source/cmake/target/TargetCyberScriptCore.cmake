if (NOT TARGET CyberKit::CyberScriptCore)
    if (NOT INTERNAL_BUILD)
        message(FATAL_ERROR "CyberKit::CyberScriptCore target not found")
    endif ()

    # This should be moved to an if block if the Apple Mac/iOS build moves completely to CMake
    # Just assuming Windows for the moment
    add_library(CyberKit::CyberScriptCore SHARED IMPORTED)
    set_target_properties(CyberKit::CyberScriptCore PROPERTIES
        IMPORTED_LOCATION ${WEBKIT_LIBRARIES_RUNTIME_DIR}/CyberScriptCore${DEBUG_SUFFIX}.dll
        IMPORTED_IMPLIB ${WEBKIT_LIBRARIES_LINK_DIR}/CyberScriptCore${DEBUG_SUFFIX}.lib
        # Should add Apple::CoreFoundation here when https://bugs.webkit.org/show_bug.cgi?id=205085 lands
        INTERFACE_LINK_LIBRARIES "CyberKit::WTF;ICU::data;ICU::i18n;ICU::uc"
    )
    set(CyberScriptCore_FRAMEWORK_HEADERS_DIR "${CMAKE_BINARY_DIR}/../include/private/CyberScriptCore")
    set(CyberScriptCore_PRIVATE_FRAMEWORK_HEADERS_DIR ${CyberScriptCore_FRAMEWORK_HEADERS_DIR})

    target_include_directories(CyberKit::CyberScriptCore INTERFACE
        ${CyberScriptCore_FRAMEWORK_HEADERS_DIR}
        ${CyberScriptCore_PRIVATE_FRAMEWORK_HEADERS_DIR}
    )
endif ()
