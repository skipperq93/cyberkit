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

#import "DOMHTMLIFrameElement.h"

#import "DOMAbstractViewInternal.h"
#import "DOMDocumentInternal.h"
#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/Document.h>
#import <CyberCore/HTMLIFrameElement.h>
#import <CyberCore/HTMLNames.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/LocalDOMWindow.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLIFrameElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLIFrameElement

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

- (NSString *)frameBorder
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::frameborderAttr);
}

- (void)setFrameBorder:(NSString *)newFrameBorder
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::frameborderAttr, newFrameBorder);
}

- (NSString *)height
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::heightAttr);
}

- (void)setHeight:(NSString *)newHeight
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::heightAttr, newHeight);
}

- (NSString *)longDesc
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::longdescAttr);
}

- (void)setLongDesc:(NSString *)newLongDesc
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::longdescAttr, newLongDesc);
}

- (NSString *)marginHeight
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::marginheightAttr);
}

- (void)setMarginHeight:(NSString *)newMarginHeight
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::marginheightAttr, newMarginHeight);
}

- (NSString *)marginWidth
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::marginwidthAttr);
}

- (void)setMarginWidth:(NSString *)newMarginWidth
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::marginwidthAttr, newMarginWidth);
}

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

- (NSString *)sandbox
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::sandboxAttr);
}

- (void)setSandbox:(NSString *)newSandbox
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::sandboxAttr, newSandbox);
}

- (NSString *)scrolling
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::scrollingAttr);
}

- (void)setScrolling:(NSString *)newScrolling
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::scrollingAttr, newScrolling);
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

- (NSString *)srcdoc
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::srcdocAttr);
}

- (void)setSrcdoc:(NSString *)newSrcdoc
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::srcdocAttr, newSrcdoc);
}

- (NSString *)width
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::widthAttr);
}

- (void)setWidth:(NSString *)newWidth
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::widthAttr, newWidth);
}

- (DOMDocument *)contentDocument
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->contentDocument()));
}

- (DOMAbstractView *)contentWindow
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->contentWindow()));
}

@end

#undef IMPL
