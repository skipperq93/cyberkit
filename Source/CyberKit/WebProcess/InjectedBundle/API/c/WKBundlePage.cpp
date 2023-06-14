/*
 * Copyright (C) 2010, 2011, 2013, 2015 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "WKBundlePage.h"
#include "WKBundlePagePrivate.h"

#include "APIArray.h"
#include "APIFrameHandle.h"
#include "APIString.h"
#include "APIURL.h"
#include "APIURLRequest.h"
#include "InjectedBundleNodeHandle.h"
#include "InjectedBundlePageEditorClient.h"
#include "InjectedBundlePageFormClient.h"
#include "InjectedBundlePageLoaderClient.h"
#include "InjectedBundlePageResourceLoadClient.h"
#include "InjectedBundlePageUIClient.h"
#include "PageBanner.h"
#include "WKAPICast.h"
#include "WKArray.h"
#include "WKBundleAPICast.h"
#include "WKRetainPtr.h"
#include "WKString.h"
#include "WebContextMenu.h"
#include "WebContextMenuItem.h"
#include "WebFrame.h"
#include "WebFullScreenManager.h"
#include "WebImage.h"
#include "WebInspector.h"
#include "WebPage.h"
#include "WebPageGroupProxy.h"
#include "WebPageOverlay.h"
#include "WebRenderLayer.h"
#include "WebRenderObject.h"
#include <CyberCore/AXObjectCache.h>
#include <CyberCore/AccessibilityObject.h>
#include <CyberCore/ApplicationCacheStorage.h>
#include <CyberCore/Frame.h>
#include <CyberCore/Page.h>
#include <CyberCore/PageOverlay.h>
#include <CyberCore/PageOverlayController.h>
#include <CyberCore/RenderLayerCompositor.h>
#include <CyberCore/ScriptExecutionContext.h>
#include <CyberCore/SecurityOriginData.h>
#include <CyberCore/WheelEventTestTrigger.h>
#include <wtf/StdLibExtras.h>
#include <wtf/URL.h>

WKTypeID WKBundlePageGetTypeID()
{
    return CyberKit::toAPI(CyberKit::WebPage::APIType);
}

void WKBundlePageSetContextMenuClient(WKBundlePageRef pageRef, WKBundlePageContextMenuClientBase* wkClient)
{
#if ENABLE(CONTEXT_MENUS)
    CyberKit::toImpl(pageRef)->setInjectedBundleContextMenuClient(std::make_unique<CyberKit::InjectedBundlePageContextMenuClient>(wkClient));
#else
    UNUSED_PARAM(pageRef);
    UNUSED_PARAM(wkClient);
#endif
}

void WKBundlePageSetEditorClient(WKBundlePageRef pageRef, WKBundlePageEditorClientBase* wkClient)
{
    CyberKit::toImpl(pageRef)->setInjectedBundleEditorClient(wkClient ? std::make_unique<CyberKit::InjectedBundlePageEditorClient>(*wkClient) : std::make_unique<API::InjectedBundle::EditorClient>());
}

void WKBundlePageSetFormClient(WKBundlePageRef pageRef, WKBundlePageFormClientBase* wkClient)
{
    CyberKit::toImpl(pageRef)->setInjectedBundleFormClient(std::make_unique<CyberKit::InjectedBundlePageFormClient>(wkClient));
}

void WKBundlePageSetPageLoaderClient(WKBundlePageRef pageRef, WKBundlePageLoaderClientBase* wkClient)
{
    CyberKit::toImpl(pageRef)->setInjectedBundlePageLoaderClient(std::make_unique<CyberKit::InjectedBundlePageLoaderClient>(wkClient));
}

void WKBundlePageSetResourceLoadClient(WKBundlePageRef pageRef, WKBundlePageResourceLoadClientBase* wkClient)
{
    CyberKit::toImpl(pageRef)->setInjectedBundleResourceLoadClient(std::make_unique<CyberKit::InjectedBundlePageResourceLoadClient>(wkClient));
}

void WKBundlePageSetPolicyClient(WKBundlePageRef pageRef, WKBundlePagePolicyClientBase* wkClient)
{
    CyberKit::toImpl(pageRef)->initializeInjectedBundlePolicyClient(wkClient);
}

void WKBundlePageSetUIClient(WKBundlePageRef pageRef, WKBundlePageUIClientBase* wkClient)
{
    CyberKit::toImpl(pageRef)->setInjectedBundleUIClient(std::make_unique<CyberKit::InjectedBundlePageUIClient>(wkClient));
}

void WKBundlePageSetFullScreenClient(WKBundlePageRef pageRef, WKBundlePageFullScreenClientBase* wkClient)
{
#if defined(ENABLE_FULLSCREEN_API) && ENABLE_FULLSCREEN_API
    CyberKit::toImpl(pageRef)->initializeInjectedBundleFullScreenClient(wkClient);
#else
    UNUSED_PARAM(pageRef);
    UNUSED_PARAM(wkClient);
#endif
}

void WKBundlePageWillEnterFullScreen(WKBundlePageRef pageRef)
{
#if defined(ENABLE_FULLSCREEN_API) && ENABLE_FULLSCREEN_API
    CyberKit::toImpl(pageRef)->fullScreenManager()->willEnterFullScreen();
#else
    UNUSED_PARAM(pageRef);
#endif
}

void WKBundlePageDidEnterFullScreen(WKBundlePageRef pageRef)
{
#if defined(ENABLE_FULLSCREEN_API) && ENABLE_FULLSCREEN_API
    CyberKit::toImpl(pageRef)->fullScreenManager()->didEnterFullScreen();
#else
    UNUSED_PARAM(pageRef);
#endif
}

void WKBundlePageWillExitFullScreen(WKBundlePageRef pageRef)
{
#if defined(ENABLE_FULLSCREEN_API) && ENABLE_FULLSCREEN_API
    CyberKit::toImpl(pageRef)->fullScreenManager()->willExitFullScreen();
#else
    UNUSED_PARAM(pageRef);
#endif
}

void WKBundlePageDidExitFullScreen(WKBundlePageRef pageRef)
{
#if defined(ENABLE_FULLSCREEN_API) && ENABLE_FULLSCREEN_API
    CyberKit::toImpl(pageRef)->fullScreenManager()->didExitFullScreen();
#else
    UNUSED_PARAM(pageRef);
#endif
}

WKBundlePageGroupRef WKBundlePageGetPageGroup(WKBundlePageRef pageRef)
{
    return toAPI(CyberKit::toImpl(pageRef)->pageGroup());
}

WKBundleFrameRef WKBundlePageGetMainFrame(WKBundlePageRef pageRef)
{
    return toAPI(CyberKit::toImpl(pageRef)->mainWebFrame());
}

WKFrameHandleRef WKBundleFrameCreateFrameHandle(WKBundleFrameRef bundleFrameRef)
{
    return CyberKit::toAPI(&API::FrameHandle::create(CyberKit::toImpl(bundleFrameRef)->frameID()).leakRef());
}

void WKBundlePageClickMenuItem(WKBundlePageRef pageRef, WKContextMenuItemRef item)
{
#if ENABLE(CONTEXT_MENUS)
    CyberKit::toImpl(pageRef)->contextMenu()->itemSelected(CyberKit::toImpl(item)->data());
#else
    UNUSED_PARAM(pageRef);
    UNUSED_PARAM(item);
#endif
}

#if ENABLE(CONTEXT_MENUS)
static Ref<API::Array> contextMenuItems(const CyberKit::WebContextMenu& contextMenu)
{
    auto items = contextMenu.items();

    Vector<RefPtr<API::Object>> menuItems;
    menuItems.reserveInitialCapacity(items.size());

    for (const auto& item : items)
        menuItems.uncheckedAppend(CyberKit::WebContextMenuItem::create(item));

    return API::Array::create(WTFMove(menuItems));
}
#endif

WKArrayRef WKBundlePageCopyContextMenuItems(WKBundlePageRef pageRef)
{
#if ENABLE(CONTEXT_MENUS)
    CyberKit::WebContextMenu* contextMenu = CyberKit::toImpl(pageRef)->contextMenu();

    return CyberKit::toAPI(&contextMenuItems(*contextMenu).leakRef());
#else
    UNUSED_PARAM(pageRef);
    return nullptr;
#endif
}

WKArrayRef WKBundlePageCopyContextMenuAtPointInWindow(WKBundlePageRef pageRef, WKPoint point)
{
#if ENABLE(CONTEXT_MENUS)
    CyberKit::WebContextMenu* contextMenu = CyberKit::toImpl(pageRef)->contextMenuAtPointInWindow(CyberKit::toIntPoint(point));
    if (!contextMenu)
        return nullptr;

    return CyberKit::toAPI(&contextMenuItems(*contextMenu).leakRef());
#else
    UNUSED_PARAM(pageRef);
    UNUSED_PARAM(point);
    return nullptr;
#endif
}

void WKBundlePageInsertNewlineInQuotedContent(WKBundlePageRef pageRef)
{
    CyberKit::toImpl(pageRef)->insertNewlineInQuotedContent();
}

void* WKAccessibilityRootObject(WKBundlePageRef pageRef)
{
#if HAVE(ACCESSIBILITY)
    if (!pageRef)
        return 0;
    
    CyberCore::Page* page = CyberKit::toImpl(pageRef)->corePage();
    if (!page)
        return 0;
    
    CyberCore::Frame& core = page->mainFrame();
    if (!core.document())
        return 0;
    
    CyberCore::AXObjectCache::enableAccessibility();

    CyberCore::AccessibilityObject* root = core.document()->axObjectCache()->rootObject();
    if (!root)
        return 0;
    
    return root->wrapper();
#else
    UNUSED_PARAM(pageRef);
    return 0;
#endif
}

void* WKAccessibilityFocusedObject(WKBundlePageRef pageRef)
{
#if HAVE(ACCESSIBILITY)
    if (!pageRef)
        return 0;
    
    CyberCore::Page* page = CyberKit::toImpl(pageRef)->corePage();
    if (!page)
        return 0;

    CyberCore::AXObjectCache::enableAccessibility();

    CyberCore::AccessibilityObject* focusedObject = CyberCore::AXObjectCache::focusedUIElementForPage(page);
    if (!focusedObject)
        return 0;
    
    return focusedObject->wrapper();
#else
    UNUSED_PARAM(pageRef);
    return 0;
#endif
}

void WKAccessibilityEnableEnhancedAccessibility(bool enable)
{
#if HAVE(ACCESSIBILITY)
    CyberCore::AXObjectCache::setEnhancedUserInterfaceAccessibility(enable);
#endif
}

bool WKAccessibilityEnhancedAccessibilityEnabled()
{
#if HAVE(ACCESSIBILITY)
    return CyberCore::AXObjectCache::accessibilityEnhancedUserInterfaceEnabled();
#else
    return false;
#endif
}

void WKBundlePageStopLoading(WKBundlePageRef pageRef)
{
    CyberKit::toImpl(pageRef)->stopLoading();
}

void WKBundlePageSetDefersLoading(WKBundlePageRef pageRef, bool defersLoading)
{
    CyberKit::toImpl(pageRef)->setDefersLoading(defersLoading);
}

WKStringRef WKBundlePageCopyRenderTreeExternalRepresentation(WKBundlePageRef pageRef)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(pageRef)->renderTreeExternalRepresentation());
}

WKStringRef WKBundlePageCopyRenderTreeExternalRepresentationForPrinting(WKBundlePageRef pageRef)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(pageRef)->renderTreeExternalRepresentationForPrinting());
}

void WKBundlePageExecuteEditingCommand(WKBundlePageRef pageRef, WKStringRef name, WKStringRef argument)
{
    CyberKit::toImpl(pageRef)->executeEditingCommand(CyberKit::toWTFString(name), CyberKit::toWTFString(argument));
}

bool WKBundlePageIsEditingCommandEnabled(WKBundlePageRef pageRef, WKStringRef name)
{
    return CyberKit::toImpl(pageRef)->isEditingCommandEnabled(CyberKit::toWTFString(name));
}

void WKBundlePageClearMainFrameName(WKBundlePageRef pageRef)
{
    CyberKit::toImpl(pageRef)->clearMainFrameName();
}

void WKBundlePageClose(WKBundlePageRef pageRef)
{
    CyberKit::toImpl(pageRef)->sendClose();
}

double WKBundlePageGetTextZoomFactor(WKBundlePageRef pageRef)
{
    return CyberKit::toImpl(pageRef)->textZoomFactor();
}

void WKBundlePageSetTextZoomFactor(WKBundlePageRef pageRef, double zoomFactor)
{
    CyberKit::toImpl(pageRef)->setTextZoomFactor(zoomFactor);
}

double WKBundlePageGetPageZoomFactor(WKBundlePageRef pageRef)
{
    return CyberKit::toImpl(pageRef)->pageZoomFactor();
}

void WKBundlePageSetPageZoomFactor(WKBundlePageRef pageRef, double zoomFactor)
{
    CyberKit::toImpl(pageRef)->setPageZoomFactor(zoomFactor);
}

void WKBundlePageSetScaleAtOrigin(WKBundlePageRef pageRef, double scale, WKPoint origin)
{
    CyberKit::toImpl(pageRef)->scalePage(scale, CyberKit::toIntPoint(origin));
}

WKStringRef WKBundlePageDumpHistoryForTesting(WKBundlePageRef page, WKStringRef directory)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(page)->dumpHistoryForTesting(CyberKit::toWTFString(directory)));
}

void WKBundleClearHistoryForTesting(WKBundlePageRef page)
{
    CyberKit::toImpl(page)->clearHistory();
}

WKBundleBackForwardListRef WKBundlePageGetBackForwardList(WKBundlePageRef pageRef)
{
    return nullptr;
}

void WKBundlePageInstallPageOverlay(WKBundlePageRef pageRef, WKBundlePageOverlayRef pageOverlayRef)
{
    CyberKit::toImpl(pageRef)->corePage()->pageOverlayController().installPageOverlay(*CyberKit::toImpl(pageOverlayRef)->coreOverlay(), CyberCore::PageOverlay::FadeMode::DoNotFade);
}

void WKBundlePageUninstallPageOverlay(WKBundlePageRef pageRef, WKBundlePageOverlayRef pageOverlayRef)
{
    CyberKit::toImpl(pageRef)->corePage()->pageOverlayController().uninstallPageOverlay(*CyberKit::toImpl(pageOverlayRef)->coreOverlay(), CyberCore::PageOverlay::FadeMode::DoNotFade);
}

void WKBundlePageInstallPageOverlayWithAnimation(WKBundlePageRef pageRef, WKBundlePageOverlayRef pageOverlayRef)
{
    CyberKit::toImpl(pageRef)->corePage()->pageOverlayController().installPageOverlay(*CyberKit::toImpl(pageOverlayRef)->coreOverlay(), CyberCore::PageOverlay::FadeMode::Fade);
}

void WKBundlePageUninstallPageOverlayWithAnimation(WKBundlePageRef pageRef, WKBundlePageOverlayRef pageOverlayRef)
{
    CyberKit::toImpl(pageRef)->corePage()->pageOverlayController().uninstallPageOverlay(*CyberKit::toImpl(pageOverlayRef)->coreOverlay(), CyberCore::PageOverlay::FadeMode::Fade);
}

void WKBundlePageSetTopOverhangImage(WKBundlePageRef pageRef, WKImageRef imageRef)
{
#if PLATFORM(MAC)
    CyberKit::toImpl(pageRef)->setTopOverhangImage(CyberKit::toImpl(imageRef));
#else
    UNUSED_PARAM(pageRef);
    UNUSED_PARAM(imageRef);
#endif
}

void WKBundlePageSetBottomOverhangImage(WKBundlePageRef pageRef, WKImageRef imageRef)
{
#if PLATFORM(MAC)
    CyberKit::toImpl(pageRef)->setBottomOverhangImage(CyberKit::toImpl(imageRef));
#else
    UNUSED_PARAM(pageRef);
    UNUSED_PARAM(imageRef);
#endif
}

#if !PLATFORM(IOS_FAMILY)
void WKBundlePageSetHeaderBanner(WKBundlePageRef pageRef, WKBundlePageBannerRef bannerRef)
{
    CyberKit::toImpl(pageRef)->setHeaderPageBanner(CyberKit::toImpl(bannerRef));
}

void WKBundlePageSetFooterBanner(WKBundlePageRef pageRef, WKBundlePageBannerRef bannerRef)
{
    CyberKit::toImpl(pageRef)->setFooterPageBanner(CyberKit::toImpl(bannerRef));
}
#endif // !PLATFORM(IOS_FAMILY)

bool WKBundlePageHasLocalDataForURL(WKBundlePageRef pageRef, WKURLRef urlRef)
{
    return CyberKit::toImpl(pageRef)->hasLocalDataForURL(URL(URL(), CyberKit::toWTFString(urlRef)));
}

bool WKBundlePageCanHandleRequest(WKURLRequestRef requestRef)
{
    if (!requestRef)
        return false;
    return CyberKit::WebPage::canHandleRequest(CyberKit::toImpl(requestRef)->resourceRequest());
}

bool WKBundlePageFindString(WKBundlePageRef pageRef, WKStringRef target, WKFindOptions findOptions)
{
    return CyberKit::toImpl(pageRef)->findStringFromInjectedBundle(CyberKit::toWTFString(target), CyberKit::toFindOptions(findOptions));
}

void WKBundlePageFindStringMatches(WKBundlePageRef pageRef, WKStringRef target, WKFindOptions findOptions)
{
    CyberKit::toImpl(pageRef)->findStringMatchesFromInjectedBundle(CyberKit::toWTFString(target), CyberKit::toFindOptions(findOptions));
}

void WKBundlePageReplaceStringMatches(WKBundlePageRef pageRef, WKArrayRef matchIndicesRef, WKStringRef replacementText, bool selectionOnly)
{
    auto* matchIndices = CyberKit::toImpl(matchIndicesRef);

    Vector<uint32_t> indices;
    indices.reserveInitialCapacity(matchIndices->size());

    auto numberOfMatchIndices = matchIndices->size();
    for (size_t i = 0; i < numberOfMatchIndices; ++i) {
        if (auto* indexAsObject = matchIndices->at<API::UInt64>(i))
            indices.uncheckedAppend(indexAsObject->value());
    }
    CyberKit::toImpl(pageRef)->replaceStringMatchesFromInjectedBundle(indices, CyberKit::toWTFString(replacementText), selectionOnly);
}

WKImageRef WKBundlePageCreateSnapshotWithOptions(WKBundlePageRef pageRef, WKRect rect, WKSnapshotOptions options)
{
    RefPtr<CyberKit::WebImage> webImage = CyberKit::toImpl(pageRef)->scaledSnapshotWithOptions(CyberKit::toIntRect(rect), 1, CyberKit::toSnapshotOptions(options));
    return toAPI(webImage.leakRef());
}

WKImageRef WKBundlePageCreateSnapshotInViewCoordinates(WKBundlePageRef pageRef, WKRect rect, WKImageOptions options)
{
    auto snapshotOptions = CyberKit::snapshotOptionsFromImageOptions(options);
    snapshotOptions |= CyberKit::SnapshotOptionsInViewCoordinates;
    RefPtr<CyberKit::WebImage> webImage = CyberKit::toImpl(pageRef)->scaledSnapshotWithOptions(CyberKit::toIntRect(rect), 1, snapshotOptions);
    return toAPI(webImage.leakRef());
}

WKImageRef WKBundlePageCreateSnapshotInDocumentCoordinates(WKBundlePageRef pageRef, WKRect rect, WKImageOptions options)
{
    RefPtr<CyberKit::WebImage> webImage = CyberKit::toImpl(pageRef)->scaledSnapshotWithOptions(CyberKit::toIntRect(rect), 1, CyberKit::snapshotOptionsFromImageOptions(options));
    return toAPI(webImage.leakRef());
}

WKImageRef WKBundlePageCreateScaledSnapshotInDocumentCoordinates(WKBundlePageRef pageRef, WKRect rect, double scaleFactor, WKImageOptions options)
{
    RefPtr<CyberKit::WebImage> webImage = CyberKit::toImpl(pageRef)->scaledSnapshotWithOptions(CyberKit::toIntRect(rect), scaleFactor, CyberKit::snapshotOptionsFromImageOptions(options));
    return toAPI(webImage.leakRef());
}

double WKBundlePageGetBackingScaleFactor(WKBundlePageRef pageRef)
{
    return CyberKit::toImpl(pageRef)->deviceScaleFactor();
}

void WKBundlePageListenForLayoutMilestones(WKBundlePageRef pageRef, WKLayoutMilestones milestones)
{
    CyberKit::toImpl(pageRef)->listenForLayoutMilestones(CyberKit::toLayoutMilestones(milestones));
}

WKBundleInspectorRef WKBundlePageGetInspector(WKBundlePageRef pageRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(pageRef)->inspector());
}

void WKBundlePageForceRepaint(WKBundlePageRef page)
{
    CyberKit::toImpl(page)->forceRepaintWithoutCallback();
}

void WKBundlePageSimulateMouseDown(WKBundlePageRef page, int button, WKPoint position, int clickCount, WKEventModifiers modifiers, double time)
{
    CyberKit::toImpl(page)->simulateMouseDown(button, CyberKit::toIntPoint(position), clickCount, modifiers, WallTime::fromRawSeconds(time));
}

void WKBundlePageSimulateMouseUp(WKBundlePageRef page, int button, WKPoint position, int clickCount, WKEventModifiers modifiers, double time)
{
    CyberKit::toImpl(page)->simulateMouseUp(button, CyberKit::toIntPoint(position), clickCount, modifiers, WallTime::fromRawSeconds(time));
}

void WKBundlePageSimulateMouseMotion(WKBundlePageRef page, WKPoint position, double time)
{
    CyberKit::toImpl(page)->simulateMouseMotion(CyberKit::toIntPoint(position), WallTime::fromRawSeconds(time));
}

uint64_t WKBundlePageGetRenderTreeSize(WKBundlePageRef pageRef)
{
    return CyberKit::toImpl(pageRef)->renderTreeSize();
}

WKRenderObjectRef WKBundlePageCopyRenderTree(WKBundlePageRef pageRef)
{
    return CyberKit::toAPI(CyberKit::WebRenderObject::create(CyberKit::toImpl(pageRef)).leakRef());
}

WKRenderLayerRef WKBundlePageCopyRenderLayerTree(WKBundlePageRef pageRef)
{
    return CyberKit::toAPI(CyberKit::WebRenderLayer::create(CyberKit::toImpl(pageRef)).leakRef());
}

void WKBundlePageSetPaintedObjectsCounterThreshold(WKBundlePageRef, uint64_t)
{
    // FIXME: This function is only still here to keep open source Mac builds building.
    // We should remove it as soon as we can.
}

void WKBundlePageSetTracksRepaints(WKBundlePageRef pageRef, bool trackRepaints)
{
    CyberKit::toImpl(pageRef)->setTracksRepaints(trackRepaints);
}

bool WKBundlePageIsTrackingRepaints(WKBundlePageRef pageRef)
{
    return CyberKit::toImpl(pageRef)->isTrackingRepaints();
}

void WKBundlePageResetTrackedRepaints(WKBundlePageRef pageRef)
{
    CyberKit::toImpl(pageRef)->resetTrackedRepaints();
}

WKArrayRef WKBundlePageCopyTrackedRepaintRects(WKBundlePageRef pageRef)
{
    return CyberKit::toAPI(&CyberKit::toImpl(pageRef)->trackedRepaintRects().leakRef());
}

void WKBundlePageSetComposition(WKBundlePageRef pageRef, WKStringRef text, int from, int length, bool suppressUnderline)
{
    CyberKit::toImpl(pageRef)->setCompositionForTesting(CyberKit::toWTFString(text), from, length, suppressUnderline);
}

bool WKBundlePageHasComposition(WKBundlePageRef pageRef)
{
    return CyberKit::toImpl(pageRef)->hasCompositionForTesting();
}

void WKBundlePageConfirmComposition(WKBundlePageRef pageRef)
{
    CyberKit::toImpl(pageRef)->confirmCompositionForTesting(String());
}

void WKBundlePageConfirmCompositionWithText(WKBundlePageRef pageRef, WKStringRef text)
{
    CyberKit::toImpl(pageRef)->confirmCompositionForTesting(CyberKit::toWTFString(text));
}

void WKBundlePageSetUseDarkAppearance(WKBundlePageRef pageRef, bool useDarkAppearance)
{
    CyberKit::WebPage* webPage = CyberKit::toImpl(pageRef);
    if (CyberCore::Page* page = webPage ? webPage->corePage() : nullptr)
        page->setUseDarkAppearance(useDarkAppearance);
}

bool WKBundlePageIsUsingDarkAppearance(WKBundlePageRef pageRef)
{
    CyberKit::WebPage* webPage = CyberKit::toImpl(pageRef);
    if (CyberCore::Page* page = webPage ? webPage->corePage() : nullptr)
        return page->useDarkAppearance();
    return false;
}

bool WKBundlePageCanShowMIMEType(WKBundlePageRef pageRef, WKStringRef mimeTypeRef)
{
    return CyberKit::toImpl(pageRef)->canShowMIMEType(CyberKit::toWTFString(mimeTypeRef));
}

WKRenderingSuppressionToken WKBundlePageExtendIncrementalRenderingSuppression(WKBundlePageRef pageRef)
{
    return CyberKit::toImpl(pageRef)->extendIncrementalRenderingSuppression();
}

void WKBundlePageStopExtendingIncrementalRenderingSuppression(WKBundlePageRef pageRef, WKRenderingSuppressionToken token)
{
    CyberKit::toImpl(pageRef)->stopExtendingIncrementalRenderingSuppression(token);
}

bool WKBundlePageIsUsingEphemeralSession(WKBundlePageRef pageRef)
{
    return CyberKit::toImpl(pageRef)->usesEphemeralSession();
}

bool WKBundlePageIsControlledByAutomation(WKBundlePageRef pageRef)
{
    return CyberKit::toImpl(pageRef)->isControlledByAutomation();
}

#if TARGET_OS_IPHONE
void WKBundlePageSetUseTestingViewportConfiguration(WKBundlePageRef pageRef, bool useTestingViewportConfiguration)
{
    CyberKit::toImpl(pageRef)->setUseTestingViewportConfiguration(useTestingViewportConfiguration);
}
#endif

void WKBundlePageStartMonitoringScrollOperations(WKBundlePageRef pageRef)
{
    CyberKit::WebPage* webPage = CyberKit::toImpl(pageRef);
    CyberCore::Page* page = webPage ? webPage->corePage() : nullptr;
    
    if (!page)
        return;

    page->ensureTestTrigger();
}

void WKBundlePageRegisterScrollOperationCompletionCallback(WKBundlePageRef pageRef, WKBundlePageTestNotificationCallback callback, void* context)
{
    if (!callback)
        return;
    
    CyberKit::WebPage* webPage = CyberKit::toImpl(pageRef);
    CyberCore::Page* page = webPage ? webPage->corePage() : nullptr;
    if (!page || !page->expectsWheelEventTriggers())
        return;
    
    page->ensureTestTrigger().setTestCallbackAndStartNotificationTimer([=]() {
        callback(context);
    });
}

void WKBundlePageCallAfterTasksAndTimers(WKBundlePageRef pageRef, WKBundlePageTestNotificationCallback callback, void* context)
{
    if (!callback)
        return;
    
    CyberKit::WebPage* webPage = CyberKit::toImpl(pageRef);
    CyberCore::Page* page = webPage ? webPage->corePage() : nullptr;
    if (!page)
        return;

    CyberCore::Document* document = page->mainFrame().document();
    if (!document)
        return;

    class TimerOwner {
    public:
        TimerOwner(WTF::Function<void (void*)>&& callback, void* context)
            : m_timer(*this, &TimerOwner::timerFired)
            , m_callback(WTFMove(callback))
            , m_context(context)
        {
            m_timer.startOneShot(0_s);
        }
        
        void timerFired()
        {
            m_callback(m_context);
            delete this;
        }
        
        CyberCore::Timer m_timer;
        WTF::Function<void (void*)> m_callback;
        void* m_context;
    };
    
    document->postTask([=] (CyberCore::ScriptExecutionContext&) {
        new TimerOwner(callback, context); // deletes itself when done.
    });
}

void WKBundlePagePostMessage(WKBundlePageRef pageRef, WKStringRef messageNameRef, WKTypeRef messageBodyRef)
{
    CyberKit::toImpl(pageRef)->postMessage(CyberKit::toWTFString(messageNameRef), CyberKit::toImpl(messageBodyRef));
}

void WKBundlePagePostMessageIgnoringFullySynchronousMode(WKBundlePageRef pageRef, WKStringRef messageNameRef, WKTypeRef messageBodyRef)
{
    CyberKit::toImpl(pageRef)->postMessageIgnoringFullySynchronousMode(CyberKit::toWTFString(messageNameRef), CyberKit::toImpl(messageBodyRef));
}

void WKBundlePagePostSynchronousMessageForTesting(WKBundlePageRef pageRef, WKStringRef messageNameRef, WKTypeRef messageBodyRef, WKTypeRef* returnRetainedDataRef)
{
    CyberKit::WebPage* page = CyberKit::toImpl(pageRef);
    page->layoutIfNeeded();

    RefPtr<API::Object> returnData;
    page->postSynchronousMessageForTesting(CyberKit::toWTFString(messageNameRef), CyberKit::toImpl(messageBodyRef), returnData);
    if (returnRetainedDataRef)
        *returnRetainedDataRef = CyberKit::toAPI(returnData.leakRef());
}

void WKBundlePageAddUserScript(WKBundlePageRef pageRef, WKStringRef source, _WKUserScriptInjectionTime injectionTime, WKUserContentInjectedFrames injectedFrames)
{
    CyberKit::toImpl(pageRef)->addUserScript(CyberKit::toWTFString(source), CyberKit::toUserContentInjectedFrames(injectedFrames), CyberKit::toUserScriptInjectionTime(injectionTime));
}

void WKBundlePageAddUserStyleSheet(WKBundlePageRef pageRef, WKStringRef source, WKUserContentInjectedFrames injectedFrames)
{
    CyberKit::toImpl(pageRef)->addUserStyleSheet(CyberKit::toWTFString(source), CyberKit::toUserContentInjectedFrames(injectedFrames));
}

void WKBundlePageRemoveAllUserContent(WKBundlePageRef pageRef)
{
    CyberKit::toImpl(pageRef)->removeAllUserContent();
}

WKStringRef WKBundlePageCopyGroupIdentifier(WKBundlePageRef pageRef)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(pageRef)->pageGroup()->identifier());
}

void WKBundlePageClearApplicationCache(WKBundlePageRef page)
{
    CyberKit::toImpl(page)->corePage()->applicationCacheStorage().deleteAllEntries();
}

void WKBundlePageClearApplicationCacheForOrigin(WKBundlePageRef page, WKStringRef origin)
{
    CyberKit::toImpl(page)->corePage()->applicationCacheStorage().deleteCacheForOrigin(CyberCore::SecurityOrigin::createFromString(CyberKit::toImpl(origin)->string()));
}

void WKBundlePageSetAppCacheMaximumSize(WKBundlePageRef page, uint64_t size)
{
    CyberKit::toImpl(page)->corePage()->applicationCacheStorage().setMaximumSize(size);
}

uint64_t WKBundlePageGetAppCacheUsageForOrigin(WKBundlePageRef page, WKStringRef origin)
{
    return CyberKit::toImpl(page)->corePage()->applicationCacheStorage().diskUsageForOrigin(CyberCore::SecurityOrigin::createFromString(CyberKit::toImpl(origin)->string()));
}

void WKBundlePageSetApplicationCacheOriginQuota(WKBundlePageRef page, WKStringRef origin, uint64_t bytes)
{
    CyberKit::toImpl(page)->corePage()->applicationCacheStorage().storeUpdatedQuotaForOrigin(CyberCore::SecurityOrigin::createFromString(CyberKit::toImpl(origin)->string()).ptr(), bytes);
}

void WKBundlePageResetApplicationCacheOriginQuota(WKBundlePageRef page, WKStringRef origin)
{
    CyberKit::toImpl(page)->corePage()->applicationCacheStorage().storeUpdatedQuotaForOrigin(CyberCore::SecurityOrigin::createFromString(CyberKit::toImpl(origin)->string()).ptr(), CyberKit::toImpl(page)->corePage()->applicationCacheStorage().defaultOriginQuota());
}

WKArrayRef WKBundlePageCopyOriginsWithApplicationCache(WKBundlePageRef page)
{
    auto origins = CyberKit::toImpl(page)->corePage()->applicationCacheStorage().originsWithCache();

    Vector<RefPtr<API::Object>> originIdentifiers;
    originIdentifiers.reserveInitialCapacity(origins.size());

    for (const auto& origin : origins)
        originIdentifiers.uncheckedAppend(API::String::create(origin->data().databaseIdentifier()));

    return CyberKit::toAPI(&API::Array::create(WTFMove(originIdentifiers)).leakRef());
}

void WKBundlePageSetEventThrottlingBehaviorOverride(WKBundlePageRef page, WKEventThrottlingBehavior* behavior)
{
    Optional<CyberCore::EventThrottlingBehavior> behaviorValue;
    if (behavior) {
        switch (*behavior) {
        case kWKEventThrottlingBehaviorResponsive:
            behaviorValue = CyberCore::EventThrottlingBehavior::Responsive;
            break;
        case kWKEventThrottlingBehaviorUnresponsive:
            behaviorValue = CyberCore::EventThrottlingBehavior::Unresponsive;
            break;
        }
    }

    CyberKit::toImpl(page)->corePage()->setEventThrottlingBehaviorOverride(behaviorValue);
}

void WKBundlePageSetCompositingPolicyOverride(WKBundlePageRef page, WKCompositingPolicy* policy)
{
    Optional<CyberCore::CompositingPolicy> policyValue;
    if (policy) {
        switch (*policy) {
        case kWKCompositingPolicyNormal:
            policyValue = CyberCore::CompositingPolicy::Normal;
            break;
        case kWKCompositingPolicyConservative:
            policyValue = CyberCore::CompositingPolicy::Conservative;
            break;
        }
    }

    CyberKit::toImpl(page)->corePage()->setCompositingPolicyOverride(policyValue);
}

