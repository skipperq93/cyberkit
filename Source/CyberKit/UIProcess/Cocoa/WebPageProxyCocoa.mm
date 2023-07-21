/*
 * Copyright (C) 2014-2019 Apple Inc. All rights reserved.
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
#import "WebPageProxy.h"

#import "APIAttachment.h"
#import "APIUIClient.h"
#import "DataDetectionResult.h"
#import "LoadParameters.h"
#import "PageClient.h"
#import "SafeBrowsingSPI.h"
#import "SafeBrowsingWarning.h"
#import "WebPageMessages.h"
#import "WebProcessProxy.h"
#import "WebsiteDataStore.h"
#import <CyberCore/DragItem.h>
#import <CyberCore/NotImplemented.h>
#import <CyberCore/SearchPopupMenuCocoa.h>
#import <CyberCore/ValidationBubble.h>
#import <wtf/BlockPtr.h>
#import <wtf/cf/TypeCastsCF.h>

namespace CyberKit {
using namespace CyberCore;

#if ENABLE(DATA_DETECTION)
void WebPageProxy::setDataDetectionResult(const DataDetectionResult& dataDetectionResult)
{
    m_dataDetectionResults = dataDetectionResult.results;
}
#endif

void WebPageProxy::saveRecentSearches(const String& name, const Vector<CyberCore::RecentSearch>& searchItems)
{
    if (!name) {
        // FIXME: This should be a message check.
        return;
    }

    CyberCore::saveRecentSearches(name, searchItems);
}

void WebPageProxy::loadRecentSearches(const String& name, CompletionHandler<void(Vector<CyberCore::RecentSearch>&&)>&& completionHandler)
{
    if (!name) {
        // FIXME: This should be a message check.
        return completionHandler({ });
    }

    completionHandler(CyberCore::loadRecentSearches(name));
}

void WebPageProxy::beginSafeBrowsingCheck(const URL& url, bool forMainFrameNavigation, WebFramePolicyListenerProxy& listener)
{
#if HAVE(SAFE_BROWSING)
    SSBLookupContext *context = [SSBLookupContext sharedLookupContext];
    if (!context)
        return listener.didReceiveSafeBrowsingResults({ });
    [context lookUpURL:url completionHandler:makeBlockPtr([listener = makeRef(listener), forMainFrameNavigation, url = url] (SSBLookupResult *result, NSError *error) mutable {
        RunLoop::main().dispatch([listener = WTFMove(listener), result = retainPtr(result), error = retainPtr(error), forMainFrameNavigation, url = WTFMove(url)] {
            if (error) {
                listener->didReceiveSafeBrowsingResults({ });
                return;
            }

            for (SSBServiceLookupResult *lookupResult in [result serviceLookupResults]) {
                if (lookupResult.isPhishing || lookupResult.isMalware || lookupResult.isUnwantedSoftware) {
                    listener->didReceiveSafeBrowsingResults(SafeBrowsingWarning::create(url, forMainFrameNavigation, lookupResult));
                    return;
                }
            }
            listener->didReceiveSafeBrowsingResults({ });
        });
    }).get()];
#else
    listener.didReceiveSafeBrowsingResults({ });
#endif
}

#if ENABLE(CONTENT_FILTERING)
void WebPageProxy::contentFilterDidBlockLoadForFrame(const CyberCore::ContentFilterUnblockHandler& unblockHandler, uint64_t frameID)
{
    contentFilterDidBlockLoadForFrameShared(m_process.copyRef(), unblockHandler, frameID);
}

void WebPageProxy::contentFilterDidBlockLoadForFrameShared(Ref<WebProcessProxy>&& process, const CyberCore::ContentFilterUnblockHandler& unblockHandler, uint64_t frameID)
{
    if (WebFrameProxy* frame = process->webFrame(frameID))
        frame->contentFilterDidBlockLoad(unblockHandler);
}
#endif

void WebPageProxy::addPlatformLoadParameters(LoadParameters& loadParameters)
{
    loadParameters.dataDetectionContext = m_uiClient->dataDetectionContext();
}

void WebPageProxy::createSandboxExtensionsIfNeeded(const Vector<String>& files, SandboxExtension::Handle& fileReadHandle, SandboxExtension::HandleArray& fileUploadHandles)
{
    if (!files.size())
        return;

    if (files.size() == 1) {
        BOOL isDirectory;
        if ([[NSFileManager defaultManager] fileExistsAtPath:files[0] isDirectory:&isDirectory] && !isDirectory) {
#if HAVE(SANDBOX_ISSUE_READ_EXTENSION_TO_PROCESS_BY_PID)
            if (!SandboxExtension::createHandleForReadByPid("/", processIdentifier(), fileReadHandle))
#endif
            SandboxExtension::createHandle("/", SandboxExtension::Type::ReadOnly, fileReadHandle);
            willAcquireUniversalFileReadSandboxExtension(m_process);
        }
    }

    fileUploadHandles.allocate(files.size());
    for (size_t i = 0; i< files.size(); i++) {
        NSString *file = files[i];
        if (![[NSFileManager defaultManager] fileExistsAtPath:file])
            continue;
        SandboxExtension::createHandle(file, SandboxExtension::Type::ReadOnly, fileUploadHandles[i]);
    }
}

bool WebPageProxy::scrollingUpdatesDisabledForTesting()
{
    return pageClient().scrollingUpdatesDisabledForTesting();
}

#if ENABLE(DRAG_SUPPORT)

void WebPageProxy::startDrag(const DragItem& dragItem, const ShareableBitmap::Handle& dragImageHandle)
{
    pageClient().startDrag(dragItem, dragImageHandle);
}

#if PLATFORM(IOS_FAMILY)

void WebPageProxy::setPromisedDataForImage(const String&, const SharedMemory::Handle&, uint64_t, const String&, const String&, const String&, const String&, const String&, const SharedMemory::Handle&, uint64_t)
{
    notImplemented();
}

void WebPageProxy::setDragCaretRect(const IntRect& dragCaretRect)
{
    if (m_currentDragCaretRect == dragCaretRect)
        return;

    auto previousRect = m_currentDragCaretRect;
    m_currentDragCaretRect = dragCaretRect;
    pageClient().didChangeDragCaretRect(previousRect, dragCaretRect);
}

#endif // PLATFORM(IOS_FAMILY)

#endif // ENABLE(DRAG_SUPPORT)

#if ENABLE(ATTACHMENT_ELEMENT)

void WebPageProxy::platformRegisterAttachment(Ref<API::Attachment>&& attachment, const String& preferredFileName, const IPC::DataReference& dataReference)
{
    if (dataReference.isEmpty())
        return;

    auto buffer = SharedBuffer::create(dataReference.data(), dataReference.size());
    auto fileWrapper = adoptNS([pageClient().allocFileWrapperInstance() initRegularFileWithContents:buffer->createNSData().autorelease()]);
    [fileWrapper setPreferredFilename:preferredFileName];
    attachment->setFileWrapper(fileWrapper.get());
}

void WebPageProxy::platformRegisterAttachment(Ref<API::Attachment>&& attachment, const String& filePath)
{
    if (!filePath)
        return;

    auto fileWrapper = adoptNS([pageClient().allocFileWrapperInstance() initWithURL:[NSURL fileURLWithPath:filePath] options:0 error:nil]);
    attachment->setFileWrapper(fileWrapper.get());
}

void WebPageProxy::platformCloneAttachment(Ref<API::Attachment>&& fromAttachment, Ref<API::Attachment>&& toAttachment)
{
    toAttachment->setFileWrapper(fromAttachment->fileWrapper());
}

#endif // ENABLE(ATTACHMENT_ELEMENT)
    
void WebPageProxy::performDictionaryLookupAtLocation(const CyberCore::FloatPoint& point)
{
    if (!hasRunningProcess())
        return;
    
    process().send(Messages::WebPage::PerformDictionaryLookupAtLocation(point), m_pageID);
}

void WebPageProxy::performDictionaryLookupOfCurrentSelection()
{
    if (!hasRunningProcess())
        return;
    
    process().send(Messages::WebPage::PerformDictionaryLookupOfCurrentSelection(), m_pageID);
}
    
#if ENABLE(APPLE_PAY)

IPC::Connection* WebPageProxy::paymentCoordinatorConnection(const WebPaymentCoordinatorProxy&)
{
    return messageSenderConnection();
}

const String& WebPageProxy::paymentCoordinatorBoundInterfaceIdentifier(const WebPaymentCoordinatorProxy&, PAL::SessionID sessionID)
{
    ASSERT_UNUSED(sessionID, sessionID == websiteDataStore().sessionID());
    return websiteDataStore().boundInterfaceIdentifier();
}

const String& WebPageProxy::paymentCoordinatorSourceApplicationBundleIdentifier(const WebPaymentCoordinatorProxy&, PAL::SessionID sessionID)
{
    ASSERT_UNUSED(sessionID, sessionID == websiteDataStore().sessionID());
    return websiteDataStore().sourceApplicationBundleIdentifier();
}

const String& WebPageProxy::paymentCoordinatorSourceApplicationSecondaryIdentifier(const WebPaymentCoordinatorProxy&, PAL::SessionID sessionID)
{
    ASSERT_UNUSED(sessionID, sessionID == websiteDataStore().sessionID());
    return websiteDataStore().sourceApplicationSecondaryIdentifier();
}

void WebPageProxy::paymentCoordinatorAddMessageReceiver(WebPaymentCoordinatorProxy&, const IPC::StringReference& messageReceiverName, IPC::MessageReceiver& messageReceiver)
{
    process().addMessageReceiver(messageReceiverName, m_pageID, messageReceiver);
}

void WebPageProxy::paymentCoordinatorRemoveMessageReceiver(WebPaymentCoordinatorProxy&, const IPC::StringReference& messageReceiverName)
{
    process().removeMessageReceiver(messageReceiverName, m_pageID);
}

#endif

#if ENABLE(SPEECH_SYNTHESIS)
void WebPageProxy::didStartSpeaking(CyberCore::PlatformSpeechSynthesisUtterance&)
{
    if (speechSynthesisData().speakingStartedCompletionHandler)
        speechSynthesisData().speakingStartedCompletionHandler();
}

void WebPageProxy::didFinishSpeaking(CyberCore::PlatformSpeechSynthesisUtterance&)
{
    if (speechSynthesisData().speakingFinishedCompletionHandler)
        speechSynthesisData().speakingFinishedCompletionHandler();
}

void WebPageProxy::didPauseSpeaking(CyberCore::PlatformSpeechSynthesisUtterance&)
{
    if (speechSynthesisData().speakingPausedCompletionHandler)
        speechSynthesisData().speakingPausedCompletionHandler();
}

void WebPageProxy::didResumeSpeaking(CyberCore::PlatformSpeechSynthesisUtterance&)
{
    if (speechSynthesisData().speakingResumedCompletionHandler)
        speechSynthesisData().speakingResumedCompletionHandler();
}

void WebPageProxy::speakingErrorOccurred(CyberCore::PlatformSpeechSynthesisUtterance&)
{
    process().send(Messages::WebPage::SpeakingErrorOccurred(), m_pageID);
}

void WebPageProxy::boundaryEventOccurred(CyberCore::PlatformSpeechSynthesisUtterance&, CyberCore::SpeechBoundary speechBoundary, unsigned charIndex)
{
    process().send(Messages::WebPage::BoundaryEventOccurred(speechBoundary == CyberCore::SpeechBoundary::SpeechWordBoundary, charIndex), m_pageID);
}

void WebPageProxy::voicesDidChange()
{
    process().send(Messages::WebPage::VoicesDidChange(), m_pageID);
}
#endif // ENABLE(SPEECH_SYNTHESIS)

#if HAVE(VISIBILITY_PROPAGATION_VIEW)
void WebPageProxy::didCreateContextForVisibilityPropagation(LayerHostingContextID contextID)
{
    m_contextIDForVisibilityPropagation = contextID;
    pageClient().didCreateContextForVisibilityPropagation(contextID);
}
#endif

} // namespace CyberKit
