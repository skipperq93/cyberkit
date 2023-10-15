/*
 * Copyright (C) 2010-2020 Apple Inc. All rights reserved.
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

#include "ArgumentCoders.h"
#include "IdentifierTypes.h"
#include <CyberCore/Color.h>
#include <CyberCore/ElementContext.h>
#include <CyberCore/FontAttributes.h>
#include <CyberCore/IntRect.h>
#include <CyberCore/WritingDirection.h>
#include <wtf/text/WTFString.h>

#if PLATFORM(IOS_FAMILY)
#include <CyberCore/SelectionGeometry.h>
#endif

#if USE(DICTATION_ALTERNATIVES)
#include <CyberCore/DictationContext.h>
#endif

namespace WTF {
class TextStream;
};

namespace CyberKit {

enum TypingAttributes {
    AttributeNone = 0,
    AttributeBold = 1,
    AttributeItalics = 2,
    AttributeUnderline = 4,
    AttributeStrikeThrough = 8
};

enum TextAlignment {
    NoAlignment = 0,
    LeftAlignment = 1,
    RightAlignment = 2,
    CenterAlignment = 3,
    JustifiedAlignment = 4,
};

enum ListType {
    NoList = 0,
    OrderedList,
    UnorderedList
};

struct EditorState {
    EditorStateIdentifier identifier;
    bool shouldIgnoreSelectionChanges { false };
    bool selectionIsNone { true }; // This will be false when there is a caret selection.
    bool selectionIsRange { false };
    bool selectionIsRangeInsideImageOverlay { false };
    bool selectionIsRangeInAutoFilledAndViewableField { false };
    bool isContentEditable { false };
    bool isContentRichlyEditable { false };
    bool isInPasswordField { false };
    bool isInPlugin { false };
    bool hasComposition { false };
    bool triggeredByAccessibilitySelectionChange { false };
#if PLATFORM(MAC)
    bool canEnableAutomaticSpellingCorrection { true };
#endif

    struct PostLayoutData {
        uint32_t typingAttributes { AttributeNone };
#if PLATFORM(COCOA)
        uint64_t selectedTextLength { 0 };
        uint32_t textAlignment { NoAlignment };
        CyberCore::Color textColor { CyberCore::Color::black }; // FIXME: Maybe this should be on VisualData?
        uint32_t enclosingListType { NoList };
        CyberCore::WritingDirection baseWritingDirection { CyberCore::WritingDirection::Natural };
        bool editableRootIsTransparentOrFullyClipped { false };
#endif
#if PLATFORM(IOS_FAMILY)
        String markedText;
        String wordAtSelection;
        UChar32 characterAfterSelection { 0 };
        UChar32 characterBeforeSelection { 0 };
        UChar32 twoCharacterBeforeSelection { 0 };
#if USE(DICTATION_ALTERNATIVES)
        Vector<CyberCore::DictationContext> dictationContextsForSelection;
#endif
        bool isReplaceAllowed { false };
        bool hasContent { false };
        bool isStableStateUpdate { false };
        bool insideFixedPosition { false };
        bool hasPlainText { false };
        CyberCore::Color caretColor; // FIXME: Maybe this should be on VisualData?
        bool atStartOfSentence { false };
        bool selectionStartIsAtParagraphBoundary { false };
        bool selectionEndIsAtParagraphBoundary { false };
        std::optional<CyberCore::ElementContext> selectedEditableImage;
#endif
#if PLATFORM(MAC)
        CyberCore::IntRect selectionBoundingRect; // FIXME: Maybe this should be on VisualData?
        uint64_t candidateRequestStartPosition { 0 };
        String paragraphContextForCandidateRequest;
        String stringForCandidateRequest;
#endif
#if PLATFORM(GTK) || PLATFORM(WPE)
        String surroundingContext;
        uint64_t surroundingContextCursorPosition { 0 };
        uint64_t surroundingContextSelectionPosition { 0 };
#endif

        std::optional<CyberCore::FontAttributes> fontAttributes;

        bool canCut { false };
        bool canCopy { false };
        bool canPaste { false };
    };

    bool hasPostLayoutData() const { return !!postLayoutData; }

    // Visual data is only updated in sync with rendering updates.
    struct VisualData {
#if PLATFORM(IOS_FAMILY) || PLATFORM(GTK) || PLATFORM(WPE)
        CyberCore::IntRect caretRectAtStart;
#endif
#if PLATFORM(IOS_FAMILY)
        CyberCore::IntRect selectionClipRect;
        CyberCore::IntRect caretRectAtEnd;
        Vector<CyberCore::SelectionGeometry> selectionGeometries;
        Vector<CyberCore::SelectionGeometry> markedTextRects;
        CyberCore::IntRect markedTextCaretRectAtStart;
        CyberCore::IntRect markedTextCaretRectAtEnd;
#endif
    };

    bool hasVisualData() const { return !!visualData; }

    void encode(IPC::Encoder&) const;
    static WARN_UNUSED_RETURN bool decode(IPC::Decoder&, EditorState&);

    bool hasPostLayoutAndVisualData() const { return hasPostLayoutData() && hasVisualData(); }

    std::optional<PostLayoutData> postLayoutData;
    std::optional<VisualData> visualData;

private:
    friend TextStream& operator<<(TextStream&, const EditorState&);
};

WTF::TextStream& operator<<(WTF::TextStream&, const EditorState&);

} // namespace CyberKit
