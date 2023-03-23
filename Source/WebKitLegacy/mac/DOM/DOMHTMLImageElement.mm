/*
 * Copyright (C) 2004-2016 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "DOMHTMLImageElementInternal.h"

#import "DOMNodeInternal.h"
#import "DOMPrivate.h"
#import "ExceptionHandlers.h"
#import <CyberCore/HTMLImageElement.h>
#import <CyberCore/HTMLNames.h>
#import <CyberCore/HitTestResult.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLImageElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLImageElement

- (NSString *)name
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getNameAttribute();
}

- (void)setName:(NSString *)newName
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::nameAttr, newName);
}

- (NSString *)align
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::alignAttr);
}

- (void)setAlign:(NSString *)newAlign
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr, newAlign);
}

- (NSString *)alt
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::altAttr);
}

- (void)setAlt:(NSString *)newAlt
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::altAttr, newAlt);
}

- (NSString *)border
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::borderAttr);
}

- (void)setBorder:(NSString *)newBorder
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::borderAttr, newBorder);
}

- (NSString *)crossOrigin
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->crossOrigin();
}

- (void)setCrossOrigin:(NSString *)newCrossOrigin
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setCrossOrigin(newCrossOrigin);
}

- (int)height
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->height();
}

- (void)setHeight:(int)newHeight
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setHeight(newHeight);
}

- (int)hspace
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getIntegralAttribute(CyberCore::HTMLNames::hspaceAttr);
}

- (void)setHspace:(int)newHspace
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setIntegralAttribute(CyberCore::HTMLNames::hspaceAttr, newHspace);
}

- (BOOL)isMap
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::ismapAttr);
}

- (void)setIsMap:(BOOL)newIsMap
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::ismapAttr, newIsMap);
}

- (NSString *)longDesc
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getURLAttribute(CyberCore::HTMLNames::longdescAttr).string();
}

- (void)setLongDesc:(NSString *)newLongDesc
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::longdescAttr, newLongDesc);
}

- (NSString *)src
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getURLAttribute(CyberCore::HTMLNames::srcAttr).string();
}

- (void)setSrc:(NSString *)newSrc
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::srcAttr, newSrc);
}

- (NSString *)srcset
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::srcsetAttr);
}

- (void)setSrcset:(NSString *)newSrcset
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::srcsetAttr, newSrcset);
}

- (NSString *)sizes
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::sizesAttr);
}

- (void)setSizes:(NSString *)newSizes
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::sizesAttr, newSizes);
}

- (NSString *)currentSrc
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->currentSrc().string();
}

- (NSString *)useMap
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::usemapAttr);
}

- (void)setUseMap:(NSString *)newUseMap
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::usemapAttr, newUseMap);
}

- (int)vspace
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getIntegralAttribute(CyberCore::HTMLNames::vspaceAttr);
}

- (void)setVspace:(int)newVspace
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setIntegralAttribute(CyberCore::HTMLNames::vspaceAttr, newVspace);
}

- (int)width
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->width();
}

- (void)setWidth:(int)newWidth
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setWidth(newWidth);
}

- (BOOL)complete
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->complete();
}

- (NSString *)lowsrc
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getURLAttribute(CyberCore::HTMLNames::lowsrcAttr).string();
}

- (void)setLowsrc:(NSString *)newLowsrc
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::lowsrcAttr, newLowsrc);
}

- (int)naturalHeight
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->naturalHeight();
}

- (int)naturalWidth
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->naturalWidth();
}

- (int)x
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->x();
}

- (int)y
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->y();
}

- (NSString *)altDisplayString
{
    CyberCore::JSMainThreadNullState state;
    return CyberCore::displayString(IMPL->alt(), core(self));
}

- (NSURL *)absoluteImageURL
{
    CyberCore::JSMainThreadNullState state;
    return [self _getURLAttribute:@"src"];
}

@end

CyberCore::HTMLImageElement* core(DOMHTMLImageElement *wrapper)
{
    return wrapper ? reinterpret_cast<CyberCore::HTMLImageElement*>(wrapper->_internal) : 0;
}

#undef IMPL
