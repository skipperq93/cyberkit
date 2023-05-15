/**
 * Copyright (C) 2010-2023 Apple Inc. All rights reserved.
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

#include "ContextMenuContextData.h"
#include "EditorState.h"
#include "GeolocationPermissionRequestManagerProxy.h"
#include "HiddenPageThrottlingAutoIncreasesCounter.h"
#include "LayerTreeContext.h"
#include "PageLoadState.h"
#include "ProcessThrottler.h"
#include "ScrollingAccelerationCurve.h"
#include "VisibleWebPageCounter.h"
#include "WebColorPicker.h"
#include "WebNotificationManagerMessageHandler.h"
#include "WebPageProxy.h"
#include "WebPopupMenuProxy.h"
#include "WebURLSchemeHandlerIdentifier.h"
#include "WindowKind.h"
#include <CyberCore/PrivateClickMeasurement.h>
#include <CyberCore/RegistrableDomain.h>
#include <CyberCore/ResourceRequest.h>
#include <pal/HysteresisActivity.h>

#if ENABLE(APPLE_PAY)
#include "WebPaymentCoordinatorProxy.h"
#endif

#if ENABLE(DATALIST_ELEMENT)
#include "WebDataListSuggestionsDropdown.h"
#endif

#if ENABLE(DRAG_SUPPORT)
#include <CyberCore/DragActions.h>
#endif

#if PLATFORM(MACCATALYST)
#include "EndowmentStateTracker.h"
#endif

#if ENABLE(META_VIEWPORT)
#include <CyberCore/ViewportArguments.h>
#endif

#if ENABLE(SPEECH_SYNTHESIS)
#include <CyberCore/PlatformSpeechSynthesisUtterance.h>
#include <CyberCore/PlatformSpeechSynthesizer.h>
#endif

#if HAVE(TOUCH_BAR)
#include "TouchBarMenuData.h"
#endif

#if ENABLE(TOUCH_EVENTS)
#include "NativeWebTouchEvent.h"
#include <CyberCore/EventTrackingRegions.h>
#endif

#if ENABLE(UI_SIDE_COMPOSITING)
#include "VisibleContentRectUpdateInfo.h"
#endif

#if ENABLE(WIRELESS_PLAYBACK_TARGET) && !PLATFORM(IOS_FAMILY)
#include <CyberCore/WebMediaSessionManagerClient.h>
#endif

namespace CyberKit {

struct PrivateClickMeasurementAndMetadata {
    CyberCore::PrivateClickMeasurement pcm;
    String sourceDescription;
    String purchaser;
};

struct SpeechSynthesisData {
    Ref<CyberCore::PlatformSpeechSynthesizer> synthesizer;
    RefPtr<CyberCore::PlatformSpeechSynthesisUtterance> utterance;
    CompletionHandler<void()> speakingStartedCompletionHandler;
    CompletionHandler<void()> speakingFinishedCompletionHandler;
    CompletionHandler<void()> speakingPausedCompletionHandler;
    CompletionHandler<void()> speakingResumedCompletionHandler;
};

#if ENABLE(TOUCH_EVENTS)

struct QueuedTouchEvents {
    QueuedTouchEvents(const NativeWebTouchEvent& event)
        : forwardedEvent(event)
    {
    }
    NativeWebTouchEvent forwardedEvent;
    Vector<NativeWebTouchEvent> deferredTouchEvents;
};

struct TouchEventTracking {
    CyberCore::TrackingType touchForceChangedTracking { CyberCore::TrackingType::NotTracking };
    CyberCore::TrackingType touchStartTracking { CyberCore::TrackingType::NotTracking };
    CyberCore::TrackingType touchMoveTracking { CyberCore::TrackingType::NotTracking };
    CyberCore::TrackingType touchEndTracking { CyberCore::TrackingType::NotTracking };

    bool isTrackingAnything() const
    {
        return touchForceChangedTracking != CyberCore::TrackingType::NotTracking
            || touchStartTracking != CyberCore::TrackingType::NotTracking
            || touchMoveTracking != CyberCore::TrackingType::NotTracking
            || touchEndTracking != CyberCore::TrackingType::NotTracking;
    }

    void reset()
    {
        touchForceChangedTracking = CyberCore::TrackingType::NotTracking;
        touchStartTracking = CyberCore::TrackingType::NotTracking;
        touchMoveTracking = CyberCore::TrackingType::NotTracking;
        touchEndTracking = CyberCore::TrackingType::NotTracking;
    }
};

#endif

struct WebPageProxy::Internals final : WebPopupMenuProxy::Client
#if ENABLE(APPLE_PAY)
    , WebPaymentCoordinatorProxy::Client
#endif
#if ENABLE(INPUT_TYPE_COLOR)
    , WebColorPickerClient
#endif
#if PLATFORM(MACCATALYST)
    , EndowmentStateTracker::Client
#endif
#if ENABLE(SPEECH_SYNTHESIS)
    , CyberCore::PlatformSpeechSynthesisUtteranceClient
    , CyberCore::PlatformSpeechSynthesizerClient
#endif
#if ENABLE(WIRELESS_PLAYBACK_TARGET) && !PLATFORM(IOS_FAMILY)
    , CyberCore::WebMediaSessionManagerClient
#endif
{
    WTF_MAKE_STRUCT_FAST_ALLOCATED;

    WebPageProxy& page;
    OptionSet<CyberCore::ActivityState> activityState;
    RunLoop::Timer audibleActivityTimer;
    std::optional<CyberCore::Color> backgroundColor;
    CyberCore::LayoutSize baseLayoutViewportSize;
    std::optional<CyberCore::FontAttributes> cachedFontAttributesAtSelectionStart;
    Vector<Function<void()>> callbackHandlersAfterProcessingPendingMouseEvents;
    CyberCore::ResourceRequest decidePolicyForResponseRequest;
    CyberCore::FloatSize defaultUnobscuredSize;
    EditorState editorState;
    CyberCore::IntSize fixedLayoutSize;
    GeolocationPermissionRequestManagerProxy geolocationPermissionRequestManager;
    HiddenPageThrottlingAutoIncreasesCounter::Token hiddenPageDOMTimerThrottlingAutoIncreasesCount;
    Identifier identifier;
    Deque<NativeWebKeyboardEvent> keyEventQueue;
    LayerHostingMode layerHostingMode { LayerHostingMode::InProcess };
    CyberCore::RectEdges<bool> mainFramePinnedState { true, true, true, true };
    CyberCore::LayoutPoint maxStableLayoutViewportOrigin;
    CyberCore::FloatSize maximumUnobscuredSize;
    CyberCore::MediaProducerMediaStateFlags mediaState;
    CyberCore::LayoutPoint minStableLayoutViewportOrigin;
    CyberCore::IntSize minimumSizeForAutoLayout;
    CyberCore::FloatSize minimumUnobscuredSize;
    Deque<NativeWebMouseEvent> mouseEventQueue;
    CyberCore::MediaProducerMutedStateFlags mutedState;
    WebNotificationManagerMessageHandler notificationManagerMessageHandler;
    OptionSet<CyberCore::LayoutMilestone> observedLayoutMilestones;
    CyberCore::Color pageExtendedBackgroundColor;
    UserObservablePageCounter::Token pageIsUserObservableCount;
    std::optional<MonotonicTime> pageLoadStart;
    PageLoadState pageLoadState;
    OptionSet<CyberCore::ActivityState> potentiallyChangedActivityStateFlags;
    ProcessSuppressionDisabledToken preventProcessSuppressionCount;
    std::optional<PrivateClickMeasurementAndMetadata> privateClickMeasurement;
    CyberCore::MediaProducerMediaStateFlags reportedMediaCaptureState;
    RunLoop::Timer resetRecentCrashCountTimer;
    CyberCore::RectEdges<bool> rubberBandableEdges { true, true, true, true };
    CyberCore::Color sampledPageTopColor;
    CyberCore::ScrollPinningBehavior scrollPinningBehavior { CyberCore::ScrollPinningBehavior::DoNotPin };
    CyberCore::IntSize sizeToContentAutoSizeMaximumSize;
    CyberCore::Color themeColor;
    RunLoop::Timer tryCloseTimeoutTimer;
    CyberCore::Color underPageBackgroundColorOverride;
    CyberCore::Color underlayColor;
    RunLoop::Timer updateReportedMediaCaptureStateTimer;
    HashMap<WebURLSchemeHandlerIdentifier, Ref<WebURLSchemeHandler>> urlSchemeHandlersByIdentifier;
    std::optional<CyberCore::FloatRect> viewExposedRect;
    std::optional<CyberCore::FloatSize> viewportSizeForCSSViewportUnits;
    VisibleWebPageToken visiblePageToken;
    CyberCore::IntRect visibleScrollerThumbRect;
    CyberCore::PageIdentifier webPageID;
    WindowKind windowKind { WindowKind::Unparented };

    HashMap<CyberCore::RegistrableDomain, UniqueRef<SubframePageProxy>> domainToSubframePageProxyMap;
    HashMap<CyberCore::FrameIdentifier, CyberCore::RegistrableDomain> frameIdentifierToDomainMap;

#if ENABLE(APPLE_PAY)
    std::unique_ptr<WebPaymentCoordinatorProxy> paymentCoordinator;
#endif

#if PLATFORM(COCOA)
    WeakObjCPtr<WKWebView> cocoaView;
    TransactionID firstLayerTreeTransactionIdAfterDidCommitLoad;
#endif

#if ENABLE(CONTEXT_MENUS)
    ContextMenuContextData activeContextMenuContextData;
#endif

#if HAVE(CVDISPLAYLINK)
    PAL::HysteresisActivity wheelEventActivityHysteresis;
#endif

#if ENABLE(DATALIST_ELEMENT)
    RefPtr<WebDataListSuggestionsDropdown> dataListSuggestionsDropdown;
#endif

#if ENABLE(DRAG_SUPPORT)
    CyberCore::IntRect currentDragCaretEditableElementRect;
    CyberCore::IntRect currentDragCaretRect;
    CyberCore::DragHandlingMethod currentDragHandlingMethod { CyberCore::DragHandlingMethod::None };
#endif

#if ENABLE(INPUT_TYPE_COLOR)
    RefPtr<WebColorPicker> colorPicker;
#endif

#if ENABLE(MAC_GESTURE_EVENTS)
    Deque<NativeWebGestureEvent> gestureEventQueue;
#endif

#if ENABLE(META_VIEWPORT)
    std::optional<CyberCore::ViewportArguments> overrideViewportArguments;
    CyberCore::FloatSize viewportConfigurationViewLayoutSize;
#endif

#if ENABLE(MOMENTUM_EVENT_DISPATCHER)
    std::optional<ScrollingAccelerationCurve> lastSentScrollingAccelerationCurve;
    std::optional<ScrollingAccelerationCurve> scrollingAccelerationCurve;
#endif

#if ENABLE(NOTIFICATIONS)
    HashSet<CyberCore::SecurityOriginData> notificationPermissionRequesters;
#endif

#if ENABLE(SERVICE_WORKER)
    CompletionHandler<void(bool)> serviceWorkerLaunchCompletionHandler;
    CompletionHandler<void(std::optional<CyberCore::PageIdentifier>)> serviceWorkerOpenWindowCompletionCallback;
#endif

#if ENABLE(SPEECH_SYNTHESIS)
    std::optional<SpeechSynthesisData> optionalSpeechSynthesisData;
#endif

#if HAVE(TOUCH_BAR)
    TouchBarMenuData touchBarMenuData;
#endif

#if ENABLE(TOUCH_EVENTS)
    TouchEventTracking touchEventTracking;
#endif

#if ENABLE(TOUCH_EVENTS) && !ENABLE(IOS_TOUCH_EVENTS)
    Deque<QueuedTouchEvents> touchEventQueue;
#endif

#if ENABLE(TRACKING_PREVENTION)
    MonotonicTime didFinishDocumentLoadForMainFrameTimestamp;
#endif

#if ENABLE(UI_SIDE_COMPOSITING)
    VisibleContentRectUpdateInfo lastVisibleContentRectUpdate;
#endif

#if ENABLE(VIDEO_PRESENTATION_MODE)
    RunLoop::Timer fullscreenVideoTextRecognitionTimer;
    std::optional<PlaybackSessionContextIdentifier> currentFullscreenVideoSessionIdentifier;
#endif

#if ENABLE(WEBXR) && !USE(OPENXR)
    std::unique_ptr<PlatformXRSystem> xrSystem;
#endif

    explicit Internals(WebPageProxy&);

    SpeechSynthesisData& speechSynthesisData();

    // WebPopupMenuProxy::Client
    void valueChangedForPopupMenu(WebPopupMenuProxy*, int32_t newSelectedIndex) final;
    void setTextFromItemForPopupMenu(WebPopupMenuProxy*, int32_t index) final;
    NativeWebMouseEvent* currentlyProcessedMouseDownEvent() final;
#if PLATFORM(GTK)
    void failedToShowPopupMenu() final;
#endif

#if ENABLE(APPLE_PAY)
    // WebPaymentCoordinatorProxy::Client
    IPC::Connection* paymentCoordinatorConnection(const WebPaymentCoordinatorProxy&) final;
    const String& paymentCoordinatorBoundInterfaceIdentifier(const WebPaymentCoordinatorProxy&) final;
    const String& paymentCoordinatorSourceApplicationBundleIdentifier(const WebPaymentCoordinatorProxy&) final;
    const String& paymentCoordinatorSourceApplicationSecondaryIdentifier(const WebPaymentCoordinatorProxy&) final;
    void paymentCoordinatorAddMessageReceiver(WebPaymentCoordinatorProxy&, IPC::ReceiverName, IPC::MessageReceiver&) final;
    void paymentCoordinatorRemoveMessageReceiver(WebPaymentCoordinatorProxy&, IPC::ReceiverName) final;
#endif
#if ENABLE(APPLE_PAY) && PLATFORM(IOS_FAMILY)
    UIViewController *paymentCoordinatorPresentingViewController(const WebPaymentCoordinatorProxy&) final;
    const String& paymentCoordinatorCTDataConnectionServiceType(const WebPaymentCoordinatorProxy&) final;
    std::unique_ptr<PaymentAuthorizationPresenter> paymentCoordinatorAuthorizationPresenter(WebPaymentCoordinatorProxy&, PKPaymentRequest *) final;
#endif
#if ENABLE(APPLE_PAY) && PLATFORM(IOS_FAMILY) && ENABLE(APPLE_PAY_REMOTE_UI_USES_SCENE)
    void getWindowSceneIdentifierForPaymentPresentation(WebPageProxyIdentifier, CompletionHandler<void(const String&)>&&) final;
#endif
#if ENABLE(APPLE_PAY) && PLATFORM(MAC)
    NSWindow *paymentCoordinatorPresentingWindow(const WebPaymentCoordinatorProxy&) final;
#endif

#if ENABLE(INPUT_TYPE_COLOR)
    // WebColorPickerClient
    void didChooseColor(const CyberCore::Color&) final;
    void didEndColorPicker() final;
#endif

#if PLATFORM(MACCATALYST)
    // EndowmentStateTracker::Client
    void isUserFacingChanged(bool) final;
#endif

#if ENABLE(SPEECH_SYNTHESIS)
    // PlatformSpeechSynthesisUtteranceClient
    void didStartSpeaking(CyberCore::PlatformSpeechSynthesisUtterance&) final;
    void didFinishSpeaking(CyberCore::PlatformSpeechSynthesisUtterance&) final;
    void didPauseSpeaking(CyberCore::PlatformSpeechSynthesisUtterance&) final;
    void didResumeSpeaking(CyberCore::PlatformSpeechSynthesisUtterance&) final;
    void speakingErrorOccurred(CyberCore::PlatformSpeechSynthesisUtterance&) final;
    void boundaryEventOccurred(CyberCore::PlatformSpeechSynthesisUtterance&, CyberCore::SpeechBoundary, unsigned characterIndex, unsigned characterLength) final;

    // PlatformSpeechSynthesizerClient
    void voicesDidChange() final;
#endif

#if ENABLE(WIRELESS_PLAYBACK_TARGET) && !PLATFORM(IOS_FAMILY)
    // WebMediaSessionManagerClient
    void setPlaybackTarget(CyberCore::PlaybackTargetClientContextIdentifier, Ref<CyberCore::MediaPlaybackTarget>&&) final;
    void externalOutputDeviceAvailableDidChange(CyberCore::PlaybackTargetClientContextIdentifier, bool) final;
    void setShouldPlayToPlaybackTarget(CyberCore::PlaybackTargetClientContextIdentifier, bool) final;
    void playbackTargetPickerWasDismissed(CyberCore::PlaybackTargetClientContextIdentifier) final;
    bool alwaysOnLoggingAllowed() const final { return page.sessionID().isAlwaysOnLoggingAllowed(); }
    PlatformView* platformView() const final;
#endif
};

} // namespace CyberKit
