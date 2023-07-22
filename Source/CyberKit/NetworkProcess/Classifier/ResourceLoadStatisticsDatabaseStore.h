/*
 * Copyright (C) 2019 Apple Inc. All rights reserved.
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

#if ENABLE(RESOURCE_LOAD_STATISTICS)

#include "ResourceLoadStatisticsStore.h"
#include "WebResourceLoadStatisticsStore.h"
#include <CyberCore/SQLiteDatabase.h>
#include <CyberCore/SQLiteStatement.h>
#include <pal/SessionID.h>
#include <wtf/CompletionHandler.h>
#include <wtf/StdSet.h>
#include <wtf/Vector.h>
#include <wtf/WorkQueue.h>

namespace CyberCore {
class SQLiteDatabase;
class SQLiteStatement;
enum class StorageAccessPromptWasShown : bool;
enum class StorageAccessWasGranted : bool;
struct ResourceLoadStatistics;
}

namespace CyberKit {

static constexpr size_t numberOfBucketsPerStatistic = 5;
static constexpr size_t numberOfStatistics = 7;
static constexpr std::array<unsigned, numberOfBucketsPerStatistic> bucketSizes {{ 1, 3, 10, 50, 100 }};

struct PrevalentResourceDatabaseTelemetry {
    using Buckets = std::array<unsigned, numberOfBucketsPerStatistic>;

    enum class Statistic {
        NumberOfPrevalentResourcesWithUI,
        MedianSubFrameWithoutUI,
        MedianSubResourceWithoutUI,
        MedianUniqueRedirectsWithoutUI,
        MedianDataRecordsRemovedWithoutUI,
        MedianTimesAccessedDueToUserInteractionWithoutUI,
        MedianTimesAccessedDueToStorageAccessAPIWithoutUI
    };

    unsigned numberOfPrevalentResources;
    unsigned numberOfPrevalentResourcesWithUserInteraction;
    unsigned numberOfPrevalentResourcesWithoutUserInteraction;
    unsigned topPrevalentResourceWithUserInteractionDaysSinceUserInteraction;
    unsigned medianDaysSinceUserInteractionPrevalentResourceWithUserInteraction;

    std::array<Buckets, numberOfStatistics> statistics;
};

class ResourceLoadStatisticsMemoryStore;

// This is always constructed / used / destroyed on the WebResourceLoadStatisticsStore's statistics queue.
class ResourceLoadStatisticsDatabaseStore final : public ResourceLoadStatisticsStore {
public:
    ResourceLoadStatisticsDatabaseStore(WebResourceLoadStatisticsStore&, WorkQueue&, ShouldIncludeLocalhost, const String& storageDirectoryPath, PAL::SessionID);

    void populateFromMemoryStore(const ResourceLoadStatisticsMemoryStore&);
    void mergeStatistics(Vector<ResourceLoadStatistics>&&) override;
    void clear(CompletionHandler<void()>&&) override;
    bool isEmpty() const override;

    Vector<WebResourceLoadStatisticsStore::ThirdPartyData> aggregatedThirdPartyData() const override;
    void updateCookieBlocking(CompletionHandler<void()>&&) override;

    void classifyPrevalentResources() override;
    void syncStorageIfNeeded() override;
    void syncStorageImmediately() override;

    void requestStorageAccessUnderOpener(DomainInNeedOfStorageAccess&&, CyberCore::PageIdentifier openerID, OpenerDomain&&) override;

    void grandfatherDataForDomains(const HashSet<RegistrableDomain>&) override;

    bool isRegisteredAsSubresourceUnder(const SubResourceDomain&, const TopFrameDomain&) const override;
    bool isRegisteredAsSubFrameUnder(const SubFrameDomain&, const TopFrameDomain&) const override;
    bool isRegisteredAsRedirectingTo(const RedirectedFromDomain&, const RedirectedToDomain&) const override;

    void clearPrevalentResource(const RegistrableDomain&) override;
    void dumpResourceLoadStatistics(CompletionHandler<void(const String&)>&&) final;
    bool isPrevalentResource(const RegistrableDomain&) const override;
    bool isVeryPrevalentResource(const RegistrableDomain&) const override;
    void setPrevalentResource(const RegistrableDomain&) override;
    void setVeryPrevalentResource(const RegistrableDomain&) override;

    void setGrandfathered(const RegistrableDomain&, bool value) override;
    bool isGrandfathered(const RegistrableDomain&) const override;

    void setIsScheduledForAllButCookieDataRemoval(const RegistrableDomain&, bool value);
    void setSubframeUnderTopFrameDomain(const SubFrameDomain&, const TopFrameDomain&) override;
    void setSubresourceUnderTopFrameDomain(const SubResourceDomain&, const TopFrameDomain&) override;
    void setSubresourceUniqueRedirectTo(const SubResourceDomain&, const RedirectDomain&) override;
    void setSubresourceUniqueRedirectFrom(const SubResourceDomain&, const RedirectDomain&) override;
    void setTopFrameUniqueRedirectTo(const TopFrameDomain&, const RedirectDomain&) override;
    void setTopFrameUniqueRedirectFrom(const TopFrameDomain&, const RedirectDomain&) override;

    void calculateAndSubmitTelemetry() const override;

    void hasStorageAccess(const SubFrameDomain&, const TopFrameDomain&, Optional<CyberCore::FrameIdentifier>, CyberCore::PageIdentifier, CompletionHandler<void(bool)>&&) override;
    void requestStorageAccess(SubFrameDomain&&, TopFrameDomain&&, CyberCore::FrameIdentifier, CyberCore::PageIdentifier, CompletionHandler<void(StorageAccessStatus)>&&) override;
    void grantStorageAccess(SubFrameDomain&&, TopFrameDomain&&, CyberCore::FrameIdentifier, CyberCore::PageIdentifier, CyberCore::StorageAccessPromptWasShown, CompletionHandler<void(CyberCore::StorageAccessWasGranted)>&&) override;

    void logFrameNavigation(const NavigatedToDomain&, const TopFrameDomain&, const NavigatedFromDomain&, bool isRedirect, bool isMainFrame, Seconds delayAfterMainFrameDocumentLoad, bool wasPotentiallyInitiatedByUser) override;
    void logUserInteraction(const TopFrameDomain&, CompletionHandler<void()>&&) override;
    void logCrossSiteLoadWithLinkDecoration(const NavigatedFromDomain&, const NavigatedToDomain&) override;

    void clearUserInteraction(const RegistrableDomain&, CompletionHandler<void()>&&) override;
    bool hasHadUserInteraction(const RegistrableDomain&, OperatingDatesWindow) override;

    void setLastSeen(const RegistrableDomain&, Seconds) override;
    bool isCorrectSubStatisticsCount(const RegistrableDomain&, const TopFrameDomain&);
    void resourceToString(StringBuilder&, const String&) const;
    Seconds getMostRecentlyUpdatedTimestamp(const RegistrableDomain&, const TopFrameDomain&) const;

private:
    void openITPDatabase();
    bool isCorrectTableSchema();
    bool hasStorageAccess(const TopFrameDomain&, const SubFrameDomain&) const;
    Vector<WebResourceLoadStatisticsStore::ThirdPartyDataForSpecificFirstParty> getThirdPartyDataForSpecificFirstPartyDomains(unsigned, const RegistrableDomain&) const;
    void openAndDropOldDatabaseIfNecessary();
    String getDomainStringFromDomainID(unsigned) const;
    String getSubStatisticStatement(const String&) const;
    void appendSubStatisticList(StringBuilder&, const String& tableName, const String& domain) const;
    void mergeStatistic(const ResourceLoadStatistics&);
    void merge(CyberCore::SQLiteStatement&, const ResourceLoadStatistics&);
    void clearDatabaseContents();
    unsigned getNumberOfPrevalentResources() const;
    unsigned getNumberOfPrevalentResourcesWithUI() const;
    unsigned getNumberOfPrevalentResourcesWithoutUI() const;
    unsigned getTopPrevelentResourceDaysSinceUI() const;
    void resetTelemetryPreparedStatements() const;
    void resetTelemetryStatements() const;
    void calculateTelemetryData(PrevalentResourceDatabaseTelemetry&) const;
    bool insertObservedDomain(const ResourceLoadStatistics&);
    void insertDomainRelationships(const ResourceLoadStatistics&);
    void insertDomainRelationshipList(const String&, const HashSet<RegistrableDomain>&, unsigned);
    bool insertDomainRelationship(CyberCore::SQLiteStatement&, unsigned domainID, const RegistrableDomain& topFrameDomain);
    bool relationshipExists(CyberCore::SQLiteStatement&, Optional<unsigned> firstDomainID, const RegistrableDomain& secondDomain) const;
    Optional<unsigned> domainID(const RegistrableDomain&) const;
    bool domainExists(const RegistrableDomain&) const;
    void updateLastSeen(const RegistrableDomain&, WallTime);
    void updateDataRecordsRemoved(const RegistrableDomain&, int);
    void setUserInteraction(const RegistrableDomain&, bool hadUserInteraction, WallTime);
    Vector<RegistrableDomain> domainsToBlockAndDeleteCookiesFor() const;
    Vector<RegistrableDomain> domainsToBlockButKeepCookiesFor() const;
    Vector<RegistrableDomain> domainsWithUserInteractionAsFirstParty() const;

    struct DomainData {
        unsigned domainID;
        RegistrableDomain registrableDomain;
        WallTime mostRecentUserInteractionTime;
        bool hadUserInteraction;
        bool grandfathered;
        bool isScheduledForAllButCookieDataRemoval;
    };
    Vector<DomainData> domains() const;
    bool hasHadUnexpiredRecentUserInteraction(const DomainData&, OperatingDatesWindow);
    Vector<unsigned> findExpiredUserInteractions() const;
    void clearExpiredUserInteractions();
    void clearGrandfathering(Vector<unsigned>&&);
    CyberCore::StorageAccessPromptWasShown hasUserGrantedStorageAccessThroughPrompt(unsigned domainID, const RegistrableDomain&) const;
    void incrementRecordsDeletedCountForDomains(HashSet<RegistrableDomain>&&) override;

    void reclassifyResources();
    struct NotVeryPrevalentResources {
        RegistrableDomain registrableDomain;
        ResourceLoadPrevalence prevalence;
        unsigned subresourceUnderTopFrameDomainsCount;
        unsigned subresourceUniqueRedirectsToCount;
        unsigned subframeUnderTopFrameDomainsCount;
        unsigned topFrameUniqueRedirectsToCount;
    };
    HashMap<unsigned, NotVeryPrevalentResources> findNotVeryPrevalentResources();

    bool predicateValueForDomain(CyberCore::SQLiteStatement&, const RegistrableDomain&) const;

    bool areAllThirdPartyCookiesBlockedUnder(const TopFrameDomain&) override;
    CookieAccess cookieAccess(const SubResourceDomain&, const TopFrameDomain&);

    void setPrevalentResource(const RegistrableDomain&, ResourceLoadPrevalence);
    unsigned recursivelyFindNonPrevalentDomainsThatRedirectedToThisDomain(unsigned primaryDomainID, StdSet<unsigned>& nonPrevalentRedirectionSources, unsigned numberOfRecursiveCalls);
    void setDomainsAsPrevalent(StdSet<unsigned>&&);
    void grantStorageAccessInternal(SubFrameDomain&&, TopFrameDomain&&, Optional<CyberCore::FrameIdentifier>, CyberCore::PageIdentifier, CyberCore::StorageAccessPromptWasShown, CompletionHandler<void(CyberCore::StorageAccessWasGranted)>&&);
    void markAsPrevalentIfHasRedirectedToPrevalent();
    Vector<RegistrableDomain> ensurePrevalentResourcesForDebugMode() override;
    void removeDataRecords(CompletionHandler<void()>&&);
    void pruneStatisticsIfNeeded() override;
    enum class AddedRecord { No, Yes };
    std::pair<AddedRecord, unsigned> ensureResourceStatisticsForRegistrableDomain(const RegistrableDomain&);
    bool shouldRemoveAllWebsiteDataFor(const DomainData&, bool shouldCheckForGrandfathering);
    bool shouldRemoveAllButCookiesFor(const DomainData&, bool shouldCheckForGrandfathering);
    Vector<std::pair<RegistrableDomain, WebsiteDataToRemove>> registrableDomainsToRemoveWebsiteDataFor() override;
    bool isDatabaseStore() const final { return true; }

    bool createUniqueIndices();
    bool createSchema();
    bool prepareStatements();
    String ensureAndMakeDomainList(const HashSet<RegistrableDomain>&);

    
    const String m_storageDirectoryPath;
    mutable CyberCore::SQLiteDatabase m_database;
    mutable CyberCore::SQLiteStatement m_observedDomainCount;
    CyberCore::SQLiteStatement m_insertObservedDomainStatement;
    CyberCore::SQLiteStatement m_insertTopLevelDomainStatement;
    mutable CyberCore::SQLiteStatement m_domainIDFromStringStatement;
    mutable CyberCore::SQLiteStatement m_topFrameLinkDecorationsFromExists;
    mutable CyberCore::SQLiteStatement m_topFrameLoadedThirdPartyScriptsExists;
    mutable CyberCore::SQLiteStatement m_subframeUnderTopFrameDomainExists;
    mutable CyberCore::SQLiteStatement m_subresourceUnderTopFrameDomainExists;
    mutable CyberCore::SQLiteStatement m_subresourceUniqueRedirectsToExists;
    CyberCore::SQLiteStatement m_mostRecentUserInteractionStatement;
    CyberCore::SQLiteStatement m_updateLastSeenStatement;
    mutable CyberCore::SQLiteStatement m_updateDataRecordsRemovedStatement;
    CyberCore::SQLiteStatement m_updatePrevalentResourceStatement;
    mutable CyberCore::SQLiteStatement m_isPrevalentResourceStatement;
    CyberCore::SQLiteStatement m_updateVeryPrevalentResourceStatement;
    mutable CyberCore::SQLiteStatement m_isVeryPrevalentResourceStatement;
    CyberCore::SQLiteStatement m_clearPrevalentResourceStatement;
    mutable CyberCore::SQLiteStatement m_hadUserInteractionStatement;
    CyberCore::SQLiteStatement m_updateGrandfatheredStatement;
    mutable CyberCore::SQLiteStatement m_updateIsScheduledForAllButCookieDataRemovalStatement;
    mutable CyberCore::SQLiteStatement m_isGrandfatheredStatement;
    mutable CyberCore::SQLiteStatement m_findExpiredUserInteractionStatement;
    mutable CyberCore::SQLiteStatement m_countPrevalentResourcesStatement;
    mutable CyberCore::SQLiteStatement m_countPrevalentResourcesWithUserInteractionStatement;
    mutable CyberCore::SQLiteStatement m_countPrevalentResourcesWithoutUserInteractionStatement;
    mutable CyberCore::SQLiteStatement m_getResourceDataByDomainNameStatement;
    mutable CyberCore::SQLiteStatement m_getAllDomainsStatement;
    mutable CyberCore::SQLiteStatement m_domainStringFromDomainIDStatement;
    mutable CyberCore::SQLiteStatement m_getAllSubStatisticsStatement;
    mutable CyberCore::SQLiteStatement m_storageAccessExistsStatement;
    mutable CyberCore::SQLiteStatement m_getMostRecentlyUpdatedTimestampStatement;
    PAL::SessionID m_sessionID;
};

} // namespace CyberKit

SPECIALIZE_TYPE_TRAITS_BEGIN(CyberKit::ResourceLoadStatisticsDatabaseStore)
    static bool isType(const CyberKit::ResourceLoadStatisticsStore& store) { return store.isDatabaseStore(); }
SPECIALIZE_TYPE_TRAITS_END()

#endif
