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

#include "MessageReceiver.h"
#include "MessageSender.h"
#include "UserData.h"
#include "VisitedLinkStore.h"
#include "WebPageProxyIdentifier.h"
#include <CyberCore/CertificateInfo.h>
#include <CyberCore/DocumentLoader.h>
#include <CyberCore/FrameIdentifier.h>
#include <CyberCore/FrameLoaderTypes.h>
#include <CyberCore/LayerHostingContextIdentifier.h>
#include <CyberCore/PageIdentifier.h>
#include <wtf/WeakPtr.h>

namespace CyberCore {
class ResourceRequest;
class ResourceResponse;
}

namespace WebKit {

class WebFrameProxy;
class WebProcessProxy;
struct FrameInfoData;

class ProvisionalFrameProxy : public IPC::MessageReceiver, public IPC::MessageSender {
    WTF_MAKE_FAST_ALLOCATED;
public:
    ProvisionalFrameProxy(WebFrameProxy&, Ref<WebProcessProxy>&&, const CyberCore::ResourceRequest&);
    ~ProvisionalFrameProxy();

    WebProcessProxy& process() { return m_process.get(); }

    CyberCore::LayerHostingContextIdentifier layerHostingContextIdentifier() const { return m_layerHostingContextIdentifier; }

private:
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

    void decidePolicyForResponse(CyberCore::FrameIdentifier, FrameInfoData&&, CyberCore::PolicyCheckIdentifier, uint64_t navigationID, const CyberCore::ResourceResponse&, const CyberCore::ResourceRequest&, bool canShowMIMEType, const String& downloadAttribute, uint64_t listenerID);
    void didCommitLoadForFrame(CyberCore::FrameIdentifier, FrameInfoData&&, CyberCore::ResourceRequest&&, uint64_t navigationID, const String& mimeType, bool frameHasCustomContentProvider, CyberCore::FrameLoadType, const CyberCore::CertificateInfo&, bool usedLegacyTLS, bool privateRelayed, bool containsPluginDocument, CyberCore::HasInsecureContent, CyberCore::MouseEventPolicy, const UserData&);

    IPC::Connection* messageSenderConnection() const final;
    uint64_t messageSenderDestinationID() const final;

    WebFrameProxy& m_frame;
    Ref<WebProcessProxy> m_process;
    Ref<VisitedLinkStore> m_visitedLinkStore;
    CyberCore::PageIdentifier m_pageID;
    WebPageProxyIdentifier m_webPageID;
    bool m_wasCommitted { false };
    CyberCore::LayerHostingContextIdentifier m_layerHostingContextIdentifier;
};

} // namespace WebKit
