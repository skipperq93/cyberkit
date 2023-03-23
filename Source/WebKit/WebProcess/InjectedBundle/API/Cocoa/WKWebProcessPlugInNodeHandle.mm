/*
 * Copyright (C) 2013-2022 Apple Inc. All rights reserved.
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
#import "WKWebProcessPlugInNodeHandleInternal.h"

#import "CocoaImage.h"
#import "WKSharedAPICast.h"
#import "WKWebProcessPlugInFrameInternal.h"
#import "WebImage.h"
#import <CyberCore/HTMLTextFormControlElement.h>
#import <CyberCore/IntRect.h>
#import <CyberCore/NativeImage.h>
#import <CyberCore/CyberCoreObjCExtras.h>

@implementation WKWebProcessPlugInNodeHandle {
    API::ObjectStorage<CyberKit::InjectedBundleNodeHandle> _nodeHandle;
}

- (void)dealloc
{
    if (CyberCoreObjCScheduleDeallocateOnMainRunLoop(WKWebProcessPlugInNodeHandle.class, self))
        return;
    _nodeHandle->~InjectedBundleNodeHandle();
    [super dealloc];
}

+ (WKWebProcessPlugInNodeHandle *)nodeHandleWithJSValue:(JSValue *)value inContext:(JSContext *)context
{
    JSContextRef contextRef = [context JSGlobalContextRef];
    JSObjectRef objectRef = JSValueToObject(contextRef, [value JSValueRef], nullptr);
    return CyberKit::wrapper(CyberKit::InjectedBundleNodeHandle::getOrCreate(contextRef, objectRef));
}

- (WKWebProcessPlugInFrame *)htmlIFrameElementContentFrame
{
    return CyberKit::wrapper(_nodeHandle->htmlIFrameElementContentFrame());
}

- (CocoaImage *)renderedImageWithOptions:(WKSnapshotOptions)options
{
    return [self renderedImageWithOptions:options width:nil];
}

- (CocoaImage *)renderedImageWithOptions:(WKSnapshotOptions)options width:(NSNumber *)width
{
    std::optional<float> optionalWidth;
    if (width)
        optionalWidth = width.floatValue;

    auto image = _nodeHandle->renderedImage(CyberKit::toSnapshotOptions(options), options & kWKSnapshotOptionsExcludeOverflow, optionalWidth);
    if (!image)
        return nil;

    auto nativeImage = image->copyNativeImage(CyberCore::DontCopyBackingStore);
    if (!nativeImage)
        return nil;

#if USE(APPKIT)
    return adoptNS([[NSImage alloc] initWithCGImage:nativeImage->platformImage().get() size:NSZeroSize]).autorelease();
#else
    return adoptNS([[UIImage alloc] initWithCGImage:nativeImage->platformImage().get()]).autorelease();
#endif
}

- (CGRect)elementBounds
{
    return _nodeHandle->elementBounds();
}

- (BOOL)HTMLInputElementIsAutoFilled
{
    return _nodeHandle->isHTMLInputElementAutoFilled();
}

- (BOOL)HTMLInputElementIsAutoFilledAndViewable
{
    return _nodeHandle->isHTMLInputElementAutoFilledAndViewable();
}

- (BOOL)HTMLInputElementIsAutoFilledAndObscured
{
    return _nodeHandle->isHTMLInputElementAutoFilledAndObscured();
}

- (void)setHTMLInputElementIsAutoFilled:(BOOL)isAutoFilled
{
    _nodeHandle->setHTMLInputElementAutoFilled(isAutoFilled);
}

- (void)setHTMLInputElementIsAutoFilledAndViewable:(BOOL)isAutoFilledAndViewable
{
    _nodeHandle->setHTMLInputElementAutoFilledAndViewable(isAutoFilledAndViewable);
}

- (void)setHTMLInputElementIsAutoFilledAndObscured:(BOOL)isAutoFilledAndObscured
{
    _nodeHandle->setHTMLInputElementAutoFilledAndObscured(isAutoFilledAndObscured);
}

- (BOOL)isHTMLInputElementAutoFillButtonEnabled
{
    return _nodeHandle->isHTMLInputElementAutoFillButtonEnabled();
}

static CyberCore::AutoFillButtonType toAutoFillButtonType(_WKAutoFillButtonType autoFillButtonType)
{
    switch (autoFillButtonType) {
    case _WKAutoFillButtonTypeNone:
        return CyberCore::AutoFillButtonType::None;
    case _WKAutoFillButtonTypeContacts:
        return CyberCore::AutoFillButtonType::Contacts;
    case _WKAutoFillButtonTypeCredentials:
        return CyberCore::AutoFillButtonType::Credentials;
    case _WKAutoFillButtonTypeStrongPassword:
        return CyberCore::AutoFillButtonType::StrongPassword;
    case _WKAutoFillButtonTypeCreditCard:
        return CyberCore::AutoFillButtonType::CreditCard;
    case _WKAutoFillButtonTypeLoading:
        return CyberCore::AutoFillButtonType::Loading;
    }
    ASSERT_NOT_REACHED();
    return CyberCore::AutoFillButtonType::None;
}

static _WKAutoFillButtonType toWKAutoFillButtonType(CyberCore::AutoFillButtonType autoFillButtonType)
{
    switch (autoFillButtonType) {
    case CyberCore::AutoFillButtonType::None:
        return _WKAutoFillButtonTypeNone;
    case CyberCore::AutoFillButtonType::Contacts:
        return _WKAutoFillButtonTypeContacts;
    case CyberCore::AutoFillButtonType::Credentials:
        return _WKAutoFillButtonTypeCredentials;
    case CyberCore::AutoFillButtonType::StrongPassword:
        return _WKAutoFillButtonTypeStrongPassword;
    case CyberCore::AutoFillButtonType::CreditCard:
        return _WKAutoFillButtonTypeCreditCard;
    case CyberCore::AutoFillButtonType::Loading:
        return _WKAutoFillButtonTypeLoading;
    }
    ASSERT_NOT_REACHED();
    return _WKAutoFillButtonTypeNone;

}

- (void)setHTMLInputElementAutoFillButtonEnabledWithButtonType:(_WKAutoFillButtonType)autoFillButtonType
{
    _nodeHandle->setHTMLInputElementAutoFillButtonEnabled(toAutoFillButtonType(autoFillButtonType));
}

- (_WKAutoFillButtonType)htmlInputElementAutoFillButtonType
{
    return toWKAutoFillButtonType(_nodeHandle->htmlInputElementAutoFillButtonType());
}

- (_WKAutoFillButtonType)htmlInputElementLastAutoFillButtonType
{
    return toWKAutoFillButtonType(_nodeHandle->htmlInputElementLastAutoFillButtonType());
}

- (BOOL)HTMLInputElementIsUserEdited
{
    return _nodeHandle->htmlInputElementLastChangeWasUserEdit();
}

- (BOOL)HTMLTextAreaElementIsUserEdited
{
    return _nodeHandle->htmlTextAreaElementLastChangeWasUserEdit();
}

- (BOOL)isSelectElement
{
    return _nodeHandle->isSelectElement();
}

- (BOOL)isSelectableTextNode
{
    return _nodeHandle->isSelectableTextNode();
}

- (BOOL)isTextField
{
    return _nodeHandle->isTextField();
}

- (WKWebProcessPlugInNodeHandle *)HTMLTableCellElementCellAbove
{
    return CyberKit::wrapper(_nodeHandle->htmlTableCellElementCellAbove());
}

- (WKWebProcessPlugInFrame *)frame
{
    return CyberKit::wrapper(_nodeHandle->document()->documentFrame());
}

- (CyberKit::InjectedBundleNodeHandle&)_nodeHandle
{
    return *_nodeHandle;
}

#pragma mark WKObject protocol implementation

- (API::Object&)_apiObject
{
    return *_nodeHandle;
}

@end
