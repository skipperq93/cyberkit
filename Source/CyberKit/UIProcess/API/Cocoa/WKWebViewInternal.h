/*
 * Copyright (C) 2014-2016 Apple Inc. All rights reserved.
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

#import "WKWebViewPrivate.h"

#import "SameDocumentNavigationType.h"
#import "WKShareSheet.h"
#import "WKWebViewConfiguration.h"
#import "_WKAttachmentInternal.h"
#import "_WKWebViewPrintFormatterInternal.h"
#import <wtf/CompletionHandler.h>
#import <wtf/RefPtr.h>
#import <wtf/RetainPtr.h>
#import <wtf/Variant.h>
#include <wtf/WeakObjCPtr.h>

#if PLATFORM(IOS_FAMILY)
#import "DynamicViewportSizeUpdate.h"
#import "UIKitSPI.h"
#import "WKContentView.h"
#import "WKContentViewInteraction.h"
#import "WKFullScreenWindowControllerIOS.h"
#import <CyberCore/FloatRect.h>
#import <CyberCore/LengthBox.h>
#endif

#if PLATFORM(IOS_FAMILY)
#define WK_WEB_VIEW_PROTOCOLS <UIScrollViewDelegate>
#endif

#if PLATFORM(MAC)
#define WK_WEB_VIEW_PROTOCOLS <WKShareSheetDelegate>
#endif

#if !defined(WK_WEB_VIEW_PROTOCOLS)
#define WK_WEB_VIEW_PROTOCOLS
#endif

typedef const struct OpaqueWKPage* WKPageRef;

namespace API {
class Attachment;
}

namespace CyberKit {
enum class ContinueUnsafeLoad : bool;
class IconLoadingDelegate;
class NavigationState;
class SafeBrowsingWarning;
class ViewSnapshot;
class WebPageProxy;
class UIDelegate;
struct PrintInfo;
#if PLATFORM(MAC)
class WebViewImpl;
#endif
#if PLATFORM(IOS_FAMILY)
class ViewGestureController;
#endif
}

@class WKWebViewContentProviderRegistry;
@class WKPasswordView;
@class _WKFrameHandle;
@class WKSafeBrowsingWarning;

#if PLATFORM(IOS_FAMILY)
@class WKScrollView;
@class WKFullScreenWindowController;
@protocol WKWebViewContentProvider;
#endif

#if PLATFORM(MAC)
@class WKTextFinderClient;
#endif

@protocol _WKTextManipulationDelegate;
@protocol _WKInputDelegate;

@interface WKWebView () WK_WEB_VIEW_PROTOCOLS {

@package
    RetainPtr<WKWebViewConfiguration> _configuration;
    RefPtr<CyberKit::WebPageProxy> _page;

    std::unique_ptr<CyberKit::NavigationState> _navigationState;
    std::unique_ptr<CyberKit::UIDelegate> _uiDelegate;
    std::unique_ptr<CyberKit::IconLoadingDelegate> _iconLoadingDelegate;

    WeakObjCPtr<id <_WKTextManipulationDelegate>> _textManipulationDelegate;
    WeakObjCPtr<id <_WKInputDelegate>> _inputDelegate;

    RetainPtr<WKSafeBrowsingWarning> _safeBrowsingWarning;

    Optional<BOOL> _resolutionForShareSheetImmediateCompletionForTesting;

    _WKSelectionAttributes _selectionAttributes;
    _WKRenderingProgressEvents _observedRenderingProgressEvents;
    BOOL _usePlatformFindUI;

#if PLATFORM(MAC)
    std::unique_ptr<CyberKit::WebViewImpl> _impl;
    RetainPtr<WKTextFinderClient> _textFinderClient;
#endif

#if PLATFORM(IOS_FAMILY)
    RetainPtr<WKScrollView> _scrollView;
    RetainPtr<WKContentView> _contentView;
    std::unique_ptr<CyberKit::ViewGestureController> _gestureController;
    Vector<BlockPtr<void ()>> _visibleContentRectUpdateCallbacks;

#if ENABLE(FULLSCREEN_API)
    RetainPtr<WKFullScreenWindowController> _fullScreenWindowController;
#endif

    RetainPtr<_WKRemoteObjectRegistry> _remoteObjectRegistry;

    Optional<CGSize> _viewLayoutSizeOverride;
    Optional<CyberCore::FloatSize> _lastSentViewLayoutSize;
    Optional<CGSize> _maximumUnobscuredSizeOverride;
    Optional<CyberCore::FloatSize> _lastSentMaximumUnobscuredSize;
    CGRect _inputViewBounds;

    CGFloat _viewportMetaTagWidth;
    BOOL _viewportMetaTagWidthWasExplicit;
    BOOL _viewportMetaTagCameFromImageDocument;
    CGFloat _initialScaleFactor;
    BOOL _fastClickingIsDisabled;

    BOOL _allowsLinkPreview;

    UIEdgeInsets _obscuredInsets;
    BOOL _haveSetObscuredInsets;
    BOOL _isChangingObscuredInsetsInteractively;

    UIEdgeInsets _unobscuredSafeAreaInsets;
    BOOL _haveSetUnobscuredSafeAreaInsets;
    BOOL _avoidsUnsafeArea;
    UIRectEdge _obscuredInsetEdgesAffectedBySafeArea;

    UIInterfaceOrientation _interfaceOrientationOverride;
    BOOL _overridesInterfaceOrientation;
    Optional<int32_t> _lastSentDeviceOrientation;

    BOOL _allowsViewportShrinkToFit;

    BOOL _hasCommittedLoadForMainFrame;
    BOOL _needsResetViewStateAfterCommitLoadForMainFrame;
    CyberKit::TransactionID _firstPaintAfterCommitLoadTransactionID;
    CyberKit::TransactionID _lastTransactionID;
    CyberKit::DynamicViewportUpdateMode _dynamicViewportUpdateMode;
    CyberKit::DynamicViewportSizeUpdateID _currentDynamicViewportSizeUpdateID;
    CATransform3D _resizeAnimationTransformAdjustments;
    CGFloat _animatedResizeOriginalContentWidth;
    RetainPtr<UIView> _resizeAnimationView;
    CGFloat _lastAdjustmentForScroller;
    Optional<CGRect> _frozenVisibleContentRect;
    Optional<CGRect> _frozenUnobscuredContentRect;

    BOOL _commitDidRestoreScrollPosition;
    Optional<CyberCore::FloatPoint> _scrollOffsetToRestore;
    CyberCore::FloatBoxExtent _obscuredInsetsWhenSaved;

    Optional<CyberCore::FloatPoint> _unobscuredCenterToRestore;
    Optional<CyberKit::TransactionID> _firstTransactionIDAfterPageRestore;
    double _scaleToRestore;

    BOOL _allowsBackForwardNavigationGestures;

    RetainPtr<UIView <WKWebViewContentProvider>> _customContentView;
    RetainPtr<UIView> _customContentFixedOverlayView;

    RetainPtr<NSTimer> _enclosingScrollViewScrollTimer;
    BOOL _didScrollSinceLastTimerFire;

    CyberCore::Color _scrollViewBackgroundColor;

    // This value tracks the current adjustment added to the bottom inset due to the keyboard sliding out from the bottom
    // when computing obscured content insets. This is used when updating the visible content rects where we should not
    // include this adjustment.
    CGFloat _totalScrollViewBottomInsetAdjustmentForKeyboard;
    BOOL _currentlyAdjustingScrollViewInsetsForKeyboard;

    BOOL _invokingUIScrollViewDelegateCallback;
    BOOL _didDeferUpdateVisibleContentRectsForUIScrollViewDelegateCallback;
    BOOL _didDeferUpdateVisibleContentRectsForAnyReason;
    BOOL _didDeferUpdateVisibleContentRectsForUnstableScrollView;

    BOOL _waitingForEndAnimatedResize;
    BOOL _waitingForCommitAfterAnimatedResize;

    Vector<WTF::Function<void ()>> _callbacksDeferredDuringResize;
    RetainPtr<NSMutableArray> _stableStatePresentationUpdateCallbacks;

    RetainPtr<WKPasswordView> _passwordView;

    BOOL _hasScheduledVisibleRectUpdate;
    BOOL _visibleContentRectUpdateScheduledFromScrollViewInStableState;

    _WKDragInteractionPolicy _dragInteractionPolicy;

    // For release-logging for <rdar://problem/39281269>.
    MonotonicTime _timeOfRequestForVisibleContentRectUpdate;
    MonotonicTime _timeOfLastVisibleContentRectUpdate;

    Optional<MonotonicTime> _timeOfFirstVisibleContentRectUpdateWithPendingCommit;

    NSUInteger _focusPreservationCount;
    NSUInteger _activeFocusedStateRetainCount;
#endif
}

- (BOOL)_isValid;
- (void)_didChangeEditorState;

#if ENABLE(ATTACHMENT_ELEMENT)
- (void)_didRemoveAttachment:(API::Attachment&)attachment;
- (void)_didInsertAttachment:(API::Attachment&)attachment withSource:(NSString *)source;
- (void)_didInvalidateDataForAttachment:(API::Attachment&)attachment;
#endif

- (void)_internalDoAfterNextPresentationUpdate:(void (^)(void))updateBlock withoutWaitingForPainting:(BOOL)withoutWaitingForPainting withoutWaitingForAnimatedResize:(BOOL)withoutWaitingForAnimatedResize;

- (void)_showSafeBrowsingWarning:(const CyberKit::SafeBrowsingWarning&)warning completionHandler:(CompletionHandler<void(Variant<CyberKit::ContinueUnsafeLoad, URL>&&)>&&)completionHandler;
- (void)_clearSafeBrowsingWarning;
- (void)_clearSafeBrowsingWarningIfForMainFrameNavigation;

- (Optional<BOOL>)_resolutionForShareSheetImmediateCompletionForTesting;

- (WKPageRef)_pageForTesting;
- (CyberKit::WebPageProxy*)_page;

@end

WKWebView* fromWebPageProxy(CyberKit::WebPageProxy&);

#if ENABLE(FULLSCREEN_API) && PLATFORM(IOS_FAMILY)
@interface WKWebView (FullScreenAPI_Internal)
-(WKFullScreenWindowController *)fullScreenWindowController;
@end
#endif

#if PLATFORM(IOS_FAMILY)
@interface WKWebView (_WKWebViewPrintFormatter)
@property (nonatomic, readonly) id <_WKWebViewPrintProvider> _printProvider;
@end
#endif
