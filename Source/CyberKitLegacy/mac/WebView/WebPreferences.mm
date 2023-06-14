/*
 * Copyright (C) 2005-2017 Apple Inc. All rights reserved.
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

#import "WebPreferencesPrivate.h"
#import "WebPreferenceKeysPrivate.h"

#import "WebApplicationCache.h"
#import "WebFrameNetworkingContext.h"
#import "CyberKitLogging.h"
#import "CyberKitNSStringExtras.h"
#import "CyberKitVersionChecks.h"
#import "WebNSDictionaryExtras.h"
#import "WebNSURLExtras.h"
#import <CyberScriptCore/InitializeThreading.h>
#import <CyberCore/ApplicationCacheStorage.h>
#import <CyberCore/AudioSession.h>
#import <CyberCore/DeprecatedGlobalSettings.h>
#import <CyberCore/NetworkStorageSession.h>
#import <CyberCore/RuntimeApplicationChecks.h>
#import <CyberCore/Settings.h>
#import <CyberCore/TextEncodingRegistry.h>
#import <pal/spi/cf/CFNetworkSPI.h>
#import <wtf/MainThread.h>
#import <wtf/RetainPtr.h>
#import <wtf/RunLoop.h>

using namespace CyberCore;

#if HAVE(AUDIO_TOOLBOX_AUDIO_SESSION)
#import <AudioToolbox/AudioSession.h>
#endif

#if PLATFORM(IOS_FAMILY)
#import <CyberCore/Device.h>
#import <CyberCore/GraphicsContext.h>
#import <CyberCore/CyberCoreThreadMessage.h>
#endif

NSString *WebPreferencesChangedNotification = @"WebPreferencesChangedNotification";
NSString *WebPreferencesRemovedNotification = @"WebPreferencesRemovedNotification";
NSString *WebPreferencesChangedInternalNotification = @"WebPreferencesChangedInternalNotification";
NSString *WebPreferencesCacheModelChangedInternalNotification = @"WebPreferencesCacheModelChangedInternalNotification";

#define KEY(x) (_private->identifier ? [_private->identifier.get() stringByAppendingString:(x)] : (x))

enum { WebPreferencesVersion = 1 };

static WebPreferences *_standardPreferences;
static NSMutableDictionary *webPreferencesInstances;

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

static WebCacheModel cacheModelForMainBundle(void)
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
            "com.apple.dashboard.client", 
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
            "com.app4mac.KidsBrowser"
            "com.app4mac.wKiosk",
            "com.freeverse.bumpercar",
            "com.omnigroup.OmniWeb5",
            "com.sunrisebrowser.Sunrise",
            "net.hmdt-web.Shiira",
        };

        const char* bundleID = [[[NSBundle mainBundle] bundleIdentifier] UTF8String];
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

@interface WebPreferences ()
- (void)_postCacheModelChangedNotification;
@end

@interface WebPreferences (WebInternal)
+ (NSString *)_concatenateKeyWithIBCreatorID:(NSString *)key;
+ (NSString *)_IBCreatorID;
@end

struct WebPreferencesPrivate
{
public:
    WebPreferencesPrivate()
    : inPrivateBrowsing(NO)
    , autosaves(NO)
    , automaticallyDetectsCacheModel(NO)
    , numWebViews(0)
#if PLATFORM(IOS_FAMILY)
    , readWriteQueue(dispatch_queue_create("com.apple.WebPreferences.ReadWriteQueue", DISPATCH_QUEUE_CONCURRENT))
#endif
    {
    }

#if PLATFORM(IOS_FAMILY)
    ~WebPreferencesPrivate()
    {
        dispatch_release(readWriteQueue);
    }
#endif

    RetainPtr<NSMutableDictionary> values;
    BOOL inPrivateBrowsing;
    RetainPtr<NSString> identifier;
    BOOL autosaves;
    BOOL automaticallyDetectsCacheModel;
    unsigned numWebViews;
#if PLATFORM(IOS_FAMILY)
    dispatch_queue_t readWriteQueue;
#endif
};

@interface WebPreferences (WebForwardDeclarations)
// This pseudo-category is needed so these methods can be used from within other category implementations
// without being in the public header file.
- (BOOL)_boolValueForKey:(NSString *)key;
- (void)_setBoolValue:(BOOL)value forKey:(NSString *)key;
- (int)_integerValueForKey:(NSString *)key;
- (void)_setIntegerValue:(int)value forKey:(NSString *)key;
- (float)_floatValueForKey:(NSString *)key;
- (void)_setFloatValue:(float)value forKey:(NSString *)key;
- (void)_setLongLongValue:(long long)value forKey:(NSString *)key;
- (long long)_longLongValueForKey:(NSString *)key;
- (void)_setUnsignedLongLongValue:(unsigned long long)value forKey:(NSString *)key;
- (unsigned long long)_unsignedLongLongValueForKey:(NSString *)key;
@end

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
            [decoder decodeValueOfObjCType:@encode(int) at:&version];
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
        dispatch_sync(_private->readWriteQueue, ^{
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
        dispatch_sync(_private->readWriteQueue, ^{
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
    if (_standardPreferences == nil) {
        _standardPreferences = [[WebPreferences alloc] initWithIdentifier:nil];
        [_standardPreferences setAutosaves:YES];
    }
#else
    // FIXME: This check is necessary to avoid recursion (see <rdar://problem/9564337>), but it also makes _standardPreferences construction not thread safe.
    if (_standardPreferences)
        return _standardPreferences;

    static dispatch_once_t pred;
    dispatch_once(&pred, ^{
        _standardPreferences = [[WebPreferences alloc] initWithIdentifier:nil sendChangeNotification:NO];
        [_standardPreferences _postPreferencesChangedNotification];
        [_standardPreferences setAutosaves:YES];
    });
#endif

    return _standardPreferences;
}

// if we ever have more than one WebPreferences object, this would move to init
+ (void)initialize
{
#if !PLATFORM(IOS_FAMILY)
    JSC::initializeThreading();
    WTF::initializeMainThreadToProcessMainThread();
    RunLoop::initializeMainRunLoop();
    bool attachmentElementEnabled = MacApplication::isAppleMail();
#else
    bool allowsInlineMediaPlayback = CyberCore::deviceClass() == MGDeviceClassiPad;
    bool allowsInlineMediaPlaybackAfterFullscreen = CyberCore::deviceClass() != MGDeviceClassiPad;
    bool requiresPlaysInlineAttribute = !allowsInlineMediaPlayback;
    bool attachmentElementEnabled = IOSApplication::isMobileMail();
#endif

    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
        @"Times",                       CyberKitStandardFontPreferenceKey,
        @"Courier",                     CyberKitFixedFontPreferenceKey,
        @"Times",                       CyberKitSerifFontPreferenceKey,
        @"Helvetica",                   CyberKitSansSerifFontPreferenceKey,
#if !PLATFORM(IOS_FAMILY)
        @"Apple Chancery",              CyberKitCursiveFontPreferenceKey,
#else
        @"Snell Roundhand",             CyberKitCursiveFontPreferenceKey,
#endif
        @"Papyrus",                     CyberKitFantasyFontPreferenceKey,
#if PLATFORM(IOS_FAMILY)
        @"AppleColorEmoji",             CyberKitPictographFontPreferenceKey,
#else
        @"Apple Color Emoji",           CyberKitPictographFontPreferenceKey,
#endif
        @"0",                           CyberKitMinimumFontSizePreferenceKey,
        @"9",                           CyberKitMinimumLogicalFontSizePreferenceKey, 
        @"16",                          CyberKitDefaultFontSizePreferenceKey,
        @"13",                          CyberKitDefaultFixedFontSizePreferenceKey,
        @"ISO-8859-1",                  CyberKitDefaultTextEncodingNamePreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitUsesEncodingDetectorPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitUserStyleSheetEnabledPreferenceKey,
        @"",                            CyberKitUserStyleSheetLocationPreferenceKey,
#if !PLATFORM(IOS_FAMILY)
        [NSNumber numberWithBool:NO],   CyberKitShouldPrintBackgroundsPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitTextAreasAreResizablePreferenceKey,
#endif
        [NSNumber numberWithBool:NO],   CyberKitShrinksStandaloneImagesToFitPreferenceKey,
#if !PLATFORM(IOS_FAMILY)
        [NSNumber numberWithBool:YES],  CyberKitJavaEnabledPreferenceKey,
#endif
        [NSNumber numberWithBool:YES],  CyberKitJavaScriptEnabledPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitJavaScriptMarkupEnabledPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitWebSecurityEnabledPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitAllowUniversalAccessFromFileURLsPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitAllowFileAccessFromFileURLsPreferenceKey,
#if PLATFORM(IOS_FAMILY)
        [NSNumber numberWithBool:NO],   CyberKitJavaScriptCanOpenWindowsAutomaticallyPreferenceKey,
#else
        [NSNumber numberWithBool:YES],  CyberKitJavaScriptCanOpenWindowsAutomaticallyPreferenceKey,
#endif
        [NSNumber numberWithBool:YES],  CyberKitPluginsEnabledPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitDatabasesEnabledPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitHTTPEquivEnabledPreferenceKey,

#if PLATFORM(IOS_FAMILY)
        [NSNumber numberWithBool:NO],   CyberKitStorageTrackerEnabledPreferenceKey,
#endif
        [NSNumber numberWithBool:YES],  CyberKitLocalStorageEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitExperimentalNotificationsEnabledPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitAllowAnimatedImagesPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitAllowAnimatedImageLoopingPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitDisplayImagesKey,
        [NSNumber numberWithBool:NO],   CyberKitLoadSiteIconsKey,
        @"1800",                        CyberKitBackForwardCacheExpirationIntervalKey,
#if !PLATFORM(IOS_FAMILY)
        [NSNumber numberWithBool:NO],   CyberKitTabToLinksPreferenceKey,
#endif
        [NSNumber numberWithBool:NO],   CyberKitPrivateBrowsingEnabledPreferenceKey,
#if !PLATFORM(IOS_FAMILY)
        [NSNumber numberWithBool:NO],   CyberKitRespectStandardStyleKeyEquivalentsPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitShowsURLsInToolTipsPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitShowsToolTipOverTruncatedTextPreferenceKey,
        @"1",                           CyberKitPDFDisplayModePreferenceKey,
        @"0",                           CyberKitPDFScaleFactorPreferenceKey,
#endif
        @"0",                           CyberKitUseSiteSpecificSpoofingPreferenceKey,
        [NSNumber numberWithInt:CyberKitEditableLinkDefaultBehavior], CyberKitEditableLinkBehaviorPreferenceKey,
#if !PLATFORM(IOS_FAMILY)
        [NSNumber numberWithInt:WebTextDirectionSubmenuAutomaticallyIncluded],
                                        CyberKitTextDirectionSubmenuInclusionBehaviorPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitDOMPasteAllowedPreferenceKey,
#endif
        [NSNumber numberWithBool:YES],  CyberKitUsesPageCachePreferenceKey,
        [NSNumber numberWithInt:cacheModelForMainBundle()], CyberKitCacheModelPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitPageCacheSupportsPluginsPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitDeveloperExtrasEnabledPreferenceKey,
        [NSNumber numberWithUnsignedInt:0], CyberKitJavaScriptRuntimeFlagsPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitAuthorAndUserStylesEnabledPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitDOMTimersThrottlingEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitWebArchiveDebugModeEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitLocalFileContentSniffingEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitOfflineWebApplicationCacheEnabledPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitZoomsTextOnlyPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitJavaScriptCanAccessClipboardPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitXSSAuditorEnabledPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitAcceleratedCompositingEnabledPreferenceKey,

#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101300
#define DEFAULT_SUBPIXEL_ANTIALIASED_LAYER_TEXT_ENABLED YES
#else
#define DEFAULT_SUBPIXEL_ANTIALIASED_LAYER_TEXT_ENABLED NO
#endif
        [NSNumber numberWithBool:DEFAULT_SUBPIXEL_ANTIALIASED_LAYER_TEXT_ENABLED], CyberKitSubpixelAntialiasedLayerTextEnabledPreferenceKey,

        [NSNumber numberWithBool:NO],   CyberKitDisplayListDrawingEnabledPreferenceKey,
#if PLATFORM(IOS_FAMILY) && !PLATFORM(IOS_FAMILY_SIMULATOR)
        [NSNumber numberWithBool:YES],  CyberKitAcceleratedDrawingEnabledPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitCanvasUsesAcceleratedDrawingPreferenceKey,
#else
        [NSNumber numberWithBool:NO],  CyberKitAcceleratedDrawingEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],  CyberKitCanvasUsesAcceleratedDrawingPreferenceKey,
#endif
        [NSNumber numberWithBool:NO],   CyberKitShowDebugBordersPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitSimpleLineLayoutEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitSimpleLineLayoutDebugBordersEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitShowRepaintCounterPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitWebGLEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],  CyberKitForceSoftwareWebGLRenderingPreferenceKey,
        [NSNumber numberWithBool:YES],   CyberKitForceWebGLUsesLowPowerPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitAccelerated2dCanvasEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],  CyberKitSubpixelCSSOMElementMetricsEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],  CyberKitResourceLoadStatisticsEnabledPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitLargeImageAsyncDecodingEnabledPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitAnimatedImageAsyncDecodingEnabledPreferenceKey,
#if PLATFORM(IOS_FAMILY)
        [NSNumber numberWithUnsignedInt:static_cast<uint32_t>(FrameFlattening::FullyEnabled)], CyberKitFrameFlatteningPreferenceKey,
#else
        [NSNumber numberWithUnsignedInt:static_cast<uint32_t>(FrameFlattening::Disabled)], CyberKitFrameFlatteningPreferenceKey,
#endif
        [NSNumber numberWithBool:NO], CyberKitAsyncFrameScrollingEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitSpatialNavigationEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],  CyberKitDNSPrefetchingEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitFullScreenEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitAsynchronousSpellCheckingEnabledPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitHyperlinkAuditingEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitUsePreHTML5ParserQuirksKey,
        [NSNumber numberWithBool:YES],  CyberKitAVFoundationEnabledKey,
        [NSNumber numberWithBool:YES],  CyberKitAVFoundationNSURLSessionEnabledKey,
        [NSNumber numberWithBool:NO],   CyberKitSuppressesIncrementalRenderingKey,
        [NSNumber numberWithBool:attachmentElementEnabled], CyberKitAttachmentElementEnabledPreferenceKey,
#if !PLATFORM(IOS_FAMILY)
        [NSNumber numberWithBool:YES],  CyberKitAllowsInlineMediaPlaybackPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitAllowsInlineMediaPlaybackAfterFullscreenPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitInlineMediaPlaybackRequiresPlaysInlineAttributeKey,
        [NSNumber numberWithBool:YES],  CyberKitMediaControlsScaleWithPageZoomPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitWebAudioEnabledPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitBackspaceKeyNavigationEnabledKey,
        [NSNumber numberWithBool:NO],   CyberKitShouldDisplaySubtitlesPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitShouldDisplayCaptionsPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitShouldDisplayTextDescriptionsPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitNotificationsEnabledKey,
        [NSNumber numberWithBool:NO],   CyberKitShouldRespectImageOrientationKey,
        [NSNumber numberWithBool:YES],  CyberKitMediaDataLoadsAutomaticallyPreferenceKey,
#else
        [NSNumber numberWithBool:allowsInlineMediaPlayback],   CyberKitAllowsInlineMediaPlaybackPreferenceKey,
        [NSNumber numberWithBool:allowsInlineMediaPlaybackAfterFullscreen],   CyberKitAllowsInlineMediaPlaybackAfterFullscreenPreferenceKey,
        [NSNumber numberWithBool:requiresPlaysInlineAttribute], CyberKitInlineMediaPlaybackRequiresPlaysInlineAttributeKey,
        [NSNumber numberWithBool:NO],   CyberKitMediaControlsScaleWithPageZoomPreferenceKey,
        [NSNumber numberWithUnsignedInt:AudioSession::None],  CyberKitAudioSessionCategoryOverride,
        [NSNumber numberWithBool:NO],   CyberKitMediaDataLoadsAutomaticallyPreferenceKey,
#if HAVE(AVKIT)
        [NSNumber numberWithBool:YES],  CyberKitAVKitEnabled,
#endif
        [NSNumber numberWithBool:YES],  CyberKitRequiresUserGestureForMediaPlaybackPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitRequiresUserGestureForVideoPlaybackPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitRequiresUserGestureForAudioPlaybackPreferenceKey,
        [NSNumber numberWithLongLong:CyberCore::ApplicationCacheStorage::noQuota()], CyberKitApplicationCacheTotalQuota,

        // Per-Origin Quota on iOS is 25MB. When the quota is reached for a particular origin
        // the quota for that origin can be increased. See also webView:exceededApplicationCacheOriginQuotaForSecurityOrigin:totalSpaceNeeded in WebUI/WebUIDelegate.m.
        [NSNumber numberWithLongLong:(25 * 1024 * 1024)], CyberKitApplicationCacheDefaultOriginQuota,

        // Enable WebAudio by default in all iOS UIWebViews
        [NSNumber numberWithBool:YES],   CyberKitWebAudioEnabledPreferenceKey,

        [NSNumber numberWithBool:YES],   CyberKitShouldRespectImageOrientationKey,
#endif // PLATFORM(IOS_FAMILY)
#if ENABLE(WIRELESS_TARGET_PLAYBACK)
        [NSNumber numberWithBool:YES],  CyberKitAllowsAirPlayForMediaPlaybackPreferenceKey,
#endif
        [NSNumber numberWithBool:YES],  CyberKitAllowsPictureInPictureMediaPlaybackPreferenceKey,
        [NSNumber numberWithBool:YES],  CyberKitRequestAnimationFrameEnabledPreferenceKey,
        [NSNumber numberWithBool:NO],   CyberKitWantsBalancedSetDefersLoadingBehaviorKey,
        [NSNumber numberWithBool:NO],   CyberKitDiagnosticLoggingEnabledKey,
        [NSNumber numberWithInt:WebAllowAllStorage], CyberKitStorageBlockingPolicyKey,
        [NSNumber numberWithBool:NO],   CyberKitPlugInSnapshottingEnabledPreferenceKey,

#if PLATFORM(IOS_FAMILY)
        [NSNumber numberWithBool:NO],   CyberKitTelephoneParsingEnabledPreferenceKey,
        [NSNumber numberWithInt:-1],      CyberKitLayoutIntervalPreferenceKey,
        [NSNumber numberWithFloat:-1.0f], CyberKitMaxParseDurationPreferenceKey,
        [NSNumber numberWithBool:NO],     CyberKitAllowMultiElementImplicitFormSubmissionPreferenceKey,
        [NSNumber numberWithBool:NO],     CyberKitAlwaysRequestGeolocationPermissionPreferenceKey,
        [NSNumber numberWithInt:InterpolationLow], CyberKitInterpolationQualityPreferenceKey,
        [NSNumber numberWithBool:YES],    CyberKitPasswordEchoEnabledPreferenceKey,
        [NSNumber numberWithFloat:2.0f],  CyberKitPasswordEchoDurationPreferenceKey,
        [NSNumber numberWithBool:NO],     CyberKitNetworkDataUsageTrackingEnabledPreferenceKey,
        @"",                              CyberKitNetworkInterfaceNamePreferenceKey,
#endif
#if ENABLE(TEXT_AUTOSIZING)
        [NSNumber numberWithFloat:Settings::defaultMinimumZoomFontSize()], CyberKitMinimumZoomFontSizePreferenceKey,
        [NSNumber numberWithBool:Settings::defaultTextAutosizingEnabled()], CyberKitTextAutosizingEnabledPreferenceKey,
#endif
        [NSNumber numberWithLongLong:ApplicationCacheStorage::noQuota()], CyberKitApplicationCacheTotalQuota,
        [NSNumber numberWithLongLong:ApplicationCacheStorage::noQuota()], CyberKitApplicationCacheDefaultOriginQuota,
        [NSNumber numberWithBool:NO], CyberKitHiddenPageDOMTimerThrottlingEnabledPreferenceKey,
        [NSNumber numberWithBool:NO], CyberKitHiddenPageCSSAnimationSuspensionEnabledPreferenceKey,
        [NSNumber numberWithBool:NO], CyberKitLowPowerVideoAudioBufferSizeEnabledPreferenceKey,
        
        [NSNumber numberWithBool:NO], CyberKitUseLegacyTextAlignPositionedElementBehaviorPreferenceKey,
#if ENABLE(MEDIA_SOURCE)
        [NSNumber numberWithBool:YES], CyberKitMediaSourceEnabledPreferenceKey,
        @YES, CyberKitSourceBufferChangeTypeEnabledPreferenceKey,
#endif
#if ENABLE(SERVICE_CONTROLS)
        [NSNumber numberWithBool:NO], CyberKitImageControlsEnabledPreferenceKey,
        [NSNumber numberWithBool:NO], CyberKitServiceControlsEnabledPreferenceKey,
#endif
        [NSNumber numberWithBool:NO], CyberKitEnableInheritURIQueryComponentPreferenceKey,
#if ENABLE(LEGACY_ENCRYPTED_MEDIA)
        @"~/Library/CyberKit/MediaKeys", CyberKitMediaKeysStorageDirectoryKey,
#endif
#if ENABLE(MEDIA_STREAM)
        [NSNumber numberWithBool:NO], CyberKitMockCaptureDevicesEnabledPreferenceKey,
        [NSNumber numberWithBool:YES], CyberKitMockCaptureDevicesPromptEnabledPreferenceKey,
        [NSNumber numberWithBool:YES], CyberKitMediaCaptureRequiresSecureConnectionPreferenceKey,
#endif
        [NSNumber numberWithBool:YES], CyberKitShadowDOMEnabledPreferenceKey,
        [NSNumber numberWithBool:YES], CyberKitCustomElementsEnabledPreferenceKey,
        [NSNumber numberWithBool:YES], CyberKitDataTransferItemsEnabledPreferenceKey,
        [NSNumber numberWithBool:NO], CyberKitCustomPasteboardDataEnabledPreferenceKey,
        [NSNumber numberWithBool:YES], CyberKitModernMediaControlsEnabledPreferenceKey,
        [NSNumber numberWithBool:NO], CyberKitWebAnimationsCSSIntegrationEnabledPreferenceKey,

#if ENABLE(WEBGL2)
        [NSNumber numberWithBool:NO], CyberKitWebGL2EnabledPreferenceKey,
#endif
#if ENABLE(WEBGPU)
        [NSNumber numberWithBool:NO], CyberKitWebGPUEnabledPreferenceKey,
#endif
#if ENABLE(WEBMETAL)
        [NSNumber numberWithBool:NO], CyberKitWebMetalEnabledPreferenceKey,
#endif
        [NSNumber numberWithBool:NO], CyberKitCacheAPIEnabledPreferenceKey,
        [NSNumber numberWithBool:YES], CyberKitFetchAPIEnabledPreferenceKey,

#if ENABLE(STREAMS_API)
        [NSNumber numberWithBool:NO], CyberKitReadableByteStreamAPIEnabledPreferenceKey,
        [NSNumber numberWithBool:NO], CyberKitWritableStreamAPIEnabledPreferenceKey,
#endif
#if ENABLE(DOWNLOAD_ATTRIBUTE)
        [NSNumber numberWithBool:NO], CyberKitDownloadAttributeEnabledPreferenceKey,
#endif
        [NSNumber numberWithBool:NO], CyberKitDirectoryUploadEnabledPreferenceKey,
        [NSNumber numberWithBool:NO], CyberKitWebAnimationsEnabledPreferenceKey,

#if PLATFORM(IOS_FAMILY)
        [NSNumber numberWithBool:NO], CyberKitVisualViewportEnabledPreferenceKey,
#else
        [NSNumber numberWithBool:YES], CyberKitVisualViewportEnabledPreferenceKey,
#endif
        [NSNumber numberWithBool:NO], CyberKitVisualViewportAPIEnabledPreferenceKey,

        [NSNumber numberWithBool:NO], CyberKitCSSOMViewScrollingAPIEnabledPreferenceKey,
        [NSNumber numberWithBool:YES], CyberKitNeedsStorageAccessFromFileURLsQuirkKey,
        [NSNumber numberWithBool:NO], CyberKitAllowCrossOriginSubresourcesToAskForCredentialsKey,
#if ENABLE(MEDIA_STREAM)
        [NSNumber numberWithBool:NO], CyberKitMediaDevicesEnabledPreferenceKey,
        [NSNumber numberWithBool:YES], CyberKitMediaStreamEnabledPreferenceKey,
        [NSNumber numberWithBool:NO], CyberKitMediaRecorderEnabledPreferenceKey,
#endif
#if ENABLE(WEB_RTC)
        [NSNumber numberWithBool:YES], CyberKitPeerConnectionEnabledPreferenceKey,
#endif
        [NSNumber numberWithBool:YES], CyberKitSelectionAcrossShadowBoundariesEnabledPreferenceKey,
#if ENABLE(INTERSECTION_OBSERVER)
        @NO, CyberKitIntersectionObserverEnabledPreferenceKey,
#endif
        @YES, CyberKitDisplayContentsEnabledPreferenceKey,
        @NO, CyberKitUserTimingEnabledPreferenceKey,
        @NO, CyberKitResourceTimingEnabledPreferenceKey,
        @NO, CyberKitMediaUserGestureInheritsFromDocument,
        @NO, CyberKitIsSecureContextAttributeEnabledPreferenceKey,
        @YES, CyberKitLegacyEncryptedMediaAPIEnabledKey,
        @NO, CyberKitEncryptedMediaAPIEnabledKey,
        @YES, CyberKitViewportFitEnabledPreferenceKey,
        @YES, CyberKitConstantPropertiesEnabledPreferenceKey,
        @NO, CyberKitColorFilterEnabledPreferenceKey,
        @NO, CyberKitPunchOutWhiteBackgroundsInDarkModePreferenceKey,
        @YES, CyberKitAllowMediaContentTypesRequiringHardwareSupportAsFallbackKey,
        @NO, CyberKitInspectorAdditionsEnabledPreferenceKey,
        (NSString *)Settings::defaultMediaContentTypesRequiringHardwareSupport(), CyberKitMediaContentTypesRequiringHardwareSupportPreferenceKey,
        @NO, CyberKitAccessibilityObjectModelEnabledPreferenceKey,
        @NO, CyberKitAriaReflectionEnabledPreferenceKey,
        @NO, CyberKitMediaCapabilitiesEnabledPreferenceKey,
        @NO, CyberKitFetchAPIKeepAliveEnabledPreferenceKey,
        @NO, CyberKitServerTimingEnabledPreferenceKey,
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
    dispatch_sync(_private->readWriteQueue, ^{
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
    dispatch_barrier_sync(_private->readWriteQueue, ^{
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
    dispatch_barrier_sync(_private->readWriteQueue, ^{
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
    dispatch_barrier_sync(_private->readWriteQueue, ^{
#endif
    [_private->values.get() setObject:@(value) forKey:_key];
#if PLATFORM(IOS_FAMILY)
    });
#endif
    if (_private->autosaves)
        [[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithUnsignedInt:value] forKey:_key];
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
    dispatch_barrier_sync(_private->readWriteQueue, ^{
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
    dispatch_barrier_sync(_private->readWriteQueue, ^{
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
    dispatch_barrier_sync(_private->readWriteQueue, ^{
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
    dispatch_barrier_sync(_private->readWriteQueue, ^{
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

#if !PLATFORM(IOS_FAMILY)
- (BOOL)shouldPrintBackgrounds
{
    return [self _boolValueForKey: CyberKitShouldPrintBackgroundsPreferenceKey];
}

- (void)setShouldPrintBackgrounds:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitShouldPrintBackgroundsPreferenceKey];
}
#endif

- (BOOL)isJavaEnabled
{
    return [self _boolValueForKey: CyberKitJavaEnabledPreferenceKey];
}

- (void)setJavaEnabled:(BOOL)flag
{
    [self _setBoolValue: flag forKey: CyberKitJavaEnabledPreferenceKey];
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
#if ENABLE(WIRELESS_TARGET_PLAYBACK)
    return [self _boolValueForKey:CyberKitAllowsAirPlayForMediaPlaybackPreferenceKey];
#else
    return false;
#endif
}

- (void)setAllowsAirPlayForMediaPlayback:(BOOL)flag
{
#if ENABLE(WIRELESS_TARGET_PLAYBACK)
    [self _setBoolValue:flag forKey:CyberKitAllowsAirPlayForMediaPlaybackPreferenceKey];
#endif
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

// FIXME: applicationChromeMode is no longer needed by ToT, but is still used in Safari 8.
- (BOOL)applicationChromeModeEnabled
{
    return NO;
}

- (void)setApplicationChromeModeEnabled:(BOOL)flag
{
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

- (BOOL)isXSSAuditorEnabled
{
    return [self _boolValueForKey:CyberKitXSSAuditorEnabledPreferenceKey];
}

- (void)setXSSAuditorEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitXSSAuditorEnabledPreferenceKey];
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
- (void)_setLayoutInterval:(int)l
{
    [self _setIntegerValue:l forKey:CyberKitLayoutIntervalPreferenceKey];
}

- (int)_layoutInterval
{
    return [self _integerValueForKey:CyberKitLayoutIntervalPreferenceKey];
}

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

- (void)_setAlwaysUseAcceleratedOverflowScroll:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAlwaysUseAcceleratedOverflowScrollPreferenceKey];
}

- (BOOL)_alwaysUseAcceleratedOverflowScroll
{
    return [self _boolValueForKey:CyberKitAlwaysUseAcceleratedOverflowScrollPreferenceKey];
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
    WebTextDirectionSubmenuInclusionBehavior value = static_cast<WebTextDirectionSubmenuInclusionBehavior>([self _integerValueForKey:CyberKitTextDirectionSubmenuInclusionBehaviorPreferenceKey]);
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

- (BOOL)experimentalNotificationsEnabled
{
    return [self _boolValueForKey:CyberKitExperimentalNotificationsEnabledPreferenceKey];
}

- (void)setExperimentalNotificationsEnabled:(BOOL)experimentalNotificationsEnabled
{
    [self _setBoolValue:experimentalNotificationsEnabled forKey:CyberKitExperimentalNotificationsEnabledPreferenceKey];
}

+ (WebPreferences *)_getInstanceForIdentifier:(NSString *)ident
{
    LOG(Encoding, "requesting for %@\n", ident);

    if (!ident)
        return _standardPreferences;
    
    WebPreferences *instance = [webPreferencesInstances objectForKey:[self _concatenateKeyWithIBCreatorID:ident]];

    return instance;
}

+ (void)_setInstance:(WebPreferences *)instance forIdentifier:(NSString *)ident
{
    if (!webPreferencesInstances)
        webPreferencesInstances = [[NSMutableDictionary alloc] init];
    if (ident) {
        [webPreferencesInstances setObject:instance forKey:[self _concatenateKeyWithIBCreatorID:ident]];
        LOG(Encoding, "recording %p for %@\n", instance, [self _concatenateKeyWithIBCreatorID:ident]);
    }
}

+ (void)_checkLastReferenceForIdentifier:(id)identifier
{
    // FIXME: This won't work at all under garbage collection because retainCount returns a constant.
    // We may need to change WebPreferences API so there's an explicit way to end the lifetime of one.
    WebPreferences *instance = [webPreferencesInstances objectForKey:identifier];
    if ([instance retainCount] == 1)
        [webPreferencesInstances removeObjectForKey:identifier];
}

+ (void)_removeReferenceForIdentifier:(NSString *)ident
{
    if (ident)
        [self performSelector:@selector(_checkLastReferenceForIdentifier:) withObject:[self _concatenateKeyWithIBCreatorID:ident] afterDelay:0.1];
}

- (void)_postPreferencesChangedNotification
{
#if !PLATFORM(IOS_FAMILY)
    if (!pthread_main_np()) {
        [self performSelectorOnMainThread:_cmd withObject:nil waitUntilDone:NO];
        return;
    }
#endif

    [[NSNotificationCenter defaultCenter] postNotificationName:WebPreferencesChangedInternalNotification object:self userInfo:nil];
    [[NSNotificationCenter defaultCenter] postNotificationName:WebPreferencesChangedNotification object:self userInfo:nil];
}

- (void)_postPreferencesChangedAPINotification
{
    if (!pthread_main_np()) {
        [self performSelectorOnMainThread:_cmd withObject:nil waitUntilDone:NO];
        return;
    }

    [[NSNotificationCenter defaultCenter] postNotificationName:WebPreferencesChangedNotification object:self userInfo:nil];
}

+ (CFStringEncoding)_systemCFStringEncoding
{
    return webDefaultCFStringEncoding();
}

+ (void)_setInitialDefaultTextEncodingToSystemEncoding
{
    [[NSUserDefaults standardUserDefaults] registerDefaults:
        [NSDictionary dictionaryWithObject:defaultTextEncodingNameForSystemLanguage() forKey:CyberKitDefaultTextEncodingNamePreferenceKey]];
}

static NSString *classIBCreatorID = nil;

+ (void)_setIBCreatorID:(NSString *)string
{
    NSString *old = classIBCreatorID;
    classIBCreatorID = [string copy];
    [old release];
}

+ (void)_switchNetworkLoaderToNewTestingSession
{
#if PLATFORM(IOS_FAMILY)
    WebThreadLock();
#endif
    NetworkStorageSession::switchToNewTestingSession();
}

+ (void)_clearNetworkLoaderSession
{
    NetworkStorageSession::defaultStorageSession().deleteAllCookies();
}

+ (void)_setCurrentNetworkLoaderSessionCookieAcceptPolicy:(NSHTTPCookieAcceptPolicy)policy
{
    RetainPtr<CFHTTPCookieStorageRef> cookieStorage = NetworkStorageSession::defaultStorageSession().cookieStorage();
    ASSERT(cookieStorage); // Will fail when NetworkStorageSession::switchToNewTestingSession() was not called beforehand.
    CFHTTPCookieStorageSetCookieAcceptPolicy(cookieStorage.get(), policy);
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

- (BOOL)subpixelAntialiasedLayerTextEnabled
{
    return [self _boolValueForKey:CyberKitSubpixelAntialiasedLayerTextEnabledPreferenceKey];
}

- (void)setSubpixelAntialiasedLayerTextEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitSubpixelAntialiasedLayerTextEnabledPreferenceKey];
}

- (BOOL)simpleLineLayoutEnabled
{
    return [self _boolValueForKey:CyberKitSimpleLineLayoutEnabledPreferenceKey];
}

- (void)setSimpleLineLayoutEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitSimpleLineLayoutEnabledPreferenceKey];
}

- (BOOL)simpleLineLayoutDebugBordersEnabled
{
    return [self _boolValueForKey:CyberKitSimpleLineLayoutDebugBordersEnabledPreferenceKey];
}

- (void)setSimpleLineLayoutDebugBordersEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitSimpleLineLayoutDebugBordersEnabledPreferenceKey];
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

- (BOOL)subpixelCSSOMElementMetricsEnabled
{
    return [self _boolValueForKey:CyberKitSubpixelCSSOMElementMetricsEnabledPreferenceKey];
}

- (void)setSubpixelCSSOMElementMetricsEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitSubpixelCSSOMElementMetricsEnabledPreferenceKey];
}

- (BOOL)webGLEnabled
{
    return [self _boolValueForKey:CyberKitWebGLEnabledPreferenceKey];
}

- (void)setWebGLEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitWebGLEnabledPreferenceKey];
}

- (BOOL)webGL2Enabled
{
    return [self _boolValueForKey:CyberKitWebGL2EnabledPreferenceKey];
}

- (void)setWebGL2Enabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitWebGL2EnabledPreferenceKey];
}

- (BOOL)forceSoftwareWebGLRendering
{
    return [self _boolValueForKey:CyberKitForceSoftwareWebGLRenderingPreferenceKey];
}

- (void)setForceSoftwareWebGLRendering:(BOOL)forced
{
    [self _setBoolValue:forced forKey:CyberKitForceSoftwareWebGLRenderingPreferenceKey];
}

- (BOOL)forceLowPowerGPUForWebGL
{
    return [self _boolValueForKey:CyberKitForceWebGLUsesLowPowerPreferenceKey];
}

- (void)setForceWebGLUsesLowPower:(BOOL)forceLowPower
{
    [self _setBoolValue:forceLowPower forKey:CyberKitForceWebGLUsesLowPowerPreferenceKey];
}

- (BOOL)webGPUEnabled
{
    return [self _boolValueForKey:CyberKitWebGPUEnabledPreferenceKey];
}

- (void)setWebGPUEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitWebGPUEnabledPreferenceKey];
}

- (BOOL)webMetalEnabled
{
    return [self _boolValueForKey:CyberKitWebMetalEnabledPreferenceKey];
}

- (void)setWebMetalEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitWebMetalEnabledPreferenceKey];
}

- (BOOL)accelerated2dCanvasEnabled
{
    return [self _boolValueForKey:CyberKitAccelerated2dCanvasEnabledPreferenceKey];
}

- (void)setAccelerated2dCanvasEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitAccelerated2dCanvasEnabledPreferenceKey];
}

- (void)setDiskImageCacheEnabled:(BOOL)enabled
{
    // Staging. Can be removed once there are no more callers.
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

- (BOOL)paginateDuringLayoutEnabled
{
    return [self _boolValueForKey:CyberKitPaginateDuringLayoutEnabledPreferenceKey];
}

- (void)setPaginateDuringLayoutEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitPaginateDuringLayoutEnabledPreferenceKey];
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

- (void)_setPreferenceForTestWithValue:(NSString *)value forKey:(NSString *)key
{
    [self _setStringValue:value forKey:key];
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
    [self _setBoolValue:flag forKey:CyberKitAVFoundationNSURLSessionEnabledKey];
}

- (BOOL)isAVFoundationNSURLSessionEnabled
{
    return [self _boolValueForKey:CyberKitAVFoundationNSURLSessionEnabledKey];
}

- (void)setVideoPluginProxyEnabled:(BOOL)flag
{
    // No-op, left for SPI compatibility.
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
    return false;
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
#if HAVE(AUDIO_TOOLBOX_AUDIO_SESSION)
    if (override > AudioSession::AudioProcessing) {
        // Clients are passing us OSTypes values from AudioToolbox/AudioSession.h,
        // which need to be translated into AudioSession::CategoryType:
        switch (override) {
        case kAudioSessionCategory_AmbientSound:
            override = AudioSession::AmbientSound;
            break;
        case kAudioSessionCategory_SoloAmbientSound:
            override = AudioSession::SoloAmbientSound;
            break;
        case kAudioSessionCategory_MediaPlayback:
            override = AudioSession::MediaPlayback;
            break;
        case kAudioSessionCategory_RecordAudio:
            override = AudioSession::RecordAudio;
            break;
        case kAudioSessionCategory_PlayAndRecord:
            override = AudioSession::PlayAndRecord;
            break;
        case kAudioSessionCategory_AudioProcessing:
            override = AudioSession::AudioProcessing;
            break;
        default:
            override = AudioSession::None;
            break;
        }
    }
#endif

    [self _setUnsignedIntValue:override forKey:CyberKitAudioSessionCategoryOverride];
}

- (BOOL)avKitEnabled
{
    return [self _boolValueForKey:CyberKitAVKitEnabled];
}

- (void)setAVKitEnabled:(bool)flag
{
#if HAVE(AVKIT)
    [self _setBoolValue:flag forKey:CyberKitAVKitEnabled];
#endif
}

- (BOOL)networkDataUsageTrackingEnabled
{
    return [self _boolValueForKey:CyberKitNetworkDataUsageTrackingEnabledPreferenceKey];
}

- (void)setNetworkDataUsageTrackingEnabled:(bool)trackingEnabled
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
    dispatch_barrier_sync(_private->readWriteQueue, ^{
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

- (BOOL)requestAnimationFrameEnabled
{
    return [self _boolValueForKey:CyberKitRequestAnimationFrameEnabledPreferenceKey];
}

- (void)setRequestAnimationFrameEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitRequestAnimationFrameEnabledPreferenceKey];
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
    NSDictionary *dict = [NSDictionary dictionaryWithObject:[NSNumber numberWithInt:storageBlockingPolicy] forKey:CyberKitStorageBlockingPolicyKey];
    [[NSUserDefaults standardUserDefaults] registerDefaults:dict];
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
    return [self _boolValueForKey:CyberKitPlugInSnapshottingEnabledPreferenceKey];
}

- (void)setPlugInSnapshottingEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitPlugInSnapshottingEnabledPreferenceKey];
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
    return [self _boolValueForKey:CyberKitImageControlsEnabledPreferenceKey];
}

- (void)setImageControlsEnabled:(BOOL)enabled
{
    [self _setBoolValue:enabled forKey:CyberKitImageControlsEnabledPreferenceKey];
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

- (BOOL)shadowDOMEnabled
{
    return [self _boolValueForKey:CyberKitShadowDOMEnabledPreferenceKey];
}

- (void)setShadowDOMEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitShadowDOMEnabledPreferenceKey];
}

- (BOOL)customElementsEnabled
{
    return [self _boolValueForKey:CyberKitCustomElementsEnabledPreferenceKey];
}

- (void)setCustomElementsEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitCustomElementsEnabledPreferenceKey];
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

- (BOOL)fetchAPIEnabled
{
    return [self _boolValueForKey:CyberKitFetchAPIEnabledPreferenceKey];
}

- (void)setFetchAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitFetchAPIEnabledPreferenceKey];
}

- (BOOL)readableByteStreamAPIEnabled
{
    return [self _boolValueForKey:CyberKitReadableByteStreamAPIEnabledPreferenceKey];
}

- (void)setReadableByteStreamAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitReadableByteStreamAPIEnabledPreferenceKey];
}

- (BOOL)writableStreamAPIEnabled
{
    return [self _boolValueForKey:CyberKitWritableStreamAPIEnabledPreferenceKey];
}

- (void)setWritableStreamAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitWritableStreamAPIEnabledPreferenceKey];
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

- (BOOL)visualViewportEnabled
{
    return [self _boolValueForKey:CyberKitVisualViewportEnabledPreferenceKey];
}

- (void)setVisualViewportEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitVisualViewportEnabledPreferenceKey];
}

- (BOOL)visualViewportAPIEnabled
{
    return [self _boolValueForKey:CyberKitVisualViewportAPIEnabledPreferenceKey];
}

- (void)setVisualViewportAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitVisualViewportAPIEnabledPreferenceKey];
}

- (BOOL)CSSOMViewScrollingAPIEnabled
{
    return [self _boolValueForKey:CyberKitCSSOMViewScrollingAPIEnabledPreferenceKey];
}

- (void)setCSSOMViewScrollingAPIEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitCSSOMViewScrollingAPIEnabledPreferenceKey];
}

- (BOOL)webAnimationsEnabled
{
    return [self _boolValueForKey:CyberKitWebAnimationsEnabledPreferenceKey];
}

- (void)setWebAnimationsEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitWebAnimationsEnabledPreferenceKey];
}

- (BOOL)fetchAPIKeepAliveEnabled
{
    return [self _boolValueForKey:CyberKitFetchAPIEnabledPreferenceKey];
}

- (void)setFetchAPIKeepAliveEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitFetchAPIEnabledPreferenceKey];
}

- (BOOL)modernMediaControlsEnabled
{
    return [self _boolValueForKey:CyberKitModernMediaControlsEnabledPreferenceKey];
}

- (void)setModernMediaControlsEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitModernMediaControlsEnabledPreferenceKey];
}

- (BOOL)webAnimationsCSSIntegrationEnabled
{
    return [self _boolValueForKey:CyberKitWebAnimationsCSSIntegrationEnabledPreferenceKey];
}

- (void)setWebAnimationsCSSIntegrationEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitWebAnimationsCSSIntegrationEnabledPreferenceKey];
}

- (BOOL)intersectionObserverEnabled
{
    return [self _boolValueForKey:CyberKitIntersectionObserverEnabledPreferenceKey];
}

- (void)setIntersectionObserverEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitIntersectionObserverEnabledPreferenceKey];
}

- (BOOL)menuItemElementEnabled
{
    return [self _boolValueForKey:CyberKitMenuItemElementEnabledPreferenceKey];
}

- (void)setMenuItemElementEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMenuItemElementEnabledPreferenceKey];
}

- (BOOL)displayContentsEnabled
{
    return [self _boolValueForKey:CyberKitDisplayContentsEnabledPreferenceKey];
}

- (void)setDisplayContentsEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitDisplayContentsEnabledPreferenceKey];
}

- (BOOL)userTimingEnabled
{
    return [self _boolValueForKey:CyberKitUserTimingEnabledPreferenceKey];
}

- (void)setUserTimingEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitUserTimingEnabledPreferenceKey];
}

- (BOOL)resourceTimingEnabled
{
    return [self _boolValueForKey:CyberKitResourceTimingEnabledPreferenceKey];
}

- (void)setResourceTimingEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitResourceTimingEnabledPreferenceKey];
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

- (BOOL)isSecureContextAttributeEnabled
{
    return [self _boolValueForKey:CyberKitIsSecureContextAttributeEnabledPreferenceKey];
}

- (void)setIsSecureContextAttributeEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitIsSecureContextAttributeEnabledPreferenceKey];
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

- (BOOL)viewportFitEnabled
{
    return [self _boolValueForKey:CyberKitViewportFitEnabledPreferenceKey];
}

- (void)setViewportFitEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitViewportFitEnabledPreferenceKey];
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

- (BOOL)inspectorAdditionsEnabled
{
    return [self _boolValueForKey:CyberKitInspectorAdditionsEnabledPreferenceKey];
}

- (void)setInspectorAdditionsEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitInspectorAdditionsEnabledPreferenceKey];
}

- (BOOL)accessibilityObjectModelEnabled
{
    return [self _boolValueForKey:CyberKitAccessibilityObjectModelEnabledPreferenceKey];
}

- (void)setAccessibilityObjectModelEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAccessibilityObjectModelEnabledPreferenceKey];
}

- (BOOL)ariaReflectionEnabled
{
    return [self _boolValueForKey:CyberKitAriaReflectionEnabledPreferenceKey];
}

- (void)setAriaReflectionEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitAriaReflectionEnabledPreferenceKey];
}

- (BOOL)mediaCapabilitiesEnabled
{
    return [self _boolValueForKey:CyberKitMediaCapabilitiesEnabledPreferenceKey];
}

- (void)setMediaCapabilitiesEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMediaCapabilitiesEnabledPreferenceKey];
}

- (BOOL)mediaRecorderEnabled
{
    return [self _boolValueForKey:CyberKitMediaRecorderEnabledPreferenceKey];
}

- (void)setMediaRecorderEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitMediaRecorderEnabledPreferenceKey];
}

- (BOOL)serverTimingEnabled
{
    return [self _boolValueForKey:CyberKitServerTimingEnabledPreferenceKey];
}

- (void)setServerTimingEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitServerTimingEnabledPreferenceKey];
}

- (BOOL)selectionAcrossShadowBoundariesEnabled
{
    return [self _boolValueForKey:CyberKitSelectionAcrossShadowBoundariesEnabledPreferenceKey];
}

- (void)setSelectionAcrossShadowBoundariesEnabled:(BOOL)flag
{
    [self _setBoolValue:flag forKey:CyberKitSelectionAcrossShadowBoundariesEnabledPreferenceKey];
}

@end

@implementation WebPreferences (WebInternal)

+ (NSString *)_IBCreatorID
{
    return classIBCreatorID;
}

+ (NSString *)_concatenateKeyWithIBCreatorID:(NSString *)key
{
    NSString *IBCreatorID = [WebPreferences _IBCreatorID];
    if (!IBCreatorID)
        return key;
    return [IBCreatorID stringByAppendingString:key];
}

@end
