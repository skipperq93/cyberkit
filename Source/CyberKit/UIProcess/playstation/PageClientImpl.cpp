/*
 * Copyright (C) 2020 Sony Interactive Entertainment Inc.
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
#include "PageClientImpl.h"

#include "DrawingAreaProxyCoordinatedGraphics.h"
#include "PlayStationWebView.h"
#include "WebPageProxy.h"

namespace CyberKit {

PageClientImpl::PageClientImpl(PlayStationWebView& view)
    : m_view(view)
{
}

// PageClient's pure virtual functions
std::unique_ptr<DrawingAreaProxy> PageClientImpl::createDrawingAreaProxy(WebProcessProxy& processProxy)
{
    return makeUnique<DrawingAreaProxyCoordinatedGraphics>(*m_view.page(), processProxy);
}

void PageClientImpl::setViewNeedsDisplay(const CyberCore::Region& region)
{
}

void PageClientImpl::requestScroll(const CyberCore::FloatPoint& scrollPosition, const CyberCore::IntPoint& scrollOrigin)
{
}

CyberCore::FloatPoint PageClientImpl::viewScrollPosition()
{
    return CyberCore::FloatPoint { };
}

CyberCore::IntSize PageClientImpl::viewSize()
{
    return CyberCore::IntSize { };
}

bool PageClientImpl::isViewWindowActive()
{
    return m_view.isActive();
}

bool PageClientImpl::isViewFocused()
{
    return m_view.isFocused();
}

bool PageClientImpl::isViewVisible()
{
    return m_view.isVisible();
}

bool PageClientImpl::isViewInWindow()
{
    notImplemented();
    return true;
}

void PageClientImpl::processDidExit()
{
}

void PageClientImpl::didRelaunchProcess()
{
}
void PageClientImpl::pageClosed()
{
}

void PageClientImpl::preferencesDidChange()
{
}

void PageClientImpl::toolTipChanged(const String&, const String&)
{
}

void PageClientImpl::didCommitLoadForMainFrame(const String& mimeType, bool useCustomContentProvider)
{
    notImplemented();
}

void PageClientImpl::handleDownloadRequest(DownloadProxy&)
{
}

void PageClientImpl::didChangeContentSize(const CyberCore::IntSize& size)
{
    notImplemented();
}

void PageClientImpl::setCursor(const CyberCore::Cursor& cursor)
{
    notImplemented();
}

void PageClientImpl::setCursorHiddenUntilMouseMoves(bool)
{
}

void PageClientImpl::didChangeViewportProperties(const CyberCore::ViewportAttributes& attributes)
{
}

void PageClientImpl::registerEditCommand(Ref<WebEditCommandProxy>&&, UndoOrRedo)
{
}

void PageClientImpl::clearAllEditCommands()
{
}

bool PageClientImpl::canUndoRedo(UndoOrRedo)
{
    return false;
}

void PageClientImpl::executeUndoRedo(UndoOrRedo)
{
}

void PageClientImpl::wheelEventWasNotHandledByCyberCore(const NativeWebWheelEvent&)
{
}

CyberCore::FloatRect PageClientImpl::convertToDeviceSpace(const CyberCore::FloatRect& rect)
{
    return rect;
}

CyberCore::FloatRect PageClientImpl::convertToUserSpace(const CyberCore::FloatRect& rect)
{
    return rect;
}

CyberCore::IntPoint PageClientImpl::screenToRootView(const CyberCore::IntPoint& point)
{
    return point;
}

CyberCore::IntRect PageClientImpl::rootViewToScreen(const CyberCore::IntRect& rect)
{
    return rect;
}

CyberCore::IntPoint PageClientImpl::accessibilityScreenToRootView(const CyberCore::IntPoint& point)
{
    return screenToRootView(point);
}

CyberCore::IntRect PageClientImpl::rootViewToAccessibilityScreen(const CyberCore::IntRect& rect)
{
    return rootViewToScreen(rect);    
}

void PageClientImpl::doneWithKeyEvent(const NativeWebKeyboardEvent& event, bool wasEventHandled)
{
    notImplemented();
}

RefPtr<WebPopupMenuProxy> PageClientImpl::createPopupMenuProxy(WebPageProxy&  pageProxy)
{
    notImplemented();
    return { };
}

void PageClientImpl::enterAcceleratedCompositingMode(const LayerTreeContext& context)
{
    notImplemented();
}

void PageClientImpl::exitAcceleratedCompositingMode()
{
    notImplemented();
}

void PageClientImpl::updateAcceleratedCompositingMode(const LayerTreeContext&)
{
    notImplemented();
}

#if ENABLE(FULLSCREEN_API)
WebFullScreenManagerProxyClient& PageClientImpl::fullScreenManagerProxyClient()
{
    return *(WebFullScreenManagerProxyClient*)this;
}
#endif

// Custom representations.
void PageClientImpl::didFinishLoadingDataForCustomContentProvider(const String& suggestedFilename, const IPC::DataReference&)
{
}

void PageClientImpl::navigationGestureDidBegin()
{
}

void PageClientImpl::navigationGestureWillEnd(bool willNavigate, WebBackForwardListItem&)
{
}

void PageClientImpl::navigationGestureDidEnd(bool willNavigate, WebBackForwardListItem&)
{
}

void PageClientImpl::navigationGestureDidEnd()
{
}

void PageClientImpl::willRecordNavigationSnapshot(WebBackForwardListItem&)
{
}

void PageClientImpl::didRemoveNavigationGestureSnapshot()
{
}

void PageClientImpl::didFirstVisuallyNonEmptyLayoutForMainFrame()
{
}

void PageClientImpl::didFinishNavigation(API::Navigation*)
{
}

void PageClientImpl::didFailNavigation(API::Navigation*)
{
}

void PageClientImpl::didSameDocumentNavigationForMainFrame(SameDocumentNavigationType)
{
}

void PageClientImpl::didChangeBackgroundColor()
{
}

void PageClientImpl::isPlayingAudioWillChange()
{
}

void PageClientImpl::isPlayingAudioDidChange()
{
}

void PageClientImpl::refView()
{
}

void PageClientImpl::derefView()
{
}

void PageClientImpl::didRestoreScrollPosition()
{
}

CyberCore::UserInterfaceLayoutDirection PageClientImpl::userInterfaceLayoutDirection()
{
    return CyberCore::UserInterfaceLayoutDirection::LTR;
}

void PageClientImpl::requestDOMPasteAccess(const CyberCore::IntRect&, const String&, CompletionHandler<void(CyberCore::DOMPasteAccessResponse)>&& completionHandler)
{
    completionHandler(CyberCore::DOMPasteAccessResponse::DeniedForGesture);
}

} // namespace CyberKit
