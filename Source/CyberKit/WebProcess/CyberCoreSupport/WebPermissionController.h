/*
 * Copyright (C) 2021 Apple Inc. All rights reserved.
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
#include "WebPageProxyIdentifier.h"
#include <CyberCore/ClientOrigin.h>
#include <CyberCore/PermissionController.h>
#include <CyberCore/PermissionDescriptor.h>
#include <wtf/Deque.h>
#include <wtf/WeakHashSet.h>

namespace CyberCore {
enum class PermissionQuerySource : uint8_t;
enum class PermissionState : uint8_t;
class Page;
class SecurityOriginData;
}

namespace CyberKit {

class WebProcess;

class WebPermissionController final : public CyberCore::PermissionController, public IPC::MessageReceiver {
public:
    static Ref<WebPermissionController> create(WebProcess&);
    ~WebPermissionController();

    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

private:
    explicit WebPermissionController(WebProcess&);

    // CyberCore::PermissionController
    void query(CyberCore::ClientOrigin&&, CyberCore::PermissionDescriptor, const WeakPtr<CyberCore::Page>&, CyberCore::PermissionQuerySource, CompletionHandler<void(std::optional<CyberCore::PermissionState>)>&&) final;
    void addObserver(CyberCore::PermissionObserver&) final;
    void removeObserver(CyberCore::PermissionObserver&) final;
    void permissionChanged(CyberCore::PermissionName, const CyberCore::SecurityOriginData&) final;

    WeakHashSet<CyberCore::PermissionObserver> m_observers;
};

} // namespace CyberCore
