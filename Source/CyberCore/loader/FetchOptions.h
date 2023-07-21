/*
 * Copyright (C) 2016 Canon Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions
 * are required to be met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Canon Inc. nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY CANON INC. AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CANON INC. AND ITS CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "DocumentIdentifier.h"
#include "ReferrerPolicy.h"
#include <wtf/text/WTFString.h>

namespace CyberCore {

struct FetchOptions {
    enum class Destination : uint8_t { EmptyString, Audio, Document, Embed, Font, Image, Manifest, Object, Report, Script, Serviceworker, Sharedworker, Style, Track, Video, Worker, Xslt };
    enum class Mode : uint8_t { Navigate, SameOrigin, NoCors, Cors };
    enum class Credentials : uint8_t { Omit, SameOrigin, Include };
    enum class Cache : uint8_t { Default, NoStore, Reload, NoCache, ForceCache, OnlyIfCached };
    enum class Redirect : uint8_t { Follow, Error, Manual };

    FetchOptions() = default;
    FetchOptions(Destination, Mode, Credentials, Cache, Redirect, ReferrerPolicy, String&&, bool);
    FetchOptions isolatedCopy() const { return { destination, mode, credentials, cache, redirect, referrerPolicy, integrity.isolatedCopy(), keepAlive }; }

    template<class Encoder> void encodePersistent(Encoder&) const;
    template<class Decoder> static bool decodePersistent(Decoder&, FetchOptions&);
    template<class Encoder> void encode(Encoder&) const;
    template<class Decoder> static Optional<FetchOptions> decode(Decoder&);

    Destination destination { Destination::EmptyString };
    Mode mode { Mode::NoCors };
    Credentials credentials { Credentials::Omit };
    Cache cache { Cache::Default };
    Redirect redirect { Redirect::Follow };
    ReferrerPolicy referrerPolicy { ReferrerPolicy::EmptyString };
    bool keepAlive { false };
    String integrity;
    Optional<DocumentIdentifier> clientIdentifier;
};

inline FetchOptions::FetchOptions(Destination destination, Mode mode, Credentials credentials, Cache cache, Redirect redirect, ReferrerPolicy referrerPolicy, String&& integrity, bool keepAlive)
    : destination(destination)
    , mode(mode)
    , credentials(credentials)
    , cache(cache)
    , redirect(redirect)
    , referrerPolicy(referrerPolicy)
    , keepAlive(keepAlive)
    , integrity(WTFMove(integrity))
{
}

inline bool isPotentialNavigationOrSubresourceRequest(FetchOptions::Destination destination)
{
    return destination == FetchOptions::Destination::Object
        || destination == FetchOptions::Destination::Embed;
}

inline bool isNonSubresourceRequest(FetchOptions::Destination destination)
{
    return destination == FetchOptions::Destination::Document
        || destination == FetchOptions::Destination::Report
        || destination == FetchOptions::Destination::Serviceworker
        || destination == FetchOptions::Destination::Sharedworker
        || destination == FetchOptions::Destination::Worker;
}

inline bool isScriptLikeDestination(FetchOptions::Destination destination)
{
    return destination == FetchOptions::Destination::Script
        || destination == FetchOptions::Destination::Serviceworker
        || destination == FetchOptions::Destination::Worker;
}

}

namespace WTF {

template<> struct EnumTraits<CyberCore::FetchOptions::Destination> {
    using values = EnumValues<
        CyberCore::FetchOptions::Destination,
        CyberCore::FetchOptions::Destination::EmptyString,
        CyberCore::FetchOptions::Destination::Audio,
        CyberCore::FetchOptions::Destination::Document,
        CyberCore::FetchOptions::Destination::Embed,
        CyberCore::FetchOptions::Destination::Font,
        CyberCore::FetchOptions::Destination::Image,
        CyberCore::FetchOptions::Destination::Manifest,
        CyberCore::FetchOptions::Destination::Object,
        CyberCore::FetchOptions::Destination::Report,
        CyberCore::FetchOptions::Destination::Script,
        CyberCore::FetchOptions::Destination::Serviceworker,
        CyberCore::FetchOptions::Destination::Sharedworker,
        CyberCore::FetchOptions::Destination::Style,
        CyberCore::FetchOptions::Destination::Track,
        CyberCore::FetchOptions::Destination::Video,
        CyberCore::FetchOptions::Destination::Worker,
        CyberCore::FetchOptions::Destination::Xslt
    >;
};

template<> struct EnumTraits<CyberCore::FetchOptions::Mode> {
    using values = EnumValues<
        CyberCore::FetchOptions::Mode,
        CyberCore::FetchOptions::Mode::Navigate,
        CyberCore::FetchOptions::Mode::SameOrigin,
        CyberCore::FetchOptions::Mode::NoCors,
        CyberCore::FetchOptions::Mode::Cors
    >;
};

template<> struct EnumTraits<CyberCore::FetchOptions::Credentials> {
    using values = EnumValues<
        CyberCore::FetchOptions::Credentials,
        CyberCore::FetchOptions::Credentials::Omit,
        CyberCore::FetchOptions::Credentials::SameOrigin,
        CyberCore::FetchOptions::Credentials::Include
    >;
};

template<> struct EnumTraits<CyberCore::FetchOptions::Cache> {
    using values = EnumValues<
        CyberCore::FetchOptions::Cache,
        CyberCore::FetchOptions::Cache::Default,
        CyberCore::FetchOptions::Cache::NoStore,
        CyberCore::FetchOptions::Cache::Reload,
        CyberCore::FetchOptions::Cache::NoCache,
        CyberCore::FetchOptions::Cache::ForceCache,
        CyberCore::FetchOptions::Cache::OnlyIfCached
    >;
};

template<> struct EnumTraits<CyberCore::FetchOptions::Redirect> {
    using values = EnumValues<
        CyberCore::FetchOptions::Redirect,
        CyberCore::FetchOptions::Redirect::Follow,
        CyberCore::FetchOptions::Redirect::Error,
        CyberCore::FetchOptions::Redirect::Manual
    >;
};

}

namespace CyberCore {

template<class Encoder> inline void FetchOptions::encodePersistent(Encoder& encoder) const
{
    // Changes to encoding here should bump NetworkCache Storage format version.
    encoder << destination;
    encoder << mode;
    encoder << credentials;
    encoder << cache;
    encoder << redirect;
    encoder << referrerPolicy;
    encoder << integrity;
    encoder << keepAlive;
}

template<class Decoder> inline bool FetchOptions::decodePersistent(Decoder& decoder, FetchOptions& options)
{
    FetchOptions::Destination destination;
    if (!decoder.decode(destination))
        return false;

    FetchOptions::Mode mode;
    if (!decoder.decode(mode))
        return false;

    FetchOptions::Credentials credentials;
    if (!decoder.decode(credentials))
        return false;

    FetchOptions::Cache cache;
    if (!decoder.decode(cache))
        return false;

    FetchOptions::Redirect redirect;
    if (!decoder.decode(redirect))
        return false;

    ReferrerPolicy referrerPolicy;
    if (!decoder.decode(referrerPolicy))
        return false;

    String integrity;
    if (!decoder.decode(integrity))
        return false;

    bool keepAlive;
    if (!decoder.decode(keepAlive))
        return false;

    options.destination = destination;
    options.mode = mode;
    options.credentials = credentials;
    options.cache = cache;
    options.redirect = redirect;
    options.referrerPolicy = referrerPolicy;
    options.integrity = WTFMove(integrity);
    options.keepAlive = keepAlive;

    return true;
}

template<class Encoder> inline void FetchOptions::encode(Encoder& encoder) const
{
    encodePersistent(encoder);
    encoder << clientIdentifier;
}

template<class Decoder> inline Optional<FetchOptions> FetchOptions::decode(Decoder& decoder)
{
    FetchOptions options;
    if (!decodePersistent(decoder, options))
        return WTF::nullopt;

    Optional<Optional<DocumentIdentifier>> clientIdentifier;
    decoder >> clientIdentifier;
    if (!clientIdentifier)
        return WTF::nullopt;
    options.clientIdentifier = WTFMove(clientIdentifier.value());

    return options;
}

} // namespace CyberCore
