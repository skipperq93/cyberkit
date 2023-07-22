/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
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

#ifndef FindIndicatorOverlayClientIOS_h
#define FindIndicatorOverlayClientIOS_h

#import <CyberCore/Frame.h>
#import <CyberCore/GraphicsContext.h>
#import <CyberCore/PageOverlay.h>
#import <CyberCore/TextIndicator.h>

namespace CyberKit {

class FindIndicatorOverlayClientIOS : public CyberCore::PageOverlay::Client {
    WTF_MAKE_FAST_ALLOCATED;
public:
    FindIndicatorOverlayClientIOS(CyberCore::Frame& frame, CyberCore::TextIndicator* textIndicator)
        : m_frame(frame)
        , m_textIndicator(textIndicator)
    {
    }

private:
    void willMoveToPage(CyberCore::PageOverlay&, CyberCore::Page*) override { }
    void didMoveToPage(CyberCore::PageOverlay&, CyberCore::Page*) override { }
    void drawRect(CyberCore::PageOverlay&, CyberCore::GraphicsContext&, const CyberCore::IntRect& dirtyRect) override;
    bool mouseEvent(CyberCore::PageOverlay&, const CyberCore::PlatformMouseEvent&) override { return false; }

    CyberCore::Frame& m_frame;
    RefPtr<CyberCore::TextIndicator> m_textIndicator;
};

} // namespace CyberKit

#endif // FindIndicatorOverlayClientIOS_h
