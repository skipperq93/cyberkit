/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
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

#import <CyberCore/DragActions.h>
#import <CyberKit/WKDragDestinationAction.h>

namespace WebKit {

inline OptionSet<CyberCore::DragDestinationAction> coreDragDestinationActionMask(WKDragDestinationAction action)
{
    OptionSet<CyberCore::DragDestinationAction> result;
    if (action & WKDragDestinationActionDHTML)
        result.add(CyberCore::DragDestinationAction::DHTML);
    if (action & WKDragDestinationActionEdit)
        result.add(CyberCore::DragDestinationAction::Edit);
    if (action & WKDragDestinationActionLoad)
        result.add(CyberCore::DragDestinationAction::Load);
    return result;
}

#if USE(APPKIT)
inline OptionSet<CyberCore::DragOperation> coreDragOperationMask(NSDragOperation operation)
{
    OptionSet<CyberCore::DragOperation> result;
    if (operation & NSDragOperationCopy)
        result.add(CyberCore::DragOperation::Copy);
    if (operation & NSDragOperationLink)
        result.add(CyberCore::DragOperation::Link);
    if (operation & NSDragOperationGeneric)
        result.add(CyberCore::DragOperation::Generic);
    if (operation & NSDragOperationPrivate)
        result.add(CyberCore::DragOperation::Private);
    if (operation & NSDragOperationMove)
        result.add(CyberCore::DragOperation::Move);
    if (operation & NSDragOperationDelete)
        result.add(CyberCore::DragOperation::Delete);
    return result;
}
#endif // USE(APPKIT)

} // namespace WebKit
