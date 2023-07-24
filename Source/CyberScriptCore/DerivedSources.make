# Copyright (C) 2006-2020 Apple Inc. All rights reserved.
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
    $(CyberScriptCore) \
    $(CyberScriptCore)/parser \
    $(CyberScriptCore)/runtime \
    $(CyberScriptCore)/interpreter \
    $(CyberScriptCore)/jit \
    $(CyberScriptCore)/builtins \
    $(CyberScriptCore)/wasm/js \
#

CyberScriptCore_SCRIPTS_DIR = $(CyberScriptCore)/Scripts

.PHONY : all
all : \
    udis86_itab.h \
    InjectedScriptSource.h \
    JSCBuiltins.h \
    Lexer.lut.h \
    KeywordLookup.h \
    RegExpJitTables.h \
    UnicodePatternTables.h \
    yarr/YarrCanonicalizeUnicode.cpp \
    WasmOps.h \
    WasmB3IRGeneratorInlines.h \
#

# JavaScript builtins.

BUILTINS_GENERATOR_SCRIPTS = \
    $(CyberScriptCore_SCRIPTS_DIR)/wkbuiltins/__init__.py \
    $(CyberScriptCore_SCRIPTS_DIR)/wkbuiltins/wkbuiltins.py \
    $(CyberScriptCore_SCRIPTS_DIR)/wkbuiltins/builtins_generator.py \
    $(CyberScriptCore_SCRIPTS_DIR)/wkbuiltins/builtins_model.py \
    $(CyberScriptCore_SCRIPTS_DIR)/wkbuiltins/builtins_templates.py \
    $(CyberScriptCore_SCRIPTS_DIR)/wkbuiltins/builtins_generate_combined_header.py \
    $(CyberScriptCore_SCRIPTS_DIR)/wkbuiltins/builtins_generate_combined_implementation.py \
    $(CyberScriptCore_SCRIPTS_DIR)/wkbuiltins/builtins_generate_separate_header.py \
    $(CyberScriptCore_SCRIPTS_DIR)/wkbuiltins/builtins_generate_separate_implementation.py \
    ${CyberScriptCore_SCRIPTS_DIR}/wkbuiltins/builtins_generate_wrapper_header.py \
    ${CyberScriptCore_SCRIPTS_DIR}/wkbuiltins/builtins_generate_wrapper_implementation.py \
    ${CyberScriptCore_SCRIPTS_DIR}/wkbuiltins/builtins_generate_internals_wrapper_header.py \
    ${CyberScriptCore_SCRIPTS_DIR}/wkbuiltins/builtins_generate_internals_wrapper_implementation.py \
    $(CyberScriptCore_SCRIPTS_DIR)/generate-js-builtins.py \
    $(CyberScriptCore_SCRIPTS_DIR)/lazywriter.py \
#

CyberScriptCore_BUILTINS_SOURCES = \
    $(CyberScriptCore)/builtins/AsyncFromSyncIteratorPrototype.js \
    $(CyberScriptCore)/builtins/ArrayConstructor.js \
    $(CyberScriptCore)/builtins/ArrayIteratorPrototype.js \
    $(CyberScriptCore)/builtins/ArrayPrototype.js \
    $(CyberScriptCore)/builtins/AsyncIteratorPrototype.js \
    $(CyberScriptCore)/builtins/AsyncFunctionPrototype.js \
    $(CyberScriptCore)/builtins/AsyncGeneratorPrototype.js \
    $(CyberScriptCore)/builtins/DatePrototype.js \
    $(CyberScriptCore)/builtins/FunctionPrototype.js \
    $(CyberScriptCore)/builtins/GeneratorPrototype.js \
    $(CyberScriptCore)/builtins/GlobalObject.js \
    $(CyberScriptCore)/builtins/GlobalOperations.js \
    $(CyberScriptCore)/builtins/InspectorInstrumentationObject.js \
    $(CyberScriptCore)/builtins/InternalPromiseConstructor.js \
    $(CyberScriptCore)/builtins/IteratorHelpers.js \
    $(CyberScriptCore)/builtins/IteratorPrototype.js \
    $(CyberScriptCore)/builtins/MapIteratorPrototype.js \
    $(CyberScriptCore)/builtins/MapPrototype.js \
    $(CyberScriptCore)/builtins/ModuleLoader.js \
    $(CyberScriptCore)/builtins/NumberConstructor.js \
    $(CyberScriptCore)/builtins/ObjectConstructor.js \
    $(CyberScriptCore)/builtins/PromiseConstructor.js \
    $(CyberScriptCore)/builtins/PromiseOperations.js \
    $(CyberScriptCore)/builtins/PromisePrototype.js \
    $(CyberScriptCore)/builtins/ReflectObject.js \
    $(CyberScriptCore)/builtins/RegExpPrototype.js \
    ${CyberScriptCore}/builtins/RegExpStringIteratorPrototype.js \
    $(CyberScriptCore)/builtins/SetIteratorPrototype.js \
    $(CyberScriptCore)/builtins/SetPrototype.js \
    $(CyberScriptCore)/builtins/StringConstructor.js \
    $(CyberScriptCore)/builtins/StringIteratorPrototype.js \
    $(CyberScriptCore)/builtins/StringPrototype.js \
    $(CyberScriptCore)/builtins/TypedArrayConstructor.js \
    $(CyberScriptCore)/builtins/TypedArrayPrototype.js \
    $(CyberScriptCore)/builtins/WebAssembly.js \
#

# The combined output file depends on the contents of builtins and generator scripts, so
# adding, modifying, or removing builtins or scripts will trigger regeneration of files.

CyberScriptCore_BUILTINS_DEPENDENCIES_LIST : $(CyberScriptCore_SCRIPTS_DIR)/UpdateContents.py DerivedSources.make
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/UpdateContents.py '$(CyberScriptCore_BUILTINS_SOURCES) $(BUILTINS_GENERATOR_SCRIPTS)' $@

JSCBuiltins.h: $(BUILTINS_GENERATOR_SCRIPTS) $(CyberScriptCore_BUILTINS_SOURCES) CyberScriptCore_BUILTINS_DEPENDENCIES_LIST
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/generate-js-builtins.py --combined --output-directory . --framework CyberScriptCore $(CyberScriptCore_BUILTINS_SOURCES)

# Perfect hash lookup tables for JavaScript classes.

OBJECT_LUT_HEADERS = \
    AsyncFromSyncIteratorPrototype.lut.h \
    ArrayConstructor.lut.h \
    AsyncGeneratorPrototype.lut.h \
    BigIntConstructor.lut.h \
    BigIntPrototype.lut.h \
    BooleanPrototype.lut.h \
    DateConstructor.lut.h \
    DatePrototype.lut.h \
    ErrorPrototype.lut.h \
    GeneratorPrototype.lut.h \
    InspectorInstrumentationObject.lut.h \
    IntlCollatorConstructor.lut.h \
    IntlCollatorPrototype.lut.h \
    IntlDateTimeFormatConstructor.lut.h \
    IntlDateTimeFormatPrototype.lut.h \
    IntlDisplayNamesConstructor.lut.h \
    IntlDisplayNamesPrototype.lut.h \
    IntlLocalePrototype.lut.h \
    IntlNumberFormatConstructor.lut.h \
    IntlNumberFormatPrototype.lut.h \
    IntlObject.lut.h \
    IntlPluralRulesConstructor.lut.h \
    IntlPluralRulesPrototype.lut.h \
    IntlRelativeTimeFormatConstructor.lut.h \
    IntlRelativeTimeFormatPrototype.lut.h \
    JSDataViewPrototype.lut.h \
    JSGlobalObject.lut.h \
    JSInternalPromiseConstructor.lut.h \
    JSModuleLoader.lut.h \
    JSONObject.lut.h \
    JSPromiseConstructor.lut.h \
    JSPromisePrototype.lut.h \
    JSWebAssembly.lut.h \
    MapPrototype.lut.h \
    NumberConstructor.lut.h \
    NumberPrototype.lut.h \
    ObjectConstructor.lut.h \
    ReflectObject.lut.h \
    RegExpConstructor.lut.h \
    SetPrototype.lut.h \
    StringConstructor.lut.h \
    StringPrototype.lut.h \
    SymbolConstructor.lut.h \
    SymbolPrototype.lut.h \
    WebAssemblyCompileErrorConstructor.lut.h \
    WebAssemblyCompileErrorPrototype.lut.h \
    WebAssemblyGlobalConstructor.lut.h \
    WebAssemblyGlobalPrototype.lut.h \
    WebAssemblyInstanceConstructor.lut.h \
    WebAssemblyInstancePrototype.lut.h \
    WebAssemblyLinkErrorConstructor.lut.h \
    WebAssemblyLinkErrorPrototype.lut.h \
    WebAssemblyMemoryConstructor.lut.h \
    WebAssemblyMemoryPrototype.lut.h \
    WebAssemblyModuleConstructor.lut.h \
    WebAssemblyModulePrototype.lut.h \
    WebAssemblyRuntimeErrorConstructor.lut.h \
    WebAssemblyRuntimeErrorPrototype.lut.h \
    WebAssemblyTableConstructor.lut.h \
    WebAssemblyTablePrototype.lut.h \
#

$(OBJECT_LUT_HEADERS): %.lut.h : %.cpp $(CyberScriptCore)/create_hash_table
	$(PERL) $(CyberScriptCore)/create_hash_table $< > $@

Lexer.lut.h: Keywords.table $(CyberScriptCore)/create_hash_table
	$(PERL) $(CyberScriptCore)/create_hash_table $< > $@

# character tables for Yarr

RegExpJitTables.h: yarr/create_regex_tables
	$(PYTHON) $^ > $@

KeywordLookup.h: KeywordLookupGenerator.py Keywords.table
	$(PYTHON) $^ > $@

# udis86 instruction tables

udis86_itab.h: $(CyberScriptCore)/disassembler/udis86/ud_itab.py $(CyberScriptCore)/disassembler/udis86/optable.xml
	$(PYTHON) $(CyberScriptCore)/disassembler/udis86/ud_itab.py $(CyberScriptCore)/disassembler/udis86/optable.xml .

# Bytecode files

BYTECODE_FILES = \
    Bytecodes.h \
    BytecodeIndices.h \
    BytecodeStructs.h \
    InitBytecodes.asm \
    WasmLLIntGeneratorInlines.h \
    InitWasm.asm \
    BytecodeDumperGenerated.cpp \
#
BYTECODE_FILES_PATTERNS = $(subst .,%,$(BYTECODE_FILES))

all : $(BYTECODE_FILES)

$(BYTECODE_FILES_PATTERNS): $(wildcard $(CyberScriptCore)/generator/*.rb) $(CyberScriptCore)/bytecode/BytecodeList.rb $(CyberScriptCore)/wasm/wasm.json
	$(RUBY) $(CyberScriptCore)/generator/main.rb $(CyberScriptCore)/bytecode/BytecodeList.rb \
    --bytecode_structs_h BytecodeStructs.h \
    --init_bytecodes_asm InitBytecodes.asm \
    --bytecodes_h Bytecodes.h \
    --bytecode_indices_h BytecodeIndices.h \
    --wasm_json $(CyberScriptCore)/wasm/wasm.json \
    --wasm_llint_generator_h WasmLLIntGeneratorInlines.h \
    --init_wasm_llint InitWasm.asm \
    --bytecode_dumper BytecodeDumperGenerated.cpp \

# Inspector interfaces

INSPECTOR_DOMAINS := \
    $(CyberScriptCore)/inspector/protocol/Animation.json \
    $(CyberScriptCore)/inspector/protocol/ApplicationCache.json \
    $(CyberScriptCore)/inspector/protocol/Audit.json \
    $(CyberScriptCore)/inspector/protocol/Browser.json \
    $(CyberScriptCore)/inspector/protocol/CPUProfiler.json \
    $(CyberScriptCore)/inspector/protocol/CSS.json \
    $(CyberScriptCore)/inspector/protocol/Canvas.json \
    $(CyberScriptCore)/inspector/protocol/Console.json \
    $(CyberScriptCore)/inspector/protocol/DOM.json \
    $(CyberScriptCore)/inspector/protocol/DOMDebugger.json \
    $(CyberScriptCore)/inspector/protocol/DOMStorage.json \
    $(CyberScriptCore)/inspector/protocol/Database.json \
    $(CyberScriptCore)/inspector/protocol/Debugger.json \
    $(CyberScriptCore)/inspector/protocol/GenericTypes.json \
    $(CyberScriptCore)/inspector/protocol/Heap.json \
    $(CyberScriptCore)/inspector/protocol/IndexedDB.json \
    $(CyberScriptCore)/inspector/protocol/Inspector.json \
    $(CyberScriptCore)/inspector/protocol/LayerTree.json \
    $(CyberScriptCore)/inspector/protocol/Memory.json \
    $(CyberScriptCore)/inspector/protocol/Network.json \
    $(CyberScriptCore)/inspector/protocol/Page.json \
    $(CyberScriptCore)/inspector/protocol/Recording.json \
    $(CyberScriptCore)/inspector/protocol/Runtime.json \
    $(CyberScriptCore)/inspector/protocol/ScriptProfiler.json \
    $(CyberScriptCore)/inspector/protocol/Security.json \
    $(CyberScriptCore)/inspector/protocol/ServiceWorker.json \
    $(CyberScriptCore)/inspector/protocol/Target.json \
    $(CyberScriptCore)/inspector/protocol/Timeline.json \
    $(CyberScriptCore)/inspector/protocol/Worker.json \
#

INSPECTOR_GENERATOR_SCRIPTS = \
	$(CyberScriptCore)/inspector/scripts/codegen/__init__.py \
	$(CyberScriptCore)/inspector/scripts/codegen/cpp_generator_templates.py \
	$(CyberScriptCore)/inspector/scripts/codegen/cpp_generator.py \
	$(CyberScriptCore)/inspector/scripts/codegen/generate_cpp_backend_dispatcher_header.py \
	$(CyberScriptCore)/inspector/scripts/codegen/generate_cpp_backend_dispatcher_implementation.py \
	$(CyberScriptCore)/inspector/scripts/codegen/generate_cpp_frontend_dispatcher_header.py \
	$(CyberScriptCore)/inspector/scripts/codegen/generate_cpp_frontend_dispatcher_implementation.py \
	$(CyberScriptCore)/inspector/scripts/codegen/generate_cpp_protocol_types_header.py \
	$(CyberScriptCore)/inspector/scripts/codegen/generate_cpp_protocol_types_implementation.py \
	$(CyberScriptCore)/inspector/scripts/codegen/generate_js_backend_commands.py \
	$(CyberScriptCore)/inspector/scripts/codegen/generator_templates.py \
	$(CyberScriptCore)/inspector/scripts/codegen/generator.py \
	$(CyberScriptCore)/inspector/scripts/codegen/models.py \
	$(CyberScriptCore)/inspector/scripts/codegen/preprocess.pl \
	$(CyberScriptCore)/inspector/scripts/generate-inspector-protocol-bindings.py \
	$(CyberScriptCore_SCRIPTS_DIR)/generate-combined-inspector-json.py \
#

# TODO: Is there some way to not hardcode this? Can we get it from
# generate-inspector-protocol-bindings.py and ./CombinedDomains.json?
INSPECTOR_DISPATCHER_FILES = \
    inspector/InspectorAlternateBackendDispatchers.h \
    inspector/InspectorBackendDispatchers.cpp \
    inspector/InspectorBackendDispatchers.h \
    inspector/InspectorFrontendDispatchers.cpp \
    inspector/InspectorFrontendDispatchers.h \
    inspector/InspectorProtocolObjects.cpp \
    inspector/InspectorProtocolObjects.h \
#
INSPECTOR_DISPATCHER_FILES_PATTERNS = $(subst .,%,$(INSPECTOR_DISPATCHER_FILES))

all : $(INSPECTOR_DISPATCHER_FILES) inspector/InspectorBackendCommands.js

# The combined JSON file depends on the actual set of domains and their file contents, so that
# adding, modifying, or removing domains will trigger regeneration of inspector files.

.PHONY: force
EnabledInspectorDomains : $(CyberScriptCore_SCRIPTS_DIR)/UpdateContents.py force
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/UpdateContents.py '$(INSPECTOR_DOMAINS)' $@

CombinedDomains.json : $(CyberScriptCore_SCRIPTS_DIR)/generate-combined-inspector-json.py $(INSPECTOR_DOMAINS) EnabledInspectorDomains
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/generate-combined-inspector-json.py $(INSPECTOR_DOMAINS) > ./CombinedDomains.json

# Inspector Backend Dispatchers, Frontend Dispatchers, Type Builders
$(INSPECTOR_DISPATCHER_FILES_PATTERNS) : CombinedDomains.json $(INSPECTOR_GENERATOR_SCRIPTS)
	$(PYTHON) $(CyberScriptCore)/inspector/scripts/generate-inspector-protocol-bindings.py --framework CyberScriptCore --outputDir inspector ./CombinedDomains.json

inspector/InspectorBackendCommands.js : CombinedDomains.json $(INSPECTOR_GENERATOR_SCRIPTS) $(FEATURE_AND_PLATFORM_DEFINE_DEPENDENCIES)
	$(PYTHON) $(CyberScriptCore)/inspector/scripts/generate-inspector-protocol-bindings.py --framework WebInspectorUI --outputDir inspector ./CombinedDomains.json
	@echo Pre-processing InspectorBackendCommands...
	$(PERL) $(CyberScriptCore)/inspector/scripts/codegen/preprocess.pl --input inspector/InspectorBackendCommands.js.in --defines "$(FEATURE_AND_PLATFORM_DEFINES)" --output inspector/InspectorBackendCommands.js
	$(DELETE) inspector/InspectorBackendCommands.js.in

InjectedScriptSource.h : inspector/InjectedScriptSource.js $(CyberScriptCore_SCRIPTS_DIR)/jsmin.py $(CyberScriptCore_SCRIPTS_DIR)/xxd.pl
	echo "//# sourceURL=__InjectedScript_InjectedScriptSource.js" > ./InjectedScriptSource.min.js
	$(PYTHON) $(CyberScriptCore_SCRIPTS_DIR)/jsmin.py < $(CyberScriptCore)/inspector/InjectedScriptSource.js >> ./InjectedScriptSource.min.js
	$(PERL) $(CyberScriptCore_SCRIPTS_DIR)/xxd.pl InjectedScriptSource_js ./InjectedScriptSource.min.js InjectedScriptSource.h
	$(DELETE) InjectedScriptSource.min.js

AIR_OPCODE_FILES = \
    AirOpcode.h \
    AirOpcodeUtils.h \
    AirOpcodeGenerated.h \
#
AIR_OPCODE_FILES_PATTERNS = $(subst .,%,$(AIR_OPCODE_FILES))

all : $(AIR_OPCODE_FILES)

$(AIR_OPCODE_FILES_PATTERNS) : $(CyberScriptCore)/b3/air/opcode_generator.rb $(CyberScriptCore)/b3/air/AirOpcode.opcodes
	$(RUBY) $^

UnicodePatternTables.h: $(CyberScriptCore)/yarr/generateYarrUnicodePropertyTables.py $(CyberScriptCore)/yarr/hasher.py $(CyberScriptCore)/ucd/DerivedBinaryProperties.txt $(CyberScriptCore)/ucd/DerivedCoreProperties.txt $(CyberScriptCore)/ucd/DerivedNormalizationProps.txt $(CyberScriptCore)/ucd/PropList.txt $(CyberScriptCore)/ucd/PropertyAliases.txt $(CyberScriptCore)/ucd/PropertyValueAliases.txt $(CyberScriptCore)/ucd/ScriptExtensions.txt $(CyberScriptCore)/ucd/Scripts.txt $(CyberScriptCore)/ucd/UnicodeData.txt $(CyberScriptCore)/ucd/emoji-data.txt
	$(PYTHON) $(CyberScriptCore)/yarr/generateYarrUnicodePropertyTables.py $(CyberScriptCore)/ucd ./UnicodePatternTables.h

yarr/YarrCanonicalizeUnicode.cpp: $(CyberScriptCore)/yarr/generateYarrCanonicalizeUnicode $(CyberScriptCore)/ucd/CaseFolding.txt
	$(PYTHON) $(CyberScriptCore)/yarr/generateYarrCanonicalizeUnicode $(CyberScriptCore)/ucd/CaseFolding.txt ./yarr/YarrCanonicalizeUnicode.cpp

WasmOps.h: $(CyberScriptCore)/wasm/generateWasmOpsHeader.py $(CyberScriptCore)/wasm/generateWasm.py $(CyberScriptCore)/wasm/wasm.json
	$(PYTHON) $(CyberScriptCore)/wasm/generateWasmOpsHeader.py $(CyberScriptCore)/wasm/wasm.json ./WasmOps.h

WasmB3IRGeneratorInlines.h: $(CyberScriptCore)/wasm/generateWasmB3IRGeneratorInlinesHeader.py $(CyberScriptCore)/wasm/generateWasm.py $(CyberScriptCore)/wasm/wasm.json
	$(PYTHON) $(CyberScriptCore)/wasm/generateWasmB3IRGeneratorInlinesHeader.py $(CyberScriptCore)/wasm/wasm.json ./WasmB3IRGeneratorInlines.h

# Dynamically-defined targets are listed below. Static targets belong up top.

all : \
    $(OBJECT_LUT_HEADERS) \
#
