/*
 * Copyright (C) 2010-2017 Apple Inc. All rights reserved.
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

#include <CyberCore/CookiesStrategy.h>
#include <CyberCore/LoaderStrategy.h>
#include <CyberCore/PlatformStrategies.h>
#include <wtf/Forward.h>

class WebPlatformStrategies : public CyberCore::PlatformStrategies, private CyberCore::CookiesStrategy {
public:
    static void initialize();
    
private:
    friend NeverDestroyed<WebPlatformStrategies>;
    WebPlatformStrategies();

    // CyberCore::PlatformStrategies
    virtual CyberCore::CookiesStrategy* createCookiesStrategy();
    virtual CyberCore::LoaderStrategy* createLoaderStrategy();
    virtual CyberCore::PasteboardStrategy* createPasteboardStrategy();
    virtual CyberCore::BlobRegistry* createBlobRegistry();

    // CyberCore::CookiesStrategy
    std::pair<String, bool> cookiesForDOM(const CyberCore::NetworkStorageSession&, const URL& firstParty, const CyberCore::SameSiteInfo&, const URL&, Optional<uint64_t> frameID, Optional<uint64_t> pageID, CyberCore::IncludeSecureCookies) override;
    virtual void setCookiesFromDOM(const CyberCore::NetworkStorageSession&, const URL& firstParty, const CyberCore::SameSiteInfo&, const URL&, Optional<uint64_t> frameID, Optional<uint64_t> pageID, const String&);
    virtual bool cookiesEnabled(const CyberCore::NetworkStorageSession&);
    std::pair<String, bool> cookieRequestHeaderFieldValue(const CyberCore::NetworkStorageSession&, const URL& firstParty, const CyberCore::SameSiteInfo&, const URL&, Optional<uint64_t> frameID, Optional<uint64_t> pageID, CyberCore::IncludeSecureCookies) override;
    std::pair<String, bool> cookieRequestHeaderFieldValue(PAL::SessionID, const URL& firstParty, const CyberCore::SameSiteInfo&, const URL&, Optional<uint64_t> frameID, Optional<uint64_t> pageID, CyberCore::IncludeSecureCookies) override;
    virtual bool getRawCookies(const CyberCore::NetworkStorageSession&, const URL& firstParty, const CyberCore::SameSiteInfo&, const URL&, Optional<uint64_t> frameID, Optional<uint64_t> pageID, Vector<CyberCore::Cookie>&);
    virtual void deleteCookie(const CyberCore::NetworkStorageSession&, const URL&, const String&);
};
