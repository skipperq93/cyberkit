# Copyright (C) 2010-2023 Apple Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1.  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import collections
import itertools
import re
import sys

from webkit import parser
from webkit.model import BUILTIN_ATTRIBUTE, SYNCHRONOUS_ATTRIBUTE, ALLOWEDWHENWAITINGFORSYNCREPLY_ATTRIBUTE, ALLOWEDWHENWAITINGFORSYNCREPLYDURINGUNBOUNDEDIPC_ATTRIBUTE, MAINTHREADCALLBACK_ATTRIBUTE, STREAM_ATTRIBUTE, CALL_WITH_REPLY_ID_ATTRIBUTE, MessageReceiver, Message

_license_header = """/*
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

"""

WANTS_DISPATCH_MESSAGE_ATTRIBUTE = 'WantsDispatchMessage'
WANTS_ASYNC_DISPATCH_MESSAGE_ATTRIBUTE = 'WantsAsyncDispatchMessage'
LEGACY_RECEIVER_ATTRIBUTE = 'LegacyReceiver'
NOT_REFCOUNTED_RECEIVER_ATTRIBUTE = 'NotRefCounted'
NOT_STREAM_ENCODABLE_ATTRIBUTE = 'NotStreamEncodable'
NOT_STREAM_ENCODABLE_REPLY_ATTRIBUTE = 'NotStreamEncodableReply'
STREAM_BATCHED_ATTRIBUTE = 'StreamBatched'

attributes_to_generate_validators = {
    "messageAllowedWhenWaitingForSyncReply": [ALLOWEDWHENWAITINGFORSYNCREPLY_ATTRIBUTE, SYNCHRONOUS_ATTRIBUTE, STREAM_ATTRIBUTE],
    "messageAllowedWhenWaitingForUnboundedSyncReply": [ALLOWEDWHENWAITINGFORSYNCREPLYDURINGUNBOUNDEDIPC_ATTRIBUTE],
}

def receiver_enumerator_order_key(receiver_name):
    if receiver_name == 'IPC':
        return 1
    elif receiver_name == 'AsyncReply':
        return 2
    return 0


class MessageEnumerator(object):

    def __init__(self, receiver, messages):
        self.receiver = receiver
        self.messages = messages

    def __str__(self):
        if self.messages[0].has_attribute(BUILTIN_ATTRIBUTE):
            return self.messages[0].name
        if self.receiver.name == 'AsyncReply':
            return self.messages[0].name
        return '%s_%s' % (self.receiver.name, self.messages[0].name)

    def condition(self):
        conditions = [message.condition for message in self.messages]
        if any([condition is None for condition in conditions]):
            return None
        return " || ".join(conditions)

    def synchronous(self):
        is_synchronous = self.messages[0].has_attribute(SYNCHRONOUS_ATTRIBUTE)
        assert(all([message.has_attribute(SYNCHRONOUS_ATTRIBUTE) == is_synchronous for message in self.messages]))
        return is_synchronous

    @classmethod
    def sort_key(cls, obj):
        return obj.synchronous(), receiver_enumerator_order_key(obj.receiver.name), str(obj)


def get_message_enumerators(receivers):
    enumerators = []
    for receiver in receivers:
        receiver_enumerators = {}
        for message in receiver.messages:
            if message.name not in receiver_enumerators:
                receiver_enumerators[message.name] = MessageEnumerator(receiver, messages=[message])
            else:
                receiver_enumerators[message.name].messages.append(message)
        enumerators += receiver_enumerators.values()
    assert(len(enumerators) == len(set(enumerators)))
    return sorted(enumerators, key=MessageEnumerator.sort_key)


def get_receiver_enumerators(receivers):
    return sorted((r.name for r in receivers), key=lambda n: (receiver_enumerator_order_key(n), n))


def messages_header_filename(receiver):
    return '%sMessages.h' % receiver.name


def surround_in_condition(string, condition):
    if not condition:
        return string
    return '#if %s\n%s#endif\n' % (condition, string)


def function_parameter_type(type, kind):
    # Don't use references for built-in types.
    builtin_types = frozenset([
        'bool',
        'float',
        'double',
        'uint8_t',
        'uint16_t',
        'uint32_t',
        'uint64_t',
        'int8_t',
        'int16_t',
        'int32_t',
        'int64_t',
        'size_t',
    ])

    if type in builtin_types:
        return type

    if kind.startswith('enum:'):
        return type

    return 'const %s&' % type


def message_to_struct_declaration(receiver, message):
    result = []
    function_parameters = [(function_parameter_type(x.type, x.kind), x.name) for x in message.parameters]

    result.append('class %s {\n' % message.name)
    result.append('public:\n')
    result.append('    using Arguments = std::tuple<%s>;\n' % ', '.join([parameter.type for parameter in message.parameters]))
    result.append('\n')
    result.append('    static IPC::MessageName name() { return IPC::MessageName::%s_%s; }\n' % (receiver.name, message.name))
    result.append('    static constexpr bool isSync = %s;\n' % ('false', 'true')[message.reply_parameters is not None and message.has_attribute(SYNCHRONOUS_ATTRIBUTE)])
    if receiver.has_attribute(STREAM_ATTRIBUTE):
        result.append('    static constexpr bool isStreamEncodable = %s;\n' % ('true', 'false')[message.has_attribute(NOT_STREAM_ENCODABLE_ATTRIBUTE)])
        if message.reply_parameters is not None:
            result.append('    static constexpr bool isReplyStreamEncodable = %s;\n' % ('true', 'false')[message.has_attribute(NOT_STREAM_ENCODABLE_REPLY_ATTRIBUTE)])
            if message.has_attribute(STREAM_BATCHED_ATTRIBUTE):
                sys.stderr.write("Error: %s::%s has a reply but is marked as batched. Messages with replies are intended to be sent without latency.\n" % (receiver.name, message.name))
                sys.exit(1)
        result.append('    static constexpr bool isStreamBatched = %s;\n' % ('false', 'true')[message.has_attribute(STREAM_BATCHED_ATTRIBUTE)])

    result.append('\n')
    if message.reply_parameters != None:
        if not message.has_attribute(SYNCHRONOUS_ATTRIBUTE):
            result.append('    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::%s_%sReply; }\n' % (receiver.name, message.name))
        if message.has_attribute(MAINTHREADCALLBACK_ATTRIBUTE):
            result.append('    static constexpr auto callbackThread = WTF::CompletionHandlerCallThread::MainThread;\n')
        else:
            result.append('    static constexpr auto callbackThread = WTF::CompletionHandlerCallThread::ConstructionThread;\n')
        result.append('    using ReplyArguments = std::tuple<%s>;\n' % ', '.join([parameter.type for parameter in message.reply_parameters]))

    if len(function_parameters):
        result.append('    %s%s(%s)' % (len(function_parameters) == 1 and 'explicit ' or '', message.name, ', '.join([' '.join(x) for x in function_parameters])))
        result.append('\n        : m_arguments(%s)\n' % ', '.join([x[1] for x in function_parameters]))
        result.append('    {\n')
        result.append('    }\n\n')
    result.append('    const auto& arguments() const\n')
    result.append('    {\n')
    result.append('        return m_arguments;\n')
    result.append('    }\n')
    result.append('\n')
    result.append('private:\n')
    result.append('    std::tuple<%s> m_arguments;\n' % ', '.join([x[0] for x in function_parameters]))
    result.append('};\n')
    return surround_in_condition(''.join(result), message.condition)


def forward_declaration(namespace, kind_and_type):
    kind, type = kind_and_type

    qualified_name = '%s::%s' % (namespace, type)
    if kind == 'struct':
        return 'struct %s' % type
    elif kind.startswith('enum:'):
        return 'enum class %s : %s' % (type, kind[5:])
    else:
        return 'class %s' % type


def forward_declarations_for_namespace(namespace, kind_and_types):
    result = []
    result.append('namespace %s {\n' % namespace)
    result += ['%s;\n' % forward_declaration(namespace, x) for x in sorted(kind_and_types)]
    result.append('}\n')
    return ''.join(result)


# When updating this list, run "make -C Source/CyberKit/Scripts/webkit/tests" to keep the webkitpy tests passing.
def serialized_identifiers():
    return [
        'CyberCore::BackgroundFetchRecordIdentifier',
        'CyberCore::BroadcastChannelIdentifier',
        'CyberCore::DOMCacheIdentifier',
        'CyberCore::DictationContext',
        'CyberCore::DisplayList::ItemBufferIdentifier',
        'CyberCore::ElementIdentifier',
        'CyberCore::FetchIdentifier',
        'CyberCore::FileSystemHandleIdentifier',
        'CyberCore::FileSystemSyncAccessHandleIdentifier',
        'CyberCore::ImageDecoderIdentifier',
        'CyberCore::LayerHostingContextIdentifier',
        'CyberCore::LibWebRTCSocketIdentifier',
        'CyberCore::MediaKeySystemRequestIdentifier',
        'CyberCore::MediaPlayerIdentifier',
        'CyberCore::MediaSessionIdentifier',
        'CyberCore::OpaqueOriginIdentifier',
        'CyberCore::PageIdentifier',
        'CyberCore::PlaybackTargetClientContextIdentifier',
        'CyberCore::PushSubscriptionIdentifier',
        'CyberCore::PortIdentifier',
        'CyberCore::ProcessIdentifier',
        'CyberCore::RealtimeMediaSourceIdentifier',
        'CyberCore::RenderingResourceIdentifier',
        'CyberCore::ResourceLoaderIdentifier',
        'CyberCore::RTCDataChannelLocalIdentifier',
        'CyberCore::SWServerConnectionIdentifier',
        'CyberCore::ServiceWorkerIdentifier',
        'CyberCore::ServiceWorkerJobIdentifier',
        'CyberCore::ServiceWorkerRegistrationIdentifier',
        'CyberCore::SharedWorkerIdentifier',
        'CyberCore::SleepDisablerIdentifier',
        'CyberCore::SpeechRecognitionConnectionClientIdentifier',
        'CyberCore::TextCheckingRequestIdentifier',
        'CyberCore::TextManipulationTokenIdentifier',
        'CyberCore::UserMediaRequestIdentifier',
        'CyberCore::WebSocketIdentifier',
        'CyberKit::AudioMediaStreamTrackRendererInternalUnitIdentifier',
        'CyberKit::AuthenticationChallengeIdentifier',
        'CyberKit::ContentWorldIdentifier',
        'CyberKit::DataTaskIdentifier',
        'CyberKit::DownloadID',
        'CyberKit::FormSubmitListenerIdentifier',
        'CyberKit::GeolocationIdentifier',
        'CyberKit::GraphicsContextGLIdentifier',
        'CyberKit::IPCConnectionTesterIdentifier',
        'CyberKit::IPCStreamTesterIdentifier',
        'CyberKit::LegacyCustomProtocolID',
        'CyberKit::LibWebRTCResolverIdentifier',
        'CyberKit::MDNSRegisterIdentifier',
        'CyberKit::MarkSurfacesAsVolatileRequestIdentifier',
        'CyberKit::MediaRecorderIdentifier',
        'CyberKit::NetworkResourceLoadIdentifier',
        'CyberKit::PDFPluginIdentifier',
        'CyberKit::PageGroupIdentifier',
        'CyberKit::PlaybackSessionContextIdentifier',
        'CyberKit::QuotaIncreaseRequestIdentifier',
        'CyberKit::RemoteAudioDestinationIdentifier',
        'CyberKit::RemoteAudioHardwareListenerIdentifier',
        'CyberKit::RemoteAudioSessionIdentifier',
        'CyberKit::RemoteCDMIdentifier',
        'CyberKit::RemoteCDMInstanceIdentifier',
        'CyberKit::RemoteCDMInstanceSessionIdentifier',
        'CyberKit::RemoteLegacyCDMIdentifier',
        'CyberKit::RemoteLegacyCDMSessionIdentifier',
        'CyberKit::RemoteMediaResourceIdentifier',
        'CyberKit::RemoteSerializedImageBufferIdentifier',
        'CyberKit::RemoteVideoFrameIdentifier',
        'CyberKit::RemoteRemoteCommandListenerIdentifier',
        'CyberKit::RenderingBackendIdentifier',
        'CyberKit::RenderingUpdateID',
        'CyberKit::RetrieveRecordResponseBodyCallbackIdentifier',
        'CyberKit::SampleBufferDisplayLayerIdentifier',
        'CyberKit::StorageAreaIdentifier',
        'CyberKit::StorageAreaImplIdentifier',
        'CyberKit::StorageAreaMapIdentifier',
        'CyberKit::StorageNamespaceIdentifier',
        'CyberKit::TapIdentifier',
        'CyberKit::TrackPrivateRemoteIdentifier',
        'CyberKit::TransactionID',
        'CyberKit::UserContentControllerIdentifier',
        'CyberKit::VideoDecoderIdentifier',
        'CyberKit::VideoEncoderIdentifier',
        'CyberKit::WebExtensionContextIdentifier',
        'CyberKit::WebExtensionControllerIdentifier',
        'CyberKit::WebGPUIdentifier',
        'CyberKit::WebPageProxyIdentifier',
        'CyberKit::WebURLSchemeHandlerIdentifier',
    ]


def types_that_cannot_be_forward_declared():
    return frozenset([
        'CVPixelBufferRef',
        'GCGLint',
        'IPC::AsyncReplyID',
        'IPC::DataReference',
        'IPC::FontReference',
        'IPC::Semaphore',
        'MachSendRight',
        'MediaTime',
        'PlatformXR::ReferenceSpaceType',
        'PlatformXR::SessionFeature',
        'PlatformXR::SessionMode',
        'PlatformXR::VisibilityState',
        'String',
        'CyberCore::BackForwardItemIdentifier',
        'CyberCore::ControlStyle',
        'CyberCore::DestinationColorSpace',
        'CyberCore::DiagnosticLoggingDomain',
        'CyberCore::DictationContext',
        'CyberCore::DragApplicationFlags',
        'CyberCore::FrameIdentifier',
        'CyberCore::GraphicsContextGLAttributes',
        'CyberCore::GraphicsLayer::PlatformLayerID',
        'CyberCore::ModalContainerControlType',
        'CyberCore::NativeImageReference',
        'CyberCore::PluginLoadClientPolicy',
        'CyberCore::PointerID',
        'CyberCore::PolicyCheckIdentifier',
        'CyberCore::RenderingMode',
        'CyberCore::RenderingPurpose',
        'CyberCore::ScriptExecutionContextIdentifier',
        'CyberCore::ServiceWorkerOrClientData',
        'CyberCore::ServiceWorkerOrClientIdentifier',
        'CyberCore::SharedStringHash',
        'CyberCore::SharedWorkerObjectIdentifier',
        'CyberCore::SourceBufferAppendMode',
        'CyberCore::StorageType',
        'CyberCore::TransferredMessagePort',
        'CyberCore::WebLockIdentifier',
        'CyberKit::ActivityStateChangeID',
        'CyberKit::DisplayLinkObserverID',
        'CyberKit::DisplayListRecorderFlushIdentifier',
        'CyberKit::DownloadID',
        'CyberKit::FileSystemStorageError',
        'CyberKit::FileSystemSyncAccessHandleInfo',
        'CyberKit::ImageBufferBackendHandle',
        'CyberKit::LayerHostingContextID',
        'CyberKit::LegacyCustomProtocolID',
        'CyberKit::RemoteMediaSourceIdentifier',
        'CyberKit::RemoteSourceBufferIdentifier',
        'CyberKit::RemoteVideoFrameWriteReference',
        'CyberKit::RemoteVideoFrameReadReference',
        'CyberKit::RemoteSerializedImageBufferWriteReference',
        'CyberKit::RemoteSerializedImageBufferReadReference',
        'CyberKit::RenderingUpdateID',
        'CyberKit::TextCheckerRequestID',
        'CyberKit::TransactionID',
        'CyberKit::WCLayerTreeHostIdentifier',
        'CyberKit::WCContentBufferIdentifier',
        'CyberKit::WebExtensionEventListenerType',
        'CyberKit::XRDeviceIdentifier',
    ] + serialized_identifiers())


def conditions_for_header(header):
    conditions = {
        '"InputMethodState.h"': ["PLATFORM(GTK)", "PLATFORM(WPE)"],
        '"GestureTypes.h"': ["PLATFORM(IOS_FAMILY)"],
        '"RemoteCDMIdentifier.h"': ["ENABLE(GPU_PROCESS) && ENABLE(ENCRYPTED_MEDIA)"],
        '"RemoteCDMInstanceIdentifier.h"': ["ENABLE(GPU_PROCESS) && ENABLE(ENCRYPTED_MEDIA)"],
        '"RemoteCDMInstanceSessionIdentifier.h"': ["ENABLE(GPU_PROCESS) && ENABLE(ENCRYPTED_MEDIA)"],
        '"RemoteLegacyCDMIdentifier.h"': ["ENABLE(GPU_PROCESS) && ENABLE(LEGACY_ENCRYPTED_MEDIA)"],
        '"RemoteLegacyCDMSessionIdentifier.h"': ["ENABLE(GPU_PROCESS) && ENABLE(LEGACY_ENCRYPTED_MEDIA)"],
        '"SharedCARingBuffer.h"': ["PLATFORM(COCOA)"],
        '"WCContentBufferIdentifier.h"': ["USE(GRAPHICS_LAYER_WC)"],
        '"WCLayerTreeHostIdentifier.h"': ["USE(GRAPHICS_LAYER_WC)"],
        '<CyberCore/CVUtilities.h>': ["PLATFORM(COCOA)", ],
        '<CyberCore/DataDetectorType.h>': ["ENABLE(DATA_DETECTION)"],
        '<CyberCore/MediaPlaybackTargetContext.h>': ["ENABLE(WIRELESS_PLAYBACK_TARGET)"],
        '<CyberCore/VideoFrameCV.h>': ["PLATFORM(COCOA)", ],
    }
    if not header in conditions:
        return None
    return conditions[header]


def forward_declarations_and_headers(receiver):
    types_by_namespace = collections.defaultdict(set)

    headers = set([
        '"ArgumentCoders.h"',
        '"Connection.h"',
        '"MessageNames.h"',
        '<wtf/Forward.h>',
        '<wtf/ThreadSafeRefCounted.h>',
    ])

    non_template_wtf_types = frozenset([
        'MachSendRight',
        'MediaType',
        'String',
    ])

    no_forward_declaration_types = types_that_cannot_be_forward_declared()
    for parameter in receiver.iterparameters():
        kind = parameter.kind
        type = parameter.type

        if type.find('<') != -1 or type in no_forward_declaration_types:
            # Don't forward declare class templates.
            headers.update(headers_for_type(type))
            continue

        split = type.split('::')

        # Handle WTF types even if the WTF:: prefix is not given
        if split[0] in non_template_wtf_types:
            split.insert(0, 'WTF')

        if len(split) == 2:
            namespace = split[0]
            inner_type = split[1]
            types_by_namespace[namespace].add((kind, inner_type))
        elif len(split) > 2:
            # We probably have a nested struct, which means we can't forward declare it.
            # Include its header instead.
            headers.update(headers_for_type(type))

    forward_declarations = '\n'.join([forward_declarations_for_namespace(namespace, types) for (namespace, types) in sorted(types_by_namespace.items())])

    header_includes = []
    for header in sorted(headers):
        conditions = conditions_for_header(header)
        if conditions and not None in conditions:
            header_include = '#if %s\n' % ' || '.join(sorted(set(conditions)))
            header_include += '#include %s\n' % header
            header_include += '#endif\n'
            header_includes.append(header_include)
        else:
            header_includes.append('#include %s\n' % header)

    return (forward_declarations, header_includes)


def generate_messages_header(receiver):
    result = []

    result.append(_license_header)

    result.append('#pragma once\n')
    result.append('\n')

    if receiver.condition:
        result.append('#if %s\n\n' % receiver.condition)

    forward_declarations, headers = forward_declarations_and_headers(receiver)

    result += headers
    result.append('\n')

    result.append(forward_declarations)
    result.append('\n')

    result.append('namespace Messages {\nnamespace %s {\n' % receiver.name)
    result.append('\n')
    result.append('static inline IPC::ReceiverName messageReceiverName()\n')
    result.append('{\n')
    result.append('    return IPC::ReceiverName::%s;\n' % receiver.name)
    result.append('}\n')
    result.append('\n')
    result.append('\n'.join([message_to_struct_declaration(receiver, x) for x in receiver.messages]))
    result.append('\n')
    result.append('} // namespace %s\n} // namespace Messages\n' % receiver.name)

    if receiver.condition:
        result.append('\n#endif // %s\n' % receiver.condition)

    return ''.join(result)


def handler_function(receiver, message):
    if message.name.find('URL') == 0:
        return '%s::%s' % (receiver.name, 'url' + message.name[3:])
    return '%s::%s' % (receiver.name, message.name[0].lower() + message.name[1:])


def async_message_statement(receiver, message):
    dispatch_function_args = ['decoder', 'this', '&%s' % handler_function(receiver, message)]

    dispatch_function = 'handleMessage'
    if message.reply_parameters is not None and not message.has_attribute(SYNCHRONOUS_ATTRIBUTE):
        dispatch_function += 'Async'
    if message.has_attribute(CALL_WITH_REPLY_ID_ATTRIBUTE):
        dispatch_function += 'WithReplyID'

    connection = 'connection'
    if receiver.has_attribute(STREAM_ATTRIBUTE):
        connection = 'connection.connection()'

    result = []
    result.append('    if (decoder.messageName() == Messages::%s::%s::name())\n' % (receiver.name, message.name))
    result.append('        return IPC::%s<Messages::%s::%s>(%s, %s);\n' % (dispatch_function, receiver.name, message.name, connection, ', '.join(dispatch_function_args)))
    return result


def sync_message_statement(receiver, message):
    dispatch_function = 'handleMessage'
    if message.has_attribute(SYNCHRONOUS_ATTRIBUTE):
        dispatch_function += 'Synchronous'
    elif message.reply_parameters is not None:
        dispatch_function += 'Async'

    maybe_reply_encoder = ", *replyEncoder"
    if receiver.has_attribute(STREAM_ATTRIBUTE):
        maybe_reply_encoder = ''
    elif message.reply_parameters is not None:
        maybe_reply_encoder = ', replyEncoder'

    result = []
    result.append('    if (decoder.messageName() == Messages::%s::%s::name())\n' % (receiver.name, message.name))
    result.append('        return IPC::%s<Messages::%s::%s>(connection, decoder%s, this, &%s);\n' % (dispatch_function, receiver.name, message.name, maybe_reply_encoder, handler_function(receiver, message)))
    return result


def class_template_headers(template_string):
    template_string = template_string.strip()

    class_template_types = {
        'CyberCore::RectEdges': {'headers': ['<CyberCore/RectEdges.h>'], 'argument_coder_headers': ['"ArgumentCoders.h"']},
        'Expected': {'headers': ['<wtf/Expected.h>'], 'argument_coder_headers': ['"ArgumentCoders.h"']},
        'HashMap': {'headers': ['<wtf/HashMap.h>'], 'argument_coder_headers': ['"ArgumentCoders.h"']},
        'HashSet': {'headers': ['<wtf/HashSet.h>'], 'argument_coder_headers': ['"ArgumentCoders.h"']},
        'OptionSet': {'headers': ['<wtf/OptionSet.h>'], 'argument_coder_headers': ['"ArgumentCoders.h"']},
        'Vector': {'headers': ['<wtf/Vector.h>'], 'argument_coder_headers': ['"ArgumentCoders.h"']},
        'std::optional': {'headers': ['<optional>'], 'argument_coder_headers': ['"ArgumentCoders.h"']},
        'std::pair': {'headers': ['<utility>'], 'argument_coder_headers': ['"ArgumentCoders.h"']},
        'IPC::ArrayReference': {'headers': ['"ArrayReference.h"'], 'argument_coder_headers': ['"ArgumentCoders.h"']},
        'IPC::ArrayReferenceTuple': {'headers': ['"ArrayReferenceTuple.h"'], 'argument_coder_headers': ['"ArgumentCoders.h"']},
        'Ref': {'headers': ['<wtf/Ref.h>'], 'argument_coder_headers': ['"ArgumentCoders.h"']},
        'RefPtr': {'headers': ['<wtf/RefCounted.h>'], 'argument_coder_headers': ['"ArgumentCoders.h"']},
        'RetainPtr': {'headers': ['<wtf/RetainPtr.h>'], 'argument_coder_headers': ['"ArgumentCodersCF.h"']},
    }

    match = re.match('(?P<template_name>.+?)<(?P<parameter_string>.+)>', template_string)
    if not match:
        return {'header_infos': [], 'types': [template_string]}

    template_name = match.groupdict()['template_name']
    if template_name not in class_template_types:
        sys.stderr.write("Error: no class template type is defined for '%s'\n" % (template_string))
        sys.exit(1)

    header_infos = [class_template_types[template_name]]
    types = []

    for parameter in parser.split_parameters_string(match.groupdict()['parameter_string']):
        parameter_header_infos_and_types = class_template_headers(parameter)

        header_infos += parameter_header_infos_and_types['header_infos']
        types += parameter_header_infos_and_types['types']

    return {'header_infos': header_infos, 'types': types}


def argument_coder_headers_for_type(type):
    header_infos_and_types = class_template_headers(type)

    special_cases = {
        'IPC::Connection::Handle': '"Connection.h"',
        'String': '"ArgumentCoders.h"',
        'MachSendRight': '"ArgumentCodersDarwin.h"',
        'CyberKit::ScriptMessageHandlerHandle': '"WebScriptMessageHandler.h"',
    }

    headers = []
    for header_info in header_infos_and_types['header_infos']:
        headers += header_info['argument_coder_headers']

    for type in header_infos_and_types['types']:
        if type in special_cases:
            headers.append(special_cases[type])
            continue

        split = type.split('::')
        if len(split) < 2:
            continue
        if split[0] == 'CyberCore':
            headers.append('"CyberCoreArgumentCoders.h"')

    return headers


def headers_for_type(type):
    header_infos_and_types = class_template_headers(type)

    special_cases = {
        'CVPixelBufferRef': ['<CyberCore/CVUtilities.h>'],
        'GCGLint': ['<CyberCore/GraphicsTypesGL.h>'],
        'Inspector::ExtensionError': ['"InspectorExtensionTypes.h"'],
        'Inspector::FrontendChannel::ConnectionType': ['<CyberScriptCore/InspectorFrontendChannel.h>'],
        'Inspector::InspectorTargetType': ['<CyberScriptCore/InspectorTarget.h>'],
        'IPC::AsyncReplyID': ['"Connection.h"'],
        'IPC::Semaphore': ['"IPCSemaphore.h"'],
        'JSC::MessageLevel': ['<CyberScriptCore/ConsoleTypes.h>'],
        'JSC::MessageSource': ['<CyberScriptCore/ConsoleTypes.h>'],
        'MachSendRight': ['<wtf/MachSendRight.h>'],
        'MediaTime': ['<wtf/MediaTime.h>'],
        'MonotonicTime': ['<wtf/MonotonicTime.h>'],
        'PAL::SessionID': ['<pal/SessionID.h>'],
        'PAL::WebGPU::AddressMode': ['<pal/graphics/WebGPU/WebGPUAddressMode.h>'],
        'PAL::WebGPU::BlendFactor': ['<pal/graphics/WebGPU/WebGPUBlendFactor.h>'],
        'PAL::WebGPU::BlendOperation': ['<pal/graphics/WebGPU/WebGPUBlendOperation.h>'],
        'PAL::WebGPU::BufferBindingType': ['<pal/graphics/WebGPU/WebGPUBufferBindingType.h>'],
        'PAL::WebGPU::BufferDynamicOffset': ['<pal/graphics/WebGPU/WebGPUIntegralTypes.h>'],
        'PAL::WebGPU::BufferUsageFlags': ['<pal/graphics/WebGPU/WebGPUBufferUsage.h>'],
        'PAL::WebGPU::CanvasCompositingAlphaMode': ['<pal/graphics/WebGPU/WebGPUCanvasCompositingAlphaMode.h>'],
        'PAL::WebGPU::ColorWriteFlags': ['<pal/graphics/WebGPU/WebGPUColorWrite.h>'],
        'PAL::WebGPU::CompareFunction': ['<pal/graphics/WebGPU/WebGPUCompareFunction.h>'],
        'PAL::WebGPU::CompilationMessageType': ['<pal/graphics/WebGPU/WebGPUCompilationMessageType.h>'],
        'PAL::WebGPU::ComputePassTimestampLocation': ['<pal/graphics/WebGPU/WebGPUComputePassTimestampLocation.h>'],
        'PAL::WebGPU::CullMode': ['<pal/graphics/WebGPU/WebGPUCullMode.h>'],
        'PAL::WebGPU::DepthBias': ['<pal/graphics/WebGPU/WebGPUIntegralTypes.h>'],
        'PAL::WebGPU::DeviceLostReason': ['<pal/graphics/WebGPU/WebGPUDeviceLostReason.h>'],
        'PAL::WebGPU::ErrorFilter': ['<pal/graphics/WebGPU/WebGPUErrorFilter.h>'],
        'PAL::WebGPU::FeatureName': ['<pal/graphics/WebGPU/WebGPUFeatureName.h>'],
        'PAL::WebGPU::FilterMode': ['<pal/graphics/WebGPU/WebGPUFilterMode.h>'],
        'PAL::WebGPU::FlagsConstant': ['<pal/graphics/WebGPU/WebGPUIntegralTypes.h>'],
        'PAL::WebGPU::FrontFace': ['<pal/graphics/WebGPU/WebGPUFrontFace.h>'],
        'PAL::WebGPU::Index32': ['<pal/graphics/WebGPU/WebGPUIntegralTypes.h>'],
        'PAL::WebGPU::IndexFormat': ['<pal/graphics/WebGPU/WebGPUIndexFormat.h>'],
        'PAL::WebGPU::IntegerCoordinate': ['<pal/graphics/WebGPU/WebGPUIntegralTypes.h>'],
        'PAL::WebGPU::LoadOp': ['<pal/graphics/WebGPU/WebGPULoadOp.h>'],
        'PAL::WebGPU::MapModeFlags': ['<pal/graphics/WebGPU/WebGPUMapMode.h>'],
        'PAL::WebGPU::PipelineStatisticName': ['<pal/graphics/WebGPU/WebGPUPipelineStatisticName.h>'],
        'PAL::WebGPU::PowerPreference': ['<pal/graphics/WebGPU/WebGPUPowerPreference.h>'],
        'PAL::WebGPU::PredefinedColorSpace': ['<pal/graphics/WebGPU/WebGPUPredefinedColorSpace.h>'],
        'PAL::WebGPU::PrimitiveTopology': ['<pal/graphics/WebGPU/WebGPUPrimitiveTopology.h>'],
        'PAL::WebGPU::QueryType': ['<pal/graphics/WebGPU/WebGPUQueryType.h>'],
        'PAL::WebGPU::RenderPassTimestampLocation': ['<pal/graphics/WebGPU/WebGPURenderPassTimestampLocation.h>'],
        'PAL::WebGPU::SampleMask': ['<pal/graphics/WebGPU/WebGPUIntegralTypes.h>'],
        'PAL::WebGPU::SamplerBindingType': ['<pal/graphics/WebGPU/WebGPUSamplerBindingType.h>'],
        'PAL::WebGPU::ShaderStageFlags': ['<pal/graphics/WebGPU/WebGPUShaderStage.h>'],
        'PAL::WebGPU::SignedOffset32': ['<pal/graphics/WebGPU/WebGPUIntegralTypes.h>'],
        'PAL::WebGPU::Size32': ['<pal/graphics/WebGPU/WebGPUIntegralTypes.h>'],
        'PAL::WebGPU::Size64': ['<pal/graphics/WebGPU/WebGPUIntegralTypes.h>'],
        'PAL::WebGPU::StencilOperation': ['<pal/graphics/WebGPU/WebGPUStencilOperation.h>'],
        'PAL::WebGPU::StencilValue': ['<pal/graphics/WebGPU/WebGPUIntegralTypes.h>'],
        'PAL::WebGPU::StorageTextureAccess': ['<pal/graphics/WebGPU/WebGPUStorageTextureAccess.h>'],
        'PAL::WebGPU::StoreOp': ['<pal/graphics/WebGPU/WebGPUStoreOp.h>'],
        'PAL::WebGPU::TextureAspect': ['<pal/graphics/WebGPU/WebGPUTextureAspect.h>'],
        'PAL::WebGPU::TextureDimension': ['<pal/graphics/WebGPU/WebGPUTextureDimension.h>'],
        'PAL::WebGPU::TextureFormat': ['<pal/graphics/WebGPU/WebGPUTextureFormat.h>'],
        'PAL::WebGPU::TextureSampleType': ['<pal/graphics/WebGPU/WebGPUTextureSampleType.h>'],
        'PAL::WebGPU::TextureUsageFlags': ['<pal/graphics/WebGPU/WebGPUTextureUsage.h>'],
        'PAL::WebGPU::TextureViewDimension': ['<pal/graphics/WebGPU/WebGPUTextureViewDimension.h>'],
        'PAL::WebGPU::VertexFormat': ['<pal/graphics/WebGPU/WebGPUVertexFormat.h>'],
        'PAL::WebGPU::VertexStepMode': ['<pal/graphics/WebGPU/WebGPUVertexStepMode.h>'],
        'PlatformXR::Device::FrameData': ['<CyberCore/PlatformXR.h>'],
        'PlatformXR::ReferenceSpaceType': ['<CyberCore/PlatformXR.h>'],
        'PlatformXR::SessionFeature': ['<CyberCore/PlatformXR.h>'],
        'PlatformXR::SessionMode': ['<CyberCore/PlatformXR.h>'],
        'PlatformXR::VisibilityState': ['<CyberCore/PlatformXR.h>'],
        'Seconds': ['<wtf/Seconds.h>'],
        'String': ['<wtf/text/WTFString.h>'],
        'URL': ['<wtf/URLHash.h>'],
        'UUID': ['<wtf/UUID.h>'],
        'WallTime': ['<wtf/WallTime.h>'],
        'CyberCore::AlternativeTextType': ['<CyberCore/AlternativeTextClient.h>'],
        'CyberCore::ApplyTrackingPrevention': ['<CyberCore/NetworkStorageSession.h>'],
        'CyberCore::ArcData': ['<CyberCore/InlinePathData.h>'],
        'CyberCore::AutoplayEventFlags': ['<CyberCore/AutoplayEvent.h>'],
        'CyberCore::BackForwardItemIdentifier': ['<CyberCore/ProcessQualified.h>', '<CyberCore/BackForwardItemIdentifier.h>', '<wtf/ObjectIdentifier.h>'],
        'CyberCore::BezierCurveData': ['<CyberCore/InlinePathData.h>'],
        'CyberCore::BlendMode': ['<CyberCore/GraphicsTypes.h>'],
        'CyberCore::BrowsingContextGroupSwitchDecision': ['<CyberCore/FrameLoaderTypes.h>'],
        'CyberCore::COEPDisposition': ['<CyberCore/CrossOriginEmbedderPolicy.h>'],
        'CyberCore::ColorSchemePreference': ['<CyberCore/DocumentLoader.h>'],
        'CyberCore::CompositeOperator': ['<CyberCore/GraphicsTypes.h>'],
        'CyberCore::Cookie': ['<CyberCore/Cookie.h>'],
        'CyberCore::COOPDisposition': ['<CyberCore/CrossOriginOpenerPolicy.h>'],
        'CyberCore::CreateNewGroupForHighlight': ['<CyberCore/AppHighlight.h>'],
        'CyberCore::DictationContext': ['<CyberCore/DictationContext.h>'],
        'CyberCore::DisplayList::ItemBufferIdentifier': ['<CyberCore/DisplayList.h>'],
        'CyberCore::DocumentMarkerLineStyle': ['<CyberCore/GraphicsTypes.h>'],
        'CyberCore::DOMPasteAccessCategory': ['<CyberCore/DOMPasteAccess.h>'],
        'CyberCore::DOMPasteAccessResponse': ['<CyberCore/DOMPasteAccess.h>'],
        'CyberCore::DragApplicationFlags': ['<CyberCore/DragData.h>'],
        'CyberCore::DragHandlingMethod': ['<CyberCore/DragActions.h>'],
        'CyberCore::DragOperation': ['<CyberCore/DragActions.h>'],
        'CyberCore::DragSourceAction': ['<CyberCore/DragActions.h>'],
        'CyberCore::DynamicRangeMode': ['<CyberCore/PlatformScreen.h>'],
        'CyberCore::ElementContext': ['<CyberCore/ElementContext.h>'],
        'CyberCore::EventMakesGamepadsVisible': ['<CyberCore/GamepadProviderClient.h>'],
        'CyberCore::ExceptionDetails': ['<CyberCore/JSDOMExceptionHandling.h>'],
        'CyberCore::FileChooserSettings': ['<CyberCore/FileChooser.h>'],
        'CyberCore::FirstPartyWebsiteDataRemovalMode': ['<CyberCore/NetworkStorageSession.h>'],
        'CyberCore::FontChanges': ['<CyberCore/FontAttributeChanges.h>'],
        'CyberCore::FontSmoothingMode': ['<CyberCore/GraphicsTypes.h>'],
        'CyberCore::FrameLoadType': ['<CyberCore/FrameLoaderTypes.h>'],
        'CyberCore::GenericCueData': ['<CyberCore/InbandGenericCue.h>'],
        'CyberCore::GrammarDetail': ['<CyberCore/TextCheckerClient.h>'],
        'CyberCore::GraphicsContextGLActiveInfo': ['<CyberCore/GraphicsContextGL.h>'],
        'CyberCore::HasInsecureContent': ['<CyberCore/FrameLoaderTypes.h>'],
        'CyberCore::HighlightRequestOriginatedInApp': ['<CyberCore/AppHighlight.h>'],
        'CyberCore::HighlightVisibility': ['<CyberCore/HighlightVisibility.h>'],
        'CyberCore::IncludeSecureCookies': ['<CyberCore/CookieJar.h>'],
        'CyberCore::IndexedDB::ObjectStoreOverwriteMode': ['<CyberCore/IndexedDB.h>'],
        'CyberCore::InputMode': ['<CyberCore/InputMode.h>'],
        'CyberCore::InspectorOverlay::Highlight': ['<CyberCore/InspectorOverlay.h>'],
        'CyberCore::ISOWebVTTCue': ['<CyberCore/ISOVTTCue.h>'],
        'CyberCore::KeyframeValueList': ['<CyberCore/GraphicsLayer.h>'],
        'CyberCore::KeypressCommand': ['<CyberCore/KeyboardEvent.h>'],
        'CyberCore::LastNavigationWasAppInitiated': ['<CyberCore/ServiceWorkerClientData.h>'],
        'CyberCore::LegacyCDMSessionClient::MediaKeyErrorCode': ['<CyberCore/LegacyCDMSession.h>'],
        'CyberCore::LineCap': ['<CyberCore/GraphicsTypes.h>'],
        'CyberCore::LineData': ['<CyberCore/InlinePathData.h>'],
        'CyberCore::LineJoin': ['<CyberCore/GraphicsTypes.h>'],
        'CyberCore::LockBackForwardList': ['<CyberCore/FrameLoaderTypes.h>'],
        'CyberCore::MediaEngineSupportParameters': ['<CyberCore/MediaPlayer.h>'],
        'CyberCore::MediaProducerMediaCaptureKind': ['<CyberCore/MediaProducer.h>'],
        'CyberCore::MediaProducerMediaState': ['<CyberCore/MediaProducer.h>'],
        'CyberCore::MediaProducerMutedState': ['<CyberCore/MediaProducer.h>'],
        'CyberCore::MessagePortChannelProvider::HasActivity': ['<CyberCore/MessagePortChannelProvider.h>'],
        'CyberCore::ModalContainerControlType': ['<CyberCore/ModalContainerTypes.h>'],
        'CyberCore::ModalContainerDecision': ['<CyberCore/ModalContainerTypes.h>'],
        'CyberCore::MouseEventPolicy': ['<CyberCore/DocumentLoader.h>'],
        'CyberCore::MoveData': ['<CyberCore/InlinePathData.h>'],
        'CyberCore::NetworkTransactionInformation': ['<CyberCore/NetworkLoadInformation.h>'],
        'CyberCore::OpaqueOriginIdentifier': ['<CyberCore/SecurityOriginData.h>'],
        'CyberCore::PasteboardCustomData': ['<CyberCore/Pasteboard.h>'],
        'CyberCore::PasteboardImage': ['<CyberCore/Pasteboard.h>'],
        'CyberCore::PasteboardURL': ['<CyberCore/Pasteboard.h>'],
        'CyberCore::PasteboardWebContent': ['<CyberCore/Pasteboard.h>'],
        'CyberCore::PixelFormat': ['<CyberCore/ImageBufferBackend.h>'],
        'CyberCore::PlatformTextTrackData': ['<CyberCore/PlatformTextTrack.h>'],
        'CyberCore::PlatformWheelEventPhase': ['<CyberCore/PlatformWheelEvent.h>'],
        'CyberCore::PlaybackSessionModel::PlaybackState': ['<CyberCore/PlaybackSessionModel.h>'],
        'CyberCore::PluginInfo': ['<CyberCore/PluginData.h>'],
        'CyberCore::PluginLoadClientPolicy': ['<CyberCore/PluginData.h>'],
        'CyberCore::PolicyAction': ['<CyberCore/FrameLoaderTypes.h>'],
        'CyberCore::PolicyCheckIdentifier': ['<CyberCore/FrameLoaderTypes.h>'],
        'CyberCore::PreserveResolution': ['<CyberCore/ImageBufferBackend.h>'],
        'CyberCore::ProcessIdentifier': ['<CyberCore/ProcessIdentifier.h>'],
        'CyberCore::PushSubscriptionIdentifier': ['<CyberCore/PushSubscriptionIdentifier.h>'],
        'CyberCore::QuadCurveData': ['<CyberCore/InlinePathData.h>'],
        'CyberCore::RecentSearch': ['<CyberCore/SearchPopupMenu.h>'],
        'CyberCore::ReloadOption': ['<CyberCore/FrameLoaderTypes.h>'],
        'CyberCore::RenderingPurpose': ['<CyberCore/RenderingMode.h>'],
        'CyberCore::RequestStorageAccessResult': ['<CyberCore/DocumentStorageAccess.h>'],
        'CyberCore::RouteSharingPolicy': ['<CyberCore/AudioSession.h>'],
        'CyberCore::SameSiteStrictEnforcementEnabled': ['<CyberCore/NetworkStorageSession.h>'],
        'CyberCore::ScriptExecutionContextIdentifier': ['<CyberCore/ProcessQualified.h>', '<CyberCore/ScriptExecutionContextIdentifier.h>', '<wtf/ObjectIdentifier.h>'],
        'CyberCore::ScrollGranularity': ['<CyberCore/ScrollTypes.h>'],
        'CyberCore::ScrollPinningBehavior': ['<CyberCore/ScrollTypes.h>'],
        'CyberCore::SecurityPolicyViolationEventInit': ['<CyberCore/SecurityPolicyViolationEvent.h>'],
        'CyberCore::SelectionDirection': ['<CyberCore/VisibleSelection.h>'],
        'CyberCore::SelectionGeometry': ['"EditorState.h"'],
        'CyberCore::ServiceWorkerJobIdentifier': ['<CyberCore/ServiceWorkerTypes.h>'],
        'CyberCore::ServiceWorkerOrClientData': ['<CyberCore/ServiceWorkerTypes.h>', '<CyberCore/ServiceWorkerClientData.h>', '<CyberCore/ServiceWorkerData.h>'],
        'CyberCore::ServiceWorkerOrClientIdentifier': ['<CyberCore/ServiceWorkerTypes.h>'],
        'CyberCore::ServiceWorkerRegistrationIdentifier': ['<CyberCore/ServiceWorkerTypes.h>'],
        'CyberCore::ServiceWorkerRegistrationState': ['<CyberCore/ServiceWorkerTypes.h>'],
        'CyberCore::ServiceWorkerState': ['<CyberCore/ServiceWorkerTypes.h>'],
        'CyberCore::ShareDataWithParsedURL': ['<CyberCore/ShareData.h>'],
        'CyberCore::ShouldContinuePolicyCheck': ['<CyberCore/FrameLoaderTypes.h>'],
        'CyberCore::ShouldNotifyWhenResolved': ['<CyberCore/ServiceWorkerTypes.h>'],
        'CyberCore::ShouldSample': ['<CyberCore/DiagnosticLoggingClient.h>'],
        'CyberCore::SourceBufferAppendMode': ['<CyberCore/SourceBufferPrivate.h>'],
        'CyberCore::StorageAccessPromptWasShown': ['<CyberCore/DocumentStorageAccess.h>'],
        'CyberCore::StorageAccessScope': ['<CyberCore/DocumentStorageAccess.h>'],
        'CyberCore::StorageAccessWasGranted': ['<CyberCore/DocumentStorageAccess.h>'],
        'CyberCore::SupportedPluginIdentifier': ['<CyberCore/PluginData.h>'],
        'CyberCore::SWServerConnectionIdentifier': ['<CyberCore/ServiceWorkerTypes.h>'],
        'CyberCore::SystemPreviewInfo': ['<CyberCore/FrameLoaderTypes.h>'],
        'CyberCore::TextCheckingRequestData': ['<CyberCore/TextChecking.h>'],
        'CyberCore::TextCheckingResult': ['<CyberCore/TextCheckerClient.h>'],
        'CyberCore::TextCheckingType': ['<CyberCore/TextChecking.h>'],
        'CyberCore::TextIndicatorData': ['<CyberCore/TextIndicator.h>'],
        'CyberCore::TextManipulationTokenIdentifier': ['<CyberCore/TextManipulationToken.h>'],
        'CyberCore::ThirdPartyCookieBlockingMode': ['<CyberCore/NetworkStorageSession.h>'],
        'CyberCore::UsedLegacyTLS': ['<CyberCore/ResourceResponseBase.h>'],
        'CyberCore::VideoPlaybackQualityMetrics': ['<CyberCore/VideoPlaybackQualityMetrics.h>'],
        'CyberCore::VideoPresetData': ['<CyberCore/VideoPreset.h>'],
        'CyberCore::ViewportAttributes': ['<CyberCore/ViewportArguments.h>'],
        'CyberCore::WheelEventProcessingSteps': ['<CyberCore/ScrollingCoordinatorTypes.h>'],
        'CyberCore::WillContinueLoading': ['<CyberCore/FrameLoaderTypes.h>'],
        'CyberCore::WillInternallyHandleFailure': ['<CyberCore/FrameLoaderTypes.h>'],
        'CyberKit::ActivityStateChangeID': ['"DrawingAreaInfo.h"'],
        'CyberKit::AllowOverwrite': ['"DownloadID.h"'],
        'CyberKit::AppPrivacyReportTestingData': ['"AppPrivacyReport.h"'],
        'CyberKit::AuthenticationChallengeIdentifier': ['"IdentifierTypes.h"'],
        'CyberKit::BackForwardListItemState': ['"SessionState.h"'],
        'CyberKit::CallDownloadDidStart': ['"DownloadManager.h"'],
        'CyberKit::ConsumerSharedCARingBuffer::Handle': ['"SharedCARingBuffer.h"'],
        'CyberKit::ContentWorldIdentifier': ['"ContentWorldShared.h"'],
        'CyberKit::DocumentEditingContextRequest': ['"DocumentEditingContext.h"'],
        'CyberKit::FindDecorationStyle': ['"WebFindOptions.h"'],
        'CyberKit::FindOptions': ['"WebFindOptions.h"'],
        'CyberKit::FormSubmitListenerIdentifier': ['"IdentifierTypes.h"'],
        'CyberKit::GestureRecognizerState': ['"GestureTypes.h"'],
        'CyberKit::GestureType': ['"GestureTypes.h"'],
        'CyberKit::LastNavigationWasAppInitiated': ['"AppPrivacyReport.h"'],
        'CyberKit::LayerHostingContextID': ['"LayerHostingContext.h"'],
        'CyberKit::LayerHostingMode': ['"LayerTreeContext.h"'],
        'CyberKit::PageGroupIdentifier': ['"IdentifierTypes.h"'],
        'CyberKit::PageState': ['"SessionState.h"'],
        'CyberKit::PaymentSetupConfiguration': ['"PaymentSetupConfigurationCyberKit.h"'],
        'CyberKit::PaymentSetupFeatures': ['"ApplePayPaymentSetupFeaturesCyberKit.h"'],
        'CyberKit::PrepareBackingStoreBuffersInputData': ['"PrepareBackingStoreBuffersData.h"'],
        'CyberKit::PrepareBackingStoreBuffersOutputData': ['"PrepareBackingStoreBuffersData.h"'],
        'CyberKit::RemoteSerializedImageBufferReadReference': ['"RemoteSerializedImageBufferIdentifier.h"'],
        'CyberKit::RemoteSerializedImageBufferWriteReference': ['"RemoteSerializedImageBufferIdentifier.h"'],
        'CyberKit::RemoteVideoFrameReadReference': ['"RemoteVideoFrameIdentifier.h"'],
        'CyberKit::RemoteVideoFrameWriteReference': ['"RemoteVideoFrameIdentifier.h"'],
        'CyberKit::RespectSelectionAnchor': ['"GestureTypes.h"'],
        'CyberKit::SelectionFlags': ['"GestureTypes.h"'],
        'CyberKit::SelectionTouch': ['"GestureTypes.h"'],
        'CyberKit::TapIdentifier': ['"IdentifierTypes.h"'],
        'CyberKit::TextCheckerRequestID': ['"IdentifierTypes.h"'],
        'CyberKit::WebEventType': ['"WebEvent.h"'],
        'CyberKit::WebGPU::BindGroupDescriptor': ['"WebGPUBindGroupDescriptor.h"'],
        'CyberKit::WebGPU::BindGroupEntry': ['"WebGPUBindGroupEntry.h"'],
        'CyberKit::WebGPU::BindGroupLayoutDescriptor': ['"WebGPUBindGroupLayoutDescriptor.h"'],
        'CyberKit::WebGPU::BindGroupLayoutEntry': ['"WebGPUBindGroupLayoutEntry.h"'],
        'CyberKit::WebGPU::BlendComponent': ['"WebGPUBlendComponent.h"'],
        'CyberKit::WebGPU::BlendState': ['"WebGPUBlendState.h"'],
        'CyberKit::WebGPU::BufferBinding': ['"WebGPUBufferBinding.h"'],
        'CyberKit::WebGPU::BufferBindingLayout': ['"WebGPUBufferBindingLayout.h"'],
        'CyberKit::WebGPU::BufferDescriptor': ['"WebGPUBufferDescriptor.h"'],
        'CyberKit::WebGPU::CanvasConfiguration': ['"WebGPUCanvasConfiguration.h"'],
        'CyberKit::WebGPU::Color': ['"WebGPUColor.h"'],
        'CyberKit::WebGPU::ColorTargetState': ['"WebGPUColorTargetState.h"'],
        'CyberKit::WebGPU::CommandBufferDescriptor': ['"WebGPUCommandBufferDescriptor.h"'],
        'CyberKit::WebGPU::CommandEncoderDescriptor': ['"WebGPUCommandEncoderDescriptor.h"'],
        'CyberKit::WebGPU::CompilationMessage': ['"WebGPUCompilationMessage.h"'],
        'CyberKit::WebGPU::ComputePassDescriptor': ['"WebGPUComputePassDescriptor.h"'],
        'CyberKit::WebGPU::ComputePassTimestampWrites': ['"WebGPUComputePassTimestampWrites.h"'],
        'CyberKit::WebGPU::ComputePipelineDescriptor': ['"WebGPUComputePipelineDescriptor.h"'],
        'CyberKit::WebGPU::ConvertFromBackingContext': ['"WebGPUConvertFromBackingContext.h"'],
        'CyberKit::WebGPU::ConvertToBackingContext': ['"WebGPUConvertToBackingContext.h"'],
        'CyberKit::WebGPU::DepthStencilState': ['"WebGPUDepthStencilState.h"'],
        'CyberKit::WebGPU::DeviceDescriptor': ['"WebGPUDeviceDescriptor.h"'],
        'CyberKit::WebGPU::Error': ['"WebGPUError.h"'],
        'CyberKit::WebGPU::Extent3D': ['"WebGPUExtent3D.h"'],
        'CyberKit::WebGPU::ExternalTextureBindingLayout': ['"WebGPUExternalTextureBindingLayout.h"'],
        'CyberKit::WebGPU::ExternalTextureDescriptor': ['"WebGPUExternalTextureDescriptor.h"'],
        'CyberKit::WebGPU::FragmentState': ['"WebGPUFragmentState.h"'],
        'CyberKit::WebGPU::Identifier': ['"WebGPUIdentifier.h"'],
        'CyberKit::WebGPU::ImageCopyBuffer': ['"WebGPUImageCopyBuffer.h"'],
        'CyberKit::WebGPU::ImageCopyExternalImage': ['"WebGPUImageCopyExternalImage.h"'],
        'CyberKit::WebGPU::ImageCopyTexture': ['"WebGPUImageCopyTexture.h"'],
        'CyberKit::WebGPU::ImageCopyTextureTagged': ['"WebGPUImageCopyTextureTagged.h"'],
        'CyberKit::WebGPU::ImageDataLayout': ['"WebGPUImageDataLayout.h"'],
        'CyberKit::WebGPU::MultisampleState': ['"WebGPUMultisampleState.h"'],
        'CyberKit::WebGPU::ObjectDescriptorBase': ['"WebGPUObjectDescriptorBase.h"'],
        'CyberKit::WebGPU::Origin2D': ['"WebGPUOrigin2D.h"'],
        'CyberKit::WebGPU::Origin3D': ['"WebGPUOrigin3D.h"'],
        'CyberKit::WebGPU::OutOfMemoryError': ['"WebGPUOutOfMemoryError.h"'],
        'CyberKit::WebGPU::PipelineDescriptorBase': ['"WebGPUPipelineDescriptorBase.h"'],
        'CyberKit::WebGPU::PipelineLayoutDescriptor': ['"WebGPUPipelineLayoutDescriptor.h"'],
        'CyberKit::WebGPU::PresentationContextDescriptor': ['"WebGPUPresentationContextDescriptor.h"'],
        'CyberKit::WebGPU::PrimitiveState': ['"WebGPUPrimitiveState.h"'],
        'CyberKit::WebGPU::ProgrammableStage': ['"WebGPUProgrammableStage.h"'],
        'CyberKit::WebGPU::QuerySetDescriptor': ['"WebGPUQuerySetDescriptor.h"'],
        'CyberKit::WebGPU::RenderBundleDescriptor': ['"WebGPURenderBundleDescriptor.h"'],
        'CyberKit::WebGPU::RenderBundleEncoderDescriptor': ['"WebGPURenderBundleEncoderDescriptor.h"'],
        'CyberKit::WebGPU::RenderPassColorAttachment': ['"WebGPURenderPassColorAttachment.h"'],
        'CyberKit::WebGPU::RenderPassDepthStencilAttachment': ['"WebGPURenderPassDepthStencilAttachment.h"'],
        'CyberKit::WebGPU::RenderPassDescriptor': ['"WebGPURenderPassDescriptor.h"'],
        'CyberKit::WebGPU::RenderPassLayout': ['"WebGPURenderPassLayout.h"'],
        'CyberKit::WebGPU::RenderPassTimestampWrites': ['"WebGPURenderPassTimestampWrites.h"'],
        'CyberKit::WebGPU::RenderPipelineDescriptor': ['"WebGPURenderPipelineDescriptor.h"'],
        'CyberKit::WebGPU::RequestAdapterOptions': ['"WebGPURequestAdapterOptions.h"'],
        'CyberKit::WebGPU::SamplerBindingLayout': ['"WebGPUSamplerBindingLayout.h"'],
        'CyberKit::WebGPU::SamplerDescriptor': ['"WebGPUSamplerDescriptor.h"'],
        'CyberKit::WebGPU::ShaderModuleDescriptor': ['"WebGPUShaderModuleDescriptor.h"'],
        'CyberKit::WebGPU::StencilFaceState': ['"WebGPUStencilFaceState.h"'],
        'CyberKit::WebGPU::StorageTextureBindingLayout': ['"WebGPUStorageTextureBindingLayout.h"'],
        'CyberKit::WebGPU::SupportedFeatures': ['"WebGPUSupportedFeatures.h"'],
        'CyberKit::WebGPU::SupportedLimits': ['"WebGPUSupportedLimits.h"'],
        'CyberKit::WebGPU::TextureBindingLayout': ['"WebGPUTextureBindingLayout.h"'],
        'CyberKit::WebGPU::TextureDescriptor': ['"WebGPUTextureDescriptor.h"'],
        'CyberKit::WebGPU::TextureViewDescriptor': ['"WebGPUTextureViewDescriptor.h"'],
        'CyberKit::WebGPU::ValidationError': ['"WebGPUValidationError.h"'],
        'CyberKit::WebGPU::VertexAttribute': ['"WebGPUVertexAttribute.h"'],
        'CyberKit::WebGPU::VertexBufferLayout': ['"WebGPUVertexBufferLayout.h"'],
        'CyberKit::WebGPU::VertexState': ['"WebGPUVertexState.h"'],
        'CyberKit::WebScriptMessageHandlerData': ['"WebUserContentControllerDataTypes.h"'],
        'CyberKit::WebUserScriptData': ['"WebUserContentControllerDataTypes.h"'],
        'CyberKit::WebUserStyleSheetData': ['"WebUserContentControllerDataTypes.h"'],
        'webrtc::CyberKitEncodedFrameInfo': ['"RTCCyberKitEncodedFrameInfo.h"', '<CyberCore/LibWebRTCEnumTraits.h>'],
    }

    headers = []
    for header_info in header_infos_and_types['header_infos']:
        headers += header_info['headers']

    for type in header_infos_and_types['types']:
        if type in special_cases:
            headers += special_cases[type]
            continue

        # We assume that we must include a header for a type iff it has a scope
        # resolution operator (::).
        split = type.split('::')
        if len(split) < 2:
            continue

        if split[0] == 'CyberKit' or split[0] == 'IPC':
            headers.append('"%s.h"' % split[1])
        elif split[0] == 'CyberKitLegacy':
        	headers.append('<CyberKitLegacy/%s.h>' % split[1])
        elif split[0] == 'CyberCore':
        	headers.append('<CyberCore/%s.h>' % split[1])
        else:
            headers.append('<%s/%s.h>' % tuple(split[0:2]))

    return headers


def collect_header_conditions_for_receiver(receiver, header_conditions):
    type_conditions = {}
    for parameter in receiver.iterparameters():
        if not parameter.type in type_conditions:
            type_conditions[parameter.type] = []

        if not parameter.condition in type_conditions[parameter.type]:
            type_conditions[parameter.type].append(parameter.condition)

    for parameter in receiver.iterparameters():
        type = parameter.type
        conditions = type_conditions[type]

        argument_encoder_headers = argument_coder_headers_for_type(type)
        if argument_encoder_headers:
            for header in argument_encoder_headers:
                if header not in header_conditions:
                    header_conditions[header] = []
                header_conditions[header].extend(conditions)

        type_headers = headers_for_type(type)
        for header in type_headers:
            if header not in header_conditions:
                header_conditions[header] = []
            header_conditions[header].extend(conditions)

    for message in receiver.messages:
        if message.reply_parameters is not None:
            for reply_parameter in message.reply_parameters:
                type = reply_parameter.type
                argument_encoder_headers = argument_coder_headers_for_type(type)
                if argument_encoder_headers:
                    for header in argument_encoder_headers:
                        if header not in header_conditions:
                            header_conditions[header] = []
                        header_conditions[header].append(message.condition)

                type_headers = headers_for_type(type)
                for header in type_headers:
                    if header not in header_conditions:
                        header_conditions[header] = []
                    header_conditions[header].append(message.condition)

    return header_conditions


def generate_header_includes_from_conditions(header_conditions):
    result = []
    # FIXME(https://bugs.webkit.org/show_bug.cgi?id=241854): NOLINT due to order not as CyberKit expects.
    for header in sorted(header_conditions):
        if header_conditions[header] and not None in header_conditions[header]:
            result.append('#if %s\n' % ' || '.join(sorted(set(header_conditions[header]))))
            result += ['#include %s // NOLINT\n' % header]
            result.append('#endif\n')
        else:
            result += ['#include %s // NOLINT\n' % header]
    return result


def generate_message_handler(receiver):
    header_conditions = {
        '"%s"' % messages_header_filename(receiver): [None],
        '"HandleMessage.h"': [None],
        '"Decoder.h"': [None],
    }

    collect_header_conditions_for_receiver(receiver, header_conditions)

    result = []

    result.append(_license_header)
    result.append('#include "config.h"\n')

    if receiver.condition:
        result.append('#if %s\n' % receiver.condition)

    result.append('#include "%s.h"\n\n' % receiver.name)
    result += generate_header_includes_from_conditions(header_conditions)
    result.append('\n')

    result.append('#if ENABLE(IPC_TESTING_API)\n')
    result.append('#include "JSIPCBinding.h"\n')
    result.append("#endif\n\n")

    result.append('namespace CyberKit {\n\n')

    async_messages = []
    sync_messages = []
    for message in receiver.messages:
        if message.has_attribute(SYNCHRONOUS_ATTRIBUTE):
            sync_messages.append(message)
        else:
            async_messages.append(message)

    def collect_message_statements(messages, message_statement_function):
        result = []
        for condition, messages in itertools.groupby(messages, lambda m: m.condition):
            if condition:
                result.append('#if %s\n' % condition)
            for message in messages:
                result += message_statement_function(receiver, message)
            if condition:
                result.append('#endif\n')
        return result

    async_message_statements = collect_message_statements(async_messages, async_message_statement)
    sync_message_statements = collect_message_statements(sync_messages, sync_message_statement)

    if receiver.has_attribute(STREAM_ATTRIBUTE):
        result.append('void %s::didReceiveStreamMessage(IPC::StreamServerConnection& connection, IPC::Decoder& decoder)\n' % (receiver.name))
        result.append('{\n')
        assert(receiver.has_attribute(NOT_REFCOUNTED_RECEIVER_ATTRIBUTE))
        assert(not receiver.has_attribute(WANTS_DISPATCH_MESSAGE_ATTRIBUTE))
        assert(not receiver.has_attribute(WANTS_ASYNC_DISPATCH_MESSAGE_ATTRIBUTE))
        result += async_message_statements
        result += sync_message_statements
        if (receiver.superclass):
            result.append('    %s::didReceiveStreamMessage(connection, decoder);\n' % (receiver.superclass))
        else:
            result.append('    UNUSED_PARAM(decoder);\n')
            result.append('    UNUSED_PARAM(connection);\n')
            result.append('#if ENABLE(IPC_TESTING_API)\n')
            result.append('    if (connection.connection().ignoreInvalidMessageForTesting())\n')
            result.append('        return;\n')
            result.append('#endif // ENABLE(IPC_TESTING_API)\n')
            result.append('    ASSERT_NOT_REACHED_WITH_MESSAGE("Unhandled stream message %s to %" PRIu64, IPC::description(decoder.messageName()), decoder.destinationID());\n')
        result.append('}\n')
    else:
        receive_variant = receiver.name if receiver.has_attribute(LEGACY_RECEIVER_ATTRIBUTE) else ''
        result.append('void %s::didReceive%sMessage(IPC::Connection& connection, IPC::Decoder& decoder)\n' % (receiver.name, receive_variant))
        result.append('{\n')
        if not (receiver.has_attribute(NOT_REFCOUNTED_RECEIVER_ATTRIBUTE) or receiver.has_attribute(STREAM_ATTRIBUTE)):
            result.append('    Ref protectedThis { *this };\n')
        result += async_message_statements
        if receiver.has_attribute(WANTS_DISPATCH_MESSAGE_ATTRIBUTE) or receiver.has_attribute(WANTS_ASYNC_DISPATCH_MESSAGE_ATTRIBUTE):
            result.append('    if (dispatchMessage(connection, decoder))\n')
            result.append('        return;\n')
        if (receiver.superclass):
            result.append('    %s::didReceive%sMessage(connection, decoder);\n' % (receiver.superclass, receive_variant))
        else:
            result.append('    UNUSED_PARAM(connection);\n')
            result.append('    UNUSED_PARAM(decoder);\n')
            result.append('#if ENABLE(IPC_TESTING_API)\n')
            result.append('    if (connection.ignoreInvalidMessageForTesting())\n')
            result.append('        return;\n')
            result.append('#endif // ENABLE(IPC_TESTING_API)\n')
            result.append('    ASSERT_NOT_REACHED_WITH_MESSAGE("Unhandled message %s to %" PRIu64, IPC::description(decoder.messageName()), decoder.destinationID());\n')
        result.append('}\n')

    if not receiver.has_attribute(STREAM_ATTRIBUTE) and (sync_messages or receiver.has_attribute(WANTS_DISPATCH_MESSAGE_ATTRIBUTE)):
        result.append('\n')
        result.append('bool %s::didReceiveSync%sMessage(IPC::Connection& connection, IPC::Decoder& decoder, UniqueRef<IPC::Encoder>& replyEncoder)\n' % (receiver.name, receiver.name if receiver.has_attribute(LEGACY_RECEIVER_ATTRIBUTE) else ''))
        result.append('{\n')
        if not receiver.has_attribute(NOT_REFCOUNTED_RECEIVER_ATTRIBUTE):
            result.append('    Ref protectedThis { *this };\n')
        result += sync_message_statements
        if receiver.has_attribute(WANTS_DISPATCH_MESSAGE_ATTRIBUTE):
            result.append('    if (dispatchSyncMessage(connection, decoder, replyEncoder))\n')
            result.append('        return true;\n')
        result.append('    UNUSED_PARAM(connection);\n')
        result.append('    UNUSED_PARAM(decoder);\n')
        result.append('    UNUSED_PARAM(replyEncoder);\n')
        result.append('#if ENABLE(IPC_TESTING_API)\n')
        result.append('    if (connection.ignoreInvalidMessageForTesting())\n')
        result.append('        return false;\n')
        result.append('#endif // ENABLE(IPC_TESTING_API)\n')
        result.append('    ASSERT_NOT_REACHED_WITH_MESSAGE("Unhandled synchronous message %s to %" PRIu64, description(decoder.messageName()), decoder.destinationID());\n')
        result.append('    return false;\n')
        result.append('}\n')

    result.append('\n')
    result.append('} // namespace CyberKit\n')

    result.append('\n')
    result.append('#if ENABLE(IPC_TESTING_API)\n')
    result.append('\n')
    result.append('namespace IPC {\n')
    result.append('\n')
    for condition, messages in itertools.groupby(receiver.messages, lambda m: m.condition):
        if condition:
            result.append('#if %s\n' % condition)
        for message in messages:
            result.append('template<> std::optional<JSC::JSValue> jsValueForDecodedMessage<MessageName::%s_%s>(JSC::JSGlobalObject* globalObject, Decoder& decoder)\n' % (receiver.name, message.name))
            result.append('{\n')
            result.append('    return jsValueForDecodedArguments<Messages::%s::%s::%s>(globalObject, decoder);\n' % (receiver.name, message.name, 'Arguments'))
            result.append('}\n')
            has_reply = message.reply_parameters is not None
            if has_reply:
                result.append('template<> std::optional<JSC::JSValue> jsValueForDecodedMessageReply<MessageName::%s_%s>(JSC::JSGlobalObject* globalObject, Decoder& decoder)\n' % (receiver.name, message.name))
                result.append('{\n')
                result.append('    return jsValueForDecodedArguments<Messages::%s::%s::%s>(globalObject, decoder);\n' % (receiver.name, message.name, 'ReplyArguments'))
                result.append('}\n')
        if condition:
            result.append('#endif\n')
    result.append('\n')
    result.append('}\n')
    result.append('\n')
    result.append('#endif\n')
    result.append('\n')

    if receiver.condition:
        result.append('\n')
        result.append('#endif // %s\n' % receiver.condition)

    return ''.join(result)


def generate_message_names_header(receivers):
    result = []
    result.append(_license_header)
    result.append('#pragma once\n')
    result.append('\n')
    result.append('#include <algorithm>\n')
    result.append('#include <wtf/EnumTraits.h>\n')
    result.append('\n')
    result.append('namespace IPC {\n')
    result.append('\n')
    result.append('enum class ReceiverName : uint8_t {')
    result.append('\n    ')
    enums = ['%s = %d' % (e, v) for v, e in enumerate(get_receiver_enumerators(receivers), 1)]
    result.append('\n    , '.join(enums))
    result.append('\n    , Invalid = %d' % (len(enums) + 1))
    result.append('\n};\n')
    result.append('\n')

    result.append('enum class MessageName : uint16_t {\n')
    message_enumerators = get_message_enumerators(receivers)
    seen_synchronous = False
    for (condition, synchronous), enumerators in itertools.groupby(message_enumerators, lambda e: (e.condition(), e.synchronous())):
        if synchronous and not seen_synchronous:
            result.append('    FirstSynchronous,\n')
            result.append('    LastAsynchronous = FirstSynchronous - 1,\n')
            seen_synchronous = True
        if condition:
            result.append('#if %s\n' % condition)
        for enumerator in enumerators:
            result.append('    %s,\n' % enumerator)
        if condition:
            result.append('#endif\n')
    result.append('    Count,\n')
    result.append('    Last = Count - 1\n')
    result.append('};\n')
    result.append('\n')
    result.append('namespace Detail {\n')
    result.append('struct MessageDescription {\n')
    result.append('    const char* const description;\n')
    result.append('    ReceiverName receiverName;\n')
    for fname, _ in sorted(attributes_to_generate_validators.items()):
        result.append('    bool %s : 1;\n' % fname)
    result.append('};\n')
    result.append('\n')
    result.append('extern const MessageDescription messageDescriptions[static_cast<size_t>(MessageName::Count) + 1];\n')
    result.append('}\n')
    result.append('\n')
    fnames = [('ReceiverName', 'receiverName'), ('const char*', 'description')]
    fnames += [('bool', fname) for fname, _ in sorted(attributes_to_generate_validators.items())]
    for returnType, fname in fnames:
        result.append('inline %s %s(MessageName messageName)\n' % (returnType, fname))
        result.append('{\n')
        result.append('    messageName = std::min(messageName, MessageName::Last);\n')
        result.append('    return Detail::messageDescriptions[static_cast<size_t>(messageName)].%s;\n' % fname)
        result.append('}\n')
        result.append('\n')
    result.append('constexpr bool messageIsSync(MessageName name)\n')
    result.append('{\n')
    if seen_synchronous:
        result.append('    return name >= MessageName::FirstSynchronous;\n')
    else:
        result.append('    UNUSED_PARAM(name);\n')
        result.append('    return false;\n')
    result.append('}\n')
    result.append('\n')
    result.append('} // namespace IPC\n')
    result.append('\n')
    result.append('namespace WTF {\n')
    result.append('\n')
    result.append('template<> constexpr bool isValidEnum<IPC::MessageName, void>(std::underlying_type_t<IPC::MessageName> messageName)\n')
    result.append('{\n')
    result.append('    return messageName <= WTF::enumToUnderlyingType(IPC::MessageName::Last);\n')
    result.append('}\n')
    result.append('\n')
    result.append('} // namespace WTF\n')
    return ''.join(result)


def generate_message_names_implementation(receivers):
    result = []
    result.append(_license_header)
    result.append('#include "config.h"\n')
    result.append('#include "MessageNames.h"\n')
    result.append('\n')
    result.append('namespace IPC::Detail {\n')
    result.append('\n')
    result.append('const MessageDescription messageDescriptions[static_cast<size_t>(MessageName::Count) + 1] = {\n')

    message_enumerators = get_message_enumerators(receivers)
    for condition, enumerators in itertools.groupby(message_enumerators, lambda e: e.condition()):
        if condition:
            result.append('#if %s\n' % condition)
        for enumerator in enumerators:
            result.append('    { "%s", ReceiverName::%s' % (enumerator, enumerator.receiver.name))
            for attr_list in sorted(attributes_to_generate_validators.values()):
                value = "true" if set(attr_list).intersection(set(enumerator.messages[0].attributes).union(set(enumerator.receiver.attributes))) else "false"
                result.append(', %s' % value)
            result.append(' },\n')
        if condition:
            result.append('#endif\n')
    result.append('    { "<invalid message name>", ReceiverName::Invalid%s }\n' % (", false" * len(attributes_to_generate_validators)))
    result.append('};\n')
    result.append('\n')
    result.append('} // namespace IPC::Detail\n')
    return ''.join(result)


def generate_js_value_conversion_function(result, receivers, function_name, decoder_function_name, argument_type, predicate=lambda message: True):
    result.append('std::optional<JSC::JSValue> %s(JSC::JSGlobalObject* globalObject, MessageName name, Decoder& decoder)\n' % function_name)
    result.append('{\n')
    result.append('    switch (name) {\n')
    for receiver in receivers:
        if receiver.has_attribute(BUILTIN_ATTRIBUTE):
            continue
        if receiver.condition:
            result.append('#if %s\n' % receiver.condition)
        previous_message_condition = None
        for message in receiver.messages:
            if not predicate(message):
                continue
            if previous_message_condition != message.condition:
                if previous_message_condition:
                    result.append('#endif\n')
                if message.condition:
                    result.append('#if %s\n' % message.condition)
            previous_message_condition = message.condition
            result.append('    case MessageName::%s_%s:\n' % (receiver.name, message.name))
            result.append('        return %s<MessageName::%s_%s>(globalObject, decoder);\n' % (decoder_function_name, receiver.name, message.name))
        if previous_message_condition:
            result.append('#endif\n')
        if receiver.condition:
            result.append('#endif\n')
    result.append('    default:\n')
    result.append('        break;\n')
    result.append('    }\n')
    result.append('    return std::nullopt;\n')
    result.append('}\n')


def generate_js_argument_descriptions(receivers, function_name, arguments_from_message):
    result = []
    result.append('std::optional<Vector<ArgumentDescription>> %s(MessageName name)\n' % function_name)
    result.append('{\n')
    result.append('    switch (name) {\n')
    for receiver in receivers:
        if receiver.has_attribute(BUILTIN_ATTRIBUTE):
            continue
        if receiver.condition:
            result.append('#if %s\n' % receiver.condition)
        previous_message_condition = None
        for message in receiver.messages:
            if message.has_attribute(BUILTIN_ATTRIBUTE):
                continue
            argument_list = arguments_from_message(message)
            if argument_list is None:
                continue
            if previous_message_condition != message.condition:
                if previous_message_condition:
                    result.append('#endif\n')
                if message.condition:
                    result.append('#if %s\n' % message.condition)
            previous_message_condition = message.condition
            result.append('    case MessageName::%s_%s:\n' % (receiver.name, message.name))

            if not len(argument_list):
                result.append('        return Vector<ArgumentDescription> { };\n')
                continue

            result.append('        return Vector<ArgumentDescription> {\n')
            for argument in argument_list:
                argument_type = argument.type
                enum_type = None
                is_optional = False
                if argument.kind.startswith('enum:'):
                    enum_type = '"%s"' % argument_type
                    argument_type = argument.kind[5:]
                if argument_type.startswith('std::optional<') and argument_type.endswith('>'):
                    argument_type = argument_type[14:-1]
                    is_optional = True
                result.append('            { "%s", "%s", %s, %s },\n' % (argument.name, argument_type, enum_type or 'nullptr', 'true' if is_optional else 'false'))
            result.append('        };\n')
        if previous_message_condition:
            result.append('#endif\n')
        if receiver.condition:
            result.append('#endif\n')
    result.append('    default:\n')
    result.append('        break;\n')
    result.append('    }\n')
    result.append('    return std::nullopt;\n')
    result.append('}\n')
    return result


def generate_message_argument_description_implementation(receivers, receiver_headers):
    result = []
    result.append(_license_header)
    result.append('#include "config.h"\n')
    result.append('#include "MessageArgumentDescriptions.h"\n')
    result.append('\n')
    result.append('#if ENABLE(IPC_TESTING_API) || !LOG_DISABLED\n')
    result.append('\n')
    all_headers = ['"JSIPCBinding.h"']
    for identifier in serialized_identifiers():
        for header in headers_for_type(identifier):
            all_headers.append(header)
    all_headers = sorted(list(dict.fromkeys(all_headers)))
    for header in all_headers:
        conditions = conditions_for_header(header)
        if conditions and None not in conditions:
            result.append('#if %s\n' % ' || '.join(sorted(set(conditions))))
            result.append('#include %s\n' % header)
            result.append('#endif\n')
        else:
            result.append('#include %s\n' % header)

    for receiver in receivers:
        if receiver.has_attribute(BUILTIN_ATTRIBUTE):
            continue
        if receiver.condition:
            result.append('#if %s\n' % receiver.condition)
        header_conditions = {
            '"%s"' % messages_header_filename(receiver): [None]
        }
        result += generate_header_includes_from_conditions(header_conditions)
        if receiver.condition:
            result.append('#endif\n')

    result.append('\n')

    result.append('namespace IPC {\n')
    result.append('\n')
    result.append('#if ENABLE(IPC_TESTING_API)\n')
    result.append('\n')

    generate_js_value_conversion_function(result, receivers, 'jsValueForArguments', 'jsValueForDecodedMessage', 'Arguments')

    result.append('\n')

    generate_js_value_conversion_function(result, receivers, 'jsValueForReplyArguments', 'jsValueForDecodedMessageReply', 'ReplyArguments', lambda message: message.reply_parameters is not None)

    result.append('\n')
    result.append('Vector<ASCIILiteral> serializedIdentifiers()\n')
    result.append('{\n')

    identifier_headers = {}
    for identifier in serialized_identifiers():
        assert(len(headers_for_type(identifier)) == 1)
        identifier_headers[identifier] = headers_for_type(identifier)[0]

    for identifier in serialized_identifiers():
        conditions = conditions_for_header(identifier_headers[identifier])
        statement = '    static_assert(sizeof(uint64_t) == sizeof(' + identifier + '));\n'
        if conditions and None not in conditions:
            result.append('#if %s\n' % ' || '.join(sorted(set(conditions))))
            result.append(statement)
            result.append('#endif\n')
        else:
            result.append(statement)

    result.append('    return {\n')

    for identifier in serialized_identifiers():
        conditions = conditions_for_header(identifier_headers[identifier])
        statement = '        "' + identifier + '"_s,\n'
        if conditions and None not in conditions:
            result.append('#if %s\n' % ' || '.join(sorted(set(conditions))))
            result.append(statement)
            result.append('#endif\n')
        else:
            result.append(statement)

    result.append('    };\n')
    result.append('}\n')

    result.append('\n')
    result.append('#endif // ENABLE(IPC_TESTING_API)\n')
    result.append('\n')

    result += generate_js_argument_descriptions(receivers, 'messageArgumentDescriptions', lambda message: message.parameters)

    result.append('\n')

    result += generate_js_argument_descriptions(receivers, 'messageReplyArgumentDescriptions', lambda message: message.reply_parameters)

    result.append('\n')

    result.append('} // namespace CyberKit\n')
    result.append('\n')
    result.append('#endif // ENABLE(IPC_TESTING_API) || !LOG_DISABLED\n')
    return ''.join(result)
