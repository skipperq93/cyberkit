/*
 * Copyright (C) 2014-2018 Apple Inc. All rights reserved.
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

#include "WKPage.h"
#include "WebEvent.h"
#include "WebHitTestResultData.h"
#include "WebPageProxy.h"
#include <CyberCore/FloatRect.h>
#include <wtf/CompletionHandler.h>

#if PLATFORM(IOS_FAMILY)
OBJC_CLASS NSArray;
OBJC_CLASS _WKActivatedElementInfo;
OBJC_CLASS UIViewController;
#endif

namespace CyberCore {
class ResourceRequest;
struct FontAttributes;
struct SecurityOriginData;
struct WindowFeatures;
}

namespace CyberKit {
class NativeWebKeyboardEvent;
class NativeWebWheelEvent;
class NotificationPermissionRequest;
class UserMediaPermissionRequestProxy;
class WebColorPickerResultListenerProxy;
class WebFrameProxy;
class WebOpenPanelResultListenerProxy;
class WebPageProxy;
struct NavigationActionData;

#if ENABLE(MEDIA_SESSION)
class WebMediaSessionMetadata;
#endif
}

namespace API {

class Data;
class Dictionary;
class Object;
class OpenPanelParameters;
class SecurityOrigin;

class UIClient {
public:
    virtual ~UIClient() { }

    virtual void createNewPage(CyberKit::WebPageProxy&, Ref<FrameInfo>&&, CyberCore::ResourceRequest&&, CyberCore::WindowFeatures&&, CyberKit::NavigationActionData&&, CompletionHandler<void(RefPtr<CyberKit::WebPageProxy>&&)>&& completionHandler) { completionHandler(nullptr); }
    virtual void showPage(CyberKit::WebPageProxy*) { }
    virtual void fullscreenMayReturnToInline(CyberKit::WebPageProxy*) { }
    virtual void didEnterFullscreen(CyberKit::WebPageProxy*) { }
    virtual void didExitFullscreen(CyberKit::WebPageProxy*) { }
    virtual void hasVideoInPictureInPictureDidChange(CyberKit::WebPageProxy*, bool) { }
    virtual void close(CyberKit::WebPageProxy*) { }

    virtual bool takeFocus(CyberKit::WebPageProxy*, WKFocusDirection) { return false; }
    virtual void focus(CyberKit::WebPageProxy*) { }
    virtual void unfocus(CyberKit::WebPageProxy*) { }

    virtual void runJavaScriptAlert(CyberKit::WebPageProxy*, const WTF::String&, CyberKit::WebFrameProxy*, const CyberCore::SecurityOriginData&, Function<void ()>&& completionHandler) { completionHandler(); }
    virtual void runJavaScriptConfirm(CyberKit::WebPageProxy*, const WTF::String&, CyberKit::WebFrameProxy*, const CyberCore::SecurityOriginData&, Function<void (bool)>&& completionHandler) { completionHandler(false); }
    virtual void runJavaScriptPrompt(CyberKit::WebPageProxy*, const WTF::String&, const WTF::String&, CyberKit::WebFrameProxy*, const CyberCore::SecurityOriginData&, Function<void (const WTF::String&)>&& completionHandler) { completionHandler(WTF::String()); }

    virtual void setStatusText(CyberKit::WebPageProxy*, const WTF::String&) { }
    virtual void mouseDidMoveOverElement(CyberKit::WebPageProxy&, const CyberKit::WebHitTestResultData&, CyberKit::WebEvent::Modifiers, Object*) { }
#if ENABLE(NETSCAPE_PLUGIN_API)
    virtual void unavailablePluginButtonClicked(CyberKit::WebPageProxy&, WKPluginUnavailabilityReason, Dictionary&) { }
#endif // ENABLE(NETSCAPE_PLUGIN_API)

    virtual void didNotHandleKeyEvent(CyberKit::WebPageProxy*, const CyberKit::NativeWebKeyboardEvent&) { }
    virtual void didNotHandleWheelEvent(CyberKit::WebPageProxy*, const CyberKit::NativeWebWheelEvent&) { }

    virtual void toolbarsAreVisible(CyberKit::WebPageProxy&, Function<void(bool)>&& completionHandler) { completionHandler(true); }
    virtual void setToolbarsAreVisible(CyberKit::WebPageProxy&, bool) { }
    virtual void menuBarIsVisible(CyberKit::WebPageProxy&, Function<void(bool)>&& completionHandler) { completionHandler(true); }
    virtual void setMenuBarIsVisible(CyberKit::WebPageProxy&, bool) { }
    virtual void statusBarIsVisible(CyberKit::WebPageProxy&, Function<void(bool)>&& completionHandler) { completionHandler(true); }
    virtual void setStatusBarIsVisible(CyberKit::WebPageProxy&, bool) { }
    virtual void setIsResizable(CyberKit::WebPageProxy&, bool) { }

    virtual void setWindowFrame(CyberKit::WebPageProxy&, const CyberCore::FloatRect&) { }
    virtual void windowFrame(CyberKit::WebPageProxy&, Function<void(CyberCore::FloatRect)>&& completionHandler) { completionHandler({ }); }

    virtual bool canRunBeforeUnloadConfirmPanel() const { return false; }
    virtual void runBeforeUnloadConfirmPanel(CyberKit::WebPageProxy*, const WTF::String&, CyberKit::WebFrameProxy*, const CyberCore::SecurityOriginData&, Function<void (bool)>&& completionHandler) { completionHandler(true); }

    virtual void pageDidScroll(CyberKit::WebPageProxy*) { }

    virtual void exceededDatabaseQuota(CyberKit::WebPageProxy*, CyberKit::WebFrameProxy*, SecurityOrigin*, const WTF::String&, const WTF::String&, unsigned long long currentQuota, unsigned long long, unsigned long long, unsigned long long, Function<void (unsigned long long)>&& completionHandler)
    {
        completionHandler(currentQuota);
    }

    virtual void reachedApplicationCacheOriginQuota(CyberKit::WebPageProxy*, const CyberCore::SecurityOrigin&, uint64_t currentQuota, uint64_t, Function<void (unsigned long long)>&& completionHandler)
    {
        completionHandler(currentQuota);
    }

    virtual bool needsFontAttributes() const { return false; }
    virtual void didChangeFontAttributes(const CyberCore::FontAttributes&) { }

    virtual bool runOpenPanel(CyberKit::WebPageProxy*, CyberKit::WebFrameProxy*, const CyberCore::SecurityOriginData&, OpenPanelParameters*, CyberKit::WebOpenPanelResultListenerProxy*) { return false; }
    virtual void decidePolicyForGeolocationPermissionRequest(CyberKit::WebPageProxy&, CyberKit::WebFrameProxy&, SecurityOrigin&, Function<void(bool)>&) { }
    virtual bool decidePolicyForUserMediaPermissionRequest(CyberKit::WebPageProxy&, CyberKit::WebFrameProxy&, SecurityOrigin&, SecurityOrigin&, CyberKit::UserMediaPermissionRequestProxy&) { return false; }
    virtual bool checkUserMediaPermissionForOrigin(CyberKit::WebPageProxy&, CyberKit::WebFrameProxy&, SecurityOrigin&, SecurityOrigin&, CyberKit::UserMediaPermissionCheckProxy&) { return false; }
    virtual void decidePolicyForNotificationPermissionRequest(CyberKit::WebPageProxy&, SecurityOrigin&, Function<void(bool)>&& completionHandler) { completionHandler(false); }
    virtual void requestStorageAccessConfirm(CyberKit::WebPageProxy&, CyberKit::WebFrameProxy*, const WTF::String& requestingDomain, const WTF::String& currentDomain, CompletionHandler<void(bool)>&& completionHandler) { completionHandler(true); }

    // Printing.
    virtual float headerHeight(CyberKit::WebPageProxy&, CyberKit::WebFrameProxy&) { return 0; }
    virtual float footerHeight(CyberKit::WebPageProxy&, CyberKit::WebFrameProxy&) { return 0; }
    virtual void drawHeader(CyberKit::WebPageProxy&, CyberKit::WebFrameProxy&, CyberCore::FloatRect&&) { }
    virtual void drawFooter(CyberKit::WebPageProxy&, CyberKit::WebFrameProxy&, CyberCore::FloatRect&&) { }
    virtual void printFrame(CyberKit::WebPageProxy&, CyberKit::WebFrameProxy&) { }

    virtual bool canRunModal() const { return false; }
    virtual void runModal(CyberKit::WebPageProxy&) { }

    virtual void saveDataToFileInDownloadsFolder(CyberKit::WebPageProxy*, const WTF::String&, const WTF::String&, const WTF::URL&, Data&) { }

    virtual void pinnedStateDidChange(CyberKit::WebPageProxy&) { }

    virtual void isPlayingMediaDidChange(CyberKit::WebPageProxy&) { }
    virtual void mediaCaptureStateDidChange(CyberCore::MediaProducer::MediaStateFlags) { }
    virtual void handleAutoplayEvent(CyberKit::WebPageProxy&, CyberCore::AutoplayEvent, OptionSet<CyberCore::AutoplayEventFlags>) { }

#if ENABLE(MEDIA_SESSION)
    virtual void mediaSessionMetadataDidChange(CyberKit::WebPageProxy&, CyberKit::WebMediaSessionMetadata*) { }
#endif

#if PLATFORM(IOS_FAMILY)
#if HAVE(APP_LINKS)
    virtual bool shouldIncludeAppLinkActionsForElement(_WKActivatedElementInfo *) { return true; }
#endif
    virtual RetainPtr<NSArray> actionsForElement(_WKActivatedElementInfo *, RetainPtr<NSArray> defaultActions) { return defaultActions; }
    virtual void didNotHandleTapAsClick(const CyberCore::IntPoint&) { }
    virtual UIViewController *presentingViewController() { return nullptr; }
#endif
#if PLATFORM(COCOA)
    virtual NSDictionary *dataDetectionContext() { return nullptr; }
#endif

#if ENABLE(POINTER_LOCK)
    virtual void requestPointerLock(CyberKit::WebPageProxy*) { }
    virtual void didLosePointerLock(CyberKit::WebPageProxy*) { }
#endif

    virtual void didClickAutoFillButton(CyberKit::WebPageProxy&, Object*) { }

    virtual void didResignInputElementStrongPasswordAppearance(CyberKit::WebPageProxy&, Object*) { }

    virtual void imageOrMediaDocumentSizeChanged(const CyberCore::IntSize&) { }

    virtual void didExceedBackgroundResourceLimitWhileInForeground(CyberKit::WebPageProxy&, WKResourceLimit) { }
    
    virtual void didShowSafeBrowsingWarning() { }
    virtual void didClickGoBackFromSafeBrowsingWarning() { }
};

} // namespace API
