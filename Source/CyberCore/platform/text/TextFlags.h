/*
 * Copyright (C) 2003, 2006, 2017 Apple Inc.  All rights reserved.
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

#include <wtf/EnumTraits.h>
#include <wtf/Optional.h>

namespace CyberCore {

enum class TextRenderingMode : uint8_t {
    AutoTextRendering,
    OptimizeSpeed,
    OptimizeLegibility,
    GeometricPrecision
};

enum class FontSmoothingMode : uint8_t {
    AutoSmoothing,
    NoSmoothing,
    Antialiased,
    SubpixelAntialiased
};

// This setting is used to provide ways of switching between multiple rendering modes that may have different
// metrics. It is used to switch between CG and GDI text on Windows.
enum class FontRenderingMode : uint8_t {
    Normal,
    Alternate
};

enum class FontOrientation : uint8_t {
    Horizontal,
    Vertical
};

enum class NonCJKGlyphOrientation : uint8_t {
    Mixed,
    Upright
};

enum ExpansionBehaviorFlags {
    ForbidRightExpansion = 0 << 0,
    AllowRightExpansion = 1 << 0,
    ForceRightExpansion = 2 << 0,
    RightExpansionMask = 3 << 0,

    ForbidLeftExpansion = 0 << 2,
    AllowLeftExpansion = 1 << 2,
    ForceLeftExpansion = 2 << 2,
    LeftExpansionMask = 3 << 2,

    DefaultExpansion = AllowRightExpansion | ForbidLeftExpansion,
};
typedef unsigned ExpansionBehavior;

enum FontSynthesisValues {
    FontSynthesisNone = 0x0,
    FontSynthesisWeight = 0x1,
    FontSynthesisStyle = 0x2,
    FontSynthesisSmallCaps = 0x4
};
typedef unsigned FontSynthesis;
const unsigned FontSynthesisWidth = 3;

enum class FontVariantLigatures : uint8_t {
    Normal,
    Yes,
    No
};

enum class FontVariantPosition : uint8_t {
    Normal,
    Subscript,
    Superscript
};

enum class FontVariantCaps : uint8_t {
    Normal,
    Small,
    AllSmall,
    Petite,
    AllPetite,
    Unicase,
    Titling
};

enum class FontVariantNumericFigure : uint8_t {
    Normal,
    LiningNumbers,
    OldStyleNumbers
};

enum class FontVariantNumericSpacing : uint8_t {
    Normal,
    ProportionalNumbers,
    TabularNumbers
};

enum class FontVariantNumericFraction : uint8_t {
    Normal,
    DiagonalFractions,
    StackedFractions
};

enum class FontVariantNumericOrdinal : uint8_t {
    Normal,
    Yes
};

enum class FontVariantNumericSlashedZero : uint8_t {
    Normal,
    Yes
};

enum class FontVariantAlternates : uint8_t {
    Normal,
    HistoricalForms
};

enum class FontVariantEastAsianVariant : uint8_t {
    Normal,
    Jis78,
    Jis83,
    Jis90,
    Jis04,
    Simplified,
    Traditional
};

enum class FontVariantEastAsianWidth : uint8_t {
    Normal,
    Full,
    Proportional
};

enum class FontVariantEastAsianRuby : uint8_t {
    Normal,
    Yes
};

struct FontVariantSettings {
    FontVariantSettings()
        : commonLigatures(FontVariantLigatures::Normal)
        , discretionaryLigatures(FontVariantLigatures::Normal)
        , historicalLigatures(FontVariantLigatures::Normal)
        , contextualAlternates(FontVariantLigatures::Normal)
        , position(FontVariantPosition::Normal)
        , caps(FontVariantCaps::Normal)
        , numericFigure(FontVariantNumericFigure::Normal)
        , numericSpacing(FontVariantNumericSpacing::Normal)
        , numericFraction(FontVariantNumericFraction::Normal)
        , numericOrdinal(FontVariantNumericOrdinal::Normal)
        , numericSlashedZero(FontVariantNumericSlashedZero::Normal)
        , alternates(FontVariantAlternates::Normal)
        , eastAsianVariant(FontVariantEastAsianVariant::Normal)
        , eastAsianWidth(FontVariantEastAsianWidth::Normal)
        , eastAsianRuby(FontVariantEastAsianRuby::Normal)
    {
    }

    FontVariantSettings(
        FontVariantLigatures commonLigatures,
        FontVariantLigatures discretionaryLigatures,
        FontVariantLigatures historicalLigatures,
        FontVariantLigatures contextualAlternates,
        FontVariantPosition position,
        FontVariantCaps caps,
        FontVariantNumericFigure numericFigure,
        FontVariantNumericSpacing numericSpacing,
        FontVariantNumericFraction numericFraction,
        FontVariantNumericOrdinal numericOrdinal,
        FontVariantNumericSlashedZero numericSlashedZero,
        FontVariantAlternates alternates,
        FontVariantEastAsianVariant eastAsianVariant,
        FontVariantEastAsianWidth eastAsianWidth,
        FontVariantEastAsianRuby eastAsianRuby)
            : commonLigatures(commonLigatures)
            , discretionaryLigatures(discretionaryLigatures)
            , historicalLigatures(historicalLigatures)
            , contextualAlternates(contextualAlternates)
            , position(position)
            , caps(caps)
            , numericFigure(numericFigure)
            , numericSpacing(numericSpacing)
            , numericFraction(numericFraction)
            , numericOrdinal(numericOrdinal)
            , numericSlashedZero(numericSlashedZero)
            , alternates(alternates)
            , eastAsianVariant(eastAsianVariant)
            , eastAsianWidth(eastAsianWidth)
            , eastAsianRuby(eastAsianRuby)
    {
    }

    bool isAllNormal() const
    {
        return commonLigatures == FontVariantLigatures::Normal
            && discretionaryLigatures == FontVariantLigatures::Normal
            && historicalLigatures == FontVariantLigatures::Normal
            && contextualAlternates == FontVariantLigatures::Normal
            && position == FontVariantPosition::Normal
            && caps == FontVariantCaps::Normal
            && numericFigure == FontVariantNumericFigure::Normal
            && numericSpacing == FontVariantNumericSpacing::Normal
            && numericFraction == FontVariantNumericFraction::Normal
            && numericOrdinal == FontVariantNumericOrdinal::Normal
            && numericSlashedZero == FontVariantNumericSlashedZero::Normal
            && alternates == FontVariantAlternates::Normal
            && eastAsianVariant == FontVariantEastAsianVariant::Normal
            && eastAsianWidth == FontVariantEastAsianWidth::Normal
            && eastAsianRuby == FontVariantEastAsianRuby::Normal;
    }

    bool operator==(const FontVariantSettings& other) const
    {
        return commonLigatures == other.commonLigatures
            && discretionaryLigatures == other.discretionaryLigatures
            && historicalLigatures == other.historicalLigatures
            && contextualAlternates == other.contextualAlternates
            && position == other.position
            && caps == other.caps
            && numericFigure == other.numericFigure
            && numericSpacing == other.numericSpacing
            && numericFraction == other.numericFraction
            && numericOrdinal == other.numericOrdinal
            && numericSlashedZero == other.numericSlashedZero
            && alternates == other.alternates
            && eastAsianVariant == other.eastAsianVariant
            && eastAsianWidth == other.eastAsianWidth
            && eastAsianRuby == other.eastAsianRuby;
    }

    bool operator!=(const FontVariantSettings& other) const { return !(*this == other); }

    unsigned uniqueValue() const
    {
        return static_cast<unsigned>(commonLigatures) << 26
            | static_cast<unsigned>(discretionaryLigatures) << 24
            | static_cast<unsigned>(historicalLigatures) << 22
            | static_cast<unsigned>(contextualAlternates) << 20
            | static_cast<unsigned>(position) << 18
            | static_cast<unsigned>(caps) << 15
            | static_cast<unsigned>(numericFigure) << 13
            | static_cast<unsigned>(numericSpacing) << 11
            | static_cast<unsigned>(numericFraction) << 9
            | static_cast<unsigned>(numericOrdinal) << 8
            | static_cast<unsigned>(numericSlashedZero) << 7
            | static_cast<unsigned>(alternates) << 6
            | static_cast<unsigned>(eastAsianVariant) << 3
            | static_cast<unsigned>(eastAsianWidth) << 1
            | static_cast<unsigned>(eastAsianRuby) << 0;
    }

    template<class Encoder> void encode(Encoder&) const;
    template<class Decoder> static Optional<FontVariantSettings> decode(Decoder&);

    // FIXME: this would be much more compact with bitfields.
    FontVariantLigatures commonLigatures;
    FontVariantLigatures discretionaryLigatures;
    FontVariantLigatures historicalLigatures;
    FontVariantLigatures contextualAlternates;
    FontVariantPosition position;
    FontVariantCaps caps;
    FontVariantNumericFigure numericFigure;
    FontVariantNumericSpacing numericSpacing;
    FontVariantNumericFraction numericFraction;
    FontVariantNumericOrdinal numericOrdinal;
    FontVariantNumericSlashedZero numericSlashedZero;
    FontVariantAlternates alternates;
    FontVariantEastAsianVariant eastAsianVariant;
    FontVariantEastAsianWidth eastAsianWidth;
    FontVariantEastAsianRuby eastAsianRuby;
};

template<class Encoder>
void FontVariantSettings::encode(Encoder& encoder) const
{
    encoder << commonLigatures;
    encoder << discretionaryLigatures;
    encoder << historicalLigatures;
    encoder << contextualAlternates;
    encoder << position;
    encoder << caps;
    encoder << numericFigure;
    encoder << numericSpacing;
    encoder << numericFraction;
    encoder << numericOrdinal;
    encoder << numericSlashedZero;
    encoder << alternates;
    encoder << eastAsianVariant;
    encoder << eastAsianWidth;
    encoder << eastAsianRuby;
}

template<class Decoder>
Optional<FontVariantSettings> FontVariantSettings::decode(Decoder& decoder)
{
    Optional<FontVariantLigatures> commonLigatures;
    decoder >> commonLigatures;
    if (!commonLigatures)
        return WTF::nullopt;

    Optional<FontVariantLigatures> discretionaryLigatures;
    decoder >> discretionaryLigatures;
    if (!discretionaryLigatures)
        return WTF::nullopt;

    Optional<FontVariantLigatures> historicalLigatures;
    decoder >> historicalLigatures;
    if (!historicalLigatures)
        return WTF::nullopt;

    Optional<FontVariantLigatures> contextualAlternates;
    decoder >> contextualAlternates;
    if (!contextualAlternates)
        return WTF::nullopt;

    Optional<FontVariantPosition> position;
    decoder >> position;
    if (!position)
        return WTF::nullopt;

    Optional<FontVariantCaps> caps;
    decoder >> caps;
    if (!caps)
        return WTF::nullopt;

    Optional<FontVariantNumericFigure> numericFigure;
    decoder >> numericFigure;
    if (!numericFigure)
        return WTF::nullopt;

    Optional<FontVariantNumericSpacing> numericSpacing;
    decoder >> numericSpacing;
    if (!numericSpacing)
        return WTF::nullopt;

    Optional<FontVariantNumericFraction> numericFraction;
    decoder >> numericFraction;
    if (!numericFraction)
        return WTF::nullopt;

    Optional<FontVariantNumericOrdinal> numericOrdinal;
    decoder >> numericOrdinal;
    if (!numericOrdinal)
        return WTF::nullopt;

    Optional<FontVariantNumericSlashedZero> numericSlashedZero;
    decoder >> numericSlashedZero;
    if (!numericSlashedZero)
        return WTF::nullopt;

    Optional<FontVariantAlternates> alternates;
    decoder >> alternates;
    if (!alternates)
        return WTF::nullopt;

    Optional<FontVariantEastAsianVariant> eastAsianVariant;
    decoder >> eastAsianVariant;
    if (!eastAsianVariant)
        return WTF::nullopt;

    Optional<FontVariantEastAsianWidth> eastAsianWidth;
    decoder >> eastAsianWidth;
    if (!eastAsianWidth)
        return WTF::nullopt;

    Optional<FontVariantEastAsianRuby> eastAsianRuby;
    decoder >> eastAsianRuby;
    if (!eastAsianRuby)
        return WTF::nullopt;

    return {{
        *commonLigatures,
        *discretionaryLigatures,
        *historicalLigatures,
        *contextualAlternates,
        *position,
        *caps,
        *numericFigure,
        *numericSpacing,
        *numericFraction,
        *numericOrdinal,
        *numericSlashedZero,
        *alternates,
        *eastAsianVariant,
        *eastAsianWidth,
        *eastAsianRuby
    }};
}

struct FontVariantLigaturesValues {
    FontVariantLigaturesValues(
        FontVariantLigatures commonLigatures,
        FontVariantLigatures discretionaryLigatures,
        FontVariantLigatures historicalLigatures,
        FontVariantLigatures contextualAlternates)
            : commonLigatures(commonLigatures)
            , discretionaryLigatures(discretionaryLigatures)
            , historicalLigatures(historicalLigatures)
            , contextualAlternates(contextualAlternates)
    {
    }

    template<class Encoder> void encode(Encoder&) const;
    template<class Decoder> static Optional<FontVariantLigaturesValues> decode(Decoder&);

    FontVariantLigatures commonLigatures;
    FontVariantLigatures discretionaryLigatures;
    FontVariantLigatures historicalLigatures;
    FontVariantLigatures contextualAlternates;
};

template<class Encoder>
void FontVariantLigaturesValues::encode(Encoder& encoder) const
{
    encoder << commonLigatures;
    encoder << discretionaryLigatures;
    encoder << historicalLigatures;
    encoder << contextualAlternates;
}

template<class Decoder>
Optional<FontVariantLigaturesValues> FontVariantLigaturesValues::decode(Decoder& decoder)
{
    Optional<FontVariantLigatures> commonLigatures;
    decoder >> commonLigatures;
    if (!commonLigatures)
        return WTF::nullopt;

    Optional<FontVariantLigatures> discretionaryLigatures;
    decoder >> discretionaryLigatures;
    if (!discretionaryLigatures)
        return WTF::nullopt;

    Optional<FontVariantLigatures> historicalLigatures;
    decoder >> historicalLigatures;
    if (!historicalLigatures)
        return WTF::nullopt;

    Optional<FontVariantLigatures> contextualAlternates;
    decoder >> contextualAlternates;
    if (!contextualAlternates)
        return WTF::nullopt;

    return {{ *commonLigatures, *discretionaryLigatures, *historicalLigatures, *contextualAlternates }};
}

struct FontVariantNumericValues {
    FontVariantNumericValues(
        FontVariantNumericFigure figure,
        FontVariantNumericSpacing spacing,
        FontVariantNumericFraction fraction,
        FontVariantNumericOrdinal ordinal,
        FontVariantNumericSlashedZero slashedZero)
            : figure(figure)
            , spacing(spacing)
            , fraction(fraction)
            , ordinal(ordinal)
            , slashedZero(slashedZero)
    {
    }

    template<class Encoder> void encode(Encoder&) const;
    template<class Decoder> static Optional<FontVariantNumericValues> decode(Decoder&);

    FontVariantNumericFigure figure;
    FontVariantNumericSpacing spacing;
    FontVariantNumericFraction fraction;
    FontVariantNumericOrdinal ordinal;
    FontVariantNumericSlashedZero slashedZero;
};

template<class Encoder>
void FontVariantNumericValues::encode(Encoder& encoder) const
{
    encoder << figure;
    encoder << spacing;
    encoder << fraction;
    encoder << ordinal;
    encoder << slashedZero;
}

template<class Decoder>
Optional<FontVariantNumericValues> FontVariantNumericValues::decode(Decoder& decoder)
{
    Optional<FontVariantNumericFigure> figure;
    decoder >> figure;
    if (!figure)
        return WTF::nullopt;

    Optional<FontVariantNumericSpacing> spacing;
    decoder >> spacing;
    if (!spacing)
        return WTF::nullopt;

    Optional<FontVariantNumericFraction> fraction;
    decoder >> fraction;
    if (!fraction)
        return WTF::nullopt;

    Optional<FontVariantNumericOrdinal> ordinal;
    decoder >> ordinal;
    if (!ordinal)
        return WTF::nullopt;

    Optional<FontVariantNumericSlashedZero> slashedZero;
    decoder >> slashedZero;
    if (!slashedZero)
        return WTF::nullopt;

    return {{ *figure, *spacing, *fraction, *ordinal, *slashedZero }};
}

struct FontVariantEastAsianValues {
    FontVariantEastAsianValues(
        FontVariantEastAsianVariant variant,
        FontVariantEastAsianWidth width,
        FontVariantEastAsianRuby ruby)
            : variant(variant)
            , width(width)
            , ruby(ruby)
    {
    }

    template<class Encoder> void encode(Encoder&) const;
    template<class Decoder> static Optional<FontVariantEastAsianValues> decode(Decoder&);

    FontVariantEastAsianVariant variant;
    FontVariantEastAsianWidth width;
    FontVariantEastAsianRuby ruby;
};

template<class Encoder>
void FontVariantEastAsianValues::encode(Encoder& encoder) const
{
    encoder << variant;
    encoder << width;
    encoder << ruby;
}

template<class Decoder>
Optional<FontVariantEastAsianValues> FontVariantEastAsianValues::decode(Decoder& decoder)
{
    Optional<FontVariantEastAsianVariant> variant;
    decoder >> variant;
    if (!variant)
        return WTF::nullopt;

    Optional<FontVariantEastAsianWidth> width;
    decoder >> width;
    if (!width)
        return WTF::nullopt;

    Optional<FontVariantEastAsianRuby> ruby;
    decoder >> ruby;
    if (!ruby)
        return WTF::nullopt;

    return {{ *variant, *width, *ruby }};
}

enum class FontWidthVariant : uint8_t {
    RegularWidth,
    HalfWidth,
    ThirdWidth,
    QuarterWidth,
    LastFontWidthVariant = QuarterWidth
};

const unsigned FontWidthVariantWidth = 2;

COMPILE_ASSERT(!(static_cast<unsigned>(FontWidthVariant::LastFontWidthVariant) >> FontWidthVariantWidth), FontWidthVariantWidth_is_correct);

enum class FontSmallCaps : uint8_t {
    Off = 0,
    On = 1
};

enum class Kerning : uint8_t {
    Auto,
    Normal,
    NoShift
};

enum class FontOpticalSizing : uint8_t {
    Enabled,
    Disabled
};

// https://www.microsoft.com/typography/otspec/fvar.htm#VAT
enum class FontStyleAxis : uint8_t {
    slnt,
    ital
};

enum class AllowUserInstalledFonts : uint8_t {
    No,
    Yes
};

}

namespace WTF {

template<> struct EnumTraits<CyberCore::TextRenderingMode> {
    using values = EnumValues<
    CyberCore::TextRenderingMode,
    CyberCore::TextRenderingMode::AutoTextRendering,
    CyberCore::TextRenderingMode::OptimizeSpeed,
    CyberCore::TextRenderingMode::OptimizeLegibility,
    CyberCore::TextRenderingMode::GeometricPrecision
    >;
};

template<> struct EnumTraits<CyberCore::FontSmoothingMode> {
    using values = EnumValues<
    CyberCore::FontSmoothingMode,
    CyberCore::FontSmoothingMode::AutoSmoothing,
    CyberCore::FontSmoothingMode::NoSmoothing,
    CyberCore::FontSmoothingMode::Antialiased,
    CyberCore::FontSmoothingMode::SubpixelAntialiased,
    CyberCore::FontSmoothingMode::AutoSmoothing
    >;
};

template<> struct EnumTraits<CyberCore::FontRenderingMode> {
    using values = EnumValues<
    CyberCore::FontRenderingMode,
    CyberCore::FontRenderingMode::Normal,
    CyberCore::FontRenderingMode::Alternate
    >;
};

template<> struct EnumTraits<CyberCore::FontOrientation> {
    using values = EnumValues<
    CyberCore::FontOrientation,
    CyberCore::FontOrientation::Horizontal,
    CyberCore::FontOrientation::Vertical
    >;
};

template<> struct EnumTraits<CyberCore::NonCJKGlyphOrientation> {
    using values = EnumValues<
    CyberCore::NonCJKGlyphOrientation,
    CyberCore::NonCJKGlyphOrientation::Mixed,
    CyberCore::NonCJKGlyphOrientation::Upright
    >;
};

template<> struct EnumTraits<CyberCore::FontVariantLigatures> {
    using values = EnumValues<
    CyberCore::FontVariantLigatures,
    CyberCore::FontVariantLigatures::Normal,
    CyberCore::FontVariantLigatures::Yes,
    CyberCore::FontVariantLigatures::No
    >;
};

template<> struct EnumTraits<CyberCore::FontVariantPosition> {
    using values = EnumValues<
    CyberCore::FontVariantPosition,
    CyberCore::FontVariantPosition::Normal,
    CyberCore::FontVariantPosition::Subscript,
    CyberCore::FontVariantPosition::Superscript
    >;
};

template<> struct EnumTraits<CyberCore::FontVariantCaps> {
    using values = EnumValues<
    CyberCore::FontVariantCaps,
    CyberCore::FontVariantCaps::Normal,
    CyberCore::FontVariantCaps::Small,
    CyberCore::FontVariantCaps::AllSmall,
    CyberCore::FontVariantCaps::Petite,
    CyberCore::FontVariantCaps::AllPetite,
    CyberCore::FontVariantCaps::Unicase,
    CyberCore::FontVariantCaps::Titling
    >;
};

template<> struct EnumTraits<CyberCore::FontVariantNumericFigure> {
    using values = EnumValues<
    CyberCore::FontVariantNumericFigure,
    CyberCore::FontVariantNumericFigure::Normal,
    CyberCore::FontVariantNumericFigure::LiningNumbers,
    CyberCore::FontVariantNumericFigure::OldStyleNumbers
    >;
};

template<> struct EnumTraits<CyberCore::FontVariantNumericSpacing> {
    using values = EnumValues<
    CyberCore::FontVariantNumericSpacing,
    CyberCore::FontVariantNumericSpacing::Normal,
    CyberCore::FontVariantNumericSpacing::ProportionalNumbers,
    CyberCore::FontVariantNumericSpacing::TabularNumbers
    >;
};

template<> struct EnumTraits<CyberCore::FontVariantNumericFraction> {
    using values = EnumValues<
    CyberCore::FontVariantNumericFraction,
    CyberCore::FontVariantNumericFraction::Normal,
    CyberCore::FontVariantNumericFraction::DiagonalFractions,
    CyberCore::FontVariantNumericFraction::StackedFractions
    >;
};

template<> struct EnumTraits<CyberCore::FontVariantNumericOrdinal> {
    using values = EnumValues<
    CyberCore::FontVariantNumericOrdinal,
    CyberCore::FontVariantNumericOrdinal::Normal,
    CyberCore::FontVariantNumericOrdinal::Yes
    >;
};

template<> struct EnumTraits<CyberCore::FontVariantNumericSlashedZero> {
    using values = EnumValues<
    CyberCore::FontVariantNumericSlashedZero,
    CyberCore::FontVariantNumericSlashedZero::Normal,
    CyberCore::FontVariantNumericSlashedZero::Yes
    >;
};

template<> struct EnumTraits<CyberCore::FontVariantAlternates> {
    using values = EnumValues<
    CyberCore::FontVariantAlternates,
    CyberCore::FontVariantAlternates::Normal,
    CyberCore::FontVariantAlternates::HistoricalForms
    >;
};

template<> struct EnumTraits<CyberCore::FontVariantEastAsianVariant> {
    using values = EnumValues<
    CyberCore::FontVariantEastAsianVariant,
    CyberCore::FontVariantEastAsianVariant::Normal,
    CyberCore::FontVariantEastAsianVariant::Jis78,
    CyberCore::FontVariantEastAsianVariant::Jis83,
    CyberCore::FontVariantEastAsianVariant::Jis90,
    CyberCore::FontVariantEastAsianVariant::Jis04,
    CyberCore::FontVariantEastAsianVariant::Simplified,
    CyberCore::FontVariantEastAsianVariant::Traditional,
    CyberCore::FontVariantEastAsianVariant::Normal
    >;
};

template<> struct EnumTraits<CyberCore::FontVariantEastAsianWidth> {
    using values = EnumValues<
    CyberCore::FontVariantEastAsianWidth,
    CyberCore::FontVariantEastAsianWidth::Normal,
    CyberCore::FontVariantEastAsianWidth::Full,
    CyberCore::FontVariantEastAsianWidth::Proportional
    >;
};

template<> struct EnumTraits<CyberCore::FontVariantEastAsianRuby> {
    using values = EnumValues<
    CyberCore::FontVariantEastAsianRuby,
    CyberCore::FontVariantEastAsianRuby::Normal,
    CyberCore::FontVariantEastAsianRuby::Yes
    >;
};

template<> struct EnumTraits<CyberCore::FontWidthVariant> {
    using values = EnumValues<
    CyberCore::FontWidthVariant,
    CyberCore::FontWidthVariant::RegularWidth,
    CyberCore::FontWidthVariant::HalfWidth,
    CyberCore::FontWidthVariant::ThirdWidth,
    CyberCore::FontWidthVariant::QuarterWidth
    >;
};

template<> struct EnumTraits<CyberCore::FontSmallCaps> {
    using values = EnumValues<
    CyberCore::FontSmallCaps,
    CyberCore::FontSmallCaps::Off,
    CyberCore::FontSmallCaps::On
    >;
};

template<> struct EnumTraits<CyberCore::Kerning> {
    using values = EnumValues<
    CyberCore::Kerning,
    CyberCore::Kerning::Auto,
    CyberCore::Kerning::Normal,
    CyberCore::Kerning::NoShift
    >;
};

template<> struct EnumTraits<CyberCore::FontOpticalSizing> {
    using values = EnumValues<
    CyberCore::FontOpticalSizing,
    CyberCore::FontOpticalSizing::Enabled,
    CyberCore::FontOpticalSizing::Disabled
    >;
};

template<> struct EnumTraits<CyberCore::FontStyleAxis> {
    using values = EnumValues<
    CyberCore::FontStyleAxis,
    CyberCore::FontStyleAxis::slnt,
    CyberCore::FontStyleAxis::ital
    >;
};

template<> struct EnumTraits<CyberCore::AllowUserInstalledFonts> {
    using values = EnumValues<
    CyberCore::AllowUserInstalledFonts,
    CyberCore::AllowUserInstalledFonts::No,
    CyberCore::AllowUserInstalledFonts::Yes
    >;
};

} // namespace WTF
