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
    ${ICU_INCLUDE_DIRS}
    ${WEBCORE_DIR}/testing/cocoa
    ${WEBKITLEGACY_DIR}
    ${CyberKitTestRunner_DIR}/cf
    ${CyberKitTestRunner_DIR}/cg
    ${CyberKitTestRunner_DIR}/cocoa
    ${CyberKitTestRunner_DIR}/mac
    ${CyberKitTestRunner_DIR}/InjectedBundle/mac
    ${CyberKitTestRunner_SHARED_DIR}/EventSerialization/mac
    ${CyberKitTestRunner_SHARED_DIR}/cocoa
    ${CyberKitTestRunner_SHARED_DIR}/mac
    ${CyberKitTestRunner_SHARED_DIR}/spi
)

list(APPEND TestRunnerInjectedBundle_SOURCES
    ${CyberKitTestRunner_DIR}/InjectedBundle/cocoa/AccessibilityCommonCocoa.mm
    ${CyberKitTestRunner_DIR}/InjectedBundle/cocoa/ActivateFontsCocoa.mm
    ${CyberKitTestRunner_DIR}/InjectedBundle/cocoa/InjectedBundlePageCocoa.mm

    ${CyberKitTestRunner_DIR}/InjectedBundle/mac/AccessibilityControllerMac.mm
    ${CyberKitTestRunner_DIR}/InjectedBundle/mac/AccessibilityNotificationHandler.mm
    ${CyberKitTestRunner_DIR}/InjectedBundle/mac/AccessibilityTextMarkerRangeMac.mm
    ${CyberKitTestRunner_DIR}/InjectedBundle/mac/InjectedBundleMac.mm
    ${CyberKitTestRunner_DIR}/InjectedBundle/mac/AccessibilityTextMarkerMac.mm
    ${CyberKitTestRunner_DIR}/InjectedBundle/mac/AccessibilityUIElementMac.mm
    ${CyberKitTestRunner_DIR}/InjectedBundle/mac/TestRunnerMac.mm

    ${CyberKitTestRunner_SHARED_DIR}/EventSerialization/mac/EventSerializerMac.mm
    ${CyberKitTestRunner_SHARED_DIR}/EventSerialization/mac/SharedEventStreamsMac.mm
)

list(APPEND TestRunnerInjectedBundle_LIBRARIES
    ${FOUNDATION_LIBRARY}
    JavaScriptCore
    WTF
    CyberCoreTestSupport
    CyberKit
)
set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "-framework Cocoa")

list(APPEND CyberKitTestRunner_SOURCES
    ${CyberKitTestRunner_DIR}/cocoa/TestControllerCocoa.mm
    ${CyberKitTestRunner_DIR}/cocoa/TestRunnerWKWebView.mm
    ${CyberKitTestRunner_DIR}/cocoa/TestWebsiteDataStoreDelegate.mm
    ${CyberKitTestRunner_DIR}/cocoa/UIScriptControllerCocoa.mm

    ${CyberKitTestRunner_DIR}/mac/EventSenderProxy.mm
    ${CyberKitTestRunner_DIR}/mac/PlatformWebViewMac.mm
    ${CyberKitTestRunner_DIR}/mac/TestControllerMac.mm
    ${CyberKitTestRunner_DIR}/mac/UIScriptControllerMac.mm
    ${CyberKitTestRunner_DIR}/mac/CyberKitTestRunnerDraggingInfo.mm
    ${CyberKitTestRunner_DIR}/mac/CyberKitTestRunnerEvent.mm
    ${CyberKitTestRunner_DIR}/mac/CyberKitTestRunnerPasteboard.mm
    ${CyberKitTestRunner_DIR}/mac/CyberKitTestRunnerWindow.mm
    ${CyberKitTestRunner_DIR}/mac/main.mm

    ${CyberKitTestRunner_SHARED_DIR}/cocoa/ClassMethodSwizzler.mm
    ${CyberKitTestRunner_SHARED_DIR}/cocoa/PlatformViewHelpers.mm
    ${CyberKitTestRunner_SHARED_DIR}/cocoa/PoseAsClass.mm

    ${CyberKitTestRunner_SHARED_DIR}/EventSerialization/mac/EventSerializerMac.mm
    ${CyberKitTestRunner_SHARED_DIR}/EventSerialization/mac/SharedEventStreamsMac.mm
)

link_directories(../../CyberKitLibraries)
