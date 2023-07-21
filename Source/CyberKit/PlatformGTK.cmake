include(InspectorGResources.cmake)

set(CyberKit_OUTPUT_NAME webkit2gtk-${WEBKITGTK_API_VERSION})
set(CyberKit_WebProcess_OUTPUT_NAME CyberKitWebProcess)
set(CyberKit_NetworkProcess_OUTPUT_NAME CyberKitNetworkProcess)
set(CyberKit_PluginProcess_OUTPUT_NAME CyberKitPluginProcess)

file(MAKE_DIRECTORY ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR})
file(MAKE_DIRECTORY ${FORWARDING_HEADERS_WEBKIT2GTK_DIR})
file(MAKE_DIRECTORY ${FORWARDING_HEADERS_WEBKIT2GTK_EXTENSION_DIR})

configure_file(UIProcess/API/gtk/CyberKitVersion.h.in ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitVersion.h)
configure_file(gtk/webkit2gtk.pc.in ${CyberKit2_PKGCONFIG_FILE} @ONLY)
configure_file(gtk/webkit2gtk-web-extension.pc.in ${CyberKit2WebExtension_PKGCONFIG_FILE} @ONLY)

add_definitions(-DBUILDING_WEBKIT)
add_definitions(-DWEBKIT2_COMPILATION)
add_definitions(-DWEBKIT_DOM_USE_UNSTABLE_API)

add_definitions(-DPKGLIBEXECDIR="${LIBEXEC_INSTALL_DIR}")
add_definitions(-DLOCALEDIR="${CMAKE_INSTALL_FULL_LOCALEDIR}")
add_definitions(-DDATADIR="${CMAKE_INSTALL_FULL_DATADIR}")
add_definitions(-DLIBDIR="${LIB_INSTALL_DIR}")

if (NOT DEVELOPER_MODE AND NOT CMAKE_SYSTEM_NAME MATCHES "Darwin")
    WEBKIT_ADD_TARGET_PROPERTIES(CyberKit LINK_FLAGS "-Wl,--version-script,${CMAKE_CURRENT_SOURCE_DIR}/webkitglib-symbols.map")
endif ()

set(CyberKit_USE_PREFIX_HEADER ON)

list(APPEND CyberKit_UNIFIED_SOURCE_LIST_FILES
    "SourcesGTK.txt"
)

list(APPEND CyberKit_MESSAGES_IN_FILES
    NetworkProcess/CustomProtocols/LegacyCustomProtocolManager.messages.in

    UIProcess/ViewGestureController.messages.in

    UIProcess/Network/CustomProtocols/LegacyCustomProtocolManagerProxy.messages.in

    WebProcess/WebPage/ViewGestureGeometryCollector.messages.in
)

list(APPEND CyberKit_DERIVED_SOURCES
    ${DERIVED_SOURCES_WEBKIT2GTK_DIR}/InspectorGResourceBundle.c
    ${DERIVED_SOURCES_WEBKIT2GTK_DIR}/CyberKitResourcesGResourceBundle.c

    ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitEnumTypes.cpp
    ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitWebProcessEnumTypes.cpp
)

if (ENABLE_WAYLAND_TARGET)
    list(APPEND CyberKit_DERIVED_SOURCES
        ${DERIVED_SOURCES_WEBKIT2GTK_DIR}/CyberKitWaylandClientProtocol.c
    )
endif ()

set(CyberKit2GTK_INSTALLED_HEADERS
    ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitEnumTypes.h
    ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitVersion.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitApplicationInfo.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitAuthenticationRequest.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitAutocleanups.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitAutomationSession.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitBackForwardList.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitBackForwardListItem.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitColorChooserRequest.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitCredential.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitContextMenu.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitContextMenuActions.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitContextMenuItem.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitCookieManager.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitDefines.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitDeviceInfoPermissionRequest.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitDownload.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitEditingCommands.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitEditorState.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitError.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitFaviconDatabase.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitFileChooserRequest.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitFindController.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitFormSubmissionRequest.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitForwardDeclarations.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitGeolocationManager.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitGeolocationPermissionRequest.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitHitTestResult.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitInstallMissingMediaPluginsPermissionRequest.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitJavascriptResult.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitMimeInfo.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitNavigationAction.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitNavigationPolicyDecision.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitNetworkProxySettings.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitNotificationPermissionRequest.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitNotification.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitOptionMenu.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitOptionMenuItem.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitPermissionRequest.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitPlugin.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitPolicyDecision.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitPrintCustomWidget.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitPrintOperation.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitResponsePolicyDecision.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitScriptDialog.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitSecurityManager.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitSecurityOrigin.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitSettings.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitURIRequest.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitURIResponse.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitURISchemeRequest.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitURIUtilities.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitUserContent.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitUserContentFilterStore.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitUserContentManager.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitUserMediaPermissionRequest.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitWebContext.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitWebInspector.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitWebResource.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitWebView.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitWebViewBase.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitWebViewSessionState.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitWebsiteData.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitWebsiteDataManager.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitWindowProperties.h
    ${WEBKIT_DIR}/UIProcess/API/gtk/webkit2.h
)

set(CyberKit2WebExtension_INSTALLED_HEADERS
    ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitWebProcessEnumTypes.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/CyberKitConsoleMessage.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/CyberKitFrame.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/CyberKitScriptWorld.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/CyberKitWebEditor.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/CyberKitWebExtension.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/CyberKitWebExtensionAutocleanups.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/CyberKitWebHitTestResult.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/CyberKitWebPage.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/webkit-web-extension.h
)

set(CyberKitDOM_INSTALLED_HEADERS
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/webkitdomautocleanups.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/webkitdomdefines.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/webkitdom.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMAttr.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMBlob.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCDATASection.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCharacterData.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMClientRect.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMClientRectList.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMComment.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCSSRule.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCSSRuleList.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCSSStyleDeclaration.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCSSStyleSheet.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCSSValue.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCustom.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCustomUnstable.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDeprecated.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDocumentFragment.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDocumentFragmentUnstable.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDocument.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDocumentType.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDocumentUnstable.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDOMImplementation.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDOMSelection.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDOMTokenList.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDOMWindow.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDOMWindowUnstable.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMElementUnstable.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMEvent.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMEventTarget.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMFile.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMFileList.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLAnchorElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLAppletElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLAreaElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLBaseElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLBodyElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLBRElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLButtonElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLCanvasElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLCollection.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLDirectoryElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLDivElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLDListElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLDocument.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLElementUnstable.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLEmbedElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLFieldSetElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLFontElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLFormElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLFrameElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLFrameSetElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLHeadElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLHeadingElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLHRElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLHtmlElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLIFrameElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLImageElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLInputElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLLabelElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLLegendElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLLIElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLLinkElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLMapElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLMarqueeElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLMenuElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLMetaElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLModElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLObjectElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLOListElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLOptGroupElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLOptionElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLOptionsCollection.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLParagraphElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLParamElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLPreElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLQuoteElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLScriptElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLSelectElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLStyleElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTableCaptionElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTableCellElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTableColElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTableElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTableRowElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTableSectionElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTextAreaElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTitleElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLUListElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMKeyboardEvent.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMMediaList.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMMouseEvent.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMNamedNodeMap.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMNodeFilter.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMNode.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMNodeIterator.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMNodeList.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMObject.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMProcessingInstruction.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMRange.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMRangeUnstable.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMStyleSheet.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMStyleSheetList.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMText.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMTreeWalker.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMUIEvent.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMWheelEvent.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMXPathExpression.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMXPathNSResolver.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMXPathResult.h
)

set(CyberKitDOM_GTKDOC_HEADERS
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMAttr.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMBlob.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCDATASection.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCharacterData.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMClientRect.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMClientRectList.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMComment.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCSSRule.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCSSRuleList.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCSSStyleDeclaration.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCSSStyleSheet.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCSSValue.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMCustom.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDeprecated.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDocumentFragment.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDocument.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDocumentType.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDOMImplementation.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDOMSelection.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDOMTokenList.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMDOMWindow.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMEvent.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMEventTarget.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMFile.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMFileList.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLAnchorElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLAppletElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLAreaElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLBaseElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLBodyElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLBRElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLButtonElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLCanvasElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLCollection.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLDirectoryElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLDivElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLDListElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLDocument.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLEmbedElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLFieldSetElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLFontElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLFormElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLFrameElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLFrameSetElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLHeadElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLHeadingElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLHRElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLHtmlElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLIFrameElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLImageElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLInputElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLLabelElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLLegendElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLLIElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLLinkElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLMapElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLMarqueeElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLMenuElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLMetaElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLModElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLObjectElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLOListElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLOptGroupElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLOptionElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLOptionsCollection.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLParagraphElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLParamElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLPreElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLQuoteElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLScriptElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLSelectElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLStyleElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTableCaptionElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTableCellElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTableColElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTableElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTableRowElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTableSectionElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTextAreaElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLTitleElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMHTMLUListElement.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMKeyboardEvent.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMMediaList.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMMouseEvent.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMNamedNodeMap.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMNodeFilter.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMNode.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMNodeIterator.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMNodeList.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMObject.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMProcessingInstruction.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMRange.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMStyleSheet.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMStyleSheetList.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMText.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMTreeWalker.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMUIEvent.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMWheelEvent.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMXPathExpression.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMXPathNSResolver.h
    ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/CyberKitDOMXPathResult.h
)

# This is necessary because of a conflict between the GTK+ API CyberKitVersion.h and one generated by CyberCore.
list(INSERT CyberKit_INCLUDE_DIRECTORIES 0
    "${FORWARDING_HEADERS_WEBKIT2GTK_DIR}"
    "${FORWARDING_HEADERS_WEBKIT2GTK_EXTENSION_DIR}"
    "${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}"
    "${DERIVED_SOURCES_WEBKIT2GTK_DIR}"
)

list(APPEND CyberKit_INCLUDE_DIRECTORIES
    "${DERIVED_SOURCES_JAVASCRIPCOREGTK_DIR}"
    "${FORWARDING_HEADERS_DIR}/CyberScriptCore/"
    "${FORWARDING_HEADERS_DIR}/CyberScriptCore/glib"
    "${WEBKIT_DIR}/PluginProcess/unix"
    "${WEBKIT_DIR}/NetworkProcess/CustomProtocols/soup"
    "${WEBKIT_DIR}/NetworkProcess/glib"
    "${WEBKIT_DIR}/NetworkProcess/gtk"
    "${WEBKIT_DIR}/NetworkProcess/soup"
    "${WEBKIT_DIR}/NetworkProcess/unix"
    "${WEBKIT_DIR}/Platform/IPC/glib"
    "${WEBKIT_DIR}/Platform/IPC/unix"
    "${WEBKIT_DIR}/Platform/classifier"
    "${WEBKIT_DIR}/Shared/API/c/gtk"
    "${WEBKIT_DIR}/Shared/API/glib"
    "${WEBKIT_DIR}/Shared/CoordinatedGraphics"
    "${WEBKIT_DIR}/Shared/CoordinatedGraphics/threadedcompositor"
    "${WEBKIT_DIR}/Shared/Plugins/unix"
    "${WEBKIT_DIR}/Shared/glib"
    "${WEBKIT_DIR}/Shared/gtk"
    "${WEBKIT_DIR}/Shared/linux"
    "${WEBKIT_DIR}/Shared/soup"
    "${WEBKIT_DIR}/Shared/unix"
    "${WEBKIT_DIR}/UIProcess/API/C/cairo"
    "${WEBKIT_DIR}/UIProcess/API/C/gtk"
    "${WEBKIT_DIR}/UIProcess/API/glib"
    "${WEBKIT_DIR}/UIProcess/API/gtk"
    "${WEBKIT_DIR}/UIProcess/CoordinatedGraphics"
    "${WEBKIT_DIR}/UIProcess/Network/CustomProtocols/soup"
    "${WEBKIT_DIR}/UIProcess/Plugins/gtk"
    "${WEBKIT_DIR}/UIProcess/geoclue"
    "${WEBKIT_DIR}/UIProcess/glib"
    "${WEBKIT_DIR}/UIProcess/gstreamer"
    "${WEBKIT_DIR}/UIProcess/gtk"
    "${WEBKIT_DIR}/UIProcess/linux"
    "${WEBKIT_DIR}/UIProcess/soup"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/DOM"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM"
    "${WEBKIT_DIR}/WebProcess/Plugins/Netscape/unix"
    "${WEBKIT_DIR}/WebProcess/Plugins/Netscape/x11"
    "${WEBKIT_DIR}/WebProcess/gtk"
    "${WEBKIT_DIR}/WebProcess/soup"
    "${WEBKIT_DIR}/WebProcess/unix"
    "${WEBKIT_DIR}/WebProcess/CyberCoreSupport/gtk"
    "${WEBKIT_DIR}/WebProcess/CyberCoreSupport/soup"
    "${WEBKIT_DIR}/WebProcess/WebPage/CoordinatedGraphics"
    "${WEBKIT_DIR}/WebProcess/WebPage/atk"
    "${WEBKIT_DIR}/WebProcess/WebPage/gtk"
)

list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
    ${CAIRO_INCLUDE_DIRS}
    ${ENCHANT_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${GSTREAMER_INCLUDE_DIRS}
    ${GSTREAMER_PBUTILS_INCLUDE_DIRS}
    ${GTK_INCLUDE_DIRS}
    ${GTK_UNIX_PRINT_INCLUDE_DIRS}
    ${HARFBUZZ_INCLUDE_DIRS}
    ${LIBSOUP_INCLUDE_DIRS}
)

if (USE_WPE_RENDERER)
    list(APPEND CyberKit_INCLUDE_DIRECTORIES
        "${WEBKIT_DIR}/WebProcess/WebPage/libwpe"
    )
    list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
        ${WPE_INCLUDE_DIRS}
        ${WPEBACKEND_FDO_INCLUDE_DIRS}
    )
endif ()

if (USE_LIBNOTIFY)
list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
    ${LIBNOTIFY_INCLUDE_DIRS}
)
endif ()

set(CyberKitCommonIncludeDirectories ${CyberKit_INCLUDE_DIRECTORIES})
set(CyberKitCommonSystemIncludeDirectories ${CyberKit_SYSTEM_INCLUDE_DIRECTORIES})

list(APPEND WebProcess_SOURCES
    WebProcess/EntryPoint/unix/WebProcessMain.cpp
)

list(APPEND NetworkProcess_SOURCES
    NetworkProcess/EntryPoint/unix/NetworkProcessMain.cpp
)

set(SharedCyberKitLibraries
    ${CyberKit_LIBRARIES}
)

list(APPEND CyberKit_LIBRARIES
    ${GTK_UNIX_PRINT_LIBRARIES}
)

if (USE_WPE_RENDERER)
    list(APPEND CyberKit_LIBRARIES
      ${WPE_LIBRARIES}
      ${WPEBACKEND_FDO_LIBRARIES}
    )
endif ()

if (LIBNOTIFY_FOUND)
list(APPEND CyberKit_LIBRARIES
    PRIVATE ${LIBNOTIFY_LIBRARIES}
)
endif ()

if (USE_LIBWEBRTC)
list(APPEND CyberKit_SYSTEM_INCLUDE_DIRECTORIES
    "${THIRDPARTY_DIR}/libwebrtc/Source/"
    "${THIRDPARTY_DIR}/libwebrtc/Source/webrtc"
)
endif ()

# To generate CyberKitEnumTypes.h we want to use all installed headers, except CyberKitEnumTypes.h itself.
set(CyberKit2GTK_ENUM_GENERATION_HEADERS ${CyberKit2GTK_INSTALLED_HEADERS})
list(REMOVE_ITEM CyberKit2GTK_ENUM_GENERATION_HEADERS ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitEnumTypes.h)
add_custom_command(
    OUTPUT ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitEnumTypes.h
           ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitEnumTypes.cpp
    DEPENDS ${CyberKit2GTK_ENUM_GENERATION_HEADERS}

    COMMAND glib-mkenums --template ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitEnumTypes.h.template ${CyberKit2GTK_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ | sed s/WEBKIT_TYPE_KIT/WEBKIT_TYPE/ > ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitEnumTypes.h

    COMMAND glib-mkenums --template ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitEnumTypes.cpp.template ${CyberKit2GTK_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ > ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitEnumTypes.cpp
    VERBATIM
)

set(CyberKit2GTK_WEB_PROCESS_ENUM_GENERATION_HEADERS ${CyberKit2WebExtension_INSTALLED_HEADERS})
list(REMOVE_ITEM CyberKit2GTK_WEB_PROCESS_ENUM_GENERATION_HEADERS ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitWebProcessEnumTypes.h)
add_custom_command(
    OUTPUT ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitWebProcessEnumTypes.h
           ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitWebProcessEnumTypes.cpp
    DEPENDS ${CyberKit2GTK_WEB_PROCESS_ENUM_GENERATION_HEADERS}

    COMMAND glib-mkenums --template ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/CyberKitWebProcessEnumTypes.h.template ${CyberKit2GTK_WEB_PROCESS_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ | sed s/WEBKIT_TYPE_KIT/WEBKIT_TYPE/ > ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitWebProcessEnumTypes.h

    COMMAND glib-mkenums --template ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/CyberKitWebProcessEnumTypes.cpp.template ${CyberKit2GTK_WEB_PROCESS_ENUM_GENERATION_HEADERS} | sed s/web_kit/webkit/ > ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}/CyberKitWebProcessEnumTypes.cpp
    VERBATIM
)

WEBKIT_BUILD_INSPECTOR_GRESOURCES(${DERIVED_SOURCES_WEBKIT2GTK_DIR})

set(CyberKitResources
    "        <file alias=\"images/missingImage\">missingImage.png</file>\n"
    "        <file alias=\"images/missingImage@2x\">missingImage@2x.png</file>\n"
    "        <file alias=\"images/panIcon\">panIcon.png</file>\n"
    "        <file alias=\"images/textAreaResizeCorner\">textAreaResizeCorner.png</file>\n"
    "        <file alias=\"images/textAreaResizeCorner@2x\">textAreaResizeCorner@2x.png</file>\n"
)

if (ENABLE_WEB_AUDIO)
    list(APPEND CyberKitResources
        "        <file alias=\"audio/Composite\">Composite.wav</file>\n"
    )
endif ()

file(WRITE ${DERIVED_SOURCES_WEBKIT2GTK_DIR}/CyberKitResourcesGResourceBundle.xml
    "<?xml version=1.0 encoding=UTF-8?>\n"
    "<gresources>\n"
    "    <gresource prefix=\"/org/webkitgtk/resources\">\n"
    ${CyberKitResources}
    "    </gresource>\n"
    "</gresources>\n"
)

add_custom_command(
    OUTPUT ${DERIVED_SOURCES_WEBKIT2GTK_DIR}/CyberKitResourcesGResourceBundle.c
    DEPENDS ${DERIVED_SOURCES_WEBKIT2GTK_DIR}/CyberKitResourcesGResourceBundle.xml
    COMMAND glib-compile-resources --generate --sourcedir=${CMAKE_SOURCE_DIR}/Source/CyberCore/Resources --sourcedir=${CMAKE_SOURCE_DIR}/Source/CyberCore/platform/audio/resources --target=${DERIVED_SOURCES_WEBKIT2GTK_DIR}/CyberKitResourcesGResourceBundle.c ${DERIVED_SOURCES_WEBKIT2GTK_DIR}/CyberKitResourcesGResourceBundle.xml
    VERBATIM
)

if (ENABLE_WAYLAND_TARGET)
    # Wayland protocol extension.
    add_custom_command(
        OUTPUT ${DERIVED_SOURCES_WEBKIT2GTK_DIR}/CyberKitWaylandClientProtocol.c
        DEPENDS ${WEBKIT_DIR}/Shared/gtk/CyberKitWaylandProtocol.xml
        COMMAND wayland-scanner server-header < ${WEBKIT_DIR}/Shared/gtk/CyberKitWaylandProtocol.xml > ${DERIVED_SOURCES_WEBKIT2GTK_DIR}/CyberKitWaylandServerProtocol.h
        COMMAND wayland-scanner client-header < ${WEBKIT_DIR}/Shared/gtk/CyberKitWaylandProtocol.xml > ${DERIVED_SOURCES_WEBKIT2GTK_DIR}/CyberKitWaylandClientProtocol.h
        COMMAND wayland-scanner code < ${WEBKIT_DIR}/Shared/gtk/CyberKitWaylandProtocol.xml > ${DERIVED_SOURCES_WEBKIT2GTK_DIR}/CyberKitWaylandClientProtocol.c
    )
endif ()

# GTK3 PluginProcess
list(APPEND PluginProcess_SOURCES
    PluginProcess/EntryPoint/unix/PluginProcessMain.cpp
)

# Commands for building the built-in injected bundle.
add_library(webkit2gtkinjectedbundle MODULE "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/CyberKitInjectedBundleMain.cpp")
ADD_WEBKIT_PREFIX_HEADER(webkit2gtkinjectedbundle)
target_link_libraries(webkit2gtkinjectedbundle CyberKit)

target_include_directories(webkit2gtkinjectedbundle PRIVATE
    ${CyberKit_INCLUDE_DIRECTORIES}
    "${DERIVED_SOURCES_DIR}/InjectedBundle"
    "${FORWARDING_HEADERS_DIR}"
    "${FORWARDING_HEADERS_WEBKIT2GTK_DIR}"
    "${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}"
)

target_include_directories(webkit2gtkinjectedbundle SYSTEM PRIVATE
    ${CyberKit_SYSTEM_INCLUDE_DIRECTORIES}
)

if (COMPILER_IS_GCC_OR_CLANG)
    WEBKIT_ADD_TARGET_CXX_FLAGS(webkit2gtkinjectedbundle -Wno-unused-parameter)
endif ()

# Add ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} to LD_LIBRARY_PATH or DYLD_LIBRARY_PATH
if (APPLE)
    set(LOADER_LIBRARY_PATH_VAR "DYLD_LIBRARY_PATH")
    set(PREV_LOADER_LIBRARY_PATH "$ENV{DYLD_LIBRARY_PATH}")
else ()
    set(LOADER_LIBRARY_PATH_VAR "LD_LIBRARY_PATH")
    set(PREV_LOADER_LIBRARY_PATH "$ENV{LD_LIBRARY_PATH}")
endif ()

if (ENABLE_INTROSPECTION)
    string(COMPARE EQUAL "${PREV_LOADER_LIBRARY_PATH}" "" ld_library_path_does_not_exist)
    if (ld_library_path_does_not_exist)
        set(INTROSPECTION_ADDITIONAL_LIBRARY_PATH
            "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
        )
    else ()
        set(INTROSPECTION_ADDITIONAL_LIBRARY_PATH
            "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}:${PREV_LOADER_LIBRARY_PATH}"
        )
    endif ()

    # Add required -L flags from ${CMAKE_SHARED_LINKER_FLAGS} for g-ir-scanner
    string(REGEX MATCHALL "-L[^ ]*"
        INTROSPECTION_ADDITIONAL_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/CyberKit2-${WEBKITGTK_API_VERSION}.gir
        DEPENDS CyberKit
        DEPENDS ${CMAKE_BINARY_DIR}/CyberScriptCore-${WEBKITGTK_API_VERSION}.gir
        COMMAND CC=${CMAKE_C_COMPILER} CFLAGS=-Wno-deprecated-declarations LDFLAGS=
            ${LOADER_LIBRARY_PATH_VAR}="${INTROSPECTION_ADDITIONAL_LIBRARY_PATH}"
            ${INTROSPECTION_SCANNER}
            --quiet
            --warn-all
            --symbol-prefix=webkit
            --identifier-prefix=CyberKit
            --namespace=CyberKit2
            --nsversion=${WEBKITGTK_API_VERSION}
            --include=GObject-2.0
            --include=Gtk-3.0
            --include=Soup-2.4
            --include-uninstalled=${CMAKE_BINARY_DIR}/CyberScriptCore-${WEBKITGTK_API_VERSION}.gir
            --library=webkit2gtk-${WEBKITGTK_API_VERSION}
            --library=javascriptcoregtk-${WEBKITGTK_API_VERSION}
            -L${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
            ${INTROSPECTION_ADDITIONAL_LINKER_FLAGS}
            --no-libtool
            --pkg=gobject-2.0
            --pkg=gtk+-3.0
            --pkg=libsoup-2.4
            --pkg-export=webkit2gtk-${WEBKITGTK_API_VERSION}
            --output=${CMAKE_BINARY_DIR}/CyberKit2-${WEBKITGTK_API_VERSION}.gir
            --c-include="webkit2/webkit2.h"
            -DBUILDING_WEBKIT
            -DWEBKIT2_COMPILATION
            -I${CMAKE_SOURCE_DIR}/Source
            -I${WEBKIT_DIR}
            -I${DERIVED_SOURCES_DIR}
            -I${DERIVED_SOURCES_WEBKIT2GTK_DIR}
            -I${DERIVED_SOURCES_JAVASCRIPCOREGTK_DIR}
            -I${FORWARDING_HEADERS_DIR}
            -I${FORWARDING_HEADERS_DIR}/CyberScriptCore/glib
            -I${FORWARDING_HEADERS_WEBKIT2GTK_DIR}
            ${CyberKit2GTK_INSTALLED_HEADERS}
            ${WEBKIT_DIR}/Shared/API/glib/*.cpp
            ${WEBKIT_DIR}/UIProcess/API/glib/*.cpp
            ${WEBKIT_DIR}/UIProcess/API/gtk/*.cpp
    )

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/CyberKit2WebExtension-${WEBKITGTK_API_VERSION}.gir
        DEPENDS ${CMAKE_BINARY_DIR}/CyberScriptCore-${WEBKITGTK_API_VERSION}.gir
        DEPENDS ${CMAKE_BINARY_DIR}/CyberKit2-${WEBKITGTK_API_VERSION}.gir
        COMMAND CC=${CMAKE_C_COMPILER} CFLAGS=-Wno-deprecated-declarations
            LDFLAGS="${INTROSPECTION_ADDITIONAL_LDFLAGS}"
            ${LOADER_LIBRARY_PATH_VAR}="${INTROSPECTION_ADDITIONAL_LIBRARY_PATH}"
            ${INTROSPECTION_SCANNER}
            --quiet
            --warn-all
            --symbol-prefix=webkit
            --identifier-prefix=CyberKit
            --namespace=CyberKit2WebExtension
            --nsversion=${WEBKITGTK_API_VERSION}
            --include=GObject-2.0
            --include=Gtk-3.0
            --include=Soup-2.4
            --include-uninstalled=${CMAKE_BINARY_DIR}/CyberScriptCore-${WEBKITGTK_API_VERSION}.gir
            --library=webkit2gtk-${WEBKITGTK_API_VERSION}
            --library=javascriptcoregtk-${WEBKITGTK_API_VERSION}
            ${INTROSPECTION_ADDITIONAL_LIBRARIES}
            -L${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
            ${INTROSPECTION_ADDITIONAL_LINKER_FLAGS}
            --no-libtool
            --pkg=gobject-2.0
            --pkg=gtk+-3.0
            --pkg=libsoup-2.4
            --pkg-export=webkit2gtk-web-extension-${WEBKITGTK_API_VERSION}
            --output=${CMAKE_BINARY_DIR}/CyberKit2WebExtension-${WEBKITGTK_API_VERSION}.gir
            --c-include="webkit2/webkit-web-extension.h"
            -DBUILDING_WEBKIT
            -DWEBKIT2_COMPILATION
            -I${CMAKE_SOURCE_DIR}/Source
            -I${WEBKIT_DIR}
            -I${DERIVED_SOURCES_DIR}
            -I${DERIVED_SOURCES_WEBKIT2GTK_DIR}
            -I${DERIVED_SOURCES_JAVASCRIPCOREGTK_DIR}
            -I${FORWARDING_HEADERS_DIR}
            -I${FORWARDING_HEADERS_DIR}/CyberScriptCore/glib
            -I${FORWARDING_HEADERS_WEBKIT2GTK_DIR}
            -I${FORWARDING_HEADERS_WEBKIT2GTK_EXTENSION_DIR}
            -I${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk
            ${CyberKitDOM_INSTALLED_HEADERS}
            ${CyberKit2WebExtension_INSTALLED_HEADERS}
            ${WEBKIT_DIR}/Shared/API/glib/CyberKitContextMenu.cpp
            ${WEBKIT_DIR}/Shared/API/glib/CyberKitContextMenuItem.cpp
            ${WEBKIT_DIR}/Shared/API/glib/CyberKitHitTestResult.cpp
            ${WEBKIT_DIR}/Shared/API/glib/CyberKitURIRequest.cpp
            ${WEBKIT_DIR}/Shared/API/glib/CyberKitURIResponse.cpp
            ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitContextMenu.h
            ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitContextMenuActions.h
            ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitContextMenuItem.h
            ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitHitTestResult.h
            ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitURIRequest.h
            ${WEBKIT_DIR}/UIProcess/API/gtk/CyberKitURIResponse.h
            ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/*.cpp
            ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/DOM/*.cpp
    )

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/CyberKit2-${WEBKITGTK_API_VERSION}.typelib
        DEPENDS ${CMAKE_BINARY_DIR}/CyberKit2-${WEBKITGTK_API_VERSION}.gir
        COMMAND ${INTROSPECTION_COMPILER} --includedir=${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/CyberKit2-${WEBKITGTK_API_VERSION}.gir -o ${CMAKE_BINARY_DIR}/CyberKit2-${WEBKITGTK_API_VERSION}.typelib
    )

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/CyberKit2WebExtension-${WEBKITGTK_API_VERSION}.typelib
        DEPENDS ${CMAKE_BINARY_DIR}/CyberKit2WebExtension-${WEBKITGTK_API_VERSION}.gir
        COMMAND ${INTROSPECTION_COMPILER} --includedir=${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/CyberKit2WebExtension-${WEBKITGTK_API_VERSION}.gir -o ${CMAKE_BINARY_DIR}/CyberKit2WebExtension-${WEBKITGTK_API_VERSION}.typelib
    )

    ADD_TYPELIB(${CMAKE_BINARY_DIR}/CyberKit2-${WEBKITGTK_API_VERSION}.typelib)
    ADD_TYPELIB(${CMAKE_BINARY_DIR}/CyberKit2WebExtension-${WEBKITGTK_API_VERSION}.typelib)
endif ()

install(TARGETS webkit2gtkinjectedbundle
        DESTINATION "${LIB_INSTALL_DIR}/webkit2gtk-${WEBKITGTK_API_VERSION}/injected-bundle"
)
install(FILES "${CMAKE_BINARY_DIR}/Source/CyberKit/webkit2gtk-${WEBKITGTK_API_VERSION}.pc"
              "${CMAKE_BINARY_DIR}/Source/CyberKit/webkit2gtk-web-extension-${WEBKITGTK_API_VERSION}.pc"
        DESTINATION "${LIB_INSTALL_DIR}/pkgconfig"
)
install(FILES ${CyberKit2GTK_INSTALLED_HEADERS}
              ${CyberKit2WebExtension_INSTALLED_HEADERS}
        DESTINATION "${WEBKITGTK_HEADER_INSTALL_DIR}/webkit2"
)
install(FILES ${CyberKitDOM_INSTALLED_HEADERS}
        DESTINATION "${WEBKITGTK_HEADER_INSTALL_DIR}/webkitdom"
)

if (ENABLE_INTROSPECTION)
    install(FILES ${CMAKE_BINARY_DIR}/CyberKit2-${WEBKITGTK_API_VERSION}.gir
                  ${CMAKE_BINARY_DIR}/CyberKit2WebExtension-${WEBKITGTK_API_VERSION}.gir
            DESTINATION ${INTROSPECTION_INSTALL_GIRDIR}
    )
    install(FILES ${CMAKE_BINARY_DIR}/CyberKit2-${WEBKITGTK_API_VERSION}.typelib
                  ${CMAKE_BINARY_DIR}/CyberKit2WebExtension-${WEBKITGTK_API_VERSION}.typelib
            DESTINATION ${INTROSPECTION_INSTALL_TYPELIBDIR}
    )
endif ()

file(WRITE ${CMAKE_BINARY_DIR}/gtkdoc-webkit2gtk.cfg
    "[webkit2gtk-${WEBKITGTK_API_VERSION}]\n"
    "pkgconfig_file=${CyberKit2_PKGCONFIG_FILE}\n"
    "decorator=WEBKIT_API|WEBKIT_DEPRECATED|WEBKIT_DEPRECATED_FOR\\(.+\\)\n"
    "deprecation_guard=WEBKIT_DISABLE_DEPRECATED\n"
    "namespace=webkit\n"
    "cflags=-I${CMAKE_SOURCE_DIR}/Source\n"
    "       -I${WEBKIT_DIR}/Shared/API/glib\n"
    "       -I${WEBKIT_DIR}/UIProcess/API/glib\n"
    "       -I${WEBKIT_DIR}/UIProcess/API/gtk\n"
    "       -I${DERIVED_SOURCES_WEBKIT2GTK_DIR}\n"
    "       -I${FORWARDING_HEADERS_WEBKIT2GTK_DIR}\n"
    "doc_dir=${WEBKIT_DIR}/UIProcess/API/gtk/docs\n"
    "source_dirs=${WEBKIT_DIR}/Shared/API/glib\n"
    "            ${WEBKIT_DIR}/UIProcess/API/glib\n"
    "            ${WEBKIT_DIR}/UIProcess/API/gtk\n"
    "            ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib\n"
    "            ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk\n"
    "            ${DERIVED_SOURCES_WEBKIT2GTK_API_DIR}\n"
    "headers=${CyberKit2GTK_ENUM_GENERATION_HEADERS} ${CyberKit2WebExtension_INSTALLED_HEADERS}\n"
    "main_sgml_file=webkit2gtk-docs.sgml\n"
)

file(WRITE ${CMAKE_BINARY_DIR}/gtkdoc-webkitdom.cfg
    "[webkitdomgtk-${WEBKITGTK_API_VERSION}]\n"
    "pkgconfig_file=${CyberKit2_PKGCONFIG_FILE}\n"
    "decorator=WEBKIT_API|WEBKIT_DEPRECATED|WEBKIT_DEPRECATED_FOR\\(.+\\)\n"
    "deprecation_guard=WEBKIT_DISABLE_DEPRECATED\n"
    "namespace=webkit_dom\n"
    "cflags=-I${CMAKE_SOURCE_DIR}/Source\n"
    "       -I${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM\n"
    "       -I${DERIVED_SOURCES_WEBKIT2GTK_DIR}\n"
    "       -I${FORWARDING_HEADERS_WEBKIT2GTK_DIR}\n"
    "doc_dir=${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM/docs\n"
    "source_dirs=${WEBKIT_DIR}/WebProcess/InjectedBundle/API/glib/DOM\n"
    "            ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM\n"
    "headers=${CyberKitDOM_GTKDOC_HEADERS}\n"
    "main_sgml_file=webkitdomgtk-docs.sgml\n"
)

add_custom_target(CyberKit-forwarding-headers
    COMMAND ${PERL_EXECUTABLE} ${WEBKIT_DIR}/Scripts/generate-forwarding-headers.pl --include-path ${WEBKIT_DIR} --output ${FORWARDING_HEADERS_DIR} --platform gtk --platform soup
)

# These symbolic link allows includes like #include <webkit2/WebkitWebView.h> which simulates installed headers.
add_custom_command(
    OUTPUT ${FORWARDING_HEADERS_WEBKIT2GTK_DIR}/webkit2
    DEPENDS ${WEBKIT_DIR}/UIProcess/API/gtk
    COMMAND ln -n -s -f ${WEBKIT_DIR}/UIProcess/API/gtk ${FORWARDING_HEADERS_WEBKIT2GTK_DIR}/webkit2
)
add_custom_command(
    OUTPUT ${FORWARDING_HEADERS_WEBKIT2GTK_EXTENSION_DIR}/webkit2
    DEPENDS ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk
    COMMAND ln -n -s -f ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk ${FORWARDING_HEADERS_WEBKIT2GTK_EXTENSION_DIR}/webkit2
)
add_custom_command(
    OUTPUT ${FORWARDING_HEADERS_WEBKIT2GTK_EXTENSION_DIR}/webkitdom
    DEPENDS ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM
    COMMAND ln -n -s -f ${WEBKIT_DIR}/WebProcess/InjectedBundle/API/gtk/DOM ${FORWARDING_HEADERS_WEBKIT2GTK_EXTENSION_DIR}/webkitdom
)
add_custom_target(CyberKit-fake-api-headers
    DEPENDS ${FORWARDING_HEADERS_WEBKIT2GTK_DIR}/webkit2
            ${FORWARDING_HEADERS_WEBKIT2GTK_EXTENSION_DIR}/webkit2
            ${FORWARDING_HEADERS_WEBKIT2GTK_EXTENSION_DIR}/webkitdom
)

set(WEBKIT_EXTRA_DEPENDENCIES
     CyberKit-fake-api-headers
     CyberKit-forwarding-headers
)
