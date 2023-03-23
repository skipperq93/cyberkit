set(TESTWEBKITAPI_DIR "${TOOLS_DIR}/TestCyberKitAPI")
set(TestCyberKitAPI_DISABLED_WARNINGS
    -Wno-dangling-else
    -Wno-sign-compare
    -Wno-undef
    -Wno-unused-parameter
)

macro(WEBKIT_TEST _target)
    WEBKIT_EXECUTABLE(${_target})
    add_test(${_target} ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}/${_target})
    set_tests_properties(${_target} PROPERTIES TIMEOUT 60)
    set_target_properties(${_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TESTWEBKITAPI_RUNTIME_OUTPUT_DIRECTORY}
    )

    if (COMPILER_IS_GCC_OR_CLANG)
        WEBKIT_ADD_TARGET_CXX_FLAGS(${_target} ${TestCyberKitAPI_DISABLED_WARNINGS})
    endif ()
endmacro()

# TestWTF definitions
set(TestWTF_SOURCES
    Counters.cpp
    TestsController.cpp
    Utilities.cpp

    Tests/WTF/AtomString.cpp
    Tests/WTF/Bitmap.cpp
    Tests/WTF/BloomFilter.cpp
    Tests/WTF/BoxPtr.cpp
    Tests/WTF/BumpPointerAllocator.cpp
    Tests/WTF/CString.cpp
    Tests/WTF/CheckedArithmeticOperations.cpp
    Tests/WTF/CompactPtr.cpp
    Tests/WTF/CompactRefPtr.cpp
    Tests/WTF/CompactRefPtrTuple.cpp
    Tests/WTF/CompactUniquePtrTuple.cpp
    Tests/WTF/CompletionHandlerTests.cpp
    Tests/WTF/ConcurrentPtrHashSet.cpp
    Tests/WTF/Condition.cpp
    Tests/WTF/CrossThreadCopierTests.cpp
    Tests/WTF/CrossThreadTask.cpp
    Tests/WTF/DataMutex.cpp
    Tests/WTF/DateMath.cpp
    Tests/WTF/Deque.cpp
    Tests/WTF/EmbeddedFixedVector.cpp
    Tests/WTF/EnumTraits.cpp
    Tests/WTF/Expected.cpp
    Tests/WTF/FileSystem.cpp
    Tests/WTF/FixedBitVector.cpp
    Tests/WTF/FixedVector.cpp
    Tests/WTF/Function.cpp
    Tests/WTF/HashCountedSet.cpp
    Tests/WTF/HashMap.cpp
    Tests/WTF/HashSet.cpp
    Tests/WTF/Hasher.cpp
    Tests/WTF/HexNumber.cpp
    Tests/WTF/IndexSparseSet.cpp
    Tests/WTF/Int128.cpp
    Tests/WTF/IntegerToStringConversion.cpp
    Tests/WTF/IteratorRange.cpp
    Tests/WTF/JSONValue.cpp
    Tests/WTF/LEBDecoder.cpp
    Tests/WTF/LifecycleLogger.cpp
    Tests/WTF/LineEnding.cpp
    Tests/WTF/ListHashSet.cpp
    Tests/WTF/Lock.cpp
    Tests/WTF/Logger.cpp
    Tests/WTF/Markable.cpp
    Tests/WTF/MathExtras.cpp
    Tests/WTF/MediaTime.cpp
    Tests/WTF/MetaAllocator.cpp
    Tests/WTF/MoveOnlyLifecycleLogger.cpp
    Tests/WTF/NakedPtr.cpp
    Tests/WTF/NeverDestroyed.cpp
    Tests/WTF/OptionSet.cpp
    Tests/WTF/Packed.cpp
    Tests/WTF/PackedRef.cpp
    Tests/WTF/PackedRefPtr.cpp
    Tests/WTF/ParkingLot.cpp
    Tests/WTF/PriorityQueue.cpp
    Tests/WTF/RedBlackTree.cpp
    Tests/WTF/Ref.cpp
    Tests/WTF/RefCountedFixedVector.cpp
    Tests/WTF/RefCounter.cpp
    Tests/WTF/RefLogger.cpp
    Tests/WTF/RefPtr.cpp
    Tests/WTF/RobinHoodHashMap.cpp
    Tests/WTF/RobinHoodHashSet.cpp
    Tests/WTF/RunLoop.cpp
    Tests/WTF/SHA1.cpp
    Tests/WTF/SafeStrerror.cpp
    Tests/WTF/SaturatedArithmeticOperations.cpp
    Tests/WTF/Scope.cpp
    Tests/WTF/ScopedLambda.cpp
    Tests/WTF/SentinelLinkedList.cpp
    Tests/WTF/SetForScope.cpp
    Tests/WTF/Span.cpp
    Tests/WTF/StackTraceTest.cpp
    Tests/WTF/StdLibExtras.cpp
    Tests/WTF/StringBuilder.cpp
    Tests/WTF/StringCommon.cpp
    Tests/WTF/StringConcatenate.cpp
    Tests/WTF/StringHasher.cpp
    Tests/WTF/StringImpl.cpp
    Tests/WTF/StringOperators.cpp
    Tests/WTF/StringParsingBuffer.cpp
    Tests/WTF/StringSearch.cpp
    Tests/WTF/StringToIntegerConversion.cpp
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
    Tests/WTF/UniqueRefVector.cpp
    Tests/WTF/Vector.cpp
    Tests/WTF/WTFString.cpp
    Tests/WTF/WeakPtr.cpp
    Tests/WTF/WorkQueue.cpp
    Tests/WTF/WorkerPool.cpp
)

set(TestWTF_PRIVATE_INCLUDE_DIRECTORIES
    ${CMAKE_BINARY_DIR}
    ${TESTWEBKITAPI_DIR}
)

set(TestWTF_LIBRARIES
    CyberKit::gtest
)

set(TestWTF_FRAMEWORKS
    WTF
)
if (NOT USE_SYSTEM_MALLOC)
    list(APPEND TestWTF_FRAMEWORKS bmalloc)
endif ()

WEBKIT_EXECUTABLE_DECLARE(TestWTF)

# TestJavaScriptCore definitions
if (ENABLE_JAVASCRIPTCORE)
    set(TestJavaScriptCore_SOURCES
        TestsController.cpp

        Tests/JavaScriptCore/DisallowVMEntry.cpp
        Tests/JavaScriptCore/InspectorConsoleMessage.cpp
        Tests/JavaScriptCore/PropertySlot.cpp
    )

    set(TestJavaScriptCore_LIBRARIES
        CyberKit::gtest
    )
    set(TestJavaScriptCore_FRAMEWORKS
        JavaScriptCore
        WTF
    )
    if (NOT USE_SYSTEM_MALLOC)
        list(APPEND TestJavaScriptCore_FRAMEWORKS bmalloc)
    endif ()

    set(TestJavaScriptCore_PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_BINARY_DIR}
        ${TESTWEBKITAPI_DIR}
    )

    WEBKIT_EXECUTABLE_DECLARE(TestJavaScriptCore)
endif ()

# TestCyberCore definitions
if (ENABLE_WEBCORE)
    set(TestCyberCore_SOURCES
        TestsController.cpp
        Utilities.cpp

        Tests/CyberCore/AffineTransform.cpp
        Tests/CyberCore/CSSParser.cpp
        Tests/CyberCore/CalculationValue.cpp
        Tests/CyberCore/ColorTests.cpp
        Tests/CyberCore/ComplexTextController.cpp
        Tests/CyberCore/ContextMenuAction.cpp
        Tests/CyberCore/FileMonitor.cpp
        Tests/CyberCore/FloatPointTests.cpp
        Tests/CyberCore/FloatRectTests.cpp
        Tests/CyberCore/FloatSizeTests.cpp
        Tests/CyberCore/GridPosition.cpp
        Tests/CyberCore/HTMLParserIdioms.cpp
        Tests/CyberCore/HTTPParsers.cpp
        Tests/CyberCore/IntPointTests.cpp
        Tests/CyberCore/IntRectTests.cpp
        Tests/CyberCore/IntSizeTests.cpp
        Tests/CyberCore/KeyedCoding.cpp
        Tests/CyberCore/LayoutUnitTests.cpp
        Tests/CyberCore/MIMETypeRegistry.cpp
        Tests/CyberCore/NowPlayingInfoTests.cpp
        Tests/CyberCore/ParsedContentRange.cpp
        Tests/CyberCore/PublicSuffix.cpp
        Tests/CyberCore/SecurityOrigin.cpp
        Tests/CyberCore/SharedBuffer.cpp
        Tests/CyberCore/SharedBufferTest.cpp
        Tests/CyberCore/TimeRanges.cpp
        Tests/CyberCore/TransformationMatrix.cpp
        Tests/CyberCore/URLParserTextEncoding.cpp
        Tests/CyberCore/WritingModeTests.cpp
    )

    set(TestCyberCore_LIBRARIES
        CyberKit::gtest
    )
    set(TestCyberCore_FRAMEWORKS
        JavaScriptCore
        PAL
        WTF
        CyberCore
    )
    if (NOT USE_SYSTEM_MALLOC)
        list(APPEND TestCyberCore_FRAMEWORKS bmalloc)
    endif ()

    set(TestCyberCore_PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_BINARY_DIR}
        ${TESTWEBKITAPI_DIR}
    )

    set(TestCyberCore_DEFINITIONS
      WEBKIT_SRC_DIR="${CMAKE_SOURCE_DIR}"
    )

    WEBKIT_EXECUTABLE_DECLARE(TestCyberCore)
    target_compile_definitions(TestCyberCore PUBLIC TestCyberCore_DEFINITIONS)
endif ()

# TestCyberKitLegacy definitions
if (ENABLE_WEBKIT_LEGACY)
    set(TestCyberKitLegacy_SOURCES
        TestsController.cpp
    )

    set(TestCyberKitLegacy_LIBRARIES
        CyberKit::gtest
    )

    set(TestCyberKitLegacy_FRAMEWORKS
        JavaScriptCore
        PAL
        WTF
        CyberCore
    )
    if (NOT USE_SYSTEM_MALLOC)
        list(APPEND TestCyberKitLegacy_FRAMEWORKS bmalloc)
    endif ()
    if (NOT APPLE)
        list(APPEND TestCyberKitLegacy_FRAMEWORKS CyberKitLegacy)
    endif ()

    set(TestCyberKitLegacy_PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_BINARY_DIR}
        ${TESTWEBKITAPI_DIR}
    )

    WEBKIT_EXECUTABLE_DECLARE(TestCyberKitLegacy)
endif ()

# TestCyberKit definitions
if (ENABLE_WEBKIT)
    set(TestCyberKit_SOURCES
        Utilities.cpp

        Tests/CyberKit/AboutBlankLoad.cpp
        Tests/CyberKit/CanHandleRequest.cpp
        Tests/CyberKit/DOMWindowExtensionBasic.cpp
        Tests/CyberKit/DOMWindowExtensionNoCache.cpp
        Tests/CyberKit/DidNotHandleKeyDown.cpp
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
        Tests/CyberKit/PageLoadState.cpp
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
        ${JavaScriptCore_PRIVATE_FRAMEWORK_HEADERS_DIR}
        ${PAL_FRAMEWORK_HEADERS_DIR}
        ${TESTWEBKITAPI_DIR}
        ${CyberCore_PRIVATE_FRAMEWORK_HEADERS_DIR}
        ${CyberKit_FRAMEWORK_HEADERS_DIR}
    )

    set(TestCyberKit_LIBRARIES
        TestCyberKitAPIBase
        CyberKit::gtest
    )
    set(TestCyberKit_FRAMEWORKS
        CyberKit
    )

    set(TestCyberKit_DEPENDENCIES
        TestCyberKitAPIInjectedBundle
    )

    add_library(TestCyberKitAPIBase STATIC
        JavaScriptTest.cpp
        PlatformUtilities.cpp
        TestsController.cpp
    )
    target_compile_definitions(TestCyberKitAPIBase PRIVATE BUILDING_TestCyberKit)
    target_include_directories(TestCyberKitAPIBase PRIVATE ${TestCyberKit_PRIVATE_INCLUDE_DIRECTORIES})
    target_link_libraries(TestCyberKitAPIBase PRIVATE CyberKit::CyberKit CyberKit::gtest)

    if (COMPILER_IS_GCC_OR_CLANG)
        WEBKIT_ADD_TARGET_CXX_FLAGS(TestCyberKitAPIBase ${TestCyberKitAPI_DISABLED_WARNINGS} -Wno-deprecated-declarations)
    endif ()

    set(TestCyberKitAPIInjectedBundle_SOURCES
        InjectedBundleController.cpp
        InjectedBundleMain.cpp
        PlatformUtilities.cpp
        Utilities.cpp

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
    if (APPLE)
        list(APPEND TestCyberKitAPIInjectedBundle_SOURCES
            Tests/CyberKit/DOMWindowExtensionBasic_Bundle.cpp

            cocoa/UtilitiesCocoa.mm

            mac/InjectedBundleControllerMac.mm
            mac/PlatformUtilitiesMac.mm
        )
    endif ()
    add_library(TestCyberKitAPIInjectedBundle SHARED ${TestCyberKitAPIInjectedBundle_SOURCES})

    target_compile_definitions(TestCyberKitAPIInjectedBundle PRIVATE BUILDING_TestCyberKit)
    target_include_directories(TestCyberKitAPIInjectedBundle PRIVATE ${TestCyberKit_PRIVATE_INCLUDE_DIRECTORIES})
    target_link_libraries(TestCyberKitAPIInjectedBundle PRIVATE CyberKit::CyberKit CyberKit::gtest)

    if (COMPILER_IS_GCC_OR_CLANG)
        WEBKIT_ADD_TARGET_CXX_FLAGS(TestCyberKitAPIInjectedBundle ${TestCyberKitAPI_DISABLED_WARNINGS})
    endif ()

    WEBKIT_EXECUTABLE_DECLARE(TestCyberKit)
endif ()

# Include platform specific files
WEBKIT_INCLUDE_CONFIG_FILES_IF_EXISTS()

# TestWTF target
WEBKIT_TEST(TestWTF)

# TestJavaScriptCore target
if (ENABLE_JAVASCRIPTCORE)
    WEBKIT_TEST(TestJavaScriptCore)
endif ()

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
