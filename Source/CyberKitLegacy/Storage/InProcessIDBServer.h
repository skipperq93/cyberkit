/*
 * Copyright (C) 2015 Apple Inc. All rights reserved.
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

#if ENABLE(INDEXED_DATABASE)

#include <CyberCore/IDBConnectionToClient.h>
#include <CyberCore/IDBConnectionToServer.h>
#include <CyberCore/IDBServer.h>
#include <wtf/RefCounted.h>
#include <wtf/RefPtr.h>
#include <wtf/ThreadSafeRefCounted.h>

namespace PAL {
class SessionID;
}

namespace CyberCore {
struct ClientOrigin;
class StorageQuotaManager;
class StorageThread;

namespace IDBClient {
class IDBConnectionToServer;
}

namespace IDBServer {
class IDBServer;
}
} // namespace CyberCore

class InProcessIDBServer final : public CyberCore::IDBClient::IDBConnectionToServerDelegate, public CyberCore::IDBServer::IDBConnectionToClientDelegate, public ThreadSafeRefCounted<InProcessIDBServer> {
public:

    WEBCORE_EXPORT static Ref<InProcessIDBServer> create(PAL::SessionID);
    WEBCORE_EXPORT static Ref<InProcessIDBServer> create(PAL::SessionID, const String& databaseDirectoryPath);

    WEBCORE_EXPORT virtual ~InProcessIDBServer();

    CyberCore::IDBClient::IDBConnectionToServer& connectionToServer() const;
    CyberCore::IDBServer::IDBConnectionToClient& connectionToClient() const;
    CyberCore::IDBServer::IDBServer& server() { return *m_server; }

    // IDBConnectionToServer
    void deleteDatabase(const CyberCore::IDBRequestData&) final;
    void openDatabase(const CyberCore::IDBRequestData&) final;
    void abortTransaction(const CyberCore::IDBResourceIdentifier&) final;
    void commitTransaction(const CyberCore::IDBResourceIdentifier&) final;
    void didFinishHandlingVersionChangeTransaction(uint64_t databaseConnectionIdentifier, const CyberCore::IDBResourceIdentifier&) final;
    void createObjectStore(const CyberCore::IDBRequestData&, const CyberCore::IDBObjectStoreInfo&) final;
    void deleteObjectStore(const CyberCore::IDBRequestData&, const String& objectStoreName) final;
    void renameObjectStore(const CyberCore::IDBRequestData&, uint64_t objectStoreIdentifier, const String& newName) final;
    void clearObjectStore(const CyberCore::IDBRequestData&, uint64_t objectStoreIdentifier) final;
    void createIndex(const CyberCore::IDBRequestData&, const CyberCore::IDBIndexInfo&) final;
    void deleteIndex(const CyberCore::IDBRequestData&, uint64_t objectStoreIdentifier, const String& indexName) final;
    void renameIndex(const CyberCore::IDBRequestData&, uint64_t objectStoreIdentifier, uint64_t indexIdentifier, const String& newName) final;
    void putOrAdd(const CyberCore::IDBRequestData&, const CyberCore::IDBKeyData&, const CyberCore::IDBValue&, const CyberCore::IndexedDB::ObjectStoreOverwriteMode) final;
    void getRecord(const CyberCore::IDBRequestData&, const CyberCore::IDBGetRecordData&) final;
    void getAllRecords(const CyberCore::IDBRequestData&, const CyberCore::IDBGetAllRecordsData&) final;
    void getCount(const CyberCore::IDBRequestData&, const CyberCore::IDBKeyRangeData&) final;
    void deleteRecord(const CyberCore::IDBRequestData&, const CyberCore::IDBKeyRangeData&) final;
    void openCursor(const CyberCore::IDBRequestData&, const CyberCore::IDBCursorInfo&) final;
    void iterateCursor(const CyberCore::IDBRequestData&, const CyberCore::IDBIterateCursorData&) final;
    void establishTransaction(uint64_t databaseConnectionIdentifier, const CyberCore::IDBTransactionInfo&) final;
    void databaseConnectionPendingClose(uint64_t databaseConnectionIdentifier) final;
    void databaseConnectionClosed(uint64_t databaseConnectionIdentifier) final;
    void abortOpenAndUpgradeNeeded(uint64_t databaseConnectionIdentifier, const CyberCore::IDBResourceIdentifier& transactionIdentifier) final;
    void didFireVersionChangeEvent(uint64_t databaseConnectionIdentifier, const CyberCore::IDBResourceIdentifier& requestIdentifier, const CyberCore::IndexedDB::ConnectionClosedOnBehalfOfServer) final;
    void openDBRequestCancelled(const CyberCore::IDBRequestData&) final;
    void getAllDatabaseNames(const CyberCore::SecurityOriginData& mainFrameOrigin, const CyberCore::SecurityOriginData& openingOrigin, uint64_t callbackID) final;

    // IDBConnectionToClient
    CyberCore::IDBConnectionIdentifier identifier() const final;
    void didDeleteDatabase(const CyberCore::IDBResultData&) final;
    void didOpenDatabase(const CyberCore::IDBResultData&) final;
    void didAbortTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier, const CyberCore::IDBError&) final;
    void didCommitTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier, const CyberCore::IDBError&) final;
    void didCreateObjectStore(const CyberCore::IDBResultData&) final;
    void didDeleteObjectStore(const CyberCore::IDBResultData&) final;
    void didRenameObjectStore(const CyberCore::IDBResultData&) final;
    void didClearObjectStore(const CyberCore::IDBResultData&) final;
    void didCreateIndex(const CyberCore::IDBResultData&) final;
    void didDeleteIndex(const CyberCore::IDBResultData&) final;
    void didRenameIndex(const CyberCore::IDBResultData&) final;
    void didPutOrAdd(const CyberCore::IDBResultData&) final;
    void didGetRecord(const CyberCore::IDBResultData&) final;
    void didGetAllRecords(const CyberCore::IDBResultData&) final;
    void didGetCount(const CyberCore::IDBResultData&) final;
    void didDeleteRecord(const CyberCore::IDBResultData&) final;
    void didOpenCursor(const CyberCore::IDBResultData&) final;
    void didIterateCursor(const CyberCore::IDBResultData&) final;
    void fireVersionChangeEvent(CyberCore::IDBServer::UniqueIDBDatabaseConnection&, const CyberCore::IDBResourceIdentifier& requestIdentifier, uint64_t requestedVersion) final;
    void didStartTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier, const CyberCore::IDBError&) final;
    void didCloseFromServer(CyberCore::IDBServer::UniqueIDBDatabaseConnection&, const CyberCore::IDBError&) final;
    void notifyOpenDBRequestBlocked(const CyberCore::IDBResourceIdentifier& requestIdentifier, uint64_t oldVersion, uint64_t newVersion) final;
    void didGetAllDatabaseNames(uint64_t callbackID, const Vector<String>& databaseNames) final;

    void closeAndDeleteDatabasesModifiedSince(WallTime);

    void dispatchTask(Function<void()>&&);
    void dispatchTaskReply(Function<void()>&&);

    CyberCore::StorageQuotaManager* quotaManager(const CyberCore::ClientOrigin&);

private:
    InProcessIDBServer(PAL::SessionID, const String& databaseDirectoryPath = nullString());

    std::unique_ptr<CyberCore::IDBServer::IDBServer> m_server;
    RefPtr<CyberCore::IDBClient::IDBConnectionToServer> m_connectionToServer;
    RefPtr<CyberCore::IDBServer::IDBConnectionToClient> m_connectionToClient;
    std::unique_ptr<CyberCore::StorageThread> m_thread;

    HashMap<CyberCore::ClientOrigin, RefPtr<CyberCore::StorageQuotaManager>> m_quotaManagers;
};

#endif // ENABLE(INDEXED_DATABASE)
