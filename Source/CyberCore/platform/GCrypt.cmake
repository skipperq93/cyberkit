if (ENABLE_WEB_CRYPTO)
    list(APPEND CyberCore_UNIFIED_SOURCE_LIST_FILES
        "platform/SourcesGCrypt.txt"
    )
endif ()

list(APPEND CyberCore_LIBRARIES
    LibGcrypt::LibGcrypt
)
