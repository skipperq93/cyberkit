find_library(CARBON_LIBRARY Carbon)
find_library(FOUNDATION_LIBRARY Foundation)

find_library(APPLICATIONSERVICES_LIBRARY ApplicationServices)
find_library(CORESERVICES_LIBRARY CoreServices)
add_definitions(-iframework ${APPLICATIONSERVICES_LIBRARY}/Versions/Current/Frameworks)
add_definitions(-iframework ${CORESERVICES_LIBRARY}/Versions/Current/Frameworks)

link_directories(../../CyberKitLibraries)
add_definitions(-DJSC_API_AVAILABLE\\\(...\\\)=)
add_definitions(-DJSC_CLASS_AVAILABLE\\\(...\\\)=)

list(APPEND CyberKitTestRunner_LIBRARIES
    ${CARBON_LIBRARY}
)

list(APPEND CyberKitTestRunner_INCLUDE_DIRECTORIES
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/CyberKitLibraries
    ${DERIVED_SOURCES_DIR}
    ${DERIVED_SOURCES_DIR}/CyberCore
    ${FORWARDING_HEADERS_DIR}
    ${FORWARDING_HEADERS_DIR}/CyberScriptCore
    ${FORWARDING_HEADERS_DIR}/CyberCore
    ${ICU_INCLUDE_DIRS}
    ${WEBCORE_DIR}/testing/cocoa
    ${WEBKITLEGACY_DIR}
    ${WEBKIT_TESTRUNNER_DIR}/cf
    ${WEBKIT_TESTRUNNER_DIR}/cg
    ${WEBKIT_TESTRUNNER_DIR}/cocoa
    ${WEBKIT_TESTRUNNER_DIR}/mac
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/mac
    ${WEBKIT_TESTRUNNER_SHARED_DIR}/EventSerialization/mac
    ${WEBKIT_TESTRUNNER_SHARED_DIR}/cocoa
    ${WEBKIT_TESTRUNNER_SHARED_DIR}/mac
    ${WEBKIT_TESTRUNNER_SHARED_DIR}/spi
)

list(APPEND CyberKitTestRunnerInjectedBundle_SOURCES
    ${WEBKIT_TESTRUNNER_DIR}/cocoa/CrashReporterInfo.mm

    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/cocoa/ActivateFontsCocoa.mm
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/cocoa/InjectedBundlePageCocoa.mm

    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/mac/AccessibilityControllerMac.mm
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/mac/AccessibilityNotificationHandler.mm
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/mac/AccessibilityTextMarkerRangeMac.mm
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/mac/InjectedBundleMac.mm
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/mac/AccessibilityCommonMac.mm
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/mac/AccessibilityTextMarkerMac.mm
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/mac/AccessibilityUIElementMac.mm
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/mac/TestRunnerMac.mm

    ${WEBKIT_TESTRUNNER_SHARED_DIR}/EventSerialization/mac/EventSerializerMac.mm
    ${WEBKIT_TESTRUNNER_SHARED_DIR}/EventSerialization/mac/SharedEventStreamsMac.mm
)

list(APPEND CyberKitTestRunnerInjectedBundle_LIBRARIES
    ${FOUNDATION_LIBRARY}
    CyberScriptCore
    WTF
    CyberCoreTestSupport
    CyberKit
)
set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "-framework Cocoa")

list(APPEND CyberKitTestRunner_SOURCES
    ${WEBKIT_TESTRUNNER_DIR}/cg/TestInvocationCG.cpp

    ${WEBKIT_TESTRUNNER_DIR}/cocoa/CrashReporterInfo.mm
    ${WEBKIT_TESTRUNNER_DIR}/cocoa/TestControllerCocoa.mm
    ${WEBKIT_TESTRUNNER_DIR}/cocoa/TestRunnerWKWebView.mm
    ${WEBKIT_TESTRUNNER_DIR}/cocoa/TestWebsiteDataStoreDelegate.mm
    ${WEBKIT_TESTRUNNER_DIR}/cocoa/UIScriptControllerCocoa.mm

    ${WEBKIT_TESTRUNNER_DIR}/mac/EventSenderProxy.mm
    ${WEBKIT_TESTRUNNER_DIR}/mac/PlatformWebViewMac.mm
    ${WEBKIT_TESTRUNNER_DIR}/mac/PoseAsClass.mm
    ${WEBKIT_TESTRUNNER_DIR}/mac/TestControllerMac.mm
    ${WEBKIT_TESTRUNNER_DIR}/mac/UIScriptControllerMac.mm
    ${WEBKIT_TESTRUNNER_DIR}/mac/CyberKitTestRunnerDraggingInfo.mm
    ${WEBKIT_TESTRUNNER_DIR}/mac/CyberKitTestRunnerEvent.mm
    ${WEBKIT_TESTRUNNER_DIR}/mac/CyberKitTestRunnerPasteboard.mm
    ${WEBKIT_TESTRUNNER_DIR}/mac/CyberKitTestRunnerWindow.mm
    ${WEBKIT_TESTRUNNER_DIR}/mac/main.mm
    
    ${WEBKIT_TESTRUNNER_SHARED_DIR}/cocoa/ClassMethodSwizzler.mm

    ${WEBKIT_TESTRUNNER_SHARED_DIR}/EventSerialization/mac/EventSerializerMac.mm
    ${WEBKIT_TESTRUNNER_SHARED_DIR}/EventSerialization/mac/SharedEventStreamsMac.mm
)

link_directories(../../CyberKitLibraries)
