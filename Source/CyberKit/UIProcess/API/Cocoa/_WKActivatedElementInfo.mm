/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
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
#import "_WKActivatedElementInfoInternal.h"

#import "CocoaImage.h"
#import "ShareableBitmap.h"
#import <wtf/RetainPtr.h>

#if USE(APPKIT)
#import <AppKit/NSImage.h>
#else
#import <UIKit/UIImage.h>
#endif

@implementation _WKActivatedElementInfo  {
    RetainPtr<NSURL> _URL;
    RetainPtr<NSURL> _imageURL;
    RetainPtr<NSString> _title;
    CyberCore::IntPoint _interactionLocation;
    RetainPtr<NSString> _ID;
    RefPtr<CyberKit::ShareableBitmap> _image;
    RetainPtr<NSString> _imageMIMEType;
    RetainPtr<CocoaImage> _cocoaImage;
#if PLATFORM(IOS_FAMILY)
    RetainPtr<NSDictionary> _userInfo;
    BOOL _isAnimating;
    BOOL _canShowAnimationControls;
    Vector<CyberCore::ElementAnimationContext> _animationsUnderElement;
#endif
    BOOL _animatedImage;
    BOOL _isImage;
    BOOL _isUsingAlternateURLForImage;
}

#if PLATFORM(IOS_FAMILY)
+ (instancetype)activatedElementInfoWithInteractionInformationAtPosition:(const CyberKit::InteractionInformationAtPosition&)information userInfo:(NSDictionary *)userInfo
{
    return adoptNS([[self alloc] _initWithInteractionInformationAtPosition:information isUsingAlternateURLForImage:NO userInfo:userInfo]).autorelease();
}

- (instancetype)_initWithInteractionInformationAtPosition:(const CyberKit::InteractionInformationAtPosition&)information isUsingAlternateURLForImage:(BOOL)isUsingAlternateURLForImage userInfo:(NSDictionary *)userInfo
{
    if (!(self = [super init]))
        return nil;
    
    _URL = information.url;
    _imageURL = information.imageURL;
    _imageMIMEType = information.imageMIMEType;
    _interactionLocation = information.request.point;
    _title = information.title;
    _boundingRect = information.bounds;
    
    if (information.isAttachment)
        _type = _WKActivatedElementTypeAttachment;
    else if (information.isLink)
        _type = _WKActivatedElementTypeLink;
    else if (information.isImage)
        _type = _WKActivatedElementTypeImage;
    else
        _type = _WKActivatedElementTypeUnspecified;
    
    _image = information.image;
    _ID = information.idAttribute;
    _animatedImage = information.isAnimatedImage;
    _isAnimating = information.isAnimating;
    _canShowAnimationControls = information.canShowAnimationControls;
    _isImage = information.isImage;
    _isUsingAlternateURLForImage = isUsingAlternateURLForImage;
    _userInfo = userInfo;
#if ENABLE(ACCESSIBILITY_ANIMATION_CONTROL)
    _animationsUnderElement = information.animationsAtPoint;
#endif

    return self;
}

- (instancetype)_initWithType:(_WKActivatedElementType)type URL:(NSURL *)url information:(const CyberKit::InteractionInformationAtPosition&)information
{
    return [self _initWithType:type URL:url imageURL:information.imageURL information:information];
}

- (instancetype)_initWithType:(_WKActivatedElementType)type image:(CyberKit::ShareableBitmap*)image information:(const CyberKit::InteractionInformationAtPosition&)information
{
    return [self _initWithType:type URL:information.url imageURL:information.imageURL image:image userInfo:nil information:information];
}

- (instancetype)_initWithType:(_WKActivatedElementType)type URL:(NSURL *)url imageURL:(NSURL *)imageURL information:(const CyberKit::InteractionInformationAtPosition&)information
{
    return [self _initWithType:type URL:url imageURL:imageURL image:information.image.get() userInfo:nil information:information];
}

- (instancetype)_initWithType:(_WKActivatedElementType)type URL:(NSURL *)url image:(CyberKit::ShareableBitmap*)image information:(const CyberKit::InteractionInformationAtPosition&)information
{
    return [self _initWithType:type URL:url imageURL:information.imageURL image:image userInfo:nil information:information];
}

- (instancetype)_initWithType:(_WKActivatedElementType)type URL:(NSURL *)url imageURL:(NSURL *)imageURL userInfo:(NSDictionary *)userInfo information:(const CyberKit::InteractionInformationAtPosition&)information
{
    return [self _initWithType:type URL:url imageURL:imageURL image:information.image.get() userInfo:userInfo information:information];
}

- (instancetype)_initWithType:(_WKActivatedElementType)type URL:(NSURL *)url imageURL:(NSURL *)imageURL image:(CyberKit::ShareableBitmap*)image userInfo:(NSDictionary *)userInfo information:(const CyberKit::InteractionInformationAtPosition&)information
{
#if ENABLE(ACCESSIBILITY_ANIMATION_CONTROL)
    auto animationsAtPoint = information.animationsAtPoint;
#else
    Vector<CyberCore::ElementAnimationContext> animationsAtPoint;
#endif

    return [self _initWithType:type URL:url imageURL:imageURL location:information.request.point title:information.title ID:information.idAttribute rect:information.bounds image:image imageMIMEType:information.imageMIMEType isAnimatedImage:information.isAnimatedImage isAnimating:information.isAnimating canShowAnimationControls:information.canShowAnimationControls animationsUnderElement:animationsAtPoint userInfo:userInfo];
}
#endif // PLATFORM(IOS_FAMILY)

- (instancetype)_initWithType:(_WKActivatedElementType)type URL:(NSURL *)url imageURL:(NSURL *)imageURL location:(const CyberCore::IntPoint&)location title:(NSString *)title ID:(NSString *)ID rect:(CGRect)rect image:(CyberKit::ShareableBitmap*)image imageMIMEType:(NSString *)imageMIMEType isAnimatedImage:(BOOL)isAnimatedImage isAnimating:(BOOL)isAnimating canShowAnimationControls:(BOOL)canShowAnimationControls animationsUnderElement:(Vector<CyberCore::ElementAnimationContext>)animationsUnderElement userInfo:(NSDictionary *)userInfo
{
    if (!(self = [super init]))
        return nil;

    _URL = adoptNS([url copy]);
    _imageURL = adoptNS([imageURL copy]);
    _imageMIMEType = adoptNS(imageMIMEType.copy);
    _interactionLocation = location;
    _title = adoptNS([title copy]);
    _boundingRect = rect;
    _type = type;
    _image = image;
    _ID = ID;
#if PLATFORM(IOS_FAMILY)
    _userInfo = adoptNS([userInfo copy]);
    _isAnimating = isAnimating;
    _canShowAnimationControls = canShowAnimationControls;
    _animationsUnderElement = animationsUnderElement;
#endif
    _animatedImage = isAnimatedImage;

    return self;
}

- (NSURL *)URL
{
    return _URL.get();
}

- (NSURL *)imageURL
{
    return _imageURL.get();
}

- (NSString *)title
{
    return _title.get();
}

- (NSString *)imageMIMEType
{
    return _imageMIMEType.get();
}

- (NSString *)ID
{
    return _ID.get();
}

- (CyberCore::IntPoint)_interactionLocation
{
    return _interactionLocation;
}

- (BOOL)isAnimatedImage
{
    return _animatedImage;
}

- (BOOL)_isUsingAlternateURLForImage
{
    return _isUsingAlternateURLForImage;
}

#if PLATFORM(IOS_FAMILY)
- (BOOL)isAnimating
{
    return _isAnimating;
}

- (BOOL)canShowAnimationControls
{
    return _canShowAnimationControls;
}

- (const Vector<CyberCore::ElementAnimationContext>&)_animationsUnderElement
{
    return _animationsUnderElement;
}

- (NSDictionary *)userInfo
{
    return _userInfo.get();
}
#endif

- (BOOL)_isImage
{
    return _isImage;
}

- (CocoaImage *)image
{
    if (_cocoaImage)
        return adoptNS([_cocoaImage copy]).autorelease();

    if (!_image)
        return nil;

#if USE(APPKIT)
    _cocoaImage = adoptNS([[NSImage alloc] initWithCGImage:_image->makeCGImageCopy().get() size:NSSizeFromCGSize(_boundingRect.size)]);
#else
    _cocoaImage = adoptNS([[UIImage alloc] initWithCGImage:_image->makeCGImageCopy().get()]);
#endif
    _image = nullptr;

    return adoptNS([_cocoaImage copy]).autorelease();
}

@end
