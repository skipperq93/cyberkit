/*
 * Copyright (C) 2010-2018 Apple Inc. All rights reserved.
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

#include "APIInjectedBundleEditorClient.h"
#include "APIInjectedBundleFormClient.h"
#include "APIInjectedBundlePageContextMenuClient.h"
#include "APIInjectedBundlePageLoaderClient.h"
#include "APIInjectedBundlePageResourceLoadClient.h"
#include "APIInjectedBundlePageUIClient.h"
#include "APIObject.h"
#include "CallbackID.h"
#include "DrawingAreaInfo.h"
#include "EditingRange.h"
#include "FocusedElementInformation.h"
#include "InjectedBundlePageContextMenuClient.h"
#include "InjectedBundlePageFullScreenClient.h"
#include "InjectedBundlePagePolicyClient.h"
#include "LayerTreeContext.h"
#include "MessageReceiver.h"
#include "MessageSender.h"
#include "OptionalCallbackID.h"
#include "Plugin.h"
#include "SandboxExtension.h"
#include "ShareSheetCallbackID.h"
#include "SharedMemory.h"
#include "UserData.h"
#include "WebBackForwardListProxy.h"
#include "WebURLSchemeHandler.h"
#include "WebUndoStepID.h"
#include "WebUserContentController.h"
#include "WebsitePoliciesData.h"
#include <CyberScriptCore/InspectorFrontendChannel.h>
#include <CyberCore/ActivityState.h>
#include <CyberCore/DictionaryPopupInfo.h>
#include <CyberCore/DisabledAdaptations.h>
#include <CyberCore/FrameLoaderTypes.h>
#include <CyberCore/HTMLMenuElement.h>
#include <CyberCore/HTMLMenuItemElement.h>
#include <CyberCore/IntRect.h>
#include <CyberCore/IntSizeHash.h>
#include <CyberCore/Page.h>
#include <CyberCore/PageOverlay.h>
#include <CyberCore/PluginData.h>
#include <CyberCore/SecurityPolicyViolationEvent.h>
#include <CyberCore/ShareData.h>
#include <CyberCore/UserActivity.h>
#include <CyberCore/UserContentTypes.h>
#include <CyberCore/UserInterfaceLayoutDirection.h>
#include <CyberCore/UserScriptTypes.h>
#include <CyberCore/VisibilityState.h>
#include <CyberCore/CyberCoreKeyboardUIMode.h>
#include <memory>
#include <pal/HysteresisActivity.h>
#include <wtf/HashMap.h>
#include <wtf/MonotonicTime.h>
#include <wtf/RefPtr.h>
#include <wtf/RunLoop.h>
#include <wtf/Seconds.h>
#include <wtf/WallTime.h>
#include <wtf/text/WTFString.h>

#if HAVE(ACCESSIBILITY) && PLATFORM(GTK)
#include "WebPageAccessibilityObject.h"
#include <wtf/glib/GRefPtr.h>
#endif

#if PLATFORM(GTK)
#include "ArgumentCodersGtk.h"
#include "WebPrintOperationGtk.h"
#endif

#if PLATFORM(IOS_FAMILY)
#include "GestureTypes.h"
#include "WebPageMessages.h"
#include <CyberCore/IntPointHash.h>
#include <CyberCore/ViewportConfiguration.h>
#endif

#if ENABLE(APPLICATION_MANIFEST)
#include <CyberCore/ApplicationManifest.h>
#endif

#if ENABLE(IOS_TOUCH_EVENTS)
#include <CyberKitAdditions/PlatformTouchEventIOS.h>
#elif ENABLE(TOUCH_EVENTS)
#include <CyberCore/PlatformTouchEvent.h>
#endif

#if ENABLE(DATA_DETECTION)
#include <CyberCore/DataDetection.h>
#endif

#if ENABLE(MAC_GESTURE_EVENTS)
#include <CyberKitAdditions/PlatformGestureEventMac.h>
#endif

#if PLATFORM(COCOA)
#include "DynamicViewportSizeUpdate.h"
#include <CyberCore/VisibleSelection.h>
#include <wtf/RetainPtr.h>
OBJC_CLASS CALayer;
OBJC_CLASS NSArray;
OBJC_CLASS NSDictionary;
OBJC_CLASS NSObject;
OBJC_CLASS WKAccessibilityWebPageObject;
#endif

namespace API {
class Array;
}

namespace IPC {
class Connection;
class Decoder;
class FormDataReference;
}

namespace CyberCore {

class CaptureDevice;
class DocumentLoader;
class DragData;
class FontAttributeChanges;
class FontChanges;
class Frame;
class FrameSelection;
class FrameView;
class GraphicsContext;
class HTMLMenuElement;
class HTMLMenuItemElement;
class HTMLPlugInElement;
class HTMLPlugInImageElement;
class IntPoint;
class KeyboardEvent;
class MediaPlaybackTargetContext;
class MediaPlayerRequestInstallMissingPluginsCallback;
class Page;
class PrintContext;
class Range;
class ResourceRequest;
class ResourceResponse;
class SelectionRect;
class SharedBuffer;
class SubstituteData;
class TextCheckingRequest;
class VisiblePosition;

enum SyntheticClickType : int8_t;
enum class DragHandlingMethod : uint8_t;
enum class ShouldTreatAsContinuingLoad : bool;
enum class TextIndicatorPresentationTransition : uint8_t;
enum class WritingDirection : uint8_t;

struct BackForwardItemIdentifier;
struct CompositionUnderline;
struct DictationAlternative;
struct GlobalFrameIdentifier;
struct GlobalWindowIdentifier;
struct Highlight;
struct KeypressCommand;
struct PromisedAttachmentInfo;
struct TextCheckingResult;
struct ViewportArguments;

#if ENABLE(ATTACHMENT_ELEMENT)
class HTMLAttachmentElement;
#endif
}

namespace CyberKit {

class DataReference;
class DrawingArea;
class DownloadID;
class FindController;
class GamepadData;
class GeolocationPermissionRequestManager;
class MediaDeviceSandboxExtensions;
class NotificationPermissionRequestManager;
class PDFPlugin;
class PageBanner;
class PluginView;
    class RemoteObjectRegistry;
class RemoteWebInspectorUI;
class UserMediaPermissionRequestManager;
class ViewGestureGeometryCollector;
class VisibleContentRectUpdateInfo;
class WebColorChooser;
class WebContextMenu;
class WebContextMenuItemData;
class WebDataListSuggestionPicker;
class WebDocumentLoader;
class WebEvent;
class PlaybackSessionManager;
class VideoFullscreenManager;
class WebFrame;
class WebFullScreenManager;
class WebGestureEvent;
class WebImage;
class WebInspector;
class WebInspectorClient;
class WebInspectorUI;
class WebKeyboardEvent;
class WebMouseEvent;
class WebNotificationClient;
class WebOpenPanelResultListener;
class WebPageGroupProxy;
class WebPageInspectorTargetController;
class WebPageOverlay;
class WebPopupMenu;
class WebTouchEvent;
class WebURLSchemeHandlerProxy;
class WebUndoStep;
class WebUserContentController;
class WebWheelEvent;
class RemoteLayerTreeTransaction;

enum FindOptions : uint16_t;
enum class DragControllerAction : uint8_t;

struct AttributedString;
struct DataDetectionResult;
struct BackForwardListItemState;
struct EditorState;
struct InteractionInformationAtPosition;
struct InteractionInformationRequest;
struct LoadParameters;
struct PrintInfo;
struct WebPageCreationParameters;
struct WebPreferencesStore;
struct WebSelectionData;
struct WebsitePoliciesData;

using SnapshotOptions = uint32_t;
using WKEventModifiers = uint32_t;

class WebPage : public API::ObjectImpl<API::Object::Type::BundlePage>, public IPC::MessageReceiver, public IPC::MessageSender, public CanMakeWeakPtr<WebPage> {
public:
    static Ref<WebPage> create(uint64_t pageID, WebPageCreationParameters&&);
    virtual ~WebPage();

    void reinitializeWebPage(WebPageCreationParameters&&);

    void close();

    static WebPage* fromCorePage(CyberCore::Page*);

    CyberCore::Page* corePage() const { return m_page.get(); }
    uint64_t pageID() const { return m_pageID; }
    PAL::SessionID sessionID() const { return m_page->sessionID(); }
    bool usesEphemeralSession() const { return m_page->usesEphemeralSession(); }

    void setSessionID(PAL::SessionID);

    void setSize(const CyberCore::IntSize&);
    const CyberCore::IntSize& size() const { return m_viewSize; }
    CyberCore::IntRect bounds() const { return CyberCore::IntRect(CyberCore::IntPoint(), size()); }

    DrawingArea* drawingArea() const { return m_drawingArea.get(); }

#if ENABLE(ASYNC_SCROLLING)
    CyberCore::ScrollingCoordinator* scrollingCoordinator() const;
#endif

    WebPageGroupProxy* pageGroup() const { return m_pageGroup.get(); }

    void scrollMainFrameIfNotAtMaxScrollPosition(const CyberCore::IntSize& scrollOffset);

    bool scrollBy(uint32_t scrollDirection, uint32_t scrollGranularity);

    void centerSelectionInVisibleArea();

#if PLATFORM(COCOA)
    void willCommitLayerTree(RemoteLayerTreeTransaction&);
    void didFlushLayerTreeAtTime(MonotonicTime);
#endif

    void willDisplayPage();

    enum class LazyCreationPolicy { UseExistingOnly, CreateIfNeeded };

    WebInspector* inspector(LazyCreationPolicy = LazyCreationPolicy::CreateIfNeeded);
    WebInspectorUI* inspectorUI();
    RemoteWebInspectorUI* remoteInspectorUI();
    bool isInspectorPage() { return !!m_inspectorUI || !!m_remoteInspectorUI; }

    void inspectorFrontendCountChanged(unsigned);

#if PLATFORM(IOS_FAMILY) || (PLATFORM(MAC) && ENABLE(VIDEO_PRESENTATION_MODE))
    PlaybackSessionManager& playbackSessionManager();
    VideoFullscreenManager& videoFullscreenManager();
    void videoControlsManagerDidChange();
#endif

#if PLATFORM(IOS_FAMILY)
    void setAllowsMediaDocumentInlinePlayback(bool);
    bool allowsMediaDocumentInlinePlayback() const { return m_allowsMediaDocumentInlinePlayback; }
#endif

#if ENABLE(FULLSCREEN_API)
    WebFullScreenManager* fullScreenManager();
#endif

    void addConsoleMessage(uint64_t frameID, MessageSource, MessageLevel, const String&, uint64_t requestID = 0);
    void sendCSPViolationReport(uint64_t frameID, const URL& reportURL, IPC::FormDataReference&&);
    void enqueueSecurityPolicyViolationEvent(uint64_t frameID, CyberCore::SecurityPolicyViolationEvent::Init&&);

    // -- Called by the DrawingArea.
    // FIXME: We could genericize these into a DrawingArea client interface. Would that be beneficial?
    void drawRect(CyberCore::GraphicsContext&, const CyberCore::IntRect&);
    void layoutIfNeeded();

    // -- Called from CyberCore clients.
    bool handleEditingKeyboardEvent(CyberCore::KeyboardEvent*);

    void didStartPageTransition();
    void didCompletePageTransition();
    void didCommitLoad(WebFrame*);
    void willReplaceMultipartContent(const WebFrame&);
    void didReplaceMultipartContent(const WebFrame&);
    void didFinishLoad(WebFrame*);
    void show();
    String userAgent(const URL&) const;
    String userAgent(WebFrame*, const URL&) const;
    String platformUserAgent(const URL&) const;
    CyberCore::KeyboardUIMode keyboardUIMode();

    void didInsertMenuElement(CyberCore::HTMLMenuElement&);
    void didRemoveMenuElement(CyberCore::HTMLMenuElement&);
    void didInsertMenuItemElement(CyberCore::HTMLMenuItemElement&);
    void didRemoveMenuItemElement(CyberCore::HTMLMenuItemElement&);

    const String& overrideContentSecurityPolicy() const { return m_overrideContentSecurityPolicy; }

    WebUndoStep* webUndoStep(WebUndoStepID);
    void addWebUndoStep(WebUndoStepID, Ref<WebUndoStep>&&);
    void removeWebEditCommand(WebUndoStepID);
    bool isInRedo() const { return m_isInRedo; }

    bool isAlwaysOnLoggingAllowed() const;
    void setActivePopupMenu(WebPopupMenu*);

    void setHiddenPageDOMTimerThrottlingIncreaseLimit(Seconds limit)
    {
        m_page->setDOMTimerAlignmentIntervalIncreaseLimit(limit);
    }

#if ENABLE(INPUT_TYPE_COLOR)
    WebColorChooser* activeColorChooser() const { return m_activeColorChooser; }
    void setActiveColorChooser(WebColorChooser*);
    void didChooseColor(const CyberCore::Color&);
    void didEndColorPicker();
#endif

#if ENABLE(DATALIST_ELEMENT)
    void setActiveDataListSuggestionPicker(WebDataListSuggestionPicker*);
    void didSelectDataListOption(const String&);
    void didCloseSuggestions();
#endif

    WebOpenPanelResultListener* activeOpenPanelResultListener() const { return m_activeOpenPanelResultListener.get(); }
    void setActiveOpenPanelResultListener(Ref<WebOpenPanelResultListener>&&);

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;
    void didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, std::unique_ptr<IPC::Encoder>&) override;

    // -- InjectedBundle methods
#if ENABLE(CONTEXT_MENUS)
    void setInjectedBundleContextMenuClient(std::unique_ptr<API::InjectedBundle::PageContextMenuClient>&&);
#endif
    void setInjectedBundleEditorClient(std::unique_ptr<API::InjectedBundle::EditorClient>&&);
    void setInjectedBundleFormClient(std::unique_ptr<API::InjectedBundle::FormClient>&&);
    void setInjectedBundlePageLoaderClient(std::unique_ptr<API::InjectedBundle::PageLoaderClient>&&);
    void initializeInjectedBundlePolicyClient(WKBundlePagePolicyClientBase*);
    void setInjectedBundleResourceLoadClient(std::unique_ptr<API::InjectedBundle::ResourceLoadClient>&&);
    void setInjectedBundleUIClient(std::unique_ptr<API::InjectedBundle::PageUIClient>&&);
#if ENABLE(FULLSCREEN_API)
    void initializeInjectedBundleFullScreenClient(WKBundlePageFullScreenClientBase*);
#endif

#if ENABLE(CONTEXT_MENUS)
    API::InjectedBundle::PageContextMenuClient& injectedBundleContextMenuClient() { return *m_contextMenuClient; }
#endif
    API::InjectedBundle::EditorClient& injectedBundleEditorClient() { return *m_editorClient; }
    API::InjectedBundle::FormClient& injectedBundleFormClient() { return *m_formClient; }
    API::InjectedBundle::PageLoaderClient& injectedBundleLoaderClient() { return *m_loaderClient; }
    InjectedBundlePagePolicyClient& injectedBundlePolicyClient() { return m_policyClient; }
    API::InjectedBundle::ResourceLoadClient& injectedBundleResourceLoadClient() { return *m_resourceLoadClient; }
    API::InjectedBundle::PageUIClient& injectedBundleUIClient() { return *m_uiClient; }
#if ENABLE(FULLSCREEN_API)
    InjectedBundlePageFullScreenClient& injectedBundleFullScreenClient() { return m_fullScreenClient; }
#endif

    bool findStringFromInjectedBundle(const String&, FindOptions);
    void findStringMatchesFromInjectedBundle(const String&, FindOptions);
    void replaceStringMatchesFromInjectedBundle(const Vector<uint32_t>& matchIndices, const String& replacementText, bool selectionOnly);

    WebFrame* mainWebFrame() const { return m_mainFrame.get(); }

    CyberCore::Frame* mainFrame() const; // May return nullptr.
    CyberCore::FrameView* mainFrameView() const; // May return nullptr.

    RefPtr<CyberCore::Range> currentSelectionAsRange();

#if ENABLE(NETSCAPE_PLUGIN_API)
    RefPtr<Plugin> createPlugin(WebFrame*, CyberCore::HTMLPlugInElement*, const Plugin::Parameters&, String& newMIMEType);
#endif

#if ENABLE(WEBGL)
    CyberCore::WebGLLoadPolicy webGLPolicyForURL(WebFrame*, const URL&);
    CyberCore::WebGLLoadPolicy resolveWebGLPolicyForURL(WebFrame*, const URL&);
#endif
    
    enum class IncludePostLayoutDataHint { No, Yes };
    EditorState editorState(IncludePostLayoutDataHint = IncludePostLayoutDataHint::Yes) const;
    void updateEditorStateAfterLayoutIfEditabilityChanged();

    String renderTreeExternalRepresentation() const;
    String renderTreeExternalRepresentationForPrinting() const;
    uint64_t renderTreeSize() const;

    void setTracksRepaints(bool);
    bool isTrackingRepaints() const;
    void resetTrackedRepaints();
    Ref<API::Array> trackedRepaintRects();

    void executeEditingCommand(const String& commandName, const String& argument);
    bool isEditingCommandEnabled(const String& commandName);
    void clearMainFrameName();
    void sendClose();

    void suspendForProcessSwap();

    void sendSetWindowFrame(const CyberCore::FloatRect&);

    double textZoomFactor() const;
    void setTextZoomFactor(double);
    double pageZoomFactor() const;
    void setPageZoomFactor(double);
    void setPageAndTextZoomFactors(double pageZoomFactor, double textZoomFactor);
    void windowScreenDidChange(uint32_t);
    String dumpHistoryForTesting(const String& directory);
    void clearHistory();

    void accessibilitySettingsDidChange();
#if ENABLE(ACCESSIBILITY_EVENTS)
    void updateAccessibilityEventsEnabled(bool);
#endif

    void scalePage(double scale, const CyberCore::IntPoint& origin);
    void scalePageInViewCoordinates(double scale, CyberCore::IntPoint centerInViewCoordinates);
    double pageScaleFactor() const;
    double totalScaleFactor() const;
    double viewScaleFactor() const;
    void scaleView(double scale);

    void setUseFixedLayout(bool);
    bool useFixedLayout() const { return m_useFixedLayout; }
    bool setFixedLayoutSize(const CyberCore::IntSize&);
    CyberCore::IntSize fixedLayoutSize() const;

    void listenForLayoutMilestones(OptionSet<CyberCore::LayoutMilestone>);

    void setSuppressScrollbarAnimations(bool);
    
    void setEnableVerticalRubberBanding(bool);
    void setEnableHorizontalRubberBanding(bool);
    
    void setBackgroundExtendsBeyondPage(bool);

    void setPaginationMode(uint32_t /* CyberCore::Pagination::Mode */);
    void setPaginationBehavesLikeColumns(bool);
    void setPageLength(double);
    void setGapBetweenPages(double);
    void setPaginationLineGridEnabled(bool);
    
    void postInjectedBundleMessage(const String& messageName, const UserData&);

    bool drawsBackground() const { return m_drawsBackground; }

    void setUnderlayColor(const CyberCore::Color& color) { m_underlayColor = color; }
    CyberCore::Color underlayColor() const { return m_underlayColor; }

    void stopLoading();
    void stopLoadingFrame(uint64_t frameID);
    bool defersLoading() const;
    void setDefersLoading(bool deferLoading);

    void enterAcceleratedCompositingMode(CyberCore::GraphicsLayer*);
    void exitAcceleratedCompositingMode();

    void addPluginView(PluginView*);
    void removePluginView(PluginView*);

    bool isVisible() const { return m_activityState.contains(CyberCore::ActivityState::IsVisible); }
    bool isVisibleOrOccluded() const { return m_activityState.contains(CyberCore::ActivityState::IsVisibleOrOccluded); }

    LayerHostingMode layerHostingMode() const { return m_layerHostingMode; }
    void setLayerHostingMode(LayerHostingMode);

#if PLATFORM(COCOA)
    void updatePluginsActiveAndFocusedState();
    const CyberCore::FloatRect& windowFrameInScreenCoordinates() const { return m_windowFrameInScreenCoordinates; }
    const CyberCore::FloatRect& windowFrameInUnflippedScreenCoordinates() const { return m_windowFrameInUnflippedScreenCoordinates; }
    const CyberCore::FloatRect& viewFrameInWindowCoordinates() const { return m_viewFrameInWindowCoordinates; }

    bool hasCachedWindowFrame() const { return m_hasCachedWindowFrame; }

    void updateHeaderAndFooterLayersForDeviceScaleChange(float scaleFactor);
#endif

#if PLATFORM(MAC)
    void setTopOverhangImage(WebImage*);
    void setBottomOverhangImage(WebImage*);
    
    void setUseSystemAppearance(bool);
    void setUseDarkAppearance(bool);
#endif

    bool windowIsFocused() const;
    bool windowAndWebPageAreFocused() const;

#if !PLATFORM(IOS_FAMILY)
    void setHeaderPageBanner(PageBanner*);
    PageBanner* headerPageBanner();
    void setFooterPageBanner(PageBanner*);
    PageBanner* footerPageBanner();

    void hidePageBanners();
    void showPageBanners();
    
    void setHeaderBannerHeightForTesting(int);
    void setFooterBannerHeightForTesting(int);
#endif

    CyberCore::IntPoint screenToRootView(const CyberCore::IntPoint&);
    CyberCore::IntRect rootViewToScreen(const CyberCore::IntRect&);
    
#if PLATFORM(IOS_FAMILY)
    CyberCore::IntPoint accessibilityScreenToRootView(const CyberCore::IntPoint&);
    CyberCore::IntRect rootViewToAccessibilityScreen(const CyberCore::IntRect&);
#endif
    
    RefPtr<WebImage> scaledSnapshotWithOptions(const CyberCore::IntRect&, double additionalScaleFactor, SnapshotOptions);

    static const WebEvent* currentEvent();

    FindController& findController() { return m_findController.get(); }

#if ENABLE(GEOLOCATION)
    GeolocationPermissionRequestManager& geolocationPermissionRequestManager() { return m_geolocationPermissionRequestManager.get(); }
#endif

#if PLATFORM(IOS_FAMILY)
    void savePageState(CyberCore::HistoryItem&);
    void restorePageState(const CyberCore::HistoryItem&);
#endif

#if ENABLE(MEDIA_STREAM)
    UserMediaPermissionRequestManager& userMediaPermissionRequestManager() { return m_userMediaPermissionRequestManager; }
    void prepareToSendUserMediaPermissionRequest();
    void captureDevicesChanged();
#endif

    void elementDidFocus(CyberCore::Element&);
    void elementDidRefocus(CyberCore::Element&);
    void elementDidBlur(CyberCore::Element&);
    void resetFocusedElementForFrame(WebFrame*);

    void disabledAdaptationsDidChange(const OptionSet<CyberCore::DisabledAdaptations>&);
    void viewportPropertiesDidChange(const CyberCore::ViewportArguments&);
    void executeEditCommandWithCallback(const String&, const String& argument, CallbackID);

#if PLATFORM(IOS_FAMILY)
    CyberCore::FloatSize screenSize() const;
    CyberCore::FloatSize availableScreenSize() const;
    CyberCore::FloatSize overrideScreenSize() const;
    int32_t deviceOrientation() const { return m_deviceOrientation; }
    void didReceiveMobileDocType(bool);

    void setUseTestingViewportConfiguration(bool useTestingViewport) { m_useTestingViewportConfiguration = useTestingViewport; }
    bool isUsingTestingViewportConfiguration() const { return m_useTestingViewportConfiguration; }

    double minimumPageScaleFactor() const;
    double maximumPageScaleFactor() const;
    double maximumPageScaleFactorIgnoringAlwaysScalable() const;
    bool allowsUserScaling() const;
    bool hasStablePageScaleFactor() const { return m_hasStablePageScaleFactor; }

    void handleTap(const CyberCore::IntPoint&, uint64_t lastLayerTreeTransactionId);
    void potentialTapAtPosition(uint64_t requestID, const CyberCore::FloatPoint&);
    void commitPotentialTap(uint64_t lastLayerTreeTransactionId);
    void commitPotentialTapFailed();
    void cancelPotentialTap();
    void cancelPotentialTapInFrame(WebFrame&);
    void tapHighlightAtPosition(uint64_t requestID, const CyberCore::FloatPoint&);

    void inspectorNodeSearchMovedToPosition(const CyberCore::FloatPoint&);
    void inspectorNodeSearchEndedAtPosition(const CyberCore::FloatPoint&);

    void blurFocusedElement();
    void requestFocusedElementInformation(CallbackID);
    void selectWithGesture(const CyberCore::IntPoint&, uint32_t granularity, uint32_t gestureType, uint32_t gestureState, bool isInteractingWithFocusedElement, CallbackID);
    void updateSelectionWithTouches(const CyberCore::IntPoint&, uint32_t touches, bool baseIsStart, CallbackID);
    void selectWithTwoTouches(const CyberCore::IntPoint& from, const CyberCore::IntPoint& to, uint32_t gestureType, uint32_t gestureState, CallbackID);
    void extendSelection(uint32_t granularity);
    void selectWordBackward();
    void moveSelectionByOffset(int32_t offset, CallbackID);
    void selectTextWithGranularityAtPoint(const CyberCore::IntPoint&, uint32_t granularity, bool isInteractingWithFocusedElement, CallbackID);
    void selectPositionAtBoundaryWithDirection(const CyberCore::IntPoint&, uint32_t granularity, uint32_t direction, bool isInteractingWithFocusedElement, CallbackID);
    void moveSelectionAtBoundaryWithDirection(uint32_t granularity, uint32_t direction, CallbackID);
    void selectPositionAtPoint(const CyberCore::IntPoint&, bool isInteractingWithFocusedElement, CallbackID);
    void beginSelectionInDirection(uint32_t direction, CallbackID);
    void updateSelectionWithExtentPoint(const CyberCore::IntPoint&, bool isInteractingWithFocusedElement, CallbackID);
    void updateSelectionWithExtentPointAndBoundary(const CyberCore::IntPoint&, uint32_t granularity, bool isInteractingWithFocusedElement, CallbackID);

    void requestDictationContext(CallbackID);
    void replaceDictatedText(const String& oldText, const String& newText);
    void replaceSelectedText(const String& oldText, const String& newText);
    void requestAutocorrectionData(const String& textForAutocorrection, CallbackID);
    void applyAutocorrection(const String& correction, const String& originalText, CallbackID);
    void syncApplyAutocorrection(const String& correction, const String& originalText, bool& correctionApplied);
    void requestAutocorrectionContext(CallbackID);
    void getAutocorrectionContext(String& beforeText, String& markedText, String& selectedText, String& afterText, uint64_t& location, uint64_t& length);
    void getPositionInformation(const InteractionInformationRequest&, InteractionInformationAtPosition&);
    void requestPositionInformation(const InteractionInformationRequest&);
    void startInteractionWithElementAtPosition(const CyberCore::IntPoint&);
    void stopInteraction();
    void performActionOnElement(uint32_t action);
    void focusNextFocusedElement(bool isForward, CallbackID);
    void autofillLoginCredentials(const String&, const String&);
    void setFocusedElementValue(const String&);
    void setFocusedElementValueAsNumber(double);
    void setFocusedElementSelectedIndex(uint32_t index, bool allowMultipleSelection);
    CyberCore::IntRect rectForElementAtInteractionLocation();
    void updateSelectionAppearance();
    void getSelectionContext(CallbackID);
    void handleTwoFingerTapAtPoint(const CyberCore::IntPoint&, uint64_t requestID);
    void handleStylusSingleTapAtPoint(const CyberCore::IntPoint&, uint64_t requestID);
    void getRectsForGranularityWithSelectionOffset(uint32_t, int32_t, CallbackID);
    void getRectsAtSelectionOffsetWithText(int32_t, const String&, CallbackID);
    void storeSelectionForAccessibility(bool);
    void startAutoscrollAtPosition(const CyberCore::FloatPoint&);
    void cancelAutoscroll();

    void contentSizeCategoryDidChange(const String&);

    Seconds eventThrottlingDelay() const;

    void showInspectorHighlight(const CyberCore::Highlight&);
    void hideInspectorHighlight();

    void showInspectorIndication();
    void hideInspectorIndication();

    void enableInspectorNodeSearch();
    void disableInspectorNodeSearch();

    bool forceAlwaysUserScalable() const { return m_forceAlwaysUserScalable; }
    void setForceAlwaysUserScalable(bool);
#endif

#if PLATFORM(IOS_FAMILY) && ENABLE(IOS_TOUCH_EVENTS)
    void dispatchAsynchronousTouchEvents(const Vector<WebTouchEvent, 1>& queue);
#endif

    bool hasRichlyEditableSelection() const;

    enum class LayerTreeFreezeReason {
        PageTransition          = 1 << 0,
        BackgroundApplication   = 1 << 1,
        ProcessSuspended        = 1 << 2,
        PageSuspended           = 1 << 3,
        Printing                = 1 << 4,
        ProcessSwap             = 1 << 5,
    };
    void freezeLayerTree(LayerTreeFreezeReason);
    void unfreezeLayerTree(LayerTreeFreezeReason);

    void markLayersVolatile(WTF::Function<void (bool)>&& completionHandler = { });
    void cancelMarkLayersVolatile();

    NotificationPermissionRequestManager* notificationPermissionRequestManager();

    void pageDidScroll();

#if ENABLE(CONTEXT_MENUS)
    WebContextMenu* contextMenu();
    WebContextMenu* contextMenuAtPointInWindow(const CyberCore::IntPoint&);
#endif

    bool hasLocalDataForURL(const URL&);

    static bool canHandleRequest(const CyberCore::ResourceRequest&);

    class SandboxExtensionTracker {
    public:
        ~SandboxExtensionTracker();

        void invalidate();

        void beginLoad(WebFrame*, SandboxExtension::Handle&&);
        void beginReload(WebFrame*, SandboxExtension::Handle&&);
        void willPerformLoadDragDestinationAction(RefPtr<SandboxExtension>&& pendingDropSandboxExtension);
        void didStartProvisionalLoad(WebFrame*);
        void didCommitProvisionalLoad(WebFrame*);
        void didFailProvisionalLoad(WebFrame*);

    private:
        void setPendingProvisionalSandboxExtension(RefPtr<SandboxExtension>&&);

        RefPtr<SandboxExtension> m_pendingProvisionalSandboxExtension;
        RefPtr<SandboxExtension> m_provisionalSandboxExtension;
        RefPtr<SandboxExtension> m_committedSandboxExtension;
    };

    SandboxExtensionTracker& sandboxExtensionTracker() { return m_sandboxExtensionTracker; }

#if PLATFORM(GTK)
    void setComposition(const String& text, const Vector<CyberCore::CompositionUnderline>& underlines, uint64_t selectionStart, uint64_t selectionEnd, uint64_t replacementRangeStart, uint64_t replacementRangeLength);
    void confirmComposition(const String& text, int64_t selectionStart, int64_t selectionLength);
    void cancelComposition();

    void collapseSelectionInFrame(uint64_t frameID);
#endif

#if PLATFORM (GTK) && HAVE(GTK_GESTURES)
    void getCenterForZoomGesture(const CyberCore::IntPoint& centerInViewCoordinates, CyberCore::IntPoint& result);
#endif

    void didApplyStyle();
    void didChangeSelection();
    void didChangeContents();
    void discardedComposition();
    void canceledComposition();
    void didUpdateComposition();
    void didEndUserTriggeredSelectionChanges();

#if PLATFORM(COCOA)
    void registerUIProcessAccessibilityTokens(const IPC::DataReference& elemenToken, const IPC::DataReference& windowToken);
    WKAccessibilityWebPageObject* accessibilityRemoteObject();
    NSObject *accessibilityObjectForMainFramePlugin();
    const CyberCore::FloatPoint& accessibilityPosition() const { return m_accessibilityPosition; }
    
    void sendComplexTextInputToPlugin(uint64_t pluginComplexTextInputIdentifier, const String& textInput);

    void setTextAsync(const String&);
    void insertTextAsync(const String& text, const EditingRange& replacementRange, bool registerUndoGroup = false, uint32_t editingRangeIsRelativeTo = (uint32_t)EditingRangeIsRelativeTo::EditableRoot, bool suppressSelectionUpdate = false);
    void getMarkedRangeAsync(CallbackID);
    void getSelectedRangeAsync(CallbackID);
    void characterIndexForPointAsync(const CyberCore::IntPoint&, CallbackID);
    void firstRectForCharacterRangeAsync(const EditingRange&, CallbackID);
    void setCompositionAsync(const String& text, const Vector<CyberCore::CompositionUnderline>& underlines, const EditingRange& selectionRange, const EditingRange& replacementRange);
    void confirmCompositionAsync();

    void readSelectionFromPasteboard(const WTF::String& pasteboardName, bool& result);
    void getStringSelectionForPasteboard(WTF::String& stringValue);
    void getDataSelectionForPasteboard(const WTF::String pasteboardType, SharedMemory::Handle& handle, uint64_t& size);
    void shouldDelayWindowOrderingEvent(const CyberKit::WebMouseEvent&, bool& result);
    void acceptsFirstMouse(int eventNumber, const CyberKit::WebMouseEvent&, bool& result);
    bool performNonEditingBehaviorForSelector(const String&, CyberCore::KeyboardEvent*);
#endif

#if PLATFORM(MAC)
    void insertDictatedTextAsync(const String& text, const EditingRange& replacementRange, const Vector<CyberCore::DictationAlternative>& dictationAlternativeLocations, bool registerUndoGroup = false);
    void attributedSubstringForCharacterRangeAsync(const EditingRange&, CallbackID);
    void fontAtSelection(CallbackID);
#endif

#if PLATFORM(COCOA) && ENABLE(SERVICE_CONTROLS)
    void replaceSelectionWithPasteboardData(const Vector<String>& types, const IPC::DataReference&);
#endif

#if HAVE(ACCESSIBILITY) && PLATFORM(GTK)
    void updateAccessibilityTree();
#endif

    void setCompositionForTesting(const String& compositionString, uint64_t from, uint64_t length, bool suppressUnderline);
    bool hasCompositionForTesting();
    void confirmCompositionForTesting(const String& compositionString);

#if PLATFORM(COCOA)
    bool isSpeaking();
    void speak(const String&);
    void stopSpeaking();

    void performDictionaryLookupForSelection(CyberCore::Frame&, const CyberCore::VisibleSelection&, CyberCore::TextIndicatorPresentationTransition);
#endif

    bool isSmartInsertDeleteEnabled();
    void setSmartInsertDeleteEnabled(bool);

    bool isSelectTrailingWhitespaceEnabled() const;
    void setSelectTrailingWhitespaceEnabled(bool);

    void replaceSelectionWithText(CyberCore::Frame*, const String&);
    void clearSelection();
    void restoreSelectionInFocusedEditableElement();

#if ENABLE(DRAG_SUPPORT) && PLATFORM(GTK)
    void performDragControllerAction(DragControllerAction, const CyberCore::IntPoint& clientPosition, const CyberCore::IntPoint& globalPosition, uint64_t draggingSourceOperationMask, WebSelectionData&&, uint32_t flags);
#endif

#if ENABLE(DRAG_SUPPORT) && !PLATFORM(GTK)
    void performDragControllerAction(DragControllerAction, const CyberCore::DragData&, SandboxExtension::Handle&&, SandboxExtension::HandleArray&&);
#endif

#if ENABLE(DRAG_SUPPORT)
    void dragEnded(CyberCore::IntPoint clientPosition, CyberCore::IntPoint globalPosition, uint64_t operation);

    void willPerformLoadDragDestinationAction();
    void mayPerformUploadDragDestinationAction();

    void willStartDrag() { ASSERT(!m_isStartingDrag); m_isStartingDrag = true; }
    void didStartDrag();
    void dragCancelled();
#endif

    void beginPrinting(uint64_t frameID, const PrintInfo&);
    void endPrinting();
    void computePagesForPrinting(uint64_t frameID, const PrintInfo&, CallbackID);
    void computePagesForPrintingImpl(uint64_t frameID, const PrintInfo&, Vector<CyberCore::IntRect>& pageRects, double& totalScaleFactor);

#if PLATFORM(COCOA)
    void drawRectToImage(uint64_t frameID, const PrintInfo&, const CyberCore::IntRect&, const CyberCore::IntSize&, CallbackID);
    void drawPagesToPDF(uint64_t frameID, const PrintInfo&, uint32_t first, uint32_t count, CallbackID);
    void drawPagesToPDFImpl(uint64_t frameID, const PrintInfo&, uint32_t first, uint32_t count, RetainPtr<CFMutableDataRef>& pdfPageData);
#endif

#if PLATFORM(IOS_FAMILY)
    void computePagesForPrintingAndDrawToPDF(uint64_t frameID, const PrintInfo&, CallbackID, Messages::WebPage::ComputePagesForPrintingAndDrawToPDF::DelayedReply&&);
#endif

#if PLATFORM(GTK)
    void drawPagesForPrinting(uint64_t frameID, const PrintInfo&, CallbackID);
    void didFinishPrintOperation(const CyberCore::ResourceError&, CallbackID);
#endif

    void addResourceRequest(unsigned long, const CyberCore::ResourceRequest&);
    void removeResourceRequest(unsigned long);

    void setMediaVolume(float);
    void setMuted(CyberCore::MediaProducer::MutedStateFlags);
    void setMayStartMediaWhenInWindow(bool);
    void stopMediaCapture();

#if ENABLE(MEDIA_SESSION)
    void handleMediaEvent(uint32_t /* CyberCore::MediaEventType */);
    void setVolumeOfMediaElement(double, uint64_t);
#endif

    void updateMainFrameScrollOffsetPinning();

    bool mainFrameHasCustomContentProvider() const;
    void addMIMETypeWithCustomContentProvider(const String&);

    void mainFrameDidLayout();

    bool canRunBeforeUnloadConfirmPanel() const { return m_canRunBeforeUnloadConfirmPanel; }
    void setCanRunBeforeUnloadConfirmPanel(bool canRunBeforeUnloadConfirmPanel) { m_canRunBeforeUnloadConfirmPanel = canRunBeforeUnloadConfirmPanel; }

    bool canRunModal() const { return m_canRunModal; }
    void setCanRunModal(bool canRunModal) { m_canRunModal = canRunModal; }

    void runModal();

    void setDeviceScaleFactor(float);
    float deviceScaleFactor() const;

    void forceRepaintWithoutCallback();

    void unmarkAllMisspellings();
    void unmarkAllBadGrammar();

#if PLATFORM(COCOA)
    void handleAlternativeTextUIResult(const String&);
#endif

    // For testing purpose.
    void simulateMouseDown(int button, CyberCore::IntPoint, int clickCount, WKEventModifiers, WallTime);
    void simulateMouseUp(int button, CyberCore::IntPoint, int clickCount, WKEventModifiers, WallTime);
    void simulateMouseMotion(CyberCore::IntPoint, WallTime);

#if ENABLE(CONTEXT_MENUS)
    void contextMenuShowing() { m_isShowingContextMenu = true; }
#endif

    void wheelEvent(const WebWheelEvent&);

    void wheelEventHandlersChanged(bool);
    void recomputeShortCircuitHorizontalWheelEventsState();

#if ENABLE(MAC_GESTURE_EVENTS)
    void gestureEvent(const WebGestureEvent&);
#endif

    void updateVisibilityState(bool isInitialState = false);

#if PLATFORM(IOS_FAMILY)
    void setViewportConfigurationViewLayoutSize(const CyberCore::FloatSize&, double scaleFactor, double minimumEffectiveDeviceWidth);
    void setMaximumUnobscuredSize(const CyberCore::FloatSize&);
    void setDeviceOrientation(int32_t);
    void setOverrideViewportArguments(const Optional<CyberCore::ViewportArguments>&);
    void dynamicViewportSizeUpdate(const CyberCore::FloatSize& viewLayoutSize, const CyberCore::FloatSize& maximumUnobscuredSize, const CyberCore::FloatRect& targetExposedContentRect, const CyberCore::FloatRect& targetUnobscuredRect, const CyberCore::FloatRect& targetUnobscuredRectInScrollViewCoordinates, const CyberCore::FloatBoxExtent& targetUnobscuredSafeAreaInsets, double scale, int32_t deviceOrientation, DynamicViewportSizeUpdateID);
    Optional<float> scaleFromUIProcess(const VisibleContentRectUpdateInfo&) const;
    void updateVisibleContentRects(const VisibleContentRectUpdateInfo&, MonotonicTime oldestTimestamp);
    bool scaleWasSetByUIProcess() const { return m_scaleWasSetByUIProcess; }
    void willStartUserTriggeredZooming();
    void applicationWillResignActive();
    void applicationDidEnterBackground(bool isSuspendedUnderLock);
    void applicationDidFinishSnapshottingAfterEnteringBackground();
    void applicationWillEnterForeground(bool isSuspendedUnderLock);
    void applicationDidBecomeActive();
    void completePendingSyntheticClickForContentChangeObserver();

    bool platformPrefersTextLegibilityBasedZoomScaling() const;
    const CyberCore::ViewportConfiguration& viewportConfiguration() const { return m_viewportConfiguration; }

    void hardwareKeyboardAvailabilityChanged();

    void updateStringForFind(const String&);
#endif

#if ENABLE(IOS_TOUCH_EVENTS)
    void dispatchTouchEvent(const WebTouchEvent&, bool& handled);
#endif

#if PLATFORM(GTK) && USE(TEXTURE_MAPPER_GL)
    uint64_t nativeWindowHandle() { return m_nativeWindowHandle; }
#endif

    bool shouldUseCustomContentProviderForResponse(const CyberCore::ResourceResponse&);

    bool asynchronousPluginInitializationEnabled() const { return m_asynchronousPluginInitializationEnabled; }
    void setAsynchronousPluginInitializationEnabled(bool enabled) { m_asynchronousPluginInitializationEnabled = enabled; }
    bool asynchronousPluginInitializationEnabledForAllPlugins() const { return m_asynchronousPluginInitializationEnabledForAllPlugins; }
    void setAsynchronousPluginInitializationEnabledForAllPlugins(bool enabled) { m_asynchronousPluginInitializationEnabledForAllPlugins = enabled; }
    bool artificialPluginInitializationDelayEnabled() const { return m_artificialPluginInitializationDelayEnabled; }
    void setArtificialPluginInitializationDelayEnabled(bool enabled) { m_artificialPluginInitializationDelayEnabled = enabled; }
    void setTabToLinksEnabled(bool enabled) { m_tabToLinks = enabled; }
    bool tabToLinksEnabled() const { return m_tabToLinks; }

    bool scrollingPerformanceLoggingEnabled() const { return m_scrollingPerformanceLoggingEnabled; }
    void setScrollingPerformanceLoggingEnabled(bool);

#if PLATFORM(COCOA)
    bool shouldUsePDFPlugin() const;
    bool pdfPluginEnabled() const { return m_pdfPluginEnabled; }
    void setPDFPluginEnabled(bool enabled) { m_pdfPluginEnabled = enabled; }

    NSDictionary *dataDetectionContext() const { return m_dataDetectionContext.get(); }
#endif

    void savePDFToFileInDownloadsFolder(const String& suggestedFilename, const URL& originatingURL, const uint8_t* data, unsigned long size);

#if PLATFORM(COCOA)
    void savePDFToTemporaryFolderAndOpenWithNativeApplication(const String& suggestedFilename, const String& originatingURLString, const uint8_t* data, unsigned long size, const String& pdfUUID);
#endif

    bool mainFrameIsScrollable() const { return m_mainFrameIsScrollable; }

    void setAlwaysShowsHorizontalScroller(bool);
    void setAlwaysShowsVerticalScroller(bool);

    bool alwaysShowsHorizontalScroller() const { return m_alwaysShowsHorizontalScroller; };
    bool alwaysShowsVerticalScroller() const { return m_alwaysShowsVerticalScroller; };

    void setViewLayoutSize(const CyberCore::IntSize&);
    CyberCore::IntSize viewLayoutSize() const { return m_viewLayoutSize; }

    void setAutoSizingShouldExpandToViewHeight(bool shouldExpand);
    bool autoSizingShouldExpandToViewHeight() { return m_autoSizingShouldExpandToViewHeight; }

    void setViewportSizeForCSSViewportUnits(Optional<CyberCore::IntSize>);
    Optional<CyberCore::IntSize> viewportSizeForCSSViewportUnits() const { return m_viewportSizeForCSSViewportUnits; }

    bool canShowMIMEType(const String& MIMEType) const;
    bool canShowResponse(const CyberCore::ResourceResponse&) const;

    void addTextCheckingRequest(uint64_t requestID, Ref<CyberCore::TextCheckingRequest>&&);
    void didFinishCheckingText(uint64_t requestID, const Vector<CyberCore::TextCheckingResult>&);
    void didCancelCheckingText(uint64_t requestID);

#if ENABLE(PRIMARY_SNAPSHOTTED_PLUGIN_HEURISTIC)
    void determinePrimarySnapshottedPlugIn();
    void determinePrimarySnapshottedPlugInTimerFired();
    void resetPrimarySnapshottedPlugIn();
    bool matchesPrimaryPlugIn(const String& pageOrigin, const String& pluginOrigin, const String& mimeType) const;
    bool plugInIntersectsSearchRect(CyberCore::HTMLPlugInImageElement& pluginImageElement);
    bool plugInIsPrimarySize(CyberCore::HTMLPlugInImageElement& pluginImageElement, unsigned &pluginArea);
#endif

#if ENABLE(DATA_DETECTION)
    void setDataDetectionResults(NSArray *);
    void detectDataInAllFrames(uint64_t, CompletionHandler<void(const DataDetectionResult&)>&&);
    void removeDataDetectedLinks(CompletionHandler<void(const DataDetectionResult&)>&&);
#endif

    unsigned extendIncrementalRenderingSuppression();
    void stopExtendingIncrementalRenderingSuppression(unsigned token);
    bool shouldExtendIncrementalRenderingSuppression() { return !m_activeRenderingSuppressionTokens.isEmpty(); }

    CyberCore::ScrollPinningBehavior scrollPinningBehavior() { return m_scrollPinningBehavior; }
    void setScrollPinningBehavior(uint32_t /* CyberCore::ScrollPinningBehavior */ pinning);

    Optional<CyberCore::ScrollbarOverlayStyle> scrollbarOverlayStyle() { return m_scrollbarOverlayStyle; }
    void setScrollbarOverlayStyle(Optional<uint32_t /* CyberCore::ScrollbarOverlayStyle */> scrollbarStyle);

    Ref<CyberCore::DocumentLoader> createDocumentLoader(CyberCore::Frame&, const CyberCore::ResourceRequest&, const CyberCore::SubstituteData&);
    void updateCachedDocumentLoader(WebDocumentLoader&, CyberCore::Frame&);

    void getBytecodeProfile(CallbackID);
    void getSamplingProfilerOutput(CallbackID);
    
#if ENABLE(SERVICE_CONTROLS) || ENABLE(TELEPHONE_NUMBER_DETECTION)
    void handleTelephoneNumberClick(const String& number, const CyberCore::IntPoint&);
    void handleSelectionServiceClick(CyberCore::FrameSelection&, const Vector<String>& telephoneNumbers, const CyberCore::IntPoint&);
#endif

    void didChangeScrollOffsetForFrame(CyberCore::Frame*);

    void setMainFrameProgressCompleted(bool completed) { m_mainFrameProgressCompleted = completed; }
    bool shouldDispatchFakeMouseMoveEvents() const { return m_shouldDispatchFakeMouseMoveEvents; }

    void postMessage(const String& messageName, API::Object* messageBody);
    void postSynchronousMessageForTesting(const String& messageName, API::Object* messageBody, RefPtr<API::Object>& returnData);
    void postMessageIgnoringFullySynchronousMode(const String& messageName, API::Object* messageBody);

#if PLATFORM(GTK)
    void setInputMethodState(bool);
#endif

    void imageOrMediaDocumentSizeChanged(const CyberCore::IntSize&);

#if ENABLE(VIDEO) && USE(GSTREAMER)
    void requestInstallMissingMediaPlugins(const String& details, const String& description, CyberCore::MediaPlayerRequestInstallMissingPluginsCallback&);
#endif

    void addUserScript(String&& source, CyberCore::UserContentInjectedFrames, CyberCore::UserScriptInjectionTime);
    void addUserStyleSheet(const String& source, CyberCore::UserContentInjectedFrames);
    void removeAllUserContent();

    void dispatchDidReachLayoutMilestone(OptionSet<CyberCore::LayoutMilestone>);

    void didRestoreScrollPosition();

    bool isControlledByAutomation() const;
    void setControlledByAutomation(bool);

    void connectInspector(const String& targetId, Inspector::FrontendChannel::ConnectionType);
    void disconnectInspector(const String& targetId);
    void sendMessageToTargetBackend(const String& targetId, const String& message);

    void insertNewlineInQuotedContent();

#if USE(OS_STATE)
    WallTime loadCommitTime() const { return m_loadCommitTime; }
#endif

#if ENABLE(GAMEPAD)
    void gamepadActivity(const Vector<GamepadData>&, bool shouldMakeGamepadsVisible);
#endif
    
#if ENABLE(POINTER_LOCK)
    void didAcquirePointerLock();
    void didNotAcquirePointerLock();
    void didLosePointerLock();
#endif

    void didGetLoadDecisionForIcon(bool decision, CallbackID loadIdentifier, OptionalCallbackID);
    void setUseIconLoadingClient(bool);

#if ENABLE(DATA_INTERACTION)
    void didConcludeEditDataInteraction();
#endif

    WebURLSchemeHandlerProxy* urlSchemeHandlerForScheme(const String&);
    void stopAllURLSchemeTasks();

    Optional<double> cpuLimit() const { return m_cpuLimit; }

    static PluginView* pluginViewForFrame(CyberCore::Frame*);

    void sendPartialEditorStateAndSchedulePostLayoutUpdate();
    void flushPendingEditorStateUpdate();

#if ENABLE(RESOURCE_LOAD_STATISTICS)
    void hasStorageAccess(String&& subFrameHost, String&& topFrameHost, uint64_t frameID, CompletionHandler<void(bool)>&& callback);
    void requestStorageAccess(String&& subFrameHost, String&& topFrameHost, uint64_t frameID, CompletionHandler<void(bool)>&& callback);
#endif

    void showShareSheet(CyberCore::ShareDataWithParsedURL&, WTF::CompletionHandler<void(bool)>&& callback);
    void didCompleteShareSheet(bool wasCompleted, ShareSheetCallbackID contextId);
    
#if ENABLE(ATTACHMENT_ELEMENT)
    void insertAttachment(const String& identifier, Optional<uint64_t>&& fileSize, const String& fileName, const String& contentType, CallbackID);
    void updateAttachmentAttributes(const String& identifier, Optional<uint64_t>&& fileSize, const String& contentType, const String& fileName, const IPC::DataReference& enclosingImageData, CallbackID);
#endif

#if ENABLE(APPLICATION_MANIFEST)
    void getApplicationManifest(CallbackID);
    void didFinishLoadingApplicationManifest(uint64_t, const Optional<CyberCore::ApplicationManifest>&);
#endif

    void updateCurrentModifierState(OptionSet<CyberCore::PlatformEvent::Modifier> modifiers);

    UserContentControllerIdentifier userContentControllerIdentifier() const { return m_userContentController->identifier(); }

    CyberCore::UserInterfaceLayoutDirection userInterfaceLayoutDirection() const { return m_userInterfaceLayoutDirection; }

    bool isSuspended() const { return m_isSuspended; }

    void didReceiveWebPageMessage(IPC::Connection&, IPC::Decoder&);

    void setRemoteObjectRegistry(RemoteObjectRegistry&);
private:
    WebPage(uint64_t pageID, WebPageCreationParameters&&);

    void updateThrottleState();
    void updateUserActivity();

    // IPC::MessageSender
    IPC::Connection* messageSenderConnection() override;
    uint64_t messageSenderDestinationID() override;

    void platformInitialize();
    void platformReinitialize();
    void platformDetach();
    void platformEditorState(CyberCore::Frame&, EditorState& result, IncludePostLayoutDataHint) const;
    void sendEditorStateUpdate();
    void scheduleFullEditorStateUpdate();

#if PLATFORM(COCOA)
    void sendTouchBarMenuDataAddedUpdate(CyberCore::HTMLMenuElement&);
    void sendTouchBarMenuDataRemovedUpdate(CyberCore::HTMLMenuElement&);
    void sendTouchBarMenuItemDataAddedUpdate(CyberCore::HTMLMenuItemElement&);
    void sendTouchBarMenuItemDataRemovedUpdate(CyberCore::HTMLMenuItemElement&);
#endif

    void didReceiveSyncWebPageMessage(IPC::Connection&, IPC::Decoder&, std::unique_ptr<IPC::Encoder>&);

#if PLATFORM(IOS_FAMILY)
    void resetViewportDefaultConfiguration(WebFrame* mainFrame, bool hasMobileDocType = false);
    enum class ZoomToInitialScale { No, Yes };
    void viewportConfigurationChanged(ZoomToInitialScale = ZoomToInitialScale::No);
    void updateViewportSizeForCSSViewportUnits();

    static void convertSelectionRectsToRootView(CyberCore::FrameView*, Vector<CyberCore::SelectionRect>&);
    RefPtr<CyberCore::Range> rangeForWebSelectionAtPosition(const CyberCore::IntPoint&, const CyberCore::VisiblePosition&, SelectionFlags&);
    void getFocusedElementInformation(FocusedElementInformation&);
    void platformInitializeAccessibility();
    void handleSyntheticClick(CyberCore::Node* nodeRespondingToClick, const CyberCore::FloatPoint& location);
    void completeSyntheticClick(CyberCore::Node* nodeRespondingToClick, const CyberCore::FloatPoint& location, CyberCore::SyntheticClickType);
    void sendTapHighlightForNodeIfNecessary(uint64_t requestID, CyberCore::Node*);
    void resetTextAutosizing();
    CyberCore::VisiblePosition visiblePositionInFocusedNodeForPoint(const CyberCore::Frame&, const CyberCore::IntPoint&, bool isInteractingWithFocusedElement);
    RefPtr<CyberCore::Range> rangeForGranularityAtPoint(CyberCore::Frame&, const CyberCore::IntPoint&, uint32_t granularity, bool isInteractingWithFocusedElement);
#endif

#if PLATFORM(IOS_FAMILY) && ENABLE(DATA_INTERACTION)
    void requestStartDataInteraction(const CyberCore::IntPoint& clientPosition, const CyberCore::IntPoint& globalPosition);
    void requestAdditionalItemsForDragSession(const CyberCore::IntPoint& clientPosition, const CyberCore::IntPoint& globalPosition);
#endif

#if !PLATFORM(COCOA) && !PLATFORM(WPE)
    static const char* interpretKeyEvent(const CyberCore::KeyboardEvent*);
#endif

    bool performDefaultBehaviorForKeyEvent(const WebKeyboardEvent&);

#if PLATFORM(MAC)
    bool executeKeypressCommandsInternal(const Vector<CyberCore::KeypressCommand>&, CyberCore::KeyboardEvent*);
#endif

    void updateDrawingAreaLayerTreeFreezeState();
    bool markLayersVolatileImmediatelyIfPossible();
    void layerVolatilityTimerFired();
    void callVolatilityCompletionHandlers(bool succeeded);

    String sourceForFrame(WebFrame*);

    void loadDataImpl(uint64_t navigationID, bool shouldTreatAsContinuingLoad, Optional<WebsitePoliciesData>&&, Ref<CyberCore::SharedBuffer>&&, const String& MIMEType, const String& encodingName, const URL& baseURL, const URL& failingURL, const UserData&);

    // Actions
    void tryClose();
    void platformDidReceiveLoadParameters(const LoadParameters&);
    void loadRequest(LoadParameters&&);
    void loadData(LoadParameters&&);
    void loadAlternateHTML(LoadParameters&&);
    void navigateToPDFLinkWithSimulatedClick(const String& url, CyberCore::IntPoint documentPoint, CyberCore::IntPoint screenPoint);
    void reload(uint64_t navigationID, uint32_t reloadOptions, SandboxExtension::Handle&&);
    void goToBackForwardItem(uint64_t navigationID, const CyberCore::BackForwardItemIdentifier&, CyberCore::FrameLoadType, CyberCore::ShouldTreatAsContinuingLoad, Optional<WebsitePoliciesData>&&);
    void tryRestoreScrollPosition();
    void setInitialFocus(bool forward, bool isKeyboardEventValid, const WebKeyboardEvent&, CallbackID);
    void updateIsInWindow(bool isInitialState = false);
    void visibilityDidChange();
    void setActivityState(OptionSet<CyberCore::ActivityState::Flag>, ActivityStateChangeID, const Vector<CallbackID>& callbackIDs);
    void validateCommand(const String&, CallbackID);
    void executeEditCommand(const String&, const String&);
    void setEditable(bool);

    void increaseListLevel();
    void decreaseListLevel();
    void changeListType();

    void setBaseWritingDirection(CyberCore::WritingDirection);

    void setNeedsFontAttributes(bool);

    void mouseEvent(const WebMouseEvent&);
    void keyEvent(const WebKeyboardEvent&);

#if ENABLE(IOS_TOUCH_EVENTS)
    void touchEventSync(const WebTouchEvent&, bool& handled);
    void updatePotentialTapSecurityOrigin(const WebTouchEvent&, bool wasHandled);
#elif ENABLE(TOUCH_EVENTS)
    void touchEvent(const WebTouchEvent&);
#endif

#if ENABLE(CONTEXT_MENUS)
    void contextMenuHidden() { m_isShowingContextMenu = false; }
    void contextMenuForKeyEvent();
#endif

    static bool scroll(CyberCore::Page*, CyberCore::ScrollDirection, CyberCore::ScrollGranularity);
    static bool logicalScroll(CyberCore::Page*, CyberCore::ScrollLogicalDirection, CyberCore::ScrollGranularity);

    void loadURLInFrame(URL&&, uint64_t frameID);

    enum class WasRestoredByAPIRequest { No, Yes };
    void restoreSessionInternal(const Vector<BackForwardListItemState>&, WasRestoredByAPIRequest, WebBackForwardListProxy::OverwriteExistingItem);
    void restoreSession(const Vector<BackForwardListItemState>&);
    void didRemoveBackForwardItem(const CyberCore::BackForwardItemIdentifier&);
    void updateBackForwardListForReattach(const Vector<CyberKit::BackForwardListItemState>&);
    void setCurrentHistoryItemForReattach(CyberKit::BackForwardListItemState&&);

    void requestFontAttributesAtSelectionStart(CallbackID);

#if ENABLE(REMOTE_INSPECTOR)
    void setIndicating(bool);
#endif

    void setDrawsBackground(bool);

#if PLATFORM(COCOA)
    void setTopContentInsetFenced(float, IPC::Attachment);
#endif
    void setTopContentInset(float);

    void viewWillStartLiveResize();
    void viewWillEndLiveResize();

    void getContentsAsString(CallbackID);
#if ENABLE(MHTML)
    void getContentsAsMHTMLData(CallbackID);
#endif
    void getMainResourceDataOfFrame(uint64_t frameID, CallbackID);
    void getResourceDataFromFrame(uint64_t frameID, const String& resourceURL, CallbackID);
    void getRenderTreeExternalRepresentation(CallbackID);
    void getSelectionOrContentsAsString(CallbackID);
    void getSelectionAsWebArchiveData(CallbackID);
    void getSourceForFrame(uint64_t frameID, CallbackID);
    void getWebArchiveOfFrame(uint64_t frameID, CallbackID);
    void runJavaScript(const String&, bool forceUserGesture, Optional<String> worldName, CallbackID);
    void runJavaScriptInMainFrame(const String&, bool forceUserGesture, CallbackID);
    void runJavaScriptInMainFrameScriptWorld(const String&, bool forceUserGesture, const String& worldName, CallbackID);
    void forceRepaint(CallbackID);
    void takeSnapshot(CyberCore::IntRect snapshotRect, CyberCore::IntSize bitmapSize, uint32_t options, CallbackID);

    void preferencesDidChange(const WebPreferencesStore&);
    void updatePreferences(const WebPreferencesStore&);
    void updatePreferencesGenerated(const WebPreferencesStore&);

#if PLATFORM(IOS_FAMILY)
    bool parentProcessHasServiceWorkerEntitlement() const;
#else
    bool parentProcessHasServiceWorkerEntitlement() const { return true; }
#endif

    void didReceivePolicyDecision(uint64_t frameID, uint64_t listenerID, CyberCore::PolicyCheckIdentifier, CyberCore::PolicyAction, uint64_t navigationID, const DownloadID&, Optional<WebsitePoliciesData>&&);
    void continueWillSubmitForm(uint64_t frameID, uint64_t listenerID);
    void setUserAgent(const String&);
    void setCustomTextEncodingName(const String&);
    void suspendActiveDOMObjectsAndAnimations();
    void resumeActiveDOMObjectsAndAnimations();

#if PLATFORM(COCOA)
    void performDictionaryLookupAtLocation(const CyberCore::FloatPoint&);
    void performDictionaryLookupOfCurrentSelection();
    void performDictionaryLookupForRange(CyberCore::Frame&, CyberCore::Range&, NSDictionary *options, CyberCore::TextIndicatorPresentationTransition);
    CyberCore::DictionaryPopupInfo dictionaryPopupInfoForRange(CyberCore::Frame&, CyberCore::Range&, NSDictionary *options, CyberCore::TextIndicatorPresentationTransition);
#if ENABLE(PDFKIT_PLUGIN)
    CyberCore::DictionaryPopupInfo dictionaryPopupInfoForSelectionInPDFPlugin(PDFSelection *, PDFPlugin&, NSDictionary *options, CyberCore::TextIndicatorPresentationTransition);
#endif

    void windowAndViewFramesChanged(const CyberCore::FloatRect& windowFrameInScreenCoordinates, const CyberCore::FloatRect& windowFrameInUnflippedScreenCoordinates, const CyberCore::FloatRect& viewFrameInWindowCoordinates, const CyberCore::FloatPoint& accessibilityViewCoordinates);

    RetainPtr<PDFDocument> pdfDocumentForPrintingFrame(CyberCore::Frame*);
    void computePagesForPrintingPDFDocument(uint64_t frameID, const PrintInfo&, Vector<CyberCore::IntRect>& resultPageRects);
    void drawPDFDocument(CGContextRef, PDFDocument *, const PrintInfo&, const CyberCore::IntRect&);
    void drawPagesToPDFFromPDFDocument(CGContextRef, PDFDocument *, const PrintInfo&, uint32_t first, uint32_t count);
#endif

    void setMainFrameIsScrollable(bool);

    void unapplyEditCommand(WebUndoStepID commandID);
    void reapplyEditCommand(WebUndoStepID commandID);
    void didRemoveEditCommand(WebUndoStepID commandID);

    void findString(const String&, uint32_t findOptions, uint32_t maxMatchCount);
    void findStringMatches(const String&, uint32_t findOptions, uint32_t maxMatchCount);
    void getImageForFindMatch(uint32_t matchIndex);
    void selectFindMatch(uint32_t matchIndex);
    void hideFindUI();
    void countStringMatches(const String&, uint32_t findOptions, uint32_t maxMatchCount);
    void replaceMatches(const Vector<uint32_t>& matchIndices, const String& replacementText, bool selectionOnly, CallbackID);

#if USE(COORDINATED_GRAPHICS)
    void sendViewportAttributesChanged(const CyberCore::ViewportArguments&);
#endif

    void didChangeSelectedIndexForActivePopupMenu(int32_t newIndex);
    void setTextForActivePopupMenu(int32_t index);

#if PLATFORM(GTK)
    void failedToShowPopupMenu();
#endif

    void didChooseFilesForOpenPanel(const Vector<String>&);
    void didCancelForOpenPanel();

#if PLATFORM(IOS_FAMILY)
    void didChooseFilesForOpenPanelWithDisplayStringAndIcon(const Vector<String>&, const String& displayString, const IPC::DataReference& iconData);
#endif

#if ENABLE(SANDBOX_EXTENSIONS)
    void extendSandboxForFilesFromOpenPanel(SandboxExtension::HandleArray&&);
#endif

    void didReceiveGeolocationPermissionDecision(uint64_t geolocationID, bool allowed);

    void didReceiveNotificationPermissionDecision(uint64_t notificationID, bool allowed);

#if ENABLE(MEDIA_STREAM)
    void userMediaAccessWasGranted(uint64_t userMediaID, CyberCore::CaptureDevice&& audioDeviceUID, CyberCore::CaptureDevice&& videoDeviceUID, String&& mediaDeviceIdentifierHashSalt);
    void userMediaAccessWasDenied(uint64_t userMediaID, uint64_t reason, String&& invalidConstraint);

    void didCompleteMediaDeviceEnumeration(uint64_t userMediaID, const Vector<CyberCore::CaptureDevice>& devices, String&& deviceIdentifierHashSalt, bool originHasPersistentAccess);
#endif

#if ENABLE(MEDIA_STREAM) && ENABLE(SANDBOX_EXTENSIONS)
    void grantUserMediaDeviceSandboxExtensions(MediaDeviceSandboxExtensions&&);
    void revokeUserMediaDeviceSandboxExtensions(const Vector<String>&);
#endif

#if ENABLE(WEB_RTC)
    void disableICECandidateFiltering();
    void enableICECandidateFiltering();
#endif

#if ENABLE(WEB_RTC) && USE(LIBWEBRTC)
    void disableEnumeratingAllNetworkInterfaces();
    void enableEnumeratingAllNetworkInterfaces();
#endif

    void stopAllMediaPlayback();
    void suspendAllMediaPlayback();
    void resumeAllMediaPlayback();

    void advanceToNextMisspelling(bool startBeforeSelection);
    void changeSpellingToWord(const String& word);

#if USE(APPKIT)
    void uppercaseWord();
    void lowercaseWord();
    void capitalizeWord();
#endif

#if ENABLE(CONTEXT_MENUS)
    void didSelectItemFromActiveContextMenu(const WebContextMenuItemData&);
#endif

    void changeSelectedIndex(int32_t index);
    void setCanStartMediaTimerFired();

    static bool platformCanHandleRequest(const CyberCore::ResourceRequest&);

    static PluginView* focusedPluginViewForFrame(CyberCore::Frame&);

    static RefPtr<CyberCore::Range> rangeFromEditingRange(CyberCore::Frame&, const EditingRange&, EditingRangeIsRelativeTo = EditingRangeIsRelativeTo::EditableRoot);

    void reportUsedFeatures();

    void updateWebsitePolicies(WebsitePoliciesData&&);

    void changeFont(CyberCore::FontChanges&&);
    void changeFontAttributes(CyberCore::FontAttributeChanges&&);

#if PLATFORM(MAC)
    void performImmediateActionHitTestAtLocation(CyberCore::FloatPoint);
    std::tuple<RefPtr<CyberCore::Range>, NSDictionary *> lookupTextAtLocation(CyberCore::FloatPoint);
    void immediateActionDidUpdate();
    void immediateActionDidCancel();
    void immediateActionDidComplete();

    void dataDetectorsDidPresentUI(CyberCore::PageOverlay::PageOverlayID);
    void dataDetectorsDidChangeUI(CyberCore::PageOverlay::PageOverlayID);
    void dataDetectorsDidHideUI(CyberCore::PageOverlay::PageOverlayID);

    void handleAcceptedCandidate(CyberCore::TextCheckingResult);
#endif

#if PLATFORM(COCOA)
    void requestActiveNowPlayingSessionInfo(CallbackID);
    RetainPtr<NSData> accessibilityRemoteTokenData() const;
    void accessibilityTransferRemoteToken(RetainPtr<NSData>);
#endif

    void setShouldDispatchFakeMouseMoveEvents(bool dispatch) { m_shouldDispatchFakeMouseMoveEvents = dispatch; }

#if ENABLE(WIRELESS_PLAYBACK_TARGET) && !PLATFORM(IOS_FAMILY)
    void playbackTargetSelected(uint64_t, const CyberCore::MediaPlaybackTargetContext& outputDevice) const;
    void playbackTargetAvailabilityDidChange(uint64_t, bool);
    void setShouldPlayToPlaybackTarget(uint64_t, bool);
#endif

    void clearWheelEventTestTrigger();

    void setShouldScaleViewToFitDocument(bool);

    void pageStoppedScrolling();

#if ENABLE(VIDEO) && USE(GSTREAMER)
    void didEndRequestInstallMissingMediaPlugins(uint32_t result);
#endif

    void setResourceCachingDisabled(bool);
    void setUserInterfaceLayoutDirection(uint32_t);

    bool canPluginHandleResponse(const CyberCore::ResourceResponse&);

#if USE(QUICK_LOOK)
    void didReceivePasswordForQuickLookDocument(const String&);
#endif

    void simulateDeviceOrientationChange(double alpha, double beta, double gamma);

    void frameBecameRemote(uint64_t frameID, CyberCore::GlobalFrameIdentifier&& remoteFrameIdentifier, CyberCore::GlobalWindowIdentifier&& remoteWindowIdentifier);

    void registerURLSchemeHandler(uint64_t identifier, const String& scheme);

    void urlSchemeTaskDidPerformRedirection(uint64_t handlerIdentifier, uint64_t taskIdentifier, CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&);
    void urlSchemeTaskDidReceiveResponse(uint64_t handlerIdentifier, uint64_t taskIdentifier, const CyberCore::ResourceResponse&);
    void urlSchemeTaskDidReceiveData(uint64_t handlerIdentifier, uint64_t taskIdentifier, const IPC::DataReference&);
    void urlSchemeTaskDidComplete(uint64_t handlerIdentifier, uint64_t taskIdentifier, const CyberCore::ResourceError&);

    void setIsSuspended(bool);

    RefPtr<WebImage> snapshotAtSize(const CyberCore::IntRect&, const CyberCore::IntSize& bitmapSize, SnapshotOptions);
    RefPtr<WebImage> snapshotNode(CyberCore::Node&, SnapshotOptions, unsigned maximumPixelCount = std::numeric_limits<unsigned>::max());
#if USE(CF)
    RetainPtr<CFDataRef> pdfSnapshotAtSize(const CyberCore::IntRect&, const CyberCore::IntSize& bitmapSize, SnapshotOptions);
#endif

#if ENABLE(ATTACHMENT_ELEMENT)
    RefPtr<CyberCore::HTMLAttachmentElement> attachmentElementWithIdentifier(const String& identifier) const;
#endif

    bool canShowMIMEType(const String&, const Function<bool(const String&, CyberCore::PluginData::AllowedPluginTypes)>& supportsPlugin) const;

    uint64_t m_pageID;

    std::unique_ptr<CyberCore::Page> m_page;
    RefPtr<WebFrame> m_mainFrame;

    RefPtr<WebPageGroupProxy> m_pageGroup;

    String m_userAgent;

    CyberCore::IntSize m_viewSize;
    std::unique_ptr<DrawingArea> m_drawingArea;
    bool m_shouldResetDrawingAreaAfterSuspend { false };

    HashSet<PluginView*> m_pluginViews;
    bool m_hasSeenPlugin { false };

    HashMap<uint64_t, RefPtr<CyberCore::TextCheckingRequest>> m_pendingTextCheckingRequestMap;

    bool m_useFixedLayout { false };
    bool m_drawsBackground { true };

    CyberCore::Color m_underlayColor;

    bool m_isInRedo { false };
    bool m_isClosed { false };
    bool m_tabToLinks { false };
    
    bool m_asynchronousPluginInitializationEnabled { false };
    bool m_asynchronousPluginInitializationEnabledForAllPlugins { false };
    bool m_artificialPluginInitializationDelayEnabled { false };
    bool m_scrollingPerformanceLoggingEnabled { false };
    bool m_mainFrameIsScrollable { true };

    bool m_alwaysShowsHorizontalScroller { false };
    bool m_alwaysShowsVerticalScroller { false };

#if PLATFORM(IOS_FAMILY)
    bool m_ignoreViewportScalingConstraints { false };
#endif

#if ENABLE(PRIMARY_SNAPSHOTTED_PLUGIN_HEURISTIC)
    bool m_readyToFindPrimarySnapshottedPlugin { false };
    bool m_didFindPrimarySnapshottedPlugin { false };
    unsigned m_numberOfPrimarySnapshotDetectionAttempts { 0 };
    String m_primaryPlugInPageOrigin;
    String m_primaryPlugInOrigin;
    String m_primaryPlugInMimeType;
    RunLoop::Timer<WebPage> m_determinePrimarySnapshottedPlugInTimer;
#endif

    // The layer hosting mode.
    LayerHostingMode m_layerHostingMode;

#if PLATFORM(COCOA)
    bool m_pdfPluginEnabled { false };
    bool m_hasCachedWindowFrame { false };

    // The frame of the containing window in screen coordinates.
    CyberCore::FloatRect m_windowFrameInScreenCoordinates;

    // The frame of the containing window in unflipped screen coordinates.
    CyberCore::FloatRect m_windowFrameInUnflippedScreenCoordinates;

    // The frame of the view in window coordinates.
    CyberCore::FloatRect m_viewFrameInWindowCoordinates;

    // The accessibility position of the view.
    CyberCore::FloatPoint m_accessibilityPosition;
    
    RetainPtr<WKAccessibilityWebPageObject> m_mockAccessibilityElement;

    std::unique_ptr<ViewGestureGeometryCollector> m_viewGestureGeometryCollector;

    RetainPtr<NSDictionary> m_dataDetectionContext;
#endif

#if HAVE(ACCESSIBILITY) && PLATFORM(GTK)
    GRefPtr<WebPageAccessibilityObject> m_accessibilityObject;
#endif

#if PLATFORM(GTK) && USE(TEXTURE_MAPPER_GL)
    // Our view's window in the UI process.
    uint64_t m_nativeWindowHandle { 0 };
#endif

#if !PLATFORM(IOS_FAMILY)
    RefPtr<PageBanner> m_headerBanner;
    RefPtr<PageBanner> m_footerBanner;
#endif

    RunLoop::Timer<WebPage> m_setCanStartMediaTimer;
    bool m_mayStartMediaWhenInWindow { false };

    HashMap<WebUndoStepID, RefPtr<WebUndoStep>> m_undoStepMap;

#if ENABLE(CONTEXT_MENUS)
    std::unique_ptr<API::InjectedBundle::PageContextMenuClient> m_contextMenuClient;
#endif
    std::unique_ptr<API::InjectedBundle::EditorClient> m_editorClient;
    std::unique_ptr<API::InjectedBundle::FormClient> m_formClient;
    std::unique_ptr<API::InjectedBundle::PageLoaderClient> m_loaderClient;
    InjectedBundlePagePolicyClient m_policyClient;
    std::unique_ptr<API::InjectedBundle::ResourceLoadClient> m_resourceLoadClient;
    std::unique_ptr<API::InjectedBundle::PageUIClient> m_uiClient;
#if ENABLE(FULLSCREEN_API)
    InjectedBundlePageFullScreenClient m_fullScreenClient;
#endif

    UniqueRef<FindController> m_findController;

    RefPtr<WebInspector> m_inspector;
    RefPtr<WebInspectorUI> m_inspectorUI;
    RefPtr<RemoteWebInspectorUI> m_remoteInspectorUI;
    std::unique_ptr<WebPageInspectorTargetController> m_inspectorTargetController;

#if (PLATFORM(IOS_FAMILY) && HAVE(AVKIT)) || (PLATFORM(MAC) && ENABLE(VIDEO_PRESENTATION_MODE))
    RefPtr<PlaybackSessionManager> m_playbackSessionManager;
    RefPtr<VideoFullscreenManager> m_videoFullscreenManager;
#endif

#if PLATFORM(IOS_FAMILY)
    bool m_allowsMediaDocumentInlinePlayback { false };
    RefPtr<CyberCore::Range> m_startingGestureRange;
#endif

#if ENABLE(FULLSCREEN_API)
    RefPtr<WebFullScreenManager> m_fullScreenManager;
#endif

    RefPtr<WebPopupMenu> m_activePopupMenu;

#if ENABLE(CONTEXT_MENUS)
    RefPtr<WebContextMenu> m_contextMenu;
#endif

#if ENABLE(INPUT_TYPE_COLOR)
    WebColorChooser* m_activeColorChooser { nullptr };
#endif

#if ENABLE(DATALIST_ELEMENT)
    WeakPtr<WebDataListSuggestionPicker> m_activeDataListSuggestionPicker;
#endif

    RefPtr<WebOpenPanelResultListener> m_activeOpenPanelResultListener;
    RefPtr<NotificationPermissionRequestManager> m_notificationPermissionRequestManager;

    Ref<WebUserContentController> m_userContentController;

#if ENABLE(GEOLOCATION)
    UniqueRef<GeolocationPermissionRequestManager> m_geolocationPermissionRequestManager;
#endif

#if ENABLE(MEDIA_STREAM)
    UniqueRef<UserMediaPermissionRequestManager> m_userMediaPermissionRequestManager;
#endif

    std::unique_ptr<CyberCore::PrintContext> m_printContext;
#if PLATFORM(GTK)
    RefPtr<WebPrintOperationGtk> m_printOperation;
#endif

    SandboxExtensionTracker m_sandboxExtensionTracker;

    RefPtr<SandboxExtension> m_pendingDropSandboxExtension;
    Vector<RefPtr<SandboxExtension>> m_pendingDropExtensionsForFileUpload;

    PAL::HysteresisActivity m_pageScrolledHysteresis;

    bool m_canRunBeforeUnloadConfirmPanel { false };

    bool m_canRunModal { false };
    bool m_isRunningModal { false };

#if ENABLE(DRAG_SUPPORT)
    bool m_isStartingDrag { false };
#endif

    bool m_cachedMainFrameIsPinnedToLeftSide { true };
    bool m_cachedMainFrameIsPinnedToRightSide { true };
    bool m_cachedMainFrameIsPinnedToTopSide { true };
    bool m_cachedMainFrameIsPinnedToBottomSide { true };
    bool m_canShortCircuitHorizontalWheelEvents { false };
    bool m_hasWheelEventHandlers { false };

    unsigned m_cachedPageCount { 0 };

    HashSet<unsigned long> m_trackedNetworkResourceRequestIdentifiers;

    CyberCore::IntSize m_viewLayoutSize;
    bool m_autoSizingShouldExpandToViewHeight { false };
    Optional<CyberCore::IntSize> m_viewportSizeForCSSViewportUnits;

    bool m_userIsInteracting { false };
    bool m_isFocusingElementDueToUserInteraction { false };
    bool m_hasEverFocusedElementDueToUserInteractionSincePageTransition { false };
    bool m_needsHiddenContentEditableQuirk { false };
    bool m_needsPlainTextQuirk { false };
    bool m_changingActivityState { false };

#if ENABLE(CONTEXT_MENUS)
    bool m_isShowingContextMenu { false };
#endif

    RefPtr<CyberCore::Element> m_focusedElement;
    bool m_hasPendingBlurNotification { false };
    bool m_hasPendingEditorStateUpdate { false };
    
#if PLATFORM(IOS_FAMILY)
    RefPtr<CyberCore::Range> m_currentWordRange;
    RefPtr<CyberCore::Node> m_interactionNode;
    CyberCore::IntPoint m_lastInteractionLocation;
    
    enum SelectionAnchor { Start, End };
    SelectionAnchor m_selectionAnchor { Start };

    RefPtr<CyberCore::Node> m_potentialTapNode;
    CyberCore::FloatPoint m_potentialTapLocation;
    RefPtr<CyberCore::SecurityOrigin> m_potentialTapSecurityOrigin;

    CyberCore::ViewportConfiguration m_viewportConfiguration;

    bool m_hasReceivedVisibleContentRectsAfterDidCommitLoad { false };
    bool m_hasRestoredExposedContentRectAfterDidCommitLoad { false };
    bool m_scaleWasSetByUIProcess { false };
    bool m_userHasChangedPageScaleFactor { false };
    bool m_hasStablePageScaleFactor { true };
    bool m_useTestingViewportConfiguration { false };
    bool m_isInStableState { true };
    bool m_forceAlwaysUserScalable { false };
    MonotonicTime m_oldestNonStableUpdateVisibleContentRectsTimestamp;
    Seconds m_estimatedLatency { 0 };
    CyberCore::FloatSize m_screenSize;
    CyberCore::FloatSize m_availableScreenSize;
    CyberCore::FloatSize m_overrideScreenSize;
    RefPtr<CyberCore::Range> m_currentBlockSelection;
    CyberCore::IntRect m_blockRectForTextSelection;

    RefPtr<CyberCore::Range> m_initialSelection;
    CyberCore::VisibleSelection m_storedSelectionForAccessibility { CyberCore::VisibleSelection() };
    CyberCore::IntSize m_blockSelectionDesiredSize;
    CyberCore::FloatSize m_maximumUnobscuredSize;
    int32_t m_deviceOrientation { 0 };
    bool m_inDynamicSizeUpdate { false };
    HashMap<std::pair<CyberCore::IntSize, double>, CyberCore::IntPoint> m_dynamicSizeUpdateHistory;
    RefPtr<CyberCore::Node> m_pendingSyntheticClickNode;
    CyberCore::FloatPoint m_pendingSyntheticClickLocation;
    CyberCore::FloatRect m_previousExposedContentRect;
    FocusedElementIdentifier m_currentFocusedElementIdentifier { 0 };
    Optional<DynamicViewportSizeUpdateID> m_pendingDynamicViewportSizeUpdateID;
    double m_lastTransactionPageScaleFactor { 0 };
    uint64_t m_lastTransactionIDWithScaleChange { 0 };
#endif

    CyberCore::Timer m_layerVolatilityTimer;
    Vector<WTF::Function<void (bool)>> m_markLayersAsVolatileCompletionHandlers;
    bool m_isSuspendedUnderLock { false };

    HashSet<String, ASCIICaseInsensitiveHash> m_mimeTypesWithCustomContentProviders;
    CyberCore::Color m_backgroundColor { CyberCore::Color::white };

    HashSet<unsigned> m_activeRenderingSuppressionTokens;
    unsigned m_maximumRenderingSuppressionToken { 0 };
    
    CyberCore::ScrollPinningBehavior m_scrollPinningBehavior { CyberCore::DoNotPin };
    Optional<CyberCore::ScrollbarOverlayStyle> m_scrollbarOverlayStyle;

    bool m_useAsyncScrolling { false };

    OptionSet<CyberCore::ActivityState::Flag> m_activityState;

    bool m_processSuppressionEnabled;
    UserActivity m_userActivity;
    PAL::HysteresisActivity m_userActivityHysteresis;

    uint64_t m_pendingNavigationID { 0 };
    Optional<WebsitePoliciesData> m_pendingWebsitePolicies;

    bool m_mainFrameProgressCompleted { false };
    bool m_shouldDispatchFakeMouseMoveEvents { true };
    bool m_isEditorStateMissingPostLayoutData { false };
    bool m_isSelectingTextWhileInsertingAsynchronously { false };

    enum class EditorStateIsContentEditable { No, Yes, Unset };
    mutable EditorStateIsContentEditable m_lastEditorStateWasContentEditable { EditorStateIsContentEditable::Unset };

#if PLATFORM(GTK)
    bool m_inputMethodEnabled { false };
#endif

#if ENABLE(VIDEO) && USE(GSTREAMER)
    RefPtr<CyberCore::MediaPlayerRequestInstallMissingPluginsCallback> m_installMediaPluginsCallback;
#endif

#if USE(OS_STATE)
    WallTime m_loadCommitTime;
#endif

    CyberCore::UserInterfaceLayoutDirection m_userInterfaceLayoutDirection { CyberCore::UserInterfaceLayoutDirection::LTR };

    const String m_overrideContentSecurityPolicy;
    const Optional<double> m_cpuLimit;

    HashMap<String, RefPtr<WebURLSchemeHandlerProxy>> m_schemeToURLSchemeHandlerProxyMap;
    HashMap<uint64_t, WebURLSchemeHandlerProxy*> m_identifierToURLSchemeHandlerProxyMap;

    HashMap<ShareSheetCallbackID, WTF::Function<void(bool completed)>> m_shareSheetResponseCallbackMap;

#if ENABLE(APPLICATION_MANIFEST)
    HashMap<uint64_t, uint64_t> m_applicationManifestFetchCallbackMap;
#endif

    OptionSet<LayerTreeFreezeReason> m_LayerTreeFreezeReasons;
    bool m_isSuspended { false };
    bool m_needsFontAttributes { false };
#if PLATFORM(COCOA)
    WeakPtr<RemoteObjectRegistry> m_remoteObjectRegistry;
#endif
};

} // namespace CyberKit

