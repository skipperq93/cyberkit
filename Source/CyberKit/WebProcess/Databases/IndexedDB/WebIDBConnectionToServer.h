/*
 * Copyright (C) 2016 Apple Inc. All rights reserved.
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

#include "MessageSender.h"
#include "SandboxExtension.h"
#include <CyberCore/IDBConnectionToServer.h>
#include <CyberCore/ProcessIdentifier.h>

namespace CyberKit {

class WebIDBResult;

class WebIDBConnectionToServer final : private CyberCore::IDBClient::IDBConnectionToServerDelegate, private IPC::MessageSender, public RefCounted<WebIDBConnectionToServer> {
public:
    static Ref<WebIDBConnectionToServer> create();
    virtual ~WebIDBConnectionToServer();

    CyberCore::IDBClient::IDBConnectionToServer& coreConnectionToServer();
    CyberCore::IDBConnectionIdentifier identifier() const final;

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&);
    void connectionToServerLost();

private:
    WebIDBConnectionToServer();

    IPC::Connection* messageSenderConnection() const final;
    uint64_t messageSenderDestinationID() const final { return 0; }

    // IDBConnectionToServerDelegate
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

    void getAllDatabaseNames(const CyberCore::SecurityOriginData& topOrigin, const CyberCore::SecurityOriginData& openingOrigin, uint64_t callbackID) final;

    // Messages received from Network Process
    void didDeleteDatabase(const CyberCore::IDBResultData&);
    void didOpenDatabase(const CyberCore::IDBResultData&);
    void didAbortTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier, const CyberCore::IDBError&);
    void didCommitTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier, const CyberCore::IDBError&);
    void didCreateObjectStore(const CyberCore::IDBResultData&);
    void didDeleteObjectStore(const CyberCore::IDBResultData&);
    void didRenameObjectStore(const CyberCore::IDBResultData&);
    void didClearObjectStore(const CyberCore::IDBResultData&);
    void didCreateIndex(const CyberCore::IDBResultData&);
    void didDeleteIndex(const CyberCore::IDBResultData&);
    void didRenameIndex(const CyberCore::IDBResultData&);
    void didPutOrAdd(const CyberCore::IDBResultData&);
    void didGetRecord(const WebIDBResult&);
    void didGetAllRecords(const WebIDBResult&);
    void didGetCount(const CyberCore::IDBResultData&);
    void didDeleteRecord(const CyberCore::IDBResultData&);
    void didOpenCursor(const WebIDBResult&);
    void didIterateCursor(const WebIDBResult&);
    void fireVersionChangeEvent(uint64_t uniqueDatabaseConnectionIdentifier, const CyberCore::IDBResourceIdentifier& requestIdentifier, uint64_t requestedVersion);
    void didStartTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier, const CyberCore::IDBError&);
    void didCloseFromServer(uint64_t databaseConnectionIdentifier, const CyberCore::IDBError&);
    void notifyOpenDBRequestBlocked(const CyberCore::IDBResourceIdentifier& requestIdentifier, uint64_t oldVersion, uint64_t newVersion);
    void didGetAllDatabaseNames(uint64_t callbackID, const Vector<String>& databaseNames);

    Ref<CyberCore::IDBClient::IDBConnectionToServer> m_connectionToServer;
};

} // namespace CyberKit

#endif // ENABLE(INDEXED_DATABASE)
