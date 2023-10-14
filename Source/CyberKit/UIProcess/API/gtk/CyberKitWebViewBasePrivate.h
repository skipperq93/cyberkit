/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 * Portions Copyright (c) 2010 Motorola Mobility, Inc.  All rights reserved.
 * Copyright (C) 2011 Igalia S.L.
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

#pragma once

#include "APIPageConfiguration.h"
#include "InputMethodState.h"
#include "SameDocumentNavigationType.h"
#include "ShareableBitmap.h"
#include "ViewGestureController.h"
#include "ViewSnapshotStore.h"
#include "WebContextMenuProxyGtk.h"
#include "WebHitTestResultData.h"
#include "WebInspectorUIProxy.h"
#include "CyberKitInputMethodContext.h"
#include "CyberKitWebViewBase.h"
#include "CyberKitWebViewBaseInternal.h"
#include "WebPageProxy.h"
#include <CyberCore/DragActions.h>
#include <CyberCore/GRefPtrGtk.h>
#include <CyberCore/GUniquePtrGtk.h>
#include <CyberCore/SelectionData.h>

CyberKitWebViewBase* webkitWebViewBaseCreate(const API::PageConfiguration&);
CyberKit::WebPageProxy* webkitWebViewBaseGetPage(CyberKitWebViewBase*);
void webkitWebViewBaseCreateWebPage(CyberKitWebViewBase*, Ref<API::PageConfiguration>&&);
void webkitWebViewBaseSetTooltipText(CyberKitWebViewBase*, const char*);
void webkitWebViewBaseSetTooltipArea(CyberKitWebViewBase*, const CyberCore::IntRect&);
void webkitWebViewBaseSetMouseIsOverScrollbar(CyberKitWebViewBase*, CyberKit::WebHitTestResultData::IsScrollbar);
void webkitWebViewBasePropagateKeyEvent(CyberKitWebViewBase*, GdkEvent*);
void webkitWebViewBasePropagateWheelEvent(CyberKitWebViewBase*, GdkEvent*);
void webkitWebViewBaseChildMoveResize(CyberKitWebViewBase*, GtkWidget*, const CyberCore::IntRect&);
void webkitWebViewBaseEnterFullScreen(CyberKitWebViewBase*);
void webkitWebViewBaseExitFullScreen(CyberKitWebViewBase*);
bool webkitWebViewBaseIsFullScreen(CyberKitWebViewBase*);
void webkitWebViewBaseSetInspectorViewSize(CyberKitWebViewBase*, unsigned size);
void webkitWebViewBaseSetActiveContextMenuProxy(CyberKitWebViewBase*, CyberKit::WebContextMenuProxyGtk*);
CyberKit::WebContextMenuProxyGtk* webkitWebViewBaseGetActiveContextMenuProxy(CyberKitWebViewBase*);

#if USE(GTK4)
GRefPtr<GdkEvent> webkitWebViewBaseTakeContextMenuEvent(CyberKitWebViewBase*);
#else
GUniquePtr<GdkEvent> webkitWebViewBaseTakeContextMenuEvent(CyberKitWebViewBase*);
#endif

void webkitWebViewBaseSetInputMethodState(CyberKitWebViewBase*, std::optional<CyberKit::InputMethodState>&&);
void webkitWebViewBaseUpdateTextInputState(CyberKitWebViewBase*);
void webkitWebViewBaseSetContentsSize(CyberKitWebViewBase*, const CyberCore::IntSize&);

void webkitWebViewBaseSetFocus(CyberKitWebViewBase*, bool focused);
void webkitWebViewBaseSetEditable(CyberKitWebViewBase*, bool editable);
CyberCore::IntSize webkitWebViewBaseGetViewSize(CyberKitWebViewBase*);
bool webkitWebViewBaseIsInWindowActive(CyberKitWebViewBase*);
bool webkitWebViewBaseIsFocused(CyberKitWebViewBase*);
bool webkitWebViewBaseIsVisible(CyberKitWebViewBase*);
bool webkitWebViewBaseIsInWindow(CyberKitWebViewBase*);

void webkitWebViewBaseAddDialog(CyberKitWebViewBase*, GtkWidget*);
void webkitWebViewBaseAddWebInspector(CyberKitWebViewBase*, GtkWidget* inspector, CyberKit::AttachmentSide);
void webkitWebViewBaseRemoveWebInspector(CyberKitWebViewBase*, GtkWidget*);
void webkitWebViewBaseResetClickCounter(CyberKitWebViewBase*);
void webkitWebViewBaseEnterAcceleratedCompositingMode(CyberKitWebViewBase*, const CyberKit::LayerTreeContext&);
void webkitWebViewBaseUpdateAcceleratedCompositingMode(CyberKitWebViewBase*, const CyberKit::LayerTreeContext&);
void webkitWebViewBaseExitAcceleratedCompositingMode(CyberKitWebViewBase*);
bool webkitWebViewBaseMakeGLContextCurrent(CyberKitWebViewBase*);
void webkitWebViewBaseWillSwapWebProcess(CyberKitWebViewBase*);
void webkitWebViewBaseDidExitWebProcess(CyberKitWebViewBase*);
void webkitWebViewBaseDidRelaunchWebProcess(CyberKitWebViewBase*);
void webkitWebViewBasePageClosed(CyberKitWebViewBase*);

#if ENABLE(DRAG_SUPPORT)
void webkitWebViewBaseStartDrag(CyberKitWebViewBase*, CyberCore::SelectionData&&, OptionSet<CyberCore::DragOperation>, RefPtr<CyberKit::ShareableBitmap>&&, CyberCore::IntPoint&& dragImageHotspot);
void webkitWebViewBaseDidPerformDragControllerAction(CyberKitWebViewBase*);
#endif

RefPtr<CyberKit::ViewSnapshot> webkitWebViewBaseTakeViewSnapshot(CyberKitWebViewBase*, std::optional<CyberCore::IntRect>&&);
void webkitWebViewBaseSetEnableBackForwardNavigationGesture(CyberKitWebViewBase*, bool enabled);
CyberKit::ViewGestureController* webkitWebViewBaseViewGestureController(CyberKitWebViewBase*);

bool webkitWebViewBaseBeginBackSwipeForTesting(CyberKitWebViewBase*);
bool webkitWebViewBaseCompleteBackSwipeForTesting(CyberKitWebViewBase*);

GVariant* webkitWebViewBaseContentsOfUserInterfaceItem(CyberKitWebViewBase*, const char* userInterfaceItem);

void webkitWebViewBaseDidStartProvisionalLoadForMainFrame(CyberKitWebViewBase*);
void webkitWebViewBaseDidFirstVisuallyNonEmptyLayoutForMainFrame(CyberKitWebViewBase*);
void webkitWebViewBaseDidFinishNavigation(CyberKitWebViewBase*, API::Navigation*);
void webkitWebViewBaseDidFailNavigation(CyberKitWebViewBase*, API::Navigation*);
void webkitWebViewBaseDidSameDocumentNavigationForMainFrame(CyberKitWebViewBase*, CyberKit::SameDocumentNavigationType);
void webkitWebViewBaseDidRestoreScrollPosition(CyberKitWebViewBase*);

void webkitWebViewBaseShowEmojiChooser(CyberKitWebViewBase*, const CyberCore::IntRect&, CompletionHandler<void(String)>&&);

#if USE(WPE_RENDERER)
int webkitWebViewBaseRenderHostFileDescriptor(CyberKitWebViewBase*);
#endif

void webkitWebViewBaseRequestPointerLock(CyberKitWebViewBase*);
void webkitWebViewBaseDidLosePointerLock(CyberKitWebViewBase*);

void webkitWebViewBaseSetInputMethodContext(CyberKitWebViewBase*, CyberKitInputMethodContext*);
CyberKitInputMethodContext* webkitWebViewBaseGetInputMethodContext(CyberKitWebViewBase*);
void webkitWebViewBaseSynthesizeCompositionKeyPress(CyberKitWebViewBase*, const String& text, std::optional<Vector<CyberCore::CompositionUnderline>>&&, std::optional<CyberKit::EditingRange>&&);
void webkitWebViewBaseSynthesizeWheelEvent(CyberKitWebViewBase*, const GdkEvent*, double deltaX, double deltaY, int x, int y, WheelEventPhase, WheelEventPhase momentumPhase, bool hasPreciseDeltas);

void webkitWebViewBaseMakeBlank(CyberKitWebViewBase*, bool);
void webkitWebViewBasePageGrabbedTouch(CyberKitWebViewBase*);
void webkitWebViewBaseSetShouldNotifyFocusEvents(CyberKitWebViewBase*, bool);
