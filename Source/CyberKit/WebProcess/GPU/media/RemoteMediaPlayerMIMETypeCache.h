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

#if ENABLE(GPU_PROCESS)

#include "RemoteMediaPlayerManager.h"
#include <CyberCore/MIMETypeCache.h>
#include <CyberCore/MediaPlayerPrivate.h>

namespace CyberCore {
struct MediaEngineSupportParameters;
}

namespace CyberKit {

class RemoteMediaPlayerMIMETypeCache final : public CyberCore::MIMETypeCache {
public:
    RemoteMediaPlayerMIMETypeCache(RemoteMediaPlayerManager&, CyberCore::MediaPlayerEnums::MediaEngineIdentifier);

    CyberCore::MediaPlayerEnums::SupportsType supportsTypeAndCodecs(const CyberCore::MediaEngineSupportParameters&);

private:
    const HashSet<String, ASCIICaseInsensitiveHash>& staticContainerTypeList() final;
    bool isUnsupportedContainerType(const String&) final;
    void initializeCache(HashSet<String, ASCIICaseInsensitiveHash>&) final;
    bool canDecodeExtendedType(const CyberCore::ContentType&) final;

    MIMETypeCache* mimeCache() const;

    RemoteMediaPlayerManager& m_manager;
    CyberCore::MediaPlayerEnums::MediaEngineIdentifier m_engineIdentifier;
    Optional<HashMap<String, CyberCore::MediaPlayerEnums::SupportsType, ASCIICaseInsensitiveHash>> m_supportsTypeAndCodecsCache;
};

} // namespace CyberKit

#endif
