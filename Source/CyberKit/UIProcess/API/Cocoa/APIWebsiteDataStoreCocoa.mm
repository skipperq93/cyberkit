/*
 * Copyright (C) 2014, 2016 Apple Inc. All rights reserved.
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

#include "SandboxExtension.h"
#include "SandboxUtilities.h"

#include <Foundation/Foundation.h>
#include <CyberCore/FileSystem.h>

#if PLATFORM(IOS_FAMILY)
#import <CyberCore/RuntimeApplicationChecks.h>
#endif

namespace API {

NSString *WebDatabaseDirectoryDefaultsKey = @"WebDatabaseDirectory";
NSString *WebStorageDirectoryDefaultsKey = @"CyberKitLocalStorageDatabasePathPreferenceKey";
NSString *CyberKitMediaCacheDirectoryDefaultsKey = @"CyberKitMediaCacheDirectory";
NSString *CyberKitMediaKeysStorageDirectoryDefaultsKey = @"CyberKitMediaKeysStorageDirectory";

WTF::String WebsiteDataStore::defaultApplicationCacheDirectory()
{
#if PLATFORM(IOS_FAMILY)
    // This quirk used to make these apps share application cache storage, but doesn't accomplish that any more.
    // Preserving it avoids the need to migrate data when upgrading.
    // FIXME: Ideally we should just have Safari, WebApp, and webbookmarksd create a data store with
    // this application cache path, but that's not supported as of right now.
    if (CyberCore::IOSApplication::isMobileSafari() || CyberCore::IOSApplication::isWebApp() || CyberCore::IOSApplication::isWebBookmarksD()) {
        NSString *cachePath = [NSHomeDirectory() stringByAppendingPathComponent:@"Library/Caches/com.apple.WebAppCache"];

        return CyberKit::stringByResolvingSymlinksInPath(cachePath.stringByStandardizingPath);
    }
#endif

    return cacheDirectoryFileSystemRepresentation("OfflineWebApplicationCache");
}

WTF::String WebsiteDataStore::defaultCacheStorageDirectory()
{
    return cacheDirectoryFileSystemRepresentation("CacheStorage");
}

WTF::String WebsiteDataStore::defaultNetworkCacheDirectory()
{
    return cacheDirectoryFileSystemRepresentation("NetworkCache");
}

WTF::String WebsiteDataStore::defaultMediaCacheDirectory()
{
    return tempDirectoryFileSystemRepresentation("MediaCache");
}

WTF::String WebsiteDataStore::defaultIndexedDBDatabaseDirectory()
{
    return websiteDataDirectoryFileSystemRepresentation("IndexedDB");
}

WTF::String WebsiteDataStore::defaultServiceWorkerRegistrationDirectory()
{
    return cacheDirectoryFileSystemRepresentation("ServiceWorkers");
}

WTF::String WebsiteDataStore::defaultLocalStorageDirectory()
{
    return websiteDataDirectoryFileSystemRepresentation("LocalStorage");
}

WTF::String WebsiteDataStore::defaultMediaKeysStorageDirectory()
{
    return websiteDataDirectoryFileSystemRepresentation("MediaKeys");
}

WTF::String WebsiteDataStore::defaultDeviceIdHashSaltsStorageDirectory()
{
    // Not implemented.
    return String();
}

WTF::String WebsiteDataStore::defaultWebSQLDatabaseDirectory()
{
    return websiteDataDirectoryFileSystemRepresentation("WebSQL");
}

WTF::String WebsiteDataStore::defaultResourceLoadStatisticsDirectory()
{
    return websiteDataDirectoryFileSystemRepresentation("ResourceLoadStatistics");
}

WTF::String WebsiteDataStore::defaultJavaScriptConfigurationDirectory()
{
    return tempDirectoryFileSystemRepresentation("CyberScriptCoreDebug", DontCreateDirectory);
}

WTF::String WebsiteDataStore::legacyDefaultApplicationCacheDirectory()
{
    NSString *appName = [[NSBundle mainBundle] bundleIdentifier];
    if (!appName)
        appName = [[NSProcessInfo processInfo] processName];
#if PLATFORM(IOS_FAMILY)
    // This quirk used to make these apps share application cache storage, but doesn't accomplish that any more.
    // Preserving it avoids the need to migrate data when upgrading.
    if (CyberCore::IOSApplication::isMobileSafari() || CyberCore::IOSApplication::isWebApp())
        appName = @"com.apple.WebAppCache";
#endif
    
    ASSERT(appName);
    
#if PLATFORM(IOS_FAMILY)
    NSString *cacheDir = [NSHomeDirectory() stringByAppendingPathComponent:@"Library/Caches"];
#else
    char cacheDirectory[MAXPATHLEN];
    size_t cacheDirectoryLen = confstr(_CS_DARWIN_USER_CACHE_DIR, cacheDirectory, MAXPATHLEN);
    if (!cacheDirectoryLen)
        return WTF::String();
    
    NSString *cacheDir = [[NSFileManager defaultManager] stringWithFileSystemRepresentation:cacheDirectory length:cacheDirectoryLen - 1];
#endif
    NSString* cachePath = [cacheDir stringByAppendingPathComponent:appName];
    return CyberKit::stringByResolvingSymlinksInPath([cachePath stringByStandardizingPath]);
}

WTF::String WebsiteDataStore::legacyDefaultNetworkCacheDirectory()
{
    NSString *cachePath = CFBridgingRelease(_CFURLCacheCopyCacheDirectory([[NSURLCache sharedURLCache] _CFURLCache]));
    if (!cachePath)
        cachePath = @"~/Library/Caches/com.matthewbenedict.CyberKit.WebProcess";
    
    cachePath = [cachePath stringByAppendingPathComponent:@"CyberKitCache"];
    
    return CyberKit::stringByResolvingSymlinksInPath([cachePath stringByStandardizingPath]);
}

WTF::String WebsiteDataStore::legacyDefaultWebSQLDatabaseDirectory()
{
    NSString *databasesDirectory = [[NSUserDefaults standardUserDefaults] objectForKey:WebDatabaseDirectoryDefaultsKey];
    if (!databasesDirectory || ![databasesDirectory isKindOfClass:[NSString class]])
        databasesDirectory = @"~/Library/CyberKit/Databases";
    return CyberKit::stringByResolvingSymlinksInPath([databasesDirectory stringByStandardizingPath]);
}

WTF::String WebsiteDataStore::legacyDefaultIndexedDBDatabaseDirectory()
{
    // Indexed databases exist in a subdirectory of the "database directory path."
    // Currently, the top level of that directory contains entities related to WebSQL databases.
    // We should fix this, and move WebSQL into a subdirectory (https://bugs.webkit.org/show_bug.cgi?id=124807)
    // In the meantime, an entity name prefixed with three underscores will not conflict with any WebSQL entities.
    return CyberCore::FileSystem::pathByAppendingComponent(legacyDefaultWebSQLDatabaseDirectory(), "___IndexedDB");
}

WTF::String WebsiteDataStore::legacyDefaultLocalStorageDirectory()
{
    NSString *localStorageDirectory = [[NSUserDefaults standardUserDefaults] objectForKey:WebStorageDirectoryDefaultsKey];
    if (!localStorageDirectory || ![localStorageDirectory isKindOfClass:[NSString class]])
        localStorageDirectory = @"~/Library/CyberKit/LocalStorage";
    return CyberKit::stringByResolvingSymlinksInPath([localStorageDirectory stringByStandardizingPath]);
}

WTF::String WebsiteDataStore::legacyDefaultMediaCacheDirectory()
{
    NSString *mediaKeysCacheDirectory = [[NSUserDefaults standardUserDefaults] objectForKey:CyberKitMediaCacheDirectoryDefaultsKey];
    if (!mediaKeysCacheDirectory || ![mediaKeysCacheDirectory isKindOfClass:[NSString class]]) {
        mediaKeysCacheDirectory = NSTemporaryDirectory();
        
        if (!CyberKit::processHasContainer()) {
            NSString *bundleIdentifier = [NSBundle mainBundle].bundleIdentifier;
            if (!bundleIdentifier)
                bundleIdentifier = [NSProcessInfo processInfo].processName;
            mediaKeysCacheDirectory = [mediaKeysCacheDirectory stringByAppendingPathComponent:bundleIdentifier];
        }
        mediaKeysCacheDirectory = [mediaKeysCacheDirectory stringByAppendingPathComponent:@"CyberKit/MediaCache"];
    }
    return CyberKit::stringByResolvingSymlinksInPath([mediaKeysCacheDirectory stringByStandardizingPath]);
}

WTF::String WebsiteDataStore::legacyDefaultMediaKeysStorageDirectory()
{
    NSString *mediaKeysStorageDirectory = [[NSUserDefaults standardUserDefaults] objectForKey:CyberKitMediaKeysStorageDirectoryDefaultsKey];
    if (!mediaKeysStorageDirectory || ![mediaKeysStorageDirectory isKindOfClass:[NSString class]])
        mediaKeysStorageDirectory = @"~/Library/CyberKit/MediaKeys";
    return CyberKit::stringByResolvingSymlinksInPath([mediaKeysStorageDirectory stringByStandardizingPath]);
}

WTF::String WebsiteDataStore::legacyDefaultDeviceIdHashSaltsStorageDirectory()
{
    // Not implemented.
    return String();
}

WTF::String WebsiteDataStore::legacyDefaultJavaScriptConfigurationDirectory()
{
#if PLATFORM(IOS_FAMILY)
    WTF::String path = CyberKit::pathForProcessContainer();
    if (path.isEmpty())
        path = NSHomeDirectory();
    
    path = path + "/Library/CyberKit/CyberScriptCoreDebug";
    path = CyberKit::stringByResolvingSymlinksInPath(path);
    
    return path;
#else
    RetainPtr<NSString> javaScriptConfigPath = @"~/Library/CyberKit/CyberScriptCoreDebug";
    
    return CyberKit::stringByResolvingSymlinksInPath([javaScriptConfigPath stringByStandardizingPath]);
#endif
}

WTF::String WebsiteDataStore::tempDirectoryFileSystemRepresentation(const WTF::String& directoryName, ShouldCreateDirectory shouldCreateDirectory)
{
    static dispatch_once_t onceToken;
    static NSURL *tempURL;
    
    dispatch_once(&onceToken, ^{
        NSURL *url = [NSURL fileURLWithPath:NSTemporaryDirectory() isDirectory:YES];
        if (!url)
            RELEASE_ASSERT_NOT_REACHED();
        
        if (!CyberKit::processHasContainer()) {
            NSString *bundleIdentifier = [NSBundle mainBundle].bundleIdentifier;
            if (!bundleIdentifier)
                bundleIdentifier = [NSProcessInfo processInfo].processName;
            url = [url URLByAppendingPathComponent:bundleIdentifier isDirectory:YES];
        }
        
        tempURL = [[url URLByAppendingPathComponent:@"CyberKit" isDirectory:YES] retain];
    });
    
    NSURL *url = [tempURL URLByAppendingPathComponent:directoryName isDirectory:YES];

    if (shouldCreateDirectory == CreateDirectory
        && (![[NSFileManager defaultManager] createDirectoryAtURL:url withIntermediateDirectories:YES attributes:nil error:nullptr]))
        LOG_ERROR("Failed to create directory %@", url);
    
    return url.absoluteURL.path.fileSystemRepresentation;
}

WTF::String WebsiteDataStore::cacheDirectoryFileSystemRepresentation(const WTF::String& directoryName)
{
    static dispatch_once_t onceToken;
    static NSURL *cacheURL;

    dispatch_once(&onceToken, ^{
        NSURL *url = [[NSFileManager defaultManager] URLForDirectory:NSCachesDirectory inDomain:NSUserDomainMask appropriateForURL:nullptr create:NO error:nullptr];
        if (!url)
            RELEASE_ASSERT_NOT_REACHED();

        if (!CyberKit::processHasContainer()) {
            NSString *bundleIdentifier = [NSBundle mainBundle].bundleIdentifier;
            if (!bundleIdentifier)
                bundleIdentifier = [NSProcessInfo processInfo].processName;
            url = [url URLByAppendingPathComponent:bundleIdentifier isDirectory:YES];
        }

        cacheURL = [[url URLByAppendingPathComponent:@"CyberKit" isDirectory:YES] retain];
    });

    NSURL *url = [cacheURL URLByAppendingPathComponent:directoryName isDirectory:YES];
    if (![[NSFileManager defaultManager] createDirectoryAtURL:url withIntermediateDirectories:YES attributes:nil error:nullptr])
        LOG_ERROR("Failed to create directory %@", url);

    return url.absoluteURL.path.fileSystemRepresentation;
}

WTF::String WebsiteDataStore::websiteDataDirectoryFileSystemRepresentation(const WTF::String& directoryName)
{
    static dispatch_once_t onceToken;
    static NSURL *websiteDataURL;

    dispatch_once(&onceToken, ^{
        NSURL *url = [[NSFileManager defaultManager] URLForDirectory:NSLibraryDirectory inDomain:NSUserDomainMask appropriateForURL:nullptr create:NO error:nullptr];
        if (!url)
            RELEASE_ASSERT_NOT_REACHED();

        url = [url URLByAppendingPathComponent:@"CyberKit" isDirectory:YES];

        if (!CyberKit::processHasContainer()) {
            NSString *bundleIdentifier = [NSBundle mainBundle].bundleIdentifier;
            if (!bundleIdentifier)
                bundleIdentifier = [NSProcessInfo processInfo].processName;
            url = [url URLByAppendingPathComponent:bundleIdentifier isDirectory:YES];
        }

        websiteDataURL = [[url URLByAppendingPathComponent:@"WebsiteData" isDirectory:YES] retain];
    });

    NSURL *url = [websiteDataURL URLByAppendingPathComponent:directoryName isDirectory:YES];
    if (![[NSFileManager defaultManager] createDirectoryAtURL:url withIntermediateDirectories:YES attributes:nil error:nullptr])
        LOG_ERROR("Failed to create directory %@", url);

    return url.absoluteURL.path.fileSystemRepresentation;
}

Ref<CyberKit::WebsiteDataStoreConfiguration> WebsiteDataStore::defaultDataStoreConfiguration()
{
    auto configuration = CyberKit::WebsiteDataStoreConfiguration::create();

    configuration->setApplicationCacheDirectory(defaultApplicationCacheDirectory());
    configuration->setApplicationCacheFlatFileSubdirectoryName("Files");
    configuration->setCacheStorageDirectory(defaultCacheStorageDirectory());
    configuration->setNetworkCacheDirectory(defaultNetworkCacheDirectory());
    configuration->setMediaCacheDirectory(defaultMediaCacheDirectory());

    configuration->setIndexedDBDatabaseDirectory(defaultIndexedDBDatabaseDirectory());
    configuration->setServiceWorkerRegistrationDirectory(defaultServiceWorkerRegistrationDirectory());
    configuration->setWebSQLDatabaseDirectory(defaultWebSQLDatabaseDirectory());
    configuration->setLocalStorageDirectory(defaultLocalStorageDirectory());
    configuration->setMediaKeysStorageDirectory(defaultMediaKeysStorageDirectory());
    configuration->setResourceLoadStatisticsDirectory(defaultResourceLoadStatisticsDirectory());
    
    configuration->setJavaScriptConfigurationDirectory(defaultJavaScriptConfigurationDirectory());

    return configuration;
}

} // namespace API
