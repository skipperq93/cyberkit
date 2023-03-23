set(CyberKitTestRunner_DIR "${TOOLS_DIR}/CyberKitTestRunner")

file(MAKE_DIRECTORY ${CyberKitTestRunner_DERIVED_SOURCES_DIR}/InjectedBundle)
file(MAKE_DIRECTORY ${CyberKitTestRunner_DERIVED_SOURCES_DIR}/UIScriptContext)

set(CyberKitTestRunner_SOURCES
    CyclicRedundancyCheck.cpp
    GeolocationProviderMock.cpp
    Options.cpp
    PixelDumpSupport.cpp
    TestController.cpp
    TestInvocation.cpp
    TestOptions.cpp
    UIScriptControllerCommon.cpp
    WebNotificationProvider.cpp
    WorkQueueManager.cpp
)

set(CyberKitTestRunner_LIBRARIES
    CyberKit::TestRunnerShared
    CyberKit::CyberCoreTestSupport
)
set(CyberKitTestRunner_FRAMEWORKS
    CyberScriptCore
    PAL
    WTF
    CyberCore
    CyberKit
)
if (NOT USE_SYSTEM_MALLOC)
    list(APPEND CyberKitTestRunner_FRAMEWORKS bmalloc)
endif ()

if (COMPILER_IS_GCC_OR_CLANG)
    if (HAVE_STD_EXPERIMENTAL_FILESYSTEM)
        list(APPEND CyberKitTestRunner_LIBRARIES stdc++fs)
    endif ()
endif ()

set(CyberKitTestRunner_PRIVATE_INCLUDE_DIRECTORIES
    ${CMAKE_BINARY_DIR}
    ${CyberKitTestRunner_DERIVED_SOURCES_DIR}
    ${CyberKitTestRunner_DERIVED_SOURCES_DIR}/UIScriptContext
    ${CyberKitTestRunner_DIR}
)

set(CyberKitTestRunner_WEB_PREFERENCES_TEMPLATES
    ${CyberKitTestRunner_DIR}/Scripts/PreferencesTemplates/TestOptionsGeneratedKeys.h.erb
)

set(CyberKitTestRunner_WEB_PREFERENCES
    ${WTF_SCRIPTS_DIR}/Preferences/UnifiedWebPreferences.yaml
)

set_source_files_properties(${CyberKitTestRunner_WEB_PREFERENCES} PROPERTIES GENERATED TRUE)

add_custom_command(
    OUTPUT ${CyberKitTestRunner_DERIVED_SOURCES_DIR}/TestOptionsGeneratedKeys.h
    DEPENDS ${CyberKitTestRunner_WEB_PREFERENCES_TEMPLATES} ${CyberKitTestRunner_WEB_PREFERENCES} WTF_CopyPreferences
    COMMAND ${RUBY_EXECUTABLE} ${WTF_SCRIPTS_DIR}/GeneratePreferences.rb --frontend CyberKit --outputDir "${CyberKitTestRunner_DERIVED_SOURCES_DIR}" --template ${CyberKitTestRunner_DIR}/Scripts/PreferencesTemplates/TestOptionsGeneratedKeys.h.erb ${CyberKitTestRunner_WEB_PREFERENCES}
    COMMAND_EXPAND_LISTS
    VERBATIM)

list(APPEND CyberKitTestRunner_SOURCES
    ${CyberKitTestRunner_DERIVED_SOURCES_DIR}/TestOptionsGeneratedKeys.h
)

set(CyberKitTestRunner_DEPENDENCIES TestRunnerInjectedBundle)

set(TestRunnerInjectedBundle_SOURCES
    InjectedBundle/AccessibilityController.cpp
    InjectedBundle/AccessibilityTextMarker.cpp
    InjectedBundle/AccessibilityTextMarkerRange.cpp
    InjectedBundle/AccessibilityUIElement.cpp
    InjectedBundle/EventSendingController.cpp
    InjectedBundle/GCController.cpp
    InjectedBundle/InjectedBundle.cpp
    InjectedBundle/InjectedBundleMain.cpp
    InjectedBundle/InjectedBundlePage.cpp
    InjectedBundle/TestRunner.cpp
    InjectedBundle/TextInputController.cpp
)

set(TestRunnerInjectedBundle_PRIVATE_INCLUDE_DIRECTORIES
    ${CMAKE_BINARY_DIR}
    ${CyberKitTestRunner_DERIVED_SOURCES_DIR}/InjectedBundle
    ${CyberKitTestRunner_DIR}
    ${CyberKitTestRunner_DIR}/InjectedBundle
)

set(TestRunnerInjectedBundle_LIBRARY_TYPE SHARED)
set(TestRunnerInjectedBundle_PRIVATE_LIBRARIES
    CyberKit::TestRunnerShared
    CyberKit::CyberCoreTestSupport
)
set(TestRunnerInjectedBundle_FRAMEWORKS ${CyberKitTestRunner_FRAMEWORKS})

set(TestRunnerInjectedBundle_IDL_FILES
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/AccessibilityController.idl"
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/AccessibilityTextMarker.idl"
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/AccessibilityTextMarkerRange.idl"
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/AccessibilityUIElement.idl"
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/EventSendingController.idl"
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/GCController.idl"
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/TestRunner.idl"
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/TextInputController.idl"
)

GENERATE_BINDINGS(TestRunnerInjectedBundleBindings
    OUTPUT_SOURCE TestRunnerInjectedBundle_SOURCES
    INPUT_FILES ${TestRunnerInjectedBundle_IDL_FILES}
    BASE_DIR ${CyberKitTestRunner_DIR}/InjectedBundle/Bindings
    IDL_INCLUDES ${CyberKitTestRunner_DIR}/InjectedBundle/Bindings
    FEATURES ${FEATURE_DEFINES_WITH_SPACE_SEPARATOR}
    DESTINATION ${CyberKitTestRunner_DERIVED_SOURCES_DIR}/InjectedBundle
    GENERATOR TestRunner
)

set(TestRunnerInjectedBundle_DEPENDENCIES
    TestRunnerInjectedBundleBindings
)

WEBKIT_EXECUTABLE_DECLARE(CyberKitTestRunner)
WEBKIT_LIBRARY_DECLARE(TestRunnerInjectedBundle)
WEBKIT_INCLUDE_CONFIG_FILES_IF_EXISTS()

WEBKIT_LIBRARY(TestRunnerInjectedBundle)
WEBKIT_EXECUTABLE(CyberKitTestRunner)

if (COMPILER_IS_GCC_OR_CLANG)
    WEBKIT_ADD_TARGET_CXX_FLAGS(TestRunnerInjectedBundle -Wno-unused-parameter)
    WEBKIT_ADD_TARGET_CXX_FLAGS(CyberKitTestRunner -Wno-unused-parameter)
endif ()
