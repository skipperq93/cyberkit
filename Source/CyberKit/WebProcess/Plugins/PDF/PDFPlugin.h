/*
 * Copyright (C) 2011-2022 Apple Inc. All rights reserved.
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

#include "DataReference.h"
#include "PDFPluginIdentifier.h"
#include "WebMouseEvent.h"
#include <CyberCore/AffineTransform.h>
#include <CyberCore/FindOptions.h>
#include <CyberCore/NetscapePlugInStreamLoader.h>
#include <CyberCore/ScrollableArea.h>
#include <wtf/HashMap.h>
#include <wtf/Identified.h>
#include <wtf/Range.h>
#include <wtf/RangeSet.h>
#include <wtf/RefCounted.h>
#include <wtf/RetainPtr.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/Threading.h>
#include <wtf/URL.h>
#include <wtf/Vector.h>
#include <wtf/WeakPtr.h>

typedef struct objc_object* id;

OBJC_CLASS CALayer;
OBJC_CLASS NSArray;
OBJC_CLASS NSAttributedString;
OBJC_CLASS NSData;
OBJC_CLASS NSDictionary;
OBJC_CLASS NSEvent;
OBJC_CLASS NSString;
OBJC_CLASS PDFAnnotation;
OBJC_CLASS PDFDocument;
OBJC_CLASS PDFLayerController;
OBJC_CLASS PDFSelection;
OBJC_CLASS WKPDFLayerControllerDelegate;
OBJC_CLASS WKPDFPluginAccessibilityObject;

typedef const struct OpaqueJSContext* JSContextRef;
typedef struct OpaqueJSValue* JSObjectRef;
typedef const struct OpaqueJSValue* JSValueRef;

namespace CyberCore {
class AXObjectCache;
class Element;
class FloatPoint;
class FloatSize;
class FragmentedSharedBuffer;
class GraphicsContext;
class HTMLPlugInElement;
class Scrollbar;
struct PluginInfo;
}

namespace CyberKit {

class PDFPluginAnnotation;
class PDFPluginPasswordField;
class PluginView;
class ShareableBitmap;
class WebFrame;
class WebKeyboardEvent;
class WebWheelEvent;

struct FrameInfoData;
struct WebHitTestResultData;

class PDFPlugin final : public ThreadSafeRefCounted<PDFPlugin>, public CyberCore::ScrollableArea {
public:
    static Ref<PDFPlugin> create(CyberCore::HTMLPlugInElement&);
    ~PDFPlugin();

    bool isBeingDestroyed() const { return m_isBeingDestroyed; }

    static CyberCore::PluginInfo pluginInfo();

    CyberCore::IntSize size() const { return m_size; }

    void didMutatePDFDocument() { m_pdfDocumentWasMutated = true; }

    void paintControlForLayerInContext(CALayer *, CGContextRef);
    void setActiveAnnotation(PDFAnnotation *);

    void notifyContentScaleFactorChanged(CGFloat scaleFactor);
    void notifyDisplayModeChanged(int);

    void notifySelectionChanged(PDFSelection *);
    void notifyCursorChanged(uint64_t /* PDFLayerControllerCursorType */);

#if ENABLE(UI_PROCESS_PDF_HUD)
    void zoomIn();
    void zoomOut();
    void save(CompletionHandler<void(const String&, const URL&, const IPC::DataReference&)>&&);
    void openWithPreview(CompletionHandler<void(const String&, FrameInfoData&&, const IPC::DataReference&, const String&)>&&);
    PDFPluginIdentifier identifier() const { return m_identifier; }
#endif

    void clickedLink(NSURL *);
#if !ENABLE(UI_PROCESS_PDF_HUD)
    void saveToPDF();
    void openWithNativeApplication();
#endif
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
    float deviceScaleFactor() const;

    PDFPluginAnnotation* activeAnnotation() const { return m_activeAnnotation.get(); }
    CyberCore::AXObjectCache* axObjectCache() const;

#if HAVE(INCREMENTAL_PDF_APIS)
    void getResourceBytesAtPosition(size_t count, off_t position, CompletionHandler<void(const uint8_t*, size_t count)>&&);
    size_t getResourceBytesAtPositionMainThread(void* buffer, off_t position, size_t count);
    void receivedNonLinearizedPDFSentinel();
    bool incrementalPDFLoadingEnabled() const { return m_incrementalPDFLoadingEnabled; }
#endif

#if HAVE(INCREMENTAL_PDF_APIS) && !LOG_DISABLED
    void pdfLog(const String& event);
    size_t incrementThreadsWaitingOnCallback() { return ++m_threadsWaitingOnCallback; }
    size_t decrementThreadsWaitingOnCallback() { return --m_threadsWaitingOnCallback; }
#endif

    void setView(PluginView&);
    void destroy();
    void updateControlTints(CyberCore::GraphicsContext&);
    RefPtr<ShareableBitmap> snapshot();
    CALayer *pluginLayer();
    void geometryDidChange(const CyberCore::IntSize& pluginSize, const CyberCore::AffineTransform& pluginToRootViewTransform);
    void contentsScaleFactorChanged(float);
    void visibilityDidChange(bool);
    void streamDidReceiveResponse(const CyberCore::ResourceResponse&);
    void streamDidReceiveData(const CyberCore::SharedBuffer&);
    void streamDidFinishLoading();
    void streamDidFail();
    bool handleMouseEvent(const WebMouseEvent&);
    bool handleWheelEvent(const WebWheelEvent&);
    bool handleMouseEnterEvent(const WebMouseEvent&);
    bool handleMouseLeaveEvent(const WebMouseEvent&);
    bool handleContextMenuEvent(const WebMouseEvent&);
    bool handleKeyboardEvent(const WebKeyboardEvent&);
    bool handleEditingCommand(StringView commandName);
    bool isEditingCommandEnabled(StringView commandName);
    CyberCore::Scrollbar* horizontalScrollbar() { return m_horizontalScrollbar.get(); }
    CyberCore::Scrollbar* verticalScrollbar() { return m_verticalScrollbar.get(); }
    RefPtr<CyberCore::FragmentedSharedBuffer> liveResourceData() const;
    void willDetachRenderer();

    RetainPtr<PDFDocument> pdfDocumentForPrinting() const { return m_pdfDocument; }
    CyberCore::FloatSize pdfDocumentSizeForPrinting() const;
    id accessibilityHitTest(const CyberCore::IntPoint&) const;
    id accessibilityObject() const;
    id accessibilityAssociatedPluginParentForElement(CyberCore::Element*) const;

    unsigned countFindMatches(const String& target, CyberCore::FindOptions, unsigned maxMatchCount);
    bool findString(const String& target, CyberCore::FindOptions, unsigned maxMatchCount);

    bool performDictionaryLookupAtLocation(const CyberCore::FloatPoint&);
    String getSelectionString() const;
    bool existingSelectionContainsPoint(const CyberCore::FloatPoint&) const;

    bool isFullFramePlugin() const;

private:
    explicit PDFPlugin(CyberCore::HTMLPlugInElement&);

    PDFSelection *nextMatchForString(const String& target, bool searchForward, bool caseSensitive, bool wrapSearch, PDFSelection *initialSelection, bool startInSelection);

    // ScrollableArea functions.
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
    bool shouldPlaceVerticalScrollbarOnLeft() const final { return false; }
    String debugDescription() const final;

    void updateScrollbars();
    Ref<CyberCore::Scrollbar> createScrollbar(CyberCore::ScrollbarOrientation);
    void destroyScrollbar(CyberCore::ScrollbarOrientation);
    void installPDFDocument();
    void addArchiveResource();
    void calculateSizes();
    void tryRunScriptsInPDFDocument();

    NSEvent *nsEventForWebMouseEvent(const WebMouseEvent&);
    CyberCore::IntPoint convertFromPluginToPDFView(const CyberCore::IntPoint&) const;
    CyberCore::IntPoint convertFromRootViewToPlugin(const CyberCore::IntPoint&) const;
    CyberCore::IntPoint convertFromPDFViewToRootView(const CyberCore::IntPoint&) const;
    CyberCore::IntRect convertFromPDFViewToRootView(const CyberCore::IntRect&) const;
    CyberCore::IntRect frameForHUD() const;
    void ensureDataBufferLength(uint64_t length);

    bool supportsForms();

    bool handlesPageScaleFactor() const;
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

    WeakPtr<PluginView> m_view;
    WeakPtr<WebFrame> m_frame;

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
    bool m_isBeingDestroyed { false };
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
            : m_pdfPlugin(pdfPlugin)
        {
        }

        ~PDFPluginStreamLoaderClient() = default;

        void willSendRequest(CyberCore::NetscapePlugInStreamLoader*, CyberCore::ResourceRequest&&, const CyberCore::ResourceResponse& redirectResponse, CompletionHandler<void(CyberCore::ResourceRequest&&)>&&) final;
        void didReceiveResponse(CyberCore::NetscapePlugInStreamLoader*, const CyberCore::ResourceResponse&) final;
        void didReceiveData(CyberCore::NetscapePlugInStreamLoader*, const CyberCore::SharedBuffer&) final;
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
    void logStreamLoader(TextStream&, CyberCore::NetscapePlugInStreamLoader&);
    std::atomic<size_t> m_threadsWaitingOnCallback { 0 };
    std::atomic<size_t> m_completedRangeRequests { 0 };
    std::atomic<size_t> m_completedNetworkRangeRequests { 0 };
#endif

#endif // HAVE(INCREMENTAL_PDF_APIS)

    PDFPluginIdentifier m_identifier;
};

} // namespace CyberKit

#endif // ENABLE(PDFKIT_PLUGIN)
