/*
 * Copyright (C) 2012-2020 Apple Inc. All rights reserved.
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

#include "AppPrivacyReport.h"
#include "AuxiliaryProcessProxy.h"
#include "DataReference.h"
#include "DataTaskIdentifier.h"
#include "IdentifierTypes.h"
#include "NetworkResourceLoadIdentifier.h"
#include "ProcessLauncher.h"
#include "ProcessThrottler.h"
#include "ProcessThrottlerClient.h"
#include "QuotaIncreaseRequestIdentifier.h"
#include "UserContentControllerIdentifier.h"
#include "WebsiteDataStore.h"
#include <CyberCore/CrossSiteNavigationDataTransfer.h>
#include <CyberCore/FrameIdentifier.h>
#include <CyberCore/NotificationEventType.h>
#include <CyberCore/RegistrableDomain.h>
#include <memory>
#include <wtf/Deque.h>

#if ENABLE(LEGACY_CUSTOM_PROTOCOL_MANAGER)
#include "LegacyCustomProtocolManagerProxy.h"
#endif

#if PLATFORM(COCOA)
#include "XPCEventHandler.h"
#include <wtf/OSObjectPtr.h>
#endif

namespace IPC {
class FormDataReference;
}

namespace API {
class CustomProtocolManagerClient;
class DataTask;
}

namespace PAL {
class SessionID;
}

namespace CyberCore {
class AuthenticationChallenge;
class SharedBuffer;
class ProtectionSpace;
class ResourceRequest;
class SecurityOrigin;
enum class ShouldSample : bool;
enum class StorageAccessPromptWasShown : bool;
enum class StorageAccessWasGranted : bool;
enum class StoredCredentialsPolicy : uint8_t;
struct ClientOrigin;
struct NotificationData;
class SecurityOriginData;
}

namespace WebKit {

class DownloadProxy;
class DownloadProxyMap;
class WebPageProxy;
class WebUserContentControllerProxy;

enum class ProcessTerminationReason;
enum class RemoteWorkerType : bool;
enum class ShouldGrandfatherStatistics : bool;
enum class StorageAccessStatus : uint8_t;
enum class WebsiteDataFetchOption : uint8_t;
enum class WebsiteDataType : uint32_t;

struct FrameInfoData;
struct NetworkProcessCreationParameters;
struct ResourceLoadInfo;
struct WebPushMessage;
struct WebsiteData;
struct WebsiteDataStoreParameters;

class NetworkProcessProxy final : public AuxiliaryProcessProxy, private ProcessThrottlerClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    using RegistrableDomain = CyberCore::RegistrableDomain;
    using TopFrameDomain = CyberCore::RegistrableDomain;
    using SubFrameDomain = CyberCore::RegistrableDomain;
    using SubResourceDomain = CyberCore::RegistrableDomain;
    using RedirectDomain = CyberCore::RegistrableDomain;
    using RedirectedFromDomain = CyberCore::RegistrableDomain;
    using RedirectedToDomain = CyberCore::RegistrableDomain;
    using NavigatedFromDomain = CyberCore::RegistrableDomain;
    using NavigatedToDomain = CyberCore::RegistrableDomain;
    using DomainInNeedOfStorageAccess = CyberCore::RegistrableDomain;
    using OpenerDomain = CyberCore::RegistrableDomain;

    static Ref<NetworkProcessProxy> ensureDefaultNetworkProcess();
    static WeakPtr<NetworkProcessProxy>& defaultNetworkProcess();
    static Ref<NetworkProcessProxy> create() { return adoptRef(*new NetworkProcessProxy); }
    ~NetworkProcessProxy();

    static Vector<Ref<NetworkProcessProxy>> allNetworkProcesses();
    
    void getNetworkProcessConnection(WebProcessProxy&, CompletionHandler<void(NetworkProcessConnectionInfo&&)>&&);

    DownloadProxy& createDownloadProxy(WebsiteDataStore&, Ref<API::DownloadClient>&&, const CyberCore::ResourceRequest&, const FrameInfoData&, WebPageProxy* originatingPage);
    void dataTaskWithRequest(WebPageProxy&, PAL::SessionID, CyberCore::ResourceRequest&&, CompletionHandler<void(API::DataTask&)>&&);

    void fetchWebsiteData(PAL::SessionID, OptionSet<WebsiteDataType>, OptionSet<WebsiteDataFetchOption>, CompletionHandler<void(WebsiteData)>&&);
    void deleteWebsiteData(PAL::SessionID, OptionSet<WebsiteDataType>, WallTime modifiedSince, CompletionHandler<void()>&& completionHandler);
    void deleteWebsiteDataForOrigins(PAL::SessionID, OptionSet<WebKit::WebsiteDataType>, const Vector<CyberCore::SecurityOriginData>& origins, const Vector<String>& cookieHostNames, const Vector<String>& HSTSCacheHostNames, const Vector<RegistrableDomain>&, CompletionHandler<void()>&&);
    void renameOriginInWebsiteData(PAL::SessionID, const CyberCore::SecurityOriginData&, const CyberCore::SecurityOriginData&, OptionSet<WebsiteDataType>, CompletionHandler<void()>&&);
    void websiteDataOriginDirectoryForTesting(PAL::SessionID, CyberCore::ClientOrigin&&, OptionSet<WebsiteDataType>, CompletionHandler<void(const String&)>&&);

    void preconnectTo(PAL::SessionID, WebPageProxyIdentifier, CyberCore::PageIdentifier, const URL&, const String&, CyberCore::StoredCredentialsPolicy, std::optional<NavigatingToAppBoundDomain>, LastNavigationWasAppInitiated);

#if ENABLE(TRACKING_PREVENTION)
    void clearPrevalentResource(PAL::SessionID, const RegistrableDomain&, CompletionHandler<void()>&&);
    void clearUserInteraction(PAL::SessionID, const RegistrableDomain&, CompletionHandler<void()>&&);
    void dumpResourceLoadStatistics(PAL::SessionID, CompletionHandler<void(String)>&&);
    void updatePrevalentDomainsToBlockCookiesFor(PAL::SessionID, const Vector<RegistrableDomain>&, CompletionHandler<void()>&&);
    void hasHadUserInteraction(PAL::SessionID, const RegistrableDomain&, CompletionHandler<void(bool)>&&);
    void isRelationshipOnlyInDatabaseOnce(PAL::SessionID, const RegistrableDomain& subDomain, const RegistrableDomain& topDomain, CompletionHandler<void(bool)>&&);
    void hasLocalStorage(PAL::SessionID, const RegistrableDomain&, CompletionHandler<void(bool)>&&);
    void isGrandfathered(PAL::SessionID, const RegistrableDomain&, CompletionHandler<void(bool)>&&);
    void isPrevalentResource(PAL::SessionID, const RegistrableDomain&, CompletionHandler<void(bool)>&&);
    void isRegisteredAsRedirectingTo(PAL::SessionID, const RedirectedFromDomain&, const RedirectedToDomain&, CompletionHandler<void(bool)>&&);
    void isRegisteredAsSubFrameUnder(PAL::SessionID, const SubFrameDomain&, const TopFrameDomain&, CompletionHandler<void(bool)>&&);
    void isRegisteredAsSubresourceUnder(PAL::SessionID, const SubResourceDomain&, const TopFrameDomain&, CompletionHandler<void(bool)>&&);
    void isVeryPrevalentResource(PAL::SessionID, const RegistrableDomain&, CompletionHandler<void(bool)>&&);
    void logUserInteraction(PAL::SessionID, const RegistrableDomain&, CompletionHandler<void()>&&);
    void scheduleStatisticsAndDataRecordsProcessing(PAL::SessionID, CompletionHandler<void()>&&);
    void setLastSeen(PAL::SessionID, const RegistrableDomain&, Seconds, CompletionHandler<void()>&&);
    void domainIDExistsInDatabase(PAL::SessionID, int domainID, CompletionHandler<void(bool)>&&);
    void statisticsDatabaseHasAllTables(PAL::SessionID, CompletionHandler<void(bool)>&&);
    void mergeStatisticForTesting(PAL::SessionID, const RegistrableDomain&, const TopFrameDomain& topFrameDomain1, const TopFrameDomain& topFrameDomain2, Seconds lastSeen, bool hadUserInteraction, Seconds mostRecentUserInteraction, bool isGrandfathered, bool isPrevalent, bool isVeryPrevalent, unsigned dataRecordsRemoved, CompletionHandler<void()>&&);
    void insertExpiredStatisticForTesting(PAL::SessionID, const RegistrableDomain&, unsigned numberOfOperatingDaysPassed, bool hadUserInteraction, bool isScheduledForAllButCookieDataRemoval, bool isPrevalent, CompletionHandler<void()>&&);
    void setCacheMaxAgeCap(PAL::SessionID, Seconds, CompletionHandler<void()>&&);
    void setGrandfathered(PAL::SessionID, const RegistrableDomain&, bool isGrandfathered, CompletionHandler<void()>&&);
    void setNotifyPagesWhenDataRecordsWereScanned(PAL::SessionID, bool, CompletionHandler<void()>&&);
    void setResourceLoadStatisticsTimeAdvanceForTesting(PAL::SessionID, Seconds, CompletionHandler<void()>&&);
    void setIsRunningResourceLoadStatisticsTest(PAL::SessionID, bool, CompletionHandler<void()>&&);
    void setSubframeUnderTopFrameDomain(PAL::SessionID, const SubFrameDomain&, const TopFrameDomain&, CompletionHandler<void()>&&);
    void setSubresourceUnderTopFrameDomain(PAL::SessionID, const SubResourceDomain&, const TopFrameDomain&, CompletionHandler<void()>&&);
    void setSubresourceUniqueRedirectTo(PAL::SessionID, const SubResourceDomain&, const RedirectedToDomain&, CompletionHandler<void()>&&);
    void setSubresourceUniqueRedirectFrom(PAL::SessionID, const SubResourceDomain&, const RedirectedFromDomain&, CompletionHandler<void()>&&);
    void setTimeToLiveUserInteraction(PAL::SessionID, Seconds, CompletionHandler<void()>&&);
    void setTopFrameUniqueRedirectTo(PAL::SessionID, const TopFrameDomain&, const RedirectedToDomain&, CompletionHandler<void()>&&);
    void setTopFrameUniqueRedirectFrom(PAL::SessionID, const TopFrameDomain&, const RedirectedFromDomain&, CompletionHandler<void()>&&);
    void setPrevalentResource(PAL::SessionID, const RegistrableDomain&, CompletionHandler<void()>&&);
    void setPrevalentResourceForDebugMode(PAL::SessionID, const RegistrableDomain&, CompletionHandler<void()>&&);
    void setVeryPrevalentResource(PAL::SessionID, const RegistrableDomain&, CompletionHandler<void()>&&);
    void getResourceLoadStatisticsDataSummary(PAL::SessionID, CompletionHandler<void(Vector<WebResourceLoadStatisticsStore::ThirdPartyData>&&)>&&);
    void getAllStorageAccessEntries(PAL::SessionID, CompletionHandler<void(Vector<String> domains)>&&);
    void requestStorageAccessConfirm(WebPageProxyIdentifier, CyberCore::FrameIdentifier, const SubFrameDomain&, const TopFrameDomain&, CompletionHandler<void(bool)>&&);
    void resetParametersToDefaultValues(PAL::SessionID, CompletionHandler<void()>&&);
    void scheduleClearInMemoryAndPersistent(PAL::SessionID, ShouldGrandfatherStatistics, CompletionHandler<void()>&&);
    void scheduleClearInMemoryAndPersistent(PAL::SessionID, std::optional<WallTime> modifiedSince, ShouldGrandfatherStatistics, CompletionHandler<void()>&&);
    void scheduleCookieBlockingUpdate(PAL::SessionID, CompletionHandler<void()>&&);
    void setCacheMaxAgeCapForPrevalentResources(PAL::SessionID, Seconds, CompletionHandler<void()>&&);
    void setGrandfatheringTime(PAL::SessionID, Seconds, CompletionHandler<void()>&&);
    void setMaxStatisticsEntries(PAL::SessionID, size_t maximumEntryCount, CompletionHandler<void()>&&);
    void setMinimumTimeBetweenDataRecordsRemoval(PAL::SessionID, Seconds, CompletionHandler<void()>&&);
    void setPruneEntriesDownTo(PAL::SessionID, size_t pruneTargetCount, CompletionHandler<void()>&&);
    void setResourceLoadStatisticsDebugMode(PAL::SessionID, bool debugMode, CompletionHandler<void()>&&);
    void isResourceLoadStatisticsEphemeral(PAL::SessionID, CompletionHandler<void(bool)>&&);
    void setShouldClassifyResourcesBeforeDataRecordsRemoval(PAL::SessionID, bool, CompletionHandler<void()>&&);
    void resetCacheMaxAgeCapForPrevalentResources(PAL::SessionID, CompletionHandler<void()>&&);
    void didCommitCrossSiteLoadWithDataTransfer(PAL::SessionID, const NavigatedFromDomain&, const NavigatedToDomain&, OptionSet<CyberCore::CrossSiteNavigationDataTransfer::Flag>, WebPageProxyIdentifier, CyberCore::PageIdentifier);
    void didCommitCrossSiteLoadWithDataTransferFromPrevalentResource(WebPageProxyIdentifier);
    void setCrossSiteLoadWithLinkDecorationForTesting(PAL::SessionID, const NavigatedFromDomain&, const NavigatedToDomain&, CompletionHandler<void()>&&);
    void resetCrossSiteLoadsWithLinkDecorationForTesting(PAL::SessionID, CompletionHandler<void()>&&);
    void deleteCookiesForTesting(PAL::SessionID, const RegistrableDomain&, bool includeHttpOnlyCookies, CompletionHandler<void()>&&);
    void deleteWebsiteDataInUIProcessForRegistrableDomains(PAL::SessionID, OptionSet<WebsiteDataType>, OptionSet<WebsiteDataFetchOption>, Vector<RegistrableDomain>&&, CompletionHandler<void(HashSet<CyberCore::RegistrableDomain>&&)>&&);
    void hasIsolatedSession(PAL::SessionID, const RegistrableDomain&, CompletionHandler<void(bool)>&&);
#if ENABLE(APP_BOUND_DOMAINS)
    void setAppBoundDomainsForResourceLoadStatistics(PAL::SessionID, const HashSet<RegistrableDomain>&, CompletionHandler<void()>&&);
#endif
#if ENABLE(MANAGED_DOMAINS)
    void setManagedDomainsForResourceLoadStatistics(PAL::SessionID, const HashSet<RegistrableDomain>&, CompletionHandler<void()>&&);
#endif
    void setShouldDowngradeReferrerForTesting(bool, CompletionHandler<void()>&&);
    void setThirdPartyCookieBlockingMode(PAL::SessionID, CyberCore::ThirdPartyCookieBlockingMode, CompletionHandler<void()>&&);
    void setShouldEnbleSameSiteStrictEnforcementForTesting(PAL::SessionID, CyberCore::SameSiteStrictEnforcementEnabled, CompletionHandler<void()>&&);
    void setFirstPartyWebsiteDataRemovalModeForTesting(PAL::SessionID, CyberCore::FirstPartyWebsiteDataRemovalMode, CompletionHandler<void()>&&);
    void setToSameSiteStrictCookiesForTesting(PAL::SessionID, const RegistrableDomain&, CompletionHandler<void()>&&);
    void setFirstPartyHostCNAMEDomainForTesting(PAL::SessionID, const String& firstPartyHost, const RegistrableDomain& cnameDomain, CompletionHandler<void()>&&);
    void setThirdPartyCNAMEDomainForTesting(PAL::SessionID, const CyberCore::RegistrableDomain&, CompletionHandler<void()>&&);
    void setDomainsWithUserInteraction(HashSet<CyberCore::RegistrableDomain>&&);
    void setDomainsWithCrossPageStorageAccess(HashMap<TopFrameDomain, SubResourceDomain>&&, CompletionHandler<void()>&&);
#endif

    void setPrivateClickMeasurementDebugMode(PAL::SessionID, bool);
    
    void synthesizeAppIsBackground(bool background);

    void flushCookies(PAL::SessionID, CompletionHandler<void()>&&);

    void testProcessIncomingSyncMessagesWhenWaitingForSyncReply(WebPageProxyIdentifier, CompletionHandler<void(bool)>&&);
    void terminateUnresponsiveServiceWorkerProcesses(CyberCore::ProcessIdentifier);

    void requestTermination();

    ProcessThrottler& throttler() final { return m_throttler; }
    void updateProcessAssertion();

#if ENABLE(CONTENT_EXTENSIONS)
    void didDestroyWebUserContentControllerProxy(WebUserContentControllerProxy&);
#endif

    enum class SendParametersToNetworkProcess : bool { No, Yes };
    void addSession(WebsiteDataStore&, SendParametersToNetworkProcess);
    void removeSession(WebsiteDataStore&);
    
    void createSymLinkForFileUpgrade(const String& indexedDatabaseDirectory);

    // ProcessThrottlerClient
    void sendProcessDidResume(ResumeReason) final;
    ASCIILiteral clientName() const final { return "NetworkProcess"_s; }
    
    static void setSuspensionAllowedForTesting(bool);
    void sendProcessWillSuspendImminentlyForTesting();

    void registerSchemeForLegacyCustomProtocol(const String&);
    void unregisterSchemeForLegacyCustomProtocol(const String&);

    void networkProcessDidTerminate(ProcessTerminationReason);
    
    void resetQuota(PAL::SessionID, CompletionHandler<void()>&&);
    void resetStoragePersistedState(PAL::SessionID, CompletionHandler<void()>&&);
#if PLATFORM(IOS_FAMILY)
    void setBackupExclusionPeriodForTesting(PAL::SessionID, Seconds, CompletionHandler<void()>&&);
#endif

#if ENABLE(NETWORK_ISSUE_REPORTING)
    void reportNetworkIssue(WebPageProxyIdentifier, const URL&);
#endif

    void resourceLoadDidSendRequest(WebPageProxyIdentifier, ResourceLoadInfo&&, CyberCore::ResourceRequest&&, std::optional<IPC::FormDataReference>&&);
    void resourceLoadDidPerformHTTPRedirection(WebPageProxyIdentifier, ResourceLoadInfo&&, CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&);
    void resourceLoadDidReceiveChallenge(WebPageProxyIdentifier, ResourceLoadInfo&&, CyberCore::AuthenticationChallenge&&);
    void resourceLoadDidReceiveResponse(WebPageProxyIdentifier, ResourceLoadInfo&&, CyberCore::ResourceResponse&&);
    void resourceLoadDidCompleteWithError(WebPageProxyIdentifier, ResourceLoadInfo&&, CyberCore::ResourceResponse&&, CyberCore::ResourceError&&);

#if ENABLE(APP_BOUND_DOMAINS)
    void hasAppBoundSession(PAL::SessionID, CompletionHandler<void(bool)>&&);
    void clearAppBoundSession(PAL::SessionID, CompletionHandler<void()>&&);
    void getAppBoundDomains(PAL::SessionID, CompletionHandler<void(HashSet<CyberCore::RegistrableDomain>&&)>&&);
#endif

#if ENABLE(APPLE_PAY_REMOTE_UI_USES_SCENE)
    void getWindowSceneIdentifierForPaymentPresentation(WebPageProxyIdentifier, CompletionHandler<void(const String&)>&&);
#endif
    // ProcessThrottlerClient
    void sendPrepareToSuspend(IsSuspensionImminent, double remainingRunTime, CompletionHandler<void()>&&) final;
    void updateBundleIdentifier(const String&, CompletionHandler<void()>&&);
    void clearBundleIdentifier(CompletionHandler<void()>&&);

    API::CustomProtocolManagerClient& customProtocolManagerClient() { return m_customProtocolManagerClient.get(); }

#if PLATFORM(COCOA)
    bool sendXPCEndpointToProcess(AuxiliaryProcessProxy&);
    xpc_object_t xpcEndpointMessage() const { return m_endpointMessage.get(); }
#endif

#if ENABLE(SERVICE_WORKER)
    void getPendingPushMessages(PAL::SessionID, CompletionHandler<void(const Vector<WebPushMessage>&)>&&);
    void processPushMessage(PAL::SessionID, const WebPushMessage&, CompletionHandler<void(bool wasProcessed)>&&);
    void processNotificationEvent(const CyberCore::NotificationData&, CyberCore::NotificationEventType, CompletionHandler<void(bool wasProcessed)>&&);
#endif

    void setPushAndNotificationsEnabledForOrigin(PAL::SessionID, const CyberCore::SecurityOriginData&, bool, CompletionHandler<void()>&&);
    void deletePushAndNotificationRegistration(PAL::SessionID, const CyberCore::SecurityOriginData&, CompletionHandler<void(const String&)>&&);
    void getOriginsWithPushAndNotificationPermissions(PAL::SessionID, CompletionHandler<void(const Vector<CyberCore::SecurityOriginData>&)>&&);
    void hasPushSubscriptionForTesting(PAL::SessionID, const URL&, CompletionHandler<void(bool)>&&);

    void dataTaskReceivedChallenge(DataTaskIdentifier, CyberCore::AuthenticationChallenge&&, CompletionHandler<void(AuthenticationChallengeDisposition, CyberCore::Credential&&)>&&);
    void dataTaskWillPerformHTTPRedirection(DataTaskIdentifier, CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&, CompletionHandler<void(bool)>&&);
    void dataTaskDidReceiveResponse(DataTaskIdentifier, CyberCore::ResourceResponse&&, CompletionHandler<void(bool)>&&);
    void dataTaskDidReceiveData(DataTaskIdentifier, const IPC::DataReference&);
    void dataTaskDidCompleteWithError(DataTaskIdentifier, CyberCore::ResourceError&&);
    void cancelDataTask(DataTaskIdentifier, PAL::SessionID);

    void deleteWebsiteDataInWebProcessesForOrigin(OptionSet<WebsiteDataType>, const CyberCore::ClientOrigin&, PAL::SessionID, WebPageProxyIdentifier, CompletionHandler<void()>&&);
    void reloadExecutionContextsForOrigin(const CyberCore::ClientOrigin&, PAL::SessionID, std::optional<CyberCore::FrameIdentifier> triggeringFrame, CompletionHandler<void()>&&);

    void terminateRemoteWorkerContextConnectionWhenPossible(RemoteWorkerType, PAL::SessionID, const CyberCore::RegistrableDomain&, CyberCore::ProcessIdentifier);

    void openWindowFromServiceWorker(PAL::SessionID, const String& urlString, const CyberCore::SecurityOriginData& serviceWorkerOrigin, CompletionHandler<void(std::optional<CyberCore::PageIdentifier>&&)>&&);

    void navigateServiceWorkerClient(CyberCore::FrameIdentifier, CyberCore::ScriptExecutionContextIdentifier, const URL&, CompletionHandler<void(std::optional<CyberCore::PageIdentifier>, std::optional<CyberCore::FrameIdentifier>)>&&);

    void cookiesDidChange(PAL::SessionID);

#if ENABLE(INSPECTOR_NETWORK_THROTTLING)
    void setEmulatedConditions(PAL::SessionID, std::optional<int64_t>&& bytesPerSecondLimit);
#endif

    void notifyMediaStreamingActivity(bool);

private:
    explicit NetworkProcessProxy();

    void sendCreationParametersToNewProcess();

    // AuxiliaryProcessProxy
    ASCIILiteral processName() const final { return "Networking"_s; }

    void getLaunchOptions(ProcessLauncher::LaunchOptions&) override;
    void connectionWillOpen(IPC::Connection&) override;
    void processWillShutDown(IPC::Connection&) override;
    void terminate() final;

    // IPC::Connection::Client
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;
    bool didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, UniqueRef<IPC::Encoder>&) override;
    void didClose(IPC::Connection&) override;
    void didReceiveInvalidMessage(IPC::Connection&, IPC::MessageName) override;
    bool didReceiveSyncNetworkProcessProxyMessage(IPC::Connection&, IPC::Decoder&, UniqueRef<IPC::Encoder>&);

    // ResponsivenessTimer::Client
    void didBecomeUnresponsive() final;

    // Message handlers
    void didReceiveNetworkProcessProxyMessage(IPC::Connection&, IPC::Decoder&);
    void didReceiveAuthenticationChallenge(PAL::SessionID, WebPageProxyIdentifier, const std::optional<CyberCore::SecurityOriginData>&, CyberCore::AuthenticationChallenge&&, bool, AuthenticationChallengeIdentifier);
    void negotiatedLegacyTLS(WebPageProxyIdentifier);
    void didNegotiateModernTLS(WebPageProxyIdentifier, const URL&);
    void didFailLoadDueToNetworkConnectionIntegrity(WebPageProxyIdentifier, const URL&); 
    void setWebProcessHasUploads(CyberCore::ProcessIdentifier, bool);
    void logDiagnosticMessage(WebPageProxyIdentifier, const String& message, const String& description, CyberCore::ShouldSample);
    void logDiagnosticMessageWithResult(WebPageProxyIdentifier, const String& message, const String& description, uint32_t result, CyberCore::ShouldSample);
    void logDiagnosticMessageWithValue(WebPageProxyIdentifier, const String& message, const String& description, double value, unsigned significantFigures, CyberCore::ShouldSample);
#if ENABLE(TRACKING_PREVENTION)
    void logTestingEvent(PAL::SessionID, const String& event);
    void notifyResourceLoadStatisticsProcessed();
    void notifyWebsiteDataDeletionForRegistrableDomainsFinished();
    void notifyWebsiteDataScanForRegistrableDomainsFinished();
#endif

#if ENABLE(CONTENT_EXTENSIONS)
    void contentExtensionRules(UserContentControllerIdentifier);
#endif

#if ENABLE(SERVICE_WORKER)
    void startServiceWorkerBackgroundProcessing(CyberCore::ProcessIdentifier serviceWorkerProcessIdentifier);
    void endServiceWorkerBackgroundProcessing(CyberCore::ProcessIdentifier serviceWorkerProcessIdentifier);
    void requestBackgroundFetchPermission(PAL::SessionID, const CyberCore::ClientOrigin&, CompletionHandler<void(bool)>&&);
#endif
    void remoteWorkerContextConnectionNoLongerNeeded(RemoteWorkerType, CyberCore::ProcessIdentifier);
    void establishRemoteWorkerContextConnectionToNetworkProcess(RemoteWorkerType, CyberCore::RegistrableDomain&&, std::optional<CyberCore::ProcessIdentifier> requestingProcessIdentifier, std::optional<CyberCore::ScriptExecutionContextIdentifier> serviceWorkerPageIdentifier, PAL::SessionID, CompletionHandler<void(CyberCore::ProcessIdentifier)>&&);
    void registerRemoteWorkerClientProcess(RemoteWorkerType, CyberCore::ProcessIdentifier clientProcessIdentifier, CyberCore::ProcessIdentifier remoteWorkerProcessIdentifier);
    void unregisterRemoteWorkerClientProcess(RemoteWorkerType, CyberCore::ProcessIdentifier clientProcessIdentifier, CyberCore::ProcessIdentifier remoteWorkerProcessIdentifier);

    void terminateWebProcess(CyberCore::ProcessIdentifier);

    void triggerBrowsingContextGroupSwitchForNavigation(WebPageProxyIdentifier, uint64_t navigationID, CyberCore::BrowsingContextGroupSwitchDecision, const CyberCore::RegistrableDomain& responseDomain, NetworkResourceLoadIdentifier existingNetworkResourceLoadIdentifierToResume, CompletionHandler<void(bool success)>&&);

    void requestStorageSpace(PAL::SessionID, const CyberCore::ClientOrigin&, uint64_t quota, uint64_t currentSize, uint64_t spaceRequired, CompletionHandler<void(std::optional<uint64_t> quota)>&&);
    void increaseQuota(PAL::SessionID, const CyberCore::ClientOrigin&, QuotaIncreaseRequestIdentifier, uint64_t currentQuota, uint64_t currentUsage, uint64_t spaceRequested);

    WebsiteDataStore* websiteDataStoreFromSessionID(PAL::SessionID);

    // ProcessLauncher::Client
    void didFinishLaunching(ProcessLauncher*, IPC::Connection::Identifier) override;
#if PLATFORM(COCOA)
    RefPtr<XPCEventHandler> xpcEventHandler() const override;
#endif

    void processAuthenticationChallenge(PAL::SessionID, Ref<AuthenticationChallengeProxy>&&);

#if USE(SOUP)
    void didExceedMemoryLimit();
#endif

    void applicationDidEnterBackground();
    void applicationWillEnterForeground();
#if PLATFORM(IOS_FAMILY)
    void addBackgroundStateObservers();
    void removeBackgroundStateObservers();
#endif

    std::unique_ptr<DownloadProxyMap> m_downloadProxyMap;

    UniqueRef<API::CustomProtocolManagerClient> m_customProtocolManagerClient;
#if ENABLE(LEGACY_CUSTOM_PROTOCOL_MANAGER)
    LegacyCustomProtocolManagerProxy m_customProtocolManagerProxy;
#endif

    ProcessThrottler m_throttler;
    ProcessThrottler::ActivityVariant m_activityFromWebProcesses;

#if ENABLE(CONTENT_EXTENSIONS)
    HashSet<WebUserContentControllerProxy*> m_webUserContentControllerProxies;
#endif

    struct UploadActivity {
        RefPtr<ProcessAssertion> uiAssertion;
        RefPtr<ProcessAssertion> networkAssertion;
        HashMap<CyberCore::ProcessIdentifier, RefPtr<ProcessAssertion>> webProcessAssertions;
    };
    std::optional<UploadActivity> m_uploadActivity;

#if PLATFORM(COCOA)
    class XPCEventHandler : public WebKit::XPCEventHandler {
    public:
        XPCEventHandler(const NetworkProcessProxy&);

        bool handleXPCEvent(xpc_object_t) const override;

    private:
        WeakPtr<NetworkProcessProxy> m_networkProcess;
    };
    OSObjectPtr<xpc_object_t> m_endpointMessage;
#endif

    WeakHashSet<WebsiteDataStore> m_websiteDataStores;
    HashMap<DataTaskIdentifier, Ref<API::DataTask>> m_dataTasks;

#if PLATFORM(IOS_FAMILY)
    RetainPtr<id> m_backgroundObserver;
    RetainPtr<id> m_foregroundObserver;
#endif
};

} // namespace WebKit
