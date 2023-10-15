/*
 * Copyright (C) 2014 Igalia S.L.
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

#include "APIViewClient.h"
#include "DrawingAreaProxyCoordinatedGraphics.h"
#include "NativeWebMouseEvent.h"
#include "NativeWebTouchEvent.h"
#include "NativeWebWheelEvent.h"
#include "TouchGestureController.h"
#include "WPEView.h"
#include "WebContextMenuProxy.h"
#include "WebContextMenuProxyWPE.h"
#include "CyberKitPopupMenu.h"
#include <CyberCore/ActivityState.h>
#include <CyberCore/DOMPasteAccess.h>
#include <CyberCore/NotImplemented.h>

#if ENABLE(ACCESSIBILITY)
#include <atk/atk.h>
#endif

namespace CyberKit {

PageClientImpl::PageClientImpl(WKWPE::View& view)
    : m_view(view)
{
}

PageClientImpl::~PageClientImpl() = default;

struct wpe_view_backend* PageClientImpl::viewBackend()
{
    return m_view.backend();
}

UnixFileDescriptor PageClientImpl::hostFileDescriptor()
{
    return UnixFileDescriptor { wpe_view_backend_get_renderer_host_fd(m_view.backend()), UnixFileDescriptor::Adopt };
}

std::unique_ptr<DrawingAreaProxy> PageClientImpl::createDrawingAreaProxy(WebProcessProxy& process)
{
    return makeUnique<DrawingAreaProxyCoordinatedGraphics>(m_view.page());
}

void PageClientImpl::setViewNeedsDisplay(const CyberCore::Region&)
{
}

void PageClientImpl::requestScroll(const CyberCore::FloatPoint&, const CyberCore::IntPoint&, CyberCore::ScrollIsAnimated)
{
}

CyberCore::FloatPoint PageClientImpl::viewScrollPosition()
{
    return { };
}

CyberCore::IntSize PageClientImpl::viewSize()
{
    return m_view.size();
}

bool PageClientImpl::isViewWindowActive()
{
    return m_view.viewState().contains(CyberCore::ActivityState::WindowIsActive);
}

bool PageClientImpl::isViewFocused()
{
    return m_view.viewState().contains(CyberCore::ActivityState::IsFocused);
}

bool PageClientImpl::isViewVisible()
{
    return m_view.viewState().contains(CyberCore::ActivityState::IsVisible);
}

bool PageClientImpl::isViewInWindow()
{
    return m_view.viewState().contains(CyberCore::ActivityState::IsInWindow);
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

void PageClientImpl::didCommitLoadForMainFrame(const String&, bool)
{
}

void PageClientImpl::didChangeContentSize(const CyberCore::IntSize&)
{
}

void PageClientImpl::setCursor(const CyberCore::Cursor&)
{
}

void PageClientImpl::setCursorHiddenUntilMouseMoves(bool)
{
}

void PageClientImpl::didChangeViewportProperties(const CyberCore::ViewportAttributes&)
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

void PageClientImpl::doneWithKeyEvent(const NativeWebKeyboardEvent&, bool)
{
}

#if ENABLE(TOUCH_EVENTS)
void PageClientImpl::doneWithTouchEvent(const NativeWebTouchEvent& touchEvent, bool wasEventHandled)
{
    if (wasEventHandled)
        return;

    const struct wpe_input_touch_event_raw* touchPoint = touchEvent.nativeFallbackTouchPoint();
    if (touchPoint->type == wpe_input_touch_event_type_null)
        return;

    auto& page = m_view.page();
    auto& touchGestureController = m_view.touchGestureController();

    auto generatedEvent = touchGestureController.handleEvent(touchPoint);
    WTF::switchOn(generatedEvent,
        [](TouchGestureController::NoEvent&) { },
        [&](TouchGestureController::ClickEvent& clickEvent)
        {
            auto* event = &clickEvent.event;

            // Mouse motion towards the point of the click.
            event->type = wpe_input_pointer_event_type_motion;
            page.handleMouseEvent(NativeWebMouseEvent(event, page.deviceScaleFactor()));

            event->type = wpe_input_pointer_event_type_button;
            event->button = 1;

            // Mouse down on the point of the click.
            event->state = 1;
            event->modifiers |= wpe_input_pointer_modifier_button1;
            page.handleMouseEvent(NativeWebMouseEvent(event, page.deviceScaleFactor()));

            // Mouse up on the same location.
            event->state = 0;
            event->modifiers &= ~wpe_input_pointer_modifier_button1;
            page.handleMouseEvent(NativeWebMouseEvent(event, page.deviceScaleFactor()));
        },
        [&](TouchGestureController::ContextMenuEvent&) {
            // FIXME: Generate contextmenuevent without accidentally generating mouseup/mousedown events
        },
        [](TouchGestureController::AxisEvent&) { });
}
#endif

void PageClientImpl::wheelEventWasNotHandledByCyberCore(const NativeWebWheelEvent&)
{
}

RefPtr<WebPopupMenuProxy> PageClientImpl::createPopupMenuProxy(WebPageProxy& page)
{
    if (!m_view.client().isGLibBasedAPI())
        return nullptr;
    return CyberKitPopupMenu::create(m_view, page.popupMenuClient());
}

#if ENABLE(CONTEXT_MENUS)
Ref<WebContextMenuProxy> PageClientImpl::createContextMenuProxy(WebPageProxy& page, ContextMenuContextData&& context, const UserData& userData)
{
    return WebContextMenuProxyWPE::create(page, WTFMove(context), userData);
}
#endif

void PageClientImpl::enterAcceleratedCompositingMode(const LayerTreeContext&)
{
}

void PageClientImpl::exitAcceleratedCompositingMode()
{
}

void PageClientImpl::updateAcceleratedCompositingMode(const LayerTreeContext&)
{
}

void PageClientImpl::didFinishLoadingDataForCustomContentProvider(const String&, const IPC::DataReference&)
{
}

void PageClientImpl::navigationGestureDidBegin()
{
}

void PageClientImpl::navigationGestureWillEnd(bool, WebBackForwardListItem&)
{
}

void PageClientImpl::navigationGestureDidEnd(bool, WebBackForwardListItem&)
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

void PageClientImpl::didStartProvisionalLoadForMainFrame()
{
    m_view.willStartLoad();
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

#if ENABLE(FULLSCREEN_API)
WebFullScreenManagerProxyClient& PageClientImpl::fullScreenManagerProxyClient()
{
    return *this;
}

void PageClientImpl::closeFullScreenManager()
{
    notImplemented();
}

bool PageClientImpl::isFullScreen()
{
    return m_view.isFullScreen();
}

void PageClientImpl::enterFullScreen()
{
    if (isFullScreen())
        return;

    WebFullScreenManagerProxy* fullScreenManagerProxy = m_view.page().fullScreenManager();
    if (fullScreenManagerProxy) {
        fullScreenManagerProxy->willEnterFullScreen();
        if (!m_view.setFullScreen(true))
            fullScreenManagerProxy->didExitFullScreen();
    }
}

void PageClientImpl::exitFullScreen()
{
    if (!isFullScreen())
        return;

    WebFullScreenManagerProxy* fullScreenManagerProxy = m_view.page().fullScreenManager();
    if (fullScreenManagerProxy) {
        fullScreenManagerProxy->willExitFullScreen();
        if (!m_view.setFullScreen(false))
            fullScreenManagerProxy->didEnterFullScreen();

    }
}

void PageClientImpl::beganEnterFullScreen(const CyberCore::IntRect& /* initialFrame */, const CyberCore::IntRect& /* finalFrame */)
{
    notImplemented();
}

void PageClientImpl::beganExitFullScreen(const CyberCore::IntRect& /* initialFrame */, const CyberCore::IntRect& /* finalFrame */)
{
    notImplemented();
}

#endif // ENABLE(FULLSCREEN_API)

void PageClientImpl::requestDOMPasteAccess(CyberCore::DOMPasteAccessCategory, const CyberCore::IntRect&, const String&, CompletionHandler<void(CyberCore::DOMPasteAccessResponse)>&& completionHandler)
{
    completionHandler(CyberCore::DOMPasteAccessResponse::DeniedForGesture);
}

#if ENABLE(ACCESSIBILITY)
AtkObject* PageClientImpl::accessible()
{
    return ATK_OBJECT(m_view.accessible());
}
#endif

void PageClientImpl::didChangeWebPageID() const
{
    m_view.didChangePageID();
}

void PageClientImpl::sendMessageToWebView(UserMessage&& message, CompletionHandler<void(UserMessage&&)>&& completionHandler)
{
    m_view.didReceiveUserMessage(WTFMove(message), WTFMove(completionHandler));
}

void PageClientImpl::setInputMethodState(std::optional<InputMethodState>&& state)
{
    m_view.setInputMethodState(WTFMove(state));
}

void PageClientImpl::selectionDidChange()
{
    m_view.selectionDidChange();
}

CyberKitWebResourceLoadManager* PageClientImpl::webResourceLoadManager()
{
    return m_view.webResourceLoadManager();
}

} // namespace CyberKit
