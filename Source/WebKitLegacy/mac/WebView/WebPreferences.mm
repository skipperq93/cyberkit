/*
 * Copyright (C) 2005-2023 Apple Inc. All rights reserved.
 *           (C) 2006 Graham Dennis (graham.dennis@gmail.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "WebPreferencesInternal.h"

#import "NetworkStorageSessionMap.h"
#import "TestingFunctions.h"
#import "WebApplicationCache.h"
#import "WebFeature.h"
#import "WebFrameNetworkingContext.h"
#import "CyberKitLogging.h"
#import "CyberKitNSStringExtras.h"
#import "CyberKitVersionChecks.h"
#import "WebNSDictionaryExtras.h"
#import "WebNSURLExtras.h"
#import "WebPreferenceKeysPrivate.h"
#import "WebPreferencesDefinitions.h"
#import <CyberScriptCore/InitializeThreading.h>
#import <CyberCore/ApplicationCacheStorage.h>
#import <CyberCore/AudioSession.h>
#import <CyberCore/MediaPlayerEnums.h>
#import <CyberCore/NetworkStorageSession.h>
#import <CyberCore/RuntimeApplicationChecks.h>
#import <CyberCore/Settings.h>
#import <CyberCore/CyberCoreJITOperations.h>
#import <pal/spi/cf/CFNetworkSPI.h>
#import <pal/text/TextEncodingRegistry.h>
#import <wtf/BlockPtr.h>
#import <wtf/Compiler.h>
#import <wtf/MainThread.h>
#import <wtf/OptionSet.h>
#import <wtf/RetainPtr.h>
#import <wtf/RunLoop.h>
#import <wtf/cocoa/RuntimeApplicationChecksCocoa.h>

using namespace CyberCore;

#if PLATFORM(IOS_FAMILY)
#import <CyberCore/GraphicsContext.h>
#import <CyberCore/CyberCoreThreadMessage.h>
#endif

NSString *WebPreferencesChangedNotification = @"WebPreferencesChangedNotification";
NSString *WebPreferencesRemovedNotification = @"WebPreferencesRemovedNotification";
NSString *WebPreferencesChangedInternalNotification = @"WebPreferencesChangedInternalNotification";
NSString *WebPreferencesCacheModelChangedInternalNotification = @"WebPreferencesCacheModelChangedInternalNotification";

#define KEY(x) (_private->identifier ? [_private->identifier.get() stringByAppendingString:(x)] : (x))

enum { WebPreferencesVersion = 1 };

static RetainPtr<WebPreferences>& standardPreferences()
{
    static NeverDestroyed<RetainPtr<WebPreferences>> standardPreferences;
    return standardPreferences;
}

static RetainPtr<NSMutableDictionary>& webPreferencesInstances()
{
    static NeverDestroyed<RetainPtr<NSMutableDictionary>> webPreferencesInstances;
    return webPreferencesInstances;
}

static unsigned webPreferencesInstanceCountWithPrivateBrowsingEnabled;

template<unsigned size> static bool contains(const char* const (&array)[size], const char* item)
{
    if (!item)
        return false;
    for (auto* string : array) {
        if (equalIgnoringASCIICase(string, item))
            return true;
    }
    return false;
}

static WebCacheModel cacheModelForMainBundle(NSString *bundleIdentifier)
{
    @autoreleasepool {
        // Apps that probably need the small setting
        static const char* const documentViewerIDs[] = {
            "Microsoft/com.microsoft.Messenger",
            "com.adiumX.adiumX",
            "com.alientechnology.Proteus",
            "com.apple.Dashcode",
            "com.apple.iChat",
            "com.barebones.bbedit",
            "com.barebones.textwrangler",
            "com.barebones.yojimbo",
            "com.equinux.iSale4",
            "com.growl.growlframework",
            "com.intrarts.PandoraMan",
            "com.karelia.Sandvox",
            "com.macromates.textmate",
            "com.realmacsoftware.rapidweaverpro",
            "com.red-sweater.marsedit",
            "com.yahoo.messenger3",
            "de.codingmonkeys.SubEthaEdit",
            "fi.karppinen.Pyro",
            "info.colloquy",
            "kungfoo.tv.ecto",
        };

        // Apps that probably need the medium setting
        static const char* const documentBrowserIDs[] = {
            "com.apple.Dictionary",
            "com.apple.Xcode",
            "com.apple.helpviewer",
            "com.culturedcode.xyle",
            "com.macrabbit.CSSEdit",
            "com.panic.Coda",
            "com.ranchero.NetNewsWire",
            "com.thinkmac.NewsLife",
            "org.xlife.NewsFire",
            "uk.co.opencommunity.vienna2",
        };

        // Apps that probably need the large setting
        static const char* const primaryWebBrowserIDs[] = {
            "com.app4mac.KidsBrowser",
            "com.app4mac.wKiosk",
            "com.freeverse.bumpercar",
            "com.omnigroup.OmniWeb5",
            "com.sunrisebrowser.Sunrise",
            "net.hmdt-web.Shiira",
        };

        const char* bundleID = [bundleIdentifier UTF8String];
        if (contains(documentViewerIDs, bundleID))
            return WebCacheModelDocumentViewer;
        if (contains(documentBrowserIDs, bundleID))
            return WebCacheModelDocumentBrowser;
        if (contains(primaryWebBrowserIDs, bundleID))
            return WebCacheModelPrimaryWebBrowser;

        bool isLinkedAgainstCyberKit = CyberKitLinkedOnOrAfter(0);
        if (!isLinkedAgainstCyberKit)
            return WebCacheModelDocumentViewer; // Apps that don't link against CyberKit probably aren't meant to be browsers.

#if !PLATFORM(IOS_FAMILY)
        bool isLegacyApp = !CyberKitLinkedOnOrAfter(WEBKIT_FIRST_VERSION_WITH_CACHE_MODEL_API);
#else
        bool isLegacyApp = false;
#endif
        if (isLegacyApp)
            return WebCacheModelDocumentBrowser; // To avoid regressions in apps that depended on old CyberKit's large cache.

        return WebCacheModelDocumentViewer; // To save memory.
    }
}

#if ENABLE(BUILD_FOR_TESTING)
WebCacheModel TestWebPreferencesCacheModelForMainBundle(NSString *bundleIdentifier)
{
    return cacheModelForMainBundle(bundleIdentifier);
}
#endif // ENABLE(BUILD_FOR_TESTING)

@interface WebPreferences ()
- (void)_postCacheModelChangedNotification;
@end

@interface WebPreferences (WebInternal)
+ (NSString *)_concatenateKeyWithIBCreatorID:(NSString *)key;
+ (NSString *)_IBCreatorID;
@end

enum class UpdateAfterBatchType : uint8_t {
    API         = 1 << 0,
    Internal    = 1 << 1
};

struct WebPreferencesPrivate
{
public:
    WebPreferencesPrivate()
#if PLATFORM(IOS_FAMILY)
        : readWriteQueue { adoptNS(dispatch_queue_create("com.apple.WebPreferences.ReadWriteQueue", DISPATCH_QUEUE_CONCURRENT)) }
#endif
    {
    }

#if PLATFORM(IOS_FAMILY)
    RetainPtr<dispatch_queue_t> readWriteQueue;
#endif
    RetainPtr<NSMutableDictionary> values;
    RetainPtr<NSString> identifier;
    BOOL inPrivateBrowsing { NO };
    BOOL autosaves { NO };
    BOOL automaticallyDetectsCacheModel { NO };
    unsigned numWebViews { 0 };
    unsigned updateBatchCount { 0 };
    OptionSet<UpdateAfterBatchType> updateAfterBatchType;
};

#if PLATFORM(IOS_FAMILY)
@interface WebPreferences ()
- (id)initWithIdentifier:(NSString *)anIdentifier sendChangeNotification:(BOOL)sendChangeNotification;
@end
#endif

@implementation WebPreferences

- (instancetype)init
{
    // Create fake identifier
    static int instanceCount = 1;
    NSString *fakeIdentifier;

    // At least ensure that identifier hasn't been already used.
    fakeIdentifier = [NSString stringWithFormat:@"WebPreferences%d", instanceCount++];
    while ([[self class] _getInstanceForIdentifier:fakeIdentifier]){
        fakeIdentifier = [NSString stringWithFormat:@"WebPreferences%d", instanceCount++];
    }

    return [self initWithIdentifier:fakeIdentifier];
}

#if PLATFORM(IOS_FAMILY)
- (id)initWithIdentifier:(NSString *)anIdentifier
{
    return [self initWithIdentifier:anIdentifier sendChangeNotification:YES];
}
#endif

#if PLATFORM(IOS_FAMILY)
- (instancetype)initWithIdentifier:(NSString *)anIdentifier sendChangeNotification:(BOOL)sendChangeNotification
#else
- (instancetype)initWithIdentifier:(NSString *)anIdentifier
#endif
{
    WebPreferences *instance = [[self class] _getInstanceForIdentifier:anIdentifier];
    if (instance) {
        [self release];
        return [instance retain];
    }

    self = [super init];
    if (!self)
        return nil;

    _private = new WebPreferencesPrivate;
    _private->values = adoptNS([[NSMutableDictionary alloc] init]);
    _private->identifier = adoptNS([anIdentifier copy]);
    _private->automaticallyDetectsCacheModel = YES;

    [[self class] _setInstance:self forIdentifier:_private->identifier.get()];

    [self _updatePrivateBrowsingStateTo:[self privateBrowsingEnabled]];

#if PLATFORM(IOS_FAMILY)
    if (sendChangeNotification) {
        [self _postPreferencesChangedNotification];
        [self _postCacheModelChangedNotification];
    }
#else
    [self _postPreferencesChangedNotification];
    [self _postCacheModelChangedNotification];
#endif

    return self;
}

- (instancetype)initWithCoder:(NSCoder *)decoder
{
    self = [super init];
    if (!self)
        return nil;

    _private = new WebPreferencesPrivate;
    _private->automaticallyDetectsCacheModel = YES;

    @try {
        id identifier = nil;
        id values = nil;
        if ([decoder allowsKeyedCoding]) {
            identifier = [decoder decodeObjectForKey:@"Identifier"];
            values = [decoder decodeObjectForKey:@"Values"];
        } else {
            int version;
            [decoder decodeValueOfObjCType:@encode(int) at:&version size:sizeof(int)];
            if (version == 1) {
                identifier = [decoder decodeObject];
                values = [decoder decodeObject];
            }
        }

        if ([identifier isKindOfClass:[NSString class]])
            _private->identifier = adoptNS([identifier copy]);
        if ([values isKindOfClass:[NSDictionary class]])
            _private->values = adoptNS([values mutableCopy]); // ensure dictionary is mutable

        LOG(Encoding, "Identifier = %@, Values = %@\n", _private->identifier.get(), _private->values.get());
    } @catch(id) {
        [self release];
        return nil;
    }

    // If we load a nib multiple times, or have instances in multiple
    // nibs with the same name, the first guy up wins.
    WebPreferences *instance = [[self class] _getInstanceForIdentifier:_private->identifier.get()];
    if (instance) {
        [self release];
        self = [instance retain];
    } else {
        [[self class] _setInstance:self forIdentifier:_private->identifier.get()];
        [self _updatePrivateBrowsingStateTo:[self privateBrowsingEnabled]];
    }

    return self;
}

- (void)encodeWithCoder:(NSCoder *)encoder
{
    if ([encoder allowsKeyedCoding]){
        [encoder encodeObject:_private->identifier.get() forKey:@"Identifier"];
#if PLATFORM(IOS_FAMILY)
        dispatch_sync(_private->readWriteQueue.get(), ^{
#endif
        [encoder encodeObject:_private->values.get() forKey:@"Values"];
        LOG (Encoding, "Identifier = %@, Values = %@\n", _private->identifier.get(), _private->values.get());
#if PLATFORM(IOS_FAMILY)
        });
#endif
    }
    else {
        int version = WebPreferencesVersion;
        [encoder encodeValueOfObjCType:@encode(int) at:&version];
        [encoder encodeObject:_private->identifier.get()];
#if PLATFORM(IOS_FAMILY)
        dispatch_sync(_private->readWriteQueue.get(), ^{
#endif
        [encoder encodeObject:_private->values.get()];
#if PLATFORM(IOS_FAMILY)
        });
#endif
    }
}

+ (WebPreferences *)standardPreferences
{
#if !PLATFORM(IOS_FAMILY)
    if (!standardPreferences()) {
        standardPreferences() = adoptNS([[WebPreferences alloc] initWithIdentifier:nil]);
        [standardPreferences() setAutosaves:YES];
    }
#else
    static dispatch_once_t pred;
    dispatch_once(&pred, ^{
        standardPreferences() = adoptNS([[WebPreferences alloc] initWithIdentifier:nil sendChangeNotification:NO]);
        [standardPreferences() _postPreferencesChangedNotification];
        [standardPreferences() setAutosaves:YES];
    });
#endif

    return standardPreferences().get();
}

// if we ever have more than one WebPreferences object, this would move to init
+ (void)initialize
{
#if PLATFORM(MAC)
    JSC::initialize();
    WTF::initializeMainThread();
    CyberCore::populateJITOperations();
#endif

    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
        INITIALIZE_DEFAULT_PREFERENCES_DICTIONARY_FROM_GENERATED_PREFERENCES

        @NO, CyberKitUserStyleSheetEnabledPreferenceKey,
        @"", CyberKitUserStyleSheetLocationPreferenceKey,
        @YES, CyberKitAllowAnimatedImagesPreferenceKey,
        @YES, CyberKitAllowAnimatedImageLoopingPreferenceKey,
        @"1800", CyberKitBackForwardCacheExpirationIntervalKey,
        @NO, CyberKitPrivateBrowsingEnabledPreferenceKey,
        @(cacheModelForMainBundle([[NSBundle mainBundle] bundleIdentifier])), CyberKitCacheModelPreferenceKey,
        @YES, CyberKitZoomsTextOnlyPreferenceKey,
        [NSNumber numberWithLongLong:ApplicationCacheStorage::noQuota()], CyberKitApplicationCacheTotalQuota,

        // FIXME: Are these relevent to CyberKitLegacy? If not, we should remove them.
        @NO, CyberKitResourceLoadStatisticsEnabledPreferenceKey,
        @NO, CyberKitDebugInAppBrowserPrivacyEnabledPreferenceKey,

#if ENABLE(LEGACY_ENCRYPTED_MEDIA)
        @"~/Library/CyberKit/MediaKeys", CyberKitMediaKeysStorageDirectoryKey,
#endif

#if PLATFORM(MAC)
        @NO, CyberKitRespectStandardStyleKeyEquivalentsPreferenceKey,
        @"1", CyberKitPDFDisplayModePreferenceKey,
        @"0", CyberKitPDFScaleFactorPreferenceKey,
        @(WebTextDirectionSubmenuAutomaticallyIncluded), CyberKitTextDirectionSubmenuInclusionBehaviorPreferenceKey,
        [NSNumber numberWithLongLong:ApplicationCacheStorage::noQuota()], CyberKitApplicationCacheDefaultOriginQuota,
#endif

#if PLATFORM(IOS_FAMILY)
        @NO, CyberKitStorageTrackerEnabledPreferenceKey,
        @(static_cast<unsigned>(AudioSession::CategoryType::None)), CyberKitAudioSessionCategoryOverride,

        // Per-Origin Quota on iOS is 25MB. When the quota is reached for a particular origin
        // the quota for that origin can be increased. See also webView:exceededApplicationCacheOriginQuotaForSecurityOrigin:totalSpaceNeeded in WebUI/WebUIDelegate.m.
        [NSNumber numberWithLongLong:(25 * 1024 * 1024)], CyberKitApplicationCacheDefaultOriginQuota,

        @NO, CyberKitAlwaysRequestGeolocationPermissionPreferenceKey,
        @(static_cast<int>(InterpolationQuality::Low)), CyberKitInterpolationQualityPreferenceKey,
        @NO, CyberKitNetworkDataUsageTrackingEnabledPreferenceKey,
        @"", CyberKitNetworkInterfaceNamePreferenceKey,
#endif
        nil];

#if !PLATFORM(IOS_FAMILY)
    // This value shouldn't ever change, which is assumed in the initialization of CyberKitPDFDisplayModePreferenceKey above
    ASSERT(kPDFDisplaySinglePageContinuous == 1);
#endif
    [[NSUserDefaults standardUserDefaults] registerDefaults:dict];
}

- (void)dealloc
{
    [self _updatePrivateBrowsingStateTo:NO];

    delete _private;
    [super dealloc];
}

- (NSString *)identifier
{
    return _private->identifier.get();
}

- (id)_valueForKey:(NSString *)key
{
    NSString *_key = KEY(key);
#if PLATFORM(IOS_FAMILY)
    __block id o = nil;
    dispatch_sync(_private->readWriteQueue.get(), ^{
        o = [_private->values.get() objectForKey:_key];
    });
#else
    id o = [_private->values.get() objectForKey:_key];
#endif
    if (o)
        return o;
    o = [[NSUserDefaults standardUserDefaults] objectForKey:_key];
    if (!o && key != _key)
        o = [[NSUserDefaults standardUserDefaults] objectForKey:key];
    return o;
}

- (NSString *)_stringValueForKey:(NSString *)key
{
    id s = [self _valueForKey:key];
    return [s isKindOfClass:[NSString class]] ? (NSString *)s : nil;
}

- (void)_setStringValue:(NSString *)value forKey:(NSString *)key
{
    if ([[self _stringValueForKey:key] isEqualToString:value])
        return;
    NSString *_key = KEY(key);
#if PLATFORM(IOS_FAMILY)
    dispatch_barrier_sync(_private->readWriteQueue.get(), ^{
#endif
    [_private->values.get() setObject:value forKey:_key];
#if PLATFORM(IOS_FAMILY)
    });
#endif
    if (_private->autosaves)
        [[NSUserDefaults standardUserDefaults] setObject:value forKey:_key];
    [self _postPreferencesChangedNotification];
}

- (NSArray<NSString *> *)_stringArrayValueForKey:(NSString *)key
{
    id value = [self _valueForKey:key];
    if (![value isKindOfClass:[NSArray class]])
        return nil;

    NSArray *array = (NSArray *)value;
    for (id object in array) {
        if (![object isKindOfClass:[NSString class]])
            return nil;
    }

    return (NSArray<NSString *> *)array;
}

- (void)_setStringArrayValueForKey:(NSArray<NSString *> *)value forKey:(NSString *)key
{
    NSString *_key = KEY(key);
#if PLATFORM(IOS_FAMILY)
    dispatch_barrier_sync(_private->readWriteQueue.get(), ^{
#endif
        [_private->values.get() setObject:value forKey:_key];
#if PLATFORM(IOS_FAMILY)
    });
#endif
    if (_private->autosaves)
        [[NSUserDefaults standardUserDefaults] setObject:value forKey:_key];
    [self _postPreferencesChangedNotification];
}

- (int)_integerValueForKey:(NSString *)key
{
    id o = [self _valueForKey:key];
    return [o respondsToSelector:@selector(intValue)] ? [o intValue] : 0;
}

- (void)_setIntegerValue:(int)value forKey:(NSString *)key
{
    if ([self _integerValueForKey:key] == value)
        return;
    NSString *_key = KEY(key);
#if PLATFORM(IOS_FAMILY)
    dispatch_barrier_sync(_private->readWriteQueue.get(), ^{
#endif
    [_private->values.get() setObject:@(value) forKey:_key];
#if PLATFORM(IOS_FAMILY)
    });
#endif
    if (_private->autosaves)
        [[NSUserDefaults standardUserDefaults] setInteger:value forKey:_key];
    [self _postPreferencesChangedNotification];
}

- (unsigned int)_unsignedIntValueForKey:(NSString *)key
{
    id o = [self _valueForKey:key];
    return [o respondsToSelector:@selector(unsignedIntValue)] ? [o unsignedIntValue] : 0;
}

- (void)_setUnsignedIntValue:(unsigned int)value forKey:(NSString *)key
{
    if ([self _unsignedIntValueForKey:key] == value)
        return;
    NSString *_key = KEY(key);
#if PLATFORM(IOS_FAMILY)
    dispatch_barrier_sync(_private->readWriteQueue.get(), ^{
#endif
    [_private->values.get() setObject:@(value) forKey:_key];
#if PLATFORM(IOS_FAMILY)
    });
#endif
    if (_private->autosaves)
        [[NSUserDefaults standardUserDefaults] setObject:@(value) forKey:_key];
    [self _postPreferencesChangedNotification];
}

- (float)_floatValueForKey:(NSString *)key
{
    id o = [self _valueForKey:key];
    return [o respondsToSelector:@selector(floatValue)] ? [o floatValue] : 0.0f;
}

- (void)_setFloatValue:(float)value forKey:(NSString *)key
{
    if ([self _floatValueForKey:key] == value)
        return;
    NSString *_key = KEY(key);
#if PLATFORM(IOS_FAMILY)
    dispatch_barrier_sync(_private->readWriteQueue.get(), ^{
#endif
    [_private->values.get() setObject:@(value) forKey:_key];
#if PLATFORM(IOS_FAMILY)
    });
#endif
    if (_private->autosaves)
        [[NSUserDefaults standardUserDefaults] setFloat:value forKey:_key];
    [self _postPreferencesChangedNotification];
}

- (BOOL)_boolValueForKey:(NSString *)key
{
    return [self _integerValueForKey:key] != 0;
}

- (void)_setBoolValue:(BOOL)value forKey:(NSString *)key
{
    if ([self _boolValueForKey:key] == value)
        return;
    NSString *_key = KEY(key);
#if PLATFORM(IOS_FAMILY)
    dispatch_barrier_sync(_private->readWriteQueue.get(), ^{
#endif
    [_private->values.get() setObject:@(value) forKey:_key];
#if PLATFORM(IOS_FAMILY)
    });
#endif
    if (_private->autosaves)
        [[NSUserDefaults standardUserDefaults] setBool:value forKey:_key];
    [self _postPreferencesChangedNotification];
}

- (long long)_longLongValueForKey:(NSString *)key
{
    id o = [self _valueForKey:key];
    return [o respondsToSelector:@selector(longLongValue)] ? [o longLongValue] : 0;
}

- (void)_setLongLongValue:(long long)value forKey:(NSString *)key
{
    if ([self _longLongValueForKey:key] == value)
        return;
    NSString *_key = KEY(key);
#if PLATFORM(IOS_FAMILY)
    dispatch_barrier_sync(_private->readWriteQueue.get(), ^{
#endif
    [_private->values.get() setObject:@(value) forKey:_key];
#if PLATFORM(IOS_FAMILY)
    });
#endif
    if (_private->autosaves)
        [[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithLongLong:value] forKey:_key];
    [self _postPreferencesChangedNotification];
}

- (unsigned long long)_unsignedLongLongValueForKey:(NSString *)key
{
    id o = [self _valueForKey:key];
    return [o respondsToSelector:@selector(unsignedLongLongValue)] ? [o unsignedLongLongValue] : 0;
}

- (void)_setUnsignedLongLongValue:(unsigned long long)value forKey:(NSString *)key
{
    if ([self _unsignedLongLongValueForKey:key] == value)
        return;
    NSString *_key = KEY(key);
#if PLATFORM(IOS_FAMILY)
    dispatch_barrier_sync(_private->readWriteQueue.get(), ^{
#endif
    [_private->values.get() setObject:@(value) forKey:_key];
#if PLATFORM(IOS_FAMILY)
    });
#endif
    if (_private->autosaves)
        [[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithUnsignedLongLong:value] forKey:_key];
    [self _postPreferencesChangedNotification];
}

- (NSString *)standardFontFamily
{
    return [self _stringValueForKey: CyberKitStandardFontPreferenceKey];
}

- (void)setStandardFontFamily:(NSString *)family
{
    [self _setStringValue: family forKey: CyberKitStandardFontPreferenceKey];
}

- (NSString *)fixedFontFamily
{
    return [self _stringValueForKey: CyberKitFixedFontPreferenceKey];
}

- (void)setFixedFontFamily:(NSString *)family
{
    [self _setStringValue: family forKey: CyberKitFixedFontPreferenceKey];
}

- (NSString *)serifFontFamily
{
    return [self _stringValueForKey: CyberKitSerifFontPreferenceKey];
}

- (void)setSerifFontFamily:(NSString *)family
{
    [self _setStringValue: family forKey: CyberKitSerifFontPreferenceKey];
}

- (NSString *)sansSerifFontFamily
{
    return [self _stringValueForKey: CyberKitSansSerifFontPreferenceKey];
}

- (void)setSansSerifFontFamily:(NSString *)family
{
    [self _setStringValue: family forKey: CyberKitSansSerifFontPreferenceKey];
}

- (NSString *)cursiveFontFamily
{
    return [self _stringValueForKey: CyberKitCursiveFontPreferenceKey];
}

- (void)setCursiveFontFamily:(NSString *)family
{
    [self _setStringValue: family forKey: CyberKitCursiveFontPreferenceKey];
}

- (NSString *)fantasyFontFamily
{
    return [self _stringValueForKey: CyberKitFantasyFontPreferenceKey];
}

- (void)setFantasyFontFamily:(NSString *)family
{
    [self _setStringValue: family forKey: CyberKitFantasyFontPreferenceKey];
}

- (int)defaultFontSize
{
    return [self _integerValueForKey: CyberKitDefaultFontSizePreferenceKey];
}

- (void)setDefaultFontSize:(int)size
{
    [self _setIntegerValue: size forKey: CyberKitDefaultFontSizePreferenceKey];
}

- (int)defaultFixedFontSize
{
    return [self _integerValueForKey: CyberKitDefaultFixedFontSizePreferenceKey];
}

- (void)setDefaultFixedFontSize:(int)size
{
    [self _setIntegerValue: size forKey: CyberKitDefaultFixedFontSizePreferenceKey];
}

- (int)minimumFontSize
{
    return [self _integerValueForKey: CyberKitMinimumFontSizePreferenceKey];
}

- (void)setMinimumFontSize:(int)size
{
    [self _setIntegerValue: size forKey: CyberKitMinimumFontSizePreferenceKey];
}

- (int)minimumLogicalFontSize
{
  return [self _integerValueForKey: CyberKitMinimumLogicalFontSizePreferenceKey];
}

- (void)setMinimumLogicalFontSize:(int)size
{
  [self _setIntegerValue: size forKey: CyberKitMinimumLogicalFontSizePreferenceKey];
}

- (NSString *)defaultTextEncodingName
{
    return [self _stringValueForKey: CyberKitDefaultTextEncodingNamePreferenceKey];
}

- (void)setDefaultTextEncodingName:(NSString *)encoding
{
    [self _setStringValue: encoding forKey: CyberKitDefaultTextEncodingNamePreferenceKey];
}

#if !PLATFORM(IOS_FAMILY)
- (BOOL)userStyleSheetEnabled
{
    return [self _boolValueForKey: CyberKitUserStyleSheetEnabledPreferenceKey];
}

- (void)setUserStyleSheetEnabled:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitUserStyleSheetEnabledPreferenceKey];
}

- (NSURL *)userStyleSheetLocation
{
    NSString *locationString = [self _stringValueForKey: CyberKitUserStyleSheetLocationPreferenceKey];

    if ([locationString _webkit_looksLikeAbsoluteURL]) {
        return [NSURL _web_URLWithDataAsString:locationString];
    } else {
        locationString = [locationString stringByExpandingTildeInPath];
        return [NSURL fileURLWithPath:locationString isDirectory:NO];
    }
}

- (void)setUserStyleSheetLocation:(NSURL *)URL
{
    NSString *locationString;

    if ([URL isFileURL]) {
        locationString = [[URL path] _web_stringByAbbreviatingWithTildeInPath];
    } else {
        locationString = [URL _web_originalDataAsString];
    }

    if (!locationString)
        locationString = @"";

    [self _setStringValue:locationString forKey: CyberKitUserStyleSheetLocationPreferenceKey];
}
#else

// These methods have had their implementations removed on iOS since it
// is wrong to have such a setting stored in preferences that, when read,
// is applied to all WebViews in a iOS process. Such a design might work
// OK for an application like Safari on Mac OS X, where the only WebViews
// in use display web content in a straightforward manner. However, it is
// wrong for iOS, where WebViews are used for various purposes, like
// text editing, text rendering, and displaying web content.
//
// I have changed the user style sheet mechanism to be a per-WebView
// setting, rather than a per-process preference. This seems to give the
// behavior we want for iOS.

- (BOOL)userStyleSheetEnabled
{
    return NO;
}

- (void)setUserStyleSheetEnabled:(BOOL)flag
{
    // no-op
}

- (NSURL *)userStyleSheetLocation
{
    return nil;
}

- (void)setUserStyleSheetLocation:(NSURL *)URL
{
    // no-op
}
#endif // PLATFORM(IOS_FAMILY)

- (BOOL)shouldPrintBackgrounds
{
    return [self _boolValueForKey: CyberKitShouldPrintBackgroundsPreferenceKey];
}

- (void)setShouldPrintBackgrounds:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitShouldPrintBackgroundsPreferenceKey];
}

- (BOOL)isJavaScriptEnabled
{
    return [self _boolValueForKey: CyberKitJavaScriptEnabledPreferenceKey];
}

- (void)setJavaScriptEnabled:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitJavaScriptEnabledPreferenceKey];
}

- (BOOL)javaScriptCanOpenWindowsAutomatically
{
    return [self _boolValueForKey: CyberKitJavaScriptCanOpenWindowsAutomaticallyPreferenceKey];
}

- (void)setJavaScriptCanOpenWindowsAutomatically:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitJavaScriptCanOpenWindowsAutomaticallyPreferenceKey];
}

- (BOOL)arePlugInsEnabled
{
    return [self _boolValueForKey: CyberKitPluginsEnabledPreferenceKey];
}

- (void)setPlugInsEnabled:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitPluginsEnabledPreferenceKey];
}

- (BOOL)allowsAnimatedImages
{
    return [self _boolValueForKey: CyberKitAllowAnimatedImagesPreferenceKey];
}

- (void)setAllowsAnimatedImages:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitAllowAnimatedImagesPreferenceKey];
}

- (BOOL)allowsAnimatedImageLooping
{
    return [self _boolValueForKey: CyberKitAllowAnimatedImageLoopingPreferenceKey];
}

- (void)setAllowsAnimatedImageLooping: (BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitAllowAnimatedImageLoopingPreferenceKey];
}

- (void)setLoadsImagesAutomatically: (BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitDisplayImagesKey];
}

- (BOOL)loadsImagesAutomatically
{
    return [self _boolValueForKey: CyberKitDisplayImagesKey];
}

- (void)setAdditionalSupportedImageTypes:(NSArray<NSString*> *)imageTypes
{
    [self _setStringArrayValueForKey:imageTypes forKey:CyberKitAdditionalSupportedImageTypesKey];
}

- (NSArray<NSString *> *)additionalSupportedImageTypes
{
    return [self _stringArrayValueForKey:CyberKitAdditionalSupportedImageTypesKey];
}

- (void)setAutosaves:(BOOL)flag
{
    _private->autosaves = flag;
}

- (BOOL)autosaves
{
    return _private->autosaves;
}

#if !PLATFORM(IOS_FAMILY)
- (void)setTabsToLinks:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitTabToLinksPreferenceKey];
}

- (BOOL)tabsToLinks
{
    return [self _boolValueForKey:CyberKitTabToLinksPreferenceKey];
}
#endif

- (void)setPrivateBrowsingEnabled:(BOOL)enabled
{
    [self _updatePrivateBrowsingStateTo:enabled];
    [self _setBoolValue:enabled forKey:CyberKitPrivateBrowsingEnabledPreferenceKey];
}

- (BOOL)privateBrowsingEnabled
{
    // Changes to private browsing defaults do not have effect on existing WebPreferences, and must be done through -setPrivateBrowsingEnabled.
    // This is needed to accurately track private browsing sessions in the process.
    return _private->inPrivateBrowsing;
}

- (void)_updatePrivateBrowsingStateTo:(BOOL)enabled
{
    if (!_private) {
        ASSERT(!enabled);
        return;
    }

    if (enabled == _private->inPrivateBrowsing)
        return;
    if (enabled > _private->inPrivateBrowsing) {
        WebFrameNetworkingContext::ensurePrivateBrowsingSession();
        ++webPreferencesInstanceCountWithPrivateBrowsingEnabled;
    } else {
        ASSERT(webPreferencesInstanceCountWithPrivateBrowsingEnabled);
        --webPreferencesInstanceCountWithPrivateBrowsingEnabled;
        if (!webPreferencesInstanceCountWithPrivateBrowsingEnabled)
            WebFrameNetworkingContext::destroyPrivateBrowsingSession();
    }
    _private->inPrivateBrowsing = enabled;
}

- (void)setUsesPageCache:(BOOL)usesPageCache
{
    [self _setBoolValue:usesPageCache forKey:CyberKitUsesPageCachePreferenceKey];
}

- (BOOL)usesPageCache
{
    return [self _boolValueForKey:CyberKitUsesPageCachePreferenceKey];
}

- (void)_postCacheModelChangedNotification
{
#if !PLATFORM(IOS_FAMILY)
    if (!pthread_main_np()) {
        [self performSelectorOnMainThread:_cmd withObject:nil waitUntilDone:NO];
        return;
    }
#endif

    [[NSNotificationCenter defaultCenter] postNotificationName:WebPreferencesCacheModelChangedInternalNotification object:self userInfo:nil];
}

- (void)setCacheModel:(WebCacheModel)cacheModel
{
    [self _setIntegerValue:cacheModel forKey:CyberKitCacheModelPreferenceKey];
    [self setAutomaticallyDetectsCacheModel:NO];
    [self _postCacheModelChangedNotification];
}

- (WebCacheModel)cacheModel
{
    return (WebCacheModel)[self _integerValueForKey:CyberKitCacheModelPreferenceKey];
}


- (void)setSuppressesIncrementalRendering:(BOOL)suppressesIncrementalRendering
{
    [self _setBoolValue:suppressesIncrementalRendering forKey:CyberKitSuppressesIncrementalRenderingKey];
}

- (BOOL)suppressesIncrementalRendering
{
    return [self _boolValueForKey:CyberKitSuppressesIncrementalRenderingKey];
}

- (BOOL)allowsAirPlayForMediaPlayback
{
#if ENABLE(WIRELESS_PLAYBACK_TARGET)
    return [self _boolValueForKey:CyberKitAllowsAirPlayForMediaPlaybackPreferenceKey];
#else
    return false;
#endif
}

- (void)setAllowsAirPlayForMediaPlayback:(BOOL)flag
{
#if ENABLE(WIRELESS_PLAYBACK_TARGET)
    [self _setBoolValue:flag forKey:CyberKitAllowsAirPlayForMediaPlaybackPreferenceKey];
#endif
}

- (BOOL)isJavaEnabled
{
    return NO;
}

- (void)setJavaEnabled:(BOOL)flag
{
}

@end

@implementation WebPreferences (WebPrivate)

- (BOOL)isDNSPrefetchingEnabled
{
    return [self _boolValueForKey:CyberKitDNSPrefetchingEnabledPreferenceKey];
}

- (void)setDNSPrefetchingEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitDNSPrefetchingEnabledPreferenceKey];
}

- (BOOL)developerExtrasEnabled
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    if ([defaults boolForKey:@"DisableCyberKitDeveloperExtras"])
        return NO;
#ifdef NDEBUG
    if ([defaults boolForKey:@"CyberKitDeveloperExtras"] || [defaults boolForKey:@"IncludeDebugMenu"])
        return YES;
    return [self _boolValueForKey:CyberKitDeveloperExtrasEnabledPreferenceKey];
#else
    return YES; // always enable in debug builds
#endif
}

- (CyberKitJavaScriptRuntimeFlags)javaScriptRuntimeFlags
{
    return static_cast<CyberKitJavaScriptRuntimeFlags>([self _unsignedIntValueForKey:CyberKitJavaScriptRuntimeFlagsPreferenceKey]);
}

- (void)setJavaScriptRuntimeFlags:(CyberKitJavaScriptRuntimeFlags)flags
{
    [self _setUnsignedIntValue:flags forKey:CyberKitJavaScriptRuntimeFlagsPreferenceKey];
}

- (void)setDeveloperExtrasEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitDeveloperExtrasEnabledPreferenceKey];
}

- (BOOL)authorAndUserStylesEnabled
{
    return [self _boolValueForKey:CyberKitAuthorAndUserStylesEnabledPreferenceKey];
}

- (void)setAuthorAndUserStylesEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAuthorAndUserStylesEnabledPreferenceKey];
}

- (BOOL)domTimersThrottlingEnabled
{
    return [self _boolValueForKey:CyberKitDOMTimersThrottlingEnabledPreferenceKey];
}

- (void)setDOMTimersThrottlingEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitDOMTimersThrottlingEnabledPreferenceKey];
}

- (BOOL)webArchiveDebugModeEnabled
{
    return [self _boolValueForKey:CyberKitWebArchiveDebugModeEnabledPreferenceKey];
}

- (void)setWebArchiveDebugModeEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitWebArchiveDebugModeEnabledPreferenceKey];
}

- (BOOL)localFileContentSniffingEnabled
{
    return [self _boolValueForKey:CyberKitLocalFileContentSniffingEnabledPreferenceKey];
}

- (void)setLocalFileContentSniffingEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitLocalFileContentSniffingEnabledPreferenceKey];
}

- (BOOL)offlineWebApplicationCacheEnabled
{
    return [self _boolValueForKey:CyberKitOfflineWebApplicationCacheEnabledPreferenceKey];
}

- (void)setOfflineWebApplicationCacheEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitOfflineWebApplicationCacheEnabledPreferenceKey];
}

- (BOOL)zoomsTextOnly
{
    return [self _boolValueForKey:CyberKitZoomsTextOnlyPreferenceKey];
}

- (void)setZoomsTextOnly:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitZoomsTextOnlyPreferenceKey];
}

- (BOOL)javaScriptCanAccessClipboard
{
    return [self _boolValueForKey:CyberKitJavaScriptCanAccessClipboardPreferenceKey];
}

- (void)setJavaScriptCanAccessClipboard:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitJavaScriptCanAccessClipboardPreferenceKey];
}

#if !PLATFORM(IOS_FAMILY)
- (BOOL)respectStandardStyleKeyEquivalents
{
    return [self _boolValueForKey:CyberKitRespectStandardStyleKeyEquivalentsPreferenceKey];
}

- (void)setRespectStandardStyleKeyEquivalents:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitRespectStandardStyleKeyEquivalentsPreferenceKey];
}

- (BOOL)showsURLsInToolTips
{
    return [self _boolValueForKey:CyberKitShowsURLsInToolTipsPreferenceKey];
}

- (void)setShowsURLsInToolTips:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitShowsURLsInToolTipsPreferenceKey];
}

- (BOOL)showsToolTipOverTruncatedText
{
    return [self _boolValueForKey:CyberKitShowsToolTipOverTruncatedTextPreferenceKey];
}

- (void)setShowsToolTipOverTruncatedText:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitShowsToolTipOverTruncatedTextPreferenceKey];
}

- (BOOL)textAreasAreResizable
{
    return [self _boolValueForKey: CyberKitTextAreasAreResizablePreferenceKey];
}

- (void)setTextAreasAreResizable:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitTextAreasAreResizablePreferenceKey];
}
#endif // !PLATFORM(IOS_FAMILY)

- (BOOL)shrinksStandaloneImagesToFit
{
    return [self _boolValueForKey:CyberKitShrinksStandaloneImagesToFitPreferenceKey];
}

- (void)setShrinksStandaloneImagesToFit:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitShrinksStandaloneImagesToFitPreferenceKey];
}

- (BOOL)automaticallyDetectsCacheModel
{
    return _private->automaticallyDetectsCacheModel;
}

- (void)setAutomaticallyDetectsCacheModel:(BOOL)automaticallyDetectsCacheModel
{
    _private->automaticallyDetectsCacheModel = automaticallyDetectsCacheModel;
}

- (BOOL)usesEncodingDetector
{
    return [self _boolValueForKey: CyberKitUsesEncodingDetectorPreferenceKey];
}

- (void)setUsesEncodingDetector:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitUsesEncodingDetectorPreferenceKey];
}

- (BOOL)isWebSecurityEnabled
{
    return [self _boolValueForKey: CyberKitWebSecurityEnabledPreferenceKey];
}

- (void)setWebSecurityEnabled:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitWebSecurityEnabledPreferenceKey];
}

- (BOOL)allowUniversalAccessFromFileURLs
{
    return [self _boolValueForKey: CyberKitAllowUniversalAccessFromFileURLsPreferenceKey];
}

- (void)setAllowUniversalAccessFromFileURLs:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitAllowUniversalAccessFromFileURLsPreferenceKey];
}

- (BOOL)allowFileAccessFromFileURLs
{
    return [self _boolValueForKey: CyberKitAllowFileAccessFromFileURLsPreferenceKey];
}

- (void)setAllowFileAccessFromFileURLs:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitAllowFileAccessFromFileURLsPreferenceKey];
}

- (BOOL)allowTopNavigationToDataURLs
{
    return [self _boolValueForKey: CyberKitAllowTopNavigationToDataURLsPreferenceKey];
}

- (void)setAllowTopNavigationToDataURLs:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitAllowTopNavigationToDataURLsPreferenceKey];
}

- (BOOL)allowCrossOriginSubresourcesToAskForCredentials
{
    return [self _boolValueForKey:CyberKitAllowCrossOriginSubresourcesToAskForCredentialsKey];
}

- (void)setAllowCrossOriginSubresourcesToAskForCredentials:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAllowCrossOriginSubresourcesToAskForCredentialsKey];
}

- (BOOL)needsStorageAccessFromFileURLsQuirk
{
    return [self _boolValueForKey: CyberKitNeedsStorageAccessFromFileURLsQuirkKey];
}

-(void)setNeedsStorageAccessFromFileURLsQuirk:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitNeedsStorageAccessFromFileURLsQuirkKey];
}

- (NSTimeInterval)_backForwardCacheExpirationInterval
{
    return (NSTimeInterval)[self _floatValueForKey:CyberKitBackForwardCacheExpirationIntervalKey];
}

#if PLATFORM(IOS_FAMILY)
- (BOOL)_standalone
{
    return [self _boolValueForKey:CyberKitStandalonePreferenceKey];
}

- (void)_setStandalone:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitStandalonePreferenceKey];
}

- (void)_setTelephoneNumberParsingEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitTelephoneParsingEnabledPreferenceKey];
}

- (BOOL)_telephoneNumberParsingEnabled
{
    return [self _boolValueForKey:CyberKitTelephoneParsingEnabledPreferenceKey];
}

- (BOOL)contentChangeObserverEnabled
{
    return [self _boolValueForKey:CyberKitContentChangeObserverEnabledPreferenceKey];
}

- (void)setContentChangeObserverEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitContentChangeObserverEnabledPreferenceKey];
}
#endif

#if ENABLE(TEXT_AUTOSIZING)
- (void)_setMinimumZoomFontSize:(float)size
{
    [self _setFloatValue:size forKey:CyberKitMinimumZoomFontSizePreferenceKey];
}

- (float)_minimumZoomFontSize
{
    return [self _floatValueForKey:CyberKitMinimumZoomFontSizePreferenceKey];
}

- (void)_setTextAutosizingEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitTextAutosizingEnabledPreferenceKey];
}

- (BOOL)_textAutosizingEnabled
{
    return [self _boolValueForKey:CyberKitTextAutosizingEnabledPreferenceKey];
}
#endif

#if PLATFORM(IOS_FAMILY)
- (void)_setMaxParseDuration:(float)d
{
    [self _setFloatValue:d forKey:CyberKitMaxParseDurationPreferenceKey];
}

- (float)_maxParseDuration
{
    return [self _floatValueForKey:CyberKitMaxParseDurationPreferenceKey];
}

- (void)_setAllowMultiElementImplicitFormSubmission:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAllowMultiElementImplicitFormSubmissionPreferenceKey];
}

- (BOOL)_allowMultiElementImplicitFormSubmission
{
    return [self _boolValueForKey:CyberKitAllowMultiElementImplicitFormSubmissionPreferenceKey];
}

- (void)_setAlwaysRequestGeolocationPermission:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAlwaysRequestGeolocationPermissionPreferenceKey];
}

- (BOOL)_alwaysRequestGeolocationPermission
{
    return [self _boolValueForKey:CyberKitAlwaysRequestGeolocationPermissionPreferenceKey];
}

- (void)_setInterpolationQuality:(int)quality
{
    [self _setIntegerValue:quality forKey:CyberKitInterpolationQualityPreferenceKey];
}

- (int)_interpolationQuality
{
    return [self _integerValueForKey:CyberKitInterpolationQualityPreferenceKey];
}

- (BOOL)_allowPasswordEcho
{
    return [self _boolValueForKey:CyberKitPasswordEchoEnabledPreferenceKey];
}

- (float)_passwordEchoDuration
{
    return [self _floatValueForKey:CyberKitPasswordEchoDurationPreferenceKey];
}

#endif // PLATFORM(IOS_FAMILY)

#if !PLATFORM(IOS_FAMILY)
- (float)PDFScaleFactor
{
    return [self _floatValueForKey:CyberKitPDFScaleFactorPreferenceKey];
}

- (void)setPDFScaleFactor:(float)factor
{
    [self _setFloatValue:factor forKey:CyberKitPDFScaleFactorPreferenceKey];
}
#endif

- (int64_t)applicationCacheTotalQuota
{
    return [self _longLongValueForKey:CyberKitApplicationCacheTotalQuota];
}

- (void)setApplicationCacheTotalQuota:(int64_t)quota
{
    [self _setLongLongValue:quota forKey:CyberKitApplicationCacheTotalQuota];

    // Application Cache Preferences are stored on the global cache storage manager, not in Settings.
    [WebApplicationCache setMaximumSize:quota];
}

- (int64_t)applicationCacheDefaultOriginQuota
{
    return [self _longLongValueForKey:CyberKitApplicationCacheDefaultOriginQuota];
}

- (void)setApplicationCacheDefaultOriginQuota:(int64_t)quota
{
    [self _setLongLongValue:quota forKey:CyberKitApplicationCacheDefaultOriginQuota];
}

#if !PLATFORM(IOS_FAMILY)
- (PDFDisplayMode)PDFDisplayMode
{
    PDFDisplayMode value = static_cast<PDFDisplayMode>([self _integerValueForKey:CyberKitPDFDisplayModePreferenceKey]);
    if (value != kPDFDisplaySinglePage && value != kPDFDisplaySinglePageContinuous && value != kPDFDisplayTwoUp && value != kPDFDisplayTwoUpContinuous) {
        // protect against new modes from future versions of OS X stored in defaults
        value = kPDFDisplaySinglePageContinuous;
    }
    return value;
}

- (void)setPDFDisplayMode:(PDFDisplayMode)mode
{
    [self _setIntegerValue:mode forKey:CyberKitPDFDisplayModePreferenceKey];
}
#endif

- (CyberKitEditableLinkBehavior)editableLinkBehavior
{
    CyberKitEditableLinkBehavior value = static_cast<CyberKitEditableLinkBehavior> ([self _integerValueForKey:CyberKitEditableLinkBehaviorPreferenceKey]);
    if (value != CyberKitEditableLinkDefaultBehavior &&
        value != CyberKitEditableLinkAlwaysLive &&
        value != CyberKitEditableLinkNeverLive &&
        value != CyberKitEditableLinkOnlyLiveWithShiftKey &&
        value != CyberKitEditableLinkLiveWhenNotFocused) {
        // ensure that a valid result is returned
        value = CyberKitEditableLinkDefaultBehavior;
    }

    return value;
}

- (void)setEditableLinkBehavior:(CyberKitEditableLinkBehavior)behavior
{
    [self _setIntegerValue:behavior forKey:CyberKitEditableLinkBehaviorPreferenceKey];
}

- (WebTextDirectionSubmenuInclusionBehavior)textDirectionSubmenuInclusionBehavior
{
    auto value = static_cast<WebTextDirectionSubmenuInclusionBehavior>([self _integerValueForKey:CyberKitTextDirectionSubmenuInclusionBehaviorPreferenceKey]);
    if (value != WebTextDirectionSubmenuNeverIncluded &&
        value != WebTextDirectionSubmenuAutomaticallyIncluded &&
        value != WebTextDirectionSubmenuAlwaysIncluded) {
        // Ensure that a valid result is returned.
        value = WebTextDirectionSubmenuNeverIncluded;
    }
    return value;
}

- (void)setTextDirectionSubmenuInclusionBehavior:(WebTextDirectionSubmenuInclusionBehavior)behavior
{
    [self _setIntegerValue:behavior forKey:CyberKitTextDirectionSubmenuInclusionBehaviorPreferenceKey];
}

- (BOOL)_useSiteSpecificSpoofing
{
    return [self _boolValueForKey:CyberKitUseSiteSpecificSpoofingPreferenceKey];
}

- (void)_setUseSiteSpecificSpoofing:(BOOL)newValue
{
    [self _setBoolValue:newValue forKey:CyberKitUseSiteSpecificSpoofingPreferenceKey];
}

- (BOOL)databasesEnabled
{
    return [self _boolValueForKey:CyberKitDatabasesEnabledPreferenceKey];
}

- (void)setDatabasesEnabled:(BOOL)databasesEnabled
{
    [self _setBoolValue:databasesEnabled forKey:CyberKitDatabasesEnabledPreferenceKey];
}

#if PLATFORM(IOS_FAMILY)
- (BOOL)storageTrackerEnabled
{
    return [self _boolValueForKey:CyberKitStorageTrackerEnabledPreferenceKey];
}

- (void)setStorageTrackerEnabled:(BOOL)storageTrackerEnabled
{
    [self _setBoolValue:storageTrackerEnabled forKey:CyberKitStorageTrackerEnabledPreferenceKey];
}
#endif

- (BOOL)localStorageEnabled
{
    return [self _boolValueForKey:CyberKitLocalStorageEnabledPreferenceKey];
}

- (void)setLocalStorageEnabled:(BOOL)localStorageEnabled
{
    [self _setBoolValue:localStorageEnabled forKey:CyberKitLocalStorageEnabledPreferenceKey];
}

+ (WebPreferences *)_getInstanceForIdentifier:(NSString *)ident
{
    LOG(Encoding, "requesting for %@\n", ident);

    if (!ident)
        return standardPreferences().get();

    WebPreferences *instance = [webPreferencesInstances() objectForKey:[self _concatenateKeyWithIBCreatorID:ident]];

    return instance;
}

+ (void)_setInstance:(WebPreferences *)instance forIdentifier:(NSString *)ident
{
    if (!ident)
        return;

    auto& instances = webPreferencesInstances();
    if (!instances)
        instances = adoptNS([[NSMutableDictionary alloc] init]);
    [instances setObject:instance forKey:[self _concatenateKeyWithIBCreatorID:ident]];
    LOG(Encoding, "recording %p for %@\n", instance, [self _concatenateKeyWithIBCreatorID:ident]);
}

+ (void)_checkLastReferenceForIdentifier:(id)identifier
{
    // FIXME: This won't work at all under garbage collection because retainCount returns a constant.
    // We may need to change WebPreferences API so there's an explicit way to end the lifetime of one.
    WebPreferences *instance = [webPreferencesInstances() objectForKey:identifier];
    if ([instance retainCount] == 1)
        [webPreferencesInstances() removeObjectForKey:identifier];
}

+ (void)_removeReferenceForIdentifier:(NSString *)ident
{
    if (ident)
        [self performSelector:@selector(_checkLastReferenceForIdentifier:) withObject:[self _concatenateKeyWithIBCreatorID:ident] afterDelay:0.1];
}

- (void)_startBatchingUpdates
{
    if (!_private->updateBatchCount)
        _private->updateAfterBatchType = { };

    _private->updateBatchCount++;
}

- (void)_stopBatchingUpdates
{
    ASSERT(_private->updateBatchCount > 0);
    if (_private->updateBatchCount <= 0)
        NSLog(@"ERROR: Unbalanced _startBatchingUpdates/_stopBatchingUpdates.");

    _private->updateBatchCount--;
    if (!_private->updateBatchCount) {
        if (_private->updateAfterBatchType.contains(UpdateAfterBatchType::Internal)) {
            if (_private->updateAfterBatchType.contains(UpdateAfterBatchType::API))
                [self _postPreferencesChangedNotification];
            else
                [self _postPreferencesChangedAPINotification];
        }
    }
}

- (void)_batchUpdatePreferencesInBlock:(void (^)(WebPreferences *))block
{
    [self _startBatchingUpdates];
    block(self);
    [self _stopBatchingUpdates];
}

- (void)_resetForTesting
{
    _private->values = adoptNS([[NSMutableDictionary alloc] init]);
    [self _postPreferencesChangedNotification];
}

- (void)_postPreferencesChangedNotification
{
#if !PLATFORM(IOS_FAMILY)
    if (!pthread_main_np()) {
        [self performSelectorOnMainThread:_cmd withObject:nil waitUntilDone:NO];
        return;
    }
#endif

    if (_private->updateBatchCount) {
        _private->updateAfterBatchType.add({ UpdateAfterBatchType::API, UpdateAfterBatchType::Internal });
        return;
    }

    [[NSNotificationCenter defaultCenter] postNotificationName:WebPreferencesChangedInternalNotification object:self userInfo:nil];
    [[NSNotificationCenter defaultCenter] postNotificationName:WebPreferencesChangedNotification object:self userInfo:nil];
}

- (void)_postPreferencesChangedAPINotification
{
    if (!pthread_main_np()) {
        [self performSelectorOnMainThread:_cmd withObject:nil waitUntilDone:NO];
        return;
    }

    if (_private->updateBatchCount) {
        _private->updateAfterBatchType.add({ UpdateAfterBatchType::Internal });
        return;
    }

    [[NSNotificationCenter defaultCenter] postNotificationName:WebPreferencesChangedNotification object:self userInfo:nil];
}

+ (CFStringEncoding)_systemCFStringEncoding
{
    return PAL::webDefaultCFStringEncoding();
}

+ (void)_setInitialDefaultTextEncodingToSystemEncoding
{
    [[NSUserDefaults standardUserDefaults] registerDefaults:
        @{ CyberKitDefaultTextEncodingNamePreferenceKey: PAL::defaultTextEncodingNameForSystemLanguage() }];
}

static RetainPtr<NSString>& classIBCreatorID()
{
    static NeverDestroyed<RetainPtr<NSString>> classIBCreatorID;
    return classIBCreatorID;
}

+ (void)_setIBCreatorID:(NSString *)string
{
    classIBCreatorID() = adoptNS([string copy]);
}

- (BOOL)isDOMPasteAllowed
{
    return [self _boolValueForKey:CyberKitDOMPasteAllowedPreferenceKey];
}

- (void)setDOMPasteAllowed:(BOOL)DOMPasteAllowed
{
    [self _setBoolValue:DOMPasteAllowed forKey:CyberKitDOMPasteAllowedPreferenceKey];
}

- (NSString *)_localStorageDatabasePath
{
    return [[self _stringValueForKey:CyberKitLocalStorageDatabasePathPreferenceKey] stringByStandardizingPath];
}

- (void)_setLocalStorageDatabasePath:(NSString *)path
{
    [self _setStringValue:[path stringByStandardizingPath] forKey:CyberKitLocalStorageDatabasePathPreferenceKey];
}

- (NSString *)_ftpDirectoryTemplatePath
{
    return [[self _stringValueForKey:CyberKitFTPDirectoryTemplatePath] stringByStandardizingPath];
}

- (void)_setFTPDirectoryTemplatePath:(NSString *)path
{
    [self _setStringValue:[path stringByStandardizingPath] forKey:CyberKitFTPDirectoryTemplatePath];
}

- (BOOL)_forceFTPDirectoryListings
{
    return [self _boolValueForKey:CyberKitForceFTPDirectoryListings];
}

- (void)_setForceFTPDirectoryListings:(BOOL)force
{
    [self _setBoolValue:force forKey:CyberKitForceFTPDirectoryListings];
}

- (BOOL)acceleratedDrawingEnabled
{
    return [self _boolValueForKey:CyberKitAcceleratedDrawingEnabledPreferenceKey];
}

- (void)setAcceleratedDrawingEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitAcceleratedDrawingEnabledPreferenceKey];
}

- (BOOL)displayListDrawingEnabled
{
    return [self _boolValueForKey:CyberKitDisplayListDrawingEnabledPreferenceKey];
}

- (void)setDisplayListDrawingEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitDisplayListDrawingEnabledPreferenceKey];
}

- (BOOL)resourceLoadStatisticsEnabled
{
    return [self _boolValueForKey:CyberKitResourceLoadStatisticsEnabledPreferenceKey];
}

- (void)setResourceLoadStatisticsEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitResourceLoadStatisticsEnabledPreferenceKey];
}

- (BOOL)largeImageAsyncDecodingEnabled
{
    return [self _boolValueForKey:CyberKitLargeImageAsyncDecodingEnabledPreferenceKey];
}

- (void)setLargeImageAsyncDecodingEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitLargeImageAsyncDecodingEnabledPreferenceKey];
}

- (BOOL)animatedImageAsyncDecodingEnabled
{
    return [self _boolValueForKey:CyberKitAnimatedImageAsyncDecodingEnabledPreferenceKey];
}

- (void)setAnimatedImageAsyncDecodingEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitAnimatedImageAsyncDecodingEnabledPreferenceKey];
}

- (BOOL)canvasUsesAcceleratedDrawing
{
    return [self _boolValueForKey:CyberKitCanvasUsesAcceleratedDrawingPreferenceKey];
}

- (void)setCanvasUsesAcceleratedDrawing:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitCanvasUsesAcceleratedDrawingPreferenceKey];
}

- (BOOL)acceleratedCompositingEnabled
{
    return [self _boolValueForKey:CyberKitAcceleratedCompositingEnabledPreferenceKey];
}

- (void)setAcceleratedCompositingEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitAcceleratedCompositingEnabledPreferenceKey];
}

- (BOOL)showDebugBorders
{
    return [self _boolValueForKey:CyberKitShowDebugBordersPreferenceKey];
}

- (void)setShowDebugBorders:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitShowDebugBordersPreferenceKey];
}

- (BOOL)legacyLineLayoutVisualCoverageEnabled
{
    return [self _boolValueForKey:CyberKitLegacyLineLayoutVisualCoverageEnabledPreferenceKey];
}

- (void)setLegacyLineLayoutVisualCoverageEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitLegacyLineLayoutVisualCoverageEnabledPreferenceKey];
}

- (BOOL)showRepaintCounter
{
    return [self _boolValueForKey:CyberKitShowRepaintCounterPreferenceKey];
}

- (void)setShowRepaintCounter:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitShowRepaintCounterPreferenceKey];
}

- (BOOL)webAudioEnabled
{
    return [self _boolValueForKey:CyberKitWebAudioEnabledPreferenceKey];
}

- (void)setWebAudioEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitWebAudioEnabledPreferenceKey];
}

- (BOOL)webGLEnabled
{
    return [self _boolValueForKey:CyberKitWebGLEnabledPreferenceKey];
}

- (void)setWebGLEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitWebGLEnabledPreferenceKey];
}

- (BOOL)forceLowPowerGPUForWebGL
{
    return [self _boolValueForKey:CyberKitForceWebGLUsesLowPowerPreferenceKey];
}

- (void)setForceWebGLUsesLowPower:(BOOL)forceLowPower
{
    [self _setBoolValue:forceLowPower forKey:CyberKitForceWebGLUsesLowPowerPreferenceKey];
}

- (BOOL)isFrameFlatteningEnabled
{
    return [self _unsignedIntValueForKey:CyberKitFrameFlatteningPreferenceKey] != CyberKitFrameFlatteningDisabled;
}

- (void)setFrameFlatteningEnabled:(BOOL)flattening
{
    CyberKitFrameFlattening value = flattening ? CyberKitFrameFlatteningFullyEnabled : CyberKitFrameFlatteningDisabled;
    [self _setUnsignedIntValue:value forKey:CyberKitFrameFlatteningPreferenceKey];
}

- (CyberKitFrameFlattening)frameFlattening
{
    return static_cast<CyberKitFrameFlattening>([self _unsignedIntValueForKey:CyberKitFrameFlatteningPreferenceKey]);
}

- (void)setFrameFlattening:(CyberKitFrameFlattening)flattening
{
    [self _setUnsignedIntValue:flattening forKey:CyberKitFrameFlatteningPreferenceKey];
}

- (BOOL)asyncFrameScrollingEnabled
{
    return [self _boolValueForKey:CyberKitAsyncFrameScrollingEnabledPreferenceKey];
}

- (void)setAsyncFrameScrollingEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitAsyncFrameScrollingEnabledPreferenceKey];
}

- (BOOL)isSpatialNavigationEnabled
{
    return [self _boolValueForKey:CyberKitSpatialNavigationEnabledPreferenceKey];
}

- (void)setSpatialNavigationEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitSpatialNavigationEnabledPreferenceKey];
}

- (BOOL)hyperlinkAuditingEnabled
{
    return [self _boolValueForKey:CyberKitHyperlinkAuditingEnabledPreferenceKey];
}

- (void)setHyperlinkAuditingEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitHyperlinkAuditingEnabledPreferenceKey];
}

- (BOOL)usePreHTML5ParserQuirks
{
    return [self _boolValueForKey:CyberKitUsePreHTML5ParserQuirksKey];
}

- (void)setUsePreHTML5ParserQuirks:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitUsePreHTML5ParserQuirksKey];
}

- (void)didRemoveFromWebView
{
    ASSERT(_private->numWebViews);
    if (--_private->numWebViews == 0)
        [[NSNotificationCenter defaultCenter]
            postNotificationName:WebPreferencesRemovedNotification
                          object:self
                        userInfo:nil];
}

- (void)willAddToWebView
{
    ++_private->numWebViews;
}

- (void)setFullScreenEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitFullScreenEnabledPreferenceKey];
}

- (BOOL)fullScreenEnabled
{
    return [self _boolValueForKey:CyberKitFullScreenEnabledPreferenceKey];
}

- (void)setAsynchronousSpellCheckingEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAsynchronousSpellCheckingEnabledPreferenceKey];
}

- (BOOL)asynchronousSpellCheckingEnabled
{
    return [self _boolValueForKey:CyberKitAsynchronousSpellCheckingEnabledPreferenceKey];
}

+ (void)setCyberKitLinkTimeVersion:(int)version
{
    setCyberKitLinkTimeVersion(version);
}

- (void)setLoadsSiteIconsIgnoringImageLoadingPreference: (BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitLoadSiteIconsKey];
}

- (BOOL)loadsSiteIconsIgnoringImageLoadingPreference
{
    return [self _boolValueForKey: CyberKitLoadSiteIconsKey];
}

- (void)setAVFoundationEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAVFoundationEnabledKey];
}

- (BOOL)isAVFoundationEnabled
{
    return [self _boolValueForKey:CyberKitAVFoundationEnabledKey];
}

- (void)setAVFoundationNSURLSessionEnabled:(BOOL)flag
{
}

- (BOOL)isAVFoundationNSURLSessionEnabled
{
    return YES;
}

- (BOOL)isInheritURIQueryComponentEnabled
{
    return [self _boolValueForKey: CyberKitEnableInheritURIQueryComponentPreferenceKey];
}

- (void)setEnableInheritURIQueryComponent:(BOOL)flag
{
    [self _setBoolValue:flag forKey: CyberKitEnableInheritURIQueryComponentPreferenceKey];
}

#if PLATFORM(IOS_FAMILY)
- (BOOL)mediaPlaybackAllowsAirPlay
{
    return [self _boolValueForKey:CyberKitAllowsAirPlayForMediaPlaybackPreferenceKey];
}

- (void)setMediaPlaybackAllowsAirPlay:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAllowsAirPlayForMediaPlaybackPreferenceKey];
}

- (unsigned)audioSessionCategoryOverride
{
    return [self _unsignedIntValueForKey:CyberKitAudioSessionCategoryOverride];
}

- (void)setAudioSessionCategoryOverride:(unsigned)override
{
    if (override > static_cast<unsigned>(AudioSession::CategoryType::AudioProcessing)) {
        // Clients are passing us OSTypes values from AudioToolbox/AudioSession.h,
        // which need to be translated into AudioSession::CategoryType:
        switch (override) {
        case CyberKitAudioSessionCategoryAmbientSound:
            override = static_cast<unsigned>(AudioSession::CategoryType::AmbientSound);
            break;
        case CyberKitAudioSessionCategorySoloAmbientSound:
            override = static_cast<unsigned>(AudioSession::CategoryType::SoloAmbientSound);
            break;
        case CyberKitAudioSessionCategoryMediaPlayback:
            override = static_cast<unsigned>(AudioSession::CategoryType::MediaPlayback);
            break;
        case CyberKitAudioSessionCategoryRecordAudio:
            override = static_cast<unsigned>(AudioSession::CategoryType::RecordAudio);
            break;
        case CyberKitAudioSessionCategoryPlayAndRecord:
            override = static_cast<unsigned>(AudioSession::CategoryType::PlayAndRecord);
            break;
        case CyberKitAudioSessionCategoryAudioProcessing:
            override = static_cast<unsigned>(AudioSession::CategoryType::AudioProcessing);
            break;
        default:
            override = static_cast<unsigned>(AudioSession::CategoryType::None);
            break;
        }
    }

    [self _setUnsignedIntValue:override forKey:CyberKitAudioSessionCategoryOverride];
}

- (BOOL)networkDataUsageTrackingEnabled
{
    return [self _boolValueForKey:CyberKitNetworkDataUsageTrackingEnabledPreferenceKey];
}

- (void)setNetworkDataUsageTrackingEnabled:(BOOL)trackingEnabled
{
    [self _setBoolValue:trackingEnabled forKey:CyberKitNetworkDataUsageTrackingEnabledPreferenceKey];
}

- (NSString *)networkInterfaceName
{
    return [self _stringValueForKey:CyberKitNetworkInterfaceNamePreferenceKey];
}

- (void)setNetworkInterfaceName:(NSString *)name
{
    [self _setStringValue:name forKey:CyberKitNetworkInterfaceNamePreferenceKey];
}
#endif // PLATFORM(IOS_FAMILY)

// Deprecated. Use -videoPlaybackRequiresUserGesture and -audioPlaybackRequiresUserGesture instead.
- (BOOL)mediaPlaybackRequiresUserGesture
{
    return [self _boolValueForKey:CyberKitRequiresUserGestureForMediaPlaybackPreferenceKey];
}

// Deprecated. Use -setVideoPlaybackRequiresUserGesture and -setAudioPlaybackRequiresUserGesture instead.
- (void)setMediaPlaybackRequiresUserGesture:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitRequiresUserGestureForMediaPlaybackPreferenceKey];
}

- (BOOL)videoPlaybackRequiresUserGesture
{
    return [self _boolValueForKey:CyberKitRequiresUserGestureForVideoPlaybackPreferenceKey];
}

- (void)setVideoPlaybackRequiresUserGesture:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitRequiresUserGestureForVideoPlaybackPreferenceKey];
}

- (BOOL)audioPlaybackRequiresUserGesture
{
    return [self _boolValueForKey:CyberKitRequiresUserGestureForAudioPlaybackPreferenceKey];
}

- (void)setAudioPlaybackRequiresUserGesture:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitRequiresUserGestureForAudioPlaybackPreferenceKey];
}

- (BOOL)overrideUserGestureRequirementForMainContent
{
    return [self _boolValueForKey:CyberKitMainContentUserGestureOverrideEnabledPreferenceKey];
}

- (void)setOverrideUserGestureRequirementForMainContent:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMainContentUserGestureOverrideEnabledPreferenceKey];
}

- (BOOL)mediaPlaybackAllowsInline
{
    return [self _boolValueForKey:CyberKitAllowsInlineMediaPlaybackPreferenceKey];
}

- (void)setMediaPlaybackAllowsInline:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAllowsInlineMediaPlaybackPreferenceKey];
}

- (BOOL)inlineMediaPlaybackRequiresPlaysInlineAttribute
{
    return [self _boolValueForKey:CyberKitInlineMediaPlaybackRequiresPlaysInlineAttributeKey];
}

- (void)setInlineMediaPlaybackRequiresPlaysInlineAttribute:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitInlineMediaPlaybackRequiresPlaysInlineAttributeKey];
}

- (BOOL)invisibleAutoplayNotPermitted
{
    return [self _boolValueForKey:CyberKitInvisibleAutoplayNotPermittedKey];
}

- (void)setInvisibleAutoplayNotPermitted:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitInvisibleAutoplayNotPermittedKey];
}

- (BOOL)mediaControlsScaleWithPageZoom
{
    return [self _boolValueForKey:CyberKitMediaControlsScaleWithPageZoomPreferenceKey];
}

- (void)setMediaControlsScaleWithPageZoom:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMediaControlsScaleWithPageZoomPreferenceKey];
}

- (BOOL)allowsAlternateFullscreen
{
    return [self allowsPictureInPictureMediaPlayback];
}

- (void)setAllowsAlternateFullscreen:(BOOL)flag
{
    [self setAllowsPictureInPictureMediaPlayback:flag];
}

- (BOOL)allowsPictureInPictureMediaPlayback
{
    return [self _boolValueForKey:CyberKitAllowsPictureInPictureMediaPlaybackPreferenceKey];
}

- (void)setAllowsPictureInPictureMediaPlayback:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAllowsPictureInPictureMediaPlaybackPreferenceKey];
}

- (BOOL)mockScrollbarsEnabled
{
    return [self _boolValueForKey:CyberKitMockScrollbarsEnabledPreferenceKey];
}

- (void)setMockScrollbarsEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMockScrollbarsEnabledPreferenceKey];
}

- (NSString *)pictographFontFamily
{
    return [self _stringValueForKey: CyberKitPictographFontPreferenceKey];
}

- (void)setPictographFontFamily:(NSString *)family
{
    [self _setStringValue: family forKey: CyberKitPictographFontPreferenceKey];
}

- (BOOL)pageCacheSupportsPlugins
{
    return [self _boolValueForKey:CyberKitPageCacheSupportsPluginsPreferenceKey];
}

- (void)setPageCacheSupportsPlugins:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitPageCacheSupportsPluginsPreferenceKey];

}

#if PLATFORM(IOS_FAMILY)
- (void)_invalidateCachedPreferences
{
    dispatch_barrier_sync(_private->readWriteQueue.get(), ^{
        if (_private->values)
            _private->values = adoptNS([[NSMutableDictionary alloc] init]);
    });

    [self _updatePrivateBrowsingStateTo:[self privateBrowsingEnabled]];

    // Tell any live WebViews to refresh their preferences
    [self _postPreferencesChangedNotification];
}

- (void)_synchronizeWebStoragePolicyWithCookiePolicy
{
    // FIXME: This should be done in clients, CyberKit shouldn't be making such policy decisions.

    NSHTTPCookieAcceptPolicy cookieAcceptPolicy = [[NSHTTPCookieStorage sharedHTTPCookieStorage] cookieAcceptPolicy];
    WebStorageBlockingPolicy storageBlockingPolicy;
    switch (static_cast<unsigned>(cookieAcceptPolicy)) {
    case NSHTTPCookieAcceptPolicyAlways:
        storageBlockingPolicy = WebAllowAllStorage;
        break;
    case NSHTTPCookieAcceptPolicyOnlyFromMainDocumentDomain:
    case NSHTTPCookieAcceptPolicyExclusivelyFromMainDocumentDomain:
        storageBlockingPolicy = WebBlockThirdPartyStorage;
        break;
    case NSHTTPCookieAcceptPolicyNever:
        storageBlockingPolicy = WebBlockAllStorage;
        break;
    default:
        ASSERT_NOT_REACHED();
        storageBlockingPolicy = WebBlockAllStorage;
        break;
    }

    [self setStorageBlockingPolicy:storageBlockingPolicy];
}
#endif

- (void)setBackspaceKeyNavigationEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitBackspaceKeyNavigationEnabledKey];
}

- (BOOL)backspaceKeyNavigationEnabled
{
    return [self _boolValueForKey:CyberKitBackspaceKeyNavigationEnabledKey];
}

- (void)setWantsBalancedSetDefersLoadingBehavior:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitWantsBalancedSetDefersLoadingBehaviorKey];
}

- (BOOL)wantsBalancedSetDefersLoadingBehavior
{
    return [self _boolValueForKey:CyberKitWantsBalancedSetDefersLoadingBehaviorKey];
}

- (void)setShouldDisplaySubtitles:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitShouldDisplaySubtitlesPreferenceKey];
}

- (BOOL)shouldDisplaySubtitles
{
    return [self _boolValueForKey:CyberKitShouldDisplaySubtitlesPreferenceKey];
}

- (void)setShouldDisplayCaptions:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitShouldDisplayCaptionsPreferenceKey];
}

- (BOOL)shouldDisplayCaptions
{
    return [self _boolValueForKey:CyberKitShouldDisplayCaptionsPreferenceKey];
}

- (void)setShouldDisplayTextDescriptions:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitShouldDisplayTextDescriptionsPreferenceKey];
}

- (BOOL)shouldDisplayTextDescriptions
{
    return [self _boolValueForKey:CyberKitShouldDisplayTextDescriptionsPreferenceKey];
}

- (void)setNotificationsEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitNotificationsEnabledKey];
}

- (BOOL)notificationsEnabled
{
    return [self _boolValueForKey:CyberKitNotificationsEnabledKey];
}

- (void)setShouldRespectImageOrientation:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitShouldRespectImageOrientationKey];
}

- (BOOL)shouldRespectImageOrientation
{
    return [self _boolValueForKey:CyberKitShouldRespectImageOrientationKey];
}

- (void)setIncrementalRenderingSuppressionTimeoutInSeconds:(NSTimeInterval)timeout
{
    [self _setFloatValue:timeout forKey:CyberKitIncrementalRenderingSuppressionTimeoutInSecondsKey];
}

- (NSTimeInterval)incrementalRenderingSuppressionTimeoutInSeconds
{
    return [self _floatValueForKey:CyberKitIncrementalRenderingSuppressionTimeoutInSecondsKey];
}

- (BOOL)diagnosticLoggingEnabled
{
    return [self _boolValueForKey:CyberKitDiagnosticLoggingEnabledKey];
}

- (void)setDiagnosticLoggingEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitDiagnosticLoggingEnabledKey];
}

- (void)setStorageBlockingPolicy:(WebStorageBlockingPolicy)storageBlockingPolicy
{
#if PLATFORM(IOS_FAMILY)
    // We don't want to write the setting out, so we just reset the default instead of storing the new setting.
    // FIXME: This code removes any defaults previously registered by client process, which is not appropriate for this method to do.
    [[NSUserDefaults standardUserDefaults] registerDefaults:@{ CyberKitStorageBlockingPolicyKey: @(storageBlockingPolicy) }];
#else
    [self _setIntegerValue:storageBlockingPolicy forKey:CyberKitStorageBlockingPolicyKey];
#endif
}

- (WebStorageBlockingPolicy)storageBlockingPolicy
{
    return static_cast<WebStorageBlockingPolicy>([self _integerValueForKey:CyberKitStorageBlockingPolicyKey]);
}

- (BOOL)plugInSnapshottingEnabled
{
    return NO;
}

- (void)setPlugInSnapshottingEnabled:(BOOL)enabled
{
}

- (BOOL)hiddenPageDOMTimerThrottlingEnabled
{
    return [self _boolValueForKey:CyberKitHiddenPageDOMTimerThrottlingEnabledPreferenceKey];
}

- (void)setHiddenPageDOMTimerThrottlingEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitHiddenPageDOMTimerThrottlingEnabledPreferenceKey];
}

- (BOOL)hiddenPageCSSAnimationSuspensionEnabled
{
    return [self _boolValueForKey:CyberKitHiddenPageCSSAnimationSuspensionEnabledPreferenceKey];
}

- (void)setHiddenPageCSSAnimationSuspensionEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitHiddenPageCSSAnimationSuspensionEnabledPreferenceKey];
}

- (BOOL)lowPowerVideoAudioBufferSizeEnabled
{
    return [self _boolValueForKey:CyberKitLowPowerVideoAudioBufferSizeEnabledPreferenceKey];
}

- (void)setLowPowerVideoAudioBufferSizeEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitLowPowerVideoAudioBufferSizeEnabledPreferenceKey];
}

- (BOOL)useLegacyTextAlignPositionedElementBehavior
{
    return [self _boolValueForKey:CyberKitUseLegacyTextAlignPositionedElementBehaviorPreferenceKey];
}

- (void)setUseLegacyTextAlignPositionedElementBehavior:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitUseLegacyTextAlignPositionedElementBehaviorPreferenceKey];
}

- (BOOL)mediaSourceEnabled
{
    return [self _boolValueForKey:CyberKitMediaSourceEnabledPreferenceKey];
}

- (void)setMediaSourceEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitMediaSourceEnabledPreferenceKey];
}

- (BOOL)sourceBufferChangeTypeEnabled
{
    return [self _boolValueForKey:CyberKitSourceBufferChangeTypeEnabledPreferenceKey];
}

- (void)setSourceBufferChangeTypeEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitSourceBufferChangeTypeEnabledPreferenceKey];
}

- (BOOL)imageControlsEnabled
{
    // Image controls are no longer supported.
    return NO;
}

- (void)setImageControlsEnabled:(BOOL)enabled
{
    // Image controls are no longer supported.
    UNUSED_PARAM(enabled);
}

- (BOOL)serviceControlsEnabled
{
    return [self _boolValueForKey:CyberKitServiceControlsEnabledPreferenceKey];
}

- (void)setServiceControlsEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitServiceControlsEnabledPreferenceKey];
}

- (BOOL)gamepadsEnabled
{
    return [self _boolValueForKey:CyberKitGamepadsEnabledPreferenceKey];
}

- (void)setGamepadsEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitGamepadsEnabledPreferenceKey];
}

- (BOOL)shouldConvertPositionStyleOnCopy
{
    return [self _boolValueForKey:CyberKitShouldConvertPositionStyleOnCopyPreferenceKey];
}

- (void)setShouldConvertPositionStyleOnCopy:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitShouldConvertPositionStyleOnCopyPreferenceKey];
}

- (NSString *)mediaKeysStorageDirectory
{
    return [[self _stringValueForKey:CyberKitMediaKeysStorageDirectoryKey] stringByStandardizingPath];
}

- (void)setMediaKeysStorageDirectory:(NSString *)directory
{
    [self _setStringValue:directory forKey:CyberKitMediaKeysStorageDirectoryKey];
}

- (BOOL)mediaDevicesEnabled
{
    return [self _boolValueForKey:CyberKitMediaDevicesEnabledPreferenceKey];
}

- (void)setMediaDevicesEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMediaDevicesEnabledPreferenceKey];
}

- (BOOL)mediaStreamEnabled
{
    return [self _boolValueForKey:CyberKitMediaStreamEnabledPreferenceKey];
}

- (void)setMediaStreamEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMediaStreamEnabledPreferenceKey];
}

- (BOOL)peerConnectionEnabled
{
    return [self _boolValueForKey:CyberKitPeerConnectionEnabledPreferenceKey];
}

- (void)setPeerConnectionEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitPeerConnectionEnabledPreferenceKey];
}

- (BOOL)linkPreloadEnabled
{
    return [self _boolValueForKey:CyberKitLinkPreloadEnabledPreferenceKey];
}

- (void)setLinkPreloadEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitLinkPreloadEnabledPreferenceKey];
}

- (BOOL)mediaPreloadingEnabled
{
    return [self _boolValueForKey:CyberKitMediaPreloadingEnabledPreferenceKey];
}

- (void)setMediaPreloadingEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMediaPreloadingEnabledPreferenceKey];
}

- (void)setMetaRefreshEnabled:(BOOL)enabled
{
    [self setHTTPEquivEnabled:enabled];
}

- (BOOL)metaRefreshEnabled
{
    return [self httpEquivEnabled];
}

- (void)setHTTPEquivEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitHTTPEquivEnabledPreferenceKey];
}

- (BOOL)httpEquivEnabled
{
    return [self _boolValueForKey:CyberKitHTTPEquivEnabledPreferenceKey];
}

- (BOOL)javaScriptMarkupEnabled
{
    return [self _boolValueForKey:CyberKitJavaScriptMarkupEnabledPreferenceKey];
}

- (void)setJavaScriptMarkupEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitJavaScriptMarkupEnabledPreferenceKey];
}

- (BOOL)mediaDataLoadsAutomatically
{
    return [self _boolValueForKey:CyberKitMediaDataLoadsAutomaticallyPreferenceKey];
}

- (void)setMediaDataLoadsAutomatically:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMediaDataLoadsAutomaticallyPreferenceKey];
}

- (BOOL)attachmentElementEnabled
{
    return [self _boolValueForKey:CyberKitAttachmentElementEnabledPreferenceKey];
}

- (void)setAttachmentElementEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAttachmentElementEnabledPreferenceKey];
}

- (BOOL)allowsInlineMediaPlaybackAfterFullscreen
{
    return [self _boolValueForKey:CyberKitAllowsInlineMediaPlaybackAfterFullscreenPreferenceKey];
}

- (void)setAllowsInlineMediaPlaybackAfterFullscreen:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAllowsInlineMediaPlaybackAfterFullscreenPreferenceKey];
}

- (BOOL)mockCaptureDevicesEnabled
{
    return [self _boolValueForKey:CyberKitMockCaptureDevicesEnabledPreferenceKey];
}

- (void)setMockCaptureDevicesEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMockCaptureDevicesEnabledPreferenceKey];
}

- (BOOL)mockCaptureDevicesPromptEnabled
{
    return [self _boolValueForKey:CyberKitMockCaptureDevicesPromptEnabledPreferenceKey];
}

- (void)setMockCaptureDevicesPromptEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMockCaptureDevicesPromptEnabledPreferenceKey];
}

- (BOOL)enumeratingAllNetworkInterfacesEnabled
{
    return [self _boolValueForKey:CyberKitEnumeratingAllNetworkInterfacesEnabledPreferenceKey];
}

- (void)setEnumeratingAllNetworkInterfacesEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitEnumeratingAllNetworkInterfacesEnabledPreferenceKey];
}

- (BOOL)iceCandidateFilteringEnabled
{
    return [self _boolValueForKey:CyberKitICECandidateFilteringEnabledPreferenceKey];
}

- (void)setIceCandidateFilteringEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitICECandidateFilteringEnabledPreferenceKey];
}

- (BOOL)mediaCaptureRequiresSecureConnection
{
    return [self _boolValueForKey:CyberKitMediaCaptureRequiresSecureConnectionPreferenceKey];
}

- (void)setMediaCaptureRequiresSecureConnection:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMediaCaptureRequiresSecureConnectionPreferenceKey];
}

- (BOOL)dataTransferItemsEnabled
{
    return [self _boolValueForKey:CyberKitDataTransferItemsEnabledPreferenceKey];
}

- (void)setDataTransferItemsEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitDataTransferItemsEnabledPreferenceKey];
}

- (BOOL)customPasteboardDataEnabled
{
    return [self _boolValueForKey:CyberKitCustomPasteboardDataEnabledPreferenceKey];
}

- (void)setCustomPasteboardDataEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitCustomPasteboardDataEnabledPreferenceKey];
}

- (BOOL)cacheAPIEnabled
{
    return [self _boolValueForKey:CyberKitCacheAPIEnabledPreferenceKey];
}

- (void)setCacheAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitCacheAPIEnabledPreferenceKey];
}

- (BOOL)downloadAttributeEnabled
{
    return [self _boolValueForKey:CyberKitDownloadAttributeEnabledPreferenceKey];
}

- (void)setDownloadAttributeEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitDownloadAttributeEnabledPreferenceKey];
}

- (void)setDirectoryUploadEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitDirectoryUploadEnabledPreferenceKey];
}

- (BOOL)directoryUploadEnabled
{
    return [self _boolValueForKey:CyberKitDirectoryUploadEnabledPreferenceKey];
}

- (BOOL)CSSOMViewScrollingAPIEnabled
{
    return [self _boolValueForKey:CyberKitCSSOMViewScrollingAPIEnabledPreferenceKey];
}

- (void)setCSSOMViewScrollingAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitCSSOMViewScrollingAPIEnabledPreferenceKey];
}

- (BOOL)menuItemElementEnabled
{
    return [self _boolValueForKey:CyberKitMenuItemElementEnabledPreferenceKey];
}

- (void)setMenuItemElementEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMenuItemElementEnabledPreferenceKey];
}

- (BOOL)mediaUserGestureInheritsFromDocument
{
    return [self _boolValueForKey:CyberKitMediaUserGestureInheritsFromDocument];
}

- (void)setMediaUserGestureInheritsFromDocument:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMediaUserGestureInheritsFromDocument];
}

#if PLATFORM(IOS_FAMILY)
- (BOOL)quickLookDocumentSavingEnabled
{
    return [self _boolValueForKey:CyberKitQuickLookDocumentSavingPreferenceKey];
}

- (void)setQuickLookDocumentSavingEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitQuickLookDocumentSavingPreferenceKey];
}
#endif

- (NSString *)mediaContentTypesRequiringHardwareSupport
{
    return [self _stringValueForKey:CyberKitMediaContentTypesRequiringHardwareSupportPreferenceKey];
}

- (void)setMediaContentTypesRequiringHardwareSupport:(NSString *)value
{
    [self _setStringValue:value forKey:CyberKitMediaContentTypesRequiringHardwareSupportPreferenceKey];
}

- (BOOL)legacyEncryptedMediaAPIEnabled
{
    return [self _boolValueForKey:CyberKitLegacyEncryptedMediaAPIEnabledKey];
}

- (void)setLegacyEncryptedMediaAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitLegacyEncryptedMediaAPIEnabledKey];
}

- (BOOL)encryptedMediaAPIEnabled
{
    return [self _boolValueForKey:CyberKitEncryptedMediaAPIEnabledKey];
}

- (void)setEncryptedMediaAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitEncryptedMediaAPIEnabledKey];
}

- (BOOL)pictureInPictureAPIEnabled
{
    return [self _boolValueForKey:CyberKitPictureInPictureAPIEnabledKey];
}

- (void)setPictureInPictureAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitPictureInPictureAPIEnabledKey];
}

- (BOOL)constantPropertiesEnabled
{
    return [self _boolValueForKey:CyberKitConstantPropertiesEnabledPreferenceKey];
}

- (void)setConstantPropertiesEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitConstantPropertiesEnabledPreferenceKey];
}

- (BOOL)colorFilterEnabled
{
    return [self _boolValueForKey:CyberKitColorFilterEnabledPreferenceKey];
}

- (void)setColorFilterEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitColorFilterEnabledPreferenceKey];
}

- (BOOL)punchOutWhiteBackgroundsInDarkMode
{
    return [self _boolValueForKey:CyberKitPunchOutWhiteBackgroundsInDarkModePreferenceKey];
}

- (void)setPunchOutWhiteBackgroundsInDarkMode:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitPunchOutWhiteBackgroundsInDarkModePreferenceKey];
}

- (BOOL)allowMediaContentTypesRequiringHardwareSupportAsFallback
{
    return [self _boolValueForKey:CyberKitAllowMediaContentTypesRequiringHardwareSupportAsFallbackKey];
}

- (void)setAllowMediaContentTypesRequiringHardwareSupportAsFallback:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAllowMediaContentTypesRequiringHardwareSupportAsFallbackKey];
}

- (BOOL)mediaCapabilitiesEnabled
{
    return [self _boolValueForKey:CyberKitMediaCapabilitiesEnabledPreferenceKey];
}

- (void)setMediaCapabilitiesEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMediaCapabilitiesEnabledPreferenceKey];
}

- (BOOL)lineHeightUnitsEnabled
{
    return [self _boolValueForKey:CyberKitLineHeightUnitsEnabledPreferenceKey];
}

- (void)setLineHeightUnitsEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitLineHeightUnitsEnabledPreferenceKey];
}

- (BOOL)layoutFormattingContextIntegrationEnabled
{
    return [self _boolValueForKey:CyberKitLayoutFormattingContextIntegrationEnabledPreferenceKey];
}

- (void)setLayoutFormattingContextIntegrationEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitLayoutFormattingContextIntegrationEnabledPreferenceKey];
}

- (BOOL)isInAppBrowserPrivacyEnabled
{
    return [self _boolValueForKey:CyberKitDebugInAppBrowserPrivacyEnabledPreferenceKey];
}

- (void)setInAppBrowserPrivacyEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitDebugInAppBrowserPrivacyEnabledPreferenceKey];
}

- (BOOL)webSQLEnabled
{
    return [self _boolValueForKey:CyberKitWebSQLEnabledPreferenceKey];
}

- (void)setWebSQLEnabled:(BOOL)webSQLEnabled
{
    [self _setBoolValue:webSQLEnabled forKey:CyberKitWebSQLEnabledPreferenceKey];
}

@end

@implementation WebPreferences (WebInternal)

+ (NSString *)_IBCreatorID
{
    return classIBCreatorID().get();
}

+ (NSString *)_concatenateKeyWithIBCreatorID:(NSString *)key
{
    NSString *IBCreatorID = [WebPreferences _IBCreatorID];
    if (!IBCreatorID)
        return key;
    return [IBCreatorID stringByAppendingString:key];
}

@end

@implementation WebPreferences (WebPrivateFeatures)

- (BOOL)_isEnabledForFeature:(WebFeature *)feature
{
    return [self _boolValueForKey:feature.preferenceKey];
}

- (void)_setEnabled:(BOOL)value forFeature:(WebFeature *)feature
{
    [self _setBoolValue:value forKey:feature.preferenceKey];
}

@end

@implementation WebPreferences (WebPrivateTesting)

+ (void)_switchNetworkLoaderToNewTestingSession
{
#if PLATFORM(IOS_FAMILY)
    WebThreadLock();
#endif
    NetworkStorageSessionMap::switchToNewTestingSession();
}

+ (void)_setCurrentNetworkLoaderSessionCookieAcceptPolicy:(NSHTTPCookieAcceptPolicy)policy
{
    auto cookieStorage = NetworkStorageSessionMap::defaultStorageSession().cookieStorage();
    RELEASE_ASSERT(cookieStorage); // Will fail when NetworkStorageSessionMap::switchToNewTestingSession() was not called beforehand.
    CFHTTPCookieStorageSetCookieAcceptPolicy(cookieStorage.get(), policy);
}

+ (void)_clearNetworkLoaderSession:(void (^)(void))completionHandler
{
    NetworkStorageSessionMap::defaultStorageSession().deleteAllCookies([completionHandler = makeBlockPtr(completionHandler)] {
        completionHandler();
    });
}

- (void)_setBoolPreferenceForTestingWithValue:(BOOL)value forKey:(NSString *)key
{
    [self _setBoolValue:value forKey:key];
}

- (void)_setUInt32PreferenceForTestingWithValue:(uint32_t)value forKey:(NSString *)key
{
    [self _setIntegerValue:value forKey:key];
}

- (void)_setDoublePreferenceForTestingWithValue:(double)value forKey:(NSString *)key
{
    [self _setFloatValue:value forKey:key];
}

- (void)_setStringPreferenceForTestingWithValue:(NSString *)value forKey:(NSString *)key
{
    [self _setStringValue:value forKey:key];
}

@end

@implementation WebPreferences (WebPrivatePreferencesConvertedToWebFeature)

- (BOOL)userGesturePromisePropagationEnabled
{
    return [self _boolValueForKey:CyberKitUserGesturePromisePropagationEnabledPreferenceKey];
}

- (void)setUserGesturePromisePropagationEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitUserGesturePromisePropagationEnabledPreferenceKey];
}

- (BOOL)requestIdleCallbackEnabled
{
    return [self _boolValueForKey:CyberKitRequestIdleCallbackEnabledPreferenceKey];
}

- (void)setRequestIdleCallbackEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitRequestIdleCallbackEnabledPreferenceKey];
}

- (BOOL)highlightAPIEnabled
{
    return [self _boolValueForKey:CyberKitHighlightAPIEnabledPreferenceKey];
}

- (void)setHighlightAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitHighlightAPIEnabledPreferenceKey];
}

- (BOOL)asyncClipboardAPIEnabled
{
    return [self _boolValueForKey:CyberKitAsyncClipboardAPIEnabledPreferenceKey];
}

- (void)setAsyncClipboardAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAsyncClipboardAPIEnabledPreferenceKey];
}

- (BOOL)contactPickerAPIEnabled
{
    return [self _boolValueForKey:CyberKitContactPickerAPIEnabledPreferenceKey];
}

- (void)setContactPickerAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitContactPickerAPIEnabledPreferenceKey];
}

- (BOOL)intersectionObserverEnabled
{
    return [self _boolValueForKey:CyberKitIntersectionObserverEnabledPreferenceKey];
}

- (void)setIntersectionObserverEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitIntersectionObserverEnabledPreferenceKey];
}

- (BOOL)visualViewportAPIEnabled
{
    return [self _boolValueForKey:CyberKitVisualViewportAPIEnabledPreferenceKey];
}

- (void)setVisualViewportAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitVisualViewportAPIEnabledPreferenceKey];
}

- (BOOL)syntheticEditingCommandsEnabled
{
    return [self _boolValueForKey:CyberKitSyntheticEditingCommandsEnabledPreferenceKey];
}

- (void)setSyntheticEditingCommandsEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitSyntheticEditingCommandsEnabledPreferenceKey];
}

- (BOOL)CSSOMViewSmoothScrollingEnabled
{
    return [self _boolValueForKey:CyberKitCSSOMViewSmoothScrollingEnabledPreferenceKey];
}

- (void)setCSSOMViewSmoothScrollingEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitCSSOMViewSmoothScrollingEnabledPreferenceKey];
}

- (BOOL)webAnimationsCompositeOperationsEnabled
{
    return [self _boolValueForKey:CyberKitWebAnimationsCompositeOperationsEnabledPreferenceKey];
}

- (void)setWebAnimationsCompositeOperationsEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitWebAnimationsCompositeOperationsEnabledPreferenceKey];
}

- (BOOL)webAnimationsMutableTimelinesEnabled
{
    return [self _boolValueForKey:CyberKitWebAnimationsMutableTimelinesEnabledPreferenceKey];
}

- (void)setWebAnimationsMutableTimelinesEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitWebAnimationsMutableTimelinesEnabledPreferenceKey];
}

- (BOOL)maskWebGLStringsEnabled
{
    return [self _boolValueForKey:CyberKitMaskWebGLStringsEnabledPreferenceKey];
}

- (void)setMaskWebGLStringsEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitMaskWebGLStringsEnabledPreferenceKey];
}

- (BOOL)serverTimingEnabled
{
    return [self _boolValueForKey:CyberKitServerTimingEnabledPreferenceKey];
}

- (void)setServerTimingEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitServerTimingEnabledPreferenceKey];
}

- (BOOL)CSSCustomPropertiesAndValuesEnabled
{
    return [self _boolValueForKey:CyberKitCSSCustomPropertiesAndValuesEnabledPreferenceKey];
}

- (void)setCSSCustomPropertiesAndValuesEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitCSSCustomPropertiesAndValuesEnabledPreferenceKey];
}

- (BOOL)resizeObserverEnabled
{
    return [self _boolValueForKey:CyberKitResizeObserverEnabledPreferenceKey];
}

- (void)setResizeObserverEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitResizeObserverEnabledPreferenceKey];
}

- (BOOL)privateClickMeasurementEnabled
{
    return [self _boolValueForKey:CyberKitPrivateClickMeasurementEnabledPreferenceKey];
}

- (void)setPrivateClickMeasurementEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitPrivateClickMeasurementEnabledPreferenceKey];
}

- (BOOL)fetchAPIKeepAliveEnabled
{
    return [self _boolValueForKey:CyberKitFetchAPIEnabledPreferenceKey];
}

- (void)setFetchAPIKeepAliveEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitFetchAPIEnabledPreferenceKey];
}

- (BOOL)genericCueAPIEnabled
{
    return [self _boolValueForKey:CyberKitGenericCueAPIEnabledKey];
}

- (void)setGenericCueAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitGenericCueAPIEnabledKey];
}

- (BOOL)aspectRatioOfImgFromWidthAndHeightEnabled
{
    return [self _boolValueForKey:CyberKitAspectRatioOfImgFromWidthAndHeightEnabledPreferenceKey];
}

- (void)setAspectRatioOfImgFromWidthAndHeightEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAspectRatioOfImgFromWidthAndHeightEnabledPreferenceKey];
}

- (BOOL)referrerPolicyAttributeEnabled
{
    return [self _boolValueForKey:CyberKitReferrerPolicyAttributeEnabledPreferenceKey];
}

- (void)setReferrerPolicyAttributeEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitReferrerPolicyAttributeEnabledPreferenceKey];
}

- (BOOL)coreMathMLEnabled
{
    return [self _boolValueForKey:CyberKitCoreMathMLEnabledPreferenceKey];
}

- (void)setCoreMathMLEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitCoreMathMLEnabledPreferenceKey];
}

- (BOOL)linkPreloadResponsiveImagesEnabled
{
    return [self _boolValueForKey:CyberKitLinkPreloadResponsiveImagesEnabledPreferenceKey];
}

- (void)setLinkPreloadResponsiveImagesEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitLinkPreloadResponsiveImagesEnabledPreferenceKey];
}

- (BOOL)remotePlaybackEnabled
{
    return [self _boolValueForKey:CyberKitRemotePlaybackEnabledPreferenceKey];
}

- (void)setRemotePlaybackEnabled:(BOOL)remotePlaybackEnabled
{
    [self _setBoolValue:remotePlaybackEnabled forKey:CyberKitRemotePlaybackEnabledPreferenceKey];
}

- (BOOL)readableByteStreamAPIEnabled
{
    return [self _boolValueForKey:CyberKitReadableByteStreamAPIEnabledPreferenceKey];
}

- (void)setReadableByteStreamAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitReadableByteStreamAPIEnabledPreferenceKey];
}

- (BOOL)transformStreamAPIEnabled
{
    return [self _boolValueForKey:CyberKitTransformStreamAPIEnabledPreferenceKey];
}

- (void)setTransformStreamAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitTransformStreamAPIEnabledPreferenceKey];
}

- (BOOL)_mediaRecorderEnabled
{
    return [self _boolValueForKey:CyberKitMediaRecorderEnabledPreferenceKey];
}

- (void)_setMediaRecorderEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMediaRecorderEnabledPreferenceKey];
}

- (BOOL)mediaRecorderEnabled
{
    return [self _boolValueForKey:CyberKitMediaRecorderEnabledPreferenceKey];
}

- (void)setMediaRecorderEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMediaRecorderEnabledPreferenceKey];
}

- (BOOL)CSSIndividualTransformPropertiesEnabled
{
    return [self _boolValueForKey:CyberKitCSSIndividualTransformPropertiesEnabledPreferenceKey];
}

- (void)setCSSIndividualTransformPropertiesEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitCSSIndividualTransformPropertiesEnabledPreferenceKey];
}

- (BOOL)_speechRecognitionEnabled
{
    return [self _boolValueForKey:CyberKitSpeechRecognitionEnabledPreferenceKey];
}

- (void)_setSpeechRecognitionEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitSpeechRecognitionEnabledPreferenceKey];
}

- (CyberKitPitchCorrectionAlgorithm)_pitchCorrectionAlgorithm
{
    return static_cast<CyberKitPitchCorrectionAlgorithm>([self _unsignedIntValueForKey:CyberKitPitchCorrectionAlgorithmPreferenceKey]);
}

- (void)_setPitchCorrectionAlgorithm:(CyberKitPitchCorrectionAlgorithm)pitchCorrectionAlgorithm
{
    [self _setUnsignedIntValue:pitchCorrectionAlgorithm forKey:CyberKitPitchCorrectionAlgorithmPreferenceKey];
}

@end

@implementation WebPreferences (WebPrivateDeprecated)

// The preferences in this category are deprecated and have no effect. They should
// be removed when it is considered safe to do so.

- (void)setSubpixelCSSOMElementMetricsEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitSubpixelCSSOMElementMetricsEnabledPreferenceKey];
}

- (BOOL)subpixelCSSOMElementMetricsEnabled
{
    return [self _boolValueForKey:CyberKitSubpixelCSSOMElementMetricsEnabledPreferenceKey];
}

- (void)setUserTimingEnabled:(BOOL)flag
{
}

- (BOOL)userTimingEnabled
{
    return YES;
}

- (BOOL)requestAnimationFrameEnabled
{
    return YES;
}

- (void)setRequestAnimationFrameEnabled:(BOOL)enabled
{
}

- (void)setResourceTimingEnabled:(BOOL)flag
{
}

- (BOOL)resourceTimingEnabled
{
    return YES;
}

- (void)setCSSShadowPartsEnabled:(BOOL)flag
{
}

- (BOOL)cssShadowPartsEnabled
{
    return YES;
}

- (void)setIsSecureContextAttributeEnabled:(BOOL)flag
{
}

- (BOOL)isSecureContextAttributeEnabled
{
    return YES;
}

- (void)setFetchAPIEnabled:(BOOL)flag
{
}

- (BOOL)fetchAPIEnabled
{
    return YES;
}

- (void)setShadowDOMEnabled:(BOOL)flag
{
}

- (BOOL)shadowDOMEnabled
{
    return YES;
}

- (void)setCustomElementsEnabled:(BOOL)flag
{
}

- (BOOL)customElementsEnabled
{
    return YES;
}

- (BOOL)keygenElementEnabled
{
    return NO;
}

- (void)setKeygenElementEnabled:(BOOL)flag
{
}

- (void)setVideoPluginProxyEnabled:(BOOL)flag
{
}

- (BOOL)isVideoPluginProxyEnabled
{
    return NO;
}

- (void)setHixie76WebSocketProtocolEnabled:(BOOL)flag
{
}

- (BOOL)isHixie76WebSocketProtocolEnabled
{
    return NO;
}

- (void)setDiskImageCacheEnabled:(BOOL)enabled
{
}

- (void)setAccelerated2dCanvasEnabled:(BOOL)enabled
{
}

- (BOOL)accelerated2dCanvasEnabled
{
    return NO;
}

- (void)setExperimentalNotificationsEnabled:(BOOL)enabled
{
}

- (BOOL)experimentalNotificationsEnabled
{
    return NO;
}

- (BOOL)selectionAcrossShadowBoundariesEnabled
{
    return YES;
}

- (void)setSelectionAcrossShadowBoundariesEnabled:(BOOL)flag
{
}

- (BOOL)isXSSAuditorEnabled
{
    return NO;
}

- (void)setXSSAuditorEnabled:(BOOL)flag
{
}

- (BOOL)subpixelAntialiasedLayerTextEnabled
{
    return NO;
}

- (void)setSubpixelAntialiasedLayerTextEnabled:(BOOL)enabled
{
}

- (BOOL)webGL2Enabled
{
    return [self _boolValueForKey:CyberKitWebGLEnabledPreferenceKey];
}

- (void)setWebGL2Enabled:(BOOL)enabled
{
}

@end
