/*
 * Copyright (C) 2021 Apple Inc. All rights reserved.
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
#include "FileSystemStorageError.h"
#include "FileSystemSyncAccessHandleInfo.h"
#include "OriginStorageManager.h"
#include "StorageAreaIdentifier.h"
#include "StorageAreaImplIdentifier.h"
#include "StorageAreaMapIdentifier.h"
#include "StorageNamespaceIdentifier.h"
#include "WebPageProxyIdentifier.h"
#include "WebsiteData.h"
#include "WorkQueueMessageReceiver.h"
#include <CyberCore/ClientOrigin.h>
#include <CyberCore/DOMCacheEngine.h>
#include <CyberCore/FileSystemHandleIdentifier.h>
#include <CyberCore/FileSystemSyncAccessHandleIdentifier.h>
#include <CyberCore/IDBResourceIdentifier.h>
#include <CyberCore/IndexedDB.h>
#include <CyberCore/ServiceWorkerTypes.h>
#include <pal/SessionID.h>
#include <wtf/Forward.h>
#include <wtf/ThreadSafeWeakHashSet.h>

namespace IPC {
class SharedFileHandle;
}

namespace CyberCore {
class IDBCursorInfo;
class IDBKeyData;
class IDBIndexInfo;
class IDBObjectStoreInfo;
class IDBRequestData;
class IDBTransactionInfo;
class IDBValue;
class ServiceWorkerRegistrationKey;
struct ClientOrigin;
struct IDBGetAllRecordsData;
struct IDBGetRecordData;
struct IDBGetAllRecordsData;
struct IDBIterateCursorData;
struct IDBKeyRangeData;
struct RetrieveRecordsOptions;
struct ServiceWorkerContextData;
enum class StorageType : uint8_t;
}

namespace CyberKit {

enum class BackgroundFetchChange : uint8_t;
enum class UnifiedOriginStorageLevel : uint8_t;
class FileSystemStorageHandleRegistry;
class IDBStorageRegistry;
class NetworkProcess;
class ServiceWorkerStorageManager;
class StorageAreaBase;
class StorageAreaRegistry;

class NetworkStorageManager final : public IPC::WorkQueueMessageReceiver {
public:
    static Ref<NetworkStorageManager> create(NetworkProcess&, PAL::SessionID, Markable<UUID>, IPC::Connection::UniqueID, const String& path, const String& customLocalStoragePath, const String& customIDBStoragePath, const String& customCacheStoragePath, const String& customServiceWorkerStoragePath, uint64_t defaultOriginQuota, std::optional<double> originQuotaRatio, std::optional<double> totalQuotaRatio, std::optional<uint64_t> volumeCapacityOverride, UnifiedOriginStorageLevel);
    static bool canHandleTypes(OptionSet<WebsiteDataType>);
    static OptionSet<WebsiteDataType> allManagedTypes();

    void startReceivingMessageFromConnection(IPC::Connection&);
    void stopReceivingMessageFromConnection(IPC::Connection&);

    PAL::SessionID sessionID() const { return m_sessionID; }
    void close(CompletionHandler<void()>&&);
    void resetStoragePersistedState(CompletionHandler<void()>&&);
    void clearStorageForWebPage(WebPageProxyIdentifier);
    void cloneSessionStorageForWebPage(WebPageProxyIdentifier, WebPageProxyIdentifier);
    void didIncreaseQuota(CyberCore::ClientOrigin&&, QuotaIncreaseRequestIdentifier, std::optional<uint64_t> newQuota);
    enum class ShouldComputeSize : bool { No, Yes };
    void fetchData(OptionSet<WebsiteDataType>, ShouldComputeSize, CompletionHandler<void(Vector<WebsiteData::Entry>&&)>&&);
    void deleteData(OptionSet<WebsiteDataType>, const Vector<CyberCore::SecurityOriginData>&, CompletionHandler<void()>&&);
    void deleteData(OptionSet<WebsiteDataType>, const CyberCore::ClientOrigin&, CompletionHandler<void()>&&);
    void deleteDataModifiedSince(OptionSet<WebsiteDataType>, WallTime, CompletionHandler<void()>&&);
    void deleteDataForRegistrableDomains(OptionSet<WebsiteDataType>, const Vector<CyberCore::RegistrableDomain>&, CompletionHandler<void(HashSet<CyberCore::RegistrableDomain>&&)>&&);
    void moveData(OptionSet<WebsiteDataType>, CyberCore::SecurityOriginData&& source, CyberCore::SecurityOriginData&& target, CompletionHandler<void()>&&);
    void getOriginDirectory(CyberCore::ClientOrigin&&, WebsiteDataType, CompletionHandler<void(const String&)>&&);
    void suspend(CompletionHandler<void()>&&);
    void resume();
    void handleLowMemoryWarning();
    void syncLocalStorage(CompletionHandler<void()>&&);
    void registerTemporaryBlobFilePaths(IPC::Connection&, const Vector<String>&);
    void requestSpace(const CyberCore::ClientOrigin&, uint64_t size, CompletionHandler<void(bool)>&&);
    void resetQuotaForTesting(CompletionHandler<void()>&&);
    void resetQuotaUpdatedBasedOnUsageForTesting(CyberCore::ClientOrigin&&);
#if PLATFORM(IOS_FAMILY)
    void setBackupExclusionPeriodForTesting(Seconds, CompletionHandler<void()>&&);
#endif

#if ENABLE(SERVICE_WORKER)
    void dispatchTaskToBackgroundFetchManager(const CyberCore::ClientOrigin&, Function<void(BackgroundFetchStoreManager*)>&&);
    void notifyBackgroundFetchChange(const String&, BackgroundFetchChange);
    void closeServiceWorkerRegistrationFiles(CompletionHandler<void()>&&);
    void clearServiceWorkerRegistrations(CompletionHandler<void()>&&);
    void importServiceWorkerRegistrations(CompletionHandler<void(std::optional<Vector<CyberCore::ServiceWorkerContextData>>)>&&);
    void updateServiceWorkerRegistrations(Vector<CyberCore::ServiceWorkerContextData>&&, Vector<CyberCore::ServiceWorkerRegistrationKey>&&, CompletionHandler<void(std::optional<Vector<CyberCore::ServiceWorkerScripts>>)>&&);
#endif // ENABLE(SERVICE_WORKER)

private:
    NetworkStorageManager(NetworkProcess&, PAL::SessionID, Markable<UUID>, IPC::Connection::UniqueID, const String& path, const String& customLocalStoragePath, const String& customIDBStoragePath, const String& customCacheStoragePath, const String& customServiceWorkerStoragePath, uint64_t defaultOriginQuota, std::optional<double> originQuotaRatio, std::optional<double> totalQuotaRatio, std::optional<uint64_t> volumeCapacityOverride, UnifiedOriginStorageLevel);
    ~NetworkStorageManager();
    void writeOriginToFileIfNecessary(const CyberCore::ClientOrigin&, StorageAreaBase* = nullptr);
    enum class ShouldWriteOriginFile : bool { No, Yes };
    OriginStorageManager& originStorageManager(const CyberCore::ClientOrigin&, ShouldWriteOriginFile = ShouldWriteOriginFile::Yes);
    bool removeOriginStorageManagerIfPossible(const CyberCore::ClientOrigin&);

    void forEachOriginDirectory(const Function<void(const String&)>&);
    HashSet<CyberCore::ClientOrigin> getAllOrigins();
    Vector<WebsiteData::Entry> fetchDataFromDisk(OptionSet<WebsiteDataType>, ShouldComputeSize);
    HashSet<CyberCore::ClientOrigin> deleteDataOnDisk(OptionSet<WebsiteDataType>, WallTime, const Function<bool(const CyberCore::ClientOrigin&)>&);
    bool evictDataByTopOrigin(const CyberCore::SecurityOriginData&);
#if PLATFORM(IOS_FAMILY)
    void includeOriginInBackupIfNecessary(OriginStorageManager&);
#endif

    // IPC::MessageReceiver (implemented by generated code)
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&);
    bool didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, UniqueRef<IPC::Encoder>& replyEncoder);

    // Message handlers for FileSystem.
    void persisted(const CyberCore::ClientOrigin&, CompletionHandler<void(bool)>&&);
    void persist(const CyberCore::ClientOrigin&, CompletionHandler<void(bool)>&&);
    void estimate(const CyberCore::ClientOrigin&, CompletionHandler<void(std::optional<CyberCore::StorageEstimate>)>&&);
    void fileSystemGetDirectory(IPC::Connection&, CyberCore::ClientOrigin&&, CompletionHandler<void(Expected<CyberCore::FileSystemHandleIdentifier, FileSystemStorageError>)>&&);
    void closeHandle(CyberCore::FileSystemHandleIdentifier);
    void isSameEntry(CyberCore::FileSystemHandleIdentifier, CyberCore::FileSystemHandleIdentifier, CompletionHandler<void(bool)>&&);
    void move(CyberCore::FileSystemHandleIdentifier, CyberCore::FileSystemHandleIdentifier, const String& newName, CompletionHandler<void(std::optional<FileSystemStorageError>)>&&);
    void getFileHandle(IPC::Connection&, CyberCore::FileSystemHandleIdentifier, String&& name, bool createIfNecessary, CompletionHandler<void(Expected<CyberCore::FileSystemHandleIdentifier, FileSystemStorageError>)>&&);
    void getDirectoryHandle(IPC::Connection&, CyberCore::FileSystemHandleIdentifier, String&& name, bool createIfNecessary, CompletionHandler<void(Expected<CyberCore::FileSystemHandleIdentifier, FileSystemStorageError>)>&&);
    void removeEntry(CyberCore::FileSystemHandleIdentifier, const String& name, bool deleteRecursively, CompletionHandler<void(std::optional<FileSystemStorageError>)>&&);
    void resolve(CyberCore::FileSystemHandleIdentifier, CyberCore::FileSystemHandleIdentifier, CompletionHandler<void(Expected<Vector<String>, FileSystemStorageError>)>&&);
    void getFile(CyberCore::FileSystemHandleIdentifier, CompletionHandler<void(Expected<String, FileSystemStorageError>)>&&);
    void createSyncAccessHandle(CyberCore::FileSystemHandleIdentifier, CompletionHandler<void(Expected<FileSystemSyncAccessHandleInfo, FileSystemStorageError>)>&&);
    void closeSyncAccessHandle(CyberCore::FileSystemHandleIdentifier, CyberCore::FileSystemSyncAccessHandleIdentifier, CompletionHandler<void()>&&);
    void requestNewCapacityForSyncAccessHandle(CyberCore::FileSystemHandleIdentifier, CyberCore::FileSystemSyncAccessHandleIdentifier, uint64_t newCapacity, CompletionHandler<void(std::optional<uint64_t>)>&&);
    void getHandleNames(CyberCore::FileSystemHandleIdentifier, CompletionHandler<void(Expected<Vector<String>, FileSystemStorageError>)>&&);
    void getHandle(IPC::Connection&, CyberCore::FileSystemHandleIdentifier, String&& name, CompletionHandler<void(Expected<std::pair<CyberCore::FileSystemHandleIdentifier, bool>, FileSystemStorageError>)>&&);
    
    // Message handlers for WebStorage.
    void connectToStorageArea(IPC::Connection&, CyberCore::StorageType, StorageAreaMapIdentifier, std::optional<StorageNamespaceIdentifier>, const CyberCore::ClientOrigin&, CompletionHandler<void(StorageAreaIdentifier, HashMap<String, String>, uint64_t)>&&);
    void connectToStorageAreaSync(IPC::Connection&, CyberCore::StorageType, StorageAreaMapIdentifier, std::optional<StorageNamespaceIdentifier>, const CyberCore::ClientOrigin&, CompletionHandler<void(StorageAreaIdentifier, HashMap<String, String>, uint64_t)>&&);
    void cancelConnectToStorageArea(IPC::Connection&, CyberCore::StorageType, std::optional<StorageNamespaceIdentifier>, const CyberCore::ClientOrigin&);
    void disconnectFromStorageArea(IPC::Connection&, StorageAreaIdentifier);
    void cloneSessionStorageNamespace(StorageNamespaceIdentifier, StorageNamespaceIdentifier);
    void setItem(IPC::Connection&, StorageAreaIdentifier, StorageAreaImplIdentifier, String&& key, String&& value, String&& urlString, CompletionHandler<void(bool, HashMap<String, String>&&)>&&);
    void removeItem(IPC::Connection&, StorageAreaIdentifier, StorageAreaImplIdentifier, String&& key, String&& urlString, CompletionHandler<void()>&&);
    void clear(IPC::Connection&, StorageAreaIdentifier, StorageAreaImplIdentifier, String&& urlString, CompletionHandler<void()>&&);

    // Message handlers for IndexedDB.
    void openDatabase(IPC::Connection&, const CyberCore::IDBRequestData&);
    void openDBRequestCancelled(const CyberCore::IDBRequestData&);
    void deleteDatabase(IPC::Connection&, const CyberCore::IDBRequestData&);
    void establishTransaction(uint64_t databaseConnectionIdentifier, const CyberCore::IDBTransactionInfo&);
    void databaseConnectionPendingClose(uint64_t databaseConnectionIdentifier);
    void databaseConnectionClosed(uint64_t databaseConnectionIdentifier);
    void abortOpenAndUpgradeNeeded(uint64_t databaseConnectionIdentifier, const std::optional<CyberCore::IDBResourceIdentifier>& transactionIdentifier);
    void didFireVersionChangeEvent(uint64_t databaseConnectionIdentifier, const CyberCore::IDBResourceIdentifier& requestIdentifier, const CyberCore::IndexedDB::ConnectionClosedOnBehalfOfServer);
    void abortTransaction(const CyberCore::IDBResourceIdentifier&);
    void commitTransaction(const CyberCore::IDBResourceIdentifier&, uint64_t pendingRequestCount);
    void didFinishHandlingVersionChangeTransaction(uint64_t databaseConnectionIdentifier, const CyberCore::IDBResourceIdentifier&);
    void createObjectStore(const CyberCore::IDBRequestData&, const CyberCore::IDBObjectStoreInfo&);
    void deleteObjectStore(const CyberCore::IDBRequestData&, const String& objectStoreName);
    void renameObjectStore(const CyberCore::IDBRequestData&, uint64_t objectStoreIdentifier, const String& newName);
    void clearObjectStore(const CyberCore::IDBRequestData&, uint64_t objectStoreIdentifier);
    void createIndex(const CyberCore::IDBRequestData&, const CyberCore::IDBIndexInfo&);
    void deleteIndex(const CyberCore::IDBRequestData&, uint64_t objectStoreIdentifier, const String& indexName);
    void renameIndex(const CyberCore::IDBRequestData&, uint64_t objectStoreIdentifier, uint64_t indexIdentifier, const String& newName);
    void putOrAdd(IPC::Connection&, const CyberCore::IDBRequestData&, const CyberCore::IDBKeyData&, const CyberCore::IDBValue&, CyberCore::IndexedDB::ObjectStoreOverwriteMode);
    void getRecord(const CyberCore::IDBRequestData&, const CyberCore::IDBGetRecordData&);
    void getAllRecords(const CyberCore::IDBRequestData&, const CyberCore::IDBGetAllRecordsData&);
    void getCount(const CyberCore::IDBRequestData&, const CyberCore::IDBKeyRangeData&);
    void deleteRecord(const CyberCore::IDBRequestData&, const CyberCore::IDBKeyRangeData&);
    void openCursor(const CyberCore::IDBRequestData&, const CyberCore::IDBCursorInfo&);
    void iterateCursor(const CyberCore::IDBRequestData&, const CyberCore::IDBIterateCursorData&);
    void getAllDatabaseNamesAndVersions(IPC::Connection&, const CyberCore::IDBResourceIdentifier&, const CyberCore::ClientOrigin&);

    // Message handlers for CacheStorage.
    void cacheStorageOpenCache(const CyberCore::ClientOrigin&, const String& cacheName, CyberCore::DOMCacheEngine::CacheIdentifierCallback&&);
    void cacheStorageRemoveCache(CyberCore::DOMCacheIdentifier, CyberCore::DOMCacheEngine::RemoveCacheIdentifierCallback&&);
    void cacheStorageAllCaches(const CyberCore::ClientOrigin&, uint64_t updateCounter, CyberCore::DOMCacheEngine::CacheInfosCallback&&);
    void cacheStorageReference(IPC::Connection&, CyberCore::DOMCacheIdentifier);
    void cacheStorageDereference(IPC::Connection&, CyberCore::DOMCacheIdentifier);
    void cacheStorageRetrieveRecords(CyberCore::DOMCacheIdentifier, CyberCore::RetrieveRecordsOptions&&, CyberCore::DOMCacheEngine::CrossThreadRecordsCallback&&);
    void cacheStorageRemoveRecords(CyberCore::DOMCacheIdentifier, CyberCore::ResourceRequest&&, CyberCore::CacheQueryOptions&&, CyberCore::DOMCacheEngine::RecordIdentifiersCallback&&);
    void cacheStoragePutRecords(CyberCore::DOMCacheIdentifier, Vector<CyberCore::DOMCacheEngine::CrossThreadRecord>&&, CyberCore::DOMCacheEngine::RecordIdentifiersCallback&&);
    void cacheStorageClearMemoryRepresentation(const CyberCore::ClientOrigin&, CompletionHandler<void(std::optional<CyberCore::DOMCacheEngine::Error>&&)>&&);
    void cacheStorageRepresentation(CompletionHandler<void(String&&)>&&);

#if ENABLE(SERVICE_WORKER)
    bool shouldManageServiceWorkerRegistrationsByOrigin();
    void migrateServiceWorkerRegistrationsToOrigins();
    Vector<CyberCore::ServiceWorkerScripts> updateServiceWorkerRegistrationsByOrigin(Vector<CyberCore::ServiceWorkerContextData>&&, Vector<CyberCore::ServiceWorkerRegistrationKey>&&);
#endif

    void spaceGrantedForOrigin(const CyberCore::ClientOrigin&, uint64_t amount);
    void prepareForEviction();
    void donePrepareForEviction(const std::optional<HashMap<CyberCore::RegistrableDomain, WallTime>>&);
    WallTime lastModificationTimeForOrigin(const CyberCore::ClientOrigin&, OriginStorageManager&) const;
    void updateLastModificationTimeForOrigin(const CyberCore::ClientOrigin&);
    void schedulePerformEviction();
    struct AccessRecord {
        bool isActive { false };
        uint64_t usage { 0 };
        WallTime lastAccessTime;
        Vector<CyberCore::SecurityOriginData> clientOrigins;
    };
    void performEviction(HashMap<CyberCore::SecurityOriginData, AccessRecord>&&);
    SuspendableWorkQueue& workQueue() WTF_RETURNS_CAPABILITY(m_queue.get()) { return m_queue; }

    WeakPtr<NetworkProcess> m_process;
    PAL::SessionID m_sessionID;
    String m_queueName;
    Ref<SuspendableWorkQueue> m_queue;
    String m_path;
    FileSystem::Salt m_salt;
    bool m_closed { false };
    HashMap<CyberCore::ClientOrigin, std::unique_ptr<OriginStorageManager>> m_originStorageManagers WTF_GUARDED_BY_CAPABILITY(workQueue());
    ThreadSafeWeakHashSet<IPC::Connection> m_connections;
    std::unique_ptr<FileSystemStorageHandleRegistry> m_fileSystemStorageHandleRegistry;
    std::unique_ptr<StorageAreaRegistry> m_storageAreaRegistry;
    std::unique_ptr<IDBStorageRegistry> m_idbStorageRegistry;
    std::unique_ptr<CacheStorageRegistry> m_cacheStorageRegistry;
    String m_customLocalStoragePath;
    String m_customIDBStoragePath;
    String m_customCacheStoragePath;
    String m_customServiceWorkerStoragePath;
    uint64_t m_defaultOriginQuota;
    std::optional<double> m_originQuotaRatio;
    std::optional<double> m_totalQuotaRatio;
    std::optional<uint64_t> m_volumeCapacityOverride;
    std::optional<uint64_t> m_totalUsage;
    uint64_t m_totalQuota;
    bool m_isEvictionScheduled { false };
    UnifiedOriginStorageLevel m_unifiedOriginStorageLevel;
    IPC::Connection::UniqueID m_parentConnection;
    HashMap<IPC::Connection::UniqueID, HashSet<String>> m_temporaryBlobPathsByConnection WTF_GUARDED_BY_CAPABILITY(workQueue());
#if PLATFORM(IOS_FAMILY)
    Seconds m_backupExclusionPeriod;
#endif
#if ENABLE(SERVICE_WORKER)
    std::unique_ptr<ServiceWorkerStorageManager> m_sharedServiceWorkerStorageManager;
#endif
};

} // namespace CyberKit
