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

#import "DOMRangeInternal.h"

#import "DOMDocumentFragmentInternal.h"
#import "DOMInternal.h"
#import "DOMNodeInternal.h"
#import <CyberCore/DocumentFragment.h>
#import "ExceptionHandlers.h"
#import <CyberCore/JSExecState.h>
#import <CyberCore/Range.h>
#import <CyberCore/SimpleRange.h>
#import <CyberCore/TextIterator.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/CyberCoreObjCExtras.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL reinterpret_cast<CyberCore::Range*>(_internal)

@implementation DOMRange

- (void)dealloc
{
    if (CyberCoreObjCScheduleDeallocateOnMainThread([DOMRange class], self))
        return;

    if (_internal)
        IMPL->deref();
    [super dealloc];
}

- (DOMNode *)startContainer
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->startContainer()));
}

- (int)startOffset
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->startOffset();
}

- (DOMNode *)endContainer
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->endContainer()));
}

- (int)endOffset
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->endOffset();
}

- (BOOL)collapsed
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->collapsed();
}

- (DOMNode *)commonAncestorContainer
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->commonAncestorContainer()));
}

- (NSString *)text
{
    CyberCore::JSMainThreadNullState state;
    auto range = makeSimpleRange(*IMPL);
    range.start.document().updateLayout();
    return plainText(range);
}

- (void)setStart:(DOMNode *)refNode offset:(int)offset
{
    CyberCore::JSMainThreadNullState state;
    if (!refNode)
        raiseTypeErrorException();
    raiseOnDOMError(IMPL->setStart(*core(refNode), offset));
}

- (void)setEnd:(DOMNode *)refNode offset:(int)offset
{
    CyberCore::JSMainThreadNullState state;
    if (!refNode)
        raiseTypeErrorException();
    raiseOnDOMError(IMPL->setEnd(*core(refNode), offset));
}

- (void)setStartBefore:(DOMNode *)refNode
{
    CyberCore::JSMainThreadNullState state;
    if (!refNode)
        raiseTypeErrorException();
    raiseOnDOMError(IMPL->setStartBefore(*core(refNode)));
}

- (void)setStartAfter:(DOMNode *)refNode
{
    CyberCore::JSMainThreadNullState state;
    if (!refNode)
        raiseTypeErrorException();
    raiseOnDOMError(IMPL->setStartAfter(*core(refNode)));
}

- (void)setEndBefore:(DOMNode *)refNode
{
    CyberCore::JSMainThreadNullState state;
    if (!refNode)
        raiseTypeErrorException();
    raiseOnDOMError(IMPL->setEndBefore(*core(refNode)));
}

- (void)setEndAfter:(DOMNode *)refNode
{
    CyberCore::JSMainThreadNullState state;
    if (!refNode)
        raiseTypeErrorException();
    raiseOnDOMError(IMPL->setEndAfter(*core(refNode)));
}

- (void)collapse:(BOOL)toStart
{
    CyberCore::JSMainThreadNullState state;
    IMPL->collapse(toStart);
}

- (void)selectNode:(DOMNode *)refNode
{
    CyberCore::JSMainThreadNullState state;
    if (!refNode)
        raiseTypeErrorException();
    raiseOnDOMError(IMPL->selectNode(*core(refNode)));
}

- (void)selectNodeContents:(DOMNode *)refNode
{
    CyberCore::JSMainThreadNullState state;
    if (!refNode)
        raiseTypeErrorException();
    raiseOnDOMError(IMPL->selectNodeContents(*core(refNode)));
}

- (short)compareBoundaryPoints:(unsigned short)how sourceRange:(DOMRange *)sourceRange
{
    CyberCore::JSMainThreadNullState state;
    if (!sourceRange)
        raiseTypeErrorException();
    return raiseOnDOMError(IMPL->compareBoundaryPoints(how, *core(sourceRange)));
}

- (void)deleteContents
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->deleteContents());
}

- (DOMDocumentFragment *)extractContents
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->extractContents()).ptr());
}

- (DOMDocumentFragment *)cloneContents
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->cloneContents()).ptr());
}

- (void)insertNode:(DOMNode *)newNode
{
    CyberCore::JSMainThreadNullState state;
    if (!newNode)
        raiseTypeErrorException();
    raiseOnDOMError(IMPL->insertNode(*core(newNode)));
}

- (void)surroundContents:(DOMNode *)newParent
{
    CyberCore::JSMainThreadNullState state;
    if (!newParent)
        raiseTypeErrorException();
    raiseOnDOMError(IMPL->surroundContents(*core(newParent)));
}

- (DOMRange *)cloneRange
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->cloneRange()));
}

- (NSString *)toString
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->toString();
}

- (void)detach
{
    CyberCore::JSMainThreadNullState state;
    IMPL->detach();
}

- (DOMDocumentFragment *)createContextualFragment:(NSString *)html
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->createContextualFragment(html)).ptr());
}

- (short)compareNode:(DOMNode *)refNode
{
    CyberCore::JSMainThreadNullState state;
    if (!refNode)
        raiseTypeErrorException();
    return raiseOnDOMError(IMPL->compareNode(*core(refNode)));
}

- (BOOL)intersectsNode:(DOMNode *)refNode
{
    CyberCore::JSMainThreadNullState state;
    if (!refNode)
        raiseTypeErrorException();
    return IMPL->intersectsNode(*core(refNode));
}

- (short)comparePoint:(DOMNode *)refNode offset:(int)offset
{
    CyberCore::JSMainThreadNullState state;
    if (!refNode)
        raiseTypeErrorException();
    return raiseOnDOMError(IMPL->comparePoint(*core(refNode), offset));
}

- (BOOL)isPointInRange:(DOMNode *)refNode offset:(int)offset
{
    CyberCore::JSMainThreadNullState state;
    if (!refNode)
        raiseTypeErrorException();
    return raiseOnDOMError(IMPL->isPointInRange(*core(refNode), offset));
}

- (void)expand:(NSString *)unit
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->expand(unit));
}

@end

@implementation DOMRange (DOMRangeDeprecated)

- (void)setStart:(DOMNode *)refNode :(int)offset
{
    [self setStart:refNode offset:offset];
}

- (void)setEnd:(DOMNode *)refNode :(int)offset
{
    [self setEnd:refNode offset:offset];
}

- (short)compareBoundaryPoints:(unsigned short)how :(DOMRange *)sourceRange
{
    return [self compareBoundaryPoints:how sourceRange:sourceRange];
}

@end

CyberCore::Range* core(DOMRange *wrapper)
{
    return wrapper ? reinterpret_cast<CyberCore::Range*>(wrapper->_internal) : 0;
}

DOMRange *kit(CyberCore::Range* value)
{
    CyberCoreThreadViolationCheckRoundOne();
    if (!value)
        return nil;
    if (DOMRange *wrapper = getDOMWrapper(value))
        return retainPtr(wrapper).autorelease();
    auto wrapper = adoptNS([[DOMRange alloc] _init]);
    wrapper->_internal = reinterpret_cast<DOMObjectInternal*>(value);
    value->ref();
    addDOMWrapper(wrapper.get(), value);
    return wrapper.autorelease();
}

DOMRange *kit(const std::optional<CyberCore::SimpleRange>& value)
{
    return kit(createLiveRange(value).get());
}

#undef IMPL
