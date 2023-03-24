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

#include "config.h"
#include "WKWebsiteDataStoreConfigurationRef.h"

#include "WKAPICast.h"
#include "WebsiteDataStoreConfiguration.h"

WKTypeID WKWebsiteDataStoreConfigurationGetTypeID()
{
    return CyberKit::toAPI(CyberKit::WebsiteDataStoreConfiguration::APIType);
}

WKWebsiteDataStoreConfigurationRef WKWebsiteDataStoreConfigurationCreate()
{
#if PLATFORM(COCOA)
    auto configuration = CyberKit::WebsiteDataStoreConfiguration::create(CyberKit::IsPersistent::Yes);
#else
    auto configuration = CyberKit::WebsiteDataStoreConfiguration::createWithBaseDirectories(nullString(), nullString());
#endif
    return toAPI(&configuration.leakRef());
}

WKStringRef WKWebsiteDataStoreConfigurationCopyApplicationCacheDirectory(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(configuration)->applicationCacheDirectory());
}

void WKWebsiteDataStoreConfigurationSetApplicationCacheDirectory(WKWebsiteDataStoreConfigurationRef configuration, WKStringRef directory)
{
    CyberKit::toImpl(configuration)->setApplicationCacheDirectory(CyberKit::toImpl(directory)->string());
}

WKStringRef WKWebsiteDataStoreConfigurationCopyNetworkCacheDirectory(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(configuration)->networkCacheDirectory());
}

void WKWebsiteDataStoreConfigurationSetNetworkCacheDirectory(WKWebsiteDataStoreConfigurationRef configuration, WKStringRef directory)
{
    CyberKit::toImpl(configuration)->setNetworkCacheDirectory(CyberKit::toImpl(directory)->string());
}

WKStringRef WKWebsiteDataStoreConfigurationCopyIndexedDBDatabaseDirectory(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(configuration)->indexedDBDatabaseDirectory());
}

void WKWebsiteDataStoreConfigurationSetIndexedDBDatabaseDirectory(WKWebsiteDataStoreConfigurationRef configuration, WKStringRef directory)
{
    CyberKit::toImpl(configuration)->setIndexedDBDatabaseDirectory(CyberKit::toImpl(directory)->string());
}

WKStringRef WKWebsiteDataStoreConfigurationCopyLocalStorageDirectory(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(configuration)->localStorageDirectory());
}

void WKWebsiteDataStoreConfigurationSetLocalStorageDirectory(WKWebsiteDataStoreConfigurationRef configuration, WKStringRef directory)
{
    CyberKit::toImpl(configuration)->setLocalStorageDirectory(CyberKit::toImpl(directory)->string());
}

WKStringRef WKWebsiteDataStoreConfigurationCopyWebSQLDatabaseDirectory(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(configuration)->webSQLDatabaseDirectory());
}

void WKWebsiteDataStoreConfigurationSetWebSQLDatabaseDirectory(WKWebsiteDataStoreConfigurationRef configuration, WKStringRef directory)
{
    CyberKit::toImpl(configuration)->setWebSQLDatabaseDirectory(CyberKit::toImpl(directory)->string());
}

WKStringRef WKWebsiteDataStoreConfigurationCopyCacheStorageDirectory(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(configuration)->cacheStorageDirectory());
}

void WKWebsiteDataStoreConfigurationSetCacheStorageDirectory(WKWebsiteDataStoreConfigurationRef configuration, WKStringRef directory)
{
    CyberKit::toImpl(configuration)->setCacheStorageDirectory(CyberKit::toImpl(directory)->string());
}

WKStringRef WKWebsiteDataStoreConfigurationCopyGeneralStorageDirectory(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(configuration)->generalStorageDirectory());
}

void WKWebsiteDataStoreConfigurationSetGeneralStorageDirectory(WKWebsiteDataStoreConfigurationRef configuration, WKStringRef directory)
{
    CyberKit::toImpl(configuration)->setGeneralStorageDirectory(CyberKit::toImpl(directory)->string());
}

WKStringRef WKWebsiteDataStoreConfigurationCopyMediaKeysStorageDirectory(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(configuration)->mediaKeysStorageDirectory());
}

void WKWebsiteDataStoreConfigurationSetMediaKeysStorageDirectory(WKWebsiteDataStoreConfigurationRef configuration, WKStringRef directory)
{
    CyberKit::toImpl(configuration)->setMediaKeysStorageDirectory(CyberKit::toImpl(directory)->string());
}

WKStringRef WKWebsiteDataStoreConfigurationCopyResourceLoadStatisticsDirectory(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(configuration)->resourceLoadStatisticsDirectory());
}

void WKWebsiteDataStoreConfigurationSetResourceLoadStatisticsDirectory(WKWebsiteDataStoreConfigurationRef configuration, WKStringRef directory)
{
    CyberKit::toImpl(configuration)->setResourceLoadStatisticsDirectory(CyberKit::toImpl(directory)->string());
}

WKStringRef WKWebsiteDataStoreConfigurationCopyServiceWorkerRegistrationDirectory(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(configuration)->serviceWorkerRegistrationDirectory());
}

void WKWebsiteDataStoreConfigurationSetServiceWorkerRegistrationDirectory(WKWebsiteDataStoreConfigurationRef configuration, WKStringRef directory)
{
    CyberKit::toImpl(configuration)->setServiceWorkerRegistrationDirectory(CyberKit::toImpl(directory)->string());
}

WKStringRef WKWebsiteDataStoreConfigurationCopyCookieStorageFile(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(configuration)->cookieStorageFile());
}

void WKWebsiteDataStoreConfigurationSetCookieStorageFile(WKWebsiteDataStoreConfigurationRef configuration, WKStringRef cookieStorageFile)
{
    CyberKit::toImpl(configuration)->setCookieStorageFile(CyberKit::toImpl(cookieStorageFile)->string());
}

uint64_t WKWebsiteDataStoreConfigurationGetPerOriginStorageQuota(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toImpl(configuration)->perOriginStorageQuota();
}

void WKWebsiteDataStoreConfigurationSetPerOriginStorageQuota(WKWebsiteDataStoreConfigurationRef configuration, uint64_t quota)
{
    CyberKit::toImpl(configuration)->setPerOriginStorageQuota(quota);
}

bool WKWebsiteDataStoreConfigurationGetNetworkCacheSpeculativeValidationEnabled(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toImpl(configuration)->networkCacheSpeculativeValidationEnabled();
}

void WKWebsiteDataStoreConfigurationSetNetworkCacheSpeculativeValidationEnabled(WKWebsiteDataStoreConfigurationRef configuration, bool enabled)
{
    CyberKit::toImpl(configuration)->setNetworkCacheSpeculativeValidationEnabled(enabled);
}

bool WKWebsiteDataStoreConfigurationGetTestingSessionEnabled(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toImpl(configuration)->testingSessionEnabled();
}

void WKWebsiteDataStoreConfigurationSetTestingSessionEnabled(WKWebsiteDataStoreConfigurationRef configuration, bool enabled)
{
    CyberKit::toImpl(configuration)->setTestingSessionEnabled(enabled);
}

bool WKWebsiteDataStoreConfigurationGetStaleWhileRevalidateEnabled(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toImpl(configuration)->staleWhileRevalidateEnabled();
}

void WKWebsiteDataStoreConfigurationSetStaleWhileRevalidateEnabled(WKWebsiteDataStoreConfigurationRef configuration, bool enabled)
{
    CyberKit::toImpl(configuration)->setStaleWhileRevalidateEnabled(enabled);
}

WKStringRef WKWebsiteDataStoreConfigurationCopyPCMMachServiceName(WKWebsiteDataStoreConfigurationRef configuration)
{
    return CyberKit::toCopiedAPI(CyberKit::toImpl(configuration)->pcmMachServiceName());
}

void WKWebsiteDataStoreConfigurationSetPCMMachServiceName(WKWebsiteDataStoreConfigurationRef configuration, WKStringRef name)
{
    CyberKit::toImpl(configuration)->setPCMMachServiceName(name ? CyberKit::toImpl(name)->string() : String());
}
