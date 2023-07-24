/*
 * Copyright (C) 2011, 2012 Apple Inc. All rights reserved.
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

#if ENABLE(NOTIFICATIONS)

#import <CyberCore/Notification.h>
#import <CyberCore/NotificationClient.h>
#import <wtf/HashMap.h>
#import <wtf/RefPtr.h>
#import <wtf/RetainPtr.h>

@class WebNotification;
@class WebNotificationPolicyListener;
@class WebView;

class WebNotificationClient : public CyberCore::NotificationClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    WebNotificationClient(WebView *);
    WebView *webView() { return m_webView; }

    // For testing purposes.
    uint64_t notificationIDForTesting(CyberCore::Notification*);

private:
    bool show(CyberCore::Notification*) override;
    void cancel(CyberCore::Notification*) override;
    void clearNotifications(CyberCore::ScriptExecutionContext*) override;
    void notificationObjectDestroyed(CyberCore::Notification*) override;
    void notificationControllerDestroyed() override;
    void requestPermission(CyberCore::ScriptExecutionContext*, RefPtr<CyberCore::NotificationPermissionCallback>&&) override;
    void cancelRequestsForPermission(CyberCore::ScriptExecutionContext*) override { }
    bool hasPendingPermissionRequests(CyberCore::ScriptExecutionContext*) const override;
    CyberCore::NotificationClient::Permission checkPermission(CyberCore::ScriptExecutionContext*) override;

    void requestPermission(CyberCore::ScriptExecutionContext*, WebNotificationPolicyListener *);

    WebView *m_webView;
    HashMap<RefPtr<CyberCore::Notification>, RetainPtr<WebNotification>> m_notificationMap;
    
    typedef HashMap<RefPtr<CyberCore::ScriptExecutionContext>, Vector<RetainPtr<WebNotification>>> NotificationContextMap;
    NotificationContextMap m_notificationContextMap;

    bool m_everRequestedPermission { false };
};

#endif
