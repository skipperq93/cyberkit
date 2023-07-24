/*
 * Copyright (C) 2016-2019 Apple Inc. All rights reserved.
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
#include "WebIDBConnectionToClient.h"

#if ENABLE(INDEXED_DATABASE)

#include "CyberCoreArgumentCoders.h"
#include "WebIDBConnectionToServerMessages.h"
#include "WebIDBResult.h"
#include "WebIDBServer.h"
#include <CyberCore/IDBGetAllRecordsData.h>
#include <CyberCore/IDBGetRecordData.h>
#include <CyberCore/IDBResultData.h>
#include <CyberCore/UniqueIDBDatabaseConnection.h>

namespace CyberKit {
using namespace CyberCore;

WebIDBConnectionToClient::WebIDBConnectionToClient(IPC::Connection& connection, CyberCore::IDBConnectionIdentifier serverConnectionIdentifier)
    : m_connection(makeRef(connection))
    , m_identifier(serverConnectionIdentifier)
    , m_connectionToClient(IDBServer::IDBConnectionToClient::create(*this))
{
}

WebIDBConnectionToClient::~WebIDBConnectionToClient()
{
    m_connectionToClient->clearDelegate();
}

IPC::Connection* WebIDBConnectionToClient::messageSenderConnection() const
{
    return m_connection.ptr();
}

CyberCore::IDBServer::IDBConnectionToClient& WebIDBConnectionToClient::connectionToClient()
{
    return m_connectionToClient;
}

void WebIDBConnectionToClient::didDeleteDatabase(const CyberCore::IDBResultData& resultData)
{
    send(Messages::WebIDBConnectionToServer::DidDeleteDatabase(resultData));
}

void WebIDBConnectionToClient::didOpenDatabase(const CyberCore::IDBResultData& resultData)
{
    send(Messages::WebIDBConnectionToServer::DidOpenDatabase(resultData));
}

void WebIDBConnectionToClient::didAbortTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier, const CyberCore::IDBError& error)
{
    send(Messages::WebIDBConnectionToServer::DidAbortTransaction(transactionIdentifier, error));
}

void WebIDBConnectionToClient::didCommitTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier, const CyberCore::IDBError& error)
{
    send(Messages::WebIDBConnectionToServer::DidCommitTransaction(transactionIdentifier, error));
}

void WebIDBConnectionToClient::didCreateObjectStore(const CyberCore::IDBResultData& resultData)
{
    send(Messages::WebIDBConnectionToServer::DidCreateObjectStore(resultData));
}

void WebIDBConnectionToClient::didDeleteObjectStore(const CyberCore::IDBResultData& resultData)
{
    send(Messages::WebIDBConnectionToServer::DidDeleteObjectStore(resultData));
}

void WebIDBConnectionToClient::didRenameObjectStore(const CyberCore::IDBResultData& resultData)
{
    send(Messages::WebIDBConnectionToServer::DidRenameObjectStore(resultData));
}

void WebIDBConnectionToClient::didClearObjectStore(const CyberCore::IDBResultData& resultData)
{
    send(Messages::WebIDBConnectionToServer::DidClearObjectStore(resultData));
}

void WebIDBConnectionToClient::didCreateIndex(const CyberCore::IDBResultData& resultData)
{
    send(Messages::WebIDBConnectionToServer::DidCreateIndex(resultData));
}

void WebIDBConnectionToClient::didDeleteIndex(const CyberCore::IDBResultData& resultData)
{
    send(Messages::WebIDBConnectionToServer::DidDeleteIndex(resultData));
}

void WebIDBConnectionToClient::didRenameIndex(const CyberCore::IDBResultData& resultData)
{
    send(Messages::WebIDBConnectionToServer::DidRenameIndex(resultData));
}

void WebIDBConnectionToClient::didPutOrAdd(const CyberCore::IDBResultData& resultData)
{
    send(Messages::WebIDBConnectionToServer::DidPutOrAdd(resultData));
}

template<class MessageType> void WebIDBConnectionToClient::handleGetResult(const CyberCore::IDBResultData& resultData)
{
    if (resultData.type() == IDBResultType::Error) {
        send(MessageType(resultData));
        return;
    }

    if (resultData.type() == IDBResultType::GetAllRecordsSuccess && resultData.getAllResult().type() == IndexedDB::GetAllType::Keys) {
        send(MessageType(resultData));
        return;
    }

    auto blobFilePaths = resultData.type() == IDBResultType::GetAllRecordsSuccess ? resultData.getAllResult().allBlobFilePaths() : resultData.getResult().value().blobFilePaths();
    if (blobFilePaths.isEmpty()) {
        send(MessageType(resultData));
        return;
    }

    send(MessageType(resultData));
}

void WebIDBConnectionToClient::didGetRecord(const CyberCore::IDBResultData& resultData)
{
    handleGetResult<Messages::WebIDBConnectionToServer::DidGetRecord>(resultData);
}

void WebIDBConnectionToClient::didGetAllRecords(const CyberCore::IDBResultData& resultData)
{
    handleGetResult<Messages::WebIDBConnectionToServer::DidGetAllRecords>(resultData);
}

void WebIDBConnectionToClient::didGetCount(const CyberCore::IDBResultData& resultData)
{
    send(Messages::WebIDBConnectionToServer::DidGetCount(resultData));
}

void WebIDBConnectionToClient::didDeleteRecord(const CyberCore::IDBResultData& resultData)
{
    send(Messages::WebIDBConnectionToServer::DidDeleteRecord(resultData));
}

void WebIDBConnectionToClient::didOpenCursor(const CyberCore::IDBResultData& resultData)
{
    handleGetResult<Messages::WebIDBConnectionToServer::DidOpenCursor>(resultData);
}

void WebIDBConnectionToClient::didIterateCursor(const CyberCore::IDBResultData& resultData)
{
    handleGetResult<Messages::WebIDBConnectionToServer::DidIterateCursor>(resultData);
}

void WebIDBConnectionToClient::fireVersionChangeEvent(CyberCore::IDBServer::UniqueIDBDatabaseConnection& connection, const CyberCore::IDBResourceIdentifier& requestIdentifier, uint64_t requestedVersion)
{
    send(Messages::WebIDBConnectionToServer::FireVersionChangeEvent(connection.identifier(), requestIdentifier, requestedVersion));
}

void WebIDBConnectionToClient::didStartTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier, const CyberCore::IDBError& error)
{
    send(Messages::WebIDBConnectionToServer::DidStartTransaction(transactionIdentifier, error));
}

void WebIDBConnectionToClient::didCloseFromServer(CyberCore::IDBServer::UniqueIDBDatabaseConnection& connection, const CyberCore::IDBError& error)
{
    send(Messages::WebIDBConnectionToServer::DidCloseFromServer(connection.identifier(), error));
}

void WebIDBConnectionToClient::notifyOpenDBRequestBlocked(const CyberCore::IDBResourceIdentifier& requestIdentifier, uint64_t oldVersion, uint64_t newVersion)
{
    send(Messages::WebIDBConnectionToServer::NotifyOpenDBRequestBlocked(requestIdentifier, oldVersion, newVersion));
}

void WebIDBConnectionToClient::didGetAllDatabaseNamesAndVersions(const CyberCore::IDBResourceIdentifier& requestIdentifier, const Vector<CyberCore::IDBDatabaseNameAndVersion>& databases)
{
    send(Messages::WebIDBConnectionToServer::DidGetAllDatabaseNamesAndVersions(requestIdentifier, databases));
}

} // namespace CyberKit

#endif // ENABLE(INDEXED_DATABASE)
