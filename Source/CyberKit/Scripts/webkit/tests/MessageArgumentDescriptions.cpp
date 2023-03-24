/*
 * Copyright (C) 2021 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "MessageArgumentDescriptions.h"

#if ENABLE(IPC_TESTING_API) || !LOG_DISABLED

#include "AudioMediaStreamTrackRendererInternalUnitIdentifier.h"
#include "ContentWorldShared.h"
#include "DataTaskIdentifier.h"
#include "DownloadID.h"
#include "GeolocationIdentifier.h"
#include "GraphicsContextGLIdentifier.h"
#include "IPCConnectionTesterIdentifier.h"
#include "IPCStreamTesterIdentifier.h"
#include "IdentifierTypes.h"
#include "JSIPCBinding.h"
#include "LegacyCustomProtocolID.h"
#include "LibWebRTCResolverIdentifier.h"
#include "MDNSRegisterIdentifier.h"
#include "MarkSurfacesAsVolatileRequestIdentifier.h"
#include "MediaRecorderIdentifier.h"
#include "NetworkResourceLoadIdentifier.h"
#include "PDFPluginIdentifier.h"
#include "PlaybackSessionContextIdentifier.h"
#include "QuotaIncreaseRequestIdentifier.h"
#include "RemoteAudioDestinationIdentifier.h"
#include "RemoteAudioHardwareListenerIdentifier.h"
#include "RemoteAudioSessionIdentifier.h"
#if ENABLE(GPU_PROCESS) && ENABLE(ENCRYPTED_MEDIA)
#include "RemoteCDMIdentifier.h"
#endif
#if ENABLE(GPU_PROCESS) && ENABLE(ENCRYPTED_MEDIA)
#include "RemoteCDMInstanceIdentifier.h"
#endif
#if ENABLE(GPU_PROCESS) && ENABLE(ENCRYPTED_MEDIA)
#include "RemoteCDMInstanceSessionIdentifier.h"
#endif
#if ENABLE(GPU_PROCESS) && ENABLE(LEGACY_ENCRYPTED_MEDIA)
#include "RemoteLegacyCDMIdentifier.h"
#endif
#if ENABLE(GPU_PROCESS) && ENABLE(LEGACY_ENCRYPTED_MEDIA)
#include "RemoteLegacyCDMSessionIdentifier.h"
#endif
#include "RemoteMediaResourceIdentifier.h"
#include "RemoteRemoteCommandListenerIdentifier.h"
#include "RemoteSerializedImageBufferIdentifier.h"
#include "RemoteVideoFrameIdentifier.h"
#include "RenderingBackendIdentifier.h"
#include "RenderingUpdateID.h"
#include "RetrieveRecordResponseBodyCallbackIdentifier.h"
#include "SampleBufferDisplayLayerIdentifier.h"
#include "StorageAreaIdentifier.h"
#include "StorageAreaImplIdentifier.h"
#include "StorageAreaMapIdentifier.h"
#include "StorageNamespaceIdentifier.h"
#include "TrackPrivateRemoteIdentifier.h"
#include "TransactionID.h"
#include "UserContentControllerIdentifier.h"
#include "VideoDecoderIdentifier.h"
#include "VideoEncoderIdentifier.h"
#include "WebExtensionContextIdentifier.h"
#include "WebExtensionControllerIdentifier.h"
#include "WebGPUIdentifier.h"
#include "WebPageProxyIdentifier.h"
#include "WebURLSchemeHandlerIdentifier.h"
#include <CyberCore/BackgroundFetchRecordIdentifier.h>
#include <CyberCore/BroadcastChannelIdentifier.h>
#include <CyberCore/DOMCacheIdentifier.h>
#include <CyberCore/DictationContext.h>
#include <CyberCore/DisplayList.h>
#include <CyberCore/ElementIdentifier.h>
#include <CyberCore/FetchIdentifier.h>
#include <CyberCore/FileSystemHandleIdentifier.h>
#include <CyberCore/FileSystemSyncAccessHandleIdentifier.h>
#include <CyberCore/ImageDecoderIdentifier.h>
#include <CyberCore/LayerHostingContextIdentifier.h>
#include <CyberCore/LibWebRTCSocketIdentifier.h>
#include <CyberCore/MediaKeySystemRequestIdentifier.h>
#include <CyberCore/MediaPlayerIdentifier.h>
#include <CyberCore/MediaSessionIdentifier.h>
#include <CyberCore/PageIdentifier.h>
#include <CyberCore/PlaybackTargetClientContextIdentifier.h>
#include <CyberCore/PortIdentifier.h>
#include <CyberCore/ProcessIdentifier.h>
#include <CyberCore/PushSubscriptionIdentifier.h>
#include <CyberCore/RTCDataChannelLocalIdentifier.h>
#include <CyberCore/RealtimeMediaSourceIdentifier.h>
#include <CyberCore/RenderingResourceIdentifier.h>
#include <CyberCore/ResourceLoaderIdentifier.h>
#include <CyberCore/SecurityOriginData.h>
#include <CyberCore/ServiceWorkerIdentifier.h>
#include <CyberCore/ServiceWorkerTypes.h>
#include <CyberCore/SharedWorkerIdentifier.h>
#include <CyberCore/SleepDisablerIdentifier.h>
#include <CyberCore/SpeechRecognitionConnectionClientIdentifier.h>
#include <CyberCore/TextCheckingRequestIdentifier.h>
#include <CyberCore/TextManipulationItemIdentifier.h>
#include <CyberCore/TextManipulationToken.h>
#include <CyberCore/UserMediaRequestIdentifier.h>
#include <CyberCore/WebSocketIdentifier.h>
#include "TestWithSuperclassMessages.h" // NOLINT
#if (ENABLE(WEBKIT2) && (NESTED_MASTER_CONDITION || MASTER_OR && MASTER_AND))
#include "TestWithLegacyReceiverMessages.h" // NOLINT
#endif
#if (ENABLE(WEBKIT2) && (NESTED_MASTER_CONDITION || MASTER_OR && MASTER_AND))
#include "TestWithoutAttributesMessages.h" // NOLINT
#endif
#include "TestWithIfMessageMessages.h" // NOLINT
#include "TestWithSemaphoreMessages.h" // NOLINT
#include "TestWithImageDataMessages.h" // NOLINT
#include "TestWithStreamMessages.h" // NOLINT
#include "TestWithStreamBatchedMessages.h" // NOLINT
#include "TestWithStreamBufferMessages.h" // NOLINT
#include "TestWithCVPixelBufferMessages.h" // NOLINT

namespace IPC {

#if ENABLE(IPC_TESTING_API)

std::optional<JSC::JSValue> jsValueForArguments(JSC::JSGlobalObject* globalObject, MessageName name, Decoder& decoder)
{
    switch (name) {
    case MessageName::TestWithSuperclass_LoadURL:
        return jsValueForDecodedMessage<MessageName::TestWithSuperclass_LoadURL>(globalObject, decoder);
#if ENABLE(TEST_FEATURE)
    case MessageName::TestWithSuperclass_TestAsyncMessage:
        return jsValueForDecodedMessage<MessageName::TestWithSuperclass_TestAsyncMessage>(globalObject, decoder);
    case MessageName::TestWithSuperclass_TestAsyncMessageWithNoArguments:
        return jsValueForDecodedMessage<MessageName::TestWithSuperclass_TestAsyncMessageWithNoArguments>(globalObject, decoder);
    case MessageName::TestWithSuperclass_TestAsyncMessageWithMultipleArguments:
        return jsValueForDecodedMessage<MessageName::TestWithSuperclass_TestAsyncMessageWithMultipleArguments>(globalObject, decoder);
    case MessageName::TestWithSuperclass_TestAsyncMessageWithConnection:
        return jsValueForDecodedMessage<MessageName::TestWithSuperclass_TestAsyncMessageWithConnection>(globalObject, decoder);
#endif
    case MessageName::TestWithSuperclass_TestSyncMessage:
        return jsValueForDecodedMessage<MessageName::TestWithSuperclass_TestSyncMessage>(globalObject, decoder);
    case MessageName::TestWithSuperclass_TestSynchronousMessage:
        return jsValueForDecodedMessage<MessageName::TestWithSuperclass_TestSynchronousMessage>(globalObject, decoder);
#if (ENABLE(WEBKIT2) && (NESTED_MASTER_CONDITION || MASTER_OR && MASTER_AND))
    case MessageName::TestWithLegacyReceiver_LoadURL:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_LoadURL>(globalObject, decoder);
#if ENABLE(TOUCH_EVENTS)
    case MessageName::TestWithLegacyReceiver_LoadSomething:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_LoadSomething>(globalObject, decoder);
#endif
#if (ENABLE(TOUCH_EVENTS) && (NESTED_MESSAGE_CONDITION || SOME_OTHER_MESSAGE_CONDITION))
    case MessageName::TestWithLegacyReceiver_TouchEvent:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_TouchEvent>(globalObject, decoder);
#endif
#if (ENABLE(TOUCH_EVENTS) && (NESTED_MESSAGE_CONDITION && SOME_OTHER_MESSAGE_CONDITION))
    case MessageName::TestWithLegacyReceiver_AddEvent:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_AddEvent>(globalObject, decoder);
#endif
#if ENABLE(TOUCH_EVENTS)
    case MessageName::TestWithLegacyReceiver_LoadSomethingElse:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_LoadSomethingElse>(globalObject, decoder);
#endif
    case MessageName::TestWithLegacyReceiver_DidReceivePolicyDecision:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_DidReceivePolicyDecision>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_Close:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_Close>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_PreferencesDidChange:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_PreferencesDidChange>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_SendDoubleAndFloat:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_SendDoubleAndFloat>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_SendInts:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_SendInts>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_CreatePlugin:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_CreatePlugin>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_RunJavaScriptAlert:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_RunJavaScriptAlert>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_GetPlugins:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_GetPlugins>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_GetPluginProcessConnection:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_GetPluginProcessConnection>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_TestMultipleAttributes:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_TestMultipleAttributes>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_TestParameterAttributes:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_TestParameterAttributes>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_TemplateTest:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_TemplateTest>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_SetVideoLayerID:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_SetVideoLayerID>(globalObject, decoder);
#if PLATFORM(MAC)
    case MessageName::TestWithLegacyReceiver_DidCreateWebProcessConnection:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_DidCreateWebProcessConnection>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_InterpretKeyEvent:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_InterpretKeyEvent>(globalObject, decoder);
#endif
#if ENABLE(DEPRECATED_FEATURE)
    case MessageName::TestWithLegacyReceiver_DeprecatedOperation:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_DeprecatedOperation>(globalObject, decoder);
#endif
#if ENABLE(FEATURE_FOR_TESTING)
    case MessageName::TestWithLegacyReceiver_ExperimentalOperation:
        return jsValueForDecodedMessage<MessageName::TestWithLegacyReceiver_ExperimentalOperation>(globalObject, decoder);
#endif
#endif
#if (ENABLE(WEBKIT2) && (NESTED_MASTER_CONDITION || MASTER_OR && MASTER_AND))
    case MessageName::TestWithoutAttributes_LoadURL:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_LoadURL>(globalObject, decoder);
#if ENABLE(TOUCH_EVENTS)
    case MessageName::TestWithoutAttributes_LoadSomething:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_LoadSomething>(globalObject, decoder);
#endif
#if (ENABLE(TOUCH_EVENTS) && (NESTED_MESSAGE_CONDITION || SOME_OTHER_MESSAGE_CONDITION))
    case MessageName::TestWithoutAttributes_TouchEvent:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_TouchEvent>(globalObject, decoder);
#endif
#if (ENABLE(TOUCH_EVENTS) && (NESTED_MESSAGE_CONDITION && SOME_OTHER_MESSAGE_CONDITION))
    case MessageName::TestWithoutAttributes_AddEvent:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_AddEvent>(globalObject, decoder);
#endif
#if ENABLE(TOUCH_EVENTS)
    case MessageName::TestWithoutAttributes_LoadSomethingElse:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_LoadSomethingElse>(globalObject, decoder);
#endif
    case MessageName::TestWithoutAttributes_DidReceivePolicyDecision:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_DidReceivePolicyDecision>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_Close:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_Close>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_PreferencesDidChange:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_PreferencesDidChange>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_SendDoubleAndFloat:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_SendDoubleAndFloat>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_SendInts:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_SendInts>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_CreatePlugin:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_CreatePlugin>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_RunJavaScriptAlert:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_RunJavaScriptAlert>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_GetPlugins:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_GetPlugins>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_GetPluginProcessConnection:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_GetPluginProcessConnection>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_TestMultipleAttributes:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_TestMultipleAttributes>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_TestParameterAttributes:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_TestParameterAttributes>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_TemplateTest:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_TemplateTest>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_SetVideoLayerID:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_SetVideoLayerID>(globalObject, decoder);
#if PLATFORM(MAC)
    case MessageName::TestWithoutAttributes_DidCreateWebProcessConnection:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_DidCreateWebProcessConnection>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_InterpretKeyEvent:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_InterpretKeyEvent>(globalObject, decoder);
#endif
#if ENABLE(DEPRECATED_FEATURE)
    case MessageName::TestWithoutAttributes_DeprecatedOperation:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_DeprecatedOperation>(globalObject, decoder);
#endif
#if ENABLE(FEATURE_FOR_TESTING)
    case MessageName::TestWithoutAttributes_ExperimentalOperation:
        return jsValueForDecodedMessage<MessageName::TestWithoutAttributes_ExperimentalOperation>(globalObject, decoder);
#endif
#endif
#if PLATFORM(COCOA)
    case MessageName::TestWithIfMessage_LoadURL:
        return jsValueForDecodedMessage<MessageName::TestWithIfMessage_LoadURL>(globalObject, decoder);
#endif
#if PLATFORM(GTK)
    case MessageName::TestWithIfMessage_LoadURL:
        return jsValueForDecodedMessage<MessageName::TestWithIfMessage_LoadURL>(globalObject, decoder);
#endif
    case MessageName::TestWithSemaphore_SendSemaphore:
        return jsValueForDecodedMessage<MessageName::TestWithSemaphore_SendSemaphore>(globalObject, decoder);
    case MessageName::TestWithSemaphore_ReceiveSemaphore:
        return jsValueForDecodedMessage<MessageName::TestWithSemaphore_ReceiveSemaphore>(globalObject, decoder);
    case MessageName::TestWithImageData_SendImageData:
        return jsValueForDecodedMessage<MessageName::TestWithImageData_SendImageData>(globalObject, decoder);
    case MessageName::TestWithImageData_ReceiveImageData:
        return jsValueForDecodedMessage<MessageName::TestWithImageData_ReceiveImageData>(globalObject, decoder);
    case MessageName::TestWithStream_SendString:
        return jsValueForDecodedMessage<MessageName::TestWithStream_SendString>(globalObject, decoder);
    case MessageName::TestWithStream_SendStringAsync:
        return jsValueForDecodedMessage<MessageName::TestWithStream_SendStringAsync>(globalObject, decoder);
    case MessageName::TestWithStream_SendStringSync:
        return jsValueForDecodedMessage<MessageName::TestWithStream_SendStringSync>(globalObject, decoder);
    case MessageName::TestWithStream_CallWithIdentifier:
        return jsValueForDecodedMessage<MessageName::TestWithStream_CallWithIdentifier>(globalObject, decoder);
#if PLATFORM(COCOA)
    case MessageName::TestWithStream_SendMachSendRight:
        return jsValueForDecodedMessage<MessageName::TestWithStream_SendMachSendRight>(globalObject, decoder);
    case MessageName::TestWithStream_ReceiveMachSendRight:
        return jsValueForDecodedMessage<MessageName::TestWithStream_ReceiveMachSendRight>(globalObject, decoder);
    case MessageName::TestWithStream_SendAndReceiveMachSendRight:
        return jsValueForDecodedMessage<MessageName::TestWithStream_SendAndReceiveMachSendRight>(globalObject, decoder);
#endif
    case MessageName::TestWithStreamBatched_SendString:
        return jsValueForDecodedMessage<MessageName::TestWithStreamBatched_SendString>(globalObject, decoder);
    case MessageName::TestWithStreamBuffer_SendStreamBuffer:
        return jsValueForDecodedMessage<MessageName::TestWithStreamBuffer_SendStreamBuffer>(globalObject, decoder);
#if USE(AVFOUNDATION)
    case MessageName::TestWithCVPixelBuffer_SendCVPixelBuffer:
        return jsValueForDecodedMessage<MessageName::TestWithCVPixelBuffer_SendCVPixelBuffer>(globalObject, decoder);
    case MessageName::TestWithCVPixelBuffer_ReceiveCVPixelBuffer:
        return jsValueForDecodedMessage<MessageName::TestWithCVPixelBuffer_ReceiveCVPixelBuffer>(globalObject, decoder);
#endif
    default:
        break;
    }
    return std::nullopt;
}

std::optional<JSC::JSValue> jsValueForReplyArguments(JSC::JSGlobalObject* globalObject, MessageName name, Decoder& decoder)
{
    switch (name) {
#if ENABLE(TEST_FEATURE)
    case MessageName::TestWithSuperclass_TestAsyncMessage:
        return jsValueForDecodedMessageReply<MessageName::TestWithSuperclass_TestAsyncMessage>(globalObject, decoder);
    case MessageName::TestWithSuperclass_TestAsyncMessageWithNoArguments:
        return jsValueForDecodedMessageReply<MessageName::TestWithSuperclass_TestAsyncMessageWithNoArguments>(globalObject, decoder);
    case MessageName::TestWithSuperclass_TestAsyncMessageWithMultipleArguments:
        return jsValueForDecodedMessageReply<MessageName::TestWithSuperclass_TestAsyncMessageWithMultipleArguments>(globalObject, decoder);
    case MessageName::TestWithSuperclass_TestAsyncMessageWithConnection:
        return jsValueForDecodedMessageReply<MessageName::TestWithSuperclass_TestAsyncMessageWithConnection>(globalObject, decoder);
#endif
    case MessageName::TestWithSuperclass_TestSyncMessage:
        return jsValueForDecodedMessageReply<MessageName::TestWithSuperclass_TestSyncMessage>(globalObject, decoder);
    case MessageName::TestWithSuperclass_TestSynchronousMessage:
        return jsValueForDecodedMessageReply<MessageName::TestWithSuperclass_TestSynchronousMessage>(globalObject, decoder);
#if (ENABLE(WEBKIT2) && (NESTED_MASTER_CONDITION || MASTER_OR && MASTER_AND))
    case MessageName::TestWithLegacyReceiver_CreatePlugin:
        return jsValueForDecodedMessageReply<MessageName::TestWithLegacyReceiver_CreatePlugin>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_RunJavaScriptAlert:
        return jsValueForDecodedMessageReply<MessageName::TestWithLegacyReceiver_RunJavaScriptAlert>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_GetPlugins:
        return jsValueForDecodedMessageReply<MessageName::TestWithLegacyReceiver_GetPlugins>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_GetPluginProcessConnection:
        return jsValueForDecodedMessageReply<MessageName::TestWithLegacyReceiver_GetPluginProcessConnection>(globalObject, decoder);
    case MessageName::TestWithLegacyReceiver_TestMultipleAttributes:
        return jsValueForDecodedMessageReply<MessageName::TestWithLegacyReceiver_TestMultipleAttributes>(globalObject, decoder);
#if PLATFORM(MAC)
    case MessageName::TestWithLegacyReceiver_InterpretKeyEvent:
        return jsValueForDecodedMessageReply<MessageName::TestWithLegacyReceiver_InterpretKeyEvent>(globalObject, decoder);
#endif
#endif
#if (ENABLE(WEBKIT2) && (NESTED_MASTER_CONDITION || MASTER_OR && MASTER_AND))
    case MessageName::TestWithoutAttributes_CreatePlugin:
        return jsValueForDecodedMessageReply<MessageName::TestWithoutAttributes_CreatePlugin>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_RunJavaScriptAlert:
        return jsValueForDecodedMessageReply<MessageName::TestWithoutAttributes_RunJavaScriptAlert>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_GetPlugins:
        return jsValueForDecodedMessageReply<MessageName::TestWithoutAttributes_GetPlugins>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_GetPluginProcessConnection:
        return jsValueForDecodedMessageReply<MessageName::TestWithoutAttributes_GetPluginProcessConnection>(globalObject, decoder);
    case MessageName::TestWithoutAttributes_TestMultipleAttributes:
        return jsValueForDecodedMessageReply<MessageName::TestWithoutAttributes_TestMultipleAttributes>(globalObject, decoder);
#if PLATFORM(MAC)
    case MessageName::TestWithoutAttributes_InterpretKeyEvent:
        return jsValueForDecodedMessageReply<MessageName::TestWithoutAttributes_InterpretKeyEvent>(globalObject, decoder);
#endif
#endif
    case MessageName::TestWithSemaphore_ReceiveSemaphore:
        return jsValueForDecodedMessageReply<MessageName::TestWithSemaphore_ReceiveSemaphore>(globalObject, decoder);
    case MessageName::TestWithImageData_ReceiveImageData:
        return jsValueForDecodedMessageReply<MessageName::TestWithImageData_ReceiveImageData>(globalObject, decoder);
    case MessageName::TestWithStream_SendStringAsync:
        return jsValueForDecodedMessageReply<MessageName::TestWithStream_SendStringAsync>(globalObject, decoder);
    case MessageName::TestWithStream_SendStringSync:
        return jsValueForDecodedMessageReply<MessageName::TestWithStream_SendStringSync>(globalObject, decoder);
    case MessageName::TestWithStream_CallWithIdentifier:
        return jsValueForDecodedMessageReply<MessageName::TestWithStream_CallWithIdentifier>(globalObject, decoder);
#if PLATFORM(COCOA)
    case MessageName::TestWithStream_ReceiveMachSendRight:
        return jsValueForDecodedMessageReply<MessageName::TestWithStream_ReceiveMachSendRight>(globalObject, decoder);
    case MessageName::TestWithStream_SendAndReceiveMachSendRight:
        return jsValueForDecodedMessageReply<MessageName::TestWithStream_SendAndReceiveMachSendRight>(globalObject, decoder);
#endif
#if USE(AVFOUNDATION)
    case MessageName::TestWithCVPixelBuffer_ReceiveCVPixelBuffer:
        return jsValueForDecodedMessageReply<MessageName::TestWithCVPixelBuffer_ReceiveCVPixelBuffer>(globalObject, decoder);
#endif
    default:
        break;
    }
    return std::nullopt;
}

Vector<ASCIILiteral> serializedIdentifiers()
{
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::BackgroundFetchRecordIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::BroadcastChannelIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::DOMCacheIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::DictationContext));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::DisplayList::ItemBufferIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::ElementIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::FetchIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::FileSystemHandleIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::FileSystemSyncAccessHandleIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::ImageDecoderIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::LayerHostingContextIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::LibWebRTCSocketIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::MediaKeySystemRequestIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::MediaPlayerIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::MediaSessionIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::OpaqueOriginIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::PageIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::PlaybackTargetClientContextIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::PushSubscriptionIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::PortIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::ProcessIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::RealtimeMediaSourceIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::RenderingResourceIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::ResourceLoaderIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::RTCDataChannelLocalIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::SWServerConnectionIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::ServiceWorkerIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::ServiceWorkerJobIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::ServiceWorkerRegistrationIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::SharedWorkerIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::SleepDisablerIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::SpeechRecognitionConnectionClientIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::TextCheckingRequestIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::TextManipulationItemIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::TextManipulationTokenIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::UserMediaRequestIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberCore::WebSocketIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::AudioMediaStreamTrackRendererInternalUnitIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::AuthenticationChallengeIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::ContentWorldIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::DataTaskIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::DownloadID));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::FormSubmitListenerIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::GeolocationIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::GraphicsContextGLIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::IPCConnectionTesterIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::IPCStreamTesterIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::LegacyCustomProtocolID));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::LibWebRTCResolverIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::MDNSRegisterIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::MarkSurfacesAsVolatileRequestIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::MediaRecorderIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::NetworkResourceLoadIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::PDFPluginIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::PageGroupIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::PlaybackSessionContextIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::QuotaIncreaseRequestIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::RemoteAudioDestinationIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::RemoteAudioHardwareListenerIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::RemoteAudioSessionIdentifier));
#if ENABLE(GPU_PROCESS) && ENABLE(ENCRYPTED_MEDIA)
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::RemoteCDMIdentifier));
#endif
#if ENABLE(GPU_PROCESS) && ENABLE(ENCRYPTED_MEDIA)
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::RemoteCDMInstanceIdentifier));
#endif
#if ENABLE(GPU_PROCESS) && ENABLE(ENCRYPTED_MEDIA)
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::RemoteCDMInstanceSessionIdentifier));
#endif
#if ENABLE(GPU_PROCESS) && ENABLE(LEGACY_ENCRYPTED_MEDIA)
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::RemoteLegacyCDMIdentifier));
#endif
#if ENABLE(GPU_PROCESS) && ENABLE(LEGACY_ENCRYPTED_MEDIA)
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::RemoteLegacyCDMSessionIdentifier));
#endif
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::RemoteMediaResourceIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::RemoteSerializedImageBufferIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::RemoteVideoFrameIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::RemoteRemoteCommandListenerIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::RenderingBackendIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::RenderingUpdateID));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::RetrieveRecordResponseBodyCallbackIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::SampleBufferDisplayLayerIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::StorageAreaIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::StorageAreaImplIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::StorageAreaMapIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::StorageNamespaceIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::TapIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::TrackPrivateRemoteIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::TransactionID));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::UserContentControllerIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::VideoDecoderIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::VideoEncoderIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::WebExtensionContextIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::WebExtensionControllerIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::WebGPUIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::WebPageProxyIdentifier));
    static_assert(sizeof(uint64_t) == sizeof(CyberKit::WebURLSchemeHandlerIdentifier));
    return {
        "CyberCore::BackgroundFetchRecordIdentifier"_s,
        "CyberCore::BroadcastChannelIdentifier"_s,
        "CyberCore::DOMCacheIdentifier"_s,
        "CyberCore::DictationContext"_s,
        "CyberCore::DisplayList::ItemBufferIdentifier"_s,
        "CyberCore::ElementIdentifier"_s,
        "CyberCore::FetchIdentifier"_s,
        "CyberCore::FileSystemHandleIdentifier"_s,
        "CyberCore::FileSystemSyncAccessHandleIdentifier"_s,
        "CyberCore::ImageDecoderIdentifier"_s,
        "CyberCore::LayerHostingContextIdentifier"_s,
        "CyberCore::LibWebRTCSocketIdentifier"_s,
        "CyberCore::MediaKeySystemRequestIdentifier"_s,
        "CyberCore::MediaPlayerIdentifier"_s,
        "CyberCore::MediaSessionIdentifier"_s,
        "CyberCore::OpaqueOriginIdentifier"_s,
        "CyberCore::PageIdentifier"_s,
        "CyberCore::PlaybackTargetClientContextIdentifier"_s,
        "CyberCore::PushSubscriptionIdentifier"_s,
        "CyberCore::PortIdentifier"_s,
        "CyberCore::ProcessIdentifier"_s,
        "CyberCore::RealtimeMediaSourceIdentifier"_s,
        "CyberCore::RenderingResourceIdentifier"_s,
        "CyberCore::ResourceLoaderIdentifier"_s,
        "CyberCore::RTCDataChannelLocalIdentifier"_s,
        "CyberCore::SWServerConnectionIdentifier"_s,
        "CyberCore::ServiceWorkerIdentifier"_s,
        "CyberCore::ServiceWorkerJobIdentifier"_s,
        "CyberCore::ServiceWorkerRegistrationIdentifier"_s,
        "CyberCore::SharedWorkerIdentifier"_s,
        "CyberCore::SleepDisablerIdentifier"_s,
        "CyberCore::SpeechRecognitionConnectionClientIdentifier"_s,
        "CyberCore::TextCheckingRequestIdentifier"_s,
        "CyberCore::TextManipulationItemIdentifier"_s,
        "CyberCore::TextManipulationTokenIdentifier"_s,
        "CyberCore::UserMediaRequestIdentifier"_s,
        "CyberCore::WebSocketIdentifier"_s,
        "CyberKit::AudioMediaStreamTrackRendererInternalUnitIdentifier"_s,
        "CyberKit::AuthenticationChallengeIdentifier"_s,
        "CyberKit::ContentWorldIdentifier"_s,
        "CyberKit::DataTaskIdentifier"_s,
        "CyberKit::DownloadID"_s,
        "CyberKit::FormSubmitListenerIdentifier"_s,
        "CyberKit::GeolocationIdentifier"_s,
        "CyberKit::GraphicsContextGLIdentifier"_s,
        "CyberKit::IPCConnectionTesterIdentifier"_s,
        "CyberKit::IPCStreamTesterIdentifier"_s,
        "CyberKit::LegacyCustomProtocolID"_s,
        "CyberKit::LibWebRTCResolverIdentifier"_s,
        "CyberKit::MDNSRegisterIdentifier"_s,
        "CyberKit::MarkSurfacesAsVolatileRequestIdentifier"_s,
        "CyberKit::MediaRecorderIdentifier"_s,
        "CyberKit::NetworkResourceLoadIdentifier"_s,
        "CyberKit::PDFPluginIdentifier"_s,
        "CyberKit::PageGroupIdentifier"_s,
        "CyberKit::PlaybackSessionContextIdentifier"_s,
        "CyberKit::QuotaIncreaseRequestIdentifier"_s,
        "CyberKit::RemoteAudioDestinationIdentifier"_s,
        "CyberKit::RemoteAudioHardwareListenerIdentifier"_s,
        "CyberKit::RemoteAudioSessionIdentifier"_s,
#if ENABLE(GPU_PROCESS) && ENABLE(ENCRYPTED_MEDIA)
        "CyberKit::RemoteCDMIdentifier"_s,
#endif
#if ENABLE(GPU_PROCESS) && ENABLE(ENCRYPTED_MEDIA)
        "CyberKit::RemoteCDMInstanceIdentifier"_s,
#endif
#if ENABLE(GPU_PROCESS) && ENABLE(ENCRYPTED_MEDIA)
        "CyberKit::RemoteCDMInstanceSessionIdentifier"_s,
#endif
#if ENABLE(GPU_PROCESS) && ENABLE(LEGACY_ENCRYPTED_MEDIA)
        "CyberKit::RemoteLegacyCDMIdentifier"_s,
#endif
#if ENABLE(GPU_PROCESS) && ENABLE(LEGACY_ENCRYPTED_MEDIA)
        "CyberKit::RemoteLegacyCDMSessionIdentifier"_s,
#endif
        "CyberKit::RemoteMediaResourceIdentifier"_s,
        "CyberKit::RemoteSerializedImageBufferIdentifier"_s,
        "CyberKit::RemoteVideoFrameIdentifier"_s,
        "CyberKit::RemoteRemoteCommandListenerIdentifier"_s,
        "CyberKit::RenderingBackendIdentifier"_s,
        "CyberKit::RenderingUpdateID"_s,
        "CyberKit::RetrieveRecordResponseBodyCallbackIdentifier"_s,
        "CyberKit::SampleBufferDisplayLayerIdentifier"_s,
        "CyberKit::StorageAreaIdentifier"_s,
        "CyberKit::StorageAreaImplIdentifier"_s,
        "CyberKit::StorageAreaMapIdentifier"_s,
        "CyberKit::StorageNamespaceIdentifier"_s,
        "CyberKit::TapIdentifier"_s,
        "CyberKit::TrackPrivateRemoteIdentifier"_s,
        "CyberKit::TransactionID"_s,
        "CyberKit::UserContentControllerIdentifier"_s,
        "CyberKit::VideoDecoderIdentifier"_s,
        "CyberKit::VideoEncoderIdentifier"_s,
        "CyberKit::WebExtensionContextIdentifier"_s,
        "CyberKit::WebExtensionControllerIdentifier"_s,
        "CyberKit::WebGPUIdentifier"_s,
        "CyberKit::WebPageProxyIdentifier"_s,
        "CyberKit::WebURLSchemeHandlerIdentifier"_s,
    };
}

#endif // ENABLE(IPC_TESTING_API)

std::optional<Vector<ArgumentDescription>> messageArgumentDescriptions(MessageName name)
{
    switch (name) {
    case MessageName::TestWithSuperclass_LoadURL:
        return Vector<ArgumentDescription> {
            { "url", "String", nullptr, false },
        };
#if ENABLE(TEST_FEATURE)
    case MessageName::TestWithSuperclass_TestAsyncMessage:
        return Vector<ArgumentDescription> {
            { "twoStateEnum", "bool", "CyberKit::TestTwoStateEnum", false },
        };
    case MessageName::TestWithSuperclass_TestAsyncMessageWithNoArguments:
        return Vector<ArgumentDescription> { };
    case MessageName::TestWithSuperclass_TestAsyncMessageWithMultipleArguments:
        return Vector<ArgumentDescription> { };
    case MessageName::TestWithSuperclass_TestAsyncMessageWithConnection:
        return Vector<ArgumentDescription> {
            { "value", "int", nullptr, false },
        };
#endif
    case MessageName::TestWithSuperclass_TestSyncMessage:
        return Vector<ArgumentDescription> {
            { "param", "uint32_t", nullptr, false },
        };
    case MessageName::TestWithSuperclass_TestSynchronousMessage:
        return Vector<ArgumentDescription> {
            { "value", "bool", nullptr, false },
        };
#if (ENABLE(WEBKIT2) && (NESTED_MASTER_CONDITION || MASTER_OR && MASTER_AND))
    case MessageName::TestWithLegacyReceiver_LoadURL:
        return Vector<ArgumentDescription> {
            { "url", "String", nullptr, false },
        };
#if ENABLE(TOUCH_EVENTS)
    case MessageName::TestWithLegacyReceiver_LoadSomething:
        return Vector<ArgumentDescription> {
            { "url", "String", nullptr, false },
        };
#endif
#if (ENABLE(TOUCH_EVENTS) && (NESTED_MESSAGE_CONDITION || SOME_OTHER_MESSAGE_CONDITION))
    case MessageName::TestWithLegacyReceiver_TouchEvent:
        return Vector<ArgumentDescription> {
            { "event", "CyberKit::WebTouchEvent", nullptr, false },
        };
#endif
#if (ENABLE(TOUCH_EVENTS) && (NESTED_MESSAGE_CONDITION && SOME_OTHER_MESSAGE_CONDITION))
    case MessageName::TestWithLegacyReceiver_AddEvent:
        return Vector<ArgumentDescription> {
            { "event", "CyberKit::WebTouchEvent", nullptr, false },
        };
#endif
#if ENABLE(TOUCH_EVENTS)
    case MessageName::TestWithLegacyReceiver_LoadSomethingElse:
        return Vector<ArgumentDescription> {
            { "url", "String", nullptr, false },
        };
#endif
    case MessageName::TestWithLegacyReceiver_DidReceivePolicyDecision:
        return Vector<ArgumentDescription> {
            { "frameID", "uint64_t", nullptr, false },
            { "listenerID", "uint64_t", nullptr, false },
            { "policyAction", "uint32_t", nullptr, false },
        };
    case MessageName::TestWithLegacyReceiver_Close:
        return Vector<ArgumentDescription> { };
    case MessageName::TestWithLegacyReceiver_PreferencesDidChange:
        return Vector<ArgumentDescription> {
            { "store", "CyberKit::WebPreferencesStore", nullptr, false },
        };
    case MessageName::TestWithLegacyReceiver_SendDoubleAndFloat:
        return Vector<ArgumentDescription> {
            { "d", "double", nullptr, false },
            { "f", "float", nullptr, false },
        };
    case MessageName::TestWithLegacyReceiver_SendInts:
        return Vector<ArgumentDescription> {
            { "ints", "Vector<uint64_t>", nullptr, false },
            { "intVectors", "Vector<Vector<uint64_t>>", nullptr, false },
        };
    case MessageName::TestWithLegacyReceiver_CreatePlugin:
        return Vector<ArgumentDescription> {
            { "pluginInstanceID", "uint64_t", nullptr, false },
            { "parameters", "CyberKit::Plugin::Parameters", nullptr, false },
        };
    case MessageName::TestWithLegacyReceiver_RunJavaScriptAlert:
        return Vector<ArgumentDescription> {
            { "frameID", "uint64_t", nullptr, false },
            { "message", "String", nullptr, false },
        };
    case MessageName::TestWithLegacyReceiver_GetPlugins:
        return Vector<ArgumentDescription> {
            { "refresh", "bool", nullptr, false },
        };
    case MessageName::TestWithLegacyReceiver_GetPluginProcessConnection:
        return Vector<ArgumentDescription> {
            { "pluginPath", "String", nullptr, false },
        };
    case MessageName::TestWithLegacyReceiver_TestMultipleAttributes:
        return Vector<ArgumentDescription> { };
    case MessageName::TestWithLegacyReceiver_TestParameterAttributes:
        return Vector<ArgumentDescription> {
            { "foo", "uint64_t", nullptr, false },
            { "bar", "double", nullptr, false },
            { "baz", "double", nullptr, false },
        };
    case MessageName::TestWithLegacyReceiver_TemplateTest:
        return Vector<ArgumentDescription> {
            { "a", "HashMap<String, std::pair<String, uint64_t>>", nullptr, false },
        };
    case MessageName::TestWithLegacyReceiver_SetVideoLayerID:
        return Vector<ArgumentDescription> {
            { "videoLayerID", "CyberCore::GraphicsLayer::PlatformLayerID", nullptr, false },
        };
#if PLATFORM(MAC)
    case MessageName::TestWithLegacyReceiver_DidCreateWebProcessConnection:
        return Vector<ArgumentDescription> {
            { "connectionIdentifier", "MachSendRight", nullptr, false },
            { "flags", "OptionSet<CyberKit::SelectionFlags>", nullptr, false },
        };
    case MessageName::TestWithLegacyReceiver_InterpretKeyEvent:
        return Vector<ArgumentDescription> {
            { "type", "uint32_t", nullptr, false },
        };
#endif
#if ENABLE(DEPRECATED_FEATURE)
    case MessageName::TestWithLegacyReceiver_DeprecatedOperation:
        return Vector<ArgumentDescription> {
            { "dummy", "IPC::DummyType", nullptr, false },
        };
#endif
#if ENABLE(FEATURE_FOR_TESTING)
    case MessageName::TestWithLegacyReceiver_ExperimentalOperation:
        return Vector<ArgumentDescription> {
            { "dummy", "IPC::DummyType", nullptr, false },
        };
#endif
#endif
#if (ENABLE(WEBKIT2) && (NESTED_MASTER_CONDITION || MASTER_OR && MASTER_AND))
    case MessageName::TestWithoutAttributes_LoadURL:
        return Vector<ArgumentDescription> {
            { "url", "String", nullptr, false },
        };
#if ENABLE(TOUCH_EVENTS)
    case MessageName::TestWithoutAttributes_LoadSomething:
        return Vector<ArgumentDescription> {
            { "url", "String", nullptr, false },
        };
#endif
#if (ENABLE(TOUCH_EVENTS) && (NESTED_MESSAGE_CONDITION || SOME_OTHER_MESSAGE_CONDITION))
    case MessageName::TestWithoutAttributes_TouchEvent:
        return Vector<ArgumentDescription> {
            { "event", "CyberKit::WebTouchEvent", nullptr, false },
        };
#endif
#if (ENABLE(TOUCH_EVENTS) && (NESTED_MESSAGE_CONDITION && SOME_OTHER_MESSAGE_CONDITION))
    case MessageName::TestWithoutAttributes_AddEvent:
        return Vector<ArgumentDescription> {
            { "event", "CyberKit::WebTouchEvent", nullptr, false },
        };
#endif
#if ENABLE(TOUCH_EVENTS)
    case MessageName::TestWithoutAttributes_LoadSomethingElse:
        return Vector<ArgumentDescription> {
            { "url", "String", nullptr, false },
        };
#endif
    case MessageName::TestWithoutAttributes_DidReceivePolicyDecision:
        return Vector<ArgumentDescription> {
            { "frameID", "uint64_t", nullptr, false },
            { "listenerID", "uint64_t", nullptr, false },
            { "policyAction", "uint32_t", nullptr, false },
        };
    case MessageName::TestWithoutAttributes_Close:
        return Vector<ArgumentDescription> { };
    case MessageName::TestWithoutAttributes_PreferencesDidChange:
        return Vector<ArgumentDescription> {
            { "store", "CyberKit::WebPreferencesStore", nullptr, false },
        };
    case MessageName::TestWithoutAttributes_SendDoubleAndFloat:
        return Vector<ArgumentDescription> {
            { "d", "double", nullptr, false },
            { "f", "float", nullptr, false },
        };
    case MessageName::TestWithoutAttributes_SendInts:
        return Vector<ArgumentDescription> {
            { "ints", "Vector<uint64_t>", nullptr, false },
            { "intVectors", "Vector<Vector<uint64_t>>", nullptr, false },
        };
    case MessageName::TestWithoutAttributes_CreatePlugin:
        return Vector<ArgumentDescription> {
            { "pluginInstanceID", "uint64_t", nullptr, false },
            { "parameters", "CyberKit::Plugin::Parameters", nullptr, false },
        };
    case MessageName::TestWithoutAttributes_RunJavaScriptAlert:
        return Vector<ArgumentDescription> {
            { "frameID", "uint64_t", nullptr, false },
            { "message", "String", nullptr, false },
        };
    case MessageName::TestWithoutAttributes_GetPlugins:
        return Vector<ArgumentDescription> {
            { "refresh", "bool", nullptr, false },
        };
    case MessageName::TestWithoutAttributes_GetPluginProcessConnection:
        return Vector<ArgumentDescription> {
            { "pluginPath", "String", nullptr, false },
        };
    case MessageName::TestWithoutAttributes_TestMultipleAttributes:
        return Vector<ArgumentDescription> { };
    case MessageName::TestWithoutAttributes_TestParameterAttributes:
        return Vector<ArgumentDescription> {
            { "foo", "uint64_t", nullptr, false },
            { "bar", "double", nullptr, false },
            { "baz", "double", nullptr, false },
        };
    case MessageName::TestWithoutAttributes_TemplateTest:
        return Vector<ArgumentDescription> {
            { "a", "HashMap<String, std::pair<String, uint64_t>>", nullptr, false },
        };
    case MessageName::TestWithoutAttributes_SetVideoLayerID:
        return Vector<ArgumentDescription> {
            { "videoLayerID", "CyberCore::GraphicsLayer::PlatformLayerID", nullptr, false },
        };
#if PLATFORM(MAC)
    case MessageName::TestWithoutAttributes_DidCreateWebProcessConnection:
        return Vector<ArgumentDescription> {
            { "connectionIdentifier", "MachSendRight", nullptr, false },
            { "flags", "OptionSet<CyberKit::SelectionFlags>", nullptr, false },
        };
    case MessageName::TestWithoutAttributes_InterpretKeyEvent:
        return Vector<ArgumentDescription> {
            { "type", "uint32_t", nullptr, false },
        };
#endif
#if ENABLE(DEPRECATED_FEATURE)
    case MessageName::TestWithoutAttributes_DeprecatedOperation:
        return Vector<ArgumentDescription> {
            { "dummy", "IPC::DummyType", nullptr, false },
        };
#endif
#if ENABLE(FEATURE_FOR_TESTING)
    case MessageName::TestWithoutAttributes_ExperimentalOperation:
        return Vector<ArgumentDescription> {
            { "dummy", "IPC::DummyType", nullptr, false },
        };
#endif
#endif
#if PLATFORM(COCOA)
    case MessageName::TestWithIfMessage_LoadURL:
        return Vector<ArgumentDescription> {
            { "url", "String", nullptr, false },
        };
#endif
#if PLATFORM(GTK)
    case MessageName::TestWithIfMessage_LoadURL:
        return Vector<ArgumentDescription> {
            { "url", "String", nullptr, false },
            { "value", "int64_t", nullptr, false },
        };
#endif
    case MessageName::TestWithSemaphore_SendSemaphore:
        return Vector<ArgumentDescription> {
            { "s0", "IPC::Semaphore", nullptr, false },
        };
    case MessageName::TestWithSemaphore_ReceiveSemaphore:
        return Vector<ArgumentDescription> { };
    case MessageName::TestWithImageData_SendImageData:
        return Vector<ArgumentDescription> {
            { "s0", "RefPtr<CyberCore::ImageData>", nullptr, false },
        };
    case MessageName::TestWithImageData_ReceiveImageData:
        return Vector<ArgumentDescription> { };
    case MessageName::TestWithStream_SendString:
        return Vector<ArgumentDescription> {
            { "url", "String", nullptr, false },
        };
    case MessageName::TestWithStream_SendStringAsync:
        return Vector<ArgumentDescription> {
            { "url", "String", nullptr, false },
        };
    case MessageName::TestWithStream_SendStringSync:
        return Vector<ArgumentDescription> {
            { "url", "String", nullptr, false },
        };
    case MessageName::TestWithStream_CallWithIdentifier:
        return Vector<ArgumentDescription> { };
#if PLATFORM(COCOA)
    case MessageName::TestWithStream_SendMachSendRight:
        return Vector<ArgumentDescription> {
            { "a1", "MachSendRight", nullptr, false },
        };
    case MessageName::TestWithStream_ReceiveMachSendRight:
        return Vector<ArgumentDescription> { };
    case MessageName::TestWithStream_SendAndReceiveMachSendRight:
        return Vector<ArgumentDescription> {
            { "a1", "MachSendRight", nullptr, false },
        };
#endif
    case MessageName::TestWithStreamBatched_SendString:
        return Vector<ArgumentDescription> {
            { "url", "String", nullptr, false },
        };
    case MessageName::TestWithStreamBuffer_SendStreamBuffer:
        return Vector<ArgumentDescription> {
            { "stream", "IPC::StreamConnectionBuffer", nullptr, false },
        };
#if USE(AVFOUNDATION)
    case MessageName::TestWithCVPixelBuffer_SendCVPixelBuffer:
        return Vector<ArgumentDescription> {
            { "s0", "RetainPtr<CVPixelBufferRef>", nullptr, false },
        };
    case MessageName::TestWithCVPixelBuffer_ReceiveCVPixelBuffer:
        return Vector<ArgumentDescription> { };
#endif
    default:
        break;
    }
    return std::nullopt;
}

std::optional<Vector<ArgumentDescription>> messageReplyArgumentDescriptions(MessageName name)
{
    switch (name) {
#if ENABLE(TEST_FEATURE)
    case MessageName::TestWithSuperclass_TestAsyncMessage:
        return Vector<ArgumentDescription> {
            { "result", "uint64_t", nullptr, false },
        };
    case MessageName::TestWithSuperclass_TestAsyncMessageWithNoArguments:
        return Vector<ArgumentDescription> { };
    case MessageName::TestWithSuperclass_TestAsyncMessageWithMultipleArguments:
        return Vector<ArgumentDescription> {
            { "flag", "bool", nullptr, false },
            { "value", "uint64_t", nullptr, false },
        };
    case MessageName::TestWithSuperclass_TestAsyncMessageWithConnection:
        return Vector<ArgumentDescription> {
            { "flag", "bool", nullptr, false },
        };
#endif
    case MessageName::TestWithSuperclass_TestSyncMessage:
        return Vector<ArgumentDescription> {
            { "reply", "uint8_t", nullptr, false },
        };
    case MessageName::TestWithSuperclass_TestSynchronousMessage:
        return Vector<ArgumentDescription> {
            { "optionalReply", "CyberKit::TestClassName", nullptr, true },
        };
#if (ENABLE(WEBKIT2) && (NESTED_MASTER_CONDITION || MASTER_OR && MASTER_AND))
    case MessageName::TestWithLegacyReceiver_CreatePlugin:
        return Vector<ArgumentDescription> {
            { "result", "bool", nullptr, false },
        };
    case MessageName::TestWithLegacyReceiver_RunJavaScriptAlert:
        return Vector<ArgumentDescription> { };
    case MessageName::TestWithLegacyReceiver_GetPlugins:
        return Vector<ArgumentDescription> {
            { "plugins", "Vector<CyberCore::PluginInfo>", nullptr, false },
        };
    case MessageName::TestWithLegacyReceiver_GetPluginProcessConnection:
        return Vector<ArgumentDescription> {
            { "connectionHandle", "IPC::Connection::Handle", nullptr, false },
        };
    case MessageName::TestWithLegacyReceiver_TestMultipleAttributes:
        return Vector<ArgumentDescription> { };
#if PLATFORM(MAC)
    case MessageName::TestWithLegacyReceiver_InterpretKeyEvent:
        return Vector<ArgumentDescription> {
            { "commandName", "Vector<CyberCore::KeypressCommand>", nullptr, false },
        };
#endif
#endif
#if (ENABLE(WEBKIT2) && (NESTED_MASTER_CONDITION || MASTER_OR && MASTER_AND))
    case MessageName::TestWithoutAttributes_CreatePlugin:
        return Vector<ArgumentDescription> {
            { "result", "bool", nullptr, false },
        };
    case MessageName::TestWithoutAttributes_RunJavaScriptAlert:
        return Vector<ArgumentDescription> { };
    case MessageName::TestWithoutAttributes_GetPlugins:
        return Vector<ArgumentDescription> {
            { "plugins", "Vector<CyberCore::PluginInfo>", nullptr, false },
        };
    case MessageName::TestWithoutAttributes_GetPluginProcessConnection:
        return Vector<ArgumentDescription> {
            { "connectionHandle", "IPC::Connection::Handle", nullptr, false },
        };
    case MessageName::TestWithoutAttributes_TestMultipleAttributes:
        return Vector<ArgumentDescription> { };
#if PLATFORM(MAC)
    case MessageName::TestWithoutAttributes_InterpretKeyEvent:
        return Vector<ArgumentDescription> {
            { "commandName", "Vector<CyberCore::KeypressCommand>", nullptr, false },
        };
#endif
#endif
    case MessageName::TestWithSemaphore_ReceiveSemaphore:
        return Vector<ArgumentDescription> {
            { "r0", "IPC::Semaphore", nullptr, false },
        };
    case MessageName::TestWithImageData_ReceiveImageData:
        return Vector<ArgumentDescription> {
            { "r0", "RefPtr<CyberCore::ImageData>", nullptr, false },
        };
    case MessageName::TestWithStream_SendStringAsync:
        return Vector<ArgumentDescription> {
            { "returnValue", "int64_t", nullptr, false },
        };
    case MessageName::TestWithStream_SendStringSync:
        return Vector<ArgumentDescription> {
            { "returnValue", "int64_t", nullptr, false },
        };
    case MessageName::TestWithStream_CallWithIdentifier:
        return Vector<ArgumentDescription> { };
#if PLATFORM(COCOA)
    case MessageName::TestWithStream_ReceiveMachSendRight:
        return Vector<ArgumentDescription> {
            { "r1", "MachSendRight", nullptr, false },
        };
    case MessageName::TestWithStream_SendAndReceiveMachSendRight:
        return Vector<ArgumentDescription> {
            { "r1", "MachSendRight", nullptr, false },
        };
#endif
#if USE(AVFOUNDATION)
    case MessageName::TestWithCVPixelBuffer_ReceiveCVPixelBuffer:
        return Vector<ArgumentDescription> {
            { "r0", "RetainPtr<CVPixelBufferRef>", nullptr, false },
        };
#endif
    default:
        break;
    }
    return std::nullopt;
}

} // namespace CyberKit

#endif // ENABLE(IPC_TESTING_API) || !LOG_DISABLED
