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

#include "config.h"
#include "MediaCapabilitiesLogging.h"

#include "AudioConfiguration.h"
#include "JSColorGamut.h"
#include "JSHdrMetadataType.h"
#include "JSMediaDecodingType.h"
#include "JSMediaEncodingType.h"
#include "JSTransferFunction.h"
#include "MediaCapabilitiesDecodingInfo.h"
#include "MediaCapabilitiesEncodingInfo.h"
#include "MediaDecodingConfiguration.h"
#include "MediaDecodingType.h"
#include "MediaEncodingConfiguration.h"
#include "MediaEncodingType.h"
#include "VideoConfiguration.h"
#include <wtf/JSONValues.h>

namespace CyberCore {

static Ref<JSON::Object> toJSONObject(const VideoConfiguration& configuration)
{
    auto object = JSON::Object::create();
    object->setString("contentType"_s, configuration.contentType);
    object->setInteger("width"_s, configuration.width);
    object->setInteger("height"_s, configuration.height);
    object->setInteger("bitrate"_s, static_cast<int>(configuration.bitrate));
    object->setDouble("framerate"_s, configuration.framerate);
    if (configuration.alphaChannel)
        object->setBoolean("alphaChannel"_s, configuration.alphaChannel.value());
    if (configuration.colorGamut)
        object->setString("colorGamut"_s, convertEnumerationToString(configuration.colorGamut.value()));
    if (configuration.hdrMetadataType)
        object->setString("hdrMetadataType"_s, convertEnumerationToString(configuration.hdrMetadataType.value()));
    if (configuration.transferFunction)
        object->setString("transferFunction"_s, convertEnumerationToString(configuration.transferFunction.value()));
    return object;
}

static Ref<JSON::Object> toJSONObject(const AudioConfiguration& configuration)
{
    auto object = JSON::Object::create();
    object->setString("contentType"_s, configuration.contentType);
    object->setString("channels"_s, configuration.channels);
    object->setInteger("bitrate"_s, static_cast<int>(configuration.bitrate));
    object->setDouble("samplerate"_s, configuration.samplerate);
    return object;
}

static Ref<JSON::Object> toJSONObject(const MediaConfiguration& configuration)
{
    auto object = JSON::Object::create();
    if (configuration.video)
        object->setValue("video"_s, toJSONObject(configuration.video.value()));
    if (configuration.audio)
        object->setValue("audio"_s, toJSONObject(configuration.audio.value()));
    return object;
}

static Ref<JSON::Object> toJSONObject(const MediaDecodingConfiguration& configuration)
{
    auto object = toJSONObject(static_cast<const MediaConfiguration&>(configuration));
    object->setString("type"_s, convertEnumerationToString(configuration.type));
    return object;
}

static Ref<JSON::Object> toJSONObject(const MediaEncodingConfiguration& configuration)
{
    auto object = toJSONObject(static_cast<const MediaConfiguration&>(configuration));
    object->setString("type"_s, convertEnumerationToString(configuration.type));
    return object;
}

static Ref<JSON::Object> toJSONObject(const MediaCapabilitiesInfo& info)
{
    auto object = JSON::Object::create();
    object->setBoolean("supported"_s, info.supported);
    object->setBoolean("smooth"_s, info.smooth);
    object->setBoolean("powerEfficient"_s, info.powerEfficient);
    return object;
}

static Ref<JSON::Object> toJSONObject(const MediaCapabilitiesDecodingInfo& info)
{
    auto object = toJSONObject(static_cast<const MediaCapabilitiesInfo&>(info));
    object->setValue("supportedConfiguration"_s, toJSONObject(info.supportedConfiguration));
    return object;
}

static Ref<JSON::Object> toJSONObject(const MediaCapabilitiesEncodingInfo& info)
{
    auto object = toJSONObject(static_cast<const MediaCapabilitiesInfo&>(info));
    object->setValue("supportedConfiguration"_s, toJSONObject(info.supportedConfiguration));
    return object;
}

static String toJSONString(const VideoConfiguration& configuration)
{
    return toJSONObject(configuration)->toJSONString();
}

static String toJSONString(const AudioConfiguration& configuration)
{
    return toJSONObject(configuration)->toJSONString();
}

static String toJSONString(const MediaConfiguration& configuration)
{
    return toJSONObject(configuration)->toJSONString();
}

static String toJSONString(const MediaDecodingConfiguration& configuration)
{
    return toJSONObject(configuration)->toJSONString();
}

static String toJSONString(const MediaEncodingConfiguration& configuration)
{
    return toJSONObject(configuration)->toJSONString();
}

static String toJSONString(const MediaCapabilitiesInfo& info)
{
    return toJSONObject(info)->toJSONString();
}

static String toJSONString(const MediaCapabilitiesDecodingInfo& info)
{
    return toJSONObject(info)->toJSONString();
}

static String toJSONString(const MediaCapabilitiesEncodingInfo& info)
{
    return toJSONObject(info)->toJSONString();
}

}

namespace WTF {

String LogArgument<CyberCore::VideoConfiguration>::toString(const CyberCore::VideoConfiguration& configuration)
{
    return toJSONString(configuration);
}

String LogArgument<CyberCore::AudioConfiguration>::toString(const CyberCore::AudioConfiguration& configuration)
{
    return toJSONString(configuration);
}

String LogArgument<CyberCore::MediaConfiguration>::toString(const CyberCore::MediaConfiguration& configuration)
{
    return toJSONString(configuration);
}

String LogArgument<CyberCore::MediaDecodingConfiguration>::toString(const CyberCore::MediaDecodingConfiguration& configuration)
{
    return toJSONString(configuration);
}

String LogArgument<CyberCore::MediaEncodingConfiguration>::toString(const CyberCore::MediaEncodingConfiguration& configuration)
{
    return toJSONString(configuration);
}

String LogArgument<CyberCore::MediaCapabilitiesInfo>::toString(const CyberCore::MediaCapabilitiesInfo& info)
{
    return toJSONString(info);
}

String LogArgument<CyberCore::MediaCapabilitiesDecodingInfo>::toString(const CyberCore::MediaCapabilitiesDecodingInfo& info)
{
    return toJSONString(info);
}

String LogArgument<CyberCore::MediaCapabilitiesEncodingInfo>::toString(const CyberCore::MediaCapabilitiesEncodingInfo& info)
{
    return toJSONString(info);
}

String LogArgument<CyberCore::ColorGamut>::toString(const CyberCore::ColorGamut& type)
{
    return convertEnumerationToString(type);
}

String LogArgument<CyberCore::HdrMetadataType>::toString(const CyberCore::HdrMetadataType& type)
{
    return convertEnumerationToString(type);
}

String LogArgument<CyberCore::TransferFunction>::toString(const CyberCore::TransferFunction& type)
{
    return convertEnumerationToString(type);
}

String LogArgument<CyberCore::MediaDecodingType>::toString(const CyberCore::MediaDecodingType& type)
{
    return convertEnumerationToString(type);
}

String LogArgument<CyberCore::MediaEncodingType>::toString(const CyberCore::MediaEncodingType& type)
{
    return convertEnumerationToString(type);
}

}
