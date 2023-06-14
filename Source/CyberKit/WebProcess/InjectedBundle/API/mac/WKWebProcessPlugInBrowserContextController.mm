/*
 * Copyright (C) 2012 Apple Inc. All rights reserved.
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

#import "config.h"
#import "WKWebProcessPlugInBrowserContextControllerInternal.h"

#if WK_API_ENABLED

#import "APIData.h"
#import "RemoteObjectRegistry.h"
#import "RemoteObjectRegistryMessages.h"
#import "WKBrowsingContextHandleInternal.h"
#import "WKBundleAPICast.h"
#import "WKBundlePage.h"
#import "WKBundlePagePrivate.h"
#import "WKDOMInternals.h"
#import "WKNSDictionary.h"
#import "WKNSError.h"
#import "WKNSString.h"
#import "WKNSURL.h"
#import "WKNSURLRequest.h"
#import "WKRetainPtr.h"
#import "WKStringCF.h"
#import "WKURLRequestNS.h"
#import "WKWebProcessPlugInEditingDelegate.h"
#import "WKWebProcessPlugInFrameInternal.h"
#import "WKWebProcessPlugInInternal.h"
#import "WKWebProcessPlugInFormDelegatePrivate.h"
#import "WKWebProcessPlugInLoadDelegate.h"
#import "WKWebProcessPlugInNodeHandleInternal.h"
#import "WKWebProcessPlugInPageGroupInternal.h"
#import "WKWebProcessPlugInRangeHandleInternal.h"
#import "WKWebProcessPlugInScriptWorldInternal.h"
#import "WebPage.h"
#import "WebProcess.h"
#import "_WKRemoteObjectRegistryInternal.h"
#import "_WKRenderingProgressEventsInternal.h"
#import "_WKSameDocumentNavigationTypeInternal.h"
#import <CyberCore/Document.h>
#import <CyberCore/DocumentFragment.h>
#import <CyberCore/Frame.h>
#import <CyberCore/HTMLFormElement.h>
#import <CyberCore/HTMLInputElement.h>
#import <pal/spi/cocoa/NSKeyedArchiverSPI.h>
#import <wtf/WeakObjCPtr.h>

@interface NSObject (WKDeprecatedDelegateMethods)
- (void)webProcessPlugInBrowserContextController:(WKWebProcessPlugInBrowserContextController *)controller didSameDocumentNavigationForFrame:(WKWebProcessPlugInFrame *)frame;
@end

@implementation WKWebProcessPlugInBrowserContextController {
    API::ObjectStorage<CyberKit::WebPage> _page;
    WeakObjCPtr<id <WKWebProcessPlugInLoadDelegate>> _loadDelegate;
    WeakObjCPtr<id <WKWebProcessPlugInFormDelegatePrivate>> _formDelegate;
    WeakObjCPtr<id <WKWebProcessPlugInEditingDelegate>> _editingDelegate;
    
    RetainPtr<_WKRemoteObjectRegistry> _remoteObjectRegistry;
}

static void didStartProvisionalLoadForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef* userDataRef, const void *clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:didStartProvisionalLoadForFrame:)])
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController didStartProvisionalLoadForFrame:wrapper(*CyberKit::toImpl(frame))];
}

static void didReceiveServerRedirectForProvisionalLoadForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef *userDataRef, const void *clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:didReceiveServerRedirectForProvisionalLoadForFrame:)])
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController didReceiveServerRedirectForProvisionalLoadForFrame:wrapper(*CyberKit::toImpl(frame))];
}

static void didFinishLoadForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef* userData, const void *clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:didFinishLoadForFrame:)])
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController didFinishLoadForFrame:wrapper(*CyberKit::toImpl(frame))];
}

static void globalObjectIsAvailableForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKBundleScriptWorldRef scriptWorld, const void* clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:globalObjectIsAvailableForFrame:inScriptWorld:)])
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController globalObjectIsAvailableForFrame:wrapper(*CyberKit::toImpl(frame)) inScriptWorld:wrapper(*CyberKit::toImpl(scriptWorld))];
}

static void didRemoveFrameFromHierarchy(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef* userData, const void* clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:didRemoveFrameFromHierarchy:)])
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController didRemoveFrameFromHierarchy:wrapper(*CyberKit::toImpl(frame))];
}

static void didCommitLoadForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef* userData, const void *clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:didCommitLoadForFrame:)])
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController didCommitLoadForFrame:wrapper(*CyberKit::toImpl(frame))];
}

static void didFinishDocumentLoadForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef* userData, const void *clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:didFinishDocumentLoadForFrame:)])
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController didFinishDocumentLoadForFrame:wrapper(*CyberKit::toImpl(frame))];
}

static void didFailProvisionalLoadWithErrorForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKErrorRef wkError, WKTypeRef* userData, const void *clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:didFailProvisionalLoadWithErrorForFrame:error:)])
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController didFailProvisionalLoadWithErrorForFrame:wrapper(*CyberKit::toImpl(frame)) error:wrapper(*CyberKit::toImpl(wkError))];
}

static void didFailLoadWithErrorForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKErrorRef wkError, WKTypeRef* userData, const void *clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:didFailLoadWithErrorForFrame:error:)])
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController didFailLoadWithErrorForFrame:wrapper(*CyberKit::toImpl(frame)) error:wrapper(*CyberKit::toImpl(wkError))];
}

static void didSameDocumentNavigationForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKSameDocumentNavigationType type, WKTypeRef* userData, const void *clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:didSameDocumentNavigation:forFrame:)])
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController didSameDocumentNavigation:toWKSameDocumentNavigationType(CyberKit::toSameDocumentNavigationType(type)) forFrame:wrapper(*CyberKit::toImpl(frame))];
    else {
        // FIXME: Remove this once clients switch to implementing the above delegate method.
        if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:didSameDocumentNavigationForFrame:)])
            [(NSObject *)loadDelegate webProcessPlugInBrowserContextController:pluginContextController didSameDocumentNavigationForFrame:wrapper(*CyberKit::toImpl(frame))];
    }
}

static void didLayoutForFrame(WKBundlePageRef page, WKBundleFrameRef frame, const void* clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:didLayoutForFrame:)])
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController didLayoutForFrame:wrapper(*CyberKit::toImpl(frame))];
}

static void didReachLayoutMilestone(WKBundlePageRef page, WKLayoutMilestones milestones, WKTypeRef* userData, const void *clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:renderingProgressDidChange:)])
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController renderingProgressDidChange:renderingProgressEvents(CyberKit::toLayoutMilestones(milestones))];
}

static WKLayoutMilestones layoutMilestones(const void *clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();
    
    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextControllerRenderingProgressEvents:)]) {
        _WKRenderingProgressEvents milestones = [loadDelegate webProcessPlugInBrowserContextControllerRenderingProgressEvents:pluginContextController];
        return static_cast<WKLayoutMilestones>(milestones);
    }
    return { };
}

static void didFirstVisuallyNonEmptyLayoutForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef* userData, const void *clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:didFirstVisuallyNonEmptyLayoutForFrame:)])
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController didFirstVisuallyNonEmptyLayoutForFrame:wrapper(*CyberKit::toImpl(frame))];
}

static void didHandleOnloadEventsForFrame(WKBundlePageRef page, WKBundleFrameRef frame, const void* clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:didHandleOnloadEventsForFrame:)])
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController didHandleOnloadEventsForFrame:wrapper(*CyberKit::toImpl(frame))];
}

static WKStringRef userAgentForURL(WKBundleFrameRef frame, WKURLRef url, const void* clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();
    
    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:frame:userAgentForURL:)]) {
        WKWebProcessPlugInFrame *newFrame = wrapper(*CyberKit::toImpl(frame));
        NSString *string = [loadDelegate webProcessPlugInBrowserContextController:pluginContextController frame:newFrame userAgentForURL:wrapper(*CyberKit::toImpl(url))];
        if (!string)
            return nullptr;

        return WKStringCreateWithCFString((__bridge CFStringRef)string);
    }
    
    return nullptr;
}

static void setUpPageLoaderClient(WKWebProcessPlugInBrowserContextController *contextController, CyberKit::WebPage& page)
{
    WKBundlePageLoaderClientV10 client;
    memset(&client, 0, sizeof(client));

    client.base.version = 10;
    client.base.clientInfo = (__bridge void*)contextController;
    client.didStartProvisionalLoadForFrame = didStartProvisionalLoadForFrame;
    client.didReceiveServerRedirectForProvisionalLoadForFrame = didReceiveServerRedirectForProvisionalLoadForFrame;
    client.didCommitLoadForFrame = didCommitLoadForFrame;
    client.didFinishDocumentLoadForFrame = didFinishDocumentLoadForFrame;
    client.didFailProvisionalLoadWithErrorForFrame = didFailProvisionalLoadWithErrorForFrame;
    client.didFailLoadWithErrorForFrame = didFailLoadWithErrorForFrame;
    client.didSameDocumentNavigationForFrame = didSameDocumentNavigationForFrame;
    client.didFinishLoadForFrame = didFinishLoadForFrame;
    client.globalObjectIsAvailableForFrame = globalObjectIsAvailableForFrame;
    client.didRemoveFrameFromHierarchy = didRemoveFrameFromHierarchy;
    client.didHandleOnloadEventsForFrame = didHandleOnloadEventsForFrame;
    client.didFirstVisuallyNonEmptyLayoutForFrame = didFirstVisuallyNonEmptyLayoutForFrame;
    client.userAgentForURL = userAgentForURL;

    client.didLayoutForFrame = didLayoutForFrame;
    client.didLayout = didReachLayoutMilestone;
    client.layoutMilestones = layoutMilestones;

    WKBundlePageSetPageLoaderClient(toAPI(&page), &client.base);
}

static WKURLRequestRef willSendRequestForFrame(WKBundlePageRef, WKBundleFrameRef frame, uint64_t resourceIdentifier, WKURLRequestRef request, WKURLResponseRef redirectResponse, const void* clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:frame:willSendRequestForResource:request:redirectResponse:)]) {
        NSURLRequest *originalRequest = wrapper(*CyberKit::toImpl(request));
        RetainPtr<NSURLRequest> substituteRequest = [loadDelegate webProcessPlugInBrowserContextController:pluginContextController frame:wrapper(*CyberKit::toImpl(frame)) willSendRequestForResource:resourceIdentifier
            request:originalRequest redirectResponse:CyberKit::toImpl(redirectResponse)->resourceResponse().nsURLResponse()];

        if (substituteRequest != originalRequest)
            return substituteRequest ? WKURLRequestCreateWithNSURLRequest(substituteRequest.get()) : nullptr;
    } else if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:frame:willSendRequest:redirectResponse:)]) {
        NSURLRequest *originalRequest = wrapper(*CyberKit::toImpl(request));
        RetainPtr<NSURLRequest> substituteRequest = [loadDelegate webProcessPlugInBrowserContextController:pluginContextController frame:wrapper(*CyberKit::toImpl(frame)) willSendRequest:originalRequest
            redirectResponse:CyberKit::toImpl(redirectResponse)->resourceResponse().nsURLResponse()];

        if (substituteRequest != originalRequest)
            return substituteRequest ? WKURLRequestCreateWithNSURLRequest(substituteRequest.get()) : nullptr;
    }

    WKRetain(request);
    return request;
}

static void didInitiateLoadForResource(WKBundlePageRef, WKBundleFrameRef frame, uint64_t resourceIdentifier, WKURLRequestRef request, bool pageIsProvisionallyLoading, const void* clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:frame:didInitiateLoadForResource:request:pageIsProvisionallyLoading:)]) {
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController frame:wrapper(*CyberKit::toImpl(frame)) didInitiateLoadForResource:resourceIdentifier request:wrapper(*CyberKit::toImpl(request))
            pageIsProvisionallyLoading:pageIsProvisionallyLoading];
    } else if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:frame:didInitiateLoadForResource:request:)]) {
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController frame:wrapper(*CyberKit::toImpl(frame)) didInitiateLoadForResource:resourceIdentifier request:wrapper(*CyberKit::toImpl(request))];
    }
}

static void didFinishLoadForResource(WKBundlePageRef, WKBundleFrameRef frame, uint64_t resourceIdentifier, const void* clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:frame:didFinishLoadForResource:)]) {
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController frame:wrapper(*CyberKit::toImpl(frame)) didFinishLoadForResource:resourceIdentifier];
    }
}

static void didFailLoadForResource(WKBundlePageRef, WKBundleFrameRef frame, uint64_t resourceIdentifier, WKErrorRef error, const void* clientInfo)
{
    auto pluginContextController = (__bridge WKWebProcessPlugInBrowserContextController *)clientInfo;
    auto loadDelegate = pluginContextController->_loadDelegate.get();

    if ([loadDelegate respondsToSelector:@selector(webProcessPlugInBrowserContextController:frame:didFailLoadForResource:error:)]) {
        [loadDelegate webProcessPlugInBrowserContextController:pluginContextController frame:wrapper(*CyberKit::toImpl(frame)) didFailLoadForResource:resourceIdentifier error:wrapper(*CyberKit::toImpl(error))];
    }
}

static void setUpResourceLoadClient(WKWebProcessPlugInBrowserContextController *contextController, CyberKit::WebPage& page)
{
    WKBundlePageResourceLoadClientV1 client;
    memset(&client, 0, sizeof(client));

    client.base.version = 1;
    client.base.clientInfo = (__bridge void*) contextController;
    client.willSendRequestForFrame = willSendRequestForFrame;
    client.didInitiateLoadForResource = didInitiateLoadForResource;
    client.didFinishLoadForResource = didFinishLoadForResource;
    client.didFailLoadForResource = didFailLoadForResource;

    WKBundlePageSetResourceLoadClient(toAPI(&page), &client.base);
}

- (id <WKWebProcessPlugInLoadDelegate>)loadDelegate
{
    return _loadDelegate.getAutoreleased();
}

- (void)setLoadDelegate:(id <WKWebProcessPlugInLoadDelegate>)loadDelegate
{
    _loadDelegate = loadDelegate;

    if (loadDelegate) {
        setUpPageLoaderClient(self, *_page);
        setUpResourceLoadClient(self, *_page);
    } else {
        WKBundlePageSetPageLoaderClient(toAPI(_page.get()), nullptr);
        WKBundlePageSetResourceLoadClient(toAPI(_page.get()), nullptr);
    }
}

- (void)dealloc
{
    if (_remoteObjectRegistry)
        [_remoteObjectRegistry _invalidate];

    _page->~WebPage();

    [super dealloc];
}

- (WKDOMDocument *)mainFrameDocument
{
    CyberCore::Frame* webCoreMainFrame = _page->mainFrame();
    if (!webCoreMainFrame)
        return nil;

    return CyberKit::toWKDOMDocument(webCoreMainFrame->document());
}

- (WKDOMRange *)selectedRange
{
    RefPtr<CyberCore::Range> range = _page->currentSelectionAsRange();
    if (!range)
        return nil;

    return CyberKit::toWKDOMRange(range.get());
}

- (WKWebProcessPlugInFrame *)mainFrame
{
    return wrapper(_page->mainWebFrame());
}

- (WKWebProcessPlugInPageGroup *)pageGroup
{
    return wrapper(*_page->pageGroup());
}

#pragma mark WKObject protocol implementation

- (API::Object&)_apiObject
{
    return *_page;
}

@end

@implementation WKWebProcessPlugInBrowserContextController (WKPrivate)

- (WKBundlePageRef)_bundlePageRef
{
    return toAPI(_page.get());
}

- (WKBrowsingContextHandle *)handle
{
    return [[[WKBrowsingContextHandle alloc] _initWithPageID:_page->pageID()] autorelease];
}

+ (instancetype)lookUpBrowsingContextFromHandle:(WKBrowsingContextHandle *)handle
{
    return wrapper(CyberKit::WebProcess::singleton().webPage(handle.pageID));
}

- (_WKRemoteObjectRegistry *)_remoteObjectRegistry
{
    if (!_remoteObjectRegistry)
        _remoteObjectRegistry = adoptNS([[_WKRemoteObjectRegistry alloc] _initWithWebPage:*_page]);

    return _remoteObjectRegistry.get();
}

- (id <WKWebProcessPlugInFormDelegatePrivate>)_formDelegate
{
    return _formDelegate.getAutoreleased();
}

- (void)_setFormDelegate:(id <WKWebProcessPlugInFormDelegatePrivate>)formDelegate
{
    _formDelegate = formDelegate;

    class FormClient : public API::InjectedBundle::FormClient {
    public:
        explicit FormClient(WKWebProcessPlugInBrowserContextController *controller)
            : m_controller(controller)
        {
        }

        virtual ~FormClient() { }

        void didFocusTextField(CyberKit::WebPage*, CyberCore::HTMLInputElement* inputElement, CyberKit::WebFrame* frame) override
        {
            auto formDelegate = m_controller->_formDelegate.get();

            if ([formDelegate respondsToSelector:@selector(_webProcessPlugInBrowserContextController:didFocusTextField:inFrame:)])
                [formDelegate _webProcessPlugInBrowserContextController:m_controller didFocusTextField:wrapper(*CyberKit::InjectedBundleNodeHandle::getOrCreate(inputElement).get()) inFrame:wrapper(*frame)];
        }

        void willSendSubmitEvent(CyberKit::WebPage*, CyberCore::HTMLFormElement* formElement, CyberKit::WebFrame* targetFrame, CyberKit::WebFrame* sourceFrame, const Vector<std::pair<String, String>>& values) override
        {
            auto formDelegate = m_controller->_formDelegate.get();

            if ([formDelegate respondsToSelector:@selector(_webProcessPlugInBrowserContextController:willSendSubmitEventToForm:inFrame:targetFrame:values:)]) {
                auto valueMap = adoptNS([[NSMutableDictionary alloc] initWithCapacity:values.size()]);
                for (const auto& pair : values)
                    [valueMap setObject:pair.second forKey:pair.first];

                [formDelegate _webProcessPlugInBrowserContextController:m_controller willSendSubmitEventToForm:wrapper(*CyberKit::InjectedBundleNodeHandle::getOrCreate(formElement).get())
                    inFrame:wrapper(*sourceFrame) targetFrame:wrapper(*targetFrame) values:valueMap.get()];
            }
        }

        static void encodeUserObject(NSObject <NSSecureCoding> *userObject, RefPtr<API::Object>& userData)
        {
            if (!userObject)
                return;

            auto archiver = secureArchiver();
            @try {
                [archiver encodeObject:userObject forKey:@"userObject"];
            } @catch (NSException *exception) {
                LOG_ERROR("Failed to encode user object: %@", exception);
                return;
            }
            [archiver finishEncoding];

            userData = API::Data::createWithoutCopying(archiver.get().encodedData);
        }

        void willSubmitForm(CyberKit::WebPage*, CyberCore::HTMLFormElement* formElement, CyberKit::WebFrame* frame, CyberKit::WebFrame* sourceFrame, const Vector<std::pair<WTF::String, WTF::String>>& values, RefPtr<API::Object>& userData) override
        {
            auto formDelegate = m_controller->_formDelegate.get();

            if ([formDelegate respondsToSelector:@selector(_webProcessPlugInBrowserContextController:willSubmitForm:toFrame:fromFrame:withValues:)]) {
                auto valueMap = adoptNS([[NSMutableDictionary alloc] initWithCapacity:values.size()]);
                for (const auto& pair : values)
                    [valueMap setObject:pair.second forKey:pair.first];

                NSObject <NSSecureCoding> *userObject = [formDelegate _webProcessPlugInBrowserContextController:m_controller willSubmitForm:wrapper(*CyberKit::InjectedBundleNodeHandle::getOrCreate(formElement).get()) toFrame:wrapper(*frame) fromFrame:wrapper(*sourceFrame) withValues:valueMap.get()];
                encodeUserObject(userObject, userData);
            }
        }

        void textDidChangeInTextField(CyberKit::WebPage*, CyberCore::HTMLInputElement* inputElement, CyberKit::WebFrame* frame, bool initiatedByUserTyping) override
        {
            auto formDelegate = m_controller->_formDelegate.get();

            if ([formDelegate respondsToSelector:@selector(_webProcessPlugInBrowserContextController:textDidChangeInTextField:inFrame:initiatedByUserTyping:)])
                [formDelegate _webProcessPlugInBrowserContextController:m_controller textDidChangeInTextField:wrapper(*CyberKit::InjectedBundleNodeHandle::getOrCreate(inputElement)) inFrame:wrapper(*frame) initiatedByUserTyping:initiatedByUserTyping];
        }

        void willBeginInputSession(CyberKit::WebPage*, CyberCore::Element* element, CyberKit::WebFrame* frame, bool userIsInteracting, RefPtr<API::Object>& userData) override
        {
            auto formDelegate = m_controller->_formDelegate.get();

            if ([formDelegate respondsToSelector:@selector(_webProcessPlugInBrowserContextController:willBeginInputSessionForElement:inFrame:userIsInteracting:)]) {
                NSObject<NSSecureCoding> *userObject = [formDelegate _webProcessPlugInBrowserContextController:m_controller willBeginInputSessionForElement:wrapper(*CyberKit::InjectedBundleNodeHandle::getOrCreate(element)) inFrame:wrapper(*frame) userIsInteracting:userIsInteracting];
                encodeUserObject(userObject, userData);
            } else if (userIsInteracting && [formDelegate respondsToSelector:@selector(_webProcessPlugInBrowserContextController:willBeginInputSessionForElement:inFrame:)]) {
                // FIXME: We check userIsInteracting so that we don't begin an input session for a
                // programmatic focus that doesn't cause the keyboard to appear. But this misses the case of
                // a programmatic focus happening while the keyboard is already shown. Once we have a way to
                // know the keyboard state in the Web Process, we should refine the condition.
                NSObject<NSSecureCoding> *userObject = [formDelegate _webProcessPlugInBrowserContextController:m_controller willBeginInputSessionForElement:wrapper(*CyberKit::InjectedBundleNodeHandle::getOrCreate(element)) inFrame:wrapper(*frame)];
                encodeUserObject(userObject, userData);
            }
        }

        bool shouldNotifyOnFormChanges(CyberKit::WebPage*) override
        {
            auto formDelegate = m_controller->_formDelegate.get();

            if (![formDelegate respondsToSelector:@selector(_webProcessPlugInBrowserContextControllerShouldNotifyOnFormChanges:)])
                return false;

            return [formDelegate _webProcessPlugInBrowserContextControllerShouldNotifyOnFormChanges:m_controller];
        }

        void didAssociateFormControls(CyberKit::WebPage*, const Vector<RefPtr<CyberCore::Element>>& elements) override
        {
            auto formDelegate = m_controller->_formDelegate.get();

            if (![formDelegate respondsToSelector:@selector(_webProcessPlugInBrowserContextController:didAssociateFormControls:)])
                return;

            auto controls = adoptNS([[NSMutableArray alloc] initWithCapacity:elements.size()]);
            for (const auto& element : elements)
                [controls addObject:wrapper(*CyberKit::InjectedBundleNodeHandle::getOrCreate(element.get()))];
            return [formDelegate _webProcessPlugInBrowserContextController:m_controller didAssociateFormControls:controls.get()];
        }

    private:
        WKWebProcessPlugInBrowserContextController *m_controller;
    };

    if (formDelegate)
        _page->setInjectedBundleFormClient(std::make_unique<FormClient>(self));
    else
        _page->setInjectedBundleFormClient(nullptr);
}

- (id <WKWebProcessPlugInEditingDelegate>)_editingDelegate
{
    return _editingDelegate.getAutoreleased();
}

static inline WKEditorInsertAction toWK(CyberCore::EditorInsertAction action)
{
    switch (action) {
    case CyberCore::EditorInsertAction::Typed:
        return WKEditorInsertActionTyped;
    case CyberCore::EditorInsertAction::Pasted:
        return WKEditorInsertActionPasted;
    case CyberCore::EditorInsertAction::Dropped:
        return WKEditorInsertActionDropped;
    }
}

- (void)_setEditingDelegate:(id <WKWebProcessPlugInEditingDelegate>)editingDelegate
{
    _editingDelegate = editingDelegate;

    class Client final : public API::InjectedBundle::EditorClient {
    public:
        explicit Client(WKWebProcessPlugInBrowserContextController *controller)
            : m_controller { controller }
            , m_delegateMethods { m_controller->_editingDelegate.get() }
        {
        }

    private:
        bool shouldInsertText(CyberKit::WebPage&, StringImpl* text, CyberCore::Range* rangeToReplace, CyberCore::EditorInsertAction action) final
        {
            if (!m_delegateMethods.shouldInsertText)
                return true;

            return [m_controller->_editingDelegate.get() _webProcessPlugInBrowserContextController:m_controller shouldInsertText:String(text) replacingRange:wrapper(*CyberKit::InjectedBundleRangeHandle::getOrCreate(rangeToReplace)) givenAction:toWK(action)];
        }

        bool shouldChangeSelectedRange(CyberKit::WebPage&, CyberCore::Range* fromRange, CyberCore::Range* toRange, CyberCore::EAffinity affinity, bool stillSelecting) final
        {
            if (!m_delegateMethods.shouldChangeSelectedRange)
                return true;

            auto apiFromRange = fromRange ? adoptNS([[WKDOMRange alloc] _initWithImpl:fromRange]) : nil;
            auto apiToRange = toRange ? adoptNS([[WKDOMRange alloc] _initWithImpl:toRange]) : nil;
#if PLATFORM(IOS_FAMILY)
            UITextStorageDirection apiAffinity = affinity == CyberCore::UPSTREAM ? UITextStorageDirectionBackward : UITextStorageDirectionForward;
#else
            NSSelectionAffinity apiAffinity = affinity == CyberCore::UPSTREAM ? NSSelectionAffinityUpstream : NSSelectionAffinityDownstream;
#endif

            return [m_controller->_editingDelegate.get() _webProcessPlugInBrowserContextController:m_controller shouldChangeSelectedRange:apiFromRange.get() toRange:apiToRange.get() affinity:apiAffinity stillSelecting:stillSelecting];
        }

        void didChange(CyberKit::WebPage&, StringImpl*) final
        {
            if (!m_delegateMethods.didChange)
                return;

            [m_controller->_editingDelegate.get() _webProcessPlugInBrowserContextControllerDidChangeByEditing:m_controller];
        }

        void willWriteToPasteboard(CyberKit::WebPage&, CyberCore::Range* range) final
        {
            if (!m_delegateMethods.willWriteToPasteboard)
                return;

            [m_controller->_editingDelegate.get() _webProcessPlugInBrowserContextController:m_controller willWriteRangeToPasteboard:wrapper(*CyberKit::InjectedBundleRangeHandle::getOrCreate(range).get())];
        }

        void getPasteboardDataForRange(CyberKit::WebPage&, CyberCore::Range* range, Vector<String>& pasteboardTypes, Vector<RefPtr<CyberCore::SharedBuffer>>& pasteboardData) final
        {
            if (!m_delegateMethods.getPasteboardDataForRange)
                return;

            auto dataByType = [m_controller->_editingDelegate.get() _webProcessPlugInBrowserContextController:m_controller pasteboardDataForRange:wrapper(*CyberKit::InjectedBundleRangeHandle::getOrCreate(range).get())];
            for (NSString *type in dataByType) {
                pasteboardTypes.append(type);
                pasteboardData.append(CyberCore::SharedBuffer::create(dataByType[type]));
            };
        }

        void didWriteToPasteboard(CyberKit::WebPage&) final
        {
            if (!m_delegateMethods.didWriteToPasteboard)
                return;

            [m_controller->_editingDelegate.get() _webProcessPlugInBrowserContextControllerDidWriteToPasteboard:m_controller];
        }

        bool performTwoStepDrop(CyberKit::WebPage&, CyberCore::DocumentFragment& fragment, CyberCore::Range& range, bool isMove) final
        {
            if (!m_delegateMethods.performTwoStepDrop)
                return false;

            auto rangeHandle = CyberKit::InjectedBundleRangeHandle::getOrCreate(&range);
            auto nodeHandle = CyberKit::InjectedBundleNodeHandle::getOrCreate(&fragment);
            return [m_controller->_editingDelegate.get() _webProcessPlugInBrowserContextController:m_controller performTwoStepDrop:wrapper(*nodeHandle) atDestination:wrapper(*rangeHandle) isMove:isMove];
        }

        WTF::String replacementURLForResource(CyberKit::WebPage&, Ref<CyberCore::SharedBuffer>&& resourceData, const WTF::String& mimeType)
        {
            if (!m_delegateMethods.replacementURLForResource)
                return { };

            NSString *type = (NSString *)mimeType;
            auto data = resourceData->createNSData();
            return [m_controller->_editingDelegate.get() _webProcessPlugInBrowserContextController:m_controller replacementURLForResource:data.get() mimeType:type];
        }

        WKWebProcessPlugInBrowserContextController *m_controller;
        const struct DelegateMethods {
            DelegateMethods(RetainPtr<id <WKWebProcessPlugInEditingDelegate>> delegate)
                : shouldInsertText([delegate respondsToSelector:@selector(_webProcessPlugInBrowserContextController:shouldInsertText:replacingRange:givenAction:)])
                , shouldChangeSelectedRange([delegate respondsToSelector:@selector(_webProcessPlugInBrowserContextController:shouldChangeSelectedRange:toRange:affinity:stillSelecting:)])
                , didChange([delegate respondsToSelector:@selector(_webProcessPlugInBrowserContextControllerDidChangeByEditing:)])
                , willWriteToPasteboard([delegate respondsToSelector:@selector(_webProcessPlugInBrowserContextController:willWriteRangeToPasteboard:)])
                , getPasteboardDataForRange([delegate respondsToSelector:@selector(_webProcessPlugInBrowserContextController:pasteboardDataForRange:)])
                , didWriteToPasteboard([delegate respondsToSelector:@selector(_webProcessPlugInBrowserContextControllerDidWriteToPasteboard:)])
                , performTwoStepDrop([delegate respondsToSelector:@selector(_webProcessPlugInBrowserContextController:performTwoStepDrop:atDestination:isMove:)])
                , replacementURLForResource([delegate respondsToSelector:@selector(_webProcessPlugInBrowserContextController:replacementURLForResource:mimeType:)])
            {
            }

            bool shouldInsertText;
            bool shouldChangeSelectedRange;
            bool didChange;
            bool willWriteToPasteboard;
            bool getPasteboardDataForRange;
            bool didWriteToPasteboard;
            bool performTwoStepDrop;
            bool replacementURLForResource;
        } m_delegateMethods;
    };

    if (editingDelegate)
        _page->setInjectedBundleEditorClient(std::make_unique<Client>(self));
    else
        _page->setInjectedBundleEditorClient(nullptr);
}

- (BOOL)_defersLoading
{
    return _page->defersLoading();
}

- (void)_setDefersLoading:(BOOL)defersLoading
{
    _page->setDefersLoading(defersLoading);
}

- (BOOL)_usesNonPersistentWebsiteDataStore
{
    return _page->usesEphemeralSession();
}

@end

#endif // WK_API_ENABLED
