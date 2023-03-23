/*
 * Copyright (C) 2019 Igalia S.L.
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
#include "InputMethodState.h"

#include "ArgumentCoders.h"
#include <CyberCore/ElementInlines.h>
#include <CyberCore/HTMLInputElement.h>

namespace WebKit {

void InputMethodState::setPurposeOrHintForInputMode(CyberCore::InputMode inputMode)
{
    switch (inputMode) {
    case CyberCore::InputMode::None:
        hints.add(InputMethodState::Hint::InhibitOnScreenKeyboard);
        break;
    case CyberCore::InputMode::Unspecified:
    case CyberCore::InputMode::Text:
        purpose = Purpose::FreeForm;
        break;
    case CyberCore::InputMode::Telephone:
        purpose = Purpose::Phone;
        break;
    case CyberCore::InputMode::Url:
        purpose = Purpose::Url;
        break;
    case CyberCore::InputMode::Email:
        purpose = Purpose::Email;
        break;
    case CyberCore::InputMode::Numeric:
        purpose = Purpose::Digits;
        break;
    case CyberCore::InputMode::Decimal:
        purpose = Purpose::Number;
        break;
    case CyberCore::InputMode::Search:
        break;
    }
}

static bool inputElementHasDigitsPattern(CyberCore::HTMLInputElement& element)
{
    const auto& pattern = element.attributeWithoutSynchronization(CyberCore::HTMLNames::patternAttr);
    return pattern == "\\d*"_s || pattern == "[0-9]*"_s;
}

void InputMethodState::setPurposeForInputElement(CyberCore::HTMLInputElement& element)
{
    if (element.isPasswordField())
        purpose = Purpose::Password;
    else if (element.isEmailField())
        purpose = Purpose::Email;
    else if (element.isTelephoneField())
        purpose = Purpose::Phone;
    else if (element.isNumberField())
        purpose = inputElementHasDigitsPattern(element) ? Purpose::Digits : Purpose::Number;
    else if (element.isURLField())
        purpose = Purpose::Url;
    else if (element.isText() && inputElementHasDigitsPattern(element))
        purpose = Purpose::Digits;
}

void InputMethodState::addHintsForAutocapitalizeType(CyberCore::AutocapitalizeType autocapitalizeType)
{
    switch (autocapitalizeType) {
    case CyberCore::AutocapitalizeType::Default:
        break;
    case CyberCore::AutocapitalizeType::None:
        hints.add(InputMethodState::Hint::Lowercase);
        break;
    case CyberCore::AutocapitalizeType::Words:
        hints.add(InputMethodState::Hint::UppercaseWords);
        break;
    case CyberCore::AutocapitalizeType::Sentences:
        hints.add(InputMethodState::Hint::UppercaseSentences);
        break;
    case CyberCore::AutocapitalizeType::AllCharacters:
        hints.add(InputMethodState::Hint::UppercaseChars);
        break;
    }
}

void InputMethodState::encode(IPC::Encoder& encoder) const
{
    encoder << purpose;
    encoder << hints;
}

std::optional<InputMethodState> InputMethodState::decode(IPC::Decoder& decoder)
{
    InputMethodState state;
    if (!decoder.decode(state.purpose))
        return std::nullopt;
    if (!decoder.decode(state.hints))
        return std::nullopt;
    return state;
}

} // namespace WebKit
