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

class ResourceLoadStatisticsMemoryStore;

// This is always constructed / used / destroyed on the WebResourceLoadStatisticsStore's statistics queue.
class ResourceLoadStatisticsDatabaseStore final : public ResourceLoadStatisticsStore {
public:
    ResourceLoadStatisticsDatabaseStore(WebResourceLoadStatisticsStore&, WorkQueue&, ShouldIncludeLocalhost, const String& storageDirectoryPath);

    void populateFromMemoryStore(const ResourceLoadStatisticsMemoryStore&);

    void clear(CompletionHandler<void()>&&) override;
    bool isEmpty() const override;

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

    void setSubframeUnderTopFrameDomain(const SubFrameDomain&, const TopFrameDomain&) override;
    void setSubresourceUnderTopFrameDomain(const SubResourceDomain&, const TopFrameDomain&) override;
    void setSubresourceUniqueRedirectTo(const SubResourceDomain&, const RedirectDomain&) override;
    void setSubresourceUniqueRedirectFrom(const SubResourceDomain&, const RedirectDomain&) override;
    void setTopFrameUniqueRedirectTo(const TopFrameDomain&, const RedirectDomain&) override;
    void setTopFrameUniqueRedirectFrom(const TopFrameDomain&, const RedirectDomain&) override;

    void calculateAndSubmitTelemetry() const override;

    void hasStorageAccess(const SubFrameDomain&, const TopFrameDomain&, Optional<FrameID>, CyberCore::PageIdentifier, CompletionHandler<void(bool)>&&) override;
    void requestStorageAccess(SubFrameDomain&&, TopFrameDomain&&, FrameID, CyberCore::PageIdentifier, CompletionHandler<void(StorageAccessStatus)>&&) override;
    void grantStorageAccess(SubFrameDomain&&, TopFrameDomain&&, FrameID, CyberCore::PageIdentifier, CyberCore::StorageAccessPromptWasShown, CompletionHandler<void(CyberCore::StorageAccessWasGranted)>&&) override;

    void logFrameNavigation(const NavigatedToDomain&, const TopFrameDomain&, const NavigatedFromDomain&, bool isRedirect, bool isMainFrame) override;
    void logUserInteraction(const TopFrameDomain&) override;
    void logSubresourceLoading(const SubResourceDomain&, const TopFrameDomain&, WallTime lastSeen) override;
    void logSubresourceRedirect(const RedirectedFromDomain&, const RedirectedToDomain&) override;
    void logCrossSiteLoadWithLinkDecoration(const NavigatedFromDomain&, const NavigatedToDomain&) override;

    void clearUserInteraction(const RegistrableDomain&) override;
    bool hasHadUserInteraction(const RegistrableDomain&, OperatingDatesWindow) override;

    void setLastSeen(const RegistrableDomain&, Seconds) override;

private:
    bool insertObservedDomain(const ResourceLoadStatistics&);
    void insertDomainRelationships(const ResourceLoadStatistics&);
    bool insertDomainRelationship(CyberCore::SQLiteStatement&, unsigned domainID, const RegistrableDomain& topFrameDomain);
    bool relationshipExists(CyberCore::SQLiteStatement&, unsigned firstDomainID, const RegistrableDomain& secondDomain) const;
    unsigned domainID(const RegistrableDomain&) const;
#ifndef NDEBUG
    bool confirmDomainDoesNotExist(const RegistrableDomain&) const;
#endif
    void updateLastSeen(const RegistrableDomain&, WallTime);
    void setUserInteraction(const RegistrableDomain&, bool hadUserInteraction, WallTime);
    Vector<RegistrableDomain> domainsToBlock() const;

    struct PrevalentDomainData {
        unsigned domainID;
        RegistrableDomain registerableDomain;
        WallTime mostRecentUserInteractionTime;
        bool hadUserInteraction;
        bool grandfathered;
    };
    Vector<PrevalentDomainData> prevalentDomains() const;
    Vector<unsigned> findExpiredUserInteractions() const;
    void clearExpiredUserInteractions();
    void clearGrandfathering(Vector<unsigned>&&);
    CyberCore::StorageAccessPromptWasShown hasUserGrantedStorageAccessThroughPrompt(unsigned domainID, const RegistrableDomain&) const;
    void incrementRecordsDeletedCountForDomains(HashSet<RegistrableDomain>&&) override;

    void reclassifyResources();
    struct NotVeryPrevalentResources {
        RegistrableDomain registerableDomain;
        ResourceLoadPrevalence prevalence;
        unsigned subresourceUnderTopFrameDomainsCount;
        unsigned subresourceUniqueRedirectsToCount;
        unsigned subframeUnderTopFrameDomainsCount;
        unsigned topFrameUniqueRedirectsToCount;
    };
    HashMap<unsigned, NotVeryPrevalentResources> findNotVeryPrevalentResources();

    bool predicateValueForDomain(CyberCore::SQLiteStatement&, const RegistrableDomain&) const;

    enum class CookieTreatmentResult { Allow, BlockAndKeep, BlockAndPurge };
    CookieTreatmentResult cookieTreatmentForOrigin(const RegistrableDomain&) const;
    
    void setPrevalentResource(const RegistrableDomain&, ResourceLoadPrevalence);
    unsigned recursivelyFindNonPrevalentDomainsThatRedirectedToThisDomain(unsigned primaryDomainID, StdSet<unsigned>& nonPrevalentRedirectionSources, unsigned numberOfRecursiveCalls);
    void setDomainsAsPrevalent(StdSet<unsigned>&&);
    void grantStorageAccessInternal(SubFrameDomain&&, TopFrameDomain&&, Optional<FrameID>, CyberCore::PageIdentifier, CyberCore::StorageAccessPromptWasShown, CompletionHandler<void(CyberCore::StorageAccessWasGranted)>&&);
    void markAsPrevalentIfHasRedirectedToPrevalent();
    Vector<RegistrableDomain> ensurePrevalentResourcesForDebugMode() override;
    void removeDataRecords(CompletionHandler<void()>&&);
    void pruneStatisticsIfNeeded() override;
    enum class AddedRecord { No, Yes };
    std::pair<AddedRecord, unsigned> ensureResourceStatisticsForRegistrableDomain(const RegistrableDomain&);
    bool shouldRemoveAllWebsiteDataFor(const PrevalentDomainData&, bool shouldCheckForGrandfathering) const;
    bool shouldRemoveAllButCookiesFor(const PrevalentDomainData&, bool shouldCheckForGrandfathering) const;
    HashMap<RegistrableDomain, WebsiteDataToRemove> registrableDomainsToRemoveWebsiteDataFor() override;
    bool isDatabaseStore() const final { return true; }

    bool createSchema();
    bool prepareStatements();
    
    const String m_storageDirectoryPath;
    mutable CyberCore::SQLiteDatabase m_database;
    mutable CyberCore::SQLiteStatement m_observedDomainCount;
    CyberCore::SQLiteStatement m_insertObservedDomainStatement;
    CyberCore::SQLiteStatement m_insertTopLevelDomainStatement;
    mutable CyberCore::SQLiteStatement m_domainIDFromStringStatement;
    CyberCore::SQLiteStatement m_storageAccessUnderTopFrameDomainsStatement;
    CyberCore::SQLiteStatement m_topFrameUniqueRedirectsTo;
    mutable CyberCore::SQLiteStatement m_topFrameUniqueRedirectsToExists;
    CyberCore::SQLiteStatement m_topFrameUniqueRedirectsFrom;
    mutable CyberCore::SQLiteStatement m_topFrameUniqueRedirectsFromExists;
    CyberCore::SQLiteStatement m_topFrameLinkDecorationsFrom;
    mutable CyberCore::SQLiteStatement m_topFrameLinkDecorationsFromExists;
    CyberCore::SQLiteStatement m_subframeUnderTopFrameDomains;
    mutable CyberCore::SQLiteStatement m_subframeUnderTopFrameDomainExists;
    CyberCore::SQLiteStatement m_subresourceUnderTopFrameDomains;
    mutable CyberCore::SQLiteStatement m_subresourceUnderTopFrameDomainExists;
    CyberCore::SQLiteStatement m_subresourceUniqueRedirectsTo;
    mutable CyberCore::SQLiteStatement m_subresourceUniqueRedirectsToExists;
    CyberCore::SQLiteStatement m_subresourceUniqueRedirectsFrom;
    mutable CyberCore::SQLiteStatement m_subresourceUniqueRedirectsFromExists;
    CyberCore::SQLiteStatement m_mostRecentUserInteractionStatement;
    CyberCore::SQLiteStatement m_updateLastSeenStatement;
    CyberCore::SQLiteStatement m_updatePrevalentResourceStatement;
    mutable CyberCore::SQLiteStatement m_isPrevalentResourceStatement;
    CyberCore::SQLiteStatement m_updateVeryPrevalentResourceStatement;
    mutable CyberCore::SQLiteStatement m_isVeryPrevalentResourceStatement;
    CyberCore::SQLiteStatement m_clearPrevalentResourceStatement;
    mutable CyberCore::SQLiteStatement m_hadUserInteractionStatement;
    CyberCore::SQLiteStatement m_updateGrandfatheredStatement;
    mutable CyberCore::SQLiteStatement m_isGrandfatheredStatement;
    mutable CyberCore::SQLiteStatement m_findExpiredUserInteractionStatement;
};

} // namespace CyberKit

SPECIALIZE_TYPE_TRAITS_BEGIN(CyberKit::ResourceLoadStatisticsDatabaseStore)
    static bool isType(const CyberKit::ResourceLoadStatisticsStore& store) { return store.isDatabaseStore(); }
SPECIALIZE_TYPE_TRAITS_END()

#endif
