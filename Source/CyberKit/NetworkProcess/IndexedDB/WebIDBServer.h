/*
 * Copyright (C) 2019 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(INDEXED_DATABASE)

#include "Connection.h"

#include "WebIDBConnectionToClient.h"
#include <CyberCore/IDBServer.h>
#include <CyberCore/StorageQuotaManager.h>
#include <wtf/CrossThreadTaskHandler.h>

namespace CyberCore {
class StorageQuotaManager;
namespace IDBServer {
class IDBServer;
}
}

namespace CyberKit {

class WebIDBServer final : public CrossThreadTaskHandler, public IPC::Connection::ThreadMessageReceiver {
public:
    static Ref<WebIDBServer> create(PAL::SessionID, const String& directory, CyberCore::IDBServer::IDBServer::StorageQuotaManagerSpaceRequester&&);

    void closeAndDeleteDatabasesModifiedSince(WallTime, CompletionHandler<void()>&& callback);
    void closeAndDeleteDatabasesForOrigins(const Vector<CyberCore::SecurityOriginData>&, CompletionHandler<void()>&& callback);

    enum class ShouldForceStop : bool { No, Yes };
    void suspend(ShouldForceStop);
    void resume();

    // Message handlers.
    void openDatabase(const CyberCore::IDBRequestData&);
    void deleteDatabase(const CyberCore::IDBRequestData&);
    void abortTransaction(const CyberCore::IDBResourceIdentifier&);
    void commitTransaction(const CyberCore::IDBResourceIdentifier&);
    void didFinishHandlingVersionChangeTransaction(uint64_t databaseConnectionIdentifier, const CyberCore::IDBResourceIdentifier&);
    void createObjectStore(const CyberCore::IDBRequestData&, const CyberCore::IDBObjectStoreInfo&);
    void deleteObjectStore(const CyberCore::IDBRequestData&, const String& objectStoreName);
    void renameObjectStore(const CyberCore::IDBRequestData&, uint64_t objectStoreIdentifier, const String& newName);
    void clearObjectStore(const CyberCore::IDBRequestData&, uint64_t objectStoreIdentifier);
    void createIndex(const CyberCore::IDBRequestData&, const CyberCore::IDBIndexInfo&);
    void deleteIndex(const CyberCore::IDBRequestData&, uint64_t objectStoreIdentifier, const String& indexName);
    void renameIndex(const CyberCore::IDBRequestData&, uint64_t objectStoreIdentifier, uint64_t indexIdentifier, const String& newName);
    void putOrAdd(const CyberCore::IDBRequestData&, const CyberCore::IDBKeyData&, const CyberCore::IDBValue&, CyberCore::IndexedDB::ObjectStoreOverwriteMode);
    void getRecord(const CyberCore::IDBRequestData&, const CyberCore::IDBGetRecordData&);
    void getAllRecords(const CyberCore::IDBRequestData&, const CyberCore::IDBGetAllRecordsData&);
    void getCount(const CyberCore::IDBRequestData&, const CyberCore::IDBKeyRangeData&);
    void deleteRecord(const CyberCore::IDBRequestData&, const CyberCore::IDBKeyRangeData&);
    void openCursor(const CyberCore::IDBRequestData&, const CyberCore::IDBCursorInfo&);
    void iterateCursor(const CyberCore::IDBRequestData&, const CyberCore::IDBIterateCursorData&);
    void establishTransaction(uint64_t databaseConnectionIdentifier, const CyberCore::IDBTransactionInfo&);
    void databaseConnectionPendingClose(uint64_t databaseConnectionIdentifier);
    void databaseConnectionClosed(uint64_t databaseConnectionIdentifier);
    void abortOpenAndUpgradeNeeded(uint64_t databaseConnectionIdentifier, const CyberCore::IDBResourceIdentifier& transactionIdentifier);
    void didFireVersionChangeEvent(uint64_t databaseConnectionIdentifier, const CyberCore::IDBResourceIdentifier& requestIdentifier, const CyberCore::IndexedDB::ConnectionClosedOnBehalfOfServer);
    void openDBRequestCancelled(const CyberCore::IDBRequestData&);
    void getAllDatabaseNames(IPC::Connection&, const CyberCore::SecurityOriginData&, const CyberCore::SecurityOriginData&, uint64_t callbackID);

    void addConnection(IPC::Connection&, CyberCore::ProcessIdentifier);
    void removeConnection(IPC::Connection&);

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&);
    void dispatchToThread(WTF::Function<void()>&&);
    void close();

private:
    WebIDBServer(PAL::SessionID, const String& directory, CyberCore::IDBServer::IDBServer::StorageQuotaManagerSpaceRequester&&);
    ~WebIDBServer();

    void postTask(WTF::Function<void()>&&);

    std::unique_ptr<CyberCore::IDBServer::IDBServer> m_server;
    bool m_isSuspended { false };

    HashMap<IPC::Connection::UniqueID, std::unique_ptr<WebIDBConnectionToClient>> m_connectionMap;
    HashSet<IPC::Connection*> m_connections;
};

} // namespace CyberKit

#endif
