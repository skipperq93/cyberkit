find_library(APPLICATIONSERVICES_LIBRARY ApplicationServices)
find_library(QUARTZ_LIBRARY Quartz)
add_definitions(-iframework ${QUARTZ_LIBRARY}/Frameworks)
add_definitions(-iframework ${APPLICATIONSERVICES_LIBRARY}/Versions/Current/Frameworks)

list(APPEND CyberKitLegacy_PRIVATE_INCLUDE_DIRECTORIES
    "${WEBKITLEGACY_DIR}/mac"
    "${WEBKITLEGACY_DIR}/mac/Carbon"
    "${WEBKITLEGACY_DIR}/mac/DefaultDelegates"
    "${WEBKITLEGACY_DIR}/mac/DOM"
    "${WEBKITLEGACY_DIR}/mac/History"
    "${WEBKITLEGACY_DIR}/mac/icu"
    "${WEBKITLEGACY_DIR}/mac/Misc"
    "${WEBKITLEGACY_DIR}/mac/Panels"
    "${WEBKITLEGACY_DIR}/mac/Plugins"
    "${WEBKITLEGACY_DIR}/mac/Plugins/Hosted"
    "${WEBKITLEGACY_DIR}/mac/Storage"
    "${WEBKITLEGACY_DIR}/mac/CyberCoreSupport"
    "${WEBKITLEGACY_DIR}/mac/WebInspector"
    "${WEBKITLEGACY_DIR}/mac/WebView"
)

list(APPEND CyberKitLegacy_SOURCES
    cf/CyberCoreSupport/WebInspectorClientCF.cpp

    mac/DOM/DOM.mm
    mac/DOM/DOMAbstractView.mm
    mac/DOM/DOMAttr.mm
    mac/DOM/DOMBlob.mm
    mac/DOM/DOMCDATASection.mm
    mac/DOM/DOMCharacterData.mm
    mac/DOM/DOMComment.mm
    mac/DOM/DOMCounter.mm
    mac/DOM/DOMCSS.mm
    mac/DOM/DOMCSSCharsetRule.mm
    mac/DOM/DOMCSSFontFaceRule.mm
    mac/DOM/DOMCSSImportRule.mm
    mac/DOM/DOMCSSMediaRule.mm
    mac/DOM/DOMCSSPageRule.mm
    mac/DOM/DOMCSSPrimitiveValue.mm
    mac/DOM/DOMCSSRule.mm
    mac/DOM/DOMCSSRuleList.mm
    mac/DOM/DOMCSSStyleDeclaration.mm
    mac/DOM/DOMCSSStyleRule.mm
    mac/DOM/DOMCSSStyleSheet.mm
    mac/DOM/DOMCSSUnknownRule.mm
    mac/DOM/DOMCSSValue.mm
    mac/DOM/DOMCSSValueList.mm
    mac/DOM/DOMCustomXPathNSResolver.mm
    mac/DOM/DOMDocument.mm
    mac/DOM/DOMDocumentFragment.mm
    mac/DOM/DOMDocumentType.mm
    mac/DOM/DOMElement.mm
    mac/DOM/DOMEntityReference.mm
    mac/DOM/DOMEvent.mm
    mac/DOM/DOMEvents.mm
    mac/DOM/DOMFile.mm
    mac/DOM/DOMFileList.mm
    mac/DOM/DOMHTML.mm
    mac/DOM/DOMHTMLAnchorElement.mm
    mac/DOM/DOMHTMLAppletElement.mm
    mac/DOM/DOMHTMLAreaElement.mm
    mac/DOM/DOMHTMLBRElement.mm
    mac/DOM/DOMHTMLBaseElement.mm
    mac/DOM/DOMHTMLBaseFontElement.mm
    mac/DOM/DOMHTMLBodyElement.mm
    mac/DOM/DOMHTMLButtonElement.mm
    mac/DOM/DOMHTMLCanvasElement.mm
    mac/DOM/DOMHTMLCollection.mm
    mac/DOM/DOMHTMLDListElement.mm
    mac/DOM/DOMHTMLDirectoryElement.mm
    mac/DOM/DOMHTMLDivElement.mm
    mac/DOM/DOMHTMLDocument.mm
    mac/DOM/DOMHTMLElement.mm
    mac/DOM/DOMHTMLEmbedElement.mm
    mac/DOM/DOMHTMLFieldSetElement.mm
    mac/DOM/DOMHTMLFontElement.mm
    mac/DOM/DOMHTMLFormElement.mm
    mac/DOM/DOMHTMLFrameElement.mm
    mac/DOM/DOMHTMLFrameSetElement.mm
    mac/DOM/DOMHTMLHRElement.mm
    mac/DOM/DOMHTMLHeadElement.mm
    mac/DOM/DOMHTMLHeadingElement.mm
    mac/DOM/DOMHTMLHtmlElement.mm
    mac/DOM/DOMHTMLIFrameElement.mm
    mac/DOM/DOMHTMLImageElement.mm
    mac/DOM/DOMHTMLInputElement.mm
    mac/DOM/DOMHTMLLIElement.mm
    mac/DOM/DOMHTMLLabelElement.mm
    mac/DOM/DOMHTMLLegendElement.mm
    mac/DOM/DOMHTMLLinkElement.mm
    mac/DOM/DOMHTMLMapElement.mm
    mac/DOM/DOMHTMLMarqueeElement.mm
    mac/DOM/DOMHTMLMediaElement.mm
    mac/DOM/DOMHTMLMenuElement.mm
    mac/DOM/DOMHTMLMetaElement.mm
    mac/DOM/DOMHTMLModElement.mm
    mac/DOM/DOMHTMLOListElement.mm
    mac/DOM/DOMHTMLObjectElement.mm
    mac/DOM/DOMHTMLOptGroupElement.mm
    mac/DOM/DOMHTMLOptionElement.mm
    mac/DOM/DOMHTMLOptionsCollection.mm
    mac/DOM/DOMHTMLParagraphElement.mm
    mac/DOM/DOMHTMLParamElement.mm
    mac/DOM/DOMHTMLPreElement.mm
    mac/DOM/DOMHTMLQuoteElement.mm
    mac/DOM/DOMHTMLScriptElement.mm
    mac/DOM/DOMHTMLSelectElement.mm
    mac/DOM/DOMHTMLStyleElement.mm
    mac/DOM/DOMHTMLTableCaptionElement.mm
    mac/DOM/DOMHTMLTableCellElement.mm
    mac/DOM/DOMHTMLTableColElement.mm
    mac/DOM/DOMHTMLTableElement.mm
    mac/DOM/DOMHTMLTableRowElement.mm
    mac/DOM/DOMHTMLTableSectionElement.mm
    mac/DOM/DOMHTMLTextAreaElement.mm
    mac/DOM/DOMHTMLTitleElement.mm
    mac/DOM/DOMHTMLUListElement.mm
    mac/DOM/DOMHTMLVideoElement.mm
    mac/DOM/DOMInternal.mm
    mac/DOM/DOMImplementation.mm
    mac/DOM/DOMKeyboardEvent.mm
    mac/DOM/DOMMediaError.mm
    mac/DOM/DOMMediaList.mm
    mac/DOM/DOMMouseEvent.mm
    mac/DOM/DOMMutationEvent.mm
    mac/DOM/DOMNamedNodeMap.mm
    mac/DOM/DOMNode.mm
    mac/DOM/DOMNodeIterator.mm
    mac/DOM/DOMNodeList.mm
    mac/DOM/DOMObject.mm
    mac/DOM/DOMOverflowEvent.mm
    mac/DOM/DOMProcessingInstruction.mm
    mac/DOM/DOMProgressEvent.mm
    mac/DOM/DOMRGBColor.mm
    mac/DOM/DOMRange.mm
    mac/DOM/DOMRect.mm
    mac/DOM/DOMStyleSheet.mm
    mac/DOM/DOMStyleSheetList.mm
    mac/DOM/DOMText.mm
    mac/DOM/DOMTextEvent.mm
    mac/DOM/DOMTimeRanges.mm
    mac/DOM/DOMTokenList.mm
    mac/DOM/DOMTreeWalker.mm
    mac/DOM/DOMUIEvent.mm
    mac/DOM/DOMUIKitExtensions.mm
    mac/DOM/DOMUtility.mm
    mac/DOM/DOMWheelEvent.mm
    mac/DOM/DOMXPath.mm
    mac/DOM/DOMXPathExpression.mm
    mac/DOM/DOMXPathResult.mm
    mac/DOM/ExceptionHandlers.mm
    mac/DOM/ObjCEventListener.mm
    mac/DOM/ObjCNodeFilterCondition.mm

    mac/DefaultDelegates/WebDefaultContextMenuDelegate.mm
    mac/DefaultDelegates/WebDefaultEditingDelegate.m
    mac/DefaultDelegates/WebDefaultPolicyDelegate.m
    mac/DefaultDelegates/WebDefaultUIDelegate.mm

    mac/History/BackForwardList.mm
    mac/History/BinaryPropertyList.cpp
    mac/History/HistoryPropertyList.mm
    mac/History/WebBackForwardList.mm
    mac/History/WebHistory.mm
    mac/History/WebHistoryItem.mm
    mac/History/WebURLsWithTitles.m

    mac/Misc/WebCache.mm
    mac/Misc/CyberCoreStatistics.mm
    mac/Misc/WebDownload.mm
    mac/Misc/WebElementDictionary.mm
    mac/Misc/WebIconDatabase.mm
    mac/Misc/CyberKitErrors.m
    mac/Misc/CyberKitLogging.m
    mac/Misc/CyberKitNSStringExtras.mm
    mac/Misc/CyberKitStatistics.m
    mac/Misc/CyberKitVersionChecks.mm
    mac/Misc/WebLocalizableStrings.mm
    mac/Misc/WebLocalizableStringsInternal.mm
    mac/Misc/WebNSControlExtras.m
    mac/Misc/WebNSDataExtras.mm
    mac/Misc/WebNSDictionaryExtras.m
    mac/Misc/WebNSEventExtras.m
    mac/Misc/WebNSFileManagerExtras.mm
    mac/Misc/WebNSImageExtras.m
    mac/Misc/WebNSObjectExtras.mm
    mac/Misc/WebNSPasteboardExtras.mm
    mac/Misc/WebNSPrintOperationExtras.m
    mac/Misc/WebNSURLExtras.mm
    mac/Misc/WebNSURLRequestExtras.m
    mac/Misc/WebNSUserDefaultsExtras.mm
    mac/Misc/WebNSViewExtras.m
    mac/Misc/WebNSWindowExtras.m
    mac/Misc/WebSharingServicePickerController.mm
    mac/Misc/WebStringTruncator.mm
    mac/Misc/WebUserContentURLPattern.mm

    mac/Panels/WebAuthenticationPanel.m
    mac/Panels/WebPanelAuthenticationHandler.m

    mac/Plugins/WebBaseNetscapePluginView.mm
    mac/Plugins/WebBasePluginPackage.mm
    mac/Plugins/WebNetscapePluginEventHandler.mm
    mac/Plugins/WebNetscapePluginEventHandlerCarbon.mm
    mac/Plugins/WebNetscapePluginEventHandlerCocoa.mm
    mac/Plugins/WebNetscapePluginPackage.mm
    mac/Plugins/WebNetscapePluginStream.mm
    mac/Plugins/WebNetscapePluginView.mm
    mac/Plugins/WebPluginContainerCheck.mm
    mac/Plugins/WebPluginController.mm
    mac/Plugins/WebPluginDatabase.mm
    mac/Plugins/WebPluginPackage.mm
    mac/Plugins/WebPluginRequest.m
    mac/Plugins/npapi.mm

    mac/Plugins/Hosted/HostedNetscapePluginStream.mm
    mac/Plugins/Hosted/NetscapePluginHostManager.mm
    mac/Plugins/Hosted/NetscapePluginHostProxy.mm
    mac/Plugins/Hosted/NetscapePluginInstanceProxy.mm
    mac/Plugins/Hosted/ProxyInstance.mm
    mac/Plugins/Hosted/ProxyRuntimeObject.mm
    mac/Plugins/Hosted/WebHostedNetscapePluginView.mm
    mac/Plugins/Hosted/CyberKitPluginAgent.defs
    mac/Plugins/Hosted/CyberKitPluginAgentReply.defs
    mac/Plugins/Hosted/CyberKitPluginClient.defs
    mac/Plugins/Hosted/CyberKitPluginHost.defs
    mac/Plugins/Hosted/CyberKitPluginHostTypes.defs
    mac/Plugins/Hosted/WebTextInputWindowController.m

    mac/Storage/WebDatabaseManager.mm
    mac/Storage/WebDatabaseManagerClient.mm
    mac/Storage/WebDatabaseProvider.mm
    mac/Storage/WebDatabaseQuotaManager.mm
    mac/Storage/WebStorageManager.mm
    mac/Storage/WebStorageTrackerClient.mm

    mac/CyberCoreSupport/CorrectionPanel.mm
    mac/CyberCoreSupport/PopupMenuMac.mm
    mac/CyberCoreSupport/SearchPopupMenuMac.mm
    mac/CyberCoreSupport/WebAlternativeTextClient.mm
    mac/CyberCoreSupport/WebApplicationCache.mm
    mac/CyberCoreSupport/WebApplicationCacheQuotaManager.mm
    mac/CyberCoreSupport/WebChromeClient.mm
    mac/CyberCoreSupport/WebContextMenuClient.mm
    mac/CyberCoreSupport/WebDeviceOrientationClient.mm
    mac/CyberCoreSupport/WebDragClient.mm
    mac/CyberCoreSupport/WebEditorClient.mm
    mac/CyberCoreSupport/WebFrameLoaderClient.mm
    mac/CyberCoreSupport/WebFrameNetworkingContext.mm
    mac/CyberCoreSupport/WebGeolocationClient.mm
    mac/CyberCoreSupport/WebInspectorClient.mm
    mac/CyberCoreSupport/WebJavaScriptTextInputPanel.m
    mac/CyberCoreSupport/CyberKitFullScreenListener.mm
    mac/CyberCoreSupport/WebNotificationClient.mm
    mac/CyberCoreSupport/WebOpenPanelResultListener.mm
    mac/CyberCoreSupport/WebPlatformStrategies.mm
    mac/CyberCoreSupport/WebPluginInfoProvider.mm
    mac/CyberCoreSupport/WebProgressTrackerClient.mm
    mac/CyberCoreSupport/WebSecurityOrigin.mm
    mac/CyberCoreSupport/WebSelectionServiceController.mm
    mac/CyberCoreSupport/WebValidationMessageClient.mm
    mac/CyberCoreSupport/WebVisitedLinkStore.mm

    mac/WebInspector/WebInspector.mm
    mac/WebInspector/WebInspectorFrontend.mm
    mac/WebInspector/WebNodeHighlight.mm
    mac/WebInspector/WebNodeHighlightView.mm
    mac/WebInspector/WebNodeHighlighter.mm

    mac/WebView/WebArchive.mm
    mac/WebView/WebClipView.mm
    mac/WebView/WebDashboardRegion.mm
    mac/WebView/WebDataSource.mm
    mac/WebView/WebDelegateImplementationCaching.mm
    mac/WebView/WebDeviceOrientation.mm
    mac/WebView/WebDeviceOrientationProviderMock.mm
    mac/WebView/WebDocumentLoaderMac.mm
    mac/WebView/WebDynamicScrollBarsView.mm
    mac/WebView/WebFormDelegate.m
    mac/WebView/WebFrame.mm
    mac/WebView/WebFrameView.mm
    mac/WebView/WebFullScreenController.mm
    mac/WebView/WebGeolocationPosition.mm
    mac/WebView/WebHTMLRepresentation.mm
    mac/WebView/WebHTMLView.mm
    mac/WebView/WebImmediateActionController.mm
    mac/WebView/WebIndicateLayer.mm
    mac/WebView/WebJSPDFDoc.mm
    mac/WebView/WebNavigationData.mm
    mac/WebView/WebNotification.mm
    mac/WebView/WebPDFDocumentExtras.mm
    mac/WebView/WebPDFRepresentation.mm
    mac/WebView/WebPDFView.mm
    mac/WebView/WebPolicyDelegate.mm
    mac/WebView/WebPreferences.mm
    mac/WebView/WebResource.mm
    mac/WebView/WebScriptDebugDelegate.mm
    mac/WebView/WebScriptDebugger.mm
    mac/WebView/WebScriptWorld.mm
    mac/WebView/WebTextCompletionController.mm
    mac/WebView/WebTextIterator.mm
    mac/WebView/WebView.mm
    mac/WebView/WebViewData.mm
)

set(CyberKitLegacy_FORWARDING_HEADERS_DIRECTORIES
    mac/DOM
    mac/DefaultDelegates
    mac/History
    mac/Misc
    mac/Panels
    mac/Plugins
    mac/Storage
    mac/CyberCoreSupport
    mac/WebInspector
    mac/WebView
)

set(CyberKitLegacy_FORWARDING_HEADERS_FILES
    mac/DOM/WebDOMOperations.h

    mac/History/WebHistory.h
    mac/History/WebHistoryItem.h

    mac/Misc/WebNSURLExtras.h

    mac/Panels/WebPanelAuthenticationHandler.h

    mac/Plugins/WebBasePluginPackage.h

    mac/Storage/WebDatabaseManagerPrivate.h

    mac/WebInspector/WebInspector.h

    mac/WebView/WebFrame.h
    mac/WebView/WebView.h

    ${WEBCORE_DIR}/plugins/npfunctions.h
)

add_definitions("-include CyberKitPrefix.h")

set(C99_FILES
    ${WEBKITLEGACY_DIR}/mac/Carbon/CarbonUtils.m
    ${WEBKITLEGACY_DIR}/mac/Carbon/CarbonWindowContentView.m
    ${WEBKITLEGACY_DIR}/mac/Carbon/CarbonWindowFrame.m
    ${WEBKITLEGACY_DIR}/mac/Carbon/HIViewAdapter.m

    mac/DefaultDelegates/WebDefaultEditingDelegate.m
    mac/DefaultDelegates/WebDefaultPolicyDelegate.m
    mac/DefaultDelegates/WebDefaultUIDelegate.m

    mac/Misc/CyberKitErrors.m
    mac/Misc/CyberKitLogging.m
    mac/Misc/CyberKitStatistics.m
    mac/Misc/CyberKitSystemBits.m
    mac/Misc/WebNSArrayExtras.m
    mac/Misc/WebNSControlExtras.m
    mac/Misc/WebNSDictionaryExtras.m
    mac/Misc/WebNSEventExtras.m
    mac/Misc/WebNSImageExtras.m
    mac/Misc/WebNSPrintOperationExtras.m
    mac/Misc/WebNSURLRequestExtras.m
    mac/Misc/WebNSViewExtras.m
    mac/Misc/WebNSWindowExtras.m

    mac/Panels/WebAuthenticationPanel.m
    mac/Panels/WebPanelAuthenticationHandler.m

    mac/Plugins/WebPluginRequest.m
    mac/Plugins/WebPluginsPrivate.m

    mac/Plugins/Hosted/WebTextInputWindowController.m

    mac/CyberCoreSupport/WebJavaScriptTextInputPanel.m

    mac/WebView/WebFormDelegate.m
)

foreach (_file ${CyberKitLegacy_SOURCES})
    list(FIND C99_FILES ${_file} _c99_index)
    if (${_c99_index} EQUAL -1)
        set_source_files_properties(${_file} PROPERTIES COMPILE_FLAGS "-ObjC++ -std=c++17")
    else ()
        set_source_files_properties(${_file} PROPERTIES COMPILE_FLAGS -std=c99)
    endif ()
endforeach ()

file(COPY
    mac/Plugins/Hosted/CyberKitPluginAgent.defs
    mac/Plugins/Hosted/CyberKitPluginAgentReply.defs
    mac/Plugins/Hosted/CyberKitPluginClient.defs
    mac/Plugins/Hosted/CyberKitPluginHost.defs
    mac/Plugins/Hosted/CyberKitPluginHostTypes.defs
    mac/Plugins/Hosted/CyberKitPluginHostTypes.h
DESTINATION ${CyberKitLegacy_DERIVED_SOURCES_DIR})

add_custom_command(
    OUTPUT
        ${CyberKitLegacy_DERIVED_SOURCES_DIR}/CyberKitPluginAgentReplyServer.c
        ${CyberKitLegacy_DERIVED_SOURCES_DIR}/CyberKitPluginAgentReplyUser.c
        ${CyberKitLegacy_DERIVED_SOURCES_DIR}/CyberKitPluginAgentServer.c
        ${CyberKitLegacy_DERIVED_SOURCES_DIR}/CyberKitPluginAgentUser.c
        ${CyberKitLegacy_DERIVED_SOURCES_DIR}/CyberKitPluginHostServer.c
        ${CyberKitLegacy_DERIVED_SOURCES_DIR}/CyberKitPluginHostUser.c
    DEPENDS mac/Plugins/Hosted/CyberKitPluginAgent.defs mac/Plugins/Hosted/CyberKitPluginHost.defs
    WORKING_DIRECTORY ${CyberKitLegacy_DERIVED_SOURCES_DIR}
    COMMAND mig -I.. CyberKitPluginAgent.defs CyberKitPluginAgentReply.defs CyberKitPluginHost.defs
    VERBATIM)
add_custom_command(
    OUTPUT
        ${CyberKitLegacy_DERIVED_SOURCES_DIR}/CyberKitPluginClientServer.c
        ${CyberKitLegacy_DERIVED_SOURCES_DIR}/CyberKitPluginClientUser.c
    DEPENDS mac/Plugins/Hosted/CyberKitPluginClient.defs
    WORKING_DIRECTORY ${CyberKitLegacy_DERIVED_SOURCES_DIR}
    COMMAND mig -I.. -sheader CyberKitPluginClientServer.h CyberKitPluginClient.defs
    VERBATIM)
list(APPEND CyberKitLegacy_SOURCES
    ${CyberKitLegacy_DERIVED_SOURCES_DIR}/CyberKitPluginAgentUser.c
    ${CyberKitLegacy_DERIVED_SOURCES_DIR}/CyberKitPluginClientServer.c
    ${CyberKitLegacy_DERIVED_SOURCES_DIR}/CyberKitPluginHostUser.c
)

WEBKIT_MAKE_FORWARDING_HEADERS(CyberKitLegacy
    TARGET_NAME CyberKitLegacyFrameworkHeaders
    DESTINATION ${CyberKitLegacy_FRAMEWORK_HEADERS_DIR}/CyberKitLegacy
    FILES ${CyberKitLegacy_FORWARDING_HEADERS_FILES}
    FLATTENED
)
add_dependencies(CyberKitLegacyFrameworkHeaders CyberCorePrivateFrameworkHeaders)

set(CyberKitLegacy_OUTPUT_NAME CyberKitLegacy)

set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "-compatibility_version 1 -current_version ${WEBKIT_MAC_VERSION}")
