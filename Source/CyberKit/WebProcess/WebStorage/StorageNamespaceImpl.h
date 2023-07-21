/*
 * Copyright (C) 2013-2017 Apple Inc. All rights reserved.
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

#include <CyberCore/SecurityOriginData.h>
#include <CyberCore/SecurityOriginHash.h>
#include <CyberCore/StorageArea.h>
#include <CyberCore/StorageMap.h>
#include <CyberCore/StorageNamespace.h>
#include <wtf/HashMap.h>

namespace CyberKit {

class StorageAreaMap;
class WebPage;

class StorageNamespaceImpl : public CyberCore::StorageNamespace {
public:
    static Ref<StorageNamespaceImpl> createSessionStorageNamespace(uint64_t identifier, unsigned quotaInBytes);
    static Ref<StorageNamespaceImpl> createEphemeralLocalStorageNamespace(uint64_t identifier, unsigned quotaInBytes);

    enum class IsEphemeral : bool { No, Yes };
    static Ref<StorageNamespaceImpl> createLocalStorageNamespace(uint64_t identifier, unsigned quotaInBytes, IsEphemeral isEphemeral);
    static Ref<StorageNamespaceImpl> createTransientLocalStorageNamespace(uint64_t identifier, CyberCore::SecurityOrigin& topLevelOrigin, uint64_t quotaInBytes);

    virtual ~StorageNamespaceImpl();

    CyberCore::StorageType storageType() const { return m_storageType; }
    uint64_t storageNamespaceID() const { return m_storageNamespaceID; }
    CyberCore::SecurityOrigin* topLevelOrigin() const { return m_topLevelOrigin.get(); }
    unsigned quotaInBytes() const { return m_quotaInBytes; }

    void didDestroyStorageAreaMap(StorageAreaMap&);

private:
    explicit StorageNamespaceImpl(CyberCore::StorageType, uint64_t storageNamespaceID, CyberCore::SecurityOrigin* topLevelOrigin, unsigned quotaInBytes);

    Ref<CyberCore::StorageArea> storageArea(const CyberCore::SecurityOriginData&) override;
    Ref<CyberCore::StorageNamespace> copy(CyberCore::Page*) override;

    const CyberCore::StorageType m_storageType;
    const uint64_t m_storageNamespaceID;

    // Only used for transient local storage namespaces.
    const RefPtr<CyberCore::SecurityOrigin> m_topLevelOrigin;

    const unsigned m_quotaInBytes;

    HashMap<CyberCore::SecurityOriginData, StorageAreaMap*> m_storageAreaMaps;
};

} // namespace CyberKit
