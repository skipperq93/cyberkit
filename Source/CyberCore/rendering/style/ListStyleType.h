/*
 * Copyright (C) 2023 Apple Inc.  All rights reserved.
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

#include <optional>
#include <wtf/Forward.h>
#include <wtf/text/AtomString.h>
#include <wtf/text/TextStream.h>


namespace CyberCore {
struct ListStyleType {

    // The order of this enum must match the order of the list style types in CSSValueKeywords.in.
    enum class Type : uint8_t {
        Disc,
        Circle,
        Square,
        Decimal,
        DecimalLeadingZero,
        ArabicIndic,
        Binary,
        Bengali,
        Cambodian,
        Khmer,
        Devanagari,
        Gujarati,
        Gurmukhi,
        Kannada,
        LowerHexadecimal,
        Lao,
        Malayalam,
        Mongolian,
        Myanmar,
        Octal,
        Oriya,
        Persian,
        Urdu,
        Telugu,
        Tibetan,
        Thai,
        UpperHexadecimal,
        LowerRoman,
        UpperRoman,
        LowerGreek,
        LowerAlpha,
        LowerLatin,
        UpperAlpha,
        UpperLatin,
        Afar,
        EthiopicHalehameAaEt,
        EthiopicHalehameAaEr,
        Amharic,
        EthiopicHalehameAmEt,
        AmharicAbegede,
        EthiopicAbegedeAmEt,
        CJKEarthlyBranch,
        CJKHeavenlyStem,
        Ethiopic,
        EthiopicHalehameGez,
        EthiopicAbegede,
        EthiopicAbegedeGez,
        HangulConsonant,
        Hangul,
        LowerNorwegian,
        Oromo,
        EthiopicHalehameOmEt,
        Sidama,
        EthiopicHalehameSidEt,
        Somali,
        EthiopicHalehameSoEt,
        Tigre,
        EthiopicHalehameTig,
        TigrinyaEr,
        EthiopicHalehameTiEr,
        TigrinyaErAbegede,
        EthiopicAbegedeTiEr,
        TigrinyaEt,
        EthiopicHalehameTiEt,
        TigrinyaEtAbegede,
        EthiopicAbegedeTiEt,
        UpperGreek,
        UpperNorwegian,
        Asterisks,
        Footnotes,
        Hebrew,
        Armenian,
        LowerArmenian,
        UpperArmenian,
        Georgian,
        CJKIdeographic,
        Hiragana,
        Katakana,
        HiraganaIroha,
        KatakanaIroha,
        CJKDecimal,
        Tamil,
        DisclosureOpen,
        DisclosureClosed,
        JapaneseInformal,
        JapaneseFormal,
        KoreanHangulFormal,
        KoreanHanjaInformal,
        KoreanHanjaFormal,
        SimplifiedChineseInformal,
        SimplifiedChineseFormal,
        TraditionalChineseInformal,
        TraditionalChineseFormal,
        EthiopicNumeric,
        CounterStyle,
        String,
        None
    };

    bool isCircle() const;
    bool isSquare() const;
    bool isDisc() const;
    bool isDisclosureClosed() const;

    bool operator==(const ListStyleType& other) const { return type == other.type && identifier == other.identifier; }
    bool operator!=(const ListStyleType& other) const { return !(*this == other); }

    Type type { Type::None };
    // The identifier is the string when the type is String and is the @counter-style name when the type is CounterStyle.
    AtomString identifier;
};

TextStream& operator<<(TextStream&, ListStyleType::Type);
WTF::TextStream& operator<<(WTF::TextStream&, ListStyleType);


} // namespace CyberCore

namespace WTF {

template<> struct EnumTraits<CyberCore::ListStyleType::Type> {
    using values = EnumValues<
        CyberCore::ListStyleType::Type,
        CyberCore::ListStyleType::Type::Disc,
        CyberCore::ListStyleType::Type::Circle,
        CyberCore::ListStyleType::Type::Square,
        CyberCore::ListStyleType::Type::Decimal,
        CyberCore::ListStyleType::Type::DecimalLeadingZero,
        CyberCore::ListStyleType::Type::ArabicIndic,
        CyberCore::ListStyleType::Type::Binary,
        CyberCore::ListStyleType::Type::Bengali,
        CyberCore::ListStyleType::Type::Cambodian,
        CyberCore::ListStyleType::Type::Khmer,
        CyberCore::ListStyleType::Type::Devanagari,
        CyberCore::ListStyleType::Type::Gujarati,
        CyberCore::ListStyleType::Type::Gurmukhi,
        CyberCore::ListStyleType::Type::Kannada,
        CyberCore::ListStyleType::Type::LowerHexadecimal,
        CyberCore::ListStyleType::Type::Lao,
        CyberCore::ListStyleType::Type::Malayalam,
        CyberCore::ListStyleType::Type::Mongolian,
        CyberCore::ListStyleType::Type::Myanmar,
        CyberCore::ListStyleType::Type::Octal,
        CyberCore::ListStyleType::Type::Oriya,
        CyberCore::ListStyleType::Type::Persian,
        CyberCore::ListStyleType::Type::Urdu,
        CyberCore::ListStyleType::Type::Telugu,
        CyberCore::ListStyleType::Type::Tibetan,
        CyberCore::ListStyleType::Type::Thai,
        CyberCore::ListStyleType::Type::UpperHexadecimal,
        CyberCore::ListStyleType::Type::LowerRoman,
        CyberCore::ListStyleType::Type::UpperRoman,
        CyberCore::ListStyleType::Type::LowerGreek,
        CyberCore::ListStyleType::Type::LowerAlpha,
        CyberCore::ListStyleType::Type::LowerLatin,
        CyberCore::ListStyleType::Type::UpperAlpha,
        CyberCore::ListStyleType::Type::UpperLatin,
        CyberCore::ListStyleType::Type::Afar,
        CyberCore::ListStyleType::Type::EthiopicHalehameAaEt,
        CyberCore::ListStyleType::Type::EthiopicHalehameAaEr,
        CyberCore::ListStyleType::Type::Amharic,
        CyberCore::ListStyleType::Type::EthiopicHalehameAmEt,
        CyberCore::ListStyleType::Type::AmharicAbegede,
        CyberCore::ListStyleType::Type::EthiopicAbegedeAmEt,
        CyberCore::ListStyleType::Type::CJKEarthlyBranch,
        CyberCore::ListStyleType::Type::CJKHeavenlyStem,
        CyberCore::ListStyleType::Type::Ethiopic,
        CyberCore::ListStyleType::Type::EthiopicHalehameGez,
        CyberCore::ListStyleType::Type::EthiopicAbegede,
        CyberCore::ListStyleType::Type::EthiopicAbegedeGez,
        CyberCore::ListStyleType::Type::HangulConsonant,
        CyberCore::ListStyleType::Type::Hangul,
        CyberCore::ListStyleType::Type::LowerNorwegian,
        CyberCore::ListStyleType::Type::Oromo,
        CyberCore::ListStyleType::Type::EthiopicHalehameOmEt,
        CyberCore::ListStyleType::Type::Sidama,
        CyberCore::ListStyleType::Type::EthiopicHalehameSidEt,
        CyberCore::ListStyleType::Type::Somali,
        CyberCore::ListStyleType::Type::EthiopicHalehameSoEt,
        CyberCore::ListStyleType::Type::Tigre,
        CyberCore::ListStyleType::Type::EthiopicHalehameTig,
        CyberCore::ListStyleType::Type::TigrinyaEr,
        CyberCore::ListStyleType::Type::EthiopicHalehameTiEr,
        CyberCore::ListStyleType::Type::TigrinyaErAbegede,
        CyberCore::ListStyleType::Type::EthiopicAbegedeTiEr,
        CyberCore::ListStyleType::Type::TigrinyaEt,
        CyberCore::ListStyleType::Type::EthiopicHalehameTiEt,
        CyberCore::ListStyleType::Type::TigrinyaEtAbegede,
        CyberCore::ListStyleType::Type::EthiopicAbegedeTiEt,
        CyberCore::ListStyleType::Type::UpperGreek,
        CyberCore::ListStyleType::Type::UpperNorwegian,
        CyberCore::ListStyleType::Type::Asterisks,
        CyberCore::ListStyleType::Type::Footnotes,
        CyberCore::ListStyleType::Type::Hebrew,
        CyberCore::ListStyleType::Type::Armenian,
        CyberCore::ListStyleType::Type::LowerArmenian,
        CyberCore::ListStyleType::Type::UpperArmenian,
        CyberCore::ListStyleType::Type::Georgian,
        CyberCore::ListStyleType::Type::CJKIdeographic,
        CyberCore::ListStyleType::Type::Hiragana,
        CyberCore::ListStyleType::Type::Katakana,
        CyberCore::ListStyleType::Type::HiraganaIroha,
        CyberCore::ListStyleType::Type::KatakanaIroha,
        CyberCore::ListStyleType::Type::CJKDecimal,
        CyberCore::ListStyleType::Type::Tamil,
        CyberCore::ListStyleType::Type::DisclosureOpen,
        CyberCore::ListStyleType::Type::DisclosureClosed,
        CyberCore::ListStyleType::Type::JapaneseInformal,
        CyberCore::ListStyleType::Type::JapaneseFormal,
        CyberCore::ListStyleType::Type::KoreanHangulFormal,
        CyberCore::ListStyleType::Type::KoreanHanjaInformal,
        CyberCore::ListStyleType::Type::KoreanHanjaFormal,
        CyberCore::ListStyleType::Type::SimplifiedChineseInformal,
        CyberCore::ListStyleType::Type::SimplifiedChineseFormal,
        CyberCore::ListStyleType::Type::TraditionalChineseInformal,
        CyberCore::ListStyleType::Type::TraditionalChineseFormal,
        CyberCore::ListStyleType::Type::EthiopicNumeric,
        CyberCore::ListStyleType::Type::CounterStyle,
        CyberCore::ListStyleType::Type::String,
        CyberCore::ListStyleType::Type::None
    >;
};

} // namespace WTF
