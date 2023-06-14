set(TESTWEBKITAPI_DIR "${TOOLS_DIR}/TestCyberKitAPI")
set(test_wtf_LIBRARIES
    WTF${DEBUG_SUFFIX}
    gtest
)

set(test_webcore_LIBRARIES
    WTF${DEBUG_SUFFIX}
    CyberCore${DEBUG_SUFFIX}
    gtest
)

set(TestCyberKitAPI_LIBRARIES
    WTF${DEBUG_SUFFIX}
)

set(TestCyberKitAPI_DEPENDENCIES
    CyberScriptCoreForwardingHeaders
    CyberScriptCorePrivateForwardingHeaders
    WTFForwardingHeaders
)

if (ENABLE_WEBCORE)
    list(APPEND TestCyberKitAPI_DEPENDENCIES PALForwardingHeaders)
endif ()

if (ENABLE_WEBKIT)
    set(test_webkit_api_LIBRARIES
        CyberScriptCore
        TestCyberKitAPIBase
        WTF
        CyberKit
        gtest
    )

    set(test_webkit_api_SOURCES
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/AboutBlankLoad.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/CanHandleRequest.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/CookieManager.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/DocumentStartUserScriptAlertCrash.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/DOMWindowExtensionBasic.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/DOMWindowExtensionNoCache.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/DownloadDecideDestinationCrash.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/EnumerateMediaDevices.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/EvaluateJavaScript.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/FailedLoad.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/Find.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/FirstMeaningfulPaintMilestone.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/ForceRepaint.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/FrameMIMETypeHTML.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/FrameMIMETypePNG.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/Geolocation.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/GetInjectedBundleInitializationUserDataCallback.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/HitTestResultNodeHandle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/InjectedBundleBasic.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/InjectedBundleFrameHitTest.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/InjectedBundleInitializationUserDataCallbackWins.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/LoadAlternateHTMLStringWithNonDirectoryURL.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/LoadCanceledNoServerRedirectCallback.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/LoadPageOnCrash.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/MouseMoveAfterCrash.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/NewFirstVisuallyNonEmptyLayout.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/NewFirstVisuallyNonEmptyLayoutFails.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/NewFirstVisuallyNonEmptyLayoutForImages.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/NewFirstVisuallyNonEmptyLayoutFrames.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/PageLoadBasic.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/PageLoadDidChangeLocationWithinPageForFrame.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/ParentFrame.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/PendingAPIRequestURL.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/PreventEmptyUserAgent.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/PrivateBrowsingPushStateNoHistoryCallback.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/ProvisionalURLAfterWillSendRequestCallback.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/ReloadPageAfterCrash.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/ResizeWindowAfterCrash.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/RestoreSessionStateContainingFormData.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/TextFieldDidBeginAndEndEditing.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/UserMedia.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/UserMessage.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/WillSendSubmitEvent.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/WKPageCopySessionStateWithFiltering.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/WKPageGetScaleFactorNotZero.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/WKPreferences.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/WKRetainPtr.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/WKString.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/WKStringJSString.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/WKURL.cpp
    )

    set(TestCyberKitAPIBase_LIBRARIES
        CyberScriptCore
        WTF
        CyberKit
        gtest
    )

    set(TestCyberKitAPIBase_SOURCES
        ${TESTWEBKITAPI_DIR}/JavaScriptTest.cpp
        ${TESTWEBKITAPI_DIR}/PlatformUtilities.cpp
        ${TESTWEBKITAPI_DIR}/TestsController.cpp
    )

    list(APPEND TestCyberKitAPI_LIBRARIES
        CyberKit
    )
else ()
    list(APPEND TestCyberKitAPI_LIBRARIES
        CyberKitLegacy${DEBUG_SUFFIX}
    )
endif ()


set(TestWTF_SOURCES
    ${TESTWEBKITAPI_DIR}/Counters.cpp
    ${TESTWEBKITAPI_DIR}/TestsController.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/AtomicString.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/BloomFilter.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/CString.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/CheckedArithmeticOperations.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/ConcurrentPtrHashSet.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Condition.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/CrossThreadTask.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/DateMath.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Deque.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/EnumTraits.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Expected.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Function.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/HashCountedSet.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/HashMap.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/HashSet.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Hasher.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/IntegerToStringConversion.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/JSONValue.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/LEBDecoder.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/LifecycleLogger.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/LineEnding.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/ListHashSet.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Lock.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Logger.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/MD5.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Markable.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/MathExtras.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/MediaTime.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/MetaAllocator.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/MoveOnlyLifecycleLogger.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/NakedPtr.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/NeverDestroyed.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Optional.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/OptionSet.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/ParkingLot.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Poisoned.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/PoisonedRef.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/PoisonedRefPtr.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/PoisonedUniquePtr.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/PoisonedUniquePtrForNonTriviallyDestructibleArrays.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/PoisonedUniquePtrForTriviallyDestructibleArrays.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/PriorityQueue.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/RedBlackTree.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Ref.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/RefCounter.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/RefLogger.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/RefPtr.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/SHA1.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/SaturatedArithmeticOperations.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Scope.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/ScopedLambda.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/SetForScope.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/StringBuilder.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/StringConcatenate.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/StringHasher.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/StringImpl.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/StringOperators.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/StringView.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/SynchronizedFixedQueue.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/TextBreakIterator.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/ThreadGroup.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/ThreadMessages.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Threading.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Time.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/URL.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/URLParser.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/UniqueArray.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/UniqueRef.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Variant.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/Vector.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/WTFString.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/WeakPtr.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/WorkQueue.cpp
    ${TESTWEBKITAPI_DIR}/Tests/WTF/WorkerPool.cpp
)

# FIXME: Tests/WTF/RunLoop.cpp is missing because it doesn't work for Windows.
# FIXME: Platform-specific sources in Tests/WTF are not included in TestWTF_SOURCES.

WEBKIT_INCLUDE_CONFIG_FILES_IF_EXISTS()

include_directories(
    ${TESTWEBKITAPI_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/Source
    ${JAVASCRIPTCORE_DIR}
    ${THIRDPARTY_DIR}/gtest/include
    ${WEBKIT_DIR}/Platform/IPC
    ${WEBKIT_DIR}/Shared
    ${WEBKIT_DIR}/Shared/API
    ${WEBKIT_DIR}/Shared/API/c
    ${WEBKIT_DIR}/Shared/Plugins
    ${WEBKIT_DIR}/UIProcess
    ${WEBKIT_DIR}/UIProcess/API
    ${WEBKIT_DIR}/UIProcess/API/C
    ${WEBKIT_DIR}/WebProcess/InjectedBundle
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/c
)

if (ENABLE_WEBKIT)
    add_library(TestCyberKitAPIInjectedBundle SHARED
        ${bundle_harness_SOURCES}
        ${TESTWEBKITAPI_DIR}/InjectedBundleController.cpp
        ${TESTWEBKITAPI_DIR}/InjectedBundleMain.cpp
        ${TESTWEBKITAPI_DIR}/PlatformUtilities.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/CanHandleRequest_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/DidAssociateFormControls_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/DOMWindowExtensionBasic_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/DOMWindowExtensionNoCache_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/DocumentStartUserScriptAlertCrash_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/GetInjectedBundleInitializationUserDataCallback_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/HitTestResultNodeHandle_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/InjectedBundleBasic_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/InjectedBundleFrameHitTest_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/InjectedBundleInitializationUserDataCallbackWins_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/LoadCanceledNoServerRedirectCallback_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/MouseMoveAfterCrash_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/NewFirstVisuallyNonEmptyLayoutFails_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/NewFirstVisuallyNonEmptyLayoutForImages_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/NewFirstVisuallyNonEmptyLayoutFrames_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/NewFirstVisuallyNonEmptyLayout_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/FirstMeaningfulPaintMilestone_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/ParentFrame_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/ProvisionalURLAfterWillSendRequestCallback_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/ResponsivenessTimerDoesntFireEarly_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/TextFieldDidBeginAndEndEditing_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/UserMessage_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/WillLoad_Bundle.cpp
        ${TESTWEBKITAPI_DIR}/Tests/CyberKit/WillSendSubmitEvent_Bundle.cpp
    )

    target_link_libraries(TestCyberKitAPIInjectedBundle ${TestCyberKitAPI_LIBRARIES})
    add_dependencies(TestCyberKitAPIInjectedBundle WTF ${TestCyberKitAPI_DEPENDENCIES})

    if (COMPILER_IS_GCC_OR_CLANG)
        WEBKIT_ADD_TARGET_CXX_FLAGS(TestCyberKitAPIInjectedBundle -Wno-dangling-else
                                                                -Wno-sign-compare
                                                                -Wno-undef
                                                                -Wno-unused-parameter)
    endif ()
endif ()

get_property(gtest_DEFINITIONS GLOBAL PROPERTY gtest_DEFINITIONS)

add_definitions(
    ${gtest_DEFINITIONS}
    -DTEST_WEBKIT2_RESOURCES_DIR=\"${TESTWEBKITAPI_DIR}/Tests/CyberKit\"
)

add_executable(TestWTF
    ${test_main_SOURCES}
    ${TestWTF_SOURCES}
)

if (WIN32)
    add_dependencies(TestWTF TestWTFLib)
else ()
    add_dependencies(TestWTF WTF ${TestCyberKitAPI_DEPENDENCIES})
endif ()

target_link_libraries(TestWTF ${test_wtf_LIBRARIES})
add_test(TestWTF ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY_WTF}/TestWTF)
set_tests_properties(TestWTF PROPERTIES TIMEOUT 60)
set_target_properties(TestWTF PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY_WTF}
)

if (COMPILER_IS_GCC_OR_CLANG)
    WEBKIT_ADD_TARGET_CXX_FLAGS(TestWTF -Wno-dangling-else
                                        -Wno-sign-compare
                                        -Wno-undef
                                        -Wno-unused-parameter)
endif ()

if (ENABLE_WEBKIT)
    add_library(TestCyberKitAPIBase
        ${test_main_SOURCES}
        ${webkit_api_harness_SOURCES}
        ${TestCyberKitAPIBase_SOURCES}
    )

    target_link_libraries(TestCyberKitAPIBase ${TestCyberKitAPIBase_LIBRARIES})

    add_dependencies(TestCyberKitAPIBase CyberKit ${TestCyberKitAPI_DEPENDENCIES})

    if (COMPILER_IS_GCC_OR_CLANG)
        WEBKIT_ADD_TARGET_CXX_FLAGS(TestCyberKitAPIBase -Wno-sign-compare
                                                      -Wno-undef
                                                      -Wno-unused-parameter)
    endif ()
endif ()
