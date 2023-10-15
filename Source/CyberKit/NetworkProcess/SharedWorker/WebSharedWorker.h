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

#include <CyberCore/SharedWorkerIdentifier.h>
#include <CyberCore/SharedWorkerKey.h>
#include <CyberCore/SharedWorkerObjectIdentifier.h>
#include <CyberCore/TransferredMessagePort.h>
#include <CyberCore/WorkerFetchResult.h>
#include <CyberCore/WorkerInitializationData.h>
#include <CyberCore/WorkerOptions.h>
#include <wtf/ListHashSet.h>
#include <wtf/WeakPtr.h>

namespace CyberCore {
class RegistrableDomain;
}

namespace CyberKit {

class WebSharedWorkerServer;
class WebSharedWorkerServerToContextConnection;

class WebSharedWorker : public CanMakeWeakPtr<WebSharedWorker> {
    WTF_MAKE_FAST_ALLOCATED;
public:
    WebSharedWorker(WebSharedWorkerServer&, const CyberCore::SharedWorkerKey&, const CyberCore::WorkerOptions&);
    ~WebSharedWorker();

    static WebSharedWorker* fromIdentifier(CyberCore::SharedWorkerIdentifier);

    CyberCore::SharedWorkerIdentifier identifier() const { return m_identifier; }
    const CyberCore::SharedWorkerKey& key() const { return m_key; }
    const CyberCore::WorkerOptions& workerOptions() const { return m_workerOptions; }
    const CyberCore::ClientOrigin& origin() const { return m_key.origin; }
    const URL& url() const { return m_key.url; }
    CyberCore::RegistrableDomain registrableDomain() const;
    WebSharedWorkerServerToContextConnection* contextConnection() const;

    void addSharedWorkerObject(CyberCore::SharedWorkerObjectIdentifier, const CyberCore::TransferredMessagePort&);
    void removeSharedWorkerObject(CyberCore::SharedWorkerObjectIdentifier);
    void suspend(CyberCore::SharedWorkerObjectIdentifier);
    void resume(CyberCore::SharedWorkerObjectIdentifier);
    unsigned sharedWorkerObjectsCount() const { return m_sharedWorkerObjects.size(); }
    void forEachSharedWorkerObject(const Function<void(CyberCore::SharedWorkerObjectIdentifier, const CyberCore::TransferredMessagePort&)>&) const;
    std::optional<CyberCore::ProcessIdentifier> firstSharedWorkerObjectProcess() const;

    void didCreateContextConnection(WebSharedWorkerServerToContextConnection&);

    bool isRunning() const { return m_isRunning; }
    void markAsRunning() { m_isRunning = true; }

    const CyberCore::WorkerInitializationData& initializationData() const { return m_initializationData; }
    void setInitializationData(CyberCore::WorkerInitializationData&& initializationData) { m_initializationData = WTFMove(initializationData); }

    const CyberCore::WorkerFetchResult& fetchResult() const { return m_fetchResult; }
    void setFetchResult(CyberCore::WorkerFetchResult&&);
    bool didFinishFetching() const { return !!m_fetchResult.script; }

    void launch(WebSharedWorkerServerToContextConnection&);

    struct SharedWorkerObjectState {
        bool isSuspended { false };
        CyberCore::TransferredMessagePort port;
    };

    struct Object {
        CyberCore::SharedWorkerObjectIdentifier identifier;
        SharedWorkerObjectState state;
    };

private:
    WebSharedWorker(WebSharedWorker&&) = delete;
    WebSharedWorker& operator=(WebSharedWorker&&) = delete;
    WebSharedWorker(const WebSharedWorker&) = delete;
    WebSharedWorker& operator=(const WebSharedWorker&) = delete;

    void suspendIfNeeded();
    void resumeIfNeeded();

    WebSharedWorkerServer& m_server;
    CyberCore::SharedWorkerIdentifier m_identifier;
    CyberCore::SharedWorkerKey m_key;
    CyberCore::WorkerOptions m_workerOptions;
    ListHashSet<Object> m_sharedWorkerObjects;
    CyberCore::WorkerFetchResult m_fetchResult;
    CyberCore::WorkerInitializationData m_initializationData;
    bool m_isRunning { false };
    bool m_isSuspended { false };
};

} // namespace CyberKit

namespace WTF {

struct WebSharedWorkerObjectHash {
    static unsigned hash(const CyberKit::WebSharedWorker::Object& object) { return DefaultHash<CyberCore::SharedWorkerObjectIdentifier>::hash(object.identifier); }
    static bool equal(const CyberKit::WebSharedWorker::Object& a, const CyberKit::WebSharedWorker::Object& b) { return a.identifier == b.identifier; }
    static constexpr bool safeToCompareToEmptyOrDeleted = true;
};

template<> struct HashTraits<CyberKit::WebSharedWorker::Object> : SimpleClassHashTraits<WebSharedWorkerObjectHash> { };
template<> struct DefaultHash<CyberKit::WebSharedWorker::Object> : WebSharedWorkerObjectHash { };

} // namespace WTF
