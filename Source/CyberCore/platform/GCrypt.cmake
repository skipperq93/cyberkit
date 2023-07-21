if (ENABLE_WEB_CRYPTO)
    list(APPEND CyberCore_UNIFIED_SOURCE_LIST_FILES
        "platform/SourcesGCrypt.txt"
    )
endif ()

list(APPEND CyberCore_LIBRARIES
    ${LIBGCRYPT_LIBRARIES}
)
list(APPEND CyberCore_SYSTEM_INCLUDE_DIRECTORIES
    ${LIBGCRYPT_INCLUDE_DIRS}
)
