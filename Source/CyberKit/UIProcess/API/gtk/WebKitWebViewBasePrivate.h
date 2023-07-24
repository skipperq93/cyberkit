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
#include "GestureController.h"
#include "InputMethodState.h"
#include "SameDocumentNavigationType.h"
#include "ShareableBitmap.h"
#include "ViewGestureController.h"
#include "ViewSnapshotStore.h"
#include "WebContextMenuProxyGtk.h"
#include "WebHitTestResultData.h"
#include "WebInspectorProxy.h"
#include "CyberKitInputMethodContext.h"
#include "CyberKitWebViewBase.h"
#include "WebPageProxy.h"
#include <CyberCore/DragActions.h>
#include <CyberCore/SelectionData.h>
#include <wtf/Optional.h>

CyberKitWebViewBase* webkitWebViewBaseCreate(const API::PageConfiguration&);
CyberKit::WebPageProxy* webkitWebViewBaseGetPage(CyberKitWebViewBase*);
void webkitWebViewBaseCreateWebPage(CyberKitWebViewBase*, Ref<API::PageConfiguration>&&);
void webkitWebViewBaseSetTooltipText(CyberKitWebViewBase*, const char*);
void webkitWebViewBaseSetTooltipArea(CyberKitWebViewBase*, const CyberCore::IntRect&);
void webkitWebViewBaseSetMouseIsOverScrollbar(CyberKitWebViewBase*, CyberKit::WebHitTestResultData::IsScrollbar);
void webkitWebViewBaseForwardNextKeyEvent(CyberKitWebViewBase*);
void webkitWebViewBaseForwardNextWheelEvent(CyberKitWebViewBase*);
void webkitWebViewBaseChildMoveResize(CyberKitWebViewBase*, GtkWidget*, const CyberCore::IntRect&);
void webkitWebViewBaseEnterFullScreen(CyberKitWebViewBase*);
void webkitWebViewBaseExitFullScreen(CyberKitWebViewBase*);
bool webkitWebViewBaseIsFullScreen(CyberKitWebViewBase*);
void webkitWebViewBaseSetInspectorViewSize(CyberKitWebViewBase*, unsigned size);
void webkitWebViewBaseSetActiveContextMenuProxy(CyberKitWebViewBase*, CyberKit::WebContextMenuProxyGtk*);
CyberKit::WebContextMenuProxyGtk* webkitWebViewBaseGetActiveContextMenuProxy(CyberKitWebViewBase*);
GdkEvent* webkitWebViewBaseTakeContextMenuEvent(CyberKitWebViewBase*);
void webkitWebViewBaseSetInputMethodState(CyberKitWebViewBase*, Optional<CyberKit::InputMethodState>&&);
void webkitWebViewBaseUpdateTextInputState(CyberKitWebViewBase*);
void webkitWebViewBaseSetContentsSize(CyberKitWebViewBase*, const CyberCore::IntSize&);

void webkitWebViewBaseSetFocus(CyberKitWebViewBase*, bool focused);
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
void webkitWebViewBaseStartDrag(CyberKitWebViewBase*, CyberCore::SelectionData&&, OptionSet<CyberCore::DragOperation>, RefPtr<CyberKit::ShareableBitmap>&&);
void webkitWebViewBaseDidPerformDragControllerAction(CyberKitWebViewBase*);
#endif

#if !USE(GTK4)
CyberKit::GestureController& webkitWebViewBaseGestureController(CyberKitWebViewBase*);
#endif

RefPtr<CyberKit::ViewSnapshot> webkitWebViewBaseTakeViewSnapshot(CyberKitWebViewBase*, Optional<CyberCore::IntRect>&&);
void webkitWebViewBaseSetEnableBackForwardNavigationGesture(CyberKitWebViewBase*, bool enabled);
#if !USE(GTK4)
CyberKit::ViewGestureController* webkitWebViewBaseViewGestureController(CyberKitWebViewBase*);
#endif

bool webkitWebViewBaseBeginBackSwipeForTesting(CyberKitWebViewBase*);
bool webkitWebViewBaseCompleteBackSwipeForTesting(CyberKitWebViewBase*);

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
void webkitWebViewBaseSynthesizeCompositionKeyPress(CyberKitWebViewBase*, const String& text, Optional<Vector<CyberCore::CompositionUnderline>>&&, Optional<CyberKit::EditingRange>&&);
