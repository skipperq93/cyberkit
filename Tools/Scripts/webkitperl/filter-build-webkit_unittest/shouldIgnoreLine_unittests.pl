#!/usr/bin/env perl
#
# Copyright (C) 2013 Apple Inc. All rights reserved.
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
# THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# This script tests parts of filter-build-webkit.
# This script runs the unittests specified in @testFiles.

use strict;
use warnings;

use English;
use FindBin;
use Test::More;
use lib File::Spec->catdir($FindBin::Bin, "..");
require "filter-build-webkit";

FilterBuildCyberKit->import(qw(shouldIgnoreLine));

sub description($);

#
# Test whitespace
#
is(shouldIgnoreLine("", ""), 1, "Ignored: empty line");
is(shouldIgnoreLine("", " "), 1, "Ignored: one space");
is(shouldIgnoreLine("", "\t"), 1, "Ignored: one tab");

#
# Test input that should be ignored regardless of previous line
#
my @expectIgnoredLines = split(/$INPUT_RECORD_SEPARATOR/, <<'END');
make: Nothing to be done for `all'.
CyberScriptCore/create_hash_table CyberScriptCore/runtime/ArrayConstructor.cpp -i > ArrayConstructor.lut.h
Creating hashtable for CyberScriptCore/runtime/ArrayConstructor.cpp
Wrote output to /Volumes/Data/Build/Release/DerivedSources/CyberCore/ExportFileGenerator.cpp
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/libtool: file: /Volumes/Data/Build/ANGLE.build/Release/ANGLE.build/Objects-normal/i386/debug.o has no symbols
Showing first 200 notices only
printf "CyberCore/Modules/encryptedmedia/MediaKeyMessageEvent.idl\nCyberCore/Modules/encryptedmedia/MediaKeyNeededEvent.idl\nCyberCore/Modules/encryptedmedia/MediaKeySession.idl\nCyberCore/Modules/encryptedmedia/MediaKeys.idl\nCyberCore/Modules/filesystem/DOMFileSystem.idl\nCyberCore/Modules/filesystem/DOMFileSystemSync.idl\nCyberCore/Modules/filesystem/DOMWindowFileSystem.idl\nCyberCore/Modules/filesystem/DirectoryEntry.idl\nCyberCore/Modules/filesystem/DirectoryEntrySync.idl\nCyberCore/Modules/filesystem/DirectoryReader.idl\nCyberCore/Modules/filesystem/DirectoryReaderSync.idl\nCyberCore/Modules/filesystem/EntriesCallback.idl\nCyberCore/Modules/filesystem/Entry.idl\nCyberCore/Modules/filesystem/EntryArray.idl\nCyberCore/Modules/filesystem/EntryArraySync.idl\nCyberCore/Modules/filesystem/EntryCallback.idl\nCyberCore/Modules/filesystem/EntrySync.idl\nCyberCore/Modules/filesystem/ErrorCallback.idl\nCyberCore/Modules/filesystem/FileCallback.idl\nCyberCore/Modules/filesystem/FileEntry.idl\nCyberCore/Modules/filesystem/FileEntrySync.idl\nCyberCore/Modules/filesystem/FileSystemCallback.idl\nCyberCore/Modules/filesystem/FileWriter.idl\nCyberCore/Modules/filesystem/FileWriterCallback.idl\nCyberCore/Modules/filesystem/FileWriterSync.idl\nCyberCore/Modules/filesystem/Metadata.idl\nCyberCore/Modules/filesystem/MetadataCallback.idl\nCyberCore/Modules/filesystem/WorkerContextFileSystem.idl\nCyberCore/Modules/geolocation/Coordinates.idl\nCyberCore/Modules/geolocation/Geolocation.idl\nCyberCore/Modules/geolocation/Geoposition.idl\nCyberCore/Modules/geolocation/NavigatorGeolocation.idl\nCyberCore/Modules/geolocation/PositionCallback.idl\nCyberCore/Modules/geolocation/PositionError.idl\nCyberCore/Modules/geolocation/PositionErrorCallback.idl\nCyberCore/Modules/indexeddb/DOMWindowIndexedDatabase.idl\nCyberCore/Modules/indexeddb/IDBAny.idl\nCyberCore/Modules/indexeddb/IDBCursor.idl\nCyberCore/Modules/indexeddb/IDBDatabase.idl\nCyberCore/Modules/indexeddb/IDBFactory.idl\nCyberCore/Modules/indexeddb/IDBIndex.idl\nCyberCore/Modules/indexeddb/IDBKeyRange.idl\nCyberCore/Modules/indexeddb/IDBObjectStore.idl\nCyberCore/Modules/indexeddb/IDBRequest.idl\nCyberCore/Modules/indexeddb/IDBTransaction.idl\nCyberCore/Modules/indexeddb/IDBVersionChangeEvent.idl\nCyberCore/Modules/indexeddb/WorkerContextIndexedDatabase.idl\nCyberCore/Modules/mediasource/MediaSource.idl\nCyberCore/Modules/mediasource/SourceBuffer.idl\nCyberCore/Modules/mediasource/SourceBufferList.idl\nCyberCore/Modules/notifications/DOMWindowNotifications.idl\nCyberCore/Modules/notifications/Notification.idl\nCyberCore/Modules/notifications/NotificationCenter.idl\nCyberCore/Modules/notifications/NotificationPermissionCallback.idl\nCyberCore/Modules/notifications/WorkerContextNotifications.idl\nCyberCore/Modules/quota/DOMWindowQuota.idl\nCyberCore/Modules/quota/NavigatorStorageQuota.idl\nCyberCore/Modules/quota/StorageInfo.idl\nCyberCore/Modules/quota/StorageErrorCallback.idl\nCyberCore/Modules/quota/StorageQuota.idl\nCyberCore/Modules/quota/StorageQuotaCallback.idl\nCyberCore/Modules/quota/StorageUsageCallback.idl\nCyberCore/Modules/quota/WorkerNavigatorStorageQuota.idl\nCyberCore/Modules/speech/DOMWindowSpeechSynthesis.idl\nCyberCore/Modules/speech/SpeechSynthesis.idl\nCyberCore/Modules/speech/SpeechSynthesisEvent.idl\nCyberCore/Modules/speech/SpeechSynthesisUtterance.idl\nCyberCore/Modules/speech/SpeechSynthesisVoice.idl\nCyberCore/Modules/webaudio/AudioBuffer.idl\nCyberCore/Modules/webaudio/AudioBufferCallback.idl\nCyberCore/Modules/webaudio/AudioBufferSourceNode.idl\nCyberCore/Modules/webaudio/ChannelMergerNode.idl\nCyberCore/Modules/webaudio/ChannelSplitterNode.idl\nCyberCore/Modules/webaudio/AudioContext.idl\nCyberCore/Modules/webaudio/AudioDestinationNode.idl\nCyberCore/Modules/webaudio/GainNode.idl\nCyberCore/Modules/webaudio/AudioListener.idl\nCyberCore/Modules/webaudio/AudioNode.idl\nCyberCore/Modules/webaudio/PannerNode.idl\nCyberCore/Modules/webaudio/AudioParam.idl\nCyberCore/Modules/webaudio/AudioProcessingEvent.idl\nCyberCore/Modules/webaudio/BiquadFilterNode.idl\nCyberCore/Modules/webaudio/ConvolverNode.idl\nCyberCore/Modules/webaudio/DOMWindowWebAudio.idl\nCyberCore/Modules/webaudio/DelayNode.idl\nCyberCore/Modules/webaudio/DynamicsCompressorNode.idl\nCyberCore/Modules/webaudio/ScriptProcessorNode.idl\nCyberCore/Modules/webaudio/MediaElementAudioSourceNode.idl\nCyberCore/Modules/webaudio/MediaStreamAudioSourceNode.idl\nCyberCore/Modules/webaudio/OscillatorNode.idl\nCyberCore/Modules/webaudio/OfflineAudioContext.idl\nCyberCore/Modules/webaudio/OfflineAudioCompletionEvent.idl\nCyberCore/Modules/webaudio/AnalyserNode.idl\nCyberCore/Modules/webaudio/WaveShaperNode.idl\nCyberCore/Modules/webaudio/WaveTable.idl\nCyberCore/Modules/webdatabase/DOMWindowWebDatabase.idl\nCyberCore/Modules/webdatabase/Database.idl\nCyberCore/Modules/webdatabase/DatabaseCallback.idl\nCyberCore/Modules/webdatabase/DatabaseSync.idl\nCyberCore/Modules/webdatabase/SQLError.idl\nCyberCore/Modules/webdatabase/SQLException.idl\nCyberCore/Modules/webdatabase/SQLResultSet.idl\nCyberCore/Modules/webdatabase/SQLResultSetRowList.idl\nCyberCore/Modules/webdatabase/SQLStatementCallback.idl\nCyberCore/Modules/webdatabase/SQLStatementErrorCallback.idl\nCyberCore/Modules/webdatabase/SQLTransaction.idl\nCyberCore/Modules/webdatabase/SQLTransactionCallback.idl\nCyberCore/Modules/webdatabase/SQLTransactionErrorCallback.idl\nCyberCore/Modules/webdatabase/SQLTransactionSync.idl\nCyberCore/Modules/webdatabase/SQLTransactionSyncCallback.idl\nCyberCore/Modules/webdatabase/WorkerContextWebDatabase.idl\nCyberCore/Modules/websockets/CloseEvent.idl\nCyberCore/Modules/websockets/DOMWindowWebSocket.idl\nCyberCore/Modules/websockets/WebSocket.idl\nCyberCore/Modules/websockets/WorkerContextWebSocket.idl\nCyberCore/css/CSSCharsetRule.idl\nCyberCore/css/CSSFontFaceLoadEvent.idl\nCyberCore/css/CSSFontFaceRule.idl\nCyberCore/css/CSSHostRule.idl\nCyberCore/css/CSSImportRule.idl\nCyberCore/css/CSSMediaRule.idl\nCyberCore/css/CSSPageRule.idl\nCyberCore/css/CSSPrimitiveValue.idl\nCyberCore/css/CSSRule.idl\nCyberCore/css/CSSRuleList.idl\nCyberCore/css/CSSStyleDeclaration.idl\nCyberCore/css/CSSStyleRule.idl\nCyberCore/css/CSSStyleSheet.idl\nCyberCore/css/CSSSupportsRule.idl\nCyberCore/css/CSSUnknownRule.idl\nCyberCore/css/CSSValue.idl\nCyberCore/css/CSSValueList.idl\nCyberCore/css/Counter.idl\nCyberCore/css/DOMWindowCSS.idl\nCyberCore/css/FontLoader.idl\nCyberCore/css/MediaList.idl\nCyberCore/css/MediaQueryList.idl\nCyberCore/css/MediaQueryListListener.idl\nCyberCore/css/RGBColor.idl\nCyberCore/css/Rect.idl\nCyberCore/css/StyleMedia.idl\nCyberCore/css/StyleSheet.idl\nCyberCore/css/StyleSheetList.idl\nCyberCore/css/CyberKitCSSFilterValue.idl\nCyberCore/css/CyberKitCSSFilterRule.idl\nCyberCore/css/CyberKitCSSKeyframeRule.idl\nCyberCore/css/CyberKitCSSKeyframesRule.idl\nCyberCore/css/CyberKitCSSMatrix.idl\nCyberCore/css/CyberKitCSSMixFunctionValue.idl\nCyberCore/css/CyberKitCSSRegionRule.idl\nCyberCore/css/CyberKitCSSTransformValue.idl\nCyberCore/css/CyberKitCSSViewportRule.idl\nCyberCore/dom/Attr.idl\nCyberCore/dom/BeforeLoadEvent.idl\nCyberCore/dom/CDATASection.idl\nCyberCore/dom/CharacterData.idl\nCyberCore/dom/ClientRect.idl\nCyberCore/dom/ClientRectList.idl\nCyberCore/dom/Clipboard.idl\nCyberCore/dom/Comment.idl\nCyberCore/dom/CompositionEvent.idl\nCyberCore/dom/CustomElementConstructor.idl\nCyberCore/dom/CustomEvent.idl\nCyberCore/dom/DOMCoreException.idl\nCyberCore/dom/DOMError.idl\nCyberCore/dom/DOMImplementation.idl\nCyberCore/dom/DOMStringList.idl\nCyberCore/dom/DOMStringMap.idl\nCyberCore/dom/DataTransferItem.idl\nCyberCore/dom/DataTransferItemList.idl\nCyberCore/dom/DeviceMotionEvent.idl\nCyberCore/dom/DeviceOrientationEvent.idl\nCyberCore/dom/Document.idl\nCyberCore/dom/DocumentFragment.idl\nCyberCore/dom/DocumentType.idl\nCyberCore/dom/Element.idl\nCyberCore/dom/Entity.idl\nCyberCore/dom/EntityReference.idl\nCyberCore/dom/ErrorEvent.idl\nCyberCore/dom/Event.idl\nCyberCore/dom/EventException.idl\nCyberCore/dom/EventListener.idl\nCyberCore/dom/EventTarget.idl\nCyberCore/dom/FocusEvent.idl\nCyberCore/dom/HashChangeEvent.idl\nCyberCore/dom/KeyboardEvent.idl\nCyberCore/dom/MessageChannel.idl\nCyberCore/dom/MessageEvent.idl\nCyberCore/dom/MessagePort.idl\nCyberCore/dom/MouseEvent.idl\nCyberCore/dom/MutationEvent.idl\nCyberCore/dom/MutationObserver.idl\nCyberCore/dom/MutationRecord.idl\nCyberCore/dom/DOMNamedFlowCollection.idl\nCyberCore/dom/NamedNodeMap.idl\nCyberCore/dom/Node.idl\nCyberCore/dom/NodeFilter.idl\nCyberCore/dom/NodeIterator.idl\nCyberCore/dom/NodeList.idl\nCyberCore/dom/Notation.idl\nCyberCore/dom/OverflowEvent.idl\nCyberCore/dom/PageTransitionEvent.idl\nCyberCore/dom/PopStateEvent.idl\nCyberCore/dom/ProcessingInstruction.idl\nCyberCore/dom/ProgressEvent.idl\nCyberCore/dom/ProgressEvent.idl\nCyberCore/dom/PropertyNodeList.idl\nCyberCore/dom/Range.idl\nCyberCore/dom/RangeException.idl\nCyberCore/dom/RequestAnimationFrameCallback.idl\nCyberCore/dom/ShadowRoot.idl\nCyberCore/dom/StringCallback.idl\nCyberCore/dom/Text.idl\nCyberCore/dom/TextEvent.idl\nCyberCore/dom/Touch.idl\nCyberCore/dom/TouchEvent.idl\nCyberCore/dom/TouchList.idl\nCyberCore/dom/TransitionEvent.idl\nCyberCore/dom/TreeWalker.idl\nCyberCore/dom/UIEvent.idl\nCyberCore/dom/CyberKitAnimationEvent.idl\nCyberCore/dom/CyberKitNamedFlow.idl\nCyberCore/dom/CyberKitTransitionEvent.idl\nCyberCore/dom/WheelEvent.idl\nCyberCore/fileapi/Blob.idl\nCyberCore/fileapi/File.idl\nCyberCore/fileapi/FileError.idl\nCyberCore/fileapi/FileException.idl\nCyberCore/fileapi/FileList.idl\nCyberCore/fileapi/FileReader.idl\nCyberCore/fileapi/FileReaderSync.idl\nCyberCore/html/DOMFormData.idl\nCyberCore/html/DOMSettableTokenList.idl\nCyberCore/html/DOMTokenList.idl\nCyberCore/html/DOMURL.idl\nCyberCore/html/HTMLAllCollection.idl\nCyberCore/html/HTMLAnchorElement.idl\nCyberCore/html/HTMLAppletElement.idl\nCyberCore/html/HTMLAreaElement.idl\nCyberCore/html/HTMLAudioElement.idl\nCyberCore/html/HTMLBRElement.idl\nCyberCore/html/HTMLBaseElement.idl\nCyberCore/html/HTMLBaseFontElement.idl\nCyberCore/html/HTMLBodyElement.idl\nCyberCore/html/HTMLButtonElement.idl\nCyberCore/html/HTMLCanvasElement.idl\nCyberCore/html/HTMLCollection.idl\nCyberCore/html/HTMLDListElement.idl\nCyberCore/html/HTMLDataListElement.idl\nCyberCore/html/HTMLDetailsElement.idl\nCyberCore/html/HTMLDialogElement.idl\nCyberCore/html/HTMLDirectoryElement.idl\nCyberCore/html/HTMLDivElement.idl\nCyberCore/html/HTMLDocument.idl\nCyberCore/html/HTMLElement.idl\nCyberCore/html/HTMLEmbedElement.idl\nCyberCore/html/HTMLFieldSetElement.idl\nCyberCore/html/HTMLFontElement.idl\nCyberCore/html/HTMLFormControlsCollection.idl\nCyberCore/html/HTMLFormElement.idl\nCyberCore/html/HTMLFrameElement.idl\nCyberCore/html/HTMLFrameSetElement.idl\nCyberCore/html/HTMLHRElement.idl\nCyberCore/html/HTMLHeadElement.idl\nCyberCore/html/HTMLHeadingElement.idl\nCyberCore/html/HTMLHtmlElement.idl\nCyberCore/html/HTMLIFrameElement.idl\nCyberCore/html/HTMLImageElement.idl\nCyberCore/html/HTMLInputElement.idl\nCyberCore/html/HTMLKeygenElement.idl\nCyberCore/html/HTMLLIElement.idl\nCyberCore/html/HTMLLabelElement.idl\nCyberCore/html/HTMLLegendElement.idl\nCyberCore/html/HTMLLinkElement.idl\nCyberCore/html/HTMLMapElement.idl\nCyberCore/html/HTMLMarqueeElement.idl\nCyberCore/html/HTMLMediaElement.idl\nCyberCore/html/HTMLMenuElement.idl\nCyberCore/html/HTMLMetaElement.idl\nCyberCore/html/HTMLMeterElement.idl\nCyberCore/html/HTMLModElement.idl\nCyberCore/html/HTMLOListElement.idl\nCyberCore/html/HTMLObjectElement.idl\nCyberCore/html/HTMLOptGroupElement.idl\nCyberCore/html/HTMLOptionElement.idl\nCyberCore/html/HTMLOptionsCollection.idl\nCyberCore/html/HTMLOutputElement.idl\nCyberCore/html/HTMLParagraphElement.idl\nCyberCore/html/HTMLParamElement.idl\nCyberCore/html/HTMLPreElement.idl\nCyberCore/html/HTMLProgressElement.idl\nCyberCore/html/HTMLPropertiesCollection.idl\nCyberCore/html/HTMLQuoteElement.idl\nCyberCore/html/HTMLScriptElement.idl\nCyberCore/html/HTMLSelectElement.idl\nCyberCore/html/HTMLSourceElement.idl\nCyberCore/html/HTMLSpanElement.idl\nCyberCore/html/HTMLStyleElement.idl\nCyberCore/html/HTMLTableCaptionElement.idl\nCyberCore/html/HTMLTableCellElement.idl\nCyberCore/html/HTMLTableColElement.idl\nCyberCore/html/HTMLTableElement.idl\nCyberCore/html/HTMLTableRowElement.idl\nCyberCore/html/HTMLTableSectionElement.idl\nCyberCore/html/HTMLTemplateElement.idl\nCyberCore/html/HTMLTextAreaElement.idl\nCyberCore/html/HTMLTitleElement.idl\nCyberCore/html/HTMLTrackElement.idl\nCyberCore/html/HTMLUListElement.idl\nCyberCore/html/HTMLUnknownElement.idl\nCyberCore/html/HTMLVideoElement.idl\nCyberCore/html/ImageData.idl\nCyberCore/html/MediaController.idl\nCyberCore/html/MediaError.idl\nCyberCore/html/MediaKeyError.idl\nCyberCore/html/MediaKeyEvent.idl\nCyberCore/html/MicroDataItemValue.idl\nCyberCore/html/RadioNodeList.idl\nCyberCore/html/TextMetrics.idl\nCyberCore/html/TimeRanges.idl\nCyberCore/html/ValidityState.idl\nCyberCore/html/VoidCallback.idl\nCyberCore/html/canvas/ArrayBuffer.idl\nCyberCore/html/canvas/ArrayBufferView.idl\nCyberCore/html/canvas/CanvasGradient.idl\nCyberCore/html/canvas/CanvasPattern.idl\nCyberCore/html/canvas/CanvasProxy.idl\nCyberCore/html/canvas/CanvasRenderingContext.idl\nCyberCore/html/canvas/CanvasRenderingContext2D.idl\nCyberCore/html/canvas/DataView.idl\nCyberCore/html/canvas/DOMPath.idl\nCyberCore/html/canvas/EXTDrawBuffers.idl\nCyberCore/html/canvas/EXTTextureFilterAnisotropic.idl\nCyberCore/html/canvas/Float32Array.idl\nCyberCore/html/canvas/Float64Array.idl\nCyberCore/html/canvas/Int16Array.idl\nCyberCore/html/canvas/Int32Array.idl\nCyberCore/html/canvas/Int8Array.idl\nCyberCore/html/canvas/OESElementIndexUint.idl\nCyberCore/html/canvas/OESStandardDerivatives.idl\nCyberCore/html/canvas/OESTextureFloat.idl\nCyberCore/html/canvas/OESTextureHalfFloat.idl\nCyberCore/html/canvas/OESVertexArrayObject.idl\nCyberCore/html/canvas/Uint16Array.idl\nCyberCore/html/canvas/Uint32Array.idl\nCyberCore/html/canvas/Uint8Array.idl\nCyberCore/html/canvas/Uint8ClampedArray.idl\nCyberCore/html/canvas/WebGLActiveInfo.idl\nCyberCore/html/canvas/WebGLBuffer.idl\nCyberCore/html/canvas/WebGLCompressedTextureATC.idl\nCyberCore/html/canvas/WebGLCompressedTexturePVRTC.idl\nCyberCore/html/canvas/WebGLCompressedTextureS3TC.idl\nCyberCore/html/canvas/WebGLContextAttributes.idl\nCyberCore/html/canvas/WebGLContextEvent.idl\nCyberCore/html/canvas/WebGLDepthTexture.idl\nCyberCore/html/canvas/WebGLFramebuffer.idl\nCyberCore/html/canvas/WebGLLoseContext.idl\nCyberCore/html/canvas/WebGLProgram.idl\nCyberCore/html/canvas/WebGLRenderbuffer.idl\nCyberCore/html/canvas/WebGLRenderingContext.idl\nCyberCore/html/canvas/WebGLShader.idl\nCyberCore/html/canvas/WebGLShaderPrecisionFormat.idl\nCyberCore/html/canvas/WebGLTexture.idl\nCyberCore/html/canvas/WebGLUniformLocation.idl\nCyberCore/html/canvas/WebGLVertexArrayObjectOES.idl\nCyberCore/html/shadow/HTMLContentElement.idl\nCyberCore/html/shadow/HTMLShadowElement.idl\nCyberCore/html/track/TextTrack.idl\nCyberCore/html/track/TextTrackCue.idl\nCyberCore/html/track/TextTrackCueList.idl\nCyberCore/html/track/TextTrackList.idl\nCyberCore/html/track/TrackEvent.idl\nCyberCore/inspector/InjectedScriptHost.idl\nCyberCore/inspector/InspectorFrontendHost.idl\nCyberCore/inspector/ScriptProfile.idl\nCyberCore/inspector/ScriptProfileNode.idl\nCyberCore/loader/appcache/DOMApplicationCache.idl\nCyberCore/page/AbstractView.idl\nCyberCore/page/BarInfo.idl\nCyberCore/page/Console.idl\nCyberCore/page/Crypto.idl\nCyberCore/page/DOMSecurityPolicy.idl\nCyberCore/page/DOMSelection.idl\nCyberCore/page/DOMWindow.idl\nCyberCore/page/EventSource.idl\nCyberCore/page/History.idl\nCyberCore/page/Location.idl\nCyberCore/page/Navigator.idl\nCyberCore/page/Performance.idl\nCyberCore/page/PerformanceNavigation.idl\nCyberCore/page/PerformanceTiming.idl\nCyberCore/page/Screen.idl\nCyberCore/page/SpeechInputEvent.idl\nCyberCore/page/SpeechInputResult.idl\nCyberCore/page/SpeechInputResultList.idl\nCyberCore/page/CyberKitPoint.idl\nCyberCore/page/WorkerNavigator.idl\nCyberCore/plugins/DOMMimeType.idl\nCyberCore/plugins/DOMMimeTypeArray.idl\nCyberCore/plugins/DOMPlugin.idl\nCyberCore/plugins/DOMPluginArray.idl\nCyberCore/storage/Storage.idl\nCyberCore/storage/StorageEvent.idl\nCyberCore/svg/ElementTimeControl.idl\nCyberCore/svg/SVGAElement.idl\nCyberCore/svg/SVGAltGlyphDefElement.idl\nCyberCore/svg/SVGAltGlyphElement.idl\nCyberCore/svg/SVGAltGlyphItemElement.idl\nCyberCore/svg/SVGAngle.idl\nCyberCore/svg/SVGAnimateColorElement.idl\nCyberCore/svg/SVGAnimateElement.idl\nCyberCore/svg/SVGAnimateMotionElement.idl\nCyberCore/svg/SVGAnimateTransformElement.idl\nCyberCore/svg/SVGAnimatedAngle.idl\nCyberCore/svg/SVGAnimatedBoolean.idl\nCyberCore/svg/SVGAnimatedEnumeration.idl\nCyberCore/svg/SVGAnimatedInteger.idl\nCyberCore/svg/SVGAnimatedLength.idl\nCyberCore/svg/SVGAnimatedLengthList.idl\nCyberCore/svg/SVGAnimatedNumber.idl\nCyberCore/svg/SVGAnimatedNumberList.idl\nCyberCore/svg/SVGAnimatedPreserveAspectRatio.idl\nCyberCore/svg/SVGAnimatedRect.idl\nCyberCore/svg/SVGAnimatedString.idl\nCyberCore/svg/SVGAnimatedTransformList.idl\nCyberCore/svg/SVGAnimationElement.idl\nCyberCore/svg/SVGCircleElement.idl\nCyberCore/svg/SVGClipPathElement.idl\nCyberCore/svg/SVGColor.idl\nCyberCore/svg/SVGComponentTransferFunctionElement.idl\nCyberCore/svg/SVGCursorElement.idl\nCyberCore/svg/SVGDefsElement.idl\nCyberCore/svg/SVGDescElement.idl\nCyberCore/svg/SVGDocument.idl\nCyberCore/svg/SVGElement.idl\nCyberCore/svg/SVGElementInstance.idl\nCyberCore/svg/SVGEllipseElement.idl\nCyberCore/svg/SVGException.idl\nCyberCore/svg/SVGExternalResourcesRequired.idl\nCyberCore/svg/SVGFEBlendElement.idl\nCyberCore/svg/SVGFEColorMatrixElement.idl\nCyberCore/svg/SVGFEComponentTransferElement.idl\nCyberCore/svg/SVGFECompositeElement.idl\nCyberCore/svg/SVGFEConvolveMatrixElement.idl\nCyberCore/svg/SVGFEDiffuseLightingElement.idl\nCyberCore/svg/SVGFEDisplacementMapElement.idl\nCyberCore/svg/SVGFEDistantLightElement.idl\nCyberCore/svg/SVGFEDropShadowElement.idl\nCyberCore/svg/SVGFEFloodElement.idl\nCyberCore/svg/SVGFEFuncAElement.idl\nCyberCore/svg/SVGFEFuncBElement.idl\nCyberCore/svg/SVGFEFuncGElement.idl\nCyberCore/svg/SVGFEFuncRElement.idl\nCyberCore/svg/SVGFEGaussianBlurElement.idl\nCyberCore/svg/SVGFEImageElement.idl\nCyberCore/svg/SVGFEMergeElement.idl\nCyberCore/svg/SVGFEMergeNodeElement.idl\nCyberCore/svg/SVGFEMorphologyElement.idl\nCyberCore/svg/SVGFEOffsetElement.idl\nCyberCore/svg/SVGFEPointLightElement.idl\nCyberCore/svg/SVGFESpecularLightingElement.idl\nCyberCore/svg/SVGFESpotLightElement.idl\nCyberCore/svg/SVGFETileElement.idl\nCyberCore/svg/SVGFETurbulenceElement.idl\nCyberCore/svg/SVGFilterElement.idl\nCyberCore/svg/SVGFilterPrimitiveStandardAttributes.idl\nCyberCore/svg/SVGFitToViewBox.idl\nCyberCore/svg/SVGFontElement.idl\nCyberCore/svg/SVGFontFaceElement.idl\nCyberCore/svg/SVGFontFaceFormatElement.idl\nCyberCore/svg/SVGFontFaceNameElement.idl\nCyberCore/svg/SVGFontFaceSrcElement.idl\nCyberCore/svg/SVGFontFaceUriElement.idl\nCyberCore/svg/SVGForeignObjectElement.idl\nCyberCore/svg/SVGGElement.idl\nCyberCore/svg/SVGGlyphElement.idl\nCyberCore/svg/SVGGlyphRefElement.idl\nCyberCore/svg/SVGGradientElement.idl\nCyberCore/svg/SVGHKernElement.idl\nCyberCore/svg/SVGImageElement.idl\nCyberCore/svg/SVGLangSpace.idl\nCyberCore/svg/SVGLength.idl\nCyberCore/svg/SVGLengthList.idl\nCyberCore/svg/SVGLineElement.idl\nCyberCore/svg/SVGLinearGradientElement.idl\nCyberCore/svg/SVGLocatable.idl\nCyberCore/svg/SVGMPathElement.idl\nCyberCore/svg/SVGMarkerElement.idl\nCyberCore/svg/SVGMaskElement.idl\nCyberCore/svg/SVGMatrix.idl\nCyberCore/svg/SVGMetadataElement.idl\nCyberCore/svg/SVGMissingGlyphElement.idl\nCyberCore/svg/SVGNumber.idl\nCyberCore/svg/SVGNumberList.idl\nCyberCore/svg/SVGPaint.idl\nCyberCore/svg/SVGPathElement.idl\nCyberCore/svg/SVGPathSeg.idl\nCyberCore/svg/SVGPathSegArcAbs.idl\nCyberCore/svg/SVGPathSegArcRel.idl\nCyberCore/svg/SVGPathSegClosePath.idl\nCyberCore/svg/SVGPathSegCurvetoCubicAbs.idl\nCyberCore/svg/SVGPathSegCurvetoCubicRel.idl\nCyberCore/svg/SVGPathSegCurvetoCubicSmoothAbs.idl\nCyberCore/svg/SVGPathSegCurvetoCubicSmoothRel.idl\nCyberCore/svg/SVGPathSegCurvetoQuadraticAbs.idl\nCyberCore/svg/SVGPathSegCurvetoQuadraticRel.idl\nCyberCore/svg/SVGPathSegCurvetoQuadraticSmoothAbs.idl\nCyberCore/svg/SVGPathSegCurvetoQuadraticSmoothRel.idl\nCyberCore/svg/SVGPathSegLinetoAbs.idl\nCyberCore/svg/SVGPathSegLinetoHorizontalAbs.idl\nCyberCore/svg/SVGPathSegLinetoHorizontalRel.idl\nCyberCore/svg/SVGPathSegLinetoRel.idl\nCyberCore/svg/SVGPathSegLinetoVerticalAbs.idl\nCyberCore/svg/SVGPathSegLinetoVerticalRel.idl\nCyberCore/svg/SVGPathSegList.idl\nCyberCore/svg/SVGPathSegMovetoAbs.idl\nCyberCore/svg/SVGPathSegMovetoRel.idl\nCyberCore/svg/SVGPatternElement.idl\nCyberCore/svg/SVGPoint.idl\nCyberCore/svg/SVGPointList.idl\nCyberCore/svg/SVGPolygonElement.idl\nCyberCore/svg/SVGPolylineElement.idl\nCyberCore/svg/SVGPreserveAspectRatio.idl\nCyberCore/svg/SVGRadialGradientElement.idl\nCyberCore/svg/SVGRect.idl\nCyberCore/svg/SVGRectElement.idl\nCyberCore/svg/SVGRenderingIntent.idl\nCyberCore/svg/SVGSVGElement.idl\nCyberCore/svg/SVGScriptElement.idl\nCyberCore/svg/SVGSetElement.idl\nCyberCore/svg/SVGStopElement.idl\nCyberCore/svg/SVGStringList.idl\nCyberCore/svg/SVGStyleElement.idl\nCyberCore/svg/SVGStyledElement.idl\nCyberCore/svg/SVGSwitchElement.idl\nCyberCore/svg/SVGSymbolElement.idl\nCyberCore/svg/SVGTRefElement.idl\nCyberCore/svg/SVGTSpanElement.idl\nCyberCore/svg/SVGTests.idl\nCyberCore/svg/SVGTextContentElement.idl\nCyberCore/svg/SVGTextElement.idl\nCyberCore/svg/SVGTextPathElement.idl\nCyberCore/svg/SVGTextPositioningElement.idl\nCyberCore/svg/SVGTitleElement.idl\nCyberCore/svg/SVGTransform.idl\nCyberCore/svg/SVGTransformList.idl\nCyberCore/svg/SVGTransformable.idl\nCyberCore/svg/SVGURIReference.idl\nCyberCore/svg/SVGUnitTypes.idl\nCyberCore/svg/SVGUseElement.idl\nCyberCore/svg/SVGVKernElement.idl\nCyberCore/svg/SVGViewElement.idl\nCyberCore/svg/SVGViewSpec.idl\nCyberCore/svg/SVGZoomAndPan.idl\nCyberCore/svg/SVGZoomEvent.idl\nCyberCore/testing/Internals.idl\nCyberCore/testing/InternalSettings.idl\nCyberCore/testing/MallocStatistics.idl\nCyberCore/testing/MemoryInfo.idl\nCyberCore/testing/TypeConversions.idl\nCyberCore/workers/AbstractWorker.idl\nCyberCore/workers/DedicatedWorkerContext.idl\nCyberCore/workers/SharedWorker.idl\nCyberCore/workers/SharedWorkerContext.idl\nCyberCore/workers/Worker.idl\nCyberCore/workers/WorkerContext.idl\nCyberCore/workers/WorkerLocation.idl\nCyberCore/xml/DOMParser.idl\nCyberCore/xml/XMLHttpRequest.idl\nCyberCore/xml/XMLHttpRequestException.idl\nCyberCore/xml/XMLHttpRequestProgressEvent.idl\nCyberCore/xml/XMLHttpRequestUpload.idl\nCyberCore/xml/XMLSerializer.idl\nCyberCore/xml/XPathEvaluator.idl\nCyberCore/xml/XPathException.idl\nCyberCore/xml/XPathExpression.idl\nCyberCore/xml/XPathNSResolver.idl\nCyberCore/xml/XPathResult.idl\nCyberCore/xml/XSLTProcessor.idl\nInternalSettingsGenerated.idl\nCyberCore/inspector/JavaScriptCallFrame.idl\n" > ./idl_files.tmp
perl CyberScriptCore/docs/make-bytecode-docs.pl CyberScriptCore/interpreter/Interpreter.cpp docs/bytecode.html
cat CyberCore/css/CSSPropertyNames.in CyberCore/css/SVGCSSPropertyNames.in > CSSPropertyNames.in
rm -f ./idl_files.tmp
python CyberScriptCore/KeywordLookupGenerator.py CyberScriptCore/parser/Keywords.table > KeywordLookup.h
sed -e s/\<CyberCore/\<CyberKit/ -e s/DOMDOMImplementation/DOMImplementation/ /Volumes/Data/Build/Release/CyberCore.framework/PrivateHeaders/DOM.h > /Volumes/Data/Build/Release/CyberKit.framework/Versions/A/Headers/DOM.h
END

for my $line (@expectIgnoredLines) {
    is(shouldIgnoreLine("", $line), 1, description("Ignored: " . $line));
}

#
# Test input starting with four spaces
#
my @buildSettingsLines = split(/$INPUT_RECORD_SEPARATOR/, <<'END');
Build settings from command line:
    ARCHS = i386 x86_64
    OBJROOT = /Volumes/Data/Build
    ONLY_ACTIVE_ARCH = NO
    SHARED_PRECOMPS_DIR = /Volumes/Data/Build/PrecompiledHeaders
    SYMROOT = /Volumes/Data/Build
END

for my $i (0..scalar(@buildSettingsLines) - 1) {
    my $previousLine = $i ? $buildSettingsLines[$i - 1] : "";
    my $line = $buildSettingsLines[$i];
    is(shouldIgnoreLine($previousLine, $line), 1, description("Ignored: " . $line));
}

#
# Test input for undefined symbols error message
#
my @undefinedSymbolsLines = split(/$INPUT_RECORD_SEPARATOR/, <<'END');
Undefined symbols for architecture x86_64:
  "__ZN6CyberKit12WebPageProxy28exposedRectChangedTimerFiredEPN7CyberCore5TimerIS0_EE", referenced from:
      __ZN6CyberKit12WebPageProxyC2EPNS_10PageClientEN3WTF10PassRefPtrINS_15WebProcessProxyEEEPNS_12WebPageGroupEy in WebPageProxy.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
END

for my $i (0..scalar(@undefinedSymbolsLines) - 1) {
    my $previousLine = $i ? $undefinedSymbolsLines[$i - 1] : "";
    my $line = $undefinedSymbolsLines[$i];
    is(shouldIgnoreLine($previousLine, $line), 0, description("Printed: " . $line));
}

my @ruleScriptLines = split(/$INPUT_RECORD_SEPARATOR/, <<'END');
RuleScriptExecution /Users/u/Build/Debug/LLIntOffsets/arm64e/LLIntDesiredOffsets.h /Users/u/Build/CyberScriptCore.build/Debug/JSCLLIntOffsetsExtractor.build/LowLevelInterpreter.d /Users/u/CyberKit/OpenSource/Source/CyberScriptCore/llint/LowLevelInterpreter.asm normal undefined_arch (in target 'JSCLLIntOffsetsExtractor' from project 'CyberScriptCore')
    cd /Users/u/CyberKit/OpenSource/Source/CyberScriptCore
    /bin/sh -c set\ -e'
''
'OFFLINEASM_ARGS\=\"\"'
'if\ \[\[\ \"\$\{DEPLOYMENT_LOCATION\}\"\ \=\=\ \"YES\"\ \]\]\;\ then'
'\ \ \ \ OFFLINEASM_ARGS\=\"\$\{OFFLINEASM_ARGS\}\ --webkit-additions-path\=\$\{WK_WEBKITADDITIONS_HEADERS_FOLDER_PATH\}\"'
'fi'
''
'/usr/bin/env\ ruby\ \"\$\{SRCROOT\}/offlineasm/generate_offset_extractor.rb\"\ \"-I\$\{BUILT_PRODUCTS_DIR\}/DerivedSources/CyberScriptCore\"\ \"\$\{INPUT_FILE_PATH\}\"\ \ \"\$\{BUILT_PRODUCTS_DIR\}/JSCLLIntSettingsExtractor\"\ \"\$\{SCRIPT_OUTPUT_FILE_0\}\"\ \"\$\{ARCHS\}\ C_LOOP\"\ \"\$\{BUILD_VARIANTS\}\"\ \$\{OFFLINEASM_ARGS\}\ --depfile\=\"\$\{TARGET_TEMP_DIR\}/\$\{INPUT_FILE_BASE\}.d\"'
''
'
RuleScriptExecution /Users/u/Build/Debug/usr/local/include/absl/utility/utility.h /Users/u/CyberKit/OpenSource/Source/ThirdParty/libwebrtc/Source/third_party/abseil-cpp/absl/utility/utility.h normal undefined_arch (in target 'absl' from project 'libwebrtc')
    cd /Users/u/CyberKit/OpenSource/Source/ThirdParty/libwebrtc
    /bin/sh -c cp\ -f\ \"\$\{INPUT_FILE_PATH\}\"\ \"\$\{SCRIPT_OUTPUT_FILE_0\}\"'
'
RuleScriptExecution stoptest.h
END

for my $i (0..scalar(@ruleScriptLines) - 1) {
    my $previousLine = $i ? $ruleScriptLines[$i - 1] : "";
    my $line = $ruleScriptLines[$i];
    if ($line =~ /RuleScriptExecution/) {
        is(shouldIgnoreLine($previousLine, $line), 0, description("Printed: " . $line));
    } else {
        is(shouldIgnoreLine($previousLine, $line), 1, description("Ignored: " . $line));
    }
}

# Investigate these in https://bugs.webkit.org/show_bug.cgi?id=245263.
my @diagProblemLines = split(/$INPUT_RECORD_SEPARATOR/, <<'END');
CompileC /Users/u/Build/CyberCore.build/Debug/CyberCore.build/Objects-normal/arm64e/UnifiedSource245.o /Users/u/Build/Debug/DerivedSources/CyberCore/unified-sources/UnifiedSource245.cpp normal arm64e c++ com.apple.compilers.llvm.clang.1_0.compiler (in target 'CyberCore' from project 'CyberCore')
    cd /Users/u/CyberKit/OpenSource/Source/CyberCore
    /Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Toolchains/OSX13.0.xctoolchain/usr/bin/clang -x c++ -target arm64e-apple-macos13.0 -fmessage-length\=0 -fdiagnostics-show-note-include-stack -fmacro-backtrace-limit\=0 -std\=c++2a -stdlib\=libc++ -gmodules -Wno-trigraphs -fno-exceptions -fno-rtti -fno-sanitize\=vptr -fpascal-strings -O0 -fno-common -Werror -Wno-missing-field-initializers -Wmissing-prototypes -Wunreachable-code -Wnon-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wno-float-conversion -Wnon-literal-null-conversion -Wobjc-literal-conversion -Wsign-compare -Wno-shorten-64-to-32 -Wnewline-eof -Wno-c++11-extensions -Wno-implicit-fallthrough -DBUILDING_WEBKIT -DGL_SILENCE_DEPRECATION\=1 -DGLES_SILENCE_DEPRECATION\=1 -isysroot /Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.0.Internal.sdk -fstrict-aliasing -Wdeprecated-declarations -Winvalid-offsetof -g -fvisibility\=hidden -fvisibility-inlines-hidden -fno-threadsafe-statics -Wno-sign-conversion -Winfinite-recursion -Wmove -Wcomma -Wblock-capture-autoreleasing -Wstrict-prototypes -Wrange-loop-analysis -Wno-semicolon-before-method-body -index-store-path /Users/u/Library/Developer/Xcode/DerivedData/CyberKit-hbntwurqoeetjbbukcpuwpfssnio/Index.noindex/DataStore -iquote /Users/u/Build/CyberCore.build/Debug/CyberCore.build/CyberCore-generated-files.hmap -I/Users/u/Build/CyberCore.build/Debug/CyberCore.build/CyberCore-own-target-headers.hmap -I/Users/u/Build/CyberCore.build/Debug/CyberCore.build/CyberCore-all-target-headers.hmap -iquote /Users/u/Build/CyberCore.build/Debug/CyberCore.build/CyberCore-project-headers.hmap -I/Users/u/Build/Debug/include -IPAL -IForwardingHeaders -I/Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.0.Internal.sdk/usr/include/libxslt -I/Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.0.Internal.sdk/usr/include/libxml2 -I/Users/u/Build/Debug/DerivedSources/CyberCore -I/Users/u/Build/Debug/usr/local/include -I/Users/u/Build/Debug/usr/local/include/CyberKitAdditions -I/Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.0.Internal.sdk/usr/local/include/CyberKitAdditions -I/Users/u/Build/Debug/usr/local/include -I/Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.0.Internal.sdk/usr/local/include -I/Users/u/Build/Debug/usr/local/include/webrtc -I/Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.0.Internal.sdk/usr/local/include/webrtc -I/Users/u/Build/Debug/usr/local/include/webrtc/sdk/objc/Framework/Headers -I/Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.0.Internal.sdk/usr/local/include/webrtc/sdk/objc/Framework/Headers -I/Users/u/CyberKit/OpenSource/Source/CyberCore -I/Users/u/Build/CyberCore.build/Debug/CyberCore.build/DerivedSources-normal/arm64e -I/Users/u/Build/CyberCore.build/Debug/CyberCore.build/DerivedSources/arm64e -I/Users/u/Build/CyberCore.build/Debug/CyberCore.build/DerivedSources -Wall -Wextra -Wcast-qual -Wchar-subscripts -Wconditional-uninitialized -Wextra-tokens -Wformat\=2 -Winit-self -Wmissing-format-attribute -Wmissing-noreturn -Wpacked -Wpointer-arith -Wredundant-decls -Wundef -Wwrite-strings -Wexit-time-destructors -Wglobal-constructors -Wtautological-compare -Wimplicit-fallthrough -Wvla -Wno-unknown-warning-option -Wliteral-conversion -Wthread-safety -Wno-profile-instr-out-of-date -Wno-profile-instr-unprofiled -F/Users/u/Build/Debug -iframework /Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.0.Internal.sdk/System/Library/PrivateFrameworks -iframework /Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.0.Internal.sdk/Library/Apple/System/Library/PrivateFrameworks -iframework /Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.0.Internal.sdk/System/Library/Frameworks -isystem /Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.0.Internal.sdk/System/Library/Frameworks/System.framework/PrivateHeaders -include /Users/u/Build/PrecompiledHeaders/SharedPrecompiledHeaders/15890480973332163591/CyberCorePrefix.h -MMD -MT dependencies -MF /Users/u/Build/CyberCore.build/Debug/CyberCore.build/Objects-normal/arm64e/UnifiedSource245.d --serialize-diagnostics /Users/u/Build/CyberCore.build/Debug/CyberCore.build/Objects-normal/arm64e/UnifiedSource245.dia -c /Users/u/Build/Debug/DerivedSources/CyberCore/unified-sources/UnifiedSource245.cpp -o /Users/u/Build/CyberCore.build/Debug/CyberCore.build/Objects-normal/arm64e/UnifiedSource245.o -index-unit-output-path /CyberCore.build/Debug/CyberCore.build/Objects-normal/arm64e/UnifiedSource245.o
/Users/u/Build/CyberCore.build/Debug/CyberCore.build/Objects-normal/arm64e/UnifiedSource245.dia:1:1: warning: Could not read serialized diagnostics file: error("Failed to open diagnostics file") (in target 'CyberCore' from project 'CyberCore')

CompileC stoptest
END

for my $i (0..scalar(@diagProblemLines) - 1) {
    my $previousLine = $i ? $diagProblemLines[$i - 1] : "";
    my $line = $diagProblemLines[$i];
    if ($line =~ /CompileC/) {
        is(shouldIgnoreLine($previousLine, $line), 0, description("Printed: " . $line));
    } else {
        is(shouldIgnoreLine($previousLine, $line), 1, description("Ignored: " . $line));
    }
}

my @productPackagingLines = split(/$INPUT_RECORD_SEPARATOR/, <<'END');
ProcessProductPackaging "" /Users/u/Build/libwebrtc.build/Debug/yasm.build/yasm.xcent (in target 'yasm' from project 'libwebrtc')
    cd /Users/u/CyberKit/OpenSource/Source/ThirdParty/libwebrtc

    Entitlements:

    {
    "com.apple.security.get-task-allow" = 1;
}

    builtin-productPackagingUtility -entitlements -format xml -o /Users/u/Build/libwebrtc.build/Debug/yasm.build/yasm.xcent

ProcessProductPackaging stoptest.h
END
for my $i (0..scalar(@productPackagingLines) - 1) {
    my $previousLine = $i ? $productPackagingLines[$i - 1] : "";
    my $line = $productPackagingLines[$i];
    if ($line =~ /ProcessProductPackaging/) {
        is(shouldIgnoreLine($previousLine, $line), 0, description("Printed: " . $line));
    } else {
        is(shouldIgnoreLine($previousLine, $line), 1, description("Ignored: " . $line));
    }
}


my @processProductPackagingDER = split(/$INPUT_RECORD_SEPARATOR/, <<'END');
ProcessProductPackagingDER /Users/u/Build/DumpRenderTree.build/Debug/DumpRenderTree.app.build/DumpRenderTree.app.xcent /Users/u/Build/DumpRenderTree.build/Debug/DumpRenderTree.app.build/DumpRenderTree.app.xcent.der (in target 'DumpRenderTree.app' from project 'DumpRenderTree')
    cd /Users/u/CyberKit/OpenSource/Tools/DumpRenderTree
    /usr/bin/derq query -f xml -i /Users/u/Build/DumpRenderTree.build/Debug/DumpRenderTree.app.build/DumpRenderTree.app.xcent -o /Users/u/Build/DumpRenderTree.build/Debug/DumpRenderTree.app.build/DumpRenderTree.app.xcent.der --raw

ProcessProductPackagingDER stoptest
END
for my $i (0..scalar(@processProductPackagingDER) - 1) {
    my $previousLine = $i ? $processProductPackagingDER[$i - 1] : "";
    my $line = $processProductPackagingDER[$i];
    if ($line =~ /ProcessProductPackagingDER/) {
        is(shouldIgnoreLine($previousLine, $line), 0, description("Printed: " . $line));
    } else {
        is(shouldIgnoreLine($previousLine, $line), 1, description("Ignored: " . $line));
    }
}

# Two below once https://bugs.webkit.org/show_bug.cgi?id=175997 is fixed.
my @libtoolSameMemberLines = split(/$INPUT_RECORD_SEPARATOR/, <<'END');
Libtool /Users/u/Build/Debug/libWTF.a normal (in target 'WTF' from project 'WTF')
    cd /Users/u/CyberKit/OpenSource/Source/WTF
    /Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Toolchains/OSX13.0.xctoolchain/usr/bin/libtool -static -arch_only arm64e -D -syslibroot /Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.0.Internal.sdk -L/Users/u/Build/Debug -L/Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.0.Internal.sdk/usr/local/lib -filelist /Users/u/Build/WTF.build/Debug/WTF.build/Objects-normal/arm64e/WTF.LinkFileList -lbmalloc -dependency_info /Users/u/Build/WTF.build/Debug/WTF.build/Objects-normal/arm64e/WTF_libtool_dependency_info.dat -o /Users/u/Build/Debug/libWTF.a
/Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Toolchains/OSX13.0.xctoolchain/usr/bin/libtool: warning same member name (DebugHeap.o) in output file used for input files: /Users/u/Build/WTF.build/Debug/WTF.build/Objects-normal/arm64e/DebugHeap.o and: /Users/u/Build/Debug/libbmalloc.a(DebugHeap.o) due to use of basename, truncation and blank padding
/Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Toolchains/OSX13.0.xctoolchain/usr/bin/libtool: warning same member name (Gigacage.o) in output file used for input files: /Users/u/Build/WTF.build/Debug/WTF.build/Objects-normal/arm64e/Gigacage.o and: /Users/u/Build/Debug/libbmalloc.a(Gigacage.o) due to use of basename, truncation and blank padding
/Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Toolchains/OSX13.0.xctoolchain/usr/bin/libtool: warning same member name (Logging.o) in output file used for input files: /Users/u/Build/WTF.build/Debug/WTF.build/Objects-normal/arm64e/Logging.o and: /Users/u/Build/Debug/libbmalloc.a(Logging.o) due to use of basename, truncation and blank padding

Libtool stoptest
END

for my $i (0..scalar(@libtoolSameMemberLines) - 1) {
    my $previousLine = $i ? $libtoolSameMemberLines[$i - 1] : "";
    my $line = $libtoolSameMemberLines[$i];
    if ($line =~ /Libtool/) {
        is(shouldIgnoreLine($previousLine, $line), 0, description("Printed: " . $line));
    } else {
        is(shouldIgnoreLine($previousLine, $line), 1, description("Ignored: " . $line));
    }
}

my @dsymNoObjectFileLines = split(/$INPUT_RECORD_SEPARATOR/, <<'END');
GenerateDSYMFile /Users/u/Build/Debug/TestWTF.dSYM /Users/u/Build/Debug/TestWTF (in target 'TestWTF' from project 'TestCyberKitAPI')
    cd /Users/u/CyberKit/OpenSource/Tools/TestCyberKitAPI
    /Volumes/Xcode14A6270e_m20A2411_m22A338_i20A358_FastSim_Boost_Encrypted_53GB/Xcode.app/Contents/Developer/Toolchains/OSX13.0.xctoolchain/usr/bin/dsymutil /Users/u/Build/Debug/TestWTF -o /Users/u/Build/Debug/TestWTF.dSYM
warning: (arm64e)  could not find object file symbol for symbol __ZNK3WTF10AtomString23convertToASCIILowercaseEv
warning: (arm64e)  could not find object file symbol for symbol __ZNK3WTF10AtomString16convertASCIICaseILNS0_15CaseConvertTypeE1EEES0_v
warning: (arm64e)  could not find object file symbol for symbol __ZNK3WTF10AtomString23convertToASCIIUppercaseEv
warning: (arm64e)  could not find object file symbol for symbol __ZNK3WTF10AtomString16convertASCIICaseILNS0_15CaseConvertTypeE0EEES0_v
warning: (arm64e)  could not find object file symbol for symbol __ZN3WTF10AtomString6numberEi
warning: (arm64e)  could not find object file symbol for symbol __ZN3WTF20numberToStringSignedINS_10AtomStringEiEENS_30IntegerToStringConversionTraitIT_E10ReturnTypeET0_PNS4_22AdditionalArgumentTypeE
warning: (arm64e)  could not find object file symbol for symbol __ZN3WTF10AtomString6numberEj

GenerateDSYMFile stoptest
END
for my $i (0..scalar(@dsymNoObjectFileLines) - 1) {
    my $previousLine = $i ? $dsymNoObjectFileLines[$i - 1] : "";
    my $line = $dsymNoObjectFileLines[$i];
    if ($line =~ /GenerateDSYMFile/) {
        is(shouldIgnoreLine($previousLine, $line), 0, description("Printed: " . $line));
    } else {
        is(shouldIgnoreLine($previousLine, $line), 1, description("Ignored: " . $line));
    }
}

my @buildDescriptionLines = split(/$INPUT_RECORD_SEPARATOR/, <<'END');
Create build description
Build description signature: 6df54043eda5ce9a5bc62735755fc500
Build description path: /Users/u/Build/XCBuildData/6df54043eda5ce9a5bc62735755fc500-desc.xcbuild
note: Building targets in dependency order
Create build description
END
for my $i (0..scalar(@buildDescriptionLines) - 1) {
    my $previousLine = $i ? $dsymNoObjectFileLines[$i - 1] : "";
    my $line = $buildDescriptionLines[$i];
    if ($line =~ /Create build description/) {
        is(shouldIgnoreLine($previousLine, $line), 0, description("Printed: " . $line));
    } else {
        is(shouldIgnoreLine($previousLine, $line), 1, description("Ignored: " . $line));
    }
}
my @libtoolEmptyTOC = split(/$INPUT_RECORD_SEPARATOR/, <<'END');
Libtool libCyberKitAdditions
warning: /Volumes/Somevolume/Xcode.app/Contents/Developer/Toolchains/OSX13.0.xctoolchain/usr/bin/libtool: archive library: /Users/u/Build/Debug/libCyberKitAdditions.a the table of contents is empty (no object file members in the library define global symbols)
Libtool endtest
END
for my $i (0..scalar(@libtoolEmptyTOC) - 1) {
    my $previousLine = $i ? $libtoolEmptyTOC[$i - 1] : "";
    my $line = $libtoolEmptyTOC[$i];
    if ($line =~ /Libtool/) {
        is(shouldIgnoreLine($previousLine, $line), 0, description("Printed: " . $line));
    } else {
        is(shouldIgnoreLine($previousLine, $line), 1, description("Ignored: " . $line));
    }
}

my @generatePrefsLines = split(/$INPUT_RECORD_SEPARATOR/, <<'END');
PhaseScriptExecution Run\ Script /Users/u/Build/DumpRenderTree.build/Debug/Derived\ Sources.build/Script-0F18E7011D6B9CC60027E547.sh (in target 'Derived Sources' from project 'DumpRenderTree')
    cd /Users/u/CyberKit/OpenSource/Tools/DumpRenderTree
    export ACTION\=build
    /bin/sh -c /Users/u/Build/DumpRenderTree.build/Debug/Derived\\\ Sources.build/Script-0F18E7011D6B9CC60027E547.sh
Generating bindings for UIScriptController...
ruby /Users/u/Build/Debug/usr/local/include/wtf/Scripts/GeneratePreferences.rb --frontend CyberKitLegacy --base /Users/u/Build/Debug/usr/local/include/wtf/Scripts/Preferences/WebPreferences.yaml --debug /Users/u/Build/Debug/usr/local/include/wtf/Scripts/Preferences/WebPreferencesDebug.yaml --experimental /Users/u/Build/Debug/usr/local/include/wtf/Scripts/Preferences/WebPreferencesExperimental.yaml --internal /Users/u/Build/Debug/usr/local/include/wtf/Scripts/Preferences/WebPreferencesInternal.yaml --template /Users/u/CyberKit/OpenSource/Tools/DumpRenderTree/Scripts/PreferencesTemplates/TestOptionsGeneratedCyberKitLegacyKeyMapping.cpp.erb
perl -I /Users/u/Build/Debug/CyberCore.framework/Versions/A/PrivateHeaders -I /Users/u/CyberKit/OpenSource/Tools/DumpRenderTree/../TestRunnerShared/UIScriptContext/Bindings -I /Users/u/CyberKit/OpenSource/Tools/DumpRenderTree/Bindings /Users/u/Build/Debug/CyberCore.framework/Versions/A/PrivateHeaders/generate-bindings.pl --defines "ENABLE_3D_TRANSFORMS WTF_PLATFORM_COCOA WTF_PLATFORM_MAC" --include /Users/u/CyberKit/OpenSource/Tools/DumpRenderTree/../TestRunnerShared/UIScriptContext/Bindings --outputDir . --generator DumpRenderTree --idlAttributesFile /Users/u/Build/Debug/CyberCore.framework/Versions/A/PrivateHeaders/IDLAttributes.json /Users/u/CyberKit/OpenSource/Tools/DumpRenderTree/../TestRunnerShared/UIScriptContext/Bindings/UIScriptController.idl
ruby /Users/u/Build/Debug/usr/local/include/wtf/Scripts/GeneratePreferences.rb --frontend CyberKitLegacy --base /Users/u/Build/Debug/usr/local/include/wtf/Scripts/Preferences/WebPreferences.yaml --debug /Users/u/Build/Debug/usr/local/include/wtf/Scripts/Preferences/WebPreferencesDebug.yaml --experimental /Users/u/Build/Debug/usr/local/include/wtf/Scripts/Preferences/WebPreferencesExperimental.yaml --internal /Users/u/Build/Debug/usr/local/include/wtf/Scripts/Preferences/WebPreferencesInternal.yaml --template /Users/u/CyberKit/OpenSource/Tools/DumpRenderTree/Scripts/PreferencesTemplates/TestOptionsGeneratedKeys.h.erb
PhaseScriptExecution endtest
END
for my $i (0..scalar(@generatePrefsLines) - 1) {
    my $previousLine = $i ? $generatePrefsLines[$i - 1] : "";
    my $line = $generatePrefsLines[$i];
    if ($line =~ /GeneratePreferences.rb|PhaseScriptExecution|Generating bindings/) {
        is(shouldIgnoreLine($previousLine, $line), 0, description("Printed: " . $line));
    } else {
        is(shouldIgnoreLine($previousLine, $line), 1, description("Ignored: " . $line));
    }
}

done_testing();

sub description($)
{
    my ($line) = @_;

    my $maxLineLength = 200;
    my $ellipsis = "...";
    my $truncateLength = $maxLineLength - length($ellipsis);

    my $description = length($line) > $maxLineLength ? substr($line, 0, $truncateLength) : $line;
    $description .= $ellipsis if length($line) != length($description);

    return $description;
}
