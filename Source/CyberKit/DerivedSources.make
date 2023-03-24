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

VPATH = \
    $(CyberKit2) \
    $(CyberKit2)/GPUProcess \
    $(CyberKit2)/GPUProcess/graphics \
    $(CyberKit2)/GPUProcess/mac \
    $(CyberKit2)/GPUProcess/media \
    $(CyberKit2)/GPUProcess/media/ios \
    $(CyberKit2)/GPUProcess/webrtc \
    $(CyberKit2)/NetworkProcess \
    $(CyberKit2)/NetworkProcess/Cookies \
    $(CyberKit2)/NetworkProcess/cache \
    $(CyberKit2)/NetworkProcess/CustomProtocols \
    $(CyberKit2)/NetworkProcess/mac \
    $(CyberKit2)/NetworkProcess/webrtc \
    $(CyberKit2)/NetworkProcess/IndexedDB \
    $(CyberKit2)/NetworkProcess/ServiceWorker \
    $(CyberKit2)/NetworkProcess/WebStorage \
    $(CyberKit2)/NetworkProcess/storage \
    $(CyberKit2)/Resources/SandboxProfiles/ios \
    $(CyberKit2)/Shared/Plugins \
    $(CyberKit2)/Shared \
    $(CyberKit2)/Shared/API/Cocoa \
    $(CyberKit2)/Shared/ApplePay \
    $(CyberKit2)/Shared/Authentication \
    $(CyberKit2)/Shared/mac \
    $(CyberKit2)/Shared/Notifications \
    $(CyberKit2)/WebProcess/ApplePay \
    $(CyberKit2)/WebProcess/ApplicationCache \
    $(CyberKit2)/WebProcess/Automation \
    $(CyberKit2)/WebProcess/Cache \
    $(CyberKit2)/WebProcess/Databases/IndexedDB \
    $(CyberKit2)/WebProcess/Extensions/Interfaces \
    $(CyberKit2)/WebProcess/FullScreen \
    $(CyberKit2)/WebProcess/Geolocation \
    $(CyberKit2)/WebProcess/GPU \
    $(CyberKit2)/WebProcess/GPU/graphics \
    $(CyberKit2)/WebProcess/GPU/media \
    $(CyberKit2)/WebProcess/GPU/media/ios \
    $(CyberKit2)/WebProcess/GPU/webrtc \
    $(CyberKit2)/WebProcess/IconDatabase \
    $(CyberKit2)/WebProcess/Inspector \
    $(CyberKit2)/WebProcess/MediaCache \
    $(CyberKit2)/WebProcess/MediaSession \
    $(CyberKit2)/WebProcess/MediaStream \
    $(CyberKit2)/WebProcess/Network \
    $(CyberKit2)/WebProcess/Network/webrtc \
    $(CyberKit2)/WebProcess/Notifications \
    $(CyberKit2)/WebProcess/OriginData \
    $(CyberKit2)/WebProcess/Plugins \
    $(CyberKit2)/WebProcess/ResourceCache \
	$(CyberKit2)/WebProcess/Speech \
    $(CyberKit2)/WebProcess/Storage \
    $(CyberKit2)/WebProcess/UserContent \
    $(CyberKit2)/WebProcess/WebAuthentication \
    $(CyberKit2)/WebProcess/CyberCoreSupport \
    $(CyberKit2)/WebProcess/WebPage \
    $(CyberKit2)/WebProcess/WebPage/Cocoa \
    $(CyberKit2)/WebProcess/WebPage/RemoteLayerTree \
    $(CyberKit2)/WebProcess/WebStorage \
    $(CyberKit2)/WebProcess/cocoa \
    $(CyberKit2)/WebProcess/ios \
    $(CyberKit2)/WebProcess \
    $(CyberKit2)/UIProcess \
    $(CyberKit2)/UIProcess/Automation \
    $(CyberKit2)/UIProcess/Cocoa \
    $(CyberKit2)/UIProcess/Databases \
    $(CyberKit2)/UIProcess/Downloads \
    $(CyberKit2)/UIProcess/GPU \
    $(CyberKit2)/UIProcess/Inspector \
    $(CyberKit2)/UIProcess/Inspector/Agents \
    $(CyberKit2)/UIProcess/Media \
    $(CyberKit2)/UIProcess/Media/cocoa \
    $(CyberKit2)/UIProcess/MediaStream \
    $(CyberKit2)/UIProcess/Network \
    $(CyberKit2)/UIProcess/Network/CustomProtocols \
    $(CyberKit2)/UIProcess/Notifications \
    $(CyberKit2)/UIProcess/Plugins \
    $(CyberKit2)/UIProcess/RemoteLayerTree \
    $(CyberKit2)/UIProcess/Storage \
    $(CyberKit2)/UIProcess/UserContent \
    $(CyberKit2)/UIProcess/WebAuthentication \
    $(CyberKit2)/UIProcess/mac \
    $(CyberKit2)/UIProcess/ios \
    $(CyberKit2)/webpushd/mac \
    $(WEBKITADDITIONS_HEADER_SEARCH_PATHS) \
#

# Workaround for rdar://84212106.
find_tool = $(realpath $(shell xcrun --sdk $(SDK_NAME) -f $(1)))

PYTHON := $(call find_tool,python3)
PERL := perl
RUBY := ruby

ifeq ($(OS),Windows_NT)
    DELETE = cmd //C del
else
    DELETE = rm -f
endif

MESSAGE_RECEIVERS = \
	NetworkProcess/NetworkBroadcastChannelRegistry \
	NetworkProcess/NetworkConnectionToWebProcess \
	NetworkProcess/NetworkContentRuleListManager \
	NetworkProcess/cache/CacheStorageEngineConnection \
	NetworkProcess/CustomProtocols/LegacyCustomProtocolManager \
	NetworkProcess/NetworkSocketChannel \
	NetworkProcess/ServiceWorker/WebSWServerConnection \
	NetworkProcess/ServiceWorker/ServiceWorkerDownloadTask \
	NetworkProcess/ServiceWorker/ServiceWorkerFetchTask \
	NetworkProcess/ServiceWorker/WebSWServerToContextConnection \
	NetworkProcess/SharedWorker/WebSharedWorkerServerConnection \
	NetworkProcess/SharedWorker/WebSharedWorkerServerToContextConnection \
	NetworkProcess/NetworkSocketStream \
	NetworkProcess/NetworkProcess \
	NetworkProcess/NetworkResourceLoader \
	NetworkProcess/webrtc/NetworkMDNSRegister \
	NetworkProcess/webrtc/NetworkRTCProvider \
	NetworkProcess/webrtc/NetworkRTCMonitor \
	NetworkProcess/webrtc/RTCDataChannelRemoteManagerProxy \
	NetworkProcess/Cookies/WebCookieManager \
	NetworkProcess/storage/NetworkStorageManager \
	Shared/AuxiliaryProcess \
	Shared/API/Cocoa/RemoteObjectRegistry \
	Shared/ApplePay/WebPaymentCoordinatorProxy \
	Shared/Authentication/AuthenticationManager \
	Shared/Notifications/NotificationManagerMessageHandler \
	Shared/WebConnection \
	Shared/IPCConnectionTester \
	Shared/IPCStreamTester \
	Shared/IPCStreamTesterProxy \
	Shared/IPCTester \
	Shared/IPCTesterReceiver \
	UIProcess/WebFullScreenManagerProxy \
	UIProcess/RemoteLayerTree/RemoteLayerTreeDrawingAreaProxy \
	UIProcess/GPU/GPUProcessProxy \
	UIProcess/WebAuthentication/WebAuthenticatorCoordinatorProxy \
	UIProcess/WebPasteboardProxy \
	UIProcess/UserContent/WebUserContentControllerProxy \
	UIProcess/Inspector/WebInspectorUIProxy \
	UIProcess/Inspector/RemoteWebInspectorUIProxy \
	UIProcess/Inspector/WebInspectorUIExtensionControllerProxy \
	UIProcess/DrawingAreaProxy \
	UIProcess/Network/NetworkProcessProxy \
	UIProcess/Network/CustomProtocols/LegacyCustomProtocolManagerProxy \
	UIProcess/WebFrameProxy \
	UIProcess/WebPageProxy \
	UIProcess/VisitedLinkStore \
	UIProcess/ios/WebDeviceOrientationUpdateProviderProxy \
	UIProcess/ios/SmartMagnificationController \
	UIProcess/mac/SecItemShimProxy \
	UIProcess/WebGeolocationManagerProxy \
	UIProcess/WebLockRegistryProxy \
	UIProcess/WebPermissionControllerProxy \
	UIProcess/Cocoa/PlaybackSessionManagerProxy \
	UIProcess/Cocoa/UserMediaCaptureManagerProxy \
	UIProcess/Cocoa/VideoFullscreenManagerProxy \
	UIProcess/ViewGestureController \
	UIProcess/WebProcessProxy \
	UIProcess/Automation/WebAutomationSession \
	UIProcess/WebProcessPool \
	UIProcess/WebScreenOrientationManagerProxy \
	UIProcess/Downloads/DownloadProxy \
	UIProcess/Extensions/WebExtensionContext \
	UIProcess/Extensions/WebExtensionController \
	UIProcess/Media/AudioSessionRoutingArbitratorProxy \
	UIProcess/Media/RemoteMediaSessionCoordinatorProxy \
	UIProcess/SpeechRecognitionRemoteRealtimeMediaSourceManager \
	UIProcess/SpeechRecognitionServer \
	UIProcess/XR/PlatformXRSystem \
	WebProcess/Databases/IndexedDB/WebIDBConnectionToServer \
	WebProcess/Extensions/WebExtensionContextProxy \
	WebProcess/Extensions/WebExtensionControllerProxy \
	WebProcess/GPU/GPUProcessConnection \
	WebProcess/GPU/graphics/RemoteRenderingBackendProxy \
	WebProcess/GPU/graphics/RemoteGraphicsContextGLProxy \
	WebProcess/GPU/graphics/WebGPU/RemoteGPUProxy \
	WebProcess/GPU/webrtc/LibWebRTCCodecs \
	WebProcess/GPU/webrtc/SampleBufferDisplayLayer \
	WebProcess/GPU/media/MediaPlayerPrivateRemote \
	WebProcess/GPU/media/MediaSourcePrivateRemote \
	WebProcess/GPU/media/RemoteAudioHardwareListener \
	WebProcess/GPU/media/RemoteAudioSession \
	WebProcess/GPU/media/RemoteAudioSourceProviderManager \
	WebProcess/GPU/media/RemoteCDMInstance \
	WebProcess/GPU/media/RemoteCDMInstanceSession \
	WebProcess/GPU/media/RemoteImageDecoderAVFManager \
	WebProcess/GPU/media/RemoteLegacyCDMSession \
	WebProcess/GPU/media/RemoteRemoteCommandListener \
	WebProcess/GPU/media/SourceBufferPrivateRemote \
	WebProcess/GPU/media/ios/RemoteMediaSessionHelper \
	WebProcess/GPU/webrtc/RemoteVideoFrameObjectHeapProxyProcessor \
	WebProcess/WebStorage/StorageAreaMap \
	WebProcess/UserContent/WebUserContentController \
	WebProcess/Inspector/WebInspectorInterruptDispatcher \
	WebProcess/Inspector/WebInspectorUI \
	WebProcess/Inspector/WebInspectorUIExtensionController \
	WebProcess/Inspector/WebInspector \
	WebProcess/Inspector/RemoteWebInspectorUI \
	WebProcess/MediaSession/RemoteMediaSessionCoordinator \
	WebProcess/Network/WebSocketChannel \
	WebProcess/Network/NetworkProcessConnection \
	WebProcess/Network/WebSocketStream \
	WebProcess/Network/WebResourceLoader \
	WebProcess/Network/webrtc/LibWebRTCNetwork \
	WebProcess/Network/webrtc/RTCDataChannelRemoteManager \
	WebProcess/Network/webrtc/WebRTCMonitor \
	WebProcess/Network/webrtc/WebMDNSRegister \
	WebProcess/Network/webrtc/WebRTCResolver \
	WebProcess/CyberCoreSupport/RemoteWebLockRegistry \
	WebProcess/CyberCoreSupport/WebBroadcastChannelRegistry \
	WebProcess/CyberCoreSupport/WebDeviceOrientationUpdateProvider \
	WebProcess/CyberCoreSupport/WebFileSystemStorageConnection \
	WebProcess/CyberCoreSupport/WebPermissionController \
	WebProcess/CyberCoreSupport/WebScreenOrientationManager \
	WebProcess/CyberCoreSupport/WebSpeechRecognitionConnection \
	WebProcess/Speech/SpeechRecognitionRealtimeMediaSourceManager \
	WebProcess/Storage/WebSharedWorkerContextManagerConnection \
	WebProcess/Storage/WebSharedWorkerObjectConnection \
	WebProcess/Storage/WebSWContextManagerConnection \
	WebProcess/Storage/WebSWClientConnection \
	WebProcess/WebProcess \
	WebProcess/cocoa/PlaybackSessionManager \
	WebProcess/cocoa/RemoteCaptureSampleManager \
	WebProcess/cocoa/UserMediaCaptureManager \
	WebProcess/cocoa/VideoFullscreenManager \
	WebProcess/Geolocation/WebGeolocationManager \
	WebProcess/Automation/WebAutomationSessionProxy \
	WebProcess/FullScreen/WebFullScreenManager \
	WebProcess/ApplePay/WebPaymentCoordinator \
	WebProcess/Notifications/WebNotificationManager \
	WebProcess/WebPage/EventDispatcher \
	WebProcess/WebPage/RemoteLayerTree/RemoteScrollingCoordinator \
	WebProcess/WebPage/ViewGestureGeometryCollector \
	WebProcess/WebPage/DrawingArea \
	WebProcess/WebPage/WebFrame \
	WebProcess/WebPage/WebPage \
	WebProcess/WebPage/VisitedLinkTableController \
	WebProcess/WebPage/Cocoa/TextCheckingControllerProxy \
	WebProcess/WebPage/ViewUpdateDispatcher \
	WebProcess/XR/PlatformXRSystemProxy \
	GPUProcess/GPUConnectionToWebProcess \
	GPUProcess/graphics/RemoteDisplayListRecorder \
	GPUProcess/graphics/RemoteRenderingBackend \
	GPUProcess/graphics/RemoteGraphicsContextGL \
	GPUProcess/graphics/WebGPU/RemoteAdapter \
	GPUProcess/graphics/WebGPU/RemoteBindGroup \
	GPUProcess/graphics/WebGPU/RemoteBindGroupLayout \
	GPUProcess/graphics/WebGPU/RemoteBuffer \
	GPUProcess/graphics/WebGPU/RemoteCommandBuffer \
	GPUProcess/graphics/WebGPU/RemoteCommandEncoder \
	GPUProcess/graphics/WebGPU/RemoteCompositorIntegration \
	GPUProcess/graphics/WebGPU/RemoteComputePassEncoder \
	GPUProcess/graphics/WebGPU/RemoteComputePipeline \
	GPUProcess/graphics/WebGPU/RemoteDevice \
	GPUProcess/graphics/WebGPU/RemoteExternalTexture \
	GPUProcess/graphics/WebGPU/RemoteGPU \
	GPUProcess/graphics/WebGPU/RemotePipelineLayout \
	GPUProcess/graphics/WebGPU/RemotePresentationContext \
	GPUProcess/graphics/WebGPU/RemoteQuerySet \
	GPUProcess/graphics/WebGPU/RemoteQueue \
	GPUProcess/graphics/WebGPU/RemoteRenderBundle \
	GPUProcess/graphics/WebGPU/RemoteRenderBundleEncoder \
	GPUProcess/graphics/WebGPU/RemoteRenderPassEncoder \
	GPUProcess/graphics/WebGPU/RemoteRenderPipeline \
	GPUProcess/graphics/WebGPU/RemoteSampler \
	GPUProcess/graphics/WebGPU/RemoteShaderModule \
	GPUProcess/graphics/WebGPU/RemoteTexture \
	GPUProcess/graphics/WebGPU/RemoteTextureView \
	GPUProcess/webrtc/LibWebRTCCodecsProxy \
	GPUProcess/webrtc/RemoteSampleBufferDisplayLayerManager \
	GPUProcess/webrtc/RemoteMediaRecorderManager \
	GPUProcess/webrtc/RemoteSampleBufferDisplayLayer \
	GPUProcess/webrtc/RemoteMediaRecorder \
	GPUProcess/webrtc/RemoteAudioMediaStreamTrackRendererInternalUnitManager \
	GPUProcess/GPUProcess \
	GPUProcess/media/RemoteImageDecoderAVFProxy \
	GPUProcess/media/RemoteLegacyCDMSessionProxy \
	GPUProcess/media/RemoteLegacyCDMFactoryProxy \
	GPUProcess/media/RemoteAudioSessionProxy \
	GPUProcess/media/RemoteCDMInstanceSessionProxy \
	GPUProcess/media/RemoteCDMProxy \
	GPUProcess/media/ios/RemoteMediaSessionHelperProxy \
	GPUProcess/media/RemoteAudioDestinationManager \
	GPUProcess/media/RemoteCDMFactoryProxy \
	GPUProcess/media/RemoteCDMInstanceProxy \
	GPUProcess/media/RemoteLegacyCDMProxy \
	GPUProcess/media/RemoteMediaEngineConfigurationFactoryProxy \
	GPUProcess/media/RemoteMediaPlayerManagerProxy \
	GPUProcess/media/RemoteMediaPlayerProxy \
	GPUProcess/media/RemoteMediaResourceManager \
	GPUProcess/media/RemoteVideoFrameObjectHeap \
	GPUProcess/media/RemoteMediaSourceProxy \
	GPUProcess/media/RemoteRemoteCommandListenerProxy \
	GPUProcess/media/RemoteSourceBufferProxy \
#

GENERATE_MESSAGE_RECEIVER_SCRIPT = $(CyberKit2)/Scripts/generate-message-receiver.py
GENERATE_MESSAGE_RECEIVER_SCRIPTS = \
    $(GENERATE_MESSAGE_RECEIVER_SCRIPT) \
    $(CyberKit2)/Scripts/webkit/__init__.py \
    $(CyberKit2)/Scripts/webkit/messages.py \
    $(CyberKit2)/Scripts/webkit/model.py \
    $(CyberKit2)/Scripts/webkit/parser.py \
    $(CyberKit2)/DerivedSources.make \
#

FRAMEWORK_FLAGS := $(addprefix -F, $(BUILT_PRODUCTS_DIR) $(FRAMEWORK_SEARCH_PATHS) $(SYSTEM_FRAMEWORK_SEARCH_PATHS))
HEADER_FLAGS := $(addprefix -I, $(BUILT_PRODUCTS_DIR) $(HEADER_SEARCH_PATHS) $(SYSTEM_HEADER_SEARCH_PATHS))
EXTERNAL_FLAGS := -DRELEASE_WITHOUT_OPTIMIZATIONS $(addprefix -D, $(GCC_PREPROCESSOR_DEFINITIONS))

platform_h_compiler_command = $(CC) -std=c++2a -x c++ $(1) $(SDK_FLAGS) $(TARGET_TRIPLE_FLAGS) $(FRAMEWORK_FLAGS) $(HEADER_FLAGS) $(EXTERNAL_FLAGS) -include "wtf/Platform.h" /dev/null

FEATURE_AND_PLATFORM_DEFINES := $(shell $(call platform_h_compiler_command,-E -P -dM) | $(PERL) -ne "print if s/\#define ((HAVE_|USE_|ENABLE_|WTF_PLATFORM_)\w+) 1/\1/")

PLATFORM_HEADER_DIR := $(realpath $(BUILT_PRODUCTS_DIR)$(WK_LIBRARY_HEADERS_FOLDER_PATH))
PLATFORM_HEADER_DEPENDENCIES := $(filter $(PLATFORM_HEADER_DIR)/%,$(realpath $(shell $(call platform_h_compiler_command,-M) | $(PERL) -e "local \$$/; my (\$$target, \$$deps) = split(/:/, <>); print split(/\\\\/, \$$deps);")))
FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES = $(CyberKit2)/DerivedSources.make $(PLATFORM_HEADER_DEPENDENCIES)

MESSAGE_RECEIVER_FILES := $(addsuffix MessageReceiver.cpp,$(notdir $(MESSAGE_RECEIVERS)))
MESSAGES_FILES := $(addsuffix Messages.h,$(notdir $(MESSAGE_RECEIVERS)))

GENERATED_MESSAGES_FILES := $(MESSAGE_RECEIVER_FILES) $(MESSAGES_FILES) MessageNames.h MessageNames.cpp MessageArgumentDescriptions.cpp
GENERATED_MESSAGES_FILES_AS_PATTERNS := $(subst .,%,$(GENERATED_MESSAGES_FILES))

MESSAGES_IN_FILES := $(addsuffix .messages.in,$(MESSAGE_RECEIVERS))

.PHONY : all

all : $(GENERATED_MESSAGES_FILES)

$(GENERATED_MESSAGES_FILES_AS_PATTERNS) : $(MESSAGES_IN_FILES) $(GENERATE_MESSAGE_RECEIVER_SCRIPTS)
	$(PYTHON) $(GENERATE_MESSAGE_RECEIVER_SCRIPT) $(CyberKit2) $(MESSAGE_RECEIVERS)

TEXT_PREPROCESSOR_FLAGS=-E -P -w

ifneq ($(SDKROOT),)
	SDK_FLAGS=-isysroot $(SDKROOT)
endif

WK_CURRENT_ARCH=$(word 1, $(ARCHS))
TARGET_TRIPLE_FLAGS=-target $(WK_CURRENT_ARCH)-$(LLVM_TARGET_TRIPLE_VENDOR)-$(LLVM_TARGET_TRIPLE_OS_VERSION)$(LLVM_TARGET_TRIPLE_SUFFIX)

ifeq ($(USE_SYSTEM_CONTENT_PATH),YES)
	SANDBOX_DEFINES = -DUSE_SYSTEM_CONTENT_PATH=1 -DSYSTEM_CONTENT_PATH=$(SYSTEM_CONTENT_PATH)
endif

SANDBOX_PROFILES = \
	com.apple.WebProcess.sb \
	com.apple.CyberKit.NetworkProcess.sb \
	com.apple.CyberKit.GPUProcess.sb \
	com.apple.CyberKit.webpushd.mac.sb
	
SANDBOX_PROFILES_IOS = \
	com.apple.CyberKit.adattributiond.sb \
	com.apple.CyberKit.webpushd.sb \
	com.apple.CyberKit.GPU.sb \
	com.apple.CyberKit.Networking.sb \
	com.apple.CyberKit.WebContent.sb

sandbox-profiles-ios : $(SANDBOX_PROFILES_IOS)

all : $(SANDBOX_PROFILES) $(SANDBOX_PROFILES_IOS)

%.sb : %.sb.in
	@echo Pre-processing $* sandbox profile...
	grep -o '^[^;]*' $< | $(CC) $(SDK_FLAGS) $(TARGET_TRIPLE_FLAGS) $(SANDBOX_DEFINES) $(TEXT_PREPROCESSOR_FLAGS) $(FRAMEWORK_FLAGS) $(HEADER_FLAGS) $(EXTERNAL_FLAGS) -include "wtf/Platform.h" - > $@

AUTOMATION_PROTOCOL_GENERATOR_SCRIPTS = \
	$(CyberScriptCore_SCRIPTS_DIR)/cpp_generator_templates.py \
	$(CyberScriptCore_SCRIPTS_DIR)/cpp_generator.py \
	$(CyberScriptCore_SCRIPTS_DIR)/generate_cpp_backend_dispatcher_header.py \
	$(CyberScriptCore_SCRIPTS_DIR)/generate_cpp_backend_dispatcher_implementation.py \
	$(CyberScriptCore_SCRIPTS_DIR)/generate_cpp_frontend_dispatcher_header.py \
	$(CyberScriptCore_SCRIPTS_DIR)/generate_cpp_frontend_dispatcher_implementation.py \
	$(CyberScriptCore_SCRIPTS_DIR)/generate_cpp_protocol_types_header.py \
	$(CyberScriptCore_SCRIPTS_DIR)/generate_cpp_protocol_types_implementation.py \
	$(CyberScriptCore_SCRIPTS_DIR)/generator_templates.py \
	$(CyberScriptCore_SCRIPTS_DIR)/generator.py \
	$(CyberScriptCore_SCRIPTS_DIR)/models.py \
	$(CyberScriptCore_SCRIPTS_DIR)/generate-inspector-protocol-bindings.py \
#

AUTOMATION_PROTOCOL_INPUT_FILES = \
    $(CyberKit2)/UIProcess/Automation/Automation.json \
#

AUTOMATION_PROTOCOL_OUTPUT_FILES = \
    AutomationBackendDispatchers.h \
    AutomationBackendDispatchers.cpp \
    AutomationFrontendDispatchers.h \
    AutomationFrontendDispatchers.cpp \
    AutomationProtocolObjects.h \
    AutomationProtocolObjects.cpp \
#
AUTOMATION_PROTOCOL_OUTPUT_PATTERNS = $(subst .,%,$(AUTOMATION_PROTOCOL_OUTPUT_FILES))

# JSON-RPC Frontend Dispatchers, Backend Dispatchers, Type Builders
$(AUTOMATION_PROTOCOL_OUTPUT_PATTERNS) : $(AUTOMATION_PROTOCOL_INPUT_FILES) $(AUTOMATION_PROTOCOL_GENERATOR_SCRIPTS)
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/generate-inspector-protocol-bindings.py --framework CyberKit --backend --outputDir . $(AUTOMATION_PROTOCOL_INPUT_FILES)

all : $(AUTOMATION_PROTOCOL_OUTPUT_FILES)

%ScriptSource.h : %.js $(CyberScriptCore_SCRIPTS_DIR)/jsmin.py $(CyberScriptCore_SCRIPTS_DIR)/xxd.pl
	echo "//# sourceURL=__InjectedScript_$(notdir $<)" > $(basename $(notdir $<)).min.js
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/jsmin.py < $< >> $(basename $(notdir $<)).min.js
	$(PERL) $(CyberScriptCore_SCRIPTS_DIR)/xxd.pl $(basename $(notdir $<))ScriptSource $(basename $(notdir $<)).min.js $@
	$(DELETE) $(basename $(notdir $<)).min.js

all : WebAutomationSessionProxyScriptSource.h

# WebPreferences generation

WEB_PREFERENCES = \
    $(WTF_BUILD_SCRIPTS_DIR)/Preferences/UnifiedWebPreferences.yaml \
    $(ADDITIONAL_WEB_PREFERENCES_INPUT_FILES) \
#

WEB_PREFERENCES_TEMPLATES = \
    $(CyberKit2)/Scripts/PreferencesTemplates/WebPageUpdatePreferences.cpp.erb \
    $(CyberKit2)/Scripts/PreferencesTemplates/WebPreferencesDefinitions.h.erb \
    $(CyberKit2)/Scripts/PreferencesTemplates/WebPreferencesFeatures.cpp.erb \
    $(CyberKit2)/Scripts/PreferencesTemplates/WebPreferencesGetterSetters.cpp.erb \
    $(CyberKit2)/Scripts/PreferencesTemplates/WebPreferencesKeys.cpp.erb \
    $(CyberKit2)/Scripts/PreferencesTemplates/WebPreferencesKeys.h.erb \
    $(CyberKit2)/Scripts/PreferencesTemplates/WebPreferencesStoreDefaultsMap.cpp.erb \
#
WEB_PREFERENCES_FILES = $(basename $(notdir $(WEB_PREFERENCES_TEMPLATES)))
WEB_PREFERENCES_PATTERNS = $(subst .,%,$(WEB_PREFERENCES_FILES))

all : $(WEB_PREFERENCES_FILES)

$(WEB_PREFERENCES_PATTERNS) : $(WTF_BUILD_SCRIPTS_DIR)/GeneratePreferences.rb $(WEB_PREFERENCES_TEMPLATES) $(WEB_PREFERENCES)
	$(RUBY) $< --frontend CyberKit $(addprefix --template , $(WEB_PREFERENCES_TEMPLATES)) $(WEB_PREFERENCES)

SERIALIZATION_DESCRIPTION_FILES = \
	GPUProcess/GPUProcessSessionParameters.serialization.in \
	GPUProcess/graphics/InlinePathData.serialization.in \
	GPUProcess/graphics/RemoteRenderingBackendCreationParameters.serialization.in \
	GPUProcess/graphics/WebGPU/RemoteGPURequestAdapterResponse.serialization.in \
	GPUProcess/media/AudioTrackPrivateRemoteConfiguration.serialization.in \
	GPUProcess/media/InitializationSegmentInfo.serialization.in \
	GPUProcess/media/MediaDescriptionInfo.serialization.in \
	GPUProcess/media/RemoteMediaPlayerProxyConfiguration.serialization.in \
	GPUProcess/media/TextTrackPrivateRemoteConfiguration.serialization.in \
	GPUProcess/media/TrackPrivateRemoteConfiguration.serialization.in \
	GPUProcess/media/VideoTrackPrivateRemoteConfiguration.serialization.in \
	NetworkProcess/NetworkProcessCreationParameters.serialization.in \
	Shared/API/APIError.serialization.in \
	Shared/API/APIFrameHandle.serialization.in \
	NetworkProcess/NetworkResourceLoadParameters.serialization.in \
	Shared/API/APIGeometry.serialization.in \
	Shared/API/APIPageHandle.serialization.in \
	Shared/API/APIURL.serialization.in \
	Shared/API/APIURLRequest.serialization.in \
	Shared/API/APIURLResponse.serialization.in \
	Shared/AlternativeTextClient.serialization.in \
	Shared/Cocoa/CacheStoragePolicy.serialization.in \
	Shared/Cocoa/DataDetectionResult.serialization.in \
	Shared/Cocoa/RevealItem.serialization.in \
	Shared/Cocoa/CyberCoreArgumentCodersCocoa.serialization.in \
	Shared/CallbackID.serialization.in \
	Shared/BackgroundFetchState.serialization.in \
	Shared/DisplayListArgumentCoders.serialization.in \
	Shared/EditorState.serialization.in \
	Shared/Extensions/WebExtensionEventListenerType.serialization.in \
	Shared/FileSystemSyncAccessHandleInfo.serialization.in \
	Shared/FocusedElementInformation.serialization.in \
	Shared/FrameInfoData.serialization.in \
	Shared/FrameTreeCreationParameters.serialization.in \
	Shared/FrameTreeNodeData.serialization.in \
	Shared/Gamepad/GamepadData.serialization.in \
	Shared/GPUProcessConnectionParameters.serialization.in \
	Shared/ios/DynamicViewportSizeUpdate.serialization.in \
	Shared/ios/InteractionInformationAtPosition.serialization.in \
	Shared/ios/WebAutocorrectionContext.serialization.in \
	Shared/ios/WebAutocorrectionData.serialization.in \
	Shared/LayerTreeContext.serialization.in \
	Shared/Model.serialization.in \
	Shared/NavigationActionData.serialization.in \
	Shared/NetworkProcessConnectionParameters.serialization.in \
	Shared/PALArgumentCoders.serialization.in \
	Shared/Pasteboard.serialization.in \
	Shared/PlatformPopupMenuData.serialization.in \
	Shared/PolicyDecision.serialization.in \
	Shared/SameDocumentNavigationType.serialization.in \
	Shared/SessionState.serialization.in \
	Shared/ShareableBitmap.serialization.in \
	Shared/TextFlags.serialization.in \
	Shared/TextRecognitionResult.serialization.in \
	Shared/TouchBarMenuItemData.serialization.in \
	Shared/WTFArgumentCoders.serialization.in \
	Shared/CyberCoreArgumentCoders.serialization.in \
	Shared/WebExtensionContextParameters.serialization.in \
	Shared/WebEvent.serialization.in \
	Shared/WebExtensionControllerParameters.serialization.in \
	Shared/WebHitTestResultData.serialization.in \
	Shared/WebPopupItem.serialization.in \
	Shared/WebProcessDataStoreParameters.serialization.in \
	Shared/WebPushDaemonConnectionConfiguration.serialization.in \
	Shared/WebPushMessage.serialization.in \
	Shared/WebsitePoliciesData.serialization.in \
	Shared/ApplePay/ApplePayPaymentSetupFeatures.serialization.in \
	Shared/ApplePay/PaymentSetupConfiguration.serialization.in \
	Shared/Databases/IndexedDB/WebIDBResult.serialization.in \
	Shared/RemoteLayerTree/RemoteLayerTree.serialization.in \
	Shared/mac/PDFContextMenuItem.serialization.in \
	Shared/mac/SecItemRequestData.serialization.in \
	Shared/mac/SecItemResponseData.serialization.in \
	Shared/mac/WebHitTestResultPlatformData.serialization.in \
	Shared/WebsiteDataStoreParameters.serialization.in \
	Shared/WebsiteData/WebsiteData.serialization.in \
	Shared/WebsiteData/WebsiteDataFetchOption.serialization.in \
	Shared/WebsiteData/WebsiteDataType.serialization.in \
	Shared/WebGPU/WebGPUBindGroupDescriptor.serialization.in \
	Shared/WebGPU/WebGPUBindGroupLayoutDescriptor.serialization.in \
	Shared/WebGPU/WebGPUBindGroupLayoutEntry.serialization.in \
	Shared/WebGPU/WebGPUBufferDescriptor.serialization.in \
	Shared/WebGPU/WebGPUCommandBufferDescriptor.serialization.in \
	Shared/WebGPU/WebGPUCommandEncoderDescriptor.serialization.in \
	Shared/WebGPU/WebGPUCompilationMessage.serialization.in \
	Shared/WebGPU/WebGPUComputePassDescriptor.serialization.in \
	Shared/WebGPU/WebGPUComputePipelineDescriptor.serialization.in \
	Shared/WebGPU/WebGPUDepthStencilState.serialization.in \
	Shared/WebGPU/WebGPUDeviceDescriptor.serialization.in \
	Shared/WebGPU/WebGPUExtent3D.serialization.in \
	Shared/WebGPU/WebGPUExternalTextureBindingLayout.serialization.in \
	Shared/WebGPU/WebGPUExternalTextureDescriptor.serialization.in \
	Shared/WebGPU/WebGPUFeatureName.serialization.in \
	Shared/WebGPU/WebGPUFragmentState.serialization.in \
	Shared/WebGPU/WebGPUImageCopyBuffer.serialization.in \
	Shared/WebGPU/WebGPUImageCopyExternalImage.serialization.in \
	Shared/WebGPU/WebGPUImageCopyTexture.serialization.in \
	Shared/WebGPU/WebGPUImageCopyTextureTagged.serialization.in \
	Shared/WebGPU/WebGPUImageDataLayout.serialization.in \
	Shared/WebGPU/WebGPUMultisampleState.serialization.in \
	Shared/WebGPU/WebGPUOrigin2D.serialization.in \
	Shared/WebGPU/WebGPUOutOfMemoryError.serialization.in \
	Shared/WebGPU/WebGPUPipelineDescriptorBase.serialization.in \
	Shared/WebGPU/WebGPUPipelineLayoutDescriptor.serialization.in \
	Shared/WebGPU/WebGPUPresentationContextDescriptor.serialization.in \
	Shared/WebGPU/WebGPUQuerySetDescriptor.serialization.in \
	Shared/WebGPU/WebGPURenderBundleDescriptor.serialization.in \
	Shared/WebGPU/WebGPURenderBundleEncoderDescriptor.serialization.in \
	Shared/WebGPU/WebGPURenderPassDescriptor.serialization.in \
	Shared/WebGPU/WebGPURenderPassLayout.serialization.in \
	Shared/WebGPU/WebGPURenderPipelineDescriptor.serialization.in \
	Shared/WebGPU/WebGPURequestAdapterOptions.serialization.in \
	Shared/WebGPU/WebGPUSamplerBindingLayout.serialization.in \
	Shared/WebGPU/WebGPUSamplerDescriptor.serialization.in \
	Shared/WebGPU/WebGPUShaderModuleCompilationHint.serialization.in \
	Shared/WebGPU/WebGPUShaderModuleDescriptor.serialization.in \
	Shared/WebGPU/WebGPUTextureDescriptor.serialization.in \
	Shared/WebGPU/WebGPUTextureViewDescriptor.serialization.in \
	Shared/WebGPU/WebGPUVertexState.serialization.in \
	Shared/WebGPU/WebGPUVertexBufferLayout.serialization.in \
 	Shared/WebGPU/WebGPUVertexAttribute.serialization.in \
	Shared/WebGPU/WebGPUValidationError.serialization.in \
	Shared/WebGPU/WebGPUTextureBindingLayout.serialization.in \
	Shared/WebGPU/WebGPUSupportedLimits.serialization.in \
	Shared/WebGPU/WebGPUSupportedFeatures.serialization.in \
	Shared/WebGPU/WebGPUStorageTextureBindingLayout.serialization.in \
	Shared/WebGPU/WebGPUStencilFaceState.serialization.in \
	Shared/WebGPU/WebGPURenderPassTimestampWrites.serialization.in \
	Shared/WebGPU/WebGPURenderPassDepthStencilAttachment.serialization.in \
	Shared/WebGPU/WebGPURenderPassColorAttachment.serialization.in \
	Shared/WebGPU/WebGPUProgrammableStage.serialization.in \
	Shared/WebGPU/WebGPUPrimitiveState.serialization.in \
	Shared/WebGPU/WebGPUOrigin3D.serialization.in \
	Shared/WebGPU/WebGPUObjectDescriptorBase.serialization.in \
	Shared/WebGPU/WebGPUComputePassTimestampWrites.serialization.in \
	Shared/WebGPU/WebGPUColorTargetState.serialization.in \
	Shared/WebGPU/WebGPUColor.serialization.in \
	Shared/WebGPU/WebGPUCanvasConfiguration.serialization.in \
	Shared/WebGPU/WebGPUBufferBindingLayout.serialization.in \
	Shared/WebGPU/WebGPUBufferBinding.serialization.in \
	Shared/WebGPU/WebGPUBlendState.serialization.in \
	Shared/WebGPU/WebGPUBlendComponent.serialization.in \
	Shared/WebGPU/WebGPUBindGroupEntry.serialization.in \
	Shared/XR/XRSystem.serialization.in \
	WebProcess/GPU/graphics/BufferIdentifierSet.serialization.in \
	WebProcess/GPU/graphics/PrepareBackingStoreBuffersData.serialization.in \
	WebProcess/GPU/media/RemoteCDMConfiguration.serialization.in \
	WebProcess/GPU/media/RemoteAudioSessionConfiguration.serialization.in \
	WebProcess/GPU/media/RemoteMediaPlayerConfiguration.serialization.in \
	WebProcess/GPU/media/RemoteMediaPlayerState.serialization.in \
	WebProcess/CyberCoreSupport/WebSpeechSynthesisVoice.serialization.in \
#

all : GeneratedSerializers.h GeneratedSerializers.mm SerializedTypeInfo.mm

GeneratedSerializers.h GeneratedSerializers.mm SerializedTypeInfo.mm : $(CyberKit2)/Scripts/generate-serializers.py $(SERIALIZATION_DESCRIPTION_FILES) $(CyberKit2)/DerivedSources.make
	$(PYTHON) $(CyberKit2)/Scripts/generate-serializers.py mm $(CyberKit2)/ $(SERIALIZATION_DESCRIPTION_FILES)

EXTENSIONS_DIR = $(CyberKit2)/WebProcess/Extensions
EXTENSIONS_SCRIPTS_DIR = $(EXTENSIONS_DIR)/Bindings/Scripts
EXTENSIONS_INTERFACES_DIR = $(EXTENSIONS_DIR)/Interfaces
IDL_ATTRIBUTES_FILE = $(EXTENSIONS_SCRIPTS_DIR)/IDLAttributes.json

BINDINGS_SCRIPTS = \
    $(CyberCorePrivateHeaders)/generate-bindings.pl \
    $(CyberCorePrivateHeaders)/IDLParser.pm \
    $(CyberCorePrivateHeaders)/CodeGenerator.pm \
    $(EXTENSIONS_SCRIPTS_DIR)/CodeGeneratorExtensions.pm \
#

EXTENSION_INTERFACES = \
    WebExtensionAPIEvent \
    WebExtensionAPIExtension \
    WebExtensionAPINamespace \
    WebExtensionAPIPermissions \
    WebExtensionAPIRuntime \
    WebExtensionAPITest \
    WebExtensionAPIWebNavigation \
    WebExtensionAPIWebNavigationEvent \
#

JS%.h JS%.mm : %.idl $(BINDINGS_SCRIPTS) $(IDL_ATTRIBUTES_FILE) $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	@echo Generating bindings for $*...
	$(PERL) -I $(CyberCorePrivateHeaders) -I $(EXTENSIONS_SCRIPTS_DIR) $(CyberCorePrivateHeaders)/generate-bindings.pl --defines "$(FEATURE_AND_PLATFORM_DEFINES)" --include $(EXTENSIONS_INTERFACES_DIR) --outputDir . --generator Extensions --idlAttributesFile $(IDL_ATTRIBUTES_FILE) $<

all : $(EXTENSION_INTERFACES:%=JS%.h) $(EXTENSION_INTERFACES:%=JS%.mm)
