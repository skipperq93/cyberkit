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

#include "config.h"
#include "WebPermissionController.h"

#include "WebPage.h"
#include "WebPageProxyMessages.h"
#include "WebPermissionControllerMessages.h"
#include "WebPermissionControllerProxyMessages.h"
#include "WebProcess.h"
#include <CyberCore/Document.h>
#include <CyberCore/Page.h>
#include <CyberCore/PermissionObserver.h>
#include <CyberCore/PermissionQuerySource.h>
#include <CyberCore/PermissionState.h>
#include <CyberCore/Permissions.h>
#include <CyberCore/SecurityOriginData.h>
#include <optional>

namespace CyberKit {

Ref<WebPermissionController> WebPermissionController::create(WebProcess& process)
{
    return adoptRef(*new WebPermissionController(process));
}

WebPermissionController::WebPermissionController(WebProcess& process)
{
    process.addMessageReceiver(Messages::WebPermissionController::messageReceiverName(), *this);
}

WebPermissionController::~WebPermissionController()
{
    WebProcess::singleton().removeMessageReceiver(Messages::WebPermissionController::messageReceiverName());
}

void WebPermissionController::query(CyberCore::ClientOrigin&& origin, CyberCore::PermissionDescriptor descriptor, const WeakPtr<CyberCore::Page>& page, CyberCore::PermissionQuerySource source, CompletionHandler<void(std::optional<CyberCore::PermissionState>)>&& completionHandler)
{
    std::optional<WebPageProxyIdentifier> proxyIdentifier;
    if (source == CyberCore::PermissionQuerySource::Window || source == CyberCore::PermissionQuerySource::DedicatedWorker) {
        ASSERT(page);
        proxyIdentifier = WebPage::fromCorePage(*page).webPageProxyIdentifier();
    }

    WebProcess::singleton().sendWithAsyncReply(Messages::WebPermissionControllerProxy::Query(origin, descriptor, proxyIdentifier, source), WTFMove(completionHandler));
}

void WebPermissionController::addObserver(CyberCore::PermissionObserver& observer)
{
    m_observers.add(observer);
}

void WebPermissionController::removeObserver(CyberCore::PermissionObserver& observer)
{
    m_observers.remove(observer);
}

void WebPermissionController::permissionChanged(CyberCore::PermissionName permissionName, const CyberCore::SecurityOriginData& topOrigin)
{
    ASSERT(isMainRunLoop());

    for (auto& observer : m_observers) {
        if (observer.descriptor().name != permissionName || observer.origin().topOrigin != topOrigin)
            return;

        auto source = observer.source();
        if (!observer.page() && (source == CyberCore::PermissionQuerySource::Window || source == CyberCore::PermissionQuerySource::DedicatedWorker))
            return;

        query(CyberCore::ClientOrigin { observer.origin() }, CyberCore::PermissionDescriptor { permissionName }, observer.page(), source, [observer = WeakPtr { observer }](auto newState) {
            if (observer && newState != observer->currentState())
                observer->stateChanged(*newState);
        });
    }
}

} // namespace CyberKit
