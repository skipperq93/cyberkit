/*
 * Copyright (C) 2006-2020 Apple Inc.  All rights reserved.
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

#include "CyberKit.h"
#include "CyberKitDLL.h"
#include "WebPreferences.h"

#include "NetworkStorageSessionMap.h"
#include "WebNotificationCenter.h"
#include "WebPreferenceKeysPrivate.h"

#if USE(CG)
#include <CoreGraphics/CoreGraphics.h>
#include <CyberCore/CACFLayerTreeHost.h>
#endif

#if USE(CF)
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <CyberCore/BString.h>
#include <CyberCore/COMPtr.h>
#include <CyberCore/FontCascade.h>
#include <CyberCore/LocalizedStrings.h>
#include <CyberCore/NetworkStorageSession.h>
#include <limits>
#include <shlobj.h>
#include <wchar.h>
#include <wtf/FileSystem.h>
#include <wtf/HashMap.h>
#include <wtf/NeverDestroyed.h>
#include <wtf/StdLibExtras.h>
#include <wtf/text/CString.h>
#include <wtf/text/StringHash.h>
#include <wtf/text/WTFString.h>

using namespace CyberCore;
using std::numeric_limits;

static const String& oldPreferencesPath()
{
    static String path = FileSystem::pathByAppendingComponent(FileSystem::roamingUserSpecificStorageDirectory(), "CyberKitPreferences.plist");
    return path;
}

#if USE(CF)

template<typename NumberType> struct CFNumberTraits { static const CFNumberType Type; };
template<> struct CFNumberTraits<int> { static const CFNumberType Type = kCFNumberSInt32Type; };
template<> struct CFNumberTraits<LONGLONG> { static const CFNumberType Type = kCFNumberLongLongType; };
template<> struct CFNumberTraits<float> { static const CFNumberType Type = kCFNumberFloat32Type; };

template<typename NumberType>
static NumberType numberValueForPreferencesValue(CFPropertyListRef value)
{
    if (!value)
        return 0;

    CFTypeID cfType = CFGetTypeID(value);
    if (cfType == CFStringGetTypeID())
        return static_cast<NumberType>(CFStringGetIntValue(static_cast<CFStringRef>(value)));
    else if (cfType == CFBooleanGetTypeID()) {
        Boolean boolVal = CFBooleanGetValue(static_cast<CFBooleanRef>(value));
        return boolVal ? 1 : 0;
    } else if (cfType == CFNumberGetTypeID()) {
        NumberType val = 0;
        CFNumberGetValue(static_cast<CFNumberRef>(value), CFNumberTraits<NumberType>::Type, &val);
        return val;
    }

    return 0;
}

template<typename NumberType>
static RetainPtr<CFNumberRef> cfNumber(NumberType value)
{
    return adoptCF(CFNumberCreate(0, CFNumberTraits<NumberType>::Type, &value));
}

static bool booleanValueForPreferencesValue(CFPropertyListRef value)
{
    return numberValueForPreferencesValue<int>(value);
}

// WebPreferences ----------------------------------------------------------------

static CFDictionaryRef defaultSettings;

RetainPtr<CFStringRef> WebPreferences::m_applicationId = kCFPreferencesCurrentApplication;
#endif

static HashMap<WTF::String, COMPtr<WebPreferences>>& webPreferencesInstances()
{
    static NeverDestroyed<HashMap<WTF::String, COMPtr<WebPreferences>>> webPreferencesInstances;
    return webPreferencesInstances;
}

WebPreferences* WebPreferences::sharedStandardPreferences()
{
    static WebPreferences* standardPreferences;
    if (!standardPreferences) {
        standardPreferences = WebPreferences::createInstance();
        standardPreferences->setAutosaves(TRUE);
        standardPreferences->load();
    }

    return standardPreferences;
}

WebPreferences::WebPreferences()
{
    gClassCount++;
    gClassNameCount().add("WebPreferences");
}

WebPreferences::~WebPreferences()
{
    gClassCount--;
    gClassNameCount().remove("WebPreferences");
}

WebPreferences* WebPreferences::createInstance()
{
    WebPreferences* instance = new WebPreferences();
    instance->AddRef();
    return instance;
}

HRESULT WebPreferences::postPreferencesChangesNotification()
{
    IWebNotificationCenter* nc = WebNotificationCenter::defaultCenterInternal();
    HRESULT hr = nc->postNotificationName(webPreferencesChangedNotification(), static_cast<IWebPreferences*>(this), 0);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

WebPreferences* WebPreferences::getInstanceForIdentifier(BSTR identifier)
{
    if (!identifier)
        return sharedStandardPreferences();

    WTF::String identifierString(identifier, SysStringLen(identifier));
    if (identifierString.isEmpty())
        return sharedStandardPreferences();

    return webPreferencesInstances().get(identifierString);
}

void WebPreferences::setInstance(WebPreferences* instance, BSTR identifier)
{
    if (!identifier || !instance)
        return;
    WTF::String identifierString(identifier, SysStringLen(identifier));
    if (identifierString.isEmpty())
        return;
    webPreferencesInstances().add(identifierString, instance);
}

void WebPreferences::removeReferenceForIdentifier(BSTR identifier)
{
    if (!identifier || webPreferencesInstances().isEmpty())
        return;

    WTF::String identifierString(identifier, SysStringLen(identifier));
    if (identifierString.isEmpty())
        return;
    WebPreferences* webPreference = webPreferencesInstances().get(identifierString);
    if (webPreference && webPreference->m_refCount == 1)
        webPreferencesInstances().remove(identifierString);
}

#if USE(CF)
CFStringRef WebPreferences::applicationId()
{
    return m_applicationId.get();
}
#endif

void WebPreferences::initializeDefaultSettings()
{
#if USE(CF)
    if (defaultSettings)
        return;

    CFMutableDictionaryRef defaults = CFDictionaryCreateMutable(0, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitStandardFontPreferenceKey), CFSTR("Times New Roman"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitFixedFontPreferenceKey), CFSTR("Courier New"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitSerifFontPreferenceKey), CFSTR("Times New Roman"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitSansSerifFontPreferenceKey), CFSTR("Arial"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitCursiveFontPreferenceKey), CFSTR("Comic Sans MS"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitFantasyFontPreferenceKey), CFSTR("Comic Sans MS"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitPictographFontPreferenceKey), CFSTR("Segoe UI Symbol"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitMinimumFontSizePreferenceKey), CFSTR("0"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitMinimumLogicalFontSizePreferenceKey), CFSTR("9"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitDefaultFontSizePreferenceKey), CFSTR("16"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitDefaultFixedFontSizePreferenceKey), CFSTR("13"));

    String defaultDefaultEncoding(WEB_UI_STRING("ISO-8859-1", "The default, default character encoding on Windows"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitDefaultTextEncodingNamePreferenceKey), defaultDefaultEncoding.createCFString().get());

    CFDictionaryAddValue(defaults, CFSTR(CyberKitUserStyleSheetEnabledPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitUserStyleSheetLocationPreferenceKey), CFSTR(""));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitShouldPrintBackgroundsPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitTextAreasAreResizablePreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitJavaEnabledPreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitJavaScriptEnabledPreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitJavaScriptRuntimeFlagsPreferenceKey), CFSTR("0"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitWebSecurityEnabledPreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitAllowTopNavigationToDataURLsPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitWebAudioEnabledPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitAllowUniversalAccessFromFileURLsPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitAllowFileAccessFromFileURLsPreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitJavaScriptCanAccessClipboardPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitXSSAuditorEnabledPreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitFrameFlatteningEnabledPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitJavaScriptCanOpenWindowsAutomaticallyPreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitPluginsEnabledPreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitCSSRegionsEnabledPreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitDatabasesEnabledPreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitLocalStorageEnabledPreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitExperimentalNotificationsEnabledPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitZoomsTextOnlyPreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitAllowAnimatedImagesPreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitAllowAnimatedImageLoopingPreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitDisplayImagesKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitLoadSiteIconsKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitBackForwardCacheExpirationIntervalKey), CFSTR("1800"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitTabToLinksPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitPrivateBrowsingEnabledPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitRespectStandardStyleKeyEquivalentsPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitShowsURLsInToolTipsPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitShowsToolTipOverTruncatedTextPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitPDFDisplayModePreferenceKey), CFSTR("1"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitPDFScaleFactorPreferenceKey), CFSTR("0"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitShouldDisplaySubtitlesPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitShouldDisplayCaptionsPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitShouldDisplayTextDescriptionsPreferenceKey), kCFBooleanFalse);

    RetainPtr<CFStringRef> linkBehaviorStringRef = adoptCF(CFStringCreateWithFormat(0, 0, CFSTR("%d"), CyberKitEditableLinkDefaultBehavior));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitEditableLinkBehaviorPreferenceKey), linkBehaviorStringRef.get());

    CFDictionaryAddValue(defaults, CFSTR(CyberKitHistoryItemLimitKey), CFSTR("1000"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitHistoryAgeInDaysLimitKey), CFSTR("7"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitIconDatabaseLocationKey), CFSTR(""));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitIconDatabaseEnabledPreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitFontSmoothingTypePreferenceKey), CFSTR("2"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitFontSmoothingContrastPreferenceKey), CFSTR("2"));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitCookieStorageAcceptPolicyPreferenceKey), CFSTR("2"));
    CFDictionaryAddValue(defaults, CFSTR(WebContinuousSpellCheckingEnabledPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(WebGrammarCheckingEnabledPreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(AllowContinuousSpellCheckingPreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitUsesPageCachePreferenceKey), kCFBooleanTrue);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitLocalStorageDatabasePathPreferenceKey), CFSTR(""));

    RetainPtr<CFStringRef> cacheModelRef = adoptCF(CFStringCreateWithFormat(0, 0, CFSTR("%d"), WebCacheModelDocumentViewer));
    CFDictionaryAddValue(defaults, CFSTR(CyberKitCacheModelPreferenceKey), cacheModelRef.get());

    CFDictionaryAddValue(defaults, CFSTR(CyberKitAuthorAndUserStylesEnabledPreferenceKey), kCFBooleanTrue);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitOfflineWebApplicationCacheEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitPaintNativeControlsPreferenceKey), kCFBooleanTrue);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitUseHighResolutionTimersPreferenceKey), kCFBooleanTrue);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitAcceleratedCompositingEnabledPreferenceKey), kCFBooleanTrue);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitShowDebugBordersPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitSpatialNavigationEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitDNSPrefetchingEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitHyperlinkAuditingEnabledPreferenceKey), kCFBooleanTrue);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitMediaPlaybackRequiresUserGesturePreferenceKey), kCFBooleanFalse);
    CFDictionaryAddValue(defaults, CFSTR(CyberKitMediaPlaybackAllowsInlinePreferenceKey), kCFBooleanTrue);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitRequestAnimationFrameEnabledPreferenceKey), kCFBooleanTrue);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitFullScreenEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitRequestAnimationFrameEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitAllowDisplayAndRunningOfInsecureContentPreferenceKey), kCFBooleanTrue);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitFetchAPIEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitShadowDOMEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitCustomElementsEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitWebAnimationsEnabledPreferenceKey), kCFBooleanTrue);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitWebAnimationsCompositeOperationsEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitWebAnimationsMutableTimelinesEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitWebAnimationsCSSIntegrationEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitCSSCustomPropertiesAndValuesEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitUserTimingEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitResourceTimingEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitLinkPreloadEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitMediaPreloadingEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitIsSecureContextAttributeEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitDataTransferItemsEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitInspectorAdditionsEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitVisualViewportAPIEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitCSSOMViewScrollingAPIEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitResizeObserverEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitCSSOMViewSmoothScrollingEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitCoreMathMLEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitRequestIdleCallbackEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitAsyncClipboardAPIEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitAspectRatioOfImgFromWidthAndHeightEnabledPreferenceKey), kCFBooleanFalse);

    CFDictionaryAddValue(defaults, CFSTR(CyberKitWebSQLEnabledPreferenceKey), kCFBooleanFalse);

    defaultSettings = defaults;
#endif
}

#if USE(CF)
RetainPtr<CFPropertyListRef> WebPreferences::valueForKey(CFStringRef key)
{
    RetainPtr<CFPropertyListRef> value = CFDictionaryGetValue(m_privatePrefs.get(), key);
    if (value)
        return value;

    value = adoptCF(CFPreferencesCopyAppValue(key, applicationId()));
    if (value)
        return value;

    return CFDictionaryGetValue(defaultSettings, key);
}

void WebPreferences::setValueForKey(CFStringRef key, CFPropertyListRef value)
{
    CFDictionarySetValue(m_privatePrefs.get(), key, value);
    if (m_autoSaves) {
        CFPreferencesSetAppValue(key, value, applicationId());
        save();
    }
}

void WebPreferences::setValueForKey(const char* key, CFPropertyListRef value)
{
    RetainPtr<CFStringRef> cfKey = adoptCF(CFStringCreateWithCString(0, key, kCFStringEncodingASCII));
    setValueForKey(cfKey.get(), value);
}

RetainPtr<CFPropertyListRef> WebPreferences::valueForKey(const char* key)
{
    RetainPtr<CFStringRef> cfKey = adoptCF(CFStringCreateWithCString(0, key, kCFStringEncodingASCII));
    return valueForKey(cfKey.get());
}
#endif

BSTR WebPreferences::stringValueForKey(const char* key)
{
#if USE(CF)
    RetainPtr<CFPropertyListRef> value = valueForKey(key);
    
    if (!value || (CFGetTypeID(value.get()) != CFStringGetTypeID()))
        return 0;

    CFStringRef str = static_cast<CFStringRef>(value.get());

    CFIndex length = CFStringGetLength(str);
    const UniChar* uniChars = CFStringGetCharactersPtr(str);
    if (uniChars)
        return SysAllocStringLen((LPCTSTR)uniChars, length);

    BSTR bstr = SysAllocStringLen(0, length);
    if (!bstr)
        return 0;

    if (!CFStringGetCString(str, (char*)bstr, (length+1)*sizeof(WCHAR), kCFStringEncodingUTF16)) {
        SysFreeString(bstr);
        return 0;
    }
        
    bstr[length] = 0;
    return bstr;
#else
    BString dummy;
    return dummy;
#endif
}

int WebPreferences::integerValueForKey(const char* key)
{
#if USE(CF)
    return numberValueForPreferencesValue<int>(valueForKey(key).get());
#else
    return 0;
#endif
}

BOOL WebPreferences::boolValueForKey(const char* key)
{
#if USE(CF)
    return booleanValueForPreferencesValue(valueForKey(key).get());
#else
    return 0;
#endif
}

float WebPreferences::floatValueForKey(const char* key)
{
#if USE(CF)
    return numberValueForPreferencesValue<float>(valueForKey(key).get());
#else
    return 0;
#endif
}

LONGLONG WebPreferences::longlongValueForKey(const char* key)
{
#if USE(CF)
    return numberValueForPreferencesValue<LONGLONG>(valueForKey(key).get());
#else
    return 0;
#endif
}

void WebPreferences::setStringValue(const char* key, BSTR value)
{
    BString val;
    val.adoptBSTR(stringValueForKey(key));
    if (val && !wcscmp(val, value))
        return;

#if USE(CF)
    RetainPtr<CFStringRef> valueRef = adoptCF(CFStringCreateWithCharacters(0, reinterpret_cast<const UniChar*>(value), static_cast<CFIndex>(wcslen(value))));
    setValueForKey(key, valueRef.get());
#endif

    postPreferencesChangesNotification();
}

void WebPreferences::setIntegerValue(const char* key, int value)
{
    if (integerValueForKey(key) == value)
        return;

#if USE(CF)
    setValueForKey(key, cfNumber(value).get());
#endif

    postPreferencesChangesNotification();
}

void WebPreferences::setFloatValue(const char* key, float value)
{
    if (floatValueForKey(key) == value)
        return;

#if USE(CF)
    setValueForKey(key, cfNumber(value).get());
#endif

    postPreferencesChangesNotification();
}

void WebPreferences::setBoolValue(const char* key, BOOL value)
{
    if (boolValueForKey(key) == value)
        return;

#if USE(CF)
    setValueForKey(key, value ? kCFBooleanTrue : kCFBooleanFalse);
#endif

    postPreferencesChangesNotification();
}

void WebPreferences::setLongLongValue(const char* key, LONGLONG value)
{
    if (longlongValueForKey(key) == value)
        return;

#if USE(CF)
    setValueForKey(key, cfNumber(value).get());
#endif

    postPreferencesChangesNotification();
}

BSTR WebPreferences::webPreferencesChangedNotification()
{
    static BSTR webPreferencesChangedNotification = SysAllocString(WebPreferencesChangedNotification);
    return webPreferencesChangedNotification;
}

BSTR WebPreferences::webPreferencesRemovedNotification()
{
    static BSTR webPreferencesRemovedNotification = SysAllocString(WebPreferencesRemovedNotification);
    return webPreferencesRemovedNotification;
}

void WebPreferences::save()
{
#if USE(CF)
    CFPreferencesAppSynchronize(applicationId());
#endif
}

void WebPreferences::load()
{
    initializeDefaultSettings();

#if USE(CF)
    m_privatePrefs = adoptCF(CFDictionaryCreateMutable(0, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks));

    migrateCyberKitPreferencesToCFPreferences();
#endif
}

#if USE(CF)
void WebPreferences::migrateCyberKitPreferencesToCFPreferences()
{
    if (boolValueForKey(CyberKitDidMigrateCyberKitPreferencesToCFPreferencesPreferenceKey))
        return;
    bool oldValue = m_autoSaves;
    m_autoSaves = true;
    setBoolValue(CyberKitDidMigrateCyberKitPreferencesToCFPreferencesPreferenceKey, TRUE);
    m_autoSaves = oldValue;

    WTF::CString path = oldPreferencesPath().utf8();

    RetainPtr<CFURLRef> urlRef = adoptCF(CFURLCreateFromFileSystemRepresentation(0, reinterpret_cast<const UInt8*>(path.data()), path.length(), false));
    if (!urlRef)
        return;

    RetainPtr<CFReadStreamRef> stream = adoptCF(CFReadStreamCreateWithFile(0, urlRef.get()));
    if (!stream)
        return;

    if (!CFReadStreamOpen(stream.get()))
        return;

    auto format = static_cast<CFPropertyListFormat>(kCFPropertyListBinaryFormat_v1_0 | kCFPropertyListXMLFormat_v1_0);
    RetainPtr<CFPropertyListRef> plist = adoptCF(CFPropertyListCreateFromStream(0, stream.get(), 0, kCFPropertyListMutableContainersAndLeaves, &format, 0));
    CFReadStreamClose(stream.get());

    if (!plist || CFGetTypeID(plist.get()) != CFDictionaryGetTypeID())
        return;

    copyCyberKitPreferencesToCFPreferences(static_cast<CFDictionaryRef>(plist.get()));

    FileSystem::deleteFile(oldPreferencesPath());
}

void WebPreferences::copyCyberKitPreferencesToCFPreferences(CFDictionaryRef dict)
{
    ASSERT_ARG(dict, dict);

    int count = CFDictionaryGetCount(dict);
    if (count <= 0)
        return;

    CFStringRef didRemoveDefaultsKey = CFSTR(CyberKitDidMigrateDefaultSettingsFromSafari3BetaPreferenceKey);
    bool omitDefaults = !booleanValueForPreferencesValue(CFDictionaryGetValue(dict, didRemoveDefaultsKey));

    Vector<CFTypeRef> keys(count);
    Vector<CFTypeRef> values(count);
    CFDictionaryGetKeysAndValues(dict, keys.data(), values.data());

    for (int i = 0; i < count; ++i) {
        if (!keys[i] || !values[i] || CFGetTypeID(keys[i]) != CFStringGetTypeID())
            continue;

        if (omitDefaults) {
            CFTypeRef defaultValue = CFDictionaryGetValue(defaultSettings, keys[i]);
            if (defaultValue && CFEqual(defaultValue, values[i]))
                continue;
        }

        setValueForKey(static_cast<CFStringRef>(keys[i]), values[i]);
    }
}
#endif

// IUnknown -------------------------------------------------------------------

HRESULT WebPreferences::QueryInterface(_In_ REFIID riid, _COM_Outptr_ void** ppvObject)
{
    if (!ppvObject)
        return E_POINTER;
    *ppvObject = nullptr;
    if (IsEqualGUID(riid, IID_IUnknown))
        *ppvObject = static_cast<IWebPreferences*>(this);
    else if (IsEqualGUID(riid, IID_IWebPreferences))
        *ppvObject = static_cast<IWebPreferences*>(this);
    else if (IsEqualGUID(riid, IID_IWebPreferencesPrivate))
        *ppvObject = static_cast<IWebPreferencesPrivate*>(this);
    else if (IsEqualGUID(riid, IID_IWebPreferencesPrivate2))
        *ppvObject = static_cast<IWebPreferencesPrivate2*>(this);
    else if (IsEqualGUID(riid, IID_IWebPreferencesPrivate3))
        *ppvObject = static_cast<IWebPreferencesPrivate3*>(this);
    else if (IsEqualGUID(riid, IID_IWebPreferencesPrivate4))
        *ppvObject = static_cast<IWebPreferencesPrivate4*>(this);
    else if (IsEqualGUID(riid, IID_IWebPreferencesPrivate5))
        *ppvObject = static_cast<IWebPreferencesPrivate5*>(this);
    else if (IsEqualGUID(riid, IID_IWebPreferencesPrivate6))
        *ppvObject = static_cast<IWebPreferencesPrivate6*>(this);
    else if (IsEqualGUID(riid, IID_IWebPreferencesPrivate7))
        *ppvObject = static_cast<IWebPreferencesPrivate7*>(this);
    else if (IsEqualGUID(riid, IID_IWebPreferencesPrivate8))
        *ppvObject = static_cast<IWebPreferencesPrivate8*>(this);
    else if (IsEqualGUID(riid, CLSID_WebPreferences))
        *ppvObject = this;
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

ULONG WebPreferences::AddRef()
{
    return ++m_refCount;
}

ULONG WebPreferences::Release()
{
    ULONG newRef = --m_refCount;
    if (!newRef)
        delete(this);

    return newRef;
}

// IWebPreferences ------------------------------------------------------------

HRESULT WebPreferences::standardPreferences(_COM_Outptr_opt_ IWebPreferences** standardPreferences)
{
    if (!standardPreferences)
        return E_POINTER;
    *standardPreferences = sharedStandardPreferences();
    (*standardPreferences)->AddRef();
    return S_OK;
}

HRESULT WebPreferences::initWithIdentifier(_In_ BSTR anIdentifier, _COM_Outptr_opt_ IWebPreferences** preferences)
{
    WebPreferences *instance = getInstanceForIdentifier(anIdentifier);
    if (instance) {
        *preferences = instance;
        instance->AddRef();
        return S_OK;
    }

    load();

    *preferences = this;
    AddRef();

    if (anIdentifier) {
        m_identifier = anIdentifier;
        setInstance(this, m_identifier);
    }

    this->postPreferencesChangesNotification();

    return S_OK;
}

HRESULT WebPreferences::identifier(__deref_opt_out BSTR* ident)
{
    if (!ident)
        return E_POINTER;
    *ident = m_identifier ? SysAllocString(m_identifier) : nullptr;
    return S_OK;
}

HRESULT WebPreferences::standardFontFamily(__deref_opt_out BSTR* family)
{
    if (!family)
        return E_POINTER;
    *family = stringValueForKey(CyberKitStandardFontPreferenceKey);
    return (*family) ? S_OK : E_FAIL;
}

HRESULT WebPreferences::setStandardFontFamily(_In_ BSTR family)
{
    setStringValue(CyberKitStandardFontPreferenceKey, family);
    return S_OK;
}

HRESULT WebPreferences::fixedFontFamily(__deref_opt_out BSTR* family)
{
    if (!family)
        return E_POINTER;
    *family = stringValueForKey(CyberKitFixedFontPreferenceKey);
    return (*family) ? S_OK : E_FAIL;
}

HRESULT WebPreferences::setFixedFontFamily(_In_ BSTR family)
{
    setStringValue(CyberKitFixedFontPreferenceKey, family);
    return S_OK;
}

HRESULT WebPreferences::serifFontFamily(__deref_opt_out BSTR* fontFamily)
{
    if (!fontFamily)
        return E_POINTER;
    *fontFamily = stringValueForKey(CyberKitSerifFontPreferenceKey);
    return (*fontFamily) ? S_OK : E_FAIL;
}

HRESULT WebPreferences::setSerifFontFamily(_In_ BSTR family)
{
    setStringValue(CyberKitSerifFontPreferenceKey, family);
    return S_OK;
}

HRESULT WebPreferences::sansSerifFontFamily(__deref_opt_out BSTR* family)
{
    if (!family)
        return E_POINTER;
    *family = stringValueForKey(CyberKitSansSerifFontPreferenceKey);
    return (*family) ? S_OK : E_FAIL;
}

HRESULT WebPreferences::setSansSerifFontFamily(_In_ BSTR family)
{
    setStringValue(CyberKitSansSerifFontPreferenceKey, family);
    return S_OK;
}

HRESULT WebPreferences::cursiveFontFamily(__deref_opt_out BSTR* family)
{
    if (!family)
        return E_POINTER;
    *family = stringValueForKey(CyberKitCursiveFontPreferenceKey);
    return (*family) ? S_OK : E_FAIL;
}

HRESULT WebPreferences::setCursiveFontFamily(_In_ BSTR family)
{
    setStringValue(CyberKitCursiveFontPreferenceKey, family);
    return S_OK;
}

HRESULT WebPreferences::fantasyFontFamily(__deref_opt_out BSTR* family)
{
    if (!family)
        return E_POINTER;
    *family = stringValueForKey(CyberKitFantasyFontPreferenceKey);
    return (*family) ? S_OK : E_FAIL;
}

HRESULT WebPreferences::setFantasyFontFamily(_In_ BSTR family)
{
    setStringValue(CyberKitFantasyFontPreferenceKey, family);
    return S_OK;
}

HRESULT WebPreferences::pictographFontFamily(__deref_opt_out BSTR* family)
{
    if (!family)
        return E_POINTER;
    *family = stringValueForKey(CyberKitPictographFontPreferenceKey);
    return (*family) ? S_OK : E_FAIL;
}

HRESULT WebPreferences::setPictographFontFamily(_In_ BSTR family)
{
    setStringValue(CyberKitPictographFontPreferenceKey, family);
    return S_OK;
}

HRESULT WebPreferences::defaultFontSize(_Out_  int* fontSize)
{
    if (!fontSize)
        return E_POINTER;
    *fontSize = integerValueForKey(CyberKitDefaultFontSizePreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setDefaultFontSize(int fontSize)
{
    setIntegerValue(CyberKitDefaultFontSizePreferenceKey, fontSize);
    return S_OK;
}

HRESULT WebPreferences::defaultFixedFontSize(_Out_ int* fontSize)
{
    if (!fontSize)
        return E_POINTER;
    *fontSize = integerValueForKey(CyberKitDefaultFixedFontSizePreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setDefaultFixedFontSize(int fontSize)
{
    setIntegerValue(CyberKitDefaultFixedFontSizePreferenceKey, fontSize);
    return S_OK;
}

HRESULT WebPreferences::minimumFontSize(_Out_ int* fontSize)
{
    if (!fontSize)
        return E_POINTER;
    *fontSize = integerValueForKey(CyberKitMinimumFontSizePreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setMinimumFontSize(int fontSize)
{
    setIntegerValue(CyberKitMinimumFontSizePreferenceKey, fontSize);
    return S_OK;
}

HRESULT WebPreferences::minimumLogicalFontSize(_Out_ int* fontSize)
{
    if (!fontSize)
        return E_POINTER;
    *fontSize = integerValueForKey(CyberKitMinimumLogicalFontSizePreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setMinimumLogicalFontSize(int fontSize)
{
    setIntegerValue(CyberKitMinimumLogicalFontSizePreferenceKey, fontSize);
    return S_OK;
}

HRESULT WebPreferences::defaultTextEncodingName(__deref_opt_out BSTR* name)
{
    if (!name)
        return E_POINTER;
    *name = stringValueForKey(CyberKitDefaultTextEncodingNamePreferenceKey);
    return (*name) ? S_OK : E_FAIL;
}

HRESULT WebPreferences::setDefaultTextEncodingName(_In_ BSTR name)
{
    setStringValue(CyberKitDefaultTextEncodingNamePreferenceKey, name);
    return S_OK;
}

HRESULT WebPreferences::userStyleSheetEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitUserStyleSheetEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setUserStyleSheetEnabled(BOOL enabled)
{
    setBoolValue(CyberKitUserStyleSheetEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::userStyleSheetLocation(__deref_opt_out BSTR* location)
{
    if (!location)
        return E_POINTER;
    *location = stringValueForKey(CyberKitUserStyleSheetLocationPreferenceKey);
    return (*location) ? S_OK : E_FAIL;
}

HRESULT WebPreferences::setUserStyleSheetLocation(_In_ BSTR location)
{
    setStringValue(CyberKitUserStyleSheetLocationPreferenceKey, location);
    return S_OK;
}

HRESULT WebPreferences::isJavaEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitJavaEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setJavaEnabled(BOOL enabled)
{
    setBoolValue(CyberKitJavaEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::isJavaScriptEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitJavaScriptEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setJavaScriptEnabled(BOOL enabled)
{
    setBoolValue(CyberKitJavaScriptEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::javaScriptRuntimeFlags(_Out_ unsigned* flags)
{
    if (!flags)
        return E_POINTER;
    *flags = static_cast<unsigned>(integerValueForKey(CyberKitJavaScriptRuntimeFlagsPreferenceKey));
    return S_OK;
}

HRESULT WebPreferences::setJavaScriptRuntimeFlags(unsigned flags)
{
    setIntegerValue(CyberKitJavaScriptRuntimeFlagsPreferenceKey, static_cast<int>(flags));
    return S_OK;
}

HRESULT WebPreferences::isWebSecurityEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitWebSecurityEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setWebSecurityEnabled(BOOL enabled)
{
    setBoolValue(CyberKitWebSecurityEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::allowUniversalAccessFromFileURLs(_Out_ BOOL* allowAccess)
{
    if (!allowAccess)
        return E_POINTER;
    *allowAccess = boolValueForKey(CyberKitAllowUniversalAccessFromFileURLsPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setAllowUniversalAccessFromFileURLs(BOOL allowAccess)
{
    setBoolValue(CyberKitAllowUniversalAccessFromFileURLsPreferenceKey, allowAccess);
    return S_OK;
}

HRESULT WebPreferences::allowFileAccessFromFileURLs(_Out_ BOOL* allowAccess)
{
    if (!allowAccess)
        return E_POINTER;
    *allowAccess = boolValueForKey(CyberKitAllowFileAccessFromFileURLsPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setAllowFileAccessFromFileURLs(BOOL allowAccess)
{
    setBoolValue(CyberKitAllowFileAccessFromFileURLsPreferenceKey, allowAccess);
    return S_OK;
}

HRESULT WebPreferences::javaScriptCanAccessClipboard(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitJavaScriptCanAccessClipboardPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setJavaScriptCanAccessClipboard(BOOL enabled)
{
    setBoolValue(CyberKitJavaScriptCanAccessClipboardPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::isXSSAuditorEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitXSSAuditorEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setXSSAuditorEnabled(BOOL enabled)
{
    setBoolValue(CyberKitXSSAuditorEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::isFrameFlatteningEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitFrameFlatteningEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setFrameFlatteningEnabled(BOOL enabled)
{
    setBoolValue(CyberKitFrameFlatteningEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::javaScriptCanOpenWindowsAutomatically(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitJavaScriptCanOpenWindowsAutomaticallyPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setJavaScriptCanOpenWindowsAutomatically(BOOL enabled)
{
    setBoolValue(CyberKitJavaScriptCanOpenWindowsAutomaticallyPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::arePlugInsEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitPluginsEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setPlugInsEnabled(BOOL enabled)
{
    setBoolValue(CyberKitPluginsEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::isCSSRegionsEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitCSSRegionsEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setCSSRegionsEnabled(BOOL enabled)
{
    setBoolValue(CyberKitCSSRegionsEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::allowsAnimatedImages(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitAllowAnimatedImagesPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setAllowsAnimatedImages(BOOL enabled)
{
    setBoolValue(CyberKitAllowAnimatedImagesPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::allowAnimatedImageLooping(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitAllowAnimatedImageLoopingPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setAllowAnimatedImageLooping(BOOL enabled)
{
    setBoolValue(CyberKitAllowAnimatedImageLoopingPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::setLoadsImagesAutomatically(BOOL enabled)
{
    setBoolValue(CyberKitDisplayImagesKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::loadsImagesAutomatically(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitDisplayImagesKey);
    return S_OK;
}

HRESULT WebPreferences::setLoadsSiteIconsIgnoringImageLoadingPreference(BOOL enabled)
{
    setBoolValue(CyberKitLoadSiteIconsKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::loadsSiteIconsIgnoringImageLoadingPreference(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitLoadSiteIconsKey);
    return S_OK;
}

HRESULT WebPreferences::setHixie76WebSocketProtocolEnabled(BOOL enabled)
{
    return S_OK;
}

HRESULT WebPreferences::hixie76WebSocketProtocolEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = false;
    return S_OK;
}

HRESULT WebPreferences::setMediaPlaybackRequiresUserGesture(BOOL enabled)
{
    setBoolValue(CyberKitMediaPlaybackRequiresUserGesturePreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::mediaPlaybackRequiresUserGesture(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitMediaPlaybackRequiresUserGesturePreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setMediaPlaybackAllowsInline(BOOL enabled)
{
    setBoolValue(CyberKitMediaPlaybackAllowsInlinePreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::mediaPlaybackAllowsInline(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitMediaPlaybackAllowsInlinePreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setAutosaves(BOOL enabled)
{
    m_autoSaves = !!enabled;
    return S_OK;
}

HRESULT WebPreferences::autosaves(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = m_autoSaves ? TRUE : FALSE;
    return S_OK;
}

HRESULT WebPreferences::setShouldPrintBackgrounds(BOOL enabled)
{
    setBoolValue(CyberKitShouldPrintBackgroundsPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::shouldPrintBackgrounds(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitShouldPrintBackgroundsPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setPrivateBrowsingEnabled(BOOL enabled)
{
    setBoolValue(CyberKitPrivateBrowsingEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::privateBrowsingEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitPrivateBrowsingEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setTabsToLinks(BOOL enabled)
{
    setBoolValue(CyberKitTabToLinksPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::tabsToLinks(_Out_ BOOL* enabled)
{
    *enabled = boolValueForKey(CyberKitTabToLinksPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setUsesPageCache(BOOL usesPageCache)
{
    setBoolValue(CyberKitUsesPageCachePreferenceKey, usesPageCache);
    return S_OK;
}

HRESULT WebPreferences::usesPageCache(_Out_ BOOL* usesPageCache)
{
    if (!usesPageCache)
        return E_POINTER;
    *usesPageCache = boolValueForKey(CyberKitUsesPageCachePreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::textAreasAreResizable(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitTextAreasAreResizablePreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setTextAreasAreResizable(BOOL enabled)
{
    setBoolValue(CyberKitTextAreasAreResizablePreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::historyItemLimit(_Out_ int* limit)
{
    if (!limit)
        return E_POINTER;
    *limit = integerValueForKey(CyberKitHistoryItemLimitKey);
    return S_OK;
}

HRESULT WebPreferences::setHistoryItemLimit(int limit)
{
    setIntegerValue(CyberKitHistoryItemLimitKey, limit);
    return S_OK;
}

HRESULT WebPreferences::historyAgeInDaysLimit(_Out_ int* limit)
{
    if (!limit)
        return E_POINTER;
    *limit = integerValueForKey(CyberKitHistoryAgeInDaysLimitKey);
    return S_OK;
}

HRESULT WebPreferences::setHistoryAgeInDaysLimit(int limit)
{
    setIntegerValue(CyberKitHistoryAgeInDaysLimitKey, limit);
    return S_OK;
}

HRESULT WebPreferences::unused1()
{
    ASSERT_NOT_REACHED();
    return E_FAIL;
}

HRESULT WebPreferences::unused2()
{
    ASSERT_NOT_REACHED();
    return E_FAIL;
}

HRESULT WebPreferences::iconDatabaseLocation(__deref_opt_out BSTR* location)
{
    if (!location)
        return E_POINTER;
    *location = stringValueForKey(CyberKitIconDatabaseLocationKey);
    return (*location) ? S_OK : E_FAIL;
}

HRESULT WebPreferences::setIconDatabaseLocation(_In_ BSTR location)
{
    setStringValue(CyberKitIconDatabaseLocationKey, location);
    return S_OK;
}

HRESULT WebPreferences::iconDatabaseEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitIconDatabaseEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setIconDatabaseEnabled(BOOL enabled)
{
    setBoolValue(CyberKitIconDatabaseEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::fontSmoothing(_Out_ FontSmoothingType* smoothingType)
{
    if (!smoothingType)
        return E_POINTER;
    *smoothingType = static_cast<FontSmoothingType>(integerValueForKey(CyberKitFontSmoothingTypePreferenceKey));
    return S_OK;
}

HRESULT WebPreferences::setFontSmoothing(FontSmoothingType smoothingType)
{
    setIntegerValue(CyberKitFontSmoothingTypePreferenceKey, smoothingType);
    if (smoothingType == FontSmoothingTypeWindows)
        smoothingType = FontSmoothingTypeMedium;
#if USE(CG)
    FontCascade::setFontSmoothingLevel((int)smoothingType);
#endif
    return S_OK;
}

HRESULT WebPreferences::fontSmoothingContrast(_Out_ float* contrast)
{
    if (!contrast)
        return E_POINTER;
    *contrast = floatValueForKey(CyberKitFontSmoothingContrastPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setFontSmoothingContrast(float contrast)
{
    setFloatValue(CyberKitFontSmoothingContrastPreferenceKey, contrast);
#if USE(CG)
    FontCascade::setFontSmoothingContrast(contrast);
#endif
    return S_OK;
}

HRESULT WebPreferences::editableLinkBehavior(_Out_ CyberKitEditableLinkBehavior* editableLinkBehavior)
{
    if (!editableLinkBehavior)
        return E_POINTER;
    CyberKitEditableLinkBehavior value = static_cast<CyberKitEditableLinkBehavior>(integerValueForKey(CyberKitEditableLinkBehaviorPreferenceKey));
    switch (value) {
        case CyberKitEditableLinkDefaultBehavior:
        case CyberKitEditableLinkAlwaysLive:
        case CyberKitEditableLinkOnlyLiveWithShiftKey:
        case CyberKitEditableLinkLiveWhenNotFocused:
        case CyberKitEditableLinkNeverLive:
            *editableLinkBehavior = value;
            break;
        default: // ensure that a valid result is returned
            *editableLinkBehavior = CyberKitEditableLinkDefaultBehavior;
            break;
    }
    return S_OK;
}

HRESULT WebPreferences::setEditableLinkBehavior(CyberKitEditableLinkBehavior behavior)
{
    setIntegerValue(CyberKitEditableLinkBehaviorPreferenceKey, behavior);
    return S_OK;
}

HRESULT WebPreferences::unused5()
{
    ASSERT_NOT_REACHED();
    return E_FAIL;
}

HRESULT WebPreferences::unused6()
{
    ASSERT_NOT_REACHED();
    return E_FAIL;
}

HRESULT WebPreferences::mockScrollbarsEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitMockScrollbarsEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setMockScrollbarsEnabled(BOOL enabled)
{
    setBoolValue(CyberKitMockScrollbarsEnabledPreferenceKey, enabled);
    return S_OK;
}

// These two methods are no-ops, and only retained to keep
// the Interface consistent. DO NOT USE THEM.
HRESULT WebPreferences::screenFontSubstitutionEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = false;
    return S_OK;
}

HRESULT WebPreferences::setScreenFontSubstitutionEnabled(BOOL)
{
    return S_OK;
}

HRESULT WebPreferences::hyperlinkAuditingEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitHyperlinkAuditingEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setHyperlinkAuditingEnabled(BOOL enabled)
{
    setBoolValue(CyberKitHyperlinkAuditingEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::cookieStorageAcceptPolicy(_Out_ CyberKitCookieStorageAcceptPolicy* acceptPolicy)
{
    if (!acceptPolicy)
        return E_POINTER;

    *acceptPolicy = static_cast<CyberKitCookieStorageAcceptPolicy>(integerValueForKey(CyberKitCookieStorageAcceptPolicyPreferenceKey));
    return S_OK;
}

HRESULT WebPreferences::setCookieStorageAcceptPolicy(CyberKitCookieStorageAcceptPolicy acceptPolicy)
{
    setIntegerValue(CyberKitCookieStorageAcceptPolicyPreferenceKey, acceptPolicy);
    return S_OK;
}


HRESULT WebPreferences::continuousSpellCheckingEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(WebContinuousSpellCheckingEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setContinuousSpellCheckingEnabled(BOOL enabled)
{
    setBoolValue(WebContinuousSpellCheckingEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::grammarCheckingEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(WebGrammarCheckingEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setGrammarCheckingEnabled(BOOL enabled)
{
    setBoolValue(WebGrammarCheckingEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::allowContinuousSpellChecking(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(AllowContinuousSpellCheckingPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setAllowContinuousSpellChecking(BOOL enabled)
{
    setBoolValue(AllowContinuousSpellCheckingPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::unused7()
{
    ASSERT_NOT_REACHED();
    return E_FAIL;
}

HRESULT WebPreferences::unused8()
{
    ASSERT_NOT_REACHED();
    return E_FAIL;
}

HRESULT WebPreferences::isDOMPasteAllowed(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitDOMPasteAllowedPreferenceKey);
    return S_OK;
}
    
HRESULT WebPreferences::setDOMPasteAllowed(BOOL enabled)
{
    setBoolValue(CyberKitDOMPasteAllowedPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::cacheModel(_Out_ WebCacheModel* cacheModel)
{
    if (!cacheModel)
        return E_POINTER;

    *cacheModel = static_cast<WebCacheModel>(integerValueForKey(CyberKitCacheModelPreferenceKey));
    return S_OK;
}

HRESULT WebPreferences::setCacheModel(WebCacheModel cacheModel)
{
    setIntegerValue(CyberKitCacheModelPreferenceKey, cacheModel);
    return S_OK;
}

HRESULT WebPreferences::unused3()
{
    ASSERT_NOT_REACHED();
    return E_FAIL;
}

HRESULT WebPreferences::unused4()
{
    ASSERT_NOT_REACHED();
    return E_FAIL;
}

// These two methods are no-ops, and only retained to keep
// the Interface consistent. DO NOT USE THEM.
HRESULT WebPreferences::shouldPaintNativeControls(_Out_ BOOL* enable)
{
    if (!enable)
        return E_POINTER;
    *enable = FALSE;
    return S_OK;
}

HRESULT WebPreferences::setShouldPaintNativeControls(BOOL)
{
    return S_OK;
}

HRESULT WebPreferences::setDeveloperExtrasEnabled(BOOL enabled)
{
    setBoolValue(CyberKitDeveloperExtrasEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::developerExtrasEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;

    *enabled = boolValueForKey(CyberKitDeveloperExtrasEnabledPreferenceKey);
    return S_OK;
}

bool WebPreferences::developerExtrasDisabledByOverride()
{
    return !!boolValueForKey(DisableCyberKitDeveloperExtrasPreferenceKey);
}

HRESULT WebPreferences::setAutomaticallyDetectsCacheModel(BOOL automaticallyDetectsCacheModel)
{
    m_automaticallyDetectsCacheModel = !!automaticallyDetectsCacheModel;
    return S_OK;
}

HRESULT WebPreferences::automaticallyDetectsCacheModel(_Out_ BOOL* automaticallyDetectsCacheModel)
{
    if (!automaticallyDetectsCacheModel)
        return E_POINTER;

    *automaticallyDetectsCacheModel = m_automaticallyDetectsCacheModel;
    return S_OK;
}

HRESULT WebPreferences::setAuthorAndUserStylesEnabled(BOOL enabled)
{
    setBoolValue(CyberKitAuthorAndUserStylesEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::authorAndUserStylesEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;

    *enabled = boolValueForKey(CyberKitAuthorAndUserStylesEnabledPreferenceKey);
    return S_OK;
}

// These two methods are no-ops, and only retained to keep
// the Interface consistent. DO NOT USE THEM.
HRESULT WebPreferences::inApplicationChromeMode(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = FALSE;
    return S_OK;
}

HRESULT WebPreferences::setApplicationChromeMode(BOOL)
{
    return S_OK;
}

HRESULT WebPreferences::setOfflineWebApplicationCacheEnabled(BOOL enabled)
{
    setBoolValue(CyberKitOfflineWebApplicationCacheEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::offlineWebApplicationCacheEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitOfflineWebApplicationCacheEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setDatabasesEnabled(BOOL enabled)
{
    setBoolValue(CyberKitDatabasesEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::databasesEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitDatabasesEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setLocalStorageEnabled(BOOL enabled)
{
    setBoolValue(CyberKitLocalStorageEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::localStorageEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitLocalStorageEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::localStorageDatabasePath(__deref_opt_out BSTR* location)
{
    if (!location)
        return E_POINTER;
    *location = stringValueForKey(CyberKitLocalStorageDatabasePathPreferenceKey);
    return (*location) ? S_OK : E_FAIL;
}

HRESULT WebPreferences::setLocalStorageDatabasePath(_In_ BSTR location)
{
    setStringValue(CyberKitLocalStorageDatabasePathPreferenceKey, location);
    return S_OK;
}

HRESULT WebPreferences::setExperimentalNotificationsEnabled(BOOL enabled)
{
    setBoolValue(CyberKitExperimentalNotificationsEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::experimentalNotificationsEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitExperimentalNotificationsEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setZoomsTextOnly(BOOL zoomsTextOnly)
{
    setBoolValue(CyberKitZoomsTextOnlyPreferenceKey, zoomsTextOnly);
    return S_OK;
}

HRESULT WebPreferences::zoomsTextOnly(_Out_ BOOL* zoomsTextOnly)
{
    if (!zoomsTextOnly)
        return E_POINTER;
    *zoomsTextOnly = boolValueForKey(CyberKitZoomsTextOnlyPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setShouldUseHighResolutionTimers(BOOL useHighResolutionTimers)
{
    setBoolValue(CyberKitUseHighResolutionTimersPreferenceKey, useHighResolutionTimers);
    return S_OK;
}

HRESULT WebPreferences::shouldUseHighResolutionTimers(_Out_ BOOL* useHighResolutionTimers)
{
    if (!useHighResolutionTimers)
        return E_POINTER;
    *useHighResolutionTimers = boolValueForKey(CyberKitUseHighResolutionTimersPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setPreferenceForTest(_In_ BSTR key, _In_ BSTR value)
{
    if (!SysStringLen(key) || !SysStringLen(value))
        return E_FAIL;
#if USE(CF)
    RetainPtr<CFStringRef> keyString = adoptCF(CFStringCreateWithCharacters(0, reinterpret_cast<UniChar*>(key), SysStringLen(key)));
    RetainPtr<CFStringRef> valueString = adoptCF(CFStringCreateWithCharacters(0, reinterpret_cast<UniChar*>(value), SysStringLen(value)));
    setValueForKey(keyString.get(), valueString.get());
#endif
    postPreferencesChangesNotification();
    return S_OK;
}

HRESULT WebPreferences::setAcceleratedCompositingEnabled(BOOL enabled)
{
    setBoolValue(CyberKitAcceleratedCompositingEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::acceleratedCompositingEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitAcceleratedCompositingEnabledPreferenceKey);
#if USE(CA)
    *enabled = *enabled && CACFLayerTreeHost::acceleratedCompositingAvailable();
#endif
    return S_OK;
}

HRESULT WebPreferences::showDebugBorders(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitShowDebugBordersPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setShowDebugBorders(BOOL enabled)
{
    setBoolValue(CyberKitShowDebugBordersPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::showRepaintCounter(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitShowRepaintCounterPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setShowRepaintCounter(BOOL enabled)
{
    setBoolValue(CyberKitShowRepaintCounterPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::setCustomDragCursorsEnabled(BOOL enabled)
{
    setBoolValue(CyberKitCustomDragCursorsEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::customDragCursorsEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitCustomDragCursorsEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::spatialNavigationEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitSpatialNavigationEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setSpatialNavigationEnabled(BOOL enabled)
{
    setBoolValue(CyberKitSpatialNavigationEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::setDNSPrefetchingEnabled(BOOL enabled)
{
    setBoolValue(CyberKitDNSPrefetchingEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::isDNSPrefetchingEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitDNSPrefetchingEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::isFullScreenEnabled(_Out_ BOOL* enabled)
{
#if ENABLE(FULLSCREEN_API)
    if (!enabled)
        return E_POINTER;

    *enabled = boolValueForKey(CyberKitFullScreenEnabledPreferenceKey);
    return S_OK;
#else
    return E_NOTIMPL;
#endif
}

HRESULT WebPreferences::setFullScreenEnabled(BOOL enabled)
{
#if ENABLE(FULLSCREEN_API)
    setBoolValue(CyberKitFullScreenEnabledPreferenceKey, enabled);
    return S_OK;
#else
    return E_NOTIMPL;
#endif
}

HRESULT WebPreferences::avFoundationEnabled(_Out_ BOOL* enabled)
{
#if USE(AVFOUNDATION)
    if (!enabled)
        return E_POINTER;

    *enabled = boolValueForKey(CyberKitAVFoundationEnabledPreferenceKey);
    return S_OK;
#else
    return E_NOTIMPL;
#endif
}

HRESULT WebPreferences::setAVFoundationEnabled(BOOL enabled)
{
#if USE(AVFOUNDATION)
    setBoolValue(CyberKitAVFoundationEnabledPreferenceKey, enabled);
    return S_OK;
#else
    return E_NOTIMPL;
#endif
}

HRESULT WebPreferences::showsToolTipOverTruncatedText(_Out_ BOOL* showsToolTip)
{
    if (!showsToolTip)
        return E_POINTER;

    *showsToolTip = boolValueForKey(CyberKitShowsToolTipOverTruncatedTextPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setShowsToolTipOverTruncatedText(BOOL showsToolTip)
{
    setBoolValue(CyberKitShowsToolTipOverTruncatedTextPreferenceKey, showsToolTip);
    return S_OK;
}

HRESULT WebPreferences::shouldInvertColors(_Out_ BOOL* shouldInvertColors)
{
    if (!shouldInvertColors)
        return E_POINTER;

    *shouldInvertColors = boolValueForKey(CyberKitShouldInvertColorsPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setShouldInvertColors(BOOL shouldInvertColors)
{
    setBoolValue(CyberKitShouldInvertColorsPreferenceKey, shouldInvertColors);
    return S_OK;
}

void WebPreferences::willAddToWebView()
{
    ++m_numWebViews;
}

void WebPreferences::didRemoveFromWebView()
{
    ASSERT(m_numWebViews);
    if (--m_numWebViews == 0) {
        IWebNotificationCenter* nc = WebNotificationCenter::defaultCenterInternal();
        nc->postNotificationName(webPreferencesRemovedNotification(), static_cast<IWebPreferences*>(this), 0);
    }
}

HRESULT WebPreferences::shouldDisplaySubtitles(_Out_ BOOL* enabled)
{
#if ENABLE(VIDEO)
    if (!enabled)
        return E_POINTER;

    *enabled = boolValueForKey(CyberKitShouldDisplaySubtitlesPreferenceKey);
    return S_OK;
#else
    return E_NOTIMPL;
#endif
}

HRESULT WebPreferences::setShouldDisplaySubtitles(BOOL enabled)
{
#if ENABLE(VIDEO)
    setBoolValue(CyberKitShouldDisplaySubtitlesPreferenceKey, enabled);
    return S_OK;
#else
    return E_NOTIMPL;
#endif
}

HRESULT WebPreferences::shouldDisplayCaptions(_Out_ BOOL* enabled)
{
#if ENABLE(VIDEO)
    if (!enabled)
        return E_POINTER;

    *enabled = boolValueForKey(CyberKitShouldDisplayCaptionsPreferenceKey);
    return S_OK;
#else
    return E_NOTIMPL;
#endif
}

HRESULT WebPreferences::setShouldDisplayCaptions(BOOL enabled)
{
#if ENABLE(VIDEO)
    setBoolValue(CyberKitShouldDisplayCaptionsPreferenceKey, enabled);
    return S_OK;
#else
    return E_NOTIMPL;
#endif
}

HRESULT WebPreferences::shouldDisplayTextDescriptions(_Out_ BOOL* enabled)
{
#if ENABLE(VIDEO)
    if (!enabled)
        return E_POINTER;

    *enabled = boolValueForKey(CyberKitShouldDisplayTextDescriptionsPreferenceKey);
    return S_OK;
#else
    return E_NOTIMPL;
#endif
}

HRESULT WebPreferences::setShouldDisplayTextDescriptions(BOOL enabled)
{
#if ENABLE(VIDEO)
    setBoolValue(CyberKitShouldDisplayTextDescriptionsPreferenceKey, enabled);
    return S_OK;
#else
    return E_NOTIMPL;
#endif
}

HRESULT WebPreferences::setRequestAnimationFrameEnabled(BOOL enabled)
{
    setBoolValue(CyberKitRequestAnimationFrameEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::requestAnimationFrameEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitRequestAnimationFrameEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::isInheritURIQueryComponentEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitEnableInheritURIQueryComponentPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setEnableInheritURIQueryComponent(BOOL enabled)
{
    setBoolValue(CyberKitEnableInheritURIQueryComponentPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::allowDisplayAndRunningOfInsecureContent(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitAllowDisplayAndRunningOfInsecureContentPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setAllowDisplayAndRunningOfInsecureContent(BOOL enabled)
{
    setBoolValue(CyberKitAllowDisplayAndRunningOfInsecureContentPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::showTiledScrollingIndicator(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitShowTiledScrollingIndicatorPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setShowTiledScrollingIndicator(BOOL enabled)
{
    setBoolValue(CyberKitShowTiledScrollingIndicatorPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::fetchAPIEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitFetchAPIEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setFetchAPIEnabled(BOOL enabled)
{
    setBoolValue(CyberKitFetchAPIEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::shadowDOMEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitShadowDOMEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setShadowDOMEnabled(BOOL enabled)
{
    setBoolValue(CyberKitShadowDOMEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::customElementsEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitCustomElementsEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setCustomElementsEnabled(BOOL enabled)
{
    setBoolValue(CyberKitCustomElementsEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::menuItemElementEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitMenuItemElementEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setMenuItemElementEnabled(BOOL enabled)
{
    setBoolValue(CyberKitMenuItemElementEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::keygenElementEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitKeygenElementEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setKeygenElementEnabled(BOOL enabled)
{
    setBoolValue(CyberKitKeygenElementEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::crossOriginWindowPolicySupportEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = false;
    return S_OK;
}

HRESULT WebPreferences::setCrossOriginWindowPolicySupportEnabled(BOOL)
{
    return S_OK;
}

HRESULT WebPreferences::setModernMediaControlsEnabled(BOOL enabled)
{
    setBoolValue(CyberKitModernMediaControlsEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::modernMediaControlsEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitModernMediaControlsEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::webAnimationsCSSIntegrationEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitWebAnimationsCSSIntegrationEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setWebAnimationsCSSIntegrationEnabled(BOOL enabled)
{
    setBoolValue(CyberKitWebAnimationsCSSIntegrationEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::fetchAPIKeepAliveEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitFetchAPIKeepAliveEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setFetchAPIKeepAliveEnabled(BOOL enabled)
{
    setBoolValue(CyberKitFetchAPIKeepAliveEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::setLinkPreloadEnabled(BOOL enabled)
{
    setBoolValue(CyberKitLinkPreloadEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::linkPreloadEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitLinkPreloadEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setMediaPreloadingEnabled(BOOL enabled)
{
    setBoolValue(CyberKitMediaPreloadingEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::mediaPreloadingEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitMediaPreloadingEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::clearNetworkLoaderSession()
{
    NetworkStorageSessionMap::defaultStorageSession().deleteAllCookies();
    return S_OK;
}

HRESULT WebPreferences::switchNetworkLoaderToNewTestingSession()
{
    NetworkStorageSessionMap::switchToNewTestingSession();
    return S_OK;
}

HRESULT WebPreferences::setIsSecureContextAttributeEnabled(BOOL enabled)
{
    setBoolValue(CyberKitIsSecureContextAttributeEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::isSecureContextAttributeEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitIsSecureContextAttributeEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::dataTransferItemsEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitDataTransferItemsEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setDataTransferItemsEnabled(BOOL enabled)
{
    setBoolValue(CyberKitDataTransferItemsEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::inspectorAdditionsEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitInspectorAdditionsEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setInspectorAdditionsEnabled(BOOL enabled)
{
    setBoolValue(CyberKitInspectorAdditionsEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::visualViewportAPIEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitVisualViewportAPIEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setVisualViewportAPIEnabled(BOOL enabled)
{
    setBoolValue(CyberKitVisualViewportAPIEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::CSSOMViewScrollingAPIEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitCSSOMViewScrollingAPIEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setCSSOMViewScrollingAPIEnabled(BOOL enabled)
{
    setBoolValue(CyberKitCSSOMViewScrollingAPIEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::coreMathMLEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitCoreMathMLEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setCoreMathMLEnabled(BOOL enabled)
{
    setBoolValue(CyberKitCoreMathMLEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::CSSOMViewSmoothScrollingEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitCSSOMViewSmoothScrollingEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setCSSOMViewSmoothScrollingEnabled(BOOL enabled)
{
    setBoolValue(CyberKitCSSOMViewSmoothScrollingEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::requestIdleCallbackEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitRequestIdleCallbackEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setRequestIdleCallbackEnabled(BOOL enabled)
{
    setBoolValue(CyberKitRequestIdleCallbackEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::asyncClipboardAPIEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitAsyncClipboardAPIEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setAsyncClipboardAPIEnabled(BOOL enabled)
{
    setBoolValue(CyberKitAsyncClipboardAPIEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::setApplicationId(BSTR applicationId)
{
#if USE(CF)
    m_applicationId = String(applicationId).createCFString();
#endif
    return S_OK;
}

HRESULT WebPreferences::setWebAnimationsEnabled(BOOL enabled)
{
    setBoolValue(CyberKitWebAnimationsEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::webAnimationsEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitWebAnimationsEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setWebAnimationsCompositeOperationsEnabled(BOOL enabled)
{
    setBoolValue(CyberKitWebAnimationsCompositeOperationsEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::webAnimationsCompositeOperationsEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitWebAnimationsCompositeOperationsEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setWebAnimationsMutableTimelinesEnabled(BOOL enabled)
{
    setBoolValue(CyberKitWebAnimationsMutableTimelinesEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::webAnimationsMutableTimelinesEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitWebAnimationsMutableTimelinesEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setCSSCustomPropertiesAndValuesEnabled(BOOL enabled)
{
    setBoolValue(CyberKitCSSCustomPropertiesAndValuesEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::CSSCustomPropertiesAndValuesEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitCSSCustomPropertiesAndValuesEnabledPreferenceKey);
    return S_OK;
}
    
HRESULT WebPreferences::setUserTimingEnabled(BOOL enabled)
{
    setBoolValue(CyberKitUserTimingEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::userTimingEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitUserTimingEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setResourceTimingEnabled(BOOL enabled)
{
    setBoolValue(CyberKitResourceTimingEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::resourceTimingEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitResourceTimingEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::serverTimingEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitServerTimingEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setServerTimingEnabled(BOOL enabled)
{
    setBoolValue(CyberKitServerTimingEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::resizeObserverEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitResizeObserverEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setResizeObserverEnabled(BOOL enabled)
{
    setBoolValue(CyberKitResizeObserverEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::aspectRatioOfImgFromWidthAndHeightEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitAspectRatioOfImgFromWidthAndHeightEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setAspectRatioOfImgFromWidthAndHeightEnabled(BOOL enabled)
{
    setBoolValue(CyberKitAspectRatioOfImgFromWidthAndHeightEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::webSQLEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitWebSQLEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setWebSQLEnabled(BOOL enabled)
{
    setBoolValue(CyberKitWebSQLEnabledPreferenceKey, enabled);
    return S_OK;
}

HRESULT WebPreferences::allowTopNavigationToDataURLs(_Out_ BOOL* allowAccess)
{
    if (!allowAccess)
        return E_POINTER;
    *allowAccess = boolValueForKey(CyberKitAllowTopNavigationToDataURLsPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setAllowTopNavigationToDataURLs(BOOL allowAccess)
{
    setBoolValue(CyberKitAllowTopNavigationToDataURLsPreferenceKey, allowAccess);
    return S_OK;
}

HRESULT WebPreferences::modernUnprefixedWebAudioEnabled(_Out_ BOOL* enabled)
{
    if (!enabled)
        return E_POINTER;
    *enabled = boolValueForKey(CyberKitWebAudioEnabledPreferenceKey);
    return S_OK;
}

HRESULT WebPreferences::setModernUnprefixedWebAudioEnabled(BOOL enabled)
{
    setBoolValue(CyberKitWebAudioEnabledPreferenceKey, enabled);
    return S_OK;
}
