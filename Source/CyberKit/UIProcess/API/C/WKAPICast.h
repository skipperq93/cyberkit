/*
 * Copyright (C) 2010-2017 Apple Inc. All rights reserved.
 * Portions Copyright (c) 2010 Motorola Mobility, Inc.  All rights reserved.
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

#ifndef WKAPICast_h
#define WKAPICast_h

#include "CacheModel.h"
#include "HTTPCookieAcceptPolicy.h"
#include "InjectedBundleHitTestResultMediaType.h"
#include "PluginModuleInfo.h"
#include "ProcessTerminationReason.h"
#include "ResourceCachesToClear.h"
#include "WKBundleHitTestResult.h"
#include "WKContext.h"
#include "WKCookieManager.h"
#include "WKCredentialTypes.h"
#include "WKPage.h"
#include "WKPreferencesRef.h"
#include "WKPreferencesRefPrivate.h"
#include "WKProcessTerminationReason.h"
#include "WKProtectionSpaceTypes.h"
#include "WKResourceCacheManager.h"
#include "WKSharedAPICast.h"
#include <CyberCore/Credential.h>
#include <CyberCore/FrameLoaderTypes.h>
#include <CyberCore/PluginData.h>
#include <CyberCore/ProtectionSpace.h>
#include <CyberCore/Settings.h>

namespace API {
class ContentRuleList;
class ContentRuleListStore;
class InternalDebugFeature;
class ExperimentalFeature;
class FrameHandle;
class FrameInfo;
class HitTestResult;
class Navigation;
class NavigationAction;
class NavigationData;
class NavigationResponse;
class OpenPanelParameters;
class PageConfiguration;
class ProcessPoolConfiguration;
class SessionState;
class UserScript;
class WebsiteDataStore;
class WebsitePolicies;
class WindowFeatures;
}

namespace CyberKit {

class AuthenticationChallengeProxy;
class AuthenticationDecisionListener;
class DownloadProxy;
class GeolocationPermissionRequest;
class NotificationPermissionRequest;
class UserMediaPermissionCheckProxy;
class UserMediaPermissionRequestProxy;
class WebBackForwardList;
class WebBackForwardListItem;
class WebColorPickerResultListenerProxy;
class WebContextMenuListenerProxy;
class WebCookieManagerProxy;
class WebCredential;
class WebFormSubmissionListenerProxy;
class WebFramePolicyListenerProxy;
class WebFrameProxy;
class WebGeolocationManagerProxy;
class WebGeolocationPosition;
class WebIconDatabase;
class WebInspectorProxy;
class WebMediaSessionFocusManager;
class WebMediaSessionMetadata;
class WebNotification;
class WebNotificationManagerProxy;
class WebNotificationProvider;
class WebOpenPanelResultListenerProxy;
class WebPageGroup;
class WebPageProxy;
class WebPreferences;
class WebProcessPool;
class WebProtectionSpace;
class WebRenderLayer;
class WebRenderObject;
class WebResourceLoadStatisticsManager;
class WebTextChecker;
class WebUserContentControllerProxy;
class WebViewportAttributes;

WK_ADD_API_MAPPING(WKAuthenticationChallengeRef, AuthenticationChallengeProxy)
WK_ADD_API_MAPPING(WKAuthenticationDecisionListenerRef, AuthenticationDecisionListener)
WK_ADD_API_MAPPING(WKBackForwardListItemRef, WebBackForwardListItem)
WK_ADD_API_MAPPING(WKBackForwardListRef, WebBackForwardList)
WK_ADD_API_MAPPING(WKBundleHitTestResultMediaType, BundleHitTestResultMediaType)
WK_ADD_API_MAPPING(WKColorPickerResultListenerRef, WebColorPickerResultListenerProxy)
WK_ADD_API_MAPPING(WKContextRef, WebProcessPool)
WK_ADD_API_MAPPING(WKContextConfigurationRef, API::ProcessPoolConfiguration)
WK_ADD_API_MAPPING(WKContextMenuListenerRef, WebContextMenuListenerProxy)
WK_ADD_API_MAPPING(WKCookieManagerRef, WebCookieManagerProxy)
WK_ADD_API_MAPPING(WKCredentialRef, WebCredential)
WK_ADD_API_MAPPING(WKDownloadRef, DownloadProxy)
WK_ADD_API_MAPPING(WKFormSubmissionListenerRef, WebFormSubmissionListenerProxy)
WK_ADD_API_MAPPING(WKFramePolicyListenerRef, WebFramePolicyListenerProxy)
WK_ADD_API_MAPPING(WKFrameHandleRef, API::FrameHandle)
WK_ADD_API_MAPPING(WKFrameInfoRef, API::FrameInfo)
WK_ADD_API_MAPPING(WKFrameRef, WebFrameProxy)
WK_ADD_API_MAPPING(WKGeolocationManagerRef, WebGeolocationManagerProxy)
WK_ADD_API_MAPPING(WKGeolocationPermissionRequestRef, GeolocationPermissionRequest)
WK_ADD_API_MAPPING(WKGeolocationPositionRef, WebGeolocationPosition)
WK_ADD_API_MAPPING(WKHitTestResultRef, API::HitTestResult)
WK_ADD_API_MAPPING(WKIconDatabaseRef, WebIconDatabase)
WK_ADD_API_MAPPING(WKInspectorRef, WebInspectorProxy)
WK_ADD_API_MAPPING(WKMediaSessionFocusManagerRef, WebMediaSessionFocusManager)
WK_ADD_API_MAPPING(WKMediaSessionMetadataRef, WebMediaSessionMetadata)
WK_ADD_API_MAPPING(WKNavigationActionRef, API::NavigationAction)
WK_ADD_API_MAPPING(WKNavigationDataRef, API::NavigationData)
WK_ADD_API_MAPPING(WKNavigationRef, API::Navigation)
WK_ADD_API_MAPPING(WKNavigationResponseRef, API::NavigationResponse)
WK_ADD_API_MAPPING(WKNotificationManagerRef, WebNotificationManagerProxy)
WK_ADD_API_MAPPING(WKNotificationPermissionRequestRef, NotificationPermissionRequest)
WK_ADD_API_MAPPING(WKNotificationProviderRef, WebNotificationProvider)
WK_ADD_API_MAPPING(WKNotificationRef, WebNotification)
WK_ADD_API_MAPPING(WKOpenPanelParametersRef, API::OpenPanelParameters)
WK_ADD_API_MAPPING(WKOpenPanelResultListenerRef, WebOpenPanelResultListenerProxy)
WK_ADD_API_MAPPING(WKPageGroupRef, WebPageGroup)
WK_ADD_API_MAPPING(WKPageConfigurationRef, API::PageConfiguration)
WK_ADD_API_MAPPING(WKPageRef, WebPageProxy)
WK_ADD_API_MAPPING(WKPreferencesRef, WebPreferences)
WK_ADD_API_MAPPING(WKProtectionSpaceRef, WebProtectionSpace)
WK_ADD_API_MAPPING(WKRenderLayerRef, WebRenderLayer)
WK_ADD_API_MAPPING(WKRenderObjectRef, WebRenderObject)
WK_ADD_API_MAPPING(WKResourceLoadStatisticsManagerRef, WebResourceLoadStatisticsManager)
WK_ADD_API_MAPPING(WKSessionStateRef, API::SessionState)
WK_ADD_API_MAPPING(WKTextCheckerRef, WebTextChecker)
WK_ADD_API_MAPPING(WKUserContentControllerRef, WebUserContentControllerProxy)
WK_ADD_API_MAPPING(WKUserContentExtensionStoreRef, API::ContentRuleListStore)
WK_ADD_API_MAPPING(WKUserContentFilterRef, API::ContentRuleList)
WK_ADD_API_MAPPING(WKUserMediaPermissionCheckRef, UserMediaPermissionCheckProxy)
WK_ADD_API_MAPPING(WKUserMediaPermissionRequestRef, UserMediaPermissionRequestProxy)
WK_ADD_API_MAPPING(WKUserScriptRef, API::UserScript)
WK_ADD_API_MAPPING(WKViewportAttributesRef, WebViewportAttributes)
WK_ADD_API_MAPPING(WKWebsiteDataStoreRef, API::WebsiteDataStore)
WK_ADD_API_MAPPING(WKWebsitePoliciesRef, API::WebsitePolicies)
WK_ADD_API_MAPPING(WKWindowFeaturesRef, API::WindowFeatures)

/* Enum conversions */

inline BundleHitTestResultMediaType toBundleHitTestResultMediaType(WKBundleHitTestResultMediaType wkMediaType)
{
    switch (wkMediaType) {
    case kWKBundleHitTestResultMediaTypeNone:
        return BundleHitTestResultMediaTypeNone;
    case kWKBundleHitTestResultMediaTypeAudio:
        return BundleHitTestResultMediaTypeAudio;
    case kWKBundleHitTestResultMediaTypeVideo:
        return BundleHitTestResultMediaTypeVideo;
    }
    
    ASSERT_NOT_REACHED();
    return BundleHitTestResultMediaTypeNone;
}
    
inline WKBundleHitTestResultMediaType toAPI(BundleHitTestResultMediaType mediaType)
{
    switch (mediaType) {
    case BundleHitTestResultMediaTypeNone:
        return kWKBundleHitTestResultMediaTypeNone;
    case BundleHitTestResultMediaTypeAudio:
        return kWKBundleHitTestResultMediaTypeAudio;
    case BundleHitTestResultMediaTypeVideo:
        return kWKBundleHitTestResultMediaTypeVideo;
    }
    
    ASSERT_NOT_REACHED();
    return kWKBundleHitTestResultMediaTypeNone;
}

inline CacheModel toCacheModel(WKCacheModel wkCacheModel)
{
    switch (wkCacheModel) {
    case kWKCacheModelDocumentViewer:
        return CacheModel::DocumentViewer;
    case kWKCacheModelDocumentBrowser:
        return CacheModel::DocumentBrowser;
    case kWKCacheModelPrimaryWebBrowser:
        return CacheModel::PrimaryWebBrowser;
    }

    ASSERT_NOT_REACHED();
    return CacheModel::DocumentViewer;
}

inline WKCacheModel toAPI(CacheModel cacheModel)
{
    switch (cacheModel) {
    case CacheModel::DocumentViewer:
        return kWKCacheModelDocumentViewer;
    case CacheModel::DocumentBrowser:
        return kWKCacheModelDocumentBrowser;
    case CacheModel::PrimaryWebBrowser:
        return kWKCacheModelPrimaryWebBrowser;
    }
    
    return kWKCacheModelDocumentViewer;
}

inline WKProcessTerminationReason toAPI(ProcessTerminationReason reason)
{
    switch (reason) {
    case ProcessTerminationReason::ExceededMemoryLimit:
        return kWKProcessTerminationReasonExceededMemoryLimit;
    case ProcessTerminationReason::ExceededCPULimit:
        return kWKProcessTerminationReasonExceededCPULimit;
    case ProcessTerminationReason::NavigationSwap:
        // We probably shouldn't bother coming up with a new C-API type for process-swapping.
        // "Requested by client" seems like the best match for existing types.
        FALLTHROUGH;
    case ProcessTerminationReason::RequestedByClient:
        return kWKProcessTerminationReasonRequestedByClient;
    case ProcessTerminationReason::Crash:
        return kWKProcessTerminationReasonCrash;
    }

    return kWKProcessTerminationReasonCrash;
}

inline WKEditableLinkBehavior toAPI(CyberCore::EditableLinkBehavior behavior)
{
    switch (behavior) {
    case CyberCore::EditableLinkDefaultBehavior:
        return kWKEditableLinkBehaviorDefault;
    case CyberCore::EditableLinkAlwaysLive:
        return kWKEditableLinkBehaviorAlwaysLive;
    case CyberCore::EditableLinkOnlyLiveWithShiftKey:
        return kWKEditableLinkBehaviorOnlyLiveWithShiftKey;
    case CyberCore::EditableLinkLiveWhenNotFocused:
        return kWKEditableLinkBehaviorLiveWhenNotFocused;
    case CyberCore::EditableLinkNeverLive:
        return kWKEditableLinkBehaviorNeverLive;
    }
    
    ASSERT_NOT_REACHED();
    return kWKEditableLinkBehaviorNeverLive;
}

inline CyberCore::EditableLinkBehavior toEditableLinkBehavior(WKEditableLinkBehavior wkBehavior)
{
    switch (wkBehavior) {
    case kWKEditableLinkBehaviorDefault:
        return CyberCore::EditableLinkDefaultBehavior;
    case kWKEditableLinkBehaviorAlwaysLive:
        return CyberCore::EditableLinkAlwaysLive;
    case kWKEditableLinkBehaviorOnlyLiveWithShiftKey:
        return CyberCore::EditableLinkOnlyLiveWithShiftKey;
    case kWKEditableLinkBehaviorLiveWhenNotFocused:
        return CyberCore::EditableLinkLiveWhenNotFocused;
    case kWKEditableLinkBehaviorNeverLive:
        return CyberCore::EditableLinkNeverLive;
    }
    
    ASSERT_NOT_REACHED();
    return CyberCore::EditableLinkNeverLive;
}
    
inline WKProtectionSpaceServerType toAPI(CyberCore::ProtectionSpaceServerType type)
{
    switch (type) {
    case CyberCore::ProtectionSpaceServerHTTP:
        return kWKProtectionSpaceServerTypeHTTP;
    case CyberCore::ProtectionSpaceServerHTTPS:
        return kWKProtectionSpaceServerTypeHTTPS;
    case CyberCore::ProtectionSpaceServerFTP:
        return kWKProtectionSpaceServerTypeFTP;
    case CyberCore::ProtectionSpaceServerFTPS:
        return kWKProtectionSpaceServerTypeFTPS;
    case CyberCore::ProtectionSpaceProxyHTTP:
        return kWKProtectionSpaceProxyTypeHTTP;
    case CyberCore::ProtectionSpaceProxyHTTPS:
        return kWKProtectionSpaceProxyTypeHTTPS;
    case CyberCore::ProtectionSpaceProxyFTP:
        return kWKProtectionSpaceProxyTypeFTP;
    case CyberCore::ProtectionSpaceProxySOCKS:
        return kWKProtectionSpaceProxyTypeSOCKS;
    }
    return kWKProtectionSpaceServerTypeHTTP;
}

inline WKProtectionSpaceAuthenticationScheme toAPI(CyberCore::ProtectionSpaceAuthenticationScheme type)
{
    switch (type) {
    case CyberCore::ProtectionSpaceAuthenticationSchemeDefault:
        return kWKProtectionSpaceAuthenticationSchemeDefault;
    case CyberCore::ProtectionSpaceAuthenticationSchemeHTTPBasic:
        return kWKProtectionSpaceAuthenticationSchemeHTTPBasic;
    case CyberCore::ProtectionSpaceAuthenticationSchemeHTTPDigest:
        return kWKProtectionSpaceAuthenticationSchemeHTTPDigest;
    case CyberCore::ProtectionSpaceAuthenticationSchemeHTMLForm:
        return kWKProtectionSpaceAuthenticationSchemeHTMLForm;
    case CyberCore::ProtectionSpaceAuthenticationSchemeNTLM:
        return kWKProtectionSpaceAuthenticationSchemeNTLM;
    case CyberCore::ProtectionSpaceAuthenticationSchemeNegotiate:
        return kWKProtectionSpaceAuthenticationSchemeNegotiate;
    case CyberCore::ProtectionSpaceAuthenticationSchemeClientCertificateRequested:
        return kWKProtectionSpaceAuthenticationSchemeClientCertificateRequested;
    case CyberCore::ProtectionSpaceAuthenticationSchemeServerTrustEvaluationRequested:
        return kWKProtectionSpaceAuthenticationSchemeServerTrustEvaluationRequested;
    case CyberCore::ProtectionSpaceAuthenticationSchemeOAuth:
        return kWKProtectionSpaceAuthenticationSchemeOAuth;
    default:
        return kWKProtectionSpaceAuthenticationSchemeUnknown;
    }
}

inline CyberCore::CredentialPersistence toCredentialPersistence(WKCredentialPersistence type)
{
    switch (type) {
    case kWKCredentialPersistenceNone:
        return CyberCore::CredentialPersistenceNone;
    case kWKCredentialPersistenceForSession:
        return CyberCore::CredentialPersistenceForSession;
    case kWKCredentialPersistencePermanent:
        return CyberCore::CredentialPersistencePermanent;
    default:
        return CyberCore::CredentialPersistenceNone;
    }
}

inline ResourceCachesToClear toResourceCachesToClear(WKResourceCachesToClear wkResourceCachesToClear)
{
    switch (wkResourceCachesToClear) {
    case WKResourceCachesToClearAll:
        return AllResourceCaches;
    case WKResourceCachesToClearInMemoryOnly:
        return InMemoryResourceCachesOnly;
    }

    ASSERT_NOT_REACHED();
    return AllResourceCaches;
}

inline HTTPCookieAcceptPolicy toHTTPCookieAcceptPolicy(WKHTTPCookieAcceptPolicy policy)
{
    switch (policy) {
    case kWKHTTPCookieAcceptPolicyAlways:
        return HTTPCookieAcceptPolicyAlways;
    case kWKHTTPCookieAcceptPolicyNever:
        return HTTPCookieAcceptPolicyNever;
    case kWKHTTPCookieAcceptPolicyOnlyFromMainDocumentDomain:
        return HTTPCookieAcceptPolicyOnlyFromMainDocumentDomain;
    case kWKHTTPCookieAcceptPolicyExclusivelyFromMainDocumentDomain:
        return HTTPCookieAcceptPolicyExclusivelyFromMainDocumentDomain;
    }

    ASSERT_NOT_REACHED();
    return HTTPCookieAcceptPolicyAlways;
}

inline WKHTTPCookieAcceptPolicy toAPI(HTTPCookieAcceptPolicy policy)
{
    switch (policy) {
    case HTTPCookieAcceptPolicyAlways:
        return kWKHTTPCookieAcceptPolicyAlways;
    case HTTPCookieAcceptPolicyNever:
        return kWKHTTPCookieAcceptPolicyNever;
    case HTTPCookieAcceptPolicyOnlyFromMainDocumentDomain:
        return kWKHTTPCookieAcceptPolicyOnlyFromMainDocumentDomain;
    case HTTPCookieAcceptPolicyExclusivelyFromMainDocumentDomain:
        return kWKHTTPCookieAcceptPolicyExclusivelyFromMainDocumentDomain;
    }

    ASSERT_NOT_REACHED();
    return kWKHTTPCookieAcceptPolicyAlways;
}

inline CyberCore::SecurityOrigin::StorageBlockingPolicy toStorageBlockingPolicy(WKStorageBlockingPolicy policy)
{
    switch (policy) {
    case kWKAllowAllStorage:
        return CyberCore::SecurityOrigin::AllowAllStorage;
    case kWKBlockThirdPartyStorage:
        return CyberCore::SecurityOrigin::BlockThirdPartyStorage;
    case kWKBlockAllStorage:
        return CyberCore::SecurityOrigin::BlockAllStorage;
    }

    ASSERT_NOT_REACHED();
    return CyberCore::SecurityOrigin::AllowAllStorage;
}

inline WKStorageBlockingPolicy toAPI(CyberCore::SecurityOrigin::StorageBlockingPolicy policy)
{
    switch (policy) {
    case CyberCore::SecurityOrigin::AllowAllStorage:
        return kWKAllowAllStorage;
    case CyberCore::SecurityOrigin::BlockThirdPartyStorage:
        return kWKBlockThirdPartyStorage;
    case CyberCore::SecurityOrigin::BlockAllStorage:
        return kWKBlockAllStorage;
    }

    ASSERT_NOT_REACHED();
    return kWKAllowAllStorage;
}

inline WKPluginLoadPolicy toWKPluginLoadPolicy(PluginModuleLoadPolicy pluginModuleLoadPolicy)
{
    switch (pluginModuleLoadPolicy) {
    case PluginModuleLoadNormally:
        return kWKPluginLoadPolicyLoadNormally;
    case PluginModuleLoadUnsandboxed:
        return kWKPluginLoadPolicyLoadUnsandboxed;
    case PluginModuleBlockedForSecurity:
        return kWKPluginLoadPolicyBlocked;
    case PluginModuleBlockedForCompatibility:
        return kWKPluginLoadPolicyBlockedForCompatibility;
    }
    
    ASSERT_NOT_REACHED();
    return kWKPluginLoadPolicyBlocked;
}

inline WKPluginLoadClientPolicy toWKPluginLoadClientPolicy(CyberCore::PluginLoadClientPolicy PluginLoadClientPolicy)
{
    switch (PluginLoadClientPolicy) {
    case CyberCore::PluginLoadClientPolicyUndefined:
        return kWKPluginLoadClientPolicyUndefined;
    case CyberCore::PluginLoadClientPolicyBlock:
        return kWKPluginLoadClientPolicyBlock;
    case CyberCore::PluginLoadClientPolicyAsk:
        return kWKPluginLoadClientPolicyAsk;
    case CyberCore::PluginLoadClientPolicyAllow:
        return kWKPluginLoadClientPolicyAllow;
    case CyberCore::PluginLoadClientPolicyAllowAlways:
        return kWKPluginLoadClientPolicyAllowAlways;
    }

    ASSERT_NOT_REACHED();
    return kWKPluginLoadClientPolicyBlock;
}

inline PluginModuleLoadPolicy toPluginModuleLoadPolicy(WKPluginLoadPolicy pluginLoadPolicy)
{
    switch (pluginLoadPolicy) {
    case kWKPluginLoadPolicyLoadNormally:
        return PluginModuleLoadNormally;
    case kWKPluginLoadPolicyBlocked:
        return PluginModuleBlockedForSecurity;
    case kWKPluginLoadPolicyBlockedForCompatibility:
        return PluginModuleBlockedForCompatibility;
    case kWKPluginLoadPolicyLoadUnsandboxed:
        return PluginModuleLoadUnsandboxed;
    }
    
    ASSERT_NOT_REACHED();
    return PluginModuleBlockedForSecurity;
}

inline CyberCore::PluginLoadClientPolicy toPluginLoadClientPolicy(WKPluginLoadClientPolicy pluginLoadClientPolicy)
{
    switch (pluginLoadClientPolicy) {
    case kWKPluginLoadClientPolicyUndefined:
        return CyberCore::PluginLoadClientPolicyUndefined;
    case kWKPluginLoadClientPolicyBlock:
        return CyberCore::PluginLoadClientPolicyBlock;
    case kWKPluginLoadClientPolicyAsk:
        return CyberCore::PluginLoadClientPolicyAsk;
    case kWKPluginLoadClientPolicyAllow:
        return CyberCore::PluginLoadClientPolicyAllow;
    case kWKPluginLoadClientPolicyAllowAlways:
        return CyberCore::PluginLoadClientPolicyAllowAlways;
    }

    ASSERT_NOT_REACHED();
    return CyberCore::PluginLoadClientPolicyBlock;
}

inline CyberCore::WebGLLoadPolicy toWebGLLoadPolicy(WKWebGLLoadPolicy webGLLoadPolicy)
{
    switch (webGLLoadPolicy) {
    case kWKWebGLLoadPolicyLoadNormally:
        return CyberCore::WebGLAllowCreation;
    case kWKWebGLLoadPolicyBlocked:
        return CyberCore::WebGLBlockCreation;
    case kWKWebGLLoadPolicyPending:
        return CyberCore::WebGLPendingCreation;
    }
    
    ASSERT_NOT_REACHED();
    return CyberCore::WebGLAllowCreation;
}

inline WKWebGLLoadPolicy toAPI(CyberCore::WebGLLoadPolicy webGLLoadPolicy)
{
    switch (webGLLoadPolicy) {
    case CyberCore::WebGLAllowCreation:
        return kWKWebGLLoadPolicyLoadNormally;
    case CyberCore::WebGLBlockCreation:
        return kWKWebGLLoadPolicyBlocked;
    case CyberCore::WebGLPendingCreation:
        return kWKWebGLLoadPolicyPending;
    }

    ASSERT_NOT_REACHED();
    return kWKWebGLLoadPolicyLoadNormally;
}

} // namespace CyberKit

#if defined(BUILDING_GTK__)
#include "WKAPICastGtk.h"
#endif

#if defined(BUILDING_WPE__)
#include "WKAPICastWPE.h"
#endif

#if defined(WIN32) || defined(_WIN32)
#include "WKAPICastWin.h"
#endif

#endif // WKAPICast_h
