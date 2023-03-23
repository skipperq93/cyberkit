/*
 * Copyright (C) 2011 Apple Inc. All rights reserved.
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

#import "WebStorageManagerInternal.h"

#import "StorageTracker.h"
#import "CyberKitVersionChecks.h"
#import "WebSecurityOriginInternal.h"
#import "WebStorageNamespaceProvider.h"
#import "WebStorageTrackerClient.h"
#import <CyberCore/SecurityOrigin.h>
#import <CyberCore/SecurityOriginData.h>
#import <wtf/cocoa/VectorCocoa.h>

using namespace CyberCore;

NSString * const WebStorageDirectoryDefaultsKey = @"CyberKitLocalStorageDatabasePathPreferenceKey";
NSString * const WebStorageDidModifyOriginNotification = @"WebStorageDidModifyOriginNotification";

@implementation WebStorageManager

+ (WebStorageManager *)sharedWebStorageManager
{
    static WebStorageManager *sharedManager = [[WebStorageManager alloc] init];
    return sharedManager;
}

- (id)init
{
    if (!(self = [super init]))
        return nil;
    
    CyberKitInitializeStorageIfNecessary();
    
    return self;
}

- (NSArray *)origins
{
    return createNSArray(CyberKit::StorageTracker::tracker().origins(), [] (auto& origin) {
        return adoptNS([[WebSecurityOrigin alloc] _initWithCyberCoreSecurityOrigin:origin.securityOrigin().ptr()]);
    }).autorelease();
}

- (void)deleteAllOrigins
{
    CyberKit::StorageTracker::tracker().deleteAllOrigins();
#if PLATFORM(IOS_FAMILY)
    // FIXME: This needs to be removed once StorageTrackers in multiple processes
    // are in sync: <rdar://problem/9567500> Remove Website Data pane is not kept in sync with Safari
    [[NSFileManager defaultManager] removeItemAtPath:[WebStorageManager _storageDirectoryPath] error:NULL];
#endif
}

- (void)deleteOrigin:(WebSecurityOrigin *)origin
{
    CyberKit::StorageTracker::tracker().deleteOrigin([origin _core]->data());
}

- (unsigned long long)diskUsageForOrigin:(WebSecurityOrigin *)origin
{
    return CyberKit::StorageTracker::tracker().diskUsageForOrigin([origin _core]);
}

- (void)syncLocalStorage
{
    CyberKit::WebStorageNamespaceProvider::syncLocalStorage();
}

- (void)syncFileSystemAndTrackerDatabase
{
    CyberKit::StorageTracker::tracker().syncFileSystemAndTrackerDatabase();
}

+ (NSString *)_storageDirectoryPath
{
    static NeverDestroyed<RetainPtr<NSString>> sLocalStoragePath;
    static dispatch_once_t flag;
    dispatch_once(&flag, ^{
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        RetainPtr<NSString> localStoragePath = [defaults objectForKey:WebStorageDirectoryDefaultsKey];
        if (!localStoragePath || ![localStoragePath isKindOfClass:[NSString class]]) {
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
            NSString *libraryDirectory = [paths objectAtIndex:0];
            localStoragePath = [libraryDirectory stringByAppendingPathComponent:@"CyberKit/LocalStorage"];
        }
        sLocalStoragePath.get() = [localStoragePath stringByStandardizingPath];
    });
    return sLocalStoragePath.get().get();
}

+ (void)setStorageDatabaseIdleInterval:(double)interval
{
    CyberKit::StorageTracker::tracker().setStorageDatabaseIdleInterval(1_s * interval);
}

+ (void)closeIdleLocalStorageDatabases
{
    CyberKit::WebStorageNamespaceProvider::closeIdleLocalStorageDatabases();
}

void CyberKitInitializeStorageIfNecessary()
{
    static BOOL initialized = NO;
    if (initialized)
        return;

    auto *storagePath = [WebStorageManager _storageDirectoryPath];
    CyberKit::StorageTracker::initializeTracker(storagePath, WebStorageTrackerClient::sharedWebStorageTrackerClient());

#if PLATFORM(IOS_FAMILY)
    [[NSURL fileURLWithPath:storagePath] setResourceValue:@YES forKey:NSURLIsExcludedFromBackupKey error:nil];
#endif

    initialized = YES;
}

@end
