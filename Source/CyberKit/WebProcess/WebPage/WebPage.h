/*
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

#include "APIInjectedBundleEditorClient.h"
#include "APIInjectedBundleFormClient.h"
#include "APIInjectedBundlePageContextMenuClient.h"
#include "APIInjectedBundlePageLoaderClient.h"
#include "APIInjectedBundlePageResourceLoadClient.h"
#include "APIInjectedBundlePageUIClient.h"
#include "APIObject.h"
#include "CallbackID.h"
#include "Connection.h"
#include "ContentAsStringIncludesChildFrames.h"
#include "ContentWorldShared.h"
#include "DataReference.h"
#include "DownloadID.h"
#include "DrawingAreaInfo.h"
#include "EditingRange.h"
#include "EventDispatcher.h"
#include "GeolocationIdentifier.h"
#include "IdentifierTypes.h"
#include "InjectedBundlePageContextMenuClient.h"
#include "InjectedBundlePageFullScreenClient.h"
#include "LayerTreeContext.h"
#include "MediaPlaybackState.h"
#include "MessageReceiver.h"
#include "MessageSender.h"
#include "NetworkResourceLoadIdentifier.h"
#include "PDFPluginIdentifier.h"
#include "SandboxExtension.h"
#include "ShareableBitmap.h"
#include "SharedMemory.h"
#include "StorageNamespaceIdentifier.h"
#include "TransactionID.h"
#include "UserContentControllerIdentifier.h"
#include "UserData.h"
#include "WebBackForwardListProxy.h"
#include "WebPageProxyIdentifier.h"
#include "WebURLSchemeHandlerIdentifier.h"
#include "WebUndoStepID.h"
#include "WebsitePoliciesData.h"
#include <CyberScriptCore/InspectorFrontendChannel.h>
#include <CyberCore/ActivityState.h>
#include <CyberCore/AppHighlight.h>
#include <CyberCore/DiagnosticLoggingClient.h>
#include <CyberCore/DictationContext.h>
#include <CyberCore/DictionaryPopupInfo.h>
#include <CyberCore/DisabledAdaptations.h>
#include <CyberCore/DragActions.h>
#include <CyberCore/FrameIdentifier.h>
#include <CyberCore/FrameLoaderTypes.h>
#include <CyberCore/HTMLMenuItemElement.h>
#include <CyberCore/HighlightVisibility.h>
#include <CyberCore/IntSizeHash.h>
#include <CyberCore/LayerHostingContextIdentifier.h>
#include <CyberCore/MediaControlsContextMenuItem.h>
#include <CyberCore/MediaKeySystemRequest.h>
#include <CyberCore/Page.h>
#include <CyberCore/PageIdentifier.h>
#include <CyberCore/PageOverlay.h>
#include <CyberCore/PlatformLayerIdentifier.h>
#include <CyberCore/PlaybackTargetClientContextIdentifier.h>
#include <CyberCore/PluginData.h>
#include <CyberCore/PointerCharacteristics.h>
#include <CyberCore/PointerID.h>
#include <CyberCore/RectEdges.h>
#include <CyberCore/SecurityPolicyViolationEvent.h>
#include <CyberCore/ShareData.h>
#include <CyberCore/SimpleRange.h>
#include <CyberCore/SubstituteData.h>
#include <CyberCore/TextManipulationController.h>
#include <CyberCore/TextManipulationItem.h>
#include <CyberCore/UserActivity.h>
#include <CyberCore/UserContentTypes.h>
#include <CyberCore/UserInterfaceLayoutDirection.h>
#include <CyberCore/UserMediaRequestIdentifier.h>
#include <CyberCore/UserScriptTypes.h>
#include <CyberCore/VisibilityState.h>
#include <CyberCore/CyberCoreKeyboardUIMode.h>
#include <memory>
#include <pal/HysteresisActivity.h>
#include <wtf/HashMap.h>
#include <wtf/MonotonicTime.h>
#include <wtf/OptionSet.h>
#include <wtf/RefPtr.h>
#include <wtf/RunLoop.h>
#include <wtf/Seconds.h>
#include <wtf/WallTime.h>
#include <wtf/WeakHashSet.h>
#include <wtf/text/WTFString.h>

#if USE(ATSPI)
#include <CyberCore/AccessibilityRootAtspi.h>
#endif

#if PLATFORM(GTK)
#include "ArgumentCodersGtk.h"
#include "WebPrintOperationGtk.h"
#endif

#if PLATFORM(GTK) || PLATFORM(WPE)
#include "InputMethodState.h"
#endif

#if PLATFORM(IOS_FAMILY)
#include "DynamicViewportSizeUpdate.h"
#include "GestureTypes.h"
#include <CyberCore/InspectorOverlay.h>
#include <CyberCore/IntPointHash.h>
#include <CyberCore/WKContentObservation.h>
#endif

#if ENABLE(META_VIEWPORT)
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

#if ENABLE(NETWORK_CONNECTION_INTEGRITY)
#include <CyberCore/LookalikeCharactersSanitizationData.h>
#endif

#if ENABLE(MAC_GESTURE_EVENTS)
#include <CyberKitAdditions/PlatformGestureEventMac.h>
#endif

#if ENABLE(MEDIA_USAGE)
#include <CyberCore/MediaSessionIdentifier.h>
#endif

#if PLATFORM(COCOA)
#include <CyberCore/VisibleSelection.h>
#include <wtf/RetainPtr.h>
OBJC_CLASS NSArray;
OBJC_CLASS NSDictionary;
OBJC_CLASS NSObject;
OBJC_CLASS PDFDocument;
OBJC_CLASS PDFSelection;
OBJC_CLASS RVItem;
OBJC_CLASS WKAccessibilityWebPageObject;
#endif

#define ENABLE_VIEWPORT_RESIZING PLATFORM(IOS_FAMILY)

namespace WTF {
enum class Critical : bool;
}

namespace API {
class Array;
}

namespace IPC {
class Connection;
class Decoder;
class FormDataReference;
class SharedBufferReference;
}

namespace CyberCore {

class AbstractFrame;
class CachedPage;
class CaptureDevice;
class DocumentLoader;
class DragData;
class WeakPtrImplWithEventTargetData;
class FontAttributeChanges;
class FontChanges;
class FrameSelection;
class GraphicsContext;
class HTMLElement;
class HTMLImageElement;
class HTMLMenuElement;
class HTMLMenuItemElement;
class HTMLPlugInElement;
class HTMLVideoElement;
class IgnoreSelectionChangeForScope;
class IntPoint;
class IntRect;
class KeyboardEvent;
class LocalFrame;
class LocalFrameView;
class MediaPlaybackTargetContext;
class MediaSessionCoordinator;
class Page;
class PolicyDecision;
class PrintContext;
class Range;
class RenderImage;
class Report;
class ResourceRequest;
class ResourceResponse;
class ScrollingCoordinator;
class SelectionData;
class SelectionGeometry;
class SharedBuffer;
class FragmentedSharedBuffer;
class SubstituteData;
class TextCheckingRequest;
class VisiblePosition;

enum SyntheticClickType : int8_t;
enum class COEPDisposition : bool;
enum class CaretAnimatorType : uint8_t;
enum class CreateNewGroupForHighlight : bool;
enum class DOMPasteAccessCategory : uint8_t;
enum class DOMPasteAccessResponse : uint8_t;
enum class DragApplicationFlags : uint8_t;
enum class DragHandlingMethod : uint8_t;
enum class EventMakesGamepadsVisible : bool;
enum class FinalizeRenderingUpdateFlags : uint8_t;
enum class HighlightRequestOriginatedInApp : bool;
enum class LookalikeCharacterSanitizationTrigger : uint8_t;
enum class MediaProducerMediaCaptureKind : uint8_t;
enum class MediaProducerMediaState : uint32_t;
enum class MediaProducerMutedState : uint8_t;
enum class SelectionDirection : uint8_t;
enum class ShouldTreatAsContinuingLoad : uint8_t;
enum class TextIndicatorPresentationTransition : uint8_t;
enum class TextGranularity : uint8_t;
enum class WheelEventProcessingSteps : uint8_t;
enum class WritingDirection : uint8_t;
enum class ViolationReportType : uint8_t;

using MediaProducerMediaStateFlags = OptionSet<MediaProducerMediaState>;
using MediaProducerMutedStateFlags = OptionSet<MediaProducerMutedState>;
using PlatformDisplayID = uint32_t;

struct AttributedString;
struct CompositionHighlight;
struct CompositionUnderline;
struct ContactInfo;
struct ContactsRequestData;
struct DataDetectorElementInfo;
struct DictationAlternative;
struct ElementContext;
struct FontAttributes;
struct GlobalFrameIdentifier;
struct GlobalWindowIdentifier;
struct InteractionRegion;
struct KeypressCommand;
struct MediaUsageInfo;
struct PromisedAttachmentInfo;
struct RequestStorageAccessResult;
struct RunJavaScriptParameters;
struct TextCheckingResult;
struct TextRecognitionOptions;
struct TextRecognitionResult;
struct ViewportArguments;

#if ENABLE(ATTACHMENT_ELEMENT)
class HTMLAttachmentElement;
#endif

#if HAVE(TRANSLATION_UI_SERVICES) && ENABLE(CONTEXT_MENUS)
struct TranslationContextMenuInfo;
#endif
}

namespace CyberKit {

class DrawingArea;
class FindController;
class GPUProcessConnection;
class GamepadData;
class GeolocationPermissionRequestManager;
class InjectedBundleScriptWorld;
class LayerHostingContext;
class MediaDeviceSandboxExtensions;
class MediaKeySystemPermissionRequestManager;
class NotificationPermissionRequestManager;
class PDFPlugin;
class PageBanner;
class PluginView;
class RemoteMediaSessionCoordinator;
class RemoteRenderingBackendProxy;
class RemoteWebInspectorUI;
class TextCheckingControllerProxy;
class UserMediaPermissionRequestManager;
class ViewGestureGeometryCollector;
class WebColorChooser;
class WebContextMenu;
class WebContextMenuItemData;
class WebDataListSuggestionPicker;
class WebDateTimeChooser;
class WebDocumentLoader;
class WebEvent;
class WebFoundTextRangeController;
class PlaybackSessionManager;
class VideoFullscreenManager;
class WebBackForwardListItem;
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
class WebPaymentCoordinator;
class WebPopupMenu;
class WebRemoteObjectRegistry;
class WebScreenOrientationManager;
class WebTouchEvent;
class WebURLSchemeHandlerProxy;
class WebUndoStep;
class WebUserContentController;
class WebWheelEvent;
class RemoteLayerTreeTransaction;

enum class FindOptions : uint16_t;
enum class FindDecorationStyle : uint8_t;
enum class DragControllerAction : uint8_t;
enum class NavigatingToAppBoundDomain : bool;
enum class TextRecognitionUpdateResult : uint8_t;
enum class SyntheticEditingCommandType : uint8_t;

struct BackForwardListItemState;
struct DataDetectionResult;
struct DocumentEditingContext;
struct DocumentEditingContextRequest;
struct EditorState;
struct FrameTreeNodeData;
struct FocusedElementInformation;
struct FontInfo;
struct FrameTreeNodeData;
struct InsertTextOptions;
struct InteractionInformationAtPosition;
struct InteractionInformationRequest;
struct LoadParameters;
struct LocalFrameCreationParameters;
struct PrintInfo;
struct TextInputContext;
struct UserMessage;
struct WebAutocorrectionData;
struct WebAutocorrectionContext;
struct WebFoundTextRange;
struct WebPageCreationParameters;
struct WebPreferencesStore;

#if ENABLE(UI_SIDE_COMPOSITING)
class VisibleContentRectUpdateInfo;
#endif

#if ENABLE(REVEAL)
class RevealItem;
#endif

#if ENABLE(WK_WEB_EXTENSIONS)
class WebExtensionControllerProxy;
#endif

#if ENABLE(WEBXR) && !USE(OPENXR)
class PlatformXRSystemProxy;
#endif

using SnapshotOptions = uint32_t;
using WKEventModifiers = uint32_t;

class WebPage : public API::ObjectImpl<API::Object::Type::BundlePage>, public IPC::MessageReceiver, public IPC::MessageSender {
public:
    static Ref<WebPage> create(CyberCore::PageIdentifier, WebPageCreationParameters&&);

    virtual ~WebPage();

    void reinitializeWebPage(WebPageCreationParameters&&);

    void close();

    static WebPage* fromCorePage(CyberCore::Page&);

    CyberCore::Page* corePage() const { return m_page.get(); }
    CyberCore::PageIdentifier identifier() const { return m_identifier; }
    inline StorageNamespaceIdentifier sessionStorageNamespaceIdentifier() const;
    PAL::SessionID sessionID() const;
    bool usesEphemeralSession() const;

    void setSize(const CyberCore::IntSize&);
    const CyberCore::IntSize& size() const { return m_viewSize; }
    inline CyberCore::IntRect bounds() const;

    DrawingArea* drawingArea() const { return m_drawingArea.get(); }

#if ENABLE(ASYNC_SCROLLING)
    CyberCore::ScrollingCoordinator* scrollingCoordinator() const;
#endif

    WebPageGroupProxy* pageGroup() const { return m_pageGroup.get(); }

    void scrollMainFrameIfNotAtMaxScrollPosition(const CyberCore::IntSize& scrollOffset);

    bool scrollBy(uint32_t scrollDirection, CyberCore::ScrollGranularity);

    void centerSelectionInVisibleArea();

#if ENABLE(PDFKIT_PLUGIN)
    void createPDFHUD(PDFPlugin&, const CyberCore::IntRect&);
    void updatePDFHUDLocation(PDFPlugin&, const CyberCore::IntRect&);
    void removePDFHUD(PDFPlugin&);
    void zoomPDFIn(PDFPluginIdentifier);
    void zoomPDFOut(PDFPluginIdentifier);
    void savePDF(PDFPluginIdentifier, CompletionHandler<void(const String&, const URL&, const IPC::DataReference&)>&&);
    void openPDFWithPreview(PDFPluginIdentifier, CompletionHandler<void(const String&, FrameInfoData&&, const IPC::DataReference&, const String&)>&&);
#endif

#if PLATFORM(COCOA)
    void willCommitLayerTree(RemoteLayerTreeTransaction&, WebFrame*);
    void didFlushLayerTreeAtTime(MonotonicTime);
#endif

    void layoutIfNeeded();
    void updateRendering();
    bool shouldTriggerRenderingUpdate(unsigned rescheduledRenderingUpdateCount) const;
    void finalizeRenderingUpdate(OptionSet<CyberCore::FinalizeRenderingUpdateFlags>);

    void willStartRenderingUpdateDisplay();
    void didCompleteRenderingUpdateDisplay();
    // Called after didCompleteRenderingUpdateDisplay, but in the same run loop iteration.
    void didCompleteRenderingFrame();

    void releaseMemory(WTF::Critical);
    void willDestroyDecodedDataForAllImages();

    unsigned remoteImagesCountForTesting() const;

    enum class LazyCreationPolicy { UseExistingOnly, CreateIfNeeded };

    WebInspector* inspector(LazyCreationPolicy = LazyCreationPolicy::CreateIfNeeded);
    WebInspectorUI* inspectorUI();
    RemoteWebInspectorUI* remoteInspectorUI();
    bool isInspectorPage() { return !!m_inspectorUI || !!m_remoteInspectorUI; }

    void inspectorFrontendCountChanged(unsigned);

#if PLATFORM(IOS_FAMILY) || (PLATFORM(MAC) && ENABLE(VIDEO_PRESENTATION_MODE))
    PlaybackSessionManager& playbackSessionManager();
    void videoControlsManagerDidChange();
#endif

#if ENABLE(VIDEO_PRESENTATION_MODE)
    VideoFullscreenManager& videoFullscreenManager();
#endif

#if PLATFORM(IOS_FAMILY)
    void setAllowsMediaDocumentInlinePlayback(bool);
    bool allowsMediaDocumentInlinePlayback() const { return m_allowsMediaDocumentInlinePlayback; }
#endif

#if ENABLE(FULLSCREEN_API)
    WebFullScreenManager* fullScreenManager();
#endif

    void addConsoleMessage(CyberCore::FrameIdentifier, MessageSource, MessageLevel, const String&, std::optional<CyberCore::ResourceLoaderIdentifier> = std::nullopt);
    void enqueueSecurityPolicyViolationEvent(CyberCore::FrameIdentifier, CyberCore::SecurityPolicyViolationEventInit&&);

    void notifyReportObservers(CyberCore::FrameIdentifier, Ref<CyberCore::Report>&&);
    void sendReportToEndpoints(CyberCore::FrameIdentifier, URL&& baseURL, const Vector<String>& endpointURIs, const Vector<String>& endpointTokens, IPC::FormDataReference&&, CyberCore::ViolationReportType);

    // -- Called by the DrawingArea.
    // FIXME: We could genericize these into a DrawingArea client interface. Would that be beneficial?
    void drawRect(CyberCore::GraphicsContext&, const CyberCore::IntRect&);

    // -- Called from CyberCore clients.
    bool handleEditingKeyboardEvent(CyberCore::KeyboardEvent&);

    void didStartPageTransition();
    void didCompletePageTransition();
    void didCommitLoad(WebFrame*);
    void willReplaceMultipartContent(const WebFrame&);
    void didReplaceMultipartContent(const WebFrame&);
    void didFinishDocumentLoad(WebFrame&);
    void didFinishLoad(WebFrame&);
    void show();
    String userAgent(const URL&) const;
    String platformUserAgent(const URL&) const;
    CyberCore::KeyboardUIMode keyboardUIMode();

    bool hoverSupportedByPrimaryPointingDevice() const;
    bool hoverSupportedByAnyAvailablePointingDevice() const;
    std::optional<CyberCore::PointerCharacteristics> pointerCharacteristicsOfPrimaryPointingDevice() const;
    OptionSet<CyberCore::PointerCharacteristics> pointerCharacteristicsOfAllAvailablePointingDevices() const;

    void didInsertMenuElement(CyberCore::HTMLMenuElement&);
    void didRemoveMenuElement(CyberCore::HTMLMenuElement&);
    void didInsertMenuItemElement(CyberCore::HTMLMenuItemElement&);
    void didRemoveMenuItemElement(CyberCore::HTMLMenuItemElement&);

    void animationDidFinishForElement(const CyberCore::Element&);

    const String& overrideContentSecurityPolicy() const { return m_overrideContentSecurityPolicy; }

    WebUndoStep* webUndoStep(WebUndoStepID);
    void addWebUndoStep(WebUndoStepID, Ref<WebUndoStep>&&);
    void removeWebEditCommand(WebUndoStepID);
    bool isInRedo() const { return m_isInRedo; }

    void setActivePopupMenu(WebPopupMenu*);

    inline void setHiddenPageDOMTimerThrottlingIncreaseLimit(Seconds);

#if ENABLE(INPUT_TYPE_COLOR)
    WebColorChooser* activeColorChooser() const { return m_activeColorChooser; }
    void setActiveColorChooser(WebColorChooser*);
    void didChooseColor(const CyberCore::Color&);
    void didEndColorPicker();
#endif

#if ENABLE(DATALIST_ELEMENT)
    void setActiveDataListSuggestionPicker(WebDataListSuggestionPicker&);
    void didSelectDataListOption(const String&);
    void didCloseSuggestions();
#endif

#if ENABLE(DATE_AND_TIME_INPUT_TYPES)
    void setActiveDateTimeChooser(WebDateTimeChooser&);
    void didChooseDate(const String&);
    void didEndDateTimePicker();
#endif

    WebOpenPanelResultListener* activeOpenPanelResultListener() const { return m_activeOpenPanelResultListener.get(); }
    void setActiveOpenPanelResultListener(Ref<WebOpenPanelResultListener>&&);

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;
    bool didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, UniqueRef<IPC::Encoder>&) override;

    // -- InjectedBundle methods
#if ENABLE(CONTEXT_MENUS)
    void setInjectedBundleContextMenuClient(std::unique_ptr<API::InjectedBundle::PageContextMenuClient>&&);
#endif
    void setInjectedBundleEditorClient(std::unique_ptr<API::InjectedBundle::EditorClient>&&);
    void setInjectedBundleFormClient(std::unique_ptr<API::InjectedBundle::FormClient>&&);
    void setInjectedBundlePageLoaderClient(std::unique_ptr<API::InjectedBundle::PageLoaderClient>&&);
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
    API::InjectedBundle::ResourceLoadClient& injectedBundleResourceLoadClient() { return *m_resourceLoadClient; }
    API::InjectedBundle::PageUIClient& injectedBundleUIClient() { return *m_uiClient; }
#if ENABLE(FULLSCREEN_API)
    InjectedBundlePageFullScreenClient& injectedBundleFullScreenClient() { return m_fullScreenClient; }
#endif

    bool findStringFromInjectedBundle(const String&, OptionSet<FindOptions>);
    void findStringMatchesFromInjectedBundle(const String&, OptionSet<FindOptions>);
    void replaceStringMatchesFromInjectedBundle(const Vector<uint32_t>& matchIndices, const String& replacementText, bool selectionOnly);

    void setTextIndicator(const CyberCore::TextIndicatorData&);

    WebFrame& mainWebFrame() const { return m_mainFrame; }

    CyberCore::Frame* mainFrame() const; // May return nullptr.
    CyberCore::LocalFrameView* mainFrameView() const; // May return nullptr.

    std::optional<CyberCore::SimpleRange> currentSelectionAsRange();

    enum class ShouldPerformLayout : bool { Default, Yes };
    EditorState editorState(ShouldPerformLayout = ShouldPerformLayout::Default) const;
    void updateEditorStateAfterLayoutIfEditabilityChanged();

    // options are RenderTreeExternalRepresentationBehavior values.
    String renderTreeExternalRepresentation(unsigned options = 0) const;
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
    void windowScreenDidChange(CyberCore::PlatformDisplayID, std::optional<unsigned> nominalFramesPerSecond);
    String dumpHistoryForTesting(const String& directory);
    void clearHistory();

    void accessibilitySettingsDidChange();
    void screenPropertiesDidChange();

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

    void setDefaultUnobscuredSize(const CyberCore::FloatSize&);
    void setMinimumUnobscuredSize(const CyberCore::FloatSize&);
    void setMaximumUnobscuredSize(const CyberCore::FloatSize&);

    void listenForLayoutMilestones(OptionSet<CyberCore::LayoutMilestone>);

    void setSuppressScrollbarAnimations(bool);

    void setHasActiveAnimatedScrolls(bool);

    void setEnableVerticalRubberBanding(bool);
    void setEnableHorizontalRubberBanding(bool);
    
    void setBackgroundExtendsBeyondPage(bool);

    void setPaginationMode(uint32_t /* CyberCore::Pagination::Mode */);
    void setPaginationBehavesLikeColumns(bool);
    void setPageLength(double);
    void setGapBetweenPages(double);
    
    void postInjectedBundleMessage(const String& messageName, const UserData&);

    void setUnderPageBackgroundColorOverride(CyberCore::Color&&);

    void setUnderlayColor(const CyberCore::Color& color) { m_underlayColor = color; }
    CyberCore::Color underlayColor() const { return m_underlayColor; }

    void stopLoading();
    void stopLoadingDueToProcessSwap();
    bool defersLoading() const;
    void setDefersLoading(bool deferLoading);

    void enterAcceleratedCompositingMode(CyberCore::Frame&, CyberCore::GraphicsLayer*);
    void exitAcceleratedCompositingMode(CyberCore::Frame&);

#if ENABLE(PDFKIT_PLUGIN)
    void addPluginView(PluginView*);
    void removePluginView(PluginView*);
#endif

    inline bool isVisible() const;
    inline bool isVisibleOrOccluded() const;

    OptionSet<CyberCore::ActivityState> activityState() const { return m_activityState; }
    bool isThrottleable() const;

    LayerHostingMode layerHostingMode() const { return m_layerHostingMode; }
    void setLayerHostingMode(LayerHostingMode);

#if PLATFORM(COCOA)
    void updatePluginsActiveAndFocusedState();
    const CyberCore::FloatRect& windowFrameInScreenCoordinates() const { return m_windowFrameInScreenCoordinates; }
    const CyberCore::FloatRect& windowFrameInUnflippedScreenCoordinates() const { return m_windowFrameInUnflippedScreenCoordinates; }
    const CyberCore::FloatRect& viewFrameInWindowCoordinates() const { return m_viewFrameInWindowCoordinates; }

    bool hasCachedWindowFrame() const { return m_hasCachedWindowFrame; }

    void updateHeaderAndFooterLayersForDeviceScaleChange(float scaleFactor);

    bool isTransparentOrFullyClipped(const CyberCore::Element&) const;
#endif

    void didUpdateRendering();
    void didPaintLayers();

    // A "platform rendering update" here describes the work done by the system graphics framework before work is submitted to the system compositor.
    // On macOS, this is a CoreAnimation commit.
    void willStartPlatformRenderingUpdate();
    void didCompletePlatformRenderingUpdate();

#if PLATFORM(MAC)
    void setTopOverhangImage(WebImage*);
    void setBottomOverhangImage(WebImage*);

    void setUseSystemAppearance(bool);

    void setUseFormSemanticContext(bool);
    void semanticContextDidChange(bool);

    void didEndMagnificationGesture();
#endif

    void effectiveAppearanceDidChange(bool useDarkAppearance, bool useElevatedUserInterfaceLevel);

    bool windowIsFocused() const;
    bool windowAndWebPageAreFocused() const;

#if !PLATFORM(IOS_FAMILY)
    void setHeaderPageBanner(PageBanner*);
    PageBanner* headerPageBanner();
    void setFooterPageBanner(PageBanner*);
    PageBanner* footerPageBanner();

    void hidePageBanners();
    void showPageBanners();
#endif

#if PLATFORM(MAC)
    void setHeaderBannerHeight(int);
    void setFooterBannerHeight(int);
#endif

    CyberCore::IntPoint screenToRootView(const CyberCore::IntPoint&);
    CyberCore::IntRect rootViewToScreen(const CyberCore::IntRect&);
    CyberCore::IntPoint accessibilityScreenToRootView(const CyberCore::IntPoint&);
    CyberCore::IntRect rootViewToAccessibilityScreen(const CyberCore::IntRect&);
    
    RefPtr<WebImage> scaledSnapshotWithOptions(const CyberCore::IntRect&, double additionalScaleFactor, SnapshotOptions);

    static const WebEvent* currentEvent();

    FindController& findController() { return m_findController.get(); }
    WebFoundTextRangeController& foundTextRangeController() { return m_foundTextRangeController.get(); }

#if ENABLE(GEOLOCATION)
    GeolocationPermissionRequestManager& geolocationPermissionRequestManager() { return m_geolocationPermissionRequestManager.get(); }
#endif

#if PLATFORM(IOS_FAMILY)
    void savePageState(CyberCore::HistoryItem&);
    void restorePageState(const CyberCore::HistoryItem&);
#endif

#if ENABLE(MEDIA_STREAM)
    UserMediaPermissionRequestManager& userMediaPermissionRequestManager() { return m_userMediaPermissionRequestManager; }
    void captureDevicesChanged();
#endif

#if ENABLE(ENCRYPTED_MEDIA)
    MediaKeySystemPermissionRequestManager& mediaKeySystemPermissionRequestManager() { return m_mediaKeySystemPermissionRequestManager; }
#endif

    void elementDidFocus(CyberCore::Element&, const CyberCore::FocusOptions&);
    void elementDidRefocus(CyberCore::Element&, const CyberCore::FocusOptions&);
    void elementDidBlur(CyberCore::Element&);
    void focusedElementDidChangeInputMode(CyberCore::Element&, CyberCore::InputMode);
    void resetFocusedElementForFrame(WebFrame*);
    void updateInputContextAfterBlurringAndRefocusingElementIfNeeded(CyberCore::Element&);

    void disabledAdaptationsDidChange(const OptionSet<CyberCore::DisabledAdaptations>&);
    void viewportPropertiesDidChange(const CyberCore::ViewportArguments&);
    void executeEditCommandWithCallback(const String&, const String& argument, CompletionHandler<void()>&&);
    void selectAll();

    void setCanShowPlaceholder(const CyberCore::ElementContext&, bool);

    bool handlesPageScaleGesture();

#if PLATFORM(IOS_FAMILY)
    void textInputContextsInRect(CyberCore::FloatRect, CompletionHandler<void(const Vector<CyberCore::ElementContext>&)>&&);
    void focusTextInputContextAndPlaceCaret(const CyberCore::ElementContext&, const CyberCore::IntPoint&, CompletionHandler<void(bool)>&&);

    bool shouldRevealCurrentSelectionAfterInsertion() const { return m_shouldRevealCurrentSelectionAfterInsertion; }
    void setShouldRevealCurrentSelectionAfterInsertion(bool);

    void insertTextPlaceholder(const CyberCore::IntSize&, CompletionHandler<void(const std::optional<CyberCore::ElementContext>&)>&&);
    void removeTextPlaceholder(const CyberCore::ElementContext&, CompletionHandler<void()>&&);

    CyberCore::FloatSize screenSize() const;
    CyberCore::FloatSize availableScreenSize() const;
    CyberCore::FloatSize overrideScreenSize() const;
    CyberCore::IntDegrees deviceOrientation() const { return m_deviceOrientation; }
    void didReceiveMobileDocType(bool);
    
    bool screenIsBeingCaptured() const { return m_screenIsBeingCaptured; }
    void setScreenIsBeingCaptured(bool);

    double minimumPageScaleFactor() const;
    double maximumPageScaleFactor() const;
    double maximumPageScaleFactorIgnoringAlwaysScalable() const;
    bool allowsUserScaling() const;
    bool hasStablePageScaleFactor() const { return m_hasStablePageScaleFactor; }

    void attemptSyntheticClick(const CyberCore::IntPoint&, OptionSet<CyberKit::WebEventModifier>, TransactionID lastLayerTreeTransactionId);
    void potentialTapAtPosition(CyberKit::TapIdentifier, const CyberCore::FloatPoint&, bool shouldRequestMagnificationInformation);
    void commitPotentialTap(OptionSet<CyberKit::WebEventModifier>, TransactionID lastLayerTreeTransactionId, CyberCore::PointerID);
    void commitPotentialTapFailed();
    void cancelPotentialTap();
    void cancelPotentialTapInFrame(WebFrame&);
    void tapHighlightAtPosition(CyberKit::TapIdentifier, const CyberCore::FloatPoint&);
    void didRecognizeLongPress();
    void handleDoubleTapForDoubleClickAtPoint(const CyberCore::IntPoint&, OptionSet<CyberKit::WebEventModifier>, TransactionID lastLayerTreeTransactionId);

    void inspectorNodeSearchMovedToPosition(const CyberCore::FloatPoint&);
    void inspectorNodeSearchEndedAtPosition(const CyberCore::FloatPoint&);

    void blurFocusedElement();
    void requestFocusedElementInformation(CompletionHandler<void(const std::optional<FocusedElementInformation>&)>&&);
    void selectWithGesture(const CyberCore::IntPoint&, GestureType, GestureRecognizerState, bool isInteractingWithFocusedElement, CompletionHandler<void(const CyberCore::IntPoint&, GestureType, GestureRecognizerState, OptionSet<SelectionFlags>)>&&);
    void updateSelectionWithTouches(const CyberCore::IntPoint&, SelectionTouch, bool baseIsStart, CompletionHandler<void(const CyberCore::IntPoint&, SelectionTouch, OptionSet<SelectionFlags>)>&&);
    void selectWithTwoTouches(const CyberCore::IntPoint& from, const CyberCore::IntPoint& to, GestureType, GestureRecognizerState, CompletionHandler<void(const CyberCore::IntPoint&, GestureType, GestureRecognizerState, OptionSet<SelectionFlags>)>&&);
    void extendSelection(CyberCore::TextGranularity, CompletionHandler<void()>&&);
    void extendSelectionForReplacement(CompletionHandler<void()>&&);
    void selectWordBackward();
    void moveSelectionByOffset(int32_t offset, CompletionHandler<void()>&&);
    void selectTextWithGranularityAtPoint(const CyberCore::IntPoint&, CyberCore::TextGranularity, bool isInteractingWithFocusedElement, CompletionHandler<void()>&&);
    void selectPositionAtBoundaryWithDirection(const CyberCore::IntPoint&, CyberCore::TextGranularity, CyberCore::SelectionDirection, bool isInteractingWithFocusedElement, CompletionHandler<void()>&&);
    void moveSelectionAtBoundaryWithDirection(CyberCore::TextGranularity, CyberCore::SelectionDirection, CompletionHandler<void()>&&);
    void selectPositionAtPoint(const CyberCore::IntPoint&, bool isInteractingWithFocusedElement, CompletionHandler<void()>&&);
    void beginSelectionInDirection(CyberCore::SelectionDirection, CompletionHandler<void(bool)>&&);
    void updateSelectionWithExtentPoint(const CyberCore::IntPoint&, bool isInteractingWithFocusedElement, RespectSelectionAnchor, CompletionHandler<void(bool)>&&);
    void updateSelectionWithExtentPointAndBoundary(const CyberCore::IntPoint&, CyberCore::TextGranularity, bool isInteractingWithFocusedElement, CompletionHandler<void(bool)>&&);

    void requestDictationContext(CompletionHandler<void(const String&, const String&, const String&)>&&);
#if ENABLE(REVEAL)
    RetainPtr<RVItem> revealItemForCurrentSelection();
    void requestRVItemInCurrentSelectedRange(CompletionHandler<void(const CyberKit::RevealItem&)>&&);
    void prepareSelectionForContextMenuWithLocationInView(CyberCore::IntPoint, CompletionHandler<void(bool, const RevealItem&)>&&);
#endif
    void willInsertFinalDictationResult();
    void didInsertFinalDictationResult();
    void replaceDictatedText(const String& oldText, const String& newText);
    void replaceSelectedText(const String& oldText, const String& newText);
    void requestAutocorrectionData(const String& textForAutocorrection, CompletionHandler<void(WebAutocorrectionData)>&& reply);
    void applyAutocorrection(const String& correction, const String& originalText, bool isCandidate, CompletionHandler<void(const String&)>&&);
    void syncApplyAutocorrection(const String& correction, const String& originalText, bool isCandidate, CompletionHandler<void(bool)>&&);
    void handleAutocorrectionContextRequest();
    void preemptivelySendAutocorrectionContext();
    void requestPositionInformation(const InteractionInformationRequest&);
    void startInteractionWithElementContextOrPosition(std::optional<CyberCore::ElementContext>&&, CyberCore::IntPoint&&);
    void stopInteraction();
    void performActionOnElement(uint32_t action, const String& authorizationToken, CompletionHandler<void()>&&);
    void focusNextFocusedElement(bool isForward, CompletionHandler<void()>&&);
    void autofillLoginCredentials(const String&, const String&);
    void setFocusedElementValue(const CyberCore::ElementContext&, const String&);
    void setFocusedElementSelectedIndex(const CyberCore::ElementContext&, uint32_t index, bool allowMultipleSelection);
    void setIsShowingInputViewForFocusedElement(bool showingInputView) { m_isShowingInputViewForFocusedElement = showingInputView; }
    bool isShowingInputViewForFocusedElement() const { return m_isShowingInputViewForFocusedElement; }
    void updateSelectionAppearance();
    void getSelectionContext(CompletionHandler<void(const String&, const String&, const String&)>&&);
    void handleTwoFingerTapAtPoint(const CyberCore::IntPoint&, OptionSet<CyberKit::WebEventModifier>, CyberKit::TapIdentifier);
    void getRectsForGranularityWithSelectionOffset(CyberCore::TextGranularity, int32_t, CompletionHandler<void(const Vector<CyberCore::SelectionGeometry>&)>&&);
    void getRectsAtSelectionOffsetWithText(int32_t, const String&, CompletionHandler<void(const Vector<CyberCore::SelectionGeometry>&)>&&);
    void storeSelectionForAccessibility(bool);
    void startAutoscrollAtPosition(const CyberCore::FloatPoint&);
    void cancelAutoscroll();
    void requestEvasionRectsAboveSelection(CompletionHandler<void(const Vector<CyberCore::FloatRect>&)>&&);

    void contentSizeCategoryDidChange(const String&);

    Seconds eventThrottlingDelay() const;

    void showInspectorHighlight(const CyberCore::InspectorOverlay::Highlight&);
    void hideInspectorHighlight();

    void showInspectorIndication();
    void hideInspectorIndication();

    void enableInspectorNodeSearch();
    void disableInspectorNodeSearch();

    bool forceAlwaysUserScalable() const { return m_forceAlwaysUserScalable; }
    void setForceAlwaysUserScalable(bool);

    void updateSelectionWithDelta(int64_t locationDelta, int64_t lengthDelta, CompletionHandler<void()>&&);
    void requestDocumentEditingContext(CyberKit::DocumentEditingContextRequest, CompletionHandler<void(CyberKit::DocumentEditingContext)>&&);
    bool shouldAllowSingleClickToChangeSelection(CyberCore::Node& targetNode, const CyberCore::VisibleSelection& newSelection);
#endif

    void willChangeSelectionForAccessibility() { m_isChangingSelectionForAccessibility = true; }
    void didChangeSelectionForAccessibility() { m_isChangingSelectionForAccessibility = false; }

#if PLATFORM(IOS_FAMILY) && ENABLE(IOS_TOUCH_EVENTS)
    void dispatchAsynchronousTouchEvents(Vector<std::pair<WebTouchEvent, CompletionHandler<void(bool)>>, 1>&&);
    void cancelAsynchronousTouchEvents(Vector<std::pair<WebTouchEvent, CompletionHandler<void(bool)>>, 1>&&);
#endif

    bool hasRichlyEditableSelection() const;

    enum class LayerTreeFreezeReason {
        PageTransition          = 1 << 0,
        BackgroundApplication   = 1 << 1,
        ProcessSuspended        = 1 << 2,
        PageSuspended           = 1 << 3,
        Printing                = 1 << 4,
        ProcessSwap             = 1 << 5,
        SwipeAnimation          = 1 << 6,
    };
    void freezeLayerTree(LayerTreeFreezeReason);
    void unfreezeLayerTree(LayerTreeFreezeReason);
    
    void updateFrameSize(CyberCore::FrameIdentifier, CyberCore::IntSize);

    void isLayerTreeFrozen(CompletionHandler<void(bool)>&&);

    void markLayersVolatile(CompletionHandler<void(bool)>&& completionHandler = { });
    void cancelMarkLayersVolatile();

    void freezeLayerTreeDueToSwipeAnimation();
    void unfreezeLayerTreeDueToSwipeAnimation();

    NotificationPermissionRequestManager* notificationPermissionRequestManager();

    void pageDidScroll();

#if ENABLE(CONTEXT_MENUS)
    WebContextMenu& contextMenu();
    WebContextMenu* contextMenuAtPointInWindow(const CyberCore::IntPoint&);
#endif

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
        bool shouldReuseCommittedSandboxExtension(WebFrame*);

        RefPtr<SandboxExtension> m_pendingProvisionalSandboxExtension;
        RefPtr<SandboxExtension> m_provisionalSandboxExtension;
        RefPtr<SandboxExtension> m_committedSandboxExtension;
    };

    SandboxExtensionTracker& sandboxExtensionTracker() { return m_sandboxExtensionTracker; }

#if PLATFORM(GTK) || PLATFORM(WPE)
    void cancelComposition(const String& text);
    void deleteSurrounding(int64_t offset, unsigned characterCount);
#endif

#if PLATFORM(GTK)
    void collapseSelectionInFrame(CyberCore::FrameIdentifier);
    void showEmojiPicker(CyberCore::LocalFrame&);
#endif

    void didApplyStyle();
    void didScrollSelection();
    void didChangeSelection(CyberCore::LocalFrame&);
    void didChangeOverflowScrollPosition();
    void didChangeContents();
    void discardedComposition();
    void canceledComposition();
    void didUpdateComposition();
    void didEndUserTriggeredSelectionChanges();

    void navigateServiceWorkerClient(CyberCore::ScriptExecutionContextIdentifier, const URL&, CompletionHandler<void(bool)>&&);

#if PLATFORM(COCOA)
    void platformInitializeAccessibility();
    void registerUIProcessAccessibilityTokens(const IPC::DataReference& elemenToken, const IPC::DataReference& windowToken);
    WKAccessibilityWebPageObject* accessibilityRemoteObject();
    NSObject *accessibilityObjectForMainFramePlugin();
    const CyberCore::FloatPoint& accessibilityPosition() const { return m_accessibilityPosition; }

    void setTextAsync(const String&);
    void insertTextAsync(const String& text, const EditingRange& replacementRange, InsertTextOptions&&);
    void hasMarkedText(CompletionHandler<void(bool)>&&);
    void getMarkedRangeAsync(CompletionHandler<void(const EditingRange&)>&&);
    void getSelectedRangeAsync(CompletionHandler<void(const EditingRange&)>&&);
    void characterIndexForPointAsync(const CyberCore::IntPoint&, CompletionHandler<void(uint64_t)>&&);
    void firstRectForCharacterRangeAsync(const EditingRange&, CompletionHandler<void(const CyberCore::IntRect&, const EditingRange&)>&&);
    void setCompositionAsync(const String& text, const Vector<CyberCore::CompositionUnderline>&, const Vector<CyberCore::CompositionHighlight>&, const EditingRange& selectionRange, const EditingRange& replacementRange);
    void confirmCompositionAsync();

    void readSelectionFromPasteboard(const String& pasteboardName, CompletionHandler<void(bool&&)>&&);
    void getStringSelectionForPasteboard(CompletionHandler<void(String&&)>&&);
    void getDataSelectionForPasteboard(const String pasteboardType, CompletionHandler<void(RefPtr<CyberCore::SharedBuffer>&&)>&&);
    void shouldDelayWindowOrderingEvent(const CyberKit::WebMouseEvent&, CompletionHandler<void(bool)>&&);
    bool performNonEditingBehaviorForSelector(const String&, CyberCore::KeyboardEvent*);

    void insertDictatedTextAsync(const String& text, const EditingRange& replacementRange, const Vector<CyberCore::DictationAlternative>& dictationAlternativeLocations, InsertTextOptions&&);
    void addDictationAlternative(const String& text, CyberCore::DictationContext, CompletionHandler<void(bool)>&&);
    void dictationAlternativesAtSelection(CompletionHandler<void(Vector<CyberCore::DictationContext>&&)>&&);
    void clearDictationAlternatives(Vector<CyberCore::DictationContext>&&);
#endif // PLATFORM(COCOA)

#if PLATFORM(MAC)
    void setCaretAnimatorType(CyberCore::CaretAnimatorType);
    void setCaretBlinkingSuspended(bool);
    void attributedSubstringForCharacterRangeAsync(const EditingRange&, CompletionHandler<void(const CyberCore::AttributedString&, const EditingRange&)>&&);
    void requestAcceptsFirstMouse(int eventNumber, const CyberKit::WebMouseEvent&);
#endif

#if PLATFORM(COCOA)
    void replaceSelectionWithPasteboardData(const Vector<String>& types, const IPC::DataReference&);
#endif

#if ENABLE(IMAGE_ANALYSIS_ENHANCEMENTS)
    void replaceImageForRemoveBackground(const CyberCore::ElementContext&, const Vector<String>& types, const IPC::DataReference&);
#endif

    void setCompositionForTesting(const String& compositionString, uint64_t from, uint64_t length, bool suppressUnderline, const Vector<CyberCore::CompositionHighlight>&);
    bool hasCompositionForTesting();
    void confirmCompositionForTesting(const String& compositionString);

#if PLATFORM(COCOA)
    bool isSpeaking() const;
    void speak(const String&);
    void stopSpeaking();

    void performDictionaryLookupForSelection(CyberCore::LocalFrame&, const CyberCore::VisibleSelection&, CyberCore::TextIndicatorPresentationTransition);
#endif

    bool isStoppingLoadingDueToProcessSwap() const { return m_isStoppingLoadingDueToProcessSwap; }

    bool isSmartInsertDeleteEnabled();
    void setSmartInsertDeleteEnabled(bool);

    bool isSelectTrailingWhitespaceEnabled() const;
    void setSelectTrailingWhitespaceEnabled(bool);

    void replaceSelectionWithText(CyberCore::LocalFrame*, const String&);
    void clearSelection();
    void restoreSelectionInFocusedEditableElement();

#if ENABLE(DRAG_SUPPORT) && PLATFORM(GTK)
    void performDragControllerAction(DragControllerAction, const CyberCore::IntPoint& clientPosition, const CyberCore::IntPoint& globalPosition, OptionSet<CyberCore::DragOperation> draggingSourceOperationMask, CyberCore::SelectionData&&, OptionSet<CyberCore::DragApplicationFlags>);
#endif

#if ENABLE(DRAG_SUPPORT) && !PLATFORM(GTK)
    void performDragControllerAction(DragControllerAction, CyberCore::DragData&&, SandboxExtension::Handle&&, Vector<SandboxExtension::Handle>&&);
#endif

#if ENABLE(DRAG_SUPPORT)
    void dragEnded(CyberCore::IntPoint clientPosition, CyberCore::IntPoint globalPosition, OptionSet<CyberCore::DragOperation>);

    void willPerformLoadDragDestinationAction();
    void mayPerformUploadDragDestinationAction();

    void willStartDrag() { ASSERT(!m_isStartingDrag); m_isStartingDrag = true; }
    void didStartDrag();
    void dragCancelled();
    OptionSet<CyberCore::DragSourceAction> allowedDragSourceActions() const { return m_allowedDragSourceActions; }
#endif

    void beginPrinting(CyberCore::FrameIdentifier, const PrintInfo&);
    void beginPrintingDuringDOMPrintOperation(CyberCore::FrameIdentifier frameID, const PrintInfo& printInfo) { beginPrinting(frameID, printInfo); }
    void endPrinting();
    void endPrintingDuringDOMPrintOperation() { endPrinting(); }
    void computePagesForPrinting(CyberCore::FrameIdentifier, const PrintInfo&, CompletionHandler<void(const Vector<CyberCore::IntRect>&, double, const CyberCore::FloatBoxExtent&)>&&);
    void computePagesForPrintingDuringDOMPrintOperation(CyberCore::FrameIdentifier frameID, const PrintInfo& printInfo, CompletionHandler<void(const Vector<CyberCore::IntRect>&, double, const CyberCore::FloatBoxExtent&)>&& completionHandler) { computePagesForPrinting(frameID, printInfo, WTFMove(completionHandler)); }
    void computePagesForPrintingImpl(CyberCore::FrameIdentifier, const PrintInfo&, Vector<CyberCore::IntRect>& pageRects, double& totalScaleFactor, CyberCore::FloatBoxExtent& computedMargin);

#if PLATFORM(COCOA)
    void drawRectToImage(CyberCore::FrameIdentifier, const PrintInfo&, const CyberCore::IntRect&, const CyberCore::IntSize&, CompletionHandler<void(const CyberKit::ShareableBitmap::Handle&)>&&);
    void drawRectToImageDuringDOMPrintOperation(CyberCore::FrameIdentifier frameID, const PrintInfo& printInfo, const CyberCore::IntRect& rect, const CyberCore::IntSize& imageSize, CompletionHandler<void(const CyberKit::ShareableBitmap::Handle&)>&& completionHandler) { drawRectToImage(frameID, printInfo, rect, imageSize, WTFMove(completionHandler)); }
    void drawPagesToPDF(CyberCore::FrameIdentifier, const PrintInfo&, uint32_t first, uint32_t count, CompletionHandler<void(RefPtr<CyberCore::SharedBuffer>&&)>&&);
    void drawPagesToPDFDuringDOMPrintOperation(CyberCore::FrameIdentifier frameID, const PrintInfo& printInfo, uint32_t first, uint32_t count, CompletionHandler<void(RefPtr<CyberCore::SharedBuffer>&&)>&& completionHandler) { drawPagesToPDF(frameID, printInfo, first, count, WTFMove(completionHandler)); }
    void drawPagesToPDFImpl(CyberCore::FrameIdentifier, const PrintInfo&, uint32_t first, uint32_t count, RetainPtr<CFMutableDataRef>& pdfPageData);
#endif

#if PLATFORM(IOS_FAMILY)
    void computePagesForPrintingiOS(CyberCore::FrameIdentifier, const PrintInfo&, CompletionHandler<void(size_t)>&&);
    void drawToPDFiOS(CyberCore::FrameIdentifier, const PrintInfo&, size_t, CompletionHandler<void(RefPtr<CyberCore::SharedBuffer>&&)>&&);
    void drawToImage(CyberCore::FrameIdentifier, const PrintInfo&, size_t, CompletionHandler<void(CyberKit::ShareableBitmap::Handle&&)>&&);
#endif

    void drawToPDF(CyberCore::FrameIdentifier, const std::optional<CyberCore::FloatRect>&, bool allowTransparentBackground,  CompletionHandler<void(RefPtr<CyberCore::SharedBuffer>&&)>&&);

#if PLATFORM(GTK)
    void drawPagesForPrinting(CyberCore::FrameIdentifier, const PrintInfo&, CompletionHandler<void(std::optional<SharedMemory::Handle>&&, CyberCore::ResourceError&&)>&&);
    void drawPagesForPrintingDuringDOMPrintOperation(CyberCore::FrameIdentifier frameID, const PrintInfo& printInfo, CompletionHandler<void(std::optional<SharedMemory::Handle>&&, CyberCore::ResourceError&&)>&& completionHandler) { drawPagesForPrinting(frameID, printInfo, WTFMove(completionHandler)); }
#endif

    void addResourceRequest(CyberCore::ResourceLoaderIdentifier, const CyberCore::ResourceRequest&);
    void removeResourceRequest(CyberCore::ResourceLoaderIdentifier);

    void setMediaVolume(float);
    void setMuted(CyberCore::MediaProducerMutedStateFlags, CompletionHandler<void()>&&);
    void setMayStartMediaWhenInWindow(bool);
    void stopMediaCapture(CyberCore::MediaProducerMediaCaptureKind, CompletionHandler<void()>&&);

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
    void startWaitingForContextMenuToShow() { m_waitingForContextMenuToShow = true; }
#endif

    void handleWheelEvent(const WebWheelEvent&, const OptionSet<CyberCore::WheelEventProcessingSteps>&, std::optional<bool> willStartSwipe, CompletionHandler<void(CyberCore::ScrollingNodeID, std::optional<CyberCore::WheelScrollGestureState>)>&&);
    bool wheelEvent(const WebWheelEvent&, OptionSet<CyberCore::WheelEventProcessingSteps>, EventDispatcher::WheelEventOrigin);

    void wheelEventHandlersChanged(bool);
    void recomputeShortCircuitHorizontalWheelEventsState();

#if ENABLE(MAC_GESTURE_EVENTS)
    void gestureEvent(const WebGestureEvent&);
#endif

    void updateVisibilityState(bool isInitialState = false);

#if PLATFORM(IOS_FAMILY)
    void setDeviceOrientation(CyberCore::IntDegrees);
    void dynamicViewportSizeUpdate(const DynamicViewportSizeUpdate&);
    bool scaleWasSetByUIProcess() const { return m_scaleWasSetByUIProcess; }
    void willStartUserTriggeredZooming();
    void applicationWillResignActive();
    void applicationDidEnterBackground(bool isSuspendedUnderLock);
    void applicationDidFinishSnapshottingAfterEnteringBackground();
    void applicationWillEnterForeground(bool isSuspendedUnderLock);
    void applicationDidBecomeActive();
    void applicationDidEnterBackgroundForMedia(bool isSuspendedUnderLock);
    void applicationWillEnterForegroundForMedia(bool isSuspendedUnderLock);
    void didFinishContentChangeObserving(WKContentChange);

    bool platformPrefersTextLegibilityBasedZoomScaling() const;

    void hardwareKeyboardAvailabilityChanged(bool keyboardIsAttached);
    bool hardwareKeyboardIsAttached() const { return m_keyboardIsAttached; }

    void updateStringForFind(const String&);
    
    bool canShowWhileLocked() const { return m_canShowWhileLocked; }
#endif

#if ENABLE(META_VIEWPORT)
    void setViewportConfigurationViewLayoutSize(const CyberCore::FloatSize&, double scaleFactor, double minimumEffectiveDeviceWidth);
    void setOverrideViewportArguments(const std::optional<CyberCore::ViewportArguments>&);
    const CyberCore::ViewportConfiguration& viewportConfiguration() const { return m_viewportConfiguration; }

    void setUseTestingViewportConfiguration(bool useTestingViewport) { m_useTestingViewportConfiguration = useTestingViewport; }
    bool isUsingTestingViewportConfiguration() const { return m_useTestingViewportConfiguration; }
#endif

#if ENABLE(UI_SIDE_COMPOSITING)
    std::optional<float> scaleFromUIProcess(const VisibleContentRectUpdateInfo&) const;
    void updateVisibleContentRects(const VisibleContentRectUpdateInfo&, MonotonicTime oldestTimestamp);
#endif

#if ENABLE(IOS_TOUCH_EVENTS)
    bool dispatchTouchEvent(const WebTouchEvent&);
#endif

    bool shouldUseCustomContentProviderForResponse(const CyberCore::ResourceResponse&);

#if PLATFORM(COCOA)
    bool pdfPluginEnabled() const { return m_pdfPluginEnabled; }
    void setPDFPluginEnabled(bool enabled) { m_pdfPluginEnabled = enabled; }
    
    bool selectionFlippingEnabled() const { return m_selectionFlippingEnabled; }
    void setSelectionFlippingEnabled(bool enabled) { m_selectionFlippingEnabled = enabled; }

    std::optional<double> dataDetectionReferenceDate() const { return m_dataDetectionReferenceDate; }
#endif

    bool mainFrameIsScrollable() const { return m_mainFrameIsScrollable; }

    void setAlwaysShowsHorizontalScroller(bool);
    void setAlwaysShowsVerticalScroller(bool);

    bool alwaysShowsHorizontalScroller() const { return m_alwaysShowsHorizontalScroller; };
    bool alwaysShowsVerticalScroller() const { return m_alwaysShowsVerticalScroller; };
    
    void scrollToRect(const CyberCore::FloatRect& targetRect, const CyberCore::FloatPoint& origin);

    void setMinimumSizeForAutoLayout(const CyberCore::IntSize&);
    CyberCore::IntSize minimumSizeForAutoLayout() const { return m_minimumSizeForAutoLayout; }

    void setSizeToContentAutoSizeMaximumSize(const CyberCore::IntSize&);
    CyberCore::IntSize sizeToContentAutoSizeMaximumSize() const { return m_sizeToContentAutoSizeMaximumSize; }

    void setAutoSizingShouldExpandToViewHeight(bool shouldExpand);
    bool autoSizingShouldExpandToViewHeight() { return m_autoSizingShouldExpandToViewHeight; }

    void setViewportSizeForCSSViewportUnits(std::optional<CyberCore::FloatSize>);
    std::optional<CyberCore::FloatSize> viewportSizeForCSSViewportUnits() const { return m_viewportSizeForCSSViewportUnits; }

    bool canShowMIMEType(const String& MIMEType) const;
    bool canShowResponse(const CyberCore::ResourceResponse&) const;

    void addTextCheckingRequest(TextCheckerRequestID, Ref<CyberCore::TextCheckingRequest>&&);
    void didFinishCheckingText(TextCheckerRequestID, const Vector<CyberCore::TextCheckingResult>&);
    void didCancelCheckingText(TextCheckerRequestID);

#if ENABLE(DATA_DETECTION)
    void setDataDetectionResults(NSArray *);
    void detectDataInAllFrames(OptionSet<CyberCore::DataDetectorType>, CompletionHandler<void(const DataDetectionResult&)>&&);
    void removeDataDetectedLinks(CompletionHandler<void(const DataDetectionResult&)>&&);
    void handleClickForDataDetectionResult(const CyberCore::DataDetectorElementInfo&, const CyberCore::IntPoint&);
#endif

    unsigned extendIncrementalRenderingSuppression();
    void stopExtendingIncrementalRenderingSuppression(unsigned token);
    bool shouldExtendIncrementalRenderingSuppression() { return !m_activeRenderingSuppressionTokens.isEmpty(); }

    CyberCore::ScrollPinningBehavior scrollPinningBehavior() { return m_scrollPinningBehavior; }
    void setScrollPinningBehavior(CyberCore::ScrollPinningBehavior);

    std::optional<CyberCore::ScrollbarOverlayStyle> scrollbarOverlayStyle() { return m_scrollbarOverlayStyle; }
    void setScrollbarOverlayStyle(std::optional<uint32_t /* CyberCore::ScrollbarOverlayStyle */> scrollbarStyle);

    Ref<CyberCore::DocumentLoader> createDocumentLoader(CyberCore::LocalFrame&, const CyberCore::ResourceRequest&, const CyberCore::SubstituteData&);
    void updateCachedDocumentLoader(WebDocumentLoader&, CyberCore::LocalFrame&);

    void getBytecodeProfile(CompletionHandler<void(const String&)>&&);
    void getSamplingProfilerOutput(CompletionHandler<void(const String&)>&&);
    
#if ENABLE(SERVICE_CONTROLS) || ENABLE(TELEPHONE_NUMBER_DETECTION)
    void handleTelephoneNumberClick(const String& number, const CyberCore::IntPoint&, const CyberCore::IntRect&);
    void handleSelectionServiceClick(CyberCore::FrameSelection&, const Vector<String>& telephoneNumbers, const CyberCore::IntPoint&);
    void handleImageServiceClick(const CyberCore::IntPoint&, CyberCore::Image&, CyberCore::HTMLImageElement&);
    void handlePDFServiceClick(const CyberCore::IntPoint&, CyberCore::HTMLAttachmentElement&);
#endif

    void didChangeScrollOffsetForFrame(CyberCore::LocalFrame*);

    void setMainFrameProgressCompleted(bool completed) { m_mainFrameProgressCompleted = completed; }
    bool shouldDispatchFakeMouseMoveEvents() const { return m_shouldDispatchFakeMouseMoveEvents; }

    void postMessage(const String& messageName, API::Object* messageBody);
    void postSynchronousMessageForTesting(const String& messageName, API::Object* messageBody, RefPtr<API::Object>& returnData);
    void postMessageIgnoringFullySynchronousMode(const String& messageName, API::Object* messageBody);

#if PLATFORM(GTK) || PLATFORM(WPE)
    void setInputMethodState(CyberCore::Element*);
#endif

    void imageOrMediaDocumentSizeChanged(const CyberCore::IntSize&);

#if ENABLE(MEDIA_STREAM) && USE(GSTREAMER)
    void setOrientationForMediaCapture(uint64_t rotation);
    void setMockCaptureDevicesInterrupted(bool isCameraInterrupted, bool isMicrophoneInterrupted);
#endif

    void addUserScript(String&& source, InjectedBundleScriptWorld&, CyberCore::UserContentInjectedFrames, CyberCore::UserScriptInjectionTime);
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
    void gamepadActivity(const Vector<std::optional<GamepadData>>&, CyberCore::EventMakesGamepadsVisible);
#endif
    
#if ENABLE(POINTER_LOCK)
    void didAcquirePointerLock();
    void didNotAcquirePointerLock();
    void didLosePointerLock();
#endif

    void didGetLoadDecisionForIcon(bool decision, CallbackID, CompletionHandler<void(const IPC::SharedBufferReference&)>&&);
    void setUseIconLoadingClient(bool);

#if PLATFORM(IOS_FAMILY) && ENABLE(DRAG_SUPPORT)
    void didConcludeEditDrag();
    void didConcludeDrop();
#endif

    void didFinishLoadingImageForElement(CyberCore::HTMLImageElement&);

    WebURLSchemeHandlerProxy* urlSchemeHandlerForScheme(StringView);
    void stopAllURLSchemeTasks();

    std::optional<double> cpuLimit() const { return m_cpuLimit; }

#if ENABLE(PDFKIT_PLUGIN)
    static PluginView* pluginViewForFrame(CyberCore::LocalFrame*);
    PluginView* mainFramePlugIn() const;
#endif

    void themeColorChanged() { m_pendingThemeColorChange = true; }
    void flushPendingThemeColorChange();

    void pageExtendedBackgroundColorDidChange() { m_pendingPageExtendedBackgroundColorChange = true; }
    void flushPendingPageExtendedBackgroundColorChange();

    void sampledPageTopColorChanged() { m_pendingSampledPageTopColorChange = true; }
    void flushPendingSampledPageTopColorChange();

    void flushPendingEditorStateUpdate();

#if ENABLE(TRACKING_PREVENTION)
    void hasStorageAccess(CyberCore::RegistrableDomain&& subFrameDomain, CyberCore::RegistrableDomain&& topFrameDomain, WebFrame&, CompletionHandler<void(bool)>&&);
    void requestStorageAccess(CyberCore::RegistrableDomain&& subFrameDomain, CyberCore::RegistrableDomain&& topFrameDomain, WebFrame&, CyberCore::StorageAccessScope, CompletionHandler<void(CyberCore::RequestStorageAccessResult)>&&);
    bool hasPageLevelStorageAccess(const CyberCore::RegistrableDomain& topLevelDomain, const CyberCore::RegistrableDomain& resourceDomain) const;
    void addDomainWithPageLevelStorageAccess(const CyberCore::RegistrableDomain& topLevelDomain, const CyberCore::RegistrableDomain& resourceDomain);
    void clearPageLevelStorageAccess();
    void wasLoadedWithDataTransferFromPrevalentResource();
    void didLoadFromRegistrableDomain(CyberCore::RegistrableDomain&&);
    void clearLoadedSubresourceDomains();
    void getLoadedSubresourceDomains(CompletionHandler<void(Vector<CyberCore::RegistrableDomain>)>&&);
    const HashSet<CyberCore::RegistrableDomain>& loadedSubresourceDomains() const { return m_loadedSubresourceDomains; }
#endif

#if ENABLE(DEVICE_ORIENTATION)
    void shouldAllowDeviceOrientationAndMotionAccess(CyberCore::FrameIdentifier, FrameInfoData&&, bool mayPrompt, CompletionHandler<void(CyberCore::DeviceOrientationOrMotionPermissionState)>&&);
#endif

    void showShareSheet(CyberCore::ShareDataWithParsedURL&, CompletionHandler<void(bool)>&& callback);
    void showContactPicker(const CyberCore::ContactsRequestData&, CompletionHandler<void(std::optional<Vector<CyberCore::ContactInfo>>&&)>&&);
    
#if ENABLE(ATTACHMENT_ELEMENT)
    void insertAttachment(const String& identifier, std::optional<uint64_t>&& fileSize, const String& fileName, const String& contentType, CompletionHandler<void()>&&);
    void updateAttachmentAttributes(const String& identifier, std::optional<uint64_t>&& fileSize, const String& contentType, const String& fileName, const IPC::SharedBufferReference& enclosingImageData, CompletionHandler<void()>&&);
    void updateAttachmentThumbnail(const String& identifier, const ShareableBitmap::Handle& qlThumbnailHandle);
    void updateAttachmentIcon(const String& identifier, const ShareableBitmap::Handle& icon, const CyberCore::FloatSize&);
    void requestAttachmentIcon(const String& identifier, const CyberCore::FloatSize&);
#endif

#if ENABLE(APPLICATION_MANIFEST)
    void getApplicationManifest(CompletionHandler<void(const std::optional<CyberCore::ApplicationManifest>&)>&&);
#endif

    void getTextFragmentMatch(CompletionHandler<void(const String&)>&&);

#if USE(WPE_RENDERER)
    int hostFileDescriptor() const { return m_hostFileDescriptor.value(); }
#endif

    void updateCurrentModifierState(OptionSet<CyberCore::PlatformEvent::Modifier> modifiers);

    inline UserContentControllerIdentifier userContentControllerIdentifier() const;

#if ENABLE(WK_WEB_EXTENSIONS)
    WebExtensionControllerProxy* webExtensionControllerProxy() const { return m_webExtensionController.get(); }
#endif

    CyberCore::UserInterfaceLayoutDirection userInterfaceLayoutDirection() const { return m_userInterfaceLayoutDirection; }

    bool isSuspended() const { return m_isSuspended; }

    void didReceiveWebPageMessage(IPC::Connection&, IPC::Decoder&);

    template<typename T>
    SendSyncResult<T> sendSyncWithDelayedReply(T&& message, OptionSet<IPC::SendSyncOption> sendSyncOptions = { })
    {
        cancelCurrentInteractionInformationRequest();
        sendSyncOptions = sendSyncOptions | IPC::SendSyncOption::InformPlatformProcessWillSuspend;
        return sendSync(WTFMove(message), Seconds::infinity(), sendSyncOptions);
    }

    CyberCore::DOMPasteAccessResponse requestDOMPasteAccess(CyberCore::DOMPasteAccessCategory, const String& originIdentifier);
    CyberCore::IntRect rectForElementAtInteractionLocation() const;

    const std::optional<CyberCore::Color>& backgroundColor() const { return m_backgroundColor; }

    void suspendAllMediaBuffering();
    void resumeAllMediaBuffering();

    void configureLoggingChannel(const String&, WTFLogChannelState, WTFLogLevel);

    RefPtr<CyberCore::Element> elementForContext(const CyberCore::ElementContext&) const;
    std::optional<CyberCore::ElementContext> contextForElement(CyberCore::Element&) const;

    void startTextManipulations(Vector<CyberCore::TextManipulationController::ExclusionRule>&&, bool includesSubframes, CompletionHandler<void()>&&);
    void completeTextManipulation(const Vector<CyberCore::TextManipulationItem>&, CompletionHandler<void(bool allFailed, const Vector<CyberCore::TextManipulationController::ManipulationFailure>&)>&&);

#if ENABLE(APPLE_PAY)
    WebPaymentCoordinator* paymentCoordinator();
#endif

#if ENABLE(PLATFORM_DRIVEN_TEXT_CHECKING)
    TextCheckingControllerProxy& textCheckingController() { return m_textCheckingControllerProxy.get(); }
#endif

#if PLATFORM(COCOA)
    void setRemoteObjectRegistry(WebRemoteObjectRegistry*);
    WebRemoteObjectRegistry* remoteObjectRegistry();
#endif

    WebPageProxyIdentifier webPageProxyIdentifier() const { return m_webPageProxyIdentifier; }

    void scheduleIntrinsicContentSizeUpdate(const CyberCore::IntSize&);
    void flushPendingIntrinsicContentSizeUpdate();
    void updateIntrinsicContentSizeIfNeeded(const CyberCore::IntSize&);

    void scheduleFullEditorStateUpdate();

    bool userIsInteracting() const { return m_userIsInteracting; }
    void setUserIsInteracting(bool userIsInteracting) { m_userIsInteracting = userIsInteracting; }

#if PLATFORM(IOS_FAMILY)
    // This excludes layout overflow, includes borders.
    static CyberCore::IntRect rootViewBounds(const CyberCore::Node&);
    // These include layout overflow for overflow:visible elements, but exclude borders.
    static CyberCore::IntRect absoluteInteractionBounds(const CyberCore::Node&);
    static CyberCore::IntRect rootViewInteractionBounds(const CyberCore::Node&);

    InteractionInformationAtPosition positionInformation(const InteractionInformationRequest&);
    
#endif // PLATFORM(IOS_FAMILY)

#if USE(QUICK_LOOK)
    void didStartLoadForQuickLookDocumentInMainFrame(const String& fileName, const String& uti);
    void didFinishLoadForQuickLookDocumentInMainFrame(const CyberCore::FragmentedSharedBuffer&);
    void requestPasswordForQuickLookDocumentInMainFrame(const String& fileName, CompletionHandler<void(const String&)>&&);
#endif

    const AtomString& overriddenMediaType() const { return m_overriddenMediaType; }
    void setOverriddenMediaType(const String&);

    void updateCORSDisablingPatterns(Vector<String>&&);

#if ENABLE(IPC_TESTING_API)
    bool ipcTestingAPIEnabled() const { return m_ipcTestingAPIEnabled; }
    uint64_t webPageProxyID() const { return messageSenderDestinationID(); }
    uint64_t visitedLinkTableID() const { return m_visitedLinkTableID; }
#endif

    void getProcessDisplayName(CompletionHandler<void(String&&)>&&);

    CyberCore::AllowsContentJavaScript allowsContentJavaScriptFromMostRecentNavigation() const { return m_allowsContentJavaScriptFromMostRecentNavigation; }
    void setAllowsContentJavaScriptFromMostRecentNavigation(CyberCore::AllowsContentJavaScript allows) { m_allowsContentJavaScriptFromMostRecentNavigation = allows; }

#if ENABLE(APP_BOUND_DOMAINS)
    void notifyPageOfAppBoundBehavior();
    void setIsNavigatingToAppBoundDomain(std::optional<NavigatingToAppBoundDomain>, WebFrame*);
    bool needsInAppBrowserPrivacyQuirks() { return m_needsInAppBrowserPrivacyQuirks; }
#endif

#if ENABLE(MEDIA_USAGE)
    void addMediaUsageManagerSession(CyberCore::MediaSessionIdentifier, const String&, const URL&);
    void updateMediaUsageManagerSessionState(CyberCore::MediaSessionIdentifier, const CyberCore::MediaUsageInfo&);
    void removeMediaUsageManagerSession(CyberCore::MediaSessionIdentifier);
#endif

    void isPlayingMediaDidChange(CyberCore::MediaProducerMediaStateFlags);

    URL sanitizeLookalikeCharacters(const URL&, CyberCore::LookalikeCharacterSanitizationTrigger);
    URL allowedLookalikeCharacters(const URL&);

#if ENABLE(IMAGE_ANALYSIS)
    void requestTextRecognition(CyberCore::Element&, CyberCore::TextRecognitionOptions&&, CompletionHandler<void(RefPtr<CyberCore::Element>&&)>&& = { });
    void updateWithTextRecognitionResult(const CyberCore::TextRecognitionResult&, const CyberCore::ElementContext&, const CyberCore::FloatPoint& location, CompletionHandler<void(TextRecognitionUpdateResult)>&&);
    void startVisualTranslation(const String& sourceLanguageIdentifier, const String& targetLanguageIdentifier);
#endif

    void requestImageBitmap(const CyberCore::ElementContext&, CompletionHandler<void(const ShareableBitmap::Handle&, const String& sourceMIMEType)>&&);

#if HAVE(TRANSLATION_UI_SERVICES) && ENABLE(CONTEXT_MENUS)
    void handleContextMenuTranslation(const CyberCore::TranslationContextMenuInfo&);
#endif

#if ENABLE(MEDIA_CONTROLS_CONTEXT_MENUS) && USE(UICONTEXTMENU)
    void showMediaControlsContextMenu(CyberCore::FloatRect&&, Vector<CyberCore::MediaControlsContextMenuItem>&&, CompletionHandler<void(CyberCore::MediaControlsContextMenuItem::ID)>&&);
#endif // ENABLE(MEDIA_CONTROLS_CONTEXT_MENUS) && USE(UICONTEXTMENU)

#if USE(GRAPHICS_LAYER_TEXTURE_MAPPER) || USE(GRAPHICS_LAYER_WC)
    uint64_t nativeWindowHandle() { return m_nativeWindowHandle; }
#endif

    static void updatePreferencesGenerated(const WebPreferencesStore&);
    static void updateSettingsGenerated(const WebPreferencesStore&, CyberCore::Settings&);

    void synchronizeCORSDisablingPatternsWithNetworkProcess();

#if ENABLE(GPU_PROCESS)
    void gpuProcessConnectionDidBecomeAvailable(GPUProcessConnection&);
    RemoteRenderingBackendProxy& ensureRemoteRenderingBackendProxy();
#endif

#if ENABLE(APP_HIGHLIGHTS)
    CyberCore::CreateNewGroupForHighlight highlightIsNewGroup() const { return m_highlightIsNewGroup; }
    CyberCore::HighlightRequestOriginatedInApp highlightRequestOriginatedInApp() const { return m_highlightRequestOriginatedInApp; }
    CyberCore::HighlightVisibility appHighlightsVisiblility() const { return m_appHighlightsVisible; }

    bool createAppHighlightInSelectedRange(CyberCore::CreateNewGroupForHighlight, CyberCore::HighlightRequestOriginatedInApp);
    void restoreAppHighlightsAndScrollToIndex(const Vector<SharedMemory::Handle>&&, const std::optional<unsigned> index);
    void setAppHighlightsVisibility(const CyberCore::HighlightVisibility);
#endif

#if PLATFORM(IOS_FAMILY)
    void dispatchWheelEventWithoutScrolling(const WebWheelEvent&, CompletionHandler<void(bool)>&&);
#endif

#if ENABLE(PDFKIT_PLUGIN)
    bool shouldUsePDFPlugin(const String& contentType, StringView path) const;
#endif

#if ENABLE(MEDIA_SESSION_COORDINATOR)
    void createMediaSessionCoordinator(const String&, CompletionHandler<void(bool)>&&);
#endif

    void setLastNavigationWasAppInitiated(bool wasAppBound) { m_lastNavigationWasAppInitiated = wasAppBound; }
    void lastNavigationWasAppInitiated(CompletionHandler<void(bool)>&&);

    bool isParentProcessAWebBrowser() const;

#if ENABLE(TEXT_AUTOSIZING)
    void textAutosizingUsesIdempotentModeChanged();
#endif

#if ENABLE(WEBXR) && !USE(OPENXR)
    PlatformXRSystemProxy& xrSystemProxy();
#endif

    void prepareToRunModalJavaScriptDialog();

#if ENABLE(ARKIT_INLINE_PREVIEW)
    bool useARKitForModel() const { return m_useARKitForModel; };
#endif
#if HAVE(SCENEKIT)
    bool useSceneKitForModel() const { return m_useSceneKitForModel; };
#endif

#if ENABLE(ARKIT_INLINE_PREVIEW_IOS)
    void modelInlinePreviewDidLoad(CyberCore::PlatformLayerIdentifier);
    void modelInlinePreviewDidFailToLoad(CyberCore::PlatformLayerIdentifier, const CyberCore::ResourceError&);
#endif

#if ENABLE(VIDEO)
    void beginTextRecognitionForVideoInElementFullScreen(const CyberCore::HTMLVideoElement&);
    void cancelTextRecognitionForVideoInElementFullScreen();
#endif

#if ENABLE(IMAGE_ANALYSIS_ENHANCEMENTS)
    void shouldAllowRemoveBackground(const CyberCore::ElementContext&, CompletionHandler<void(bool)>&&) const;
#endif

#if HAVE(UIKIT_RESIZABLE_WINDOWS)
    void setIsWindowResizingEnabled(bool);
#endif

#if ENABLE(INTERACTION_REGIONS_IN_EVENT_REGION)
    void setInteractionRegionsEnabled(bool);
#endif

    void generateTestReport(String&& message, String&& group);
    
    bool isUsingUISideCompositing() const;

#if ENABLE(ACCESSIBILITY_ANIMATION_CONTROL)
    void updateImageAnimationEnabled();
    void pauseAllAnimations(CompletionHandler<void()>&&);
    void playAllAnimations(CompletionHandler<void()>&&);
    void isAnyAnimationAllowedToPlayDidChange(bool /* anyAnimationCanPlay */);
#endif

    bool shouldSkipDecidePolicyForResponse(const CyberCore::ResourceResponse&, const CyberCore::ResourceRequest&) const;
    void setSkipDecidePolicyForResponseIfPossible(bool value) { m_skipDecidePolicyForResponseIfPossible = value; }

private:
    WebPage(CyberCore::PageIdentifier, WebPageCreationParameters&&);

    void constructFrameTree(WebFrame& parent, CyberCore::FrameIdentifier localFrameIdentifier, CyberCore::LayerHostingContextIdentifier localFrameHostLayerIdentifier, const FrameTreeCreationParameters&);

    void updateThrottleState();

#if ENABLE(NOTIFICATIONS)
    void clearNotificationPermissionState();
#endif

    // IPC::MessageSender
    IPC::Connection* messageSenderConnection() const override;
    uint64_t messageSenderDestinationID() const override;

    void platformInitialize(const WebPageCreationParameters&);
    void platformReinitialize();
    void platformDetach();
    void getPlatformEditorState(CyberCore::LocalFrame&, EditorState&) const;
    bool requiresPostLayoutDataForEditorState(const CyberCore::LocalFrame&) const;
    void platformWillPerformEditingCommand();
    void sendEditorStateUpdate();

    void getPlatformEditorStateCommon(const CyberCore::LocalFrame&, EditorState&) const;

#if HAVE(TOUCH_BAR)
    void sendTouchBarMenuDataAddedUpdate(CyberCore::HTMLMenuElement&);
    void sendTouchBarMenuDataRemovedUpdate(CyberCore::HTMLMenuElement&);
    void sendTouchBarMenuItemDataAddedUpdate(CyberCore::HTMLMenuItemElement&);
    void sendTouchBarMenuItemDataRemovedUpdate(CyberCore::HTMLMenuItemElement&);
#endif

    bool didReceiveSyncWebPageMessage(IPC::Connection&, IPC::Decoder&, UniqueRef<IPC::Encoder>&);

    void updateSizeForCSSDefaultViewportUnits();
    void updateSizeForCSSSmallViewportUnits();
    void updateSizeForCSSLargeViewportUnits();

#if PLATFORM(IOS_FAMILY)
    std::optional<FocusedElementInformation> focusedElementInformation();
    void generateSyntheticEditingCommand(SyntheticEditingCommandType);
    void handleSyntheticClick(CyberCore::Node& nodeRespondingToClick, const CyberCore::FloatPoint& location, OptionSet<CyberKit::WebEventModifier>, CyberCore::PointerID = CyberCore::mousePointerID);
    void completeSyntheticClick(CyberCore::Node& nodeRespondingToClick, const CyberCore::FloatPoint& location, OptionSet<CyberKit::WebEventModifier>, CyberCore::SyntheticClickType, CyberCore::PointerID = CyberCore::mousePointerID);
    void sendTapHighlightForNodeIfNecessary(CyberKit::TapIdentifier, CyberCore::Node*);
    CyberCore::VisiblePosition visiblePositionInFocusedNodeForPoint(const CyberCore::LocalFrame&, const CyberCore::IntPoint&, bool isInteractingWithFocusedElement);
    std::optional<CyberCore::SimpleRange> rangeForGranularityAtPoint(CyberCore::LocalFrame&, const CyberCore::IntPoint&, CyberCore::TextGranularity, bool isInteractingWithFocusedElement);
    void setFocusedFrameBeforeSelectingTextAtLocation(const CyberCore::IntPoint&);
    void setSelectedRangeDispatchingSyntheticMouseEventsIfNeeded(const CyberCore::SimpleRange&, CyberCore::Affinity);
    void dispatchSyntheticMouseEventsForSelectionGesture(SelectionTouch, const CyberCore::IntPoint&);

    void sendPositionInformation(InteractionInformationAtPosition&&);
    RefPtr<ShareableBitmap> shareableBitmapSnapshotForNode(CyberCore::Element&);
    WebAutocorrectionContext autocorrectionContext();
    bool applyAutocorrectionInternal(const String& correction, const String& originalText, bool isCandidate);
    void clearSelectionAfterTapIfNeeded();
#endif

#if ENABLE(NETWORK_CONNECTION_INTEGRITY)
    void setLookalikeCharacterStrings(Vector<CyberCore::LookalikeCharactersSanitizationData>&&);
    void setAllowedLookalikeCharacterStrings(Vector<CyberCore::LookalikeCharactersSanitizationData>&&);
#endif

#if ENABLE(META_VIEWPORT)
    void resetViewportDefaultConfiguration(WebFrame* mainFrame, bool hasMobileDocType = false);
    enum class ZoomToInitialScale : bool { No, Yes };
    void viewportConfigurationChanged(ZoomToInitialScale = ZoomToInitialScale::No);
    bool shouldIgnoreMetaViewport() const;
#endif

#if ENABLE(TEXT_AUTOSIZING)
    void textAutoSizingAdjustmentTimerFired();
    void resetIdempotentTextAutosizingIfNeeded(double previousInitialScale);
#endif
    void resetTextAutosizing();

#if ENABLE(VIEWPORT_RESIZING)
    void shrinkToFitContent(ZoomToInitialScale = ZoomToInitialScale::No);
#endif

#if PLATFORM(IOS_FAMILY) && ENABLE(DRAG_SUPPORT)
    void requestDragStart(const CyberCore::IntPoint& clientPosition, const CyberCore::IntPoint& globalPosition, OptionSet<CyberCore::DragSourceAction> allowedActionsMask);
    void requestAdditionalItemsForDragSession(const CyberCore::IntPoint& clientPosition, const CyberCore::IntPoint& globalPosition, OptionSet<CyberCore::DragSourceAction> allowedActionsMask);
    void insertDroppedImagePlaceholders(const Vector<CyberCore::IntSize>&, CompletionHandler<void(const Vector<CyberCore::IntRect>&, std::optional<CyberCore::TextIndicatorData>)>&& reply);
    void computeAndSendEditDragSnapshot();
#endif

#if !PLATFORM(COCOA) && !PLATFORM(WPE)
    static const char* interpretKeyEvent(const CyberCore::KeyboardEvent*);
#endif

    bool handleKeyEventByRelinquishingFocusToChrome(const CyberCore::KeyboardEvent&);

#if PLATFORM(MAC)
    bool executeKeypressCommandsInternal(const Vector<CyberCore::KeypressCommand>&, CyberCore::KeyboardEvent*);
#endif

    void testProcessIncomingSyncMessagesWhenWaitingForSyncReply(CompletionHandler<void(bool)>&&);

    void updateDrawingAreaLayerTreeFreezeState();

    enum class MarkLayersVolatileDontRetryReason : uint8_t { None, SuspendedUnderLock, TimedOut };
    void markLayersVolatileOrRetry(MarkLayersVolatileDontRetryReason);
    void layerVolatilityTimerFired();
    void callVolatilityCompletionHandlers(bool succeeded);

    void tryMarkLayersVolatile(CompletionHandler<void(bool)>&&);
    void tryMarkLayersVolatileCompletionHandler(MarkLayersVolatileDontRetryReason, bool didSucceed);

    String sourceForFrame(WebFrame*);

    void startTextManipulationForFrame(CyberCore::Frame&);

    void loadDataImpl(uint64_t navigationID, CyberCore::ShouldTreatAsContinuingLoad, std::optional<WebsitePoliciesData>&&, Ref<CyberCore::FragmentedSharedBuffer>&&, CyberCore::ResourceRequest&&, CyberCore::ResourceResponse&&, const URL& failingURL, const UserData&, std::optional<NavigatingToAppBoundDomain>, CyberCore::SubstituteData::SessionHistoryVisibility, CyberCore::ShouldOpenExternalURLsPolicy = CyberCore::ShouldOpenExternalURLsPolicy::ShouldNotAllow);

    // Actions
    void tryClose(CompletionHandler<void(bool)>&&);
    void platformDidReceiveLoadParameters(const LoadParameters&);
    void loadRequestByCreatingNewLocalFrameOrConvertingRemoteFrame(LocalFrameCreationParameters&&, LoadParameters&&);
    void loadRequest(LoadParameters&&);
    [[noreturn]] void loadRequestWaitingForProcessLaunch(LoadParameters&&, URL&&, WebPageProxyIdentifier, bool);
    void loadData(LoadParameters&&);
    void loadAlternateHTML(LoadParameters&&);
    void loadSimulatedRequestAndResponse(LoadParameters&&, CyberCore::ResourceResponse&&);
    void navigateToPDFLinkWithSimulatedClick(const String& url, CyberCore::IntPoint documentPoint, CyberCore::IntPoint screenPoint);
    void getPDFFirstPageSize(CyberCore::FrameIdentifier, CompletionHandler<void(CyberCore::FloatSize)>&&);
    void reload(uint64_t navigationID, OptionSet<CyberCore::ReloadOption> reloadOptions, SandboxExtension::Handle&&);
    void goToBackForwardItem(uint64_t navigationID, const CyberCore::BackForwardItemIdentifier&, CyberCore::FrameLoadType, CyberCore::ShouldTreatAsContinuingLoad, std::optional<WebsitePoliciesData>&&, bool lastNavigationWasAppInitiated, std::optional<NetworkResourceLoadIdentifier> existingNetworkResourceLoadIdentifierToResume, std::optional<String> topPrivatelyControlledDomain);
    void tryRestoreScrollPosition();
    void setInitialFocus(bool forward, bool isKeyboardEventValid, const WebKeyboardEvent&, CompletionHandler<void()>&&);
    void updateIsInWindow(bool isInitialState = false);
    void visibilityDidChange();
    void setActivityState(OptionSet<CyberCore::ActivityState>, ActivityStateChangeID, CompletionHandler<void()>&&);
    void validateCommand(const String&, CompletionHandler<void(bool, int32_t)>&&);
    void executeEditCommand(const String&, const String&);
    void setEditable(bool);

    void didChangeSelectionOrOverflowScrollPosition();

    void increaseListLevel();
    void decreaseListLevel();
    void changeListType();

    void setBaseWritingDirection(CyberCore::WritingDirection);

    void setNeedsFontAttributes(bool);

    void mouseEvent(const WebMouseEvent&, std::optional<Vector<SandboxExtension::Handle>>&& sandboxExtensions);
    void keyEvent(const WebKeyboardEvent&);

#if ENABLE(IOS_TOUCH_EVENTS)
    void touchEventSync(const WebTouchEvent&, CompletionHandler<void(bool)>&&);
    void resetPotentialTapSecurityOrigin();
    void updatePotentialTapSecurityOrigin(const WebTouchEvent&, bool wasHandled);
#elif ENABLE(TOUCH_EVENTS)
    void touchEvent(const WebTouchEvent&);
#endif

    void cancelPointer(CyberCore::PointerID, const CyberCore::IntPoint&);
    void touchWithIdentifierWasRemoved(CyberCore::PointerID);

#if ENABLE(CONTEXT_MENUS)
    void didShowContextMenu();
    void didDismissContextMenu();
#endif
#if ENABLE(CONTEXT_MENU_EVENT)
    void contextMenuForKeyEvent();
#endif

    static bool scroll(CyberCore::Page*, CyberCore::ScrollDirection, CyberCore::ScrollGranularity);
    static bool logicalScroll(CyberCore::Page*, CyberCore::ScrollLogicalDirection, CyberCore::ScrollGranularity);

    void loadURLInFrame(URL&&, const String& referrer, CyberCore::FrameIdentifier);
    void loadDataInFrame(IPC::DataReference&&, String&& MIMEType, String&& encodingName, URL&& baseURL, CyberCore::FrameIdentifier);

    enum class WasRestoredByAPIRequest : bool { No, Yes };
    void restoreSessionInternal(const Vector<BackForwardListItemState>&, WasRestoredByAPIRequest, WebBackForwardListProxy::OverwriteExistingItem);
    void restoreSession(const Vector<BackForwardListItemState>&);
    void didRemoveBackForwardItem(const CyberCore::BackForwardItemIdentifier&);
    void updateBackForwardListForReattach(const Vector<CyberKit::BackForwardListItemState>&);
    void setCurrentHistoryItemForReattach(CyberKit::BackForwardListItemState&&);

    void requestFontAttributesAtSelectionStart(CompletionHandler<void(const CyberCore::FontAttributes&)>&&);

#if ENABLE(REMOTE_INSPECTOR)
    void setIndicating(bool);
#endif

    void setBackgroundColor(const std::optional<CyberCore::Color>&);

#if PLATFORM(COCOA)
    void setTopContentInsetFenced(float, const WTF::MachSendRight&);
#endif
    void setTopContentInset(float);

    void viewWillStartLiveResize();
    void viewWillEndLiveResize();

    void getContentsAsString(ContentAsStringIncludesChildFrames, CompletionHandler<void(const String&)>&&);
#if PLATFORM(COCOA)
    void getContentsAsAttributedString(CompletionHandler<void(const CyberCore::AttributedString&)>&&);
#endif
#if ENABLE(MHTML)
    void getContentsAsMHTMLData(CompletionHandler<void(const IPC::SharedBufferReference&)>&& callback);
#endif
    void getMainResourceDataOfFrame(CyberCore::FrameIdentifier, CompletionHandler<void(const std::optional<IPC::SharedBufferReference>&)>&&);
    void getResourceDataFromFrame(CyberCore::FrameIdentifier, const String& resourceURL, CompletionHandler<void(const std::optional<IPC::SharedBufferReference>&)>&&);
    void getRenderTreeExternalRepresentation(CompletionHandler<void(const String&)>&&);
    void getSelectionOrContentsAsString(CompletionHandler<void(const String&)>&&);
    void getSelectionAsWebArchiveData(CompletionHandler<void(const std::optional<IPC::SharedBufferReference>&)>&&);
    void getSourceForFrame(CyberCore::FrameIdentifier, CompletionHandler<void(const String&)>&&);
    void getWebArchiveOfFrame(CyberCore::FrameIdentifier, CompletionHandler<void(const std::optional<IPC::SharedBufferReference>&)>&&);
    void runJavaScript(WebFrame*, CyberCore::RunJavaScriptParameters&&, ContentWorldIdentifier, CompletionHandler<void(const IPC::DataReference&, const std::optional<CyberCore::ExceptionDetails>&)>&&);
    void runJavaScriptInFrameInScriptWorld(CyberCore::RunJavaScriptParameters&&, std::optional<CyberCore::FrameIdentifier>, const std::pair<ContentWorldIdentifier, String>& worldData, CompletionHandler<void(const IPC::DataReference&, const std::optional<CyberCore::ExceptionDetails>&)>&&);
    void getAccessibilityTreeData(CompletionHandler<void(const std::optional<IPC::SharedBufferReference>&)>&&);
    void forceRepaint(CompletionHandler<void()>&&);
    void takeSnapshot(CyberCore::IntRect snapshotRect, CyberCore::IntSize bitmapSize, uint32_t options, CompletionHandler<void(const CyberKit::ShareableBitmap::Handle&)>&&);

    void preferencesDidChange(const WebPreferencesStore&);
    void preferencesDidChangeDuringDOMPrintOperation(const WebPreferencesStore& store) { preferencesDidChange(store); }
    void updatePreferences(const WebPreferencesStore&);

#if PLATFORM(IOS_FAMILY)
    bool parentProcessHasServiceWorkerEntitlement() const;
    void disableServiceWorkerEntitlement();
    void clearServiceWorkerEntitlementOverride(CompletionHandler<void()>&&);
#else
    bool parentProcessHasServiceWorkerEntitlement() const { return true; }
    void disableServiceWorkerEntitlement() { }
    void clearServiceWorkerEntitlementOverride(CompletionHandler<void()>&& completionHandler) { completionHandler(); }
#endif

#if ENABLE(CONTENT_FILTERING_IN_NETWORKING_PROCESS)
    void didReceivePolicyDecision(CyberCore::FrameIdentifier, uint64_t listenerID, PolicyDecision&&);
#else
    void didReceivePolicyDecision(CyberCore::FrameIdentifier, uint64_t listenerID, PolicyDecision&&, const Vector<SandboxExtension::Handle>&);
#endif
    void setUserAgent(const String&);
    void setCustomTextEncodingName(const String&);
    void suspendActiveDOMObjectsAndAnimations();
    void resumeActiveDOMObjectsAndAnimations();

    void suspend(CompletionHandler<void(bool)>&&);
    void resume(CompletionHandler<void(bool)>&&);

#if PLATFORM(COCOA)
    void performDictionaryLookupAtLocation(const CyberCore::FloatPoint&);
    void performDictionaryLookupOfCurrentSelection();
    void performDictionaryLookupForRange(CyberCore::LocalFrame&, const CyberCore::SimpleRange&, NSDictionary *options, CyberCore::TextIndicatorPresentationTransition);
    CyberCore::DictionaryPopupInfo dictionaryPopupInfoForRange(CyberCore::LocalFrame&, const CyberCore::SimpleRange&, NSDictionary *options, CyberCore::TextIndicatorPresentationTransition);
#if ENABLE(PDFKIT_PLUGIN)
    CyberCore::DictionaryPopupInfo dictionaryPopupInfoForSelectionInPDFPlugin(PDFSelection *, PluginView&, NSDictionary *options, CyberCore::TextIndicatorPresentationTransition);
#endif

    void windowAndViewFramesChanged(const CyberCore::FloatRect& windowFrameInScreenCoordinates, const CyberCore::FloatRect& windowFrameInUnflippedScreenCoordinates, const CyberCore::FloatRect& viewFrameInWindowCoordinates, const CyberCore::FloatPoint& accessibilityViewCoordinates);

    RetainPtr<PDFDocument> pdfDocumentForPrintingFrame(CyberCore::LocalFrame*);
    void computePagesForPrintingPDFDocument(CyberCore::FrameIdentifier, const PrintInfo&, Vector<CyberCore::IntRect>& resultPageRects);
    void drawPDFDocument(CGContextRef, PDFDocument *, const PrintInfo&, const CyberCore::IntRect&);
    void drawPagesToPDFFromPDFDocument(CGContextRef, PDFDocument *, const PrintInfo&, uint32_t first, uint32_t count);
#endif

    void endPrintingImmediately();

#if ENABLE(META_VIEWPORT)
    bool shouldEnableViewportBehaviorsForResizableWindows() const;
#endif

#if HAVE(APP_ACCENT_COLORS)
    void setAccentColor(CyberCore::Color);
#endif

    void setMainFrameIsScrollable(bool);

    void unapplyEditCommand(WebUndoStepID commandID);
    void reapplyEditCommand(WebUndoStepID commandID);
    void didRemoveEditCommand(WebUndoStepID commandID);

    void findString(const String&, OptionSet<FindOptions>, uint32_t maxMatchCount, CompletionHandler<void(bool)>&&);
    void findStringMatches(const String&, OptionSet<FindOptions>, uint32_t maxMatchCount);
    void getImageForFindMatch(uint32_t matchIndex);
    void selectFindMatch(uint32_t matchIndex);
    void indicateFindMatch(uint32_t matchIndex);
    void hideFindUI();
    void countStringMatches(const String&, OptionSet<FindOptions>, uint32_t maxMatchCount);
    void replaceMatches(const Vector<uint32_t>& matchIndices, const String& replacementText, bool selectionOnly, CompletionHandler<void(uint64_t)>&&);
    void findRectsForStringMatches(const String&, OptionSet<FindOptions>, uint32_t maxMatchCount, CompletionHandler<void(Vector<CyberCore::FloatRect>&&)>&&);
    void hideFindIndicator();

    void findTextRangesForStringMatches(const String&, OptionSet<FindOptions>, uint32_t maxMatchCount, CompletionHandler<void(Vector<WebFoundTextRange>&&)>&&);
    void replaceFoundTextRangeWithString(const WebFoundTextRange&, const String&);
    void decorateTextRangeWithStyle(const WebFoundTextRange&, CyberKit::FindDecorationStyle);
    void scrollTextRangeToVisible(const WebFoundTextRange&);
    void clearAllDecoratedFoundText();
    void didBeginTextSearchOperation();
    void didEndTextSearchOperation();

    void requestRectForFoundTextRange(const WebFoundTextRange&, CompletionHandler<void(CyberCore::FloatRect)>&&);
    void addLayerForFindOverlay(CompletionHandler<void(CyberCore::PlatformLayerIdentifier)>&&);
    void removeLayerForFindOverlay(CompletionHandler<void()>&&);

#if USE(COORDINATED_GRAPHICS)
    void sendViewportAttributesChanged(const CyberCore::ViewportArguments&);
#endif

    void didChangeSelectedIndexForActivePopupMenu(int32_t newIndex);
    void setTextForActivePopupMenu(int32_t index);

#if PLATFORM(GTK)
    void failedToShowPopupMenu();
#endif

    void didChooseFilesForOpenPanel(const Vector<String>& files, const Vector<String>& replacementFiles);
    void didCancelForOpenPanel();

#if PLATFORM(IOS_FAMILY)
    void didChooseFilesForOpenPanelWithDisplayStringAndIcon(const Vector<String>&, const String& displayString, const IPC::DataReference& iconData, CyberKit::SandboxExtension::Handle&&, CyberKit::SandboxExtension::Handle&&, CyberKit::SandboxExtension::Handle&&);
#endif

#if ENABLE(SANDBOX_EXTENSIONS)
    void extendSandboxForFilesFromOpenPanel(Vector<SandboxExtension::Handle>&&);
#endif

    void didReceiveGeolocationPermissionDecision(GeolocationIdentifier, const String& authorizationToken);

#if ENABLE(MEDIA_STREAM)
    void userMediaAccessWasGranted(CyberCore::UserMediaRequestIdentifier, CyberCore::CaptureDevice&& audioDeviceUID, CyberCore::CaptureDevice&& videoDeviceUID, CyberCore::MediaDeviceHashSalts&& mediaDeviceIdentifierHashSalt, Vector<SandboxExtension::Handle>&&, CompletionHandler<void()>&&);
    void userMediaAccessWasDenied(CyberCore::UserMediaRequestIdentifier, uint64_t reason, String&& invalidConstraint);
#endif

#if ENABLE(ENCRYPTED_MEDIA)
    void mediaKeySystemWasGranted(CyberCore::MediaKeySystemRequestIdentifier, CompletionHandler<void()>&&);
    void mediaKeySystemWasDenied(CyberCore::MediaKeySystemRequestIdentifier, String&& message);
#endif

    void requestMediaPlaybackState(CompletionHandler<void(CyberKit::MediaPlaybackState)>&&);

    void pauseAllMediaPlayback(CompletionHandler<void()>&&);
    void suspendAllMediaPlayback(CompletionHandler<void()>&&);
    void resumeAllMediaPlayback(CompletionHandler<void()>&&);

    void advanceToNextMisspelling(bool startBeforeSelection);
    void changeSpellingToWord(const String& word);

#if USE(APPKIT)
    void uppercaseWord();
    void lowercaseWord();
    void capitalizeWord();
#endif

    bool shouldDispatchSyntheticMouseEventsWhenModifyingSelection() const;
    void platformDidSelectAll();
    
    void setHasResourceLoadClient(bool);
    void setCanUseCredentialStorage(bool);

#if ENABLE(CONTEXT_MENUS)
    void didSelectItemFromActiveContextMenu(const WebContextMenuItemData&);
#endif

    void changeSelectedIndex(int32_t index);
    void setCanStartMediaTimerFired();

    static bool platformCanHandleRequest(const CyberCore::ResourceRequest&);

#if ENABLE(PDFKIT_PLUGIN)
    static PluginView* focusedPluginViewForFrame(CyberCore::LocalFrame&);
#endif

    void reportUsedFeatures();

    void updateWebsitePolicies(WebsitePoliciesData&&);
    void notifyUserScripts();

    void changeFont(CyberCore::FontChanges&&);
    void changeFontAttributes(CyberCore::FontAttributeChanges&&);

#if PLATFORM(MAC)
    void performImmediateActionHitTestAtLocation(CyberCore::FloatPoint);
    std::optional<std::tuple<CyberCore::SimpleRange, NSDictionary *>> lookupTextAtLocation(CyberCore::FloatPoint);
    void immediateActionDidUpdate();
    void immediateActionDidCancel();
    void immediateActionDidComplete();

    void dataDetectorsDidPresentUI(CyberCore::PageOverlay::PageOverlayID);
    void dataDetectorsDidChangeUI(CyberCore::PageOverlay::PageOverlayID);
    void dataDetectorsDidHideUI(CyberCore::PageOverlay::PageOverlayID);

    void handleAcceptedCandidate(CyberCore::TextCheckingResult);
#endif

#if PLATFORM(COCOA)
    void requestActiveNowPlayingSessionInfo(CompletionHandler<void(bool, bool, const String&, double, double, uint64_t)>&&);
    RetainPtr<NSData> accessibilityRemoteTokenData() const;
    void accessibilityTransferRemoteToken(RetainPtr<NSData>);
#endif

    void setShouldDispatchFakeMouseMoveEvents(bool dispatch) { m_shouldDispatchFakeMouseMoveEvents = dispatch; }

#if ENABLE(WIRELESS_PLAYBACK_TARGET) && !PLATFORM(IOS_FAMILY)
    void playbackTargetSelected(CyberCore::PlaybackTargetClientContextIdentifier, CyberCore::MediaPlaybackTargetContext&&) const;
    void playbackTargetAvailabilityDidChange(CyberCore::PlaybackTargetClientContextIdentifier, bool);
    void setShouldPlayToPlaybackTarget(CyberCore::PlaybackTargetClientContextIdentifier, bool);
    void playbackTargetPickerWasDismissed(CyberCore::PlaybackTargetClientContextIdentifier);
#endif

    void clearWheelEventTestMonitor();

    void setShouldScaleViewToFitDocument(bool);

    void pageStoppedScrolling();

    void setUserInterfaceLayoutDirection(uint32_t);

    bool canPluginHandleResponse(const CyberCore::ResourceResponse&);

    void simulateDeviceOrientationChange(double alpha, double beta, double gamma);

#if USE(SYSTEM_PREVIEW)
    void systemPreviewActionTriggered(CyberCore::SystemPreviewInfo, const String& message);
#endif

#if ENABLE(SPEECH_SYNTHESIS)
    void speakingErrorOccurred();
    void boundaryEventOccurred(bool wordBoundary, unsigned charIndex, unsigned charLength);
    void voicesDidChange();
#endif

    void registerURLSchemeHandler(WebURLSchemeHandlerIdentifier, const String& scheme);

    void urlSchemeTaskWillPerformRedirection(WebURLSchemeHandlerIdentifier, CyberCore::ResourceLoaderIdentifier taskIdentifier, CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&, CompletionHandler<void(CyberCore::ResourceRequest&&)>&&);
    void urlSchemeTaskDidPerformRedirection(WebURLSchemeHandlerIdentifier, CyberCore::ResourceLoaderIdentifier taskIdentifier, CyberCore::ResourceResponse&&, CyberCore::ResourceRequest&&);
    void urlSchemeTaskDidReceiveResponse(WebURLSchemeHandlerIdentifier, CyberCore::ResourceLoaderIdentifier taskIdentifier, const CyberCore::ResourceResponse&);
    void urlSchemeTaskDidReceiveData(WebURLSchemeHandlerIdentifier, CyberCore::ResourceLoaderIdentifier taskIdentifier, Ref<CyberCore::SharedBuffer>&&);
    void urlSchemeTaskDidComplete(WebURLSchemeHandlerIdentifier, CyberCore::ResourceLoaderIdentifier taskIdentifier, const CyberCore::ResourceError&);

    void setIsTakingSnapshotsForApplicationSuspension(bool);
    void setNeedsDOMWindowResizeEvent();

    void setIsSuspended(bool);

    RefPtr<WebImage> snapshotAtSize(const CyberCore::IntRect&, const CyberCore::IntSize& bitmapSize, SnapshotOptions, CyberCore::LocalFrame&, CyberCore::LocalFrameView&);
    RefPtr<WebImage> snapshotNode(CyberCore::Node&, SnapshotOptions, unsigned maximumPixelCount = std::numeric_limits<unsigned>::max());
#if PLATFORM(COCOA)
    RetainPtr<CFDataRef> pdfSnapshotAtSize(CyberCore::IntRect, CyberCore::IntSize bitmapSize, SnapshotOptions);
#endif

#if ENABLE(ATTACHMENT_ELEMENT)
    RefPtr<CyberCore::HTMLAttachmentElement> attachmentElementWithIdentifier(const String& identifier) const;
#endif

    bool canShowMIMEType(const String&, const Function<bool(const String&, CyberCore::PluginData::AllowedPluginTypes)>& supportsPlugin) const;

    void cancelCurrentInteractionInformationRequest();

    bool shouldDispatchUpdateAfterFocusingElement(const CyberCore::Element&) const;

    void updateMockAccessibilityElementAfterCommittingLoad();

    void paintSnapshotAtSize(const CyberCore::IntRect&, const CyberCore::IntSize&, SnapshotOptions, CyberCore::LocalFrame&, CyberCore::LocalFrameView&, CyberCore::GraphicsContext&);

#if PLATFORM(GTK) || PLATFORM(WPE)
    void sendMessageToWebProcessExtension(UserMessage&&);
    void sendMessageToWebProcessExtensionWithReply(UserMessage&&, CompletionHandler<void(UserMessage&&)>&&);
#endif

    void platformDidScalePage();

    Vector<Ref<SandboxExtension>> consumeSandboxExtensions(Vector<SandboxExtension::Handle>&&);
    void revokeSandboxExtensions(Vector<Ref<SandboxExtension>>& sandboxExtensions);

    void setSelectionRange(const CyberCore::IntPoint&, CyberCore::TextGranularity, bool);
    
#if !ENABLE(CONTENT_FILTERING_IN_NETWORKING_PROCESS)
    void consumeNetworkExtensionSandboxExtensions(const Vector<SandboxExtension::Handle>&);
#endif

    bool hasPendingEditorStateUpdate() const;
    bool shouldAvoidComputingPostLayoutDataForEditorState() const;

    CyberCore::PageIdentifier m_identifier;

    std::unique_ptr<CyberCore::Page> m_page;
    Ref<WebFrame> m_mainFrame;

    RefPtr<WebPageGroupProxy> m_pageGroup;

    String m_userAgent;

    CyberCore::IntSize m_viewSize;
    std::unique_ptr<DrawingArea> m_drawingArea;
    DrawingAreaType m_drawingAreaType;

#if ENABLE(PDFKIT_PLUGIN)
    HashSet<PluginView*> m_pluginViews;
#endif

    HashMap<TextCheckerRequestID, RefPtr<CyberCore::TextCheckingRequest>> m_pendingTextCheckingRequestMap;

    bool m_useFixedLayout { false };

    CyberCore::FloatSize m_defaultUnobscuredSize;
    CyberCore::FloatSize m_minimumUnobscuredSize;
    CyberCore::FloatSize m_maximumUnobscuredSize;

    CyberCore::Color m_underlayColor;

#if ENABLE(PDFKIT_PLUGIN)
    HashMap<PDFPluginIdentifier, WeakPtr<PDFPlugin>> m_pdfPlugInsWithHUD;
#endif

    WTF::Function<void()> m_selectionChangedHandler;
    bool m_isInRedo { false };
    bool m_isClosed { false };
    bool m_tabToLinks { false };
    
    bool m_mainFrameIsScrollable { true };

    bool m_alwaysShowsHorizontalScroller { false };
    bool m_alwaysShowsVerticalScroller { false };

    bool m_shouldRenderCanvasInGPUProcess { false };
    bool m_shouldRenderDOMInGPUProcess { false };
    bool m_shouldPlayMediaInGPUProcess { false };
#if ENABLE(WEBGL)
    bool m_shouldRenderWebGLInGPUProcess { false };
#endif
#if ENABLE(APP_BOUND_DOMAINS)
    bool m_needsInAppBrowserPrivacyQuirks { false };
#endif

    // The layer hosting mode.
    LayerHostingMode m_layerHostingMode;

#if ENABLE(APP_HIGHLIGHTS)
    CyberCore::CreateNewGroupForHighlight m_highlightIsNewGroup { CyberCore::CreateNewGroupForHighlight::No };
    CyberCore::HighlightRequestOriginatedInApp m_highlightRequestOriginatedInApp { CyberCore::HighlightRequestOriginatedInApp::No };
#endif

#if PLATFORM(COCOA)
    bool m_pdfPluginEnabled { false };
    bool m_hasCachedWindowFrame { false };
    bool m_selectionFlippingEnabled { false };

    // The frame of the containing window in screen coordinates.
    CyberCore::FloatRect m_windowFrameInScreenCoordinates;

    // The frame of the containing window in unflipped screen coordinates.
    CyberCore::FloatRect m_windowFrameInUnflippedScreenCoordinates;

    // The frame of the view in window coordinates.
    CyberCore::FloatRect m_viewFrameInWindowCoordinates;

    // The accessibility position of the view.
    CyberCore::FloatPoint m_accessibilityPosition;
    
    RetainPtr<WKAccessibilityWebPageObject> m_mockAccessibilityElement;
#endif

#if ENABLE(PLATFORM_DRIVEN_TEXT_CHECKING)
    UniqueRef<TextCheckingControllerProxy> m_textCheckingControllerProxy;
#endif

#if PLATFORM(COCOA) || PLATFORM(GTK)
    std::unique_ptr<ViewGestureGeometryCollector> m_viewGestureGeometryCollector;
#endif

#if PLATFORM(COCOA)
    std::optional<double> m_dataDetectionReferenceDate;
#endif

#if USE(ATSPI)
    RefPtr<CyberCore::AccessibilityRootAtspi> m_accessibilityRootObject;
#endif

#if USE(GRAPHICS_LAYER_TEXTURE_MAPPER) || USE(GRAPHICS_LAYER_WC)
    uint64_t m_nativeWindowHandle { 0 };
#endif

#if !PLATFORM(IOS_FAMILY)
    RefPtr<PageBanner> m_headerBanner;
    RefPtr<PageBanner> m_footerBanner;
#endif

    RunLoop::Timer m_setCanStartMediaTimer;
    bool m_mayStartMediaWhenInWindow { false };

    HashMap<WebUndoStepID, RefPtr<WebUndoStep>> m_undoStepMap;

#if ENABLE(CONTEXT_MENUS)
    std::unique_ptr<API::InjectedBundle::PageContextMenuClient> m_contextMenuClient;
#endif
    std::unique_ptr<API::InjectedBundle::EditorClient> m_editorClient;
    std::unique_ptr<API::InjectedBundle::FormClient> m_formClient;
    std::unique_ptr<API::InjectedBundle::PageLoaderClient> m_loaderClient;
    std::unique_ptr<API::InjectedBundle::ResourceLoadClient> m_resourceLoadClient;
    std::unique_ptr<API::InjectedBundle::PageUIClient> m_uiClient;
#if ENABLE(FULLSCREEN_API)
    InjectedBundlePageFullScreenClient m_fullScreenClient;
#endif

    UniqueRef<FindController> m_findController;

    UniqueRef<WebFoundTextRangeController> m_foundTextRangeController;

    RefPtr<WebInspector> m_inspector;
    RefPtr<WebInspectorUI> m_inspectorUI;
    RefPtr<RemoteWebInspectorUI> m_remoteInspectorUI;
    std::unique_ptr<WebPageInspectorTargetController> m_inspectorTargetController;

#if ENABLE(VIDEO_PRESENTATION_MODE)
    RefPtr<PlaybackSessionManager> m_playbackSessionManager;
    RefPtr<VideoFullscreenManager> m_videoFullscreenManager;
#endif

#if PLATFORM(IOS_FAMILY)
    bool m_allowsMediaDocumentInlinePlayback { false };
    std::optional<CyberCore::SimpleRange> m_startingGestureRange;
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

#if ENABLE(DATE_AND_TIME_INPUT_TYPES)
    WeakPtr<WebDateTimeChooser> m_activeDateTimeChooser;
#endif

    RefPtr<WebOpenPanelResultListener> m_activeOpenPanelResultListener;
    RefPtr<NotificationPermissionRequestManager> m_notificationPermissionRequestManager;

    Ref<WebUserContentController> m_userContentController;

#if ENABLE(WK_WEB_EXTENSIONS)
    RefPtr<WebExtensionControllerProxy> m_webExtensionController;
#endif

    UniqueRef<WebScreenOrientationManager> m_screenOrientationManager;

#if ENABLE(GEOLOCATION)
    UniqueRef<GeolocationPermissionRequestManager> m_geolocationPermissionRequestManager;
#endif

#if ENABLE(MEDIA_STREAM)
    UniqueRef<UserMediaPermissionRequestManager> m_userMediaPermissionRequestManager;
#endif

#if ENABLE(ENCRYPTED_MEDIA)
    UniqueRef<MediaKeySystemPermissionRequestManager> m_mediaKeySystemPermissionRequestManager;
#endif

    std::unique_ptr<CyberCore::PrintContext> m_printContext;
    bool m_inActivePrintContextAccessScope { false };
    bool m_shouldEndPrintingImmediately { false };

    class PrintContextAccessScope {
    public:
        PrintContextAccessScope(WebPage& webPage)
            : m_webPage { webPage }
            , m_wasInActivePrintContextAccessScope { webPage.m_inActivePrintContextAccessScope }
        {
            m_webPage->m_inActivePrintContextAccessScope = true;
        }

        ~PrintContextAccessScope()
        {
            m_webPage->m_inActivePrintContextAccessScope = m_wasInActivePrintContextAccessScope;
            if (!m_wasInActivePrintContextAccessScope && m_webPage->m_shouldEndPrintingImmediately)
                m_webPage->endPrintingImmediately();
        }
    private:
        Ref<WebPage> m_webPage;
        const bool m_wasInActivePrintContextAccessScope;
    };

    friend class PrintContextAccessScope;

#if PLATFORM(GTK)
    std::unique_ptr<WebPrintOperationGtk> m_printOperation;
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
    OptionSet<CyberCore::DragSourceAction> m_allowedDragSourceActions { CyberCore::anyDragSourceAction() };
#endif

#if PLATFORM(IOS_FAMILY) && ENABLE(DRAG_SUPPORT)
    HashSet<RefPtr<CyberCore::HTMLImageElement>> m_pendingImageElementsForDropSnapshot;
    std::optional<CyberCore::SimpleRange> m_rangeForDropSnapshot;
#endif

    CyberCore::RectEdges<bool> m_cachedMainFramePinnedState { true, true, true, true };
    bool m_canShortCircuitHorizontalWheelEvents { false };
    bool m_hasWheelEventHandlers { false };

    unsigned m_cachedPageCount { 0 };

    HashSet<CyberCore::ResourceLoaderIdentifier> m_trackedNetworkResourceRequestIdentifiers;

    CyberCore::IntSize m_minimumSizeForAutoLayout;
    CyberCore::IntSize m_sizeToContentAutoSizeMaximumSize;
    bool m_autoSizingShouldExpandToViewHeight { false };
    std::optional<CyberCore::FloatSize> m_viewportSizeForCSSViewportUnits;

    bool m_userIsInteracting { false };

#if HAVE(TOUCH_BAR)
    bool m_hasEverFocusedElementDueToUserInteractionSincePageTransition { false };
    bool m_requiresUserActionForEditingControlsManager { false };
    bool m_isTouchBarUpdateSupressedForHiddenContentEditable { false };
    bool m_isNeverRichlyEditableForTouchBar { false };
#endif
    OptionSet<CyberCore::ActivityState> m_lastActivityStateChanges;

#if HAVE(UIKIT_RESIZABLE_WINDOWS)
    bool m_isWindowResizingEnabled { false };
#endif

#if ENABLE(CONTEXT_MENUS)
    bool m_waitingForContextMenuToShow { false };
#endif

    RefPtr<CyberCore::Element> m_focusedElement;
    RefPtr<CyberCore::Element> m_recentlyBlurredElement;
    bool m_hasPendingInputContextUpdateAfterBlurringAndRefocusingElement { false };
    bool m_pendingThemeColorChange { false };
    bool m_pendingPageExtendedBackgroundColorChange { false };
    bool m_pendingSampledPageTopColorChange { false };

    enum class PendingEditorStateUpdateStatus : uint8_t {
        NotScheduled,
        Scheduled,
        ScheduledDuringAccessibilitySelectionChange,
    };
    PendingEditorStateUpdateStatus m_pendingEditorStateUpdateStatus { PendingEditorStateUpdateStatus::NotScheduled };
    bool m_needsEditorStateVisualDataUpdate { false };

#if ENABLE(META_VIEWPORT)
    CyberCore::ViewportConfiguration m_viewportConfiguration;
    bool m_useTestingViewportConfiguration { false };
    bool m_forceAlwaysUserScalable { false };
#endif

#if PLATFORM(IOS_FAMILY)
    std::optional<CyberCore::SimpleRange> m_currentWordRange;
    RefPtr<CyberCore::Node> m_interactionNode;
    CyberCore::IntPoint m_lastInteractionLocation;

    bool m_isShowingInputViewForFocusedElement { false };
    bool m_wasShowingInputViewForFocusedElementDuringLastPotentialTap { false };
    bool m_completingSyntheticClick { false };
    bool m_hasHandledSyntheticClick { false };
    
    enum SelectionAnchor { Start, End };
    SelectionAnchor m_selectionAnchor { Start };

    RefPtr<CyberCore::Node> m_potentialTapNode;
    CyberCore::FloatPoint m_potentialTapLocation;
    RefPtr<CyberCore::SecurityOrigin> m_potentialTapSecurityOrigin;

    bool m_hasReceivedVisibleContentRectsAfterDidCommitLoad { false };
    bool m_hasRestoredExposedContentRectAfterDidCommitLoad { false };
    bool m_scaleWasSetByUIProcess { false };
    bool m_userHasChangedPageScaleFactor { false };
    bool m_hasStablePageScaleFactor { true };
    bool m_isInStableState { true };
    bool m_shouldRevealCurrentSelectionAfterInsertion { true };
    bool m_screenIsBeingCaptured { false };
    MonotonicTime m_oldestNonStableUpdateVisibleContentRectsTimestamp;
    Seconds m_estimatedLatency { 0 };
    CyberCore::FloatSize m_screenSize;
    CyberCore::FloatSize m_availableScreenSize;
    CyberCore::FloatSize m_overrideScreenSize;

    std::optional<CyberCore::SimpleRange> m_initialSelection;
    CyberCore::VisibleSelection m_storedSelectionForAccessibility { CyberCore::VisibleSelection() };
    CyberCore::IntDegrees m_deviceOrientation { 0 };
    bool m_keyboardIsAttached { false };
    bool m_canShowWhileLocked { false };
    bool m_inDynamicSizeUpdate { false };
    HashMap<std::pair<CyberCore::IntSize, double>, CyberCore::IntPoint> m_dynamicSizeUpdateHistory;
    RefPtr<CyberCore::Node> m_pendingSyntheticClickNode;
    CyberCore::FloatPoint m_pendingSyntheticClickLocation;
    CyberCore::FloatRect m_previousExposedContentRect;
    OptionSet<CyberKit::WebEventModifier> m_pendingSyntheticClickModifiers;
    CyberCore::PointerID m_pendingSyntheticClickPointerId { 0 };
    FocusedElementInformationIdentifier m_lastFocusedElementInformationIdentifier;
    std::optional<DynamicViewportSizeUpdateID> m_pendingDynamicViewportSizeUpdateID;
    double m_lastTransactionPageScaleFactor { 0 };
    TransactionID m_lastTransactionIDWithScaleChange;

    CompletionHandler<void(InteractionInformationAtPosition&&)> m_pendingSynchronousPositionInformationReply;
    std::optional<std::pair<TransactionID, double>> m_lastLayerTreeTransactionIdAndPageScaleBeforeScalingPage;
    bool m_sendAutocorrectionContextAfterFocusingElement { false };
    std::unique_ptr<CyberCore::IgnoreSelectionChangeForScope> m_ignoreSelectionChangeScopeForDictation;
#endif // PLATFORM(IOS_FAMILY)

    CyberCore::Timer m_layerVolatilityTimer;
    Seconds m_layerVolatilityTimerInterval;
    Vector<CompletionHandler<void(bool)>> m_markLayersAsVolatileCompletionHandlers;
    bool m_isSuspendedUnderLock { false };

    HashSet<String, ASCIICaseInsensitiveHash> m_mimeTypesWithCustomContentProviders;
    std::optional<CyberCore::Color> m_backgroundColor { CyberCore::Color::white };

    HashSet<unsigned> m_activeRenderingSuppressionTokens;
    unsigned m_maximumRenderingSuppressionToken { 0 };
    
    CyberCore::ScrollPinningBehavior m_scrollPinningBehavior { CyberCore::ScrollPinningBehavior::DoNotPin };
    std::optional<CyberCore::ScrollbarOverlayStyle> m_scrollbarOverlayStyle;

    bool m_useAsyncScrolling { false };

    OptionSet<CyberCore::ActivityState> m_activityState;

    bool m_isAppNapEnabled { true };
    UserActivity m_userActivity;

    uint64_t m_pendingNavigationID { 0 };
    std::optional<WebsitePoliciesData> m_pendingWebsitePolicies;

    bool m_mainFrameProgressCompleted { false };
    bool m_shouldDispatchFakeMouseMoveEvents { true };
    bool m_isSelectingTextWhileInsertingAsynchronously { false };
    bool m_isChangingSelectionForAccessibility { false };

    enum class EditorStateIsContentEditable { No, Yes, Unset };
    mutable EditorStateIsContentEditable m_lastEditorStateWasContentEditable { EditorStateIsContentEditable::Unset };
    mutable EditorStateIdentifier m_lastEditorStateIdentifier;

#if PLATFORM(GTK) || PLATFORM(WPE)
    std::optional<InputMethodState> m_inputMethodState;
#endif

#if USE(OS_STATE)
    WallTime m_loadCommitTime;
#endif

    CyberCore::UserInterfaceLayoutDirection m_userInterfaceLayoutDirection { CyberCore::UserInterfaceLayoutDirection::LTR };

    const String m_overrideContentSecurityPolicy;
    const std::optional<double> m_cpuLimit;

#if USE(WPE_RENDERER)
    UnixFileDescriptor m_hostFileDescriptor;
#endif

    HashMap<String, RefPtr<WebURLSchemeHandlerProxy>> m_schemeToURLSchemeHandlerProxyMap;
    HashMap<WebURLSchemeHandlerIdentifier, WebURLSchemeHandlerProxy*> m_identifierToURLSchemeHandlerProxyMap;

    HashMap<uint64_t, Function<void(bool granted)>> m_storageAccessResponseCallbackMap;

    OptionSet<LayerTreeFreezeReason> m_layerTreeFreezeReasons;
    bool m_isSuspended { false };
    bool m_needsFontAttributes { false };
    bool m_firstFlushAfterCommit { false };
#if PLATFORM(COCOA)
    WeakPtr<WebRemoteObjectRegistry> m_remoteObjectRegistry;
#endif
    WebPageProxyIdentifier m_webPageProxyIdentifier;
    std::optional<CyberCore::IntSize> m_pendingIntrinsicContentSize;
    CyberCore::IntSize m_lastSentIntrinsicContentSize;
#if HAVE(VISIBILITY_PROPAGATION_VIEW)
    std::unique_ptr<LayerHostingContext> m_contextForVisibilityPropagation;
#endif
#if ENABLE(TEXT_AUTOSIZING)
    CyberCore::Timer m_textAutoSizingAdjustmentTimer;
#endif

#if ENABLE(TRACKING_PREVENTION)
    HashMap<CyberCore::RegistrableDomain, CyberCore::RegistrableDomain> m_domainsWithPageLevelStorageAccess;
    HashSet<CyberCore::RegistrableDomain> m_loadedSubresourceDomains;
#endif

    AtomString m_overriddenMediaType;
    String m_processDisplayName;
    CyberCore::AllowsContentJavaScript m_allowsContentJavaScriptFromMostRecentNavigation { CyberCore::AllowsContentJavaScript::Yes };

#if PLATFORM(GTK)
    CyberCore::Color m_accentColor;
#endif

#if ENABLE(APP_BOUND_DOMAINS)
    bool m_limitsNavigationsToAppBoundDomains { false };
    bool m_navigationHasOccured { false };
#endif

    bool m_lastNavigationWasAppInitiated { true };

    bool m_canUseCredentialStorage { true };

    bool m_didUpdateRenderingAfterCommittingLoad { false };
    bool m_isStoppingLoadingDueToProcessSwap { false };
    bool m_skipDecidePolicyForResponseIfPossible { false };

#if ENABLE(ARKIT_INLINE_PREVIEW)
    bool m_useARKitForModel { false };
#endif
#if HAVE(SCENEKIT)
    bool m_useSceneKitForModel { false };
#endif

    bool m_textManipulationIncludesSubframes { false };
    std::optional<Vector<CyberCore::TextManipulationController::ExclusionRule>> m_textManipulationExclusionRules;

    Vector<String> m_corsDisablingPatterns;

    std::unique_ptr<CyberCore::CachedPage> m_cachedPage;

#if ENABLE(IPC_TESTING_API)
    bool m_ipcTestingAPIEnabled { false };
    uint64_t m_visitedLinkTableID;
#endif

#if ENABLE(MEDIA_SESSION_COORDINATOR)
    RefPtr<CyberCore::MediaSessionCoordinator> m_mediaSessionCoordinator;
    RefPtr<RemoteMediaSessionCoordinator> m_remoteMediaSessionCoordinator;
#endif

#if ENABLE(GPU_PROCESS)
    std::unique_ptr<RemoteRenderingBackendProxy> m_remoteRenderingBackendProxy;
#endif

#if ENABLE(IMAGE_ANALYSIS)
    Vector<std::pair<WeakPtr<CyberCore::HTMLElement, CyberCore::WeakPtrImplWithEventTargetData>, Vector<CompletionHandler<void(RefPtr<CyberCore::Element>&&)>>>> m_elementsPendingTextRecognition;
#endif

#if ENABLE(WEBXR) && !USE(OPENXR)
    std::unique_ptr<PlatformXRSystemProxy> m_xrSystemProxy;
#endif
    
#if ENABLE(APP_HIGHLIGHTS)
    CyberCore::HighlightVisibility m_appHighlightsVisible { CyberCore::HighlightVisibility::Hidden };
#endif

#if ENABLE(NETWORK_CONNECTION_INTEGRITY)
    HashSet<String> m_lookalikeCharacterStrings;
    HashMap<CyberCore::RegistrableDomain, HashSet<String>> m_domainScopedLookalikeCharacterStrings;
    HashMap<CyberCore::RegistrableDomain, HashSet<String>> m_allowedLookalikeCharacterStrings;
#endif

#if ENABLE(IMAGE_ANALYSIS_ENHANCEMENTS)
    WeakHashSet<CyberCore::HTMLImageElement, CyberCore::WeakPtrImplWithEventTargetData> m_elementsToExcludeFromRemoveBackground;
#endif
};

#if !PLATFORM(IOS_FAMILY)
inline void WebPage::platformWillPerformEditingCommand() { }
inline bool WebPage::requiresPostLayoutDataForEditorState(const CyberCore::LocalFrame&) const { return false; }
inline void WebPage::prepareToRunModalJavaScriptDialog() { }
#endif

#if !PLATFORM(MAC)
inline bool WebPage::shouldAvoidComputingPostLayoutDataForEditorState() const { return false; }
#endif

#if !PLATFORM(COCOA)
inline URL WebPage::sanitizeLookalikeCharacters(const URL& url, CyberCore::LookalikeCharacterSanitizationTrigger) { return url; }
inline URL WebPage::allowedLookalikeCharacters(const URL& url) { return url; }
#endif

#if PLATFORM(IOS_FAMILY)
bool scalesAreEssentiallyEqual(float, float);
#endif

} // namespace CyberKit
