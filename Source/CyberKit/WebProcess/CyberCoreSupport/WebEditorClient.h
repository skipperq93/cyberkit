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

#pragma once

#include <CyberCore/EditorClient.h>
#include <CyberCore/TextCheckerClient.h>

namespace CyberCore {
enum class DOMPasteAccessResponse : uint8_t;
}

namespace CyberKit {

class WebPage;

class WebEditorClient final : public CyberCore::EditorClient, public CyberCore::TextCheckerClient {
public:
    WebEditorClient(WebPage* page)
        : m_page(page)
    {
    }

private:
    bool shouldDeleteRange(CyberCore::Range*) final;
    bool smartInsertDeleteEnabled() final;
    bool isSelectTrailingWhitespaceEnabled() const final;
    bool isContinuousSpellCheckingEnabled() final;
    void toggleContinuousSpellChecking() final;
    bool isGrammarCheckingEnabled() final;
    void toggleGrammarChecking() final;
    int spellCheckerDocumentTag() final;
    
    bool shouldBeginEditing(CyberCore::Range*) final;
    bool shouldEndEditing(CyberCore::Range*) final;
    bool shouldInsertNode(CyberCore::Node*, CyberCore::Range*, CyberCore::EditorInsertAction) final;
    bool shouldInsertText(const String&, CyberCore::Range*, CyberCore::EditorInsertAction) final;
    bool shouldChangeSelectedRange(CyberCore::Range* fromRange, CyberCore::Range* toRange, CyberCore::EAffinity, bool stillSelecting) final;
    
    bool shouldApplyStyle(CyberCore::StyleProperties*, CyberCore::Range*) final;
    void didApplyStyle() final;
    bool shouldMoveRangeAfterDelete(CyberCore::Range*, CyberCore::Range*) final;

#if ENABLE(ATTACHMENT_ELEMENT)
    void registerAttachmentIdentifier(const String&, const String& contentType, const String& preferredFileName, Ref<CyberCore::SharedBuffer>&&) final;
    void registerAttachmentIdentifier(const String&, const String& contentType, const String& filePath) final;
    void registerAttachmentIdentifier(const String&) final;
    void registerAttachments(Vector<CyberCore::SerializedAttachmentData>&&) final;
    void cloneAttachmentData(const String& fromIdentifier, const String& toIdentifier) final;
    void didInsertAttachmentWithIdentifier(const String& identifier, const String& source, bool hasEnclosingImage) final;
    void didRemoveAttachmentWithIdentifier(const String& identifier) final;
    bool supportsClientSideAttachmentData() const final { return true; }
    Vector<CyberCore::SerializedAttachmentData> serializedAttachmentDataForIdentifiers(const Vector<String>&) final;
#endif

    void didBeginEditing() final;
    void respondToChangedContents() final;
    void respondToChangedSelection(CyberCore::Frame*) final;
    void didEndUserTriggeredSelectionChanges() final;
    void updateEditorStateAfterLayoutIfEditabilityChanged() final;
    void discardedComposition(CyberCore::Frame*) final;
    void canceledComposition() final;
    void didUpdateComposition() final;
    void didEndEditing() final;
    void willWriteSelectionToPasteboard(CyberCore::Range*) final;
    void didWriteSelectionToPasteboard() final;
    void getClientPasteboardDataForRange(CyberCore::Range*, Vector<String>& pasteboardTypes, Vector<RefPtr<CyberCore::SharedBuffer>>& pasteboardData) final;
    
    void registerUndoStep(CyberCore::UndoStep&) final;
    void registerRedoStep(CyberCore::UndoStep&) final;
    void clearUndoRedoOperations() final;

    CyberCore::DOMPasteAccessResponse requestDOMPasteAccess(const String& originIdentifier) final;

    bool canCopyCut(CyberCore::Frame*, bool defaultValue) const final;
    bool canPaste(CyberCore::Frame*, bool defaultValue) const final;
    bool canUndo() const final;
    bool canRedo() const final;
    
    void undo() final;
    void redo() final;

    void handleKeyboardEvent(CyberCore::KeyboardEvent&) final;
    void handleInputMethodKeydown(CyberCore::KeyboardEvent&) final;
    
    void textFieldDidBeginEditing(CyberCore::Element*) final;
    void textFieldDidEndEditing(CyberCore::Element*) final;
    void textDidChangeInTextField(CyberCore::Element*) final;
    bool doTextFieldCommandFromEvent(CyberCore::Element*, CyberCore::KeyboardEvent*) final;
    void textWillBeDeletedInTextField(CyberCore::Element*) final;
    void textDidChangeInTextArea(CyberCore::Element*) final;
    void overflowScrollPositionChanged() final;
    void subFrameScrollPositionChanged() final;

#if PLATFORM(COCOA)
    void setInsertionPasteboard(const String& pasteboardName) final;
#endif

#if USE(APPKIT)
    void uppercaseWord() final;
    void lowercaseWord() final;
    void capitalizeWord() final;
#endif

#if USE(AUTOMATIC_TEXT_REPLACEMENT)
    void showSubstitutionsPanel(bool show) final;
    bool substitutionsPanelIsShowing() final;
    void toggleSmartInsertDelete() final;
    bool isAutomaticQuoteSubstitutionEnabled() final;
    void toggleAutomaticQuoteSubstitution() final;
    bool isAutomaticLinkDetectionEnabled() final;
    void toggleAutomaticLinkDetection() final;
    bool isAutomaticDashSubstitutionEnabled() final;
    void toggleAutomaticDashSubstitution() final;
    bool isAutomaticTextReplacementEnabled() final;
    void toggleAutomaticTextReplacement() final;
    bool isAutomaticSpellingCorrectionEnabled() final;
    void toggleAutomaticSpellingCorrection() final;
#endif

#if PLATFORM(GTK)
    bool executePendingEditorCommands(CyberCore::Frame&, const Vector<WTF::String>&, bool);
    bool handleGtkEditorCommand(CyberCore::Frame&, const String& command, bool);
    void getEditorCommandsForKeyEvent(const CyberCore::KeyboardEvent*, Vector<WTF::String>&);
    void updateGlobalSelection(CyberCore::Frame*);
#endif

    TextCheckerClient* textChecker() final { return this; }

    bool shouldEraseMarkersAfterChangeSelection(CyberCore::TextCheckingType) const final;
    void ignoreWordInSpellDocument(const String&) final;
    void learnWord(const String&) final;
    void checkSpellingOfString(StringView, int* misspellingLocation, int* misspellingLength) final;
    String getAutoCorrectSuggestionForMisspelledWord(const String& misspelledWord) final;
    void checkGrammarOfString(StringView, Vector<CyberCore::GrammarDetail>&, int* badGrammarLocation, int* badGrammarLength) final;

#if USE(UNIFIED_TEXT_CHECKING)
    Vector<CyberCore::TextCheckingResult> checkTextOfParagraph(StringView, OptionSet<CyberCore::TextCheckingType> checkingTypes, const CyberCore::VisibleSelection& currentSelection) final;
#endif

    void updateSpellingUIWithGrammarString(const String&, const CyberCore::GrammarDetail&) final;
    void updateSpellingUIWithMisspelledWord(const String&) final;
    void showSpellingUI(bool show) final;
    bool spellingUIIsShowing() final;
    void getGuessesForWord(const String& word, const String& context, const CyberCore::VisibleSelection& currentSelection, Vector<String>& guesses) final;
    void willSetInputMethodState() final;
    void setInputMethodState(bool enabled) final;
    void requestCheckingOfString(CyberCore::TextCheckingRequest&, const CyberCore::VisibleSelection& currentSelection) final;

#if PLATFORM(GTK)
    bool shouldShowUnicodeMenu() final;
#endif

#if PLATFORM(IOS_FAMILY)
    void startDelayingAndCoalescingContentChangeNotifications() final;
    void stopDelayingAndCoalescingContentChangeNotifications() final;
    bool hasRichlyEditableSelection() final;
    int getPasteboardItemsCount() final;
    RefPtr<CyberCore::DocumentFragment> documentFragmentFromDelegate(int index) final;
    bool performsTwoStepPaste(CyberCore::DocumentFragment*) final;
    void updateStringForFind(const String&) final;
    bool shouldAllowSingleClickToChangeSelection(CyberCore::Node&, const CyberCore::VisibleSelection&) const final;
#endif

    bool performTwoStepDrop(CyberCore::DocumentFragment&, CyberCore::Range&, bool isMove) final;
    bool supportsGlobalSelection() final;

    bool canShowFontPanel() const final
    {
        // FIXME: Support for showing the system font panel (as well as other font styling controls) is
        // tracked in <rdar://problem/21577518>.
        return false;
    }

    WebPage* m_page;
};

} // namespace CyberKit
