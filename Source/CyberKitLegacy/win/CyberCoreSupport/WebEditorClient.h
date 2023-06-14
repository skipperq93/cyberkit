/*
 * Copyright (C) 2006-2017 Apple Inc.  All rights reserved.
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

#pragma once

#include "CyberKit.h"
#include <CyberCore/EditorClient.h>
#include <CyberCore/TextCheckerClient.h>

class WebView;
class WebNotification;
class WebEditorUndoTarget;

class WebEditorClient final : public CyberCore::EditorClient, public CyberCore::TextCheckerClient {
public:
    WebEditorClient(WebView*);
    ~WebEditorClient();

private:
    bool isContinuousSpellCheckingEnabled() final;
    void toggleGrammarChecking() final;
    bool isGrammarCheckingEnabled() final;
    void toggleContinuousSpellChecking() final;
    int spellCheckerDocumentTag() final;

    bool shouldBeginEditing(CyberCore::Range*) final;
    bool shouldEndEditing(CyberCore::Range*) final;
    bool shouldInsertText(const WTF::String&, CyberCore::Range*, CyberCore::EditorInsertAction) final;

    void didBeginEditing() final;
    void didEndEditing() final;
    void willWriteSelectionToPasteboard(CyberCore::Range*) final;
    void didWriteSelectionToPasteboard() final;
    void getClientPasteboardDataForRange(CyberCore::Range*, Vector<String>& pasteboardTypes, Vector<RefPtr<CyberCore::SharedBuffer>>& pasteboardData) final;
    String replacementURLForResource(Ref<CyberCore::SharedBuffer>&&, const String&) final;

    void didEndUserTriggeredSelectionChanges() final { }
    void respondToChangedContents() final;
    void respondToChangedSelection(CyberCore::Frame*) final;
    void updateEditorStateAfterLayoutIfEditabilityChanged() final { } 
    void canceledComposition() final;
    void discardedComposition(CyberCore::Frame*) final;
    void didUpdateComposition() final { }

    bool shouldDeleteRange(CyberCore::Range*) final;

    bool shouldInsertNode(CyberCore::Node*, CyberCore::Range* replacingRange, CyberCore::EditorInsertAction) final;
    bool shouldApplyStyle(CyberCore::StyleProperties*, CyberCore::Range*) final;
    void didApplyStyle() final;
    bool shouldMoveRangeAfterDelete(CyberCore::Range*, CyberCore::Range*) final;

    bool smartInsertDeleteEnabled() final;
    bool isSelectTrailingWhitespaceEnabled() const final;

    void registerUndoStep(CyberCore::UndoStep&) final;
    void registerRedoStep(CyberCore::UndoStep&) final;
    void clearUndoRedoOperations();

    bool canCopyCut(CyberCore::Frame*, bool defaultValue) const final;
    bool canPaste(CyberCore::Frame*, bool defaultValue) const final;
    bool canUndo() const final;
    bool canRedo() const final;
    
    void undo() final;
    void redo() final;
    
    bool shouldChangeSelectedRange(CyberCore::Range* fromRange, CyberCore::Range* toRange, CyberCore::EAffinity, bool stillSelecting) final;
    void textFieldDidBeginEditing(CyberCore::Element*) final;
    void textFieldDidEndEditing(CyberCore::Element*) final;
    void textDidChangeInTextField(CyberCore::Element*) final;
    bool doTextFieldCommandFromEvent(CyberCore::Element*, CyberCore::KeyboardEvent*) final;
    void textWillBeDeletedInTextField(CyberCore::Element* input) final;
    void textDidChangeInTextArea(CyberCore::Element*) final;
    void overflowScrollPositionChanged() final { }

    void handleKeyboardEvent(CyberCore::KeyboardEvent*) final;
    void handleInputMethodKeydown(CyberCore::KeyboardEvent*) final;

    bool shouldEraseMarkersAfterChangeSelection(CyberCore::TextCheckingType) const final;
    void ignoreWordInSpellDocument(const WTF::String&) final;
    void learnWord(const WTF::String&) final;
    void checkSpellingOfString(StringView, int* misspellingLocation, int* misspellingLength) final;
    WTF::String getAutoCorrectSuggestionForMisspelledWord(const WTF::String&) final;
    void checkGrammarOfString(StringView, Vector<CyberCore::GrammarDetail>&, int* badGrammarLocation, int* badGrammarLength) final;
    void updateSpellingUIWithGrammarString(const WTF::String&, const CyberCore::GrammarDetail&) final;
    void updateSpellingUIWithMisspelledWord(const WTF::String&) final;
    void showSpellingUI(bool show) final;
    bool spellingUIIsShowing() final;
    void getGuessesForWord(const WTF::String& word, const WTF::String& context, const CyberCore::VisibleSelection& currentSelection, WTF::Vector<WTF::String>& guesses) final;

    void willSetInputMethodState() final;
    void setInputMethodState(bool) final;
    void requestCheckingOfString(CyberCore::TextCheckingRequest&, const CyberCore::VisibleSelection&) final { }
    bool performTwoStepDrop(CyberCore::DocumentFragment&, CyberCore::Range&, bool) final { return false; }

    CyberCore::TextCheckerClient* textChecker() final { return this; }

    WebView* m_webView;
    WebEditorUndoTarget* m_undoTarget;
};
