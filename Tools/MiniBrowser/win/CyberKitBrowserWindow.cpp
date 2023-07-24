/*
 * Copyright (C) 2018 Sony Interactive Entertainment Inc.
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
#include "stdafx.h"
#include "CyberKitBrowserWindow.h"

#include "MiniBrowserLibResource.h"
#include "common.h"
#include <CyberCore/GDIUtilities.h>
#include <CyberKit/WKAuthenticationChallenge.h>
#include <CyberKit/WKAuthenticationDecisionListener.h>
#include <CyberKit/WKCertificateInfoCurl.h>
#include <CyberKit/WKCredential.h>
#include <CyberKit/WKInspector.h>
#include <CyberKit/WKPreferencesRefPrivate.h>
#include <CyberKit/WKProtectionSpace.h>
#include <CyberKit/WKProtectionSpaceCurl.h>
#include <CyberKit/WKWebsiteDataStoreRef.h>
#include <CyberKit/WKWebsiteDataStoreRefCurl.h>
#include <vector>

std::wstring createString(WKStringRef wkString)
{
    size_t maxSize = WKStringGetLength(wkString);

    std::vector<WKChar> wkCharBuffer(maxSize);
    size_t actualLength = WKStringGetCharacters(wkString, wkCharBuffer.data(), maxSize);
    return std::wstring(wkCharBuffer.data(), actualLength);
}

std::wstring createString(WKURLRef wkURL)
{
    if (!wkURL)
        return { };
    WKRetainPtr<WKStringRef> url = adoptWK(WKURLCopyString(wkURL));
    return createString(url.get());
}

std::string createUTF8String(const wchar_t* src, size_t srcLength)
{
    int length = WideCharToMultiByte(CP_UTF8, 0, src, srcLength, 0, 0, nullptr, nullptr);
    std::vector<char> buffer(length);
    size_t actualLength = WideCharToMultiByte(CP_UTF8, 0, src, srcLength, buffer.data(), length, nullptr, nullptr);
    return { buffer.data(), actualLength };
}

std::wstring createPEMString(WKCertificateInfoRef certificateInfo)
{
    auto chainSize = WKCertificateInfoGetCertificateChainSize(certificateInfo);

    std::wstring pems;

    for (auto i = 0; i < chainSize; i++) {
        auto certificate = adoptWK(WKCertificateInfoCopyCertificateAtIndex(certificateInfo, i));
        auto size = WKDataGetSize(certificate.get());
        auto data = WKDataGetBytes(certificate.get());

        for (size_t i = 0; i < size; i++)
            pems.push_back(data[i]);
    }

    return replaceString(pems, L"\n", L"\r\n");
}

WKRetainPtr<WKStringRef> createWKString(_bstr_t str)
{
    auto utf8 = createUTF8String(str, str.length());
    return adoptWK(WKStringCreateWithUTF8CString(utf8.data()));
}

WKRetainPtr<WKStringRef> createWKString(const std::wstring& str)
{
    auto utf8 = createUTF8String(str.c_str(), str.length());
    return adoptWK(WKStringCreateWithUTF8CString(utf8.data()));
}

WKRetainPtr<WKURLRef> createWKURL(_bstr_t str)
{
    auto utf8 = createUTF8String(str, str.length());
    return adoptWK(WKURLCreateWithUTF8CString(utf8.data()));
}

WKRetainPtr<WKURLRef> createWKURL(const std::wstring& str)
{
    auto utf8 = createUTF8String(str.c_str(), str.length());
    return adoptWK(WKURLCreateWithUTF8CString(utf8.data()));
}

Ref<BrowserWindow> CyberKitBrowserWindow::create(BrowserWindowClient& client, HWND mainWnd, bool)
{
    auto conf = adoptWK(WKPageConfigurationCreate());

    auto prefs = adoptWK(WKPreferencesCreate());

    auto pageGroup = adoptWK(WKPageGroupCreateWithIdentifier(createWKString("WinMiniBrowser").get()));
    WKPageConfigurationSetPageGroup(conf.get(), pageGroup.get());
    WKPageGroupSetPreferences(pageGroup.get(), prefs.get());

    WKPreferencesSetMediaCapabilitiesEnabled(prefs.get(), false);
    WKPreferencesSetDeveloperExtrasEnabled(prefs.get(), true);
    WKPageConfigurationSetPreferences(conf.get(), prefs.get());

    auto context =adoptWK(WKContextCreateWithConfiguration(nullptr));
    WKPageConfigurationSetContext(conf.get(), context.get());

    return adoptRef(*new CyberKitBrowserWindow(client, conf.get(), mainWnd));
}

CyberKitBrowserWindow::CyberKitBrowserWindow(BrowserWindowClient& client, WKPageConfigurationRef conf, HWND mainWnd)
    : m_client(client)
    , m_hMainWnd(mainWnd)
{
    RECT rect = { };
    m_view = adoptWK(WKViewCreate(rect, conf, mainWnd));
    WKViewSetIsInWindow(m_view.get(), true);

    auto page = WKViewGetPage(m_view.get());

    WKPageNavigationClientV0 navigationClient = { };
    navigationClient.base.version = 0;
    navigationClient.base.clientInfo = this;
    navigationClient.didReceiveAuthenticationChallenge = didReceiveAuthenticationChallenge;
    WKPageSetPageNavigationClient(page, &navigationClient.base);

    WKPageUIClientV13 uiClient = { };
    uiClient.base.version = 13;
    uiClient.base.clientInfo = this;
    uiClient.createNewPage = createNewPage;
    uiClient.didNotHandleKeyEvent = didNotHandleKeyEvent;
    WKPageSetPageUIClient(page, &uiClient.base);

    WKPageStateClientV0 stateClient = { };
    stateClient.base.version = 0;
    stateClient.base.clientInfo = this;
    stateClient.didChangeTitle = didChangeTitle;
    stateClient.didChangeIsLoading = didChangeIsLoading;
    stateClient.didChangeEstimatedProgress = didChangeEstimatedProgress;
    stateClient.didChangeActiveURL = didChangeActiveURL;
    WKPageSetPageStateClient(page, &stateClient.base);

    updateProxySettings();
    resetZoom();
}

void CyberKitBrowserWindow::updateProxySettings()
{
    auto context = WKPageGetContext(WKViewGetPage(m_view.get()));
    auto store = WKWebsiteDataStoreGetDefaultDataStore();

    if (!m_proxy.enable) {
        WKWebsiteDataStoreDisableNetworkProxySettings(store);
        return;
    }

    if (!m_proxy.custom) {
        WKWebsiteDataStoreEnableDefaultNetworkProxySettings(store);
        return;
    }

    auto url = createWKURL(m_proxy.url);
    auto excludeHosts = createWKString(m_proxy.excludeHosts);
    WKWebsiteDataStoreEnableCustomNetworkProxySettings(store, url.get(), excludeHosts.get());
}

HRESULT CyberKitBrowserWindow::init()
{
    return S_OK;
}

HWND CyberKitBrowserWindow::hwnd()
{
    return WKViewGetWindow(m_view.get());
}

HRESULT CyberKitBrowserWindow::loadURL(const BSTR& url)
{
    auto page = WKViewGetPage(m_view.get());
    WKPageLoadURL(page, createWKURL(_bstr_t(url)).get());
    return true;
}

void CyberKitBrowserWindow::reload()
{
    auto page = WKViewGetPage(m_view.get());
    WKPageReload(page);
}

void CyberKitBrowserWindow::navigateForwardOrBackward(bool forward)
{
    auto page = WKViewGetPage(m_view.get());
    if (forward)
        WKPageGoForward(page);
    else
        WKPageGoBack(page);
}

void CyberKitBrowserWindow::navigateToHistory(UINT menuID)
{
    // Not implemented
}

void CyberKitBrowserWindow::setPreference(UINT menuID, bool enable)
{
    auto page = WKViewGetPage(m_view.get());
    auto pgroup = WKPageGetPageGroup(page);
    auto pref = WKPageGroupGetPreferences(pgroup);
    switch (menuID) {
    case IDM_ACC_COMPOSITING:
        WKPreferencesSetAcceleratedCompositingEnabled(pref, enable);
        break;
    case IDM_COMPOSITING_BORDERS:
        WKPreferencesSetCompositingBordersVisible(pref, enable);
        WKPreferencesSetCompositingRepaintCountersVisible(pref, enable);
        break;
    case IDM_DEBUG_INFO_LAYER:
        WKPreferencesSetTiledScrollingIndicatorVisible(pref, enable);
        break;
    case IDM_DISABLE_IMAGES:
        WKPreferencesSetLoadsImagesAutomatically(pref, !enable);
        break;
    case IDM_DISABLE_JAVASCRIPT:
        WKPreferencesSetJavaScriptEnabled(pref, !enable);
        break;
    }
}

void CyberKitBrowserWindow::print()
{
    // Not implemented
}

void CyberKitBrowserWindow::launchInspector()
{
    auto page = WKViewGetPage(m_view.get());
    auto inspector = WKPageGetInspector(page);
    WKInspectorShow(inspector);
}

void CyberKitBrowserWindow::openProxySettings()
{
    if (askProxySettings(m_hMainWnd, m_proxy))
        updateProxySettings();

}

void CyberKitBrowserWindow::setUserAgent(_bstr_t& customUAString)
{
    auto page = WKViewGetPage(m_view.get());
    auto ua = createWKString(customUAString);
    WKPageSetCustomUserAgent(page, ua.get());
}

_bstr_t CyberKitBrowserWindow::userAgent()
{
    auto page = WKViewGetPage(m_view.get());
    auto ua = adoptWK(WKPageCopyUserAgent(page));
    return createString(ua.get()).c_str();
}

void CyberKitBrowserWindow::showLayerTree()
{
    // Not implemented
}

void CyberKitBrowserWindow::updateStatistics(HWND hDlg)
{
    // Not implemented
}


void CyberKitBrowserWindow::resetZoom()
{
    auto page = WKViewGetPage(m_view.get());
    WKPageSetPageZoomFactor(page, CyberCore::deviceScaleFactorForWindow(hwnd()));
}

void CyberKitBrowserWindow::zoomIn()
{
    auto page = WKViewGetPage(m_view.get());
    double s = WKPageGetPageZoomFactor(page);
    WKPageSetPageZoomFactor(page, s * 1.25);
}

void CyberKitBrowserWindow::zoomOut()
{
    auto page = WKViewGetPage(m_view.get());
    double s = WKPageGetPageZoomFactor(page);
    WKPageSetPageZoomFactor(page, s * 0.8);
}

static CyberKitBrowserWindow& toCyberKitBrowserWindow(const void *clientInfo)
{
    return *const_cast<CyberKitBrowserWindow*>(static_cast<const CyberKitBrowserWindow*>(clientInfo));
}

void CyberKitBrowserWindow::didChangeTitle(const void* clientInfo)
{
    auto& thisWindow = toCyberKitBrowserWindow(clientInfo);
    auto page = WKViewGetPage(thisWindow.m_view.get());
    WKRetainPtr<WKStringRef> title = adoptWK(WKPageCopyTitle(page));
    std::wstring titleString = createString(title.get()) + L" [CyberKit]";
    SetWindowText(thisWindow.m_hMainWnd, titleString.c_str());
}

void CyberKitBrowserWindow::didChangeIsLoading(const void* clientInfo)
{
    auto& thisWindow = toCyberKitBrowserWindow(clientInfo);
    thisWindow.m_client.progressFinished();
}

void CyberKitBrowserWindow::didChangeEstimatedProgress(const void* clientInfo)
{
    auto& thisWindow = toCyberKitBrowserWindow(clientInfo);
    auto page = WKViewGetPage(thisWindow.m_view.get());
    thisWindow.m_client.progressChanged(WKPageGetEstimatedProgress(page));
}

void CyberKitBrowserWindow::didChangeActiveURL(const void* clientInfo)
{
    auto& thisWindow = toCyberKitBrowserWindow(clientInfo);
    auto page = WKViewGetPage(thisWindow.m_view.get());
    WKRetainPtr<WKURLRef> url = adoptWK(WKPageCopyActiveURL(page));
    thisWindow.m_client.activeURLChanged(createString(url.get()));
}

void CyberKitBrowserWindow::didReceiveAuthenticationChallenge(WKPageRef page, WKAuthenticationChallengeRef challenge, const void* clientInfo)
{
    auto& thisWindow = toCyberKitBrowserWindow(clientInfo);
    auto protectionSpace = WKAuthenticationChallengeGetProtectionSpace(challenge);
    auto decisionListener = WKAuthenticationChallengeGetDecisionListener(challenge);
    auto authenticationScheme = WKProtectionSpaceGetAuthenticationScheme(protectionSpace);

    if (authenticationScheme == kWKProtectionSpaceAuthenticationSchemeServerTrustEvaluationRequested) {
        if (thisWindow.canTrustServerCertificate(protectionSpace)) {
            WKRetainPtr<WKStringRef> username = createWKString("accept server trust");
            WKRetainPtr<WKStringRef> password = createWKString("");
            WKRetainPtr<WKCredentialRef> wkCredential = adoptWK(WKCredentialCreate(username.get(), password.get(), kWKCredentialPersistenceForSession));
            WKAuthenticationDecisionListenerUseCredential(decisionListener, wkCredential.get());
            return;
        }
    } else {
        WKRetainPtr<WKStringRef> realm(WKProtectionSpaceCopyRealm(protectionSpace));

        if (auto credential = askCredential(thisWindow.hwnd(), createString(realm.get()))) {
            WKRetainPtr<WKStringRef> username = createWKString(credential->username);
            WKRetainPtr<WKStringRef> password = createWKString(credential->password);
            WKRetainPtr<WKCredentialRef> wkCredential = adoptWK(WKCredentialCreate(username.get(), password.get(), kWKCredentialPersistenceForSession));
            WKAuthenticationDecisionListenerUseCredential(decisionListener, wkCredential.get());
            return;
        }
    }

    WKAuthenticationDecisionListenerUseCredential(decisionListener, nullptr);
}

bool CyberKitBrowserWindow::canTrustServerCertificate(WKProtectionSpaceRef protectionSpace)
{
    auto host = createString(adoptWK(WKProtectionSpaceCopyHost(protectionSpace)).get());
    auto certificateInfo = adoptWK(WKProtectionSpaceCopyCertificateInfo(protectionSpace));
    auto verificationError = WKCertificateInfoGetVerificationError(certificateInfo.get());
    auto description = createString(adoptWK(WKCertificateInfoCopyVerificationErrorDescription(certificateInfo.get())).get());
    auto pem = createPEMString(certificateInfo.get());

    auto it = m_acceptedServerTrustCerts.find(host);
    if (it != m_acceptedServerTrustCerts.end() && it->second == pem)
        return true;

    std::wstring textString = L"[HOST] " + host + L"\r\n";
    textString.append(L"[ERROR] " + std::to_wstring(verificationError) + L"\r\n");
    textString.append(L"[DESCRIPTION] " + description + L"\r\n");
    textString.append(pem);

    if (askServerTrustEvaluation(hwnd(), textString)) {
        m_acceptedServerTrustCerts.emplace(host, pem);
        return true;
    }

    return false;
}

WKPageRef CyberKitBrowserWindow::createNewPage(WKPageRef page, WKPageConfigurationRef configuration, WKNavigationActionRef navigationAction, WKWindowFeaturesRef windowFeatures, const void *clientInfo)
{
    auto& newWindow = MainWindow::create().leakRef();
    auto factory = [configuration](BrowserWindowClient& client, HWND mainWnd, bool) -> auto {
        return adoptRef(*new CyberKitBrowserWindow(client, configuration, mainWnd));
    };
    bool ok = newWindow.init(factory, hInst);
    if (!ok)
        return nullptr;
    ShowWindow(newWindow.hwnd(), SW_SHOW);
    auto& newBrowserWindow = *static_cast<CyberKitBrowserWindow*>(newWindow.browserWindow());
    WKRetainPtr<WKPageRef> newPage = WKViewGetPage(newBrowserWindow.m_view.get());
    return newPage.leakRef();
}

void CyberKitBrowserWindow::didNotHandleKeyEvent(WKPageRef, WKNativeEventPtr event, const void* clientInfo)
{
    auto& thisWindow = toCyberKitBrowserWindow(clientInfo);
    PostMessage(thisWindow.m_hMainWnd, event->message, event->wParam, event->lParam);
}
