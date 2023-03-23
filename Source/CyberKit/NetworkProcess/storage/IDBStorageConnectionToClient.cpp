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

#include "config.h"
#include "IDBStorageConnectionToClient.h"

#include "CyberCoreArgumentCoders.h"
#include "WebIDBConnectionToServerMessages.h"
#include "WebIDBResult.h"
#include <CyberCore/IDBRequestData.h>
#include <CyberCore/IDBResultData.h>
#include <CyberCore/UniqueIDBDatabaseConnection.h>

namespace CyberKit {

IDBStorageConnectionToClient::IDBStorageConnectionToClient(IPC::Connection::UniqueID connection, CyberCore::IDBConnectionIdentifier identifier)
    : m_connection(connection)
    , m_identifier(identifier)
    , m_connectionToClient(CyberCore::IDBServer::IDBConnectionToClient::create(*this))
{
}

IDBStorageConnectionToClient::~IDBStorageConnectionToClient()
{
    m_connectionToClient->clearDelegate();
}

CyberCore::IDBServer::IDBConnectionToClient& IDBStorageConnectionToClient::connectionToClient()
{
    return m_connectionToClient;
}

template<class MessageType> void IDBStorageConnectionToClient::didGetResult(const CyberCore::IDBResultData& resultData)
{
    if (resultData.type() == CyberCore::IDBResultType::Error) {
        IPC::Connection::send(m_connection, MessageType(resultData), 0);
        return;
    }

    if (resultData.type() == CyberCore::IDBResultType::GetAllRecordsSuccess && resultData.getAllResult().type() == CyberCore::IndexedDB::GetAllType::Keys) {
        IPC::Connection::send(m_connection, MessageType(resultData), 0);
        return;
    }

    auto blobFilePaths = resultData.type() == CyberCore::IDBResultType::GetAllRecordsSuccess ? resultData.getAllResult().allBlobFilePaths() : resultData.getResult().value().blobFilePaths();
    if (blobFilePaths.isEmpty()) {
        IPC::Connection::send(m_connection, MessageType(resultData), 0);
        return;
    }

    IPC::Connection::send(m_connection, MessageType(resultData), 0);
}

void IDBStorageConnectionToClient::didDeleteDatabase(const CyberCore::IDBResultData& resultData)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidDeleteDatabase(resultData), 0);
}

void IDBStorageConnectionToClient::didOpenDatabase(const CyberCore::IDBResultData& resultData)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidOpenDatabase(resultData), 0);
}

void IDBStorageConnectionToClient::didStartTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier, const CyberCore::IDBError& error)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidStartTransaction(transactionIdentifier, error), 0);
}

void IDBStorageConnectionToClient::didAbortTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier, const CyberCore::IDBError& error)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidAbortTransaction(transactionIdentifier, error), 0);
}

void IDBStorageConnectionToClient::didCommitTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier, const CyberCore::IDBError& error)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidCommitTransaction(transactionIdentifier, error), 0);
}

void IDBStorageConnectionToClient::didCreateObjectStore(const CyberCore::IDBResultData& resultData)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidCreateObjectStore(resultData), 0);
}

void IDBStorageConnectionToClient::didDeleteObjectStore(const CyberCore::IDBResultData& resultData)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidDeleteObjectStore(resultData), 0);
}

void IDBStorageConnectionToClient::didRenameObjectStore(const CyberCore::IDBResultData& resultData)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidRenameObjectStore(resultData), 0);
}

void IDBStorageConnectionToClient::didClearObjectStore(const CyberCore::IDBResultData& resultData)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidClearObjectStore(resultData), 0);
}

void IDBStorageConnectionToClient::didCreateIndex(const CyberCore::IDBResultData& resultData)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidCreateIndex(resultData), 0);
}

void IDBStorageConnectionToClient::didDeleteIndex(const CyberCore::IDBResultData& resultData)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidDeleteIndex(resultData), 0);
}

void IDBStorageConnectionToClient::didRenameIndex(const CyberCore::IDBResultData& resultData)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidRenameIndex(resultData), 0);
}

void IDBStorageConnectionToClient::didPutOrAdd(const CyberCore::IDBResultData& resultData)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidPutOrAdd(resultData), 0);
}

void IDBStorageConnectionToClient::didGetRecord(const CyberCore::IDBResultData& resultData)
{
    didGetResult<Messages::WebIDBConnectionToServer::DidGetRecord>(resultData);
}

void IDBStorageConnectionToClient::didGetAllRecords(const CyberCore::IDBResultData& resultData)
{
    didGetResult<Messages::WebIDBConnectionToServer::DidGetAllRecords>(resultData);
}

void IDBStorageConnectionToClient::didGetCount(const CyberCore::IDBResultData& resultData)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidGetCount(resultData), 0);
}

void IDBStorageConnectionToClient::didDeleteRecord(const CyberCore::IDBResultData& resultData)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidDeleteRecord(resultData), 0);
}

void IDBStorageConnectionToClient::didOpenCursor(const CyberCore::IDBResultData& resultData)
{
    didGetResult<Messages::WebIDBConnectionToServer::DidOpenCursor>(resultData);
}

void IDBStorageConnectionToClient::didIterateCursor(const CyberCore::IDBResultData& resultData)
{
    didGetResult<Messages::WebIDBConnectionToServer::DidIterateCursor>(resultData);
}

void IDBStorageConnectionToClient::didGetAllDatabaseNamesAndVersions(const CyberCore::IDBResourceIdentifier& requestIdentifier, Vector<CyberCore::IDBDatabaseNameAndVersion>&& databases)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidGetAllDatabaseNamesAndVersions(requestIdentifier, databases), 0);
}

void IDBStorageConnectionToClient::fireVersionChangeEvent(CyberCore::IDBServer::UniqueIDBDatabaseConnection& connection, const CyberCore::IDBResourceIdentifier& requestIdentifier, uint64_t requestedVersion)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::FireVersionChangeEvent(connection.identifier(), requestIdentifier, requestedVersion), 0);
}

void IDBStorageConnectionToClient::didCloseFromServer(CyberCore::IDBServer::UniqueIDBDatabaseConnection& connection, const CyberCore::IDBError& error)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::DidCloseFromServer(connection.identifier(), error), 0);
}

void IDBStorageConnectionToClient::notifyOpenDBRequestBlocked(const CyberCore::IDBResourceIdentifier& requestIdentifier, uint64_t oldVersion, uint64_t newVersion)
{
    IPC::Connection::send(m_connection, Messages::WebIDBConnectionToServer::NotifyOpenDBRequestBlocked(requestIdentifier, oldVersion, newVersion), 0);
}

} // namespace CyberKit
