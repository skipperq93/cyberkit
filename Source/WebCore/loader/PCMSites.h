/*
 * Copyright (C) 2022 Apple Inc. All rights reserved.
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

#include "RegistrableDomain.h"
#include <wtf/text/StringHash.h>

namespace CyberCore::PCM {

struct SourceSite {
    explicit SourceSite(const URL& url)
        : registrableDomain { url }
    {
    }

    explicit SourceSite(RegistrableDomain&& domain)
        : registrableDomain { WTFMove(domain) }
    {
    }

    SourceSite isolatedCopy() const & { return SourceSite { registrableDomain.isolatedCopy() }; }
    SourceSite isolatedCopy() && { return SourceSite { WTFMove(registrableDomain).isolatedCopy() }; }

    bool operator==(const SourceSite& other) const
    {
        return registrableDomain == other.registrableDomain;
    }

    bool operator!=(const SourceSite& other) const
    {
        return registrableDomain != other.registrableDomain;
    }

    bool matches(const URL& url) const
    {
        return registrableDomain.matches(url);
    }

    RegistrableDomain registrableDomain;
};

struct SourceSiteHash {
    static unsigned hash(const SourceSite& sourceSite)
    {
        return sourceSite.registrableDomain.hash();
    }
    
    static bool equal(const SourceSite& a, const SourceSite& b)
    {
        return a == b;
    }

    static const bool safeToCompareToEmptyOrDeleted = false;
};

struct AttributionDestinationSite {
    AttributionDestinationSite() = default;
    explicit AttributionDestinationSite(const URL& url)
        : registrableDomain { RegistrableDomain { url } }
    {
    }

    explicit AttributionDestinationSite(RegistrableDomain&& domain)
        : registrableDomain { WTFMove(domain) }
    {
    }

    AttributionDestinationSite isolatedCopy() const & { return AttributionDestinationSite { registrableDomain.isolatedCopy() }; }
    AttributionDestinationSite isolatedCopy() && { return AttributionDestinationSite { WTFMove(registrableDomain).isolatedCopy() }; }

    bool operator==(const AttributionDestinationSite& other) const
    {
        return registrableDomain == other.registrableDomain;
    }

    bool operator!=(const AttributionDestinationSite& other) const
    {
        return registrableDomain != other.registrableDomain;
    }

    bool matches(const URL& url) const
    {
        return registrableDomain == RegistrableDomain { url };
    }

    RegistrableDomain registrableDomain;
};

struct AttributionDestinationSiteHash {
    static unsigned hash(const AttributionDestinationSite& destinationSite)
    {
        return destinationSite.registrableDomain.hash();
    }
    
    static bool equal(const AttributionDestinationSite& a, const AttributionDestinationSite& b)
    {
        return a == b;
    }

    static const bool safeToCompareToEmptyOrDeleted = false;
};

} // namespace CyberCore::PCM

namespace WTF {

template<typename T> struct DefaultHash;

template<> struct DefaultHash<CyberCore::PCM::SourceSite> : CyberCore::PCM::SourceSiteHash { };
template<> struct HashTraits<CyberCore::PCM::SourceSite> : GenericHashTraits<CyberCore::PCM::SourceSite> {
    static CyberCore::PCM::SourceSite emptyValue() { return CyberCore::PCM::SourceSite(CyberCore::RegistrableDomain()); }
    static void constructDeletedValue(CyberCore::PCM::SourceSite& slot) { new (NotNull, &slot.registrableDomain) CyberCore::RegistrableDomain(WTF::HashTableDeletedValue); }
    static bool isDeletedValue(const CyberCore::PCM::SourceSite& slot) { return slot.registrableDomain.isHashTableDeletedValue(); }
};

template<> struct DefaultHash<CyberCore::PCM::AttributionDestinationSite> : CyberCore::PCM::AttributionDestinationSiteHash { };
template<> struct HashTraits<CyberCore::PCM::AttributionDestinationSite> : GenericHashTraits<CyberCore::PCM::AttributionDestinationSite> {
    static CyberCore::PCM::AttributionDestinationSite emptyValue() { return { }; }
    static void constructDeletedValue(CyberCore::PCM::AttributionDestinationSite& slot) { new (NotNull, &slot.registrableDomain) CyberCore::RegistrableDomain(WTF::HashTableDeletedValue); }
    static bool isDeletedValue(const CyberCore::PCM::AttributionDestinationSite& slot) { return slot.registrableDomain.isHashTableDeletedValue(); }
};

} // namespace WTF
