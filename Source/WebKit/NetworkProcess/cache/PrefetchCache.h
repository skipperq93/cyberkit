/*
 * Copyright (C) 2019 Igalia S.L.
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

#include "PrivateRelayed.h"
#include <CyberCore/ResourceRequest.h>
#include <CyberCore/ResourceResponse.h>
#include <CyberCore/SharedBuffer.h>
#include <CyberCore/Timer.h>
#include <wtf/Deque.h>
#include <wtf/HashMap.h>
#include <wtf/URLHash.h>
#include <wtf/text/WTFString.h>

namespace WebKit {

class PrefetchCache {
    WTF_MAKE_NONCOPYABLE(PrefetchCache);
    WTF_MAKE_FAST_ALLOCATED;
public:
    PrefetchCache();
    ~PrefetchCache();

    void clear();

    struct Entry {
        WTF_MAKE_STRUCT_FAST_ALLOCATED;
        Entry(CyberCore::ResourceResponse&&, PrivateRelayed, RefPtr<CyberCore::FragmentedSharedBuffer>&&);
        Entry(CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&);

        Ref<CyberCore::FragmentedSharedBuffer> releaseBuffer() { return buffer.releaseNonNull(); }

        CyberCore::ResourceResponse response;
        PrivateRelayed privateRelayed { PrivateRelayed::No };
        // FIXME: This should probably be a variant<RefPtr, ResourceRequest> because we have one or the other but never both.
        RefPtr<CyberCore::FragmentedSharedBuffer> buffer;
        CyberCore::ResourceRequest redirectRequest;
    };

    std::unique_ptr<Entry> take(const URL&);
    void store(const URL&, CyberCore::ResourceResponse&&, PrivateRelayed, RefPtr<CyberCore::FragmentedSharedBuffer>&&);
    void storeRedirect(const URL&, CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&);

private:
    void clearExpiredEntries();

    using PrefetchEntriesMap = HashMap<URL, std::unique_ptr<Entry>>;
    std::unique_ptr<PrefetchEntriesMap> m_sessionPrefetches;

    using SessionPrefetchExpirationList = Deque<std::tuple<URL, WallTime>>;
    SessionPrefetchExpirationList m_sessionExpirationList;

    CyberCore::Timer m_expirationTimer;
};

} // namespace WebKit
