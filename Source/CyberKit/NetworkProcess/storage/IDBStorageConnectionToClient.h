/*
 * Copyright (C) 2016-2022 Apple Inc. All rights reserved.
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

#include "Connection.h"
#include <CyberCore/IDBConnectionToClient.h>
#include <CyberCore/IDBConnectionToClientDelegate.h>

namespace CyberKit {

class IDBStorageConnectionToClient final : public CyberCore::IDBServer::IDBConnectionToClientDelegate {
    WTF_MAKE_FAST_ALLOCATED;
public:
    IDBStorageConnectionToClient(IPC::Connection::UniqueID, CyberCore::IDBConnectionIdentifier);
    ~IDBStorageConnectionToClient();

    CyberCore::IDBConnectionIdentifier identifier() const final { return m_identifier; }
    IPC::Connection::UniqueID ipcConnection() const { return m_connection; }
    CyberCore::IDBServer::IDBConnectionToClient& connectionToClient();

private:
    template<class MessageType> void didGetResult(const CyberCore::IDBResultData&);

    // IDBConnectionToClientDelegate
    void didDeleteDatabase(const CyberCore::IDBResultData&) final;
    void didOpenDatabase(const CyberCore::IDBResultData&) final;
    void didAbortTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier, const CyberCore::IDBError&) final;
    void didCommitTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier, const CyberCore::IDBError&) final;
    void didStartTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier, const CyberCore::IDBError&) final;
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
    void didGetAllDatabaseNamesAndVersions(const CyberCore::IDBResourceIdentifier&, Vector<CyberCore::IDBDatabaseNameAndVersion>&&) final;
    void notifyOpenDBRequestBlocked(const CyberCore::IDBResourceIdentifier& requestIdentifier, uint64_t oldVersion, uint64_t newVersion) final;
    void fireVersionChangeEvent(CyberCore::IDBServer::UniqueIDBDatabaseConnection&, const CyberCore::IDBResourceIdentifier& requestIdentifier, uint64_t requestedVersion) final;
    void didCloseFromServer(CyberCore::IDBServer::UniqueIDBDatabaseConnection&, const CyberCore::IDBError&) final;

    IPC::Connection::UniqueID m_connection;
    CyberCore::IDBConnectionIdentifier m_identifier;
    Ref<CyberCore::IDBServer::IDBConnectionToClient> m_connectionToClient;
};

} // namespace CyberKit
