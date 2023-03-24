/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
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

#if ENABLE(GPU_PROCESS) && ENABLE(VIDEO)

#include <CyberCore/MediaPlayerEnums.h>
#include <wtf/HashMap.h>
#include <wtf/HashSet.h>
#include <wtf/text/StringHash.h>

namespace CyberCore {
struct MediaEngineSupportParameters;
}

namespace CyberKit {

class RemoteMediaPlayerManager;

class RemoteMediaPlayerMIMETypeCache {
    WTF_MAKE_FAST_ALLOCATED;
public:
    RemoteMediaPlayerMIMETypeCache(RemoteMediaPlayerManager&, CyberCore::MediaPlayerEnums::MediaEngineIdentifier);
    ~RemoteMediaPlayerMIMETypeCache() = default;

    HashSet<String, ASCIICaseInsensitiveHash>& supportedTypes();
    CyberCore::MediaPlayerEnums::SupportsType supportsTypeAndCodecs(const CyberCore::MediaEngineSupportParameters&);
    void addSupportedTypes(const Vector<String>&);
    bool isEmpty() const;

private:
    RemoteMediaPlayerManager& m_manager;
    CyberCore::MediaPlayerEnums::MediaEngineIdentifier m_engineIdentifier;

    using SupportedTypesAndCodecsKey = std::tuple<String, bool, bool, bool>;
    std::optional<HashMap<SupportedTypesAndCodecsKey, CyberCore::MediaPlayerEnums::SupportsType>> m_supportsTypeAndCodecsCache;
    HashSet<String, ASCIICaseInsensitiveHash> m_supportedTypesCache;
    bool m_hasPopulatedSupportedTypesCacheFromGPUProcess { false };
};

} // namespace CyberKit

#endif // ENABLE(GPU_PROCESS) && ENABLE(VIDEO)
