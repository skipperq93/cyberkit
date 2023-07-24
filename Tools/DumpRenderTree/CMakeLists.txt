set(DumpRenderTree_DIR "${TOOLS_DIR}/DumpRenderTree")
set(DumpRenderTree_BINDINGS_DIR "${TOOLS_DIR}/DumpRenderTree/Bindings")
set(CyberKitTestRunner_SHARED_DIR "${TOOLS_DIR}/TestRunnerShared/")
set(CyberKitTestRunner_UISCRIPTCONTEXT_DIR "${TOOLS_DIR}/TestRunnerShared/UIScriptContext")

file(MAKE_DIRECTORY ${DumpRenderTree_DERIVED_SOURCES_DIR})

set(DumpRenderTree_SOURCES
    ${CyberKitTestRunner_SHARED_DIR}/Bindings/JSWrapper.cpp

    ${CyberKitTestRunner_UISCRIPTCONTEXT_DIR}/UIScriptContext.cpp
    ${CyberKitTestRunner_UISCRIPTCONTEXT_DIR}/UIScriptControllerShared.cpp

    AccessibilityController.cpp
    AccessibilityTextMarker.cpp
    AccessibilityUIElement.cpp
    CyclicRedundancyCheck.cpp
    DumpRenderTreeCommon.cpp
    GCController.cpp
    JavaScriptThreading.cpp
    PixelDumpSupport.cpp
    TestOptions.cpp
    TestRunner.cpp
    UIScriptController.cpp
    WorkQueue.cpp
)

set(DumpRenderTree_LIBRARIES
    CyberKit::CyberCoreTestSupport
    CyberKit::CyberScriptCore
)

set(DumpRenderTree_PRIVATE_INCLUDE_DIRECTORIES
    ${CMAKE_BINARY_DIR}
    ${DumpRenderTree_DERIVED_SOURCES_DIR}
    ${DumpRenderTree_DIR}
    ${CyberKitTestRunner_SHARED_DIR}/Bindings
    ${CyberKitTestRunner_UISCRIPTCONTEXT_DIR}
)

list(APPEND DumpRenderTree_PRIVATE_INCLUDE_DIRECTORIES
    ${PAL_FRAMEWORK_HEADERS_DIR}
    ${CyberCore_PRIVATE_FRAMEWORK_HEADERS_DIR}
    ${CyberKitLegacy_FRAMEWORK_HEADERS_DIR}
)

set(DumpRenderTree_IDL_FILES
    "${CyberKitTestRunner_UISCRIPTCONTEXT_DIR}/Bindings/UIScriptController.idl"
)

GENERATE_BINDINGS(DumpRenderTreeBindings
    OUTPUT_SOURCE DumpRenderTree_SOURCES
    INPUT_FILES ${DumpRenderTree_IDL_FILES}
    BASE_DIR ${DumpRenderTree_BINDINGS_DIR}
    IDL_INCLUDES Bindings
    FEATURES ${FEATURE_DEFINES_WITH_SPACE_SEPARATOR}
    DESTINATION ${DumpRenderTree_DERIVED_SOURCES_DIR}
    GENERATOR DumpRenderTree)

set(DumpRenderTree_DEPENDENCIES DumpRenderTreeBindings)

WEBKIT_EXECUTABLE_DECLARE(DumpRenderTree)
WEBKIT_INCLUDE_CONFIG_FILES_IF_EXISTS()
WEBKIT_EXECUTABLE(DumpRenderTree)

if (ENABLE_NETSCAPE_PLUGIN_API)
    add_subdirectory(TestNetscapePlugIn)
endif ()
