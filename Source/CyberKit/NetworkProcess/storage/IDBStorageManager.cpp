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

#include "config.h"
#include "IDBStorageManager.h"

#include "IDBStorageRegistry.h"
#include <CyberCore/IDBRequestData.h>
#include <CyberCore/IDBServer.h>
#include <CyberCore/MemoryIDBBackingStore.h>
#include <CyberCore/SQLiteFileSystem.h>
#include <CyberCore/SQLiteIDBBackingStore.h>

namespace CyberKit {

static bool migrateOriginDataImpl(const String& oldOriginDirectory, const String& newOriginDirectory, Function<String(const String&)>&& createFileNameFunction)
{
    if (oldOriginDirectory.isEmpty() || !FileSystem::fileExists(oldOriginDirectory))
        return true;

    auto fileNames = FileSystem::listDirectory(oldOriginDirectory);
    if (fileNames.isEmpty()) {
        FileSystem::deleteEmptyDirectory(oldOriginDirectory);
        return true;
    }

    FileSystem::makeAllDirectories(newOriginDirectory);
    bool allMoved = true;
    for (auto& name : fileNames) {
        // This is an origin directory for third-party data.
        if (auto origin = CyberCore::SecurityOriginData::fromDatabaseIdentifier(name))
            continue;

        // Do not overwrite existing files.
        auto newPath = FileSystem::pathByAppendingComponent(newOriginDirectory, createFileNameFunction(name));
        if (FileSystem::fileExists(newPath))
            continue;

        auto oldPath = FileSystem::pathByAppendingComponent(oldOriginDirectory, name);
        allMoved &= FileSystem::moveFile(oldPath, newPath);
    }

    FileSystem::deleteEmptyDirectory(oldOriginDirectory);
    return allMoved;
}

String IDBStorageManager::idbStorageOriginDirectory(const String& rootDirectory, const CyberCore::ClientOrigin& origin)
{
    if (rootDirectory.isEmpty())
        return emptyString();

    auto originDirectory = CyberCore::IDBDatabaseIdentifier::databaseDirectoryRelativeToRoot(origin, rootDirectory, "v1"_s);
    auto oldOriginDirectory = CyberCore::IDBDatabaseIdentifier::databaseDirectoryRelativeToRoot(origin, rootDirectory, "v0"_s);
    migrateOriginDataImpl(oldOriginDirectory, originDirectory, [](const String& name) {
        return CyberCore::SQLiteFileSystem::computeHashForFileName(CyberCore::IDBServer::SQLiteIDBBackingStore::decodeDatabaseName(name));
    });

    return originDirectory;
}

uint64_t IDBStorageManager::idbStorageSize(const String& originDirectory)
{
    if (originDirectory.isEmpty())
        return 0;

    return CyberCore::IDBServer::SQLiteIDBBackingStore::databasesSizeForDirectory(originDirectory);
}

static void getOriginsForVersion(const String& versionPath, HashSet<CyberCore::ClientOrigin>& origins)
{
    for (auto& topDatabaseIdentifier : FileSystem::listDirectory(versionPath)) {
        auto topOrigin = CyberCore::SecurityOriginData::fromDatabaseIdentifier(topDatabaseIdentifier);
        if (!topOrigin)
            continue;

        auto topOriginDirectory = FileSystem::pathByAppendingComponent(versionPath, topDatabaseIdentifier);
        for (auto& databaseIdentifier : FileSystem::listDirectory(topOriginDirectory)) {
            auto originDirectory = FileSystem::pathByAppendingComponent(topOriginDirectory, databaseIdentifier);
            if (FileSystem::deleteEmptyDirectory(originDirectory))
                continue;

            auto clientOrigin = CyberCore::SecurityOriginData::fromDatabaseIdentifier(databaseIdentifier);
            // This is not origin directory, but may be database directory.
            if (!clientOrigin) {
                origins.add(CyberCore::ClientOrigin { *topOrigin, *topOrigin });
                continue;
            }

            origins.add(CyberCore::ClientOrigin { *topOrigin, *clientOrigin });
        }

        // We may have deleted empty children directories above, which make this directory empty.
        FileSystem::deleteEmptyDirectory(topOriginDirectory);
    }
}

HashSet<CyberCore::ClientOrigin> IDBStorageManager::originsOfIDBStorageData(const String& rootDirectory)
{
    HashSet<CyberCore::ClientOrigin> origins;
    if (rootDirectory.isEmpty())
        return origins;

    getOriginsForVersion(FileSystem::pathByAppendingComponent(rootDirectory, "v0"_s), origins);
    getOriginsForVersion(FileSystem::pathByAppendingComponent(rootDirectory, "v1"_s), origins);

    return origins;
}

bool IDBStorageManager::migrateOriginData(const String& oldOriginDirectory, const String& newOriginDirectory)
{
    return migrateOriginDataImpl(oldOriginDirectory, newOriginDirectory, [](const String& name) {
        return name;
    });
}

IDBStorageManager::IDBStorageManager(const String& path, IDBStorageRegistry& registry, QuotaCheckFunction&& quotaCheckFunction)
    : m_path(path)
    , m_registry(registry)
    , m_quotaCheckFunction(WTFMove(quotaCheckFunction))
{
}

IDBStorageManager::~IDBStorageManager()
{
    for (auto& database : m_databases.values())
        database->immediateClose();
}

bool IDBStorageManager::isActive() const
{
    return !m_databases.isEmpty();
}

bool IDBStorageManager::hasDataInMemory() const
{
    return WTF::anyOf(m_databases.values(), [&] (auto& database) {
        return database->hasDataInMemory();
    });
}

void IDBStorageManager::closeDatabasesForDeletion()
{
    for (auto& database : m_databases.values())
        database->immediateClose();

    m_databases.clear();
}

void IDBStorageManager::stopDatabaseActivitiesForSuspend()
{
    if (m_path.isEmpty())
        return;

    for (auto& database : m_databases.values()) {
        // Only stop databases with non-ephemeral backing store that can hold database file lock.
        if (!database->identifier().isTransient())
            database->abortActiveTransactions();
    }
}

CyberCore::IDBServer::UniqueIDBDatabase& IDBStorageManager::getOrCreateUniqueIDBDatabase(const CyberCore::IDBDatabaseIdentifier& identifier)
{
    auto addResult = m_databases.add(identifier, nullptr);
    if (addResult.isNewEntry)
        addResult.iterator->value = makeUnique<CyberCore::IDBServer::UniqueIDBDatabase>(*this, identifier);

    return *addResult.iterator->value;
}

void IDBStorageManager::openDatabase(CyberCore::IDBServer::IDBConnectionToClient& connectionToClient, const CyberCore::IDBRequestData& requestData)
{
    auto& database = getOrCreateUniqueIDBDatabase(requestData.databaseIdentifier());
    database.openDatabaseConnection(connectionToClient, requestData);
}

void IDBStorageManager::deleteDatabase(CyberCore::IDBServer::IDBConnectionToClient& connectionToClient, const CyberCore::IDBRequestData& requestData)
{
    auto& database = getOrCreateUniqueIDBDatabase(requestData.databaseIdentifier());
    database.handleDelete(connectionToClient, requestData);

    // This database is created for deletion.
    if (database.tryClose())
        m_databases.remove(database.identifier());
}

Vector<CyberCore::IDBDatabaseNameAndVersion> IDBStorageManager::getAllDatabaseNamesAndVersions()
{
    Vector<CyberCore::IDBDatabaseNameAndVersion> result;
    HashSet<String> visitedDatabasePaths;
    for (auto& database : m_databases.values()) {
        auto path = database->filePath();
        if (!path.isEmpty())
            visitedDatabasePaths.add(path);

        if (auto nameAndVersion = database->nameAndVersion())
            result.append(WTFMove(*nameAndVersion));
    }

    auto databaseIdentifiers = FileSystem::listDirectory(m_path);
    for (auto identifier : databaseIdentifiers) {
        auto databaseDirectory = FileSystem::pathByAppendingComponent(m_path, identifier);
        auto databasePath = CyberCore::IDBServer::SQLiteIDBBackingStore::fullDatabasePathForDirectory(databaseDirectory);
        if (visitedDatabasePaths.contains(databasePath))
            continue;

        if (auto nameAndVersion = CyberCore::IDBServer::SQLiteIDBBackingStore::databaseNameAndVersionFromFile(databasePath))
            result.append(WTFMove(*nameAndVersion));
    }

    return result;
}

void IDBStorageManager::openDBRequestCancelled(const CyberCore::IDBRequestData& requestData)
{
    auto* database = m_databases.get(requestData.databaseIdentifier());
    if (!database)
        return;

    database->openDBRequestCancelled(requestData.requestIdentifier());

    // Database becomes idle after request is cancelled.
    if (database->tryClose())
        m_databases.remove(database->identifier());
}

void IDBStorageManager::registerConnection(CyberCore::IDBServer::UniqueIDBDatabaseConnection& connection)
{
    m_registry.registerConnection(connection);
}

void IDBStorageManager::unregisterConnection(CyberCore::IDBServer::UniqueIDBDatabaseConnection& connection)
{
    m_registry.unregisterConnection(connection);
}

void IDBStorageManager::registerTransaction(CyberCore::IDBServer::UniqueIDBDatabaseTransaction& transaction)
{
    m_registry.registerTransaction(transaction);
}

void IDBStorageManager::unregisterTransaction(CyberCore::IDBServer::UniqueIDBDatabaseTransaction& transaction)
{
    m_registry.unregisterTransaction(transaction);
}

std::unique_ptr<CyberCore::IDBServer::IDBBackingStore> IDBStorageManager::createBackingStore(const CyberCore::IDBDatabaseIdentifier& identifier)
{
    if (m_path.isEmpty() || identifier.isTransient())
        return makeUnique<CyberCore::IDBServer::MemoryIDBBackingStore>(identifier);

    auto name = CyberCore::SQLiteFileSystem::computeHashForFileName(identifier.databaseName());
    return makeUnique<CyberCore::IDBServer::SQLiteIDBBackingStore>(identifier, FileSystem::pathByAppendingComponent(m_path, name));
}

void IDBStorageManager::requestSpace(const CyberCore::ClientOrigin&, uint64_t size, CompletionHandler<void(bool)>&& completionHandler)
{
    m_quotaCheckFunction(size, WTFMove(completionHandler));
}

void IDBStorageManager::handleLowMemoryWarning()
{
    for (auto& database : m_databases.values())
        database->handleLowMemoryWarning();
}

} // namespace CyberKit
