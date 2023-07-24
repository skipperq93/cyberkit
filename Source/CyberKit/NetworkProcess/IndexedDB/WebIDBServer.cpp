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

#include "config.h"
#include "WebIDBServer.h"

#include "WebIDBConnectionToClient.h"
#include "WebIDBServerMessages.h"
#include <CyberCore/SQLiteDatabaseTracker.h>
#include <CyberCore/StorageQuotaManager.h>
#include <wtf/threads/BinarySemaphore.h>

#if ENABLE(INDEXED_DATABASE)

namespace CyberKit {

Ref<WebIDBServer> WebIDBServer::create(PAL::SessionID sessionID, const String& directory, CyberCore::IDBServer::IDBServer::StorageQuotaManagerSpaceRequester&& spaceRequester)
{
    return adoptRef(*new WebIDBServer(sessionID, directory, WTFMove(spaceRequester)));
}

WebIDBServer::WebIDBServer(PAL::SessionID sessionID, const String& directory, CyberCore::IDBServer::IDBServer::StorageQuotaManagerSpaceRequester&& spaceRequester)
    : CrossThreadTaskHandler("com.matthewbenedict.CyberKit.IndexedDBServer", WTF::CrossThreadTaskHandler::AutodrainedPoolForRunLoop::Use)
{
    ASSERT(RunLoop::isMain());

    BinarySemaphore semaphore;
    postTask([this, protectedThis = makeRef(*this), &semaphore, sessionID, directory = directory.isolatedCopy(), spaceRequester = WTFMove(spaceRequester)] () mutable {
        m_server = makeUnique<CyberCore::IDBServer::IDBServer>(sessionID, directory, WTFMove(spaceRequester));
        semaphore.signal();
    });
    semaphore.wait();
}

WebIDBServer::~WebIDBServer()
{
    ASSERT(RunLoop::isMain());
}

void WebIDBServer::closeAndDeleteDatabasesModifiedSince(WallTime modificationTime, CompletionHandler<void()>&& callback)
{
    ASSERT(RunLoop::isMain());

    postTask([this, protectedThis = makeRef(*this), modificationTime, callback = WTFMove(callback)]() mutable {
        ASSERT(!RunLoop::isMain());

        LockHolder locker(m_server->lock());
        m_server->closeAndDeleteDatabasesModifiedSince(modificationTime);
        postTaskReply(CrossThreadTask([callback = WTFMove(callback)]() mutable {
            callback();
        }));
    });
}

void WebIDBServer::closeAndDeleteDatabasesForOrigins(const Vector<CyberCore::SecurityOriginData>& originDatas, CompletionHandler<void()>&& callback)
{
    ASSERT(RunLoop::isMain());

    postTask([this, protectedThis = makeRef(*this), originDatas = originDatas.isolatedCopy(), callback = WTFMove(callback)] () mutable {
        ASSERT(!RunLoop::isMain());

        LockHolder locker(m_server->lock());
        m_server->closeAndDeleteDatabasesForOrigins(originDatas);
        postTaskReply(CrossThreadTask([callback = WTFMove(callback)]() mutable {
            callback();
        }));
    });
}

void WebIDBServer::renameOrigin(const CyberCore::SecurityOriginData& oldOrigin, const CyberCore::SecurityOriginData& newOrigin, CompletionHandler<void()>&& callback)
{
    ASSERT(RunLoop::isMain());

    postTask([this, protectedThis = makeRef(*this), oldOrigin = oldOrigin.isolatedCopy(), newOrigin = newOrigin.isolatedCopy(), callback = WTFMove(callback)] () mutable {
        ASSERT(!RunLoop::isMain());

        LockHolder locker(m_server->lock());
        m_server->renameOrigin(oldOrigin, newOrigin);
        postTaskReply(CrossThreadTask(WTFMove(callback)));
    });
}

void WebIDBServer::suspend()
{
    ASSERT(RunLoop::isMain());

    if (m_isSuspended)
        return;

    m_isSuspended = true;
    m_server->lock().lock();
    m_server->stopDatabaseActivitiesOnMainThread();
}

void WebIDBServer::resume()
{
    ASSERT(RunLoop::isMain());

    if (!m_isSuspended)
        return;

    m_isSuspended = false;
    m_server->lock().unlock();
}

void WebIDBServer::openDatabase(const CyberCore::IDBRequestData& requestData)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->openDatabase(requestData);
}

void WebIDBServer::deleteDatabase(const CyberCore::IDBRequestData& requestData)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->deleteDatabase(requestData);
}

void WebIDBServer::abortTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->abortTransaction(transactionIdentifier);
}

void WebIDBServer::commitTransaction(const CyberCore::IDBResourceIdentifier& transactionIdentifier)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->commitTransaction(transactionIdentifier);
}

void WebIDBServer::didFinishHandlingVersionChangeTransaction(uint64_t databaseConnectionIdentifier, const CyberCore::IDBResourceIdentifier& transactionIdentifier)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->didFinishHandlingVersionChangeTransaction(databaseConnectionIdentifier, transactionIdentifier);
}

void WebIDBServer::createObjectStore(const CyberCore::IDBRequestData& requestData, const CyberCore::IDBObjectStoreInfo& objectStoreInfo)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->createObjectStore(requestData, objectStoreInfo);
}

void WebIDBServer::deleteObjectStore(const CyberCore::IDBRequestData& requestData, const String& objectStoreName)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->deleteObjectStore(requestData, objectStoreName);
}

void WebIDBServer::renameObjectStore(const CyberCore::IDBRequestData& requestData, uint64_t objectStoreIdentifier, const String& newName)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->renameObjectStore(requestData, objectStoreIdentifier, newName);
}

void WebIDBServer::clearObjectStore(const CyberCore::IDBRequestData& requestData, uint64_t objectStoreIdentifier)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->clearObjectStore(requestData, objectStoreIdentifier);
}

void WebIDBServer::createIndex(const CyberCore::IDBRequestData& requestData, const CyberCore::IDBIndexInfo& indexInfo)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->createIndex(requestData, indexInfo);
}

void WebIDBServer::deleteIndex(const CyberCore::IDBRequestData& requestData, uint64_t objectStoreIdentifier, const String& indexName)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->deleteIndex(requestData, objectStoreIdentifier, indexName);
}

void WebIDBServer::renameIndex(const CyberCore::IDBRequestData& requestData, uint64_t objectStoreIdentifier, uint64_t indexIdentifier, const String& newName)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->renameIndex(requestData, objectStoreIdentifier, indexIdentifier, newName);
}

void WebIDBServer::putOrAdd(const CyberCore::IDBRequestData& requestData, const CyberCore::IDBKeyData& keyData, const CyberCore::IDBValue& value, CyberCore::IndexedDB::ObjectStoreOverwriteMode overWriteMode)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->putOrAdd(requestData, keyData, value, overWriteMode);
}

void WebIDBServer::getRecord(const CyberCore::IDBRequestData& requestData, const CyberCore::IDBGetRecordData& getRecordData)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->getRecord(requestData, getRecordData);
}

void WebIDBServer::getAllRecords(const CyberCore::IDBRequestData& requestData, const CyberCore::IDBGetAllRecordsData& getAllRecordsData)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->getAllRecords(requestData, getAllRecordsData);
}

void WebIDBServer::getCount(const CyberCore::IDBRequestData& requestData, const CyberCore::IDBKeyRangeData& keyRangeData)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->getCount(requestData, keyRangeData);
}

void WebIDBServer::deleteRecord(const CyberCore::IDBRequestData& requestData, const CyberCore::IDBKeyRangeData& keyRangeData)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->deleteRecord(requestData, keyRangeData);
}

void WebIDBServer::openCursor(const CyberCore::IDBRequestData& requestData, const CyberCore::IDBCursorInfo& cursorInfo)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->openCursor(requestData, cursorInfo);
}

void WebIDBServer::iterateCursor(const CyberCore::IDBRequestData& requestData, const CyberCore::IDBIterateCursorData& iterateCursorData)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->iterateCursor(requestData, iterateCursorData);
}

void WebIDBServer::establishTransaction(uint64_t databaseConnectionIdentifier, const CyberCore::IDBTransactionInfo& transactionInfo)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->establishTransaction(databaseConnectionIdentifier, transactionInfo);
}

void WebIDBServer::databaseConnectionPendingClose(uint64_t databaseConnectionIdentifier)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->databaseConnectionPendingClose(databaseConnectionIdentifier);
}

void WebIDBServer::databaseConnectionClosed(uint64_t databaseConnectionIdentifier)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->databaseConnectionClosed(databaseConnectionIdentifier);
}

void WebIDBServer::abortOpenAndUpgradeNeeded(uint64_t databaseConnectionIdentifier, const CyberCore::IDBResourceIdentifier& transactionIdentifier)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->abortOpenAndUpgradeNeeded(databaseConnectionIdentifier, transactionIdentifier);
}

void WebIDBServer::didFireVersionChangeEvent(uint64_t databaseConnectionIdentifier, const CyberCore::IDBResourceIdentifier& requestIdentifier, CyberCore::IndexedDB::ConnectionClosedOnBehalfOfServer connectionClosed)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->didFireVersionChangeEvent(databaseConnectionIdentifier, requestIdentifier, connectionClosed);
}

void WebIDBServer::openDBRequestCancelled(const CyberCore::IDBRequestData& requestData)
{
    ASSERT(!RunLoop::isMain());

    LockHolder locker(m_server->lock());
    m_server->openDBRequestCancelled(requestData);
}

void WebIDBServer::getAllDatabaseNamesAndVersions(IPC::Connection& connection, const CyberCore::IDBResourceIdentifier& requestIdentifier, const CyberCore::ClientOrigin& origin)
{
    ASSERT(!RunLoop::isMain());

    auto* webIDBConnection = m_connectionMap.get(connection.uniqueID());
    ASSERT(webIDBConnection);

    LockHolder locker(m_server->lock());
    m_server->getAllDatabaseNamesAndVersions(webIDBConnection->identifier(), requestIdentifier, origin);
}

void WebIDBServer::addConnection(IPC::Connection& connection, CyberCore::ProcessIdentifier processIdentifier)
{
    ASSERT(RunLoop::isMain());

    postTask([this, protectedThis = makeRef(*this), protectedConnection = makeRefPtr(connection), processIdentifier] {
        auto[iter, isNewEntry] = m_connectionMap.ensure(protectedConnection->uniqueID(), [&] {
            return makeUnique<WebIDBConnectionToClient>(*protectedConnection, processIdentifier);
        });

        ASSERT_UNUSED(isNewEntry, isNewEntry);

        LockHolder locker(m_server->lock());
        m_server->registerConnection(iter->value->connectionToClient());
    });
    m_connections.add(&connection);
    connection.addThreadMessageReceiver(Messages::WebIDBServer::messageReceiverName(), this);
}

void WebIDBServer::removeConnection(IPC::Connection& connection)
{
    ASSERT(RunLoop::isMain());

    m_connections.remove(&connection);
    connection.removeThreadMessageReceiver(Messages::WebIDBServer::messageReceiverName());
    postTask([this, protectedThis = makeRef(*this), connectionID = connection.uniqueID()] {
        auto connection = m_connectionMap.take(connectionID);

        ASSERT(connection);

        LockHolder locker(m_server->lock());
        m_server->unregisterConnection(connection->connectionToClient());
    });
}

void WebIDBServer::postTask(Function<void()>&& task)
{
    ASSERT(RunLoop::isMain());

    CrossThreadTaskHandler::postTask(CrossThreadTask(WTFMove(task)));
}

void WebIDBServer::dispatchToThread(Function<void()>&& task)
{
    CrossThreadTaskHandler::postTask(CrossThreadTask(WTFMove(task)));
}

void WebIDBServer::close()
{
    ASSERT(RunLoop::isMain());

    // Remove the references held by IPC::Connection.
    for (auto* connection : m_connections)
        connection->removeThreadMessageReceiver(Messages::WebIDBServer::messageReceiverName());

    CrossThreadTaskHandler::setCompletionCallback([protectedThis = makeRef(*this)]() mutable {
        ASSERT(!RunLoop::isMain());
        callOnMainRunLoop([protectedThis = WTFMove(protectedThis)]() mutable { });
    });

    postTask([this]() mutable {
        m_connectionMap.clear();
        m_server = nullptr;

        CrossThreadTaskHandler::kill();
    });
}

} // namespace CyberKit
#endif
