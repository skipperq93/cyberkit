/*
 * Copyright (C) 2022 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
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

#include "WebKitWebResource.h"
#include "WebKitWebView.h"
#include <CyberCore/GlobalFrameIdentifier.h>
#include <CyberCore/ResourceLoaderIdentifier.h>
#include <wtf/HashMap.h>
#include <wtf/glib/GRefPtr.h>

namespace CyberCore {
class ResourceError;
class ResourceRequest;
class ResourceResponse;
}

namespace WebKit {

class WebKitWebResourceLoadManager {
    WTF_MAKE_FAST_ALLOCATED;
public:
    explicit WebKitWebResourceLoadManager(WebKitWebView*);
    ~WebKitWebResourceLoadManager();

    void didInitiateLoad(CyberCore::ResourceLoaderIdentifier, CyberCore::FrameIdentifier, CyberCore::ResourceRequest&&);
    void didSendRequest(CyberCore::ResourceLoaderIdentifier, CyberCore::FrameIdentifier, CyberCore::ResourceRequest&&, CyberCore::ResourceResponse&&);
    void didReceiveResponse(CyberCore::ResourceLoaderIdentifier, CyberCore::FrameIdentifier, CyberCore::ResourceResponse&&);
    void didFinishLoad(CyberCore::ResourceLoaderIdentifier, CyberCore::FrameIdentifier, CyberCore::ResourceError&&);

private:
    WebKitWebView* m_webView { nullptr };
    HashMap<std::pair<CyberCore::ResourceLoaderIdentifier, CyberCore::FrameIdentifier>, GRefPtr<WebKitWebResource>> m_resources;
};

} // namespace WebKit
