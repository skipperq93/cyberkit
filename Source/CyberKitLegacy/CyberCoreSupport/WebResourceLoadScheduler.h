/*
    Copyright (C) 1998 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2001 Dirk Mueller <mueller@kde.org>
    Copyright (C) 2004, 2006-2008, 2015 Apple Inc. All rights reserved.
    Copyright (C) 2010 Google Inc. All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
 */

#pragma once

#include <CyberCore/FrameLoaderTypes.h>
#include <CyberCore/LoaderStrategy.h>
#include <CyberCore/ResourceLoadPriority.h>
#include <CyberCore/ResourceLoaderOptions.h>
#include <CyberCore/Timer.h>
#include <array>
#include <wtf/Deque.h>
#include <wtf/HashMap.h>
#include <wtf/HashSet.h>
#include <wtf/Noncopyable.h>
#include <wtf/text/StringHash.h>
#include <wtf/text/WTFString.h>

class WebResourceLoadScheduler;

WebResourceLoadScheduler& webResourceLoadScheduler();

class WebResourceLoadScheduler final : public CyberCore::LoaderStrategy {
    WTF_MAKE_NONCOPYABLE(WebResourceLoadScheduler); WTF_MAKE_FAST_ALLOCATED;
public:
    WebResourceLoadScheduler();

    void loadResource(CyberCore::Frame&, CyberCore::CachedResource&, CyberCore::ResourceRequest&&, const CyberCore::ResourceLoaderOptions&, CompletionHandler<void(RefPtr<CyberCore::SubresourceLoader>&&)>&&) final;
    void loadResourceSynchronously(CyberCore::FrameLoader&, unsigned long, const CyberCore::ResourceRequest&, CyberCore::ClientCredentialPolicy, const CyberCore::FetchOptions&, const CyberCore::HTTPHeaderMap&, CyberCore::ResourceError&, CyberCore::ResourceResponse&, Vector<char>&) final;
    void pageLoadCompleted(CyberCore::Page&) final;
    void browsingContextRemoved(CyberCore::Frame&) final;

    void remove(CyberCore::ResourceLoader*) final;
    void setDefersLoading(CyberCore::ResourceLoader&, bool) final;
    void crossOriginRedirectReceived(CyberCore::ResourceLoader*, const URL& redirectURL) final;
    
    void servePendingRequests(CyberCore::ResourceLoadPriority minimumPriority = CyberCore::ResourceLoadPriority::VeryLow) final;
    void suspendPendingRequests() final;
    void resumePendingRequests() final;

    void startPingLoad(CyberCore::Frame&, CyberCore::ResourceRequest&, const CyberCore::HTTPHeaderMap&, const CyberCore::FetchOptions&, CyberCore::ContentSecurityPolicyImposition, PingLoadCompletionHandler&&) final;

    void preconnectTo(CyberCore::FrameLoader&, const URL&, CyberCore::StoredCredentialsPolicy, PreconnectCompletionHandler&&) final;

    void setCaptureExtraNetworkLoadMetricsEnabled(bool) final { }

    bool isSerialLoadingEnabled() const { return m_isSerialLoadingEnabled; }
    void setSerialLoadingEnabled(bool b) { m_isSerialLoadingEnabled = b; }

    void schedulePluginStreamLoad(CyberCore::Frame&, CyberCore::NetscapePlugInStreamLoaderClient&, CyberCore::ResourceRequest&&, CompletionHandler<void(RefPtr<CyberCore::NetscapePlugInStreamLoader>&&)>&&);

    bool isOnLine() const final;
    void addOnlineStateChangeListener(WTF::Function<void(bool)>&&) final;

private:
    virtual ~WebResourceLoadScheduler();

    void scheduleLoad(CyberCore::ResourceLoader*);
    void scheduleServePendingRequests();
    void requestTimerFired();

    bool isSuspendingPendingRequests() const { return !!m_suspendPendingRequestsCount; }

    class HostInformation {
        WTF_MAKE_NONCOPYABLE(HostInformation); WTF_MAKE_FAST_ALLOCATED;
    public:
        HostInformation(const String&, unsigned);
        ~HostInformation();
        
        const String& name() const { return m_name; }
        void schedule(CyberCore::ResourceLoader*, CyberCore::ResourceLoadPriority = CyberCore::ResourceLoadPriority::VeryLow);
        void addLoadInProgress(CyberCore::ResourceLoader*);
        void remove(CyberCore::ResourceLoader*);
        bool hasRequests() const;
        bool limitRequests(CyberCore::ResourceLoadPriority) const;

        typedef Deque<RefPtr<CyberCore::ResourceLoader>> RequestQueue;
        RequestQueue& requestsPending(CyberCore::ResourceLoadPriority priority) { return m_requestsPending[priorityToIndex(priority)]; }

    private:
        static unsigned priorityToIndex(CyberCore::ResourceLoadPriority);

        std::array<RequestQueue, CyberCore::resourceLoadPriorityCount> m_requestsPending;
        typedef HashSet<RefPtr<CyberCore::ResourceLoader>> RequestMap;
        RequestMap m_requestsLoading;
        const String m_name;
        const unsigned m_maxRequestsInFlight;
    };

    enum CreateHostPolicy {
        CreateIfNotFound,
        FindOnly
    };
    
    HostInformation* hostForURL(const URL&, CreateHostPolicy = FindOnly);
    void servePendingRequests(HostInformation*, CyberCore::ResourceLoadPriority);

    typedef HashMap<String, HostInformation*, StringHash> HostMap;
    HostMap m_hosts;
    HostInformation* m_nonHTTPProtocolHost;
        
    CyberCore::Timer m_requestTimer;

    unsigned m_suspendPendingRequestsCount;
    bool m_isSerialLoadingEnabled;
};
