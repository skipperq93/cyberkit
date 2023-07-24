/*
 * Copyright (C) 2019 Apple Inc. All rights reserved.
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

#include <wtf/text/WTFString.h>

namespace CyberCore {

struct VideoConfiguration;
struct AudioConfiguration;
struct MediaConfiguration;
struct MediaDecodingConfiguration;
struct MediaEncodingConfiguration;
struct MediaCapabilitiesInfo;
struct MediaCapabilitiesDecodingInfo;
struct MediaCapabilitiesEncodingInfo;

enum class ColorGamut;
enum class HdrMetadataType;
enum class TransferFunction;
enum class MediaDecodingType;
enum class MediaEncodingType;

}

namespace WTF {

template<typename>
struct LogArgument;

template <>
struct LogArgument<CyberCore::VideoConfiguration> {
    static String toString(const CyberCore::VideoConfiguration&);
};

template <>
struct LogArgument<CyberCore::AudioConfiguration> {
    static String toString(const CyberCore::AudioConfiguration&);
};

template <>
struct LogArgument<CyberCore::MediaConfiguration> {
    static String toString(const CyberCore::MediaConfiguration&);
};

template <>
struct LogArgument<CyberCore::MediaDecodingConfiguration> {
    static String toString(const CyberCore::MediaDecodingConfiguration&);
};

template <>
struct LogArgument<CyberCore::MediaEncodingConfiguration> {
    static String toString(const CyberCore::MediaEncodingConfiguration&);
};

template <>
struct LogArgument<CyberCore::MediaCapabilitiesInfo> {
    static String toString(const CyberCore::MediaCapabilitiesInfo&);
};

template <>
struct LogArgument<CyberCore::MediaCapabilitiesDecodingInfo> {
    static String toString(const CyberCore::MediaCapabilitiesDecodingInfo&);
};

template <>
struct LogArgument<CyberCore::MediaCapabilitiesEncodingInfo> {
    static String toString(const CyberCore::MediaCapabilitiesEncodingInfo&);
};

template <>
struct LogArgument<CyberCore::ColorGamut> {
    static String toString(const CyberCore::ColorGamut&);
};

template <>
struct LogArgument<CyberCore::HdrMetadataType> {
    static String toString(const CyberCore::HdrMetadataType&);
};

template <>
struct LogArgument<CyberCore::TransferFunction> {
    static String toString(const CyberCore::TransferFunction&);
};

template <>
struct LogArgument<CyberCore::MediaDecodingType> {
    static String toString(const CyberCore::MediaDecodingType&);
};

template <>
struct LogArgument<CyberCore::MediaEncodingType> {
    static String toString(const CyberCore::MediaEncodingType&);
};

}
