/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
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
#include "WKBundleFrame.h"
#include "WKBundleFramePrivate.h"

#include "APIArray.h"
#include "APISecurityOrigin.h"
#include "InjectedBundleHitTestResult.h"
#include "InjectedBundleNodeHandle.h"
#include "InjectedBundleRangeHandle.h"
#include "InjectedBundleScriptWorld.h"
#include "WKAPICast.h"
#include "WKBundleAPICast.h"
#include "WKData.h"
#include "WebFrame.h"
#include "WebPage.h"
#include <CyberCore/Document.h>
#include <CyberCore/FocusController.h>
#include <CyberCore/Frame.h>
#include <CyberCore/FrameLoader.h>
#include <CyberCore/FrameView.h>
#include <CyberCore/Page.h>

WKTypeID WKBundleFrameGetTypeID()
{
    return CyberKit::toAPI(CyberKit::WebFrame::APIType);
}

bool WKBundleFrameIsMainFrame(WKBundleFrameRef frameRef)
{
    return CyberKit::toImpl(frameRef)->isMainFrame();
}

WKBundleFrameRef WKBundleFrameGetParentFrame(WKBundleFrameRef frameRef)
{
    return toAPI(CyberKit::toImpl(frameRef)->parentFrame());
}

WKURLRef WKBundleFrameCopyURL(WKBundleFrameRef frameRef)
{
    return CyberKit::toCopiedURLAPI(CyberKit::toImpl(frameRef)->url());
}

WKURLRef WKBundleFrameCopyProvisionalURL(WKBundleFrameRef frameRef)
{
    return CyberKit::toCopiedURLAPI(CyberKit::toImpl(frameRef)->provisionalURL());
}

WKFrameLoadState WKBundleFrameGetFrameLoadState(WKBundleFrameRef frameRef)
{
    CyberCore::Frame* coreFrame = CyberKit::toImpl(frameRef)->coreFrame();
    if (!coreFrame)
        return kWKFrameLoadStateFinished;

    switch (coreFrame->loader().state()) {
    case CyberCore::FrameStateProvisional:
        return kWKFrameLoadStateProvisional;
    case CyberCore::FrameStateCommittedPage:
        return kWKFrameLoadStateCommitted;
    case CyberCore::FrameStateComplete:
        return kWKFrameLoadStateFinished;
    }

    ASSERT_NOT_REACHED();
    return kWKFrameLoadStateFinished;
}

WKArrayRef WKBundleFrameCopyChildFrames(WKBundleFrameRef frameRef)
{
    return CyberKit::toAPI(&CyberKit::toImpl(frameRef)->childFrames().leakRef());    
}

JSGlobalContextRef WKBundleFrameGetJavaScriptContext(WKBundleFrameRef frameRef)
{
    return CyberKit::toImpl(frameRef)->jsContext();
}

WKBundleFrameRef WKBundleFrameForJavaScriptContext(JSContextRef context)
{
    return toAPI(CyberKit::WebFrame::frameForContext(context));
}

JSGlobalContextRef WKBundleFrameGetJavaScriptContextForWorld(WKBundleFrameRef frameRef, WKBundleScriptWorldRef worldRef)
{
    return CyberKit::toImpl(frameRef)->jsContextForWorld(CyberKit::toImpl(worldRef));
}

JSValueRef WKBundleFrameGetJavaScriptWrapperForNodeForWorld(WKBundleFrameRef frameRef, WKBundleNodeHandleRef nodeHandleRef, WKBundleScriptWorldRef worldRef)
{
    return CyberKit::toImpl(frameRef)->jsWrapperForWorld(CyberKit::toImpl(nodeHandleRef), CyberKit::toImpl(worldRef));
}

JSValueRef WKBundleFrameGetJavaScriptWrapperForRangeForWorld(WKBundleFrameRef frameRef, WKBundleRangeHandleRef rangeHandleRef, WKBundleScriptWorldRef worldRef)
{
    return CyberKit::toImpl(frameRef)->jsWrapperForWorld(CyberKit::toImpl(rangeHandleRef), CyberKit::toImpl(worldRef));
}

WKStringRef WKBundleFrameCopyName(WKBundleFrameRef frameRef)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(frameRef)->name());
}

WKStringRef WKBundleFrameCopyCounterValue(WKBundleFrameRef frameRef, JSObjectRef element)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(frameRef)->counterValue(element));
}

WKStringRef WKBundleFrameCopyInnerText(WKBundleFrameRef frameRef)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(frameRef)->innerText());
}

unsigned WKBundleFrameGetPendingUnloadCount(WKBundleFrameRef frameRef)
{
    return CyberKit::toImpl(frameRef)->pendingUnloadCount();
}

WKBundlePageRef WKBundleFrameGetPage(WKBundleFrameRef frameRef)
{
    return toAPI(CyberKit::toImpl(frameRef)->page());
}

void WKBundleFrameClearOpener(WKBundleFrameRef frameRef)
{
    CyberCore::Frame* coreFrame = CyberKit::toImpl(frameRef)->coreFrame();
    if (coreFrame)
        coreFrame->loader().setOpener(0);
}

void WKBundleFrameStopLoading(WKBundleFrameRef frameRef)
{
    CyberKit::toImpl(frameRef)->stopLoading();
}

WKStringRef WKBundleFrameCopyLayerTreeAsText(WKBundleFrameRef frameRef)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(frameRef)->layerTreeAsText());
}

bool WKBundleFrameAllowsFollowingLink(WKBundleFrameRef frameRef, WKURLRef urlRef)
{
    return CyberKit::toImpl(frameRef)->allowsFollowingLink(URL(URL(), CyberKit::toWTFString(urlRef)));
}

bool WKBundleFrameHandlesPageScaleGesture(WKBundleFrameRef frameRef)
{
    return CyberKit::toImpl(frameRef)->handlesPageScaleGesture();
}

WKRect WKBundleFrameGetContentBounds(WKBundleFrameRef frameRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(frameRef)->contentBounds());
}

WKRect WKBundleFrameGetVisibleContentBounds(WKBundleFrameRef frameRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(frameRef)->visibleContentBounds());
}

WKRect WKBundleFrameGetVisibleContentBoundsExcludingScrollbars(WKBundleFrameRef frameRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(frameRef)->visibleContentBoundsExcludingScrollbars());
}

WKSize WKBundleFrameGetScrollOffset(WKBundleFrameRef frameRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(frameRef)->scrollOffset());
}

bool WKBundleFrameHasHorizontalScrollbar(WKBundleFrameRef frameRef)
{
    return CyberKit::toImpl(frameRef)->hasHorizontalScrollbar();
}

bool WKBundleFrameHasVerticalScrollbar(WKBundleFrameRef frameRef)
{
    return CyberKit::toImpl(frameRef)->hasVerticalScrollbar();
}

bool WKBundleFrameGetDocumentBackgroundColor(WKBundleFrameRef frameRef, double* red, double* green, double* blue, double* alpha)
{
    return CyberKit::toImpl(frameRef)->getDocumentBackgroundColor(red, green, blue, alpha);
}

WKStringRef WKBundleFrameCopySuggestedFilenameForResourceWithURL(WKBundleFrameRef frameRef, WKURLRef urlRef)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(frameRef)->suggestedFilenameForResourceWithURL(URL(URL(), CyberKit::toWTFString(urlRef))));
}

WKStringRef WKBundleFrameCopyMIMETypeForResourceWithURL(WKBundleFrameRef frameRef, WKURLRef urlRef)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(frameRef)->mimeTypeForResourceWithURL(URL(URL(), CyberKit::toWTFString(urlRef))));
}

bool WKBundleFrameContainsAnyFormElements(WKBundleFrameRef frameRef)
{
    return CyberKit::toImpl(frameRef)->containsAnyFormElements();
}

bool WKBundleFrameContainsAnyFormControls(WKBundleFrameRef frameRef)
{
    return CyberKit::toImpl(frameRef)->containsAnyFormControls();
}

void WKBundleFrameSetTextDirection(WKBundleFrameRef frameRef, WKStringRef directionRef)
{
    CyberKit::toImpl(frameRef)->setTextDirection(CyberKit::toWTFString(directionRef));
}

void WKBundleFrameSetAccessibleName(WKBundleFrameRef frameRef, WKStringRef accessibleNameRef)
{
    CyberKit::toImpl(frameRef)->setAccessibleName(CyberKit::toWTFString(accessibleNameRef));
}

WKDataRef WKBundleFrameCopyWebArchive(WKBundleFrameRef frameRef)
{
    return WKBundleFrameCopyWebArchiveFilteringSubframes(frameRef, 0, 0);
}

WKDataRef WKBundleFrameCopyWebArchiveFilteringSubframes(WKBundleFrameRef frameRef, WKBundleFrameFrameFilterCallback frameFilterCallback, void* context)
{
#if PLATFORM(COCOA)
    RetainPtr<CFDataRef> data = CyberKit::toImpl(frameRef)->webArchiveData(frameFilterCallback, context);
    if (data)
        return WKDataCreate(CFDataGetBytePtr(data.get()), CFDataGetLength(data.get()));
#else
    UNUSED_PARAM(frameRef);
    UNUSED_PARAM(frameFilterCallback);
    UNUSED_PARAM(context);
#endif
    
    return 0;
}

bool WKBundleFrameCallShouldCloseOnWebView(WKBundleFrameRef frameRef)
{
    CyberCore::Frame* coreFrame = CyberKit::toImpl(frameRef)->coreFrame();
    if (!coreFrame)
        return true;

    return coreFrame->loader().shouldClose();
}

WKBundleHitTestResultRef WKBundleFrameCreateHitTestResult(WKBundleFrameRef frameRef, WKPoint point)
{
    return CyberKit::toAPI(CyberKit::toImpl(frameRef)->hitTest(CyberKit::toIntPoint(point)).leakRef());
}

WKSecurityOriginRef WKBundleFrameCopySecurityOrigin(WKBundleFrameRef frameRef)
{
    CyberCore::Frame* coreFrame = CyberKit::toImpl(frameRef)->coreFrame();
    if (!coreFrame)
        return 0;

    return CyberKit::toCopiedAPI(&coreFrame->document()->securityOrigin());
}

void WKBundleFrameFocus(WKBundleFrameRef frameRef)
{
    CyberCore::Frame* coreFrame = CyberKit::toImpl(frameRef)->coreFrame();
    if (!coreFrame)
        return;

    coreFrame->page()->focusController().setFocusedFrame(coreFrame);
}
