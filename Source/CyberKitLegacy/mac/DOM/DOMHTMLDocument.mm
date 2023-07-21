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

#import "DOMHTMLDocumentInternal.h"

#import "DOMHTMLCollectionInternal.h"
#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/HTMLCollection.h>
#import <CyberCore/HTMLDocument.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLDocument*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLDocument

- (DOMHTMLCollection *)embeds
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->embeds()));
}

- (DOMHTMLCollection *)plugins
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->plugins()));
}

- (DOMHTMLCollection *)scripts
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->scripts()));
}

- (int)width
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->width();
}

- (int)height
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->height();
}

- (NSString *)dir
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->dir();
}

- (void)setDir:(NSString *)newDir
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setDir(newDir);
}

- (NSString *)designMode
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->designMode();
}

- (void)setDesignMode:(NSString *)newDesignMode
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setDesignMode(newDesignMode);
}

- (NSString *)compatMode
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->compatMode();
}

- (NSString *)bgColor
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->bgColor();
}

- (void)setBgColor:(NSString *)newBgColor
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBgColor(newBgColor);
}

- (NSString *)fgColor
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->fgColor();
}

- (void)setFgColor:(NSString *)newFgColor
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setFgColor(newFgColor);
}

- (NSString *)alinkColor
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->alinkColor();
}

- (void)setAlinkColor:(NSString *)newAlinkColor
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAlinkColor(newAlinkColor);
}

- (NSString *)linkColor
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->linkColorForBindings();
}

- (void)setLinkColor:(NSString *)newLinkColor
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setLinkColorForBindings(newLinkColor);
}

- (NSString *)vlinkColor
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->vlinkColor();
}

- (void)setVlinkColor:(NSString *)newVlinkColor
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setVlinkColor(newVlinkColor);
}

- (void)open
{
    CyberCore::JSMainThreadNullState state;
    IMPL->open();
}

- (void)close
{
    CyberCore::JSMainThreadNullState state;
    IMPL->close();
}

- (void)write:(NSString *)text
{
    CyberCore::JSMainThreadNullState state;
    IMPL->write(nullptr, { String { text } });
}

- (void)writeln:(NSString *)text
{
    CyberCore::JSMainThreadNullState state;
    IMPL->writeln(nullptr, { String { text} });
}

- (void)clear
{
    CyberCore::JSMainThreadNullState state;
    IMPL->clear();
}

- (void)captureEvents
{
    CyberCore::JSMainThreadNullState state;
    IMPL->captureEvents();
}

- (void)releaseEvents
{
    CyberCore::JSMainThreadNullState state;
    IMPL->releaseEvents();
}

@end

CyberCore::HTMLDocument* core(DOMHTMLDocument *wrapper)
{
    return wrapper ? reinterpret_cast<CyberCore::HTMLDocument*>(wrapper->_internal) : 0;
}

DOMHTMLDocument *kit(CyberCore::HTMLDocument* value)
{
    CyberCoreThreadViolationCheckRoundOne();
    return static_cast<DOMHTMLDocument*>(kit(static_cast<CyberCore::Node*>(value)));
}
