if (DEVELOPER_MODE OR ENABLE_MINIBROWSER)
    add_subdirectory(wpe/backends)
endif ()

if (DEVELOPER_MODE)
    add_subdirectory(ImageDiff)
    add_subdirectory(CyberKitTestRunner)

    if (ENABLE_API_TESTS)
        add_subdirectory(TestCyberKitAPI/glib)
    endif ()
endif ()

if (ENABLE_MINIBROWSER)
    add_subdirectory(MiniBrowser/wpe)
endif ()
