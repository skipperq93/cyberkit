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
#include "NetworkConnectionToWebProcess.h"
#include <CyberCore/IDBConnectionToClient.h>
#include <CyberCore/IndexedDB.h>
#include <pal/SessionID.h>

namespace CyberCore {
class IDBCursorInfo;
class IDBIndexInfo;
class IDBKeyData;
class IDBObjectStoreInfo;
class IDBRequestData;
class IDBTransactionInfo;
class IDBValue;
class SerializedScriptValue;
struct IDBGetAllRecordsData;
struct IDBGetRecordData;
struct IDBIterateCursorData;
struct IDBKeyRangeData;
struct SecurityOriginData;
}

namespace CyberKit {

class NetworkProcess;

class WebIDBConnectionToClient final : public CyberCore::IDBServer::IDBConnectionToClientDelegate, public IPC::MessageSender, public RefCounted<WebIDBConnectionToClient> {
public:
    static Ref<WebIDBConnectionToClient> create(NetworkProcess&, IPC::Connection&, uint64_t serverConnectionIdentifier, PAL::SessionID);

    virtual ~WebIDBConnectionToClient();

    CyberCore::IDBServer::IDBConnectionToClient& connectionToClient();
    uint64_t identifier() const final { return m_identifier; }
    uint64_t messageSenderDestinationID() const final { return m_identifier; }

    // IDBConnectionToClientDelegate
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

    void ref() override { RefCounted<WebIDBConnectionToClient>::ref(); }
    void deref() override { RefCounted<WebIDBConnectionToClient>::deref(); }

    // Messages received from WebProcess
    void deleteDatabase(const CyberCore::IDBRequestData&);
    void openDatabase(const CyberCore::IDBRequestData&);
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
    void didFireVersionChangeEvent(uint64_t databaseConnectionIdentifier, const CyberCore::IDBResourceIdentifier& requestIdentifier);
    void openDBRequestCancelled(const CyberCore::IDBRequestData&);
    void confirmDidCloseFromServer(uint64_t databaseConnectionIdentifier);

    void getAllDatabaseNames(uint64_t serverConnectionIdentifier, const CyberCore::SecurityOriginData& topOrigin, const CyberCore::SecurityOriginData& openingOrigin, uint64_t callbackID);

    void disconnectedFromWebProcess();

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&);

private:
    WebIDBConnectionToClient(NetworkProcess&, IPC::Connection&, uint64_t serverConnectionIdentifier, PAL::SessionID);

    IPC::Connection* messageSenderConnection() const final;

    template<class MessageType> void handleGetResult(const CyberCore::IDBResultData&);

    Ref<IPC::Connection> m_connection;
    Ref<NetworkProcess> m_networkProcess;

    uint64_t m_identifier;
    PAL::SessionID m_sessionID;
    RefPtr<CyberCore::IDBServer::IDBConnectionToClient> m_connectionToClient;
};

} // namespace CyberKit

#endif // ENABLE(INDEXED_DATABASE)
