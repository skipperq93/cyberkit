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

#pragma once

#include <CyberCore/UniqueIDBDatabase.h>
#include <CyberCore/UniqueIDBDatabaseManager.h>

namespace CyberCore {
class IDBRequestData;

namespace IDBServer {
class IDBBackingStore;
class IDBConnectionToClient;
class UniqueIDBTransaction;
}

struct IDBDatabaseNameAndVersion;
}

namespace CyberKit {

class IDBStorageRegistry;

class IDBStorageManager final : public CyberCore::IDBServer::UniqueIDBDatabaseManager {
public:
    static String idbStorageOriginDirectory(const String& rootDirectory, const CyberCore::ClientOrigin&);
    static uint64_t idbStorageSize(const String& originDirectory);
    static HashSet<CyberCore::ClientOrigin> originsOfIDBStorageData(const String& rootDirectory);
    static bool migrateOriginData(const String& oldOriginDirectory, const String& newOriginDirectory);

    using QuotaCheckFunction = Function<void(uint64_t spaceRequested, CompletionHandler<void(bool)>&&)>;
    IDBStorageManager(const String& path, IDBStorageRegistry&, QuotaCheckFunction&&);
    ~IDBStorageManager();
    bool isActive() const;
    bool hasDataInMemory() const;
    void closeDatabasesForDeletion();
    void stopDatabaseActivitiesForSuspend();
    void handleLowMemoryWarning();

    void openDatabase(CyberCore::IDBServer::IDBConnectionToClient&, const CyberCore::IDBRequestData&);
    void openDBRequestCancelled(const CyberCore::IDBRequestData&);
    void deleteDatabase(CyberCore::IDBServer::IDBConnectionToClient&, const CyberCore::IDBRequestData&);
    Vector<CyberCore::IDBDatabaseNameAndVersion> getAllDatabaseNamesAndVersions();

private:
    CyberCore::IDBServer::UniqueIDBDatabase& getOrCreateUniqueIDBDatabase(const CyberCore::IDBDatabaseIdentifier&);

    // CyberCore::UniqueIDBDatabaseManager
    void registerConnection(CyberCore::IDBServer::UniqueIDBDatabaseConnection&) final;
    void unregisterConnection(CyberCore::IDBServer::UniqueIDBDatabaseConnection&) final;
    void registerTransaction(CyberCore::IDBServer::UniqueIDBDatabaseTransaction&) final;
    void unregisterTransaction(CyberCore::IDBServer::UniqueIDBDatabaseTransaction&) final;
    std::unique_ptr<CyberCore::IDBServer::IDBBackingStore> createBackingStore(const CyberCore::IDBDatabaseIdentifier&) final;
    void requestSpace(const CyberCore::ClientOrigin&, uint64_t size, CompletionHandler<void(bool)>&&) final;

    String m_path;
    IDBStorageRegistry& m_registry;
    QuotaCheckFunction m_quotaCheckFunction;
    HashMap<CyberCore::IDBDatabaseIdentifier, std::unique_ptr<CyberCore::IDBServer::UniqueIDBDatabase>> m_databases;
};

} // namespace CyberKit
