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

#include "FetchOptionsCache.h"
#include "FetchOptionsCredentials.h"
#include "FetchOptionsDestination.h"
#include "FetchOptionsMode.h"
#include "FetchOptionsRedirect.h"
#include "ProcessQualified.h"
#include "ReferrerPolicy.h"
#include "ScriptExecutionContextIdentifier.h"
#include <wtf/Markable.h>
#include <wtf/text/WTFString.h>

namespace CyberCore {

struct FetchOptions {
    using Destination = FetchOptionsDestination;
    using Mode = FetchOptionsMode;
    using Credentials = FetchOptionsCredentials;
    using Cache = FetchOptionsCache;
    using Redirect = FetchOptionsRedirect;

    FetchOptions() = default;
    FetchOptions(Destination, Mode, Credentials, Cache, Redirect, ReferrerPolicy, bool, String&&, Markable<UUID>, Markable<UUID>);
    FetchOptions isolatedCopy() const & { return { destination, mode, credentials, cache, redirect, referrerPolicy, keepAlive, integrity.isolatedCopy(), clientIdentifier, resultingClientIdentifier }; }
    FetchOptions isolatedCopy() && { return { destination, mode, credentials, cache, redirect, referrerPolicy, keepAlive, WTFMove(integrity).isolatedCopy(), clientIdentifier, resultingClientIdentifier }; }

    template<class Encoder> void encodePersistent(Encoder&) const;
    template<class Decoder> static WARN_UNUSED_RETURN bool decodePersistent(Decoder&, FetchOptions&);

    Destination destination { Destination::EmptyString };
    Mode mode { Mode::NoCors };
    Credentials credentials { Credentials::Omit };
    Cache cache { Cache::Default };
    Redirect redirect { Redirect::Follow };
    ReferrerPolicy referrerPolicy { ReferrerPolicy::EmptyString };
    bool keepAlive { false };
    String integrity;
    Markable<UUID> clientIdentifier; // Identifier of https://fetch.spec.whatwg.org/#concept-request-client
    Markable<UUID> resultingClientIdentifier; // Identifier of https://fetch.spec.whatwg.org/#concept-request-reserved-client
};

inline FetchOptions::FetchOptions(Destination destination, Mode mode, Credentials credentials, Cache cache, Redirect redirect, ReferrerPolicy referrerPolicy, bool keepAlive, String&& integrity, Markable<UUID> clientIdentifier, Markable<UUID> resultingClientIdentifier)
    : destination(destination)
    , mode(mode)
    , credentials(credentials)
    , cache(cache)
    , redirect(redirect)
    , referrerPolicy(referrerPolicy)
    , keepAlive(keepAlive)
    , integrity(WTFMove(integrity))
    , clientIdentifier(clientIdentifier)
    , resultingClientIdentifier(resultingClientIdentifier)
{
}

inline bool isPotentialNavigationOrSubresourceRequest(FetchOptions::Destination destination)
{
    return destination == FetchOptions::Destination::Object
        || destination == FetchOptions::Destination::Embed;
}

// https://fetch.spec.whatwg.org/#navigation-request
inline bool isNavigationRequest(FetchOptions::Destination destination)
{
    return destination == FetchOptions::Destination::Document
        || destination == FetchOptions::Destination::Iframe
        || destination == FetchOptions::Destination::Object
        || destination == FetchOptions::Destination::Embed;
}

// https://fetch.spec.whatwg.org/#non-subresource-request
inline bool isNonSubresourceRequest(FetchOptions::Destination destination)
{
    return destination == FetchOptions::Destination::Document
        || destination == FetchOptions::Destination::Iframe
        || destination == FetchOptions::Destination::Report
        || destination == FetchOptions::Destination::Serviceworker
        || destination == FetchOptions::Destination::Sharedworker
        || destination == FetchOptions::Destination::Worker;
}

inline bool isScriptLikeDestination(FetchOptions::Destination destination)
{
    return destination == FetchOptions::Destination::Audioworklet
        || destination == FetchOptions::Destination::Paintworklet
        || destination == FetchOptions::Destination::Script
        || destination == FetchOptions::Destination::Serviceworker
        || destination == FetchOptions::Destination::Sharedworker
        || destination == FetchOptions::Destination::Worker;
}

}

namespace WTF {

template<> struct EnumTraitsForPersistence<CyberCore::FetchOptions::Destination> {
    using values = EnumValues<
        CyberCore::FetchOptions::Destination,
        CyberCore::FetchOptions::Destination::EmptyString,
        CyberCore::FetchOptions::Destination::Audio,
        CyberCore::FetchOptions::Destination::Audioworklet,
        CyberCore::FetchOptions::Destination::Document,
        CyberCore::FetchOptions::Destination::Embed,
        CyberCore::FetchOptions::Destination::Font,
        CyberCore::FetchOptions::Destination::Image,
        CyberCore::FetchOptions::Destination::Iframe,
        CyberCore::FetchOptions::Destination::Manifest,
        CyberCore::FetchOptions::Destination::Model,
        CyberCore::FetchOptions::Destination::Object,
        CyberCore::FetchOptions::Destination::Paintworklet,
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

template<> struct EnumTraitsForPersistence<CyberCore::FetchOptions::Mode> {
    using values = EnumValues<
        CyberCore::FetchOptions::Mode,
        CyberCore::FetchOptions::Mode::Navigate,
        CyberCore::FetchOptions::Mode::SameOrigin,
        CyberCore::FetchOptions::Mode::NoCors,
        CyberCore::FetchOptions::Mode::Cors
    >;
};


template<> struct EnumTraitsForPersistence<CyberCore::FetchOptions::Credentials> {
    using values = EnumValues<
        CyberCore::FetchOptions::Credentials,
        CyberCore::FetchOptions::Credentials::Omit,
        CyberCore::FetchOptions::Credentials::SameOrigin,
        CyberCore::FetchOptions::Credentials::Include
    >;
};

template<> struct EnumTraitsForPersistence<CyberCore::FetchOptions::Cache> {
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

template<> struct EnumTraitsForPersistence<CyberCore::FetchOptions::Redirect> {
    using values = EnumValues<
        CyberCore::FetchOptions::Redirect,
        CyberCore::FetchOptions::Redirect::Follow,
        CyberCore::FetchOptions::Redirect::Error,
        CyberCore::FetchOptions::Redirect::Manual
    >;
};

}

namespace CyberCore {

template<class Encoder>
inline void FetchOptions::encodePersistent(Encoder& encoder) const
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

template<class Decoder>
inline bool FetchOptions::decodePersistent(Decoder& decoder, FetchOptions& options)
{
    std::optional<FetchOptions::Destination> destination;
    decoder >> destination;
    if (!destination)
        return false;

    std::optional<FetchOptions::Mode> mode;
    decoder >> mode;
    if (!mode)
        return false;

    std::optional<FetchOptions::Credentials> credentials;
    decoder >> credentials;
    if (!credentials)
        return false;

    std::optional<FetchOptions::Cache> cache;
    decoder >> cache;
    if (!cache)
        return false;

    std::optional<FetchOptions::Redirect> redirect;
    decoder >> redirect;
    if (!redirect)
        return false;

    std::optional<ReferrerPolicy> referrerPolicy;
    decoder >> referrerPolicy;
    if (!referrerPolicy)
        return false;

    std::optional<String> integrity;
    decoder >> integrity;
    if (!integrity)
        return false;

    std::optional<bool> keepAlive;
    decoder >> keepAlive;
    if (!keepAlive)
        return false;

    options.destination = *destination;
    options.mode = *mode;
    options.credentials = *credentials;
    options.cache = *cache;
    options.redirect = *redirect;
    options.referrerPolicy = *referrerPolicy;
    options.integrity = WTFMove(*integrity);
    options.keepAlive = *keepAlive;

    return true;
}

} // namespace CyberCore
