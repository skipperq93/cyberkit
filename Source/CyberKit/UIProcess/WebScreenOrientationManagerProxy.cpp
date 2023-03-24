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

#include "config.h"
#include "WebScreenOrientationManagerProxy.h"

#include "APIUIClient.h"
#include "WebFullScreenManagerProxy.h"
#include "WebPageProxy.h"
#include "WebScreenOrientationManagerMessages.h"
#include "WebScreenOrientationManagerProxyMessages.h"
#include <CyberCore/Exception.h>
#include <CyberCore/ScreenOrientationProvider.h>

namespace CyberKit {

WebScreenOrientationManagerProxy::WebScreenOrientationManagerProxy(WebPageProxy& page)
    : m_page(page)
    , m_provider(CyberCore::ScreenOrientationProvider::create())
{
    m_page.process().addMessageReceiver(Messages::WebScreenOrientationManagerProxy::messageReceiverName(), m_page.webPageID(), *this);
    platformInitialize();
}

WebScreenOrientationManagerProxy::~WebScreenOrientationManagerProxy()
{
    unlockIfNecessary();

    m_page.process().removeMessageReceiver(Messages::WebScreenOrientationManagerProxy::messageReceiverName(), m_page.webPageID());
    m_provider->removeObserver(*this);
    platformDestroy();
}

void WebScreenOrientationManagerProxy::currentOrientation(CompletionHandler<void(CyberCore::ScreenOrientationType)>&& completionHandler)
{
    completionHandler(m_provider->currentOrientation());
}

static CyberCore::ScreenOrientationType resolveScreenOrientationLockType(CyberCore::ScreenOrientationType currentOrientation, CyberCore::ScreenOrientationLockType lockType)
{
    switch (lockType) {
    case CyberCore::ScreenOrientationLockType::Any:
        return currentOrientation;
    case CyberCore::ScreenOrientationLockType::PortraitPrimary:
        return CyberCore::ScreenOrientationType::PortraitPrimary;
    case CyberCore::ScreenOrientationLockType::Landscape:
        if (CyberCore::isLandscape(currentOrientation))
            return currentOrientation;
        return CyberCore::ScreenOrientationType::LandscapePrimary;
    case CyberCore::ScreenOrientationLockType::PortraitSecondary:
        return CyberCore::ScreenOrientationType::PortraitSecondary;
    case CyberCore::ScreenOrientationLockType::LandscapePrimary:
        return CyberCore::ScreenOrientationType::LandscapePrimary;
    case CyberCore::ScreenOrientationLockType::LandscapeSecondary:
        return CyberCore::ScreenOrientationType::LandscapeSecondary;
    case CyberCore::ScreenOrientationLockType::Natural: {
        auto naturalOrientation = CyberCore::naturalScreenOrientationType();
        if (CyberCore::isPortrait(naturalOrientation) == CyberCore::isPortrait(currentOrientation))
            return currentOrientation;
        return naturalOrientation;
    }
    case CyberCore::ScreenOrientationLockType::Portrait:
        break;
    }
    if (CyberCore::isPortrait(currentOrientation))
        return currentOrientation;
    return CyberCore::ScreenOrientationType::PortraitPrimary;
}

void WebScreenOrientationManagerProxy::lock(CyberCore::ScreenOrientationLockType lockType, CompletionHandler<void(std::optional<CyberCore::Exception>&&)>&& completionHandler)
{
    if (m_currentLockRequest)
        m_currentLockRequest(CyberCore::Exception { CyberCore::AbortError, "A new lock request was started"_s });

    if (auto exception = platformShouldRejectLockRequest()) {
        completionHandler(*exception);
        return;
    }

    m_currentLockRequest = WTFMove(completionHandler);
    auto currentOrientation = m_provider->currentOrientation();
    auto resolvedLockedOrientation = resolveScreenOrientationLockType(currentOrientation, lockType);
    bool shouldOrientationChange = currentOrientation != resolvedLockedOrientation;

    if (resolvedLockedOrientation != m_currentlyLockedOrientation) {
        bool didLockOrientation = false;
#if ENABLE(FULLSCREEN_API)
        if (m_page.fullScreenManager() && m_page.fullScreenManager()->isFullScreen()) {
            if (!m_page.fullScreenManager()->lockFullscreenOrientation(resolvedLockedOrientation)) {
                m_currentLockRequest(CyberCore::Exception { CyberCore::NotSupportedError, "Screen orientation locking is not supported"_s });
                return;
            }
            didLockOrientation = true;
        }
#endif
        if (!didLockOrientation && !m_page.uiClient().lockScreenOrientation(m_page, resolvedLockedOrientation)) {
            m_currentLockRequest(CyberCore::Exception { CyberCore::NotSupportedError, "Screen orientation locking is not supported"_s });
            return;
        }
    }
    m_currentlyLockedOrientation = resolvedLockedOrientation;
    if (!shouldOrientationChange)
        m_currentLockRequest(std::nullopt);
}

void WebScreenOrientationManagerProxy::unlock()
{
    if (!m_currentlyLockedOrientation)
        return;

    if (m_currentLockRequest)
        m_currentLockRequest(CyberCore::Exception { CyberCore::AbortError, "Unlock request was received"_s });

    bool didUnlockOrientation = false;
#if ENABLE(FULLSCREEN_API)
    if (m_page.fullScreenManager() && m_page.fullScreenManager()->isFullScreen()) {
        m_page.fullScreenManager()->unlockFullscreenOrientation();
        didUnlockOrientation = true;
    }
#endif
    if (!didUnlockOrientation)
        m_page.uiClient().unlockScreenOrientation(m_page);

    m_currentlyLockedOrientation = std::nullopt;
}

void WebScreenOrientationManagerProxy::screenOrientationDidChange(CyberCore::ScreenOrientationType orientation)
{
    m_page.send(Messages::WebScreenOrientationManager::OrientationDidChange(orientation));
    if (m_currentLockRequest)
        m_currentLockRequest(std::nullopt);
}

void WebScreenOrientationManagerProxy::setShouldSendChangeNotification(bool shouldSend)
{
    if (shouldSend)
        m_provider->addObserver(*this);
    else
        m_provider->removeObserver(*this);
}

void WebScreenOrientationManagerProxy::unlockIfNecessary()
{
    if (m_currentlyLockedOrientation)
        unlock();
    if (m_currentLockRequest)
        m_currentLockRequest(CyberCore::Exception { CyberCore::AbortError, "Screen lock request was aborted"_s });
}

#if !PLATFORM(IOS_FAMILY)
void WebScreenOrientationManagerProxy::platformInitialize()
{
}

void WebScreenOrientationManagerProxy::platformDestroy()
{
}

std::optional<CyberCore::Exception> WebScreenOrientationManagerProxy::platformShouldRejectLockRequest() const
{
    return std::nullopt;
}
#endif

} // namespace CyberKit
