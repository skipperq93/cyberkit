/*
 * Copyright (C) 2017 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2,1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#pragma once

#if ENABLE(REMOTE_INSPECTOR)

#include "RemoteInspectorClient.h"
#include "CyberKitURISchemeRequest.h"
#include "CyberKitUserContentManager.h"
#include "CyberKitWebView.h"
#include <wtf/HashMap.h>
#include <wtf/HashSet.h>
#include <wtf/text/StringHash.h>

namespace CyberKit {

class RemoteInspectorProtocolHandler final : public RemoteInspectorObserver {
    WTF_MAKE_FAST_ALLOCATED;
public:
    explicit RemoteInspectorProtocolHandler(CyberKitWebContext* context);
    ~RemoteInspectorProtocolHandler();

    void inspect(const String& hostAndPort, uint64_t connectionID, uint64_t targetID);

private:
    static void webViewDestroyed(RemoteInspectorProtocolHandler*, CyberKitWebView*);
    static void userContentManagerDestroyed(RemoteInspectorProtocolHandler*, CyberKitUserContentManager*);

    void handleRequest(CyberKitURISchemeRequest*);

    // RemoteInspectorObserver.
    void targetListChanged(RemoteInspectorClient&) override;
    void connectionClosed(RemoteInspectorClient&) override;

    CyberKitWebContext* m_context { nullptr };
    HashMap<String, std::unique_ptr<RemoteInspectorClient>> m_inspectorClients;
    HashSet<CyberKitUserContentManager*> m_userContentManagers;
    HashSet<CyberKitWebView*> m_webViews;
};

} // namespace CyberKit

#endif // ENABLE(REMOTE_INSPECTOR)
