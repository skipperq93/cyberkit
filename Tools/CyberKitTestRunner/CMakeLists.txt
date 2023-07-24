set(CyberKitTestRunner_DIR "${TOOLS_DIR}/CyberKitTestRunner")
set(CyberKitTestRunner_SHARED_DIR "${TOOLS_DIR}/TestRunnerShared/")
set(CyberKitTestRunner_UISCRIPTCONTEXT_DIR "${CyberKitTestRunner_SHARED_DIR}/UIScriptContext")
set(CyberKitTestRunner_BINDINGS_DIR "${CyberKitTestRunner_SHARED_DIR}/Bindings")

file(MAKE_DIRECTORY ${CyberKitTestRunner_DERIVED_SOURCES_DIR}/InjectedBundle)
file(MAKE_DIRECTORY ${CyberKitTestRunner_DERIVED_SOURCES_DIR}/UIScriptContext)

set(CyberKitTestRunner_SOURCES
    ${CyberKitTestRunner_BINDINGS_DIR}/JSWrapper.cpp

    ${CyberKitTestRunner_UISCRIPTCONTEXT_DIR}/UIScriptContext.cpp
    ${CyberKitTestRunner_UISCRIPTCONTEXT_DIR}/UIScriptControllerShared.cpp

    CyclicRedundancyCheck.cpp
    GeolocationProviderMock.cpp
    Options.cpp
    PixelDumpSupport.cpp
    TestController.cpp
    TestInvocation.cpp
    TestOptions.cpp
    UIScriptController.cpp
    WebNotificationProvider.cpp
    WorkQueueManager.cpp
)

set(CyberKitTestRunner_LIBRARIES
    CyberCoreTestSupport
    CyberKit::CyberScriptCore
    CyberKit::CyberKit
)

set(CyberKitTestRunner_INCLUDE_DIRECTORIES
    ${CMAKE_BINARY_DIR}
    ${CyberKitTestRunner_BINDINGS_DIR}
    ${CyberKitTestRunner_DERIVED_SOURCES_DIR}/UIScriptContext
    ${CyberKitTestRunner_DIR}
    ${CyberKitTestRunner_UISCRIPTCONTEXT_DIR}
)

list(APPEND CyberKitTestRunner_INCLUDE_DIRECTORIES
    ${WEBCORE_DIR}/testing/js
)

list(APPEND CyberKitTestRunner_INCLUDE_DIRECTORIES
    ${CyberCore_PRIVATE_FRAMEWORK_HEADERS_DIR}
)

set(CyberKitTestRunnerInjectedBundle_SOURCES
    ${CyberKitTestRunner_BINDINGS_DIR}/JSWrapper.cpp

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

set(CyberKitTestRunnerInjectedBundle_INCLUDE_DIRECTORIES
    ${CMAKE_BINARY_DIR}
    ${CyberKitTestRunner_BINDINGS_DIR}
    ${CyberKitTestRunner_DERIVED_SOURCES_DIR}/InjectedBundle
    ${CyberKitTestRunner_DIR}
    ${CyberKitTestRunner_DIR}/InjectedBundle
)

list(APPEND CyberKitTestRunnerInjectedBundle_INCLUDE_DIRECTORIES
    ${CyberCore_PRIVATE_FRAMEWORK_HEADERS_DIR}
)

set(CyberKitTestRunnerInjectedBundle_LIBRARIES
    CyberKit::CyberCoreTestSupport
    CyberKit::CyberKit
)

set(CyberKitTestRunnerInjectedBundle_IDL_FILES
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/AccessibilityController.idl"
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/AccessibilityTextMarker.idl"
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/AccessibilityTextMarkerRange.idl"
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/AccessibilityUIElement.idl"
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/EventSendingController.idl"
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/GCController.idl"
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/TestRunner.idl"
    "${CyberKitTestRunner_DIR}/InjectedBundle/Bindings/TextInputController.idl"
)

set(CyberKitTestRunner_IDL_FILES
    "${CyberKitTestRunner_UISCRIPTCONTEXT_DIR}/Bindings/UIScriptController.idl"
)

set(CyberKitTestRunner_DEPENDENCIES
    TestRunnerInjectedBundle
    CyberKitTestRunnerBindings
)

GENERATE_BINDINGS(CyberKitTestRunnerInjectedBundleBindings
    OUTPUT_SOURCE CyberKitTestRunnerInjectedBundle_SOURCES
    INPUT_FILES ${CyberKitTestRunnerInjectedBundle_IDL_FILES}
    BASE_DIR ${CyberKitTestRunner_DIR}/InjectedBundle/Bindings
    IDL_INCLUDES ${CyberKitTestRunner_DIR}/InjectedBundle/Bindings
    FEATURES ${FEATURE_DEFINES_WITH_SPACE_SEPARATOR}
    DESTINATION ${CyberKitTestRunner_DERIVED_SOURCES_DIR}/InjectedBundle
    GENERATOR TestRunner)

GENERATE_BINDINGS(CyberKitTestRunnerBindings
    OUTPUT_SOURCE CyberKitTestRunner_SOURCES
    INPUT_FILES ${CyberKitTestRunner_IDL_FILES}
    BASE_DIR ${CyberKitTestRunner_DIR}/InjectedBundle/Bindings
    IDL_INCLUDES ${CyberKitTestRunner_DIR}/InjectedBundle/Bindings
    FEATURES ${FEATURE_DEFINES_WITH_SPACE_SEPARATOR}
    DESTINATION ${CyberKitTestRunner_DERIVED_SOURCES_DIR}/UIScriptContext
    GENERATOR TestRunner)

WEBKIT_EXECUTABLE_DECLARE(CyberKitTestRunner)
WEBKIT_INCLUDE_CONFIG_FILES_IF_EXISTS()

add_library(TestRunnerInjectedBundle SHARED ${CyberKitTestRunnerInjectedBundle_SOURCES})
target_include_directories(TestRunnerInjectedBundle PRIVATE ${CyberKitTestRunnerInjectedBundle_INCLUDE_DIRECTORIES})
target_link_libraries(TestRunnerInjectedBundle PRIVATE ${CyberKitTestRunnerInjectedBundle_LIBRARIES})
add_dependencies(TestRunnerInjectedBundle CyberKitTestRunnerInjectedBundleBindings)

WEBKIT_EXECUTABLE(CyberKitTestRunner)

if (COMPILER_IS_GCC_OR_CLANG)
    WEBKIT_ADD_TARGET_CXX_FLAGS(TestRunnerInjectedBundle -Wno-unused-parameter)
    WEBKIT_ADD_TARGET_CXX_FLAGS(CyberKitTestRunner -Wno-unused-parameter)
endif ()
