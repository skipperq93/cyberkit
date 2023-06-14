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

#ifndef WebDragClient_h
#define WebDragClient_h

#if ENABLE(DRAG_SUPPORT)

#include <CyberCore/DragClient.h>

namespace CyberKit {

class WebPage;

class WebDragClient : public CyberCore::DragClient {
public:
    WebDragClient(WebPage* page)
        : m_page(page)
    {
    }

private:
    void willPerformDragDestinationAction(CyberCore::DragDestinationAction, const CyberCore::DragData&) override;
    void willPerformDragSourceAction(CyberCore::DragSourceAction, const CyberCore::IntPoint&, CyberCore::DataTransfer&) override;
    CyberCore::DragSourceAction dragSourceActionMaskForPoint(const CyberCore::IntPoint& windowPoint) override;

    void startDrag(CyberCore::DragItem, CyberCore::DataTransfer&, CyberCore::Frame&) override;
    void didConcludeEditDrag() override;

#if PLATFORM(COCOA)
    void declareAndWriteDragImage(const String& pasteboardName, CyberCore::Element&, const URL&, const String&, CyberCore::Frame*) override;
#endif

    void dragControllerDestroyed() override;

    WebPage* m_page;
};

} // namespace CyberKit

#endif // ENABLE(DRAG_SUPPORT)

#endif // WebDragClient_h
