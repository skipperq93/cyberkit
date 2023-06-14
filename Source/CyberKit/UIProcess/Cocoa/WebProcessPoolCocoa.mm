/*
 * Copyright (C) 2010-2018 Apple Inc. All rights reserved.
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

#import "config.h"
#import "WebProcessPool.h"

#import "CookieStorageUtilsCF.h"
#import "LegacyCustomProtocolManagerClient.h"
#import "NetworkProcessCreationParameters.h"
#import "NetworkProcessMessages.h"
#import "NetworkProcessProxy.h"
#import "PluginProcessManager.h"
#import "SandboxUtilities.h"
#import "TextChecker.h"
#import "VersionChecks.h"
#import "WKBrowsingContextControllerInternal.h"
#import "WebMemoryPressureHandlerCocoa.h"
#import "WebPageGroup.h"
#import "WebPreferencesKeys.h"
#import "WebProcessCreationParameters.h"
#import "WebProcessMessages.h"
#import "WindowServerConnection.h"
#import <CyberCore/Color.h>
#import <CyberCore/FileSystem.h>
#import <CyberCore/NetworkStorageSession.h>
#import <CyberCore/NotImplemented.h>
#import <CyberCore/PlatformPasteboard.h>
#import <CyberCore/RuntimeApplicationChecks.h>
#import <CyberCore/SharedBuffer.h>
#import <pal/spi/cf/CFNetworkSPI.h>
#import <pal/spi/cocoa/NSKeyedArchiverSPI.h>
#import <sys/param.h>
#import <wtf/ProcessPrivilege.h>
#import <wtf/cocoa/Entitlements.h>
#import <wtf/spi/darwin/dyldSPI.h>

#if PLATFORM(MAC)
#import <QuartzCore/CARemoteLayerServer.h>
#endif

NSString *WebServiceWorkerRegistrationDirectoryDefaultsKey = @"WebServiceWorkerRegistrationDirectory";
NSString *CyberKitLocalCacheDefaultsKey = @"CyberKitLocalCache";
NSString *CyberKitJSCJITEnabledDefaultsKey = @"CyberKitJSCJITEnabledDefaultsKey";
NSString *CyberKitJSCFTLJITEnabledDefaultsKey = @"CyberKitJSCFTLJITEnabledDefaultsKey";

#if !PLATFORM(IOS_FAMILY)
static NSString *CyberKitApplicationDidChangeAccessibilityEnhancedUserInterfaceNotification = @"NSApplicationDidChangeAccessibilityEnhancedUserInterfaceNotification";
#endif

static NSString * const CyberKitNetworkCacheEfficacyLoggingEnabledDefaultsKey = @"CyberKitNetworkCacheEfficacyLoggingEnabled";

static NSString * const CyberKitSuppressMemoryPressureHandlerDefaultsKey = @"CyberKitSuppressMemoryPressureHandler";

#if ENABLE(RESOURCE_LOAD_STATISTICS) && !RELEASE_LOG_DISABLED
static NSString * const CyberKitLogCookieInformationDefaultsKey = @"CyberKitLogCookieInformation";
#endif

namespace CyberKit {
using namespace CyberCore;

static void registerUserDefaultsIfNeeded()
{
    static bool didRegister;
    if (didRegister)
        return;

    didRegister = true;
    NSMutableDictionary *registrationDictionary = [NSMutableDictionary dictionary];
    
    [registrationDictionary setObject:@YES forKey:CyberKitJSCJITEnabledDefaultsKey];
    [registrationDictionary setObject:@YES forKey:CyberKitJSCFTLJITEnabledDefaultsKey];

    [registrationDictionary setObject:@NO forKey:CyberKitNetworkCacheEfficacyLoggingEnabledDefaultsKey];

    [[NSUserDefaults standardUserDefaults] registerDefaults:registrationDictionary];
}

void WebProcessPool::updateProcessSuppressionState()
{
    if (m_networkProcess)
        m_networkProcess->setProcessSuppressionEnabled(processSuppressionEnabled());

#if ENABLE(NETSCAPE_PLUGIN_API)
    if (!m_processSuppressionDisabledForPageCounter.value())
        m_pluginProcessManagerProcessSuppressionDisabledToken = nullptr;
    else if (!m_pluginProcessManagerProcessSuppressionDisabledToken)
        m_pluginProcessManagerProcessSuppressionDisabledToken = PluginProcessManager::singleton().processSuppressionDisabledToken();
#endif
}

NSMutableDictionary *WebProcessPool::ensureBundleParameters()
{
    if (!m_bundleParameters)
        m_bundleParameters = adoptNS([[NSMutableDictionary alloc] init]);

    return m_bundleParameters.get();
}

void WebProcessPool::platformInitialize()
{
    registerUserDefaultsIfNeeded();
    registerNotificationObservers();

    installMemoryPressureHandler();

    setLegacyCustomProtocolManagerClient(std::make_unique<LegacyCustomProtocolManagerClient>());
}

#if PLATFORM(IOS_FAMILY)
String WebProcessPool::cookieStorageDirectory() const
{
    String path = pathForProcessContainer();
    if (path.isEmpty())
        path = NSHomeDirectory();

    path = path + "/Library/Cookies";
    path = stringByResolvingSymlinksInPath(path);
    return path;
}
#endif

void WebProcessPool::platformResolvePathsForSandboxExtensions()
{
    m_resolvedPaths.uiProcessBundleResourcePath = resolvePathForSandboxExtension([[NSBundle mainBundle] resourcePath]);

#if PLATFORM(IOS_FAMILY)
    m_resolvedPaths.cookieStorageDirectory = resolveAndCreateReadWriteDirectoryForSandboxExtension(cookieStorageDirectory());
    m_resolvedPaths.containerCachesDirectory = resolveAndCreateReadWriteDirectoryForSandboxExtension(webContentCachesDirectory());
    m_resolvedPaths.containerTemporaryDirectory = resolveAndCreateReadWriteDirectoryForSandboxExtension(containerTemporaryDirectory());
#endif
}

void WebProcessPool::platformInitializeWebProcess(WebProcessCreationParameters& parameters)
{
#if PLATFORM(MAC)
    ALLOW_DEPRECATED_DECLARATIONS_BEGIN
    ASSERT(hasProcessPrivilege(ProcessPrivilege::CanCommunicateWithWindowServer));
    parameters.accessibilityEnhancedUserInterfaceEnabled = [[NSApp accessibilityAttributeValue:@"AXEnhancedUserInterface"] boolValue];
    ALLOW_DEPRECATED_DECLARATIONS_END
#else
    parameters.accessibilityEnhancedUserInterfaceEnabled = false;
#endif

    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];

    parameters.shouldEnableJIT = [defaults boolForKey:CyberKitJSCJITEnabledDefaultsKey];
    parameters.shouldEnableFTLJIT = [defaults boolForKey:CyberKitJSCFTLJITEnabledDefaultsKey];
    parameters.shouldEnableMemoryPressureReliefLogging = [defaults boolForKey:@"LogMemoryJetsamDetails"];
    parameters.shouldSuppressMemoryPressureHandler = [defaults boolForKey:CyberKitSuppressMemoryPressureHandlerDefaultsKey];

#if HAVE(HOSTED_CORE_ANIMATION)
#if !PLATFORM(IOS_FAMILY)
    parameters.acceleratedCompositingPort = MachSendRight::create([CARemoteLayerServer sharedServer].serverPort);
#endif
#endif

    // FIXME: This should really be configurable; we shouldn't just blindly allow read access to the UI process bundle.
    parameters.uiProcessBundleResourcePath = m_resolvedPaths.uiProcessBundleResourcePath;
    SandboxExtension::createHandleWithoutResolvingPath(parameters.uiProcessBundleResourcePath, SandboxExtension::Type::ReadOnly, parameters.uiProcessBundleResourcePathExtensionHandle);

    parameters.uiProcessBundleIdentifier = String([[NSBundle mainBundle] bundleIdentifier]);
    parameters.uiProcessSDKVersion = dyld_get_program_sdk_version();

#if PLATFORM(IOS_FAMILY)
    if (!m_resolvedPaths.cookieStorageDirectory.isEmpty())
        SandboxExtension::createHandleWithoutResolvingPath(m_resolvedPaths.cookieStorageDirectory, SandboxExtension::Type::ReadWrite, parameters.cookieStorageDirectoryExtensionHandle);

    if (!m_resolvedPaths.containerCachesDirectory.isEmpty())
        SandboxExtension::createHandleWithoutResolvingPath(m_resolvedPaths.containerCachesDirectory, SandboxExtension::Type::ReadWrite, parameters.containerCachesDirectoryExtensionHandle);

    if (!m_resolvedPaths.containerTemporaryDirectory.isEmpty())
        SandboxExtension::createHandleWithoutResolvingPath(m_resolvedPaths.containerTemporaryDirectory, SandboxExtension::Type::ReadWrite, parameters.containerTemporaryDirectoryExtensionHandle);
#endif

    parameters.fontWhitelist = m_fontWhitelist;

    if (m_bundleParameters) {
        auto keyedArchiver = secureArchiver();

        @try {
            [keyedArchiver encodeObject:m_bundleParameters.get() forKey:@"parameters"];
            [keyedArchiver finishEncoding];
        } @catch (NSException *exception) {
            LOG_ERROR("Failed to encode bundle parameters: %@", exception);
        }

        auto data = keyedArchiver.get().encodedData;

        parameters.bundleParameterData = API::Data::createWithoutCopying(WTFMove(data));
    }
    parameters.networkATSContext = adoptCF(_CFNetworkCopyATSContext());

#if ENABLE(MEDIA_STREAM)
    // Allow microphone access if either preference is set because WebRTC requires microphone access.
    bool mediaDevicesEnabled = m_defaultPageGroup->preferences().mediaDevicesEnabled();
    bool webRTCEnabled = m_defaultPageGroup->preferences().peerConnectionEnabled();
    if ([defaults objectForKey:@"ExperimentalPeerConnectionEnabled"])
        webRTCEnabled = [defaults boolForKey:@"ExperimentalPeerConnectionEnabled"];

    bool isSafari = false;
#if PLATFORM(IOS_FAMILY)
    if (CyberCore::IOSApplication::isMobileSafari())
        isSafari = true;
#elif PLATFORM(MAC)
    if (CyberCore::MacApplication::isSafari())
        isSafari = true;
#endif

#if !LOG_DISABLED || !RELEASE_LOG_DISABLED
    parameters.webCoreLoggingChannels = [[NSUserDefaults standardUserDefaults] stringForKey:@"CyberCoreLogging"];
    parameters.webKitLoggingChannels = [[NSUserDefaults standardUserDefaults] stringForKey:@"CyberKit2Logging"];
#endif

    // FIXME: Remove this and related parameter when <rdar://problem/29448368> is fixed.
    if (isSafari && !parameters.shouldCaptureAudioInUIProcess && mediaDevicesEnabled)
        SandboxExtension::createHandleForGenericExtension("com.apple.webkit.microphone", parameters.audioCaptureExtensionHandle);
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS) && !RELEASE_LOG_DISABLED
    parameters.shouldLogUserInteraction = [defaults boolForKey:CyberKitLogCookieInformationDefaultsKey];
#endif
    
#if PLATFORM(MAC)
    auto screenProperties = CyberCore::collectScreenProperties();
    parameters.screenProperties = WTFMove(screenProperties);
    parameters.useOverlayScrollbars = ([NSScroller preferredScrollerStyle] == NSScrollerStyleOverlay);
#endif
}

void WebProcessPool::platformInitializeNetworkProcess(NetworkProcessCreationParameters& parameters)
{
    parameters.uiProcessBundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
    parameters.uiProcessSDKVersion = dyld_get_program_sdk_version();

    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];

    {
        bool isSafari = false;
#if PLATFORM(IOS_FAMILY)
        isSafari = CyberCore::IOSApplication::isMobileSafari();
#elif PLATFORM(MAC)
        isSafari = CyberCore::MacApplication::isSafari();
#endif
        if (isSafari) {
            parameters.defaultDataStoreParameters.networkSessionParameters.httpProxy = URL(URL(), [defaults stringForKey:(NSString *)CyberKit2HTTPProxyDefaultsKey]);
            parameters.defaultDataStoreParameters.networkSessionParameters.httpsProxy = URL(URL(), [defaults stringForKey:(NSString *)CyberKit2HTTPSProxyDefaultsKey]);
        }
    }

    parameters.networkATSContext = adoptCF(_CFNetworkCopyATSContext());

    parameters.shouldEnableNetworkCacheEfficacyLogging = [defaults boolForKey:CyberKitNetworkCacheEfficacyLoggingEnabledDefaultsKey];

#if PLATFORM(IOS_FAMILY)
    parameters.ctDataConnectionServiceType = m_configuration->ctDataConnectionServiceType();
#endif

    parameters.shouldSuppressMemoryPressureHandler = [defaults boolForKey:CyberKitSuppressMemoryPressureHandlerDefaultsKey];

#if PLATFORM(MAC)
    ASSERT(parameters.uiProcessCookieStorageIdentifier.isEmpty());
    ASSERT(hasProcessPrivilege(ProcessPrivilege::CanAccessRawCookies));
    parameters.uiProcessCookieStorageIdentifier = identifyingDataFromCookieStorage([[NSHTTPCookieStorage sharedHTTPCookieStorage] _cookieStorage]);
#endif

    parameters.storageAccessAPIEnabled = storageAccessAPIEnabled();
    parameters.suppressesConnectionTerminationOnSystemChange = m_configuration->suppressesConnectionTerminationOnSystemChange();

#if ENABLE(PROXIMITY_NETWORKING)
    parameters.wirelessContextIdentifier = m_configuration->wirelessContextIdentifier();
#endif
}

void WebProcessPool::platformInvalidateContext()
{
    unregisterNotificationObservers();
}

#if PLATFORM(IOS_FAMILY)
String WebProcessPool::parentBundleDirectory() const
{
    return [[[NSBundle mainBundle] bundlePath] stringByStandardizingPath];
}

String WebProcessPool::networkingCachesDirectory() const
{
    String path = pathForProcessContainer();
    if (path.isEmpty())
        path = NSHomeDirectory();

    path = path + "/Library/Caches/com.matthewbenedict.CyberKit.Networking/";
    path = stringByResolvingSymlinksInPath(path);

    NSError *error = nil;
    NSString* nsPath = path;
    if (![[NSFileManager defaultManager] createDirectoryAtPath:nsPath withIntermediateDirectories:YES attributes:nil error:&error]) {
        NSLog(@"could not create networking caches directory \"%@\", error %@", nsPath, error);
        return String();
    }

    return path;
}

String WebProcessPool::webContentCachesDirectory() const
{
    String path = pathForProcessContainer();
    if (path.isEmpty())
        path = NSHomeDirectory();

    path = path + "/Library/Caches/com.matthewbenedict.CyberKit.WebContent/";
    path = stringByResolvingSymlinksInPath(path);

    NSError *error = nil;
    NSString* nsPath = path;
    if (![[NSFileManager defaultManager] createDirectoryAtPath:nsPath withIntermediateDirectories:YES attributes:nil error:&error]) {
        NSLog(@"could not create web content caches directory \"%@\", error %@", nsPath, error);
        return String();
    }

    return path;
}

String WebProcessPool::containerTemporaryDirectory() const
{
    String path = NSTemporaryDirectory();
    return stringByResolvingSymlinksInPath(path);
}
#endif

#if PLATFORM(IOS_FAMILY)
void WebProcessPool::setJavaScriptConfigurationFileEnabledFromDefaults()
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];

    setJavaScriptConfigurationFileEnabled([defaults boolForKey:@"CyberKitCyberScriptCoreUseConfigFile"]);
}
#endif

bool WebProcessPool::omitPDFSupport()
{
    // Since this is a "secret default" we don't bother registering it.
    return [[NSUserDefaults standardUserDefaults] boolForKey:@"CyberKitOmitPDFSupport"];
}

bool WebProcessPool::processSuppressionEnabled() const
{
    return !m_userObservablePageCounter.value() && !m_processSuppressionDisabledForPageCounter.value();
}

bool WebProcessPool::networkProcessHasEntitlementForTesting(const String& entitlement)
{
    return WTF::hasEntitlement(ensureNetworkProcess().connection()->xpcConnection(), entitlement.utf8().data());
}

void WebProcessPool::registerNotificationObservers()
{
#if !PLATFORM(IOS_FAMILY)
    // Listen for enhanced accessibility changes and propagate them to the WebProcess.
    m_enhancedAccessibilityObserver = [[NSNotificationCenter defaultCenter] addObserverForName:CyberKitApplicationDidChangeAccessibilityEnhancedUserInterfaceNotification object:nil queue:[NSOperationQueue currentQueue] usingBlock:^(NSNotification *note) {
        setEnhancedAccessibility([[[note userInfo] objectForKey:@"AXEnhancedUserInterface"] boolValue]);
    }];

    m_automaticTextReplacementNotificationObserver = [[NSNotificationCenter defaultCenter] addObserverForName:NSSpellCheckerDidChangeAutomaticTextReplacementNotification object:nil queue:[NSOperationQueue currentQueue] usingBlock:^(NSNotification *notification) {
        TextChecker::didChangeAutomaticTextReplacementEnabled();
        textCheckerStateChanged();
    }];
    
    m_automaticSpellingCorrectionNotificationObserver = [[NSNotificationCenter defaultCenter] addObserverForName:NSSpellCheckerDidChangeAutomaticSpellingCorrectionNotification object:nil queue:[NSOperationQueue currentQueue] usingBlock:^(NSNotification *notification) {
        TextChecker::didChangeAutomaticSpellingCorrectionEnabled();
        textCheckerStateChanged();
    }];

    m_automaticQuoteSubstitutionNotificationObserver = [[NSNotificationCenter defaultCenter] addObserverForName:NSSpellCheckerDidChangeAutomaticQuoteSubstitutionNotification object:nil queue:[NSOperationQueue currentQueue] usingBlock:^(NSNotification *notification) {
        TextChecker::didChangeAutomaticQuoteSubstitutionEnabled();
        textCheckerStateChanged();
    }];

    m_automaticDashSubstitutionNotificationObserver = [[NSNotificationCenter defaultCenter] addObserverForName:NSSpellCheckerDidChangeAutomaticDashSubstitutionNotification object:nil queue:[NSOperationQueue currentQueue] usingBlock:^(NSNotification *notification) {
        TextChecker::didChangeAutomaticDashSubstitutionEnabled();
        textCheckerStateChanged();
    }];

    m_accessibilityDisplayOptionsNotificationObserver = [[NSWorkspace.sharedWorkspace notificationCenter] addObserverForName:NSWorkspaceAccessibilityDisplayOptionsDidChangeNotification object:nil queue:[NSOperationQueue currentQueue] usingBlock:^(NSNotification *notification) {
        screenPropertiesStateChanged();
    }];

#if ENABLE(WEBPROCESS_WINDOWSERVER_BLOCKING)
    m_scrollerStyleNotificationObserver = [[NSNotificationCenter defaultCenter] addObserverForName:NSPreferredScrollerStyleDidChangeNotification object:nil queue:[NSOperationQueue currentQueue] usingBlock:^(NSNotification *notification) {
        auto scrollbarStyle = [NSScroller preferredScrollerStyle];
        sendToAllProcesses(Messages::WebProcess::ScrollerStylePreferenceChanged(scrollbarStyle));
    }];
#endif

    m_activationObserver = [[NSNotificationCenter defaultCenter] addObserverForName:NSApplicationDidBecomeActiveNotification object:NSApp queue:[NSOperationQueue currentQueue] usingBlock:^(NSNotification *notification) {
        setApplicationIsActive(true);
    }];

    m_deactivationObserver = [[NSNotificationCenter defaultCenter] addObserverForName:NSApplicationDidResignActiveNotification object:NSApp queue:[NSOperationQueue currentQueue] usingBlock:^(NSNotification *notification) {
        setApplicationIsActive(false);
    }];

#endif // !PLATFORM(IOS_FAMILY)
}

void WebProcessPool::unregisterNotificationObservers()
{
#if !PLATFORM(IOS_FAMILY)
    [[NSNotificationCenter defaultCenter] removeObserver:m_enhancedAccessibilityObserver.get()];    
    [[NSNotificationCenter defaultCenter] removeObserver:m_automaticTextReplacementNotificationObserver.get()];
    [[NSNotificationCenter defaultCenter] removeObserver:m_automaticSpellingCorrectionNotificationObserver.get()];
    [[NSNotificationCenter defaultCenter] removeObserver:m_automaticQuoteSubstitutionNotificationObserver.get()];
    [[NSNotificationCenter defaultCenter] removeObserver:m_automaticDashSubstitutionNotificationObserver.get()];
    [[NSWorkspace.sharedWorkspace notificationCenter] removeObserver:m_accessibilityDisplayOptionsNotificationObserver.get()];
#if ENABLE(WEBPROCESS_WINDOWSERVER_BLOCKING)
    [[NSNotificationCenter defaultCenter] removeObserver:m_scrollerStyleNotificationObserver.get()];
#endif
    [[NSNotificationCenter defaultCenter] removeObserver:m_activationObserver.get()];
    [[NSNotificationCenter defaultCenter] removeObserver:m_deactivationObserver.get()];
#endif // !PLATFORM(IOS_FAMILY)
}

static CFURLStorageSessionRef privateBrowsingSession()
{
    static CFURLStorageSessionRef session;
    static dispatch_once_t once;
    dispatch_once(&once, ^{
        NSString *identifier = [NSString stringWithFormat:@"%@.PrivateBrowsing", [[NSBundle mainBundle] bundleIdentifier]];
        session = createPrivateStorageSession((__bridge CFStringRef)identifier);
    });

    return session;
}

bool WebProcessPool::isURLKnownHSTSHost(const String& urlString, bool privateBrowsingEnabled) const
{
    RetainPtr<CFURLRef> url = URL(URL(), urlString).createCFURL();

    return _CFNetworkIsKnownHSTSHostWithSession(url.get(), privateBrowsingEnabled ? privateBrowsingSession() : nullptr);
}

void WebProcessPool::resetHSTSHosts()
{
    _CFNetworkResetHSTSHostsWithSession(nullptr);
    _CFNetworkResetHSTSHostsWithSession(privateBrowsingSession());
}

void WebProcessPool::resetHSTSHostsAddedAfterDate(double startDateIntervalSince1970)
{
    NSDate *startDate = [NSDate dateWithTimeIntervalSince1970:startDateIntervalSince1970];
    _CFNetworkResetHSTSHostsSinceDate(nullptr, (__bridge CFDateRef)startDate);
    _CFNetworkResetHSTSHostsSinceDate(privateBrowsingSession(), (__bridge CFDateRef)startDate);
}

// FIXME: Deprecated. Left here until a final decision is made.
void WebProcessPool::setCookieStoragePartitioningEnabled(bool enabled)
{
    m_cookieStoragePartitioningEnabled = enabled;
}

void WebProcessPool::setStorageAccessAPIEnabled(bool enabled)
{
    m_storageAccessAPIEnabled = enabled;
    sendToNetworkingProcess(Messages::NetworkProcess::SetStorageAccessAPIEnabled(enabled));
}

int networkProcessLatencyQOS()
{
    static const int qos = [[NSUserDefaults standardUserDefaults] integerForKey:@"CyberKitNetworkProcessLatencyQOS"];
    return qos;
}

int networkProcessThroughputQOS()
{
    static const int qos = [[NSUserDefaults standardUserDefaults] integerForKey:@"CyberKitNetworkProcessThroughputQOS"];
    return qos;
}

int webProcessLatencyQOS()
{
    static const int qos = [[NSUserDefaults standardUserDefaults] integerForKey:@"CyberKitWebProcessLatencyQOS"];
    return qos;
}

int webProcessThroughputQOS()
{
    static const int qos = [[NSUserDefaults standardUserDefaults] integerForKey:@"CyberKitWebProcessThroughputQOS"];
    return qos;
}

} // namespace CyberKit
