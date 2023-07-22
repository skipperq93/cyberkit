/*
 * Copyright (C) 2006-2016 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import <CyberCore/DOMPasteAccess.h>
#import <CyberCore/EditorClient.h>
#import <CyberCore/TextCheckerClient.h>
#import <CyberCore/VisibleSelection.h>
#import <wtf/Forward.h>
#import <wtf/RetainPtr.h>
#import <wtf/Vector.h>
#import <wtf/WeakPtr.h>
#import <wtf/text/StringView.h>

#if PLATFORM(IOS_FAMILY)
#import <CyberCore/WAKAppKitStubs.h>
#endif

@class WebView;
@class WebEditorUndoTarget;

class WebEditorClient final : public CyberCore::EditorClient, public CyberCore::TextCheckerClient, public CanMakeWeakPtr<WebEditorClient> {
    WTF_MAKE_FAST_ALLOCATED;
public:
    WebEditorClient(WebView *);
    virtual ~WebEditorClient();

    void didCheckSucceed(int sequence, NSArray *results);

private:
    bool isGrammarCheckingEnabled() final;
    void toggleGrammarChecking() final;
    bool isContinuousSpellCheckingEnabled() final;
    void toggleContinuousSpellChecking() final;
    int spellCheckerDocumentTag() final;

    bool smartInsertDeleteEnabled() final;
    bool isSelectTrailingWhitespaceEnabled() const final;

    bool shouldDeleteRange(CyberCore::Range*) final;

    bool shouldBeginEditing(CyberCore::Range*) final;
    bool shouldEndEditing(CyberCore::Range*) final;
    bool shouldInsertNode(CyberCore::Node*, CyberCore::Range*, CyberCore::EditorInsertAction) final;
    bool shouldInsertText(const String&, CyberCore::Range*, CyberCore::EditorInsertAction) final;
    bool shouldChangeSelectedRange(CyberCore::Range* fromRange, CyberCore::Range* toRange, CyberCore::EAffinity, bool stillSelecting) final;

    bool shouldApplyStyle(CyberCore::StyleProperties*, CyberCore::Range*) final;
    void didApplyStyle() final;

    bool shouldMoveRangeAfterDelete(CyberCore::Range*, CyberCore::Range* rangeToBeReplaced) final;

    void didBeginEditing() final;
    void didEndEditing() final;
    void willWriteSelectionToPasteboard(CyberCore::Range*) final;
    void didWriteSelectionToPasteboard() final;
    void getClientPasteboardDataForRange(CyberCore::Range*, Vector<String>& pasteboardTypes, Vector<RefPtr<CyberCore::SharedBuffer>>& pasteboardData) final;

    void setInsertionPasteboard(const String&) final;
    CyberCore::DOMPasteAccessResponse requestDOMPasteAccess(const String&) final { return CyberCore::DOMPasteAccessResponse::DeniedForGesture; }

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

    TextCheckerClient* textChecker() final { return this; }

    void respondToChangedContents() final;
    void respondToChangedSelection(CyberCore::Frame*) final;
    void didEndUserTriggeredSelectionChanges() final { }
    void updateEditorStateAfterLayoutIfEditabilityChanged() final;
    void discardedComposition(CyberCore::Frame*) final;
    void canceledComposition() final;
    void didUpdateComposition() final { }

    void registerUndoStep(CyberCore::UndoStep&) final;
    void registerRedoStep(CyberCore::UndoStep&) final;
    void clearUndoRedoOperations() final;

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
    void overflowScrollPositionChanged() final { };
    void subFrameScrollPositionChanged() final { };

#if PLATFORM(IOS_FAMILY)
    void startDelayingAndCoalescingContentChangeNotifications() final;
    void stopDelayingAndCoalescingContentChangeNotifications() final;
    bool hasRichlyEditableSelection() final;
    int getPasteboardItemsCount() final;
    RefPtr<CyberCore::DocumentFragment> documentFragmentFromDelegate(int index) final;
    bool performsTwoStepPaste(CyberCore::DocumentFragment*) final;
    void updateStringForFind(const String&) final { }
#endif

    bool performTwoStepDrop(CyberCore::DocumentFragment&, CyberCore::Range& destination, bool isMove) final;
    
    bool shouldEraseMarkersAfterChangeSelection(CyberCore::TextCheckingType) const final;
    void ignoreWordInSpellDocument(const String&) final;
    void learnWord(const String&) final;
    void checkSpellingOfString(StringView, int* misspellingLocation, int* misspellingLength) final;
    String getAutoCorrectSuggestionForMisspelledWord(const String&) final;
    void checkGrammarOfString(StringView, Vector<CyberCore::GrammarDetail>&, int* badGrammarLocation, int* badGrammarLength) final;
    Vector<CyberCore::TextCheckingResult> checkTextOfParagraph(StringView, OptionSet<CyberCore::TextCheckingType> checkingTypes, const CyberCore::VisibleSelection& currentSelection) final;
    void updateSpellingUIWithGrammarString(const String&, const CyberCore::GrammarDetail&) final;
    void updateSpellingUIWithMisspelledWord(const String&) final;
    void showSpellingUI(bool show) final;
    bool spellingUIIsShowing() final;
    void getGuessesForWord(const String& word, const String& context, const CyberCore::VisibleSelection& currentSelection, Vector<String>& guesses) final;

    void willSetInputMethodState() final;
    void setInputMethodState(bool enabled) final;
    void requestCheckingOfString(CyberCore::TextCheckingRequest&, const CyberCore::VisibleSelection& currentSelection) final;

#if PLATFORM(MAC)
    void requestCandidatesForSelection(const CyberCore::VisibleSelection&) final;
    void handleRequestedCandidates(NSInteger, NSArray<NSTextCheckingResult *> *);
    void handleAcceptedCandidateWithSoftSpaces(CyberCore::TextCheckingResult) final;
#endif

    void registerUndoOrRedoStep(CyberCore::UndoStep&, bool isRedo);

#if PLATFORM(IOS_FAMILY)
    bool shouldAllowSingleClickToChangeSelection(CyberCore::Node& targetNode, const CyberCore::VisibleSelection& newSelection) const;
#endif

    bool canShowFontPanel() const final
    {
#if PLATFORM(MAC)
        return true;
#else
        return false;
#endif
    }

    WebView *m_webView;
    RetainPtr<WebEditorUndoTarget> m_undoTarget;
    bool m_haveUndoRedoOperations { false };
    RefPtr<CyberCore::TextCheckingRequest> m_textCheckingRequest;

#if PLATFORM(IOS_FAMILY)
    bool m_delayingContentChangeNotifications { false };
    bool m_hasDelayedContentChangeNotification { false };
#endif

    CyberCore::VisibleSelection m_lastSelectionForRequestedCandidates;

#if PLATFORM(MAC)
    RetainPtr<NSString> m_paragraphContextForCandidateRequest;
    NSRange m_rangeForCandidates;
    NSInteger m_lastCandidateRequestSequenceNumber;
#endif

    enum class EditorStateIsContentEditable { No, Yes, Unset };
    EditorStateIsContentEditable m_lastEditorStateWasContentEditable { EditorStateIsContentEditable::Unset };
};

inline NSSelectionAffinity kit(CyberCore::EAffinity affinity)
{
    switch (affinity) {
        case CyberCore::EAffinity::UPSTREAM:
            return NSSelectionAffinityUpstream;
        case CyberCore::EAffinity::DOWNSTREAM:
            return NSSelectionAffinityDownstream;
    }
    ASSERT_NOT_REACHED();
    return NSSelectionAffinityUpstream;
}

inline CyberCore::EAffinity core(NSSelectionAffinity affinity)
{
    switch (affinity) {
    case NSSelectionAffinityUpstream:
        return CyberCore::EAffinity::UPSTREAM;
    case NSSelectionAffinityDownstream:
        return CyberCore::EAffinity::DOWNSTREAM;
    }
    ASSERT_NOT_REACHED();
    return CyberCore::EAffinity::UPSTREAM;
}

#if PLATFORM(IOS_FAMILY)

inline bool WebEditorClient::isGrammarCheckingEnabled()
{
    return false;
}

inline void WebEditorClient::toggleGrammarChecking()
{
}

inline int WebEditorClient::spellCheckerDocumentTag()
{
    return 0;
}

inline bool WebEditorClient::shouldEraseMarkersAfterChangeSelection(CyberCore::TextCheckingType) const
{
    return true;
}

inline void WebEditorClient::ignoreWordInSpellDocument(const String&)
{
}

inline void WebEditorClient::learnWord(const String&)
{
}

inline void WebEditorClient::checkSpellingOfString(StringView, int* misspellingLocation, int* misspellingLength)
{
}

inline String WebEditorClient::getAutoCorrectSuggestionForMisspelledWord(const String&)
{
    return "";
}

inline void WebEditorClient::checkGrammarOfString(StringView, Vector<CyberCore::GrammarDetail>&, int* badGrammarLocation, int* badGrammarLength)
{
}

inline void WebEditorClient::updateSpellingUIWithGrammarString(const String&, const CyberCore::GrammarDetail&)
{
}

inline void WebEditorClient::updateSpellingUIWithMisspelledWord(const String&)
{
}

inline void WebEditorClient::showSpellingUI(bool show)
{
}

inline bool WebEditorClient::spellingUIIsShowing()
{
    return false;
}

inline void WebEditorClient::getGuessesForWord(const String&, const String&, const CyberCore::VisibleSelection&, Vector<String>&)
{
}

#endif
