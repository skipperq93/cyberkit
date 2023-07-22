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

#import "DOMXPathResultInternal.h"

#import "DOMInternal.h"
#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/JSExecState.h>
#import <CyberCore/Node.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/CyberCoreObjCExtras.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <CyberCore/XPathResult.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL reinterpret_cast<CyberCore::XPathResult*>(_internal)

@implementation DOMXPathResult

- (void)dealloc
{
    if (CyberCoreObjCScheduleDeallocateOnMainThread([DOMXPathResult class], self))
        return;

    if (_internal)
        IMPL->deref();
    [super dealloc];
}

- (unsigned short)resultType
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->resultType();
}

- (double)numberValue
{
    CyberCore::JSMainThreadNullState state;
    return raiseOnDOMError(IMPL->numberValue());
}

- (NSString *)stringValue
{
    CyberCore::JSMainThreadNullState state;
    return raiseOnDOMError(IMPL->stringValue());
}

- (BOOL)booleanValue
{
    CyberCore::JSMainThreadNullState state;
    return raiseOnDOMError(IMPL->booleanValue());
}

- (DOMNode *)singleNodeValue
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->singleNodeValue()));
}

- (BOOL)invalidIteratorState
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->invalidIteratorState();
}

- (unsigned)snapshotLength
{
    CyberCore::JSMainThreadNullState state;
    return raiseOnDOMError(IMPL->snapshotLength());
}

- (DOMNode *)iterateNext
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->iterateNext()));
}

- (DOMNode *)snapshotItem:(unsigned)index
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->snapshotItem(index)));
}

@end

CyberCore::XPathResult* core(DOMXPathResult *wrapper)
{
    return wrapper ? reinterpret_cast<CyberCore::XPathResult*>(wrapper->_internal) : nullptr;
}

DOMXPathResult *kit(CyberCore::XPathResult* value)
{
    CyberCoreThreadViolationCheckRoundOne();
    if (!value)
        return nil;
    if (DOMXPathResult *wrapper = getDOMWrapper(value))
        return [[wrapper retain] autorelease];
    DOMXPathResult *wrapper = [[DOMXPathResult alloc] _init];
    wrapper->_internal = reinterpret_cast<DOMObjectInternal*>(value);
    value->ref();
    addDOMWrapper(wrapper, value);
    return [wrapper autorelease];
}

#undef IMPL
