/*
 * Copyright (C) 2012 Apple Inc. All rights reserved.
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

#include <wtf/EnumTraits.h>

namespace CyberKit {

enum class GestureType : uint8_t {
    Loupe,
    OneFingerTap,
    TapAndAHalf,
    DoubleTap,
    TapAndHalf,
    DoubleTapInUneditable,
    OneFingerTapInUneditable,
    OneFingerTapSelectsAll,
    OneFingerDoubleTap,
    OneFingerTripleTap,
    TwoFingerSingleTap,
    TwoFingerRangedSelectGesture,
    TapOnLinkWithGesture,
    PhraseBoundary
};

enum class SelectionTouch : uint8_t {
    Started,
    Moved,
    Ended,
    EndedMovingForward,
    EndedMovingBackward,
    EndedNotMoving
};

enum class GestureRecognizerState : uint8_t {
    Possible,
    Began,
    Changed,
    Ended,
    Cancelled,
    Failed
};

enum class SheetAction : bool {
    Copy,
    SaveImage
};

enum SelectionFlags : uint8_t {
    WordIsNearTap = 1 << 0,
    PhraseBoundaryChanged = 1 << 1,
};

enum class RespectSelectionAnchor : bool {
    No,
    Yes
};

} // namespace CyberKit

namespace WTF {

template<> struct EnumTraits<CyberKit::GestureRecognizerState> {
    using values = EnumValues<
        CyberKit::GestureRecognizerState,
        CyberKit::GestureRecognizerState::Possible,
        CyberKit::GestureRecognizerState::Began,
        CyberKit::GestureRecognizerState::Changed,
        CyberKit::GestureRecognizerState::Ended,
        CyberKit::GestureRecognizerState::Cancelled,
        CyberKit::GestureRecognizerState::Failed
    >;
};

template<> struct EnumTraits<CyberKit::GestureType> {
    using values = EnumValues<
        CyberKit::GestureType,
        CyberKit::GestureType::Loupe,
        CyberKit::GestureType::OneFingerTap,
        CyberKit::GestureType::TapAndAHalf,
        CyberKit::GestureType::DoubleTap,
        CyberKit::GestureType::TapAndHalf,
        CyberKit::GestureType::DoubleTapInUneditable,
        CyberKit::GestureType::OneFingerTapInUneditable,
        CyberKit::GestureType::OneFingerTapSelectsAll,
        CyberKit::GestureType::OneFingerDoubleTap,
        CyberKit::GestureType::OneFingerTripleTap,
        CyberKit::GestureType::TwoFingerSingleTap,
        CyberKit::GestureType::TwoFingerRangedSelectGesture,
        CyberKit::GestureType::TapOnLinkWithGesture,
        CyberKit::GestureType::PhraseBoundary
    >;
};

template<> struct EnumTraits<CyberKit::SelectionFlags> {
    using values = EnumValues<
        CyberKit::SelectionFlags,
        CyberKit::SelectionFlags::WordIsNearTap,
        CyberKit::SelectionFlags::PhraseBoundaryChanged
    >;
};

template<> struct EnumTraits<CyberKit::SelectionTouch> {
    using values = EnumValues<
        CyberKit::SelectionTouch,
        CyberKit::SelectionTouch::Started,
        CyberKit::SelectionTouch::Moved,
        CyberKit::SelectionTouch::Ended,
        CyberKit::SelectionTouch::EndedMovingForward,
        CyberKit::SelectionTouch::EndedMovingBackward,
        CyberKit::SelectionTouch::EndedNotMoving
    >;
};

} // namespace WTF
