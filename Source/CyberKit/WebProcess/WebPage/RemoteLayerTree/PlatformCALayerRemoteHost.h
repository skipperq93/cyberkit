/*
 * Copyright (C) 2023 Apple Inc. All rights reserved.
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

#include "PlatformCALayerRemote.h"

namespace CyberKit {

class PlatformCALayerRemoteHost final : public PlatformCALayerRemote {
    friend class PlatformCALayerRemote;
public:
    static Ref<PlatformCALayerRemote> create(CyberCore::LayerHostingContextIdentifier, CyberCore::PlatformCALayerClient*, RemoteLayerTreeContext&);

private:
    PlatformCALayerRemoteHost(CyberCore::LayerHostingContextIdentifier, CyberCore::PlatformCALayerClient*, RemoteLayerTreeContext&);

    Type type() const final { return Type::RemoteHost; }
    void populateCreationProperties(RemoteLayerTreeTransaction::LayerCreationProperties&, const RemoteLayerTreeContext&, CyberCore::PlatformCALayer::LayerType) final;

    CyberCore::LayerHostingContextIdentifier m_identifier;
};

} // namespace CyberKit

SPECIALIZE_TYPE_TRAITS_PLATFORM_CALAYER(CyberKit::PlatformCALayerRemoteHost, type() == CyberCore::PlatformCALayer::Type::RemoteHost)
