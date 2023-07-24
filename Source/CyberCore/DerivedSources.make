# Copyright (C) 2006-2020 Apple Inc. All rights reserved.
# Copyright (C) 2006 Samuel Weinig <sam.weinig@gmail.com>
# Copyright (C) 2009 Cameron McCormack <cam@mcc.id.au>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1.  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
# 3.  Neither the name of Apple Inc. ("Apple") nor the names of
#     its contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

PYTHON = python
PERL = perl
RUBY = ruby
DELETE = rm -f

ifneq ($(SDKROOT),)
    SDK_FLAGS = -isysroot $(SDKROOT)
endif

ifeq ($(USE_LLVM_TARGET_TRIPLES_FOR_CLANG),YES)
    WK_CURRENT_ARCH = $(word 1, $(ARCHS))
    TARGET_TRIPLE_FLAGS = -target $(WK_CURRENT_ARCH)-$(LLVM_TARGET_TRIPLE_VENDOR)-$(LLVM_TARGET_TRIPLE_OS_VERSION)$(LLVM_TARGET_TRIPLE_SUFFIX)
endif

FRAMEWORK_FLAGS := $(shell echo $(BUILT_PRODUCTS_DIR) $(FRAMEWORK_SEARCH_PATHS) $(SYSTEM_FRAMEWORK_SEARCH_PATHS) | $(PERL) -e 'print "-F " . join(" -F ", split(" ", <>));')
HEADER_FLAGS := $(shell echo $(BUILT_PRODUCTS_DIR) $(HEADER_SEARCH_PATHS) $(SYSTEM_HEADER_SEARCH_PATHS) | $(PERL) -e 'print "-I" . join(" -I", split(" ", <>));')
FEATURE_AND_PLATFORM_DEFINES := $(shell $(CC) -std=gnu++1z -x c++ -E -P -dM $(SDK_FLAGS) $(TARGET_TRIPLE_FLAGS) $(FRAMEWORK_FLAGS) $(HEADER_FLAGS) -include "wtf/Platform.h" /dev/null | $(PERL) -ne "print if s/\#define ((HAVE_|USE_|ENABLE_|WTF_PLATFORM_)\w+) 1/\1/")

# FIXME: This should list Platform.h and all the things it includes. Could do that by using the -MD flag in the CC line above.
FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES = DerivedSources.make

# --------

VPATH = \
    $(CyberCore) \
    $(CyberCore)/Modules/airplay \
    $(CyberCore)/Modules/applepay \
    $(CyberCore)/Modules/applepay/paymentrequest \
    $(CyberCore)/Modules/async-clipboard \
    $(CyberCore)/Modules/beacon \
    $(CyberCore)/Modules/cache \
    $(CyberCore)/Modules/credentialmanagement \
    $(CyberCore)/Modules/encryptedmedia \
    $(CyberCore)/Modules/encryptedmedia/legacy \
    $(CyberCore)/Modules/entriesapi \
    $(CyberCore)/Modules/fetch \
    $(CyberCore)/Modules/gamepad \
    $(CyberCore)/Modules/geolocation \
    $(CyberCore)/Modules/highlight \
    $(CyberCore)/Modules/indexeddb \
    $(CyberCore)/Modules/indieui \
    $(CyberCore)/Modules/mediacapabilities \
    $(CyberCore)/Modules/mediacontrols \
    $(CyberCore)/Modules/mediarecorder \
    $(CyberCore)/Modules/mediasession \
    $(CyberCore)/Modules/mediasource \
    $(CyberCore)/Modules/mediastream \
    $(CyberCore)/Modules/notifications \
    $(CyberCore)/Modules/paymentrequest \
    $(CyberCore)/Modules/pictureinpicture \
    $(CyberCore)/Modules/plugins \
    $(CyberCore)/Modules/quota \
	$(CyberCore)/Modules/remoteplayback \
    $(CyberCore)/Modules/speech \
    $(CyberCore)/Modules/streams \
    $(CyberCore)/Modules/webaudio \
    $(CyberCore)/Modules/webauthn \
    $(CyberCore)/Modules/webdatabase \
    $(CyberCore)/Modules/webdriver \
    $(CyberCore)/Modules/webgpu \
    $(CyberCore)/Modules/websockets \
    $(CyberCore)/Modules/webxr \
    $(CyberCore)/accessibility \
    $(CyberCore)/animation \
    $(CyberCore)/bindings/js \
    $(CyberCore)/crypto \
    $(CyberCore)/crypto/keys \
    $(CyberCore)/crypto/parameters \
    $(CyberCore)/css \
    $(CyberCore)/css/typedom \
    $(CyberCore)/dom \
    $(CyberCore)/editing \
    $(CyberCore)/fileapi \
    $(CyberCore)/html \
    $(CyberCore)/html/canvas \
    $(CyberCore)/html/shadow \
    $(CyberCore)/html/track \
    $(CyberCore)/inspector \
    $(CyberCore)/loader/appcache \
    $(CyberCore)/mathml \
    $(CyberCore)/page \
    $(CyberCore)/platform/network \
    $(CyberCore)/plugins \
    $(CyberCore)/storage \
    $(CyberCore)/svg \
    $(CyberCore)/testing \
    $(CyberCore)/websockets \
    $(CyberCore)/workers \
    $(CyberCore)/workers/service \
    $(CyberCore)/worklets \
    $(CyberCore)/xml \
#

JS_BINDING_IDLS = \
    $(CyberCore)/Modules/airplay/CyberKitPlaybackTargetAvailabilityEvent.idl \
    $(CyberCore)/Modules/applepay/ApplePayCancelEvent.idl \
    $(CyberCore)/Modules/applepay/ApplePayContactField.idl \
    $(CyberCore)/Modules/applepay/ApplePayError.idl \
    $(CyberCore)/Modules/applepay/ApplePayErrorCode.idl \
    $(CyberCore)/Modules/applepay/ApplePayErrorContactField.idl \
    $(CyberCore)/Modules/applepay/ApplePayInstallmentConfiguration.idl \
	$(CyberCore)/Modules/applepay/ApplePayInstallmentItem.idl \
	$(CyberCore)/Modules/applepay/ApplePayInstallmentItemType.idl \
	$(CyberCore)/Modules/applepay/ApplePayInstallmentRetailChannel.idl \
    $(CyberCore)/Modules/applepay/ApplePayLineItem.idl \
    $(CyberCore)/Modules/applepay/ApplePayMerchantCapability.idl \
    $(CyberCore)/Modules/applepay/ApplePayPayment.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentAuthorizationResult.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentAuthorizedEvent.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentContact.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentMethod.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentMethodSelectedEvent.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentMethodType.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentMethodUpdate.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentPass.idl \
    $(CyberCore)/Modules/applepay/ApplePayPaymentRequest.idl \
    $(CyberCore)/Modules/applepay/ApplePayRequestBase.idl \
    $(CyberCore)/Modules/applepay/ApplePaySession.idl \
    $(CyberCore)/Modules/applepay/ApplePaySessionError.idl \
    $(CyberCore)/Modules/applepay/ApplePaySetup.idl \
    $(CyberCore)/Modules/applepay/ApplePaySetupConfiguration.idl \
    $(CyberCore)/Modules/applepay/ApplePaySetupFeature.idl \
    $(CyberCore)/Modules/applepay/ApplePaySetupFeatureState.idl \
    $(CyberCore)/Modules/applepay/ApplePaySetupFeatureType.idl \
    $(CyberCore)/Modules/applepay/ApplePayShippingContactSelectedEvent.idl \
    $(CyberCore)/Modules/applepay/ApplePayShippingContactUpdate.idl \
    $(CyberCore)/Modules/applepay/ApplePayShippingMethod.idl \
    $(CyberCore)/Modules/applepay/ApplePayShippingMethodSelectedEvent.idl \
    $(CyberCore)/Modules/applepay/ApplePayShippingMethodUpdate.idl \
    $(CyberCore)/Modules/applepay/ApplePayValidateMerchantEvent.idl \
    $(CyberCore)/Modules/applepay/paymentrequest/ApplePayModifier.idl \
    $(CyberCore)/Modules/applepay/paymentrequest/ApplePayRequest.idl \
    $(CyberCore)/Modules/async-clipboard/Clipboard.idl \
    $(CyberCore)/Modules/async-clipboard/ClipboardItem.idl \
    $(CyberCore)/Modules/async-clipboard/NavigatorClipboard.idl \
    $(CyberCore)/Modules/beacon/NavigatorBeacon.idl \
    $(CyberCore)/Modules/cache/CacheQueryOptions.idl \
    $(CyberCore)/Modules/cache/DOMCache.idl \
    $(CyberCore)/Modules/cache/DOMCacheStorage.idl \
    $(CyberCore)/Modules/cache/DOMWindowCaches.idl \
    $(CyberCore)/Modules/cache/WorkerGlobalScopeCaches.idl \
    $(CyberCore)/Modules/credentialmanagement/BasicCredential.idl \
    $(CyberCore)/Modules/credentialmanagement/CredentialCreationOptions.idl \
    $(CyberCore)/Modules/credentialmanagement/CredentialRequestOptions.idl \
    $(CyberCore)/Modules/credentialmanagement/CredentialsContainer.idl \
    $(CyberCore)/Modules/credentialmanagement/NavigatorCredentials.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeyEncryptionScheme.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeyMessageEvent.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeySession.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeySessionType.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeyStatusMap.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeySystemAccess.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeySystemConfiguration.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeySystemMediaCapability.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeys.idl \
    $(CyberCore)/Modules/encryptedmedia/MediaKeysRequirement.idl \
    $(CyberCore)/Modules/encryptedmedia/NavigatorEME.idl \
    $(CyberCore)/Modules/encryptedmedia/legacy/CyberKitMediaKeyMessageEvent.idl \
    $(CyberCore)/Modules/encryptedmedia/legacy/CyberKitMediaKeyNeededEvent.idl \
    $(CyberCore)/Modules/encryptedmedia/legacy/CyberKitMediaKeySession.idl \
    $(CyberCore)/Modules/encryptedmedia/legacy/CyberKitMediaKeys.idl \
    $(CyberCore)/Modules/entriesapi/DOMFileSystem.idl \
    $(CyberCore)/Modules/entriesapi/ErrorCallback.idl \
    $(CyberCore)/Modules/entriesapi/FileCallback.idl \
    $(CyberCore)/Modules/entriesapi/FileSystemDirectoryEntry.idl \
    $(CyberCore)/Modules/entriesapi/FileSystemDirectoryReader.idl \
    $(CyberCore)/Modules/entriesapi/FileSystemEntriesCallback.idl \
    $(CyberCore)/Modules/entriesapi/FileSystemEntry.idl \
    $(CyberCore)/Modules/entriesapi/FileSystemEntryCallback.idl \
    $(CyberCore)/Modules/entriesapi/FileSystemFileEntry.idl \
    $(CyberCore)/Modules/entriesapi/HTMLInputElementEntriesAPI.idl \
    $(CyberCore)/Modules/fetch/DOMWindowFetch.idl \
    $(CyberCore)/Modules/fetch/FetchBody.idl \
    $(CyberCore)/Modules/fetch/FetchHeaders.idl \
    $(CyberCore)/Modules/fetch/FetchReferrerPolicy.idl \
    $(CyberCore)/Modules/fetch/FetchRequest.idl \
    $(CyberCore)/Modules/fetch/FetchRequestCache.idl \
    $(CyberCore)/Modules/fetch/FetchRequestCredentials.idl \
    $(CyberCore)/Modules/fetch/FetchRequestInit.idl \
    $(CyberCore)/Modules/fetch/FetchRequestMode.idl \
    $(CyberCore)/Modules/fetch/FetchRequestRedirect.idl \
    $(CyberCore)/Modules/fetch/FetchResponse.idl \
    $(CyberCore)/Modules/fetch/WorkerGlobalScopeFetch.idl \
    $(CyberCore)/Modules/gamepad/Gamepad.idl \
    $(CyberCore)/Modules/gamepad/GamepadButton.idl \
    $(CyberCore)/Modules/gamepad/GamepadEvent.idl \
    $(CyberCore)/Modules/gamepad/NavigatorGamepad.idl \
    $(CyberCore)/Modules/geolocation/Geolocation.idl \
    $(CyberCore)/Modules/geolocation/GeolocationCoordinates.idl \
    $(CyberCore)/Modules/geolocation/GeolocationPosition.idl \
    $(CyberCore)/Modules/geolocation/GeolocationPositionError.idl \
    $(CyberCore)/Modules/geolocation/NavigatorGeolocation.idl \
    $(CyberCore)/Modules/geolocation/PositionCallback.idl \
    $(CyberCore)/Modules/geolocation/PositionErrorCallback.idl \
    $(CyberCore)/Modules/geolocation/PositionOptions.idl \
    $(CyberCore)/Modules/highlight/HighlightMap.idl \
    $(CyberCore)/Modules/highlight/HighlightRangeGroup.idl \
    $(CyberCore)/Modules/indexeddb/DOMWindowIndexedDatabase.idl \
    $(CyberCore)/Modules/indexeddb/IDBCursor.idl \
    $(CyberCore)/Modules/indexeddb/IDBCursorDirection.idl \
    $(CyberCore)/Modules/indexeddb/IDBCursorWithValue.idl \
    $(CyberCore)/Modules/indexeddb/IDBDatabase.idl \
    $(CyberCore)/Modules/indexeddb/IDBFactory.idl \
    $(CyberCore)/Modules/indexeddb/IDBIndex.idl \
    $(CyberCore)/Modules/indexeddb/IDBKeyRange.idl \
    $(CyberCore)/Modules/indexeddb/IDBObjectStore.idl \
    $(CyberCore)/Modules/indexeddb/IDBOpenDBRequest.idl \
    $(CyberCore)/Modules/indexeddb/IDBRequest.idl \
    $(CyberCore)/Modules/indexeddb/IDBTransaction.idl \
    $(CyberCore)/Modules/indexeddb/IDBTransactionMode.idl \
    $(CyberCore)/Modules/indexeddb/IDBVersionChangeEvent.idl \
    $(CyberCore)/Modules/indexeddb/WorkerGlobalScopeIndexedDatabase.idl \
    $(CyberCore)/Modules/mediacapabilities/AudioConfiguration.idl \
    $(CyberCore)/Modules/mediacapabilities/ColorGamut.idl \
    $(CyberCore)/Modules/mediacapabilities/HdrMetadataType.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaCapabilities.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaCapabilitiesDecodingInfo.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaCapabilitiesEncodingInfo.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaCapabilitiesInfo.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaConfiguration.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaDecodingConfiguration.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaDecodingType.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaEncodingConfiguration.idl \
    $(CyberCore)/Modules/mediacapabilities/MediaEncodingType.idl \
    $(CyberCore)/Modules/mediacapabilities/NavigatorMediaCapabilities.idl \
    $(CyberCore)/Modules/mediacapabilities/TransferFunction.idl \
    $(CyberCore)/Modules/mediacapabilities/VideoConfiguration.idl \
    $(CyberCore)/Modules/mediacontrols/MediaControlsHost.idl \
    $(CyberCore)/Modules/mediarecorder/BlobEvent.idl \
    $(CyberCore)/Modules/mediarecorder/MediaRecorder.idl \
    $(CyberCore)/Modules/mediarecorder/MediaRecorderErrorEvent.idl \
    $(CyberCore)/Modules/mediasession/HTMLMediaElementMediaSession.idl \
    $(CyberCore)/Modules/mediasession/MediaRemoteControls.idl \
    $(CyberCore)/Modules/mediasession/MediaSession.idl \
    $(CyberCore)/Modules/mediasource/AudioTrackMediaSource.idl \
    $(CyberCore)/Modules/mediasource/DOMURLMediaSource.idl \
    $(CyberCore)/Modules/mediasource/MediaSource.idl \
    $(CyberCore)/Modules/mediasource/SourceBuffer.idl \
    $(CyberCore)/Modules/mediasource/SourceBufferList.idl \
    $(CyberCore)/Modules/mediasource/TextTrackMediaSource.idl \
    $(CyberCore)/Modules/mediasource/VideoPlaybackQuality.idl \
    $(CyberCore)/Modules/mediasource/VideoTrackMediaSource.idl \
    $(CyberCore)/Modules/mediastream/CanvasCaptureMediaStreamTrack.idl \
    $(CyberCore)/Modules/mediastream/DoubleRange.idl \
    $(CyberCore)/Modules/mediastream/LongRange.idl \
    $(CyberCore)/Modules/mediastream/MediaDeviceInfo.idl \
    $(CyberCore)/Modules/mediastream/MediaDevices.idl \
    $(CyberCore)/Modules/mediastream/MediaStream.idl \
    $(CyberCore)/Modules/mediastream/MediaStreamTrack.idl \
    $(CyberCore)/Modules/mediastream/MediaStreamTrackEvent.idl \
    $(CyberCore)/Modules/mediastream/MediaTrackConstraints.idl \
    $(CyberCore)/Modules/mediastream/MediaTrackSupportedConstraints.idl \
    $(CyberCore)/Modules/mediastream/NavigatorMediaDevices.idl \
    $(CyberCore)/Modules/mediastream/OverconstrainedError.idl \
    $(CyberCore)/Modules/mediastream/OverconstrainedErrorEvent.idl \
    $(CyberCore)/Modules/mediastream/RTCAnswerOptions.idl \
    $(CyberCore)/Modules/mediastream/RTCCertificate.idl \
    $(CyberCore)/Modules/mediastream/RTCConfiguration.idl \
    $(CyberCore)/Modules/mediastream/RTCDTMFSender.idl \
    $(CyberCore)/Modules/mediastream/RTCDTMFToneChangeEvent.idl \
    $(CyberCore)/Modules/mediastream/RTCDataChannel.idl \
    $(CyberCore)/Modules/mediastream/RTCDataChannelEvent.idl \
    $(CyberCore)/Modules/mediastream/RTCDegradationPreference.idl \
    $(CyberCore)/Modules/mediastream/RTCDtxStatus.idl \
    $(CyberCore)/Modules/mediastream/RTCIceCandidate.idl \
    $(CyberCore)/Modules/mediastream/RTCIceConnectionState.idl \
    $(CyberCore)/Modules/mediastream/RTCIceGatheringState.idl \
    $(CyberCore)/Modules/mediastream/RTCIceServer.idl \
    $(CyberCore)/Modules/mediastream/RTCIceTransport.idl \
    $(CyberCore)/Modules/mediastream/RTCIceTransportState.idl \
    $(CyberCore)/Modules/mediastream/RTCOfferAnswerOptions.idl \
    $(CyberCore)/Modules/mediastream/RTCOfferOptions.idl \
    $(CyberCore)/Modules/mediastream/RTCPeerConnection.idl \
    $(CyberCore)/Modules/mediastream/RTCPeerConnectionIceEvent.idl \
    $(CyberCore)/Modules/mediastream/RTCPeerConnectionIceErrorEvent.idl \
    $(CyberCore)/Modules/mediastream/RTCPeerConnectionState.idl \
    $(CyberCore)/Modules/mediastream/RTCPriorityType.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpCapabilities.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpCodecCapability.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpCodecParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpContributingSource.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpEncodingParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpFecParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpHeaderExtensionParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpReceiver.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpRtxParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpSendParameters.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpSender.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpSynchronizationSource.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpTransceiver.idl \
    $(CyberCore)/Modules/mediastream/RTCRtpTransceiverDirection.idl \
    $(CyberCore)/Modules/mediastream/RTCSessionDescription.idl \
    $(CyberCore)/Modules/mediastream/RTCSignalingState.idl \
    $(CyberCore)/Modules/mediastream/RTCStatsReport.idl \
    $(CyberCore)/Modules/mediastream/RTCTrackEvent.idl \
    $(CyberCore)/Modules/notifications/Notification.idl \
    $(CyberCore)/Modules/notifications/NotificationPermission.idl \
    $(CyberCore)/Modules/notifications/NotificationPermissionCallback.idl \
    $(CyberCore)/Modules/paymentrequest/AddressErrors.idl \
    $(CyberCore)/Modules/paymentrequest/MerchantValidationEvent.idl \
    $(CyberCore)/Modules/paymentrequest/PayerErrorFields.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentAddress.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentComplete.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentCurrencyAmount.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentDetailsBase.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentDetailsInit.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentDetailsModifier.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentDetailsUpdate.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentItem.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentMethodChangeEvent.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentMethodData.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentOptions.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentRequest.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentRequestUpdateEvent.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentRequestUpdateEventInit.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentResponse.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentShippingOption.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentShippingType.idl \
    $(CyberCore)/Modules/paymentrequest/PaymentValidationErrors.idl \
    $(CyberCore)/Modules/pictureinpicture/DocumentPictureInPicture.idl \
    $(CyberCore)/Modules/pictureinpicture/EnterPictureInPictureEvent.idl \
    $(CyberCore)/Modules/pictureinpicture/HTMLVideoElementPictureInPicture.idl \
    $(CyberCore)/Modules/pictureinpicture/PictureInPictureWindow.idl \
    $(CyberCore)/Modules/plugins/QuickTimePluginReplacement.idl \
    $(CyberCore)/Modules/quota/DOMWindowQuota.idl \
    $(CyberCore)/Modules/quota/NavigatorStorageQuota.idl \
    $(CyberCore)/Modules/quota/StorageErrorCallback.idl \
    $(CyberCore)/Modules/quota/StorageInfo.idl \
    $(CyberCore)/Modules/quota/StorageQuota.idl \
    $(CyberCore)/Modules/quota/StorageQuotaCallback.idl \
    $(CyberCore)/Modules/quota/StorageUsageCallback.idl \
    $(CyberCore)/Modules/quota/WorkerNavigatorStorageQuota.idl \
	$(CyberCore)/Modules/remoteplayback/HTMLMediaElementRemotePlayback.idl \
	$(CyberCore)/Modules/remoteplayback/RemotePlayback.idl \
	$(CyberCore)/Modules/remoteplayback/RemotePlaybackAvailabilityCallback.idl \
    $(CyberCore)/Modules/speech/DOMWindowSpeechSynthesis.idl \
    $(CyberCore)/Modules/speech/SpeechSynthesis.idl \
    $(CyberCore)/Modules/speech/SpeechSynthesisEvent.idl \
    $(CyberCore)/Modules/speech/SpeechSynthesisUtterance.idl \
    $(CyberCore)/Modules/speech/SpeechSynthesisVoice.idl \
    $(CyberCore)/Modules/streams/ByteLengthQueuingStrategy.idl \
    $(CyberCore)/Modules/streams/CountQueuingStrategy.idl \
    $(CyberCore)/Modules/streams/ReadableByteStreamController.idl \
    $(CyberCore)/Modules/streams/ReadableStream.idl \
    $(CyberCore)/Modules/streams/ReadableStreamBYOBReader.idl \
    $(CyberCore)/Modules/streams/ReadableStreamBYOBRequest.idl \
    $(CyberCore)/Modules/streams/ReadableStreamDefaultController.idl \
    $(CyberCore)/Modules/streams/ReadableStreamDefaultReader.idl \
    $(CyberCore)/Modules/streams/ReadableStreamSink.idl \
    $(CyberCore)/Modules/streams/ReadableStreamSource.idl \
    $(CyberCore)/Modules/streams/WritableStream.idl \
    $(CyberCore)/Modules/streams/WritableStreamDefaultController.idl \
    $(CyberCore)/Modules/streams/WritableStreamDefaultWriter.idl \
    $(CyberCore)/Modules/webaudio/AnalyserNode.idl \
    $(CyberCore)/Modules/webaudio/AnalyserOptions.idl \
    $(CyberCore)/Modules/webaudio/AudioBuffer.idl \
    $(CyberCore)/Modules/webaudio/AudioBufferCallback.idl \
    $(CyberCore)/Modules/webaudio/AudioBufferOptions.idl \
    $(CyberCore)/Modules/webaudio/AudioBufferSourceNode.idl \
    $(CyberCore)/Modules/webaudio/AudioBufferSourceOptions.idl \
    $(CyberCore)/Modules/webaudio/AudioContext.idl \
    $(CyberCore)/Modules/webaudio/AudioContextLatencyCategory.idl \
    $(CyberCore)/Modules/webaudio/AudioContextOptions.idl \
    $(CyberCore)/Modules/webaudio/AudioContextState.idl \
    $(CyberCore)/Modules/webaudio/AudioDestinationNode.idl \
    $(CyberCore)/Modules/webaudio/AudioListener.idl \
    $(CyberCore)/Modules/webaudio/AudioNode.idl \
    $(CyberCore)/Modules/webaudio/AudioNodeOptions.idl \
    $(CyberCore)/Modules/webaudio/AudioParam.idl \
    $(CyberCore)/Modules/webaudio/AudioProcessingEvent.idl \
    $(CyberCore)/Modules/webaudio/AudioProcessingEventInit.idl \
    $(CyberCore)/Modules/webaudio/AudioScheduledSourceNode.idl \
    $(CyberCore)/Modules/webaudio/AudioTimestamp.idl \
    $(CyberCore)/Modules/webaudio/BaseAudioContext.idl \
    $(CyberCore)/Modules/webaudio/BiquadFilterNode.idl \
    $(CyberCore)/Modules/webaudio/BiquadFilterOptions.idl \
    $(CyberCore)/Modules/webaudio/BiquadFilterType.idl \
    $(CyberCore)/Modules/webaudio/ChannelCountMode.idl \
    $(CyberCore)/Modules/webaudio/ChannelInterpretation.idl \
    $(CyberCore)/Modules/webaudio/ChannelMergerNode.idl \
    $(CyberCore)/Modules/webaudio/ChannelMergerOptions.idl \
    $(CyberCore)/Modules/webaudio/ChannelSplitterNode.idl \
    $(CyberCore)/Modules/webaudio/ChannelSplitterOptions.idl \
    $(CyberCore)/Modules/webaudio/ConstantSourceNode.idl \
    $(CyberCore)/Modules/webaudio/ConstantSourceOptions.idl \
    $(CyberCore)/Modules/webaudio/ConvolverNode.idl \
    $(CyberCore)/Modules/webaudio/ConvolverOptions.idl \
    $(CyberCore)/Modules/webaudio/DelayNode.idl \
    $(CyberCore)/Modules/webaudio/DelayOptions.idl \
    $(CyberCore)/Modules/webaudio/DistanceModelType.idl \
    $(CyberCore)/Modules/webaudio/DynamicsCompressorNode.idl \
    $(CyberCore)/Modules/webaudio/DynamicsCompressorOptions.idl \
    $(CyberCore)/Modules/webaudio/GainNode.idl \
    $(CyberCore)/Modules/webaudio/GainOptions.idl \
    $(CyberCore)/Modules/webaudio/MediaElementAudioSourceNode.idl \
    $(CyberCore)/Modules/webaudio/MediaElementAudioSourceOptions.idl \
    $(CyberCore)/Modules/webaudio/MediaStreamAudioDestinationNode.idl \
    $(CyberCore)/Modules/webaudio/MediaStreamAudioSourceNode.idl \
    $(CyberCore)/Modules/webaudio/MediaStreamAudioSourceOptions.idl \
    $(CyberCore)/Modules/webaudio/OfflineAudioCompletionEvent.idl \
    $(CyberCore)/Modules/webaudio/OfflineAudioCompletionEventInit.idl \
    $(CyberCore)/Modules/webaudio/OfflineAudioContext.idl \
    $(CyberCore)/Modules/webaudio/OfflineAudioContextOptions.idl \
    $(CyberCore)/Modules/webaudio/OscillatorNode.idl \
    $(CyberCore)/Modules/webaudio/OscillatorOptions.idl \
    $(CyberCore)/Modules/webaudio/OscillatorType.idl \
    $(CyberCore)/Modules/webaudio/OverSampleType.idl \
    $(CyberCore)/Modules/webaudio/PannerNode.idl \
    $(CyberCore)/Modules/webaudio/PannerOptions.idl \
    $(CyberCore)/Modules/webaudio/PanningModelType.idl \
    $(CyberCore)/Modules/webaudio/PeriodicWave.idl \
    $(CyberCore)/Modules/webaudio/PeriodicWaveConstraints.idl \
    $(CyberCore)/Modules/webaudio/PeriodicWaveOptions.idl \
    $(CyberCore)/Modules/webaudio/ScriptProcessorNode.idl \
    $(CyberCore)/Modules/webaudio/StereoPannerNode.idl \
	$(CyberCore)/Modules/webaudio/StereoPannerOptions.idl \
    $(CyberCore)/Modules/webaudio/WaveShaperNode.idl \
    $(CyberCore)/Modules/webaudio/WaveShaperOptions.idl \
    $(CyberCore)/Modules/webaudio/CyberKitAudioBufferSourceNode.idl \
    $(CyberCore)/Modules/webaudio/CyberKitAudioContext.idl \
    $(CyberCore)/Modules/webaudio/CyberKitAudioListener.idl \
    $(CyberCore)/Modules/webaudio/CyberKitAudioPannerNode.idl \
    $(CyberCore)/Modules/webaudio/CyberKitDynamicsCompressorNode.idl \
    $(CyberCore)/Modules/webaudio/CyberKitOfflineAudioContext.idl \
    $(CyberCore)/Modules/webaudio/CyberKitOscillatorNode.idl \
    $(CyberCore)/Modules/webauthn/AttestationConveyancePreference.idl \
    $(CyberCore)/Modules/webauthn/AuthenticationExtensionsClientInputs.idl \
    $(CyberCore)/Modules/webauthn/AuthenticationExtensionsClientOutputs.idl \
    $(CyberCore)/Modules/webauthn/AuthenticatorAssertionResponse.idl \
    $(CyberCore)/Modules/webauthn/AuthenticatorAttestationResponse.idl \
    $(CyberCore)/Modules/webauthn/AuthenticatorResponse.idl \
    $(CyberCore)/Modules/webauthn/AuthenticatorTransport.idl \
    $(CyberCore)/Modules/webauthn/PublicKeyCredential.idl \
    $(CyberCore)/Modules/webauthn/PublicKeyCredentialCreationOptions.idl \
    $(CyberCore)/Modules/webauthn/PublicKeyCredentialDescriptor.idl \
    $(CyberCore)/Modules/webauthn/PublicKeyCredentialRequestOptions.idl \
    $(CyberCore)/Modules/webauthn/PublicKeyCredentialType.idl \
    $(CyberCore)/Modules/webauthn/UserVerificationRequirement.idl \
    $(CyberCore)/Modules/webdatabase/DOMWindowWebDatabase.idl \
    $(CyberCore)/Modules/webdatabase/Database.idl \
    $(CyberCore)/Modules/webdatabase/DatabaseCallback.idl \
    $(CyberCore)/Modules/webdatabase/SQLError.idl \
    $(CyberCore)/Modules/webdatabase/SQLResultSet.idl \
    $(CyberCore)/Modules/webdatabase/SQLResultSetRowList.idl \
    $(CyberCore)/Modules/webdatabase/SQLStatementCallback.idl \
    $(CyberCore)/Modules/webdatabase/SQLStatementErrorCallback.idl \
    $(CyberCore)/Modules/webdatabase/SQLTransaction.idl \
    $(CyberCore)/Modules/webdatabase/SQLTransactionCallback.idl \
    $(CyberCore)/Modules/webdatabase/SQLTransactionErrorCallback.idl \
    $(CyberCore)/Modules/webdriver/NavigatorWebDriver.idl \
    $(CyberCore)/Modules/webgpu/GPUCanvasContext.idl \
    $(CyberCore)/Modules/webgpu/GPUColor.idl \
    $(CyberCore)/Modules/webgpu/GPUColorStateDescriptor.idl \
    $(CyberCore)/Modules/webgpu/GPUColorWrite.idl \
    $(CyberCore)/Modules/webgpu/GPUBindGroupLayoutBinding.idl \
    $(CyberCore)/Modules/webgpu/GPUBindGroupLayoutDescriptor.idl \
    $(CyberCore)/Modules/webgpu/GPUBlendDescriptor.idl \
    $(CyberCore)/Modules/webgpu/GPUBufferDescriptor.idl \
    $(CyberCore)/Modules/webgpu/GPUBufferUsage.idl \
    $(CyberCore)/Modules/webgpu/GPUCompareFunction.idl \
    $(CyberCore)/Modules/webgpu/GPUDepthStencilStateDescriptor.idl \
    $(CyberCore)/Modules/webgpu/GPUErrorFilter.idl \
    $(CyberCore)/Modules/webgpu/GPUExtent3D.idl \
    $(CyberCore)/Modules/webgpu/GPULoadOp.idl \
    $(CyberCore)/Modules/webgpu/GPUOrigin3D.idl \
    $(CyberCore)/Modules/webgpu/GPUOutOfMemoryError.idl \
    $(CyberCore)/Modules/webgpu/GPURequestAdapterOptions.idl \
    $(CyberCore)/Modules/webgpu/GPUSamplerDescriptor.idl \
    $(CyberCore)/Modules/webgpu/GPUShaderStage.idl \
    $(CyberCore)/Modules/webgpu/GPUStoreOp.idl \
    $(CyberCore)/Modules/webgpu/GPUTextureDescriptor.idl \
    $(CyberCore)/Modules/webgpu/GPUTextureFormat.idl \
    $(CyberCore)/Modules/webgpu/GPUTextureUsage.idl \
    $(CyberCore)/Modules/webgpu/GPUUncapturedErrorEvent.idl \
    $(CyberCore)/Modules/webgpu/GPUValidationError.idl \
    $(CyberCore)/Modules/webgpu/GPUVertexAttributeDescriptor.idl \
    $(CyberCore)/Modules/webgpu/GPUVertexBufferDescriptor.idl \
    $(CyberCore)/Modules/webgpu/GPUVertexInputDescriptor.idl \
    $(CyberCore)/Modules/webgpu/NavigatorGPU.idl \
    $(CyberCore)/Modules/webgpu/WebGPU.idl \
    $(CyberCore)/Modules/webgpu/WebGPUAdapter.idl \
    $(CyberCore)/Modules/webgpu/WebGPUBindGroup.idl \
    $(CyberCore)/Modules/webgpu/WebGPUBindGroupBinding.idl \
    $(CyberCore)/Modules/webgpu/WebGPUBindGroupDescriptor.idl \
    $(CyberCore)/Modules/webgpu/WebGPUBindGroupLayout.idl \
    $(CyberCore)/Modules/webgpu/WebGPUBuffer.idl \
    $(CyberCore)/Modules/webgpu/WebGPUBufferBinding.idl \
    $(CyberCore)/Modules/webgpu/WebGPUCommandBuffer.idl \
    $(CyberCore)/Modules/webgpu/WebGPUCommandEncoder.idl \
    $(CyberCore)/Modules/webgpu/WebGPUComputePassEncoder.idl \
    $(CyberCore)/Modules/webgpu/WebGPUComputePipeline.idl \
    $(CyberCore)/Modules/webgpu/WebGPUComputePipelineDescriptor.idl \
    $(CyberCore)/Modules/webgpu/WebGPUDevice.idl \
    $(CyberCore)/Modules/webgpu/WebGPUDeviceErrorScopes.idl \
    $(CyberCore)/Modules/webgpu/WebGPUDeviceEventHandler.idl \
    $(CyberCore)/Modules/webgpu/WebGPUQueue.idl \
    $(CyberCore)/Modules/webgpu/WebGPUPipelineDescriptorBase.idl \
    $(CyberCore)/Modules/webgpu/WebGPUPipelineLayout.idl \
    $(CyberCore)/Modules/webgpu/WebGPUPipelineLayoutDescriptor.idl \
    $(CyberCore)/Modules/webgpu/WebGPUProgrammableStageDescriptor.idl \
    $(CyberCore)/Modules/webgpu/WebGPUProgrammablePassEncoder.idl \
    $(CyberCore)/Modules/webgpu/WebGPURenderPassDescriptor.idl \
    $(CyberCore)/Modules/webgpu/WebGPURenderPassEncoder.idl \
    $(CyberCore)/Modules/webgpu/WebGPURenderPipeline.idl \
    $(CyberCore)/Modules/webgpu/WebGPURenderPipelineDescriptor.idl \
    $(CyberCore)/Modules/webgpu/WebGPUSampler.idl \
    $(CyberCore)/Modules/webgpu/WebGPUShaderModule.idl \
    $(CyberCore)/Modules/webgpu/WebGPUShaderModuleDescriptor.idl \
    $(CyberCore)/Modules/webgpu/WebGPUSwapChain.idl \
    $(CyberCore)/Modules/webgpu/WebGPUTexture.idl \
    $(CyberCore)/Modules/webgpu/WebGPUTextureView.idl \
    $(CyberCore)/Modules/webgpu/WorkerNavigatorGPU.idl \
    $(CyberCore)/Modules/websockets/CloseEvent.idl \
    $(CyberCore)/Modules/websockets/WebSocket.idl \
    $(CyberCore)/Modules/webxr/NavigatorWebXR.idl \
    $(CyberCore)/Modules/webxr/WebXRBoundedReferenceSpace.idl \
    $(CyberCore)/Modules/webxr/WebXRFrame.idl \
    $(CyberCore)/Modules/webxr/WebXRInputSourceArray.idl \
    $(CyberCore)/Modules/webxr/WebXRInputSource.idl \
    $(CyberCore)/Modules/webxr/WebXRLayer.idl \
    $(CyberCore)/Modules/webxr/WebXRPose.idl \
    $(CyberCore)/Modules/webxr/WebXRReferenceSpace.idl \
    $(CyberCore)/Modules/webxr/WebXRRenderState.idl \
    $(CyberCore)/Modules/webxr/WebXRRigidTransform.idl \
    $(CyberCore)/Modules/webxr/WebXRSession.idl \
    $(CyberCore)/Modules/webxr/WebXRSpace.idl \
    $(CyberCore)/Modules/webxr/WebXRSystem.idl \
    $(CyberCore)/Modules/webxr/WebXRViewerPose.idl \
    $(CyberCore)/Modules/webxr/WebXRView.idl \
    $(CyberCore)/Modules/webxr/WebXRViewport.idl \
    $(CyberCore)/Modules/webxr/WebXRWebGLLayer.idl \
    $(CyberCore)/Modules/webxr/XREnvironmentBlendMode.idl \
    $(CyberCore)/Modules/webxr/XREye.idl \
    $(CyberCore)/Modules/webxr/XRFrameRequestCallback.idl \
    $(CyberCore)/Modules/webxr/XRHandedness.idl \
    $(CyberCore)/Modules/webxr/XRInputSourceEvent.idl \
    $(CyberCore)/Modules/webxr/XRInputSourcesChangeEvent.idl \
    $(CyberCore)/Modules/webxr/XRInteractionMode.idl \
    $(CyberCore)/Modules/webxr/XRReferenceSpaceEvent.idl \
    $(CyberCore)/Modules/webxr/XRReferenceSpaceType.idl \
    $(CyberCore)/Modules/webxr/XRRenderStateInit.idl \
    $(CyberCore)/Modules/webxr/XRSessionEvent.idl \
    $(CyberCore)/Modules/webxr/XRSessionInit.idl \
    $(CyberCore)/Modules/webxr/XRSessionMode.idl \
    $(CyberCore)/Modules/webxr/XRTargetRayMode.idl \
    $(CyberCore)/Modules/webxr/XRVisibilityState.idl \
    $(CyberCore)/Modules/webxr/XRWebGLLayerInit.idl \
    $(CyberCore)/accessibility/AccessibilityRole.idl \
    $(CyberCore)/accessibility/AriaAttributes.idl \
    $(CyberCore)/animation/Animatable.idl \
    $(CyberCore)/animation/AnimationEffect.idl \
    $(CyberCore)/animation/AnimationPlaybackEvent.idl \
    $(CyberCore)/animation/AnimationPlaybackEventInit.idl \
    $(CyberCore)/animation/AnimationTimeline.idl \
    $(CyberCore)/animation/CSSAnimation.idl \
    $(CyberCore)/animation/CSSTransition.idl \
    $(CyberCore)/animation/CompositeOperation.idl \
    $(CyberCore)/animation/CompositeOperationOrAuto.idl \
    $(CyberCore)/animation/ComputedEffectTiming.idl \
    $(CyberCore)/animation/DocumentTimeline.idl \
    $(CyberCore)/animation/DocumentTimelineOptions.idl \
    $(CyberCore)/animation/EffectTiming.idl \
    $(CyberCore)/animation/FillMode.idl \
    $(CyberCore)/animation/GetAnimationsOptions.idl \
    $(CyberCore)/animation/IterationCompositeOperation.idl \
    $(CyberCore)/animation/KeyframeAnimationOptions.idl \
    $(CyberCore)/animation/KeyframeEffect.idl \
    $(CyberCore)/animation/KeyframeEffectOptions.idl \
    $(CyberCore)/animation/OptionalEffectTiming.idl \
    $(CyberCore)/animation/PlaybackDirection.idl \
    $(CyberCore)/animation/WebAnimation.idl \
    $(CyberCore)/crypto/CryptoAlgorithmParameters.idl \
    $(CyberCore)/crypto/CryptoKey.idl \
    $(CyberCore)/crypto/CryptoKeyPair.idl \
    $(CyberCore)/crypto/CryptoKeyUsage.idl \
    $(CyberCore)/crypto/JsonWebKey.idl \
    $(CyberCore)/crypto/RsaOtherPrimesInfo.idl \
    $(CyberCore)/crypto/SubtleCrypto.idl \
    $(CyberCore)/crypto/keys/CryptoAesKeyAlgorithm.idl \
    $(CyberCore)/crypto/keys/CryptoEcKeyAlgorithm.idl \
    $(CyberCore)/crypto/keys/CryptoHmacKeyAlgorithm.idl \
    $(CyberCore)/crypto/keys/CryptoKeyAlgorithm.idl \
    $(CyberCore)/crypto/keys/CryptoRsaHashedKeyAlgorithm.idl \
    $(CyberCore)/crypto/keys/CryptoRsaKeyAlgorithm.idl \
    $(CyberCore)/crypto/parameters/AesCbcCfbParams.idl \
    $(CyberCore)/crypto/parameters/AesCtrParams.idl \
    $(CyberCore)/crypto/parameters/AesGcmParams.idl \
    $(CyberCore)/crypto/parameters/AesKeyParams.idl \
    $(CyberCore)/crypto/parameters/EcKeyParams.idl \
    $(CyberCore)/crypto/parameters/EcdhKeyDeriveParams.idl \
    $(CyberCore)/crypto/parameters/EcdsaParams.idl \
    $(CyberCore)/crypto/parameters/HkdfParams.idl \
    $(CyberCore)/crypto/parameters/HmacKeyParams.idl \
    $(CyberCore)/crypto/parameters/Pbkdf2Params.idl \
    $(CyberCore)/crypto/parameters/RsaHashedImportParams.idl \
    $(CyberCore)/crypto/parameters/RsaHashedKeyGenParams.idl \
    $(CyberCore)/crypto/parameters/RsaKeyGenParams.idl \
    $(CyberCore)/crypto/parameters/RsaOaepParams.idl \
    $(CyberCore)/crypto/parameters/RsaPssParams.idl \
    $(CyberCore)/css/CSSFontFaceRule.idl \
    $(CyberCore)/css/CSSImportRule.idl \
    $(CyberCore)/css/CSSKeyframeRule.idl \
    $(CyberCore)/css/CSSKeyframesRule.idl \
    $(CyberCore)/css/CSSMediaRule.idl \
    $(CyberCore)/css/CSSNamespaceRule.idl \
    $(CyberCore)/css/CSSPageRule.idl \
    $(CyberCore)/css/CSSPaintCallback.idl \
    $(CyberCore)/css/CSSPaintSize.idl \
    $(CyberCore)/css/CSSRule.idl \
    $(CyberCore)/css/CSSRuleList.idl \
    $(CyberCore)/css/CSSStyleDeclaration.idl \
    $(CyberCore)/css/CSSStyleRule.idl \
    $(CyberCore)/css/CSSStyleSheet.idl \
    $(CyberCore)/css/CSSSupportsRule.idl \
    $(CyberCore)/css/CSSUnknownRule.idl \
    $(CyberCore)/css/DOMCSSCustomPropertyDescriptor.idl \
    $(CyberCore)/css/DOMCSSNamespace.idl \
    $(CyberCore)/css/DOMCSSPaintWorklet.idl \
    $(CyberCore)/css/DOMCSSRegisterCustomProperty.idl \
    $(CyberCore)/css/DOMMatrix.idl \
    $(CyberCore)/css/DOMMatrix2DInit.idl \
    $(CyberCore)/css/DOMMatrixInit.idl \
    $(CyberCore)/css/DOMMatrixReadOnly.idl \
    $(CyberCore)/css/DeprecatedCSSOMCounter.idl \
    $(CyberCore)/css/DeprecatedCSSOMPrimitiveValue.idl \
    $(CyberCore)/css/DeprecatedCSSOMRGBColor.idl \
    $(CyberCore)/css/DeprecatedCSSOMRect.idl \
    $(CyberCore)/css/DeprecatedCSSOMValue.idl \
    $(CyberCore)/css/DeprecatedCSSOMValueList.idl \
    $(CyberCore)/css/ElementCSSInlineStyle.idl \
    $(CyberCore)/css/FontFace.idl \
    $(CyberCore)/css/FontFaceSet.idl \
    $(CyberCore)/css/MediaList.idl \
    $(CyberCore)/css/MediaQueryList.idl \
    $(CyberCore)/css/MediaQueryListEvent.idl \
    $(CyberCore)/css/StyleMedia.idl \
    $(CyberCore)/css/StyleSheet.idl \
    $(CyberCore)/css/StyleSheetList.idl \
    $(CyberCore)/css/typedom/StylePropertyMap.idl \
    $(CyberCore)/css/typedom/StylePropertyMapReadOnly.idl \
    $(CyberCore)/css/typedom/TypedOMCSSImageValue.idl \
    $(CyberCore)/css/typedom/TypedOMCSSNumericValue.idl \
    $(CyberCore)/css/typedom/TypedOMCSSStyleValue.idl \
    $(CyberCore)/css/typedom/TypedOMCSSUnitValue.idl \
    $(CyberCore)/css/typedom/TypedOMCSSUnparsedValue.idl \
    $(CyberCore)/css/CyberKitCSSMatrix.idl \
    $(CyberCore)/dom/AbortController.idl \
    $(CyberCore)/dom/AbortSignal.idl \
    $(CyberCore)/dom/AnimationEvent.idl \
    $(CyberCore)/dom/Attr.idl \
    $(CyberCore)/dom/BeforeLoadEvent.idl \
    $(CyberCore)/dom/BeforeUnloadEvent.idl \
    $(CyberCore)/dom/CDATASection.idl \
    $(CyberCore)/dom/CharacterData.idl \
    $(CyberCore)/dom/ChildNode.idl \
    $(CyberCore)/dom/ClipboardEvent.idl \
    $(CyberCore)/dom/Comment.idl \
    $(CyberCore)/dom/CompositionEvent.idl \
    $(CyberCore)/dom/CustomElementRegistry.idl \
    $(CyberCore)/dom/CustomEvent.idl \
    $(CyberCore)/dom/DOMException.idl \
    $(CyberCore)/dom/DOMImplementation.idl \
    $(CyberCore)/dom/DOMPoint.idl \
    $(CyberCore)/dom/DOMPointInit.idl \
    $(CyberCore)/dom/DOMPointReadOnly.idl \
    $(CyberCore)/dom/DOMQuad.idl \
    $(CyberCore)/dom/DOMQuadInit.idl \
    $(CyberCore)/dom/DOMRect.idl \
    $(CyberCore)/dom/DOMRectInit.idl \
    $(CyberCore)/dom/DOMRectList.idl \
    $(CyberCore)/dom/DOMRectReadOnly.idl \
    $(CyberCore)/dom/DOMStringList.idl \
    $(CyberCore)/dom/DOMStringMap.idl \
    $(CyberCore)/dom/DataTransfer.idl \
    $(CyberCore)/dom/DataTransferItem.idl \
    $(CyberCore)/dom/DataTransferItemList.idl \
    $(CyberCore)/dom/DeviceMotionEvent.idl \
    $(CyberCore)/dom/DeviceOrientationEvent.idl \
    $(CyberCore)/dom/DeviceOrientationOrMotionEvent.idl \
    $(CyberCore)/dom/DeviceOrientationOrMotionPermissionState.idl \
    $(CyberCore)/dom/Document.idl \
    $(CyberCore)/dom/DocumentAndElementEventHandlers.idl \
    $(CyberCore)/dom/DocumentFullscreen.idl \
    $(CyberCore)/dom/DocumentFragment.idl \
    $(CyberCore)/dom/DocumentOrShadowRoot.idl \
    $(CyberCore)/dom/DocumentStorageAccess.idl \
    $(CyberCore)/dom/DocumentType.idl \
    $(CyberCore)/dom/DragEvent.idl \
    $(CyberCore)/dom/Element.idl \
    $(CyberCore)/dom/ErrorEvent.idl \
    $(CyberCore)/dom/Event.idl \
    $(CyberCore)/dom/EventInit.idl \
    $(CyberCore)/dom/EventListener.idl \
    $(CyberCore)/dom/EventModifierInit.idl \
    $(CyberCore)/dom/EventTarget.idl \
    $(CyberCore)/dom/FocusEvent.idl \
    $(CyberCore)/dom/GlobalEventHandlers.idl \
    $(CyberCore)/dom/HashChangeEvent.idl \
    $(CyberCore)/dom/IdleDeadline.idl \
    $(CyberCore)/dom/IdleRequestCallback.idl \
    $(CyberCore)/dom/IdleRequestOptions.idl \
    $(CyberCore)/dom/InputEvent.idl \
    $(CyberCore)/dom/KeyboardEvent.idl \
    $(CyberCore)/dom/MessageChannel.idl \
    $(CyberCore)/dom/MessageEvent.idl \
    $(CyberCore)/dom/MessagePort.idl \
    $(CyberCore)/dom/MouseEvent.idl \
    $(CyberCore)/dom/MouseEventInit.idl \
    $(CyberCore)/dom/MutationCallback.idl \
    $(CyberCore)/dom/MutationEvent.idl \
    $(CyberCore)/dom/MutationObserver.idl \
    $(CyberCore)/dom/MutationRecord.idl \
    $(CyberCore)/dom/NamedNodeMap.idl \
    $(CyberCore)/dom/NavigatorMaxTouchPoints.idl \
    $(CyberCore)/dom/Node.idl \
    $(CyberCore)/dom/NodeFilter.idl \
    $(CyberCore)/dom/NodeIterator.idl \
    $(CyberCore)/dom/NodeList.idl \
    $(CyberCore)/dom/NonDocumentTypeChildNode.idl \
    $(CyberCore)/dom/NonElementParentNode.idl \
    $(CyberCore)/dom/OverflowEvent.idl \
    $(CyberCore)/dom/PageTransitionEvent.idl \
    $(CyberCore)/dom/ParentNode.idl \
    $(CyberCore)/dom/PointerEvent.idl \
    $(CyberCore)/dom/PopStateEvent.idl \
    $(CyberCore)/dom/ProcessingInstruction.idl \
    $(CyberCore)/dom/ProgressEvent.idl \
    $(CyberCore)/dom/PromiseRejectionEvent.idl \
    $(CyberCore)/dom/Range.idl \
    $(CyberCore)/dom/RequestAnimationFrameCallback.idl \
    $(CyberCore)/dom/SecurityPolicyViolationEvent.idl \
    $(CyberCore)/dom/ShadowRoot.idl \
    $(CyberCore)/dom/ShadowRootMode.idl \
    $(CyberCore)/dom/Slotable.idl \
    $(CyberCore)/dom/StaticRange.idl \
    $(CyberCore)/dom/StringCallback.idl \
    $(CyberCore)/dom/Text.idl \
    $(CyberCore)/dom/TextDecoder.idl \
    $(CyberCore)/dom/TextEncoder.idl \
    $(CyberCore)/dom/TextEvent.idl \
    $(CyberCore)/dom/TransitionEvent.idl \
    $(CyberCore)/dom/TreeWalker.idl \
    $(CyberCore)/dom/UIEvent.idl \
    $(CyberCore)/dom/UIEventInit.idl \
    $(CyberCore)/dom/VisibilityState.idl \
    $(CyberCore)/dom/CyberKitAnimationEvent.idl \
    $(CyberCore)/dom/CyberKitTransitionEvent.idl \
    $(CyberCore)/dom/WheelEvent.idl \
    $(CyberCore)/dom/XMLDocument.idl \
    $(CyberCore)/fileapi/Blob.idl \
    $(CyberCore)/fileapi/BlobCallback.idl \
    $(CyberCore)/fileapi/BlobPropertyBag.idl \
    $(CyberCore)/fileapi/EndingType.idl \
    $(CyberCore)/fileapi/File.idl \
    $(CyberCore)/fileapi/FileList.idl \
    $(CyberCore)/fileapi/FileReader.idl \
    $(CyberCore)/fileapi/FileReaderSync.idl \
    $(CyberCore)/html/DOMFormData.idl \
    $(CyberCore)/html/DOMTokenList.idl \
    $(CyberCore)/html/DOMURL.idl \
    $(CyberCore)/html/HTMLAllCollection.idl \
    $(CyberCore)/html/HTMLAnchorElement.idl \
    $(CyberCore)/html/HTMLAppletElement.idl \
    $(CyberCore)/html/HTMLAreaElement.idl \
    $(CyberCore)/html/HTMLAttachmentElement.idl \
    $(CyberCore)/html/HTMLAudioElement.idl \
    $(CyberCore)/html/HTMLBRElement.idl \
    $(CyberCore)/html/HTMLBaseElement.idl \
    $(CyberCore)/html/HTMLBodyElement.idl \
    $(CyberCore)/html/HTMLButtonElement.idl \
    $(CyberCore)/html/HTMLCanvasElement.idl \
    $(CyberCore)/html/HTMLCollection.idl \
    $(CyberCore)/html/HTMLDListElement.idl \
    $(CyberCore)/html/HTMLDataElement.idl \
    $(CyberCore)/html/HTMLDataListElement.idl \
    $(CyberCore)/html/HTMLDetailsElement.idl \
    $(CyberCore)/html/HTMLDialogElement.idl \
    $(CyberCore)/html/HTMLDirectoryElement.idl \
    $(CyberCore)/html/HTMLDivElement.idl \
    $(CyberCore)/html/HTMLDocument.idl \
    $(CyberCore)/html/HTMLElement.idl \
    $(CyberCore)/html/HTMLEmbedElement.idl \
    $(CyberCore)/html/HTMLFieldSetElement.idl \
    $(CyberCore)/html/HTMLFontElement.idl \
    $(CyberCore)/html/HTMLFormControlsCollection.idl \
    $(CyberCore)/html/HTMLFormElement.idl \
    $(CyberCore)/html/HTMLFrameElement.idl \
    $(CyberCore)/html/HTMLFrameSetElement.idl \
    $(CyberCore)/html/HTMLHRElement.idl \
    $(CyberCore)/html/HTMLHeadElement.idl \
    $(CyberCore)/html/HTMLHeadingElement.idl \
    $(CyberCore)/html/HTMLHtmlElement.idl \
    $(CyberCore)/html/HTMLHyperlinkElementUtils.idl \
    $(CyberCore)/html/HTMLIFrameElement.idl \
    $(CyberCore)/html/HTMLImageElement.idl \
    $(CyberCore)/html/HTMLInputElement.idl \
    $(CyberCore)/html/HTMLKeygenElement.idl \
    $(CyberCore)/html/HTMLLIElement.idl \
    $(CyberCore)/html/HTMLLabelElement.idl \
    $(CyberCore)/html/HTMLLegendElement.idl \
    $(CyberCore)/html/HTMLLinkElement.idl \
    $(CyberCore)/html/HTMLMapElement.idl \
    $(CyberCore)/html/HTMLMarqueeElement.idl \
    $(CyberCore)/html/HTMLMediaElement.idl \
    $(CyberCore)/html/HTMLMenuElement.idl \
    $(CyberCore)/html/HTMLMenuItemElement.idl \
    $(CyberCore)/html/HTMLMetaElement.idl \
    $(CyberCore)/html/HTMLMeterElement.idl \
    $(CyberCore)/html/HTMLModElement.idl \
    $(CyberCore)/html/HTMLOListElement.idl \
    $(CyberCore)/html/HTMLObjectElement.idl \
    $(CyberCore)/html/HTMLOptGroupElement.idl \
    $(CyberCore)/html/HTMLOptionElement.idl \
    $(CyberCore)/html/HTMLOptionsCollection.idl \
    $(CyberCore)/html/HTMLOrForeignElement.idl \
    $(CyberCore)/html/HTMLOutputElement.idl \
    $(CyberCore)/html/HTMLParagraphElement.idl \
    $(CyberCore)/html/HTMLParamElement.idl \
    $(CyberCore)/html/HTMLPictureElement.idl \
    $(CyberCore)/html/HTMLPreElement.idl \
    $(CyberCore)/html/HTMLProgressElement.idl \
    $(CyberCore)/html/HTMLQuoteElement.idl \
    $(CyberCore)/html/HTMLScriptElement.idl \
    $(CyberCore)/html/HTMLSelectElement.idl \
    $(CyberCore)/html/HTMLSlotElement.idl \
    $(CyberCore)/html/HTMLSourceElement.idl \
    $(CyberCore)/html/HTMLSpanElement.idl \
    $(CyberCore)/html/HTMLStyleElement.idl \
    $(CyberCore)/html/HTMLTableCaptionElement.idl \
    $(CyberCore)/html/HTMLTableCellElement.idl \
    $(CyberCore)/html/HTMLTableColElement.idl \
    $(CyberCore)/html/HTMLTableElement.idl \
    $(CyberCore)/html/HTMLTableRowElement.idl \
    $(CyberCore)/html/HTMLTableSectionElement.idl \
    $(CyberCore)/html/HTMLTemplateElement.idl \
    $(CyberCore)/html/HTMLTextAreaElement.idl \
    $(CyberCore)/html/HTMLTimeElement.idl \
    $(CyberCore)/html/HTMLTitleElement.idl \
    $(CyberCore)/html/HTMLTrackElement.idl \
    $(CyberCore)/html/HTMLUListElement.idl \
    $(CyberCore)/html/HTMLUnknownElement.idl \
    $(CyberCore)/html/HTMLVideoElement.idl \
    $(CyberCore)/html/ImageBitmap.idl \
    $(CyberCore)/html/ImageBitmapOptions.idl \
    $(CyberCore)/html/ImageData.idl \
    $(CyberCore)/html/MediaController.idl \
    $(CyberCore)/html/MediaEncryptedEvent.idl \
    $(CyberCore)/html/MediaError.idl \
    $(CyberCore)/html/OffscreenCanvas.idl \
    $(CyberCore)/html/RadioNodeList.idl \
    $(CyberCore)/html/TextMetrics.idl \
    $(CyberCore)/html/TimeRanges.idl \
    $(CyberCore)/html/URLSearchParams.idl \
    $(CyberCore)/html/ValidityState.idl \
    $(CyberCore)/html/VoidCallback.idl \
    $(CyberCore)/html/CyberKitMediaKeyError.idl \
    $(CyberCore)/html/canvas/ANGLEInstancedArrays.idl \
    $(CyberCore)/html/canvas/CanvasCompositing.idl \
    $(CyberCore)/html/canvas/CanvasDirection.idl \
    $(CyberCore)/html/canvas/CanvasDrawImage.idl \
    $(CyberCore)/html/canvas/CanvasDrawPath.idl \
    $(CyberCore)/html/canvas/CanvasFillRule.idl \
    $(CyberCore)/html/canvas/CanvasFillStrokeStyles.idl \
    $(CyberCore)/html/canvas/CanvasFilters.idl \
    $(CyberCore)/html/canvas/CanvasGradient.idl \
    $(CyberCore)/html/canvas/CanvasImageData.idl \
    $(CyberCore)/html/canvas/CanvasImageSmoothing.idl \
    $(CyberCore)/html/canvas/CanvasLineCap.idl \
    $(CyberCore)/html/canvas/CanvasLineJoin.idl \
    $(CyberCore)/html/canvas/CanvasPath.idl \
    $(CyberCore)/html/canvas/CanvasPathDrawingStyles.idl \
    $(CyberCore)/html/canvas/CanvasPattern.idl \
    $(CyberCore)/html/canvas/CanvasRect.idl \
    $(CyberCore)/html/canvas/CanvasRenderingContext2D.idl \
    $(CyberCore)/html/canvas/CanvasShadowStyles.idl \
    $(CyberCore)/html/canvas/CanvasState.idl \
    $(CyberCore)/html/canvas/CanvasText.idl \
    $(CyberCore)/html/canvas/CanvasTextAlign.idl \
    $(CyberCore)/html/canvas/CanvasTextBaseline.idl \
    $(CyberCore)/html/canvas/CanvasTextDrawingStyles.idl \
    $(CyberCore)/html/canvas/CanvasTransform.idl \
    $(CyberCore)/html/canvas/CanvasUserInterface.idl \
    $(CyberCore)/html/canvas/EXTBlendMinMax.idl \
    $(CyberCore)/html/canvas/EXTColorBufferFloat.idl \
    $(CyberCore)/html/canvas/EXTColorBufferHalfFloat.idl \
    $(CyberCore)/html/canvas/EXTFragDepth.idl \
    $(CyberCore)/html/canvas/EXTShaderTextureLOD.idl \
    $(CyberCore)/html/canvas/EXTTextureFilterAnisotropic.idl \
    $(CyberCore)/html/canvas/EXTsRGB.idl \
    $(CyberCore)/html/canvas/ImageBitmapRenderingContext.idl \
    $(CyberCore)/html/canvas/ImageBitmapRenderingContextSettings.idl \
    $(CyberCore)/html/canvas/ImageSmoothingQuality.idl \
    $(CyberCore)/html/canvas/OESElementIndexUint.idl \
    $(CyberCore)/html/canvas/OESStandardDerivatives.idl \
    $(CyberCore)/html/canvas/OESTextureFloat.idl \
    $(CyberCore)/html/canvas/OESTextureFloatLinear.idl \
    $(CyberCore)/html/canvas/OESTextureHalfFloat.idl \
    $(CyberCore)/html/canvas/OESTextureHalfFloatLinear.idl \
    $(CyberCore)/html/canvas/OESVertexArrayObject.idl \
    $(CyberCore)/html/canvas/OffscreenCanvasRenderingContext2D.idl \
    $(CyberCore)/html/canvas/PaintRenderingContext2D.idl \
    $(CyberCore)/html/canvas/Path2D.idl \
    $(CyberCore)/html/canvas/WebGL2RenderingContext.idl \
    $(CyberCore)/html/canvas/WebGLActiveInfo.idl \
    $(CyberCore)/html/canvas/WebGLBuffer.idl \
    $(CyberCore)/html/canvas/WebGLColorBufferFloat.idl \
    $(CyberCore)/html/canvas/WebGLCompressedTextureASTC.idl \
    $(CyberCore)/html/canvas/WebGLCompressedTextureATC.idl \
    $(CyberCore)/html/canvas/WebGLCompressedTextureETC.idl \
    $(CyberCore)/html/canvas/WebGLCompressedTextureETC1.idl \
    $(CyberCore)/html/canvas/WebGLCompressedTexturePVRTC.idl \
    $(CyberCore)/html/canvas/WebGLCompressedTextureS3TC.idl \
    $(CyberCore)/html/canvas/WebGLCompressedTextureS3TCsRGB.idl \
    $(CyberCore)/html/canvas/WebGLContextAttributes.idl \
    $(CyberCore)/html/canvas/WebGLContextEvent.idl \
    $(CyberCore)/html/canvas/WebGLDebugRendererInfo.idl \
    $(CyberCore)/html/canvas/WebGLDebugShaders.idl \
    $(CyberCore)/html/canvas/WebGLDepthTexture.idl \
    $(CyberCore)/html/canvas/WebGLDrawBuffers.idl \
    $(CyberCore)/html/canvas/WebGLFramebuffer.idl \
    $(CyberCore)/html/canvas/WebGLLoseContext.idl \
    $(CyberCore)/html/canvas/WebGLProgram.idl \
    $(CyberCore)/html/canvas/WebGLQuery.idl \
    $(CyberCore)/html/canvas/WebGLRenderbuffer.idl \
    $(CyberCore)/html/canvas/WebGLRenderingContext.idl \
    $(CyberCore)/html/canvas/WebGLRenderingContextBase.idl \
    $(CyberCore)/html/canvas/WebGLSampler.idl \
    $(CyberCore)/html/canvas/WebGLShader.idl \
    $(CyberCore)/html/canvas/WebGLShaderPrecisionFormat.idl \
    $(CyberCore)/html/canvas/WebGLSync.idl \
    $(CyberCore)/html/canvas/WebGLTexture.idl \
    $(CyberCore)/html/canvas/WebGLTransformFeedback.idl \
    $(CyberCore)/html/canvas/WebGLUniformLocation.idl \
    $(CyberCore)/html/canvas/WebGLVertexArrayObject.idl \
    $(CyberCore)/html/canvas/WebGLVertexArrayObjectOES.idl \
    $(CyberCore)/html/track/AudioTrack.idl \
    $(CyberCore)/html/track/AudioTrackList.idl \
    $(CyberCore)/html/track/DataCue.idl \
    $(CyberCore)/html/track/TextTrack.idl \
    $(CyberCore)/html/track/TextTrackCue.idl \
    $(CyberCore)/html/track/TextTrackCueGeneric.idl \
    $(CyberCore)/html/track/TextTrackCueList.idl \
    $(CyberCore)/html/track/TextTrackList.idl \
    $(CyberCore)/html/track/TrackEvent.idl \
    $(CyberCore)/html/track/VTTCue.idl \
    $(CyberCore)/html/track/VTTRegion.idl \
    $(CyberCore)/html/track/VTTRegionList.idl \
    $(CyberCore)/html/track/VideoTrack.idl \
    $(CyberCore)/html/track/VideoTrackList.idl \
    $(CyberCore)/mathml/MathMLElement.idl \
    $(CyberCore)/mathml/MathMLMathElement.idl \
    $(CyberCore)/inspector/CommandLineAPIHost.idl \
    $(CyberCore)/inspector/InspectorAuditAccessibilityObject.idl \
    $(CyberCore)/inspector/InspectorAuditDOMObject.idl \
    $(CyberCore)/inspector/InspectorAuditResourcesObject.idl \
    $(CyberCore)/inspector/InspectorFrontendHost.idl \
    $(CyberCore)/loader/appcache/DOMApplicationCache.idl \
    $(CyberCore)/page/BarProp.idl \
    $(CyberCore)/page/Crypto.idl \
    $(CyberCore)/page/DOMSelection.idl \
    $(CyberCore)/page/DOMWindow.idl \
    $(CyberCore)/page/EventSource.idl \
    $(CyberCore)/page/GlobalCrypto.idl \
    $(CyberCore)/page/GlobalPerformance.idl \
    $(CyberCore)/page/History.idl \
    $(CyberCore)/page/IntersectionObserver.idl \
    $(CyberCore)/page/IntersectionObserverCallback.idl \
    $(CyberCore)/page/IntersectionObserverEntry.idl \
    $(CyberCore)/page/Location.idl \
    $(CyberCore)/page/Navigator.idl \
    $(CyberCore)/page/NavigatorID.idl \
    $(CyberCore)/page/NavigatorIsLoggedIn.idl \
    $(CyberCore)/page/NavigatorLanguage.idl \
    $(CyberCore)/page/NavigatorOnLine.idl \
    $(CyberCore)/page/NavigatorPlugins.idl \
    $(CyberCore)/page/NavigatorServiceWorker.idl \
    $(CyberCore)/page/NavigatorShare.idl \
    $(CyberCore)/page/Performance.idl \
    $(CyberCore)/page/PerformanceEntry.idl \
    $(CyberCore)/page/PerformanceMark.idl \
    $(CyberCore)/page/PerformanceMeasure.idl \
    $(CyberCore)/page/PerformanceNavigation.idl \
    $(CyberCore)/page/PerformanceObserver.idl \
    $(CyberCore)/page/PerformanceObserverCallback.idl \
    $(CyberCore)/page/PerformanceObserverEntryList.idl \
    $(CyberCore)/page/PerformancePaintTiming.idl \
    $(CyberCore)/page/PerformanceResourceTiming.idl \
    $(CyberCore)/page/PerformanceServerTiming.idl \
    $(CyberCore)/page/PerformanceTiming.idl \
    $(CyberCore)/page/PostMessageOptions.idl \
    $(CyberCore)/page/RemoteDOMWindow.idl \
    $(CyberCore)/page/ResizeObserver.idl \
    $(CyberCore)/page/ResizeObserverCallback.idl \
    $(CyberCore)/page/ResizeObserverEntry.idl \
    $(CyberCore)/page/Screen.idl \
    $(CyberCore)/page/ScrollBehavior.idl \
    $(CyberCore)/page/ScrollIntoViewOptions.idl \
    $(CyberCore)/page/ScrollLogicalPosition.idl \
    $(CyberCore)/page/ScrollOptions.idl \
    $(CyberCore)/page/ScrollToOptions.idl \
    $(CyberCore)/page/ShareData.idl \
    $(CyberCore)/page/UndoItem.idl \
    $(CyberCore)/page/UndoManager.idl \
    $(CyberCore)/page/UserMessageHandler.idl \
    $(CyberCore)/page/UserMessageHandlersNamespace.idl \
    $(CyberCore)/page/VisualViewport.idl \
    $(CyberCore)/page/CyberKitNamespace.idl \
    $(CyberCore)/page/CyberKitPoint.idl \
    $(CyberCore)/page/WindowEventHandlers.idl \
    $(CyberCore)/page/WindowOrWorkerGlobalScope.idl \
    $(CyberCore)/page/WorkerNavigator.idl \
    $(CyberCore)/plugins/DOMMimeType.idl \
    $(CyberCore)/plugins/DOMMimeTypeArray.idl \
    $(CyberCore)/plugins/DOMPlugin.idl \
    $(CyberCore)/plugins/DOMPluginArray.idl \
    $(CyberCore)/storage/Storage.idl \
    $(CyberCore)/storage/StorageEvent.idl \
    $(CyberCore)/svg/SVGAElement.idl \
    $(CyberCore)/svg/SVGAltGlyphDefElement.idl \
    $(CyberCore)/svg/SVGAltGlyphElement.idl \
    $(CyberCore)/svg/SVGAltGlyphItemElement.idl \
    $(CyberCore)/svg/SVGAngle.idl \
    $(CyberCore)/svg/SVGAnimateColorElement.idl \
    $(CyberCore)/svg/SVGAnimateElement.idl \
    $(CyberCore)/svg/SVGAnimateMotionElement.idl \
    $(CyberCore)/svg/SVGAnimateTransformElement.idl \
    $(CyberCore)/svg/SVGAnimatedAngle.idl \
    $(CyberCore)/svg/SVGAnimatedBoolean.idl \
    $(CyberCore)/svg/SVGAnimatedEnumeration.idl \
    $(CyberCore)/svg/SVGAnimatedInteger.idl \
    $(CyberCore)/svg/SVGAnimatedLength.idl \
    $(CyberCore)/svg/SVGAnimatedLengthList.idl \
    $(CyberCore)/svg/SVGAnimatedNumber.idl \
    $(CyberCore)/svg/SVGAnimatedNumberList.idl \
    $(CyberCore)/svg/SVGAnimatedPreserveAspectRatio.idl \
    $(CyberCore)/svg/SVGAnimatedRect.idl \
    $(CyberCore)/svg/SVGAnimatedString.idl \
    $(CyberCore)/svg/SVGAnimatedTransformList.idl \
    $(CyberCore)/svg/SVGAnimationElement.idl \
    $(CyberCore)/svg/SVGCircleElement.idl \
    $(CyberCore)/svg/SVGClipPathElement.idl \
    $(CyberCore)/svg/SVGComponentTransferFunctionElement.idl \
    $(CyberCore)/svg/SVGCursorElement.idl \
    $(CyberCore)/svg/SVGDefsElement.idl \
    $(CyberCore)/svg/SVGDescElement.idl \
    $(CyberCore)/svg/SVGDocument.idl \
    $(CyberCore)/svg/SVGElement.idl \
    $(CyberCore)/svg/SVGEllipseElement.idl \
    $(CyberCore)/svg/SVGFEBlendElement.idl \
    $(CyberCore)/svg/SVGFEColorMatrixElement.idl \
    $(CyberCore)/svg/SVGFEComponentTransferElement.idl \
    $(CyberCore)/svg/SVGFECompositeElement.idl \
    $(CyberCore)/svg/SVGFEConvolveMatrixElement.idl \
    $(CyberCore)/svg/SVGFEDiffuseLightingElement.idl \
    $(CyberCore)/svg/SVGFEDisplacementMapElement.idl \
    $(CyberCore)/svg/SVGFEDistantLightElement.idl \
    $(CyberCore)/svg/SVGFEDropShadowElement.idl \
    $(CyberCore)/svg/SVGFEFloodElement.idl \
    $(CyberCore)/svg/SVGFEFuncAElement.idl \
    $(CyberCore)/svg/SVGFEFuncBElement.idl \
    $(CyberCore)/svg/SVGFEFuncGElement.idl \
    $(CyberCore)/svg/SVGFEFuncRElement.idl \
    $(CyberCore)/svg/SVGFEGaussianBlurElement.idl \
    $(CyberCore)/svg/SVGFEImageElement.idl \
    $(CyberCore)/svg/SVGFEMergeElement.idl \
    $(CyberCore)/svg/SVGFEMergeNodeElement.idl \
    $(CyberCore)/svg/SVGFEMorphologyElement.idl \
    $(CyberCore)/svg/SVGFEOffsetElement.idl \
    $(CyberCore)/svg/SVGFEPointLightElement.idl \
    $(CyberCore)/svg/SVGFESpecularLightingElement.idl \
    $(CyberCore)/svg/SVGFESpotLightElement.idl \
    $(CyberCore)/svg/SVGFETileElement.idl \
    $(CyberCore)/svg/SVGFETurbulenceElement.idl \
    $(CyberCore)/svg/SVGFilterElement.idl \
    $(CyberCore)/svg/SVGFilterPrimitiveStandardAttributes.idl \
    $(CyberCore)/svg/SVGFitToViewBox.idl \
    $(CyberCore)/svg/SVGFontElement.idl \
    $(CyberCore)/svg/SVGFontFaceElement.idl \
    $(CyberCore)/svg/SVGFontFaceFormatElement.idl \
    $(CyberCore)/svg/SVGFontFaceNameElement.idl \
    $(CyberCore)/svg/SVGFontFaceSrcElement.idl \
    $(CyberCore)/svg/SVGFontFaceUriElement.idl \
    $(CyberCore)/svg/SVGForeignObjectElement.idl \
    $(CyberCore)/svg/SVGGElement.idl \
    $(CyberCore)/svg/SVGGeometryElement.idl \
    $(CyberCore)/svg/SVGGlyphElement.idl \
    $(CyberCore)/svg/SVGGlyphRefElement.idl \
    $(CyberCore)/svg/SVGGradientElement.idl \
    $(CyberCore)/svg/SVGGraphicsElement.idl \
    $(CyberCore)/svg/SVGHKernElement.idl \
    $(CyberCore)/svg/SVGImageElement.idl \
    $(CyberCore)/svg/SVGLength.idl \
    $(CyberCore)/svg/SVGLengthList.idl \
    $(CyberCore)/svg/SVGLineElement.idl \
    $(CyberCore)/svg/SVGLinearGradientElement.idl \
    $(CyberCore)/svg/SVGMPathElement.idl \
    $(CyberCore)/svg/SVGMarkerElement.idl \
    $(CyberCore)/svg/SVGMaskElement.idl \
    $(CyberCore)/svg/SVGMatrix.idl \
    $(CyberCore)/svg/SVGMetadataElement.idl \
    $(CyberCore)/svg/SVGMissingGlyphElement.idl \
    $(CyberCore)/svg/SVGNumber.idl \
    $(CyberCore)/svg/SVGNumberList.idl \
    $(CyberCore)/svg/SVGPathElement.idl \
    $(CyberCore)/svg/SVGPathSeg.idl \
    $(CyberCore)/svg/SVGPathSegArcAbs.idl \
    $(CyberCore)/svg/SVGPathSegArcRel.idl \
    $(CyberCore)/svg/SVGPathSegClosePath.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoCubicAbs.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoCubicRel.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoCubicSmoothAbs.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoCubicSmoothRel.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoQuadraticAbs.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoQuadraticRel.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoQuadraticSmoothAbs.idl \
    $(CyberCore)/svg/SVGPathSegCurvetoQuadraticSmoothRel.idl \
    $(CyberCore)/svg/SVGPathSegLinetoAbs.idl \
    $(CyberCore)/svg/SVGPathSegLinetoHorizontalAbs.idl \
    $(CyberCore)/svg/SVGPathSegLinetoHorizontalRel.idl \
    $(CyberCore)/svg/SVGPathSegLinetoRel.idl \
    $(CyberCore)/svg/SVGPathSegLinetoVerticalAbs.idl \
    $(CyberCore)/svg/SVGPathSegLinetoVerticalRel.idl \
    $(CyberCore)/svg/SVGPathSegList.idl \
    $(CyberCore)/svg/SVGPathSegMovetoAbs.idl \
    $(CyberCore)/svg/SVGPathSegMovetoRel.idl \
    $(CyberCore)/svg/SVGPatternElement.idl \
    $(CyberCore)/svg/SVGPoint.idl \
    $(CyberCore)/svg/SVGPointList.idl \
    $(CyberCore)/svg/SVGPolygonElement.idl \
    $(CyberCore)/svg/SVGPolylineElement.idl \
    $(CyberCore)/svg/SVGPreserveAspectRatio.idl \
    $(CyberCore)/svg/SVGRadialGradientElement.idl \
    $(CyberCore)/svg/SVGRect.idl \
    $(CyberCore)/svg/SVGRectElement.idl \
    $(CyberCore)/svg/SVGRenderingIntent.idl \
    $(CyberCore)/svg/SVGSVGElement.idl \
    $(CyberCore)/svg/SVGScriptElement.idl \
    $(CyberCore)/svg/SVGSetElement.idl \
    $(CyberCore)/svg/SVGStopElement.idl \
    $(CyberCore)/svg/SVGStringList.idl \
    $(CyberCore)/svg/SVGStyleElement.idl \
    $(CyberCore)/svg/SVGSwitchElement.idl \
    $(CyberCore)/svg/SVGSymbolElement.idl \
    $(CyberCore)/svg/SVGTRefElement.idl \
    $(CyberCore)/svg/SVGTSpanElement.idl \
    $(CyberCore)/svg/SVGTests.idl \
    $(CyberCore)/svg/SVGTextContentElement.idl \
    $(CyberCore)/svg/SVGTextElement.idl \
    $(CyberCore)/svg/SVGTextPathElement.idl \
    $(CyberCore)/svg/SVGTextPositioningElement.idl \
    $(CyberCore)/svg/SVGTitleElement.idl \
    $(CyberCore)/svg/SVGTransform.idl \
    $(CyberCore)/svg/SVGTransformList.idl \
    $(CyberCore)/svg/SVGURIReference.idl \
    $(CyberCore)/svg/SVGUnitTypes.idl \
    $(CyberCore)/svg/SVGUseElement.idl \
    $(CyberCore)/svg/SVGVKernElement.idl \
    $(CyberCore)/svg/SVGViewElement.idl \
    $(CyberCore)/svg/SVGViewSpec.idl \
    $(CyberCore)/svg/SVGZoomAndPan.idl \
    $(CyberCore)/svg/SVGZoomEvent.idl \
    $(CyberCore)/testing/GCObservation.idl \
    $(CyberCore)/testing/InternalSettings.idl \
    $(CyberCore)/testing/Internals.idl \
    $(CyberCore)/testing/InternalsMapLike.idl \
    $(CyberCore)/testing/InternalsSetLike.idl \
    $(CyberCore)/testing/MallocStatistics.idl \
    $(CyberCore)/testing/MemoryInfo.idl \
    $(CyberCore)/testing/MockCDMFactory.idl \
    $(CyberCore)/testing/MockContentFilterSettings.idl \
    $(CyberCore)/testing/MockPageOverlay.idl \
    $(CyberCore)/testing/MockPaymentAddress.idl \
    $(CyberCore)/testing/MockPaymentContactFields.idl \
    $(CyberCore)/testing/MockPaymentCoordinator.idl \
    $(CyberCore)/testing/MockPaymentError.idl \
    $(CyberCore)/testing/MockWebAuthenticationConfiguration.idl \
    $(CyberCore)/testing/ServiceWorkerInternals.idl \
    $(CyberCore)/testing/TypeConversions.idl \
    $(CyberCore)/testing/FakeXRBoundsPoint.idl \
    $(CyberCore)/testing/FakeXRButtonStateInit.idl \
    $(CyberCore)/testing/FakeXRInputSourceInit.idl \
    $(CyberCore)/testing/FakeXRRigidTransformInit.idl \
    $(CyberCore)/testing/FakeXRViewInit.idl \
    $(CyberCore)/testing/WebFakeXRDevice.idl \
    $(CyberCore)/testing/WebFakeXRInputController.idl \
    $(CyberCore)/testing/WebXRTest.idl \
    $(CyberCore)/testing/XRSimulateUserActivationFunction.idl \
    $(CyberCore)/workers/AbstractWorker.idl \
    $(CyberCore)/workers/DedicatedWorkerGlobalScope.idl \
    $(CyberCore)/workers/Worker.idl \
    $(CyberCore)/workers/WorkerGlobalScope.idl \
    $(CyberCore)/workers/WorkerLocation.idl \
    $(CyberCore)/workers/WorkerType.idl \
    $(CyberCore)/workers/service/ExtendableEvent.idl \
    $(CyberCore)/workers/service/ExtendableEventInit.idl \
    $(CyberCore)/workers/service/ExtendableMessageEvent.idl \
    $(CyberCore)/workers/service/FetchEvent.idl \
    $(CyberCore)/workers/service/ServiceWorker.idl \
    $(CyberCore)/workers/service/ServiceWorkerClient.idl \
    $(CyberCore)/workers/service/ServiceWorkerClientType.idl \
    $(CyberCore)/workers/service/ServiceWorkerClients.idl \
    $(CyberCore)/workers/service/ServiceWorkerContainer.idl \
    $(CyberCore)/workers/service/ServiceWorkerGlobalScope.idl \
    $(CyberCore)/workers/service/ServiceWorkerRegistration.idl \
    $(CyberCore)/workers/service/ServiceWorkerUpdateViaCache.idl \
    $(CyberCore)/workers/service/ServiceWorkerWindowClient.idl \
    $(CyberCore)/worklets/PaintWorkletGlobalScope.idl \
    $(CyberCore)/worklets/Worklet.idl \
    $(CyberCore)/worklets/WorkletGlobalScope.idl \
    $(CyberCore)/xml/CustomXPathNSResolver.idl \
    $(CyberCore)/xml/DOMParser.idl \
    $(CyberCore)/xml/XMLHttpRequest.idl \
    $(CyberCore)/xml/XMLHttpRequestEventTarget.idl \
    $(CyberCore)/xml/XMLHttpRequestProgressEvent.idl \
    $(CyberCore)/xml/XMLHttpRequestUpload.idl \
    $(CyberCore)/xml/XMLSerializer.idl \
    $(CyberCore)/xml/XPathEvaluator.idl \
    $(CyberCore)/xml/XPathExpression.idl \
    $(CyberCore)/xml/XPathNSResolver.idl \
    $(CyberCore)/xml/XPathResult.idl \
    $(CyberCore)/xml/XSLTProcessor.idl \
    InternalSettingsGenerated.idl \
#

# --------

ADDITIONAL_BINDING_IDLS =

ifeq ($(findstring ENABLE_IOS_GESTURE_EVENTS,$(FEATURE_AND_PLATFORM_DEFINES)), ENABLE_IOS_GESTURE_EVENTS)
ADDITIONAL_BINDING_IDLS += GestureEvent.idl
endif

ifeq ($(findstring ENABLE_IOS_TOUCH_EVENTS,$(FEATURE_AND_PLATFORM_DEFINES)), ENABLE_IOS_TOUCH_EVENTS)
ADDITIONAL_BINDING_IDLS += \
    DocumentTouch.idl \
    Touch.idl \
    TouchEvent.idl \
    TouchList.idl
endif

ifeq ($(findstring ENABLE_MAC_GESTURE_EVENTS,$(FEATURE_AND_PLATFORM_DEFINES)), ENABLE_MAC_GESTURE_EVENTS)
ADDITIONAL_BINDING_IDLS += GestureEvent.idl
endif

vpath %.in $(WEBKITADDITIONS_HEADER_SEARCH_PATHS)

ADDITIONAL_EVENT_NAMES =
ADDITIONAL_EVENT_TARGET_FACTORY =

-include CyberCoreDerivedSourcesAdditions.make

JS_BINDING_IDLS += $(ADDITIONAL_BINDING_IDLS)

all : $(ADDITIONAL_BINDING_IDLS:%.idl=JS%.h)

vpath %.idl $(BUILT_PRODUCTS_DIR)/usr/local/include $(SDKROOT)/usr/local/include

$(ADDITIONAL_BINDING_IDLS) : % : CyberKitAdditions/%
	cp $< .

ifneq ($(findstring ENABLE_IOS_TOUCH_EVENTS,$(FEATURE_AND_PLATFORM_DEFINES)), ENABLE_IOS_TOUCH_EVENTS)
JS_BINDING_IDLS += \
    $(CyberCore)/dom/DocumentTouch.idl \
    $(CyberCore)/dom/Touch.idl \
    $(CyberCore)/dom/TouchEvent.idl \
    $(CyberCore)/dom/TouchList.idl
endif

.PHONY : all

JS_DOM_CLASSES=$(basename $(notdir $(JS_BINDING_IDLS)))

JS_DOM_HEADERS=$(filter-out JSEventListener.h, $(JS_DOM_CLASSES:%=JS%.h))
JS_DOM_IMPLEMENTATIONS=$(filter-out JSEventListener.cpp, $(JS_DOM_CLASSES:%=JS%.cpp))

WEB_DOM_HEADERS :=

all : \
    $(JS_DOM_HEADERS) \
    $(JS_DOM_IMPLEMENTATIONS) \
    $(WEB_DOM_HEADERS) \
    \
    CSSPropertyNames.cpp \
    CSSPropertyNames.h \
    CSSValueKeywords.cpp \
    CSSValueKeywords.h \
    ColorData.cpp \
    DOMJITAbstractHeapRepository.h \
    EventInterfaces.h \
    EventTargetInterfaces.h \
    HTMLElementFactory.cpp \
    HTMLElementFactory.h \
    HTMLElementTypeHelpers.h \
    HTMLEntityTable.cpp \
    HTMLNames.cpp \
    HTMLNames.h \
    JSHTMLElementWrapperFactory.cpp \
    JSHTMLElementWrapperFactory.h \
    JSMathMLElementWrapperFactory.cpp \
    JSMathMLElementWrapperFactory.h \
    JSSVGElementWrapperFactory.cpp \
    JSSVGElementWrapperFactory.h \
    PlugInsResources.h \
    SVGElementFactory.cpp \
    SVGElementFactory.h \
    SVGElementTypeHelpers.h \
    SVGNames.cpp \
    SVGNames.h \
    SelectorPseudoClassAndCompatibilityElementMap.cpp \
    SelectorPseudoElementTypeMap.cpp \
    StyleBuilderGenerated.cpp \
    StylePropertyShorthandFunctions.cpp \
    StylePropertyShorthandFunctions.h \
    UserAgentStyleSheets.h \
    CyberKitFontFamilyNames.cpp \
    CyberKitFontFamilyNames.h \
    XLinkNames.cpp \
    XMLNSNames.cpp \
    XMLNames.cpp \
    MathMLElementFactory.cpp \
    MathMLElementFactory.h \
    MathMLElementTypeHelpers.h \
    MathMLNames.cpp \
    MathMLNames.h \
#

# --------

# CSS property names and value keywords

WEBCORE_CSS_PROPERTY_NAMES := $(CyberCore)/css/CSSProperties.json
WEBCORE_CSS_VALUE_KEYWORDS := $(CyberCore)/css/CSSValueKeywords.in
WEBCORE_CSS_VALUE_KEYWORDS := $(WEBCORE_CSS_VALUE_KEYWORDS) $(CyberCore)/css/SVGCSSValueKeywords.in

CSS_PROPERTY_NAME_FILES = \
    CSSPropertyNames.cpp \
    CSSPropertyNames.h \
    StyleBuilderGenerated.cpp \
    StylePropertyShorthandFunctions.cpp \
    StylePropertyShorthandFunctions.h \
#
CSS_PROPERTY_NAME_FILES_PATTERNS = $(subst .,%,$(CSS_PROPERTY_NAME_FILES))

all : $(CSS_PROPERTY_NAME_FILES)
$(CSS_PROPERTY_NAME_FILES_PATTERNS) : $(WEBCORE_CSS_PROPERTY_NAMES) css/makeprop.pl $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PERL) -pe '' $(WEBCORE_CSS_PROPERTY_NAMES) > CSSProperties.json
	$(PERL) "$(CyberCore)/css/makeprop.pl" --defines "$(FEATURE_AND_PLATFORM_DEFINES)"

CSS_VALUE_KEYWORD_FILES = \
    CSSValueKeywords.h \
    CSSValueKeywords.cpp \
#
CSS_VALUE_KEYWORD_FILES_PATTERNS = $(subst .,%,$(CSS_VALUE_KEYWORD_FILES))

all : $(CSS_VALUE_KEYWORD_FILES)
$(CSS_VALUE_KEYWORD_FILES_PATTERNS) : $(WEBCORE_CSS_VALUE_KEYWORDS) css/makevalues.pl bindings/scripts/preprocessor.pm $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PERL) -pe '' $(WEBCORE_CSS_VALUE_KEYWORDS) > CSSValueKeywords.in
	$(PERL) "$(CyberCore)/css/makevalues.pl" --defines "$(FEATURE_AND_PLATFORM_DEFINES)"

# --------

# CSS Selector pseudo type name to value map.

SelectorPseudoClassAndCompatibilityElementMap.cpp : $(CyberCore)/css/makeSelectorPseudoClassAndCompatibilityElementMap.py $(CyberCore)/css/SelectorPseudoClassAndCompatibilityElementMap.in $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PYTHON) "$(CyberCore)/css/makeSelectorPseudoClassAndCompatibilityElementMap.py" $(CyberCore)/css/SelectorPseudoClassAndCompatibilityElementMap.in gperf "$(FEATURE_AND_PLATFORM_DEFINES)"

SelectorPseudoElementTypeMap.cpp : $(CyberCore)/css/makeSelectorPseudoElementsMap.py $(CyberCore)/css/SelectorPseudoElementTypeMap.in $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PYTHON) "$(CyberCore)/css/makeSelectorPseudoElementsMap.py" $(CyberCore)/css/SelectorPseudoElementTypeMap.in gperf "$(FEATURE_AND_PLATFORM_DEFINES)"

# --------

# DOMJIT Abstract Heap

all : DOMJITAbstractHeapRepository.h

DOMJITAbstractHeapRepository.h : $(CyberCore)/domjit/generate-abstract-heap.rb $(CyberCore)/domjit/DOMJITAbstractHeapRepository.yaml
	$(RUBY) "$(CyberCore)/domjit/generate-abstract-heap.rb" $(CyberCore)/domjit/DOMJITAbstractHeapRepository.yaml ./DOMJITAbstractHeapRepository.h

# --------

# XMLViewer CSS

all : XMLViewerCSS.h

XMLViewerCSS.h : xml/XMLViewer.css
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/cssmin.py <"$(CyberCore)/xml/XMLViewer.css" > ./XMLViewer.min.css
	$(PERL) $(CyberScriptCore_SCRIPTS_DIR)/xxd.pl XMLViewer_css ./XMLViewer.min.css XMLViewerCSS.h
	$(DELETE) XMLViewer.min.css

# --------

# XMLViewer JS

all : XMLViewerJS.h

XMLViewerJS.h : xml/XMLViewer.js
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/jsmin.py <"$(CyberCore)/xml/XMLViewer.js" > ./XMLViewer.min.js
	$(PERL) $(CyberScriptCore_SCRIPTS_DIR)/xxd.pl XMLViewer_js ./XMLViewer.min.js XMLViewerJS.h
	$(DELETE) XMLViewer.min.js

# --------

# HTML entity names

HTMLEntityTable.cpp : html/parser/HTMLEntityNames.in $(CyberCore)/html/parser/create-html-entity-table
	$(PYTHON) $(CyberCore)/html/parser/create-html-entity-table -o HTMLEntityTable.cpp $(CyberCore)/html/parser/HTMLEntityNames.in

# --------

# HTTP header names

HTTP_HEADER_NAMES_FILES = \
    HTTPHeaderNames.cpp \
    HTTPHeaderNames.gperf \
    HTTPHeaderNames.h \
#
HTTP_HEADER_NAMES_FILES_PATTERNS = $(subst .,%,$(HTTP_HEADER_NAMES_FILES))

all : $(HTTP_HEADER_NAMES_FILES)
$(HTTP_HEADER_NAMES_FILES_PATTERNS) : platform/network/HTTPHeaderNames.in $(CyberCore)/platform/network/create-http-header-name-table
	$(PYTHON) $(CyberCore)/platform/network/create-http-header-name-table $(CyberCore)/platform/network/HTTPHeaderNames.in gperf

# --------

# color names

ColorData.cpp : platform/ColorData.gperf $(CyberCore)/make-hash-tools.pl
	$(PERL) $(CyberCore)/make-hash-tools.pl . $(CyberCore)/platform/ColorData.gperf

# --------

# user agent style sheets

USER_AGENT_STYLE_SHEETS = $(CyberCore)/css/html.css $(CyberCore)/css/dialog.css $(CyberCore)/css/quirks.css $(CyberCore)/css/plugIns.css $(CyberCore)/css/svg.css

ifeq ($(findstring ENABLE_MATHML,$(FEATURE_AND_PLATFORM_DEFINES)), ENABLE_MATHML)
    USER_AGENT_STYLE_SHEETS += $(CyberCore)/css/mathml.css
endif

ifeq ($(findstring ENABLE_VIDEO,$(FEATURE_AND_PLATFORM_DEFINES)), ENABLE_VIDEO)
    USER_AGENT_STYLE_SHEETS += $(CyberCore)/css/mediaControls.css
endif

ifeq ($(findstring ENABLE_FULLSCREEN_API,$(FEATURE_AND_PLATFORM_DEFINES)), ENABLE_FULLSCREEN_API)
    USER_AGENT_STYLE_SHEETS += $(CyberCore)/css/fullscreen.css
endif

ifeq ($(findstring ENABLE_SERVICE_CONTROLS,$(FEATURE_AND_PLATFORM_DEFINES)), ENABLE_SERVICE_CONTROLS)
    USER_AGENT_STYLE_SHEETS += $(CyberCore)/html/shadow/mac/imageControlsMac.css
endif

USER_AGENT_STYLE_SHEETS += $(CyberCore)/Modules/plugins/QuickTimePluginReplacement.css

ifeq ($(findstring ENABLE_METER_ELEMENT,$(FEATURE_AND_PLATFORM_DEFINES)), ENABLE_METER_ELEMENT)
	USER_AGENT_STYLE_SHEETS += $(CyberCore)/html/shadow/meterElementShadow.css
endif

UserAgentStyleSheets.h : css/make-css-file-arrays.pl bindings/scripts/preprocessor.pm $(USER_AGENT_STYLE_SHEETS) $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PERL) $< --defines "$(FEATURE_AND_PLATFORM_DEFINES)" $@ UserAgentStyleSheetsData.cpp $(USER_AGENT_STYLE_SHEETS)

# --------

# user agent scripts

USER_AGENT_SCRIPTS = $(CyberCore)/Modules/plugins/QuickTimePluginReplacement.js

USER_AGENT_SCRIPTS_FILES = \
    UserAgentScripts.h \
    UserAgentScriptsData.cpp \
#
USER_AGENT_SCRIPTS_FILES_PATTERNS = $(subst .,%,$(USER_AGENT_SCRIPTS_FILES))

all : $(USER_AGENT_SCRIPTS_FILES)

$(USER_AGENT_SCRIPTS_FILES_PATTERNS) : $(CyberScriptCore_SCRIPTS_DIR)/make-js-file-arrays.py $(USER_AGENT_SCRIPTS)
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/make-js-file-arrays.py -n CyberCore $(USER_AGENT_SCRIPTS_FILES) $(USER_AGENT_SCRIPTS)

# --------

# plug-ins resources

PLUG_INS_RESOURCES = $(CyberCore)/Resources/plugIns.js

PlugInsResources.h : css/make-css-file-arrays.pl bindings/scripts/preprocessor.pm $(PLUG_INS_RESOURCES) $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PERL) $< --defines "$(FEATURE_AND_PLATFORM_DEFINES)" $@ PlugInsResourcesData.cpp $(PLUG_INS_RESOURCES)

# --------

WEBKIT_FONT_FAMILY_NAME_FILES = \
    CyberKitFontFamilyNames.cpp \
    CyberKitFontFamilyNames.h \
#
WEBKIT_FONT_FAMILY_NAME_FILES_PATTERNS = $(subst .,%,$(WEBKIT_FONT_FAMILY_NAME_FILES))

all : $(WEBKIT_FONT_FAMILY_NAME_FILES)
$(WEBKIT_FONT_FAMILY_NAME_FILES_PATTERNS): dom/make_names.pl bindings/scripts/Hasher.pm bindings/scripts/StaticString.pm css/CyberKitFontFamilyNames.in
	$(PERL) $< --fonts $(CyberCore)/css/CyberKitFontFamilyNames.in

# HTML tag and attribute names

HTML_TAG_FILES = \
    JSHTMLElementWrapperFactory.cpp \
    JSHTMLElementWrapperFactory.h \
    HTMLElementFactory.cpp \
    HTMLElementFactory.h \
    HTMLElementTypeHelpers.h \
    HTMLNames.cpp \
    HTMLNames.h \
#
HTML_TAG_FILES_PATTERNS = $(subst .,%,$(HTML_TAG_FILES))

all : $(HTML_TAG_FILES)

$(HTML_TAG_FILES_PATTERNS) : dom/make_names.pl bindings/scripts/Hasher.pm bindings/scripts/StaticString.pm html/HTMLTagNames.in html/HTMLAttributeNames.in $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PERL) $< --tags $(CyberCore)/html/HTMLTagNames.in --attrs $(CyberCore)/html/HTMLAttributeNames.in --factory --wrapperFactory --extraDefines "$(FEATURE_AND_PLATFORM_DEFINES)"

XMLNSNames.cpp : dom/make_names.pl bindings/scripts/Hasher.pm bindings/scripts/StaticString.pm xml/xmlnsattrs.in
	$(PERL) $< --attrs $(CyberCore)/xml/xmlnsattrs.in

XMLNames.cpp : dom/make_names.pl bindings/scripts/Hasher.pm bindings/scripts/StaticString.pm xml/xmlattrs.in
	$(PERL) $< --attrs $(CyberCore)/xml/xmlattrs.in

# --------

# SVG tag and attribute names, and element factory

SVG_TAG_FILES = \
    JSSVGElementWrapperFactory.cpp \
    JSSVGElementWrapperFactory.h \
    SVGElementFactory.cpp \
    SVGElementFactory.h \
    SVGElementTypeHelpers.h \
    SVGNames.cpp \
    SVGNames.h \
#
SVG_TAG_FILES_PATTERNS = $(subst .,%,$(SVG_TAG_FILES))

all : $(SVG_TAG_FILES)

$(SVG_TAG_FILES_PATTERNS) : dom/make_names.pl bindings/scripts/Hasher.pm bindings/scripts/StaticString.pm svg/svgtags.in svg/svgattrs.in $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PERL) $< --tags $(CyberCore)/svg/svgtags.in --attrs $(CyberCore)/svg/svgattrs.in --factory --wrapperFactory --extraDefines "$(FEATURE_AND_PLATFORM_DEFINES)"

XLinkNames.cpp : dom/make_names.pl bindings/scripts/Hasher.pm bindings/scripts/StaticString.pm svg/xlinkattrs.in
	$(PERL) $< --attrs $(CyberCore)/svg/xlinkattrs.in

# --------

# Register event constructors and targets

EVENT_NAMES = EventNames.in $(ADDITIONAL_EVENT_NAMES)

EVENT_FACTORY_FILES = \
    EventFactory.cpp \
    EventHeaders.h \
    EventInterfaces.h \
#
EVENT_FACTORY_PATTERNS = $(subst .,%,$(EVENT_FACTORY_FILES))

all : $(EVENT_FACTORY_FILES)
$(EVENT_FACTORY_PATTERNS) : dom/make_event_factory.pl $(EVENT_NAMES)
	$(PERL) $< $(addprefix --input , $(filter-out $(CyberCore)/dom/make_event_factory.pl, $^))

EVENT_TARGET_FACTORY = EventTargetFactory.in $(ADDITIONAL_EVENT_TARGET_FACTORY)

EVENT_TARGET_FACTORY_FILES = \
    EventTargetFactory.cpp \
    EventTargetHeaders.h \
    EventTargetInterfaces.h \
#
EVENT_TARGET_FACTORY_PATTERNS = $(subst .,%,$(EVENT_TARGET_FACTORY_FILES))

all : $(EVENT_TARGET_FACTORY_FILES)
$(EVENT_TARGET_FACTORY_PATTERNS) : dom/make_event_factory.pl $(EVENT_TARGET_FACTORY)
	$(PERL) $< $(addprefix --input , $(filter-out $(CyberCore)/dom/make_event_factory.pl, $^))

# --------

# MathML tag and attribute names, and element factory

MATH_ML_GENERATED_FILES = \
    JSMathMLElementWrapperFactory.cpp \
    JSMathMLElementWrapperFactory.h \
    MathMLElementFactory.cpp \
    MathMLElementFactory.h \
    MathMLElementTypeHelpers.h \
    MathMLNames.cpp \
    MathMLNames.h \
#
MATH_ML_GENERATED_PATTERNS = $(subst .,%,$(MATH_ML_GENERATED_FILES))

all : $(MATH_ML_GENERATED_FILES)
$(MATH_ML_GENERATED_PATTERNS) : dom/make_names.pl bindings/scripts/Hasher.pm bindings/scripts/StaticString.pm mathml/mathtags.in mathml/mathattrs.in
	$(PERL) $< --tags $(CyberCore)/mathml/mathtags.in --attrs $(CyberCore)/mathml/mathattrs.in --factory --wrapperFactory

# --------

# Internal Settings

GENERATE_SETTINGS_TEMPLATES = \
    $(CyberCore)/Scripts/SettingsTemplates/InternalSettingsGenerated.cpp.erb \
    $(CyberCore)/Scripts/SettingsTemplates/InternalSettingsGenerated.idl.erb \
    $(CyberCore)/Scripts/SettingsTemplates/InternalSettingsGenerated.h.erb \
    $(CyberCore)/Scripts/SettingsTemplates/Settings.cpp.erb \
    $(CyberCore)/Scripts/SettingsTemplates/Settings.h.erb \
#
GENERATE_SETTINGS_FILES = $(basename $(notdir $(GENERATE_SETTINGS_TEMPLATES)))
GENERATE_SETTINGS_PATTERNS = $(subst .,%,$(GENERATE_SETTINGS_FILES))

all : $(GENERATE_SETTINGS_FILES)
$(GENERATE_SETTINGS_PATTERNS) : $(CyberCore)/Scripts/GenerateSettings.rb $(GENERATE_SETTINGS_TEMPLATES) page/Settings.yaml
	$(RUBY) $< --input $(CyberCore)/page/Settings.yaml

# --------

# WHLSL Standard Library

all : WHLSLStandardLibrary.h WHLSLStandardLibraryFunctionMap.cpp

WHLSLStandardLibrary.h : $(CyberScriptCore_SCRIPTS_DIR)/xxd.pl $(CyberCore)/Modules/webgpu/WHLSL/WHLSLStandardLibrary.txt
	bash -c "$(PERL) $< WHLSLStandardLibrary <(gzip -cn $(CyberCore)/Modules/webgpu/WHLSL/WHLSLStandardLibrary.txt) $@"

WHLSLStandardLibraryFunctionMap.cpp : $(CyberCore)/Modules/webgpu/WHLSL/WHLSLBuildStandardLibraryFunctionMap.py $(CyberCore)/Modules/webgpu/WHLSL/WHLSLStandardLibrary.txt
	$(PYTHON) $< $(CyberCore)/Modules/webgpu/WHLSL/WHLSLStandardLibrary.txt $@

# --------

# Common generator things

COMMON_BINDINGS_SCRIPTS = \
    bindings/scripts/CodeGenerator.pm \
    bindings/scripts/IDLParser.pm \
    bindings/scripts/generate-bindings.pl \
    bindings/scripts/preprocessor.pm

PREPROCESS_IDLS_SCRIPTS = \
    bindings/scripts/preprocess-idls.pl

# JS bindings generator

IDL_INCLUDES = \
    $(CyberCore)/Modules \
    $(CyberCore)/accessibility \
    $(CyberCore)/animation \
    $(CyberCore)/css \
    $(CyberCore)/css/typedom \
    $(CyberCore)/crypto \
    $(CyberCore)/dom \
    $(CyberCore)/fileapi \
    $(CyberCore)/html \
    $(CyberCore)/html/canvas \
    $(CyberCore)/html/shadow \
    $(CyberCore)/html/track \
    $(CyberCore)/inspector \
    $(CyberCore)/loader/appcache \
    $(CyberCore)/mathml \
    $(CyberCore)/page \
    $(CyberCore)/plugins \
    $(CyberCore)/storage \
    $(CyberCore)/svg \
    $(CyberCore)/testing \
    $(CyberCore)/workers \
    $(CyberCore)/worklets \
    $(CyberCore)/xml

IDL_COMMON_ARGS = $(IDL_INCLUDES:%=--include %) --write-dependencies --outputDir .

JS_BINDINGS_SCRIPTS = $(COMMON_BINDINGS_SCRIPTS) bindings/scripts/CodeGeneratorJS.pm

SUPPLEMENTAL_DEPENDENCY_FILE = SupplementalDependencies.txt
SUPPLEMENTAL_MAKEFILE_DEPS = SupplementalDependencies.dep
ISO_SUBSPACES_HEADER_FILE = DOMIsoSubspaces.h
WINDOW_CONSTRUCTORS_FILE = DOMWindowConstructors.idl
WORKERGLOBALSCOPE_CONSTRUCTORS_FILE = WorkerGlobalScopeConstructors.idl
DEDICATEDWORKERGLOBALSCOPE_CONSTRUCTORS_FILE = DedicatedWorkerGlobalScopeConstructors.idl
SERVICEWORKERGLOBALSCOPE_CONSTRUCTORS_FILE = ServiceWorkerGlobalScopeConstructors.idl
WORKLETGLOBALSCOPE_CONSTRUCTORS_FILE = WorkletGlobalScopeConstructors.idl
PAINTWORKLETGLOBALSCOPE_CONSTRUCTORS_FILE = PaintWorkletGlobalScopeConstructors.idl
IDL_FILES_TMP = idl_files.tmp
IDL_ATTRIBUTES_FILE = $(CyberCore)/bindings/scripts/IDLAttributes.json

# The following lines get a newline character stored in a variable.
# See <http://stackoverflow.com/questions/7039811/how-do-i-process-extremely-long-lists-of-files-in-a-make-recipe>.
define NL


endef


IDL_INTERMEDIATE_FILES = \
    $(SUPPLEMENTAL_MAKEFILE_DEPS) \
    $(SUPPLEMENTAL_DEPENDENCY_FILE) \
    $(ISO_SUBSPACES_HEADER_FILE) \
    $(WINDOW_CONSTRUCTORS_FILE) \
    $(WORKERGLOBALSCOPE_CONSTRUCTORS_FILE) \
    $(DEDICATEDWORKERGLOBALSCOPE_CONSTRUCTORS_FILE) \
    $(SERVICEWORKERGLOBALSCOPE_CONSTRUCTORS_FILE) \
    $(WORKLETGLOBALSCOPE_CONSTRUCTORS_FILE) \
    $(PAINTWORKLETGLOBALSCOPE_CONSTRUCTORS_FILE)
#
IDL_INTERMEDIATE_PATTERNS = $(subst .,%,$(IDL_INTERMEDIATE_FILES))

$(IDL_INTERMEDIATE_PATTERNS) : $(PREPROCESS_IDLS_SCRIPTS) $(JS_BINDING_IDLS) $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES) DerivedSources.make $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(foreach f,$(JS_BINDING_IDLS),@echo $(f)>>$(IDL_FILES_TMP)$(NL))
	$(PERL) $(CyberCore)/bindings/scripts/preprocess-idls.pl --defines "$(FEATURE_AND_PLATFORM_DEFINES) LANGUAGE_JAVASCRIPT" --idlFilesList $(IDL_FILES_TMP) --supplementalDependencyFile $(SUPPLEMENTAL_DEPENDENCY_FILE) --isoSubspacesHeaderFile $(ISO_SUBSPACES_HEADER_FILE) --windowConstructorsFile $(WINDOW_CONSTRUCTORS_FILE) --workerGlobalScopeConstructorsFile $(WORKERGLOBALSCOPE_CONSTRUCTORS_FILE) --dedicatedWorkerGlobalScopeConstructorsFile $(DEDICATEDWORKERGLOBALSCOPE_CONSTRUCTORS_FILE) --serviceWorkerGlobalScopeConstructorsFile $(SERVICEWORKERGLOBALSCOPE_CONSTRUCTORS_FILE) --workletGlobalScopeConstructorsFile $(WORKLETGLOBALSCOPE_CONSTRUCTORS_FILE) --paintWorkletGlobalScopeConstructorsFile $(PAINTWORKLETGLOBALSCOPE_CONSTRUCTORS_FILE) --supplementalMakefileDeps $(SUPPLEMENTAL_MAKEFILE_DEPS)
	$(DELETE) $(IDL_FILES_TMP)

JS%.cpp JS%.h : %.idl $(JS_BINDINGS_SCRIPTS) $(IDL_ATTRIBUTES_FILE) $(IDL_INTERMEDIATE_FILES) $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PERL) $(CyberCore)/bindings/scripts/generate-bindings.pl $(IDL_COMMON_ARGS) --defines "$(FEATURE_AND_PLATFORM_DEFINES) LANGUAGE_JAVASCRIPT" --generator JS --idlAttributesFile $(IDL_ATTRIBUTES_FILE) --supplementalDependencyFile $(SUPPLEMENTAL_DEPENDENCY_FILE) $<

ifneq ($(NO_SUPPLEMENTAL_FILES),1)
-include $(SUPPLEMENTAL_MAKEFILE_DEPS)
endif

ifneq ($(NO_SUPPLEMENTAL_FILES),1)
-include $(JS_DOM_HEADERS:.h=.dep)
endif

# Inspector interfaces

all : CommandLineAPIModuleSource.h

CommandLineAPIModuleSource.h : CommandLineAPIModuleSource.js
	echo "//# sourceURL=__InjectedScript_CommandLineAPIModuleSource.js" > ./CommandLineAPIModuleSource.min.js
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/jsmin.py <$(CyberCore)/inspector/CommandLineAPIModuleSource.js >> ./CommandLineAPIModuleSource.min.js
	$(PERL) $(CyberScriptCore_SCRIPTS_DIR)/xxd.pl CommandLineAPIModuleSource_js ./CommandLineAPIModuleSource.min.js CommandLineAPIModuleSource.h
	$(DELETE) CommandLineAPIModuleSource.min.js

# CyberCore JS Builtins

CyberCore_BUILTINS_SOURCES = \
    $(CyberCore)/Modules/mediastream/RTCPeerConnection.js \
    $(CyberCore)/Modules/mediastream/RTCPeerConnectionInternals.js \
    $(CyberCore)/Modules/streams/ByteLengthQueuingStrategy.js \
    $(CyberCore)/Modules/streams/CountQueuingStrategy.js \
    $(CyberCore)/Modules/streams/ReadableByteStreamController.js \
    $(CyberCore)/Modules/streams/ReadableByteStreamInternals.js \
    $(CyberCore)/Modules/streams/ReadableStream.js \
    $(CyberCore)/Modules/streams/ReadableStreamBYOBRequest.js \
    $(CyberCore)/Modules/streams/ReadableStreamDefaultController.js \
    $(CyberCore)/Modules/streams/ReadableStreamInternals.js \
    $(CyberCore)/Modules/streams/ReadableStreamBYOBReader.js \
    $(CyberCore)/Modules/streams/ReadableStreamDefaultReader.js \
    $(CyberCore)/Modules/streams/StreamInternals.js \
    $(CyberCore)/Modules/streams/WritableStream.js \
    $(CyberCore)/Modules/streams/WritableStreamDefaultController.js \
    $(CyberCore)/Modules/streams/WritableStreamDefaultWriter.js \
    $(CyberCore)/Modules/streams/WritableStreamInternals.js \
    $(CyberCore)/bindings/js/JSDOMBindingInternals.js \
#

BUILTINS_GENERATOR_SCRIPTS = \
    $(CyberScriptCore_SCRIPTS_DIR)/wkbuiltins.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generator.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_model.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_templates.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_combined_header.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_combined_implementation.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_separate_header.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_separate_implementation.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_internals_wrapper_header.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_internals_wrapper_implementation.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_wrapper_header.py \
    $(CyberScriptCore_SCRIPTS_DIR)/builtins_generate_wrapper_implementation.py \
    $(CyberScriptCore_SCRIPTS_DIR)/generate-js-builtins.py \
    $(CyberScriptCore_SCRIPTS_DIR)/lazywriter.py \
#

CyberCore_BUILTINS_WRAPPERS = \
    CyberCoreJSBuiltins.h \
    CyberCoreJSBuiltins.cpp \
    CyberCoreJSBuiltinInternals.h \
    CyberCoreJSBuiltinInternals.cpp \
#
CyberCore_BUILTINS_WRAPPERS_PATTERNS = $(subst .,%,$(CyberCore_BUILTINS_WRAPPERS))

# Adding/removing scripts should trigger regeneration, but changing which builtins are
# generated should not affect other builtins when not passing '--combined' to the generator.

CyberCore_BUILTINS_SOURCES_LIST : $(CyberScriptCore_SCRIPTS_DIR)/UpdateContents.py DerivedSources.make
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/UpdateContents.py '$(CyberCore_BUILTINS_SOURCES)' $@

CyberCore_BUILTINS_DEPENDENCIES_LIST : $(CyberScriptCore_SCRIPTS_DIR)/UpdateContents.py DerivedSources.make
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/UpdateContents.py '$(BUILTINS_GENERATOR_SCRIPTS)' $@

$(CyberCore_BUILTINS_WRAPPERS_PATTERNS) : $(CyberCore_BUILTINS_SOURCES) CyberCore_BUILTINS_SOURCES_LIST $(BUILTINS_GENERATOR_SCRIPTS) CyberCore_BUILTINS_DEPENDENCIES_LIST
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/generate-js-builtins.py --wrappers-only --output-directory . --framework CyberCore $(CyberCore_BUILTINS_SOURCES)

%Builtins.h: %.js $(BUILTINS_GENERATOR_SCRIPTS) CyberCore_BUILTINS_DEPENDENCIES_LIST
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/generate-js-builtins.py --output-directory . --framework CyberCore $<

all : $(notdir $(CyberCore_BUILTINS_SOURCES:%.js=%Builtins.h)) $(CyberCore_BUILTINS_WRAPPERS)

# ------------------------

