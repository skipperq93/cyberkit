/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
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

#include <CyberCore/CacheStorageConnection.h>
#include <wtf/HashMap.h>

namespace IPC {
class Connection;
class Decoder;
class Encoder;
}

namespace CyberKit {

class WebCacheStorageProvider;

class WebCacheStorageConnection final : public CyberCore::CacheStorageConnection {
public:
    static Ref<WebCacheStorageConnection> create(WebCacheStorageProvider& provider) { return adoptRef(*new WebCacheStorageConnection(provider)); }

    ~WebCacheStorageConnection();

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&);

private:
    WebCacheStorageConnection(WebCacheStorageProvider&);

    IPC::Connection& connection();

    // CyberCore::CacheStorageConnection
    void open(const CyberCore::ClientOrigin&, const String& cacheName, CyberCore::DOMCacheEngine::CacheIdentifierCallback&&) final;
    void remove(uint64_t cacheIdentifier, CyberCore::DOMCacheEngine::CacheIdentifierCallback&&) final;
    void retrieveCaches(const CyberCore::ClientOrigin&, uint64_t updateCounter, CyberCore::DOMCacheEngine::CacheInfosCallback&&) final;

    void retrieveRecords(uint64_t cacheIdentifier, const CyberCore::RetrieveRecordsOptions&, CyberCore::DOMCacheEngine::RecordsCallback&&) final;
    void batchDeleteOperation(uint64_t cacheIdentifier, const CyberCore::ResourceRequest&, CyberCore::CacheQueryOptions&&, CyberCore::DOMCacheEngine::RecordIdentifiersCallback&&) final;
    void batchPutOperation(uint64_t cacheIdentifier, Vector<CyberCore::DOMCacheEngine::Record>&&, CyberCore::DOMCacheEngine::RecordIdentifiersCallback&&) final;

    void reference(uint64_t cacheIdentifier) final;
    void dereference(uint64_t cacheIdentifier) final;

    void clearMemoryRepresentation(const CyberCore::ClientOrigin&, CyberCore::DOMCacheEngine::CompletionCallback&&) final;
    void engineRepresentation(CompletionHandler<void(const String&)>&&) final;
    void updateQuotaBasedOnSpaceUsage(const CyberCore::ClientOrigin&) final;

    WebCacheStorageProvider& m_provider;
};

}
