/*
 * Copyright (C) 2010, 2011 Apple Inc. All rights reserved.
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

#include "WebBackForwardListCounts.h"
#include <CyberCore/BackForwardClient.h>
#include <CyberCore/BackForwardItemIdentifier.h>
#include <CyberCore/PageIdentifier.h>

namespace CyberKit {

class WebPage;

class WebBackForwardListProxy : public CyberCore::BackForwardClient {
public: 
    static Ref<WebBackForwardListProxy> create(WebPage& page) { return adoptRef(*new WebBackForwardListProxy(page)); }

    static CyberCore::HistoryItem* itemForID(const CyberCore::BackForwardItemIdentifier&);
    static void removeItem(const CyberCore::BackForwardItemIdentifier&);

    enum class OverwriteExistingItem : bool { No, Yes };
    void addItemFromUIProcess(const CyberCore::BackForwardItemIdentifier&, Ref<CyberCore::HistoryItem>&&, CyberCore::PageIdentifier, OverwriteExistingItem);

    void clear();

private:
    WebBackForwardListProxy(WebPage&);

    void addItem(Ref<CyberCore::HistoryItem>&&) override;

    void goToItem(CyberCore::HistoryItem&) override;
        
    RefPtr<CyberCore::HistoryItem> itemAtIndex(int) override;
    unsigned backListCount() const override;
    unsigned forwardListCount() const override;
    bool containsItem(const CyberCore::HistoryItem&) const final;
    const WebBackForwardListCounts& cacheListCountsIfNecessary() const;
    void clearCachedListCounts();

    void close() override;

    WebPage* m_page;
    mutable std::optional<WebBackForwardListCounts> m_cachedBackForwardListCounts;
};

} // namespace CyberKit
