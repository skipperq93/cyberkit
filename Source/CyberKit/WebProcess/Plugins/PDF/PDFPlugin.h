/*
 * Copyright (C) 2011-2020 Apple Inc. All rights reserved.
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

#if ENABLE(PDFKIT_PLUGIN)

#include "PDFKitImports.h"
#include "Plugin.h"
#include "WebEvent.h"
#include "WebHitTestResultData.h"
#include <CyberCore/AXObjectCache.h>
#include <CyberCore/AffineTransform.h>
#include <CyberCore/FindOptions.h>
#include <CyberCore/NetscapePlugInStreamLoader.h>
#include <CyberCore/ScrollableArea.h>
#include <wtf/HashMap.h>
#include <wtf/Identified.h>
#include <wtf/Range.h>
#include <wtf/RangeSet.h>
#include <wtf/RetainPtr.h>
#include <wtf/Threading.h>

typedef const struct OpaqueJSContext* JSContextRef;
typedef struct OpaqueJSValue* JSObjectRef;
typedef const struct OpaqueJSValue* JSValueRef;

OBJC_CLASS NSArray;
OBJC_CLASS NSAttributedString;
OBJC_CLASS NSData;
OBJC_CLASS NSString;
OBJC_CLASS PDFAnnotation;
OBJC_CLASS PDFLayerController;
OBJC_CLASS PDFSelection;
OBJC_CLASS WKPDFPluginAccessibilityObject;
OBJC_CLASS WKPDFLayerControllerDelegate;

namespace IPC {
class DataReference;
}

namespace CyberCore {
class AXObjectCache;
class Element;
struct PluginInfo;
}

namespace WTF {
class TextStream;
}

namespace CyberKit {

class PDFPluginAnnotation;
class PDFPluginPasswordField;
class PluginView;
class WebFrame;

class PDFPlugin final : public Plugin, public CyberCore::ScrollableArea
{
public:
    static Ref<PDFPlugin> create(WebFrame&);
    ~PDFPlugin();

    static CyberCore::PluginInfo pluginInfo();

    CyberCore::IntSize size() const { return m_size; }

    void didMutatePDFDocument() { m_pdfDocumentWasMutated = true; }

    void paintControlForLayerInContext(CALayer *, CGContextRef);
    void setActiveAnnotation(PDFAnnotation *);

    using ScrollableArea::notifyScrollPositionChanged;
    void notifyContentScaleFactorChanged(CGFloat scaleFactor);
    void notifyDisplayModeChanged(int);

    void notifySelectionChanged(PDFSelection *);
    void notifyCursorChanged(uint64_t /* PDFLayerControllerCursorType */);

    void clickedLink(NSURL *);
    void saveToPDF();
    void openWithNativeApplication();
    void writeItemsToPasteboard(NSString *pasteboardName, NSArray *items, NSArray *types);
    void showDefinitionForAttributedString(NSAttributedString *, CGPoint);
    void performWebSearch(NSString *);
    void performSpotlightSearch(NSString *);

    void focusNextAnnotation();
    void focusPreviousAnnotation();

    void attemptToUnlockPDF(const String& password);

    CyberCore::FloatRect convertFromPDFViewToScreen(const CyberCore::FloatRect&) const;
    CyberCore::IntPoint convertFromRootViewToPDFView(const CyberCore::IntPoint&) const;
    CyberCore::IntRect boundsOnScreen() const;

    bool showContextMenuAtPoint(const CyberCore::IntPoint&);

    std::tuple<String, PDFSelection *, NSDictionary *> lookupTextAtLocation(const CyberCore::FloatPoint&, WebHitTestResultData&) const;
    CyberCore::FloatRect rectForSelectionInRootView(PDFSelection *) const;

    CGFloat scaleFactor() const;

    PDFPluginAnnotation* activeAnnotation() const { return m_activeAnnotation.get(); }
    CyberCore::AXObjectCache* axObjectCache() const;

    void ensureDataBufferLength(uint64_t length);

#if HAVE(INCREMENTAL_PDF_APIS)
    void getResourceBytesAtPosition(size_t count, off_t position, CompletionHandler<void(const uint8_t*, size_t count)>&&);
    size_t getResourceBytesAtPositionMainThread(void* buffer, off_t position, size_t count);
    void receivedNonLinearizedPDFSentinel();
    bool incrementalPDFLoadingEnabled() const { return m_incrementalPDFLoadingEnabled; }
#ifndef NDEBUG
    void pdfLog(const String& event);
    size_t incrementThreadsWaitingOnCallback() { return ++m_threadsWaitingOnCallback; }
    size_t decrementThreadsWaitingOnCallback() { return --m_threadsWaitingOnCallback; }
#endif
#endif

private:
    explicit PDFPlugin(WebFrame&);

    // Plugin functions.
    bool initialize(const Parameters&) final;
    void destroy() final;
    void paint(CyberCore::GraphicsContext&, const CyberCore::IntRect& dirtyRectInWindowCoordinates) final { }
    void updateControlTints(CyberCore::GraphicsContext&) final;
    bool supportsSnapshotting() const final { return true; }
    RefPtr<ShareableBitmap> snapshot() final;
    PlatformLayer* pluginLayer() final;
    bool isTransparent() final { return false; }
    bool wantsWheelEvents() final { return true; }
    void geometryDidChange(const CyberCore::IntSize& pluginSize, const CyberCore::IntRect& clipRect, const CyberCore::AffineTransform& pluginToRootViewTransform) final;
    void contentsScaleFactorChanged(float) final;
    void visibilityDidChange(bool) final { }
    void frameDidFinishLoading(uint64_t requestID) final;
    void frameDidFail(uint64_t requestID, bool wasCancelled) final;
    void didEvaluateJavaScript(uint64_t requestID, const String& result) final;
    void streamWillSendRequest(uint64_t streamID, const URL& requestURL, const URL& responseURL, int responseStatus) final { }
    void streamDidReceiveResponse(uint64_t streamID, const URL& responseURL, uint32_t streamLength, uint32_t lastModifiedTime, const String& mimeType, const String& headers, const String& suggestedFileName) final;
    void streamDidReceiveData(uint64_t streamID, const char* bytes, int length) final;
    void streamDidFinishLoading(uint64_t streamID) final;
    void streamDidFail(uint64_t streamID, bool wasCancelled) final;
    void manualStreamDidReceiveResponse(const URL& responseURL, uint32_t streamLength, uint32_t lastModifiedTime, const WTF::String& mimeType, const WTF::String& headers, const String& suggestedFileName) final;
    void manualStreamDidReceiveData(const char* bytes, int length) final;
    void manualStreamDidFinishLoading() final;
    void manualStreamDidFail(bool wasCancelled) final;
    bool handleMouseEvent(const WebMouseEvent&) final;
    bool handleWheelEvent(const WebWheelEvent&) final;
    bool handleMouseEnterEvent(const WebMouseEvent&) final;
    bool handleMouseLeaveEvent(const WebMouseEvent&) final;
    bool handleContextMenuEvent(const WebMouseEvent&) final;
    bool handleKeyboardEvent(const WebKeyboardEvent&) final;
    bool handleEditingCommand(const String& commandName, const String& argument) final;
    bool isEditingCommandEnabled(const String&) final;
    bool handlesPageScaleFactor() const final;
    bool requiresUnifiedScaleFactor() const final { return true; }
    void setFocus(bool) final { }
    NPObject* pluginScriptableNPObject() final { return nullptr; }
    void windowFocusChanged(bool) final { }
    void windowAndViewFramesChanged(const CyberCore::IntRect& windowFrameInScreenCoordinates, const CyberCore::IntRect& viewFrameInWindowCoordinates) final { }
    void windowVisibilityChanged(bool) final { }
    uint64_t pluginComplexTextInputIdentifier() const final { return 0; }
    void sendComplexTextInput(const String& textInput) final { }
    void setLayerHostingMode(LayerHostingMode) final { }
    CyberCore::Scrollbar* horizontalScrollbar() final { return m_horizontalScrollbar.get(); }
    CyberCore::Scrollbar* verticalScrollbar() final { return m_verticalScrollbar.get(); }
    void storageBlockingStateChanged(bool) final { }
    void privateBrowsingStateChanged(bool) final { }
    bool getFormValue(String& formValue) final { return false; }
    bool handleScroll(CyberCore::ScrollDirection, CyberCore::ScrollGranularity) final;
    RefPtr<CyberCore::SharedBuffer> liveResourceData() const final;
    void willDetachRenderer() final;
    bool pluginHandlesContentOffsetForAccessibilityHitTest() const final;
    
    bool isBeingAsynchronouslyInitialized() const final { return false; }

    RetainPtr<PDFDocument> pdfDocumentForPrinting() const final { return m_pdfDocument; }
    NSObject *accessibilityObject() const final;
    id accessibilityAssociatedPluginParentForElement(CyberCore::Element*) const final;

    unsigned countFindMatches(const String& target, CyberCore::FindOptions, unsigned maxMatchCount) final;
    bool findString(const String& target, CyberCore::FindOptions, unsigned maxMatchCount) final;

    PDFSelection *nextMatchForString(const String& target, BOOL searchForward, BOOL caseSensitive, BOOL wrapSearch, PDFSelection *initialSelection, BOOL startInSelection);

    bool performDictionaryLookupAtLocation(const CyberCore::FloatPoint&) final;
    String getSelectionString() const final;
    String getSelectionForWordAtPoint(const CyberCore::FloatPoint&) const final;
    bool existingSelectionContainsPoint(const CyberCore::FloatPoint&) const final;

    bool shouldAllowScripting() final { return false; }
    bool shouldAllowNavigationFromDrags() final { return true; }
    bool shouldAlwaysAutoStart() const final { return true; }

    // ScrollableArea functions.
    bool isPDFPlugin() const final { return true; }
    CyberCore::IntRect scrollCornerRect() const final;
    CyberCore::ScrollableArea* enclosingScrollableArea() const final;
    bool isScrollableOrRubberbandable() final { return true; }
    bool hasScrollableOrRubberbandableAncestor() final { return true; }
    CyberCore::IntRect scrollableAreaBoundingBox(bool* = nullptr) const final;
    void setScrollOffset(const CyberCore::ScrollOffset&) final;
    void invalidateScrollbarRect(CyberCore::Scrollbar&, const CyberCore::IntRect&) final;
    void invalidateScrollCornerRect(const CyberCore::IntRect&) final;
    CyberCore::IntPoint lastKnownMousePositionInView() const final { return m_lastMousePositionInPluginCoordinates; }
    bool isActive() const final;
    bool isScrollCornerVisible() const final { return false; }
    CyberCore::ScrollPosition scrollPosition() const final;
    CyberCore::ScrollPosition minimumScrollPosition() const final;
    CyberCore::ScrollPosition maximumScrollPosition() const final;
    CyberCore::IntSize visibleSize() const final { return m_size; }
    CyberCore::IntSize contentsSize() const final { return m_pdfDocumentSize; }
    CyberCore::Scrollbar* horizontalScrollbar() const final { return m_horizontalScrollbar.get(); }
    CyberCore::Scrollbar* verticalScrollbar() const final { return m_verticalScrollbar.get(); }
    bool shouldSuspendScrollAnimations() const final { return false; } // If we return true, ScrollAnimatorMac will keep cycling a timer forever, waiting for a good time to animate.
    void scrollbarStyleChanged(CyberCore::ScrollbarStyle, bool forceUpdate) final;
    CyberCore::IntRect convertFromScrollbarToContainingView(const CyberCore::Scrollbar&, const CyberCore::IntRect& scrollbarRect) const final;
    CyberCore::IntRect convertFromContainingViewToScrollbar(const CyberCore::Scrollbar&, const CyberCore::IntRect& parentRect) const final;
    CyberCore::IntPoint convertFromScrollbarToContainingView(const CyberCore::Scrollbar&, const CyberCore::IntPoint& scrollbarPoint) const final;
    CyberCore::IntPoint convertFromContainingViewToScrollbar(const CyberCore::Scrollbar&, const CyberCore::IntPoint& parentPoint) const final;
    bool forceUpdateScrollbarsOnMainThreadForPerformanceTesting() const final;
    bool shouldPlaceBlockDirectionScrollbarOnLeft() const final { return false; }
    String debugDescription() const final;

    // PDFPlugin functions.
    void updateScrollbars();
    Ref<CyberCore::Scrollbar> createScrollbar(CyberCore::ScrollbarOrientation);
    void destroyScrollbar(CyberCore::ScrollbarOrientation);
    void documentDataDidFinishLoading();
    void installPDFDocument();
    void addArchiveResource();
    void calculateSizes();
    void tryRunScriptsInPDFDocument();

    NSEvent *nsEventForWebMouseEvent(const WebMouseEvent&);
    CyberCore::IntPoint convertFromPluginToPDFView(const CyberCore::IntPoint&) const;
    CyberCore::IntPoint convertFromRootViewToPlugin(const CyberCore::IntPoint&) const;
    CyberCore::IntPoint convertFromPDFViewToRootView(const CyberCore::IntPoint&) const;
    
    bool supportsForms();
    bool isFullFramePlugin() const;

    void updatePageAndDeviceScaleFactors();

    void createPasswordEntryForm();

    CyberCore::IntSize pdfDocumentSize() const { return m_pdfDocumentSize; }
    void setPDFDocumentSize(CyberCore::IntSize size) { m_pdfDocumentSize = size; }

#ifdef __OBJC__
    NSData *liveData() const;
    NSData *rawData() const { return (__bridge NSData *)m_data.get(); }
#endif

    JSObjectRef makeJSPDFDoc(JSContextRef);
    static JSValueRef jsPDFDocPrint(JSContextRef, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

    void convertPostScriptDataIfNeeded();

    void setSuggestedFilename(const String&);

    // Regular plug-ins don't need access to view, but we add scrollbars to embedding FrameView for proper event handling.
    PluginView* pluginView();
    const PluginView* pluginView() const;

    WebFrame& m_frame;

    bool m_isPostScript { false };
    bool m_pdfDocumentWasMutated { false };

    CyberCore::IntSize m_scrollOffset;

    RetainPtr<CALayer> m_containerLayer;
    RetainPtr<CALayer> m_contentLayer;
    RetainPtr<CALayer> m_horizontalScrollbarLayer;
    RetainPtr<CALayer> m_verticalScrollbarLayer;
    RetainPtr<CALayer> m_scrollCornerLayer;
    RetainPtr<PDFLayerController> m_pdfLayerController;
    RetainPtr<WKPDFPluginAccessibilityObject> m_accessibilityObject;
    
    RefPtr<PDFPluginAnnotation> m_activeAnnotation;
    RefPtr<PDFPluginPasswordField> m_passwordField;
    RefPtr<CyberCore::Element> m_annotationContainer;

    CyberCore::AffineTransform m_rootViewToPluginTransform;
    WebMouseEvent m_lastMouseEvent;
    CyberCore::IntPoint m_lastMousePositionInPluginCoordinates;

    String m_temporaryPDFUUID;

    String m_lastFoundString;

    RetainPtr<WKPDFLayerControllerDelegate> m_pdfLayerControllerDelegate;

    CyberCore::IntSize m_size;

    URL m_sourceURL;

    String m_suggestedFilename;
    RetainPtr<CFMutableDataRef> m_data;
    uint64_t m_streamedBytes { 0 };

    RetainPtr<PDFDocument> m_pdfDocument;

    bool m_documentFinishedLoading { false };
    bool m_hasBeenDestroyed { false };
    unsigned m_firstPageHeight { 0 };
    CyberCore::IntSize m_pdfDocumentSize; // All pages, including gaps.

    RefPtr<CyberCore::Scrollbar> m_horizontalScrollbar;
    RefPtr<CyberCore::Scrollbar> m_verticalScrollbar;

#if HAVE(INCREMENTAL_PDF_APIS)
    void threadEntry(Ref<PDFPlugin>&&);
    void adoptBackgroundThreadDocument();

    bool documentFinishedLoading() { return m_documentFinishedLoading; }
    uint64_t identifierForLoader(CyberCore::NetscapePlugInStreamLoader* loader) { return m_streamLoaderMap.get(loader); }
    void removeOutstandingByteRangeRequest(uint64_t identifier) { m_outstandingByteRangeRequests.remove(identifier); }

    class PDFPluginStreamLoaderClient : public RefCounted<PDFPluginStreamLoaderClient>,
                                        public CyberCore::NetscapePlugInStreamLoaderClient {
    public:
        PDFPluginStreamLoaderClient(PDFPlugin& pdfPlugin)
            : m_pdfPlugin(makeWeakPtr(pdfPlugin))
        {
        }

        ~PDFPluginStreamLoaderClient() = default;

        void willSendRequest(CyberCore::NetscapePlugInStreamLoader*, CyberCore::ResourceRequest&&, const CyberCore::ResourceResponse& redirectResponse, CompletionHandler<void(CyberCore::ResourceRequest&&)>&&) final;
        void didReceiveResponse(CyberCore::NetscapePlugInStreamLoader*, const CyberCore::ResourceResponse&) final;
        void didReceiveData(CyberCore::NetscapePlugInStreamLoader*, const char*, int) final;
        void didFail(CyberCore::NetscapePlugInStreamLoader*, const CyberCore::ResourceError&) final;
        void didFinishLoading(CyberCore::NetscapePlugInStreamLoader*) final;

    private:
        WeakPtr<PDFPlugin> m_pdfPlugin;
    };

    class ByteRangeRequest : public Identified<ByteRangeRequest> {
    public:
        ByteRangeRequest() = default;
        ByteRangeRequest(uint64_t position, size_t count, CompletionHandler<void(const uint8_t*, size_t count)>&& completionHandler)
            : m_position(position)
            , m_count(count)
            , m_completionHandler(WTFMove(completionHandler))
        {
        }

        CyberCore::NetscapePlugInStreamLoader* streamLoader() { return m_streamLoader; }
        void setStreamLoader(CyberCore::NetscapePlugInStreamLoader* loader) { m_streamLoader = loader; }
        void clearStreamLoader();
        void addData(const uint8_t* data, size_t count) { m_accumulatedData.append(data, count); }

        void completeWithBytes(const uint8_t*, size_t, PDFPlugin&);
        void completeWithAccumulatedData(PDFPlugin&);

        bool maybeComplete(PDFPlugin&);
        void completeUnconditionally(PDFPlugin&);

        uint64_t position() const { return m_position; }
        size_t count() const { return m_count; }

    private:
        uint64_t m_position { 0 };
        size_t m_count { 0 };
        CompletionHandler<void(const uint8_t*, size_t count)> m_completionHandler;
        Vector<uint8_t> m_accumulatedData;
        CyberCore::NetscapePlugInStreamLoader* m_streamLoader { nullptr };
    };
    void unconditionalCompleteOutstandingRangeRequests();

    ByteRangeRequest* byteRangeRequestForLoader(CyberCore::NetscapePlugInStreamLoader&);
    void forgetLoader(CyberCore::NetscapePlugInStreamLoader&);
    void cancelAndForgetLoader(CyberCore::NetscapePlugInStreamLoader&);
    void maybeClearHighLatencyDataProviderFlag();

    RetainPtr<PDFDocument> m_backgroundThreadDocument;
    RefPtr<Thread> m_pdfThread;
    HashMap<uint64_t, ByteRangeRequest> m_outstandingByteRangeRequests;
    Ref<PDFPluginStreamLoaderClient> m_streamLoaderClient;
    HashMap<RefPtr<CyberCore::NetscapePlugInStreamLoader>, uint64_t> m_streamLoaderMap;
    RangeSet<WTF::Range<uint64_t>> m_completedRanges;
    bool m_incrementalPDFLoadingEnabled;

#if !LOG_DISABLED
    void verboseLog();
    void logStreamLoader(WTF::TextStream&, CyberCore::NetscapePlugInStreamLoader&);
    std::atomic<size_t> m_threadsWaitingOnCallback { 0 };
    std::atomic<size_t> m_completedRangeRequests { 0 };
    std::atomic<size_t> m_completedNetworkRangeRequests { 0 };
#endif

#endif // HAVE(INCREMENTAL_PDF_APIS)
};

} // namespace CyberKit

SPECIALIZE_TYPE_TRAITS_BEGIN(CyberKit::PDFPlugin)
    static bool isType(const CyberKit::Plugin& plugin) { return plugin.isPDFPlugin(); }
    static bool isType(const CyberCore::ScrollableArea& area) { return area.isPDFPlugin(); }
SPECIALIZE_TYPE_TRAITS_END()

#endif // ENABLE(PDFKIT_PLUGIN)
