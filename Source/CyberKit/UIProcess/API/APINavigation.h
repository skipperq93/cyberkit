/*
 * Copyright (C) 2015-2021 Apple Inc. All rights reserved.
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

#include "APIObject.h"
#include "APIWebsitePolicies.h"
#include "DataReference.h"
#include "FrameInfoData.h"
#include "NavigationActionData.h"
#include "ProcessThrottler.h"
#include "WebBackForwardListItem.h"
#include "WebContentMode.h"
#include <CyberCore/PrivateClickMeasurement.h>
#include <CyberCore/ProcessIdentifier.h>
#include <CyberCore/ResourceRequest.h>
#include <CyberCore/SecurityOriginData.h>
#include <CyberCore/SubstituteData.h>
#include <wtf/Ref.h>

namespace CyberCore {
enum class FrameLoadType : uint8_t;
class ResourceResponse;
}

namespace CyberKit {
class WebNavigationState;
}

namespace API {

struct SubstituteData {
    WTF_MAKE_STRUCT_FAST_ALLOCATED;

    SubstituteData(Vector<uint8_t>&& content, const WTF::String& MIMEType, const WTF::String& encoding, const WTF::String& baseURL, API::Object* userData, CyberCore::SubstituteData::SessionHistoryVisibility sessionHistoryVisibility = CyberCore::SubstituteData::SessionHistoryVisibility::Hidden)
        : content(WTFMove(content))
        , MIMEType(MIMEType)
        , encoding(encoding)
        , baseURL(baseURL)
        , userData(userData)
        , sessionHistoryVisibility(sessionHistoryVisibility)
    { }

    SubstituteData(Vector<uint8_t>&& content, const CyberCore::ResourceResponse&, CyberCore::SubstituteData::SessionHistoryVisibility);

    Vector<uint8_t> content;
    WTF::String MIMEType;
    WTF::String encoding;
    WTF::String baseURL;
    RefPtr<API::Object> userData;
    CyberCore::SubstituteData::SessionHistoryVisibility sessionHistoryVisibility { CyberCore::SubstituteData::SessionHistoryVisibility::Hidden };
};

class Navigation : public ObjectImpl<Object::Type::Navigation> {
    WTF_MAKE_NONCOPYABLE(Navigation);
public:
    static Ref<Navigation> create(CyberKit::WebNavigationState& state, CyberKit::WebBackForwardListItem* currentAndTargetItem)
    {
        return adoptRef(*new Navigation(state, currentAndTargetItem));
    }

    static Ref<Navigation> create(CyberKit::WebNavigationState& state, CyberKit::WebBackForwardListItem& targetItem, CyberKit::WebBackForwardListItem* fromItem, CyberCore::FrameLoadType backForwardFrameLoadType)
    {
        return adoptRef(*new Navigation(state, targetItem, fromItem, backForwardFrameLoadType));
    }

    static Ref<Navigation> create(CyberKit::WebNavigationState& state, CyberCore::ResourceRequest&& request, CyberKit::WebBackForwardListItem* fromItem)
    {
        return adoptRef(*new Navigation(state, WTFMove(request), fromItem));
    }

    static Ref<Navigation> create(CyberKit::WebNavigationState& state, std::unique_ptr<SubstituteData>&& substituteData)
    {
        return adoptRef(*new Navigation(state, WTFMove(substituteData)));
    }

    static Ref<Navigation> create(CyberKit::WebNavigationState& state, CyberCore::ResourceRequest&& simulatedRequest, std::unique_ptr<SubstituteData>&& substituteData, CyberKit::WebBackForwardListItem* fromItem)
    {
        return adoptRef(*new Navigation(state, WTFMove(simulatedRequest), WTFMove(substituteData), fromItem));
    }

    virtual ~Navigation();

    uint64_t navigationID() const { return m_navigationID; }

    const CyberCore::ResourceRequest& originalRequest() const { return m_originalRequest; }
    void setCurrentRequest(CyberCore::ResourceRequest&&, CyberCore::ProcessIdentifier);
    const CyberCore::ResourceRequest& currentRequest() const { return m_currentRequest; }
    std::optional<CyberCore::ProcessIdentifier> currentRequestProcessIdentifier() const { return m_currentRequestProcessIdentifier; }

    bool currentRequestIsRedirect() const { return m_lastNavigationAction.isRedirect; }

    CyberKit::WebBackForwardListItem* targetItem() const { return m_targetItem.get(); }
    CyberKit::WebBackForwardListItem* fromItem() const { return m_fromItem.get(); }
    std::optional<CyberCore::FrameLoadType> backForwardFrameLoadType() const { return m_backForwardFrameLoadType; }
    CyberKit::WebBackForwardListItem* reloadItem() const { return m_reloadItem.get(); }

    void appendRedirectionURL(const WTF::URL&);
    Vector<WTF::URL> takeRedirectChain() { return WTFMove(m_redirectChain); }

    bool wasUserInitiated() const { return !!m_lastNavigationAction.userGestureTokenIdentifier; }

    bool shouldPerformDownload() const { return !m_lastNavigationAction.downloadAttribute.isNull(); }

    bool isSystemPreview() const
    {
#if USE(SYSTEM_PREVIEW)
        return currentRequest().isSystemPreview();
#else
        return false;
#endif
    }

    bool treatAsSameOriginNavigation() const { return m_lastNavigationAction.treatAsSameOriginNavigation; }
    bool hasOpenedFrames() const { return m_lastNavigationAction.hasOpenedFrames; }
    bool openedByDOMWithOpener() const { return m_lastNavigationAction.openedByDOMWithOpener; }
    const CyberCore::SecurityOriginData& requesterOrigin() const { return m_lastNavigationAction.requesterOrigin; }

    void setUserContentExtensionsEnabled(bool enabled) { m_userContentExtensionsEnabled = enabled; }
    bool userContentExtensionsEnabled() const { return m_userContentExtensionsEnabled; }

    CyberCore::LockHistory lockHistory() const { return m_lastNavigationAction.lockHistory; }
    CyberCore::LockBackForwardList lockBackForwardList() const { return m_lastNavigationAction.lockBackForwardList; }

    WTF::String clientRedirectSourceForHistory() const { return m_lastNavigationAction.clientRedirectSourceForHistory; }
    CyberCore::SandboxFlags effectiveSandboxFlags() const { return m_lastNavigationAction.effectiveSandboxFlags; }

    void setLastNavigationAction(const CyberKit::NavigationActionData& navigationAction) { m_lastNavigationAction = navigationAction; }
    const CyberKit::NavigationActionData& lastNavigationAction() const { return m_lastNavigationAction; }

    void setOriginatingFrameInfo(const CyberKit::FrameInfoData& frameInfo) { m_originatingFrameInfo = frameInfo; }
    const CyberKit::FrameInfoData& originatingFrameInfo() const { return m_originatingFrameInfo; }

    void setDestinationFrameSecurityOrigin(const CyberCore::SecurityOriginData& origin) { m_destinationFrameSecurityOrigin = origin; }
    const CyberCore::SecurityOriginData& destinationFrameSecurityOrigin() const { return m_destinationFrameSecurityOrigin; }

    void setEffectiveContentMode(CyberKit::WebContentMode mode) { m_effectiveContentMode = mode; }
    CyberKit::WebContentMode effectiveContentMode() const { return m_effectiveContentMode; }

#if !LOG_DISABLED
    const char* loggingString() const;
#endif

    const std::unique_ptr<SubstituteData>& substituteData() const { return m_substituteData; }

    const std::optional<CyberCore::PrivateClickMeasurement>& privateClickMeasurement() const { return m_lastNavigationAction.privateClickMeasurement; }

    void setClientNavigationActivity(CyberKit::ProcessThrottler::ActivityVariant&& activity) { m_clientNavigationActivity = WTFMove(activity); }

    void setIsLoadedWithNavigationShared(bool value) { m_isLoadedWithNavigationShared = value; }
    bool isLoadedWithNavigationShared() const { return m_isLoadedWithNavigationShared; }

    void setWebsitePolicies(RefPtr<API::WebsitePolicies>&& policies) { m_websitePolicies = WTFMove(policies); }
    API::WebsitePolicies* websitePolicies() { return m_websitePolicies.get(); }

private:
    explicit Navigation(CyberKit::WebNavigationState&);
    Navigation(CyberKit::WebNavigationState&, CyberKit::WebBackForwardListItem*);
    Navigation(CyberKit::WebNavigationState&, CyberCore::ResourceRequest&&, CyberKit::WebBackForwardListItem* fromItem);
    Navigation(CyberKit::WebNavigationState&, CyberKit::WebBackForwardListItem& targetItem, CyberKit::WebBackForwardListItem* fromItem, CyberCore::FrameLoadType);
    Navigation(CyberKit::WebNavigationState&, std::unique_ptr<SubstituteData>&&);
    Navigation(CyberKit::WebNavigationState&, CyberCore::ResourceRequest&&, std::unique_ptr<SubstituteData>&&, CyberKit::WebBackForwardListItem* fromItem);

    uint64_t m_navigationID;
    CyberCore::ResourceRequest m_originalRequest;
    CyberCore::ResourceRequest m_currentRequest;
    std::optional<CyberCore::ProcessIdentifier> m_currentRequestProcessIdentifier;
    Vector<WTF::URL> m_redirectChain;

    RefPtr<CyberKit::WebBackForwardListItem> m_targetItem;
    RefPtr<CyberKit::WebBackForwardListItem> m_fromItem;
    RefPtr<CyberKit::WebBackForwardListItem> m_reloadItem;
    std::optional<CyberCore::FrameLoadType> m_backForwardFrameLoadType;
    std::unique_ptr<SubstituteData> m_substituteData;
    CyberKit::NavigationActionData m_lastNavigationAction;
    CyberKit::FrameInfoData m_originatingFrameInfo;
    CyberCore::SecurityOriginData m_destinationFrameSecurityOrigin;
    bool m_userContentExtensionsEnabled { true };
    CyberKit::WebContentMode m_effectiveContentMode { CyberKit::WebContentMode::Recommended };
    CyberKit::ProcessThrottler::TimedActivity m_clientNavigationActivity;
    bool m_isLoadedWithNavigationShared { false };
    RefPtr<API::WebsitePolicies> m_websitePolicies;
};

} // namespace API
