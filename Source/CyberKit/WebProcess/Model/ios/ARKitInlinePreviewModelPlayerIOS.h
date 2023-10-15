/*
 * Copyright (C) 2021-2023 Apple Inc. All rights reserved.
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

#if ENABLE(ARKIT_INLINE_PREVIEW_IOS)

#include "ARKitInlinePreviewModelPlayer.h"
#include <CyberCore/ModelPlayer.h>
#include <CyberCore/ModelPlayerClient.h>

namespace CyberKit {

class ARKitInlinePreviewModelPlayerIOS final : public ARKitInlinePreviewModelPlayer {
public:
    static Ref<ARKitInlinePreviewModelPlayerIOS> create(WebPage&, CyberCore::ModelPlayerClient&);
    virtual ~ARKitInlinePreviewModelPlayerIOS();

    static void pageLoadedModelInlinePreview(WebPage&, CyberCore::PlatformLayerIdentifier);
    static void pageFailedToLoadModelInlinePreview(WebPage&, CyberCore::PlatformLayerIdentifier, const CyberCore::ResourceError&);

private:
    ARKitInlinePreviewModelPlayerIOS(WebPage&, CyberCore::ModelPlayerClient&);

    std::optional<ModelIdentifier> modelIdentifier() override;

    static ARKitInlinePreviewModelPlayerIOS* modelPlayerForPageAndLayerID(WebPage&, CyberCore::PlatformLayerIdentifier);

    // CyberCore::ModelPlayer overrides.
    void enterFullscreen() override;
    void setInteractionEnabled(bool) override;
    void handleMouseDown(const CyberCore::LayoutPoint&, MonotonicTime) override;
    void handleMouseMove(const CyberCore::LayoutPoint&, MonotonicTime) override;
    void handleMouseUp(const CyberCore::LayoutPoint&, MonotonicTime) override;
};

}

#endif
