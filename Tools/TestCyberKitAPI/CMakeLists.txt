set(TESTWEBKITAPI_DIR "${TOOLS_DIR}/TestCyberKitAPI")

get_property(gtest_DEFINITIONS GLOBAL PROPERTY gtest_DEFINITIONS)
add_definitions(${gtest_DEFINITIONS})

macro(WEBKIT_TEST _target)
    WEBKIT_EXECUTABLE(${_target})
    add_test(${_target} ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}/${_target})
    set_tests_properties(${_target} PROPERTIES TIMEOUT 60)
    set_target_properties(${_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}
    )

    if (COMPILER_IS_GCC_OR_CLANG)
        WEBKIT_ADD_TARGET_CXX_FLAGS(${_target} -Wno-dangling-else
                                               -Wno-sign-compare
                                               -Wno-undef
                                               -Wno-unused-parameter)
    endif ()
endmacro()

# TestWTF definitions
set(TestWTF_SOURCES
    Counters.cpp
    TestsController.cpp

    Tests/WTF/AtomString.cpp
    Tests/WTF/BloomFilter.cpp
    Tests/WTF/CString.cpp
    Tests/WTF/CheckedArithmeticOperations.cpp
    Tests/WTF/ConcurrentPtrHashSet.cpp
    Tests/WTF/Condition.cpp
    Tests/WTF/CrossThreadCopier.cpp
    Tests/WTF/CrossThreadTask.cpp
    Tests/WTF/DateMath.cpp
    Tests/WTF/Deque.cpp
    Tests/WTF/EnumTraits.cpp
    Tests/WTF/Expected.cpp
    Tests/WTF/FileSystem.cpp
    Tests/WTF/Function.cpp
    Tests/WTF/HashCountedSet.cpp
    Tests/WTF/HashMap.cpp
    Tests/WTF/HashSet.cpp
    Tests/WTF/Hasher.cpp
    Tests/WTF/IntegerToStringConversion.cpp
    Tests/WTF/IteratorRange.cpp
    Tests/WTF/JSONValue.cpp
    Tests/WTF/LEBDecoder.cpp
    Tests/WTF/LifecycleLogger.cpp
    Tests/WTF/LineEnding.cpp
    Tests/WTF/ListHashSet.cpp
    Tests/WTF/Lock.cpp
    Tests/WTF/Logger.cpp
    Tests/WTF/MD5.cpp
    Tests/WTF/Markable.cpp
    Tests/WTF/MathExtras.cpp
    Tests/WTF/MediaTime.cpp
    Tests/WTF/MetaAllocator.cpp
    Tests/WTF/MoveOnlyLifecycleLogger.cpp
    Tests/WTF/NakedPtr.cpp
    Tests/WTF/NeverDestroyed.cpp
    Tests/WTF/OptionSet.cpp
    Tests/WTF/Optional.cpp
    Tests/WTF/Packed.cpp
    Tests/WTF/ParkingLot.cpp
    Tests/WTF/PriorityQueue.cpp
    Tests/WTF/RedBlackTree.cpp
    Tests/WTF/Ref.cpp
    Tests/WTF/RefCounter.cpp
    Tests/WTF/RefLogger.cpp
    Tests/WTF/RefPtr.cpp
    Tests/WTF/RunLoop.cpp
    Tests/WTF/SHA1.cpp
    Tests/WTF/SaturatedArithmeticOperations.cpp
    Tests/WTF/Scope.cpp
    Tests/WTF/ScopedLambda.cpp
    Tests/WTF/SetForScope.cpp
    Tests/WTF/StringBuilder.cpp
    Tests/WTF/StringConcatenate.cpp
    Tests/WTF/StringHasher.cpp
    Tests/WTF/StringImpl.cpp
    Tests/WTF/StringOperators.cpp
    Tests/WTF/StringView.cpp
    Tests/WTF/SynchronizedFixedQueue.cpp
    Tests/WTF/TextBreakIterator.cpp
    Tests/WTF/ThreadGroup.cpp
    Tests/WTF/ThreadMessages.cpp
    Tests/WTF/Threading.cpp
    Tests/WTF/Time.cpp
    Tests/WTF/URL.cpp
    Tests/WTF/URLParser.cpp
    Tests/WTF/UniqueArray.cpp
    Tests/WTF/UniqueRef.cpp
    Tests/WTF/Variant.cpp
    Tests/WTF/Vector.cpp
    Tests/WTF/WTFString.cpp
    Tests/WTF/WeakPtr.cpp
    Tests/WTF/WorkQueue.cpp
    Tests/WTF/WorkerPool.cpp
)

set(TestWTF_PRIVATE_INCLUDE_DIRECTORIES
    ${CMAKE_BINARY_DIR}
    ${TESTWEBKITAPI_DIR}
    ${THIRDPARTY_DIR}/gtest/include
    ${WTF_FRAMEWORK_HEADERS_DIR}
)

set(TestWTF_LIBRARIES
    WTF
    gtest
)

set(TestWTF_DEPENDENCIES WTFFrameworkHeaders)

WEBKIT_EXECUTABLE_DECLARE(TestWTF)

# TestCyberCore definitions
if (ENABLE_WEBCORE)
    set(TestCyberCore_SOURCES
        TestsController.cpp

        Tests/CyberCore/AffineTransform.cpp
        Tests/CyberCore/CSSParser.cpp
        Tests/CyberCore/CalculationValue.cpp
        Tests/CyberCore/ComplexTextController.cpp
        Tests/CyberCore/FileMonitor.cpp
        Tests/CyberCore/FloatPoint.cpp
        Tests/CyberCore/FloatRect.cpp
        Tests/CyberCore/FloatSize.cpp
        Tests/CyberCore/GridPosition.cpp
        Tests/CyberCore/HTMLParserIdioms.cpp
        Tests/CyberCore/IntPoint.cpp
        Tests/CyberCore/IntRect.cpp
        Tests/CyberCore/IntSize.cpp
        Tests/CyberCore/LayoutUnit.cpp
        Tests/CyberCore/MIMETypeRegistry.cpp
        Tests/CyberCore/ParsedContentRange.cpp
        Tests/CyberCore/PublicSuffix.cpp
        Tests/CyberCore/SecurityOrigin.cpp
        Tests/CyberCore/SharedBuffer.cpp
        Tests/CyberCore/SharedBufferTest.cpp
        Tests/CyberCore/TimeRanges.cpp
        Tests/CyberCore/TransformationMatrix.cpp
        Tests/CyberCore/URLParserTextEncoding.cpp
    )

    set(TestCyberCore_LIBRARIES
        CyberCore
        CyberCoreTestSupport
        gtest
    )

    set(TestCyberCore_PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_BINARY_DIR}
        ${TESTWEBKITAPI_DIR}
        ${THIRDPARTY_DIR}/gtest/include
        ${PAL_FRAMEWORK_HEADERS_DIR}
        ${CyberCore_PRIVATE_FRAMEWORK_HEADERS_DIR}
        ${WTF_FRAMEWORK_HEADERS_DIR}
    )

    set(TestCyberCore_DEPENDENCIES CyberCorePrivateFrameworkHeaders)

    WEBKIT_EXECUTABLE_DECLARE(TestCyberCore)
endif ()

# TestCyberKitLegacy definitions
if (ENABLE_WEBKIT_LEGACY)
    set(TestCyberKitLegacy_SOURCES
        TestsController.cpp
    )

    set(TestCyberKitLegacy_LIBRARIES
        CyberKitLegacy
        gtest
    )

    set(TestCyberKitLegacy_PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_BINARY_DIR}
        ${TESTWEBKITAPI_DIR}
        ${THIRDPARTY_DIR}/gtest/include
        ${PAL_FRAMEWORK_HEADERS_DIR}
        ${CyberCore_PRIVATE_FRAMEWORK_HEADERS_DIR}
        ${WTF_FRAMEWORK_HEADERS_DIR}
        ${CyberKitLegacy_FRAMEWORK_HEADERS_DIR}
    )

    set(TestCyberKitLegacy_DEPENDENCIES CyberKitLegacyFrameworkHeaders)

    WEBKIT_EXECUTABLE_DECLARE(TestCyberKitLegacy)
endif ()

# TestCyberKit definitions
if (ENABLE_WEBKIT)
    set(TestCyberKit_SOURCES
        Tests/CyberKit/AboutBlankLoad.cpp
        Tests/CyberKit/CanHandleRequest.cpp
        Tests/CyberKit/CookieManager.cpp
        Tests/CyberKit/DOMWindowExtensionBasic.cpp
        Tests/CyberKit/DOMWindowExtensionNoCache.cpp
        Tests/CyberKit/DocumentStartUserScriptAlertCrash.cpp
        Tests/CyberKit/DownloadDecideDestinationCrash.cpp
        Tests/CyberKit/EnumerateMediaDevices.cpp
        Tests/CyberKit/EvaluateJavaScript.cpp
        Tests/CyberKit/FailedLoad.cpp
        Tests/CyberKit/Find.cpp
        Tests/CyberKit/FirstMeaningfulPaintMilestone.cpp
        Tests/CyberKit/ForceRepaint.cpp
        Tests/CyberKit/FrameMIMETypeHTML.cpp
        Tests/CyberKit/FrameMIMETypePNG.cpp
        Tests/CyberKit/Geolocation.cpp
        Tests/CyberKit/GetInjectedBundleInitializationUserDataCallback.cpp
        Tests/CyberKit/HitTestResultNodeHandle.cpp
        Tests/CyberKit/InjectedBundleBasic.cpp
        Tests/CyberKit/InjectedBundleFrameHitTest.cpp
        Tests/CyberKit/InjectedBundleInitializationUserDataCallbackWins.cpp
        Tests/CyberKit/LoadAlternateHTMLStringWithNonDirectoryURL.cpp
        Tests/CyberKit/LoadCanceledNoServerRedirectCallback.cpp
        Tests/CyberKit/LoadPageOnCrash.cpp
        Tests/CyberKit/MouseMoveAfterCrash.cpp
        Tests/CyberKit/NewFirstVisuallyNonEmptyLayout.cpp
        Tests/CyberKit/NewFirstVisuallyNonEmptyLayoutFails.cpp
        Tests/CyberKit/NewFirstVisuallyNonEmptyLayoutForImages.cpp
        Tests/CyberKit/NewFirstVisuallyNonEmptyLayoutFrames.cpp
        Tests/CyberKit/PageLoadBasic.cpp
        Tests/CyberKit/PageLoadDidChangeLocationWithinPageForFrame.cpp
        Tests/CyberKit/ParentFrame.cpp
        Tests/CyberKit/PendingAPIRequestURL.cpp
        Tests/CyberKit/PreventEmptyUserAgent.cpp
        Tests/CyberKit/PrivateBrowsingPushStateNoHistoryCallback.cpp
        Tests/CyberKit/ProvisionalURLAfterWillSendRequestCallback.cpp
        Tests/CyberKit/ReloadPageAfterCrash.cpp
        Tests/CyberKit/ResizeWindowAfterCrash.cpp
        Tests/CyberKit/RestoreSessionStateContainingFormData.cpp
        Tests/CyberKit/TextFieldDidBeginAndEndEditing.cpp
        Tests/CyberKit/UserMedia.cpp
        Tests/CyberKit/UserMessage.cpp
        Tests/CyberKit/WKPageCopySessionStateWithFiltering.cpp
        Tests/CyberKit/WKPageGetScaleFactorNotZero.cpp
        Tests/CyberKit/WKPreferences.cpp
        Tests/CyberKit/WKRetainPtr.cpp
        Tests/CyberKit/WKString.cpp
        Tests/CyberKit/WKStringJSString.cpp
        Tests/CyberKit/WKURL.cpp
        Tests/CyberKit/WillSendSubmitEvent.cpp
    )

    set(TestCyberKit_PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_BINARY_DIR}
        ${TESTWEBKITAPI_DIR}
        ${THIRDPARTY_DIR}/gtest/include
        ${PAL_FRAMEWORK_HEADERS_DIR}
        ${CyberScriptCore_PRIVATE_FRAMEWORK_HEADERS_DIR}
        ${CyberScriptCore_FRAMEWORK_HEADERS_DIR}
        ${CyberCore_PRIVATE_FRAMEWORK_HEADERS_DIR}
        ${WTF_FRAMEWORK_HEADERS_DIR}
        ${CyberKit_FRAMEWORK_HEADERS_DIR}
    )

    set(TestCyberKit_LIBRARIES
        TestCyberKitAPIBase
        CyberKit
        gtest
    )

    set(TestCyberKit_DEPENDENCIES
        NetworkProcess
        TestCyberKitAPIInjectedBundle
        WebProcess
    )

    add_library(TestCyberKitAPIBase STATIC
        JavaScriptTest.cpp
        PlatformUtilities.cpp
        TestsController.cpp
    )
    target_compile_definitions(TestCyberKitAPIBase PRIVATE BUILDING_TestCyberKit)
    target_include_directories(TestCyberKitAPIBase PRIVATE ${TestCyberKit_PRIVATE_INCLUDE_DIRECTORIES})
    target_link_libraries(TestCyberKitAPIBase PRIVATE CyberKit gtest)

    if (COMPILER_IS_GCC_OR_CLANG)
        WEBKIT_ADD_TARGET_CXX_FLAGS(TestCyberKitAPIBase -Wno-dangling-else
                                                      -Wno-sign-compare
                                                      -Wno-undef
                                                      -Wno-unused-parameter)
    endif ()

    add_library(TestCyberKitAPIInjectedBundle SHARED
        InjectedBundleController.cpp
        InjectedBundleMain.cpp
        PlatformUtilities.cpp
        Tests/CyberKit/CanHandleRequest_Bundle.cpp
        Tests/CyberKit/DidAssociateFormControls_Bundle.cpp
        Tests/CyberKit/DOMWindowExtensionBasic_Bundle.cpp
        Tests/CyberKit/DOMWindowExtensionNoCache_Bundle.cpp
        Tests/CyberKit/DocumentStartUserScriptAlertCrash_Bundle.cpp
        Tests/CyberKit/GetInjectedBundleInitializationUserDataCallback_Bundle.cpp
        Tests/CyberKit/HitTestResultNodeHandle_Bundle.cpp
        Tests/CyberKit/InjectedBundleBasic_Bundle.cpp
        Tests/CyberKit/InjectedBundleFrameHitTest_Bundle.cpp
        Tests/CyberKit/InjectedBundleInitializationUserDataCallbackWins_Bundle.cpp
        Tests/CyberKit/LoadCanceledNoServerRedirectCallback_Bundle.cpp
        Tests/CyberKit/MouseMoveAfterCrash_Bundle.cpp
        Tests/CyberKit/NewFirstVisuallyNonEmptyLayoutFails_Bundle.cpp
        Tests/CyberKit/NewFirstVisuallyNonEmptyLayoutForImages_Bundle.cpp
        Tests/CyberKit/NewFirstVisuallyNonEmptyLayoutFrames_Bundle.cpp
        Tests/CyberKit/NewFirstVisuallyNonEmptyLayout_Bundle.cpp
        Tests/CyberKit/FirstMeaningfulPaintMilestone_Bundle.cpp
        Tests/CyberKit/ParentFrame_Bundle.cpp
        Tests/CyberKit/ProvisionalURLAfterWillSendRequestCallback_Bundle.cpp
        Tests/CyberKit/ResponsivenessTimerDoesntFireEarly_Bundle.cpp
        Tests/CyberKit/TextFieldDidBeginAndEndEditing_Bundle.cpp
        Tests/CyberKit/UserMessage_Bundle.cpp
        Tests/CyberKit/WillLoad_Bundle.cpp
        Tests/CyberKit/WillSendSubmitEvent_Bundle.cpp
    )
    target_compile_definitions(TestCyberKitAPIInjectedBundle PRIVATE BUILDING_TestCyberKit)
    target_include_directories(TestCyberKitAPIInjectedBundle PRIVATE ${TestCyberKit_PRIVATE_INCLUDE_DIRECTORIES})
    target_link_libraries(TestCyberKitAPIInjectedBundle PRIVATE CyberKit)

    if (COMPILER_IS_GCC_OR_CLANG)
        WEBKIT_ADD_TARGET_CXX_FLAGS(TestCyberKitAPIInjectedBundle -Wno-dangling-else
                                                                -Wno-sign-compare
                                                                -Wno-undef
                                                                -Wno-unused-parameter)
    endif ()

    WEBKIT_EXECUTABLE_DECLARE(TestCyberKit)
endif ()

# Include platform specific files
WEBKIT_INCLUDE_CONFIG_FILES_IF_EXISTS()

# TestWTF target
WEBKIT_TEST(TestWTF)

# TestCyberCore target
if (ENABLE_WEBCORE)
    WEBKIT_TEST(TestCyberCore)
endif ()

# TestCyberKitLegacy target
if (ENABLE_WEBKIT_LEGACY)
    WEBKIT_TEST(TestCyberKitLegacy)
endif ()

# TestCyberKit target
if (ENABLE_WEBKIT)
    WEBKIT_TEST(TestCyberKit)
endif ()
