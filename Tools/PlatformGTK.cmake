if (DEVELOPER_MODE)
    add_subdirectory(CyberKitTestRunner)
    add_subdirectory(ImageDiff)

    if (ENABLE_API_TESTS)
        add_subdirectory(TestCyberKitAPI/glib)
    endif ()
endif ()

if (ENABLE_MINIBROWSER)
  add_subdirectory(MiniBrowser/gtk)
endif ()
