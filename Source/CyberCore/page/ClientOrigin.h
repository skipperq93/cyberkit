/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
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
#include "SecurityOriginData.h"
#include <wtf/HashTraits.h>
#include <wtf/Hasher.h>
#include <wtf/URL.h>

namespace CyberCore {

struct ClientOrigin {
    static ClientOrigin emptyKey() { return { }; }

    bool operator==(const ClientOrigin&) const;
    bool operator!=(const ClientOrigin& other) const { return !(*this == other); }

    ClientOrigin isolatedCopy() const & { return { topOrigin.isolatedCopy(), clientOrigin.isolatedCopy() }; }
    ClientOrigin isolatedCopy() && { return { WTFMove(topOrigin).isolatedCopy(), WTFMove(clientOrigin).isolatedCopy() }; }
    bool isRelated(const SecurityOriginData& other) const { return topOrigin == other || clientOrigin == other; }

    RegistrableDomain clientRegistrableDomain() const { return RegistrableDomain::uncheckedCreateFromHost(clientOrigin.host()); }

    SecurityOriginData topOrigin;
    SecurityOriginData clientOrigin;

    String loggingString() const { return makeString(topOrigin.toString(), "-", clientOrigin.toString()); }
};

inline void add(Hasher& hasher, const ClientOrigin& origin)
{
    add(hasher, origin.topOrigin, origin.clientOrigin);
}

inline bool ClientOrigin::operator==(const ClientOrigin& other) const
{
    return topOrigin == other.topOrigin && clientOrigin == other.clientOrigin;
}

} // namespace CyberCore

namespace WTF {

struct ClientOriginKeyHash {
    static unsigned hash(const CyberCore::ClientOrigin& key) { return computeHash(key); }
    static bool equal(const CyberCore::ClientOrigin& a, const CyberCore::ClientOrigin& b) { return a == b; }
    static const bool safeToCompareToEmptyOrDeleted = false;
};

template<> struct HashTraits<CyberCore::ClientOrigin> : GenericHashTraits<CyberCore::ClientOrigin> {
    static CyberCore::ClientOrigin emptyValue() { return CyberCore::ClientOrigin::emptyKey(); }

    static void constructDeletedValue(CyberCore::ClientOrigin& slot) { new (NotNull, &slot.topOrigin) CyberCore::SecurityOriginData(WTF::HashTableDeletedValue); }
    static bool isDeletedValue(const CyberCore::ClientOrigin& slot) { return slot.topOrigin.isHashTableDeletedValue(); }
};

template<> struct DefaultHash<CyberCore::ClientOrigin> : ClientOriginKeyHash { };

} // namespace WTF
