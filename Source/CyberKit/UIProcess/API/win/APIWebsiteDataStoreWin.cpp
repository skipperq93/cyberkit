/*
 * Copyright (C) 2017 Sony Interactive Entertainment Inc.
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
#include "APIWebsiteDataStore.h"

#include <CyberCore/FileSystem.h>

namespace API {

String WebsiteDataStore::defaultApplicationCacheDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "ApplicationCache");
}

String WebsiteDataStore::defaultCacheStorageDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "CacheStorage");
}

String WebsiteDataStore::defaultNetworkCacheDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "NetworkCache");
}

String WebsiteDataStore::defaultIndexedDBDatabaseDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "IndexedDB");
}

String WebsiteDataStore::defaultServiceWorkerRegistrationDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "ServiceWorkers");
}

String WebsiteDataStore::defaultLocalStorageDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "LocalStorage");
}

String WebsiteDataStore::defaultMediaKeysStorageDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "MediaKeyStorage");
}

String WebsiteDataStore::defaultDeviceIdHashSaltsStorageDirectory()
{
    // Not Implemented.
    return String();
}

String WebsiteDataStore::defaultWebSQLDatabaseDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "WebSQL");
}

String WebsiteDataStore::defaultResourceLoadStatisticsDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "ResourceLoadStatistics");
}

String WebsiteDataStore::cacheDirectoryFileSystemRepresentation(const String& directoryName)
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), directoryName);
}

String WebsiteDataStore::websiteDataDirectoryFileSystemRepresentation(const String& directoryName)
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), directoryName);
}

String WebsiteDataStore::legacyDefaultApplicationCacheDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "ApplicationCache");
}

String WebsiteDataStore::legacyDefaultNetworkCacheDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "NetworkCache");
}

String WebsiteDataStore::legacyDefaultWebSQLDatabaseDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "WebSQL");
}

String WebsiteDataStore::legacyDefaultIndexedDBDatabaseDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "IndexedDB");
}

String WebsiteDataStore::legacyDefaultLocalStorageDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "LocalStorage");
}

String WebsiteDataStore::legacyDefaultMediaCacheDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "MediaCache");
}

String WebsiteDataStore::legacyDefaultMediaKeysStorageDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "MediaKeyStorage");
}

String WebsiteDataStore::legacyDefaultDeviceIdHashSaltsStorageDirectory()
{
    // Not Implemented.
    return String();
}

String WebsiteDataStore::legacyDefaultJavaScriptConfigurationDirectory()
{
    return CyberCore::FileSystem::pathByAppendingComponent(CyberCore::FileSystem::localUserSpecificStorageDirectory(), "CyberScriptCoreDebug");
}

Ref<CyberKit::WebsiteDataStoreConfiguration> WebsiteDataStore::defaultDataStoreConfiguration()
{
    auto configuration = CyberKit::WebsiteDataStoreConfiguration::create();

    configuration->setApplicationCacheDirectory(defaultApplicationCacheDirectory());
    configuration->setNetworkCacheDirectory(defaultNetworkCacheDirectory());
    configuration->setWebSQLDatabaseDirectory(defaultWebSQLDatabaseDirectory());
    configuration->setLocalStorageDirectory(defaultLocalStorageDirectory());
    configuration->setMediaKeysStorageDirectory(defaultMediaKeysStorageDirectory());
    configuration->setResourceLoadStatisticsDirectory(defaultResourceLoadStatisticsDirectory());

    return configuration;
}

} // namespace API
