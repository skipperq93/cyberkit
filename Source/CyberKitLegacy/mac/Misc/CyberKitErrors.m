/*
 * Copyright (C) 2005-2016 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import <CyberKitLegacy/CyberKitErrors.h>

#import "WebLocalizableStringsInternal.h"
#import <Foundation/NSURLError.h>
#import <CyberKitLegacy/CyberKitErrorsPrivate.h>
#import <CyberKitLegacy/WebNSURLExtras.h>

#import <dispatch/dispatch.h>

NSString *CyberKitErrorDomain = @"CyberKitErrorDomain";

NSString * const CyberKitErrorMIMETypeKey =               @"CyberKitErrorMIMETypeKey";
NSString * const CyberKitErrorPlugInNameKey =             @"CyberKitErrorPlugInNameKey";
NSString * const CyberKitErrorPlugInPageURLStringKey =    @"CyberKitErrorPlugInPageURLStringKey";

// Policy errors
#define CyberKitErrorDescriptionCannotShowMIMEType UI_STRING_INTERNAL("Content with specified MIME type can’t be shown", "CyberKitErrorCannotShowMIMEType description")
#define CyberKitErrorDescriptionCannotShowURL UI_STRING_INTERNAL("The URL can’t be shown", "CyberKitErrorCannotShowURL description")
#define CyberKitErrorDescriptionFrameLoadInterruptedByPolicyChange UI_STRING_INTERNAL("Frame load interrupted", "CyberKitErrorFrameLoadInterruptedByPolicyChange description")
#define CyberKitErrorDescriptionCannotUseRestrictedPort UI_STRING_INTERNAL("Not allowed to use restricted network port", "CyberKitErrorCannotUseRestrictedPort description")
#define CyberKitErrorDescriptionFrameLoadBlockedByContentFilter UI_STRING_INTERNAL("The URL was blocked by a content filter", "CyberKitErrorFrameLoadBlockedByContentFilter description")

// Plug-in and java errors
#define CyberKitErrorDescriptionCannotFindPlugin UI_STRING_INTERNAL("The plug-in can’t be found", "CyberKitErrorCannotFindPlugin description")
#define CyberKitErrorDescriptionCannotLoadPlugin UI_STRING_INTERNAL("The plug-in can’t be loaded", "CyberKitErrorCannotLoadPlugin description")
#define CyberKitErrorDescriptionJavaUnavailable UI_STRING_INTERNAL("Java is unavailable", "CyberKitErrorJavaUnavailable description")
#define CyberKitErrorDescriptionPlugInCancelledConnection UI_STRING_INTERNAL("Plug-in cancelled", "CyberKitErrorPlugInCancelledConnection description")
#define CyberKitErrorDescriptionPlugInWillHandleLoad UI_STRING_INTERNAL("Plug-in handled load", "CyberKitErrorPlugInWillHandleLoad description")

// Geolocations errors

#define CyberKitErrorDescriptionGeolocationLocationUnknown UI_STRING_INTERNAL("The current location cannot be found.", "CyberKitErrorGeolocationLocationUnknown description")

static NSMutableDictionary *descriptions = nil;

@interface NSError (CyberKitInternal)
- (instancetype)_webkit_initWithDomain:(NSString *)domain code:(int)code URL:(NSURL *)URL __attribute__((objc_method_family(init)));
@end

@implementation NSError (CyberKitInternal)

- (instancetype)_webkit_initWithDomain:(NSString *)domain code:(int)code URL:(NSURL *)URL
{
    // Insert a localized string here for those folks not savvy to our category methods.
    NSDictionary *descriptionsDict = [descriptions objectForKey:domain];
    NSString *localizedDescription = descriptionsDict ? [descriptionsDict objectForKey:[NSNumber numberWithInt:code]] : nil;
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
        URL, @"NSErrorFailingURLKey",
        [URL absoluteString], NSURLErrorFailingURLStringErrorKey,
        localizedDescription, NSLocalizedDescriptionKey,
        nil];
    return [self initWithDomain:domain code:code userInfo:dict];
}

@end

@implementation NSError (CyberKitExtras)

+ (void)_registerCyberKitErrors
{
    static dispatch_once_t flag;
    dispatch_once(&flag, ^{
        @autoreleasepool {
            NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
                // Policy errors
                CyberKitErrorDescriptionCannotShowMIMEType,                   [NSNumber numberWithInt: CyberKitErrorCannotShowMIMEType],
                CyberKitErrorDescriptionCannotShowURL,                        [NSNumber numberWithInt: CyberKitErrorCannotShowURL],
                CyberKitErrorDescriptionFrameLoadInterruptedByPolicyChange,   [NSNumber numberWithInt: CyberKitErrorFrameLoadInterruptedByPolicyChange],
                CyberKitErrorDescriptionCannotUseRestrictedPort,              [NSNumber numberWithInt: CyberKitErrorCannotUseRestrictedPort],
                CyberKitErrorDescriptionFrameLoadBlockedByContentFilter,      [NSNumber numberWithInt: CyberKitErrorFrameLoadBlockedByContentFilter],

                // Plug-in and java errors
                CyberKitErrorDescriptionCannotFindPlugin,                     [NSNumber numberWithInt: CyberKitErrorCannotFindPlugIn],
                CyberKitErrorDescriptionCannotLoadPlugin,                     [NSNumber numberWithInt: CyberKitErrorCannotLoadPlugIn],
                CyberKitErrorDescriptionJavaUnavailable,                      [NSNumber numberWithInt: CyberKitErrorJavaUnavailable],
                CyberKitErrorDescriptionPlugInCancelledConnection,            [NSNumber numberWithInt: CyberKitErrorPlugInCancelledConnection],
                CyberKitErrorDescriptionPlugInWillHandleLoad,                 [NSNumber numberWithInt: CyberKitErrorPlugInWillHandleLoad],

                // Geolocation errors
                CyberKitErrorDescriptionGeolocationLocationUnknown,           [NSNumber numberWithInt: CyberKitErrorGeolocationLocationUnknown],
                nil];

            [NSError _webkit_addErrorsWithCodesAndDescriptions:dict inDomain:CyberKitErrorDomain];
        }
    });
}

+(id)_webkit_errorWithDomain:(NSString *)domain code:(int)code URL:(NSURL *)URL
{
    return [[[self alloc] _webkit_initWithDomain:domain code:code URL:URL] autorelease];
}

+ (NSError *)_webKitErrorWithDomain:(NSString *)domain code:(int)code URL:(NSURL *)URL
{
    [self _registerCyberKitErrors];
    return [self _webkit_errorWithDomain:domain code:code URL:URL];
}

+ (NSError *)_webKitErrorWithCode:(int)code failingURL:(NSString *)URLString
{
    return [self _webKitErrorWithDomain:CyberKitErrorDomain code:code URL:[NSURL _webkit_URLWithUserTypedString:URLString]];
}

- (id)_initWithPluginErrorCode:(int)code
                    contentURL:(NSURL *)contentURL
                 pluginPageURL:(NSURL *)pluginPageURL
                    pluginName:(NSString *)pluginName
                      MIMEType:(NSString *)MIMEType
{
    [[self class] _registerCyberKitErrors];
    
    NSMutableDictionary *userInfo = [[NSMutableDictionary alloc] init];
    NSDictionary *descriptionsForCyberKitErrorDomain = [descriptions objectForKey:CyberKitErrorDomain];
    NSString *localizedDescription = [descriptionsForCyberKitErrorDomain objectForKey:[NSNumber numberWithInt:code]];
    if (localizedDescription)
        [userInfo setObject:localizedDescription forKey:NSLocalizedDescriptionKey];
    if (contentURL) {
        [userInfo setObject:contentURL forKey:@"NSErrorFailingURLKey"];
        [userInfo setObject:[contentURL _web_userVisibleString] forKey:NSURLErrorFailingURLStringErrorKey];
    }
    if (pluginPageURL) {
        [userInfo setObject:[pluginPageURL _web_userVisibleString] forKey:CyberKitErrorPlugInPageURLStringKey];
    }
    if (pluginName) {
        [userInfo setObject:pluginName forKey:CyberKitErrorPlugInNameKey];
    }
    if (MIMEType) {
        [userInfo setObject:MIMEType forKey:CyberKitErrorMIMETypeKey];
    }

    NSDictionary *userInfoCopy = [userInfo count] > 0 ? [[NSDictionary alloc] initWithDictionary:userInfo] : nil;
    [userInfo release];
    NSError *error = [self initWithDomain:CyberKitErrorDomain code:code userInfo:userInfoCopy];
    [userInfoCopy release];
    
    return error;
}

+ (void)_webkit_addErrorsWithCodesAndDescriptions:(NSDictionary *)dictionary inDomain:(NSString *)domain
{
    if (!descriptions)
        descriptions = [[NSMutableDictionary alloc] init];

    [descriptions setObject:dictionary forKey:domain];
}

@end
