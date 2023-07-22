/*
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#if ENABLE(DRAG_SUPPORT)

#import <CyberCore/DragClient.h>

@class WebView;

class WebDragClient : public CyberCore::DragClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    WebDragClient(WebView*);

    bool useLegacyDragClient() override;

    void willPerformDragDestinationAction(CyberCore::DragDestinationAction, const CyberCore::DragData&) override;
    void willPerformDragSourceAction(CyberCore::DragSourceAction, const CyberCore::IntPoint&, CyberCore::DataTransfer&) override;
    CyberCore::DragSourceAction dragSourceActionMaskForPoint(const CyberCore::IntPoint& windowPoint) override;
    void startDrag(CyberCore::DragItem, CyberCore::DataTransfer&, CyberCore::Frame&) override;

    void beginDrag(CyberCore::DragItem, CyberCore::Frame&, const CyberCore::IntPoint& mouseDownPosition, const CyberCore::IntPoint& mouseDraggedPosition, CyberCore::DataTransfer&, CyberCore::DragSourceAction) override;

    void declareAndWriteDragImage(const String& pasteboardName, CyberCore::Element&, const URL&, const String&, CyberCore::Frame*) override;
    void didConcludeEditDrag() override;

private:
    WebView* m_webView;
};

#endif // ENABLE(DRAG_SUPPORT)
