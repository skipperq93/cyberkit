/*
 * Copyright (C) 2014-2020 Apple Inc. All rights reserved.
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
#include <CyberCore/AutocapitalizeTypes.h>
#include <CyberCore/Autofill.h>
#include <CyberCore/Color.h>
#include <CyberCore/ElementContext.h>
#include <CyberCore/EnterKeyHint.h>
#include <CyberCore/GraphicsLayer.h>
#include <CyberCore/InputMode.h>
#include <CyberCore/IntRect.h>
#include <wtf/EnumTraits.h>
#include <wtf/URL.h>
#include <wtf/text/WTFString.h>

namespace CyberKit {

enum class InputType {
    None,
    ContentEditable,
    Text,
    Password,
    TextArea,
    Search,
    Email,
    URL,
    Phone,
    Number,
    NumberPad,
    Date,
    DateTimeLocal,
    Month,
    Week,
    Time,
    Select,
    Drawing,
#if ENABLE(INPUT_TYPE_COLOR)
    Color
#endif
};

#if PLATFORM(IOS_FAMILY)
struct OptionItem {
    OptionItem() { }

    OptionItem(const OptionItem& item)
        : text(item.text)
        , isGroup(item.isGroup)
        , isSelected(item.isSelected)
        , disabled(item.disabled)
        , parentGroupID(item.parentGroupID)
    {
    }

    OptionItem(const String& text, bool isGroup, int parentID, bool selected, bool disabled)
        : text(text)
        , isGroup(isGroup)
        , isSelected(selected)
        , disabled(disabled)
        , parentGroupID(parentID)
    {
    }
    String text;
    bool isGroup { false };
    bool isSelected { false };
    bool disabled { false };
    int parentGroupID { 0 };

    void encode(IPC::Encoder&) const;
    static Optional<OptionItem> decode(IPC::Decoder&);
};

using FocusedElementIdentifier = uint64_t;

struct FocusedElementInformation {
    CyberCore::IntRect interactionRect;
    CyberCore::ElementContext elementContext;
    CyberCore::IntPoint lastInteractionLocation;
    double minimumScaleFactor { -INFINITY };
    double maximumScaleFactor { INFINITY };
    double maximumScaleFactorIgnoringAlwaysScalable { INFINITY };
    double nodeFontSize { 0 };
    bool hasNextNode { false };
    CyberCore::IntRect nextNodeRect;
    bool hasPreviousNode { false };
    CyberCore::IntRect previousNodeRect;
    bool isAutocorrect { false };
    bool isRTL { false };
    bool isMultiSelect { false };
    bool isReadOnly {false };
    bool allowsUserScaling { false };
    bool allowsUserScalingIgnoringAlwaysScalable { false };
    bool insideFixedPosition { false };
    CyberCore::AutocapitalizeType autocapitalizeType { CyberCore::AutocapitalizeType::Default };
    InputType elementType { InputType::None };
    CyberCore::InputMode inputMode { CyberCore::InputMode::Unspecified };
    CyberCore::EnterKeyHint enterKeyHint { CyberCore::EnterKeyHint::Unspecified };
    String formAction;
    Vector<OptionItem> selectOptions;
    int selectedIndex { -1 };
    String value;
    double valueAsNumber { 0 };
    String title;
    bool acceptsAutofilledLoginCredentials { false };
    bool isAutofillableUsernameField { false };
    URL representingPageURL;
    CyberCore::AutofillFieldName autofillFieldName { CyberCore::AutofillFieldName::None };
    String placeholder;
    String label;
    String ariaLabel;
    CyberCore::GraphicsLayer::EmbeddedViewID embeddedViewID;
#if ENABLE(DATALIST_ELEMENT)
    bool hasSuggestions { false };
#if ENABLE(INPUT_TYPE_COLOR)
    Vector<CyberCore::Color> suggestedColors;
#endif
#endif
    bool shouldSynthesizeKeyEventsForEditing { false };
    bool isSpellCheckingEnabled { true };
    bool shouldAvoidResizingWhenInputViewBoundsChange { false };
    bool shouldAvoidScrollingWhenFocusedContentIsVisible { false };
    bool shouldUseLegacySelectPopoverDismissalBehaviorInDataActivation { false };

    FocusedElementIdentifier focusedElementIdentifier { 0 };

    void encode(IPC::Encoder&) const;
    static WARN_UNUSED_RETURN bool decode(IPC::Decoder&, FocusedElementInformation&);
};
#endif

} // namespace CyberKit

namespace WTF {

template<> struct EnumTraits<CyberKit::InputType> {
    using values = EnumValues<
        CyberKit::InputType,
        CyberKit::InputType::None,
        CyberKit::InputType::ContentEditable,
        CyberKit::InputType::Text,
        CyberKit::InputType::Password,
        CyberKit::InputType::TextArea,
        CyberKit::InputType::Search,
        CyberKit::InputType::Email,
        CyberKit::InputType::URL,
        CyberKit::InputType::Phone,
        CyberKit::InputType::Number,
        CyberKit::InputType::NumberPad,
        CyberKit::InputType::Date,
        CyberKit::InputType::DateTimeLocal,
        CyberKit::InputType::Month,
        CyberKit::InputType::Week,
        CyberKit::InputType::Time,
        CyberKit::InputType::Select,
        CyberKit::InputType::Drawing
#if ENABLE(INPUT_TYPE_COLOR)
        , CyberKit::InputType::Color
#endif
    >;
};

} // namespace WTF
