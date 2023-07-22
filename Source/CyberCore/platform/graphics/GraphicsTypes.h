/*
 * Copyright (C) 2004, 2005, 2006 Apple Inc.  All rights reserved.
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

#include "WindRule.h"
#include <wtf/EnumTraits.h>
#include <wtf/Forward.h>

namespace WTF {
class TextStream;
}

namespace CyberCore {

enum class CompositeOperator : uint8_t {
    Clear,
    Copy,
    SourceOver,
    SourceIn,
    SourceOut,
    SourceAtop,
    DestinationOver,
    DestinationIn,
    DestinationOut,
    DestinationAtop,
    XOR,
    PlusDarker,
    PlusLighter,
    Difference
};

enum class BlendMode : uint8_t {
    Normal = 1, // Start with 1 to match SVG's blendmode enumeration.
    Multiply,
    Screen,
    Darken,
    Lighten,
    Overlay,
    ColorDodge,
    ColorBurn,
    HardLight,
    SoftLight,
    Difference,
    Exclusion,
    Hue,
    Saturation,
    Color,
    Luminosity,
    PlusDarker,
    PlusLighter
};

enum GradientSpreadMethod {
    SpreadMethodPad,
    SpreadMethodReflect,
    SpreadMethodRepeat
};

enum class InterpolationQuality : uint8_t {
    Default,
    DoNotInterpolate,
    Low,
    Medium,
    High
};

enum LineCap {
    ButtCap,
    RoundCap,
    SquareCap
};

enum LineJoin {
    MiterJoin,
    RoundJoin,
    BevelJoin
};

enum HorizontalAlignment {
    AlignLeft,
    AlignRight,
    AlignHCenter
};

enum TextBaseline {
    AlphabeticTextBaseline,
    TopTextBaseline,
    MiddleTextBaseline,
    BottomTextBaseline,
    IdeographicTextBaseline,
    HangingTextBaseline
};

enum TextAlign {
    StartTextAlign,
    EndTextAlign,
    LeftTextAlign,
    CenterTextAlign,
    RightTextAlign
};

enum RenderingMode {
    Unaccelerated,
    UnacceleratedNonPlatformBuffer, // Use plain memory allocation rather than platform API to allocate backing store.
    Accelerated
};

enum class AlphaPremultiplication {
    Premultiplied,
    Unpremultiplied
};

String compositeOperatorName(CyberCore::CompositeOperator, CyberCore::BlendMode);
String blendModeName(CyberCore::BlendMode);
bool parseBlendMode(const String&, CyberCore::BlendMode&);
bool parseCompositeAndBlendOperator(const String&, CyberCore::CompositeOperator&, CyberCore::BlendMode&);

WEBCORE_EXPORT WTF::TextStream& operator<<(WTF::TextStream&, CyberCore::BlendMode);
WEBCORE_EXPORT WTF::TextStream& operator<<(WTF::TextStream&, CyberCore::CompositeOperator);
WEBCORE_EXPORT WTF::TextStream& operator<<(WTF::TextStream&, WindRule);
WEBCORE_EXPORT WTF::TextStream& operator<<(WTF::TextStream&, LineCap);
WEBCORE_EXPORT WTF::TextStream& operator<<(WTF::TextStream&, LineJoin);
WEBCORE_EXPORT WTF::TextStream& operator<<(WTF::TextStream&, AlphaPremultiplication);

} // namespace CyberCore

namespace WTF {

template<> struct EnumTraits<CyberCore::CompositeOperator> {
    using values = EnumValues<
    CyberCore::CompositeOperator,
    CyberCore::CompositeOperator::Clear,
    CyberCore::CompositeOperator::Copy,
    CyberCore::CompositeOperator::SourceOver,
    CyberCore::CompositeOperator::SourceIn,
    CyberCore::CompositeOperator::SourceOut,
    CyberCore::CompositeOperator::SourceAtop,
    CyberCore::CompositeOperator::DestinationOver,
    CyberCore::CompositeOperator::DestinationIn,
    CyberCore::CompositeOperator::DestinationOut,
    CyberCore::CompositeOperator::DestinationAtop,
    CyberCore::CompositeOperator::XOR,
    CyberCore::CompositeOperator::PlusDarker,
    CyberCore::CompositeOperator::PlusLighter,
    CyberCore::CompositeOperator::Difference
    >;
};

template<> struct EnumTraits<CyberCore::BlendMode> {
    using values = EnumValues<
    CyberCore::BlendMode,
    CyberCore::BlendMode::Normal,
    CyberCore::BlendMode::Multiply,
    CyberCore::BlendMode::Screen,
    CyberCore::BlendMode::Darken,
    CyberCore::BlendMode::Lighten,
    CyberCore::BlendMode::Overlay,
    CyberCore::BlendMode::ColorDodge,
    CyberCore::BlendMode::ColorBurn,
    CyberCore::BlendMode::HardLight,
    CyberCore::BlendMode::SoftLight,
    CyberCore::BlendMode::Difference,
    CyberCore::BlendMode::Exclusion,
    CyberCore::BlendMode::Hue,
    CyberCore::BlendMode::Saturation,
    CyberCore::BlendMode::Color,
    CyberCore::BlendMode::Luminosity,
    CyberCore::BlendMode::PlusDarker,
    CyberCore::BlendMode::PlusLighter
    >;
};

template<> struct EnumTraits<CyberCore::LineCap> {
    using values = EnumValues<
    CyberCore::LineCap,
    CyberCore::LineCap::ButtCap,
    CyberCore::LineCap::RoundCap,
    CyberCore::LineCap::SquareCap
    >;
};

template<> struct EnumTraits<CyberCore::LineJoin> {
    using values = EnumValues<
    CyberCore::LineJoin,
    CyberCore::LineJoin::MiterJoin,
    CyberCore::LineJoin::RoundJoin,
    CyberCore::LineJoin::BevelJoin
    >;
};

template<> struct EnumTraits<CyberCore::InterpolationQuality> {
    using values = EnumValues<
    CyberCore::InterpolationQuality,
    CyberCore::InterpolationQuality::Default,
    CyberCore::InterpolationQuality::DoNotInterpolate,
    CyberCore::InterpolationQuality::Low,
    CyberCore::InterpolationQuality::Medium,
    CyberCore::InterpolationQuality::High
    >;
};

} // namespace WTF
