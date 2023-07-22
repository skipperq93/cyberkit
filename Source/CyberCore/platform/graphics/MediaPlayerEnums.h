/*
 * Copyright (C) 2015 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#pragma once

#include <wtf/text/WTFString.h>

namespace CyberCore {

class MediaPlayerEnums {
public:
    enum class NetworkState : uint8_t {
        Empty,
        Idle,
        Loading,
        Loaded,
        FormatError,
        NetworkError,
        DecodeError
    };

    enum class ReadyState : uint8_t {
        HaveNothing,
        HaveMetadata,
        HaveCurrentData,
        HaveFutureData,
        HaveEnoughData
    };

    enum class MovieLoadType : uint8_t {
        Unknown,
        Download,
        StoredStream,
        LiveStream
    };

    enum class Preload : uint8_t {
        None,
        MetaData,
        Auto
    };

    enum class VideoGravity : uint8_t {
        Resize,
        ResizeAspect,
        ResizeAspectFill
    };

    enum class SupportsType : uint8_t {
        IsNotSupported,
        IsSupported,
        MayBeSupported
    };

    enum {
        VideoFullscreenModeNone = 0,
        VideoFullscreenModeStandard = 1 << 0,
        VideoFullscreenModePictureInPicture = 1 << 1,
    };
    typedef uint32_t VideoFullscreenMode;

    enum class BufferingPolicy : uint8_t {
        Default,
        LimitReadAhead,
        MakeResourcesPurgeable,
        PurgeResources,
    };

    enum class MediaEngineIdentifier : uint8_t {
        AVFoundation,
        AVFoundationMSE,
        AVFoundationMediaStream,
        AVFoundationCF,
        GStreamer,
        GStreamerMSE,
        HolePunch,
        MediaFoundation,
        MockMSE,
    };
};

WTF::String convertEnumerationToString(MediaPlayerEnums::ReadyState);
WTF::String convertEnumerationToString(MediaPlayerEnums::NetworkState);
WTF::String convertEnumerationToString(MediaPlayerEnums::Preload);
WTF::String convertEnumerationToString(MediaPlayerEnums::SupportsType);
WTF::String convertEnumerationToString(MediaPlayerEnums::BufferingPolicy);

} // namespace CyberCore


namespace WTF {

template<typename Type>
struct LogArgument;

template <>
struct LogArgument<CyberCore::MediaPlayerEnums::ReadyState> {
    static WTF::String toString(const CyberCore::MediaPlayerEnums::ReadyState state)
    {
        return convertEnumerationToString(state);
    }
};

template <>
struct LogArgument<CyberCore::MediaPlayerEnums::NetworkState> {
    static WTF::String toString(const CyberCore::MediaPlayerEnums::NetworkState state)
    {
        return convertEnumerationToString(state);
    }
};

template <>
struct LogArgument<CyberCore::MediaPlayerEnums::BufferingPolicy> {
    static WTF::String toString(const CyberCore::MediaPlayerEnums::BufferingPolicy policy)
    {
        return convertEnumerationToString(policy);
    }
};

template<> struct EnumTraits<CyberCore::MediaPlayerEnums::NetworkState> {
    using values = EnumValues<
        CyberCore::MediaPlayerEnums::NetworkState,
        CyberCore::MediaPlayerEnums::NetworkState::Empty,
        CyberCore::MediaPlayerEnums::NetworkState::Idle,
        CyberCore::MediaPlayerEnums::NetworkState::Loading,
        CyberCore::MediaPlayerEnums::NetworkState::Loaded,
        CyberCore::MediaPlayerEnums::NetworkState::FormatError,
        CyberCore::MediaPlayerEnums::NetworkState::NetworkError,
        CyberCore::MediaPlayerEnums::NetworkState::DecodeError
    >;
};

template<> struct EnumTraits<CyberCore::MediaPlayerEnums::ReadyState> {
    using values = EnumValues<
        CyberCore::MediaPlayerEnums::ReadyState,
        CyberCore::MediaPlayerEnums::ReadyState::HaveNothing,
        CyberCore::MediaPlayerEnums::ReadyState::HaveMetadata,
        CyberCore::MediaPlayerEnums::ReadyState::HaveCurrentData,
        CyberCore::MediaPlayerEnums::ReadyState::HaveFutureData,
        CyberCore::MediaPlayerEnums::ReadyState::HaveEnoughData
    >;
};

template<> struct EnumTraits<CyberCore::MediaPlayerEnums::MovieLoadType> {
    using values = EnumValues<
        CyberCore::MediaPlayerEnums::MovieLoadType,
        CyberCore::MediaPlayerEnums::MovieLoadType::Unknown,
        CyberCore::MediaPlayerEnums::MovieLoadType::Download,
        CyberCore::MediaPlayerEnums::MovieLoadType::StoredStream,
        CyberCore::MediaPlayerEnums::MovieLoadType::LiveStream
    >;
};

template<> struct EnumTraits<CyberCore::MediaPlayerEnums::Preload> {
    using values = EnumValues<
        CyberCore::MediaPlayerEnums::Preload,
        CyberCore::MediaPlayerEnums::Preload::None,
        CyberCore::MediaPlayerEnums::Preload::MetaData,
        CyberCore::MediaPlayerEnums::Preload::Auto
    >;
};

template<> struct EnumTraits<CyberCore::MediaPlayerEnums::VideoGravity> {
    using values = EnumValues<
        CyberCore::MediaPlayerEnums::VideoGravity,
        CyberCore::MediaPlayerEnums::VideoGravity::Resize,
        CyberCore::MediaPlayerEnums::VideoGravity::ResizeAspect,
        CyberCore::MediaPlayerEnums::VideoGravity::ResizeAspectFill
    >;
};

template<> struct EnumTraits<CyberCore::MediaPlayerEnums::SupportsType> {
    using values = EnumValues<
        CyberCore::MediaPlayerEnums::SupportsType,
        CyberCore::MediaPlayerEnums::SupportsType::IsNotSupported,
        CyberCore::MediaPlayerEnums::SupportsType::IsSupported,
        CyberCore::MediaPlayerEnums::SupportsType::MayBeSupported
    >;
};

template<> struct EnumTraits<CyberCore::MediaPlayerEnums::BufferingPolicy> {
    using values = EnumValues<
        CyberCore::MediaPlayerEnums::BufferingPolicy,
        CyberCore::MediaPlayerEnums::BufferingPolicy::Default,
        CyberCore::MediaPlayerEnums::BufferingPolicy::LimitReadAhead,
        CyberCore::MediaPlayerEnums::BufferingPolicy::MakeResourcesPurgeable,
        CyberCore::MediaPlayerEnums::BufferingPolicy::PurgeResources
        >;
    };

template<> struct EnumTraits<CyberCore::MediaPlayerEnums::MediaEngineIdentifier> {
    using values = EnumValues<
        CyberCore::MediaPlayerEnums::MediaEngineIdentifier,
        CyberCore::MediaPlayerEnums::MediaEngineIdentifier::AVFoundation,
        CyberCore::MediaPlayerEnums::MediaEngineIdentifier::AVFoundationMSE,
        CyberCore::MediaPlayerEnums::MediaEngineIdentifier::AVFoundationMediaStream,
        CyberCore::MediaPlayerEnums::MediaEngineIdentifier::GStreamer,
        CyberCore::MediaPlayerEnums::MediaEngineIdentifier::GStreamerMSE,
        CyberCore::MediaPlayerEnums::MediaEngineIdentifier::HolePunch,
        CyberCore::MediaPlayerEnums::MediaEngineIdentifier::MediaFoundation,
        CyberCore::MediaPlayerEnums::MediaEngineIdentifier::MockMSE
    >;
};

}; // namespace WTF
