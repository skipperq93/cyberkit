/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
 * Copyright (C) 2012-2020 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <CyberCore/COMPtr.h>
#include <CyberCore/Notification.h>
#include <CyberCore/NotificationClient.h>

#if ENABLE(NOTIFICATIONS)

interface IWebDesktopNotificationPresenter;

namespace CyberCore {
class Document;
}

class WebDesktopNotificationsDelegate : public CyberCore::NotificationClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    WebDesktopNotificationsDelegate(WebView* view);

    /* CyberCore::NotificationClient interface */
    virtual bool show(CyberCore::Notification* object);
    virtual void cancel(CyberCore::Notification* object);
    virtual void notificationObjectDestroyed(CyberCore::Notification* object);
    virtual void notificationControllerDestroyed();
    virtual void requestPermission(CyberCore::SecurityOrigin*, RefPtr<CyberCore::NotificationPermissionCallback>&&);
    bool hasPendingPermissionRequests(CyberCore::ScriptExecutionContext*) const override;
    virtual void cancelRequestsForPermission(CyberCore::ScriptExecutionContext*);
    virtual CyberCore::NotificationClient::Permission checkPermission(const URL&);

private:
    bool hasNotificationDelegate();
    COMPtr<IWebDesktopNotificationsDelegate> notificationDelegate();

    WebView* m_webView;
};

#endif
