/*
 *  Copyright (c) 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 *
 */

#ifndef SDK_OBJC_FRAMEWORK_CLASSES_VIDEOTOOLBOX_HELPERS_H_
#define SDK_OBJC_FRAMEWORK_CLASSES_VIDEOTOOLBOX_HELPERS_H_

#include <CoreFoundation/CoreFoundation.h>
#include <VideoToolbox/VideoToolbox.h>
#include <string>

// Convenience function for creating a dictionary.
inline CFDictionaryRef CreateCFTypeDictionary(CFTypeRef* keys,
                                              CFTypeRef* values,
                                              size_t size) {
  return CFDictionaryCreate(kCFAllocatorDefault, keys, values, size,
                            &kCFTypeDictionaryKeyCallBacks,
                            &kCFTypeDictionaryValueCallBacks);
}

// Copies characters from a CFStringRef into a std::string.
std::string CFStringToString(const CFStringRef cf_string);

// Convenience function for setting a VT property.
void SetVTSessionProperty(VTCompressionSessionRef session, CFStringRef key, int32_t value);

// Convenience function for setting a VT property.
void SetVTSessionProperty(VTCompressionSessionRef session, CFStringRef key, uint32_t value);
void SetVTSessionProperty(VTCompressionSessionRef session, CFStringRef key, double value);

// Convenience function for setting a VT property.
void SetVTSessionProperty(VTCompressionSessionRef session, CFStringRef key, bool value);

// Convenience function for setting a VT property.
void SetVTSessionProperty(VTCompressionSessionRef session, CFStringRef key, CFStringRef value);

// Convenience function for setting a VT property.
void SetVTSessionProperty(VTCompressionSessionRef session, CFStringRef key, CFArrayRef value);


// Slightly modified soft linking definitions from WTF since we can't import it

#define SOFT_LINK_FRAMEWORK_FOR_HEADER(functionNamespace, framework) \
    extern void* framework##Library(bool isOptional); \
    bool is##framework##FrameworkAvailable(void); \
    inline bool is##framework##FrameworkAvailable(void) { \
        return framework##Library(true) != NULL; \
    }

#define SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(functionNamespace, framework, variableName, variableType) \
    bool canLoad_##framework##_##variableName(void); \
    bool init_##framework##_##variableName(void); \
    variableType get_##framework##_##variableName(void); \

#define ASSERT_WITH_MESSAGE(assertion, ...) ((void)0)
#define RELEASE_ASSERT_WITH_MESSAGE(assertion, ...) ASSERT_WITH_MESSAGE(assertion, __VA_ARGS__)

#define SOFT_LINK_FRAMEWORK_FOR_SOURCE_WITH_EXPORT(functionNamespace, framework, export) \
    export void* framework##Library(bool isOptional); \
    void* framework##Library(bool isOptional) \
    { \
        static void* frameworkLibrary; \
        static dispatch_once_t once; \
        dispatch_once(&once, ^{ \
            frameworkLibrary = dlopen("/System/Library/Frameworks/" #framework ".framework/" #framework, RTLD_NOW); \
            if (!isOptional) \
                RELEASE_ASSERT_WITH_MESSAGE(frameworkLibrary, "%s", dlerror()); \
        }); \
        return frameworkLibrary; \
    }

#define SOFT_LINK_FRAMEWORK_FOR_SOURCE(functionNamespace, framework) \
    SOFT_LINK_FRAMEWORK_FOR_SOURCE_WITH_EXPORT(functionNamespace, framework, )

#define SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE_WITH_EXPORT(functionNamespace, framework, variableName, variableType, export) \
    static variableType constant##framework##variableName; \
    bool init_##framework##_##variableName(); \
    bool init_##framework##_##variableName() \
    { \
        void* constant = dlsym(framework##Library(false), #variableName); \
        if (!constant) \
            return false; \
        constant##framework##variableName = *static_cast<variableType const *>(constant); \
        return true; \
    } \
    export bool canLoad_##framework##_##variableName(); \
    bool canLoad_##framework##_##variableName() \
    { \
        static bool loaded = init_##framework##_##variableName(); \
        return loaded; \
    } \
    export variableType get_##framework##_##variableName(); \
    variableType get_##framework##_##variableName() \
    { \
        return constant##framework##variableName; \
    }

#define SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE(functionNamespace, framework, variableName, variableType) \
    SOFT_LINK_CONSTANT_MAY_FAIL_FOR_SOURCE_WITH_EXPORT(functionNamespace, framework, variableName, variableType, )

SOFT_LINK_FRAMEWORK_FOR_HEADER(, VideoToolbox)
SOFT_LINK_CONSTANT_MAY_FAIL_FOR_HEADER(, VideoToolbox, kVTCompressionPropertyKey_BaseLayerFrameRateFraction, CFStringRef)
//#define kVTCompressionPropertyKey_ProfileLevel get_VideoToolbox_kVTCompressionPropertyKey_BaseLayerFrameRateFraction()
#define kVTCompressionPropertyKey_ProfileLevel_Available canLoad_VideoToolbox_kVTCompressionPropertyKey_BaseLayerFrameRateFraction()


#endif  // SDK_OBJC_FRAMEWORK_CLASSES_VIDEOTOOLBOX_HELPERS_H_
