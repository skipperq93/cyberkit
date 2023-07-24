# Copyright (C) 2010-2018 Apple Inc. All rights reserved.
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
    $(CyberKit2)/PluginProcess \
    $(CyberKit2)/PluginProcess/mac \
    $(CyberKit2)/Shared/Plugins \
    $(CyberKit2)/Shared \
    $(CyberKit2)/Shared/API/Cocoa \
    $(CyberKit2)/Shared/ApplePay \
    $(CyberKit2)/Shared/Authentication \
    $(CyberKit2)/Shared/mac \
    $(CyberKit2)/WebProcess/ApplePay \
    $(CyberKit2)/WebProcess/ApplicationCache \
    $(CyberKit2)/WebProcess/Automation \
    $(CyberKit2)/WebProcess/Cache \
    $(CyberKit2)/WebProcess/Databases/IndexedDB \
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
    $(CyberKit2)/WebProcess/MediaStream \
    $(CyberKit2)/WebProcess/Network \
    $(CyberKit2)/WebProcess/Network/webrtc \
    $(CyberKit2)/WebProcess/Notifications \
    $(CyberKit2)/WebProcess/OriginData \
    $(CyberKit2)/WebProcess/Plugins \
    $(CyberKit2)/WebProcess/ResourceCache \
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
    $(WEBKITADDITIONS_HEADER_SEARCH_PATHS) \
#

PYTHON = python
PERL = perl

ifeq ($(OS),Windows_NT)
    DELETE = cmd //C del
else
    DELETE = rm -f
endif

MESSAGE_RECEIVERS = \
	NetworkProcess/NetworkConnectionToWebProcess \
	NetworkProcess/IndexedDB/WebIDBServer \
	NetworkProcess/NetworkContentRuleListManager \
	NetworkProcess/WebStorage/StorageManagerSet \
	NetworkProcess/cache/CacheStorageEngineConnection \
	NetworkProcess/CustomProtocols/LegacyCustomProtocolManager \
	NetworkProcess/NetworkSocketChannel \
	NetworkProcess/ServiceWorker/WebSWServerConnection \
	NetworkProcess/ServiceWorker/ServiceWorkerFetchTask \
	NetworkProcess/ServiceWorker/WebSWServerToContextConnection \
	NetworkProcess/NetworkSocketStream \
	NetworkProcess/NetworkProcess \
	NetworkProcess/NetworkResourceLoader \
	NetworkProcess/webrtc/NetworkMDNSRegister \
	NetworkProcess/webrtc/NetworkRTCProvider \
	NetworkProcess/webrtc/NetworkRTCMonitor \
	NetworkProcess/Cookies/WebCookieManager \
	Shared/Plugins/NPObjectMessageReceiver \
	Shared/AuxiliaryProcess \
	Shared/API/Cocoa/RemoteObjectRegistry \
	Shared/ApplePay/WebPaymentCoordinatorProxy \
	Shared/Authentication/AuthenticationManager \
	Shared/WebConnection \
	UIProcess/WebFullScreenManagerProxy \
	UIProcess/RemoteLayerTree/RemoteLayerTreeDrawingAreaProxy \
	UIProcess/GPU/GPUProcessProxy \
	UIProcess/WebAuthentication/WebAuthenticatorCoordinatorProxy \
	UIProcess/WebPasteboardProxy \
	UIProcess/UserContent/WebUserContentControllerProxy \
	UIProcess/Inspector/WebInspectorProxy \
	UIProcess/Inspector/RemoteWebInspectorProxy \
	UIProcess/Plugins/PluginProcessProxy \
	UIProcess/DrawingAreaProxy \
	UIProcess/Network/NetworkProcessProxy \
	UIProcess/Network/CustomProtocols/LegacyCustomProtocolManagerProxy \
	UIProcess/WebPageProxy \
	UIProcess/VisitedLinkStore \
	UIProcess/ios/WebDeviceOrientationUpdateProviderProxy \
	UIProcess/ios/EditableImageController \
	UIProcess/ios/SmartMagnificationController \
	UIProcess/mac/SecItemShimProxy \
	UIProcess/WebGeolocationManagerProxy \
	UIProcess/Cocoa/PlaybackSessionManagerProxy \
	UIProcess/Cocoa/UserMediaCaptureManagerProxy \
	UIProcess/Cocoa/VideoFullscreenManagerProxy \
	UIProcess/WebCookieManagerProxy \
	UIProcess/ViewGestureController \
	UIProcess/WebProcessProxy \
	UIProcess/Automation/WebAutomationSession \
	UIProcess/WebProcessPool \
	UIProcess/Downloads/DownloadProxy \
	UIProcess/Media/AudioSessionRoutingArbitratorProxy \
	WebProcess/Databases/IndexedDB/WebIDBConnectionToServer \
	WebProcess/GPU/GPUProcessConnection \
	WebProcess/GPU/graphics/RemoteRenderingBackend \
	WebProcess/GPU/webrtc/LibWebRTCCodecs \
	WebProcess/GPU/webrtc/SampleBufferDisplayLayer \
	WebProcess/GPU/media/MediaPlayerPrivateRemote \
	WebProcess/GPU/media/RemoteCDMInstanceSession \
	WebProcess/GPU/media/RemoteLegacyCDMSession \
	WebProcess/GPU/media/ios/RemoteMediaSessionHelper \
	WebProcess/GPU/media/RemoteMediaPlayerManager \
	WebProcess/GPU/media/RemoteAudioDestinationProxy \
	WebProcess/GPU/media/RemoteAudioSession \
	WebProcess/WebStorage/StorageAreaMap \
	WebProcess/UserContent/WebUserContentController \
	WebProcess/Inspector/WebInspectorInterruptDispatcher \
	WebProcess/Inspector/WebInspectorUI \
	WebProcess/Inspector/WebInspector \
	WebProcess/Inspector/RemoteWebInspectorUI \
	WebProcess/Plugins/PluginProcessConnectionManager \
	WebProcess/Plugins/PluginProxy \
	WebProcess/Plugins/PluginProcessConnection \
	WebProcess/Network/WebSocketChannel \
	WebProcess/Network/NetworkProcessConnection \
	WebProcess/Network/WebSocketStream \
	WebProcess/Network/WebResourceLoader \
	WebProcess/Network/webrtc/WebRTCMonitor \
	WebProcess/Network/webrtc/LibWebRTCNetwork \
	WebProcess/Network/webrtc/WebMDNSRegister \
	WebProcess/Network/webrtc/WebRTCResolver \
	WebProcess/CyberCoreSupport/WebDeviceOrientationUpdateProvider \
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
	WebProcess/WebPage/WebPage \
	WebProcess/WebPage/VisitedLinkTableController \
	WebProcess/WebPage/Cocoa/TextCheckingControllerProxy \
	WebProcess/WebPage/ViewUpdateDispatcher \
	PluginProcess/WebProcessConnection \
	PluginProcess/PluginControllerProxy \
	PluginProcess/PluginProcess \
	GPUProcess/GPUConnectionToWebProcess \
	GPUProcess/graphics/RemoteRenderingBackendProxy \
	GPUProcess/webrtc/LibWebRTCCodecsProxy \
	GPUProcess/webrtc/RemoteSampleBufferDisplayLayerManager \
	GPUProcess/webrtc/RemoteAudioMediaStreamTrackRendererManager \
	GPUProcess/webrtc/RemoteMediaRecorderManager \
	GPUProcess/webrtc/RemoteSampleBufferDisplayLayer \
	GPUProcess/webrtc/RemoteMediaRecorder \
	GPUProcess/webrtc/RemoteAudioMediaStreamTrackRenderer \
	GPUProcess/GPUProcess \
	GPUProcess/media/RemoteLegacyCDMSessionProxy \
	GPUProcess/media/RemoteLegacyCDMFactoryProxy \
	GPUProcess/media/RemoteAudioSessionProxy \
	GPUProcess/media/RemoteCDMInstanceSessionProxy \
	GPUProcess/media/RemoteCDMProxy \
	GPUProcess/media/ios/RemoteMediaSessionHelperProxy \
	GPUProcess/media/RemoteMediaPlayerProxy \
	GPUProcess/media/RemoteCDMFactoryProxy \
	GPUProcess/media/RemoteMediaResourceManager \
	GPUProcess/media/RemoteCDMInstanceProxy \
	GPUProcess/media/RemoteLegacyCDMProxy \
	GPUProcess/media/RemoteMediaPlayerManagerProxy \
	GPUProcess/media/RemoteAudioDestinationManager \
#

GENERATE_MESSAGE_RECEIVER_SCRIPT = $(CyberKit2)/Scripts/generate-message-receiver.py
GENERATE_MESSAGE_RECEIVER_SCRIPTS = \
    $(GENERATE_MESSAGE_RECEIVER_SCRIPT) \
    $(CyberKit2)/Scripts/webkit/__init__.py \
    $(CyberKit2)/Scripts/webkit/messages.py \
    $(CyberKit2)/Scripts/webkit/model.py \
    $(CyberKit2)/Scripts/webkit/parser.py \
#

FRAMEWORK_FLAGS := $(shell echo $(BUILT_PRODUCTS_DIR) $(FRAMEWORK_SEARCH_PATHS) $(SYSTEM_FRAMEWORK_SEARCH_PATHS) | perl -e 'print "-F " . join(" -F ", split(" ", <>));')
HEADER_FLAGS := $(shell echo $(BUILT_PRODUCTS_DIR) $(HEADER_SEARCH_PATHS) $(SYSTEM_HEADER_SEARCH_PATHS) | perl -e 'print "-I" . join(" -I", split(" ", <>));')

MESSAGE_RECEIVER_FILES := $(addsuffix MessageReceiver.cpp,$(notdir $(MESSAGE_RECEIVERS)))
MESSAGES_FILES := $(addsuffix Messages.h,$(notdir $(MESSAGE_RECEIVERS)))
MESSAGE_REPLIES_FILES := $(addsuffix MessagesReplies.h,$(notdir $(MESSAGE_RECEIVERS)))

GENERATED_MESSAGES_FILES := $(MESSAGE_RECEIVER_FILES) $(MESSAGES_FILES) $(MESSAGE_REPLIES_FILES) MessageNames.h MessageNames.cpp
GENERATED_MESSAGES_FILES_AS_PATTERNS := $(subst .,%,$(GENERATED_MESSAGES_FILES))

MESSAGES_IN_FILES := $(addsuffix .messages.in,$(MESSAGE_RECEIVERS))

.PHONY : all

all : $(GENERATED_MESSAGES_FILES)

$(GENERATED_MESSAGES_FILES_AS_PATTERNS) : $(MESSAGES_IN_FILES) $(GENERATE_MESSAGE_RECEIVER_SCRIPTS)
	python $(GENERATE_MESSAGE_RECEIVER_SCRIPT) $(CyberKit2) $(MESSAGE_RECEIVERS)

TEXT_PREPROCESSOR_FLAGS=-E -P -w

ifneq ($(SDKROOT),)
	SDK_FLAGS=-isysroot $(SDKROOT)
endif

ifeq ($(USE_LLVM_TARGET_TRIPLES_FOR_CLANG),YES)
	WK_CURRENT_ARCH=$(word 1, $(ARCHS))
	TARGET_TRIPLE_FLAGS=-target $(WK_CURRENT_ARCH)-$(LLVM_TARGET_TRIPLE_VENDOR)-$(LLVM_TARGET_TRIPLE_OS_VERSION)$(LLVM_TARGET_TRIPLE_SUFFIX)
endif

SANDBOX_PROFILES = \
	com.apple.WebProcess.sb \
	com.matthewbenedict.CyberKit.plugin-common.sb \
	com.matthewbenedict.CyberKit.NetworkProcess.sb \
	com.matthewbenedict.CyberKit.GPUProcess.sb

all : $(SANDBOX_PROFILES)

%.sb : %.sb.in
	@echo Pre-processing $* sandbox profile...
	grep -o '^[^;]*' $< | $(CC) $(SDK_FLAGS) $(TARGET_TRIPLE_FLAGS) $(TEXT_PREPROCESSOR_FLAGS) $(FRAMEWORK_FLAGS) $(HEADER_FLAGS) -include "wtf/Platform.h" - > $@

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

WEB_PREFERENCES_INPUT_FILES = \
    $(CyberKit2)/Shared/WebPreferences.yaml \
    $(ADDITIONAL_WEB_PREFERENCES_INPUT_FILES) \
#
WEB_PREFERENCES_COMBINED_INPUT_FILE = WebPreferencesCombined.yaml

WEB_PREFERENCES_TEMPLATES = \
    $(CyberKit2)/Scripts/PreferencesTemplates/WebPageUpdatePreferences.cpp.erb \
    $(CyberKit2)/Scripts/PreferencesTemplates/WebPreferencesDefinitions.h.erb \
    $(CyberKit2)/Scripts/PreferencesTemplates/WebPreferencesExperimentalFeatures.cpp.erb \
    $(CyberKit2)/Scripts/PreferencesTemplates/WebPreferencesInternalDebugFeatures.cpp.erb \
    $(CyberKit2)/Scripts/PreferencesTemplates/WebPreferencesKeys.h.erb \
    $(CyberKit2)/Scripts/PreferencesTemplates/WebPreferencesKeys.cpp.erb \
    $(CyberKit2)/Scripts/PreferencesTemplates/WebPreferencesStoreDefaultsMap.cpp.erb \
#
WEB_PREFERENCES_FILES = $(basename $(notdir $(WEB_PREFERENCES_TEMPLATES)))
WEB_PREFERENCES_PATTERNS = $(subst .,%,$(WEB_PREFERENCES_FILES))

all : $(WEB_PREFERENCES_FILES) $(WEB_PREFERENCES_COMBINED_INPUT_FILE)

$(WEB_PREFERENCES_COMBINED_INPUT_FILE) : $(WEB_PREFERENCES_INPUT_FILES)
	cat $^ > $(WEB_PREFERENCES_COMBINED_INPUT_FILE)

$(WEB_PREFERENCES_PATTERNS) : $(CyberKit2)/Scripts/GeneratePreferences.rb $(WEB_PREFERENCES_TEMPLATES) $(WEB_PREFERENCES_COMBINED_INPUT_FILE)
	$(RUBY) $< --input $(WEB_PREFERENCES_COMBINED_INPUT_FILE)

# FIXME: We should switch to the internal HTTPSUpgradeList.txt once the feature is ready.
# VPATH += $(CyberKit2)/Shared/HTTPSUpgrade/
VPATH := $(CyberKit2)/Shared/HTTPSUpgrade/ $(VPATH)

all : HTTPSUpgradeList.db
HTTPSUpgradeList.db : HTTPSUpgradeList.txt $(CyberKit2)/Scripts/generate-https-upgrade-database.sh
	sh $(CyberKit2)/Scripts/generate-https-upgrade-database.sh $< $@
