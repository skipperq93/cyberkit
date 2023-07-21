/*
 * Copyright (C) 2019 Apple Inc. All rights reserved.
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
#include "EditingRange.h"

#include <CyberCore/Frame.h>
#include <CyberCore/FrameSelection.h>
#include <CyberCore/TextIterator.h>
#include <CyberCore/VisibleUnits.h>

namespace CyberKit {

RefPtr<CyberCore::Range> EditingRange::toRange(CyberCore::Frame& frame, const EditingRange& range, EditingRangeIsRelativeTo editingRangeIsRelativeTo)
{
    ASSERT(range.location != notFound);

    // Sanitize the input, because TextIterator::rangeFromLocationAndLength takes signed integers.
    if (range.location > INT_MAX)
        return nullptr;
    int length;
    if (range.length <= INT_MAX && range.location + range.length <= INT_MAX)
        length = static_cast<int>(range.length);
    else
        length = INT_MAX - range.location;

    if (editingRangeIsRelativeTo == EditingRangeIsRelativeTo::EditableRoot) {
        // Our critical assumption is that this code path is called by input methods that
        // concentrate on a given area containing the selection.
        // We have to do this because of text fields and textareas. The DOM for those is not
        // directly in the document DOM, so serialization is problematic. Our solution is
        // to use the root editable element of the selection start as the positional base.
        // That fits with AppKit's idea of an input context.
        return CyberCore::TextIterator::rangeFromLocationAndLength(frame.selection().rootEditableElementOrDocumentElement(), static_cast<int>(range.location), length);
    }

    ASSERT(editingRangeIsRelativeTo == EditingRangeIsRelativeTo::Paragraph);

    const CyberCore::VisibleSelection& selection = frame.selection().selection();
    RefPtr<CyberCore::Range> selectedRange = selection.toNormalizedRange();
    if (!selectedRange)
        return nullptr;

    RefPtr<CyberCore::Range> paragraphRange = makeRange(startOfParagraph(selection.visibleStart()), selection.visibleEnd());
    if (!paragraphRange)
        return nullptr;

    CyberCore::ContainerNode& rootNode = paragraphRange.get()->startContainer().treeScope().rootNode();
    int paragraphStartIndex = CyberCore::TextIterator::rangeLength(CyberCore::Range::create(rootNode.document(), &rootNode, 0, &paragraphRange->startContainer(), paragraphRange->startOffset()).ptr());
    return CyberCore::TextIterator::rangeFromLocationAndLength(&rootNode, paragraphStartIndex + static_cast<int>(range.location), length);
}

EditingRange EditingRange::fromRange(CyberCore::Frame& frame, const CyberCore::Range* range, EditingRangeIsRelativeTo editingRangeIsRelativeTo)
{
    ASSERT(editingRangeIsRelativeTo == EditingRangeIsRelativeTo::EditableRoot);

    size_t location;
    size_t length;
    if (!range || !CyberCore::TextIterator::getLocationAndLengthFromRange(frame.selection().rootEditableElementOrDocumentElement(), range, location, length))
        return { };

    EditingRange editingRange(location, length);
    if (!editingRange.isValid())
        return { };

    return editingRange;
}

} // namespace CyberKit

namespace IPC {

void ArgumentCoder<CyberKit::EditingRange>::encode(Encoder& encoder, const CyberKit::EditingRange& editingRange)
{
    encoder << editingRange.location;
    encoder << editingRange.length;
}

Optional<CyberKit::EditingRange> ArgumentCoder<CyberKit::EditingRange>::decode(Decoder& decoder)
{
    CyberKit::EditingRange editingRange;

    if (!decoder.decode(editingRange.location))
        return WTF::nullopt;
    if (!decoder.decode(editingRange.length))
        return WTF::nullopt;

    return editingRange;
}

} // namespace IPC
