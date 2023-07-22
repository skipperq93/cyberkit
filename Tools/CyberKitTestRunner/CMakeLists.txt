set(WEBKIT_TESTRUNNER_DIR "${TOOLS_DIR}/CyberKitTestRunner")
set(WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR "${TOOLS_DIR}/CyberKitTestRunner/InjectedBundle")
set(WEBKIT_TESTRUNNER_SHARED_DIR "${TOOLS_DIR}/TestRunnerShared/")
set(WEBKIT_TESTRUNNER_UISCRIPTCONTEXT_DIR "${WEBKIT_TESTRUNNER_SHARED_DIR}/UIScriptContext")
set(WEBKIT_TESTRUNNER_BINDINGS_DIR "${WEBKIT_TESTRUNNER_SHARED_DIR}/Bindings")

file(MAKE_DIRECTORY ${DERIVED_SOURCES_DIR}/InjectedBundle)
file(MAKE_DIRECTORY ${DERIVED_SOURCES_DIR}/UIScriptContext)

set(CyberKitTestRunner_SOURCES
    ${WEBKIT_TESTRUNNER_DIR}/CyclicRedundancyCheck.cpp
    ${WEBKIT_TESTRUNNER_DIR}/GeolocationProviderMock.cpp
    ${WEBKIT_TESTRUNNER_DIR}/Options.cpp
    ${WEBKIT_TESTRUNNER_DIR}/PixelDumpSupport.cpp
    ${WEBKIT_TESTRUNNER_DIR}/TestController.cpp
    ${WEBKIT_TESTRUNNER_DIR}/TestInvocation.cpp
    ${WEBKIT_TESTRUNNER_DIR}/TestOptions.cpp
    ${WEBKIT_TESTRUNNER_DIR}/WebNotificationProvider.cpp
    ${WEBKIT_TESTRUNNER_DIR}/WorkQueueManager.cpp
    ${WEBKIT_TESTRUNNER_UISCRIPTCONTEXT_DIR}/UIScriptContext.cpp
    ${WEBKIT_TESTRUNNER_UISCRIPTCONTEXT_DIR}/UIScriptController.cpp
    ${WEBKIT_TESTRUNNER_BINDINGS_DIR}/JSWrapper.cpp
)

set(CyberKitTestRunner_LIBRARIES
    CyberKit::CyberScriptCore
    CyberCoreTestSupport
    CyberKit
)

set(CyberKitTestRunner_INCLUDE_DIRECTORIES
    ${WEBKIT_TESTRUNNER_DIR}
    ${WEBKIT_TESTRUNNER_DIR}/InjectedBundle
    ${WEBKIT_TESTRUNNER_DIR}/InjectedBundle/Bindings
    ${WEBKIT_TESTRUNNER_DIR}/InjectedBundle/atk
    ${WEBKIT_TESTRUNNER_UISCRIPTCONTEXT_DIR}
    ${WEBKIT_TESTRUNNER_BINDINGS_DIR}
    ${WEBCORE_DIR}/testing/js
    ${WEBKIT_DIR}/Platform/IPC
    ${WEBKIT_DIR}/Shared
    ${WEBKIT_DIR}/Shared/API/c
    ${WEBKIT_DIR}/Shared/Plugins
    ${WEBKIT_DIR}/UIProcess
    ${WEBKIT_DIR}/UIProcess/API/C/soup
    ${WEBKIT_DIR}/WebProcess/InjectedBundle
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/c
    ${DERIVED_SOURCES_DIR}/InjectedBundle
    ${DERIVED_SOURCES_DIR}/UIScriptContext
    ${CMAKE_SOURCE_DIR}/Source
)

set(CyberKitTestRunner_SYSTEM_INCLUDE_DIRECTORIES
    ${LIBSOUP_INCLUDE_DIRS}
)

set(CyberKitTestRunnerInjectedBundle_SOURCES
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/AccessibilityController.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/AccessibilityTextMarker.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/AccessibilityTextMarkerRange.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/AccessibilityUIElement.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/EventSendingController.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/GCController.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/InjectedBundle.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/InjectedBundleMain.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/InjectedBundlePage.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/TestRunner.cpp
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/TextInputController.cpp

    ${WEBKIT_TESTRUNNER_BINDINGS_DIR}/JSWrapper.cpp
)

set(CyberKitTestRunnerInjectedBundle_IDL_FILES
    "${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/Bindings/AccessibilityController.idl"
    "${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/Bindings/AccessibilityTextMarker.idl"
    "${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/Bindings/AccessibilityTextMarkerRange.idl"
    "${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/Bindings/AccessibilityUIElement.idl"
    "${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/Bindings/EventSendingController.idl"
    "${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/Bindings/GCController.idl"
    "${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/Bindings/TestRunner.idl"
    "${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/Bindings/TextInputController.idl"
)

set(CyberKitTestRunner_IDL_FILES
    "${WEBKIT_TESTRUNNER_UISCRIPTCONTEXT_DIR}/Bindings/UIScriptController.idl"
)

GENERATE_BINDINGS(CyberKitTestRunnerInjectedBundleBindings
    OUTPUT_SOURCE CyberKitTestRunnerInjectedBundle_SOURCES
    INPUT_FILES ${CyberKitTestRunnerInjectedBundle_IDL_FILES}
    BASE_DIR ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/Bindings
    IDL_INCLUDES ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/Bindings
    FEATURES ${FEATURE_DEFINES_WITH_SPACE_SEPARATOR}
    DESTINATION ${DERIVED_SOURCES_DIR}/InjectedBundle
    GENERATOR TestRunner)

GENERATE_BINDINGS(CyberKitTestRunnerBindings
    OUTPUT_SOURCE CyberKitTestRunner_SOURCES
    INPUT_FILES ${CyberKitTestRunner_IDL_FILES}
    BASE_DIR ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/Bindings
    IDL_INCLUDES ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/Bindings
    FEATURES ${FEATURE_DEFINES_WITH_SPACE_SEPARATOR}
    DESTINATION ${DERIVED_SOURCES_DIR}/UIScriptContext
    GENERATOR TestRunner)

WEBKIT_INCLUDE_CONFIG_FILES_IF_EXISTS()

include_directories(${CyberKitTestRunner_INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${CyberKitTestRunner_SYSTEM_INCLUDE_DIRECTORIES})

add_library(TestRunnerInjectedBundle SHARED ${CyberKitTestRunnerInjectedBundle_SOURCES})
target_link_libraries(TestRunnerInjectedBundle ${CyberKitTestRunnerInjectedBundle_LIBRARIES})
add_dependencies(TestRunnerInjectedBundle CyberKitTestRunnerInjectedBundleBindings)

add_executable(CyberKitTestRunner ${CyberKitTestRunner_SOURCES})
target_link_libraries(CyberKitTestRunner ${CyberKitTestRunner_LIBRARIES})
add_dependencies(CyberKitTestRunner CyberKitTestRunnerBindings)

if (COMPILER_IS_GCC_OR_CLANG)
    WEBKIT_ADD_TARGET_CXX_FLAGS(TestRunnerInjectedBundle -Wno-unused-parameter)
    WEBKIT_ADD_TARGET_CXX_FLAGS(CyberKitTestRunner -Wno-unused-parameter)
endif ()

if (WIN32)
    add_dependencies(CyberKitTestRunnerLib CyberKitTestRunnerBindings)
    add_dependencies(CyberKitTestRunner CyberKitTestRunnerLib)
elseif (NOT APPLE)
    add_dependencies(CyberKit ${ForwardingHeadersForCyberKitTestRunner_NAME})
endif ()
